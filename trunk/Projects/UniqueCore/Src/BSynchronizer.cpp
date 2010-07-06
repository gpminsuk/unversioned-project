#include "StdAfx.h"
#include "BSynchronizer.h"

BSynchronizer::BSynchronizer()
:	PreviousSync(0)
{
}

BSynchronizer::~BSynchronizer(void)
{
}

void BSynchronizer::Syncronize(BSynchronizer* Sync)
{
	if(PreviousSync)
	{
		delete PreviousSync;
	}
	PreviousSync = Sync;
}