#pragma once

#include <string>
#include "Camel/Base/Network/Connector.h"
#include "Camel/Base/Network/Link.h"
#include "Camel/Base/System/System.h"
#include "Camel/Base/System/Stream.h"
#include "Camel/Dar/DarBaseMsg.h"
#include "Camel/Dar/DarDispatcher.h"
#if defined (__CAMEL_DAR2_BASEFRAMEWORK__)
#include "Camel/Dar2/Dar2Dispatcher.h"
#endif // (__CAMEL_DAR2_BASEFRAMEWORK__)

using namespace Camel::Base;

#define DEFAULT_BYTE_LEN 1024

namespace Base
{
	typedef bool (__stdcall	*OnConnectedCallback )	( void* );
	typedef bool (__stdcall	*OnReceivedCallback )	( void* , Camel::Dar::PacketHeader& rHeader, Camel::Base::System::Stream::Handle, int nPacketSize);
	typedef bool (__stdcall	*OnReceivedErrorCallback )	( void* , Camel::Base::System::Stream::Handle);
#if defined (__CAMEL_DAR2_BASEFRAMEWORK__)
	typedef bool (__stdcall	*OnReceivedDar2Callback )	( void* , Camel::Dar2::PacketHeader& rHeader, Camel::Base::System::Stream::Handle, int nPacketSize);
#endif // (__CAMEL_DAR2_BASEFRAMEWORK__)
	typedef bool (__stdcall	*OnDisconnectedCallback )	( void* );

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
//		bool	Initialize(const char* szAddr, unsigned short nPort, unsigned int nRecvThreadCnt, DWORD dwTimeOut = 1000);
		bool	Initialize(const char* szAddr, unsigned short nPort, System::ThreadPool::Handle hRecvThreadPool, DWORD dwTimeOut = 1000);
		bool	UnInitialize();
		bool	Connect();
		bool	Reconnect();
		void	Disconnect();

		void	OnReceived(Network::Link::Handle hLink);
		void	OnConnected(Network::Link::Handle hLink);
		void	OnDisconnected(Network::Link::Handle hLink);

		bool	SetOnConnected(OnConnectedCallback pCallback, void* pData);
		bool	SetOnReceived(OnReceivedCallback pCallback, void* pData);
#if defined (__CAMEL_DAR2_BASEFRAMEWORK__)
		bool	SetOnReceivedDar2(OnReceivedDar2Callback pCallback, void* pData);
#endif // (__CAMEL_DAR2_BASEFRAMEWORK__)
		bool	SetOnReceivedError(OnReceivedErrorCallback pCallback, void* pData);
		bool	SetOnDisconnected(OnDisconnectedCallback pCallback, void* pData);

		DWORD 	SetTimeOut(DWORD dwTimeOut);
		DWORD	GetTimeOut()  const				{	return m_dwTimeoutInterval;	};

		unsigned int SetRecvByteLen(unsigned int dwTimeOut);
		unsigned int GetRecvByteLen() const		{	return m_nRecvByteLen;	}

		bool	Send( Camel::Dar::IDarMsg& rMessage);
#if defined (__CAMEL_DAR2_BASEFRAMEWORK__)
		bool	Send( Camel::Dar2::IDarMsg& rMessage);
#endif // (__CAMEL_DAR2_BASEFRAMEWORK__)

		std::string GetIP();
		unsigned short GetPort();
		enum ConnectorState GetState();

		void SetIP( const char* ip );
		void SetPort( const USHORT port );

	protected:
		std::pair<void* , OnConnectedCallback>		m_pairOnConnected;
		std::pair<void* , OnReceivedCallback>		m_pairOnReceived;
		std::pair<void* , OnReceivedErrorCallback>	m_pairOnReceivedError;
#if defined (__CAMEL_DAR2_BASEFRAMEWORK__)
		std::pair<void* , OnReceivedDar2Callback>	m_pairOnReceivedDar2;
#endif // (__CAMEL_DAR2_BASEFRAMEWORK__)
		std::pair<void* , OnDisconnectedCallback>	m_pairOnDisconnected;

	protected:
		ConnectorState m_eConnectorState;

		System::ThreadPool::Handle m_hThreadPool;
		Network::Connector::Handle m_hConnector;
		System::Stream::Handle m_hStream;

		Network::Link::Handle	m_hLink;

		std::string		m_strIP;
		unsigned short	m_sPort;

		DWORD			m_dwTimeoutInterval;
		unsigned int	m_nRecvByteLen;

		System::Threading::AutoResetEvent	m_clConnectEvent;
	};
}
