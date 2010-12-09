#include "StdAfx.h"

#include "CWaveIODriver.h"

#include <windows.h>
#include "WaveReader.h"
#include "WavePlayer.h"

CWaveIODriver::CWaveIODriver()
{
	Player = new CWavePlayer();
}

CWaveIODriver::~CWaveIODriver()
{
	for(unsigned int i=0;i<Readers.Size();++i)
	{
		delete Readers(i);
	}
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

bool CWaveIODriver::PlayWAVSound(int Index)
{
	Player->Open(Readers(Index));
	Player->Start();
	Player->Close();
	return true;
}

bool CWaveIODriver::LoadSound(TString Filename)
{
	CWaveReader * ptemp = 0;
	ptemp = new CWaveReader(Filename.Str);
	Readers.AddItem(ptemp);
	return true;
}