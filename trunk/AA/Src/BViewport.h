#pragma once

#include "TDataTypes.h"

class BPrimitive;
class BLight;

enum E_SceneType
{
	Scene_World			= 1 << 0,
	Scene_Collision		= 1 << 1,
};

class BViewport
{
public:
	BViewport(void);
	virtual ~BViewport(void);

	void operator =(BViewport& vp);

	TArray<BPrimitive*> m_OpaquePrimitives;
	TArray<BPrimitive*> m_TranslucentPrimitives;
	TArray<BLight*>		m_Lights;

	TBatch m_Batches;
	TBatch m_LineBatch;
	TBatch m_ParticleBatch;

	TMatrix m_ViewMatrix;
	TMatrix m_ProjectionMatrix;

	unsigned int m_Width;
	unsigned int m_Height;

	unsigned long VisibleScenes;

	void Clear();
	void Render(BPrimitive* pTemplate);
	
	void SortTemplates();
private:

	static int PrimitiveCompare(BPrimitive* tb1, BPrimitive* tb2);
};
