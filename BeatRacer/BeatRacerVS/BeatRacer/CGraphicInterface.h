#pragma once

class CBitmapResource
{
};

class CGraphicInterface
{
public:
	virtual void DrawBitmap2D(CBitmapResource* Resource, int X, int Y, int Width, int Height);
};