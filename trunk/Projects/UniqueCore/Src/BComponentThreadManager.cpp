#include "stdafx.h"
#include "BComponentThreadManager.h"

#include "AApplication.h"
#include "AObject.h"

BComponentThreadManager::BComponentThreadManager(AApplication* App) :
	AThread(App)
{
	SystemInfo = App->SystemInfo;
}

BComponentThreadManager::~BComponentThreadManager()
{

}

void BComponentThreadManager::ThreadSetup()
{	
}

void BComponentThreadManager::ThreadExecute()
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

void BComponentThreadManager::ThreadDestroy()
{
}