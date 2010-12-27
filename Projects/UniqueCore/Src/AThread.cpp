#include "StdAfx.h"
#include "AThread.h"
#include "AApplication.h"
#include "AObject.h"

AThread::AThread(AApplication* _App) :
	Application(_App)
{
}

AThread::~AThread(void)
{
}

void AThread::Start()
{
	_beginthreadex(NULL, 0, ThreadEntryPoint, this, 0, &threadID);
}

void AThread::Run()
{
	//ThreadExecute();
}

unsigned __stdcall AThread::ThreadEntryPoint(void *thread)
{
	AThread *pThread = (AThread*)thread;
	//pThread->ThreadSetup();
	//pThread->Run();
	//pThread->ThreadDestroy();
	return 0;
}

void AThread::UpdateThreadSync()
{
	for(unsigned int i=0;i<Objects.Size();++i)
	{
		AObject* Object = Objects(i);
	}
}