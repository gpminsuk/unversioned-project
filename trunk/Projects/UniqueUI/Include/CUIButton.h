#pragma once

#include "BPrimitive.h"
#include "BSynchronizer.h"
#include "BComponent.h"

class CUIButtonPrimitive;
class CUIButtonComponent;

class CUIButtonSyncronizer : public BSynchronizer
{
public:
	int	Width;
	int Height;

	CUIButtonSyncronizer& operator=(const CUIButtonSyncronizer& Sync);

	virtual void Syncronize(BSynchronizer* Sync);
	virtual void SyncData();
};

class TUIButtonPrimitive : public TPrimitive
{
public:
	TUIButtonPrimitive();
};

class CUIButtonPrimitive : public BPrimitive
{
public:
	CUIButtonPrimitive(BSynchronizer** RenderThreadData);
	~CUIButtonPrimitive(void);

	virtual void InitializeSynchronizer(BSynchronizer** Synchronizer);
	virtual void Render(TBatch *Batch);
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