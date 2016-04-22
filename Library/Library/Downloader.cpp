#include "Downloader.h"

Downloader::Downloader(
	boost::asio::io_service& io_service,
	const DownloadingFile& downloadingFile,
	const FileDistributors& adresses,
	std::function<void(const FileStatus& fileStatus, const int& filePercents)>& changeFileStatus,
	std::function<void(const FileStatus& fileStatus)>& changeDownloader,
	std::shared_ptr<std::mutex>mutexStatus,
	FileStatus* fileStatus,
	bool creating)
	: m_downloadingFile(downloadingFile),
	m_distributors(adresses),
	changeFileStatus(changeFileStatus),
	m_checkerParts(downloadingFile.m_fileInfo, creating),
	m_io_service(io_service),
	m_mutexStatus(mutexStatus),
	m_myStatus(fileStatus)
{
	changeDownloader = std::bind(&Downloader::changeDownloader, this, std::placeholders::_1);
	this->changeFileStatus(downloadingFile.m_fileStatus, 0);

	start();
}

Downloader::~Downloader()
{
}

void Downloader::changeDownloader(const FileStatus& fileStatus)
{

}

void Downloader::start()
{
	for (int i = 0; i < m_distributors.size() && 10; i++)
	{
		SessionStatus newStatus;
		newStatus.m_work = StatusValue::work;
		newStatus.m_sessionNumber = i;
		newStatus.m_fileHash = m_downloadingFile.m_fileInfo.m_fileHash;
		newStatus.m_part = m_checkerParts.getPart();

		m_statusHolder.push_back(newStatus);
		std::list<SessionStatus>::iterator p = m_statusHolder.begin();

		while (p != m_statusHolder.end())
		{
			if (p->m_sessionNumber == i)
			{
				DownloadSession newSession
					(
					m_distributors[i],
					m_io_service,
					m_mutexStatus,
					&(*p)
					);
				break;
			}
			p++;
		}
	}
}