#include "stdafx.h"
#include "CUILabel.h"

void CUILabelPane::DrawUI(RTextureBuffer* RenderTarget)
{
	GFontDrawer->DrawString(Text, RenderTarget);
}