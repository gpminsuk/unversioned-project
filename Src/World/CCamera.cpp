#include "StdAfx.h"
#include "CCamera.h"

CCamera::CCamera(void)
:	m_CameraMode(Free_Mode),
	m_Position(10.0f,10.0f,10.0f),
	m_LookAt(0.0f,0.0f,0.0f),
	m_Up(0.0f,1.0f,0.0f)
{
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
	case Free_Mode1:
	case Free_Mode:
		{
			switch(Event)
			{
			case MOUSE_Move:
				{
					if(Param.bRButtonDown)
					{
						m_Position.y += 0.3f;
					}
				}
			}
		}
		break;
	}
}

void CCamera::InputKey()
{

}
