#pragma once
#include <iostream>
#include <string>
#include "ReturnValues.h"

struct PartInfo
{
	long int m_fileHash = 0;
	int m_partNumber = 0;
};

#define PARTSIZE 2048

struct PartFile
{	
	ReturnValues m_values;
	int m_partNumber = 0;
	int m_partHash = 0;
	int m_partSize = 0;
	char m_part[PARTSIZE];
};

int calculatePartHash(const char partFile[PARTSIZE]);

int calculateNameHash(std::string fileName);


