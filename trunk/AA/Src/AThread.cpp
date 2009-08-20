#include "StdAfx.h"
#include "AThread.h"

AThread::AThread(void)
{
}

AThread::~AThread(void)
{
}

void AThread::Start()
{
	ThreadSetup();
	_beginthreadex(NULL, 0, ThreadEntryPoint, this, 0, &threadID);
}

void AThread::Run()
{
	ThreadExecute();
	ThreadDestroy();
}

unsigned __stdcall AThread::ThreadEntryPoint(void *thread)
{
	AThread *pThread = (AThread*)thread;
	pThread->Run();
	return 0;
}