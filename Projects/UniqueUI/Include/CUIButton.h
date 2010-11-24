#pragma once

#include "BPrimitive.h"
#include "BComponent.h"

class CUIButtonPrimitive;
class CUIButtonComponent;

class TUIButtonPrimitive : public TPrimitive
{
public:
	TUIButtonPrimitive();
};

class CUIButtonPrimitive : public BPrimitive
{
public:
	class CUIButtonPrimitiveSD : public ASyncData
	{
	public:
		int	Width;
		int Height;
	};
	
	int Width;
	int Height;

	CUIButtonPrimitive();
	~CUIButtonPrimitive(void);

	virtual void GetSyncData();
	virtual void SetSyncData();

	virtual void Render(BRenderingBatch *Batch);
	virtual unsigned int FillDynamicVertexBuffer(char** pData);
	virtual unsigned int FillDynamicIndexBuffer(TIndex16** pData, unsigned short* BaseIndex);
	virtual unsigned int GetNumIndices();
};

class CUIButtonComponent : public BComponent
{
public:
	int Width;
	int Height;

	CUIButtonComponent();
	~CUIButtonComponent();

	virtual void UpdateComponent();
};