#pragma once
#include <iostream>
#include <string>
#include "FileStatus.h"

struct DownloadingFile
{
	std::string m_fileHash;
	std::string m_fileName;
	std::string m_fileDescription;
	std::string m_fileLocation;
	int m_numberParts;
	int m_fileSize;
	FileStatus m_fileStatus;
};