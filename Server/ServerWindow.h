#pragma once
#include "../Gui/BaseWindow.h"

class Server;

class ServerWindow :public BaseWindow<ServerWindow>
{
public:
	ServerWindow() :m_pServer(nullptr) {}
	LPCSTR ClassName() const override { return "ServerWindow"; }
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)override;
	void BindServer(Server* pServer) { m_pServer = pServer; }
	void UnBindServer() { m_pServer = nullptr; }

private:
	char* GetScreenData(int* dataLen);

private:
	Server *m_pServer;
};