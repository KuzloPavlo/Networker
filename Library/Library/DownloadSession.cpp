#include "DownloadSession.h"


DownloadSession::DownloadSession(
	const boost::asio::ip::address& address,
	boost::asio::io_service& io_service,
	std::shared_ptr<std::mutex>mutexStatus,
	SessionStatus* myStatus,
	const std::string location
	) : m_myStatus(myStatus),
	m_mutexStatus(mutexStatus),
	m_socket(tcp::socket(io_service)),
	m_fileLocation(location)
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
		setEnd();
		return;
	}

	m_mutexStatus->lock();
	m_sessionNumber = m_myStatus->m_sessionNumber;
	m_partNumber.m_fileHash = m_myStatus->m_fileHash;
	m_partNumber.m_partNumber = m_myStatus->m_part;
	m_mutexStatus->unlock();

	m_file.open(m_fileLocation, std::ios::out | std::ios::binary);
	if (!m_file)
	{
		setEnd();
		return;
	}

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
		}
		else
		{
			setEnd();
			return;
		}
	});

}

void DownloadSession::write()
{
	auto self(shared_from_this());
	boost::asio::async_write(m_socket, boost::asio::buffer(m_sendBuffer, sendLength),
		[this, self](boost::system::error_code ec, std::size_t)
	{
		if (!ec)
		{
			read();
		}
		else
		{
			setEnd();  // возможно надо что-то другое
			return;
		}
	});
}

void  DownloadSession::addPart(const PartFile& partFile)
{
	if (partFile.m_partHash == calculatePartHash(partFile))
	{
		if (!flushPart(partFile))
		{
			setEnd();
			return;
		}
	}
	else
	{
		unsetPart();
	}

	if (getPart())
	{
		write();
	}

}

bool  DownloadSession::flushPart(const PartFile& partFile)
{
	try
	{
		int shift = (m_partNumber.m_partNumber - 1) * PARTSIZE;
		m_file.seekp(shift, std::ios::beg);
		m_file.write(partFile.m_part, partFile.m_partSize);
		m_file.flush();
	}
	catch (const std::exception& ex)
	{
		return false;
	}
	setPart();
	return true;
}

void  DownloadSession::setEnd()
{
	m_mutexStatus->lock();
	m_myStatus->m_work = StatusValue::end;
	m_mutexStatus->unlock();
}

void DownloadSession::setPart()
{
	m_mutexStatus->lock();
	m_myStatus->m_work = StatusValue::stay;
	m_myStatus->m_doit = StatusValue::set;
	m_mutexStatus->unlock();
}

void DownloadSession::unsetPart()
{
	m_mutexStatus->lock();
	m_myStatus->m_work = StatusValue::stay;
	m_myStatus->m_doit = StatusValue::unset;
	m_mutexStatus->unlock();
}

bool DownloadSession::getPart()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	m_mutexStatus->lock();
	switch (m_myStatus->m_work)
	{
	case StatusValue::work:
		m_partNumber.m_partNumber = m_myStatus->m_part; // geting a new part
		m_mutexStatus->unlock();
		break;

	case StatusValue::stay:
		m_mutexStatus->unlock();
		return getPart();

	case StatusValue::end:
		m_mutexStatus->unlock();
		return false;
	}
	return true;
}