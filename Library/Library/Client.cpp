#include "Client.h"

Client::Client() : m_countConnectedClients(0), m_clientWorking(true)
{
	std::thread listenThread(&Client::threadListen, this);
	listenThread.detach();
}

Client::~Client()
{
	m_mutexUserInteface.lock();
	this->m_clientWorking = false;
	m_mutexUserInteface.unlock();
}

void Client::threadListen()
{
	SOCKET listenSocket = INVALID_SOCKET;
	SOCKET clientSocket = INVALID_SOCKET;
	WSADATA wsaData;
	int iResult;

	//---------------
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	m_mutexUserInteface.lock();
	display("Listener Thread Started");
	m_mutexUserInteface.unlock();
	//------------------
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (iResult != 0)
	{
		// обеспечить обработку ошибки
//		return 1;
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
	//	return 1;
	}

	listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (listenSocket == INVALID_SOCKET)
	{
		// обеспечить обработку ошибки
		freeaddrinfo(result);
		WSACleanup();
		//return 1;
	}
	

	iResult = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{	
		m_mutexUserInteface.lock();
		display("listen thread SOCKET_ERROR");
		m_mutexUserInteface.unlock();
		
		freeaddrinfo(result);
		closesocket(listenSocket);
		WSACleanup();
		return ;
	}

	//----------------------------------
	//----------------------------------
	if (iResult == WSAEADDRINUSE)
	{
		freeaddrinfo(result);
		closesocket(listenSocket);
		WSACleanup();
		m_mutexUserInteface.lock();
		display("Port is busy");
		m_mutexUserInteface.unlock();
		//return;
	}
	//----------------------------------
	//----------------------------------

	iResult = listen(listenSocket, 10);
	if (iResult == SOCKET_ERROR)
	{
		// обеспечить обработку ошибки
		closesocket(listenSocket);
		WSACleanup();
		//return 1;
	}

freeaddrinfo(result);

	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(500));

		m_mutexUserInteface.lock();
		if (!this->m_clientWorking)
		{
			m_mutexUserInteface.unlock();
			break;
		}
		display("Listening");
		m_mutexUserInteface.unlock();

		clientSocket = INVALID_SOCKET;
		clientSocket = accept(listenSocket, NULL, NULL);  // !от сюда можно выт€гивать IP клиента
		if (clientSocket == INVALID_SOCKET)
		{
			m_mutexUserInteface.lock();
			display("Listen Thread 1");
			m_mutexUserInteface.unlock();
			// обеспечить обработку ошибки
			continue;
		}

		if (clientSocket == SOCKET_ERROR)
		{
			m_mutexUserInteface.lock();
			display("Listen Thread 2");
			m_mutexUserInteface.unlock();
		}

		m_mutexUserInteface.lock();
		display("Listen Thread It is connect");
		m_mutexUserInteface.unlock();
		// здесь необходимо вызывать клиентский поток
	}

	closesocket(listenSocket);
	closesocket(clientSocket);
	WSACleanup();

	// освободить все ресурсы
	// установить событие о том что поток закончил свою работу  
}

void Client::threadServer(const std::string& IPaddress, const std::string& port)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));	
	m_mutexUserInteface.lock();
	display("Server Thread Started");
	display(IPaddress);
	display(port);
	m_mutexUserInteface.unlock();

	SOCKET serverSocket = INVALID_SOCKET;
	int iResult = 0;

	if (iResult != 0)
	{
		m_mutexUserInteface.lock();
		display("Server Thread 1");
		m_mutexUserInteface.unlock();
		// обеспечить обработку ошибки
		//		return 1;
	}

	struct addrinfo *result = NULL, *ptr = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	
	iResult = getaddrinfo(IPaddress.c_str(), port.c_str(), &hints, &result);

	if (iResult != 0)
	{
		m_mutexUserInteface.lock();
		display("Server Thread == 0");
		m_mutexUserInteface.unlock();
		// обеспечить обработку ошибки
		WSACleanup();

		m_mutexUserInteface.lock();
		display(std::to_string(WSAGetLastError()));
		m_mutexUserInteface.unlock();
	}

	ptr = result;

	m_mutexUserInteface.lock();
	display("Server Thread 4");
	m_mutexUserInteface.unlock();

	serverSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

	m_mutexUserInteface.lock();
	display("Server Thread 41");
	m_mutexUserInteface.unlock();

	if (serverSocket == INVALID_SOCKET)
	{
		// обеспечить обработку ошибки

		m_mutexUserInteface.lock();
		display("Server Thread 5");
		m_mutexUserInteface.unlock();
		freeaddrinfo(result);
		WSACleanup();
	}

	iResult = connect(serverSocket, ptr->ai_addr, (int)ptr->ai_addrlen);

	m_mutexUserInteface.lock();
	display("Server Thread 51");
	m_mutexUserInteface.unlock();

	if (iResult == SOCKET_ERROR)
	{
		closesocket(serverSocket);
		serverSocket = INVALID_SOCKET;

		m_mutexUserInteface.lock();
		display("Server Thread 6");
		m_mutexUserInteface.unlock();
	}

	freeaddrinfo(result);

	if (serverSocket == INVALID_SOCKET)
	{

		m_mutexUserInteface.lock();
		display("Server Thread 7");
		m_mutexUserInteface.unlock();
		WSACleanup();
	}

	m_mutexUserInteface.lock();
	display("Server Thread Connected");
	m_mutexUserInteface.unlock();
}

void Client::threadClient(void *arg)
{
	SOCKET clientSocket = (SOCKET)arg;
	char receiveBuffer[4096] = { 0 };
	char sendBuffer[4096] = { 0 };
	int receiveSize = 0;
	int sendSize = 0;
	int returnResult = 0;
	//display("thread client");
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(500));

		m_mutexUserInteface.lock();
		if (!this->m_clientWorking)
		{
			m_mutexUserInteface.unlock();
			break;
		}
		m_mutexUserInteface.unlock();

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

void Client::connectToClient(const std::string& IPaddress, const std::string& port)
{

}

void Client::connectToServer(const std::string& IPaddress, const std::string& port)
{
	m_mutexUserInteface.lock();
	display("Connecting to server");
	m_mutexUserInteface.unlock();

	std::thread serverThread(&Client::threadServer, this , IPaddress, port);
	serverThread.detach();

}

void Client::createNewDownloadingFile(std::string location, std::string description)
{

}