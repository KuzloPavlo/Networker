#pragma once
#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost\asio.hpp>
#include <functional>
#include "DownloadingFile.h"
#include <fstream>
#include <mutex>

class Downloader
{
public:
	Downloader(
		boost::asio::io_service& io_service,
		const DownloadingFile& downloadingFile,
		std::shared_ptr<std::mutex> mutexListParts,
		std::function<void(const int& fileHash,const FileStatus& fileStatus,const int& filePercents)>&changeFileStatus);
	~Downloader();
	std::function<void(const int& fileHash, FileStatus& fileStatus, int& filePercents)>changeFileStatus;
	std::function<void(const std::string& str)>display;
	void dosmth(){ display("Downloader::Downloader"); }
	void func(std::function<void(const std::string& str)>& fu){ this->display = fu; };

private:
	std::shared_ptr<std::mutex>m_mutexListParts;
	DownloadingFile m_downloadingFile;
};

