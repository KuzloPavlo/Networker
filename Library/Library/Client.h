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

#pragma comment(lib, "Ws2_32.lib")

class Client
{
public:
	Client();
	~Client();
	void readServer();
	int readClient(char * const reseiveBuffer, const int& receiveSize, const char* sendBuffer);
	void connectToServer();
	void connectToClient();

	// Temp functionn for learning
	void setCallBackFunctions(void(*display)(const std::string str));
	void show(const std::string str);
private:
	void(*display)(const std::string str);
	int m_countConnectedClients;
	bool m_clientWorking;
	HANDLE m_mutexUserInteface;               //Only one thread is working with the interface
	DWORD __stdcall threatListen(void *arg);
	DWORD __stdcall threadServer(void *arg);
	DWORD __stdcall threadClient(void *arg);
};