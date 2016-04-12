// SocketServer.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"
#pragma comment(lib,"Ws2_32.lib")

#include<iostream>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include <string>
#include <vector>
#include <thread>
#include "SocketGuard.h"
#include "BaseSocket.h"
#include "SocketServer.h"
#include "lib.h"
#include <fstream>
#include <memory>

//#include <Library\PartFile.h> // ������ ���������� �����������

#include "BaseSocket.h"
 



//����� �������� ��������� ������ �������������
void Thread_Connect(SocketSetver & sock, DownloadingFile & dwFile, std::vector<DistributFile> & distribut);

int main()
{
	std::vector<DistributFile> distribut;

	DownloadingFile dwFile;

	setlocale(LC_ALL, "Russian");
	
	SocketSetver socketServ;
	socketServ.Setlocal_addr(AF_INET,33333);
	

	int sizeBlock = sizeof(DownloadingFile);

	socketServ.Bind();
	socketServ.Listen();
	std::cout << "Start server ..." << std::endl;

	
	while(true)
	{
		if(socketServ.Accept())
		{
			std::cout << "Client connect ...";
			std::thread thrd(Thread_Connect, socketServ, dwFile, distribut);
			thrd.join();
		}
		Sleep(100);
	}
	return 0;
}
void Thread_Connect(SocketSetver & sock, DownloadingFile & dwFile, std::vector<DistributFile> & distribut)
{
	//������ �������
	int index = sock.ClientCount -1;
	static size_t ivec;
	//�������� �� ������ ����� �������

	int size_count = distribut.size();
	char * buf = (char*)&size_count;

	//�������� ���� ������ ��� �������� ��� ��� 
	if(size_count == 0)
	{
		sock.Send(sock.vecSockClient[index], "0", 0);
	}
	//���� ���� ������� ��� ����
	else
	{
	
		sock.Send(sock.vecSockClient[index], buf, sizeof(DistributFile));
		for(size_t i = 0; i < size_count; i++)
		{
			buf = (char *)&distribut[i];
			sock.Send(sock.vecSockClient[index], buf, sizeof(DistributFile));
		}
	}
	
	//������ ��������� ��� �������
	DistributFile dNUL;
	//���������� ������� �������
	distribut.push_back(dNUL);

	// ���������� ������� ������ �������� �� �������
	int countFor=0;

	//�������������� �� ��� ���������
	char * pp = (char *)&countFor;

	//���������� ��� ��������� ����� ������
	char * p = (char *)& dwFile;

	// ��������� ���������� ������� ��� ������
	sock.Recv(sock.vecSockClient[0], pp, 2);

	// ����� ����������
	std::cout << std::endl << std::endl << countFor << std::endl << std::endl;

	//���� ��������� ����� ������
	for(size_t i = 0; i < countFor; i++)
	{
		sock.Recv(sock.vecSockClient[0], p, sizeof(DownloadingFile));

		std::cout << dwFile.m_fileName<< std::endl;
		std::cout << dwFile.m_fileDescription<< std::endl;
		distribut[ivec].dwFile = dwFile;
		distribut[ivec].addr = sock.vecClient_addr[index];
		ivec++;
		distribut.push_back(dNUL);
	}
		
	
	std::cout << "OUT" << std::endl;
}