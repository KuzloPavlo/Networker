#pragma once
#include "DownloadingFile.h"

struct DistributeFile
{
	FileInfo m_fileInfo;
	sockaddr_in m_addr;
};