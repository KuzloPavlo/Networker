#pragma once
#include <mutex>
#include <memory>
class Shared_lock
{
	std::shared_ptr<std::mutex> m_mutex;
	bool m_lock;
public:
	Shared_lock(std::shared_ptr<std::mutex> mutex);
	~Shared_lock();
	void unlock();
};

