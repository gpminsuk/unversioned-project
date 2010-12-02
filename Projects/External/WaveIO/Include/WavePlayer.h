/////////////////////////////////////////////////////////////
// The wave player, derived from CWaveIOBase 
//
// version 02.24.2005, Dublin, VA 24084
// author: Qiang Yu (qiangyu#gmail.com)
/////////////////////////////////////////////////////////////
#pragma once

#ifdef new
#undef new
#endif

#include "waveiobase.h"

class CWavePlayer :
	public CWaveIOBase
{
public:
	CWavePlayer(void);
	virtual ~CWavePlayer(void);

	//fast forward by specified seconds
	void FastForward(long time){
		Seek(time * 10*m_headerbufferlen, false);
	}
	
	//fast reverse by specified seconds
	void FastReverse(long time){
		Seek(-time * 10*m_headerbufferlen, false);
	}

protected:
	virtual void SendToSoundDevice(WAVEHDR *);
	virtual void PrepareFreeHdr(WAVEHDR *);
	virtual void FreeReturnedHdr(WAVEHDR *);

	virtual bool OpenDevice(){
		if(waveOutOpen((HWAVEOUT *)&m_hwave, WAVE_MAPPER, &m_wfx, 
			(DWORD_PTR)CWavePlayer::WaveIOProc, 
			(DWORD)this, CALLBACK_FUNCTION)!=MMSYSERR_NOERROR){
			m_hwave = 0;
			return false;
		}
		else
			return true;
	};
	
	virtual bool CloseDevice(){
		waveOutClose((HWAVEOUT)m_hwave);
		return true;
	};

	virtual bool StartDevice(){
		return true;
	};

	virtual bool PauseDevice(){
		waveOutPause((HWAVEOUT)m_hwave);
		return true;
	};

	virtual bool ResumeDevice(){
		waveOutRestart((HWAVEOUT)m_hwave);
		return true;
	};

	virtual bool ResetDevice(){
		waveOutReset((HWAVEOUT)m_hwave);
		return true;
	};
};
