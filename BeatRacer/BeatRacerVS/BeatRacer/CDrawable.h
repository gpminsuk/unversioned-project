#pragma once

class CGraphicInterface;

class CDrawable
{
public:
	virtual void Draw(CGraphicInterface* GI) = 0;
};