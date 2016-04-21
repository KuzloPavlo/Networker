#include "FileDistributors.h"


FileDistributors::FileDistributors()
{
}


FileDistributors::~FileDistributors()
{
}

bool FileDistributors::addAdress(const boost::asio::ip::address& adress)
{	
	std::vector<boost::asio::ip::address>::iterator p = m_adresses.begin();
	while (p != m_adresses.end())
	{
		if (adress == (*p))
		{
			return false;
		}
		p++;
	}
	m_adresses.push_back(adress);
	return true;
}

boost::asio::ip::address& FileDistributors::operator [](const int& i)
{
	return m_adresses[i];
}