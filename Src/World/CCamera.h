#pragma once

#include "TDataTypes.h"
#include "InputDefine.h"
#include "BThing.h"

enum ECamera_Mode
{
	First_Person,
	Thrid_Person,
	Free_Mode,	
	Free_Mode1,	
};

class CCamera : public BThing
{
public:
	CCamera(void);
	virtual ~CCamera(void);

	ECamera_Mode m_CameraMode;

	TVector3 m_Position;
	TVector3 m_LookAt;
	TVector3 m_Up;

	virtual void InputMouse(EMouse_Event Event, TMouseInput_Param Param);
	virtual void InputKey();
};