#include "stdafx.h"
#include <process.h>
#include <afxinet.h>
#include "StreamManager.h"
#include "xOgg.h"

namespace NMBASE
{
	namespace SOUND
	{


		CStreamManager& gStreamMan()
		{
			return CStreamManager::GetInstance();
		}

		DWORD WINAPI StreamThreadStartProc( LPVOID lpParameter )
		{
			CStreamManager *pStreamManager = reinterpret_cast<CStreamManager *>( lpParameter );

			pStreamManager->StreamThread_Entry();

			return(0);
		}

		CStreamManager::CStreamManager() 
			: m_hStreamThread( NULL )/*,
			m_hTime( NULL )*/
		{
			::InitializeCriticalSection( &m_Tasks_cs );
			m_StreamThreadID = 0;

			m_bExit = FALSE;

			// start threads
			BeginManager();
		}


		CStreamManager::~CStreamManager()
		{
			Destroy();
		}

		void CStreamManager::Destroy()
		{
			m_bExit = TRUE;

 			/*if( m_hTime )
 			{
 				::CloseHandle( m_hTime );	
 			}*/

			//_endthreadex(0);

			::DeleteCriticalSection( &m_Tasks_cs );

			if( m_hStreamThread )
			{		
				::CloseHandle( m_hStreamThread );			
				m_hStreamThread = NULL;
			}
		}


		CStreamManager& CStreamManager::GetInstance()
		{
			static CStreamManager s_oStatisticsManager;
			return s_oStatisticsManager;
		}

		// Data Load Thread
		void CStreamManager::StreamThread_Entry()
		{
			//m_hTime = CreateEvent( NULL, FALSE, FALSE, NULL );

			while(!m_bExit)
			{
				// HANDLE  hHandle, DWORD dwMillseconds( The same code sleep(10) )
				//WaitForSingleObject( m_hTime, 10 );	
				Sleep(10);

				// Sound FX
				xOggStream* pOggStream = NULL;

				if(m_StreamQueue.GetQueueFront(pOggStream))
				{
					if(IsBadReadPtr(pOggStream, sizeof(xOggStream)))
					{
						__asm int 3
					}					
					
					bool bDelete = false;
					if(pOggStream->ServiceBuffer(bDelete))
					{
						//m_StreamQueue.RemoveFront();						
					}

					if (bDelete)
					{
						delete pOggStream;
						pOggStream = NULL;
					}
				}		

				// Sound BGM
				xOggStream* pOggStreamBGM = NULL;

				if(m_StreamQueueBGM.GetQueueFront(pOggStreamBGM))
				{
					if(IsBadReadPtr(pOggStreamBGM, sizeof(xOggStream)))
					{
						__asm int 3
					}						

					bool bDelete = false;
					if(pOggStreamBGM->ServiceBuffer(bDelete))
					{
						//m_StreamQueue.RemoveFront();
					}

					if (bDelete)
					{
						delete pOggStreamBGM;
						pOggStreamBGM = NULL;
					}
				}	
			}
		}


		void CStreamManager::DeleteFront(xOggStream* pStream)
		{
			m_StreamQueue.RemoveFront(pStream);
		}

		void CStreamManager::AddStream(xOggStream* pStream)
		{
			m_StreamQueue.AddToBack(pStream);
		}

		void CStreamManager::DeleteFrontBGM(xOggStream* pStream)
		{
			m_StreamQueueBGM.RemoveFront(pStream);
		}

		void CStreamManager::AddStreamBGM(xOggStream* pStream)
		{
			m_StreamQueueBGM.AddToBack(pStream);
		}


		// start Data Loading threads
		bool CStreamManager::BeginManager()
		{	
			// Thread created by the use of Type Base2005 setting.
			if(NULL == (m_hStreamThread = (HANDLE)_beginthreadex(NULL, 0, (unsigned int(__stdcall*)(void*))StreamThreadStartProc, (void*)this, 0, (unsigned *)&m_StreamThreadID))) 
			{
				throw( "CStreamManager::BeginLoading(): can't create loader thread!" );
			}

			return true;
		}

	} //namespace SOUND

}	//namespace NMBASE