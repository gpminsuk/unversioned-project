#pragma once

#include "TDataTypes.h"
#include "InputDefine.h"

class BThing;
class BLight;
class BUIPane;

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

	TArray<BUIPane*> m_UIPanes;
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
	void Render(BThing* pThing);
	void RenderUIPane(BUIPane* pUIPane);
	
	void SortTemplates();

	virtual void UpdateViewport() = 0;
	virtual TVector3 GetViewportOrigin() = 0;
	virtual void InputMouse(EMouse_Event Event, TMouseInput_Param& Param) = 0;
	virtual void InputKey(EKey_Event Event, TKeyInput_Param& Param) = 0;
	virtual void InputChar() = 0;
private:

	static int PrimitiveCompare(BPrimitive* tb1, BPrimitive* tb2);
};
