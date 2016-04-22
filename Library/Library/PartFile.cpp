#include "PartFile.h"

int calculatePartHash(const PartFile& partFile)
{
	std::hash<std::string> hashFunction;
	return hashFunction(partFile.m_part);
}