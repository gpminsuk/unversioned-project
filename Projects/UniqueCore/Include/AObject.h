#pragma once

#include "TDataTypes.h"

#include "AClass.h"

class ACriticalSection;

class AReadAccessor : public AAccessor
{
public:
	AReadAccessor(TString& Filename);

	void Access(void*A, int Size);
};

class AWriteAccessor : public AAccessor
{
public:
	AWriteAccessor(TString& Filename);

	void Access(void*A, int Size);
};

class AObject
{
	DECLARE_CLASS(AObject, CLASS_Abstract)

	bool m_bIsUpdated;
public:
	virtual bool Tick(unsigned long Time);
	virtual bool Access(AAccessor& Accessor);

	void SaveObject(TString& ResourceName);

	static AObject* CreateObject(AClass* Class, TString& ResourceName);
protected:

	AObject(void);
	virtual ~AObject(void);

	unsigned int m_iObjectId;		//오브젝트마다 고유의 아이디가있음

	friend AAccessor& operator<<( AAccessor& Ar, AObject* A )
	{
		A->Access(Ar);
		return Ar;
	}
};

template <class T>
T* LoadResource(TString& ResourceName)
{
	return (T*)AObject::CreateObject(T::Class(), ResourceName);
}