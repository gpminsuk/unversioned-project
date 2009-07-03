#pragma once

class BRenderer;
class BDriver;
class UWorld;

class BApplication
{
public:
	BApplication(void);

	BRenderer *m_pRenderer;
	BDriver* m_pDriver;

	UWorld*	m_pWorld;

	bool bQuit;
};
