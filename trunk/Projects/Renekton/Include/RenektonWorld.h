#pragma once

#include "UWorld.h"

class URenektonWorld : public UWorldTemplate
{
	DECLARE_CLASS(URenektonWorld,);
public:
	URenektonWorld();
	~URenektonWorld();

	virtual void Create(UWorld* pWorld);

	class CRenekton* Renekton;
	TArray<class CRift*> Rifts;
	class CRiftCollision* RiftCollision;
	class CDirectionalLight* DirectionalLight;
};