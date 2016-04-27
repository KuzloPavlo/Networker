#include "DownloadSession.h"

#define DISPLAY Sleep(1);display("");display("");

DownloadSession::DownloadSession(
	const boost::asio::ip::address& address,
	boost::asio::io_service& io_service,
	std::shared_ptr<std::mutex>mutexStatus,
	std::shared_ptr<std::condition_variable> eventStatus,
	SessionStatus* myStatus,
	const std::string location,
	std::function<void(const std::string& str)>display
	) : m_myStatus(myStatus),
	m_mutexStatus(mutexStatus),
	m_eventStatus(eventStatus),
	m_io_service(io_service),
	m_socket(io_service),
	m_fileLocation(location)
{
	m_address = address;
	this->display = display;

	//display("DownloadSession::DownloadSession");
	//-------------
	m_mutexStatus->lock();
	m_sessionNumber = m_myStatus->m_sessionNumber;
	m_partNumber.m_fileHash = m_myStatus->m_fileHash;
	m_partNumber.m_partNumber = m_myStatus->m_part;
	m_mutexStatus->unlock();

//	DISPLAY
	display(std::to_string(m_sessionNumber));
	display(std::to_string(m_partNumber.m_fileHash));
	display(std::to_string(m_partNumber.m_partNumber));
	display(m_fileLocation);
	//------------

	start();
}

void DownloadSession::start()
{
	//display("DownloadSession::start()1");
	tcp::endpoint ep(m_address, 77777);
	m_socket.async_connect(ep, std::bind(&DownloadSession::connectSeeder, this, std::placeholders::_1));
	//m_io_service.run();

//	display("DownloadSession::start()2");
}
DownloadSession::~DownloadSession()
{
}
//
void DownloadSession::connectSeeder(const boost::system::error_code &err)
{
	//display("DownloadSession::connectSeeder1");
	if (err)
	{
		//display("DownloadSession::connectSeeder2");
		setEnd();
		return;
	}
//	display("DownloadSession::connectSeeder3");


	m_file.open(m_fileLocation, std::ios::out | std::ios::binary);
	if (!m_file)
	{
		//display("DownloadSession::connectSeeder4");
		display(std::to_string(GetLastError()));
		setEnd();
		return;
	}

	m_receiveBuffer = (char*)& m_receivedPart;
	m_sendBuffer = (char*)& m_partNumber;
//	display("DownloadSession::connectSeeder5");
	write();
}
//

void DownloadSession::write()
{
	//display("DownloadSession::write()1");
	//auto self(shared_from_this());
	boost::asio::async_write(m_socket, boost::asio::buffer(m_sendBuffer, sendLength),
		std::bind(&DownloadSession::writeHandler, this, std::placeholders::_1, std::placeholders::_2)); //[this, self](boost::system::error_code ec, std::size_t)

	//{	display("DownloadSession::write()2");
	//	if (!ec)
	//	{
	//		display("DownloadSession::write()3");
	//	//	read();
	//	}
	//	else
	//	{
	//		setEnd();  // возможно надо что-то другое
	//		return;
	//	}
	//});
}

void DownloadSession::writeHandler(const boost::system::error_code &err, std::size_t bytes)
{
	//display("DownloadSession::writeHandler1");
	if (!err)
	{/*
		display(std::to_string(sendLength));
		display(std::to_string(bytes));
		display(std::to_string(receiveLength));*/
	//	display("DownloadSession::writeHandler2");
		read();
	}
	else
	{
		setEnd();  // возможно надо что-то другое
		return;
	}

}
//
void DownloadSession::read()
{
	/*DISPLAY
	display("DownloadSession::read()");*/
	//	/*auto self(shared_from_this());
	m_socket.async_read_some(boost::asio::buffer(m_receiveBuffer, receiveLength),
		std::bind(&DownloadSession::readHandler, this, std::placeholders::_1, std::placeholders::_2));
	//		[this, self](boost::system::error_code ec, std::size_t length)
	//	{
	//		if (!ec)
	//		{
	//			addPart(m_receivedPart);
	//		}
	//		else
	//		{
	//			setEnd();
	//			return;
	//		}
	//	});*/
	//
}


void DownloadSession::readHandler(const boost::system::error_code &err, std::size_t bytes)
{
	//DISPLAY
	//display("DownloadSession::readHandler");
	//display(std::to_string(bytes));
	if (!err)
	{
		//display("DownloadSession::readHandler");
		addPart(m_receivedPart);
	}
	else
	{
		setEnd();
		return;
	}
}

//
void  DownloadSession::addPart(const PartFile& partFile)
{

	/*DISPLAY
	display(" DownloadSession::addPart1");*/
	if (partFile.m_values == ReturnValues::good && partFile.m_partHash == calculatePartHash(partFile))
	{
		//display(" DownloadSession::addPart2");
		if (!flushPart(partFile))
		{
			setEnd();
			return;
		}
	}
	else if (partFile.m_values == ReturnValues::noPart)
	{
		//display(" DownloadSession::addPart3");
		//unsetPart();
	}
	else if (partFile.m_values == ReturnValues::noDistribution)
	{
	//	display(" DownloadSession::addPart4");
		setEnd();
		return;
	}
//	display(" DownloadSession::addPart5");

	if (getPart())
	{
		write();
	}
}

bool  DownloadSession::flushPart(const PartFile& partFile)
{
	try
	{
		/*DISPLAY
			display("DownloadSession::flushPart1");*/
		int shift = (m_partNumber.m_partNumber - 1) * PARTSIZE;
		m_file.seekp(shift, std::ios::beg);
		m_file.write(partFile.m_part, partFile.m_partSize);
		m_file.flush();
		//display("DownloadSession::flushPart2");
	}
	catch (const std::exception& ex)
	{
	//	display("DownloadSession::flushPart3");
		return false;
	}
	//display("DownloadSession::flushPart4");
	setPart();
	return true;
}

void  DownloadSession::setEnd()
{//----------------
	DISPLAY
	display("DownloadSession::setEnd()1");
	//-------------------
	m_mutexStatus->lock();
	m_myStatus->m_work = StatusValue::end;
	m_mutexStatus->unlock();
	display("DownloadSession::setEnd()2");
}

void DownloadSession::setPart()
{	
	//DISPLAY
	//	display("DownloadSession::setPart()1");

	m_mutexStatus->lock();
	m_myStatus->m_work = StatusValue::stay;
	m_myStatus->m_doit = StatusValue::set;
	m_mutexStatus->unlock();
//	display("DownloadSession::setPart()2");
}
//
//void DownloadSession::unsetPart()
//{
//	m_mutexStatus->lock();
//	m_myStatus->m_work = StatusValue::stay;
//	m_myStatus->m_doit = StatusValue::unset;
//	m_mutexStatus->unlock();
//}

bool DownloadSession::getPart()
{
	Sleep(1);
//	DISPLAY
//		display("DownloadSession::getPart()");

	m_mutexStatus->lock();
//	display("DownloadSession::getPart()");
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