#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#pragma once
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include "MessageTypes.h"
#include <string>
#include <functional>
#include <thread>
#include <chrono>
#include <mutex>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

class Client
{
public:
	Client();
	~Client();
	void readServer();
	int readClient(char * const reseiveBuffer, const int& receiveSize, const char* sendBuffer);
	void connectToServer(const std::string& IPaddress, const std::string& port);
	void connectToClient(const std::string& IPaddress, const std::string& port);

	std::function<void(const std::string& str)>display;
private:
//	SOCKET m_listenSocket;
	int m_countConnectedClients;
	bool m_clientWorking;
	std::mutex m_mutexUserInteface;               //Only one thread is working with the interface
	void threadListen();
	void threadServer(const std::string& IPaddress, const std::string& port);
	void threadClient(void *arg);
};