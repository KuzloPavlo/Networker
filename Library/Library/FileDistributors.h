#pragma once
#include <iostream>
#include <vector>
#include <Windows.h>

class FileDistributors
{
public:
	FileDistributors();
	~FileDistributors();
	void addAdress(const sockaddr_in& adress);
	std::vector<sockaddr_in> getAdresses();
private:
	std::vector<sockaddr_in> m_adresses;
};

