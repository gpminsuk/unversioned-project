#pragma once
#include "BThing.h"

class CTerrain : public BThing
{
public:
	CTerrain(void);
	~CTerrain(void);

	virtual void Tick(unsigned long dTime);
};
