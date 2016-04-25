#pragma once
#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost\asio.hpp>
#include "SessionStatus.h"
#include <mutex>
#include "PartFile.h"
#include <fstream>
#include <thread>
#include "ReturnValues.h"

using boost::asio::ip::tcp;

class DownloadSession : public std::enable_shared_from_this<DownloadSession>
{
public:
	DownloadSession(
		const boost::asio::ip::address& address,
		boost::asio::io_service& io_service,
		std::shared_ptr<std::mutex>mutexStatus,
		SessionStatus* myStatus,
		const std::string location
		);
	~DownloadSession();

private:
	void read();
	void write();
	void addPart(const PartFile& partFile);
	bool flushPart(const PartFile& partFile);
	void connectSeeder(const boost::system::error_code &err);
	void setEnd();
	void setPart();
	void unsetPart();
	bool getPart();

	std::shared_ptr<std::mutex>m_mutexStatus;
	SessionStatus* m_myStatus;
	tcp::socket m_socket;
	int m_sessionNumber;

	std::ofstream m_file;
	std::string m_fileLocation;

	PartInfo m_partNumber;
	PartFile m_receivedPart;
	enum{
		receiveLength = sizeof(PartFile),
		sendLength = sizeof(PartInfo)
	};
	char* m_receiveBuffer;
	char* m_sendBuffer;
};

