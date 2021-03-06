#include "StdAfx.h"
#include "AObject.h"
#include "CCharacter.h"

#include "CBoxPrimitive.h"
#include "CSkeletalMeshPrimitive.h"

#include "CCylinderCollisionBody.h"

#include "RAnimationSequence.h"
#include "RBoneHierarchy.h"
#include "RMesh.h"

#include "RSKNImporter.h"
#include "RSKLImporter.h"
#include "RANMImporter.h"
#include "RSCBImporter.h"

#include "UWorld.h"

CCharacter::CCharacter() {
    m_Height = 0.0f;
    m_Radius = 0.0f;
    m_Weight = 0.0f;
    m_WalkSpeed = 0.0f;
    m_RunSpeed = 0.0f;

    bIsMovable = false;
    bIsPhysical = false;
    bIsPassable = false;

    m_Location = TVector3(0, 0, 0);
	
    /*RSKNImporter SKNImporter;
    RSKLImporter SKLImporter;
	RANMImporter ANMImporter;
	RSCBImporter SCBImporter;
    RAnimationSequence* AnimationSequence;
    RBoneHierarchy* BoneHierarchy;
	RMesh* SkeletalMesh;
	SCBImporter.Import(TString("..\\..\\Resources\\Rift\\AIPath.scb"),
		AnimationSequence, BoneHierarchy, SkeletalMesh);
	CMeshPrimitive* MeshPrimitive = new CMeshPrimitive();
	MeshPrimitive->SetMesh(SkeletalMesh);
	MeshPrimitive->CreateDraws();
	MeshPrimitive->RenderType = RenderType_Line;
	Primitives.AddItem(MeshPrimitive);*/
	/*
    SKNImporter.Import(TString("..\\..\\Resources\\Renekton_brutal.skn"),
                       AnimationSequence, BoneHierarchy, SkeletalMesh);
    SKLImporter.Import(TString("..\\..\\Resources\\Renekton_brutal.skl"),
                       AnimationSequence, BoneHierarchy, SkeletalMesh);
    ANMImporter.Import(TString("..\\..\\Resources\\renekton_attack1_60fps.anm"),
                       AnimationSequence, BoneHierarchy, SkeletalMesh);
	SaveAsset(BoneHierarchy, TString("..\\..\\Resources\\Renekton_brutal.exskl"));
	SaveAsset(SkeletalMesh, TString("..\\..\\Resources\\Renekton_brutal.exskn"));

	CSkeletalMeshPrimitive* SkeletalMeshPrimitive = new CSkeletalMeshPrimitive();
    Primitives.AddItem(SkeletalMeshPrimitive);
	SkeletalMeshPrimitive->SetSkeletalMesh(SkeletalMesh, BoneHierarchy);
	SkeletalMeshPrimitive->CreateDraws();
	*/
	CCylinderCollisionBody* CylinderCollisionBody = new CCylinderCollisionBody(this);
	CollisionBodies.AddItem(CylinderCollisionBody);

    UpdateTransform();
}

CCharacter::~CCharacter() {

}

void CCharacter::SetCharacterPosition(TVector3 pos) {
    m_Location = pos;

    //m_Location = GWorld->LineCheck(this, m_Location, pos).HitPosition;
    UpdateTransform();
}

bool CCharacter::Tick(unsigned long dTime) {
    /*if(GKeyMap['W'])
     SetCharacterPosition(m_Location + TVector3(0.0005f,0.0f,0.0f));
     if(GKeyMap['S'])
     SetCharacterPosition(m_Location + TVector3(-0.0005f,0.0f,0.0f));
     if(GKeyMap['A'])
     SetCharacterPosition(m_Location + TVector3(0.0f,0.0f,0.0005f));
     if(GKeyMap['D'])
     SetCharacterPosition(m_Location + TVector3(0.0f,0.0f,-0.0005f));*/
	UpdateTransform();
    return true;
}

void CCharacter::PhysicsTick(unsigned long dTime) {
    return;
    TVector3 Loc = m_Location;
    float t = 0.003f;
    Loc.y -= (float) (t * t * 9.8 / 2.0f);
    SetCharacterPosition(Loc);
}

void CCharacter::UpdateTransform() {
    for (unsigned int i = 0; i < CollisionBodies(0)->Primitives.Size(); ++i) {
        CollisionBodies(0)->Primitives(i)->Translation = m_Location;
        CollisionBodies(0)->Primitives(i)->TM._41 = m_Location.x;
        CollisionBodies(0)->Primitives(i)->TM._42 = m_Location.y;
        CollisionBodies(0)->Primitives(i)->TM._43 = m_Location.z;
    }
	for (unsigned int i = 0; i < Primitives.Size(); ++i) {
		Primitives(i)->TM.SetIdentity();
		Primitives(i)->Translation = m_Location;
		static float r = MATH_PI/2;
		Primitives(i)->TM._41 = m_Location.x;
        Primitives(i)->TM._42 = m_Location.y;
		Primitives(i)->TM._43 = m_Location.z;
    }

    CollisionBodyBounds.Box.Extent = TVector3(5.0f, 5.0f, 5.0f);
    CollisionBodyBounds.Position = m_Location;
    CollisionBodyBounds.Sphere.Radius = 5.0f;
}

void CCharacter::InputMouse(EMouse_Event Event, TMouseInput_Param& Param) {
}

void CCharacter::InputKey(EKey_Event Event, TKeyInput_Param& Param) {
}

void CCharacter::InputChar() {
}
