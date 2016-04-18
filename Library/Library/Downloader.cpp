#include "Downloader.h"

Downloader::Downloader(
	boost::asio::io_service& io_service,
	const DownloadingFile& downloadingFile,
	const FileDistributors& adresses,
	std::shared_ptr<std::mutex> mutexListParts,
	std::function<void(const FileStatus& fileStatus, const int& filePercents)>&changeFileStatus,
	std::function<void(const FileStatus& fileStatus)>* changeDownloader)
	: m_downloadingFile(downloadingFile), m_distributors(adresses), changeFileStatus(changeFileStatus)
{
	*changeDownloader = std::bind(&Downloader::changeDownloader, this, std::placeholders::_1);
	this->changeFileStatus(downloadingFile.m_fileStatus, 0);
}

Downloader::~Downloader()
{
}

void Downloader::changeDownloader(const FileStatus& fileStatus)
{

}
