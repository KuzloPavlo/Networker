#pragma once

enum FileStatus
{
	creating,
	downloading,
	verification,
	distribution,
	downloaded,
	pause,
	deleting,
	failing,
};