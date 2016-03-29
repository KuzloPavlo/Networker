#pragma once
#include <iostream>
#include <string>

struct PartFile
{
	std::string m_fileHash;
	int m_partNumber;
	std::string m_partHash;
	int m_partSize;
	const char* m_part[3072];
};