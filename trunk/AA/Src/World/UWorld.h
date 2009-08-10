#pragma once

#include <windows.h>
#include "TDataTypes.h"
#include "InputDefine.h"

class AObject;

class BRenderer;

class CCamera;
class CCameraViewport;

//юс╫ц
#include "CSkeletalMeshPrimitive.h"

class UWorld
{
public:
	UWorld(BRenderer* R = NULL);
	~UWorld(void);

	inline void SetRenderer(BRenderer* R) { m_pRenderer = R; }

	bool InitializeWorld();
	bool DestroyWorld();

	virtual void Tick(DWORD dTime);

	CCameraViewport* m_pViewport;
	CCamera* m_pCamera;
	BRenderer* m_pRenderer;

	TArray<class BThing*> Objects;

	CSkeletalMeshPrimitive	*m_Mesh;

	virtual void InputMouse(EMouse_Event Event, TMouseInput_Param& Param);
	virtual void InputKey(EKey_Event Event, TKeyInput_Param& Param);
	virtual void InputChar();
};
