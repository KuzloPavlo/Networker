#include "FileDistributors.h"


FileDistributors::FileDistributors()
{
}


FileDistributors::~FileDistributors()
{
}

void FileDistributors::addAdress(const boost::asio::ip::address& adress)
{	
	std::vector<boost::asio::ip::address>::iterator p = m_adresses.begin();
	while (p != m_adresses.end())
	{
		if (adress == (*p))
		{
			return;
		}
		p++;
	}
	m_adresses.push_back(adress);
}

boost::asio::ip::address& FileDistributors::operator [](const int& i)
{
	return m_adresses[i];
}