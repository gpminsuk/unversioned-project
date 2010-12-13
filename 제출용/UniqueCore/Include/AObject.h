#pragma once

class ACriticalSection;

class ASyncData
{
};

class AObject
{
protected:
	ASyncData* SyncData;

public:
	AObject(void);
	virtual ~AObject(void);

	virtual bool Tick();

	virtual void GetSyncData();
	virtual void SetSyncData();

	bool m_bIsUpdated;
	unsigned int m_iObjectId;		//오브젝트마다 고유의 아이디가있음
};
