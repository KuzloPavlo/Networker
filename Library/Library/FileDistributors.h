#pragma once
#include <iostream>
#include <vector>
#include <boost\asio.hpp>
#include <utility>
using boost::asio::ip::tcp;

class FileDistributors
{
public:
	FileDistributors();
	~FileDistributors();
	bool addAdress(const boost::asio::ip::address& adress);
	std::vector<boost::asio::ip::address> getAdresses();
	boost::asio::ip::address& operator [](const int& i);
	int size();
private:
	std::vector<boost::asio::ip::address> m_adresses;
};

