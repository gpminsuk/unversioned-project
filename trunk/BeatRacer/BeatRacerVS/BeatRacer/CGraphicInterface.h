#pragma once

class CBitmapResource
{
};

class CGraphicInterface
{
public:
	virtual void Initialize() = 0;
	virtual void Destroy() = 0;
	virtual void DrawBitmap2D(CBitmapResource* Resource, int X, int Y, int Width, int Height) = 0;
};