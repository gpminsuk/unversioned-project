#pragma once

#include "MyWorld.h"

#pragma comment(lib, "ws2_32.lib")

class CNetWork
{
public:
	CNetWork();
	~CNetWork();
	
	SOCKET m_socket;
	char msg[12];
	char sendmsg[12];

	myMSG mymsg;
	
	void InitializeNet();
	char* NetRecv();
	int recvn(SOCKET s, char *buf, int len, int flags);
	int Netsend(char i,char j, char q,float angle,float power);
};
