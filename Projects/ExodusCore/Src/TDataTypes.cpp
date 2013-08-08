#include "stdafx.h"
#include "TDataTypes.h"
#include "RResource.h"

#include "BPrimitive.h"

TMatrix TMatrix::Identity;

AAccessor::AAccessor()
    :
    bIsLoading(false),
    bIsSaving(false) {
}

AAccessor::~AAccessor() {
    if (FilePointer) {
        fclose(FilePointer);
    }
}

bool AAccessor::IsValid() {
    return (FilePointer == 0) ? false : true;
}

AAccessor& AAccessor::operator<<(class AObject*& A) {
    A->Access(*this);
    return *this;
}
