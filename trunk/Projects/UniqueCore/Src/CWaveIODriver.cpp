#include "StdAfx.h"

#include "CWaveIODriver.h"

#include <windows.h>
#include "WaveReader.h"
#include "WavePlayer.h"

CWaveIODriver::CWaveIODriver()
{
	Player = new CWavePlayer();
	Reader = 0;
}

CWaveIODriver::~CWaveIODriver()
{
	delete Reader;
	delete Player;
}

bool CWaveIODriver::CreateSoundDriver()
{
	return true;
}

bool CWaveIODriver::DestroySoundDriver()
{
	return true;
}

bool CWaveIODriver::PlayWAVSound()
{
	//Player->Start();
	return true;
}

bool CWaveIODriver::LoadSound(TString Filename)
{
	CWaveReader * ptemp = 0;
	ptemp = new CWaveReader(Filename.Str);
	Player->Close();
	if(Reader){
		Reader->Close();
		delete Reader;
	}
	Reader = ptemp;
	Player->Open(Reader);
	return true;
}