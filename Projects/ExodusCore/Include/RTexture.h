#pragma once

#include "RAsset.h"
#include "TDataTypes.h"

class RTexture: public RAsset
{
DECLARE_CLASS(RTexture,)
public:
	class RTextureBuffer* Buffer;
	protected:
	enum ETextureMemoryType
	{
		TMT_SYSTEM,
		TMT_MANAGED,
		TMT_VIDEO
	};
	TString FileName;
	int Width;
	int Height;
	ETextureMemoryType MemoryType;

	virtual bool Access(AAccessor& Accessor);
	TArray<TString> GetCreatablePrimitiveClassNames() {
		TArray<TString> Ret;
		return Ret;
	}
};

class RTextureBuffer
{
public:
	RTextureBuffer() {
	}
	virtual ~RTextureBuffer() {
	}

	unsigned int Width;
	unsigned int Height;

	virtual struct TLockedRect Lock() = 0;
	virtual bool Unlock() = 0;
};
