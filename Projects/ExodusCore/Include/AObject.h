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

    unsigned int m_iObjectId;		//������Ʈ���� ������ ���̵�����
};

template<class T>
T* LoadAsset(TString& ResourceName) {
    RAsset* Asset = (RAsset*) AObject::CreateObject(ResourceName);
	Asset->Path = ResourceName;
	return (T*)Asset;
}

void SaveAsset(class RAsset* Asset, TString& ResourceName);