/////////////////////////////////////////////////////////////
// The wave recorder, derived from CWaveIOBase 
//
// version 02.24.2005, Dublin, VA 24084
// author: Qiang Yu (qiangyu#gmail.com)
/////////////////////////////////////////////////////////////
#pragma once

#ifdef new
#undef new
#endif

#include "waveiobase.h"

class CWaveRecorder :
	public CWaveIOBase
{
public:
	CWaveRecorder(void);
	virtual ~CWaveRecorder(void);

protected:
	virtual void SendToSoundDevice(WAVEHDR *);
	virtual void PrepareFreeHdr(WAVEHDR *);
	virtual void FreeReturnedHdr(WAVEHDR *);

	virtual bool OpenDevice(){
		if(waveInOpen((HWAVEIN *)&m_hwave, WAVE_MAPPER, &m_wfx, 
			(DWORD_PTR)CWaveRecorder::WaveIOProc, 
			(DWORD)this, CALLBACK_FUNCTION)!=MMSYSERR_NOERROR){
			m_hwave = 0;
			return false;
		}
		else
			return true;
	};
	
	virtual bool CloseDevice(){
		waveInClose((HWAVEIN)m_hwave);
		return true;
	};

	virtual bool StartDevice(){
		waveInStart((HWAVEIN)m_hwave);
		return true;
	};

	virtual bool PauseDevice(){
		waveInStop((HWAVEIN)m_hwave);
		return true;
	};

	virtual bool ResumeDevice(){
		waveInStart((HWAVEIN)m_hwave);
		return true;
	};

	virtual bool ResetDevice(){
		waveInReset((HWAVEIN)m_hwave);
		return true;
	};
};
