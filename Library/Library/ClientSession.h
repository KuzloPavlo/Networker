#pragma once
#include <cstdlib>
#include <mutex>
#include <iostream>
#include <memory>
#include <utility>
#include <boost\asio.hpp>
#include "DownloadingFile.h"
#include "PartFile.h"


using boost::asio::ip::tcp;

class ClientSession : public std::enable_shared_from_this<ClientSession>
{
public:
	ClientSession(
		tcp::socket clientSocket,
		std::shared_ptr<std::mutex> mutexOutgoingDistribution);

	void start();
private:
	void read();
	void write(std::size_t length);
	tcp::socket m_ClientSocket;
	std::shared_ptr<std::mutex> m_mutexOutgoingDistribution;
	enum{
		receiveLength = sizeof(PartInfo),
		sendLength = sizeof(PartFile)
	};
	char m_receiveBuffer[receiveLength];
	char m_sendBuffer[sendLength];
};

