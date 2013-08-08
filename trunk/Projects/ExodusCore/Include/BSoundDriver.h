#pragma once

#include "RResource.h"

class BSoundDriver
{
public:
	BSoundDriver();
	virtual ~BSoundDriver();

	virtual bool CreateSoundDriver() = 0;
	virtual bool DestroySoundDriver() = 0;

	virtual bool LoadSound(TString Filename) = 0;
	virtual bool PlayWAVSound(int Index) = 0;
};

extern BSoundDriver *GSoundDriver;
