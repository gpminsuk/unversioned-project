#include "StdAfx.h"
#include "CCameraViewport.h"
#include "CCamera.h"

//�ӽ��ڵ�
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
		//�ӽ��ڵ�
		D3DXMatrixLookAtLH((D3DXMATRIXA16*)&m_ViewMatrix, (D3DXVECTOR3*)&CurCamera->m_Position, (D3DXVECTOR3*)&CurCamera->m_LookAt, (D3DXVECTOR3*)&CurCamera->m_Up);
	}
}