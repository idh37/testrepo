#include "stdafx.h"
#include <process.h>
#include <afxinet.h>
#include "StatisticsManager.h"


namespace NMBASE
{
	namespace UTIL
	{

		CStatisticsManager& gStatisticsMan()
		{
			return CStatisticsManager::GetInstance();
		}

		DWORD WINAPI StatisticsThreadStartProc( LPVOID lpParameter )
		{
			CStatisticsManager *pStatisticsManager = reinterpret_cast<CStatisticsManager *>( lpParameter );

			pStatisticsManager->LoaderThread_Entry();

			return(0);
		}

		CStatisticsManager::CStatisticsManager() 
			: m_hStatistics( NULL )
		{
			::InitializeCriticalSection( &m_Tasks_cs );
			m_nStatisticsThreadID = 0;
			m_bExit = FALSE;

			// start threads
			BeginManager();
		}


		CStatisticsManager::~CStatisticsManager()
		{
			Destroy();
		}


		void CStatisticsManager::Destroy()
		{
			m_bExit = TRUE;		

			::DeleteCriticalSection( &m_Tasks_cs );

			if( m_hStatistics )
			{		
				::CloseHandle( m_hStatistics );			
				m_hStatistics = NULL;
			}
		}

		CStatisticsManager& CStatisticsManager::GetInstance()
		{
			static CStatisticsManager s_oStatisticsManager;
			return s_oStatisticsManager;
		}

		// Data Load Thread
		void CStatisticsManager::LoaderThread_Entry()
		{
			while(!m_bExit)
			{
				Sleep(10);

				if(m_StatisticsQueue.GetQueueCount())
				{
					CString strURL;

					bool bReturn = m_StatisticsQueue.RemoveFromFront( strURL );	

					if(bReturn)
					{
						SendStatisticData(strURL.GetBuffer());
					}
				}						
			}
		}

		void CStatisticsManager::AddStatisticsURL(LPCTSTR pURL)
		{
			m_StatisticsQueue.AddToBack((CString)pURL);
		}

		void CStatisticsManager::DeleteFront()
		{
			m_StatisticsQueue.RemoveFront();
		}

		// start Data Loading threads
		bool CStatisticsManager::BeginManager()
		{	
			if(NULL == (m_hStatistics = (HANDLE)_beginthreadex(NULL, 0, (unsigned int(__stdcall*)(void*))StatisticsThreadStartProc, (void*)this, 0, (unsigned *)&m_nStatisticsThreadID))) 
			{
				throw( "CStatisticsManager::BeginLoading(): can't create loader thread!" );
			}

			return true;
		}


		int CStatisticsManager::SendStatisticData(LPCTSTR szURL)
		{
			int nReturn = -1;

			if( szURL == NULL )
			{
				nReturn = -1;
			}
			else
			{
				DWORD dwServiceTye = 0;
				CString strServer = 0, strObject = 0;
				INTERNET_PORT nPort;

				if(!AfxParseURL(szURL, dwServiceTye, strServer, strObject, nPort))
				{
					nReturn = -2;
				}
				else
				{
					HINTERNET hSession = ::InternetOpen ("StartPage", 0, NULL, NULL, NULL);

					if( hSession == NULL )
					{
						nReturn = -3;
					}
					else
					{
						HINTERNET hConnect = ::InternetConnect (hSession, strServer, nPort, NULL, NULL, INTERNET_SCHEME_HTTP, NULL, NULL);

						if( hConnect == NULL )
						{
							nReturn = -4;
						}
						else
						{
							DWORD dwFlags = INTERNET_FLAG_RELOAD;
							HINTERNET hObject = ::HttpOpenRequest ( hConnect, NULL, strObject, _T("HTTP/1.1"), NULL, NULL, INTERNET_FLAG_RELOAD, NULL);

							if( hObject == NULL )
							{
								nReturn = -3;
							}
							else
							{
								if(::HttpSendRequest(hObject, NULL, 0L, NULL, 0) == FALSE )
								{
									nReturn = -4;
								}
								else
								{
									DWORD dwRead = 0;
									char szRet  [1024] = "\0";

									::InternetReadFile(hObject, szRet, 1023, &dwRead);

									nReturn = dwRead;
								}
								::InternetCloseHandle(hObject);
							}
							::InternetCloseHandle(hConnect);
						}	
						::InternetCloseHandle(hSession);
					}
				}
			}

			return nReturn;
		}

	}//namespace UTIL

}//namespace NMBASE