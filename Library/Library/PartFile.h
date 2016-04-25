#pragma once
#include <iostream>
#include <string>
#include "ReturnValues.h"

struct PartInfo
{
	long int m_fileHash;
	int m_partNumber;
};

#define PARTSIZE 2048

struct PartFile
{	
	ReturnValues m_values;
	int m_partNumber;
	int m_partHash;
	int m_partSize;
	char m_part[PARTSIZE];
};

int calculatePartHash(const PartFile& partFile);
