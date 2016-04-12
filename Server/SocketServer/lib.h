#pragma once
#include <iostream>
#include <string>

enum FileStatus
{
	creating,
	downloading,
	distribution,
	downloaded,
};
struct DownloadingFile
{
	long int m_fileHash = 0;
	char m_fileName[MAX_PATH];
	char m_fileDescription[4096];
	char m_fileLocation[MAX_PATH];
	int m_numberParts = 0;
	int m_fileSize = 0;
	FileStatus m_fileStatus;
};
enum MessageTypes
{
	message,
	outgoingDistribution,
};

struct DistributFile
{
	DownloadingFile dwFile;
	sockaddr_in addr;
};
