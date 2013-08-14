#pragma once

#include "BPrimitive.h"
#include "BThing.h"

class CUIButtonPrimitive;

class TUIButtonPrimitive: public BDraw
{
public:
	TUIButtonPrimitive();
};

class CUIButtonPrimitive: public BPrimitive
{
public:
	class CUIButtonPrimitiveSD
	{
	public:
		int Width;
		int Height;
	};

	int Width;
	int Height;

	RTextureBuffer* Texture;

	CUIButtonPrimitive(RTextureBuffer* InTexture);
	~CUIButtonPrimitive(void);

	virtual void Render(BRenderingBatch *Batch);
	virtual unsigned int FillDynamicVertexBuffer(char** pData);
	virtual unsigned int FillDynamicIndexBuffer(TIndex16** pData, unsigned short* BaseIndex);
	virtual unsigned int GetNumIndices();
};

class CUIButton: public BThing
{
public:
	int Width;
	int Height;
	int PosX;
	int PosY;

	CUIButton(int TexIndex, int x = 0, int y = 0, int width = 800, int height =
			600);
	~CUIButton();
};
