/*
 *	Author : Minsuk Kang
 *	Date : 2009-07-02
 */
#pragma once

#include <process.h>
#include <windows.h>

class ACriticalSection
{
	CRITICAL_SECTION CriticalSection;

public:
	FORCEINLINE ACriticalSection(void)
	{
		InitializeCriticalSection(&CriticalSection);
		SetCriticalSectionSpinCount(&CriticalSection,4000);
	}

	FORCEINLINE ~ACriticalSection(void)
	{
		DeleteCriticalSection(&CriticalSection);
	}

	FORCEINLINE void Lock(void)
	{
		if( TryEnterCriticalSection(&CriticalSection) == 0 )
		{
			EnterCriticalSection(&CriticalSection);
		}
	}

	FORCEINLINE void Unlock(void)
	{
		LeaveCriticalSection(&CriticalSection);
	}
};

class AThread
{
public:
	AThread(void);
	virtual ~AThread(void);
	
	void Start();
protected:
	void Run();
	static unsigned __stdcall ThreadEntryPoint(void* thread);
	virtual void ThreadSetup() = 0;
	virtual void ThreadExecute() = 0;
	virtual void ThreadDestroy() = 0;

protected:
	HANDLE hThread;
	unsigned int threadID;

public:
	ACriticalSection *CriticalSection;
};