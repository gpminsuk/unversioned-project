#include "StdAfx.h"
#include "BCamera.h"
#include "CWindowApp.h"

#include "BThing.h"

BCamera::BCamera(void)
:	m_CameraMode(Thrid_Person),
	m_LookAt(0.0f,0.0f,0.0f),
	m_Up(0.0f,1.0f,0.0f),
	m_Distance(150.0f),
	m_Pi(0),
	m_Theta(0),
	m_Subject(0)
{
	m_Location.x = 0;
	m_Location.y = 0;
	m_Location.z = 0;

	//m_CameraMode = First_Person;
}

BCamera::~BCamera(void)
{
}

void BCamera::InputMouse(EMouse_Event Event, TMouseInput_Param& Param)
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
					
					if(m_Pi + (-Param.dY/100.0f) < (MATH_PI/2.0f-0.001f) && (m_Pi + -Param.dY/100.0f) > -(MATH_PI/2.0f-0.001f))
						m_Pi += -Param.dY/100.0f;
					m_Theta += -Param.dX/100.0f;
									
				}
				break;
			case MOUSE_Wheel:
				{
					m_Distance += Param.delta/MOUSE_WHEEL_DELTA*10.0f;
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
				}
				break;
			}
		}
		break;
	}
}

void BCamera::InputKey(EKey_Event Event, TKeyInput_Param& Param)
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

void BCamera::Tick(unsigned long  dTime)
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
			else
				m_Location = TVector3(0.0f,0.0f,0.0f);

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
				m_Location += (m_LookAt - m_Location).Normalize()/100000.0f;
			if(GKeyMap['S'])
				m_Location -= (m_LookAt - m_Location).Normalize()/100000.0f;
			if(GKeyMap['A'])
				m_Location += ((m_LookAt - m_Location).Normalize() ^ m_Up).Normalize()/100000.0f;
			if(GKeyMap['D'])
				m_Location -= ((m_LookAt - m_Location).Normalize() ^ m_Up).Normalize()/100000.0f;

			m_LookAt.x = COSINE(m_Pi)*COSINE(m_Theta)*m_Distance;
			m_LookAt.z = COSINE(m_Pi)*SINE(m_Theta)*m_Distance;
			m_LookAt.y = SINE(m_Pi)*m_Distance;

			m_LookAt += m_Location;
		}
		break;
	case QuarterView_Mode:
		{
			if(m_Subject)
			{
				m_LookAt = m_Subject->m_Location;				
			}
			else
			{
				m_LookAt = TVector3(0.0f,0.0f,0.0f);
			}			

			m_Location = m_LookAt;
			m_Location.x -= 300.0f;
			m_Location.y += 50.0f;
			m_Location.z += 0.0f;
		}
		break;
	}
	m_bIsUpdated = false;
}

bool BCamera::ShouldUpdate()
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