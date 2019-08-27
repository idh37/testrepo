// GameDirectGo.cpp: implementation of the CGameDirectGo class.
// 
// Creator			: Heo Jaemin <judge@cj.net>
// Create Date		: 2007/11/26
// LastModify Date	: 2008/01/22 ( 2 Modified )
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameDirectGo.h"
#include "SSOWeb.h"
#include "NMRunParam.h"

//-- 공통 파일

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace NMBASE
{
	namespace UTIL
	{

		///////////////////////////////////////////////////////////////////////
		// Etc Static Function

		static BOOL __FileExists(LPCTSTR lpszName) 
		{ 
			BOOL bRetVal;  
			UINT iDriveType; 

			if( lpszName == NULL )  
				bRetVal = FALSE;
			// If Drive Root
			else if( (iDriveType = GetDriveType(lpszName)) != DRIVE_UNKNOWN && iDriveType != DRIVE_NO_ROOT_DIR )
				bRetVal = FALSE;
			else
			{	
				HANDLE handle;
				WIN32_FIND_DATA fileData;  

				// search for file
				handle = FindFirstFile( lpszName, &fileData);

				if( handle == INVALID_HANDLE_VALUE )
					bRetVal = FALSE;
				else 
				{
					if( (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
						bRetVal = FALSE;
					else
						bRetVal = TRUE;
					FindClose(handle);  
				}  
			} 

			return bRetVal;   
		}

		//////////////////////////////////////////////////////////////////////
		// Construction/Destruction
		//////////////////////////////////////////////////////////////////////

		CGameDirectGo::CGameDirectGo()
		{
			m_pMutex = NULL;
			m_bExcuteGame = FALSE;
			m_strGameCode_This.Empty();
			m_strGameCode_Dest.Empty();
			m_strCheckURL.Empty();
		}

		CGameDirectGo::~CGameDirectGo()
		{

		}

		BOOL CGameDirectGo::IsInit()
		{
			return ( m_strGameCode_This.IsEmpty() == FALSE && m_strCheckURL.IsEmpty() == FALSE ) ;
		}

		ERROR_GAMEDIRECTGO CGameDirectGo::Init(LPCTSTR szGameCode_This, LPCTSTR szCheckURL, HANDLE* hMutex)
		{
			ERROR_GAMEDIRECTGO error;

			if( IsInit() )
			{
				error = ERROR_GAMEDIRECTGO_ALREADY_INIT;
			}
			else
			{
				m_strGameCode_This = szGameCode_This;
				m_strCheckURL = szCheckURL;
				m_pMutex = hMutex;
				m_bExcuteGame = FALSE;

				error = (IsInit() ? ERROR_GAMEDIRECTGO_SUCCESS : ERROR_GAMEDIRECTGO_NOT_INIT);
			}

			return error;
		}

		ERROR_GAMEDIRECTGO CGameDirectGo::GetSSOCookie(CString& strAuthCookie, CString& strDataCookie, CString& strCpCookie)
		{
			ERROR_GAMEDIRECTGO error;
			BOOL bSucc = NMGetSSOWebCookieString(strAuthCookie.GetBuffer(BUFFER_COOKIEDATA), strDataCookie.GetBuffer(BUFFER_COOKIEDATA), strCpCookie.GetBuffer(BUFFER_COOKIEDATA));
			strAuthCookie.ReleaseBuffer();
			strDataCookie.ReleaseBuffer();
			strCpCookie.ReleaseBuffer();

			if( bSucc == FALSE )
				error = ERROR_GAMEDIRECTGO_SSOCOOKIE_CANTGET;
			else
			{
				if( strAuthCookie.IsEmpty() == TRUE )
				{
					error = ERROR_GAMEDIRECTGO_SSOCOOKIE_MISSING;
				}
				else
				{
					error = ERROR_GAMEDIRECTGO_SUCCESS;
				}
			}	

			return error;
		}

		ERROR_GAMEDIRECTGO CGameDirectGo::SetSSOCookie(CHttpFile* pFile, LPCTSTR szURL_In)
		{
			ERROR_GAMEDIRECTGO error;

			if( szURL_In == NULL || pFile == NULL )
				error = ERROR_GAMEDIRECTGO_INVALIDPARAM;
			else	
			{
				CString strAuthCookie, strDataCookie, strCpCookie;

				error = GetSSOCookie(strAuthCookie, strDataCookie, strCpCookie);

				if( error == ERROR_GAMEDIRECTGO_SUCCESS )
				{
					CString szCookie;
					szCookie.Format("Cookie :   %s; %s; %s", strAuthCookie.GetString(), strDataCookie.GetString(), strCpCookie.GetString());
					if( pFile->AddRequestHeaders(szCookie) == FALSE )
						error = ERROR_GAMEDIRECTGO_HTTP_CANTSETCOOKIE;
				}
			}	

			return error;
		}

		ERROR_GAMEDIRECTGO CGameDirectGo::CheckHTMLData(LPCTSTR szURL)
		{
			ERROR_GAMEDIRECTGO error;

			if( szURL == NULL )
				error = ERROR_GAMEDIRECTGO_INVALIDPARAM;
			else
			{
				CInternetSession session("My Session");

				//DWORD dwHttpRequestFlags = INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_NO_AUTO_REDIRECT;
				//DWORD dwHttpRequestFlags = INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_NO_AUTO_REDIRECT | INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE;
				DWORD dwHttpRequestFlags = INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE;
				CString strServerName;
				CString strObject;
				INTERNET_PORT nPort;
				DWORD dwServiceType;

				// Parse URL
				if(!AfxParseURL(szURL, dwServiceType, strServerName, strObject, nPort) || dwServiceType != INTERNET_SERVICE_HTTP)
				{
					error = ERROR_GAMEDIRECTGO_HTTP_CANTPARSE;
				}
				else
				{
					CHttpConnection* pServer;
					pServer = session.GetHttpConnection(strServerName, nPort);

					if( pServer == NULL )
					{
						error = ERROR_GAMEDIRECTGO_HTTP_CANTCONNECT;
					}
					else
					{
						CHttpFile* pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET, strObject, NULL, 1, NULL, NULL, dwHttpRequestFlags);

						if( pFile == NULL )
						{
							error = ERROR_GAMEDIRECTGO_HTTP_CANTOPENFILE;
						}
						else
						{
							// Set Web Cookie
							error = SetSSOCookie(pFile, szURL);

							if( error == ERROR_GAMEDIRECTGO_SUCCESS )
							{	
								DWORD dwStatusCode;

								if(pFile->SendRequest() == 0)
								{
									error = ERROR_GAMEDIRECTGO_HTTP_CANTREQUEST;
								}
								else if(pFile->QueryInfoStatusCode(dwStatusCode)==FALSE || dwStatusCode != 200 )
								{
									error = ERROR_GAMEDIRECTGO_HTTP_UNKNWONSTATUSCODE;
								}
								else
								{	
									CString szBuffer;
									LPCTSTR szCode;
									error = ERROR_GAMEDIRECTGO_STATUS_UNKNOWN;

									try 
									{
										while(pFile->ReadString(szBuffer))  
										{
											szBuffer.TrimLeft();
											szCode = szBuffer.operator LPCTSTR();
											if( _tcsnicmp(szCode, "result=", 7) == 0 )
											{
												int iStausCode;
												iStausCode = _ttoi(&szCode[7]);

												switch(iStausCode)
												{
												case -3:
													error = ERROR_GAMEDIRECTGO_STATUS_NOTADULT;
													break;
												case -2:
													error = ERROR_GAMEDIRECTGO_STATUS_NOMONEY;
													break;
												case -1:
													error = ERROR_GAMEDIRECTGO_STATUS_NOTLOGIN;
													break;	
												case 0:
													error = ERROR_GAMEDIRECTGO_STATUS_DISABLE;
													break;
												case 1:
													error = ERROR_GAMEDIRECTGO_SUCCESS;
													break;
												}
												break;
											}
										}
									}
									catch(...)
									{
										error = ERROR_GAMEDIRECTGO_HTTP_CANTREAD;
									}
								}

							}	
							pFile->Close();
						}
						pServer->Close();
					}
				}
			}

			return error;
		}

		ERROR_GAMEDIRECTGO CGameDirectGo::ExistExcuteFile()
		{
			CString szExcuteFileName;

			return GetExcuteFile(szExcuteFileName);
		}

		ERROR_GAMEDIRECTGO CGameDirectGo::GetExcuteFile(CString& szExcuteFileName_Out)
		{
			ERROR_GAMEDIRECTGO error; 
			szExcuteFileName_Out.Empty();
			NMNMRUNPARAMAPIFUNC_ENVVAR::GetEnvVar(NMRUNPARAMAPIFUNC_ENVVAR::ENVVARNAME_COMMONPATH, szExcuteFileName_Out.GetBuffer(MAX_PATH+1), MAX_PATH);
			szExcuteFileName_Out.ReleaseBuffer();

			if( szExcuteFileName_Out.IsEmpty() )
			{
				error = ERROR_GAMEDIRECTGO_EXECFILE_NOTFOUND;
			}
			else
			{
				szExcuteFileName_Out.TrimRight("\\");
				szExcuteFileName_Out.TrimRight("/");

				szExcuteFileName_Out += "\\NMDirectGoGame.exe";

				if( __FileExists(szExcuteFileName_Out) == TRUE )
					error = ERROR_GAMEDIRECTGO_SUCCESS;
				else
					error = ERROR_GAMEDIRECTGO_EXECFILE_NOTFOUND;
			}

			return error;
		}

		ERROR_GAMEDIRECTGO CGameDirectGo::SetGameCodeDestination(LPCTSTR szGameCode_To)
		{
			ERROR_GAMEDIRECTGO error;

			if( IsInit() == FALSE )
			{
				error = ERROR_GAMEDIRECTGO_NOT_INIT;
			}
			else if( szGameCode_To == NULL )
			{
				error = ERROR_GAMEDIRECTGO_INVALIDPARAM;
			}
			else
			{
				error = ExistExcuteFile();
				if( error == ERROR_GAMEDIRECTGO_SUCCESS )
				{
					CString szURL;

					szURL = m_strCheckURL;
					szURL.Replace("[GameCodeFrom]", m_strGameCode_This);
					szURL.Replace("[GameCodeDest]", szGameCode_To);

					error = CheckHTMLData(szURL);
				}

				if( error == ERROR_GAMEDIRECTGO_SUCCESS )
				{
					m_strGameCode_Dest = szGameCode_To;
					m_bExcuteGame = TRUE;
				}
				else
				{
					m_strGameCode_Dest.Empty();
					m_bExcuteGame = FALSE;
				}
			}	

			return error;
		}

		ERROR_GAMEDIRECTGO CGameDirectGo::OnGameExit(DWORD dwDelayMillisecond)
		{
			ERROR_GAMEDIRECTGO error;

			if( IsInit() == FALSE ) 
			{
				error = ERROR_GAMEDIRECTGO_NOT_INIT;
			}
			else if( m_strGameCode_Dest.IsEmpty() || m_bExcuteGame == FALSE )
			{
				error = ERROR_GAMEDIRECTGO_NOT_SET_DEST;
			}
			else
			{
				CString szExcuteFileName;
				error = GetExcuteFile(szExcuteFileName);

				if( error == ERROR_GAMEDIRECTGO_SUCCESS )
				{
					CString strAuthCookie, strDataCookie, strCpCookie;

					error = GetSSOCookie(strAuthCookie, strDataCookie, strCpCookie);

					if( error == ERROR_GAMEDIRECTGO_SUCCESS )
					{
						// Mutext 제거
						try 
						{
							if( m_pMutex != NULL )
							{
								if( *m_pMutex != NULL )
								{
									::CloseHandle(*m_pMutex);
									*m_pMutex = NULL;
								}
								m_pMutex = NULL;
							}
						}
						catch(...)
						{
						}

						CNMNMRunParam cRunParam;

						if( ERROR_NMRUNPARAM_SUCCESS != cRunParam.SetParam(NMDIRECTGAMEGO_AUTHCOOKIE, strAuthCookie)	||
							ERROR_NMRUNPARAM_SUCCESS != cRunParam.SetParam(NMDIRECTGAMEGO_DATACOOKIE, strDataCookie)	||
							ERROR_NMRUNPARAM_SUCCESS != cRunParam.SetParam(NMDIRECTGAMEGO_CPCOOKIE, strCpCookie)		||
							ERROR_NMRUNPARAM_SUCCESS != cRunParam.SetParam(NMDIRECTGAMEGO_FROM, m_strGameCode_This)		||
							ERROR_NMRUNPARAM_SUCCESS != cRunParam.SetParam(NMDIRECTGAMEGO_TO, m_strGameCode_Dest) )
						{
							error = ERROR_GAMEDIRECTGO_CBDATA_CANTWRITE;
						}
						else	
						{
							UINT iRet = 0;

							if( dwDelayMillisecond > 0 )
							{
								::Sleep(dwDelayMillisecond);
							}

							if( cRunParam.RunProgram(szExcuteFileName, &iRet) == ERROR_NMRUNPARAM_SUCCESS && iRet > 32 )
							{
								error = ERROR_GAMEDIRECTGO_SUCCESS;
							}
							else
							{
								error = ERROR_GAMEDIRECTGO_EXECFILE_NOTEXCUTE;
							}
						}
					}
				}	
			}


			return error;
		}

		///////////////////////////////////////////////////////////////////////
		// Global Section

		CGameDirectGo	g_GameDirectGo;

		ERROR_GAMEDIRECTGO NMGoGameDirect_Init(HANDLE* hMutex, LPCTSTR szGameCode_This, LPCTSTR szCheckURL)
		{
			if( szCheckURL == NULL )
			{
				szCheckURL = "http://game1.netmarble.net/channel/client/GameChannel.asp?BfGame=[GameCodeFrom]&AfGame=[GameCodeDest]&param=check";
			}

			return g_GameDirectGo.Init(szGameCode_This, szCheckURL, hMutex);
		}

		ERROR_GAMEDIRECTGO NMGoGameDirect_SetDestination(LPCTSTR szGameCode_To, CString& strErrorMsg_Out)
		{
			ERROR_GAMEDIRECTGO error;

			error = g_GameDirectGo.SetGameCodeDestination(szGameCode_To);

			switch(error)
			{
			case ERROR_GAMEDIRECTGO_SUCCESS:
				// 게임 종료
				strErrorMsg_Out = _T("성공");
				break;

				// 성인 인증
			case ERROR_GAMEDIRECTGO_STATUS_NOTADULT:	
				// 게임 종료
				strErrorMsg_Out = _T("Error : 성인 인증이 필요한 게임입니다. 해당 게임에 입장할 수 없습니다.");
				break;

				// 머니 부족
			case ERROR_GAMEDIRECTGO_STATUS_NOMONEY:
				strErrorMsg_Out = _T("Error : 게임 머니가 부족하여 해당 게임에 입장할 수 없습니다.");
				break;

				// 해당 게임이 정기점검
			case ERROR_GAMEDIRECTGO_STATUS_DISABLE:
				strErrorMsg_Out = _T("Error : 해당 게임이 현재 정기점검중입니다.");
				break;

				// 쿠키 정보 세팅 오류
			case ERROR_GAMEDIRECTGO_STATUS_NOTLOGIN:
			case ERROR_GAMEDIRECTGO_STATUS_UNKNOWN:
			default:
				strErrorMsg_Out.Format("Error : 내부오류로 인하여 게임을 실행 할 수 없습니다. (오류코드 : %d)", error);
				break;
			}

			return error;
		}


		ERROR_GAMEDIRECTGO NMGoGameDirect_OnExitGame(DWORD dwDelayMillisecond)
		{
			return g_GameDirectGo.OnGameExit(dwDelayMillisecond);
		}

	}//namespace UTIL

}//namespace NMBASE