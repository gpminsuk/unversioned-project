#pragma once

#include "AObject.h"
#include "TDataTypes.h"

#include "InputDefine.h"
#include "Enumerations_Geom.h"

class BThing : public AObject
{
	DECLARE_CLASS(BThing, CLASS_Abstract)

	TVector3 m_Location;
	TVector3 m_Scale;

	TArray<class BComponent*> Components;
	TBounds PrimitiveBounds;
	TArray<class BCollisionBody*> CollisionBodies;
	TBounds CollisionBodyBounds;

	BThing(void);
	virtual ~BThing(void);

	virtual TArray<class BPrimitive*> GetPrimitives();
public:
	virtual bool Tick(unsigned long dTime);

public:
	virtual void PhysicsTick(unsigned long dTime) = 0;
	virtual void UpdateTransform() = 0;
	virtual TVector3 LineCheck(TVector3& Start, TVector3& End, TVector3& Extent);

	virtual void InputMouse(EMouse_Event Event, TMouseInput_Param& Param) {}
	virtual void InputKey(EKey_Event Event, TKeyInput_Param& Param) {}
	virtual void InputChar() {}
};
