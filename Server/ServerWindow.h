#pragma once
#include "../Gui/BaseWindow.h"

class Server;

class ServerWindow :public BaseWindow<ServerWindow>
{
public:
	ServerWindow() :m_pServer(nullptr) {}
	LPCSTR ClassName() const override { return "ServerWindow"; }
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)override;
	void InitServer(Server* pServer) { m_pServer = pServer; }

private:
	Server *m_pServer;
};