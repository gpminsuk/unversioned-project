#include "StdAfx.h"
#include "CCamera.h"
#include "CWindowApp.h"

#include "BThing.h"

CCamera::CCamera(void)
:	m_CameraMode(Thrid_Person),
	m_LookAt(0.0f,0.0f,0.0f),
	m_Up(0.0f,1.0f,0.0f),
	m_Distance(10.0f),
	m_Pi(0),
	m_Theta(0),
	m_Subject(0)
{
	//m_Location.x = m_Distance;
	//m_Location.y = m_Distance;
	//m_Location.z = m_Distance;

	m_CameraMode = First_Person;
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
		{
			switch(Event)
			{
			case MOUSE_Move:
				{
					float dY = (Param.dY/100.0f), dX = (Param.dX/100.0f);
					if(m_Pi + dY < (MATH_PI/2.0f) && (m_Pi + dY) > -(MATH_PI/2.0f))
						m_Pi += dY;
					m_Theta += dX;
					GApp->SetMousePos(0.5f, 0.5f, true);
				}
				break;
			}
		}
		break;
	case Thrid_Person:
		{
			switch(Event)
			{
			case MOUSE_Move:
				{
					if(Param.bRButtonDown)
					{
						if(m_Pi + (-Param.dY/300.0f) < (MATH_PI/2.0f) && (m_Pi + -Param.dY/300.0f) > 0.0f)
							m_Pi += -Param.dY/300.0f;
						m_Theta += -Param.dX/300.0f;
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
					float dY = (Param.dY/100.0f), dX = (Param.dX/100.0f);
					if(m_Pi + dY < (MATH_PI/2.0f) && (m_Pi + dY) > -(MATH_PI/2.0f))
						m_Pi += dY;
					m_Theta += dX;
					GApp->SetMousePos(0.5f, 0.5f, true);
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

void CCamera::Tick(unsigned long  dTime)
{
	switch(m_CameraMode)
	{
	case First_Person:
		{
			if(m_Subject)
				m_Location = m_Subject->m_Location + TVector3(0.0f,0.6f,0.0f);

			m_LookAt.x = COSINE(m_Pi)*COSINE(m_Theta)*m_Distance;
			m_LookAt.z = COSINE(m_Pi)*SINE(m_Theta)*m_Distance;
			m_LookAt.y = SINE(m_Pi)*m_Distance;

			m_LookAt += m_Location;
		}
		break;
	case Thrid_Person:
		{
			if(m_Subject)
				m_LookAt = m_Subject->m_Location;

			m_Location.x = COSINE(m_Pi)*COSINE(m_Theta)*m_Distance;			
			m_Location.z = COSINE(m_Pi)*SINE(m_Theta)*m_Distance;
			m_Location.y = SINE(m_Pi)*m_Distance;

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

			m_LookAt.x = COSINE(m_Pi)*COSINE(m_Theta)*m_Distance;
			m_LookAt.z = COSINE(m_Pi)*SINE(m_Theta)*m_Distance;
			m_LookAt.y = SINE(m_Pi)*m_Distance;

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
		return m_bIsUpdated || (!m_Subject)?false:m_Subject->m_bIsUpdated;
	case Free_Mode1:
	case Free_Mode:
		return m_bIsUpdated || GKeyMap['W'] || GKeyMap['S'] || GKeyMap['A'] || GKeyMap['D'];
	}
	return m_bIsUpdated;
}