#pragma once
#include <Windows.h>

class Semaphore
{
public:
	Semaphore(
		LPSECURITY_ATTRIBUTES lpSernaphoreAttribute,
		LONG linitialCount,
		LONG lMaxirnumCount,
		LPCTSTR lpNarne);
	~Semaphore();
	void lock();
	void unlock_one();

private:
	HANDLE m_semaphore;
};