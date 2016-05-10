#include "DownloadSession.h"

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

	m_primitives.m_mutexCounter->lock();
	if ((*m_primitives.m_counter) == 0)
	{
		m_primitives.m_goRead->lock();
		m_primitives.m_Shared->lock();
	}
	(*m_primitives.m_counter)++;
	m_primitives.m_mutexCounter->unlock();

	m_sessionNumber = m_myStatus->m_sessionNumber;
	m_partNumber.m_fileHash = m_myStatus->m_fileHash;
	m_partNumber.m_partNumber = m_myStatus->m_part;

	m_primitives.m_mutexCounter->lock();
	(*m_primitives.m_counter)--;
	if ((*m_primitives.m_counter) == 0)
	{
		m_primitives.m_Shared->unlock_one();
		m_primitives.m_goWrite->unlock_one();
	}
	m_primitives.m_mutexCounter->unlock();

	display("DownloadSession::DownloadSession Start. Her number:");
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
		setEnd(StatusValue::notConnect); 
		return;
	}

	m_file.open(m_fileLocation, std::ios::out | std::ios::binary);
	if (!m_file)
	{
		display(std::to_string(GetLastError()));
		setEnd(StatusValue::notOpen);
		return;
	}

	m_receiveBuffer = (char*)& m_receivedPart;
	m_sendBuffer = (char*)& m_partNumber;
	write();
}

void DownloadSession::write()
{
	boost::asio::async_write(m_socket, boost::asio::buffer(m_sendBuffer, sendLength),
		std::bind(&DownloadSession::writeHandler, this, std::placeholders::_1, std::placeholders::_2));
}

void DownloadSession::writeHandler(const boost::system::error_code &err, std::size_t bytes)
{
	if (!err)
	{
		std::fill(m_receivedPart.m_part, m_receivedPart.m_part + PARTSIZE, 0);
		m_receivedPart.m_partSize = 0;
		m_receivedPart.m_partHash = 0;
		m_receivedPart.m_partNumber = 0;

		//std::this_thread::sleep_for(std::chrono::microseconds(1));
		read();
	}
	else
	{
		setEnd(StatusValue::notWrite);
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
		//-------------------------------------------
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		/*
		std::string str("RECEIVE: ");
		str += std::to_string(m_receivedPart.m_partSize) + "  " + std::to_string(m_receivedPart.m_partHash) + "  " + std::to_string(m_receivedPart.m_partNumber);
		display(str);*/
		//-------------------------------------------
		
		addPart(m_receivedPart);
	}
	else
	{
		setEnd(StatusValue::notRead);
		return;
	}
}

void  DownloadSession::addPart(const PartFile& partFile)
{

	//-----------------------------------------------------------
	/*std::string parthash = std::to_string(partFile.m_partHash);
	std::string realhash = std::to_string(calculatePartHash(partFile));
	parthash += " = " + realhash;
	display(parthash);*/
	//-----------------------------------------------------------

	if (partFile.m_values == ReturnValues::good)// && partFile.m_partHash == calculatePartHash(partFile))
	{
		if (!flushPart(partFile))
		{
			setEnd(StatusValue::notSave);
			return;
		}
	}
	else if (partFile.m_values == ReturnValues::noPart)
	{
		unsetPart();
	}
	else if (partFile.m_values == ReturnValues::noDistribution)
	{
		setEnd(StatusValue::notDistribution);
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

void  DownloadSession::setEnd(const StatusValue& why)
{
	m_primitives.m_mutexCounter->lock();
	if ((*m_primitives.m_counter) == 0)
	{
		m_primitives.m_goRead->lock();
		m_primitives.m_Shared->lock();
	}
	(*m_primitives.m_counter)++;
	m_primitives.m_mutexCounter->unlock();

	m_myStatus->m_work = StatusValue::end;
	m_myStatus->m_why = why;

	m_primitives.m_mutexCounter->lock();
	(*m_primitives.m_counter)--;
	if ((*m_primitives.m_counter) == 0)
	{
		m_primitives.m_Shared->unlock_one();
		m_primitives.m_goWrite->unlock_one();
	}
	m_primitives.m_mutexCounter->unlock();

	std::string str("DownloadSession::setEnd. Her number:");
	str += std::to_string(m_sessionNumber);
	display(str);
}

void DownloadSession::setPart()
{
	m_primitives.m_mutexCounter->lock();
	if ((*m_primitives.m_counter) == 0)
	{
		m_primitives.m_goRead->lock();
		m_primitives.m_Shared->lock();
	}
	(*m_primitives.m_counter)++;
	m_primitives.m_mutexCounter->unlock();

	m_myStatus->m_work = StatusValue::stay;
	m_myStatus->m_doit = StatusValue::set;

	m_primitives.m_mutexCounter->lock();
	(*m_primitives.m_counter)--;
	if ((*m_primitives.m_counter) == 0)
	{
		m_primitives.m_Shared->unlock_one();
		m_primitives.m_goWrite->unlock_one();
	}
	m_primitives.m_mutexCounter->unlock();
}

void DownloadSession::unsetPart()
{
	m_primitives.m_mutexCounter->lock();
	if ((*m_primitives.m_counter) == 0)
	{
		m_primitives.m_goRead->lock();
		m_primitives.m_Shared->lock();
	}
	(*m_primitives.m_counter)++;
	m_primitives.m_mutexCounter->unlock();

	m_myStatus->m_work = StatusValue::stay;
	m_myStatus->m_doit = StatusValue::unset;

	m_primitives.m_mutexCounter->lock();
	(*m_primitives.m_counter)--;
	if ((*m_primitives.m_counter) == 0)
	{
		m_primitives.m_Shared->unlock_one();
		m_primitives.m_goWrite->unlock_one();
	}
	m_primitives.m_mutexCounter->unlock();
}

bool DownloadSession::getPart()
{
	while (true)
	{
		m_primitives.m_mutexCounter->lock();
		if ((*m_primitives.m_counter) == 0)
		{
			m_primitives.m_goRead->lock();
			m_primitives.m_Shared->lock();
		}
		(*m_primitives.m_counter)++;
		m_primitives.m_mutexCounter->unlock();

		switch (m_myStatus->m_work)
		{
		case StatusValue::work:
			m_partNumber.m_partNumber = m_myStatus->m_part; // geting a new part
		
			m_primitives.m_mutexCounter->lock();
			(*m_primitives.m_counter)--;
			if ((*m_primitives.m_counter) == 0)
			{
				m_primitives.m_Shared->unlock_one();
				m_primitives.m_goWrite->unlock_one();
			}
			m_primitives.m_mutexCounter->unlock();

			if (m_partNumber.m_partNumber == 0)
			{
				setEnd(StatusValue::done);
				return false;
			}
			return true;

		case StatusValue::stay:
			m_primitives.m_mutexCounter->lock();
			(*m_primitives.m_counter)--;
			if ((*m_primitives.m_counter) == 0)
			{
				m_primitives.m_Shared->unlock_one();
				m_primitives.m_goWrite->unlock_one();
			}
			m_primitives.m_mutexCounter->unlock();

			break;

		case StatusValue::end:
			m_primitives.m_mutexCounter->lock();
			(*m_primitives.m_counter)--;
			if ((*m_primitives.m_counter) == 0)
			{
				m_primitives.m_Shared->unlock_one();
				m_primitives.m_goWrite->unlock_one();
			}
			m_primitives.m_mutexCounter->unlock();

			return false;
		}
	}
	return true;
}