#pragma once
#include "BPrimitive.h"

class RMesh;
class RTextureBuffer;

class CMeshDraw: public BDraw
{
public:
	CMeshDraw(RMesh* Mesh);

	RMesh* Mesh;
};

class CMeshPrimitive: public BPrimitive
{
	DECLARE_CLASS(CMeshPrimitive,)
public:
	CMeshPrimitive(void);
	~CMeshPrimitive(void);

	void SetMesh(RMesh* InMesh);

	RAssetPtr<RMesh> Mesh;
	RAssetPtr<RTextureBuffer> Texture;

	virtual bool Access(AAccessor& Accessor);
	virtual void CreateDraws();
	virtual RMaterial* GetMaterial();
	virtual unsigned int FillDynamicVertexBuffer(char** pData);
	virtual unsigned int FillDynamicIndexBuffer(TIndex16** pData, unsigned short* BaseIndex);
	virtual unsigned int GetNumIndices();
};
