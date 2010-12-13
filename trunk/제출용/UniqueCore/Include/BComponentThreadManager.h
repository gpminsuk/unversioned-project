#pragma once

#include "AThread.h"

class ASystemInfo;

class BComponentThreadManager : public AThread
{
public:
	BComponentThreadManager(AApplication* App);
	~BComponentThreadManager();

	ASystemInfo* SystemInfo;

	virtual void ThreadSetup();
	virtual void ThreadExecute();
	virtual void ThreadDestroy();
};