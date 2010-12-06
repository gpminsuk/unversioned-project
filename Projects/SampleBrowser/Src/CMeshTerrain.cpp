#include "stdafx.h"
#include "BPrimitive.h"
#include "CTank.h"
#include "CBoxComponent.h"
#include "CSkeletalMeshComponent.h"
#include "CWaveIODriver.h"
#include "CMeshTerrain.h"

CMeshTerrain::CMeshTerrain()
{
	CSkeletalMeshComponent* SkeletalMeshComponent = new CSkeletalMeshComponent(2);
	Components.AddItem(SkeletalMeshComponent);
}

CMeshTerrain::~CMeshTerrain()
{

}

void CMeshTerrain::Tick(unsigned long dTime)
{

}

void CMeshTerrain::InputKey(EKey_Event Event, TKeyInput_Param& Param)
{
	
}

void CMeshTerrain::PhysicsTick(unsigned long dTime)
{

}

bool CMeshTerrain::SetSize(float i_fSize)
{
	m_fSize = i_fSize;
	return true;
}

bool CMeshTerrain::SetRadian(float i_fRadian)
{
	m_fRadian = i_fRadian;
	return true;
}

bool CMeshTerrain::SetRotationCylinder(TVector3 rot)
{
	m_vecRotationCylinder = rot;
	return true;
}
CMeshTerrain::CMeshTerrain(TVector3 _rot, float _radian, float _size, int i)
{
	//CBoxComponent* box = new CBoxComponent();
	//Components.AddItem(box);
	CSkeletalMeshComponent* SkeletalMeshComponent = new CSkeletalMeshComponent(i);
	Components.AddItem(SkeletalMeshComponent);
	SetSize(_size);
	SetQuaternion(_rot, _radian);
}

bool CMeshTerrain::SetQuaternion(TQuaternion _rot)
{
	m_qRot = _rot;
	return true;
}
bool CMeshTerrain::SetQuaternion(TVector3 _vecRotationCylinder, float _radian)
{
	SetRotationCylinder(_vecRotationCylinder);
	SetRadian(_radian);
	m_qRot.Rotate(m_vecRotationCylinder, m_fRadian);
	return true;
}
void CMeshTerrain::UpdateTransform()
{
	for(unsigned int i=0;i<Components.Size();++i)
	{
		for(unsigned int j=0;j<Components(i)->Primitives.Size();++j)
		{
			Components(i)->Primitives(j)->Translation = m_Location;
			Components(i)->Primitives(j)->TM = TMatrix(TVector3(m_Location.x, m_Location.y, m_Location.z),
				m_qRot, m_fSize);
			//Components(i)->Primitives(j)->TM._41 = m_Location.x;
			//Components(i)->Primitives(j)->TM._42 = m_Location.y;
			//Components(i)->Primitives(j)->TM._43 = m_Location.z;
		}
	}	
}