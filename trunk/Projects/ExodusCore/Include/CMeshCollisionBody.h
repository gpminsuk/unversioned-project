#pragma once
#include "BCollisionBody.h"

class RMesh;

class CMeshCollisionBody: public BCollisionBody
{
public:
	CMeshCollisionBody(class BThing* InOwner);
	virtual ~CMeshCollisionBody();

	void SetMesh(RMesh* InMesh);

	RMesh* Mesh;

	virtual TVector3 LineCheck(TVector3& Start, TVector3& End, TVector3& Extent);
};
