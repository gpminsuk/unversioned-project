#pragma warning(disable:4819)

#include "Max.h"

class MaxExporter : public SceneExport
{
public:
	MaxExporter();
	~MaxExporter();

	int				ExtCount();				
	const TCHAR *	Ext(int n);				
	const TCHAR *	LongDesc();				
	const TCHAR *	ShortDesc();			
	const TCHAR *	AuthorName();			
	const TCHAR *	CopyrightMessage();		
	const TCHAR *	OtherMessage1();		
	const TCHAR *	OtherMessage2();		
	unsigned int	Version();				
	void			ShowAbout(HWND hWnd);	
	int				DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options);
};