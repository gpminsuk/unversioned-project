#include "StdAfx.h"
#include "BApplication.h"
#include "BRenderer.h"

BApplication::BApplication(void)
:	m_pRenderer(0),
	m_pWorld(0),
	bQuit(false)
{
}