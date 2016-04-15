#include "Client.h"
#include <fstream>

using boost::asio::ip::tcp;

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
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	try
	{
		boost::asio::io_service io_service;
		std::shared_ptr<Listener> listener(new Listener(io_service, 77777, this->m_mutexOutgoingDistribution));
		this->m_Listener = listener;
		m_mutexUserInteface.lock();
		display("Listener Thread Started");
		m_mutexUserInteface.unlock();
		io_service.run();
	}
	catch (const std::exception& ex)
	{
		m_mutexUserInteface.lock();
		display("Listener Thread Not Started");
		m_mutexUserInteface.unlock();
	}
}

void Client::downloadFile(
	const int& fileHash,
	std::function<void(const int& fileHash,const FileStatus& fileStatus,const int& filePercents)>&changeFileStatus)
{
	DownloadingFile dw;
	std::thread downloadThread(&Client::threadDownload, this, dw, changeFileStatus);
	downloadThread.detach();
}

void Client::threadDownload(
	DownloadingFile& downloadingFile,
	std::function<void(const int& fileHash, const FileStatus& fileStatus, const int& filePercents)>&changeFileStatus)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));

	try
	{
		boost::asio::io_service io_service;

		std::shared_ptr<Downloader> downloader(new Downloader(io_service, downloadingFile, this->m_mutexListParts, changeFileStatus));

		downloader->func(this->display);
		downloader->dosmth();

		io_service.run();
	}
	catch (const std::exception& ex)
	{
		m_mutexUserInteface.lock();
		display("Client::threadDownloadNot ");
		display(ex.what());
		m_mutexUserInteface.unlock();
	}
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
	//SOCKET clientSocket = (SOCKET)arg;
	//char receiveBuffer[4096] = { 0 };
	//char sendBuffer[4096] = { 0 };
	//int receiveSize = 0;
	//int sendSize = 0;
	//int returnResult = 0;
	////display("thread client");
	//while (true)
	//{
	//	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	//	m_mutexUserInteface.lock();
	//	if (!this->m_clientWorking)
	//	{
	//		m_mutexUserInteface.unlock();
	//		break;
	//	}
	//	m_mutexUserInteface.unlock();

	//	returnResult = 0;
	//	returnResult = recv(clientSocket, receiveBuffer, 4096, 0);

	//	if (returnResult == 0)
	//	{
	//		// обеспечить обработку ошибки
	//		continue;
	//	}

	//	if (returnResult == SOCKET_ERROR)
	//	{
	//		// обеспечить обработку ошибки
	//		continue;
	//	}

	//	if (returnResult > (sizeof(char)* 2))
	//	{
	//		char* p = (char*)&receiveSize;
	//		*p = receiveBuffer[0];
	//		p++;
	//		*p = receiveBuffer[1];

	//		if (receiveSize == returnResult - (sizeof(char)* 2))
	//		{
	//			p++;
	//			readClient(p, receiveSize, sendBuffer);
	//		}

	//	}
	//}
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

	std::thread serverThread(&Client::threadServer, this, IPaddress, port);
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
	strcpy_s(newFile.m_fileInfo.m_fileDescription, description.c_str());
	strcpy_s(newFile.m_fileLocation, location.c_str());
	newFile.m_fileStatus = FileStatus::creating;

	std::string tempName;
	tempName.assign(location, location.rfind("\\") + 1, location.size());

	strcpy_s(newFile.m_fileInfo.m_fileName, tempName.c_str());

	newFile.m_fileInfo.m_fileHash = (newFile.m_fileInfo.m_fileHash) ^ hashFunction(tempName);

	std::ifstream in(location, std::ios::in | std::ios::binary);
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
		newFile.m_fileInfo.m_numberParts++;
		newFile.m_fileInfo.m_fileSize += in.gcount();
		newFile.m_fileInfo.m_fileHash = (1 / 256) * (newFile.m_fileInfo.m_fileHash << 1) ^ hashFunction(filePart);

	} while (!in.eof());

	display(location);
	display(tempName);
	display(std::to_string(newFile.m_fileInfo.m_numberParts));
	display(std::to_string(newFile.m_fileInfo.m_fileHash));

	std::ofstream out("OutgoingDistribution", std::ios::out | std::ios::app | std::ios::binary);
	if (!out)
	{
		display("client:: new file4");
		// обработать ошыбку
		return;
	}

	char* p = (char*)&newFile;
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
	FileInfo sendFile;
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

	for (int i = 0; i < numberOutDistribution; i++)
	{
		in.read(buffer, sizeof(DownloadingFile));
		sendFile = downloadingFile.m_fileInfo;
		buffer = (char*)& sendFile;
		iResult = send(*serverSocket, buffer, sizeof(FileInfo), 0);
		if (iResult == SOCKET_ERROR)
		{
			// ОБРАБОТАЙ
			return;
		}
	}
}

void Client::reciveDistribution(SOCKET *serverSocket)
{
	DistributeFile downloadingFile;
	int iResult = 0;
	int fileSize = 0;
	int numberDistribution = 0;
	char* buffer = (char*)& numberDistribution;

	//std::ofstream out("Distribution", std::ios::out | std::ios::binary);
	//if (!out)
	//{
	//	// обработай
	//	return;
	//}

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
		iResult = recv(*serverSocket, buffer, sizeof(DistributeFile), 0);
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
		display(downloadingFile.m_fileInfo.m_fileDescription);
		display(downloadingFile.m_fileInfo.m_fileName);
		m_mutexUserInteface.unlock();
		//----------------------------------------------------

		//out.write(buffer, sizeof(FileInfo));



	}
}

void Client::searchFile(const std::string& tockenFile)
{
	std::thread searchingFile(&Client::threadSearchFile, this, tockenFile);
	searchingFile.detach();
}

void Client::threadSearchFile(std::string tockenFile)
{
	m_mutexUserInteface.lock();
	display("Client::threadSearchFile");
	m_mutexUserInteface.unlock();
	FileInfo foundFile;
	char* buffer = (char*)& foundFile;

	//m_mutexDistribution->lock();

	std::ifstream in("Distribution", std::ios::in | std::ios::binary);
	if (!in)
	{
		//m_mutexDistribution->unlock();
		//
		return;
	}

	m_mutexUserInteface.lock();

	if (tockenFile == "*")
	{
		while (in)
		{
			in.read(buffer, sizeof(FileInfo));
			showFoundFile(foundFile);
		}
	}

	else if (tockenFile.size() == 5 && tockenFile[0] == '*')
	{
		while (in)
		{
			in.read(buffer, sizeof(FileInfo));
			if (4 == getLargestCommonSubstring(tockenFile, foundFile.m_fileName))
			{
				showFoundFile(foundFile);
			}
		}
	}

	else if (tockenFile.size() == 2 && tockenFile[1] == '*')
	{
		while (in)
		{
			in.read(buffer, sizeof(FileInfo));
			if (tockenFile[0] == foundFile.m_fileName[0])
			{
				showFoundFile(foundFile);
			}
		}
	}

	else if (tockenFile.size() == 3 && tockenFile[1] == '*')
	{
		while (in)
		{
			in.read(buffer, sizeof(FileInfo));
			if (tockenFile[0] == foundFile.m_fileName[0] && tockenFile[2] == foundFile.m_fileName[strlen(foundFile.m_fileName) - 5])
			{
				showFoundFile(foundFile);
			}
		}
	}

	else
	{
		while (in)
		{
			in.read(buffer, sizeof(FileInfo));
			if (4 <= getLargestCommonSubstring(tockenFile, foundFile.m_fileDescription) ||
				4 <= getLargestCommonSubstring(tockenFile, foundFile.m_fileName))
			{
				showFoundFile(foundFile);
			}

		}
	}

	m_mutexUserInteface.unlock();
	//m_mutexDistribution->unlock();
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

void Client::connnect()
{

	enum { max_length = 1024 };

	try
	{
		boost::asio::io_service io_service;

		tcp::socket s(io_service);
		tcp::resolver resolver(io_service);
		boost::asio::connect(s, resolver.resolve({ "127.0.0.1", "77777" }));

		std::cout << "Enter message: ";
		char request[max_length];
		std::cin.getline(request, max_length);
		size_t request_length = std::strlen(request);
		boost::asio::write(s, boost::asio::buffer(request, request_length));

		char reply[max_length];
		size_t reply_length = boost::asio::read(s,
			boost::asio::buffer(reply, request_length));
		std::cout << "Reply is: ";
		std::cout.write(reply, reply_length);
		std::cout << "\n";
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}


}

void Client::addDistributeFile(const DistributeFile& distributeFile)
{
	std::map<FileInfo, FileDistributors>::iterator p;

	p = m_distirbution.find(distributeFile.m_fileInfo);
	if (p != m_distirbution.end())
	{
		p->second.addAdress(distributeFile.m_addr);
	}
	else
	{
		FileDistributors addres;
		addres.addAdress(distributeFile.m_addr);
		m_distirbution.insert(std::pair<FileInfo, FileDistributors>(distributeFile.m_fileInfo, addres));
	}	
}