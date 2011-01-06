#pragma once

#include "AObject.h"

#include "TDataTypes.h"
#include "InputDefine.h"

class BThing;
class BComponent;
class BPrimitive;
class BLightComponent;
class BRenderingBatchManager;

enum ESceneFlag
{
	Scene_World			= 1 << 0,
	Scene_Collision		= 1 << 1,
};

class BViewport : public AObject
{
	DECLARE_CLASS(BViewport, CLASS_Abstract)
public:
	BViewport(void);
	virtual ~BViewport(void);

	void operator =(BViewport& vp);

	BRenderingBatchManager* BatchManager;
	TArray<BLightComponent*> Lights;

	TMatrix m_ViewMatrix;
	TMatrix m_ProjectionMatrix;

	unsigned int m_Width;
	unsigned int m_Height;

	unsigned long VisibleScenes;

	void Clear();

	void RenderViewport();

	void RenderLight(BLightComponent* pLightComponent);
	void RemoveLight(BLightComponent* pLightComponent);

	void Render(BPrimitive* pPrimitive);
	void Render(BComponent* pComponent);
	void Render(BThing* pThing);
	void Remove(BPrimitive* pPrimitive);
	void Remove(BComponent* pComponent);
	void Remove(BThing* pThing);
	
	void SortTemplates();

	virtual void UpdateViewport() = 0;
	virtual TVector3 GetViewportOrigin() = 0;
	virtual void InputMouse(EMouse_Event Event, TMouseInput_Param& Param) = 0;
	virtual void InputKey(EKey_Event Event, TKeyInput_Param& Param) = 0;
	virtual void InputChar() = 0;
};
