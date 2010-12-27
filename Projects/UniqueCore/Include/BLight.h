#pragma once

#include "TDataTypes.h"
#include "BThing.h"

class BLight : public BThing
{
public:
	TVector3 Direction;

	BLight(void);
	virtual ~BLight(void);
};
