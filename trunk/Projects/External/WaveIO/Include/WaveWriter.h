#pragma once

#ifdef new
#undef new
#endif

#include "wavefile.h"
#include "waveiobase.h"
#include <fstream>

using namespace std;

class CWaveWriter :
public CWaveFile, public CWaveDataStore
{
public:
	CWaveWriter(LPCSTR, unsigned long sps = 44100, 
			    unsigned short bps = 16, unsigned short = 2);
	~CWaveWriter(void);
	virtual void StoreData(const void *, const unsigned long *);
	virtual void GetWavePara(unsigned long *, unsigned short*, unsigned short*);
	virtual bool Seek(long, bool begin = true);

	virtual void Close();

private:
	ofstream m_of;
	string m_filename;
	unsigned long m_dataoffset;
	unsigned long m_datalength;
};
