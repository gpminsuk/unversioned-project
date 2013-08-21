#pragma once

#include "AObject.h"

#include "TDataTypes.h"
#include "RSystemMemory.h"
#include "UFreeTypeDrawer.h"
#include "Enumerations_Geom.h"
#include "RShaderClasses.h"

class RTexture;
class BSynchronizer;
class BRenderingBatch;

enum ERenderType
{
	RenderType_Opaque,
	RenderType_Translucent,
	RenderType_Particle,
	RenderType_Line,
	RenderType_UI,
};

class BDraw
{
public:
	BDraw();
	virtual ~BDraw();

	virtual void UpdatePrimitive() {
	}

	class RStaticPrimitiveBuffer* pBuffer;
};

class BPrimitive: public AObject
{
DECLARE_CLASS(BPrimitive,CLASS_Abstract);
public:
	BPrimitive();
	virtual ~BPrimitive(void);

	TVector3 Translation;
	TQuaternion Rotation;
	TVector3 Scale;

	TMatrix TM;

	TArray<BDraw*> Draws;
	unsigned int NumIndices;
	
	BRenderingBatch* Batch;
	enum ERenderType RenderType;

	virtual void UpdatePrimitive() {
	}

	virtual RMaterial* GetMaterial() = 0;
	virtual void CreateDraws() = 0;
	virtual unsigned int FillDynamicVertexBuffer(char** pData) = 0;
	virtual unsigned int FillDynamicIndexBuffer(TIndex16** pData, unsigned short* BaseIndex) = 0;
	virtual unsigned int GetNumIndices() = 0;
	virtual void IndexTessellate() {
	}
	;

	virtual RTexture* GetTexture() = 0;
	virtual TArray<TString> GetNeededAssetNames() = 0;
	virtual TArray<TString> GetCreatableThingClassNames() = 0;
};
