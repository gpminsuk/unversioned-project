#include "StdAfx.h"
#include "AObject.h"

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

AReadAccessor::AReadAccessor(TString& Filename)
{
	fopen_s(&FilePointer, Filename.GetBuffer(), "rb");
}

void AReadAccessor::Access(void *A, int Size)
{
	fread_s(A, Size, Size, 1, FilePointer);
}

AWriteAccessor::AWriteAccessor(TString& Filename)
{
	fopen_s(&FilePointer, Filename.GetBuffer(), "wb");	
}

void AWriteAccessor::Access(void *A, int Size)
{
	fwrite(A, Size, 1, FilePointer);
}

AObject::AObject(void)
: m_bIsUpdated(true)
{
	m_iObjectId = 0;
}

AObject::~AObject(void)
{
}

bool AObject::Tick(unsigned long Time)
{
	return true;
}

bool AObject::Access(AAccessor& Accessor)
{
	Accessor << m_iObjectId;
	return true;
}

AObject* AObject::CreateObject(AClass* Class, TString& ResourceName)
{
	AObject* Object = Class->Constructor->Construct();	
	AReadAccessor Accessor(ResourceName);
	if(Accessor.IsValid())
	{
		Object->Access(Accessor);
	}
	else
	{

	}
	return Object;
}