#pragma once

#include "AObject.h"

#include "TDataTypes.h"
#include "InputDefine.h"

class BThing;
class BComponent;
class BPrimitive;
class BLightComponent;
class BCamera;
class RRenderTarget;

enum EFillMode;
enum ESceneFlag
{
	Scene_World = 1 << 0,
	Scene_Collision = 1 << 1,
};

enum EViewportProjectionType
{
	Projection_Perpective,
	Projection_Orthogonal
};

enum EViewportRenderMode
{
	RenderMode_All,
	RenderMode_Wireframe
};

class BViewport: public AObject
{
public:
	BViewport(unsigned int Width, unsigned int Height, EViewportProjectionType InProjectionType, EViewportRenderMode InRenderMode, ECameraMode InCameraMode);
	virtual ~BViewport(void);

	void operator=(BViewport& vp);

	EViewportProjectionType ProjectionType;
	EViewportRenderMode RenderMode;

	BCamera* Camera;

	TMatrix m_ViewMatrix;

	unsigned int Width;
	unsigned int Height;
	unsigned int X;
	unsigned int Y;

	unsigned long VisibleScenes;

	void Clear();

	virtual void Resize(int InWidth, int InHeight);

	EFillMode GetFillMode();
	void SortTemplates();

	virtual void OnSizeChanged() = 0;
	virtual void UpdateViewport();
	virtual TVector3 GetViewportOrigin();
	virtual void InputMouse(EMouse_Event Event, TMouseInput_Param& Param);
	virtual void InputKey(EKey_Event Event, TKeyInput_Param& Param);
	virtual void InputChar();
};
