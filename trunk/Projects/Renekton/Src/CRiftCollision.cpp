#include "StdAfx.h"
#include "AObject.h"
#include "CRiftCollision.h"

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

CRiftCollision::CRiftCollision() {
	m_Location = TVector3(0, 0, 0);

	/*RSCBImporter Import;
	RMesh* Mesh;
	RBoneHierarchy* b;
	RAnimationSequence* a;
	Import.Import(TString("..\\..\\Resources\\Rift\\AIPath.SCB"), a, b, Mesh);
	SaveAsset(Mesh, TString("..\\..\\Resources\\Rift\\AIPath.exskn"));*/
	RMesh* Mesh;
	Mesh = LoadAsset<RMesh>(TString("..\\..\\Resources\\Rift\\AIPath.exskn"));
	
	/*CMeshPrimitive* MeshPrimitive = new CMeshPrimitive();
	//Primitives.AddItem(MeshPrimitive);
	MeshPrimitive->SetMesh(Mesh);
	MeshPrimitive->CreateDraws();*/

	CMeshCollisionBody* CollisionBody = new CMeshCollisionBody(this);
	CollisionBody->SetMesh(Mesh);
	CollisionBodies.AddItem(CollisionBody);
}

CRiftCollision::~CRiftCollision() {

}

bool CRiftCollision::Tick(unsigned long dTime) {
    return true;
}

void CRiftCollision::PhysicsTick(unsigned long dTime) {
}

void CRiftCollision::UpdateTransform() {
}

void CRiftCollision::InputMouse(EMouse_Event Event, TMouseInput_Param& Param) {
}

void CRiftCollision::InputKey(EKey_Event Event, TKeyInput_Param& Param) {
}

void CRiftCollision::InputChar() {
}
