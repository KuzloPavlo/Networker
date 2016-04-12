#pragma once
#include "BaseSocket.h"
#define MAX_COUNT_CLIENT 10
class SocketSetver:public BaseSocket
{
private:
	sockaddr_in local_addr;
	
	
public:
	std::vector <SOCKET> vecSockClient;
	size_t ClientCount;
	SocketSetver();
	SocketSetver(const int af, const int type, const int protocol);
	void Setlocal_addr(const ADDRESS_FAMILY & sin_family, const size_t & potr);
	int Bind();
	int Listen();
	int Accept();
	std::vector<sockaddr_in> vecClient_addr;
	virtual ~SocketSetver();
	
};

