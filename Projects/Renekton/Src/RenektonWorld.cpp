#include "stdafx.h"
#include "RenektonWorld.h"

#include "CWindowsViewport.h"
#include "CRift.h"
#include "CRenekton.h"
#include "CDirectionalLight.h"
#include "CRiftCollision.h"

IMPLEMENT_CLASS(URenektonWorld);

URenektonWorld::URenektonWorld()
{
}

void URenektonWorld::Create(UWorld* World) {
	DirectionalLight = new CDirectionalLight();
	DirectionalLight->m_Location = TVector3(0.0f, 0.0f, 0.0f);
	DirectionalLight->UpdateTransform();
	//AddThing(DirectionalLight);

	Renekton = new CRenekton();
	Renekton->UpdateTransform();
	World->AddThing(Renekton);

	// 임시 ThingGroup 만들어야됨
	UWorld* LoadedWorld = LoadAsset<UWorld>(TString("..\\..\\Resources\\rift.exmap"));
	GWorld = World;
	for(unsigned int i=0;i<((TWorldOctree*)LoadedWorld->m_pWorldData)->AllObjects.Size();++i) {
		World->AddThing(((TWorldOctree*)LoadedWorld->m_pWorldData)->AllObjects(i));
	}
	/*for(int i=1;i<=100;++i) {
		CRift* Rift = new CRift(i);
		Rift->UpdateTransform();
		World->AddThing(Rift);
		Rifts.AddItem(Rift);
	}*/	

	RiftCollision = new CRiftCollision();
	RiftCollision->UpdateTransform();
	World->AddThing(RiftCollision);
}

URenektonWorld::~URenektonWorld()
{
	delete DirectionalLight;
	delete RiftCollision;
	delete Renekton;
	for(unsigned int i=0;i<Rifts.Size();++i) {
		delete Rifts(i);
	}
}