#include "ClientSession.h"

#define DISPLAY display("");display("");

ClientSession::ClientSession(tcp::socket clientSocket,
	std::shared_ptr<std::mutex> mutexOutgoingDistribution,
	std::function<void(const std::string& str)>display)
	: m_ClientSocket(std::move(clientSocket)),
	m_mutexOutgoingDistribution(mutexOutgoingDistribution),
	m_firstTime(true)
{
	this->display = display;

	m_receiveBuffer = (char*)& m_partNumber;
	m_sendBuffer = (char*)& m_sendPart;
}

void ClientSession::start()
{
	display("ClientSession::start()1");
	read();
	display("ClientSession::start()2");
}

void ClientSession::read()
{
	auto self(shared_from_this());
	m_ClientSocket.async_read_some(boost::asio::buffer(m_receiveBuffer, receiveLength),
		[this, self](boost::system::error_code ec, std::size_t length)
	{
		if (!ec)
		{
			//display("ClientSession::read()11");
			if (m_firstTime)
			{
				//display("ClientSession::read()2");
				m_firstTime = false;

				if (!getFileInfo(m_partNumber.m_fileHash))
				{
					//display("ClientSession::read()3");
					write(ReturnValues::noDistribution);
					//display("ClientSession::read()4");
					return;
				}
			}

			if (preparePart())
			{
				//display("ClientSession::read()2");
				write(ReturnValues::good);
			}
			else
			{
				write(ReturnValues::noPart);
			}
		}
	});
}

void ClientSession::write(const ReturnValues& value)
{
	m_sendPart.m_values = value;

	auto self(shared_from_this());
	boost::asio::async_write(m_ClientSocket, boost::asio::buffer(m_sendBuffer, sendLength),
		[this, self](boost::system::error_code ec, std::size_t length)
	{
		if (!ec)
		{
			display(std::to_string(m_sendPart.m_partNumber));
			read();
		}
	});
}

bool ClientSession::getFileInfo(long int fileHash)
{
	//display("ClientSession::getFileInfo1");
	int numberOutDistribution = 0;
	int fileSize = 0;
	char* buff = (char*)& m_downloadingFile;

	m_mutexOutgoingDistribution->lock();
	//display("ClientSession::getFileInfo2");
	std::ifstream in("OutgoingDistribution", std::ios::in | std::ios::binary);
	if (!in)
	{
		//display("ClientSession::getFileInfo3");
		m_mutexOutgoingDistribution->unlock();
		// обработать ошыбку
		return false;
	}

	//display("ClientSession::getFileInfo4");
	in.seekg(0, in.end);
	fileSize = in.tellg();
	in.seekg(0, in.beg);

	numberOutDistribution = fileSize / sizeof(DownloadingFile);

	for (int i = 0; i < numberOutDistribution; i++)
	{
		in.read(buff, sizeof(DownloadingFile));

		if (m_partNumber.m_fileHash == m_downloadingFile.m_fileInfo.m_fileHash)
		{
			//------------------------------------
			display(m_downloadingFile.m_fileLocation);
			//--------------------------------------
			m_file.open(m_downloadingFile.m_fileLocation, std::ios::in | std::ios::binary);
			if (!m_file)
			{
				m_mutexOutgoingDistribution->unlock();
				return false;
			}
			m_mutexOutgoingDistribution->unlock();
			return true;
		}
	}
	m_mutexOutgoingDistribution->unlock();
	return false;
}

bool ClientSession::preparePart()
{
	try
	{
		int shift = (m_partNumber.m_partNumber - 1) * PARTSIZE;
		m_file.seekg(shift, std::ios::beg);

		m_file.read(m_sendPart.m_part, PARTSIZE);

		m_sendPart.m_partSize = m_file.gcount();

		m_sendPart.m_partHash = calculatePartHash(m_sendPart);

		m_sendPart.m_partNumber = m_partNumber.m_partNumber;
	}
	catch (const std::exception& ex)
	{
		return false;
	}
	return true;
}