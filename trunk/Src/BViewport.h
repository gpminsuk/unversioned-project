#pragma once

#include "TDataTypes.h"

class TPrimitiveTemplateBase;

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

	TPrimitiveTemplateBase** m_BasePrimitives;
	int	m_nBasePrimitives;
	TPrimitiveTemplateBase** m_AlphaPrimitives;
	int m_nAlphaPrimitives;

	TBatch m_Batches;

	TMatrix m_ViewMatrix;
	TMatrix m_ProjectionMatrix;

	void Clear();
	void Render(TPrimitiveTemplateBase* pTemplate, E_RenderType RenderType = RT_BASE_PASS);
	
	void SortTemplates();
private:

	static int PrimitiveCompare(TPrimitiveTemplateBase* tb1, TPrimitiveTemplateBase* tb2);
};
