#pragma once

class AObject
{
public:
	AObject(void);
	virtual ~AObject(void);

	bool m_bIsUpdated;
	unsigned int m_iObjectId;		//������Ʈ���� ������ ���̵�����
};
