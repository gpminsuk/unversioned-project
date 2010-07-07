#include "StdAfx.h"
#include "AThread.h"
#include "BSynchronizer.h"

BSynchronizer::BSynchronizer()
:	SyncData(0)
{
	CriticalSection = new ACriticalSection();
}

BSynchronizer::~BSynchronizer(void)
{
	delete CriticalSection;
}

void BSynchronizer::RecieveData(BPrimitive* Primitive)
{
	CriticalSection->Lock();
	SyncData->GetData(Primitive);
	CriticalSection->Unlock();
}

void BSynchronizer::SendData(BComponent* Component)
{
	CriticalSection->Lock();
	SyncData->SetData(Component);
	CriticalSection->Unlock();
}