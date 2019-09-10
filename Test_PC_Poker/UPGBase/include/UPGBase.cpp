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
		MessageBox( NULL, "������ ���� �ܺ� ���α׷� ������� ���� ������ ���� �Ǿ����ϴ�", NULL, MB_OK );
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
	// Ŭ������ ���� ���� ��Ģ(������ ���ڴ� �ĸ�(,)�� ���еǸ� ���� ������ ������ ����)
	// ���� ������ ���� : ServerIP,Port,AuthCookie,DataCookie,CpCookie,SpareParam (���ڴ� ��ȣȭ�Ǿ� �ִ�)
	// - �ݸ��� ���ӿ����� CpCookie�� �������� �ʴ´�.
	// - CP ���ӿ����� AuthCookie, DataCookie�� �������� �ʴ´�(��� CP������ ����)
	// - Extra ���ڴ� �����Ǳ⵵ �Ѵ�.
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
		// ���� IP
		g_ServIP     = nmRunParam.GetParam(NMPARAMNAME_SERVIP);
		// ���� Port
		g_ServPort   = (UINT)::atoi(nmRunParam.GetParam(NMPARAMNAME_SERVPORT));
		// ���� ��Ű.
		g_AuthCookie = nmRunParam.GetParam(NMPARAMNAME_AUTHCOOKIE);
		// ������ ��Ű.
		g_DataCookie = nmRunParam.GetParam(NMPARAMNAME_DATACOOKIE);
		// CP��Ű.
		g_CpCookie   = nmRunParam.GetParam(NMPARAMNAME_CPCOOKIE);
		// ������ ����(CP���ӵ�� ���)
		g_SpareParam = nmRunParam.GetParam(NMPARAMNAME_SPAREPARAM);                                                          
		
		g_LoginGroup = NMBASE::UTIL::GetGroupNo_BySpareParam(g_SpareParam);

		g_LoginGameCode = NMBASE::UTIL::Get_BySpareParam(g_SpareParam, "gamecode");
	}
	
	// ���ڸ� �м��ؼ� ���� ���� ȯ���� �ʱ�ȭ.
	if(retCode!=ERROR_NMRUNPARAM_SUCCESS)
	{
		CString strMsg;
		
		
		{
			CString strErrorCode;
			
			strErrorCode = NMBASE::UTIL::NMNMRUNPARAMAPIFUNC::ConvertRetCode2String(retCode);
			
			if ( strErrorCode.IsEmpty() )
				strErrorCode.Format("E%05d", retCode);
			
			strMsg.Format("���α׷��� �ʿ��� ȯ���� �ʱ�ȭ�� �� �����ϴ�.\r\n\r\n�ڵ� : %s", strErrorCode.GetBuffer()); 
		}
		
		AfxMessageBox(strMsg); 
		return false;
	}
	
	if( g_ServIP.IsEmpty() || g_ServIP.GetLength() > 16 )
	{
		AfxMessageBox("���α׷��� �ʿ��� ȯ���� �ùٸ��� �ʽ��ϴ�."); 
		return false;
	}
	
	if( g_AuthCookie.IsEmpty() 
		|| g_DataCookie.IsEmpty() ) 
	{
		AfxMessageBox("������ ���� �ʾҽ��ϴ�. �α��� �Ͻñ� �ٶ��ϴ�."); 
		return false;
	}

	return true;
}