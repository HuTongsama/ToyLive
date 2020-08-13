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
	int SendAllData(char* buf, int len);
private:
	void ClearServerInfo();
	static DWORD WINAPI ListenFunction(LPVOID lpParam);
private:
	SOCKET m_listenSocket;
	vector<SOCKET> m_clients;
	HANDLE m_listenThread;
	bool m_threadCloseFlag;
	void *m_pWindow;
};