#pragma once

#include "BLight.h"

class BPrimitive;

class CPointLight: public BLight
{
public:
	CPointLight(void);
	virtual ~CPointLight(void);

	TArray<BPrimitive*> m_AffectedObjects;
};
