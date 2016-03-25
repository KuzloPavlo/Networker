// SocketServer.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"
#pragma comment(lib,"Ws2_32.lib")
#include<iostream>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include <string>
#include <vector>
using namespace std;

//������ �� ������ ������������ ��������
vector <SOCKET> vecConnectClient;

//���������� ����������� ��������
size_t ClientCount = 0;

//����� �������� ��������� ������ �������������
DWORD WINAPI Thread(LPVOID);

int main()
{
	
	setlocale(LC_ALL, "Russian");

	WSADATA data;
	//����� ������ ����� ��������������
	WORD version = MAKEWORD(2, 2);
	
	//������������� ���������� Winsock
	int res = WSAStartup(version, &data);
	if(res)
	{
		cout << "WSAStartup failed " << GetLastError();
		cin.get();
		return 0;
	}

	// ��������� ������ ������
	struct addrinfo hints;
	struct addrinfo *result;

	//������� hints
	ZeroMemory(&hints, sizeof(hints));

	//��������� ��������� ��� ��������� addrinfo
	hints.ai_family = AF_INET; // ����� ��������
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_STREAM; // ��������
	hints.ai_protocol = IPPROTO_TCP; // �������� TCP

	// �������� ���������� � �����
	getaddrinfo(NULL, "3030", &hints, &result);


	//�������������
	SOCKET lst;

	//��������� �������
	lst = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	//������� ��� ��������
	bind(lst, result->ai_addr, result->ai_addrlen);
	

	//������� �����
	listen(lst, SOMAXCONN);

	//����������� result
	freeaddrinfo(result);

	cout << "Start server ...";
	
	//��������� ������� ����� ���������� ��� �����������
	string message = "You connetc ...!!!";

	
	//������������ �����
	SOCKET Connect;
	while(true)
	{
		//���� ��������� ���� ������
		if(Connect = accept(lst, NULL, NULL))
		{
			cout << "Client connect ...";
			vecConnectClient[ClientCount] = Connect;
			send(vecConnectClient[ClientCount], message.c_str(), message.length(), NULL);
			ClientCount++;
			CreateThread(0, 0, Thread, (LPVOID)(ClientCount - 1), 0, 0);

		}
	}

    return 0;
}

//����� �������� ��������� �������������
DWORD WINAPI Thread(LPVOID lParam)
{
	//����������� ������� � size_t
	size_t iD = reinterpret_cast<size_t>(lParam);
	// ������� ����� ��� ���������
	char * buf = new char[1024];

	//���� ��������� ���������
	while(true)
	{
		memset(buf, '\0', strlen(buf));
		if(recv(vecConnectClient[iD],buf,1024,NULL))
		{
			cout << buf << endl;
			for(size_t i = 0; i < ClientCount; i++)
			{
				send(vecConnectClient[i], buf, strlen(buf), NULL);
			}
		}
		Sleep(100);
	}
	delete buf;
	return 0;
}