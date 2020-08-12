#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include<vector>

#pragma comment (lib, "Ws2_32.lib")
using namespace std;
class Server
{
public:
	Server();
	~Server();
	bool InitListen(const string& port);

private:
	void ClearServerInfo();
private:
	SOCKET m_listenSocket;
	vector<SOCKET> m_clients;
	HANDLE m_listenThread;
	bool m_threadCloseFlag;
};