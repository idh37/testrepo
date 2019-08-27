#include "StdAfx.h"
#include "Connector.h"
#include "Camel/Base/System/SPHeap.h"
#include "Camel/Base/System/Runtime.h"
#include "BASECONNECTORAPIError.h"

namespace Base
{
	void __stdcall _OnConnected( Network::Connector::Handle hConnector, Network::Link::Handle hLink, void *pData)
	{
		Base::Connector* pInst = reinterpret_cast<Base::Connector* >(pData);
		if ( NULL == pInst)
		{
			return;
		}
		pInst->OnConnected( hLink);
		Network::Link::Recv(hLink, pInst->GetRecvByteLen());
	}

	void __stdcall _OnReceive(Network::Connector::Handle, Network::Link::Handle link, void *pData)
	{
		Base::Connector* pInst = reinterpret_cast<Base::Connector* >(pData);
		if ( NULL == pInst)
		{
			return;
		}
		pInst->OnReceived( link);
		Network::Link::Recv(link, pInst->GetRecvByteLen());
	}

	void __stdcall _OnDisconnected(Network::Connector::Handle, Network::Link::Handle link, void *pData)
	{
		Base::Connector* pInst = reinterpret_cast<Base::Connector* >(pData);
		if ( NULL == pInst)
		{
			return;
		}
		pInst->OnDisconnected( link);
	}
			
	Connector::Connector(void)
	{
		m_hThreadPool = NULL;
		m_hConnector = NULL;
		m_hStream = NULL;
		m_hLink = NULL;

		m_pairOnConnected.first = NULL;
		m_pairOnConnected.second = NULL;

		m_pairOnReceived.first = NULL;
		m_pairOnReceived.second = NULL;

		m_pairOnReceivedError.first = NULL;
		m_pairOnReceivedError.second = NULL;

		m_pairOnDisconnected.first = NULL;
		m_pairOnDisconnected.second = NULL;

		SetRecvByteLen(DEFAULT_BYTE_LEN);
		m_eConnectorState = Connector::None;
	}

	Connector::~Connector(void)
	{
		UnInitialize();
	}


	bool Connector::Initialize(const char* szAddr, unsigned short nPort, System::ThreadPool::Handle hRecvThreadPool, DWORD dwTimeOut /*= 1000*/)
	{
		if ( Connector::None != m_eConnectorState)
		{
			Camel::Base::System::Runtime::SetLastError(BASE_ERROR_INVALID_STATE);
			return false;
		}
		if ( NULL == szAddr)
		{
			Camel::Base::System::Runtime::SetLastError(BASE_ERROR_INVALID_PARAM);
			return false;
		}
		if ( NULL == hRecvThreadPool)
		{
			Camel::Base::System::Runtime::SetLastError(BASE_ERROR_INVALID_PARAM);
			return false;
		}

		m_strIP = szAddr;
		m_sPort = nPort;

		m_hStream = System::Stream::CreateHandle();
		if (NULL == m_hStream)
		{
			return false;
		}
		m_hThreadPool = hRecvThreadPool;
		m_hConnector = Network::Connector::CreateHandle( m_hThreadPool);
		if ( NULL == m_hConnector)
		{
			return false;
		}

		Network::Connector::SetOnConnected( m_hConnector, Base::_OnConnected, this);
		Network::Connector::SetOnReceived( m_hConnector, Base::_OnReceive, this);
		Network::Connector::SetOnDisconnected( m_hConnector, Base::_OnDisconnected, this);

		m_dwTimeoutInterval = dwTimeOut;
		m_eConnectorState = Connector::InitComplete;
		Camel::Base::System::Runtime::SetLastError(BASE_SUCCESS);
		return true;
	}

	bool Connector::UnInitialize()
	{
		if ( NULL != m_hConnector)
		{
			Network::Connector::DestroyHandle( m_hConnector);
			m_hConnector = NULL;
		}
		if (NULL != m_hStream)
		{
			System::Stream::DestroyHandle(m_hStream);
			m_hStream = NULL;
		}

		m_hThreadPool = NULL;
		m_eConnectorState = Connector::None;
		return true;
	}

	bool Connector::Connect()
	{
		if ( Connector::InitComplete != m_eConnectorState)
		{
			Camel::Base::System::Runtime::SetLastError(BASE_ERROR_INVALID_STATE);
			return false;
		}

		if (false == Network::Connector::Connect( m_hConnector, GetIP().c_str(), m_sPort, m_dwTimeoutInterval))
		{
			return false;
		}

		if ( m_clConnectEvent.Wait(m_dwTimeoutInterval) == false)
		{
			Camel::Base::System::Runtime::SetLastError(BASE_ERROR_SYSTEM);
			return false;
		}

		Camel::Base::System::Runtime::SetLastError(BASE_SUCCESS);
		return true;
	}

	bool Connector::Reconnect()
	{
		if ( Connector::Disconnected != m_eConnectorState)
		{
			Camel::Base::System::Runtime::SetLastError(BASE_ERROR_INVALID_STATE);
			return false;
		}

		if (false == Network::Connector::Connect( m_hConnector, GetIP().c_str(), m_sPort, m_dwTimeoutInterval))
		{
			return false;
		}

		if ( m_clConnectEvent.Wait(m_dwTimeoutInterval) == false)
		{
			Camel::Base::System::Runtime::SetLastError(BASE_ERROR_SYSTEM);
			return false;
		}

		Camel::Base::System::Runtime::SetLastError(BASE_SUCCESS);
		return true;
	}

	void Connector::Disconnect()
	{
		if ( NULL != m_hLink)
		{
			if (Connector::Disconnected != m_eConnectorState)
			{
				Network::Link::Close( m_hLink);
				m_hLink = NULL;
			}
		}
	}

	void Connector::OnConnected( Network::Link::Handle hLink)
	{
		m_hLink = hLink;
		Network::Link::AddRef( m_hLink);

		m_eConnectorState = Connector::Connected;
		m_clConnectEvent.Set();

		if (NULL != m_pairOnConnected.second)
		{
			m_pairOnConnected.second(m_pairOnConnected.first);
		}
	}
	void Connector::OnDisconnected( Network::Link::Handle hLink)
	{
		Camel::Base::System::Stream::Clear(m_hStream);
		m_eConnectorState = Connector::Disconnected;
		if ( NULL != m_pairOnDisconnected.second)
		{
			m_pairOnDisconnected.second( m_pairOnDisconnected.first);
		}

		Network::Link::Release( hLink);
		// m_hLink = NULL; // BUGFIX : 이 문장은 사용 금지. user callback 내부에서 reconnect를 시도할 경우 m_hLink는 이미 새로운 링크의 핸들로 대체되어 있을 것이다.
	}

	bool Connector::SetOnConnected( OnConnectedCallback pCallback, void* pData )
	{
		m_pairOnConnected.first = pData;
		m_pairOnConnected.second = pCallback;
		return true;
	}

	bool Connector::SetOnReceived( OnReceivedCallback pCallback, void* pData)
	{
		m_pairOnReceived.first = pData;
		m_pairOnReceived.second = pCallback;
		return true;
	}

#if defined (__CAMEL_DAR2_BASEFRAMEWORK__)
	bool Connector::SetOnReceivedDar2(OnReceivedDar2Callback pCallback, void* pData)
	{
		m_pairOnReceivedDar2.first = pData;
		m_pairOnReceivedDar2.second = pCallback;
		return true;
	}
#endif // (__CAMEL_DAR2_BASEFRAMEWORK__)

	bool Connector::SetOnReceivedError(OnReceivedErrorCallback pCallback, void* pData)
	{
		m_pairOnReceivedError.first = pData;
		m_pairOnReceivedError.second = pCallback;
		return true;
	}

	bool Connector::SetOnDisconnected( OnDisconnectedCallback pCallback, void* pData)
	{
		m_pairOnDisconnected.first = pData;
		m_pairOnDisconnected.second = pCallback;
		return true;
	}

	bool Connector::Send(Camel::Dar::IDarMsg& rMessage)
	{
		if ( NULL == m_hLink)
		{
			Camel::Base::System::Runtime::SetLastError(BASE_ERROR_NOT_INIT);
			return false;
		}
		if ( Connector::Connected != m_eConnectorState )
		{
			Camel::Base::System::Runtime::SetLastError(BASE_ERROR_INVALID_STATE);
			return false;
		}

		Camel::Dar::PacketHeader clHeader;

		clHeader.MakeHeader( &rMessage);

		System::Stream::Handle hSendStream = System::Stream::CreateHandle();
		System::Stream::Write( hSendStream, &clHeader, sizeof(clHeader));
		rMessage.Save( hSendStream);

		int nSize = System::Stream::GetLength(hSendStream);
		int nResult = Network::Link::Send( m_hLink, System::Stream::GetRawPtr(hSendStream), nSize);
		System::Stream::DestroyHandle( hSendStream);

		Camel::Base::System::Runtime::SetLastError(BASE_SUCCESS);
		return true;
	}

#if defined (__CAMEL_DAR2_BASEFRAMEWORK__)
	bool Connector::Send( Camel::Dar2::IDarMsg& rMessage)
	{
		if ( NULL == m_hLink)
		{
			Camel::Base::System::Runtime::SetLastError(BASE_ERROR_NOT_INIT);
			return false;
		}
		if ( Connector::Connected != m_eConnectorState )
		{
			Camel::Base::System::Runtime::SetLastError(BASE_ERROR_INVALID_STATE);
			return false;
		}

		Camel::Dar2::PacketHeader clHeader;
		clHeader.MakeHeader( &rMessage);

		System::Stream::Handle hSendStream = System::Stream::CreateHandle();
		System::Stream::Write( hSendStream, &clHeader, sizeof(clHeader));
		rMessage.Save( hSendStream);

		int nSize = System::Stream::GetLength(hSendStream);
		int nResult = Network::Link::Send( m_hLink, System::Stream::GetRawPtr(hSendStream), nSize);
		System::Stream::DestroyHandle( hSendStream);

		Camel::Base::System::Runtime::SetLastError(BASE_SUCCESS);
		return true;
	}
#endif // (__CAMEL_DAR2_BASEFRAMEWORK__)

	void Connector::OnReceived( Network::Link::Handle hLink)
	{
		System::Stream::Handle hStream = Network::Link::LockReadStream( hLink);
		System::SPHeap::Block pRecvBlock = reinterpret_cast<System::SPHeap::Block>(System::Stream::Detach( hStream));
		Network::Link::UnlockReadStream( hLink);

		int nSize = System::SPHeap::GetSize( pRecvBlock);
		System::Stream::Write( m_hStream, pRecvBlock, nSize);

		System::SPHeap::Free( pRecvBlock);

		int nPacketHeaderLen = PACKET_HEADER_LEN;
		char arHeader[ PACKET_HEADER_LEN] = {0,};
#if defined (__CAMEL_DAR2_BASEFRAMEWORK__)
		int nPacketBodyLen = 0;
		while (true)
		{
			int nStreamWholeLength = System::Stream::GetLength( m_hStream);
			if (nPacketHeaderLen <= nStreamWholeLength)
			{
				// packet header valid check
				System::Stream::Peek(m_hStream, arHeader, nPacketHeaderLen);

				// 우선 dar version정보를 확인해본후에, dar2로 디스패칭할지 dar로 디스패칭할지 결정한다.
				Camel::Dar2::PacketHeader packetHeader;
				if ( packetHeader.InitHeader( arHeader, sizeof(arHeader)) == false)
				{
					Camel::Base::System::Runtime::SetLastError(BASE_ERROR_INVALID_PARAM);
					// callback으로 등록하게 해서 사용하는 입장에서 제어를 하도록 하는게 좋을듯??
					if ( NULL != m_pairOnReceivedError.second)
					{
						m_pairOnReceivedError.second( m_pairOnReceivedError.first, m_hStream);
					}
					break;
				}

				nPacketBodyLen = packetHeader.GetPacketBodyLength();
				if (nPacketBodyLen + nPacketHeaderLen <= nStreamWholeLength)
				{
					if (packetHeader.GetVERSION() < Camel::Dar2::VERSION)
					{
						Camel::Dar::PacketHeader clPacketHeaderDar1;
						if ( clPacketHeaderDar1.InitHeader( arHeader, sizeof(arHeader)) == false)
						{
							Camel::Base::System::Runtime::SetLastError(BASE_ERROR_INVALID_PARAM);
							// callback으로 등록하게 해서 사용하는 입장에서 제어를 하도록 하는게 좋을듯??
							if ( NULL != m_pairOnReceivedError.second)
							{
								m_pairOnReceivedError.second( m_pairOnReceivedError.first, m_hStream);
							}
							break;
						}
						System::Stream::RemoveLeft(m_hStream, nPacketHeaderLen);
						if ( m_pairOnReceived.second != NULL)
						{
							Camel::Base::System::Runtime::SetLastError(BASE_SUCCESS);
							m_pairOnReceived.second( m_pairOnReceived.first, clPacketHeaderDar1, m_hStream, nPacketBodyLen);
						}
						else
						{
							Camel::Base::System::Runtime::SetLastError(BASE_ERROR_NOT_INIT);
							// callback으로 등록하게 해서 사용하는 입장에서 제어를 하도록 하는게 좋을듯??
							if ( NULL != m_pairOnReceivedError.second)
							{
								m_pairOnReceivedError.second( m_pairOnReceivedError.first, m_hStream);
							}
							System::Stream::RemoveLeft(m_hStream, nPacketBodyLen);
						}
					}
					else	// Dar2 Dispatch
					{
						System::Stream::RemoveLeft(m_hStream, nPacketHeaderLen);
						if ( m_pairOnReceivedDar2.second != NULL)
						{
							Camel::Base::System::Runtime::SetLastError(BASE_SUCCESS);
							m_pairOnReceivedDar2.second( m_pairOnReceivedDar2.first, packetHeader, m_hStream, nPacketBodyLen);
						}
						else
						{
							Camel::Base::System::Runtime::SetLastError(BASE_ERROR_NOT_INIT);
							// callback으로 등록하게 해서 사용하는 입장에서 제어를 하도록 하는게 좋을듯??
							if ( NULL != m_pairOnReceivedError.second)
							{
								m_pairOnReceivedError.second( m_pairOnReceivedError.first, m_hStream);
							}
							System::Stream::RemoveLeft(m_hStream, nPacketBodyLen);
						}
					}
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
#else // (__CAMEL_DAR2_BASEFRAMEWORK__)
		int nPacketBodyLen = 0;
		while (true)
		{
			int nStreamWholeLength = System::Stream::GetLength( m_hStream);
			if (nPacketHeaderLen <= nStreamWholeLength)
			{
				// packet header valid check
				System::Stream::Peek(m_hStream, arHeader, nPacketHeaderLen);

				Camel::Dar::PacketHeader clPacketHeader;
				if ( clPacketHeader.InitHeader( arHeader, sizeof(arHeader)) == false)
				{
					Camel::Base::System::Runtime::SetLastError(BASE_ERROR_INVALID_PARAM);
					// callback으로 등록하게 해서 사용하는 입장에서 제어를 하도록 하는게 좋을듯??
					if ( NULL != m_pairOnReceivedError.second)
					{
						m_pairOnReceivedError.second( m_pairOnReceivedError.first, m_hStream);
					}
					break;
				}

				nPacketBodyLen = clPacketHeader.GetPacketBodyLength();
				if (nPacketBodyLen + nPacketHeaderLen <= nStreamWholeLength)
				{
					System::Stream::RemoveLeft(m_hStream, nPacketHeaderLen);
					if ( m_pairOnReceived.second != NULL)
					{
						Camel::Base::System::Runtime::SetLastError(BASE_SUCCESS);
						m_pairOnReceived.second( m_pairOnReceived.first, clPacketHeader, m_hStream, nPacketBodyLen);
					}
					else
					{
						Camel::Base::System::Runtime::SetLastError(BASE_ERROR_NOT_INIT);
						// callback으로 등록하게 해서 사용하는 입장에서 제어를 하도록 하는게 좋을듯??
						if ( NULL != m_pairOnReceivedError.second)
						{
							m_pairOnReceivedError.second( m_pairOnReceivedError.first, m_hStream);
						}
						System::Stream::RemoveLeft(m_hStream, nPacketBodyLen);
						
					}
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
#endif // (__CAMEL_DAR2_BASEFRAMEWORK__)
	}

	DWORD Connector::SetTimeOut(DWORD dwTimeOut)
	{
		DWORD dwOldTimeOut = m_dwTimeoutInterval;
		m_dwTimeoutInterval = dwTimeOut;
		return dwOldTimeOut;
	}

	unsigned int Connector::SetRecvByteLen(unsigned int nRecvByteLen)
	{
		unsigned int nOldValue = m_nRecvByteLen;
		m_nRecvByteLen = nRecvByteLen;
		return nOldValue;	
	}

	std::string Connector::GetIP()
	{
		hostent* pent = gethostbyname( m_strIP.c_str() );
		// domain name임.
		if( NULL != pent )
		{
			std::string IPAddr = inet_ntoa( *(IN_ADDR*)pent->h_addr_list[0] );
			return IPAddr;
		}

		return m_strIP;
	}

	unsigned short Connector::GetPort()
	{
		return m_sPort;
	}

	enum Connector::ConnectorState Connector::GetState()
	{
		return m_eConnectorState;
	}

	void Connector::SetIP( const char* ip )
	{
		m_strIP = std::string(ip);
	}
	void Connector::SetPort( const USHORT port )
	{
		m_sPort = port;
	}
}
