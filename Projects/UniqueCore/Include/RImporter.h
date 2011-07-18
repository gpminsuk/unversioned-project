#pragma once

#include "TDataTypes.h"
#include "RResource.h"

class AObject;

class RImporter
{
public:
	virtual bool Import(TString& Filename) = 0;
};