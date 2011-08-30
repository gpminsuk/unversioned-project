#pragma once

#include "CDrawable.h"
#include <vector>

class CBeatRunner;
class CBitmapResource;

enum EHitType
{
	HitType_Up,
	HitType_Left,
	HitType_Right,
	HitType_Down,
};

enum EHitResult
{
	HitResult_None,
	HitResult_Perfect,
	HitResult_Good,
	HitResult_Bad,
	HitResult_Miss,
};

class CNote
{
public:
	EHitType HitType;
	int Position;
};

class CTrackData
{
public:
	CTrackData();

	int BPS;
	unsigned int Length;
	std::vector<CNote> Notes;
};

class CBeatTrack : public CDrawable
{
public:
	CBeatTrack();
	~CBeatTrack();

	void Start();
	void Pause();
	void Resume();
	bool Advance(unsigned long DeltaTime);
	void Draw(CGraphicInterface* GI);
	EHitResult Hit(EHitType HitType);
protected:
	CBeatRunner* Runner;
	CTrackData*	Track;
private:
	unsigned int NextNoteIndex;
	unsigned long PlayTime;
	bool bPlaying;

	CBitmapResource* PassedTrack[2];
	CBitmapResource* RemainTrack[2];
	CBitmapResource* CornerSpot;
};