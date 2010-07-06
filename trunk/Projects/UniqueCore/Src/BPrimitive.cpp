#include "StdAfx.h"
#include "BPrimitive.h"
#include "BSynchronizer.h"

#include "RResource.h"

BPrimitive::BPrimitive(BSynchronizer** Synchronizer)
: RenderThreadSyncronizer(0)
{
	InitializeSynchronizer(Synchronizer);
}

BPrimitive::~BPrimitive(void)
{
	Primitives.Clear();
	delete RenderThreadSyncronizer;
}

TPrimitive::TPrimitive()
: pBuffer(0)
{
}

TPrimitive::~TPrimitive()
{
	delete pBuffer;
}