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
#include "BLineBatcher.h"
#include "BPrimitive.h"
#include "BRenderingBatch.h"
#include "BCollisionBody.h"

#include "BRTRenderPass.h"

BViewport::BViewport(unsigned int InWidth, unsigned int InHeight, EViewportProjectionType InProjectionType, EViewportRenderMode InRenderMode, ECameraMode InCameraMode):
    VisibleScenes(Scene_World | Scene_Collision),
	ProjectionType(InProjectionType),
	RenderMode(InRenderMode),
	X(0), Y(0) {
	Camera = new BCamera(this, InCameraMode);
	Width = InWidth;
	Height = InHeight;
}

BViewport::~BViewport(void) {
}

EFillMode BViewport::GetFillMode() {
	switch(RenderMode) {
	case RenderMode_All:
		return FillMode_Solid;
	case RenderMode_Wireframe:
		return FillMode_Wireframe;
	}
	return FillMode_Solid;
}

void BViewport::SortTemplates() {
}

void BViewport::Clear() {
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

void BViewport::Resize(int InWidth, int InHeight) {
	Width = InWidth;
	Height = InHeight;
}