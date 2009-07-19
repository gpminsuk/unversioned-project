#pragma once

#include "BLight.h"

class TPrimitiveTemplateBase;

class CPointLight : public BLight
{
public:
	CPointLight(void);
	virtual ~CPointLight(void);

	TArray<TPrimitiveTemplateBase*> m_AffectedObjects;
};
