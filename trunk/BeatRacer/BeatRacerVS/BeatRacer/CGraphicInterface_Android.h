#pragma once

#include "CGraphicInterface.h"

class CGraphicInterface_Android : public CGraphicInterface
{
public:
	virtual void Initialize();
	virtual void Destroy();
	virtual void DrawBitmap2D(CBitmapResource* Resource, int X, int Y, int Width, int Height);
};