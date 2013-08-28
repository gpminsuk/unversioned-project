#include "StdAfx.h"
#include "AObject.h"
#include "CRift.h"

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

CRift::CRift(int num) {
    m_Location = TVector3(0, 0, 0);
	
	RMesh* Mesh;
	Mesh = LoadAsset<RMesh>(TString::Format("..\\..\\Resources\\Rift\\obj_%d.exskn", num));

	CMeshPrimitive* MeshPrimitive = new CMeshPrimitive();
	Primitives.AddItem(MeshPrimitive);
	MeshPrimitive->SetMesh(Mesh);
	MeshPrimitive->CreateDraws();
}

CRift::~CRift() {

}

bool CRift::Tick(unsigned long dTime) {
    return true;
}

void CRift::PhysicsTick(unsigned long dTime) {
}

void CRift::UpdateTransform() {
}

void CRift::InputMouse(EMouse_Event Event, TMouseInput_Param& Param) {
}

void CRift::InputKey(EKey_Event Event, TKeyInput_Param& Param) {
}

void CRift::InputChar() {
}
