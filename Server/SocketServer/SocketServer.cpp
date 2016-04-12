#include "stdafx.h"
#include "SocketServer.h"


SocketSetver::SocketSetver():BaseSocket(), vecSockClient(0), ClientCount(0)
{

}

SocketSetver::SocketSetver(const int af, const int type, const int protocol) :
	BaseSocket(af, type, protocol), vecSockClient(0), ClientCount(0)
{

}

void SocketSetver::Setlocal_addr(const ADDRESS_FAMILY &  sin_family ,const size_t & potr)
{
	
	local_addr.sin_family = sin_family;
	local_addr.sin_port = ::htons(potr);;
	local_addr.sin_addr.s_addr = NULL;

}

int SocketSetver::Bind()
{
	if(::bind(sock, (sockaddr*)&local_addr, sizeof(local_addr)) == SOCKET_ERROR)
	{
		std::cout << "Îøèáêà bind = " << ::WSAGetLastError();
		return WSAGetLastError();
	}
	return 0;
}

int SocketSetver::Listen()
{

	if(::listen(sock, MAX_COUNT_CLIENT) == SOCKET_ERROR)
	{
		std::cout << "Îøèáêà listen = " << WSAGetLastError();
		return  WSAGetLastError();
	}
	return 0;
}

int SocketSetver::Accept()
{
	vecClient_addr.push_back({0});
	int size_addr = sizeof(vecClient_addr[ClientCount]);
	vecSockClient.push_back( accept(sock, (sockaddr*)&vecClient_addr[ClientCount], &size_addr));
	if(vecSockClient[ClientCount]== INVALID_SOCKET)
	{
		//cout << "Îøèáêà accept = " << ::WSAGetLastError();
		return false;
	}
	else
	{
		ClientCount++;
		return true;
	}
	
}



SocketSetver::~SocketSetver()
{
	for(size_t i = 0; i < vecSockClient.size(); i++)
	{
		if(vecSockClient[0])
		{
			closesocket(vecSockClient[i]);
		}
	}

}
