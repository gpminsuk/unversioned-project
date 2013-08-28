#pragma once

#include "AObject.h"
#include "TDataTypes.h"
#include "RSystemMemory.h"
#include "RAsset.h"

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
		for (unsigned int i = 0; i < SubMeshes.Size(); ++i)
			delete SubMeshes(i);
		SubMeshes.Clear(true);
	}
	TArray<RSubMesh*> SubMeshes;

	virtual bool Access(AAccessor& Accessor);

	virtual TArray<TString> GetCreatablePrimitiveClassNames()
	{
		TArray<TString> Ret;
		Ret.AddItem(TString("CMeshPrimitive"));
		Ret.AddItem(TString("CSkeletalMeshPrimitive"));
		return Ret;
	}
};
