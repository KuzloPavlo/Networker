#pragma once


enum StatusValue
{
	work,
	stay,
	end,
	set,
	unset,
	notOpen,
	notConnect,
	notWrite,
	notRead,
	notSave,
	notDistribution,
	done,
};

struct SessionStatus
{
	StatusValue m_work; // work/stay/end
	StatusValue m_doit; // set/unset
	StatusValue m_why; // notOpen/notConnect/notWrite/notRead/notSave/notDistribution/done
	int m_part;
	int m_sessionNumber;
	long int m_fileHash;
};