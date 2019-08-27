#pragma once

#include "Camel/Camel.h"
#include "ClientConnectorAPI.h"

class CChatMonitor
{
public:
	CChatMonitor();
	~CChatMonitor();

	bool Connect(const CString& strIP, int nPort);
//	bool Connect
	void Disconnect();

protected:
	static bool CALLBACK OnRecieved( void* pData, Camel::Dar::PacketHeader& rHeader, Camel::Base::System::Stream::Handle hStream, int nPacketSize);
	static bool CALLBACK OnDisconnected( void* pData);

	LRESULT OnLobbyChatRun(WPARAM& wParam, LPARAM& lParam);

	bool Process_UserAuth_Ans( unsigned int nMsgId, Camel::Dar::IDarMsg* pMsg);
	bool Process_NotifyChatMsg( unsigned int nMsgId, Camel::Dar::IDarMsg* pMsg );

private:
	Client::Connector::Handle m_hClientConnector;
	bool m_bConnected;

	Camel::Dar::DarDispatcher<unsigned int> m_clDispacher;

};