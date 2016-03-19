#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#pragma once
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#pragma comment(lib, "Ws2_32.lib")

class Client
{
public:
	void readServer();
	void readClient();
	void connectToServer();
	void connectToClient();
private:
	DWORD __stdcall threatListen(void *arg);
	DWORD __stdcall threadServer(void *arg);
	DWORD __stdcall threadClient(void *arg);
};