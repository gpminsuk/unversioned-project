#include "StdAfx.h"
#include "CCamera.h"

CCamera::CCamera(void)
:	m_CameraMode(Free_Mode),
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

void CCamera::InputMouse(EMouse_Event Event, TMouseInput_Param& Param)
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
					}					
				}
				break;
			case MOUSE_Wheel:
				{
					m_Distance += Param.delta/MOUSE_WHEEL_DELTA;
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
				}
				break;
			}
		}
		break;
	}
}

void CCamera::InputKey(EKey_Event Event, TKeyInput_Param& Param)
{
	switch(m_CameraMode)
	{
	case First_Person:
	case Thrid_Person:
	case Free_Mode1:
	case Free_Mode:
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
			if(m_Subject)
				m_LookAt = m_Subject->m_Location;
			m_Location = m_Rotation.TransformVector3D(TVector3(m_Distance,0.0f,0.0f));
			m_Location += m_LookAt;
		}
		break;
	case Free_Mode1:
	case Free_Mode:
		{
			if(GKeyMap['W'])
				m_Location += (m_LookAt - m_Location).Normalize()/10000.0f;
			if(GKeyMap['S'])
				m_Location -= (m_LookAt - m_Location).Normalize()/10000.0f;
			if(GKeyMap['A'])
				m_Location += ((m_LookAt - m_Location).Normalize() ^ m_Up)/10000.0f;
			if(GKeyMap['D'])
				m_Location -= ((m_LookAt - m_Location).Normalize() ^ m_Up)/10000.0f;
			m_LookAt = m_Rotation.TransformVector3D(TVector3(-1.0f,0.0f,0.0f));
			m_LookAt += m_Location;
		}
		break;
	}
	m_bIsUpdated = false;
}

bool CCamera::ShouldUpdate()
{
	switch(m_CameraMode)
	{
	case First_Person:
	case Thrid_Person:
		return m_bIsUpdated || (m_Subject)?m_Subject->m_bIsUpdated:false;
	case Free_Mode1:
	case Free_Mode:
		return m_bIsUpdated || GKeyMap['W'] || GKeyMap['S'] || GKeyMap['A'] || GKeyMap['D'];
	}
	return m_bIsUpdated;
}