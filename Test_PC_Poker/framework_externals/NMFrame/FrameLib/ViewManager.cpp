#include "StdAfx.h"
#include "ViewManager.h"
#include "PlayTimeWarning.h"
#include "NMGameView.h"
//#include "CommMsgDef_Game.h"

namespace NMBASE
{
	namespace FRAME
	{

		// CViewManager& GGMAN()
		// {
		// 	return CViewManager::Get();
		// }
		// 
		// CViewManager& CViewManager::Get()
		// {
		// 	static CViewManager sGameManager;
		// 	return sGameManager;
		// }



		CViewManager::CViewManager(void)
			//:	m_pCurGame(NULL),
			//	m_nCurGameType(IDX_GAME_NULL),
			//	m_nMaxRoom(0),
			//	m_nMaxPlayer(0),
			//	m_nMaxGamePlayer(0),
			//	m_nMaxObserver(0),
			//	m_nMaxNewPlayer(0),
			//	m_nJokboQuestCount(0),
			//	m_nWhere(IDX_GAMEWHERE_FIRST),
			//	m_bAbnormalOutRoom(false),
			//	m_bWaitResponse(false),
			//	m_hMutex(NULL),
			//	m_bNowLogout(false),
			//	m_pMainWnd(NULL),
			//	m_pMyInfo(NULL),
			//	m_nLoadingType(EGLT_FIRST),	
			//	m_nGoDirectEnterType(CHOICE_GAME),
			//	m_nRoomNum_for_Join(0),
			//	m_llRoomKey_for_Join(0),
			//	m_bObserve_for_Join(false),
			//	m_nCurChanNo(-1),
			//	m_bUseRoomOption(false)
		{
			//#ifdef _DEBUG
			//	HINSTANCE hInstance = GetModuleHandle("UPGCommon_D.dll");
			//#else
			//	HINSTANCE hInstance = GetModuleHandle("UPGCommon.dll");
			//#endif
			//	m_hCurSel  = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_SELECT));
			//	m_hCurHand = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_HAND));
			//	m_hCurMove = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_MOVE));
			//	m_hCurSize = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_ADJUSTSIZE));
			//	m_hCurWait = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_WAIT));
			//
			//	ZeroMemory(&m_RoomOption, sizeof(m_RoomOption));


			m_pPlayTimeWarning = new UTIL::CPlayTimeWarning();
		}

		CViewManager::~CViewManager(void)
		{
			if(m_pPlayTimeWarning)
			{
				delete m_pPlayTimeWarning;
			}

			Release();

			// 	if(m_pMyPlayerInfo) 
			// 	{
			// 		delete m_pMyPlayerInfo;
			// 		m_pMyPlayerInfo = NULL;
			// 	}
		}

		void CViewManager::Release()
		{
			GAMEMAP_IT itr = m_mapGamelist.begin();

			for(; itr != m_mapGamelist.end(); ++itr )
			{
				if(itr->second)
				{
					delete itr->second;
				}
				itr->second = NULL;
			}

			m_mapGamelist.clear();	
		}


		CNMViewBase *CViewManager::GetView(string strGameName)
		{
			if(strGameName.empty())
			{
				ASSERT(0);
				return NULL;
			}

			GAMEMAP_IT findIt = m_mapGamelist.find(strGameName);
			ASSERT(findIt != m_mapGamelist.end());
			return findIt->second;
		}


		void CViewManager::OnDraw()
		{
			GAMEMAP_IT it;
			for(it = m_mapGamelist.begin(); it != m_mapGamelist.end(); ++it)
			{
				it->second->OnDraw();
			}
		}

		void CViewManager::OnUpdate()
		{
			GAMEMAP_IT it;
			for(it = m_mapGamelist.begin(); it != m_mapGamelist.end(); ++it)
			{
				it->second->OnUpdate();
			}
		}



		void CViewManager::SetCurrentGame(LPCTSTR pGameName)
		{
		}


		void CViewManager::SetMyPlayerInfo(CPlayerDataBase *pPlayer)
		{
			/// 종환복구
			// 	ASSERT(pPlayer != NULL);
			// 	
			// 	if(!pPlayer)
			// 	{
			// 		return;
			// 	}
			// 
			// 	bool bSetUserInfo = false;	
			// 	USERINFO sUserInfo;
			// 
			// 	if(m_pMyPlayerInfo)
			// 	{
			// 		bSetUserInfo = true;
			// 		memcpy(&sUserInfo, &m_pMyPlayerInfo->m_sUserInfo, sizeof(USERINFO));
			// 		delete m_pMyPlayerInfo;
			// 		m_pMyPlayerInfo = NULL;
			// 	}
			// 
			// 	m_pMyPlayerInfo = pPlayer;
			// 	
			// 	if(bSetUserInfo)
			// 	{
			// 		m_pMyPlayerInfo->SetNewPlayer(&sUserInfo);
			// 
			// 		// 이곳에서 플레이어 정보를 세팅해 준다. (종환복구)
			// 		//CPlayerDataBase *pPlayerTemp = m_pCurrentGame->GetGameProcess()->GetMyPlayer();
			// 		//pPlayerTemp->SetNewPlayer(&sUserInfo);
			// 	}
		}

		void CViewManager::BadUserReport(void)
		{	
			//	CBadUserDlg Dlg(GM().GetMainWnd());
			//	m_HideChat.MAKE_IDList( (void *)&Dlg );	
			//	Dlg.DoModal();
			//	
			//	//신고취소
			//	if(Dlg.m_Kind == REPORT_KIND_NULL )
			//	{
			//		return;
			//	}
			//
			//	//신고할 내용이 너무 짧다
			//	CString strwatchid, strchat;
			//	strchat = m_HideChat.GetData();
			//
			//	if( Dlg.m_Kind != REPORT_KIND_SCAM && ( strchat.GetLength()<30) )	//불량행위신고는검사안한다.
			//	{
			//		ShowMainMessageDlg(false, "신고할 내용이 너무 짧습니다.");	
			//		return;
			//	}
			//
			//	// 4000자 이상은 잘라냄
			//	if(strchat.GetLength()>4000) strchat = strchat.Mid(strchat.GetLength() - 4000);
			//
			//
			//	//신고의 종류에 따라서 다르게 처리한다
			//	CPlayer *pMyInfo = GetMyInfo();
			//	if( Dlg.m_Kind == REPORT_KIND_MONEYDEAL )	//머니신고 
			//	{
			//// 		CBadUser_MoneyDlg MoneyDlg;
			//// 		MoneyDlg.DoModal();
			//	
			//		// 신고기능 이용회수 카운터 증가
			//	//	Dlg.SubmitCnt++;
			//		// 신고기능 딜레이 타이머 세팅
			//		ResetBadUserTimer();
			//
			//		CSV_ASK_BADUSER abumsg;
			//		abumsg.Set( 0, Dlg.m_Kind, g_RI.RoomNum, pMyInfo->UI.UNum, pMyInfo->UI.ID,
			//			"",	"",	"", "",(char*)strchat.operator LPCTSTR());
			//		SockMan.SendData(g_MainSrvSID, abumsg.pData, abumsg.GetTotalSize());
			//
			//		// 신고기능 이용회수 카운터 증가
			//		Dlg.m_SubmitCnt++;
			//		// 신고기능 딜레이 타이머 세팅
			//		ResetBadUserTimer();
			//
			//	//	UTIL::MM().Call(UPG_CMK_BADUSERCOMPLETE);
			//
			//	}
			//	else if( Dlg.m_Kind == REPORT_KIND_SWEAR )	//욕설신고
			//	{
			//// 		CBadUserDlg BadUserDlg;
			//// 		
			//// 		if( BadUserDlg.DoModal() == IDOK )
			//// 		{
			//			CSV_ASK_BADUSER abumsg;
			//			abumsg.Set(Dlg.m_bAnnounce, Dlg.m_Kind, g_RI.RoomNum, pMyInfo->UI.UNum, pMyInfo->UI.ID,
			//				(char*)Dlg.m_strTargetID.operator LPCTSTR(),
			//				(char*)Dlg.m_TargetNickName.operator LPCTSTR(),
			//				(char*)Dlg.m_SubmitReason.operator LPCTSTR(),
			//				(char*)strwatchid.operator LPCTSTR(), (char*)strchat.operator LPCTSTR());
			//			SockMan.SendData(g_MainSrvSID, abumsg.pData, abumsg.GetTotalSize());
			//
			//			// 신고기능 이용회수 카운터 증가
			//			Dlg.m_SubmitCnt++;
			//			// 신고기능 딜레이 타이머 세팅
			//			ResetBadUserTimer();
			////		}
			//		UTIL::MM().Call(UPG_CMK_BADUSERCOMPLETE);
			//	}
			//	else if( Dlg.m_Kind == REPORT_KIND_SCAM ) //112불량행위 짜고치기
			//	{
			//// 		CBaduser_ConspireDlg ConspireUserDlg;
			//// 		
			//// 		if( ConspireUserDlg.DoModal() == IDOK )
			//// 		{
			//			
			//			CSV_ASK_BADUSER abumsg;
			//			abumsg.Set( 0, Dlg.m_Kind, g_RI.RoomNum, pMyInfo->UI.UNum, pMyInfo->UI.ID, "",
			//				"",	"",	"", (char*)strchat.operator LPCTSTR());
			//			SockMan.SendData(g_MainSrvSID, abumsg.pData, abumsg.GetTotalSize());
			//			
			//			// 신고기능 이용회수 카운터 증가
			//			Dlg.m_SubmitCnt++;
			//			// 신고기능 딜레이 타이머 세팅
			//			ResetBadUserTimer();
			//			
			//// 			CBadUserComplete SeizureDlg(GM().GetMainWnd());
			//// 			SeizureDlg.DoModal(FALSE);			
			////		}
			//
			//		
			//		UTIL::MM().Call(UPG_CMK_BADUSERCOMPLETE);
			//	}
		}


		// void CViewManager::AddGame(CNMGameView *pGame)
		// {
		// 	ASSERT(pGame != NULL);
		// 	if(!pGame)
		// 		return;
		// 
		// 	IDX_GAME nGame = pGame->GetGameType();
		// 	ASSERT(m_listGame.find(nGame) == m_listGame.end());
		// 	m_listGame[nGame] = pGame;
		// 	pGame->Create((CWnd *)GM().GetMainWnd());
		// 	pGame->GetGameView()->ShowWindow(SW_HIDE);
		// }


		//bool CViewManager::CheckLoadGame(void)
		//{
		//	ASSERT(m_pCurGame != NULL);
		//
		//	if(!m_pCurGame)
		//		return false;
		//
		//	map<IDX_GAME, CGame *>::iterator start = m_listGame.begin();
		//	map<IDX_GAME, CGame *>::iterator end = m_listGame.end();
		//
		//	for(;start != end;start++)
		//	{
		//		if(start->first != m_nCurGameType)
		//		{
		//			if(start->second->IsLoadSprite()) start->second->ReleaseSprite();
		//			if(start->second->IsLoadSound()) start->second->ReleaseSound();
		//		}
		//	}
		//
		//	if(!m_pCurGame->IsLoadSound()) m_pCurGame->LoadSound();
		//	if(!m_pCurGame->IsLoadSprite())
		//	{
		//		ClearLoadImageList();
		//		m_pCurGame->LoadSprite();
		//		LoadingImageList(0, FALSE);
		//		return false;
		//	}
		//	return true;
		//}
		//
		//LONG CViewManager::OnPacketNotify(int Sid, int Signal, int TotSize, char *lpData)
		//{	
		//	ASSERT(m_pCurGame != NULL);
		//	if(m_pCurGame)
		//		return m_pCurGame->OnPacketNotify(Sid, Signal, TotSize, lpData);
		//	
		//	return FALSE;
		//} 
		//
		//LRESULT CViewManager::ShowCreateRooomDlg(void)
		//{
		//	if(!m_pCurGame)
		//		return FALSE;
		//
		//	CDataRoomOptionList *pDataRoomOptionList = GM().GetDataRoomOptionList();
		//	if(pDataRoomOptionList == NULL) return FALSE;
		//	ROOMOPTION *pDefaultRoomOption = pDataRoomOptionList->GetDefaultRoomOption();
		//	if(pDefaultRoomOption == NULL) return FALSE;
		//
		//	ASSERT(m_pCurGame != NULL);
		//	ZeroMemory(&m_stRoomInfo, sizeof(ROOMINFO));
		//
		//	CPlayer *pMyInfo = GM().GetMyInfo();
		//	ZeroMemory(&m_stRoomInfo, sizeof(ROOMINFO));
		//	m_stRoomInfo.UNum = pMyInfo->UI.UNum;
		//	strncpy_s(m_stRoomInfo.ID, sizeof(m_stRoomInfo.ID), pMyInfo->UI.ID, 15);
		//	strncpy_s(m_stRoomInfo.NickName, sizeof(m_stRoomInfo.NickName), pMyInfo->UI.NickName, 19);
		//
		//	m_stRoomInfo.NowUserNum		= 1;
		//	m_stRoomInfo.MaxUserNum		= m_pCurGame->GetMaxPlayer();
		////	m_stRoomInfo.CreateRGrade	= 0;
		//	m_stRoomInfo.nGameIndex		= m_pCurGame->GetGameType();
		//	m_stRoomInfo.RoomNum		= 0;
		//	m_stRoomInfo.GameKind		= 0;
		//	m_stRoomInfo.State			= 0;
		//	m_stRoomInfo.ObserverOption	= 0;
		//	m_stRoomInfo.FormKind		= CHOICE_GAME;
		//	m_stRoomInfo.bSuperMaster	= FALSE;
		////	m_stRoomInfo.bPlayer		= m_pCurGame->GetMaxPlayer();
		////	m_stRoomInfo.nChoiceMode	= CHOICE_MODE;
		//	m_stRoomInfo.RuleType		= RULETYPE_7POKER;
		//	m_stRoomInfo.nAutoBetKind	= pDefaultRoomOption->sAutoBet;
		//	m_stRoomInfo.BetMoney		= pDefaultRoomOption->llSeedMoney;
		//	m_stRoomInfo.llEnterLimitMoney = pDefaultRoomOption->llEnterLimitMoney;
		//	m_stRoomInfo.bVIPEnterLimitYN = false;
		//
		//	LRESULT nRet = m_pCurGame->ShowCreateRooomDlg(&m_stRoomInfo);
		//
		//	SetWaitResponse(false);
		//	SetAccessRoomState(false);
		//	if(nRet == IDOK)
		//	{
		//		if(GM().CheckLoadGame())
		//		{
		//			ForceCreateRooom();
		//		}
		//		else
		//		{
		//			m_nLoadingType = EGLT_CREATE;
		//		}
		//	}
		//	
		//	return nRet;
		//}
		//
		//LRESULT CViewManager::ForceCreateRooom(void)
		//{
		//	CSV_ASK_CREATEROOM msg;
		//
		//	msg.Set(&m_stRoomInfo, GetMyInfo()->nSndFxKind);	
		//	SockMan.SendData(g_MainSrvSID, msg.pData, msg.GetTotalSize());
		//	SetWaitResponse(true);
		//	SetAccessRoomState(true);
		//	return TRUE;
		//}
		//
		//
		//LRESULT CViewManager::GoDirectEnterRoom(bool bUseRoomOption, ROOMOPTION *pRoomOption, bool bCheckLoad/* = false*/, bool bSecret/* = false*/, int nGameMode/* = -1*/)
		//{
		//	ASSERT(m_pCurGame != NULL);
		//
		//	m_nDirectEnterGameMode = nGameMode;
		//
		//	if(!m_pCurGame)
		//		return FALSE;
		//
		//	if(bCheckLoad && CheckLoadGame() == false)
		//	{
		//		m_nLoadingType = EGLT_DIRECTCREATE;
		//		
		//		///옵션정보저장
		//		m_bUseRoomOption = bUseRoomOption;		
		//		if (pRoomOption)
		//		{
		//			m_RoomOption = *pRoomOption;
		//		}
		//		else
		//		{
		//			m_bUseRoomOption = false;
		//			pRoomOption = NULL;
		//		}
		//
		//		return FALSE;
		//	}
		//
		//	ROOMOPTION *pMakeRoomOption = NULL;
		//	if (bUseRoomOption && pRoomOption )	
		//	{
		//		//bUseRoomOption가 TRUE이면 넘겨받은 옵션으로 방을 만든다.
		//		pMakeRoomOption = pRoomOption;
		//	}
		//	else
		//	{
		//		//bUseRoomOption가 FALSE이면 디폴트 옵션으로 방을 만든다.		
		//		CDataRoomOptionList *pDataRoomOptionList = GM().GetDataRoomOptionList();
		//		if(pDataRoomOptionList == NULL) return FALSE;
		//		pMakeRoomOption = pDataRoomOptionList->GetDefaultRoomOption();	
		//	}	
		//
		//	if(pMakeRoomOption == NULL) 
		//		return FALSE;
		//
		//
		//	if(nGameMode == -1) nGameMode = m_pCurGame->GetDefaultFormKind();
		//
		//	CPlayer *pMyInfo = GM().GetMyInfo();
		//	ZeroMemory(&m_stRoomInfo, sizeof(ROOMINFO));
		//	m_stRoomInfo.UNum = pMyInfo->UI.UNum;
		//	strncpy_s(m_stRoomInfo.ID, sizeof(m_stRoomInfo.ID), pMyInfo->UI.ID, 15);
		//	strncpy_s(m_stRoomInfo.NickName, sizeof(m_stRoomInfo.NickName), pMyInfo->UI.NickName, 19);
		//
		//	m_stRoomInfo.NowUserNum		= 1;
		//	m_stRoomInfo.MaxUserNum		= m_pCurGame->GetMaxPlayer();
		////	m_stRoomInfo.CreateRGrade	= 0;
		//	m_stRoomInfo.nGameIndex		= m_pCurGame->GetGameType();
		//	m_stRoomInfo.RoomNum		= 0;
		//	m_stRoomInfo.GameKind		= 0;
		//	m_stRoomInfo.State			= 0;
		//	m_stRoomInfo.ObserverOption	= 0;
		//	m_stRoomInfo.FormKind		= (char)nGameMode;
		//	m_stRoomInfo.bSuperMaster	= FALSE;
		////	m_stRoomInfo.bPlayer		= 0;
		////	m_stRoomInfo.nChoiceMode	= CHOICE_MODE;
		//	m_stRoomInfo.RuleType		= pMakeRoomOption->cRuleType;
		//	m_stRoomInfo.nAutoBetKind	= pMakeRoomOption->sAutoBet;
		//	m_stRoomInfo.BetMoney		= pMakeRoomOption->llSeedMoney;
		//	m_stRoomInfo.llEnterLimitMoney = pMakeRoomOption->llEnterLimitMoney;
		//	m_stRoomInfo.bVIPEnterLimitYN = false;
		//	m_stRoomInfo.Secret			=  (char)bSecret; //잠금방으로 만들지 여부
		//	m_pCurGame->CheckDirectEnterRoomInfo(0, &m_stRoomInfo);
		//
		//	CSV_ASK_CREATEROOM msg;
		//	msg.Set(&m_stRoomInfo, GM().GetMyInfo()->nSndFxKind, !bUseRoomOption);		//<--------3번째 인자가 true이면 서버에서 디폴트 베팅룰/시드머니을 알아서 만들어준다.
		//	SockMan.SendData(g_MainSrvSID, msg.pData, msg.GetTotalSize());
		//	GM().SetWaitResponse(true);
		//
		//	return TRUE;
		//}
		//
		//
		//LRESULT CViewManager::GoEnterDummyRoom(SMALLROOMINFO *pSI)
		//{
		//	ASSERT(m_pCurGame != NULL);
		//
		//	if(!m_pCurGame)
		//		return FALSE;
		//	
		//	if (pSI)
		//	{
		//		CPlayer *pMyInfo = GM().GetMyInfo();
		//		ZeroMemory(&m_stRoomInfo, sizeof(ROOMINFO));
		//	
		//		m_stRoomInfo.UNum = pMyInfo->UI.UNum;
		//		strncpy_s(m_stRoomInfo.ID, sizeof(m_stRoomInfo.ID), pMyInfo->UI.ID, 15);	
		//		strncpy_s(m_stRoomInfo.NickName, sizeof(m_stRoomInfo.NickName), pMyInfo->UI.NickName, 19);
		//		strncpy_s(m_stRoomInfo.Title, sizeof(m_stRoomInfo.Title), pSI->szTitle, 31);
		//		
		//		m_stRoomInfo.NowUserNum		= 1;
		//		m_stRoomInfo.MaxUserNum		= m_pCurGame->GetMaxPlayer();
		////		m_stRoomInfo.CreateRGrade	= 0;
		//		m_stRoomInfo.nGameIndex		= m_pCurGame->GetGameType();
		//		m_stRoomInfo.RoomNum		= 0;
		//		m_stRoomInfo.GameKind		= 0;
		//		m_stRoomInfo.State			= 0;
		//		m_stRoomInfo.ObserverOption	= 0;
		//		m_stRoomInfo.FormKind		= pSI->cFormKind;
		//		m_stRoomInfo.bSuperMaster	= FALSE;
		////		m_stRoomInfo.bPlayer		= m_pCurGame->GetMaxPlayer();
		//		//m_stRoomInfo.nChoiceMode	= CHOICE_MODE;
		//		m_stRoomInfo.RuleType		= pSI->cRuleType;
		//		m_stRoomInfo.nAutoBetKind	= pSI->sAutoBetKind;
		//		m_stRoomInfo.BetMoney		= pSI->llSeedMoney;
		//		m_stRoomInfo.llEnterLimitMoney = pSI->llEnterLimitMoney;
		//	}
		//
		//
		//	if(CheckLoadGame() == false)
		//	{
		//		m_nLoadingType = EGLT_ENTERDUMMYROOM;
		//		return FALSE;
		//	}
		//
		//
		//	CSV_ASK_CREATEROOM msg;
		//	msg.Set(&m_stRoomInfo, GM().GetMyInfo()->nSndFxKind);
		//	SockMan.SendData(g_MainSrvSID, msg.pData, msg.GetTotalSize());
		//	GM().SetWaitResponse(true);
		//
		//	return TRUE;
		//}
		//
		//LRESULT CViewManager::AskDirectEnterRoom(bool bUseRoomOption, ROOMOPTION *pRoomOption, int nGameMode, bool bSerchEnterRoom/* = false*/, bool bSerchAutoEnterRoom/* = false*/)
		//{
		//	ASSERT(m_pCurGame != NULL);	
		//
		//	if(!m_pCurGame)
		//		return FALSE;
		//
		//	m_bSerchEnterRoom = bSerchEnterRoom;
		//	m_bSerchAutoEnterRoom = bSerchAutoEnterRoom;
		//	m_nDirectEnterGameMode = nGameMode;
		//
		//	//m_nGoDirectEnterType = m_pCurGame->GetDefaultFormKind();
		//	m_bUseRoomOption = bUseRoomOption;
		//	if (pRoomOption)
		//	{
		//		m_RoomOption = *pRoomOption;
		//		if(nGameMode == -1) m_RoomOption.cFormKind = m_pCurGame->GetDefaultFormKind();
		//		else m_RoomOption.cFormKind = (char)nGameMode;
		//	}
		//	else
		//	{
		//		m_bUseRoomOption = false;
		//		pRoomOption = NULL;
		//	}
		//
		//	if(GM().CheckLoadGame())
		//	{
		//		AskForceDirectEnterRoom(m_bUseRoomOption, &m_RoomOption);
		//	}
		//	else
		//	{
		//		m_nLoadingType = EGLT_DIRECTENTER;
		//	}
		//	return TRUE;
		//}
		//
		//LRESULT CViewManager::AskForceDirectEnterRoom(bool bUseRoomOption, ROOMOPTION *pRoomOption)
		//{
		//
		//	ROOMOPTION ro = {0,};
		//
		//	bool bSet = false;
		//	if(pRoomOption == NULL && bUseRoomOption == false) bSet = true;
		//
		//	if (pRoomOption)
		//	{
		//		ro = *pRoomOption;	
		//	}
		//	else
		//	{
		//		bUseRoomOption = false;
		//	}
		//
		//	if(bSet) ro.cFormKind = m_pCurGame->GetDefaultFormKind();
		//
		//	CSV_ASK_QUICKSTART	msg;
		//	CPlayer *pPlayer = GM().GetMyInfo();
		//	if(pPlayer == NULL) return FALSE;
		//	msg.Set(pPlayer->UI.UNum, pPlayer->UI.ID, pPlayer->nSndFxKind, bUseRoomOption, ro);
		//	SockMan.SendData(g_MainSrvSID, msg.pData, msg.GetTotalSize());
		//	return TRUE;
		//}
		//
		//void CViewManager::AddGameChat(const char *str, COLORREF crNewColor/*=RGB(0,0,0)*/, bool bUserChat/* = false*/)
		//{
		//	ASSERT(m_pCurGame != NULL);
		//	if(m_pCurGame)
		//		return m_pCurGame->AddGameChat(str, crNewColor, bUserChat);
		//}
		//
		//LRESULT	CViewManager::ShowQuickJoinerDlg(void)
		//{
		//	ASSERT(m_pCurGame != NULL);
		//	if(m_pCurGame)
		//	{
		//		return m_pCurGame->ShowQuickJoinerDlg();
		//	}
		//
		//	return 0;
		//}
		//
		//LRESULT	CViewManager::SendQuickJoiner(void)
		//{
		//
		//	if(CheckLoadGame() == false)
		//	{
		//		m_nLoadingType = EGLT_PREMQUICKJOIN;
		//		return FALSE;
		//	}
		//
		//	ASSERT(m_pCurGame != NULL);
		//	return m_pCurGame->SendQuickJoiner();
		//}
		//
		//void CViewManager::ShowGameWindow(bool bShow)
		//{
		//	ASSERT(m_pCurGame != NULL);
		//	if(m_pCurGame)
		//		m_pCurGame->ShowGameWindow(bShow);
		//}
		//
		//void CViewManager::SetCurrentWhere(IDX_GAMEWHERE nWhere)
		//{
		//	if(m_nWhere == nWhere) return;
		//	m_nWhere = nWhere;
		//
		//	if(m_pCurGame == NULL) return;
		//	CGameViewBase *pViewBase = m_pCurGame->GetGameView();
		//	if(pViewBase == NULL) return;
		//
		//	if(m_nWhere == IDX_GAMEWHERE_GAME) pViewBase->StartTimer();
		//	else pViewBase->StopTimer();
		//}
		//
		//IDX_GAMEWHERE CViewManager::GetCurrentWhere(void)
		//{
		//	return m_nWhere;
		//}
		//
		////UGP작업 추가
		//void CViewManager::ExitGame()
		//{
		//	if(IsWaitResponse() == true) return;
		//
		//	if(IsAbnormalOutRoom()) // 비정상적인 퇴실이면
		//	{
		//		OnExitRoom();
		//		return;
		//	}
		//	
		//	// 방에서 나감을 메인서버에 요청
		//	CSV_ASK_OUTROOM aormsg;
		//	// ### [ 관전기능 ] ###
		//	CPlayer *pPlayer = GetMyInfo();
		//	aormsg.Set(g_RI.RoomNum, pPlayer->UI.UNum, pPlayer->UI.ID);
		//	SockMan.SendData(g_MainSrvSID, aormsg.pData, aormsg.GetTotalSize());
		//	SetWaitResponse(true);
		//}
		//
		//void CViewManager::OnExitRoom(void)
		//{
		//	SetAbnormalOutRoom(false);
		//	SetAccessRoomState(false);
		//	SetWaitResponse(false);
		//	
		//	// 게임정보 초기화
		//	ZeroMemory(&g_RI, sizeof(g_RI));
		//
		//	// 대기실 화면으로 전환
		//	SetChangeWhere(IDX_GAMEWHERE_LOBY, true);
		//
		//	m_pCurGame->OnExitRoom();
		//
		//	//관전자 초기화
		//	CObserverDlg *pObserverDlg = GetObserverDlg(); 
		//	if(NULL != pObserverDlg)
		//	{
		//		if(pObserverDlg->GetSafeHwnd())
		//		{
		//			pObserverDlg->ShowWindow(SW_HIDE);
		//			pObserverDlg->User_DelAll(); //모두 제거
		//		}
		//	}
		//
		//	// 초대 대화창 감춤
		//	CInviteDlg *pInviteDlg = GetInviteDlg();
		//	if(pInviteDlg && pInviteDlg->GetSafeHwnd())		
		//	{
		//		pInviteDlg->ShowWindow(SW_HIDE);
		//	}
		//}
		//
		//void CViewManager::OnEnterRoom(BOOL bCreation)
		//{
		//	ASSERT(m_pCurGame != NULL);
		//
		//	if(m_pCurGame)
		//		m_pCurGame->OnEnterRoom(bCreation);
		//}
		//
		//BOOL CViewManager::CheckMutex(LPCSTR lpAppName)
		//{
		//	// 중복 실행 방지
		//	m_hMutex=::CreateMutex(NULL, TRUE, _T("WoolalaMutex")); // 같은 뮤택스가 존재하면 이미 다른 프로그램이 실행중임
		//	if(ERROR_ALREADY_EXISTS==::GetLastError())
		//	{       
		//		HWND hWnd=::FindWindow(_T("#32770 (Dialog)"), lpAppName); // ClassName, WindowsName
		//		::SetForegroundWindow(hWnd);
		//
		//		if(m_hMutex)
		//			::CloseHandle(m_hMutex);		
		//
		//		AfxMessageBox( "이미 다른 게임이 실행중입니다");
		//
		//		return FALSE;
		//	}
		//	return TRUE;
		//}
		//
		//void CViewManager::DestroyMutex(void)
		//{
		//	if( m_hMutex != NULL )		
		//	{
		//		::CloseHandle( m_hMutex );
		//		m_hMutex= NULL;
		//	}
		//}
		//
		//void CViewManager::InitGoGameDirect(void)
		//{
		//	NMGoGameDirect_Init(&m_hMutex,GM().GetCurrentGameString());
		//}
		//
		//void CViewManager::OnChangeFullScreen(bool bFullScreen)
		//{
		//	map<IDX_GAME, CGame *>::iterator start = m_listGame.begin();
		//	map<IDX_GAME, CGame *>::iterator end = m_listGame.end();
		//
		//	CGameViewBase *pGameView = NULL;
		//	CRect MovRect;
		//
		//	for(;start != end;start++)
		//	{
		//		pGameView = start->second->GetGameView();
		//		MovRect.SetRect(0,0,GM().GetWidth(),GM().GetHeight());
		//
		//		if(bFullScreen)	MovRect.OffsetRect(CPoint((1024 - GM().GetWidth())/2, (768 - GM().GetHeight())/2));
		//
		//		pGameView->MoveWindow(MovRect);
		//
		//		start->second->OnChangeFullScreen(bFullScreen);
		//	}
		//}
		//
		//void CViewManager::LoadSprite(void)
		//{
		//	ASSERT(m_pCurGame != NULL);
		//
		//	if(m_pCurGame)
		//		m_pCurGame->LoadSprite();
		//}
		//
		//LRESULT CViewManager::ShowOptionDlg(void)
		//{
		//	COptionDlg Dlg(GetMainWnd());
		//	LRESULT nRet = Dlg.DoModal();
		//	if (nRet == IDOK)
		//	{
		//		//  세이브 시키고 적용
		//		SaveGameConfig(GetMyInfo()->UI.ID);
		//	}
		//	
		//	map<IDX_GAME, CGame *>::iterator start = m_listGame.begin();
		//	map<IDX_GAME, CGame *>::iterator end = m_listGame.end();
		//
		//	for(;start != end;++start)
		//	{
		//		start->second->OnCloseOptionDlg(nRet);
		//	}
		//	return nRet;
		//}
		//
		//void CViewManager::ReLoadSprite(int nSkinType)
		//{
		//	ASSERT(m_pCurGame != NULL);
		//
		//	if(m_pCurGame)
		//	{
		//		m_pCurGame->ReLoadSprite(nSkinType);
		//	}
		//}
		//
		//const int CViewManager::GetWidth(void)
		//{
		//	ASSERT(m_pCurGame != NULL);
		//
		//	if(m_pCurGame)
		//	{
		//		return m_pCurGame->GetWidth();
		//	}
		//
		//	return -1;
		//}
		//
		//const int CViewManager::GetHeight(void)
		//{
		//	ASSERT(m_pCurGame != NULL);
		//
		//	if(m_pCurGame)
		//	{
		//		return m_pCurGame->GetHeight();
		//	}
		//
		//	return -1;
		//}
		//
		//const CString CViewManager::GetCurrentGameString(void)
		//{
		//	ASSERT(m_pCurGame != NULL);
		//
		//	if(m_pCurGame)
		//	{
		//		return m_pCurGame->GetGameString();
		//	}
		//
		//	return "";
		//}
		//
		//const CString CViewManager::GetCurrentGameName(void)
		//{
		//	ASSERT(m_pCurGame != NULL);
		//
		//	if(m_pCurGame)
		//	{
		//		return m_pCurGame->GetGameName();
		//	}
		//
		//	return "";
		//}
		//
		//const int CViewManager::GetCurrentGameCode(void)
		//{
		//	ASSERT(m_pCurGame != NULL);
		//
		//	if(m_pCurGame)
		//	{
		//		return m_pCurGame->GetGameCode();
		//	}
		//
		//	return -1;
		//}
		//
		//
		//void CViewManager::AddHideChatText(CString *str)
		//{
		//	m_HideChat.AddData(str);
		//
		//}
		//void CViewManager::ClearHideChat()
		//{
		//	m_HideChat.Clear();
		//}
		//CString &CViewManager::GetHideChatData()
		//{
		//	return m_HideChat.GetData();
		//}
		//
		//void CViewManager::HideChat_MAKE_IDList(void *pDlg)
		//{
		//	m_HideChat.MAKE_IDList(pDlg);	
		//
		//}
		//

		//
		//void CViewManager::AppendLoadSprite(xSprite *pSprite)
		//{
		//	list<xSprite *>::iterator findIt = find(m_listLoadSprite.begin(), m_listLoadSprite.end(), pSprite);
		//	if(findIt != m_listLoadSprite.end()) return;
		//	m_listLoadSprite.push_back(pSprite);
		//}
		//
		//void CViewManager::RemoveLoadSprite(void)
		//{
		//	list<xSprite *>::iterator start = m_listLoadSprite.begin();
		//	list<xSprite *>::iterator end = m_listLoadSprite.end();
		//
		//	for(;start != end;start++)
		//	{
		//		(*start)->Remove();
		//	}
		//
		//	m_listLoadSprite.clear();
		//}
		//
		//void CViewManager::OnLoadingEnd(void)
		//{
		//	ASSERT(m_pCurGame != NULL);
		//
		//	if(m_pCurGame)
		//		m_pCurGame->OnLoadingEnd();
		//
		//	switch(m_nLoadingType)
		//	{
		//	case EGLT_CREATE:
		//		ForceCreateRooom();
		//		break;
		//	case EGLT_DIRECTENTER:
		//		AskForceDirectEnterRoom(m_bUseRoomOption, &m_RoomOption);		
		//		break;
		//	case EGLT_DIRECTCREATE:
		//		GoDirectEnterRoom(m_bUseRoomOption, &m_RoomOption, false, false, m_nDirectEnterGameMode);
		//		break;
		//	case EGLT_ENTERROOM:
		//		AskEnterRoom(m_nRoomNum_for_Join, m_llRoomKey_for_Join, m_bObserve_for_Join, m_strRoomPass_for_Join);
		//		break;
		//	case EGLT_ENTERDUMMYROOM:
		//		GoEnterDummyRoom(NULL);
		//		break;
		//	case EGLT_PREMQUICKJOIN:
		//		SendQuickJoiner();
		//		break;
		//	}
		//}
		//
		//LRESULT CViewManager::AskEnterRoom(int nRoomNum, UINT64 llRoomKey, bool bObserve, const char *pRoomPass)
		//{
		//	//참여접속
		//	if(GM().IsWaitResponse() == true) 
		//		return FALSE;
		//	if(IsAccessRoomState()) 
		//		return FALSE;
		//
		//	if(GM().GetMyInfo()->UI.GetMoney() <= 0) 
		//	{	
		//		ShowNewAskChargeDlg(NEW_ASK_CHARGE_DLG_TYPE_FREE_CHARGE_INSU, "");
		//		return FALSE;
		//	}
		//
		//	if(nRoomNum < 0 ) 
		//		return FALSE;
		//
		//	if(CheckLoadGame() == false)
		//	{
		//		m_nLoadingType = EGLT_ENTERROOM;
		//
		//		//들어갈 방의 정보를 저장해 놓는다.
		//		m_nRoomNum_for_Join = nRoomNum,
		//		m_llRoomKey_for_Join = llRoomKey,
		//		m_bObserve_for_Join = bObserve;	
		//		
		//		if (pRoomPass)
		//			m_strRoomPass_for_Join = pRoomPass;
		//		else 
		//			m_strRoomPass_for_Join.Empty();
		//		
		//		return FALSE;
		//	}
		//
		//	GM().SetWaitResponse(true);
		//	SetAccessRoomState(true);
		//
		//	// 방에 입장하기를 요구
		//	CSV_ASK_ENTERROOM aermsg;
		//	aermsg.Set(nRoomNum, llRoomKey, GM().GetMyInfo()->UI.UNum,  GM().GetMyInfo()->nSndFxKind, GM().GetMyInfo()->UI.ID, (char*) pRoomPass, (char)bObserve);// ### [ 관전기능 ] ###  // ### [사운드 추가 작업] ###
		//	SockMan.SendData(g_MainSrvSID, aermsg.pData, aermsg.GetTotalSize());
		//
		//	ShowMainMessageDlg(false, "요청한 방에 연결중입니다");	
		//	return TRUE;
		//}
		//
		//
		//bool CViewManager::SendWhisper(const char *szMessage, const char *szTargetNick, UINT64 llTargetUserKey, const char *szTargetID)
		//{
		//	//llTargetUserKey와 sztargetID를 모를경우 각각 -1 , 비워둔다.
		//
		//	if ( szTargetNick==NULL || szMessage==NULL)
		//		return false;
		//
		//	if (strlen(szMessage)==0)
		//		return false;
		//
		//	CString strTargetID;
		//	if (szTargetID)
		//	{
		//		strTargetID = szTargetID;
		//	}
		//
		//	//귓말 보내기 처리
		//	USERINFO *pMyUI = &GM().GetMyInfo()->UI;
		//	
		//	CSV_WHISPERCHAT wcmsg;
		//	wcmsg.Set(	pMyUI->ID,				//보내는 사람 아이디
		//				pMyUI->NickName,		//보내는 사람 닉네임
		//				pMyUI->llUserKey,		//보내는 사람 유저키
		//				strTargetID,			//받는 사람 아이디
		//				szTargetNick,			//받는 사람 닉네임
		//				llTargetUserKey,		//받는 사람 유저키
		//				szMessage );			//메세지
		//		
		//	SockMan.SendData(g_MainSrvSID, wcmsg.pData, wcmsg.GetTotalSize());
		//
		//	return true;
		//}


	}//namespace FRAME

}//namespace NMBASE
