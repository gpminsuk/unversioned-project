#include "stdafx.h"
#include "CUILabel.h"

void CUILabelPrimitive::DrawUI(RTextureBuffer* RenderTarget)
{
	GFontDrawer->DrawString(Text, RenderTarget);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

CUILabelComponent::CUILabelComponent()
:	Width(100),
	Height(100)
{
}

CUILabelComponent::~CUILabelComponent()
{

}