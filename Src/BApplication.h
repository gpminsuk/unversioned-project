#pragma once

class BRenderer;
class BDriver;
class BViewport;

class BApplication
{
public:
	BApplication(void);

	BRenderer *m_pRenderer;
	BDriver* m_pDriver;

	BViewport* m_pViewport;

	bool bQuit;
};
