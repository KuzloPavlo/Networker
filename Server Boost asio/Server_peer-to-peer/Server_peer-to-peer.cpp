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



#define SIZE_FOR_COUNT 4


std::vector<DistributeFile> vFilInf;


using boost::asio::ip::tcp;

class session
	: public std::enable_shared_from_this<session>
{
public:
	session(tcp::socket socket)
		: socket_(std::move(socket))
	{
	}

	void start()
	{
		do_write(6144);
	}

private:
	//чтение
	void do_read()
	{
		std::cout << "do Read" << std::endl;
		// в size_count запишится сколько чайстей читать
		char * buf = (char *)&size_count;
		auto self(shared_from_this());
		socket_.async_read_some(boost::asio::buffer(buf, SIZE_FOR_COUNT),
			[this, self](boost::system::error_code ec, std::size_t length)
		{
			// в форе считываем нужное количество раз
			for (size_t i = 0; i < size_count; i++)
			{
				
				char * buf = (char*)&f;
				socket_.async_read_some(boost::asio::buffer(buf, sizeof(FileInfo)),
					[this, self](boost::system::error_code ec, std::size_t length)
				{
					
				});
				boost::asio::ip::tcp::endpoint remote_ep = socket_.remote_endpoint();
				dst.m_fileInfo = f;
				dst.m_address = remote_ep.address();
				vFilInf.push_back(dst);

				// выводит что считали + IP
				std::cout << "Read <- " << dst.m_fileInfo.m_fileName << std::endl;
				std::cout << "Read <- " << dst.m_fileInfo.m_fileDescription << std::endl;
				std::cout << "Read size Vec - " << vFilInf.size() << std::endl << std::endl;

				std::cout << "Read <- IP - " << dst.m_address << std::endl;
			}
			std::cout << "|---------------------------------------------------------------|"<<std::endl;
		});
		Sleep(1000);
	}

	void do_write(std::size_t length)
	{
		Sleep(1000);
		//отправляем
		std::cout << "do_write" << std::endl;
		auto self(shared_from_this());
		
		//сколько раз будем отправлять
		size_count = vFilInf.size();
		char * buf = (char *)&size_count;
		std::cout << "Send Num size - " << size_count << std::endl;
		boost::asio::async_write(socket_, boost::asio::buffer(buf, 4),
			[this, self](boost::system::error_code ec, std::size_t )
		{
			// в форе отправляем нужное кол раз
			for (size_t i = 0; i < vFilInf.size(); i++)
			{
				dst=vFilInf[i];
				char * buf = (char *)&dst;
				boost::asio::async_write(socket_, boost::asio::buffer(buf, sizeof(DistributeFile)),
					[this, self](boost::system::error_code ec, std::size_t length)
				{
					
					
				});
				//выводим что отправили
				std::cout << "Send -> " << vFilInf[i].m_fileInfo.m_fileName << std::endl;
				std::cout << "Send -> " << vFilInf[i].m_fileInfo.m_fileDescription << std::endl;
				std::cout << "Send -> " << vFilInf[i].m_address << std::endl << std::endl;
			}
			if (!ec)
			{
				do_read();
			}
		});
	}

	tcp::socket socket_;
	enum { max_length = 6144/*4368*/ };
	char data_[max_length];
	FileInfo f;
	int size_count;
	DistributeFile dst;

	
};

class server
{
public:
	server(boost::asio::io_service& io_service, short port)
		: acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
		socket_(io_service)
	{
		std::cout << "Start to server" << std::endl;
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

int main(int argc, char* argv[])
{
	try
	{
		boost::asio::io_service io_service;

		server s(io_service, std::atoi("33333"));
		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
	return 0;
}

