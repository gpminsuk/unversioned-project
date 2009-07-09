#pragma once

#include "AObject.h"
#include "TDataTypes.h"

class BThing : public AObject
{
public:
	BThing(void);
	virtual ~BThing(void);

	TVector3 m_Location;
};
