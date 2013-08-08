#pragma once

#include "AObject.h"

#include "TDataTypes.h"

class TLine
{
public:
	TVector3 Point1, Point2;
};

class BLineBatcher // : public AObject
{
public:
	BLineBatcher(void);
	virtual ~BLineBatcher(void);

	TArray<TLine> Lines;

	void AddLine(TVector3 Point1, TVector3 Point2);
	void Clear();
};

extern BLineBatcher* GLineBatcher;
