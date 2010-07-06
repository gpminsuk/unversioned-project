#pragma once

class BPrimitive;
class BComponent;

class BSynchronizer
{
public:
	BSynchronizer();
	~BSynchronizer();

	BSynchronizer* PreviousSync;

	virtual void Syncronize(BSynchronizer* Sync);
	virtual void SyncData() = 0;
};