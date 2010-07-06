#pragma once

#include "TDataTypes.h"
#include "BPrimitive.h"
#include "BComponent.h"
#include "BSynchronizer.h"

class CUILabelPane : public BPrimitive
{
public:
	TString Text;

	virtual void DrawUI(RTextureBuffer* RenderTarget);
};

class CUILabelComponent : public BComponent
{
public:
};

class CUILableSyncronizer : public BSynchronizer
{
public:
};