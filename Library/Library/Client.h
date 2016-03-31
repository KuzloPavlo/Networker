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

#pragma comment(lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


class Client
{
public:
	Client();
	~Client();
	void readServer();
	int readClient(char * const reseiveBuffer, const int& receiveSize, const char* sendBuffer);
	void connectToServer(const std::string& IPaddress, const std::string& port);
	void connectToClient(const std::string& IPaddress, const std::string& port);
	void createNewDownloadingFile( std::string location,  std::string description);

	std::function<void(const std::string& str)>display;
	std::function<void(const DownloadingFile& newFile)>addNewFile;
	std::function<void(const DownloadingFile& fileStatus)>changeFileStatus;
private:
	int m_countConnectedClients;
	bool m_clientWorking;
	std::mutex m_mutexUserInteface;               //Only one thread is working with the interface
	void threadListen();
	void threadServer(const std::string& IPaddress, const std::string& port);
	void threadClient(void *arg);
	void threadCreateDownloadingFile(std::string location, std::string description);
	void sendOutgoingDistribution(SOCKET *serverSocket);
};