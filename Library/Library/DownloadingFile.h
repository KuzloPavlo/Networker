#pragma once
#include <iostream>
#include <string>
#include "FileStatus.h"
#include <Windows.h>

struct FileInfo
{
	long int m_fileHash = 0;
	char m_fileName[MAX_PATH];
	char m_fileDescription[4096];
	int m_numberParts = 0;         //
	int m_fileSize = 0;
};

struct DownloadingFile
{
	FileInfo m_fileInfo;
	char m_fileLocation[MAX_PATH];
	FileStatus m_fileStatus;
	int m_counterPercents = 0;
};