#pragma once

#include "BSoundDriver.h"

class CDirectXSoundDriver : public BSoundDriver
{
public:
	CDirectXSoundDriver();
	virtual ~CDirectXSoundDriver();

	virtual bool CreateSoundDriver();
	virtual bool DestroySoundDriver();

	virtual bool PlaySound();
};