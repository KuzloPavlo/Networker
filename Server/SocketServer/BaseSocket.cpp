#include "stdafx.h"
#include "BaseSocket.h"


BaseSocket::BaseSocket(): data({0}),version(0), sock(0)
{
	WSAStartUp();
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(!sock)
	{
		std::cout << "socket func failed " << WSAGetLastError();
	}

}

BaseSocket::BaseSocket(const int af, const int type, const int protocol): data({0}), version(0), sock(0)
{
	WSAStartUp();
	sock = socket(af, type, protocol);
	if(!sock)
	{
		std::cout << "socket fanc failed " << WSAGetLastError();
	}

}


int BaseSocket::Send(SOCKET sockt, const char * buf, const size_t & len)
{
	int  rez = send(sockt, buf, len, 0);
	if(rez == SOCKET_ERROR)
	{
		std::cout << "Send failed " << WSAGetLastError();
		return WSAGetLastError();
	}
	return rez;
}

int BaseSocket::Recv(SOCKET sockt, char * buf, const size_t & len)
{
	int rez = recv(sockt, buf, len, 0);
	if(rez == SOCKET_ERROR)
	{
		std::cout << "Send failed " << WSAGetLastError();
		return WSAGetLastError();
	}
	return rez;
}

BaseSocket::~BaseSocket()
{
	if(sock)
	{
		closesocket(sock);
		WSACleanup();
	}
}

int BaseSocket::WSAStartUp( )
{
	version = MAKEWORD(2, 2);

	int res = WSAStartup(version, &data);
	if(res)
	{
		std::cout << "WSAStartup failed " << WSAGetLastError();
		std::cin.get();
		return WSAGetLastError();
	}
	return 0;
}
