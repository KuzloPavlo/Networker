#include "Client.h"

Client::Client() : m_countConnectedClients(0), m_clientWorking(true), m_mutexOutgoingDistribution(new std::mutex)
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
		display("Listener Thread Started1");
		boost::asio::io_service io_service;
		//std::shared_ptr<Listener> listener(new Listener(io_service, 77777, this->m_mutexOutgoingDistribution));
		//this->m_Listener = listener;
		display("Listener Thread Started2");
		Listener listener(io_service, 77777, this->m_mutexOutgoingDistribution);
		display("Listener Thread Started3");
		listener.display = this->display;

		m_mutexUserInteface.lock();
		display("Listener Thread Started");
		m_mutexUserInteface.unlock();
		io_service.run();
		display("Listener Thread Started4");
	}
	catch (const std::exception& ex)
	{
		m_mutexUserInteface.lock();
		display("Listener Thread Not Started");
		display(ex.what());
		m_mutexUserInteface.unlock();
	}
}

void Client::downloadFile(
	const DownloadingFile& downloadingFile,
	CHANGEFILESTATUS changeFileStatus,
	CHANGEDOWNLOADER changeDownloader,
	Synchronization primitives,
	FileStatus* fileStatus)
{
	display("Client::downloadFile1 ");
	FileDistributors adresses = getDistributors(downloadingFile.m_fileInfo);
	std::thread downloadThread(&Client::threadDownload, this, downloadingFile, adresses, changeFileStatus, changeDownloader, primitives, fileStatus);
	display("Client::downloadFile2 ");
	downloadThread.detach();
}

void Client::threadDownload(
	const DownloadingFile& downloadingFile,
	const FileDistributors& adresses,
	CHANGEFILESTATUS changeFileStatus,
	CHANGEDOWNLOADER changeDownloader,
	Synchronization primitives,
	FileStatus* fileStatus)
{
	display("Client::threadDownload1111");
	//std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	display("Client::threadDownload2");
	try
	{
		boost::asio::io_service io_service;
		display("Client::threadDownload2/1");
		//-----------------------------------
		display("");
		display(downloadingFile.m_fileInfo.m_fileName);
		display(downloadingFile.m_fileInfo.m_fileDescription);
		display(std::to_string(downloadingFile.m_fileInfo.m_fileHash));
		display(std::to_string(downloadingFile.m_fileInfo.m_fileSize));
		display(std::to_string(downloadingFile.m_fileInfo.m_numberParts));
		display("");
		display(downloadingFile.m_fileLocation);
		display("");

		//----------------------------------
		//io_service.run();
		//std::shared_ptr<Downloader> downloader(new Downloader(io_service, downloadingFile, adresses/*, this->m_mutexListParts*/, changeFileStatus, changeDownloader,primitives,fileStatus, display));
		Downloader downloader(io_service, downloadingFile, adresses/*, this->m_mutexListParts*/, changeFileStatus, changeDownloader, primitives, fileStatus, display);
		//-----------------------
		display("Client::threadDownload3");
		//downloader->func(this->display);
		downloader.dosmth();
		//----------------------------	
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
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	while (true)
	{
		display("Server Thread Started");
		display(IPaddress);
		display(port);
		try
		{
			boost::asio::io_service io_service;
			tcp::socket serverSocket(io_service);
			tcp::resolver resolver(io_service);
			boost::asio::connect(serverSocket, resolver.resolve({ IPaddress.c_str(), port.c_str() }));

			reciveDistribution(&serverSocket);
			sendOutgoingDistribution(&serverSocket);

			serverSocket.close();
		}
		catch (std::exception& e)
		{
			display("Client::threadServer Not Start");
			display(e.what());
		}
		std::this_thread::sleep_for(std::chrono::minutes(1));
	}
}

void Client::threadClient(void *arg)
{

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

void Client::createNewDownloadingFile(std::string location, std::string description, ADDNEWFILE addNewFile, CHANGEFILESTATUS changeFileStatus)
{
	std::thread createingFile(&Client::threadCreateDownloadingFile, this, location, description, addNewFile, changeFileStatus);
	createingFile.detach();
}

void Client::threadCreateDownloadingFile(std::string location, std::string description, ADDNEWFILE addNewFile, CHANGEFILESTATUS changeFileStatus)
{
	try
	{
		display("Client::threadCreateDownloadingFile Started");

		char filePart[PARTSIZE] = { 0 };
		std::hash<std::string> hashFunction;

		DownloadingFile newFile;
		strcpy_s(newFile.m_fileInfo.m_fileDescription, description.c_str());
		strcpy_s(newFile.m_fileLocation, location.c_str());
		newFile.m_fileStatus = FileStatus::creating;

		changeFileStatus(newFile.m_fileStatus, 0);

		std::string tempName;
		tempName.assign(location, location.rfind("\\") + 1, location.size());

		strcpy_s(newFile.m_fileInfo.m_fileName, tempName.c_str());

		newFile.m_fileInfo.m_fileHash = (newFile.m_fileInfo.m_fileHash) ^ hashFunction(tempName);

		std::ifstream in(location, std::ios::in | std::ios::binary);
		if (!in)
		{
			newFile.m_fileStatus = FileStatus::failing;
			changeFileStatus(newFile.m_fileStatus, 0);
			throw std::exception("Client::threadCreateDownloadingFile. Not open file for creating!");
		}

		int inSize = 0;

		in.seekg(0, in.end);
		inSize = in.tellg();
		in.seekg(0, in.beg);

		int percent = 0;

		do
		{
			in.read(filePart, PARTSIZE);
			newFile.m_fileInfo.m_numberParts++;
			newFile.m_fileInfo.m_fileSize += in.gcount();
			newFile.m_fileInfo.m_fileHash = (1 / 256) * (newFile.m_fileInfo.m_fileHash << 1) ^ hashFunction(filePart);

			if (((newFile.m_fileInfo.m_fileSize * 100) / inSize) > percent)
			{
				percent = (newFile.m_fileInfo.m_fileSize * 100) / inSize;
				changeFileStatus(newFile.m_fileStatus, percent);
			}

		} while (in);

		display(std::to_string(newFile.m_fileInfo.m_fileHash));
		display(newFile.m_fileInfo.m_fileName);
		display(newFile.m_fileInfo.m_fileDescription);
		display(std::to_string(newFile.m_fileInfo.m_numberParts));
		display(std::to_string(newFile.m_fileInfo.m_fileSize));
		display(newFile.m_fileLocation);

		std::ofstream out("OutgoingDistribution", std::ios::out | std::ios::app | std::ios::binary);
		if (!out)
		{
			newFile.m_fileStatus = FileStatus::failing;
			changeFileStatus(newFile.m_fileStatus, 0);
			throw std::exception("Client::threadCreateDownloadingFile. Oops cannot open repository file");
		}

		newFile.m_fileStatus = FileStatus::distribution;

		char* outBuffer = (char*)&newFile;
		out.write(outBuffer, sizeof(DownloadingFile));

		changeFileStatus(newFile.m_fileStatus, 100);

		out.close();

		addNewFile(newFile);
	}
	catch (const std::exception& ex)
	{
		changeFileStatus(FileStatus::failing, 100);
		display(ex.what());
	}

	//---------------------------------------------------------------------------+
	// under code only for displaing									         |	
	//---------------------------------------------------------------------------+

	try
	{
		int numberOutDistribution = 0;
		int fileSize = 0;

		std::ifstream inn("OutgoingDistribution", std::ios::in | std::ios::binary);
		if (!inn)
		{
			throw std::exception("Client::threadCreateDownloadingFile. Oops cannot open file for read");
		}

		inn.seekg(0, inn.end);
		fileSize = inn.tellg();
		inn.seekg(0, inn.beg);

		numberOutDistribution = fileSize / sizeof(DownloadingFile);

		display(std::to_string(numberOutDistribution));

		DownloadingFile dff;
		char* ptr = (char*)&dff;
		for (int i = 0; i < numberOutDistribution; i++)
		{
			inn.read(ptr, sizeof(DownloadingFile));

			display(std::to_string(dff.m_fileInfo.m_fileHash));
			display(dff.m_fileInfo.m_fileName);
			display(dff.m_fileInfo.m_fileDescription);
			display(std::to_string(dff.m_fileInfo.m_numberParts));
			display(std::to_string(dff.m_fileInfo.m_fileSize));
			display(dff.m_fileLocation);
		}
	}
	catch (const std::exception& ex)
	{
		display(ex.what());
	}
}

void Client::sendOutgoingDistribution(tcp::socket* serverSocket)
{
	DownloadingFile downloadingFile;
	FileInfo sendFile;
	int sendLength = sizeof(FileInfo);
	int fileSize = 0;
	int numberOutDistribution = 0;
	char* buffer = nullptr;

	std::ifstream in("OutgoingDistribution", std::ios::in | std::ios::binary);
	if (!in)
	{
		// обработай
		return;
	}

	in.seekg(0, in.end);
	fileSize = in.tellg();
	in.seekg(0, in.beg);

	numberOutDistribution = fileSize / sizeof(DownloadingFile);

	buffer = (char*)& numberOutDistribution;

	boost::asio::write(*serverSocket, boost::asio::buffer(buffer, 4));

	display("Client::sendOutgoingDistribution numberDistribution:");
	display(std::to_string(numberOutDistribution));

	for (int i = 0; i < numberOutDistribution; i++)
	{
		buffer = (char*)& downloadingFile;

		in.read(buffer, sizeof(DownloadingFile));

		sendFile = downloadingFile.m_fileInfo;
		buffer = (char*)& sendFile;

		boost::asio::write(*serverSocket, boost::asio::buffer(buffer, sendLength));
		display("Client::sendOutgoingDistribution sended next file:");
		display(sendFile.m_fileName);
		display(sendFile.m_fileDescription);
		display(std::to_string(sendFile.m_fileHash));

	}
}

void Client::reciveDistribution(tcp::socket* serverSocket)
{
	DistributeFile distributeFile;
	int numberDistribution = 0;
	char* buffer = (char*)& numberDistribution;
	int receiveLength = sizeof(DistributeFile);

	boost::asio::read(*serverSocket,
		boost::asio::buffer(buffer, 4));

	display("Client::reciveDistribution numberDistribution:");
	display(std::to_string(numberDistribution));

	buffer = (char*)& distributeFile;

	for (int i = 0; i < numberDistribution; i++)
	{
		boost::asio::read(*serverSocket, boost::asio::buffer(buffer, receiveLength));

		display("Client::reciveDistribution received next file:");
		display(distributeFile.m_fileInfo.m_fileName);
		display(distributeFile.m_fileInfo.m_fileDescription);
		std::string s = distributeFile.m_address.to_string();
		display(s);

		addDistributeFile(distributeFile);
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

	m_mutexUserInteface.lock(); //!!
	m_mutexDistribution.lock();

	std::map<FileInfo, FileDistributors>::iterator p = m_distirbution.begin();

	if (tockenFile == "*")
	{
		while (p != m_distirbution.end())
		{
			showFoundFile(p->first);
			p++;
		}
	}

	else if (tockenFile.size() == 5 && tockenFile[0] == '*')
	{
		while (p != m_distirbution.end())
		{
			if (4 == getLargestCommonSubstring(tockenFile, p->first.m_fileName))
			{
				showFoundFile(p->first);
			}
			p++;
		}
	}

	else if (tockenFile.size() == 2 && tockenFile[1] == '*')
	{
		while (p != m_distirbution.end())
		{
			if (tockenFile[0] == p->first.m_fileName[0])
			{
				showFoundFile(p->first);
			}
			p++;
		}
	}

	else if (tockenFile.size() == 3 && tockenFile[1] == '*')
	{
		while (p != m_distirbution.end())
		{
			if (tockenFile[0] == p->first.m_fileName[0] && tockenFile[2] == p->first.m_fileName[strlen(p->first.m_fileName) - 5])
			{
				showFoundFile(p->first);
			}
			p++;
		}
	}

	else
	{
		while (p != m_distirbution.end())
		{
			if (4 <= getLargestCommonSubstring(tockenFile, p->first.m_fileDescription) ||
				4 <= getLargestCommonSubstring(tockenFile, p->first.m_fileName))
			{
				showFoundFile(p->first);
			}
			p++;
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
//
//void Client::connnect()
//{
//	enum { max_length = 1024 };
//
//	try
//	{
//		boost::asio::io_service io_service;
//
//		tcp::socket s(io_service);
//		tcp::resolver resolver(io_service);
//		boost::asio::connect(s, resolver.resolve({ "127.0.0.1", "77777" }));
//
//		std::cout << "Enter message: ";
//		char request[max_length];
//		std::cin.getline(request, max_length);
//		size_t request_length = std::strlen(request);
//		boost::asio::write(s, boost::asio::buffer(request, request_length));
//
//		char reply[max_length];
//		size_t reply_length = boost::asio::read(s,
//			boost::asio::buffer(reply, request_length));
//		std::cout << "Reply is: ";
//		std::cout.write(reply, reply_length);
//		std::cout << "\n";
//	}
//	catch (std::exception& e)
//	{
//		std::cerr << "Exception: " << e.what() << "\n";
//	}
//}

void Client::addDistributeFile(const DistributeFile& distributeFile)
{
	//--------------------------------------
	m_mutexUserInteface.lock();
	display("Client::addDistributeFile");
	m_mutexUserInteface.unlock();
	//--------------------------------------

	m_mutexDistribution.lock();
	std::map<FileInfo, FileDistributors>::iterator p;

	p = m_distirbution.find(distributeFile.m_fileInfo);
	if (p != m_distirbution.end())
	{
		bool i = p->second.addAdress(distributeFile.m_address);

		//-------------------	--------------------
		if (i)
		{
			display("Addres dodano");
		}
		else
		{
			display("Addres NE dodano");
		}

		m_mutexUserInteface.lock();
		display("Client::addDistributeFile2");
		m_mutexUserInteface.unlock();
		//---------------------------------------
	}
	else
	{
		m_mutexUserInteface.lock();
		display("Client::addDistributeFile3");
		m_mutexUserInteface.unlock();

		FileDistributors addres;
		addres.addAdress(distributeFile.m_address);
		m_distirbution.insert(std::pair<FileInfo, FileDistributors>(distributeFile.m_fileInfo, addres));
	}
	m_mutexDistribution.unlock();
}

FileDistributors Client::getDistributors(const FileInfo& fileInfo)
{
	std::map<FileInfo, FileDistributors>::iterator p;

	p = m_distirbution.find(fileInfo);

	return (p->second);
}