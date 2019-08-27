#include "StdAfx.h"
#include <algorithm>
#include "ThreadLock.h"
#include "./Delegate/MessageManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace NMBASE
{
	namespace UTIL
	{

		//BOOL CMessageManager::destroyed       = FALSE;
		//CMessageManager *CMessageManager::pInst = 0;


		/**
		* Get a CMessageManager instance. 
		*/
		CMessageManager& MM()
		{
			return CMessageManager::Get();
		}


		CMessageManager::CMessageManager(void)
		{
			InitializeCriticalSection(&m_cLock);

			Clear();
		}

		CMessageManager::~CMessageManager(void)
		{
			//destroyed = TRUE;

			DeleteCriticalSection(&m_cLock);

			Clear();	
		}

		CMessageManager& CMessageManager::Get()
		{
			static CMessageManager MessageManager;
			return MessageManager;
		}


		BOOL CMessageManager::AddMsgH(std::string stCommand, IFunctor* pFnt)
		{
			CThreadLock lock(&m_cLock);

			CDelegate* pDel = FindMsgDelegate( stCommand );

			if(!pDel)
			{
				// create a new delegate
				CDelegate del;
				del.Default( pFnt );

				m_mapMessage[ stCommand ] = del;
				return TRUE;
			}

			if( FALSE == pDel->Default( pFnt ) )
			{		
				//OutputDebugString("err");
				return FALSE;
			}

			return TRUE;
		}


		BOOL CMessageManager::AppendMsgH(std::string stCommand, IFunctor* pFunctor )
		{
			CThreadLock lock(&m_cLock);

			CDelegate* pDel = FindMsgDelegate( stCommand );

			if(!pDel )
			{
				// create a new delegate
				CDelegate del;
				del.Append( pFunctor );

				m_mapMessage[ stCommand ] = del;
				return TRUE;
			}

			if( FALSE == pDel->Append( pFunctor ) )
			{		
				//OutputDebugString("err");
				return FALSE;
			}

			return TRUE;
		}

		BOOL CMessageManager::OverrideMsgH(std::string stCommand, IFunctor* pFnt)
		{
			CThreadLock lock(&m_cLock);

			CDelegate* pDel = FindMsgDelegate( stCommand );
			if(!pDel )
			{
				// create a new delegate
				CDelegate del;
				del.Override( pFnt );

				m_mapMessage[ stCommand ] = del;
				return TRUE;
			}

			// Override
			if(FALSE ==  pDel->Override( pFnt) )
			{		
				//OutputDebugString("err");
				return FALSE;
			}

			return TRUE;
		}

		LRESULT CMessageManager::Call(std::string stCommand, WPARAM wParam, LPARAM lParam)
		{
			CThreadLock lock(&m_cLock);
			// Find
			IT it = m_mapMessage.find( stCommand );
			if(m_mapMessage.end() == it)
			{		
				//OutputDebugString("err");
				return FALSE;
			}

			// Call
			LRESULT nRet = it->second.Fire( wParam, lParam);

			if(nRet < 0)
			{		
				//OutputDebugString("err");
				return FALSE;
			}

			return nRet;
		}


		LRESULT	CMessageManager::CallDefault(std::string stCommand, WPARAM wParam, LPARAM lParam)
		{
			CThreadLock lock(&m_cLock);
			// Find
			IT it = m_mapMessage.find( stCommand );
			if(m_mapMessage.end() == it)
			{		
				//OutputDebugString("err");	
				return FALSE;
			}

			// Call
			LRESULT nRet = it->second.FireDefault( wParam, lParam);

			if(nRet < 0)
			{		
				//OutputDebugString("err");
				return FALSE;
			}

			return nRet;
		}


		BOOL CMessageManager::RemoveMsgH(std::string stCommand)
		{
			CThreadLock lock(&m_cLock);
			// Find
			IT it = m_mapMessage.find( stCommand );
			if( m_mapMessage.end() == it)
			{		
				//OutputDebugString("err");

				return FALSE;
			}

			// Remove
			it->second.Release();
			m_mapMessage.erase(it);
			return TRUE;
		}

		BOOL CMessageManager::RemoveAppendMsgH(std::string stCommand, IFunctor* pFunctor)
		{
			CThreadLock lock(&m_cLock);
			// Find
			IT it = m_mapMessage.find( stCommand );
			if( m_mapMessage.end() == it)
			{		
				//OutputDebugString("err");
				return FALSE;
			}

			// AppendRemove
			it->second.ReleaseAppend(pFunctor);
			pFunctor->Release();

			return TRUE;
		}

		BOOL CMessageManager::RemoveOverrideMsgH(std::string stCommand, IFunctor* pFunctor)
		{
			CThreadLock lock(&m_cLock);
			// Find
			IT it = m_mapMessage.find( stCommand );
			if( m_mapMessage.end() == it)
			{		
				//OutputDebugString("err");
				return FALSE;
			}

			// Remove
			it->second.ReleaseOverride(pFunctor);
			pFunctor->Release();

			return TRUE;
		}


		SFINFOS CMessageManager::Info(std::string stCommand)
		{
			/* WARN : Do NOT use assert, VERIFY, etc in this function */
			/* Because this fuction will be called in the watch window */

			// To return
			CThreadLock lock(&m_cLock);
			SFINFOS finfos = {NULL, NULL, NULL, NULL};

			/* Find */
			IT it = m_mapMessage.find( stCommand );
			if( m_mapMessage.end() == it)
			{
				return finfos;
			}

			return it->second.Info();
		}

		///< Command를 모두 삭제한다.
		void CMessageManager::ClearAllCommand()
		{
			// Clear map
			CThreadLock lock(&m_cLock);
			IT it;

			// Message
			for(it = m_mapMessage.begin(); it != m_mapMessage.end(); ++it)
			{
				it->second.Release();
			}
			m_mapMessage.clear();

			// Network
			for(it = m_mapNetWork.begin(); it != m_mapNetWork.end(); ++it)
			{
				it->second.Release();
			}
			m_mapNetWork.clear();	

		}

		void CMessageManager::Clear()
		{
			// Clear map
			CThreadLock lock(&m_cLock);

			ClearAllCommand();					

			DetachAll();
			_ASSERTE( m_cComponentList.empty() == TRUE);

			// POSTMSG 초기화 [11/18/2011 guesty]
			m_queMessage.clear();
		}


		CDelegate* CMessageManager::FindMsgDelegate( std::string stCommand )
		{
			CThreadLock lock(&m_cLock);
			IT it = m_mapMessage.find( stCommand );
			if( m_mapMessage.end() == it )
			{
				return NULL;
			}

			return &it->second;
		}

		CDelegate* CMessageManager::FindNetDelegate( std::string stCommand )
		{
			CThreadLock lock(&m_cLock);
			IT it = m_mapNetWork.find( stCommand );
			if( m_mapNetWork.end() == it )
			{
				return NULL;
			}

			return &it->second;
		}


		CDelegate* CMessageManager::FindNetDelegate( unsigned int nCommand )
		{
			CThreadLock lock(&m_cLock);
			MAPNUMBERIT it = m_mapNetWorkNumber.find( nCommand );
			if( m_mapNetWorkNumber.end() == it )
			{
				return NULL;
			}

			return &it->second;
		}

		void CMessageManager::DumpAllHandlers(BOOL bSort)
		{
			CThreadLock lock(&m_cLock);
#ifdef _DEBUG
			printf("------- Dump handlers (begin)------\n");
			//::OutputDebugString( "------- Dump handlers (begin)------\n");	
#endif

			if(!bSort) 
			{
				for(IT it = m_mapMessage.begin(); it != m_mapMessage.end(); ++it) 
				{
#ifdef _DEBUG
					printf(it->first.c_str());
					printf("\n");
					//::OutputDebugString( it->first.c_str() );			
					//::OutputDebugString( "\n" );
#endif
				}
			}
			else 
			{
				// Copy the map to a vector
				typedef std::vector<std::string> TClones;
				TClones clone( m_mapMessage.size() );

				IT it1 = m_mapMessage.begin();
				TClones::iterator it2 = clone.begin();
				for(;it1 != m_mapMessage.end(); ++it1, ++it2)		
				{
					*it2 = it1->first;
				}

				// Sort
				std::sort( clone.begin(), clone.end() );

				for( TClones::iterator it = clone.begin(); it != clone.end(); ++it ) 
				{
#ifdef _DEBUG
					printf(it->c_str());
					printf("\n");
					//::OutputDebugString( it->c_str() );
					//::OutputDebugString( "\n" );
#endif
				}
			}

#ifdef _DEBUG
			printf("------- Dump handlers (end)------\n");
			//::OutputDebugString( "------- Dump handlers (end) ------\n");
#endif
		}


		BOOL CMessageManager::AttachComponent(IComponent* pComponet)
		{
			_ASSERTE( NULL != pComponet);
			if(NULL == pComponet)
			{
				TRACE(_T("CMessageManager::Invalid Argument\n"));
			}

			//< Check if already exists.
			COMPONENT_IT it;
			it = find( m_cComponentList.begin(), m_cComponentList.end(), pComponet);
			if( m_cComponentList.end() != it)
			{
				TRACE(_T("CMessageManager::pComponet already exists\n"));
			}

			//< Notify Called
			BOOL bFlag = pComponet->OnAttached();
			if(FALSE == bFlag)
			{
				TRACE(_T("CMessageManager::pComponet->OnAttached failed\n"));
			}

			//< Attach
			m_cComponentList.push_back( pComponet);

			return TRUE;
		}


		BOOL  CMessageManager::DetachComponent(IComponent* pComponet)
		{
			_ASSERTE( NULL != pComponet);
			if(NULL == pComponet)
			{
				return FALSE;
			}

			//< Find pComponet
			COMPONENT_IT it;
			it = find( m_cComponentList.begin(), m_cComponentList.end(), pComponet);
			if(m_cComponentList.end() == it)
			{
				_ASSERTE( !"pComponet NOT found in DetachComponent()" );
				return FALSE;
			}

			//< Detach
			m_cComponentList.erase( it);

			//< Notify
			return pComponet->OnDetached();
		}


		void  CMessageManager::DetachAll()
		{
			COMPONENT_IT it;
			for(it = m_cComponentList.begin(); it != m_cComponentList.end(); ++it)
			{
				_ASSERTE( NULL != *it);
				if(NULL != *it)
				{
					IComponent* pComponet = *it;
					pComponet->OnDetached();
				}
			}

			m_cComponentList.clear();
		}



		///////////////////////////////////////////////////////////////////////////////////////////
		// Network Message
		//////////////////////////////////////////////////////////////////////////////////////////

		BOOL CMessageManager::AddNetMsgH(std::string stCommand, IFunctor* pFnt)
		{
			CThreadLock lock(&m_cLock);

			CDelegate* pDel = FindNetDelegate( stCommand );

			if(!pDel)
			{
				// create a new delegate
				CDelegate del;
				del.Default( pFnt );

				m_mapNetWork[ stCommand ] = del;
				return TRUE;
			}

			if( FALSE == pDel->Default( pFnt ) )
			{		
				//OutputDebugString("err");
				return FALSE;
			}

			return TRUE;
		}


		BOOL CMessageManager::AppendNetMsgH(std::string stCommand, IFunctor* pFunctor )
		{
			CThreadLock lock(&m_cLock);

			CDelegate* pDel = FindNetDelegate( stCommand );

			if(!pDel )
			{
				// create a new delegate
				CDelegate del;
				del.Append( pFunctor );

				m_mapNetWork[ stCommand ] = del;
				return TRUE;
			}

			if( FALSE == pDel->Append( pFunctor ) )
			{		
				//OutputDebugString("err");
				return FALSE;
			}

			return TRUE;
		}

		BOOL CMessageManager::OverrideNetMsgH(std::string stCommand, IFunctor* pFnt)
		{
			CThreadLock lock(&m_cLock);

			CDelegate* pDel = FindNetDelegate( stCommand );
			if(!pDel )
			{
				// create a new delegate
				CDelegate del;
				del.Override( pFnt );

				m_mapNetWork[ stCommand ] = del;
				return TRUE;
			}

			// Override
			if(FALSE ==  pDel->Override( pFnt) )
			{		
				//OutputDebugString("err");
				return FALSE;
			}

			return TRUE;
		}


		BOOL CMessageManager::RemoveNetMsgH(std::string stCommand)
		{
			CThreadLock lock(&m_cLock);
			// Find
			IT it = m_mapNetWork.find( stCommand );
			if( m_mapNetWork.end() == it)
			{		
				//OutputDebugString("err");

				return FALSE;
			}

			// Remove
			it->second.Release();
			m_mapNetWork.erase(it);
			return TRUE;
		}


		LRESULT CMessageManager::NetCall(std::string stCommand, WPARAM wParam, LPARAM lParam)
		{
			CThreadLock lock(&m_cLock);
			// Find
			IT it = m_mapNetWork.find( stCommand );
			if(m_mapNetWork.end() == it)
			{		
				//OutputDebugString("err");
				return FALSE;
			}

			// Call
			LRESULT nRet = it->second.Fire( wParam, lParam);

			if(nRet < 0)
			{		
				//OutputDebugString("err");
				return FALSE;
			}

			return nRet;
		}


		/////////////////////////////


		BOOL CMessageManager::AddNetMsgH(unsigned int nCommand, IFunctor* pFnt)
		{
			CThreadLock lock(&m_cLock);

			CDelegate* pDel = FindNetDelegate( nCommand );

			if(!pDel)
			{
				// create a new delegate
				CDelegate del;
				del.Default( pFnt );

				m_mapNetWorkNumber[ nCommand ] = del;
				return TRUE;
			}

			if( FALSE == pDel->Default( pFnt ) )
			{		
				//OutputDebugString("err");
				return FALSE;
			}

			return TRUE;
		}


		BOOL CMessageManager::AppendNetMsgH(unsigned int nCommand, IFunctor* pFunctor )
		{
			CThreadLock lock(&m_cLock);

			CDelegate* pDel = FindNetDelegate( nCommand );

			if(!pDel )
			{
				// create a new delegate
				CDelegate del;
				del.Append( pFunctor );

				m_mapNetWorkNumber[ nCommand ] = del;
				return TRUE;
			}

			if( FALSE == pDel->Append( pFunctor ) )
			{		
				//OutputDebugString("err");
				return FALSE;
			}

			return TRUE;
		}

		BOOL CMessageManager::OverrideNetMsgH(unsigned int nCommand, IFunctor* pFnt)
		{
			CThreadLock lock(&m_cLock);

			CDelegate* pDel = FindNetDelegate( nCommand );
			if(!pDel )
			{
				// create a new delegate
				CDelegate del;
				del.Override( pFnt );

				m_mapNetWorkNumber[ nCommand ] = del;
				return TRUE;
			}

			// Override
			if(FALSE ==  pDel->Override( pFnt) )
			{		
				//OutputDebugString("err");
				return FALSE;
			}

			return TRUE;
		}


		BOOL CMessageManager::RemoveNetMsgH(unsigned int nCommand)
		{
			CThreadLock lock(&m_cLock);
			// Find
			MAPNUMBERIT it = m_mapNetWorkNumber.find( nCommand );
			if( m_mapNetWorkNumber.end() == it)
			{		
				//OutputDebugString("err");

				return FALSE;
			}

			// Remove
			it->second.Release();
			m_mapNetWorkNumber.erase(it);
			return TRUE;
		}


		LRESULT CMessageManager::NetCall(unsigned int nCommand, WPARAM wParam, LPARAM lParam)
		{
			CThreadLock lock(&m_cLock);
			// Find
			MAPNUMBERIT it = m_mapNetWorkNumber.find( nCommand );
			if(m_mapNetWorkNumber.end() == it)
			{		
				//OutputDebugString("err");
				return FALSE;
			}

			// Call
			LRESULT nRet = it->second.Fire( wParam, lParam);

			if(nRet < 0)
			{		
				//OutputDebugString("err");
				return FALSE;
			}

			return nRet;
		}

		//////////////////////////////////////////////////



		// Post Message [7/25/2011 guesty]
		void CMessageManager::ProcPostMessage(void)
		{
			for(int i = 0; i < (int)m_queMessage.size(); i++)
			{
				POSTMSG msg;
				msg.stCommand	= m_queMessage.front().stCommand;
				msg.wParam		= m_queMessage.front().wParam;
				msg.lParam		= m_queMessage.front().lParam;

				m_queMessage.pop_front();
				Call(msg.stCommand, msg.wParam, msg.lParam);
			}
		}

		void CMessageManager::PostCall(std::string stCommand, WPARAM wParam, LPARAM lParam)
		{
			POSTMSG msg;
			msg.stCommand = stCommand;
			msg.wParam = wParam;
			msg.lParam = lParam;

			m_queMessage.push_back(msg);
		}

	}//namespace UTIL

}	//namespace NMBASE