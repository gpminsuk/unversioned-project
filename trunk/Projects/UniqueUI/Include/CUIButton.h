#pragma once

#include "BPrimitive.h"
#include "BSynchronizer.h"
#include "BComponent.h"

class CUIButtonPrimitive;
class CUIButtonComponent;

class CUIButtonSyncronizer : public BSynchronizer
{
public:
	CUIButtonSyncronizer();
};

class CUIButtonSyncronizeData : public BSynchronizeData
{
public:
	int	Width;
	int Height;

	virtual void GetData(BPrimitive* Primitive);
	virtual void SetData(BComponent* Component);
};

class TUIButtonPrimitive : public TPrimitive
{
public:
	TUIButtonPrimitive();
};

class CUIButtonPrimitive : public BPrimitive
{
public:
	int Width;
	int Height;

	CUIButtonPrimitive();
	~CUIButtonPrimitive(void);

	virtual BSynchronizer* CreateSynchronizer();
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