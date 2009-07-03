#include "StdAfx.h"
#include "CCamera.h"

CCamera::CCamera(void)
:	m_Position(10.0f,10.0f,10.0f),
	m_LookAt(0.0f,0.0f,0.0f),
	m_Up(0.0f,1.0f,0.0f)
{
}

CCamera::~CCamera(void)
{
}
