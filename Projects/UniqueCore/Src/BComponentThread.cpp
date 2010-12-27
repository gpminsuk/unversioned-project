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
		for(unsigned int i=0;i<Objects.Size();++i)
		{
			AObject* Object = Objects(i);
			Object->Tick(0);
		}
		UpdateThreadSync();
	}
}

void BComponentThread::ThreadDestroy()
{
}