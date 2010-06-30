#include "AObject.h"
#include "UFreeTypeDrawer.h"

class BUIPane : public AObject
{
public:
	virtual void DrawUI(RTextureBuffer* RenderTarget) = 0;
};