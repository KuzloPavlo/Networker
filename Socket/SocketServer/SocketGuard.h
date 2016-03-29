#pragma once
#include <WinSock2.h>
#include <iostream>
class SocketGuard
{
	SOCKET mySocket;
public:
	SocketGuard():mySocket(0)
	{}
	SocketGuard(const SOCKET & socket):mySocket(socket)
	{}
	SocketGuard(SocketGuard & sock):mySocket(0)
	{
		if(this == &sock)
		{
			return;
		}
		*this = sock;
	}
	SocketGuard & operator=(SocketGuard & sock)
	{
		if(this==&sock)
				return *this;
		
		if(mySocket)
				mySocket = 0;

		*this = sock;
		return *this;
	}
	virtual~SocketGuard()
	{
		if(mySocket)
		{
			closesocket(mySocket);
		}
	}
};

