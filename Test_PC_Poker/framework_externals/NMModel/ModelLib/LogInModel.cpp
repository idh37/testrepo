#include "stdafx.h"
#include "LogInModel.h"

namespace NMBASE
{
	namespace MODEL
	{
		// [PC방 작업]
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
		//	///< 버전이 틀리면 연결종료 ( 박종환수정 - 버젼 Define)
		//	if(nVersion != VERSION)
		//	{
		//		//mm().GetMainWnd()->PostMessage(UM_DATA_CLOSE, NULL, (long)SockMan.pDataSock[g_MasterSID]); 
		//		GNET().CloseDataSocket(pNotifyData->szSockName);
		//
		//		// 종환 복구
		//		// 1. 로딩 다이얼로그를 닫는다.
		//		//CloseLoadingDlg();
		//		// 2. 메시지 박스를 호출해준다.
		//		//ShowMainMessageDlg(false, "마스터 서버와의 버전이 일치하지 않아 접속을 종료합니다.");
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
		//	///< 접속할 게임 서버의 IP와 Port
		//	GINFO().SetGameServerIP((LPCTSTR)msg.DI->IP);
		//	GINFO().SetGameServerPort(msg.DI->Port);
		//	
		//	///< 접속할 게임 서버 정보 백업
		//	GINFO().SetGameServerInfo(msg.DI);
		//
		//	// 게임 서버와 접속 시도
		//	BOOL bResult = GNET().ConnectSocket(GAMESOCK, msg.DI->IP, msg.DI->Port);		
		//	
		//	// 로그인 중임을 표시 (종환복구)
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
		//	// 버전이 틀리면 연결종료
		//	if(nVersion != VERSION || nPreVersion != VERSION+5)// VERSIOM 값 변경 가능하다면 다른 방법
		//	{
		//		GNET().CloseDataSocket(pNotifyData->szSockName);
		//		//GM().GetMainWnd()->PostMessage(UM_DATA_CLOSE, NULL, (long)SockMan.pDataSock[g_MainSrvSID]); 		
		//		//CloseLoadingDlg();
		//		//ShowMainMessageDlg(false,"프로그램의 버전이 일치하지 않습니다. 최신 버전을 다운받으세요");
		//
		//		return FALSE;
		//	}		
		//
		//	BOOL bResult = FALSE;
		//
		//	// 처음 로그인 중이면
		////	if(GetCurrentWhere() == IDX_GAMEWHERE_LOGIN)
		//	{
		//		bResult = UTIL::MM().NetCall(NET_GAME_SND_LOGIN);
		//	}
		//// 다른 게임 서버의 게임 이동중인 경우 ( 박종환 복구 )
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
		//	GDMAN().SetString(DMAN_NOTICE_URL,		msg.SI->NoticeURL);				///< 공지 사항 URL
		//	GDMAN().SetString(DMAN_LOBBYBANNER_URL,	msg.SI->BannerURL1);			///< 로비 배너 URL
		//	GDMAN().SetString(DMAN_GAMEBANNER_URL,	msg.SI->BannerURL2);			///< 게임 배너 URL
		//	GDMAN().SetString(DMAN_LOCATION_URL,	msg.SI->LocationURL);			///< 위치알림(친구찾기)용 URL
		//	GDMAN().SetString(DMAN_AVATARITEM_URL,	msg.SI->AvatarItemURL);			///< 아바타아이템 URL
		//	GDMAN().SetString(DMAN_FUNCTIONITEM_URL,msg.SI->FuncItemURL);			///< 기능아이템 URL
		//	GDMAN().SetString(DMAN_MYITEM_URL,		msg.SI->MyItemURL);				///< 나의아이템 URL
		//	GDMAN().SetString(DMAN_BODYGUARD_URL,	msg.SI->BDURL);					///< 경호(???) URL
		//	GDMAN().SetString(DMAN_CASHCHARGE_URL,	msg.SI->CashURL);				///< 충전 URL
		//	
		//	GINFO().SetChannelNotice(msg.SI->SrvInfoMsg);							///< 로비창 공지 메세지
		//	
		//	/// 게임특화된 부분이라 주석 처리한다.(종환복구)	
		//	//// 수호천사
		//	//GDMAN().SetString(GDK_LEADERS_NORMAL_URL, msg.SI->LeadersJoinURL);	//리더스일반 페이지 URL
		//	//GDMAN().SetString(GDK_LEADERS_PLUS_URL, msg.SI->LeadersJoinURL2);		//리더스일반 페이지 URL
		//	//// [ 세븐 포커 ] 잭팟
		//	//// [ ###잭팟### ] 
		//	//GDMAN().SetINT64(GDK_JACKPOT_MONEY, (INT64)(msg.SI->JackPot * 0.9));	//다음 잭팟 신호를 받을 떄까지 올려줌
		//	//GDMAN().SetINT64(GDK_JACKPOT_MONEY_TEMP, msg.SI->JackPot);			//다음 잭팟 신호를 받을 떄까지 올려줌
		//	
		//	// 서버의 시간과 클라이언트의 현재시간을 기록
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
		//	// 접속 메세지창 종료
		//#ifndef _DEBUG
		//	// 넷마블 홈페이지를 미니마이즈 시킨다
		//	UTIL::MM().Call(UPG_CMK_SHOW_EXPLORER, FALSE);
		//#endif	
		//
		//	/// 종환복구
		//	//CPlayer *pPlayer = GM().GetCurrentProcess()->GetMyPlayer();	
		//	//pPlayer->SetNewPlayer(&msg.LI->UI);	
		//	//GM().GetMyInfo()->SetNewPlayer(&msg.LI->UI);	
		//	//g_bMyObserver = FALSE; //관전중인가
		//
		//	// 중복 처리 방지용
		//	static BOOL bLoginOverLap = FALSE;
		//	if(bLoginOverLap)
		//	{
		//		return TRUE;
		//	}	
		//	bLoginOverLap = TRUE;
		//	
		//	GINFO().SetStartTimeTick(timeGetTime());			///< 스피드 핵 체크를 위해서 설정 한다.
		//
		//	GINFO().GetRunParm()->SetLoginID(msg.LI->UI.ID);	///< 로그인 아이디
		//	GINFO().GetRunParm()->SetMyUniqNo(msg.LI->UniqNo);	///< 유니크 넘버 
		//
		//	GINFO().SetEnterGroupNo(msg.LI->GroupNo);			///< 입장한 그룹
		//	GINFO().SetEnterChannelNo(msg.LI->ChanNo);			///< 입장한 채널
		//	GINFO().SetEnterChannelName(msg.LI->ChanName);		///< 입장한 채널 이름
		//
		//
		//	/// 종환복구
		//	//GM().SetCurrentChanNo(msg.LI->ChanNo);				///< 현재 통합채널번호 저장
		//
		//	// (종환 복구)
		//	//// [한도 차감 작업] [2009-02-19]
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
		//	//// ### [ 관전기능 ] ###
		//	//if( GM().GetMyInfo()->UI.Sex == 1)
		//	//{
		//	//	GM().GetMyInfo()->nSndFxKind = 0;//기본음성
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
		//	// 환경설정 파일을 성공적으로 읽었다면 환경설정 적용(종환 복구)
		//	//LoadGameConfig(pPlayer->UI.ID);
		//
		//	// 대기실로 이동(종환 복구)
		//	//SetChangeWhere(IDX_GAMEWHERE_LOBY, false);
		//
		//	// 마스터 서버와 소켓 접속 종료
		//	GNET().CloseDataSocket(MASTERSOCK);
		//	
		//	/// 종환복구
		//	//PlayEnterLobbySound();
		//
		//	// 공지사항, 베너 오픈
		//	//if(g_BannerURL1.GetLength()>0) g_pLobyDlg->m_BannerWnd.OpenURL(g_BannerURL1);
		//	// 위치알림(친구찾기)용 URL 오픈
		//	// ### [ 관전기능 ] ###
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
		//			// 채널명에서 [-숫자]를 뺀다 : 웹 프로필 정보에 현재 채널번호가 올바르게 표기되지 않는 문제 때문
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
		//		UTIL::MM().Call(UPG_CMK_OPEN_HIDDEN_URL, (WPARAM) (const char*)strurl);	// 친구찾기 URL 호출 (히든)
		//	}
		//
		//	// 종환 복구
		//	//바탕화면 아이콘 생성
		//	UTIL::MM().Call(UPG_CMK_CREATEDESKTOPICON);
		//
		//#endif
		//
		//	// 종환 복구
		//	//채팅창 활성/비활성 (300조이상 비활성)
		//	//m_pLobbyProcess->CheckEnableChat();		
		//
		//	bLoginOverLap = FALSE;	
		//
		//	// 서버에 맥어드레스 통보	
		//	UTIL::MM().NetCall(NET_GAME_SND_MACADDRESS);
		//
		//	// 종환 복구
		//	//GM().GetPlayTimeWarning().Login(); // 장시간 게임 플레이 경고
		//
		//	// 로딩창 종료
		//	// MainFrame의 Lobby쪽에 공지를 해준다.(처리할 것이 있겠죠)	
		//	UTIL::MM().Call(MSG_VIEW_ACCEPTLOGIN);	
		//	
		//	// 룸리스트 요청
		//	UTIL::MM().NetCall(NET_GAME_SND_ALLROOMINFO);
		//	
		//	// 유저리스트 요청
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
		//	// 프로그램 종료
		//	//CloseLoadingDlg();
		//
		//	if(nMsgLen > 0) 
		//	{
		//		char* strmsg = new char[nMsgLen+1];
		//		memset(strmsg, 0, nMsgLen+1);				
		//		strncpy_s(strmsg, nMsgLen+1, msg.StrMsg, nMsgLen);
		//
		//		// 종환복구
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
		//	// 종환복구
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
		//	///< 채널 이동 정보를 보낸다 (종화복구)
		//// 	CSV_CONNECT_MOVEGAME cMoveGameMsg;
		//// 	cMoveGameMsg.Set(m_pLobbyProcess->GetMovingGameCode(),  m_pLobbyProcess->GetMoveGameInfo());
		//// 	bResult = GNET().SendData(GAMESOCK, cMoveGameMsg.pData, cMoveGameMsg.GetTotalSize());
		//
		//	// 종환복구
		//	// 다른 게임 서버로 채널 이동표시 플래그 초기화(아직 완전히 인증된 상태는 아님)
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
		//	///< 접속할 게임서버 정보를 마스터 서버에게 요청
		//	CMS_ASK_DESTSERVERINFO cDestInfo;
		//	cDestInfo.Set(GINFO().GetRunParm()->GetLoginGroup());
		//	BOOL bResult = GNET().SendData(MASTERSOCK, cDestInfo.pData, cDestInfo.GetTotalSize());
		//
		//
		//	//// 마스터 서버와 접속 중인 상태면
		//	//if(GetCurrentWhere() == IDX_GAMEWHERE_FIRST)
		//	//{
		//	//	// 접속할 게임서버 정보를 마스터 서버에게 요청
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
		//#define GAME_CODE_SP (42)	// 종환복구 (임시 ......)
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
		//	strcpy_s(sLoginInfo.SockNameIP, sizeof(sLoginInfo.SockNameIP), GNET().GetServerIP(GAMESOCK));		///< 연결된 소켓의 어드레스
		//	strcpy_s(sLoginInfo.HostNameIP, sizeof(sLoginInfo.HostNameIP), GNET().GetHostIP());					///< 호스트 시스템 어드레스
		//
		//	sLoginInfo.Port = GNET().GetServerPort(GAMESOCK);	///< 연결된 소켓의 포트번호
		//	sLoginInfo.Port += 20000;							///< 서버용 소켓을 생성하기 위해 20000정도 더해준 포트번호를 사용한다
		//	///< 실제 사용하게될 포트번호는 메인서버에 의하여 결정된다
		//
		//	CString strAuthCookie = GINFO().GetRunParm()->GetAuthCookie();
		//	CString strDataCookie = GINFO().GetRunParm()->GetDataCookie();
		//
		//	sLoginInfo.AuthCookieLen = strAuthCookie.GetLength();
		//	sLoginInfo.DataCookieLen = strDataCookie.GetLength();
		//
		//	// 마스터 서버로부터 받은 정보들을 세팅한다
		//	sLoginInfo.SeqNo		= GINFO().GetGameServerInfo().SeqNo;
		//	sLoginInfo.GroupNo		= GINFO().GetGameServerInfo().GroupNo;
		//	sLoginInfo.ChanNo		= GINFO().GetGameServerInfo().ChanNo;
		//	sLoginInfo.SNum			= GINFO().GetGameServerInfo().SNum;
		//	sLoginInfo.LocChanNo	= GINFO().GetGameServerInfo().LocChanNo;
		//	sLoginInfo.GameCode		= GAME_CODE_SP;		///< 임시	종환복구			
		//	sLoginInfo.nEtcParam	= Get_BySpareParam(GINFO().GetRunParm()->GetSpareParam(), "pcevent" ); //< CYS 100525 >
		//
		//#ifdef _DEBUG				
		//	sLoginInfo.nEtcParam	= 1;
		//#endif
		//
		//	sLoginInfo.SiteCode		= GetCurSiteNo();	///< 임시 종환복구				
		//
		//
		//	// 로그인 메세지를 보낸다
		//	CSV_ASK_LOGIN cLogInMsg;
		//	cLogInMsg.Set(&sLoginInfo, (char*)(LPCTSTR)strAuthCookie, (char*)(LPCTSTR)strDataCookie);
		//	bResult = GNET().SendData(GAMESOCK, cLogInMsg.pData, cLogInMsg.GetTotalSize());	
		//
		//	return bResult;
		//}


		

	}//namespace MODEL

}//namespace NMBASE
