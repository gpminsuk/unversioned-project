#include "StdAfx.h"
#include "UWorld.h"

#include "BRenderer.h"

#include "CCameraViewport.h"
#include "CTerrain.h"
#include "CCamera.h"
#include "CCharacter.h"

UWorld::UWorld(BRenderer* R)
: m_pRenderer(R),
	m_pViewport(0),
	m_pCamera(0)
{
}

UWorld::~UWorld()
{
//	delete m_Mesh;
	delete m_Terrain;
	delete m_Character;
}
float frame = 0;
void UWorld::Tick(DWORD dTime)
{
	m_pViewport->UpdateCameraViewport();
	for(unsigned int i=0;i<Objects.Size();++i)
		Objects(i)->Tick(dTime);
/*
	for(unsigned int i=0;i<m_Mesh->Primitives.Size();++i)
	{
		TPrimitive *Prim = m_Mesh->Primitives(i);
		TSkeletalBone* Bone = 0;
		RBone*	BoneSrc = 0;
		RAnimationBoneSequence *Seq = 0;
		RAnimationSequence *ASeq = 0;
		for(unsigned int j=0;j<m_Mesh->BoneInfo->Bones.Size();++j)
		{
			if(!strcmp(m_Mesh->BoneRef(j)->BoneName.Str, Prim->pSubMesh->BoneName.Str))
			{
				Bone = m_Mesh->BoneRef(j);
			}
			if(!strcmp(m_Mesh->BoneInfo->Bones(j)->BoneName.Str, Prim->pSubMesh->BoneName.Str))
			{
				BoneSrc = m_Mesh->BoneInfo->Bones(j);
			}
		}
		for(unsigned int j=0;j<m_Mesh->Animations.Size();++j)
		{
			for(unsigned int k=0;k<m_Mesh->Animations(j)->AnimationBoneSequences.Size();++k)
			{
				if(!strcmp(m_Mesh->Animations(j)->AnimationBoneSequences(k)->BoneName.Str, Prim->pSubMesh->BoneName.Str))
				{
					Seq = m_Mesh->Animations(j)->AnimationBoneSequences(k);
					ASeq = m_Mesh->Animations(j);
				}
			}
		}
		if(!Bone)
			continue;

		TMatrix TM;
		TMatrix Temp;
		TM *= Temp.SetIdentity().Translate(Bone->Translation = BoneSrc->Translation);
		TM *= Temp.SetIdentity().Rotate(Bone->Rotation = BoneSrc->Rotation);
		TSkeletalBone *B = Bone;
		while(B->Parent)
		{
			B = B->Parent;
			TM *= Temp.SetIdentity().Translate(B->Translation);
			TM *= Temp.SetIdentity().Rotate(B->Rotation);
		}
		if(ASeq && Seq)
		{
			frame += 0.0001f;
			if(frame > (ASeq->EndFrame - ASeq->StartFrame))
				frame = (ASeq->EndFrame - ASeq->StartFrame) - frame;
			float Time = frame * ASeq->TickPerFrame;
			for(int j=0;j<(signed int)Seq->PosKeys.Size()-1;++j)
			{
				if(Time >= Seq->PosKeys(j).Time && Time <= Seq->PosKeys(j+1).Time)
				{
					Time -= Seq->PosKeys(j).Time;
					Time = Time / (Seq->PosKeys(j+1).Time-Seq->PosKeys(j).Time);
					TM *= Temp.SetIdentity().Translate(Seq->PosKeys(j).Pos * Time + Seq->PosKeys(j+1).Pos * (1-Time));
					Bone->Translation += Seq->PosKeys(j).Pos * Time + Seq->PosKeys(j+1).Pos * (1-Time);
					break;
				}
			}
			for(int j=0;j<(signed int)Seq->RotKeys.Size()-1;++j)
			{
				if(Time >= Seq->RotKeys(j).Time && Time <= Seq->RotKeys(j+1).Time)
				{
					Time -= Seq->RotKeys(j).Time;
					Time = Time / (Seq->RotKeys(j+1).Time-Seq->RotKeys(j).Time);
					TM *= Temp.SetIdentity().Rotate(TQuaternion::Slerp(Seq->RotKeys(j).Rot, Seq->RotKeys(j+1).Rot, Time));
					Bone->Rotation *= TQuaternion::Slerp(Seq->RotKeys(j).Rot, Seq->RotKeys(j+1).Rot, Time);	
					break;
				}
			}
		}
		Prim->TM = TM;
	}*/
}

bool UWorld::InitializeWorld()
{
	m_pViewport = new CCameraViewport();

	m_pRenderer->AddViewport(m_pViewport);

/*	m_Mesh = new CSkeletalMeshPrimitive(RBoneInfoTable::BoneInfos(0));

	m_Mesh->Animations.AddItem(RAnimationSequenceTable::Sequences(0));
	m_Mesh->BoneInfo = RBoneInfoTable::BoneInfos(0);*/

/*	for(unsigned int i=0;i<RMeshTable::Meshes.Size();++i)
	{
		RMesh *Mesh = RMeshTable::Meshes(i);
		for(unsigned int j=0;j<Mesh->SubMeshes.Size();++j)
		{
			TPrimitive *Prim = new TPrimitive();

			Prim->pSubMesh = Mesh->SubMeshes(j);

			RBone* Bone = 0;
			RAnimationBoneSequence *Seq = 0;
			for(unsigned int k=0;k<m_Mesh->BoneInfo->Bones.Size();++k)
			{
				if(!strcmp(m_Mesh->BoneInfo->Bones(k)->BoneName.Str, Prim->pSubMesh->BoneName.Str))
				{
					Bone = m_Mesh->BoneInfo->Bones(k);
				}
			}
			for(unsigned int k=0;k<m_Mesh->Animations.Size();++k)
			{
				for(unsigned int l=0;l<m_Mesh->Animations(k)->AnimationBoneSequences.Size();++l)
				{
					if(!strcmp(m_Mesh->Animations(k)->AnimationBoneSequences(l)->BoneName.Str, Prim->pSubMesh->BoneName.Str))
					{
						Seq = m_Mesh->Animations(k)->AnimationBoneSequences(l);
					}
				}
			}

			Prim->pBone = Bone;
			Prim->pAnimationSequence = Seq;

			m_Mesh->Primitives.AddItem(Prim);
		}
		}
		m_pViewport->Render(m_Mesh);*/


	m_pCamera = new CCamera();

	m_pViewport->AddCamera(m_pCamera);

	m_pViewport->SetCurrentCamera(0);

	m_Terrain = new CTerrain(m_pCamera);
	m_pViewport->Render(m_Terrain->Primitives(0));
	Objects.AddItem(m_Terrain);

	m_Character = new CCharacter();
	m_pViewport->Render(m_Character->Primitives(0));
	Objects.AddItem(m_Character);

	return TRUE;
}

bool UWorld::DestroyWorld()
{
	delete m_pViewport;
	m_pViewport = 0;
	delete m_pCamera;
	m_pCamera = 0;
	return TRUE;
}

void UWorld::InputChar()
{
	m_pViewport->InputChar();
}

void UWorld::InputKey(EKey_Event Event, TKeyInput_Param& Param)
{
	m_pViewport->InputKey(Event, Param);
}

void UWorld::InputMouse(EMouse_Event Event, TMouseInput_Param& Param)
{
	m_pViewport->InputMouse(Event, Param);
}