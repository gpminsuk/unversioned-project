#include "StdAfx.h"
#include "AApplication.h"
#include "BRenderer.h"

#include "UFreeTypeDrawer.h"

IMPLEMENT_CLASS(AApplication);

AApplication::AApplication(void) :
    m_pRenderer(0), bQuit(false) {
    GFontDrawer = new UFreeTypeFontDrawer();
}

AApplication::~AApplication() {
    delete GFontDrawer;
}
