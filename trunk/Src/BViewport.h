#pragma once

#include "TDataTypes.h"

class TPrimitiveTemplateBase;
class BLight;

enum E_RenderType
{
	RT_BASE_PASS = 0,
	RT_TRANSLUCENT = 1
};

class BViewport
{
public:
	BViewport(void);
	virtual ~BViewport(void);

	void operator =(BViewport& vp);

	TArray<TPrimitiveTemplateBase*> m_OpaquePrimitives;
	TArray<TPrimitiveTemplateBase*> m_TranslucentPrimitives;
	TArray<BLight*>					m_Lights;

	TBatch m_Batches;

	TMatrix m_ViewMatrix;
	TMatrix m_ProjectionMatrix;

	void Clear();
	void Render(TPrimitiveTemplateBase* pTemplate, E_RenderType RenderType = RT_BASE_PASS);
	
	void SortTemplates();
private:

	static int PrimitiveCompare(TPrimitiveTemplateBase* tb1, TPrimitiveTemplateBase* tb2);
};
