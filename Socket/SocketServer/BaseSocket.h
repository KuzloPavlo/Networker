#pragma once
#pragma comment(lib,"Ws2_32.lib")
#include<iostream>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include <string>
#include <vector>
using namespace std;
class BaseSocket
{
protected:
	WSADATA data;
	WORD version;
	SOCKET sock;
	int WSAStartUp();
public:

	BaseSocket();
	BaseSocket(const int af, const int type, const int protocol);
	
	int Send(SOCKET sockt, const char * buf, const size_t & len);
	
	int Recv(SOCKET sockt, char * buf, const size_t & len);
	
	virtual ~BaseSocket();
private:

};

