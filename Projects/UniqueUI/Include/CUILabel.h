#pragma once

#include "TDataTypes.h"
#include "BUIPane.h"

class CUILabel : public BUIPane
{
public:
	TString Text;

	virtual void DrawUI(RTextureBuffer* RenderTarget);
};