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

	//------------------------------------
	m_mutexUserInteface.lock();
	display("Server Thread Started");
	display(IPaddress);
	display(port);
	m_mutexUserInteface.unlock();
	//------------------------------------

	SOCKET serverSocket = INVALID_SOCKET;
	int iResult = 0;
	char receiveBuffer[4096] = { 0 };	
	int receiveSize = 0;
	int returnResult = 0;

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

	sendOutgoingDistribution(&serverSocket);

	reciveDistribution(&serverSocket);

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
	std::thread createingFile(&Client::threadCreateDownloadingFile, this, location, description);
	createingFile.detach();
}

void Client::threadCreateDownloadingFile(std::string location, std::string description)
{	
	display("client:: new file1");
	const int partSize = 2048;
	char filePart[partSize] = { 0 };
	std::hash<std::string> hashFunction;

	DownloadingFile newFile;
	strcpy(newFile.m_fileDescription, description.c_str());
	strcpy(newFile.m_fileLocation, location.c_str());
	newFile.m_fileStatus = FileStatus::creating;

	std::string tempName;
	tempName.assign(location, location.rfind("\\") + 1, location.size());
	
	strcpy(newFile.m_fileName, tempName.c_str());

	newFile.m_fileHash = (newFile.m_fileHash) ^ hashFunction(tempName);

	std::ifstream in(location,std::ios::in | std::ios::binary);
	if (!in)
	{
		display("client:: new file2");
		// обработать ошыбку
		return;
	}

	display("client:: new file3");
	
	do
	{
		in.read(filePart, partSize);
		newFile.m_numberParts++;
		newFile.m_fileSize += in.gcount();
		newFile.m_fileHash = (1/256) * (newFile.m_fileHash << 1) ^ hashFunction(filePart);

	} while (!in.eof());

	display(location);
	display(tempName);
	display(std::to_string(newFile.m_numberParts));
	display(std::to_string(newFile.m_fileHash));

	std::ofstream out("OutgoingDistribution", std::ios::out | std::ios::app | std::ios::binary);
	if (!out)
	{
		display("client:: new file4");
		// обработать ошыбку
		return;
	}

	char* p =(char*) &newFile;
	char* outBuffer = new char[sizeof(newFile)];
	for (int i = 0; i < sizeof(newFile); i++)
	{
		outBuffer[i] = *p;
		p++;
	}

	out.write(outBuffer, sizeof(newFile));

	delete[]outBuffer;
}

void Client::sendOutgoingDistribution(SOCKET *serverSocket)
{
	DownloadingFile downloadingFile;
	int iResult = 0;
	int fileSize = 0;
	int numberOutDistribution = 0;
	char* buffer = nullptr;

	std::ifstream in("OutgoingDistribution", std::ios::in | std::ios::binary);
	if (!in)
	{
		display("client:: new file4");
		return;
	}
		
	in.seekg(0, in.end);
	fileSize = in.tellg();
	in.seekg(0, in.beg);
	
	numberOutDistribution = fileSize / sizeof(DownloadingFile);

	buffer = (char*)& numberOutDistribution;
	
	iResult = send(*serverSocket, buffer, 2, 0);
	if (iResult == SOCKET_ERROR)
	{
		// обработай
		return;
	}

	display(std::to_string(numberOutDistribution));

	buffer = (char*)& downloadingFile;

	for (int i = 0; i <numberOutDistribution; i++)
	{
		in.read(buffer, sizeof(DownloadingFile));

		iResult = send(*serverSocket, buffer, sizeof(DownloadingFile), 0);
		if (iResult == SOCKET_ERROR)
		{
			// ќЅ–јЅќ“ј…
			return;
		}
	}
}

void  Client::reciveDistribution(SOCKET *serverSocket)
{
	DownloadingFile downloadingFile;
	int iResult = 0;
	int fileSize = 0;
	int numberDistribution = 0;
	char* buffer = (char*)& numberDistribution;

	std::ofstream out("Distribution", std::ios::out | std::ios::binary);
	if(!out)
	{	
		// обработай
		return;
	}

	iResult = recv(*serverSocket, buffer, 2, 0);
	if (!iResult)
	{
		// обработай
		return;
	}
	else if (iResult == SOCKET_ERROR)
	{
		// обработай
		return;
	}

	//-----------------------------------------------
	m_mutexUserInteface.lock();
	display("number:");
	display(std::to_string(numberDistribution));
	m_mutexUserInteface.unlock();
	//---------------------------------------------

	buffer = (char*)& downloadingFile;

	for (int i = 0; i < numberDistribution; i++)
	{
		iResult = recv(*serverSocket, buffer, sizeof(DownloadingFile), 0);
		if (!iResult)
		{
			// обработай
			break;
		}
		else if (iResult == SOCKET_ERROR)
		{
			// обработай
			break;
		}

		//-----------------------------------------------------
		m_mutexUserInteface.lock();
		display(downloadingFile.m_fileDescription);
		display(downloadingFile.m_fileName);
		m_mutexUserInteface.unlock();
		//----------------------------------------------------

		out.write(buffer, sizeof(DownloadingFile));
	}
}

void Client::searchFile(const std::string& tockenFile)
{
	std::thread searchingFile(&Client::threadSearchFile, this, tockenFile);
	searchingFile.detach();
}

void Client::threadSearchFile(std::string tockenFile)
{	
	DownloadingFile foundFile;
	char* buffer = (char*)& foundFile;

	m_mutexDistribution.lock();
	
	std::ifstream in("Distribution", std::ios::in | std::ios::binary);
	if (!in)
	{
		//
		return;
	}

	m_mutexUserInteface.lock();

	if(tockenFile == "*")
	{
		while (in)
		{
			in.read(buffer, sizeof(DownloadingFile));
			showFoundFile(foundFile);
		}
	}

	else if(tockenFile.size() == 5 && tockenFile[0] == '*')
	{
		while (in)
		{
			in.read(buffer, sizeof(DownloadingFile));
			if (4 == getLargestCommonSubstring(tockenFile, foundFile.m_fileName))
			{
				showFoundFile(foundFile);
			}
		}
	}

	else if(tockenFile.size() == 2 && tockenFile[1] == '*')
	{
		while (in)
		{
			in.read(buffer, sizeof(DownloadingFile));
			if (tockenFile[0] == foundFile.m_fileName[0])
			{
				showFoundFile(foundFile);
			}
		}
	}

	else if(tockenFile.size() == 3 && tockenFile[1] == '*')
	{
		while (in)
		{
			in.read(buffer, sizeof(DownloadingFile));
			if (tockenFile[0] == foundFile.m_fileName[0] && tockenFile[2] == foundFile.m_fileName[strlen(foundFile.m_fileName)-5])
			{
				showFoundFile(foundFile);
			}
		}
	}

	else
	{
		while (in)
		{
			in.read(buffer, sizeof(DownloadingFile));
			if (4 <= getLargestCommonSubstring(tockenFile, foundFile.m_fileDescription) ||
				4 <= getLargestCommonSubstring(tockenFile, foundFile.m_fileName))
			{
				showFoundFile(foundFile);
			}
			
		}
	}
	
	m_mutexUserInteface.unlock();
	m_mutexDistribution.unlock();
}

int Client::getLargestCommonSubstring(/*std::string & result,*/ const std::string & a, const std::string & b)
{
	const int a_size = a.size();
	const int b_size = b.size();

	typedef std::vector<int> solution;

	const int solution_size = b_size + 1;
	solution x(solution_size, 0), y(solution_size);

	solution * previous = &x;
	solution * current = &y;

	int max_length = 0;
	int result_index = 0;

	for (int i = a_size - 1; i >= 0; i--) 
	{
		for (int j = b_size - 1; j >= 0; j--) 
		{
			int & current_match = (*current)[j];

			if (a[i] != b[j]) 
			{
				current_match = 0;
			}
			else {
				const int length = 1 + (*previous)[j + 1];
				if (length > max_length) 
				{
					max_length = length;
					result_index = i;
				}
				current_match = length;
			}
		}
		swap(previous, current);
	}
	//result = a.substr(result_index, max_length);
	return max_length;
}