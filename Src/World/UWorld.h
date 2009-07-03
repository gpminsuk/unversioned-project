#pragma once

#include <windows.h>
#include "TDataTypes.h"

class BRenderer;

class CCamera;
class CCameraViewport;

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

	TBox	m_Box;
};
