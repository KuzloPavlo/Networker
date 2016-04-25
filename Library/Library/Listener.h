#pragma once
#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost\asio.hpp>
#include "ClientSession.h"
#include <fstream>

using boost::asio::ip::tcp;

class Listener
{
public:
	Listener(
		boost::asio::io_service& io_service,
		short port,
		std::shared_ptr<std::mutex> mutexOutgoingDistribution);
	
	std::function<void(const std::string& str)>display;
private:
	void accept();
	tcp::acceptor m_Acceptor;
	tcp::socket m_Socket;
	std::shared_ptr<std::mutex> m_mutexOutgoingDistribution;
};

