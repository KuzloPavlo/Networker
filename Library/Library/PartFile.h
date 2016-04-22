#pragma once
#include <iostream>
#include <string>

struct PartInfo
{
	long int m_fileHash;
	int m_partNumber;
};

struct PartFile
{
	int m_partNumber;
	int m_partHash;
	int m_partSize;
	char m_part[2048];
};

int calculatePartHash(const PartFile& partFile);
