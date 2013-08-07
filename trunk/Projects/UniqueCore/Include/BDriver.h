#pragma once

#include "RResource.h"
#include "RSystemMemory.h"

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

enum EBlendState
{
	BlendState_Zero,
	BlendState_One,
	BlendState_SrcColor,
	BlendState_InvSrcColor,
	BlendState_SrcAlpha,
	BlendState_InvSrcAlpha,
	BlendState_DestAlpha,
	BlendState_InvDestAlpha,
	BlendState_DestColor,
	BlendState_InvDestColor,
	BlendState_SrcAlphaSat,
	BlendState_BothSrcAlpha,
	BlendState_BothInvSrcAlpha,
	BlendState_BlendFactor,
	BlendState_InvBlendFactor,
	BlendState_SrcColor2,
	BlendState_InvSrcColor2,
};

enum EVertexDeclType
{
	DeclType_Float1,
	DeclType_Float2,
	DeclType_Float3,
	DeclType_Float4,
	DeclType_Ubyte4,
	DeclType_Short2,
	DeclType_Short4,
};

enum EVertexDeclUsage
{
	DeclUsage_Position,
	DeclUsage_BlendWeight,
	DeclUsage_BlendIndices,
	DeclUsage_Normal,
	DeclUsage_TexCoord,
	DeclUsage_Tangent,
	DeclUsage_Binormal,
	DeclUsage_Color,
	DeclUsage_Depth,
	DeclUsage_End,
};

enum EBlendOperation
{
	BlendOp_Add,
	BlendOp_Subtract,
	BlendOp_Min,
	BlendOp_Max,
	BlendOp_ReverseSubtract,
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

	TStencilState(bool _EnableStencil = false, ECompareFunction _StencilTest =
			CompareFunc_Always, EStencilOperation _StencilFailOp =
			StencilOp_Keep, EStencilOperation _StencilZFailOp = StencilOp_Keep, EStencilOperation _StencilZPassOp =
			StencilOp_Keep,
			bool _EnableTwoSideStencil = false, ECompareFunction _BackFaceStencilTest =
					CompareFunc_Always, EStencilOperation _BackFaceStencilFailOp =
					StencilOp_Keep, EStencilOperation _BackFaceStencilZFailOp =
					StencilOp_Keep, EStencilOperation _BackFaceStencilZPassOp =
					StencilOp_Keep,
			unsigned long _StencilReadMask = 0xFFFFFFFF, unsigned long _StencilWriteMask =
					0xFFFFFFFF, unsigned long _StencilRef = 0) :
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
	{
	}
};

struct TDepthState
{
	bool EnableDepthWrite;
	ECompareFunction DepthTest;

	TDepthState() :
			EnableDepthWrite(true), DepthTest(CompareFunc_LessEqual) {
	}
	TDepthState(bool _EnableDepthWrite, ECompareFunction _DepthTest) :
			EnableDepthWrite(_EnableDepthWrite),
					DepthTest(_DepthTest)
	{
	}
};

struct TBlendState
{
	bool EnableAlphaBlend;
	EBlendOperation ColorBlendOp;
	EBlendState SrcColorBlendState;
	EBlendState DestColorBlendState;
	EBlendOperation AlphaBlendOp;
	EBlendState SrcAlphaBlendState;
	EBlendState DestAlphaBlendState;

	TBlendState(EBlendOperation _ColorBlendOp = BlendOp_Add, EBlendState _SrcColorBlendState =
			BlendState_One, EBlendState _DestColorBlendState = BlendState_Zero,
			EBlendOperation _AlphaBlendOp = BlendOp_Add, EBlendState _SrcAlphaBlendState =
					BlendState_One, EBlendState _DestAlphaBlendState =
					BlendState_Zero) :
			ColorBlendOp(_ColorBlendOp),
					SrcColorBlendState(_SrcColorBlendState),
					DestColorBlendState(_DestColorBlendState),
					AlphaBlendOp(_AlphaBlendOp),
					SrcAlphaBlendState(_SrcAlphaBlendState),
					DestAlphaBlendState(_DestAlphaBlendState)
	{
		EnableAlphaBlend = ColorBlendOp != BlendOp_Add
				|| SrcAlphaBlendState != BlendState_One
				|| DestColorBlendState != BlendState_Zero ||
				AlphaBlendOp != BlendOp_Add
				|| DestAlphaBlendState != BlendState_One
				|| SrcAlphaBlendState != BlendState_Zero;
	}
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
	virtual bool SetVertexDeclaration(RVertexDeclaration* Decl) = 0;

	virtual bool DrawIndexedPrimitiveUP(EPrimitiveType PrimitiveType, unsigned int NumVertices, unsigned int PrimCount, void* pIndices, unsigned int IndexStride, void* pVertices, unsigned int VertexStride) = 0;
	virtual bool DrawIndexedPrimitive(EPrimitiveType PrimitiveType, unsigned int NumVertices, unsigned int PrimCount) = 0;
	virtual bool DrawPrimitiveUP(EPrimitiveType PrimitiveType, unsigned int NumVertices, void* pVertices, unsigned int VertexStride) = 0;
	virtual bool DrawPrimitive(EPrimitiveType PrimitiveType, unsigned int PrimCount) = 0;

	virtual bool SetTexture(int nStage, RTextureBuffer* pTexture) = 0;

	virtual RDynamicPrimitiveBuffer* CreatePrimitiveBuffer(BRenderingBatch* pBatch) = 0;
	virtual RTextureBuffer* CreateTextureBuffer(const TString& Str) = 0;

	virtual bool BeginScene() = 0;
	virtual bool EndScene() = 0;

	virtual bool Clear(bool bClearColor = true, unsigned long Color = 0x00000000, bool bClearDepth =
			false, float Depth = 0.0f, bool bClearStencil = false, unsigned long Stencil =
			0.0f) = 0;

	virtual bool CompileMaterial(RMaterial* Material) = 0;
	virtual bool AssembleShaderFromFile(RShaderBase *pShader) = 0;
	virtual bool CompileShaderFromMemory(RShaderBase *pShader) = 0;
	virtual bool AssembleShaderFromMemory(RShaderBase *pShader) = 0;

	virtual RRenderTarget* CreateRenderTarget(unsigned int Width, unsigned int Height, EPixelFormat PixelFormat, ETextureUsage TexUsage =
			TexUsage_RenderTarget) = 0;
	virtual RTextureBuffer* CreateTextureBuffer(unsigned int Width, unsigned int Height) = 0;
	virtual RTextureBuffer* CreateFontBuffer(unsigned int Width, unsigned int Height) = 0;
	virtual bool SetRenderTarget(unsigned int Idx, RRenderTarget* RT) = 0;
	virtual bool SetDepthStencilSurface(RRenderTarget* RT) = 0;

	virtual RRenderTarget* GetBackBuffer() = 0;
	virtual bool SetViewport(unsigned int x, unsigned int y, unsigned int Width, unsigned int Height, float MinZ, float MaxZ) = 0;
	virtual bool SetClipRect(unsigned int x, unsigned int y, unsigned int Width, unsigned int Height) = 0;

	virtual void InitializeVertexDecl(RVertexDeclaration* Decl) = 0;

	////////////////////////////////////////////////////// RenderState //////////////////////////////////////////////////
	EFillMode CurrentFillMode;
	TStencilState CurrentStencilState;
	TDepthState CurrentDepthState;
	TBlendState CurrentBlendState;
	ECullMode CurrentCullMode;
	RVertexDeclaration* CurrentVertexDecl;

	virtual void SetFillMode(EFillMode FM) = 0;
	virtual void SetCullMode(ECullMode& CullMode) = 0;
	virtual void SetStencilState(TStencilState& StencilState) = 0;
	virtual void SetDepthState(TDepthState& DepthState) = 0;
	virtual void SetBlendState(TBlendState& BlendState) = 0;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
};

extern BDriver *GDriver;
