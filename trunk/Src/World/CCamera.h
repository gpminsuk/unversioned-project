#pragma once

#include "TDataTypes.h"
#include "BThing.h"

class CCamera : public BThing
{
public:
	CCamera(void);
	virtual ~CCamera(void);

	TVector3 m_Position;
	TVector3 m_LookAt;
	TVector3 m_Up;
};