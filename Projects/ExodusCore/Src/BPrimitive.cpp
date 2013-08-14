#include "StdAfx.h"
#include "BPrimitive.h"
#include "BRenderingBatch.h"

#include "RResource.h"

IMPLEMENT_CLASS(BPrimitive);

BPrimitive::BPrimitive() {
}

BPrimitive::~BPrimitive(void) {
    Draws.Clear();
}

BDraw::BDraw()
    :
    pBuffer(0) {
}

BDraw::~BDraw() {
    delete pBuffer;
}
