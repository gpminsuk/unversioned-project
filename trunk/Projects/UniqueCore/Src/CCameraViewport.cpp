#include "StdAfx.h"
#include "CCameraViewport.h"

#include "BCamera.h"

//임시코드
#include <d3d9.h>
#include <d3dx9effect.h>

#include "CWindowApp.h"
#include "CUIButton.h"

CCameraViewport::CCameraViewport() {
    m_pCamera = new BCamera();
    m_Width = 800;
    m_Height = 600;
}

CCameraViewport::~CCameraViewport(void) {
    delete m_pCamera;
}

TVector3 CCameraViewport::GetViewportOrigin() {
    return m_pCamera->m_Location;
}

bool CCameraViewport::Tick(unsigned long Time) {
    return true;
}

void CCameraViewport::SetProperty(TString& PropertyName, TString& Value) {

}

void CCameraViewport::UpdateViewport() {
    if (m_pCamera && m_pCamera->ShouldUpdate() || true) {
        m_pCamera->Tick(0);
        //임시코드
        D3DXMatrixLookAtLH((D3DXMATRIXA16*) &m_ViewMatrix,
                           (D3DXVECTOR3*) &m_pCamera->m_Location,
                           (D3DXVECTOR3*) &m_pCamera->m_LookAt,
                           (D3DXVECTOR3*) &m_pCamera->m_Up);
    }
}

void CCameraViewport::InputMouse(EMouse_Event Event, TMouseInput_Param& Param) {
    m_pCamera->InputMouse(Event, Param);
}

void CCameraViewport::InputChar() {

}

void CCameraViewport::InputKey(EKey_Event Event, TKeyInput_Param& Param) {
    m_pCamera->InputKey(Event, Param);
}
