#ifndef __BASECONNECTORDISPATCHER_DAR2__H__
#define __BASECONNECTORDISPATCHER_DAR2__H__

#include <Camel/Base/System/SharedPtr.h>
#include <Camel/Dar2/Dar2Dispatcher.h>

namespace BaseConnector_Dar2
{
	template <typename _Ty, typename DarProtocol>
	struct mem_fun_dispatch
	{
		typedef _Ty			instance_type ;

		mem_fun_dispatch ( instance_type * inst, bool (instance_type::*_Pm)(Camel::Base::System::TPL::SharedPtr<DarProtocol> pMsgSharedPtr) )
			:	instance_(inst), method_(_Pm)
		{
		}

		bool operator() (Camel::Base::System::TPL::SharedPtr<DarProtocol> pMsgSharedPtr) const
		{
			return ( (instance_->*method_)(pMsgSharedPtr) );
		}

	private:
		instance_type * instance_ ;
		bool (instance_type::*method_)( Camel::Base::System::TPL::SharedPtr<DarProtocol> pMsgSharedPtr) ;
	}; 


	class Dispatch
	{
	public:
		Dispatch()	{	}
		virtual ~Dispatch()	{}
		virtual bool init (Camel::Dar2::PacketHeader& rHeader, Camel::Base::System::Stream::Handle hStream, int nPacketSize) = 0;
		virtual Camel::Base::System::ThreadPool::Handle getthreadpool () = 0 ;
		virtual bool process () = 0 ;
		virtual Dispatch* clone () = 0 ;
		virtual unsigned int getmessageid() = 0;
	};

	template < typename darprotocol, typename Functor>
	class DispatchItem : public Dispatch
	{
		typedef typename Functor::instance_type	instance_type ;
		typedef typename darprotocol darprotocol_type;

	public:
		DispatchItem ( Functor functor, Camel::Base::System::ThreadPool::Handle threadpool)
			:	functor_( functor ), threadpool_(threadpool)
		{
			darmsgptr_ = Camel::Base::System::TPL::SharedPtr<darprotocol_type>(new darprotocol_type);
		}

		virtual ~DispatchItem()	{}

	public:
		bool init ( Camel::Dar2::PacketHeader& rHeader, Camel::Base::System::Stream::Handle hStream, int nPacketSize)
		{
			int nBeforeStreamSize = Camel::Base::System::Stream::GetLength( hStream);

			bool bChangeByteOrder = (rHeader.GetPacketEndianType() != Camel::Dar2::GetSystemEndianType());
			if ( false == darmsgptr_->Load(hStream, Camel::Dar2::IDarMsg::STREAM, bChangeByteOrder))
			{
				return false;
			}
			int nAfterStreamSize = Camel::Base::System::Stream::GetLength( hStream);
			if ( nBeforeStreamSize - nAfterStreamSize != nPacketSize)
			{
				return false;
			}
			return true;
		}

		Camel::Base::System::ThreadPool::Handle getthreadpool ()
		{
			return threadpool_;
		}

		bool process ()
		{
			return functor_ ( darmsgptr_);
		}

		unsigned int getmessageid()
		{
			return darmsgptr_->msgid;
		}

		Dispatch* clone ()
		{
			Dispatch* pDispatch = new DispatchItem<darprotocol, Functor>(functor_, threadpool_);
			return pDispatch;
		}

	private:
		Functor			functor_ ;
		Camel::Base::System::TPL::SharedPtr<darprotocol_type> darmsgptr_;
		Camel::Base::System::ThreadPool::Handle threadpool_;
	};


	class ConnectorDispatcher
	{
	public:
		ConnectorDispatcher(void)
		{
			m_mapDemux.clear();
			m_DefaultThreadPool = NULL;
		}

		~ConnectorDispatcher(void)
		{
			DispatchMap::iterator iter = m_mapDemux.begin();
			for (; iter != m_mapDemux.end(); ++iter)
			{
				Dispatch* pDispatch = iter->second;
				delete pDispatch;
			}
			m_mapDemux.clear();
		}
	public:
		static unsigned int __stdcall OnThreadEvent(void *context)
		{
			Dispatch* pDispath = static_cast<Dispatch* >(context);
			if ( NULL == pDispath)
			{
				return 0;
			}
			if ( false == pDispath->process())
			{
			}
			delete pDispath;
			return 0;
		}

		bool Initialize(Camel::Base::System::ThreadPool::Handle hThreadPool)
		{
			m_DefaultThreadPool = hThreadPool;
			return true;
		}
		void Uninitialize()
		{
			m_DefaultThreadPool = NULL;
		}

		bool DoDispatch ( Camel::Dar2::PacketHeader& rHeader, Camel::Base::System::Stream::Handle hstream, int nPacketSize )
		{
			unsigned int msgid = rHeader.GetPacketMsgId();

			DispatchMap::iterator iter = m_mapDemux.find( msgid);
			if ( m_mapDemux.end() == iter)
			{
				Camel::Base::System::Stream::RemoveLeft( hstream, nPacketSize);
				return false;
			}

			Dispatch* pDispath = iter->second->clone();
			if ( NULL == pDispath)
			{
				Camel::Base::System::Stream::RemoveLeft( hstream, nPacketSize);
				return false;
			}
			if ( false == pDispath->init( rHeader, hstream, nPacketSize))
			{
				delete pDispath;
				Camel::Base::System::Stream::Clear(hstream);
				return false;
			}
			if ( false == Camel::Base::System::ThreadPool::QueueWorkItem( pDispath->getthreadpool(), &ConnectorDispatcher::OnThreadEvent, pDispath))
			{
				bool bSuccess = pDispath->process();
				if ( false == bSuccess)
				{
					Camel::Base::System::Stream::Clear(hstream);
				}
				delete pDispath;
				return bSuccess;
			}

			return true;
		}

		template <typename DarProtocol, typename _Ty>
		bool AddDispatchFunc( _Ty* inst, bool (_Ty::*_Pm)(Camel::Base::System::TPL::SharedPtr<DarProtocol>), Camel::Base::System::ThreadPool::Handle hUserDefineThreadPool = NULL)
		{
			typedef mem_fun_dispatch<_Ty, DarProtocol> Functor;
			Functor functor( inst, _Pm);

			if ( (NULL == hUserDefineThreadPool) && (NULL == m_DefaultThreadPool) )
			{
				return false;
			}

			DispatchMap::iterator iter = m_mapDemux.find( DarProtocol::msgid);
			if ( iter == m_mapDemux.end())
			{
				Camel::Base::System::ThreadPool::Handle hUseThreadPool = NULL;
				if (NULL != hUserDefineThreadPool)
				{
					hUseThreadPool = hUserDefineThreadPool;
				}
				else
				{
					hUseThreadPool = m_DefaultThreadPool;
				}
				Dispatch* pDispatch = new DispatchItem<DarProtocol, Functor>(functor, hUseThreadPool);
				m_mapDemux.insert( std::make_pair( DarProtocol::msgid, pDispatch));
			}
			else
			{
				return false;
			}
			return true;
		}

	private:
		typedef std::map< unsigned int, Dispatch*>	DispatchMap;
		DispatchMap m_mapDemux;
		Camel::Base::System::ThreadPool::Handle m_DefaultThreadPool;
	};
}

#endif //__BASECONNECTORDISPATCHER_DAR2__H__