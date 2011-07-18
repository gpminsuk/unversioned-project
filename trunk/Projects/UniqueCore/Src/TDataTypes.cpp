#include "stdafx.h"
#include "TDataTypes.h"
#include "RResource.h"

#include "BPrimitive.h"

TMatrix TMatrix::Identity;

AAccessor::~AAccessor()
{
	if(FilePointer)
	{
		fclose(FilePointer);
	}
}

bool AAccessor::IsValid()
{
	return (FilePointer == 0)?false:true;
}
