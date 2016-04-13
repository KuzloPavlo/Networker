#include "Downloader.h"


Downloader::Downloader(
	boost::asio::io_service& io_service, 
	const DownloadingFile& downloadingFile, 
	std::shared_ptr<std::mutex> mutexListParts,
	std::function<void(const int& fileHash, const FileStatus& fileStatus, const int& filePercents)>&changeFileStatus) : m_downloadingFile(downloadingFile)
{
	
}


Downloader::~Downloader()
{
}
