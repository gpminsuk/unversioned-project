#include "StdAfx.h"
#include "BCamera.h"
#include "CWindowsApplication.h"

#include "BDriver.h"
#include "BThing.h"
#include "BViewport.h"

BCamera::BCamera(BViewport* InParentViewport, ECameraMode InCameraMode)
    :
    m_LookAt(0.0f, 0.0f, 0.0f),
    m_Up(0.0f, 1.0f, 0.0f),
    m_Distance(30.0f),
    m_Pi(0),
    m_Theta(0),
    m_Subject(0),
	m_CameraMode(InCameraMode),
	m_X(0),
	m_Y(0),
	ParentViewport(InParentViewport) {
    m_Location.x = 0;
    m_Location.y = 0;
    m_Location.z = 0;
}

BCamera::~BCamera(void) {
}

void BCamera::InputMouse(EMouse_Event Event, TMouseInput_Param& Param) {
    m_bIsUpdated = true;
    switch (m_CameraMode) {
    case CameraMode_FirstPerson: {
        switch (Event) {
        case MOUSE_Move: {
            float dY = (Param.dY / 100.0f), dX = (Param.dX / 100.0f);
            if (m_Pi + dY < (MATH_PI / 2.0f) && (m_Pi + dY) > -(MATH_PI / 2.0f))
                m_Pi += dY;
            m_Theta += dX;
        }
        break;
        }
    }
    break;
    case CameraMode_ThridPerson: {
        switch (Event) {
        case MOUSE_Move: {
            if (m_Pi + (-Param.dY / 100.0f) < (MATH_PI / 2.0f - 0.001f)
                    && (m_Pi + -Param.dY / 100.0f) > -(MATH_PI / 2.0f - 0.001f))
                m_Pi += -Param.dY / 100.0f;
            m_Theta += -Param.dX / 100.0f;

        }
        break;
        case MOUSE_Wheel: {
            m_Distance += Param.delta / MOUSE_WHEEL_DELTA * 10.0f;
        }
        break;
        }
    }
    break;
	case CameraMode_Editor: {
		switch (Event) {
		case MOUSE_Move:
			if(Param.bRButtonDown) {
				float dY = -(Param.dY / 100.0f), dX = (Param.dX / 100.0f);
				if (m_Pi + dY < (MATH_PI / 2.0f) && (m_Pi + dY) > -(MATH_PI / 2.0f))
					m_Pi += dY;
				m_Theta += dX;
			}
			if(Param.bMButtonDown) {
				TVector3 Up = m_Up.Normalize();
				Up = Up * -Param.dY;
				TVector3 Right = ((m_LookAt - m_Location).Normalize() ^ m_Up).Normalize();
				Right = Right * -Param.dX;
				m_Location += (Up + Right) / 10.f;
				m_LookAt += (Up + Right) / 10.f;
			}
			break;
		case MOUSE_Wheel: {
			m_Distance *= (1.0f + (Param.delta / MOUSE_WHEEL_DELTA * 0.1f));
			if(m_Distance <= 10) {
				m_Distance = 10;
			}
		}
		break;
		}
	}
	break;
    case CameraMode_FreeMode: {
        switch (Event) {
        case MOUSE_Move: {
            float dY = (Param.dY / 100.0f), dX = (Param.dX / 100.0f);
            if (m_Pi + dY < (MATH_PI / 2.0f) && (m_Pi + dY) > -(MATH_PI / 2.0f))
                m_Pi += dY;
            m_Theta += dX;
        }
        break;
        }
	}
	break;
	case CameraMode_Back:
	case CameraMode_Front:
	case CameraMode_Top:
	case CameraMode_Bottom:
	case CameraMode_Left:
	case CameraMode_Right:
		switch (Event) {
		case MOUSE_Move:
			if(Param.bRButtonDown) {				
				m_X -= Param.dX * m_Distance / ParentViewport->Width;
				m_Y += Param.dY * m_Distance / ParentViewport->Height;
			}
		break;
		case MOUSE_Wheel:
			m_Distance *= 1.0f + (Param.delta / MOUSE_WHEEL_DELTA / 10.0f);
		break;
		}
    break;
    }
}

void BCamera::InputKey(EKey_Event Event, TKeyInput_Param& Param) {
    switch (m_CameraMode) {
    case CameraMode_FirstPerson:
    case CameraMode_ThridPerson:
    case CameraMode_Editor:
    case CameraMode_FreeMode:
        break;
    }
}

void BCamera::Tick(unsigned long dTime) {
    switch (m_CameraMode) {
    case CameraMode_FirstPerson: {
        if (m_Subject)
            m_Location = m_Subject->m_Location + TVector3(0.0f, 0.6f, 0.0f);

        m_LookAt.x = COSINE(m_Pi) * COSINE(m_Theta) * m_Distance;
        m_LookAt.z = COSINE(m_Pi) * SINE(m_Theta) * m_Distance;
        m_LookAt.y = SINE(m_Pi) * m_Distance;

        m_LookAt += m_Location;
    }
    break;
    case CameraMode_ThridPerson: {
        if (m_Subject)
            m_LookAt = m_Subject->m_Location;
        else
            m_Location = TVector3(0.0f, 0.0f, 0.0f);

        m_Location.x = COSINE(m_Pi) * COSINE(m_Theta) * m_Distance;
        m_Location.z = COSINE(m_Pi) * SINE(m_Theta) * m_Distance;
        m_Location.y = SINE(m_Pi) * m_Distance;

        m_Location += m_LookAt;
    }
    break;
    case CameraMode_Editor:
    case CameraMode_FreeMode: {
        if (GKeyMap['W'])
			m_LookAt += (m_LookAt - m_Location).Normalize() / 10.0f;
        if (GKeyMap['S'])
			m_LookAt -= (m_LookAt - m_Location).Normalize() / 10.0f;
        if (GKeyMap['A'])
			m_LookAt += ((m_LookAt - m_Location).Normalize() ^ m_Up).Normalize() / 10.0f;                
        if (GKeyMap['D'])
            m_LookAt -= ((m_LookAt - m_Location).Normalize() ^ m_Up).Normalize() / 10.0f;

        m_Location.x = COSINE(m_Pi) * COSINE(m_Theta) * m_Distance;
        m_Location.z = COSINE(m_Pi) * SINE(m_Theta) * m_Distance;
        m_Location.y = SINE(m_Pi) * m_Distance;

        m_Location += m_LookAt;
    }
    break;
	case CameraMode_Top:
		m_LookAt = TVector3(0, 0, 0);
		m_Location = m_LookAt + TVector3(0, 1000, 0);
		m_Up = TVector3(-1, 0, 0);
		break;
	case CameraMode_Bottom:
		m_LookAt = TVector3(0, 0, 0);
		m_Location = m_LookAt + TVector3(0, -1000, 0);
		m_Up = TVector3(1, 0, 0);
		break;
	case CameraMode_Right:
		m_LookAt = TVector3(0, 0, 0);
		m_Location = m_LookAt + TVector3(1000, 0, 0);
		m_Up = TVector3(0, -1, 0);
		break;
	case CameraMode_Left:
		m_LookAt = TVector3(0, 0, 0);
		m_Location = m_LookAt + TVector3(-1000, 0, 0);
		m_Up = TVector3(0, 1, 0);
		break;
	case CameraMode_Front:
		m_LookAt = TVector3(0, 0, 0);
		m_Location = m_LookAt + TVector3(0, 0, 1000);
		m_Up = TVector3(0, -1, 0);
		break;
	case CameraMode_Back:
		m_LookAt = TVector3(0, 0, 0);
		m_Location = m_LookAt + TVector3(0, 0, -1000);
		m_Up = TVector3(0, 1, 0);
		break;
	}
	if(m_CameraMode != CameraMode_Top) {
		TVector3 Side;
		Side.x = (m_LookAt - m_Location).z;
		Side.z = -(m_LookAt - m_Location).x;
		Side.y = 0.0f;
		m_Up = (m_LookAt - m_Location) ^ Side;
	}	
    m_bIsUpdated = false;
}

bool BCamera::ShouldUpdate() {
    switch (m_CameraMode) {
    case CameraMode_FirstPerson:
    case CameraMode_ThridPerson:
        return m_bIsUpdated || (!m_Subject) ? false : m_Subject->m_bIsUpdated;
    case CameraMode_Editor:
    case CameraMode_FreeMode:
        return m_bIsUpdated || GKeyMap['W'] || GKeyMap['S'] || GKeyMap['A']
               || GKeyMap['D'];
    }
    return m_bIsUpdated;
}