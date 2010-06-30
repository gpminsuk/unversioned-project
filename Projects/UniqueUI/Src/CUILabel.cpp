#include "stdafx.h"
#include "CUILabel.h"

void CUILabel::DrawUI(RTextureBuffer* RenderTarget)
{
	GFontDrawer->DrawString(Text, RenderTarget);
}