#include "StdAfx.h"

#include "CWaveIODriver.h"

#include <windows.h>
#include "WaveReader.h"
#include "WavePlayer.h"

CWaveIODriver::CWaveIODriver()
{
}

CWaveIODriver::~CWaveIODriver()
{
	for(unsigned int i=0;i<Players.Size();++i)
	{
		delete Players(i);
	}
	for(unsigned int i=0;i<Readers.Size();++i)
	{
		delete Readers(i);
	}
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
	//Players(Index)->
	Players(Index)->Reset();
	Players(Index)->Start();
	return true;
}

bool CWaveIODriver::LoadSound(TString Filename)
{
	CWaveReader* Reader = new CWaveReader(Filename.Str);;
	CWavePlayer* Player = new CWavePlayer();
	Player->Open(Reader);
	Players.AddItem(Player);
	Readers.AddItem(Reader);
	return true;
}