#pragma once

#include <windows.h>

#include "TDataTypes.h"
#include "InputDefine.h"

#include "BViewport.h"

class CCamera;

class CCameraViewport : public BViewport
{
public:
	CCameraViewport(void);
	virtual ~CCameraViewport(void);

	TArray<CCamera*>	m_CameraArray;
	unsigned int m_curCameraIdx;

	inline void AddCamera(CCamera* c)
	{
		m_CameraArray.AddItem(c);
	}

	inline CCamera* SetCamera(unsigned int idx)
	{
		if(CameraNum() > idx)
			return m_CameraArray[idx];
		return NULL;
	}

	inline size_t CameraNum()
	{
		return m_CameraArray.Size();
	}

	inline CCamera* GetCurrentCamera()
	{
		if(m_curCameraIdx != -1)
			return m_CameraArray[m_curCameraIdx];
		return NULL;
	}

	inline void SetCurrentCamera(unsigned int idx)
	{
		if(CameraNum() > idx)
		{
			m_curCameraIdx = idx;
		}
		else
		{
			m_curCameraIdx = -1;
		}
	}

	virtual void UpdateCameraViewport();
	virtual void InputMouse(EMouse_Event Event, TMouseInput_Param Param);
	virtual void InputKey();
	virtual void InputChar();
};