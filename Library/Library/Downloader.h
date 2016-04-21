#pragma once
#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost\asio.hpp>
#include <functional>
#include "DownloadingFile.h"
#include "FileDistributors.h"
#include "Checker.h"
#include <fstream>
#include <mutex>

class Downloader
{
public:
	Downloader(
		boost::asio::io_service& io_service,
		const DownloadingFile& downloadingFile,
		const FileDistributors& adresses,
		std::function<void(const FileStatus& fileStatus,const int& filePercents)>& changeFileStatus,
		std::function<void(const FileStatus& fileStatus)>& changeDownloader, bool creating = true);
	~Downloader();
	std::function<void(const FileStatus& fileStatus, const int& filePercents)>changeFileStatus;
	void changeDownloader(const FileStatus& fileStatus);
	//------------------------------------------------
	std::function<void(const std::string& str)>display;
	void dosmth(){
		//-----------------------
		display("");
		display("");
		display("Downloader::Downloader"); 
		display(m_downloadingFile.m_fileInfo.m_fileName);
		display(m_downloadingFile.m_fileInfo.m_fileDescription);
		display(std::to_string(m_downloadingFile.m_fileInfo.m_fileHash));
		display(m_downloadingFile.m_fileLocation);
	}
	void func(std::function<void(const std::string& str)>& fu){ this->display = fu; };
	//-------------------------------------------------
	
private:
	//std::shared_ptr<std::mutex>m_mutexListParts;
	DownloadingFile m_downloadingFile;
	FileDistributors m_distributors;
	Checker m_checkerParts;
};

