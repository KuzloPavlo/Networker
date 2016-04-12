#pragma once
#include <iostream>
#include <string>

struct PartInfo
{
	int m_fileHash;
	int m_partNumber;
};

struct PartFile
{
	PartInfo m_partInfo;
	int m_partHash;
	int m_partSize;
	char m_part[2048];
};