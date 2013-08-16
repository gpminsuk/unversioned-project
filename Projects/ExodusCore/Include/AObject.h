#pragma once

#include "TDataTypes.h"

#include "AClass.h"

class ACriticalSection;

class AReadAccessor: public AAccessor {
public:
    AReadAccessor(TString& Filename);

    void Access(void*A, int Size);
};

class AWriteAccessor: public AAccessor {
public:
    AWriteAccessor(TString& Filename);

    void Access(void*A, int Size);
};

class AObject {
    DECLARE_CLASS_BASE(AObject, CLASS_Abstract)

    bool m_bIsUpdated;
public:
    virtual bool Tick(unsigned long Time);
    virtual bool Access(AAccessor& Accessor);
    virtual void SetProperty(TString& PropertyName, TString& Value);
	
    void SaveObject(TString& ResourceName);

    static AObject* CreateObject(TString& ResourceName);
protected:
    AObject(void);
    virtual ~AObject(void);

    unsigned int m_iObjectId;		//오브젝트마다 고유의 아이디가있음
};

extern TArray<TString> GLoadedAssetFilenames;
extern TArray<class RAsset*> GLoadedAssets;

template<class T>
T* LoadAsset(TString& ResourceName) {
	for(unsigned int i=0;i<GLoadedAssetFilenames.Size();++i) {
		if(GLoadedAssetFilenames(i) == ResourceName) {
			return (T*)GLoadedAssets(i);
		}
	}
    RAsset* Asset = (RAsset*) AObject::CreateObject(ResourceName);
	Asset->Path = ResourceName;	
	GLoadedAssetFilenames.AddItem(ResourceName);
	GLoadedAssets.AddItem(Asset);
	return (T*)Asset;
}

void SaveAsset(class RAsset* Asset, TString& ResourceName);