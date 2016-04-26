#include "ClientSession.h"

ClientSession::ClientSession(tcp::socket clientSocket,
	std::shared_ptr<std::mutex> mutexOutgoingDistribution,
	std::function<void(const std::string& str)>display)
	: m_ClientSocket(std::move(clientSocket)),
	m_mutexOutgoingDistribution(mutexOutgoingDistribution),
	m_firstTime(true)
{
	//---------------------------
	this->display = display;
	display("ClientSession::ClientSession");
	//---------------------------
	m_receiveBuffer = (char*)& m_partNumber;
	m_sendBuffer = (char*)& m_sendPart;
}

void ClientSession::start()
{
	read();
}

void ClientSession::read()
{
	Sleep(1);

	display("");
	display("");
	display("ClientSession::read()");

	auto self(shared_from_this());
	m_ClientSocket.async_read_some(boost::asio::buffer(m_receiveBuffer, receiveLength),
		[this, self](boost::system::error_code ec, std::size_t length)
	{
		if (!ec)
		{
			display("ClientSession::read()1");
			display(std::to_string(receiveLength));
			display(std::to_string(length));
			if (m_firstTime)
			{
				display("ClientSession::read()2");
				m_firstTime = false;

				if (!getFileInfo(m_partNumber.m_fileHash))
				{
					display("ClientSession::read()3");
					write(ReturnValues::noDistribution);
					return;
				}	
			}

			if (preparePart())
			{
				display("ClientSession::read()4");
				write(ReturnValues::good);
			}
			else
			{
				display("ClientSession::read()5");
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
			display("ClientSession::write1");
			display(std::to_string(sendLength));
			display(std::to_string(length));
			read();
		}
	});
}

bool ClientSession::getFileInfo(long int fileHash)
{
	display("ClientSession::getFileInfo1");
	int numberOutDistribution = 0;
	int fileSize = 0;
	char* buff = (char*)& m_downloadingFile;

	m_mutexOutgoingDistribution->lock();

	std::ifstream in("OutgoingDistribution", std::ios::in | std::ios::binary);
	if (!in)
	{
		display("ClientSession::getFileInfo2");
		m_mutexOutgoingDistribution->unlock();
		// обработать ошыбку
		return false;
	}
	display("ClientSession::getFileInfo3");
	in.seekg(0, in.end);
	fileSize = in.tellg();
	in.seekg(0, in.beg);

	numberOutDistribution = fileSize / sizeof(DownloadingFile);

	for (int i = 0; i < numberOutDistribution; i++)
	{
		in.read(buff, sizeof(DownloadingFile));

		if (m_partNumber.m_fileHash == m_downloadingFile.m_fileInfo.m_fileHash)
		{//------------------------------------
			display(m_downloadingFile.m_fileLocation);
			//--------------------------------------
			m_file.open(m_downloadingFile.m_fileLocation, std::ios::in | std::ios::binary);
			if (!m_file)
			{
				display("ClientSession::getFileInfo4");
				return false;
			}
			return true;
		}
	}
	return false;
}

bool ClientSession::preparePart()
{
	try
	{	
		Sleep(1);

		display("");
		display("");
		display("ClientSession::preparePart()1");

		int shift = (m_partNumber.m_partNumber - 1) * PARTSIZE;
		m_file.seekg(shift, std::ios::beg);

		m_file.read(m_sendPart.m_part, PARTSIZE);

		m_sendPart.m_partSize = m_file.gcount();
		
		m_sendPart.m_partHash = calculatePartHash(m_sendPart);

		m_sendPart.m_partNumber = m_partNumber.m_partNumber;

		display(std::to_string(m_sendPart.m_partSize));
		display(std::to_string(m_sendPart.m_partHash));
		display(std::to_string(m_sendPart.m_partNumber));
	}
	catch (const std::exception& ex)
	{
		display("ClientSession::preparePart()2");
		return false;
	}
	return true;
}