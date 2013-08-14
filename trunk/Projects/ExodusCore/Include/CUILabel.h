#pragma once

#include "TDataTypes.h"
#include "BPrimitive.h"
#include "BThing.h"

class CUILabelPrimitive: public BPrimitive
{
public:
	TString Text;

	virtual void DrawUI(RTextureBuffer* RenderTarget);
	virtual unsigned int FillDynamicVertexBuffer(char** pData);
	virtual unsigned int FillDynamicIndexBuffer(TIndex16** pData, unsigned short* BaseIndex);
	virtual unsigned int GetNumIndices();
};

class CUILabel: public BThing
{
public:
	int Width;
	int Height;

	CUILabel();
	~CUILabel();
};
