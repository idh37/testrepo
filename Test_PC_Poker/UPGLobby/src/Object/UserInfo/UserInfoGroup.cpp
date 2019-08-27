#include "StdAfx.h"
#include "UserInfoGroup.h"
#include "../../WidgetID.h"
#include "UserInfoDetailDlg.h"
#include "../../Object/Captcha/CaptchaDlg.h"

#define _DEF_MEMO_RESET_TIMER 30001
#define _ONETIMER			  30002

CUserInfoGroup::CUserInfoGroup()
:	m_pAvatar(NULL),
	m_pNickName(NULL),
	m_pHaveMoney(NULL),
	m_pPopup(NULL),
	m_pHaveTotalMoney(NULL),
	m_pChangeMoney(NULL),
	m_pMyScore(NULL),
	m_pBtnGoldText(NULL),
	m_pBtnCharge(NULL),
	m_pBtnDetailOpen(NULL),
	m_pBtnEvent(NULL),
	m_pBtnCafe(NULL),
	m_pBtnGoldChange(NULL),
	m_pBtnClub(NULL),
	m_i64LoginMoney(0),
	m_pUserInfo(NULL),
	m_pDetailInfo(NULL),
	m_pBtnMemoCount(NULL),
	m_pTxtMemoCount(NULL),
	m_pImgCommunityMemoCount(NULL),
	m_bConnect(false),
	m_pCommunityDisconnectBuddy(NULL),
	m_pImgCommunityConnectBuddyCount(NULL),
	m_pBtnConnectBuddyCount(NULL),
	m_pTxtConnectBuddyCount(NULL),
	m_bRefresh(false),
	m_pDailyGold(NULL),
	m_pDailyGoldAlarm(NULL)
{
}

CUserInfoGroup::~CUserInfoGroup()
{
}

void CUserInfoGroup::OnCreateChild(LSY::CObjectBase *pObject)
{
	if(pObject == NULL) return;
	bool bAddChild = false;
	switch(pObject->GetID())
	{
	case ID_LOBBY_AVT_MYINFO:					m_pAvatar			= (LSY::CAvatar *)pObject;		bAddChild = true; break;
	case ID_LOBBY_TXT_MYINFO_NAME:				m_pNickName			= (LSY::CText *)pObject;		break;
	case ID_LOBBY_IMGNB_MYINFO_MONEY:
		m_pHaveMoney		= (LSY::CImageNumber *)pObject;
		pObject->AddHandler(LSY::EM_O_MOUSEOVER, LSY::Fnt(this, &CUserInfoGroup::OnMoneyMouseOver));
		pObject->AddHandler(LSY::EM_O_MOUSELEAVE, LSY::Fnt(this, &CUserInfoGroup::OnMoneyMouseLeave));
		break;
	case ID_LOBBY_TEXT_MYINFO_GOLD:
		m_pHaveGold		= (LSY::CTextNumber *)pObject;
		break;		
	case ID_LOBBY_IMG_MYINFO_MONEY_POPUP_BACK:
		m_pPopup			= (LSY::CImage *)pObject;
		m_pPopup->SetShow(false);
		break;
	case ID_LOBBY_TXTNB_MYINFO_FULL_MONEY:		m_pHaveTotalMoney	= (LSY::CTextNumber *)pObject;	break;
	case ID_LOBBY_TXTNB_MYINFO_CHANGE_MONEY:	m_pChangeMoney		= (LSY::CTextNumber *)pObject;	break;
	case ID_LOBBY_TXT_MYINFO_SCORE:				m_pMyScore			= (LSY::CText *)pObject;		break;
	case ID_LOBBY_BTN_CHARGE:					m_pBtnCharge		= (LSY::CButton *)pObject;		bAddChild = true; break;
	case ID_LOBBY_BTN_GOLD_TEXT:				m_pBtnGoldText		= (LSY::CTextButton *)pObject;	bAddChild = true; break;
	case ID_LOBBY_BTN_MYINFO_DETAIL_OPEN:		m_pBtnDetailOpen	= (LSY::CButton *)pObject;		bAddChild = true; break;
	case ID_LOBBY_BTN_EVENT:					m_pBtnEvent			= (LSY::CButton *)pObject;	    bAddChild = true; break;
	case ID_LOBBY_BTN_WITHDRAW:					m_pBtnWithdraw		= (LSY::CButton *)pObject;	    bAddChild = true; break;
	case ID_LOBBY_BTN_MYINFO_CAFE:				m_pBtnCafe			= (LSY::CButton *)pObject;	    bAddChild = true; break;
	case ID_LOBBY_BTN_MYINFO_GOLD_CHANGE:		
		m_pBtnGoldChange	= (LSY::CButton *)pObject;		
		bAddChild = true; 
		m_pBtnGoldChange->SetEnable(false); //빅휠 삭제 2018.07.12
		break;
	case ID_LOBBY_BTN_MYINFO_CLUB:				
		m_pBtnClub			= (LSY::CButton *)pObject;		
		bAddChild = true; 
		m_pBtnClub->SetShow(false);
		break;
	case ID_LOBBY_BTN_DAILYGOLD:
		{
			m_pDailyGold = (LSY::CButton *)pObject;
			bAddChild = true;
			m_pDailyGold->SetEnable(false);
		}
		break;
	case ID_LOBBY_IMG_DAILYGOLD_ENABLE:
		{
			m_pDailyGoldAlarm = (LSY::CImage *)pObject;
			m_pDailyGoldAlarm->SetShow(false);
		}
		break;
	case ID_LOBBY_WND_MYINFO_DETAIL:
		m_pDetailInfo		= (CUserInfoDetailDlg *)pObject;
		m_pDetailInfo->SetShow(false);
		break;
	case ID_LOBBY_BTN_MEMO_COUNT:
		{
			m_pBtnMemoCount		= (LSY::CButton *)pObject;
			//0이상일때 항상 노출되는걸로 수정
			//m_pBtnMemoCount->AddHandler(LSY::EM_O_MOUSEOVER, LSY::Fnt(this, &CUserInfoGroup::OnMoneyMouseOver));
			//m_pBtnMemoCount->AddHandler(LSY::EM_O_MOUSELEAVE, LSY::Fnt(this, &CUserInfoGroup::OnMoneyMouseLeave));
			bAddChild = true; 
			break;
		}
	case ID_LOBBY_TXTNB_MEMO_COUNT:				m_pTxtMemoCount		= (LSY::CTextNumber *)pObject;	break;
	case ID_LOBBY_IMG_COMMUNITY_DISCONNECT:		m_pCommunityDisconnect = (LSY::CImage *)pObject; break;
	case ID_LOBBY_IMG_COMMUNITY_MEMOCOUNT:		m_pImgCommunityMemoCount = (LSY::CImage *)pObject; break;
	case ID_LOBBY_BTN_CONNECT_BUDDY_COUNT:
		{
			m_pBtnConnectBuddyCount		= (LSY::CButton *)pObject;
			//0이상일때 항상 노출되는걸로 수정
			//m_pBtnConnectBuddyCount->AddHandler(LSY::EM_O_MOUSEOVER, LSY::Fnt(this, &CUserInfoGroup::OnMoneyMouseOver));
			//m_pBtnConnectBuddyCount->AddHandler(LSY::EM_O_MOUSELEAVE, LSY::Fnt(this, &CUserInfoGroup::OnMoneyMouseLeave));
			bAddChild = true; 
			break;
		}
	case ID_LOBBY_TXTNB_CONNECT_BUDDY_COUNT:		m_pTxtConnectBuddyCount				= (LSY::CTextNumber *)pObject;	break;
	case ID_LOBBY_IMG_COMMUNITY_DISCONNECT_BUDDY:	m_pCommunityDisconnectBuddy			= (LSY::CImage *)pObject; break;
	case ID_LOBBY_IMG_COMMUNITY_CONNECT_BUDDY_COUNT:m_pImgCommunityConnectBuddyCount	= (LSY::CImage *)pObject; break;
	}

	if(bAddChild)
	{
		pObject->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CUserInfoGroup::OnClickButton));
	}

	__super::OnCreateChild(pObject);
}

void CUserInfoGroup::OnCreated(void)
{
	LSY::CObjectProject *pProject = GetObjectMan()->GetProjectFromID(ID_LOBBY);
	OnCreateChild(pProject->GetObject(ID_LOBBY_TXTNB_MYINFO_FULL_MONEY));

	OnCreateChild(pProject->GetObject(ID_LOBBY_IMG_COMMUNITY_DISCONNECT));
	OnCreateChild(pProject->GetObject(ID_LOBBY_TXTNB_MEMO_COUNT));
	OnCreateChild(pProject->GetObject(ID_LOBBY_IMG_COMMUNITY_MEMOCOUNT));

	OnCreateChild(pProject->GetObject(ID_LOBBY_IMG_COMMUNITY_DISCONNECT_BUDDY));
	OnCreateChild(pProject->GetObject(ID_LOBBY_TXTNB_CONNECT_BUDDY_COUNT));
	OnCreateChild(pProject->GetObject(ID_LOBBY_IMG_COMMUNITY_CONNECT_BUDDY_COUNT));

	m_pNickName->SetText("");
	m_pHaveTotalMoney->SetNumber(0);
	m_pChangeMoney->SetNumber(0);
	m_pMyScore->SetText("");
	m_pHaveMoney->SetNumber(0);
	m_pHaveGold->SetNumber(0);
	
	m_pCommunityDisconnect->SetShow(false);
	m_pTxtMemoCount->SetNumber(0);

	m_pCommunityDisconnectBuddy->SetShow(false);
	m_pTxtConnectBuddyCount->SetNumber(0);

	SetComunityConnect(false);
	SetMemoCount(0);
	MM().AddMsgH(UPG_MEMO_CHECK_START, Fnt(this, &CUserInfoGroup::StartTimer));	
	MM().AddMsgH(UPG_MEMO_CALL_LIST, Fnt(this, &CUserInfoGroup::OnEndCall));
	MM().AddMsgH(UPG_CMK_SHOW_SENDMESSAGE_DIALOG, Fnt(this, &CUserInfoGroup::OnShowSendMessageDlg));			//쪽지보내기 창을 연다.
	MM().AddMsgH(UPG_CMK_SET_MONEY_NUM, Fnt(this, &CUserInfoGroup::OnSetMoneyNum));
	MM().AddMsgH(UPG_CMK_SET_LOBBY_BUTTON_ENABLE, Fnt(this, &CUserInfoGroup::OnSetLobbyButtonEnable));
	
	__super::OnCreated();
}

LRESULT CUserInfoGroup::OnClickButton(LSY::CMessage *msg)
{
	LSY::CMO_MouseLClick *pMsg = (LSY::CMO_MouseLClick *)msg;
	LSY::CButton *pBtn = (LSY::CButton *)pMsg->GetObject();
	CWnd *pWnd = GetObjectMan()->GetWnd();
	switch(pBtn->GetID())
	{
	case ID_LOBBY_BTN_MYINFO_DETAIL_OPEN:
		m_pDetailInfo->SetShow(true);
		break;
	case ID_LOBBY_BTN_CHARGE:
		SendUILog_Basic(CSSTATS_UINO_POKER_LOBBY_PERSONAL_AREA_FREECHARGE);

		pWnd->SendMessage(LMV_BUTTON_ID_FREECHARGE);
		break;
	case ID_LOBBY_AVT_MYINFO:
		#ifdef UPGRADE_10210619_ADD_CLICKLOG
			SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_PERSONAL_AREA_PROFILE_CLICK_MYAVATAR);
		#endif //UPGRADE_10210619_ADD_CLICKLOG

		pWnd->SendMessage(ID_LOBBY_PROCESS_PROFILE, (WPARAM)m_pUserInfo);
		break;
	case ID_LOBBY_BTN_MEMO_COUNT:
		#ifdef UPGRADE_10210619_ADD_CLICKLOG
			SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_PERSONAL_AREA_POKER_MEMO_ICON);
		#endif //UPGRADE_10210619_ADD_CLICKLOG

		{	
			CString strURL;
			CPostBoxDlg dlg(GM().GetMainWnd());
			strURL = getGameURL(IDX_GAMEURL_POSTBOX).GetString();			
			dlg.DoModal(strURL);
		}
		break;
	case ID_LOBBY_BTN_CONNECT_BUDDY_COUNT:
		#ifdef UPGRADE_10210619_ADD_CLICKLOG
			SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_PERSONAL_AREA_POKER_FRIEND_ICON);
		#endif //UPGRADE_10210619_ADD_CLICKLOG

		SetUserListTabType(ESUT_MY_BUDDY);
		break;
	case ID_LOBBY_BTN_EVENT:
		#ifdef UPGRADE_10210619_ADD_CLICKLOG
			SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_USERLIST_EVENT);
		#endif //UPGRADE_10210619_ADD_CLICKLOG
		pWnd->SendMessage(LGT_BUTTON_ID_EVENT);
		break;
	case ID_LOBBY_BTN_MYINFO_CAFE:
		{
			NMBASE::UTIL::CShellMan sm;
			sm.OpenNewBrowser(getGameURL(IDX_GAMEURL_CAFE));
		}
		break;
	case ID_LOBBY_BTN_MYINFO_GOLD_CHANGE:
	//case ID_LOBBY_BTN_GOLD_TEXT:
		{
			//빅휠 삭제 2018.07.12
			//MM().Call(UPG_CMK_SHOW_GOLDBIGWHEEL_DIALOG);
		}
		break;
	case ID_LOBBY_BTN_WITHDRAW:
		{
			MM().Call(UPG_CMK_SHOW_GOLDWITHDRAW_POPUP);
		}
		break;
	case ID_LOBBY_BTN_MYINFO_CLUB:
		MM().Call(UPG_CMK_SHOW_CLUB_DIALOG);
		break;
	case ID_LOBBY_BTN_DAILYGOLD:
		{
			/* 칩 출석부 - 캡챠 적용 안함
			CCaptchaDlg* captchaDlg = NULL;
			MM().Call(UPG_CMK_GET_CAPTCHA_DLG,(WPARAM)0,(LPARAM)&captchaDlg);

			if(captchaDlg->GetSafeHwnd() != NULL)
			{
				return FALSE;
			}

			if(captchaDlg->GetNeedCaptcha())
			{
				if(captchaDlg->DoModal() == IDCANCEL)
				{
					return FALSE;
				}
			}
			*/
			
			// 골드 출석부 홀짝게임 팝업생성
			MM().Call(UPG_CMK_CREATE_GOLDATTENDANCEBOOK_POPUP);			
			MM().Call(UPG_CMK_SHOW_GOLDATTENDANCEBOOK_POPUP);
		}
		break;
	}
	return TRUE;
}

void  CUserInfoGroup::SetMyUserInfo(USERINFO *pMyInfo, BOOL bLogin)
{
	m_pUserInfo = pMyInfo;
	m_pAvatar->SetAvatar(pMyInfo->AvatarInfo);
	m_pNickName->SetText(m_pUserInfo->NickName);
	/*m_pHaveMoney->SetNumber(m_pUserInfo->GetMoney(IsTournament()));*/
	/*m_pHaveMoney->SetNumber(m_pUserInfo->GetMoney());*/
	INT64 userMarkHaveMoney = g_CheckMarkedExceedingLimitHaveMoney(m_pUserInfo->GetRealPMoney(), m_pUserInfo->PremMenuNo);
	m_pHaveMoney->SetNumber(userMarkHaveMoney); // 뉴포터 토너먼트 추가하면서 realmoney  추가.
	/*m_pHaveTotalMoney->SetNumber(m_pUserInfo->GetMoney(IsTournament()));*/
	/*m_pHaveTotalMoney->SetNumber(m_pUserInfo->GetMoney());*/
	m_pHaveTotalMoney->SetNumber(m_pUserInfo->GetRealPMoney());  // 뉴포터 토너먼트 추가하면서 realmoney  추가.
	/*if(bLogin) m_i64LoginMoney = pMyInfo->GetMoney(IsTournament());*/
	/*if(bLogin) m_i64LoginMoney = pMyInfo->GetMoney();*/
	if(bLogin) m_i64LoginMoney = pMyInfo->GetRealPMoney();
	/*m_pChangeMoney->SetNumber(pMyInfo->GetMoney(IsTournament()) - m_i64LoginMoney);*/
	/*m_pChangeMoney->SetNumber(pMyInfo->GetMoney() - m_i64LoginMoney);*/
	m_pChangeMoney->SetNumber(pMyInfo->GetRealPMoney() - m_i64LoginMoney);

	CString strTemp;
	IDX_GAME idxGame = (IDX_GAME)m_pUserInfo->nGameIndex;
	
	UINT winCount = m_pUserInfo->GetMergedAllGameRecords().GetWinNum(idxGame);
	UINT loseCount = m_pUserInfo->GetMergedAllGameRecords().GetLoseNum(idxGame) + m_pUserInfo->GetMergedAllGameRecords().GetFoldNum(idxGame);
	UINT playCount = winCount+loseCount;
	if(playCount==0)
		playCount = 1;
	strTemp.Format("전적: %d승 %d패 (%.2f%%)", winCount, loseCount, ((FLOAT)(winCount*100))/(FLOAT)playCount);

	m_pMyScore->SetText(strTemp.GetString());

	m_pDetailInfo->SetMyUserInfo(pMyInfo, bLogin);
}

void CUserInfoGroup::SetComunityConnect(bool bConnect)
{
	m_pCommunityDisconnect->SetShow(false);
	m_pCommunityDisconnectBuddy->SetShow(false);

	m_bConnect = bConnect;

	if(bConnect)
	{
		//SetMemoCount((int)m_pTxtMemoCount->GetNumber());
		SetConnectBuddyCount((int)m_pTxtConnectBuddyCount->GetNumber());
	}
	else
	{
		
		m_pImgCommunityConnectBuddyCount->SetShow(false);

		//m_pBtnMemoCount->SetOneStatusButton(4);
		m_pBtnConnectBuddyCount->SetOneStatusButton(4);
	}
}

LRESULT CUserInfoGroup::OnMoneyMouseOver(LSY::CMessage *msg)
{
	LSY::CMO_MouseOver *pMsg = (LSY::CMO_MouseOver *)msg;
	switch(pMsg->GetObject()->GetID())
	{
	case ID_LOBBY_IMGNB_MYINFO_MONEY:
		m_pPopup->SetShow(true);
		break;
	//0이상이때 항상 노출하는걸로 수정
	//case ID_LOBBY_BTN_MEMO_COUNT:
	//	if(m_bConnect)
	//	{
	//		if(m_pTxtMemoCount->IsShow()) m_pImgCommunityMemoCount->SetShow(true);
	//	}
	//	else
	//	{
	//		m_pCommunityDisconnect->SetShow(true);
	//	}
	//	break;
	//case ID_LOBBY_BTN_CONNECT_BUDDY_COUNT:
	//	if(m_bConnect)
	//	{
	//		if(m_pTxtConnectBuddyCount->IsShow()) m_pImgCommunityConnectBuddyCount->SetShow(true);
	//	}
	//	else
	//	{
	//		m_pCommunityDisconnectBuddy->SetShow(true);
	//	}
	//	break;
	}
	
	return TRUE;
}

LRESULT CUserInfoGroup::OnMoneyMouseLeave(LSY::CMessage *msg)
{
	LSY::CMO_MouseLeave *pMsg = (LSY::CMO_MouseLeave *)msg;
	switch(pMsg->GetObject()->GetID())
	{
	case ID_LOBBY_IMGNB_MYINFO_MONEY:
		m_pPopup->SetShow(false);
		break;
	//0이상이때 항상 노출하는걸로 수정
	//case ID_LOBBY_BTN_MEMO_COUNT:
	//	m_pCommunityDisconnect->SetShow(false);
	//	m_pImgCommunityMemoCount->SetShow(false);
	//	break;
	//case ID_LOBBY_BTN_CONNECT_BUDDY_COUNT:
	//	m_pCommunityDisconnectBuddy->SetShow(false);
	//	m_pImgCommunityConnectBuddyCount->SetShow(false);
	//	break;
	}

	return TRUE;
}

void CUserInfoGroup::SetMemoCount(int nMemoCount)
{
	if(nMemoCount > 0)
	{
		m_pImgCommunityMemoCount->SetShow(true);
		m_pTxtMemoCount->SetShow(true);
	}
	else
	{
		m_pImgCommunityMemoCount->SetShow(false);
		m_pTxtMemoCount->SetShow(false);
	}
	m_pTxtMemoCount->SetNumber(nMemoCount);
}

void CUserInfoGroup::SetConnectBuddyCount(int nBuddyCount)
{
	if(m_bConnect)
	{
		if(nBuddyCount > 0)
		{
			m_pBtnConnectBuddyCount->SetFourStatusButton(0);
			m_pImgCommunityConnectBuddyCount->SetShow(true);
			m_pTxtConnectBuddyCount->SetShow(true);
		}
		else
		{
			m_pBtnConnectBuddyCount->SetOneStatusButton(3);
			m_pImgCommunityConnectBuddyCount->SetShow(false);
			m_pTxtConnectBuddyCount->SetShow(false);
		}
	}

	m_pTxtConnectBuddyCount->SetNumber(nBuddyCount);
}

void CUserInfoGroup::ShowRoomList(bool bShow)
{
	//2013 토너먼트에서는 쪽지, 친구, 무료충전은 사용가능하기 때문에 주석.
	//m_pBtnCharge->SetEnable(bShow);
	//m_pBtnMemoCount->SetEnable(bShow);
	//m_pBtnConnectBuddyCount->SetEnable(bShow);
}

void CUserInfoGroup::SetEnableBtnDetailOpen(bool bEnable)
{
	m_pBtnDetailOpen->SetEnable(bEnable);
}

LRESULT CUserInfoGroup::StartTimer( WPARAM &wParam, LPARAM &lParam )
{
	KillTimer( GetLobbyWindow()->GetSafeHwnd(), _DEF_MEMO_RESET_TIMER );
	GetMemoCnt();
	SetTimer( GetLobbyWindow()->GetSafeHwnd(), _DEF_MEMO_RESET_TIMER, 60000*5, NULL );
	return TRUE;
}

void CUserInfoGroup::GetMemoCnt()
{
	char szTemp[256] = {0, };
	CString strNickName, strURL;
	char strEn[256] = {0,};
	strURL = getGameURL(IDX_GAMEURL_POSTBOX_URL);
	sprintf_s( szTemp, 256, "%s%s", GM().GetMyInfo()->UI.UniqNo, "test123" );
	Md5(szTemp, strEn);
	
	strNickName = ANSIToUTF8( GM().GetMyInfo()->UI.NickName );
	strNickName = URLencode( (char*)strNickName.GetString() );

	CString strRet ="";
	strRet.Format( "/pcgostop/message/client/msg/count?token=%s&iNickname=%s&iId=%s&iCn=%s&gameType=42&subGameType=%d",strEn,strNickName,GM().GetMyInfo()->UI.ID,GM().GetMyInfo()->UI.UniqNo, g_GetGameCode(GM().GetCurrentGameType()) );

	bool bHttps = false;
#if defined(_DEBUG)
	bHttps = false;
#else
	if(g_strConnectecMasterServerName == "183.110.61.192 12000")
	{
		bHttps = false;
	}
	else
	{
		bHttps = true;
	}
#endif

	GM().m_cWebCallThread.CreateThread(strURL.GetString(), 12002, strRet.GetString(),"", TRUE, bHttps );
	GM().m_cWebCallThread.m_bMemo = true;
}

LRESULT  CUserInfoGroup::OnEndCall(WPARAM &wParam, LPARAM &lParam)
{
	if( m_bRefresh && 
		GM().m_cWebCallThread.m_MemoData.m_nTotalCount > GM().m_cWebCallThread.m_BackupMemoData.m_nTotalCount )
	{
		CString strTemp;
		strTemp.Format("게임방에 있을 때 %ld개의 쪽지를 수신하였습니다. 쪽지함에서 확인하시기 바랍니다.", GM().m_cWebCallThread.m_MemoData.m_nTotalCount - GM().m_cWebCallThread.m_BackupMemoData.m_nTotalCount );
		//ShowMainMessageDlg(strTemp);
		m_bRefresh = false;
	}	

	SetMemoCount( GM().m_cWebCallThread.m_MemoData.m_nTotalCount );

	return TRUE;
}

LRESULT CUserInfoGroup::OnShowSendMessageDlg(WPARAM &wParam, LPARAM &lParam)
{
	CString strURL;

	bool bGroup = (lParam!=0);
	CPostBoxDlg dlg(GM().GetMainWnd());
	strURL = getGameURL(IDX_GAMEURL_POSTBOX_SEND_MESSAGE).GetString();
	dlg.DoModalSend(strURL, (char*)wParam, bGroup);
	return TRUE;
}

LRESULT CUserInfoGroup::OnSetMoneyNum( WPARAM &wParam, LPARAM &lParam )
{
	if(m_pUserInfo!=NULL)
	{
		INT64 userMarkHaveMoney = g_CheckMarkedExceedingLimitHaveMoney(m_pUserInfo->GetRealPMoney(), m_pUserInfo->PremMenuNo);

		if(m_pHaveMoney!=NULL)		m_pHaveMoney->SetNumber(userMarkHaveMoney);
		if(m_pHaveTotalMoney!=NULL)	m_pHaveTotalMoney->SetNumber(m_pUserInfo->GetRealPMoney());
		if(m_pHaveGold!=NULL)		m_pHaveGold->SetNumber(GM().GetMyGold());
	}
	return TRUE;
}

LRESULT CUserInfoGroup::OnSetLobbyButtonEnable( WPARAM &wParam, LPARAM &lParam )
{
	m_pDailyGold->SetEnable((bool)wParam);
	m_pDailyGoldAlarm->SetShow((bool)wParam);	
	
	return TRUE;
}