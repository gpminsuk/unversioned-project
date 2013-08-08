#pragma once

#include "AObject.h"
#include "TDataTypes.h"

class RTexture: public AObject
{
DECLARE_CLASS(RTexture,)
	protected:
	enum ETextureMemoryType
	{
		TMT_SYSTEM,
		TMT_MANAGED,
		TMT_VIDEO
	};
	int Width;
	int Height;
	ETextureMemoryType MemoryType;

	virtual bool Access(AAccessor& Accessor);
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
