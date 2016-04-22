#pragma once
#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost\asio.hpp>
#include "SessionStatus.h"
#include <mutex>
#include "PartFile.h"

using boost::asio::ip::tcp;

class DownloadSession : public std::enable_shared_from_this<DownloadSession>
{
public:
	DownloadSession(
		const boost::asio::ip::address& address,
		boost::asio::io_service& io_service,
		std::shared_ptr<std::mutex>mutexStatus,
		SessionStatus* myStatus
		);
	~DownloadSession();

private:
	void read();
	void write();
	void addPart(const PartFile& partFile);
	void flushPart(const PartFile& partFile);

	std::shared_ptr<std::mutex>m_mutexStatus;
	SessionStatus* m_myStatus;
	tcp::socket m_socket;
	int m_sessionNumber;
	void connectSeeder(const boost::system::error_code &err);

	PartInfo m_partNumber;
	PartFile m_receivedPart;
	enum{
		receiveLength = sizeof(PartFile),
		sendLength = sizeof(PartInfo)
	};
	char* m_receiveBuffer;
	char* m_sendBuffer;
};

