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
#include <functional>

using boost::asio::ip::tcp;

class DownloadSession : public std::enable_shared_from_this<DownloadSession>
{
public:
	DownloadSession(
		const boost::asio::ip::address& address,
		boost::asio::io_service& io_service,
		std::shared_ptr<std::mutex>mutexStatus,
		SessionStatus* myStatus,
		const std::string location,
		std::function<void(const std::string& str)>display
		);
	~DownloadSession();
	void start();
	//------------------------------------------
	std::function<void(const std::string& str)>display;
private:
	
	//void read();
	void write();
	void writeHandler(const boost::system::error_code &err, std::size_t bytes);

	//void addPart(const PartFile& partFile);
	//bool flushPart(const PartFile& partFile);
	void connectSeeder(const boost::system::error_code &err);
	void setEnd();
	//void setPart();
	//void unsetPart();
	//bool getPart();

	std::shared_ptr<std::mutex>m_mutexStatus;
	SessionStatus* m_myStatus;
	tcp::socket m_socket;
	int m_sessionNumber;

	boost::asio::io_service& m_io_service;
	boost::asio::ip::address m_address;

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

