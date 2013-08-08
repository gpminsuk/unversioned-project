#include "StdAfx.h"
#include "BPrimitive.h"
#include "BRenderingBatch.h"

#include "RResource.h"

BPrimitive::BPrimitive() {
}

BPrimitive::~BPrimitive(void) {
    Primitives.Clear();
}

TPrimitive::TPrimitive()
    :
    pBuffer(0) {
}

TPrimitive::~TPrimitive() {
    delete pBuffer;
}
