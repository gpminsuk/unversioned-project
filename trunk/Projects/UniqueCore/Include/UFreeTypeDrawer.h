#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include "TDataTypes.h"
#include "RResource.h"

#include <Windows.h>

#pragma comment(lib,"freetype237MT.lib")

class UFreeTypeFontDrawer
{
public:
	UFreeTypeFontDrawer();
	~UFreeTypeFontDrawer();

	void DrawString(TString String, RTextureBuffer *Tex);

	FT_Library Library;
};

extern UFreeTypeFontDrawer *GFontDrawer;
