#pragma once

#include "BPrimitive.h"

class TBoxPrimitive : public TPrimitive
{
public:
	TBoxPrimitive(ERenderType _RenderType, EGeometrySideType _BoxSideType);
};

class CBoxPrimitive : public BPrimitive
{
public:
	CBoxPrimitive(ERenderType _RenderType, EGeometrySideType _BoxSideType);
	~CBoxPrimitive(void);

	EGeometrySideType SideType;

	virtual void ConfigureVertexBuilder();
	virtual void ConfigurePixelShader(RPixelShader* PixelShader);
	virtual void ConfigureVertexShader(RVertexShader* VertexShader);
	virtual unsigned int FillDynamicVertexBuffer(char** pData);
	virtual unsigned int FillDynamicIndexBuffer(TIndex16** pData, unsigned short* BaseIndex);
	virtual unsigned int GetNumIndices();
};
