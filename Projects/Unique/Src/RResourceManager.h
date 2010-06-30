#pragma once

class RResourceManager
{
public:
	RResourceManager(void);
	~RResourceManager(void);

	static bool LoadResources();
	static bool ReleaseAllResources();
};
