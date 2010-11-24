#pragma once

#include "TDataTypes.h"
#include "BPrimitive.h"
#include "BComponent.h"

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