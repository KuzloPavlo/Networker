#include "Checker.h"

Checker::Checker(const FileInfo& dataFile, bool creating)
{
	m_fileParts.open(std::to_string(m_fileInfo.m_fileHash), std::ios::in | std::ios::out | std::ios::binary);
	if (!m_fileParts.is_open())
	{
		// обработай
		return;
	}

	std::vector<bool>::iterator p = m_listParts.begin();
	m_listParts.insert(p, m_fileInfo.m_numberParts, false);

	if (creating)
	{
		unsigned int zero = 0;

		int nIteration = ((m_fileInfo.m_numberParts / 8) / sizeof(unsigned int)) + 1;

		for (int i = 0; i < nIteration; i++)
		{
			m_fileParts << zero;
		}
		m_fileParts.flush();
	}
	else
	{
		getListParts();
	}
}


void Checker::getListParts()
{
	std::vector<bool>::iterator p = m_listParts.begin();

	while (p != m_listParts.end() && m_fileParts)
	{
		unsigned char Byte = 0;

		m_fileParts >> Byte;

		unsigned char mask = 1;

		while (mask && p != m_listParts.end())
		{
			*p = mask & Byte;

			mask <<= 1;
			p++;
		}
	}
}

Checker::~Checker()
{
}
