#include "stdafx.h"
#include <winsock2.h>
#include "CNetWork.h"


CNetWork::CNetWork()
{

}
CNetWork::~CNetWork()
{

}

void CNetWork::InitializeNet()
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2,2), &wsa) != 0)
		return;

	// 서버와 통신할 소켓 생성
	m_socket = socket(AF_INET, SOCK_STREAM, 0);

	u_long on = 1;
	retval = ioctlsocket(m_socket, FIONBIO, &on);

	// 서버에 연결
	SOCKADDR_IN serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(9000);
	serveraddr.sin_addr.s_addr = inet_addr("210.94.178.207");
	retval = connect(m_socket, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
}

char* CNetWork::NetRecv()
{
	recvn(m_socket,msg,11,0);
	return msg;
}
int CNetWork::recvn(SOCKET s, char *buf, int len, int flags)
{
	int received;
	char *ptr = buf;
	int left = len;

	while(left > 0){
		received = recv(s, ptr, left, flags);
		if(received == SOCKET_ERROR) 
			return SOCKET_ERROR;
		else if(received == 0) 
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}

int CNetWork::Netsend(char i,char j, char q,float angle,float power)
{
	mymsg.i=i;
	mymsg.j=j;
	mymsg.q=q;
	mymsg.angle=angle;
	mymsg.power=power;

	memcpy(sendmsg,&mymsg,11);

	send(m_socket,sendmsg,11,0);
	return 0;
}