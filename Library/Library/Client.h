#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#pragma once
#include "MessageTypes.h"
#include <string>
#include <functional>
#include <thread>
#include <stdio.h>
#include <chrono>
#include <mutex>
#include <iostream>
#include "DownloadingFile.h"
#include <fstream>
#include <vector>
#include <memory>
#include "Listener.h"
#include <cstdlib>
#include <cstring>
#include <boost/asio.hpp>
#include "DownloadSession.h"
#include "Downloader.h"
#include "FileDistributors.h"
#include "DistributeFile.h" 
#include "PartFile.h"
#include "Semaphore.h"
#include "Synchronization.h"

using boost::asio::ip::tcp;

#define	CHANGEFILESTATUS  std::function<void(const FileStatus& fileStatus, const int& filePercents)>
#define CHANGEDOWNLOADER  std::function<void(const FileStatus& fileStatus)>
#define ADDNEWFILE std::function<void (const DownloadingFile& newFile)>

class Client
{
public:
	Client();
	~Client();
	//void connnect();
	void readServer();
	//int readClient(char * const reseiveBuffer, const int& receiveSize, const char* sendBuffer);
	void connectToServer(const std::string& IPaddress, const std::string& port);
	//void connectToClient(const std::string& IPaddress, const std::string& port);
	void createNewDownloadingFile(std::string location, std::string description, ADDNEWFILE addNewFile, CHANGEFILESTATUS changeFileStatus);
	void searchFile(const std::string& tockenFile);

	void downloadFile(
		const DownloadingFile& downloadingFile,
		CHANGEFILESTATUS changeFileStatus,
		CHANGEDOWNLOADER changeDownloader,
		Synchronization primitives,
		FileStatus* fileStatus);

	std::function<void(const std::string& str)>display;
	//std::function<void(const DownloadingFile& newFile)>addNewFile;
	std::function<void(const FileInfo& foundFile)>showFoundFile;

private://
	std::map<FileInfo, FileDistributors> m_distirbution;
	int m_countConnectedClients;
	bool m_clientWorking;

	//std::mutex m_mutexUserInteface;               //Only one thread is working with the interface
	std::shared_ptr<std::mutex>m_mutexOutgoingDistribution;
	std::mutex m_mutexDistribution;
	//std::shared_ptr<Listener> m_Listener;
	void threadListen();
	void threadServer(const std::string& IPaddress, const std::string& port);
	//void threadClient(void *arg);

	void threadDownload(
		const DownloadingFile& downloadingFile,
		const FileDistributors& adresses,
		CHANGEFILESTATUS changeFileStatus,
		CHANGEDOWNLOADER changeDownloader, 
		Synchronization primitives,
		FileStatus* fileStatus
		);

	void threadCreateDownloadingFile(std::string location, std::string description, ADDNEWFILE addNewFile, CHANGEFILESTATUS changeFileStatus);
	void threadSearchFile(std::string tockenFile);
	void sendOutgoingDistribution(tcp::socket* serverSocket);
	void reciveDistribution(tcp::socket* serverSocket);
	int getLargestCommonSubstring(const std::string & a, const std::string & b);
	void addDistributeFile(const DistributeFile& distributeFile);
	FileDistributors getDistributors(const FileInfo& fileInfo);//
};
//