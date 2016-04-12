// SocketServer.cpp: определяет точку входа для консольного приложения.
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

//#include <Library\PartFile.h> // пример подключени бибилиотеки

#include "BaseSocket.h"
 



//поток передачи сообщений другим пользователям
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
	//индекс клиента
	int index = sock.ClientCount -1;
	static size_t ivec;
	//проверка на огибку сокет клиента

	int size_count = distribut.size();
	char * buf = (char*)&size_count;

	//проверка есть данные для передачи или нет 
	if(size_count == 0)
	{
		sock.Send(sock.vecSockClient[index], "0", 0);
	}
	//если есть передаю что есть
	else
	{
	
		sock.Send(sock.vecSockClient[index], buf, sizeof(DistributFile));
		for(size_t i = 0; i < size_count; i++)
		{
			buf = (char *)&distribut[i];
			sock.Send(sock.vecSockClient[index], buf, sizeof(DistributFile));
		}
	}
	
	//пустая структура для вектора
	DistributFile dNUL;
	//увеличения размера вектора
	distribut.push_back(dNUL);

	// переменная сколько частей получать от лкиента
	int countFor=0;

	//преобразование ее для получения
	char * pp = (char *)&countFor;

	//переменная для получения самих данныъ
	char * p = (char *)& dwFile;

	// получения количества пакетов для приема
	sock.Recv(sock.vecSockClient[0], pp, 2);

	// вывод количества
	std::cout << std::endl << std::endl << countFor << std::endl << std::endl;

	//цикл получения самих данных
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