#pragma once
#include "BPrimitive.h"
#include "RAsset.h"

class RMesh;
class RTexture;
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
	RAssetPtr<RTexture> Texture;

	virtual bool Access(AAccessor& Accessor);
	virtual void UpdatePrimitive();
	virtual void CreateDraws();
	virtual RMaterial* GetMaterial();
	virtual unsigned int FillDynamicVertexBuffer(char** pData);
	virtual unsigned int FillDynamicIndexBuffer(TIndex16** pData, unsigned short* BaseIndex);
	virtual unsigned int GetNumIndices();

	virtual RTexture* GetTexture() { return Texture.Get(); }
	virtual TArray<TString> GetNeededAssetNames() {
		TArray<TString> Ret;
		Ret.AddItem(TString("RMesh"));
		return Ret;
	}

	virtual TArray<TString> GetCreatableThingClassNames() {
		TArray<TString> Ret;
		Ret.AddItem(TString("CCharacter"));
		return Ret;
	}
};
