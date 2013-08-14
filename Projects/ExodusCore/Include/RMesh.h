#pragma once

#include "AObject.h"
#include "TDataTypes.h"
#include "RSystemMemory.h"

class RSubMesh: public AObject
{
DECLARE_CLASS(RSubMesh,)
public:
	TString BoneName;

	RSystemMemoryVertexBuffer* pVB;
	RSystemMemoryIndexBuffer* pIB;

	virtual bool Access(AAccessor& Accessor);
};

class RMesh: public RAsset
{
DECLARE_CLASS(RMesh,)
	public:
	~RMesh()
	{
		for (unsigned int i = 0; i < SkeletalSubMeshes.Size(); ++i)
			delete SkeletalSubMeshes(i);
		SkeletalSubMeshes.Clear(true);
	}
	TArray<RSubMesh*> SkeletalSubMeshes;

	virtual bool Access(AAccessor& Accessor);
};
