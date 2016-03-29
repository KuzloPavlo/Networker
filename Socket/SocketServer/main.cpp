// SocketServer.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#pragma comment(lib,"Ws2_32.lib")
#include<iostream>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include <string>
#include <vector>
#include "SocketGuard.h"
#include "BaseSocket.h"
#include "SocketServer.h"
using namespace std;

struct Part
{
	char code;
	char * buf;
	DWORD dwLen;
	char MD[32];
};

//поток передачи сообщений другим пользователям
DWORD WINAPI Thread(LPVOID);

int main()
{
	setlocale(LC_ALL, "Russian");
	
	SocketSetver socketServ;
	socketServ.Setlocal_addr(AF_INET,33333);



	socketServ.Bind();
	socketServ.Listen();
	
	cout << "Start server ..." << endl;
	while(true)
	{
		if(socketServ.Accept()==0)
		{
			cout << "Client connect ...";
			socketServ.Send(socketServ.vecSockClient[socketServ.ClientCount], "Message from server", 20);
		}
	}

	return 0;

	
}