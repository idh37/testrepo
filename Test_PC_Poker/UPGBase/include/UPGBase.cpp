#pragma once

#include "stdafx.h"
#include "UPGbase.h"
#include "../src/ClientDlg.h"

CMainFrame *st_pMainFrame = NULL;

CFrameWnd *CreateMainFrame(void)
{
	st_pMainFrame = new CMainFrame;

	if( st_pMainFrame->CheckAllProcessName() == 1 )
	{
		MessageBox( NULL, "허용되지 않은 외부 프로그램 사용으로 게임 접속이 종료 되었습니다", NULL, MB_OK );
		delete st_pMainFrame;
		return NULL;
	}

	if(!st_pMainFrame->LoadFrame(IDD_MAIN_FRAME))
	{
		delete st_pMainFrame;
		st_pMainFrame = NULL;
	}
	return (CFrameWnd *)st_pMainFrame;
}

void DestroyMainFrame(void)
{
	st_pMainFrame->OnDestroy();
	st_pMainFrame = NULL;
}

void MainFrameDraw(void)
{
	if(st_pMainFrame) st_pMainFrame->OnDraw();
}

void ResistryGame(void)
{
	GM().AddGame(new CGameNewPoker);
	GM().AddGame(new CGame7Poker);
	GM().AddGame(new CGame_LowBD);
	GM().AddGame(new CGameHighLow);
	GM().AddGame(new CGameSutda);
	GM().AddGame(new CGameHoola);
	//yoo
	LOG::CConsoleManager::SetConsole();
}

BOOL CheckMutex(LPCSTR lpAppName)
{
	return GM().CheckMutex(lpAppName);
}

BOOL CreateStrManager(void)
{
	char strDir[500]={0,};
	GetCurrentDirectory(500, strDir);

	switch(g_MSCode)
	{
	case 0:  // Korean
		{
			strcat(strDir, "\\common\\Data\\NPoker.str");
			if( !g_StrMan.Open(strDir) )
			{
				AfxMessageBox(_T("Can't find [NPoker.str] file!")); return FALSE;
			}
		} 
		break;
	case 1:  // English
		{
			strcat(strDir, "\\common\\Data\\SPoker_ENG.str");
			if( !g_StrMan.Open(strDir) )
			{
				AfxMessageBox(_T("Can't find [SPoker_ENG.str] file!")); return FALSE;
			}
		} 
		break;
	case 2:  // Japanese
		{
			strcat(strDir, "\\common\\Data\\SPoker_JPN.str");
			if( !g_StrMan.Open(strDir) )
			{
				AfxMessageBox(_T("Can't find [SPoker_JPN.str] file!")); return FALSE;
			}
		}
		break;
	case 3:  // Chinese
		{
			strcat(strDir, "\\common\\Data\\SPoker_CHN.str");
			if( !g_StrMan.Open(strDir) )
			{
				AfxMessageBox(_T("Can't find [SPoker_CHN.str] file!")); return FALSE;
			}
		} 
		break;
	default: // Korean
		{
			strcat(strDir, "\\common\\Data\\NPoker.str");
			if( !g_StrMan.Open(strDir) )
			{
				AfxMessageBox(_T("Can't find [NPoker.str] file!")); return FALSE;
			}
		} 
		break;
	}

	return TRUE;
}

void InitGoGameDirect(void)
{
	GM().InitGoGameDirect();
}

void SetCurPathToRunFilePath(void)
{
	NMBASE::UTIL::NMNMRUNPARAMAPIFUNC_CMDARG::SetCurPathToRunFilePath();
}

bool AnalyzeArgument(void)
{
	NMRUNPARAM_RETCODE retCode;
	
	//
	// 클립보드 인자 전달 규칙(각각의 인자는 컴마(,)로 구분되며 인자 사이의 공백은 없다)
	// 전달 인자의 순서 : ServerIP,Port,AuthCookie,DataCookie,CpCookie,SpareParam (인자는 암호화되어 있다)
	// - 넷마블 게임에서는 CpCookie를 전달하지 않는다.
	// - CP 게임에서는 AuthCookie, DataCookie를 전달하지 않는다(몇몇 CP게임은 예외)
	// - Extra 인자는 생략되기도 한다.
	//
	NMBASE::UTIL::CNMNMRunParam nmRunParam;
	retCode = nmRunParam.Load();

	bool bLoad = true;
	
	if ( retCode != ERROR_NMRUNPARAM_SUCCESS )
		bLoad = false;

	if ( !nmRunParam.ExistsParam(NMPARAMNAME_SERVIP)
		|| !nmRunParam.ExistsParam(NMPARAMNAME_SERVPORT)
		|| !nmRunParam.ExistsParam(NMPARAMNAME_AUTHCOOKIE)
		|| !nmRunParam.ExistsParam(NMPARAMNAME_DATACOOKIE)
		|| !nmRunParam.ExistsParam(NMPARAMNAME_CPCOOKIE) )
	{
		retCode = ERROR_NMRUNPARAM_LIB_MISSING_PARAM;
		bLoad = false;
	}
	
	if(bLoad)
	{
		// 서버 IP
		g_ServIP     = nmRunParam.GetParam(NMPARAMNAME_SERVIP);
		// 서버 Port
		g_ServPort   = (UINT)::atoi(nmRunParam.GetParam(NMPARAMNAME_SERVPORT));
		// 인증 쿠키.
		g_AuthCookie = nmRunParam.GetParam(NMPARAMNAME_AUTHCOOKIE);
		// 데이터 쿠키.
		g_DataCookie = nmRunParam.GetParam(NMPARAMNAME_DATACOOKIE);
		// CP쿠키.
		g_CpCookie   = nmRunParam.GetParam(NMPARAMNAME_CPCOOKIE);
		// 여분의 인자(CP게임등에서 사용)
		g_SpareParam = nmRunParam.GetParam(NMPARAMNAME_SPAREPARAM);                                                          
		
		g_LoginGroup = NMBASE::UTIL::GetGroupNo_BySpareParam(g_SpareParam);

		g_LoginGameCode = NMBASE::UTIL::Get_BySpareParam(g_SpareParam, "gamecode");
	}
	
	// 인자를 분석해서 게임 접속 환경을 초기화.
	if(retCode!=ERROR_NMRUNPARAM_SUCCESS)
	{
		CString strMsg;
		
		
		{
			CString strErrorCode;
			
			strErrorCode = NMBASE::UTIL::NMNMRUNPARAMAPIFUNC::ConvertRetCode2String(retCode);
			
			if ( strErrorCode.IsEmpty() )
				strErrorCode.Format("E%05d", retCode);
			
			strMsg.Format("프로그램에 필요한 환경을 초기화할 수 없습니다.\r\n\r\n코드 : %s", strErrorCode.GetBuffer()); 
		}
		
		AfxMessageBox(strMsg); 
		return false;
	}
	
	if( g_ServIP.IsEmpty() || g_ServIP.GetLength() > 16 )
	{
		AfxMessageBox("프로그램에 필요한 환경이 올바르지 않습니다."); 
		return false;
	}
	
	if( g_AuthCookie.IsEmpty() 
		|| g_DataCookie.IsEmpty() ) 
	{
		AfxMessageBox("인증이 되지 않았습니다. 로그인 하시기 바랍니다."); 
		return false;
	}

	return true;
}