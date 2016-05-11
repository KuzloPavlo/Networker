#include "stdafx.h"
#include "server.h"
#include "session.h"


using boost::asio::ip::tcp;

server::server(boost::asio::io_service& io_service, short port)
		: acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
		socket_(io_service)
	{
		std::cout << "Start to server V1.4" << std::endl;
		do_accept();
	}

void server::do_accept()
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

	