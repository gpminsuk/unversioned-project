#pragma once

#include "AObject.h"
#include "TDataTypes.h"

class BThing : public AObject
{
public:
	BThing(void);
	virtual ~BThing(void);

	class BPrimitive* Primitives;

	TVector3 m_Location;

	virtual void Tick(unsigned long dTime) = 0;
};
