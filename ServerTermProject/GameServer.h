#pragma once
#ifndef _GameServer

#include "Socket.h"
#include <ws2tcpip.h>
#include <Windows.h>
#include "ThreadSafeQueue.h"

#pragma comment(lib, "ws2_32")

#define THREADNUM 4
#define LOGICTHREAD 0
#define CLIENTTHREAD1 1
#define CLIENTTHREAD2 2
#define CLIENTTHREAD3 3

void err_quit(const char* msg);

void err_display(const char* msg);

struct BaseTask {
	Socket clientSocket; // 누가 보냈는가? 
	int taskType; // 무슨 요청인가? 
	char data[]; // 실제 데이터 
};

class GameServer {
public:
	GameServer(int port);
	//~GameServer();

	void ready();
	void run();

private:

	static DWORD WINAPI GameLogic(LPVOID arg);
	static DWORD WINAPI ClientThread(LPVOID arg);

	int retval;

	int Port;

	Socket listen_sock;

	ThreadSafeQueue<BaseTask*> threadQueue;

	HANDLE hThread[THREADNUM];

	struct ThreadArgs {
		GameServer* server;
		Socket* sock;
	};

};


#endif 
