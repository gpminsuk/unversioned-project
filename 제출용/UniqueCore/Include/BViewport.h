#pragma once

#include "TDataTypes.h"
#include "InputDefine.h"

class BThing;
class BComponent;
class BLight;
class BRenderingBatchManager;

enum ESceneFlag
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

	BRenderingBatchManager* BatchManager;
	TArray<BLight*>		m_Lights;

	TMatrix m_ViewMatrix;
	TMatrix m_ProjectionMatrix;

	unsigned int m_Width;
	unsigned int m_Height;

	unsigned long VisibleScenes;

	void Clear();
	void Render(BComponent* pComponent);
	void Render(BThing* pThing);
	void RemoveRender(BComponent* pComponent);
	void RemoveRender(BThing* pThing);
	
	void SortTemplates();

	virtual void UpdateViewport() = 0;
	virtual TVector3 GetViewportOrigin() = 0;
	virtual void InputMouse(EMouse_Event Event, TMouseInput_Param& Param) = 0;
	virtual void InputKey(EKey_Event Event, TKeyInput_Param& Param) = 0;
	virtual void InputChar() = 0;
};
