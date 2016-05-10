#include "Client.h"

Client::Client() : m_countConnectedClients(0), m_clientWorking(true), m_mutexOutgoingDistribution(new std::mutex)
{
	std::thread listenThread(&Client::threadListen, this);
	listenThread.detach();
}

Client::~Client()
{
	/*m_mutexUserInteface.lock();
	this->m_clientWorking = false;
	m_mutexUserInteface.unlock();*/
}

void Client::threadListen()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	try
	{
		boost::asio::io_service io_service;
		Listener listener(io_service, 77777, this->m_mutexOutgoingDistribution);
		listener.display = this->display;

		display("Listener Thread Started");

		io_service.run();
	}
	catch (const std::exception& ex)
	{
		display("Listener Thread Not Started");
		display(ex.what());
	}
}

void Client::downloadFile(
	const DownloadingFile& downloadingFile,
	CHANGEFILESTATUS changeFileStatus,
	CHANGEDOWNLOADER changeDownloader,
	Synchronization primitives,
	FileStatus* fileStatus)
{
	display("Client::downloadFile Start");
	FileDistributors adresses = getDistributors(downloadingFile.m_fileInfo);
	std::thread downloadThread(&Client::threadDownload, this, downloadingFile, adresses, changeFileStatus, changeDownloader, primitives, fileStatus);
	display("Client::downloadFile Working");
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
	try
	{
		boost::asio::io_service io_service;
		display("Client::threadDownload Start");
		display(downloadingFile.m_fileInfo.m_fileName);
		display(downloadingFile.m_fileInfo.m_fileDescription);
		display(std::to_string(downloadingFile.m_fileInfo.m_fileHash));
		display(std::to_string(downloadingFile.m_fileInfo.m_fileSize));
		display(std::to_string(downloadingFile.m_fileInfo.m_numberParts));
		display(downloadingFile.m_fileLocation);

		Downloader downloader(io_service, downloadingFile, adresses, changeFileStatus, changeDownloader, primitives, fileStatus, display);
		display("Client::threadDownload Finish");
	}
	catch (const std::exception& ex)
	{
		changeFileStatus(FileStatus::failing, 100);
		display("Client::threadDownload Not Work, Failing.");
		display(ex.what());
	}
}

void Client::threadServer(const std::string& IPaddress, const std::string& port)
{
	while (true)
	{
		display("Client::threadServer Started");
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
			display("Client::threadServer Failing");
			display(e.what());
		}
		std::this_thread::sleep_for(std::chrono::minutes(1));
	}
}

void Client::connectToServer(const std::string& IPaddress, const std::string& port)
{
	display("Client::connectToServer Start");
	std::thread serverThread(&Client::threadServer, this, IPaddress, port);
	serverThread.detach();
	display("Client::connectToServer Working");
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
	//	std::hash<std::string> hashFunction;

		DownloadingFile newFile;
		strcpy_s(newFile.m_fileInfo.m_fileDescription, description.c_str());
		strcpy_s(newFile.m_fileLocation, location.c_str());
		newFile.m_fileStatus = FileStatus::creating;

		changeFileStatus(newFile.m_fileStatus, 0);

		std::string fileName;
		fileName.assign(location, location.rfind("\\") + 1, location.size());

		strcpy_s(newFile.m_fileInfo.m_fileName, fileName.c_str());

		newFile.m_fileInfo.m_fileHash = newFile.m_fileInfo.m_fileHash ^ calculateNameHash(newFile.m_fileInfo.m_fileName); //(newFile.m_fileInfo.m_fileHash) ^ hashFunction(tempName);

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
			newFile.m_fileInfo.m_fileHash = (1 / 256) * (newFile.m_fileInfo.m_fileHash << 1) ^ calculatePartHash(filePart);

			std::fill(filePart, filePart + PARTSIZE, 0);

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
		throw std::exception("Client::sendOutgoingDistribution. Oops cannot open repository file");
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
	display("Client::sendOutgoingDistribution Distribution Sended");
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
//	m_mutexUserInteface.lock();
	display("Client::threadSearchFile");
	//m_mutexUserInteface.unlock();

	//m_mutexUserInteface.lock(); //!!
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
//	m_mutexUserInteface.unlock();
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
	return max_length;
}

void Client::addDistributeFile(const DistributeFile& distributeFile)
{
	display("Client::addDistributeFile Start");

	m_mutexDistribution.lock();

	std::map<FileInfo, FileDistributors>::iterator p;

	p = m_distirbution.find(distributeFile.m_fileInfo);
	if (p != m_distirbution.end())
	{
		bool i = p->second.addAdress(distributeFile.m_address);

		if (i)
		{
			display("Addres added");
		}
		else
		{
			display("Addres not added. This address is already there.");
		}
	}
	else
	{
		FileDistributors addres;
		addres.addAdress(distributeFile.m_address);
		m_distirbution.insert(std::pair<FileInfo, FileDistributors>(distributeFile.m_fileInfo, addres));
		display("Client::addDistributeFile added a New file");
	}
	m_mutexDistribution.unlock();
}

FileDistributors Client::getDistributors(const FileInfo& fileInfo)
{
	std::map<FileInfo, FileDistributors>::iterator p;

	p = m_distirbution.find(fileInfo);

	return (p->second);
}