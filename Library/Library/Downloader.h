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
#include <list>
#include "SessionStatus.h"
#include "DownloadSession.h"
#include <condition_variable>
#include "Semaphore.h"
#include "Synchronization.h"

class Downloader
{
public:
	Downloader(
		boost::asio::io_service& io_service,
		const DownloadingFile& downloadingFile,
		const FileDistributors& adresses,
		std::function<void(const FileStatus& fileStatus, const int& filePercents)>& changeFileStatus,
		std::function<void(const FileStatus& fileStatus)>& changeDownloader,
		Synchronization primitives,
		FileStatus* fileStatus,
		std::function<void(const std::string& str)>display,
		bool creating = true
		);
	~Downloader();

	void changeDownloader(const FileStatus& fileStatus);

	//------------------------------------------------
	std::function<void(const std::string& str)>display;
	void dosmth(){
		display("");
		display("");
		display("Downloader::Downloader");
		display(m_downloadingFile.m_fileInfo.m_fileName);
		display(m_downloadingFile.m_fileInfo.m_fileDescription);
		display(std::to_string(m_downloadingFile.m_fileInfo.m_fileHash));
		display(m_downloadingFile.m_fileLocation);
		display(m_distributors[0].to_string());
		display("");
		display("");
	}
	void func(std::function<void(const std::string& str)>& fu){ this->display = fu; };
	//-------------------------------------------------

private:
	std::function<void(const FileStatus& fileStatus, const int& filePercents)>changeFileStatus;
	void start();
	void work();
	bool readSessioStatus(SessionStatus* status);
	void readMyStatus();
	void deleteSession(int number);

	Checker m_checkerParts;
	FileStatus* m_myStatus; // pause/deleting/downloading
	DownloadingFile m_downloadingFile;
	FileDistributors m_distributors;

	//std::map<int ,DownloadSession*> m_sessions;
	std::list<SessionStatus> m_statusHolder;

	std::map<int, std::shared_ptr<DownloadSession>> m_sessions;

	Synchronization m_primitives;
	FileStatus* m_downloaderStatus;

	boost::asio::io_service& m_io_service;
	int m_lastDistributor = 0;
	int m_donnePercent;
};