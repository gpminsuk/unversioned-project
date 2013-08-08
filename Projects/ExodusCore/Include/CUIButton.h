#pragma once

#include "BPrimitive.h"
#include "BComponent.h"

class CUIButtonPrimitive;
class CUIButtonComponent;

class TUIButtonPrimitive: public TPrimitive
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

class CUIButtonComponent: public BComponent
{
public:
	int Width;
	int Height;
	int PosX;
	int PosY;

	CUIButtonComponent(int TexIndex, int x = 0, int y = 0, int width = 800, int height =
			600);
	~CUIButtonComponent();

	virtual void UpdateComponent();
};
