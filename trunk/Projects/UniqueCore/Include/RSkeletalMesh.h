#pragma once

#include "AObject.h"
#include "TDataTypes.h"
#include "RSystemMemory.h"

class RSkeletalSubMesh: public AObject
{
DECLARE_CLASS(RSkeletalSubMesh,)
public:
	TString BoneName;

	RSystemMemoryVertexBuffer* pVB;
	RSystemMemoryIndexBuffer* pIB;

	virtual bool Access(AAccessor& Accessor);
};

class RSkeletalMesh: public AObject
{
DECLARE_CLASS(RSkeletalMesh,)
	public:
	~RSkeletalMesh()
	{
		for (unsigned int i = 0; i < SkeletalSubMeshes.Size(); ++i)
			delete SkeletalSubMeshes(i);
		SkeletalSubMeshes.Clear(true);
	}
	TArray<RSkeletalSubMesh*> SkeletalSubMeshes;

	virtual bool Access(AAccessor& Accessor);
};
