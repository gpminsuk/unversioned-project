#pragma once

class AObject
{
public:
	AObject(void);
	virtual ~AObject(void);

	bool m_bIsUpdated;
	unsigned int m_iObjectId;		//오브젝트마다 고유의 아이디가있음
};
