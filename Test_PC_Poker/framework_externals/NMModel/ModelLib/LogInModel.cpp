#include "stdafx.h"
#include "LogInModel.h"

namespace NMBASE
{
	namespace MODEL
	{
		// [PC�� �۾�]
		void URLEncode(char* output, char* input)
		{
			int opt_inx=0, ipt_inx=0;

			if(strlen(input) > 7) 
			{
				if(_strnicmp(input, "http://", 7) == 0) 
				{
					ipt_inx = 7;
					opt_inx = 7;
					strncpy(output, input, 7);
				}
			}

			for( ; input[ipt_inx]; ++ipt_inx, ++opt_inx)
			{
				int char_val = input[ipt_inx]; 
				if( char_val < 0 ) 
				{
					char_val +=256; 
				}


				if(
					char_val <= 0x1F|| 
					char_val == 0x7F||
					char_val >= 0x80||
					char_val == ' ' ||
					char_val == '{' ||
					char_val == '}' ||
					char_val == '[' ||
					char_val == ']' ||
					char_val == '|' ||
					char_val == '\\'||
					char_val == '^' ||
					char_val == '~' ||
					char_val == '`' ||
					char_val == '#' ||
					char_val == ';' ||
					char_val == '/' ||
					char_val == '?' ||
					char_val == '@' ||
					char_val == '=' ||
					char_val == '&')
				{
					output[opt_inx] = '%';

					int UpperBit = char_val / 0x10;

					if(UpperBit >=0 && UpperBit <=9 )
					{
						output[++opt_inx] = UpperBit+'0';
					}
					else 
					{
						output[++opt_inx] = UpperBit+'A'-10;
					}

					int LowerBit = char_val % 0x10;

					if(LowerBit >= 0 && LowerBit <= 9 )
					{
						output[++opt_inx] = LowerBit+'0';
					}
					else 
					{
						output[++opt_inx] = LowerBit+'A'-10;
					}
				}
				else
				{
					output[opt_inx] = char_val;    
				}
			}

			output[opt_inx] = 0;
		}

		CLogInModel::CLogInModel()
		{
		}

		CLogInModel::~CLogInModel()
		{
		}

		BOOL CLogInModel::OnAttached()
		{
			/*UTIL::MM().AddNetMsgH(NET_MASTER_SND_DESTSERVERINFO,	Fnt(this, &CLogInModel::SendDestServerInfoRequest));
			UTIL::MM().AddNetMsgH(NET_GAME_SND_MACADDRESS,		Fnt(this, &CLogInModel::SendMacAddress));	
			UTIL::MM().AddNetMsgH(NET_GAME_SND_LOGIN,				Fnt(this, &CLogInModel::SendLogIn));	

			UTIL::MM().AddNetMsgH(MSG_MASTER_CHECKVERSION,		Fnt(this, &CLogInModel::MasterServerCheckVersion));
			UTIL::MM().AddNetMsgH(MSG_MASTER_DESTSERVERINFO,		Fnt(this, &CLogInModel::DestinationServerInfo));	

			UTIL::MM().AddNetMsgH(MSG_GAME_CHECKVERSION,			Fnt(this, &CLogInModel::GameServerCheckVersion));	
			UTIL::MM().AddNetMsgH(MSG_GAME_SERVERINFO,			Fnt(this, &CLogInModel::GameServerServerInfo));	
			UTIL::MM().AddNetMsgH(MSG_GAME_ACCEPTLOGIN,			Fnt(this, &CLogInModel::GameServerAcceptLogin));*/

			return TRUE;
		}

		BOOL CLogInModel::OnDetached()
		{
			///< Delete
			delete this;

			return TRUE;
		}

		//
		//LRESULT CLogInModel::MasterServerCheckVersion(WPARAM& wParam, LPARAM& lParam)
		//{
		//	NMSOCK::SGAMENOTIFY *pNotifyData = (NMSOCK::SGAMENOTIFY*)lParam;
		//
		//	CMS_CHECKVERSION msg;
		//	msg.Get(pNotifyData->lpData, pNotifyData->TotSize);
		//
		//	int nVersion = *msg.Ver;
		//
		//	///< ������ Ʋ���� �������� ( ����ȯ���� - ���� Define)
		//	if(nVersion != VERSION)
		//	{
		//		//mm().GetMainWnd()->PostMessage(UM_DATA_CLOSE, NULL, (long)SockMan.pDataSock[g_MasterSID]); 
		//		GNET().CloseDataSocket(pNotifyData->szSockName);
		//
		//		// ��ȯ ����
		//		// 1. �ε� ���̾�α׸� �ݴ´�.
		//		//CloseLoadingDlg();
		//		// 2. �޽��� �ڽ��� ȣ�����ش�.
		//		//ShowMainMessageDlg(false, "������ �������� ������ ��ġ���� �ʾ� ������ �����մϴ�.");
		//
		//		return FALSE;
		//	}
		//
		//	return UTIL::MM().NetCall(NET_MASTER_SND_DESTSERVERINFO);
		//}
		//
		//
		//LRESULT CLogInModel::DestinationServerInfo(WPARAM& wParam, LPARAM& lParam)
		//{
		//	NMSOCK::SGAMENOTIFY *pNotifyData = (NMSOCK::SGAMENOTIFY*)lParam;
		//
		//	CMS_DESTSERVERINFO msg;
		//	msg.Get(pNotifyData->lpData, pNotifyData->TotSize);
		//
		//	///< ������ ���� ������ IP�� Port
		//	GINFO().SetGameServerIP((LPCTSTR)msg.DI->IP);
		//	GINFO().SetGameServerPort(msg.DI->Port);
		//	
		//	///< ������ ���� ���� ���� ���
		//	GINFO().SetGameServerInfo(msg.DI);
		//
		//	// ���� ������ ���� �õ�
		//	BOOL bResult = GNET().ConnectSocket(GAMESOCK, msg.DI->IP, msg.DI->Port);		
		//	
		//	// �α��� ������ ǥ�� (��ȯ����)
		//	//SetChangeWhere(IDX_GAMEWHERE_LOGIN, false);
		//
		//	return bResult;
		//}
		//
		//
		//LRESULT CLogInModel::GameServerCheckVersion(WPARAM& wParam, LPARAM& lParam) 
		//{
		//	NMSOCK::SGAMENOTIFY *pNotifyData = (NMSOCK::SGAMENOTIFY*)lParam;
		//
		//	CSV_CHECKVERSION msg;
		//	msg.Get(pNotifyData->lpData, pNotifyData->TotSize);
		//	
		//	int nPreVersion = *msg.PreVersion;
		//	int nVersion    = *msg.Ver;	
		//
		//	// ������ Ʋ���� ��������
		//	if(nVersion != VERSION || nPreVersion != VERSION+5)// VERSIOM �� ���� �����ϴٸ� �ٸ� ���
		//	{
		//		GNET().CloseDataSocket(pNotifyData->szSockName);
		//		//GM().GetMainWnd()->PostMessage(UM_DATA_CLOSE, NULL, (long)SockMan.pDataSock[g_MainSrvSID]); 		
		//		//CloseLoadingDlg();
		//		//ShowMainMessageDlg(false,"���α׷��� ������ ��ġ���� �ʽ��ϴ�. �ֽ� ������ �ٿ��������");
		//
		//		return FALSE;
		//	}		
		//
		//	BOOL bResult = FALSE;
		//
		//	// ó�� �α��� ���̸�
		////	if(GetCurrentWhere() == IDX_GAMEWHERE_LOGIN)
		//	{
		//		bResult = UTIL::MM().NetCall(NET_GAME_SND_LOGIN);
		//	}
		//// �ٸ� ���� ������ ���� �̵����� ��� ( ����ȯ ���� )
		//// 	else if(m_pLobbyProcess->IsMovingGame())	
		//// 	{
		////		bResult = SetChangeGame();
		//// 	}
		//
		//	return bResult;
		//}
		//
		//
		//LRESULT CLogInModel::GameServerServerInfo(WPARAM& wParam, LPARAM& lParam)
		//{
		//	NMSOCK::SGAMENOTIFY *pNotifyData = (NMSOCK::SGAMENOTIFY*)lParam;
		//
		//	CSV_SERVERINFO msg;
		//	msg.Get(pNotifyData->lpData, pNotifyData->TotSize);
		//	
		//
		//	GDMAN().SetINT64(DMAN_GAMECODE,			msg.SI->GameCode);				///< GameCode				
		//	GDMAN().SetString(DMAN_NOTICE_URL,		msg.SI->NoticeURL);				///< ���� ���� URL
		//	GDMAN().SetString(DMAN_LOBBYBANNER_URL,	msg.SI->BannerURL1);			///< �κ� ��� URL
		//	GDMAN().SetString(DMAN_GAMEBANNER_URL,	msg.SI->BannerURL2);			///< ���� ��� URL
		//	GDMAN().SetString(DMAN_LOCATION_URL,	msg.SI->LocationURL);			///< ��ġ�˸�(ģ��ã��)�� URL
		//	GDMAN().SetString(DMAN_AVATARITEM_URL,	msg.SI->AvatarItemURL);			///< �ƹ�Ÿ������ URL
		//	GDMAN().SetString(DMAN_FUNCTIONITEM_URL,msg.SI->FuncItemURL);			///< ��ɾ����� URL
		//	GDMAN().SetString(DMAN_MYITEM_URL,		msg.SI->MyItemURL);				///< ���Ǿ����� URL
		//	GDMAN().SetString(DMAN_BODYGUARD_URL,	msg.SI->BDURL);					///< ��ȣ(???) URL
		//	GDMAN().SetString(DMAN_CASHCHARGE_URL,	msg.SI->CashURL);				///< ���� URL
		//	
		//	GINFO().SetChannelNotice(msg.SI->SrvInfoMsg);							///< �κ�â ���� �޼���
		//	
		//	/// ����Ưȭ�� �κ��̶� �ּ� ó���Ѵ�.(��ȯ����)	
		//	//// ��ȣõ��
		//	//GDMAN().SetString(GDK_LEADERS_NORMAL_URL, msg.SI->LeadersJoinURL);	//�������Ϲ� ������ URL
		//	//GDMAN().SetString(GDK_LEADERS_PLUS_URL, msg.SI->LeadersJoinURL2);		//�������Ϲ� ������ URL
		//	//// [ ���� ��Ŀ ] ����
		//	//// [ ###����### ] 
		//	//GDMAN().SetINT64(GDK_JACKPOT_MONEY, (INT64)(msg.SI->JackPot * 0.9));	//���� ���� ��ȣ�� ���� ������ �÷���
		//	//GDMAN().SetINT64(GDK_JACKPOT_MONEY_TEMP, msg.SI->JackPot);			//���� ���� ��ȣ�� ���� ������ �÷���
		//	
		//	// ������ �ð��� Ŭ���̾�Ʈ�� ����ð��� ���
		//	COleDateTime serverTime(msg.SI->ServerTime);			
		//	COleDateTimeSpan cTimeSpan = COleDateTime::GetCurrentTime() - serverTime - COleDateTimeSpan(0,0,0,1);
		//	GINFO().SetServerTimeGap(cTimeSpan);
		//
		//	return TRUE;
		//}
		//
		//LRESULT CLogInModel::GameServerAcceptLogin(WPARAM& wParam, LPARAM& lParam)
		//{
		//	NMSOCK::SGAMENOTIFY *pNotifyData = (NMSOCK::SGAMENOTIFY*)lParam;
		//
		//	CSV_ACCEPT_LOGIN msg;
		//	msg.Get(pNotifyData->lpData, pNotifyData->TotSize);
		//
		//	
		//	// ���� �޼���â ����
		//#ifndef _DEBUG
		//	// �ݸ��� Ȩ�������� �̴ϸ����� ��Ų��
		//	UTIL::MM().Call(UPG_CMK_SHOW_EXPLORER, FALSE);
		//#endif	
		//
		//	/// ��ȯ����
		//	//CPlayer *pPlayer = GM().GetCurrentProcess()->GetMyPlayer();	
		//	//pPlayer->SetNewPlayer(&msg.LI->UI);	
		//	//GM().GetMyInfo()->SetNewPlayer(&msg.LI->UI);	
		//	//g_bMyObserver = FALSE; //�������ΰ�
		//
		//	// �ߺ� ó�� ������
		//	static BOOL bLoginOverLap = FALSE;
		//	if(bLoginOverLap)
		//	{
		//		return TRUE;
		//	}	
		//	bLoginOverLap = TRUE;
		//	
		//	GINFO().SetStartTimeTick(timeGetTime());			///< ���ǵ� �� üũ�� ���ؼ� ���� �Ѵ�.
		//
		//	GINFO().GetRunParm()->SetLoginID(msg.LI->UI.ID);	///< �α��� ���̵�
		//	GINFO().GetRunParm()->SetMyUniqNo(msg.LI->UniqNo);	///< ����ũ �ѹ� 
		//
		//	GINFO().SetEnterGroupNo(msg.LI->GroupNo);			///< ������ �׷�
		//	GINFO().SetEnterChannelNo(msg.LI->ChanNo);			///< ������ ä��
		//	GINFO().SetEnterChannelName(msg.LI->ChanName);		///< ������ ä�� �̸�
		//
		//
		//	/// ��ȯ����
		//	//GM().SetCurrentChanNo(msg.LI->ChanNo);				///< ���� ����ä�ι�ȣ ����
		//
		//	// (��ȯ ����)
		//	//// [�ѵ� ���� �۾�] [2009-02-19]
		//	//g_MaxServerMoney = msg.LI->MaxServerMoney;			
		//	//if( g_MaxServerMoney < 0 )
		//	//{
		//	//	g_MaxServerMoney = 0;
		//	//}
		//	//
		//	//for( int i=0; i<MAX_CODE_NEW_FAMILY; i++ )
		//	//{
		//	//	g_DisplayMaxMoney[i] = msg.LI->DisplayMaxMoney[i];
		//	//}
		//
		//	//UTIL::MM().Call(UPG_CMK_SET_LIMIT_MONEY, 0, (LPARAM)(&msg.LI->llMaxPremiumMoneyWithOption));			
		//
		//	//// ### [ ������� ] ###
		//	//if( GM().GetMyInfo()->UI.Sex == 1)
		//	//{
		//	//	GM().GetMyInfo()->nSndFxKind = 0;//�⺻����
		//	//	pPlayer->nSndFxKind  = 0;
		//	//	g_Config.SndFxKind = 0;
		//	//}
		//	//else 
		//	//{
		//	//	GM().GetMyInfo()->nSndFxKind = 4;
		//	//	pPlayer->nSndFxKind  = 4;
		//	//	g_Config.SndFxKind = 4;
		//	//}
		//
		//	// ȯ�漳�� ������ ���������� �о��ٸ� ȯ�漳�� ����(��ȯ ����)
		//	//LoadGameConfig(pPlayer->UI.ID);
		//
		//	// ���Ƿ� �̵�(��ȯ ����)
		//	//SetChangeWhere(IDX_GAMEWHERE_LOBY, false);
		//
		//	// ������ ������ ���� ���� ����
		//	GNET().CloseDataSocket(MASTERSOCK);
		//	
		//	/// ��ȯ����
		//	//PlayEnterLobbySound();
		//
		//	// ��������, ���� ����
		//	//if(g_BannerURL1.GetLength()>0) g_pLobyDlg->m_BannerWnd.OpenURL(g_BannerURL1);
		//	// ��ġ�˸�(ģ��ã��)�� URL ����
		//	// ### [ ������� ] ###
		//
		//#ifndef _DEBUG
		//	CString strLocationURL = "http://game1.netmarble.net/Client/Log/UserLocation.asp";
		//	
		//	GDMAN().SetString(DMAN_LOCATION_URL, strLocationURL.GetString());
		//
		//	if(strLocationURL.GetLength() > 0) 
		//	{
		//		CString strChanName = GINFO().GetEnterChannelName();
		//		if(strChanName.GetLength() > 2) 
		//		{
		//			// ä�θ��� [-����]�� ���� : �� ������ ������ ���� ä�ι�ȣ�� �ùٸ��� ǥ����� �ʴ� ���� ����
		//			int npos = strChanName.Find('-');
		//			if(npos > 0)
		//			{
		//				strChanName = strChanName.Mid(0, npos);
		//			}
		//		}
		//
		//		CString strurl;
		//		char channame[1024] = {0, };
		//		URLEncode(channame, (char*)strChanName.operator LPCTSTR());
		//		strurl.Format("%s?user_id=%s&server=%s&code=%d&groupcode=%d&SiteCode=%d&user_cn=%s",
		//						strLocationURL,				
		//						GINFO().GetRunParm()->GetLoginID(), 
		//						channame, 
		//						GDMAN().GetINT64(DMAN_GAMECODE), 
		//						GINFO().GetEnterGroupNo(),
		//						GetCurSiteNo(),
		//						GINFO().GetRunParm()->GetMyUniqNo());
		//
		//
		//		UTIL::MM().Call(UPG_CMK_OPEN_HIDDEN_URL, (WPARAM) (const char*)strurl);	// ģ��ã�� URL ȣ�� (����)
		//	}
		//
		//	// ��ȯ ����
		//	//����ȭ�� ������ ����
		//	UTIL::MM().Call(UPG_CMK_CREATEDESKTOPICON);
		//
		//#endif
		//
		//	// ��ȯ ����
		//	//ä��â Ȱ��/��Ȱ�� (300���̻� ��Ȱ��)
		//	//m_pLobbyProcess->CheckEnableChat();		
		//
		//	bLoginOverLap = FALSE;	
		//
		//	// ������ �ƾ�巹�� �뺸	
		//	UTIL::MM().NetCall(NET_GAME_SND_MACADDRESS);
		//
		//	// ��ȯ ����
		//	//GM().GetPlayTimeWarning().Login(); // ��ð� ���� �÷��� ���
		//
		//	// �ε�â ����
		//	// MainFrame�� Lobby�ʿ� ������ ���ش�.(ó���� ���� �ְ���)	
		//	UTIL::MM().Call(MSG_VIEW_ACCEPTLOGIN);	
		//	
		//	// �븮��Ʈ ��û
		//	UTIL::MM().NetCall(NET_GAME_SND_ALLROOMINFO);
		//	
		//	// ��������Ʈ ��û
		//	UTIL::MM().NetCall(NET_GAME_SND_ALLUSERINFO);	
		//
		//	return TRUE;
		//}
		//
		//LRESULT CLogInModel::GameServerRefuseLogin(WPARAM& wParam, LPARAM& lParam)
		//{
		//	NMSOCK::SGAMENOTIFY *pNotifyData = (NMSOCK::SGAMENOTIFY*)lParam;
		//
		//	CSV_REFUSE_LOGIN msg;
		//	msg.Get(pNotifyData->lpData, pNotifyData->TotSize);
		//	
		//	int nCode   = *msg.Code;	
		//	int nMsgLen = *msg.l_StrMsg; 
		//
		//	// ���α׷� ����
		//	//CloseLoadingDlg();
		//
		//	if(nMsgLen > 0) 
		//	{
		//		char* strmsg = new char[nMsgLen+1];
		//		memset(strmsg, 0, nMsgLen+1);				
		//		strncpy_s(strmsg, nMsgLen+1, msg.StrMsg, nMsgLen);
		//
		//		// ��ȯ����
		//		//ShowMainMessageDlg(false, strmsg);
		//
		//		if(nCode == 3)
		//		{
		//			CShellMan ShellMan;					
		//			ShellMan.ShowRefresh("http://game1.netmarble.net/newpoker/"); 
		//		}
		//
		//		delete []strmsg;
		//	}
		//
		//	// ��ȯ����
		//	//m_pLobbyView->EnableWindow(TRUE);			
		//	//ExitGameClient(false);
		//
		//	return TRUE;
		//}
		//
		//
		//BOOL CLogInModel::SetChangeGame()
		//{
		//	BOOL bResult = FALSE;
		//
		//	///< ä�� �̵� ������ ������ (��ȭ����)
		//// 	CSV_CONNECT_MOVEGAME cMoveGameMsg;
		//// 	cMoveGameMsg.Set(m_pLobbyProcess->GetMovingGameCode(),  m_pLobbyProcess->GetMoveGameInfo());
		//// 	bResult = GNET().SendData(GAMESOCK, cMoveGameMsg.pData, cMoveGameMsg.GetTotalSize());
		//
		//	// ��ȯ����
		//	// �ٸ� ���� ������ ä�� �̵�ǥ�� �÷��� �ʱ�ȭ(���� ������ ������ ���´� �ƴ�)
		//// 	m_pLobbyProcess->SetMovingGame(false, 0);
		//// 	GM().SetWaitResponse(false);
		//
		//	return bResult;
		//}
		//
		//
		//
		//LRESULT CLogInModel::SendDestServerInfoRequest(WPARAM& wParam, LPARAM& lParam)
		//{
		//	///< ������ ���Ӽ��� ������ ������ �������� ��û
		//	CMS_ASK_DESTSERVERINFO cDestInfo;
		//	cDestInfo.Set(GINFO().GetRunParm()->GetLoginGroup());
		//	BOOL bResult = GNET().SendData(MASTERSOCK, cDestInfo.pData, cDestInfo.GetTotalSize());
		//
		//
		//	//// ������ ������ ���� ���� ���¸�
		//	//if(GetCurrentWhere() == IDX_GAMEWHERE_FIRST)
		//	//{
		//	//	// ������ ���Ӽ��� ������ ������ �������� ��û
		//	//	CMS_ASK_DESTSERVERINFO dsmsg;
		//	//	dsmsg.Set(g_LoginGroup);
		//	//	SockMan.SendData(g_MasterSID, dsmsg.pData, dsmsg.GetTotalSize());
		//	//}
		//
		//	return bResult; 
		//}
		//
		//
		//LRESULT CLogInModel::SendMacAddress(WPARAM& wParam, LPARAM& lParam)
		//{
		//	CCL_NOTIFY_MACADDRESS MacMsg;
		//	CString strMac = GetMACAddress();
		//	MacMsg.Set(strMac.GetBuffer());
		//	BOOL bResult = GNET().SendData(GAMESOCK, MacMsg.pData, MacMsg.GetTotalSize());
		//
		//	return bResult;
		//}
		//
		//
		//#define GAME_CODE_SP (42)	// ��ȯ���� (�ӽ� ......)
		//LRESULT CLogInModel::SendLogIn(WPARAM& wParam, LPARAM& lParam)
		//{
		//	BOOL bResult = FALSE;
		//
		//	ASK_LOGININFO sLoginInfo;
		//	memset(&sLoginInfo, 0, sizeof(ASK_LOGININFO));
		//
		//
		//#ifdef _DEBUG
		//	strcpy_s(sLoginInfo.ID,   sizeof(sLoginInfo.ID),   GINFO().GetRunParm()->GetLoginID());
		//	strcpy_s(sLoginInfo.Pass, sizeof(sLoginInfo.Pass), GINFO().GetRunParm()->GetLoginPass());
		//#endif
		//	strcpy_s(sLoginInfo.SockNameIP, sizeof(sLoginInfo.SockNameIP), GNET().GetServerIP(GAMESOCK));		///< ����� ������ ��巹��
		//	strcpy_s(sLoginInfo.HostNameIP, sizeof(sLoginInfo.HostNameIP), GNET().GetHostIP());					///< ȣ��Ʈ �ý��� ��巹��
		//
		//	sLoginInfo.Port = GNET().GetServerPort(GAMESOCK);	///< ����� ������ ��Ʈ��ȣ
		//	sLoginInfo.Port += 20000;							///< ������ ������ �����ϱ� ���� 20000���� ������ ��Ʈ��ȣ�� ����Ѵ�
		//	///< ���� ����ϰԵ� ��Ʈ��ȣ�� ���μ����� ���Ͽ� �����ȴ�
		//
		//	CString strAuthCookie = GINFO().GetRunParm()->GetAuthCookie();
		//	CString strDataCookie = GINFO().GetRunParm()->GetDataCookie();
		//
		//	sLoginInfo.AuthCookieLen = strAuthCookie.GetLength();
		//	sLoginInfo.DataCookieLen = strDataCookie.GetLength();
		//
		//	// ������ �����κ��� ���� �������� �����Ѵ�
		//	sLoginInfo.SeqNo		= GINFO().GetGameServerInfo().SeqNo;
		//	sLoginInfo.GroupNo		= GINFO().GetGameServerInfo().GroupNo;
		//	sLoginInfo.ChanNo		= GINFO().GetGameServerInfo().ChanNo;
		//	sLoginInfo.SNum			= GINFO().GetGameServerInfo().SNum;
		//	sLoginInfo.LocChanNo	= GINFO().GetGameServerInfo().LocChanNo;
		//	sLoginInfo.GameCode		= GAME_CODE_SP;		///< �ӽ�	��ȯ����			
		//	sLoginInfo.nEtcParam	= Get_BySpareParam(GINFO().GetRunParm()->GetSpareParam(), "pcevent" ); //< CYS 100525 >
		//
		//#ifdef _DEBUG				
		//	sLoginInfo.nEtcParam	= 1;
		//#endif
		//
		//	sLoginInfo.SiteCode		= GetCurSiteNo();	///< �ӽ� ��ȯ����				
		//
		//
		//	// �α��� �޼����� ������
		//	CSV_ASK_LOGIN cLogInMsg;
		//	cLogInMsg.Set(&sLoginInfo, (char*)(LPCTSTR)strAuthCookie, (char*)(LPCTSTR)strDataCookie);
		//	bResult = GNET().SendData(GAMESOCK, cLogInMsg.pData, cLogInMsg.GetTotalSize());	
		//
		//	return bResult;
		//}


		

	}//namespace MODEL

}//namespace NMBASE
