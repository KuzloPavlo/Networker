#include "Shared_lock.h"


Shared_lock::Shared_lock(std::shared_ptr<std::mutex> mutex) : m_mutex(mutex)
{
	m_mutex->lock();
	m_lock = true;
}


Shared_lock::~Shared_lock()
{
	if (m_lock)
	{
		m_mutex->unlock();
	}
}

void Shared_lock::unlock()
{
	if (m_lock)
	{
		m_mutex->unlock();
		m_lock = false;
	}
}