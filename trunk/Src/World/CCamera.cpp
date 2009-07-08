#include "StdAfx.h"
#include "CCamera.h"

CCamera::CCamera(void)
:	m_CameraMode(Free_Mode),
	m_Position(10.0f,0.0f,0.0f),
	m_LookAt(0.0f,0.0f,0.0f),
	m_Up(0.0f,1.0f,0.0f),
	m_Distance(10.0f)
{
	UpdateCamera();
	//m_CameraMode = Thrid_Person;
}

CCamera::~CCamera(void)
{
}

void CCamera::InputMouse(EMouse_Event Event, TMouseInput_Param Param)
{
	m_bIsUpdated = true;
	switch(m_CameraMode)
	{
	case First_Person:
	case Thrid_Person:
		{
			switch(Event)
			{
			case MOUSE_Move:
				{
					if(Param.bRButtonDown)
					{
						float dPi = Param.dY/300.0f;
						float dTheta = Param.dX/300.0f;
						m_Rotation.Rotate(TVector3(0.0f,0.0f,1.0f),dPi);
						m_Rotation.Rotate(TVector3(0.0f,1.0f,0.0f),dTheta);
						UpdateCamera();
					}					
				}
				break;
			case MOUSE_Wheel:
				{
					m_Distance += Param.delta/MOUSE_WHEEL_DELTA;
					UpdateCamera();
				}
				break;
			}
		}
		break;
	case Free_Mode1:
	case Free_Mode:
		{
			switch(Event)
			{
			case MOUSE_Move:
				{
					float dPi = Param.dY/300.0f;
					float dTheta = Param.dX/300.0f;
					m_Rotation.Rotate(TVector3(0.0f,0.0f,-1.0f),dPi);
					m_Rotation.Rotate(TVector3(0.0f,-1.0f,0.0f),dTheta);
					UpdateCamera();
				}
				break;
			}
		}
		break;
	}
}

void CCamera::UpdateCamera()
{
	switch(m_CameraMode)
	{
	case First_Person:
	case Thrid_Person:
		{
			m_Position = m_Rotation.TransformVector3D(TVector3(m_Distance,0.0f,0.0f));
			m_Position += m_LookAt;
		}
		break;
	case Free_Mode1:
	case Free_Mode:
		{
			m_LookAt = m_Rotation.TransformVector3D(TVector3(-1.0f,0.0f,0.0f));
			m_LookAt += m_Position;
		}
		break;
	}
}

void CCamera::InputKey()
{

}
