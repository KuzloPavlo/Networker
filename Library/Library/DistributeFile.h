#pragma once
#include "DownloadingFile.h"
#include <boost\asio.hpp>
#include <utility>
using boost::asio::ip::tcp;


struct DistributeFile
{
	FileInfo m_fileInfo;
	boost::asio::ip::address m_address;
};