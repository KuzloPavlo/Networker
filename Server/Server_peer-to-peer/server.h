#pragma once
#include "session.h"
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
class server
{
public:
	server(boost::asio::io_service& io_service, short port);

private:
	void do_accept();
	tcp::acceptor acceptor_;
	tcp::socket socket_;
};