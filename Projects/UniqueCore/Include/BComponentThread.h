#pragma once

#include "AThread.h"

class BComponentThread : public AThread
{
public:
	BComponentThread(AApplication* App);
	~BComponentThread();

	virtual void ThreadSetup();
	virtual void ThreadExecute();
	virtual void ThreadDestroy();
};