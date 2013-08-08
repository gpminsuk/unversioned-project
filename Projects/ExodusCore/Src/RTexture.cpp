#include "stdafx.h"
#include "RTexture.h"

IMPLEMENT_CLASS(RTexture);

RTextureBuffer* GDefaultTexture;

bool RTexture::Access(AAccessor& Accessor) {
    __super::Access(Accessor);
    return true;
}
