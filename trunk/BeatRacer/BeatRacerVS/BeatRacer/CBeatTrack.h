#pragma once

#include <vector>

class CBeatRunner;

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

class CBeatTrack
{
public:
	CBeatTrack();
	~CBeatTrack();

	void Start();
	void Pause();
	void Resume();
	bool Advance(unsigned long DeltaTime);
	EHitResult Hit(EHitType HitType);
protected:
	CBeatRunner* Runner;
	CTrackData*	Track;
private:
	unsigned int NextNoteIndex;
	unsigned long PlayTime;
	bool bPlaying;
};