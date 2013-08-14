#pragma once

#include "TDataTypes.h"
#include "BThing.h"

class RShader;

class BLight: public BThing
{
public:
	TVector3 Direction;

	BLight(void);
	virtual ~BLight(void);

	virtual void ConfigureShader(RShader* InPixelShader);
};
