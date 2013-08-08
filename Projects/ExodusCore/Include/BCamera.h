#pragma once

#include "InputDefine.h"

#include "TDataTypes.h"

class BCamera
{
public:
	BCamera();
	virtual ~BCamera(void);

	class BThing* m_Subject;

	ECamera_Mode::Enum m_CameraMode;

	TMatrix m_RotationMatrix;
	float m_Distance, m_Pi, m_Theta;

	bool m_bIsUpdated;

	void Tick(unsigned long dTime);

	virtual bool ShouldUpdate();

	virtual void InputMouse(EMouse_Event Event, TMouseInput_Param& Param);
	virtual void InputKey(EKey_Event Event, TKeyInput_Param& Param);

	TVector3 m_Location;
	TVector3 m_LookAt;
	TVector3 m_Up;
};
