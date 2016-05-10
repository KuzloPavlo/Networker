#include "PartFile.h"

int calculatePartHash(const char partFile[PARTSIZE])
{
	std::hash<std::string> hashFunction;
	return hashFunction(partFile);
}


int calculateNameHash(std::string fileName)
{
	std::hash<std::string> hashFunction;
	return hashFunction(fileName);
}


