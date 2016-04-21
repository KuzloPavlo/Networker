#pragma once
#include "DownloadingFile.h"
#include <boost\asio.hpp>
#include <utility>
using boost::asio::ip::tcp;


struct DistributeFile
{
	FileInfo m_fileInfo;
//	sockaddr_in m_addr;
	boost::asio::ip::address m_address;
};