#include "Downloader.h"

Downloader::Downloader(
	boost::asio::io_service& io_service,
	const DownloadingFile& downloadingFile,
	const FileDistributors& adresses,
	std::function<void(const FileStatus& fileStatus, const int& filePercents)>& changeFileStatus,
	std::function<void(const FileStatus& fileStatus)>& changeDownloader,
	Synchronization primitives,
	FileStatus* fileStatus,
	std::function<void(const std::string& str)>display,
	bool creating)
	: m_downloadingFile(downloadingFile),
	m_distributors(adresses),
	changeFileStatus(changeFileStatus),
	m_checkerParts(downloadingFile.m_fileInfo, creating),
	m_io_service(io_service),
	m_primitives(primitives),
	m_myStatus(fileStatus),
	m_donnePercent(0)
{
	this->display = display;
	//dosmth();
	display("Downloader::Downloader Start");
	display(m_downloadingFile.m_fileInfo.m_fileName);
	display(m_downloadingFile.m_fileInfo.m_fileDescription);
	display(std::to_string(m_downloadingFile.m_fileInfo.m_fileHash));
	display(m_downloadingFile.m_fileLocation);
	display(m_distributors[0].to_string());

	changeDownloader = std::bind(&Downloader::changeDownloader, this, std::placeholders::_1);
	this->changeFileStatus(downloadingFile.m_fileStatus, 0);

	std::thread sessionsThread(&Downloader::start, this, creating);
	sessionsThread.detach();
	
	work();
}

Downloader::~Downloader()
{
}
//
//void Downloader::dosmth()
//{
//
//
//}
void Downloader::changeDownloader(const FileStatus& fileStatus)
{
}

void Downloader::start(bool creating)
{
	std::string allLocation = m_downloadingFile.m_fileLocation;
	allLocation += "\\";
	allLocation += m_downloadingFile.m_fileInfo.m_fileName;

	if (creating)
	{
		changeFileStatus(FileStatus::creating, 0);
		if (!createEmptyFile(allLocation, m_downloadingFile.m_fileInfo.m_fileSize))
		{
			changeFileStatus(FileStatus::failing, 0);
			throw std::exception("Downloader::start cannot create an empty file");	
		}
		changeFileStatus(FileStatus::downloading, 0);
		display("Downloader::start created an empty file");
	}

	for (int i = 0; i < m_distributors.size() && 10; i++)
	{
		SessionStatus newStatus;
		newStatus.m_work = StatusValue::work;
		newStatus.m_sessionNumber = i;
		newStatus.m_fileHash = m_downloadingFile.m_fileInfo.m_fileHash;
		newStatus.m_part = m_checkerParts.getPart();

		std::list<SessionStatus>::iterator p = m_statusHolder.insert(m_statusHolder.end(), newStatus);

		std::shared_ptr<DownloadSession>newSession(new DownloadSession
			(
			m_distributors[i],
			m_io_service,
			m_primitives,
			&(*p),
			allLocation,
			display
			));

		m_sessions.insert(std::pair<int, std::shared_ptr<DownloadSession>>(i, newSession));
	}
	m_io_service.run();
}

void Downloader::work()
{
	while (true)
	{
		std::list<SessionStatus>::iterator p = m_statusHolder.begin();
		//readMyStatus(); необходимо сделать

		m_primitives.m_goWrite->lock();
		m_primitives.m_Shared->lock();

		while (p != m_statusHolder.end())
		{
			if (!readSessioStatus(&(*p)))
			{
				m_statusHolder.erase(p);
				//			m_mutexStatus->unlock();
				//	work();
				return;
			}
			p++;
		}

		m_primitives.m_Shared->unlock_one();
		m_primitives.m_goRead->unlock_one();
	}
}

bool Downloader::readSessioStatus(SessionStatus* status)
{
	switch (status->m_work)
	{
	case StatusValue::work:
		break;

	case StatusValue::stay:
		if (status->m_doit == StatusValue::set)
		{
			int percent = m_checkerParts.setPart(status->m_part);

			if (m_donnePercent < percent)
			{
				m_donnePercent = percent;
				changeFileStatus(FileStatus::failing, m_donnePercent);
			}

			status->m_part = m_checkerParts.getPart();
			status->m_work = StatusValue::work;
		}
		else if (status->m_doit == StatusValue::unset)
		{
			m_checkerParts.unsetPart(status->m_part);
			status->m_part = m_checkerParts.getPart();
			status->m_work = StatusValue::work;
		}
		break;

	case StatusValue::end:
		//deleteSession(status->m_sessionNumber); // 
		return false;

	default:
		//deleteSession(status->m_sessionNumber);
		return false;
	}
	return true;
}

void Downloader::readMyStatus()
{
	FileStatus myStatus;


	myStatus = *m_myStatus;


	switch (myStatus)
	{
	case FileStatus::downloading:
		break;

	case FileStatus::pause:
		//readMyStatus();
		break;

	case FileStatus::deleting:
		// сделай удаление
		break;
	}
}

void Downloader::deleteSession(int number)
{
	std::map<int, std::shared_ptr<DownloadSession>>::iterator q = m_sessions.begin();
	q = m_sessions.find(number);

	if (q != m_sessions.end())
	{
		m_sessions.erase(q);
	}
}

bool Downloader::createEmptyFile(const std::string& location, int sizeFile)
{
	enum
	{
		emptyLength = 32768 //
	};

	std::ofstream out(location, std::ios::out | std::ios::binary);
	if (!out)
	{
		display(std::to_string(GetLastError()));
		return false;
	}

	char emptyBuff[emptyLength] = { 0 };
	int nIteration = sizeFile / emptyLength;

	for (int i = 0; i < nIteration; i++)
	{
		out.write(emptyBuff, emptyLength);
		out.flush();
		sizeFile -= emptyLength;
	}

	out.write(emptyBuff, sizeFile);
	out.flush();

	return true;
}