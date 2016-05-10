// Server_peer-to-peer.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>
#include <DownloadingFile.h>
#include <DistributeFile.h>
#include <vector>
#include <thread>
#include <mutex>
#include "Mutex_Guard.h"
#include <map>
#include <algorithm>
#include "MyFile.h"
#define SIZE_FOR_COUNT 4

std::mutex mt;

std::vector<MyFile> vFilInf;


using boost::asio::ip::tcp;

bool Test(const DistributeFile & ds, std::vector<MyFile> & vFilInf);

class session
	: public std::enable_shared_from_this<session>
{
public:
	session(tcp::socket socket)
		: socket_(std::move(socket))
	{	}

	void start()
	{
		do_write(max_length);
	}

private:
	//чтение
	void do_read()
	{
		std::lock_guard<std::mutex> lock(mt);

		std::cout << "do Read" << std::endl;
		size_count_read = 0;
		// в size_count запишится сколько чайстей читать
		char * buf = (char *)&size_count_read;
		auto self(shared_from_this());
		size_t sizeRecv;
			// в форе считываем нужное количество раз
		std::cout << "do Read for" << std::endl;
			for (size_t i = 0; i < size_count_read +1; i++)
			{
				if (i==0)
				{
					buf = (char *)&size_count_read;
					sizeRecv = sizeof(int);
				}
				{
					socket_.async_read_some(boost::asio::buffer(buf, sizeRecv),
						[this, self](boost::system::error_code ec, std::size_t length)
					{});
				}
				if (i==0)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(70));
				}
				if (i!=0)
				{
					boost::asio::ip::tcp::endpoint remote_ep = socket_.remote_endpoint();
					mf_Dst.dst.m_fileInfo = f;
					mf_Dst.dst.m_address = remote_ep.address();
					if (Test(mf_Dst.dst,vFilInf))
					{
						std::cout << "Read <- FILE HAS!!! NOT ADDED!!!" <<  std::endl;
						continue;
					}
					mf_Dst.dwStart = GetTickCount();
					vFilInf.push_back(mf_Dst);
					// выводит что считали + IP
					std::cout << "Read <- " << mf_Dst.dst.m_fileInfo.m_fileName << std::endl;
					std::cout << "Read <- " << mf_Dst.dst.m_fileInfo.m_fileDescription << std::endl;
					std::cout << "Read <- IP - " << mf_Dst.dst.m_address << std::endl;
					std::cout << "Read <- TIME - " << mf_Dst.dwStart << std::endl;
					std::cout << "Read size Vec - " << vFilInf.size() << std::endl << std::endl;
				}

				buf = (char*)&f;
				sizeRecv = sizeof(FileInfo);

			}
			std::cout << "|---------------------------------------------------------------|"<<std::endl;
	}

	void do_write(std::size_t length)
	{
		
		//отправляем
		std::cout << "do_write" << std::endl;
		auto self(shared_from_this());
		
		//сколько раз будем отправлять
		size_count_do_write = vFilInf.size();
		char * buf = (char *)&size_count_do_write;
		std::cout << "Send Num size - " << size_count_do_write << std::endl;
		size_t sizeSend;
		// в форе отправляем нужное кол раз
		for (size_t i = 0; i < vFilInf.size()+1; i++)
		{
			if (i == 0)
			{
				buf = (char *)&size_count_do_write;
				sizeSend = sizeof(size_count_do_write);
			}
			else
			{
				mf_Dst = vFilInf[i - 1];
				std::cout << "Send -> " << vFilInf[i - 1].dst.m_fileInfo.m_fileName << std::endl;
				std::cout << "Send -> " << vFilInf[i - 1].dst.m_fileInfo.m_fileDescription << std::endl;
				std::cout << "Send -> " << vFilInf[i - 1].dst.m_address << std::endl << std::endl;
				buf = (char *)&mf_Dst.dst;
				sizeSend = sizeof(DistributeFile);
			}

			std::lock_guard<std::mutex> lock(mt);

			boost::asio::async_write(socket_, boost::asio::buffer(buf, sizeSend),
				[this, self](boost::system::error_code ec, std::size_t )
			{});
			
		}
		
		do_read();
	
	}

	tcp::socket socket_;
	enum { max_length = 4368 /*6144*/ };
	char data_[max_length];
	FileInfo f;
	int size_count_read;
	int size_count_do_write;
	MyFile mf_Dst;
};

class server
{
public:
	server(boost::asio::io_service& io_service, short port)
		: acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
		socket_(io_service)
	{
		std::cout << "Start to server V1.2.1" << std::endl;
		do_accept();
	}

private:
	//ждем клиентов
	void do_accept()
	{
		acceptor_.async_accept(socket_,
			[this](boost::system::error_code ec)
		{
			if (!ec)
			{
				std::make_shared<session>(std::move(socket_))->start();
			}

			do_accept();
		});
	}

	tcp::acceptor acceptor_;
	tcp::socket socket_;
};

void Show();

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Russian");
	try
	{
		boost::asio::io_service io_service;
		std::thread out(Show);
		out.detach();
		server s(io_service, std::atoi("33333"));
		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
	return 0;
}

void Show()
{

	char a[10];
	memset(a, '\0', 10);
	std::vector<MyFile>::iterator pos;
	size_t tim = 0;
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(90000));
		std::lock_guard<std::mutex> lock(mt);
		pos = vFilInf.begin();
		if (vFilInf.size())
		{
			for (size_t i = 0; i < vFilInf.size(); i++)
			{
				std::cout << vFilInf[i].dst.m_fileInfo.m_fileName << "\t\t\t";
				std::cout << vFilInf[i].dst.m_address << std::endl;
				if ((tim = (GetTickCount()-vFilInf[i].dwStart))> 90000)
				{
					vFilInf.erase(pos+(i));
					pos = vFilInf.begin();
					i--;
				}
				std::cout << "time over - "<< tim << std::endl;
			}
		}
		else
		{
			std::cout << "File not found" <<std::endl;
			std::cout << "vec size = " << vFilInf.size()<< std::endl;
		}
	}
	
}


bool Test(const DistributeFile & ds, std::vector<MyFile> & vFilInf)
{
	for (size_t i = 0; i < vFilInf.size(); i++)
	{
		if ((vFilInf[i].dst.m_address == ds.m_address)&& (vFilInf[i].dst.m_fileInfo.m_fileHash == ds.m_fileInfo.m_fileHash))
		{
			vFilInf[i].dwStart = GetTickCount();
			return true;
		}
	}
	return false;
}