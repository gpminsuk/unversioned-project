#include "stdafx.h"
#include "RTexture.h"

#include "BDriver.h"

IMPLEMENT_CLASS(RTexture);

RTextureBuffer* GDefaultTexture;

bool RTexture::Access(AAccessor& Accessor) {
    __super::Access(Accessor);

	Accessor << FileName;
	if(Accessor.IsLoading()) {
		Buffer = GDriver->CreateTextureBuffer(FileName);
	}
    return true;
}
