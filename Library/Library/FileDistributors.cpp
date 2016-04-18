#include "FileDistributors.h"


FileDistributors::FileDistributors()
{
}


FileDistributors::~FileDistributors()
{
}

void FileDistributors::addAdress(const sockaddr_in& adress)
{	
	std::vector<sockaddr_in>::iterator p = m_adresses.begin();
	while (p != m_adresses.end())
	{
		if (adress.sin_addr.S_un.S_addr == (*p).sin_addr.S_un.S_addr)
		{
			return;
		}
		p++;
	}
	m_adresses.push_back(adress);
}

sockaddr_in& FileDistributors::operator [](const int& i)
{
	return m_adresses[i];
}