#pragma once

#include "BSoundDriver.h"

//#pragma comment(lib, "WaveIO.lib")

class CWaveIODriver : public BSoundDriver
{
public:
	CWaveIODriver();
	virtual ~CWaveIODriver();

	virtual bool CreateSoundDriver();
	virtual bool DestroySoundDriver();

	virtual bool LoadSound(TString Filename);
	virtual bool PlayWAVSound();

	class CWavePlayer* Player;
	class CWaveReader* Reader;
};