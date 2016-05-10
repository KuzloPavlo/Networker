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

	std::function<void(const std::string& str)>display;
private:
	std::function<void(const FileStatus& fileStatus, const int& filePercents)>changeFileStatus;
	void start(bool creating);
	void work();
	bool readSessioStatus(SessionStatus* status);
	void readMyStatus();
	void deleteSession(int number);
	bool createEmptyFile(const char fileLocation[MAX_PATH], int sizeFile);
	bool recalculateHashfile();

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
	//int m_donnePercent;
};