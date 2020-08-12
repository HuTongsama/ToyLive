#include "Server.h"
#include "ServerWindow.h"


Server::Server()
{
	ServerWindow window;
	window.Create("Server", WS_OVERLAPPEDWINDOW);
	window.InitServer(this);
}

Server::~Server()
{
	ClearServerInfo();
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
