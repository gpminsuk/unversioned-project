#pragma once

#include "TDataTypes.h"

class BViewport;
class RTextureBuffer;
class TText
{
public:
	TString Str;
	RTextureBuffer* TextBuffer;
};

class BTextDrawer
{
public:
	BTextDrawer(void);
	virtual ~BTextDrawer(void);

	TArray<TText> Texts;

	void AddText(TString& Str);
	void DrawTexts(BViewport* Viewport);
};

extern BTextDrawer* GTextDrawer;
