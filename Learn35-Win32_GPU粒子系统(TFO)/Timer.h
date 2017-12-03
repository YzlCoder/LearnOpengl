#pragma once

#include <Windows.h>

class Timer
{
public:
	HANDLE mCurrentThread;
	LARGE_INTEGER mStart;
	LARGE_INTEGER mEnd;
	LARGE_INTEGER mCPUFrequency;
	DWORD_PTR mProcessAffinityMask;
	Timer();

	void Start();
	int GetPssedTicks();
	float GetPassTime();
};

