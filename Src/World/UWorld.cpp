#include "StdAfx.h"
#include "UWorld.h"

#include "BRenderer.h"

#include "CCameraViewport.h"
#include "CCamera.h"

UWorld::UWorld(BRenderer* R)
: m_pRenderer(R)
{

}

UWorld::~UWorld()
{

}

void UWorld::Tick(DWORD dTime)
{
	m_pViewport->UpdateCameraViewport();
}

bool UWorld::InitializeWorld()
{
	m_pViewport = new CCameraViewport();

	m_pRenderer->AddViewport(m_pViewport);

	m_Box.Resize(TVector3(2.0f,2.0f,2.0f));

	m_pViewport->Render(&m_Box);


	m_pCamera = new CCamera();

	m_pViewport->AddCamera(m_pCamera);

	m_pViewport->SetCurrentCamera(0);
	return TRUE;
}

bool UWorld::DestroyWorld()
{
	delete m_pViewport;
	m_pViewport = 0;
	delete m_pCamera;
	m_pCamera = 0;
	return TRUE;
}

void UWorld::InputChar()
{
	m_pViewport->InputChar();
}

void UWorld::InputKey()
{
	m_pViewport->InputKey();
}

void UWorld::InputMouse(EMouse_Event Event, TMouseInput_Param Param)
{
	m_pViewport->InputMouse(Event, Param);
}