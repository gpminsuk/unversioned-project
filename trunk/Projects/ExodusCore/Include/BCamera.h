#pragma once

#include "InputDefine.h"

#include "TDataTypes.h"

class BViewport;

class BCamera
{
public:
	BCamera(BViewport* InParentViewport, ECameraMode InCameraMode);
	virtual ~BCamera(void);

	class BThing* m_Subject;

	ECameraMode m_CameraMode;
	BViewport* ParentViewport;

	TMatrix m_RotationMatrix;
	float m_Distance, m_Pi, m_Theta;
	float m_X, m_Y; // orthogonal

	bool m_bIsUpdated;

	void Tick(unsigned long dTime);
	
	virtual bool ShouldUpdate();

	virtual void InputMouse(EMouse_Event Event, TMouseInput_Param& Param);
	virtual void InputKey(EKey_Event Event, TKeyInput_Param& Param);

	TVector3 m_Location;
	TVector3 m_LookAt;
	TVector3 m_Up;
};
