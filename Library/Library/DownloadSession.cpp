#include "DownloadSession.h"

#define DISPLAY display("");display("");

DownloadSession::DownloadSession(
	const boost::asio::ip::address& address,
	boost::asio::io_service& io_service,
	Synchronization primitives,
	SessionStatus* myStatus,
	const std::string location,
	std::function<void(const std::string& str)>display
	) : m_myStatus(myStatus), 
	m_primitives(primitives),
	m_io_service(io_service),
	m_socket(io_service),
	m_fileLocation(location)
{
	m_address = address;
	this->display = display;

	//m_mutexStatus->lock();
	m_sessionNumber = m_myStatus->m_sessionNumber;
	m_partNumber.m_fileHash = m_myStatus->m_fileHash;
	m_partNumber.m_partNumber = m_myStatus->m_part;
//	m_mutexStatus->unlock();

	display(std::to_string(m_sessionNumber));
	display(std::to_string(m_partNumber.m_fileHash));
	display(std::to_string(m_partNumber.m_partNumber));
	display(m_fileLocation);

	start();
}

void DownloadSession::start()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	tcp::endpoint ep(m_address, 77777);
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

	m_file.open(m_fileLocation, std::ios::out | std::ios::binary);
	if (!m_file)
	{
		display(std::to_string(GetLastError()));
		setEnd();
		return;
	}

	m_receiveBuffer = (char*)& m_receivedPart;
	m_sendBuffer = (char*)& m_partNumber;
	write();
}
//

void DownloadSession::write()
{
	boost::asio::async_write(m_socket, boost::asio::buffer(m_sendBuffer, sendLength),
		std::bind(&DownloadSession::writeHandler, this, std::placeholders::_1, std::placeholders::_2));
}

void DownloadSession::writeHandler(const boost::system::error_code &err, std::size_t bytes)
{
	if (!err)
	{
		read();
	}
	else
	{
		setEnd();  // возможно надо что-то другое
		return;
	}
}

void DownloadSession::read()
{
	m_socket.async_read_some(boost::asio::buffer(m_receiveBuffer, receiveLength),
		std::bind(&DownloadSession::readHandler, this, std::placeholders::_1, std::placeholders::_2));
}

void DownloadSession::readHandler(const boost::system::error_code &err, std::size_t bytes)
{
	if (!err)
	{
		display(std::to_string(m_receivedPart.m_partNumber));
		addPart(m_receivedPart);
	}
	else
	{
		setEnd();
		return;
	}
}

void  DownloadSession::addPart(const PartFile& partFile)
{
	if (partFile.m_values == ReturnValues::good)// && partFile.m_partHash == calculatePartHash(partFile))
	{
		if (!flushPart(partFile))
		{
			setEnd();
			return;
		}
	}
	else if (partFile.m_values == ReturnValues::noPart)
	{
		unsetPart();
	}
	else if (partFile.m_values == ReturnValues::noDistribution)
	{
		setEnd();
		return;
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
	//m_mutexStatus->lock();
	m_myStatus->m_work = StatusValue::end;
	//m_mutexStatus->unlock();
	display("DownloadSession::setEnd()2");
}

void DownloadSession::setPart()
{
//	m_mutexStatus->lock();
	m_myStatus->m_work = StatusValue::stay;
	m_myStatus->m_doit = StatusValue::set;
//	m_mutexStatus->unlock();
}

void DownloadSession::unsetPart()
{
//	m_mutexStatus->lock();
	m_myStatus->m_work = StatusValue::stay;
	m_myStatus->m_doit = StatusValue::unset;
//	m_mutexStatus->unlock();
}

bool DownloadSession::getPart()
{
//	m_mutexStatus->lock();

	switch (m_myStatus->m_work)
	{
	case StatusValue::work:
		m_partNumber.m_partNumber = m_myStatus->m_part; // geting a new part
	//	m_mutexStatus->unlock();
		break;

	case StatusValue::stay:
	//	m_mutexStatus->unlock();
		return getPart();

	case StatusValue::end:
	//	m_mutexStatus->unlock();
		return false;
	}
	return true;
}