#include "StdAfx.h"
#include "CCameraViewport.h"
#include "CCamera.h"

//임시코드
#include <d3d9.h>
#include <d3dx9effect.h>

CCameraViewport::CCameraViewport(void)
: m_curCameraIdx(-1)
{
}

CCameraViewport::~CCameraViewport(void)
{
}

void CCameraViewport::UpdateCameraViewport()
{
	CCamera* CurCamera = GetCurrentCamera();
	if(CurCamera && CurCamera->m_bIsUpdated)
	{
		//임시코드
		D3DXMatrixLookAtLH((D3DXMATRIXA16*)&m_ViewMatrix, (D3DXVECTOR3*)&CurCamera->m_Position, (D3DXVECTOR3*)&CurCamera->m_LookAt, (D3DXVECTOR3*)&CurCamera->m_Up);
	}
}