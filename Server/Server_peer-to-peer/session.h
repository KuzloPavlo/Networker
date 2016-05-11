#pragma once
#include <boost/asio.hpp>
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
#include <algorithm>
#include "MyFile.h"
#include <mutex>



class session :
	public std::enable_shared_from_this<session>
{
public:
	session(tcp::socket socket);

	void start()
	{
		do_write(max_length);
	}

private:
	//чтение
	void do_read();

	void do_write(std::size_t length);

	tcp::socket socket_;
	enum { max_length = 4368 /*6144*/ };
	char data_[max_length];
	FileInfo f;
	int size_count_read;
	int size_count_do_write;
	MyFile mf_Dst;
};

bool Test(const DistributeFile & ds, std::vector<MyFile> & vFilInf);
void Show();