#include "StdAfx.h"
#include "AObject.h"
#include "RResource.h"
#include "RAsset.h"

TArray<TString> GLoadedAssetFilenames;
TArray<RAsset*> GLoadedAssets;

AReadAccessor::AReadAccessor(TString& Filename) {
    bIsLoading = true;
    fopen_s(&FilePointer, Filename.GetBuffer(), "rb");
}

void AReadAccessor::Access(void *A, int Size) {
    fread_s(A, Size, Size, 1, FilePointer);
}

AWriteAccessor::AWriteAccessor(TString& Filename) {
    bIsSaving = true;
    fopen_s(&FilePointer, Filename.GetBuffer(), "wb");
}

void AWriteAccessor::Access(void *A, int Size) {
    fwrite(A, Size, 1, FilePointer);
}

AObject::AObject(void)
    :
    m_bIsUpdated(true) {
    m_iObjectId = 0;
}

AObject::~AObject(void) {
}

bool AObject::Tick(unsigned long Time) {
    return true;
}

bool AObject::Access(AAccessor& Accessor) {
    Accessor << m_iObjectId;
    return true;
}

void AObject::SetProperty(TString& PropertyName, TString& Value) {

}

TString AObject::GetProperty(TString& PropertyName) {
	return TString();
}

AObject* AObject::CreateObject(TString& ResourceName) {
    AReadAccessor Accessor(ResourceName);
	TString ClassName;
	Accessor << ClassName;
	AClass* Class = ::GetClass<AClass>(ClassName);
	AObject* Object = Class->Constructor->Construct();
    if (Accessor.IsValid()) {
        Object->Access(Accessor);
    } else {

    }
    return Object;
}

void AObject::SaveObject(TString& ResourceName) {
    AWriteAccessor Accessor(ResourceName);
	Accessor << GetClass()->ClassName;
    if (Accessor.IsValid()) {
        Access(Accessor);
    }
}

void SaveAsset(RAsset* Asset, TString& ResourceName) {
	Asset->Path = ResourceName;
	Asset->SaveObject(ResourceName);
}