#include "stdafx.h"
#include "CBeatTrack.h"

CTrackData::CTrackData()
{
	Length = 1 * 5 * 1000;
	BPS = 60;
	for(int i=0;i<60;++i)
	{
		CNote Note;
		Note.HitType = HitType_Down;
		Note.Position = 600 * i;
		Notes.push_back(Note);
	}
}

CBeatTrack::CBeatTrack() :
	PlayTime(0),
	bPlaying(false),
	NextNoteIndex(0)
{
	Track = new CTrackData();
}

CBeatTrack::~CBeatTrack()
{
	delete Track;
}

void CBeatTrack::Start()
{
	PlayTime = 0;
	bPlaying = true;
}

void CBeatTrack::Pause()
{
	bPlaying = true;
}

void CBeatTrack::Resume()
{
	bPlaying = false;
}

bool CBeatTrack::Advance(unsigned long DeltaTime)
{
	if(bPlaying)
	{
		PlayTime += DeltaTime;
		if( PlayTime >= Track->Length )
		{
			return false;
		}
	}
	return true;
}

EHitResult CBeatTrack::Hit(EHitType HitType)
{
	if( NextNoteIndex < Track->Notes.size() )
	{
		if(Track->Notes[NextNoteIndex].HitType == HitType)
		{
			unsigned long NoteSecond = (unsigned long)((float)Track->Notes[NextNoteIndex].Position/(float)Track->BPS*1000);
			if(abs(NoteSecond - PlayTime) < 100)
			{
				return HitResult_Perfect;
			}
			else if(abs(NoteSecond - PlayTime) < 300)
			{
				return HitResult_Good;
			}
			else if(abs(NoteSecond - PlayTime) < 500)
			{
				return HitResult_Bad;
			}
			else if(NoteSecond - PlayTime < -300)
			{
				return HitResult_Miss;
			}
		}
	}
	return HitResult_None;
}