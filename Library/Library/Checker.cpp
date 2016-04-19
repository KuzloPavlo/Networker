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

	}
}


Checker::~Checker()
{
}
