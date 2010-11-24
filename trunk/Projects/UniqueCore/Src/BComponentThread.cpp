#include "stdafx.h"
#include "BComponentThread.h"

#include "AApplication.h"
#include "AObject.h"

BComponentThread::BComponentThread(AApplication* App) :
	AThread(App)
{

}

BComponentThread::~BComponentThread()
{

}

void BComponentThread::ThreadSetup()
{	
}

void BComponentThread::ThreadExecute()
{
	while(!Application->bQuit)
	{
		GetThreadSync();
		for(unsigned int i=0;i<Objects.Size();++i)
		{
			AObject* Object = Objects(i);
			Object->Tick();
		}
		SetThreadSync();
	}
}

void BComponentThread::ThreadDestroy()
{
}