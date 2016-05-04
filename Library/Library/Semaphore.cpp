#include "Semaphore.h"

Semaphore::Semaphore(
	LPSECURITY_ATTRIBUTES lpSernaphoreAttribute,
	LONG linitialCount,
	LONG lMaxirnumCount,
	LPCTSTR lpNarne)
{
	m_semaphore = CreateSemaphore(lpSernaphoreAttribute, linitialCount, lMaxirnumCount, lpNarne);
}

Semaphore::~Semaphore()
{
}

void Semaphore::lock()
{
	WaitForSingleObject(m_semaphore, INFINITE);
}

void Semaphore::unlock_one()
{
	ReleaseSemaphore(m_semaphore, 1, NULL);
}