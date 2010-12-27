#pragma once

#include "TDataTypes.h"

#include "AClass.h"

class ACriticalSection;

class USyncDataClass_
{
};

class AObject
{
	DECLARE_CLASS(AObject, CLASS_Abstract)

	bool m_bIsUpdated;
public:
	virtual bool Tick(unsigned long Time);
protected:

	AObject(void);
	virtual ~AObject(void);

	unsigned int m_iObjectId;		//오브젝트마다 고유의 아이디가있음
};
