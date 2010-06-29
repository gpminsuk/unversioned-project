#include "stdafx.h"
#include "MaxExporter.h"

MaxExporter::MaxExporter()
{

}

MaxExporter::~MaxExporter()
{

}

int MaxExporter::ExtCount()
{
	return 1;
}
const TCHAR* MaxExporter::Ext(int n)
{
	switch(n)
	{
	case 0:
		return _T("mine");
	}
	return _T("");
}
const TCHAR* MaxExporter::LongDesc()
{
	return _T("");
}
const TCHAR* MaxExporter::ShortDesc()
{
	return _T("");
}
const TCHAR* MaxExporter::AuthorName()
{
	return _T("");
}
const TCHAR* MaxExporter::CopyrightMessage()
{
	return _T("");
}
const TCHAR* MaxExporter::OtherMessage1()
{
	return _T("");
}
const TCHAR* MaxExporter::OtherMessage2()
{
	return _T("");
}
unsigned int MaxExporter::Version()
{
	return 100;
}
void MaxExporter::ShowAbout(HWND hWnd)
{

}
int	MaxExporter::DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options)
{
	FILE* fp;
	fopen_s(&fp, name, "wt");
	if(fp)
	{
		INode *Node = i->GetRootNode();
		int NumOfChildren = Node->NumberOfChildren();
		INode* CurNode;
		for(int i=0;i<NumOfChildren;++i)
		{
			CurNode = Node->GetChildNode(i);
			fprintf(fp, "%s\n", CurNode->GetName());
		}
		fclose(fp);
	}	
	return 0;
}

HINSTANCE hInstance;
#define ASCIIEXP_CLASS_ID	Class_ID(0x1eaf72f4, 0x1cd7468d)

class MaxExporterClassDesc :public ClassDesc {
public:
	int				IsPublic() { return 1; }
	void*			Create(BOOL loading = FALSE) { return new MaxExporter; } 
	const TCHAR*	ClassName() { return _T("MaxExporter"); }
	SClass_ID		SuperClassID() { return SCENE_EXPORT_CLASS_ID; } 
	Class_ID		ClassID() { return ASCIIEXP_CLASS_ID; }
	const TCHAR*	Category() { return _T("Standard"); }
};

static MaxExporterClassDesc MaxExpDesc;

BOOL WINAPI DllMain(HINSTANCE hinstDLL,ULONG fdwReason,LPVOID lpvReserved) 
{
	if( fdwReason == DLL_PROCESS_ATTACH )
	{
		hInstance = hinstDLL;
		DisableThreadLibraryCalls(hInstance);
	}

	return (TRUE);
}

__declspec( dllexport ) const TCHAR* LibDescription() 
{
	return _T("");
}

/// MUST CHANGE THIS NUMBER WHEN ADD NEW CLASS 
__declspec( dllexport ) int LibNumberClasses() 
{
	return 1;
}


__declspec( dllexport ) ClassDesc* LibClassDesc(int i) 
{
	switch(i)
	{
	case 0:
		return &MaxExpDesc;
	}
	return 0;
}

__declspec( dllexport ) ULONG LibVersion() 
{
	return VERSION_3DSMAX;
}

// Let the plug-in register itself for deferred loading
__declspec( dllexport ) ULONG CanAutoDefer()
{
	return 1;
}