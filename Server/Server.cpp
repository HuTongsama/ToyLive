#include "Server.h"
#include "ServerWindow.h"


Server::Server()
{
	ServerWindow *pWindow = new ServerWindow();
	pWindow->Create("Server", WS_OVERLAPPEDWINDOW);
	pWindow->BindServer(this);
	m_pWindow = (void*)pWindow;
}

Server::~Server()
{
	ServerWindow* pWindow = (ServerWindow*)m_pWindow;
	if (pWindow != nullptr)
		pWindow->UnBindServer();
	ClearServerInfo();
}

bool Server::InitListen(const string& port)
{
	ClearServerInfo();
	m_threadCloseFlag = false;
	m_listenThread = NULL;

	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	struct addrinfo * result = NULL;
	struct addrinfo hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	iResult = getaddrinfo(NULL, port.c_str(), &hints, &result);
	m_listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	iResult = bind(m_listenSocket, result->ai_addr, (int)result->ai_addrlen);
	freeaddrinfo(result);

	iResult = listen(m_listenSocket, SOMAXCONN);

	DWORD   dwThreadId;
	m_listenThread = CreateThread(
		NULL,                   // default security attributes
		0,                      // use default stack size  
		Server::ListenFunction,       // thread function name
		this,          // argument to thread function 
		0,                      // use default creation flags 
		&dwThreadId);   // returns the thread identifier 

	return true;
}

int Server::SendAllData(char* buf, int len) 
{
	for (int i = 0; i < m_clients.size(); ++i)
	{
		auto& client = m_clients[i];
		int total = 0;
		int n = 0;
		int bitLeft = len;
		int sendLen = 256;

		while (total < len)
		{
			int len = min(bitLeft, sendLen);
			n = send(client, buf + total, len, 0);
			if (n == SOCKET_ERROR)
			{
				int errCode = WSAGetLastError();
				closesocket(client);
				m_clients.erase(m_clients.begin() + i);
				--i;
				//sendFail = true;
				break;
			}
			total += n;
			bitLeft -= n;
		}
	}
	return 0;
}

void Server::ClearServerInfo()
{
	if (m_listenThread != NULL)
	{
		m_threadCloseFlag = true;
		CloseHandle(m_listenThread);
		m_listenThread = NULL;
	}

	if (m_listenSocket != INVALID_SOCKET)
	{
		closesocket(m_listenSocket);
		m_listenSocket = INVALID_SOCKET;
	}
	for (auto client : m_clients)
		closesocket(client);
	m_clients.clear();
}

DWORD WINAPI Server::ListenFunction(LPVOID lpParam)
{
	Server *pServer = (Server*)lpParam;

	while (!pServer->m_threadCloseFlag)
	{
		fd_set readSet;
		FD_ZERO(&readSet);
		FD_SET(pServer->m_listenSocket, &readSet);
		timeval timeVal = { 0,100 };
		select(0, &readSet, nullptr, nullptr, &timeVal);
		if (FD_ISSET(pServer->m_listenSocket, &readSet))
		{
			SOCKET clientSocket = accept(pServer->m_listenSocket, NULL, NULL);
			if (clientSocket != INVALID_SOCKET)
			{
				pServer->m_clients.push_back(clientSocket);
			}
		}
	}
	return 0;
}
