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

TString TString::Format(const char* format, ...) {
	va_list argList;
	va_start(argList, format);
	TString n = "";
	vsprintf_s(n.Str, format, argList);
	va_end(argList);
	return n;
}

TVector3 TVector3::Slerp(TVector3 Start, TVector3 End, float t)
{
	TVector3 NStart = Start;
	TVector3 NEnd = End;
	double theta = ARCCOSINE(NStart.Normalize() | NEnd.Normalize());
	TVector3 Axis = (End ^ Start).Normalize();
	TQuaternion Rot = TQuaternion::Slerp(TQuaternion(0, 0, 0, 1.0f), TQuaternion(Axis, theta), t);
	TMatrix M = TMatrix::Identity;
	M.Rotate(Rot);		
	return M.TransformVector3(Start);
}