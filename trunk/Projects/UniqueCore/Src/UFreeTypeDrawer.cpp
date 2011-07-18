#include "stdafx.h"
#include "UFreeTypeDrawer.h"

#include "RDXResource.h"

#include "BDriver.h"

UFreeTypeFontDrawer *GFontDrawer;

UFreeTypeFontDrawer::UFreeTypeFontDrawer()
{
	if (FT_Init_FreeType(&Library))
	{
		// ERROR TODO
	}
}

UFreeTypeFontDrawer::~UFreeTypeFontDrawer()
{
	FT_Done_FreeType(Library);
}

void UFreeTypeFontDrawer::DrawString(TString String, RTextureBuffer *Tex)
{
	int error;
	FT_Face face;
	unsigned index;
	unsigned int x, y;
	int Color;
	unsigned int width, height;

	// ��Ʈ ����
	error=FT_New_Face(Library,"..\\..\\Resources\\font.ttf",0,&face);
	if (error != 0) {
		// ���� ó��
	}
	
	RDXFontBuffer* DXTex = dynamic_cast<RDXFontBuffer*>(Tex);
	if(!DXTex)// || DXTex->String == String)
		return;

	// TODO �������� �ؽ�Ʈ �ε� ���ص� �ǰ� ĳ�� �ؾ���
	TLockedRect Rect = DXTex->Lock();
	int *pColor = (int*)Rect.pBits;
	unsigned int TexWidth = Rect.Pitch / 4;

	for(unsigned int i=0;i<DXTex->Height;++i)
	{
		for(unsigned int j=0;j<TexWidth;++j)
		{
			pColor[i*DXTex->Width+j] = 0;
		}
	}

	// �۸��� �ε��� ����
	for(int i=0;i<1024;++i)
	{
		//if(String.Str[i] == DXTex->String.Str[i])
		//	continue;

		if(String.Str[i] == '\0')
			break;
		index = FT_Get_Char_Index(face,String.Str[i]);
		//index = FT_Get_Char_Index(face,TEXT('1'));
		if (index == 0) {
			// ���� ó��
		}

		// ũ�� ����
		//	error=FT_Set_Char_Size(face, 256 * 64, 0, Tex->Width, Tex->Height); 
		error=FT_Set_Char_Size(face, 256 * 64, 0, 5, 5); 

		// �۸��� �ε�
		error=FT_Load_Glyph(face,index,FT_LOAD_DEFAULT | FT_LOAD_NO_BITMAP);

		// ������
		error=FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);

		// ȭ������ ���
		width=face->glyph->bitmap.width;
		height=face->glyph->bitmap.rows;

		for (y=0;y<((height<Tex->Height)?height:Tex->Height);y++)
		{
			for (x=0;x<((width<TexWidth)?width:TexWidth);x++)
			{
				Color= face->glyph->bitmap.buffer[y*width+x];
				if (Color != 0 && (x+i*10 < TexWidth))
				{
					pColor[y*TexWidth+x+(i*10)] = (Color) + (Color << 8) + (Color << 16) + (Color << 24);
				}
				else
				{
					pColor[y*TexWidth+x+(i*10)] = (0) + (0 << 8) + (0 << 16) + (0 << 24);
				}
			}
		}
	}
	DXTex->Unlock();
	
	FT_Done_Face(face);
}