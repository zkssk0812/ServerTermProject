#include "GameServer.h"
#include <stdio.h>

GameServer::GameServer(int port) : listen_sock(socket(AF_INET, SOCK_STREAM, 0)), Port(port) {

	if (listen_sock == INVALID_SOCKET)
		err_quit("socket()");

	sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(Port);

	retval = bind(listen_sock, reinterpret_cast<sockaddr*>(&serveraddr), sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");


}

void err_quit(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	MessageBoxA(NULL, (const char*)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

void err_display(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	printf("[%s] %s\n", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

void GameServer::ready() {

	static int PlayerCount = 0;
	
	sockaddr_in clientaddr;
	int addrlen = sizeof(clientaddr);
	while (true) {
		Socket client_sock{ accept(listen_sock, reinterpret_cast<sockaddr*>(&clientaddr), &addrlen) };
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			return;
		}
	

		char addr[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
		printf("[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d", addr, ntohs(clientaddr.sin_port));

		char buf[4]{};
		memcpy(buf, &PlayerCount, sizeof(PlayerCount));

		retval = send(client_sock, buf, sizeof(PlayerCount), 0);

		ThreadArgs* args = new ThreadArgs{ this, new Socket(std::move(client_sock)) };

		HANDLE hThread = CreateThread(NULL, 0, ClientThread, args, 0, NULL);

		++PlayerCount;

		if (PlayerCount >= 3) break;
		
	}

	this->run();

}

DWORD WINAPI GameServer::ClientThread(LPVOID arg) {
	return 0;
}

void GameServer::run() {

}