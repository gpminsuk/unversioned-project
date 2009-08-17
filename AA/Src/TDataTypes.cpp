#include "stdafx.h"
#include "TDataTypes.h"
#include "RResource.h"

#include "BPrimitive.h"

int TBatch::GetNumIndices()
{
	int NumIndices = 0;
	for(unsigned int i=0;i<m_pTemplates.Size();++i)
	{
		for(unsigned int j=0;j<m_pTemplates(i)->Primitives.Size();++j)
			NumIndices += m_pTemplates(i)->Primitives(j)->GetNumIndices();
	}
	return NumIndices;
}