#pragma once

class BRenderer;
class UWorld;

class BApplication
{
public:
	BApplication(void);

	BRenderer *m_pRenderer;

	UWorld*	m_pWorld;

	bool bQuit;
	bool bRenderThreadQuit;
};
