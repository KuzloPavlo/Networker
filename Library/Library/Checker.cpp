#include "Checker.h"

Checker::Checker(const FileInfo& dataFile, bool creating)
{
	std::vector<bool>::iterator p = m_vectorParts.begin();
	m_vectorParts.insert(p, m_fileInfo.m_numberParts, false);

	if (creating)
	{
		createNewFileParts();
		openFileParts();
	}
	else
	{
		openFileParts();
		getVectorParts();
	}
}

void Checker::createNewFileParts()
{
	std::ofstream out(std::to_string(m_fileInfo.m_fileHash), std::ios::out | std::ios::binary);
	throw std::runtime_error(std::to_string(m_fileInfo.m_fileHash));
	if (!out)
	{
		throw std::runtime_error("std::ofstream out ne otkrilos");
		// обработай
		//return;
	}

	unsigned int zero = 0;
	int nIteration = ((m_fileInfo.m_numberParts / 8) / sizeof(unsigned int)) + 1;
	for (int i = 0; i < nIteration; i++)
	{
		out << zero;
	}
	out.flush();
}

void Checker::openFileParts()
{
	m_fileParts.open(std::to_string(m_fileInfo.m_fileHash), std::ios::in | std::ios::out | std::ios::binary);
	if (!m_fileParts)

	{
		throw std::runtime_error("m_fileParts.open ne otkrilos");
		// обработай
		//return;
	}
}

void Checker::getVectorParts()
{
	std::vector<bool>::iterator p = m_vectorParts.begin();

	while (p != m_vectorParts.end() && m_fileParts)
	{
		unsigned char Byte = 0;

		m_fileParts >> Byte;

		unsigned char mask = 128;

		while (mask && p != m_vectorParts.end())
		{
			*p = mask & Byte;

			mask >>= 1;
			p++;
		}
	}
}

int Checker::getPart()
{
	int npart = 0;

	for (int i = 0; i < m_vectorParts.size(); i++)
	{
		if (!m_vectorParts[i])
		{
			npart = i + 1;
			m_vectorParts[i] = true;
			break;
		}
	}
	return npart;
}

void Checker::unsetPart(const int& part)
{
	m_vectorParts[part - 1] = false;
}

void Checker::setPart(const int& part)
{
	int currentByte;
	unsigned char currentBit;
	unsigned char mask = 128;
	char Byte = 0;

	currentByte = (part / 8) + 1;
	currentBit = part - ((part / 8) * 8);

	mask >>= currentBit - 1;

	m_fileParts.seekg(currentByte - 1, std::ios::beg);
	m_fileParts.seekp(currentByte - 1, std::ios::beg);

	m_fileParts >> Byte;

	Byte = Byte | mask;

	m_fileParts << Byte;

	m_fileParts.flush();
}

Checker::~Checker()
{
}
