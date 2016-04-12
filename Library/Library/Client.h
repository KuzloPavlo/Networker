#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#pragma once
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Wincrypt.h>
#include <iphlpapi.h>
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

#pragma comment(lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


class Client
{
public:
	Client();
	~Client();
	void connnect();
	void readServer();
	int readClient(char * const reseiveBuffer, const int& receiveSize, const char* sendBuffer);
	void connectToServer(const std::string& IPaddress, const std::string& port);
	void connectToClient(const std::string& IPaddress, const std::string& port);
	void createNewDownloadingFile(std::string location, std::string description);
	void searchFile(const std::string& tockenFile);
	void downloadFile(const int& fileHash);

	std::function<void(const std::string& str)>display;
	std::function<void(const DownloadingFile& newFile)>addNewFile;
	std::function<void(const int& fileHash, FileStatus& fileStatus, float& filePercents)>changeFileStatus;
	std::function<void(const FileInfo& foundFile)>showFoundFile;

private:
	int m_countConnectedClients;
	bool m_clientWorking;
	std::mutex m_mutexUserInteface;               //Only one thread is working with the interface
	std::shared_ptr<std::mutex>m_mutexOutgoingDistribution;
	std::shared_ptr<std::mutex>m_mutexDistribution;
	std::shared_ptr<std::mutex>m_mutexListParts;
	std::shared_ptr<Listener> m_Listener;
	void threadListen();
	void threadServer(const std::string& IPaddress, const std::string& port);
	void threadClient(void *arg);
	void threadDownload(/*DownloadingFile downloadingFile*/);
	void threadCreateDownloadingFile(std::string location, std::string description);
	void threadSearchFile(std::string tockenFile);
	void sendOutgoingDistribution(SOCKET *serverSocket);
	void reciveDistribution(SOCKET *serverSocket);
	int getLargestCommonSubstring(/*std::string & result,*/ const std::string & a, const std::string & b);
};