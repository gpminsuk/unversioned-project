#include "stdafx.h"
#include <windows.h>

#include "CBeatTrack.h"

void main()
{
	CBeatTrack* Track = new CBeatTrack();
	Track->Start();
	DWORD PrevTime = GetTickCount();
	while(1)
	{
		if(GetTickCount() - PrevTime > 20)
		{
			if(!Track->Advance(GetTickCount() - PrevTime))
			{
				break;
			}
			PrevTime = GetTickCount();
		}		
	}	
}