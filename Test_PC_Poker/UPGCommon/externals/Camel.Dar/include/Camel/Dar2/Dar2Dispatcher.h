#pragma once

#include "Dar2BaseMsg.h"
#include "DAR2Rule_STREAM.h"
#include <Camel/Base/Network/Network.h>

#define PACKET_HEADER_CHECK_STR		"DAR"
#define PACKET_HEADER_CHECK_LEN		3
#define PACKET_HEADER_RESERVED_LEN	1 
#define PACKET_HEADER_BODY_LEN		4
#define PACKET_HEADER_MSGID_LEN		4

#define PACKET_HEADER_LEN (PACKET_HEADER_CHECK_LEN + PACKET_HEADER_RESERVED_LEN + PACKET_HEADER_BODY_LEN + PACKET_HEADER_MSGID_LEN)

namespace Camel
{
	namespace Dar2
	{
		const char VERSION = 2;
		enum ReservedBitMask
		{
			EndianCheck = 128,
		};
		enum EndianType
		{
			LittleEndian = 0,
			BigEndian = 1,
		};

		typedef union 
		{
			short m_sShort;
			char  m_arChar[2];
		} EndianUnionCheck;

		inline EndianType GetSystemEndianType()
		{
			EndianUnionCheck check;
			check.m_sShort = 0x0001;
			if ( check.m_arChar[1] == 0x01)
			{
				return BigEndian;
			}
			else
			{
				return LittleEndian;
			}
		}

		inline int GetSystemVERSION()	// local darversion
		{
			return VERSION;
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////
		class PacketHeader
		{
		public:
			PacketHeader()		{	cReserved=0;		nBodyLength = 0;	uMsgId=0;	}
			~PacketHeader()		{}

			bool InitHeader(const char* arHeader, int nArraySize)
			{
				if ( PACKET_HEADER_LEN != nArraySize)
				{
					return false;
				}

				::memcpy( arrCheck, arHeader, PACKET_HEADER_CHECK_LEN);
				if ( ::_memicmp( PACKET_HEADER_CHECK_STR, arrCheck, PACKET_HEADER_CHECK_LEN) !=0)
				{
					return false;
				}

				cReserved = *(arHeader + PACKET_HEADER_CHECK_LEN);

				::memcpy( &nBodyLength, (arHeader + PACKET_HEADER_CHECK_LEN + PACKET_HEADER_RESERVED_LEN),  PACKET_HEADER_BODY_LEN);
				::memcpy( &uMsgId, (arHeader + PACKET_HEADER_CHECK_LEN + PACKET_HEADER_RESERVED_LEN + PACKET_HEADER_BODY_LEN), PACKET_HEADER_MSGID_LEN);

				if ( GetSystemEndianType() != GetPacketEndianType())
				{
					ChangeToByteOrder<int>( &nBodyLength);
					ChangeToByteOrder<unsigned int>(&uMsgId);
				}
				return true;
			}

			void MakeHeader( Camel::Dar2::IDarMsg* pMessage)
			{
				::memcpy( arrCheck, PACKET_HEADER_CHECK_STR, PACKET_HEADER_CHECK_LEN);
				_SetEndianType();
				_SetVersion();
				nBodyLength = pMessage->GetLength();
				uMsgId = pMessage->GetMessageId();
			}

			void MakeHeader( unsigned int msgid, int nLength)
			{
				::memcpy( arrCheck, PACKET_HEADER_CHECK_STR, PACKET_HEADER_CHECK_LEN);
				_SetEndianType();
				_SetVersion();
				nBodyLength = nLength;
				uMsgId = msgid;
			}

			int				GetPacketBodyLength()	const		{	return nBodyLength;	}
			unsigned int	GetPacketMsgId()		const		{	return uMsgId;			}

			EndianType GetPacketEndianType()
			{
				if ( (cReserved & EndianCheck) == 0)
				{
					return LittleEndian;
				}
				return BigEndian;
			}

			int GetVERSION()
			{
				int nVersion = cReserved & 7; // 00000111 = 7
				return nVersion;
			}

		private:
			void _SetEndianType()
			{
				if ( GetSystemEndianType() == BigEndian)
				{
					cReserved |= EndianCheck;
				}
			}

			void _SetVersion()
			{
				cReserved &= 248;	// 11111000 = 248
				cReserved |= VERSION;
			}

			char				arrCheck[PACKET_HEADER_CHECK_LEN];
			char				cReserved;
			int					nBodyLength;
			unsigned int		uMsgId;
		};


		//////////////////////////////////////////////////////////////////////////////////////////////////////
		template <typename _Ty, typename _Py>
		struct mem_fun_dispatch
		{
			typedef _Ty			instance_type ;

			mem_fun_dispatch ( instance_type * inst, bool (instance_type::*_Pm)( _Py, Camel::Dar2::IDarMsg*) )
			:	instance_(inst), method_(_Pm)
			{
			}

			bool operator() (_Py value, Camel::Dar2::IDarMsg* pMsg) const
			{
				return ( (instance_->*method_)(value, pMsg) );
			}

		private:
			instance_type * instance_ ;
			bool (instance_type::*method_)( _Py, Camel::Dar2::IDarMsg*) ;
		}; 

		template <typename _Ty, typename _Py, typename _TStream, typename _TSize>
		struct mem_fun_default_dispatch
		{
			typedef _Ty			instance_type ;

			mem_fun_default_dispatch ( instance_type * inst, bool (instance_type::*_Pm)( _Py, _TStream, _TSize ) )
				:	instance_(inst), method_(_Pm)
			{
			}

			bool operator() (_Py value, _TStream stream, _TSize size) const
			{
				return ( (instance_->*method_)(value, stream, size) );
			}

		private:
			instance_type * instance_ ;
			bool (instance_type::*method_)( _Py, _TStream, _TSize) ;
		}; 

		template <typename _Py>
		struct Dispatch
		{
			virtual bool process ( PacketHeader& rHeader, _Py value, Camel::Base::System::Stream::Handle hStream, int nPacketSize) = 0 ;
		};

		template <typename _Py>
		struct DefaultDispatch
		{
			virtual bool process ( PacketHeader& rHeader, _Py value, Camel::Base::System::Stream::Handle hStream, int nPacketSize) = 0 ;
		};

		template < typename darprotocol, typename Functor, typename _Py>
		class DispatchItem : public Dispatch<_Py>
		{
			typedef typename Functor::instance_type	instance_type ;
			typedef typename darprotocol darprotocol_type;

		public:
			DispatchItem ( Functor functor)
			:	functor_( functor )
			{
			}

		public:
			bool process ( PacketHeader& rHeader, _Py value, Camel::Base::System::Stream::Handle hStream, int nPacketSize)
			{
				int nBeforeStreamSize = Camel::Base::System::Stream::GetLength( hStream);
				darprotocol_type darmsg;
				bool bChangeByteOrder = (rHeader.GetPacketEndianType() != GetSystemEndianType());
				if ( false == darmsg.Load(hStream, IDarMsg::STREAM, bChangeByteOrder))
				{
					return false;
				}
				int nAfterStreamSize = Camel::Base::System::Stream::GetLength( hStream);
				if ( nBeforeStreamSize - nAfterStreamSize != nPacketSize)
				{
					return false;
				}

				return functor_ ( value, &darmsg);;
			}

		private:
			Functor			functor_ ;
		};

		template <typename Functor, typename _Py>
		class DefaultDispatchItem : public DefaultDispatch<_Py>
		{
			typedef typename Functor::instance_type	instance_type ;

		public:
			DefaultDispatchItem ( Functor functor)
				:	functor_( functor )
			{
			}

		public:

			bool process ( PacketHeader& rHeader, _Py value, Camel::Base::System::Stream::Handle hStream, int nPacketSize)
			{
				functor_ ( value, hStream, nPacketSize );
				return true;
			}

		private:
			Functor			functor_ ;
		};

		////////////////   DarDispatcher 선언  ////////////////////
		typedef	std::pair<std::string, unsigned int>	TMsgDispatchCount;	// <msg name, count>
		typedef	std::vector<TMsgDispatchCount>	TMsgDispatchCountList;

		template <typename _Py>
		class DarDispatcher
		{
		public:
			DarDispatcher(void)
			{
				m_pDefaultDispatcher = NULL;
				m_mapDemux.clear();
			}

			~DarDispatcher(void)
			{
				DispatchMap::iterator iter = m_mapDemux.begin();
				for (; iter != m_mapDemux.end(); ++iter)
				{
					Dispatch<_Py>* pDispatch = iter->second.first;
					delete pDispatch;
				}
				m_mapDemux.clear();

				if ( NULL != m_pDefaultDispatcher)
				{
					delete m_pDefaultDispatcher;
					m_pDefaultDispatcher = NULL;
				}
			}

		public:
			bool DoDispatch ( PacketHeader& rHeader, _Py typeValue, Camel::Base::System::Stream::Handle hstream, int nPacketSize )
			{
				unsigned int msgid = rHeader.GetPacketMsgId();

				DispatchMap::iterator iter = m_mapDemux.find( msgid);
				if ( m_mapDemux.end() == iter)
				{
					if ( NULL != m_pDefaultDispatcher)
					{
						return m_pDefaultDispatcher->process( rHeader, typeValue, hstream, nPacketSize);
					}
					else
					{
						Camel::Base::System::Stream::RemoveLeft( hstream, nPacketSize);
						return false;
					}
				}

				Camel::Base::System::Threading::Interlocked::Increment(&iter->second.second.second);
				if ( false == (iter->second.first)->process( rHeader,typeValue, hstream, nPacketSize))
				{
					Camel::Base::System::Stream::Clear(hstream);
					return false;
				}
				
				return true;
			}

			template <typename DarProtocol, typename _Ty>
			bool AddDispatchFunc( _Ty* inst,  bool (_Ty::*_Pm)(_Py , Camel::Dar2::IDarMsg* ))
			{
				typedef mem_fun_dispatch<_Ty, _Py> Functor;
				Functor functor( inst, _Pm);

				DispatchMap::iterator iter = m_mapDemux.find( DarProtocol::msgid);
				if ( iter == m_mapDemux.end())
				{
					Dispatch<_Py>* pDispatch = new DispatchItem<DarProtocol, Functor, _Py>(functor);

					DarProtocol _darInst;	// todo: GetMessageName -> static 으로 변경 필요(DarProtocol::MsgName)
					m_mapDemux.insert( std::make_pair( DarProtocol::msgid, std::make_pair(pDispatch, TMsgDispatchCount(_darInst.GetMessageName(), 0))));
				}
				else
				{
					return false;
				}
				return true;
			}

			template <typename _Ty>
			void SetDefaultDispatchFunc(_Ty* inst,  bool (_Ty::*_Pm)(_Py, Camel::Base::System::Stream::Handle /*hstream */, int /* nPacketSize */))
			{
				typedef mem_fun_default_dispatch<_Ty, _Py, Camel::Base::System::Stream::Handle, int> Functor;
				Functor functor( inst, _Pm);
				m_pDefaultDispatcher = new DefaultDispatchItem<Functor, _Py>(functor);
			}

			void GetMsgDispatchCountList(OUT TMsgDispatchCountList& vecTMsgDispatchCount)
			{
				for (DispatchMap::const_iterator it = m_mapDemux.begin(); m_mapDemux.end() != it; ++it)
				{
					vecTMsgDispatchCount.push_back(std::make_pair(it->second.second.first, it->second.second.second));
				}
			}

		private:
			typedef std::map< unsigned int, std::pair<Dispatch<_Py>*, TMsgDispatchCount> >	DispatchMap;
			DispatchMap m_mapDemux;

			DefaultDispatch<_Py>* m_pDefaultDispatcher;
		};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		inline bool SendMsg( Camel::Base::Network::Link::Handle  hLink, Camel::Dar2::IDarMsg* pMessage)
		{
			Camel::Dar2::PacketHeader clHeader;
			clHeader.MakeHeader( pMessage);

			Camel::Base::System::Stream::Handle hSendStream = Camel::Base::System::Stream::CreateHandle();
			Camel::Base::System::Stream::Write(hSendStream, &clHeader, sizeof(clHeader));
			pMessage->Save((Base::System::Stream::Handle)hSendStream, IDarMsg::STREAM);

			int nsize = Camel::Base::System::Stream::GetLength( hSendStream);
			bool bSentRet = Camel::Base::Network::Link::Send(hLink, Camel::Base::System::Stream::GetRawPtr(hSendStream), nsize);
			Camel::Base::System::Stream::DestroyHandle(hSendStream);
			return bSentRet;
		}
	}
}
