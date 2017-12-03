#include "Timer.h"



Timer::Timer()
{
	mCurrentThread = GetCurrentThread();
	DWORD_PTR systemAffinityMask;
	GetProcessAffinityMask(GetCurrentProcess(), &mProcessAffinityMask, &systemAffinityMask);
	SetThreadAffinityMask(mCurrentThread, 1);
	QueryPerformanceFrequency(&mCPUFrequency);
	SetThreadAffinityMask(mCurrentThread, mProcessAffinityMask);
}



void Timer::Start()
{
	SetThreadAffinityMask(mCurrentThread, 1);
	QueryPerformanceCounter(&mStart);
	SetThreadAffinityMask(mCurrentThread, mProcessAffinityMask);
}

int Timer::GetPssedTicks()
{
	SetThreadAffinityMask(mCurrentThread, 1);
	QueryPerformanceCounter(&mEnd);
	SetThreadAffinityMask(mCurrentThread, mProcessAffinityMask);
	return mEnd.QuadPart - mStart.QuadPart;
}

float Timer::GetPassTime()
{
	return (float)GetPssedTicks() / mCPUFrequency.QuadPart;
}