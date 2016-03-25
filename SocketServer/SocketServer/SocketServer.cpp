// SocketServer.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#pragma comment(lib,"Ws2_32.lib")
#include<iostream>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include <string>
#include <vector>
using namespace std;

//вектор на сокеты подключенных клиентов
vector <SOCKET> vecConnectClient;

//количество подключенны клиентов
size_t ClientCount = 0;

//поток передачи сообщений другим пользователям
DWORD WINAPI Thread(LPVOID);

int main()
{
	
	setlocale(LC_ALL, "Russian");

	WSADATA data;
	//какая версия будет использоваться
	WORD version = MAKEWORD(2, 2);
	
	//инициализация библиотеки Winsock
	int res = WSAStartup(version, &data);
	if(res)
	{
		cout << "WSAStartup failed " << GetLastError();
		cin.get();
		return 0;
	}

	// структуры данных адреса
	struct addrinfo hints;
	struct addrinfo *result;

	//очистка hints
	ZeroMemory(&hints, sizeof(hints));

	//указываем параметры для структуры addrinfo
	hints.ai_family = AF_INET; // через интернет
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_STREAM; // поточный
	hints.ai_protocol = IPPROTO_TCP; // протокол TCP

	// получаем информацию о хосте
	getaddrinfo(NULL, "3030", &hints, &result);


	//инициализация
	SOCKET lst;

	//заполняем данными
	lst = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	//функция для привязки
	bind(lst, result->ai_addr, result->ai_addrlen);
	

	//слушаем сокет
	listen(lst, SOMAXCONN);

	//освобождаем result
	freeaddrinfo(result);

	cout << "Start server ...";
	
	//сообщение которое будем передавать при подключении
	string message = "You connetc ...!!!";

	
	//подключающий сокет
	SOCKET Connect;
	while(true)
	{
		//если подключен идем дальше
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

//поток передачи сообщений пользователям
DWORD WINAPI Thread(LPVOID lParam)
{
	//преобразуем обратно в size_t
	size_t iD = reinterpret_cast<size_t>(lParam);
	// создаем буфер для сообщения
	char * buf = new char[1024];

	//цикл обработки сообщений
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