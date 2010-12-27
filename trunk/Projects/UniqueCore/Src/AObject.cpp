#include "StdAfx.h"
#include "AObject.h"

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