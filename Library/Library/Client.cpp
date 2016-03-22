#include "Client.h"

Client::Client() : m_countConnectedClients(0), m_clientWorking(true)
{
	m_mutexUserInteface = CreateMutex(NULL, FALSE, NULL);
}

Client::~Client()
{
	this->m_clientWorking = false;
}

DWORD __stdcall Client::threatListen(void *arg)
{
	SOCKET listenSocket = INVALID_SOCKET;
	SOCKET clientSocket = INVALID_SOCKET;
	WSADATA wsaData;
	int iResult;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (iResult != 0)
	{
		// обеспечить обработку ошибки
		return 1;
	}

	struct addrinfo *result = NULL, *ptr = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	iResult = getaddrinfo(NULL, "3030", &hints, &result);

	if (iResult != 0)
	{
		// обеспечить обработку ошибки
		WSACleanup();
		return 1;
	}

	listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (listenSocket == INVALID_SOCKET)
	{
		// обеспечить обработку ошибки
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	iResult = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		// обеспечить обработку ошибки
		freeaddrinfo(result);
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	iResult = listen(listenSocket, 10);
	if (iResult == SOCKET_ERROR)
	{
		// обеспечить обработку ошибки
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

freeaddrinfo(result);

	while (true)
	{
		Sleep(1000);
		
		if (!this->m_clientWorking)
		{
			break;
		}

		clientSocket = INVALID_SOCKET;
		clientSocket = accept(listenSocket, NULL, NULL);  // !от сюда можно вытягивать IP клиента
		if (clientSocket == INVALID_SOCKET)
		{
			// обеспечить обработку ошибки
			continue;
		}
		// здесь необходимо вызывать клиентский поток
	}

	closesocket(listenSocket);
	closesocket(clientSocket);
	WSACleanup();

	// освободить все ресурсы
	// установить событие о том что поток закончил свою работу  

	return 0;
}

DWORD __stdcall Client::threadServer(void *arg)
{
	return 0;
}

DWORD __stdcall Client::threadClient(void *arg)
{
	SOCKET clientSocket = (SOCKET)arg;
	char receiveBuffer[4096] = { 0 };
	char sendBuffer[4096] = { 0 };
	int receiveSize = 0;
	int sendSize = 0;
	int returnResult = 0;

	while (true)
	{
		Sleep(1000);

		if (!this->m_clientWorking)
		{
			break;
		}

		returnResult = 0;
		returnResult = recv(clientSocket, receiveBuffer, 4096, 0);
		
		if (returnResult == 0)
		{
			// обеспечить обработку ошибки
			continue;
		}

		if (returnResult == SOCKET_ERROR)
		{
			// обеспечить обработку ошибки
			continue;
		}

		if (returnResult > (sizeof(char) * 2))
		{
			char* p = (char*)&receiveSize;
			*p = receiveBuffer[0];
			p++;
			*p = receiveBuffer[1];

			if (receiveSize == returnResult - (sizeof(char) * 2))
			{
				p++;
				readClient(p, receiveSize, sendBuffer);
			}
			
		}
	}

	// освободить все ресурсы
	// установить событие о том что поток закончил свою работу  

	return 0;
}

int Client::readClient(char * const reseiveBuffer, const int& receiveSize, const char* sendBuffer)
{
	char* pBuffer = reseiveBuffer;
	char requestType = pBuffer[0];
	
	switch (static_cast<MessageTypes>(requestType))
	{
	case MessageTypes::message:
		
		break;

	default:
		break;
	}

	return 1;
}

void Client::setCallBackFunctions(void(*display)(const std::string str))
{
	this->display = display;
}

void Client::show(const std::string str)
{
	display(str);
}