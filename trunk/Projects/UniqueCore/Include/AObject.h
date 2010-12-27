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

	unsigned int m_iObjectId;		//������Ʈ���� ������ ���̵�����
};
