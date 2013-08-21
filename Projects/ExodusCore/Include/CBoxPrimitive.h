#pragma once

#include "BPrimitive.h"

class TBoxPrimitive: public BDraw
{
public:
	TBoxPrimitive(ERenderType _RenderType, EGeometrySideType _BoxSideType);
};

class CBoxPrimitive: public BPrimitive
{
public:
	CBoxPrimitive(ERenderType _RenderType, EGeometrySideType _BoxSideType);
	~CBoxPrimitive(void);

	EGeometrySideType SideType;

	virtual void CreateDraws();
	virtual RMaterial* GetMaterial();
	virtual unsigned int FillDynamicVertexBuffer(char** pData);
	virtual unsigned int FillDynamicIndexBuffer(TIndex16** pData, unsigned short* BaseIndex);
	virtual unsigned int GetNumIndices();

	virtual RTexture* GetTexture() { return 0; }
	virtual TArray<TString> GetNeededAssetNames() {
		TArray<TString> Ret;
		return Ret;
	}

	virtual TArray<TString> GetCreatableThingClassNames() {
		TArray<TString> Ret;
		Ret.AddItem(TString("CBox"));
		return Ret;
	}
};
