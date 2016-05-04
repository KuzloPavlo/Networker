#pragma once
#include <iostream>
#include <mutex>
#include <memory>
#include <vector>
#include "Semaphore.h"

struct Synchronization
{
	std::shared_ptr<int> m_counter;
	std::shared_ptr<std::mutex>m_mutexCounter;
	std::shared_ptr<Semaphore> m_goRead;
	std::shared_ptr<Semaphore> m_goWrite;
	std::shared_ptr<Semaphore> m_Shared;
};