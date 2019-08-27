#pragma once

#include <string>
#include "Camel/Base/Network/Connector.h"
#include "Camel/Base/Network/Link.h"
#include "Camel/Base/System/System.h"
#include "Camel/Base/System/Stream.h"
#include "Camel/Dar/DarBaseMsg.h"
#include "ClientConnectorAPI.h"

using namespace Camel::Base;

namespace Client
{
	namespace Connector
	{
		namespace Impl
		{
			class Connector
			{
			public:
				enum	ConnectorState
				{
					None = 0,
					InitComplete = 1,
					Connected = 2,	
					Disconnected = 3,
				};

				Connector(void);
				virtual ~Connector(void);
			public:
				bool	Init(const char* szAddr, unsigned short nPort);
				bool	Connect();
				void	Disconnect();

				void OnRecieved( Network::Link::Handle hLink);

				bool Send( Camel::Dar::IDarMsg* pMessage);

			// State °ü¸®
			private:
				ConnectorState m_eConnectorState;

			private:
				System::ThreadPool::Handle m_hThreadPool;
				Network::Connector::Handle m_hConnector;
				System::Stream::Handle m_hStream;

				Network::Link::Handle	m_hLink;

				std::string		m_strIP;
				unsigned short	m_sPort;

				System::Threading::AutoResetEvent	m_clConnectEvent;

			public:
				bool	SetOnRecieved( OnRecievedCallback pCallback, void* pData);
				bool	SetOnDisconnected( OnDisconnectedCallback pCallback, void* pData);

				void OnConnected( Network::Link::Handle hLink);
				void OnDisconnected( Network::Link::Handle hLink);
			private:
				std::pair< void* , OnRecievedCallback>		m_pairOnRecieved;
				std::pair< void* , OnDisconnectedCallback>	m_pairOnDisconnected;

			};
		}
	}
}
