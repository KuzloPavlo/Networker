#pragma once
#include <iostream>
#include <mutex>
class Mutex_Guard
{
	std::mutex mtx;
public:
	Mutex_Guard()
	{
		std::cout << "Mutex_Guard" << std::endl;
		mtx.lock();
	}
	virtual ~Mutex_Guard() {
		mtx.unlock();
		std::cout << "~Mutex_Guard" << std::endl;
	}
};

