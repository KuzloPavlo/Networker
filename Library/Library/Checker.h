#pragma once
#include "DownloadingFile.h"
#include <vector>
#include <fstream>

class Checker
{
public:
	Checker(const FileInfo& dataFile, bool creating = true);
	~Checker();
	void setPart(const int& part);
	void unsetPart(const int& part);
	int getPart();
private:
	FileInfo m_fileInfo;
	std::vector<bool> m_listParts;
	std::fstream m_fileParts;
	int m_currentByte;
	unsigned char m_currentBit;
	int m_currentPart;
};