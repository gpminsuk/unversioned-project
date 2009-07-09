#pragma once

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

	BThing*	m_Subject;

	ECamera_Mode m_CameraMode;

	TQuaternion m_Rotation;
	float m_Distance;

	virtual void UpdateCamera();
	virtual bool ShouldUpdate();

	virtual void InputMouse(EMouse_Event Event, TMouseInput_Param& Param);
	virtual void InputKey(EKey_Event Event, TKeyInput_Param& Param);

	TVector3 m_LookAt;
	TVector3 m_Up;
};