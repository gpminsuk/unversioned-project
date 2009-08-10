#pragma once

#include "AObject.h"

#include "TDataTypes.h"

class BPrimitive : public AObject
{
public:
	BPrimitive(void);
	~BPrimitive(void);

	TVector4 qTranslation;
	TQuaternion qRotation;
	TVector4 qScale;

	TArray<TPrimitive*> Primitives;

	struct TBatchInfo
	{
		short BatchNum;
		short SlotNum;
	};
	TBatchInfo BatchInfo;
};
