#include "StdAfx.h"
#include "BApplication.h"
#include "BRenderer.h"

#include "UFreeTypeDrawer.h"

BApplication::BApplication(void)
:	m_pRenderer(0),
	m_pWorld(0),
	bQuit(false),
	bRenderThreadQuit(false)
{
	GFontDrawer = new UFreeTypeFontDrawer();
}

BApplication::~BApplication()
{
	delete GFontDrawer;
}