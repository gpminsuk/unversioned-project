#include "StdAfx.h"
#include "BViewport.h"
#include "RResource.h"
#include <cstdlib>
#include <algorithm>

#include "BCamera.h"
#include "BDriver.h"
#include "BRenderer.h"
#include "BTextDrawer.h"
#include "BThing.h"
#include "BComponent.h"
#include "BLineBatcher.h"
#include "BPrimitive.h"
#include "BRenderingBatch.h"
#include "BCollisionBody.h"

#include "BRTRenderPass.h"

BViewport::BViewport(unsigned int Width, unsigned int Height):
    VisibleScenes(Scene_World | Scene_Collision) {
	Camera = new BCamera();
	m_Width = Width;
	m_Height = Height;
}

BViewport::~BViewport(void) {
}

void BViewport::SortTemplates() {
}

void BViewport::Clear() {
}

RRenderTarget* BViewport::GetBackBuffer() {
	return SwapChain->GetBackBuffer();
}

TVector3 BViewport::GetViewportOrigin() {
    return Camera->m_Location;
}

//임시코드
#include <d3d9.h>
#include <d3dx9effect.h>

void BViewport::UpdateViewport() {
    if (Camera && Camera->ShouldUpdate() || true) {
        Camera->Tick(0);
        //임시코드
        D3DXMatrixLookAtLH((D3DXMATRIXA16*) &m_ViewMatrix,
                           (D3DXVECTOR3*) &Camera->m_Location,
                           (D3DXVECTOR3*) &Camera->m_LookAt,
                           (D3DXVECTOR3*) &Camera->m_Up);
    }
}

void BViewport::InputMouse(EMouse_Event Event, TMouseInput_Param& Param) {
    Camera->InputMouse(Event, Param);
}

void BViewport::InputChar() {

}

void BViewport::InputKey(EKey_Event Event, TKeyInput_Param& Param) {
    Camera->InputKey(Event, Param);
}