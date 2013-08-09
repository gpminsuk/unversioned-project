#pragma once

#include "AObject.h"

#include "TDataTypes.h"
#include "InputDefine.h"

class BThing;
class BComponent;
class BPrimitive;
class BLightComponent;
class BCamera;
class RSwapChain;
class RRenderTarget;

enum ESceneFlag
{
	Scene_World = 1 << 0,
	Scene_Collision = 1 << 1,
};

class BViewport: public AObject
{
public:
	BViewport(unsigned int Width, unsigned int Height);
	virtual ~BViewport(void);

	void operator=(BViewport& vp);
	
	RSwapChain* SwapChain;
	BCamera* Camera;

	TMatrix m_ViewMatrix;
	TMatrix m_ProjectionMatrix;

	unsigned int m_Width;
	unsigned int m_Height;

	unsigned long VisibleScenes;

	void Clear();

	RRenderTarget* GetBackBuffer();
	
	//void Render(BComponent* pComponent);
	//void Remove(BComponent* pComponent);

	void SortTemplates();

	virtual void UpdateViewport();
	virtual TVector3 GetViewportOrigin();
	virtual void InputMouse(EMouse_Event Event, TMouseInput_Param& Param);
	virtual void InputKey(EKey_Event Event, TKeyInput_Param& Param);
	virtual void InputChar();
};
