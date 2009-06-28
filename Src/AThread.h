#pragma once

#include <process.h>
#include <windows.h>

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

private:
	HANDLE hThread;
	unsigned int threadID;
};
