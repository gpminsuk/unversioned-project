#pragma once

#include "AObject.h"
#include "TDataTypes.h"


class RAnimationSequence : public AObject
{
	DECLARE_CLASS(RAnimationSequence,)
public:
	class RBoneSequence
	{
	public:
		RBoneSequence(RAnimationSequence* Seq)
		{
			AnimSequenceRef = Seq;
		}

		TString				BoneName;

		struct POSKEY
		{
			TVector3 Pos;
			float Time;
		};

		struct ROTKEY
		{
			TQuaternion Rot;
			float Time;
		};

		TArray<POSKEY>	PosKeys;
		TArray<ROTKEY>	RotKeys;
		RAnimationSequence* AnimSequenceRef;

		TVector3 GetPosKey(unsigned int InFrame)
		{
			if(PosKeys.Size() < 2)
			{
				return TVector3(0,0,0);
			}
			if(InFrame == 0)
			{
				return PosKeys(0).Pos;
			}
			int Idx = -1;
			for(unsigned int i=0;i<PosKeys.Size();++i)
			{
				POSKEY& PosKey = PosKeys(i);
				if(PosKey.Time > InFrame)
				{
					Idx = i;
					break;
				}
			}
			if(Idx == 0)
			{
				//Idx = (int)PosKeys.Size() - 1;
				//float t = 1.0f - (InFrame + AnimSequenceRef->EndFrame*AnimSequenceRef->TickPerFrame - PosKeys(Idx).Time)/((AnimSequenceRef->EndFrame*AnimSequenceRef->TickPerFrame - PosKeys(Idx).Time) + PosKeys(0).Time);
				//return (PosKeys(Idx).Pos*t + PosKeys(0).Pos*(1.0f - t));
				return PosKeys(0).Pos;
			}
			else if(Idx == -1)
			{
				Idx = (int)PosKeys.Size() - 1;
				//float t = 1.0f - (InFrame - PosKeys(Idx).Time)/((AnimSequenceRef->EndFrame*AnimSequenceRef->TickPerFrame - PosKeys(Idx).Time) + PosKeys(0).Time);
				//return (PosKeys(Idx).Pos*t + PosKeys(0).Pos*(1.0f - t));
				return PosKeys(Idx).Pos;
			}
			float t = 1.0f - (InFrame - PosKeys(Idx-1).Time)/(PosKeys(Idx).Time - PosKeys(Idx-1).Time);
			return (PosKeys(Idx-1).Pos*t + PosKeys(Idx).Pos*(1.0f - t));
		}

		TQuaternion GetRotKey(unsigned int InFrame)
		{
			if(RotKeys.Size() < 2)
			{
				return TQuaternion();
			}
			if(InFrame == 0)
			{
				return RotKeys(0).Rot;
			}
			int Idx = -1;
			for(unsigned int i=0;i<RotKeys.Size();++i)
			{
				ROTKEY& RotKey = RotKeys(i);
				if(RotKey.Time > InFrame)
				{
					Idx = i;
					break;
				}
			}
			if(Idx == 0)
			{
				//Idx = (int)RotKeys.Size() - 1;
				//float t = (InFrame + AnimSequenceRef->EndFrame*AnimSequenceRef->TickPerFrame - RotKeys(Idx).Time)/((AnimSequenceRef->EndFrame*AnimSequenceRef->TickPerFrame - RotKeys(Idx).Time) + RotKeys(0).Time);
				//return TQuaternion::Slerp(RotKeys(Idx).Rot, RotKeys(0).Rot, t);
				return RotKeys(0).Rot;
			}
			else if(Idx == -1)
			{
				Idx = (int)RotKeys.Size() - 1;
				//float t = (InFrame - RotKeys(Idx).Time)/(AnimSequenceRef->EndFrame*AnimSequenceRef->TickPerFrame - RotKeys(Idx).Time);
				//return TQuaternion::Slerp(RotKeys(Idx).Rot, RotKeys(0).Rot, t);
				return RotKeys(Idx).Rot;
			}
			float t = (InFrame - RotKeys(Idx-1).Time)/(RotKeys(Idx).Time - RotKeys(Idx-1).Time);
			return TQuaternion::Slerp(RotKeys(Idx-1).Rot, RotKeys(Idx).Rot, t);
		}
	};

	~RAnimationSequence()
	{
		for(unsigned int i = 0;i<AnimationBoneSequences.Size();++i)
			delete AnimationBoneSequences(i);
		AnimationBoneSequences.Clear(true);
	}

	TArray<RBoneSequence*> AnimationBoneSequences;

	unsigned int StartFrame;
	unsigned int EndFrame;
	unsigned int TickPerFrame;
	unsigned int FrameSpeed;
};
typedef RAnimationSequence::RBoneSequence RAnimationBoneSequence;