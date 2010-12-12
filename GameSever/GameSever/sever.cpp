#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>

DWORD WINAPI RoomThread(LPVOID arg);
#define BUFSIZE 12

int recvn(SOCKET s, char *buf, int len, int flags)
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

int main(int argc, char* argv[])
{
	
	int retval;
	
	WSADATA wsa;
	WSAStartup(MAKEWORD(2,2), &wsa);
	SOCKADDR_IN serveraddr;
	// socket()
	SOCKET RooMSocket[2], listen_sock,invalid;
	
	
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(9000);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

	listen_sock = socket(AF_INET, SOCK_STREAM, 0);	
	u_long on = 1;
	retval = ioctlsocket(listen_sock, FIONBIO, &on);
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));		
	retval = listen(listen_sock, SOMAXCONN);

	invalid=RooMSocket[0]=RooMSocket[1]=socket(AF_INET, SOCK_STREAM, 0);

	FD_SET rset;
	SOCKADDR_IN clientaddr,temp;
	SOCKET client_sock;
	int addrlen,ntemp;
	int a=-1,b=-1;
	char msg[BUFSIZE];
	msg[0] = 0;

	while(true)
	{
		FD_ZERO(&rset);

		FD_SET(listen_sock,&rset);
		
		retval = select(0, &rset, NULL, NULL, NULL);	
		
		if (FD_ISSET(listen_sock,&rset))
		{
			a=send(RooMSocket[0],msg,BUFSIZE,0);
			b=send(RooMSocket[1],msg,BUFSIZE,0);

			addrlen = sizeof(clientaddr);
			

			client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
			
			printf("[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
			
			if(a == -1 )
			{
				RooMSocket[0]=client_sock;
				printf("1\n");
				printf("%d\n",a);
			}
			else if(b == -1 )
			{
				RooMSocket[1]=client_sock;
				printf("2\n");
				printf("%d\n",b);
			}
			
			a=send(RooMSocket[0],msg,BUFSIZE,0);
			b=send(RooMSocket[1],msg,BUFSIZE,0);

			printf("%d %d\n",a,b);

			
			if(a != -1  && b != -1 )
			{
				a=b=-1;
				SOCKET *temp = new SOCKET[2];
				temp[0]=RooMSocket[0];
				temp[1]=RooMSocket[1];
				RooMSocket[0]=invalid;
				RooMSocket[1]=invalid;
				//쓰레드 시작 temp 넘겨주고 둘이 알아서 주고받게 만듬 ㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋ
				CreateThread(NULL,0,RoomThread,(LPVOID)temp,0,NULL);
				printf("3\n");
			}
		}
		printf("\nd");
	}
	return 0;	
}

DWORD WINAPI RoomThread(LPVOID arg)
{
	SOCKET *temp = (SOCKET *)arg;
	FD_SET rset,wset;
	int retval;
	char msg[BUFSIZE];
	//다음은 네트워크 아이디 부여를 위한 센드메세지
	msg[0] = 1;
	msg[1] = 0;
	retval=send(temp[0],msg,BUFSIZE,0);
	msg[0] = 1;
	msg[1] = 1;
	retval=send(temp[1],msg,BUFSIZE,0);
	//다음은 순서를 보낸다
	msg[0] = 1;
	msg[1] = 3;
	retval=send(temp[0],msg,BUFSIZE,0);
	retval=send(temp[1],msg,BUFSIZE,0);	
	//하나의 탱크를 시작한다.
	msg[0] = 1;
	msg[1] = 4;
	retval=send(temp[0],msg,BUFSIZE,0);
	retval=send(temp[1],msg,BUFSIZE,0);
	
	timeval D={1,0};

	while(true)
	{
		FD_ZERO(&rset);	
		FD_SET(temp[0],&rset);
		FD_SET(temp[1],&rset);
		
		retval = select(0, &rset, NULL, NULL, &D);	
		if(retval==2)
		{	
			printf("쓰레드 종료");
			return 1;
		}
		if (FD_ISSET(temp[0],&rset))
		{
			retval=recv(temp[0],msg,BUFSIZE,0);
			retval=send(temp[1],msg,BUFSIZE,0);			
		}
		if (FD_ISSET(temp[1],&rset))
		{
			retval=recv(temp[1],msg,BUFSIZE,0);		
			retval=send(temp[0],msg,BUFSIZE,0);
		}
	}
	closesocket(temp[0]);
	closesocket(temp[1]);


	return 0;
}