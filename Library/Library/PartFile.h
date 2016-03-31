#pragma once
#include <iostream>
#include <string>

struct PartFile
{
	int m_fileHash;
	int m_partNumber;
	int m_partHash;
	int m_partSize;
	char m_part[2048];
};