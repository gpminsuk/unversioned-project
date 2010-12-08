#pragma once

#include "BThing.h"

class CTankManager
{
private:
	class CTank* m_pTank[2];


	int m_nTimer;

public:
	CTankManager();
	~CTankManager();

	bool SetTimer(int i_nTimer);
	CTank* GetTank(int _idx);
	
	
	bool SetStartTank(int _idx);

	bool InitializeTank(TVector3 i_vecTank1, TVector3 i_vecTank2);
	void SetOpponent(CTank* InOpponent);


};