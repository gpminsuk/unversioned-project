#include "stdafx.h"
#include "BPrimitive.h"
#include "CTank.h"
#include "CBoxComponent.h"
#include "CSkeletalMeshComponent.h"
#include "CWaveIODriver.h"
#include "CCylinderCollisionBody.h"
#include "CMeshTerrain.h"

CMeshTerrain::CMeshTerrain()
{
	CSkeletalMeshComponent* SkeletalMeshComponent = new CSkeletalMeshComponent(2);
	Components.AddItem(SkeletalMeshComponent);
	CCylinderCollisionBody* CylinderCollisionBody = new CCylinderCollisionBody(this);
	CollisionBodies.AddItem(CylinderCollisionBody);
}

CMeshTerrain::~CMeshTerrain()
{
}

bool CMeshTerrain::Tick(unsigned long dTime)
{
	return true;
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
	SetQuaternion(TVector3(0.0f, 0.0f, 1.0f),1.57f);
	SetQuaternion(TVector3(1.0f, 0.0f, 0.0f),1.57f);
	SetQuaternion(TVector3(0.0f, 1.0f, 0.0f),1.57f);
	m_Location.z += 30;
	m_Location.y += 7.5;
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
	for(unsigned int i=0;i<CollisionBodies.Size();++i)
	{
		CCylinderCollisionBody* Cylinder = (CCylinderCollisionBody*)CollisionBodies(i);
		if(Cylinder)
		{
			Cylinder->Radius = 30.0f;
			Cylinder->Height =30.0f;
		}
		for(unsigned int j=0;j<CollisionBodies(i)->Primitives.Size();++j)
		{
			CollisionBodies(i)->Primitives(j)->Translation = m_Location;
			CollisionBodies(i)->Primitives(j)->TM = TMatrix(TVector3(m_Location.x, m_Location.y, m_Location.z),	TQuaternion(), 100.0f);

		}
	}	
}