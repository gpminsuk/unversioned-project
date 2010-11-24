#pragma once

#include "RResource.h"

class BViewport;
class BPrimitive;
class BRenderingBatch;

enum ETextureUsage
{
	TexUsage_RenderTarget,
	TexUsage_DepthStencil,
	TexUsage_Dynamic,
};

enum EPixelFormat
{
	PixelFormat_A8R8G8B8,
	PixelFormat_A16B16G16R16,
	PixelFormat_D24S8,
};

enum ECullMode
{
	CullMode_None,
	CullMode_CW,
	CullMode_CCW
};

enum EFillMode
{
	FillMode_Point,
	FillMode_Wireframe,
	FillMode_Solid
};

enum EPrimitiveType
{
	PrimitiveType_PointList,
	PrimitiveType_LineList,
	PrimitiveType_LineStrip,
	PrimitiveType_TriangleList,
	PrimitiveType_TriangleStrip,
	PrimitiveType_TriangleFan,
};

enum EVertexDeclarationType
{
	VertexType_Position		= 1,
	VertexType_UV			= 1 << 1,
	VertexType_Normal		= 1 << 2,
	VertexType_End			= 1 << 3,
};

enum EStencilOperation
{
	StencilOp_Keep,
	StencilOp_Zero,
	StencilOp_Replace,
	StencilOp_Incrsat,
	StencilOp_Decrsat,
	StencilOp_Invert,
	StencilOp_Incr,
	StencilOp_Decr
};

enum ECompareFunction
{
	CompareFunc_Never,
	CompareFunc_Less,
	CompareFunc_Equal,
	CompareFunc_LessEqual,
	CompareFunc_Greater,
	CompareFunc_NotEqual,
	CompareFunc_GreaterEqual,
	CompareFunc_Always,
};

struct TStencilState
{
	bool EnableStencil;
	ECompareFunction StencilTest;
	EStencilOperation StencilFailOp;
	EStencilOperation StencilZFailOp;
	EStencilOperation StencilZPassOp;
	bool EnableTwoSideStencil;
	ECompareFunction BackFaceStencilTest;
	EStencilOperation BackFaceStencilFailOp;
	EStencilOperation BackFaceStencilZFailOp;
	EStencilOperation BackFaceStencilZPassOp;
	unsigned long StencilReadMask;
	unsigned long StencilWriteMask;
	unsigned long StencilRef;

	TStencilState() : EnableStencil(false), StencilTest(CompareFunc_Always), StencilFailOp(StencilOp_Keep), StencilZFailOp(StencilOp_Keep), StencilZPassOp(StencilOp_Keep),
						EnableTwoSideStencil(false), BackFaceStencilTest(CompareFunc_Always), BackFaceStencilFailOp(StencilOp_Keep), BackFaceStencilZFailOp(StencilOp_Keep), BackFaceStencilZPassOp(StencilOp_Keep),
						StencilReadMask(0xFFFFFFFF), StencilWriteMask(0xFFFFFFFF), StencilRef(0) {}

	TStencilState(bool _EnableStencil, ECompareFunction _StencilTest=CompareFunc_Always, EStencilOperation _StencilFailOp=StencilOp_Keep, EStencilOperation _StencilZFailOp=StencilOp_Keep, EStencilOperation _StencilZPassOp=StencilOp_Keep, 
		bool _EnableTwoSideStencil=false, ECompareFunction _BackFaceStencilTest=CompareFunc_Always, EStencilOperation _BackFaceStencilFailOp=StencilOp_Keep, EStencilOperation _BackFaceStencilZFailOp=StencilOp_Keep, EStencilOperation _BackFaceStencilZPassOp=StencilOp_Keep, 
		unsigned long _StencilReadMask=0xFFFFFFFF, unsigned long _StencilWriteMask=0xFFFFFFFF, unsigned long _StencilRef=0)	: 
		EnableStencil(_EnableStencil),
		StencilTest(_StencilTest),
		StencilFailOp(_StencilFailOp),
		StencilZFailOp(_StencilZFailOp),
		StencilZPassOp(_StencilZPassOp),
		EnableTwoSideStencil(_EnableTwoSideStencil),
		BackFaceStencilTest(_BackFaceStencilTest),
		BackFaceStencilFailOp(_BackFaceStencilFailOp),
		BackFaceStencilZFailOp(_BackFaceStencilZFailOp),
		BackFaceStencilZPassOp(_BackFaceStencilZPassOp),
		StencilReadMask(_StencilReadMask),
		StencilWriteMask(_StencilWriteMask),
		StencilRef(_StencilRef)
	{ }
};

struct TDepthState
{
	bool EnableDepthWrite;
	ECompareFunction DepthTest;

	TDepthState() : EnableDepthWrite(true), DepthTest(CompareFunc_LessEqual) {}
	TDepthState(bool _EnableDepthWrite, ECompareFunction _DepthTest) :
		EnableDepthWrite(_EnableDepthWrite),
		DepthTest(_DepthTest)
	{ }
};

struct TLockedRect
{
	unsigned int Pitch;
	void* pBits;
};

class BDriver
{
public:
	BDriver();
	virtual ~BDriver();

	virtual bool CreateDriver() = 0;
	virtual bool DestroyDriver() = 0;

	virtual bool SetStreamSource(RDynamicPrimitiveBuffer* PrimitiveBuffer) = 0;
	virtual bool SetIndices(RDynamicPrimitiveBuffer* PrimitiveBuffer) = 0;
	virtual bool SetVertexDeclaration(unsigned long Type) = 0;

	virtual bool DrawIndexedPrimitiveUP(EPrimitiveType PrimitiveType, unsigned int NumVertices, unsigned int PrimCount, void* pIndices, unsigned int IndexStride, void* pVertices, unsigned int VertexStride) = 0;
	virtual bool DrawIndexedPrimitive(EPrimitiveType PrimitiveType, unsigned int NumVertices, unsigned int PrimCount) = 0;
	virtual bool DrawPrimitiveUP(EPrimitiveType PrimitiveType, unsigned int NumVertices, void* pVertices, unsigned int VertexStride) = 0;
	virtual bool DrawPrimitive(EPrimitiveType PrimitiveType, unsigned int PrimCount) = 0;

	virtual bool SetTexture(int nStage, RTextureBuffer* pTexture) = 0;

	virtual RDynamicPrimitiveBuffer* CreatePrimitiveBuffer(BRenderingBatch* pBatch) = 0;
	virtual RTextureBuffer* CreateTextureBuffer() = 0;

	virtual bool BeginScene() = 0;
	virtual bool EndScene() = 0;

	virtual bool Clear(bool bClearColor = true, unsigned long Color = 0x00000000, bool bClearDepth = false, float Depth = 0.0f, bool bClearStencil = false, unsigned long Stencil = 0.0f) = 0;

	virtual bool CompileShaderFromFile(RShaderBase *pShader) = 0;
	virtual bool AssembleShaderFromFile(RShaderBase *pShader) = 0;
	virtual bool CompileShaderFromMemory(RShaderBase *pShader) = 0;
	virtual bool AssembleShaderFromMemory(RShaderBase *pShader) = 0;

	virtual RRenderTarget* CreateRenderTarget(unsigned int Width, unsigned int Height, EPixelFormat PixelFormat, ETextureUsage TexUsage = TexUsage_RenderTarget) = 0;
	virtual RTextureBuffer* CreateTextureBuffer(unsigned int Width, unsigned int Height) = 0;
	virtual RTextureBuffer* CreateFontBuffer(unsigned int Width, unsigned int Height) = 0;
	virtual bool SetRenderTarget(unsigned int Idx, RRenderTarget* RT) = 0;
	virtual bool SetDepthStencilSurface(RRenderTarget* RT) = 0;

	virtual RRenderTarget* GetBackBuffer() = 0;
	virtual bool SetViewport(unsigned int x, unsigned int y, unsigned int Width, unsigned int Height, float MinZ, float MaxZ) = 0;
	virtual bool SetClipRect(unsigned int x, unsigned int y, unsigned int Width, unsigned int Height) = 0;

	////////////////////////////////////////////////////// RenderState //////////////////////////////////////////////////
	EFillMode CurrentFillMode;
	TStencilState CurrentStencilState;
	TDepthState CurrentDepthState;
	ECullMode CurrentCullMode;

	virtual void SetFillMode(EFillMode FM) = 0;
	virtual void SetCullMode(ECullMode& CullMode) = 0;
	virtual void SetStencilState(TStencilState& StencilState) = 0;
	virtual void SetDepthState(TDepthState& DepthState) = 0;	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
};

extern BDriver	*GDriver;