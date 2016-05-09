#pragma once
#include <cstdlib>
#include <mutex>
#include <iostream>
#include <memory>
#include <utility>
#include <boost\asio.hpp>
#include "DownloadingFile.h"
#include "PartFile.h"
#include <fstream>
#include "ReturnValues.h"

using boost::asio::ip::tcp;

class ClientSession : public std::enable_shared_from_this<ClientSession>
{
public:
	ClientSession(
		tcp::socket clientSocket,
		std::shared_ptr<std::mutex> mutexOutgoingDistribution,
		std::function<void(const std::string& str)>display);

	void start();
	std::function<void(const std::string& str)>display;
private:
	void read();
	void write(const ReturnValues& value);
	bool getFileInfo(long int fileHash);
	bool preparePart();
	
	tcp::socket m_ClientSocket;
	std::shared_ptr<std::mutex> m_mutexOutgoingDistribution;
	
	DownloadingFile m_downloadingFile;

	std::ifstream m_file;
	
	PartInfo m_partNumber;
	PartFile m_sendPart;

	bool m_firstTime;

	enum{
		receiveLength = sizeof(PartInfo),
		sendLength = sizeof(PartFile)
	};
	char* m_receiveBuffer;
	char* m_sendBuffer;
};

