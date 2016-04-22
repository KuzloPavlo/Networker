#include "DownloadSession.h"


DownloadSession::DownloadSession(
	const boost::asio::ip::address& address,
	boost::asio::io_service& io_service,
	std::shared_ptr<std::mutex>mutexStatus,
	SessionStatus* myStatus
	) : m_myStatus(myStatus),
	m_mutexStatus(mutexStatus),
	m_socket(tcp::socket(io_service))
{
	tcp::endpoint ep(address, 77777);
	m_socket.async_connect(ep, std::bind(&DownloadSession::connectSeeder, this, std::placeholders::_1));
}


DownloadSession::~DownloadSession()
{
}

void DownloadSession::connectSeeder(const boost::system::error_code &err)
{
	if (err)
	{
		// bed

	}
	m_mutexStatus->lock();
	m_sessionNumber = m_myStatus->m_sessionNumber;
	m_partNumber.m_fileHash = m_myStatus->m_fileHash;
	m_partNumber.m_partNumber = m_myStatus->m_part;
	m_mutexStatus->unlock();

	m_receiveBuffer = (char*)& m_receivedPart;
	m_sendBuffer = (char*)& m_partNumber;

	write();
}

void DownloadSession::read()
{
	auto self(shared_from_this());
	m_socket.async_read_some(boost::asio::buffer(m_receiveBuffer, receiveLength),
		[this, self](boost::system::error_code ec, std::size_t length)
	{
		if (!ec)
		{
			addPart(m_receivedPart);
			//write(/*length*/);
		}
	});



}

void DownloadSession::write()
{
	//m_sendBuffer = (char*)& m_currentPart;

	auto self(shared_from_this());
	boost::asio::async_write(m_socket, boost::asio::buffer(m_sendBuffer, sendLength),
		[this, self](boost::system::error_code ec, std::size_t)
	{
		if (!ec)
		{
			read();
		}
	});
}

void  DownloadSession::addPart(const PartFile& partFile)
{
	//if (/*partFile.m_partHash == partHash(partFile)*/)
	//{

	//}
//	write();
}

void  DownloadSession::flushPart(const PartFile& partFile)
{

}