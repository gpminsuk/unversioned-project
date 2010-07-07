#pragma once

class BPrimitive;
class BComponent;
class ACriticalSection;

class BSynchronizeData
{
public:
	virtual void SetData(BComponent* Component) = 0;
	virtual void GetData(BPrimitive* Primitive) = 0;
};

class BSynchronizer
{
public:
	BSynchronizer();
	~BSynchronizer();

	ACriticalSection* CriticalSection;
	BSynchronizeData* SyncData;

	void SendData(BComponent* Component);
	void RecieveData(BPrimitive* Primitive);
};