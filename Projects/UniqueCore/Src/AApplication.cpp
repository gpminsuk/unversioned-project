#include "StdAfx.h"
#include "AApplication.h"
#include "BRenderer.h"

#include "UFreeTypeDrawer.h"

AApplication::AApplication(void)
:	m_pRenderer(0),
	m_pWorld(0),
	m_pViewport(0),
	bQuit(false)
{
	GFontDrawer = new UFreeTypeFontDrawer();
}

AApplication::~AApplication()
{
	delete GFontDrawer;
}