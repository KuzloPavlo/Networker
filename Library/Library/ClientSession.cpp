#include "ClientSession.h"

ClientSession::ClientSession(tcp::socket clientSocket,
	std::shared_ptr<std::mutex> mutexOutgoingDistribution)
	: m_ClientSocket(std::move(clientSocket)),
	m_mutexOutgoingDistribution(mutexOutgoingDistribution),
	m_firstTime(true)
{
	m_receiveBuffer = (char*)& m_partNumber;
	m_sendBuffer = (char*)& m_sendPart;
}

void ClientSession::start()
{
	read();
}

void ClientSession::read()
{
	auto self(shared_from_this());
	m_ClientSocket.async_read_some(boost::asio::buffer(m_receiveBuffer, receiveLength),
		[this, self](boost::system::error_code ec, std::size_t length)
	{
		if (!ec)
		{
			if (m_firstTime)
			{
				//getFileInfo(m_partNumber.m_fileHash);
			}
			write(length);
		}
	});
}

void ClientSession::write(std::size_t length)
{
	auto self(shared_from_this());
	boost::asio::async_write(m_ClientSocket, boost::asio::buffer(m_sendBuffer, sendLength),
		[this, self](boost::system::error_code ec, std::size_t)
	{
		if (!ec)
		{
			read();
		}
	});
}

bool ClientSession::getFileInfo(long int fileHash)
{
	int numberOutDistribution = 0;
	int fileSize = 0;
	char* buff = (char*)& m_downloadingFile;

	m_mutexOutgoingDistribution->lock();

	std::ifstream in("OutgoingDistribution", std::ios::in | std::ios::binary);
	if (!in)
	{
		m_mutexOutgoingDistribution->unlock();
		// обработать ошыбку
		return false;
	}

	in.seekg(0, in.end);
	fileSize = in.tellg();
	in.seekg(0, in.beg);

	numberOutDistribution = fileSize / sizeof(DownloadingFile);

	for (int i = 0; i < numberOutDistribution; i++)
	{
		in.read(buff, sizeof(DownloadingFile));

		if (m_partNumber.m_fileHash == m_downloadingFile.m_fileInfo.m_fileHash)
		{
			return true;
		}
	}
	return false;
}