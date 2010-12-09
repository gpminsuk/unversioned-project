#pragma once

#include "MyWorld.h"

class CNetWork
{
public:
	CNetWork();
	~CNetWork();
	
	SOCKET m_socket;
	char msg[11];
	char sendmsg[11];

	myMSG mymsg;
	
	void InitializeNet();
	char* NetRecv();
	int recvn(SOCKET s, char *buf, int len, int flags);
	int Netsend(char i,char j, char q,float angle,float power);
};
