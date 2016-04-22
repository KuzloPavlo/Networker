#pragma once

enum StatusValue
{
	work,
	stay,
	end,
	set,
	unset,
};

struct SessionStatus
{
	StatusValue m_work; // work/stay/end
	StatusValue m_doit; // set/unset
	int m_part;
	int m_sessionNumber;
	long int m_fileHash;
};