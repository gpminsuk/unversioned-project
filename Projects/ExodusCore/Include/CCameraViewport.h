#pragma once

#include <windows.h>

#include "TDataTypes.h"
#include "BViewport.h"

class BCamera;

class CCameraViewport: public BViewport
{
public:
DECLARE_CLASS(CCameraViewport,)

public:
	CCameraViewport();
	virtual ~CCameraViewport();

	BCamera *m_pCamera;

	virtual bool Tick(unsigned long Time);
	virtual void SetProperty(TString& PropertyName, TString& Value);

	virtual void UpdateViewport();
	virtual TVector3 GetViewportOrigin();
	virtual void InputMouse(EMouse_Event Event, TMouseInput_Param& Param);
	virtual void InputKey(EKey_Event Event, TKeyInput_Param& Param);
	virtual void InputChar();
};
