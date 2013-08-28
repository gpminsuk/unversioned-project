#include "StdAfx.h"
#include "AObject.h"
#include "CRenekton.h"

#include "CBoxPrimitive.h"
#include "CSkeletalMeshPrimitive.h"

#include "CMeshCollisionBody.h"

#include "RAnimationSequence.h"
#include "RBoneHierarchy.h"
#include "RMesh.h"

#include "RSKNImporter.h"
#include "RSKLImporter.h"
#include "RANMImporter.h"
#include "RSCBImporter.h"

#include "UWorld.h"

IMPLEMENT_CLASS(CRenekton);

CRenekton::CRenekton() {
    m_Height = 0.0f;
    m_Radius = 0.0f;
    m_Weight = 0.0f;
    m_WalkSpeed = 7.0f;
    m_RunSpeed = 0.0f;

    bIsMovable = false;
    bIsPhysical = false;
    bIsPassable = false;

	m_Rotation = TVector3(1, 0, 0);
    m_Location = TVector3(500, 0, 500);
	
    RBoneHierarchy* BoneHierarchy;
	RMesh* SkeletalMesh;
	//BoneHierarchy = LoadAsset<RBoneHierarchy>(TString("..\\..\\Resources\\Renekton_brutal.exskl"));
	RSKLImporter im;
	RAnimationSequence* Seq;
	RSKNImporter imk;
	imk.Import(TString("..\\..\\Resources\\Renekton_brutal.skn"), Seq, BoneHierarchy, SkeletalMesh);
	im.Import(TString("..\\..\\Resources\\Renekton_brutal.skl"), Seq, BoneHierarchy, SkeletalMesh);
	//SkeletalMesh = LoadAsset<RMesh>(TString("..\\..\\Resources\\Renekton_brutal.exskn"));

	RAnimationTree* T = new RAnimationTree();
	RAnimationBlend* B = new RAnimationBlend();
	B->Initialize(this, BoneHierarchy);
	T->Root = B;

	CSkeletalMeshPrimitive* SkeletalMeshPrimitive = new CSkeletalMeshPrimitive();
    Primitives.AddItem(SkeletalMeshPrimitive);
	SkeletalMeshPrimitive->SetSkeletalMesh(SkeletalMesh, BoneHierarchy);
	SkeletalMeshPrimitive->SetAnimationTree(T);
	SkeletalMeshPrimitive->CreateDraws();

	CMeshCollisionBody* MeshCollisionBody = new CMeshCollisionBody(this);
	MeshCollisionBody->SetMesh(SkeletalMesh);
	CollisionBodies.AddItem(MeshCollisionBody);

    UpdateTransform();
}

CRenekton::~CRenekton() {

}

void CRenekton::SetCharacterPosition(TVector3 pos) {
    m_Location = pos;

    UpdateTransform();
}

bool CRenekton::Tick(unsigned long dTime) {
	// юс╫ц
	((CSkeletalMeshPrimitive*)Primitives(0))->AnimationTree.Get()->Tick(dTime / 1000.0f);
	TVector3 Dir = m_Rotation;
	Dir.y = 0;
	Dir.Normalize();
	m_IsMoving = false;
    if(GKeyMap['W']) {
		SetCharacterPosition(m_Location + TVector3(-Dir.x, 0, -Dir.z) * m_WalkSpeed);
		m_FrameRotation = m_Rotation;
		m_IsMoving = true;
	}
	if(GKeyMap['S']) {
		SetCharacterPosition(m_Location + Dir * m_WalkSpeed);
		m_FrameRotation = TVector3(-Dir.x, 0, -Dir.z);
		m_IsMoving = true;
	}
	if(GKeyMap['A']) {
		SetCharacterPosition(m_Location + TVector3(Dir.z, 0, -Dir.x) * m_WalkSpeed);
		m_FrameRotation = TVector3(-Dir.z, 0, Dir.x);
		m_IsMoving = true;
	}
    if(GKeyMap['D']) {
		SetCharacterPosition(m_Location + TVector3(-Dir.z, 0, Dir.x) * m_WalkSpeed);
		m_FrameRotation = TVector3(Dir.z, 0, -Dir.x);
		m_IsMoving = true;
	}

	if(GKeyMap['W'] && GKeyMap['D']) {
		m_FrameRotation = (Dir + TVector3(Dir.z, 0, -Dir.x)).Normalize();
	}
	if(GKeyMap['W'] && GKeyMap['A']) {
		m_FrameRotation = (Dir + TVector3(-Dir.z, 0, Dir.x)).Normalize();
	}
	if(GKeyMap['S'] && GKeyMap['A']) {
		m_FrameRotation = (TVector3(-Dir.x, 0, -Dir.z) + TVector3(-Dir.z, 0, Dir.x)).Normalize();
	}
	if(GKeyMap['S'] && GKeyMap['D']) {
		m_FrameRotation = (TVector3(-Dir.x, 0, -Dir.z) + TVector3(Dir.z, 0, -Dir.x)).Normalize();
	}
	
	m_Location = GWorld->LineCheck(this, TVector3(m_Location.x, 9999, m_Location.z), TVector3(m_Location.x, -9999, m_Location.z)).HitPosition;
	UpdateTransform();
    return true;
}

void CRenekton::PhysicsTick(unsigned long dTime) {
    return;
    TVector3 Loc = m_Location;
    float t = 0.003f;
    Loc.y -= (float) (t * t * 9.8 / 2.0f);
    SetCharacterPosition(Loc);
}

void CRenekton::UpdateTransform() {
	for (unsigned int i = 0; i < Primitives.Size(); ++i) {
		TMatrix Rotation(m_FrameRotation, TVector3(0, 1, 0));
		Primitives(i)->TM.SetIdentity();
		Primitives(i)->Translation = m_Location;
		Primitives(i)->TM *= Rotation;
		Primitives(i)->TM._41 = m_Location.x;
        Primitives(i)->TM._42 = m_Location.y;
		Primitives(i)->TM._43 = m_Location.z;
    }
}

void CRenekton::InputMouse(EMouse_Event Event, TMouseInput_Param& Param) {
}

void CRenekton::InputKey(EKey_Event Event, TKeyInput_Param& Param) {
}

void CRenekton::InputChar() {
}
