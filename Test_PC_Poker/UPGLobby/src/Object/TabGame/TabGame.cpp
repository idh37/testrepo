
#include "StdAfx.h"
#include "TabGame.h"
#include "../../WidgetID.h"

CTabGame::CTabGame()
:	m_pBtnPokerShop(NULL),
	m_pBtnCreateRoom(NULL),
	m_pBtnShocking8(NULL),
	m_bAdmin(NULL),
	m_nCurrentTab(IDX_GAME_NULL),
	m_pTxtBtnChance(NULL),	
	//규제안 : 이용제한 안내 버튼
	m_pBtnUseLimit(NULL),
	m_pUseLimit(NULL)
{
	MM().AppendMsgH(UPG_CMK_SHOW_CHUSEOK_ATTENDANCE_BTN, Fnt(this, &CTabGame::OnShowChuseokAttendanceBtn));
}

CTabGame::~CTabGame()
{
}

void CTabGame::OnCreateChild(LSY::CObjectBase *pObject)
{
	LSY::CObjectProject *pProject = GetObjectMan()->GetProjectFromID(ID_LOBBY);
	
	if(pObject == NULL)
		return;

	bool bAddChild = false;

	switch(pObject->GetID())
	{
	case ID_LOBBY_BTN_CREATE_ROOM:
		m_pBtnCreateRoom = (LSY::CButton *)pObject;
		bAddChild = true;
		break;
	case ID_LOBBY_BTN_POKER_SHOP:
		m_pBtnPokerShop	= (LSY::CButton *)pObject;
		bAddChild = true;
		break;
	case ID_LOBBY_BTN_SHOCKING_8:
		m_pBtnShocking8 = (LSY::CButton *)pObject;
		bAddChild = true;
		break;
	case ID_LOBBY_BTN_AUTOSTART_7POKER: 
	case ID_LOBBY_BTN_AUTOSTART_LOWBD: 
	case ID_LOBBY_BTN_AUTOSTART_NEWPOKER:
	case ID_LOBBY_BTN_AUTOSTART_HIGHLOW:
	case ID_LOBBY_BTN_AUTOSTART_SUTDA:
	case ID_LOBBY_BTN_SELECTSTART_7POKER: 
	case ID_LOBBY_BTN_SELECTSTART_LOWBD:
	case ID_LOBBY_BTN_SELECTSTART_NEWPOKER:
	case ID_LOBBY_BTN_SELECTSTART_HIGHLOW:
	case ID_LOBBY_BTN_SELECTSTART_SUTDA:
	case ID_LOBBY_BTN_SELECTSTART_HOOLA:
	case ID_LOBBY_BTN_AUTOSTART_CONFIG_7POKER:
	case ID_LOBBY_BTN_AUTOSTART_CONFIG_LOWBD:
	case ID_LOBBY_BTN_AUTOSTART_CONFIG_NEWPOKER:
	case ID_LOBBY_BTN_AUTOSTART_CONFIG_HIGHLOW:
	case ID_LOBBY_BTN_AUTOSTART_CONFIG_SUTDA:
	case ID_LOBBY_BTN_AUTOSTART_CONFIG_HOOLA:
	case ID_LOBBY_BTN_AUTOSTART_HOOLA_NORMAL:
	case ID_LOBBY_BTN_AUTOSTART_HOOLA_JOKER:
		bAddChild = true;
		break;
	case ID_LOBBY_BUTTON_MADE:
		{
			m_pBtnMade = (CChanceButton *)pObject;
			bAddChild = true;
			m_pBtnMade->OnCreateChild(pProject->GetObject(ID_LOBBY_TEXT_MADE));
			//pObject->AddHandler(LSY::EM_O_MOUSEOVER, LSY::Fnt(this,&CTabGame::OnMouseOver));
			//pObject->AddHandler(LSY::EM_O_MOUSELEAVE, LSY::Fnt(this,&CTabGame::OnMouseLeave));
		}
		break;
	case ID_LOBBY_BUTTON_REGEN:
		{
			m_pBtnRegen = (CChanceButton *)pObject;
			bAddChild = true; 
			m_pBtnRegen->OnCreateChild(pProject->GetObject(ID_LOBBY_TEXT_REGEN));
			//pObject->AddHandler(LSY::EM_O_MOUSEOVER, LSY::Fnt(this,&CTabGame::OnMouseOver));
			//pObject->AddHandler(LSY::EM_O_MOUSELEAVE, LSY::Fnt(this,&CTabGame::OnMouseLeave));
		}
		break;
		/*
	case ID_LOBBY_BTN_CHANCE:
		{
			m_pBtnDefaultChance		= (CChanceButton *)pObject;	bAddChild = true; 
			m_pBtnDefaultChance->OnCreateChild(pProject->GetObject(ID_LOBBY_TXTBTN_CHANCE));
		}
		break;
	case ID_LOBBY_BTN_VIP_CHANCE:
		{
			m_pBtnVIPChance		= (CChanceButton *)pObject;	bAddChild = true;
			m_pBtnVIPChance->OnCreateChild(pProject->GetObject(ID_LOBBY_TXTBTN_VIP_CHANCE));
			pObject->AddHandler(LSY::EM_O_MOUSEOVER, LSY::Fnt(this,&CTabGame::OnMouseOver));
			pObject->AddHandler(LSY::EM_O_MOUSELEAVE, LSY::Fnt(this,&CTabGame::OnMouseLeave));
		}
		break;
	case ID_LOBBY_BTN_GISACHANCE:
		{
			m_pBtnGisaChance		= (CChanceButton *)pObject;	bAddChild = true;
			m_pBtnGisaChance->OnCreateChild(pProject->GetObject(ID_LOBBY_TXTBTN_GISACHANCE));
			pObject->AddHandler(LSY::EM_O_MOUSEOVER, LSY::Fnt(this,&CTabGame::OnMouseOver));
			pObject->AddHandler(LSY::EM_O_MOUSELEAVE, LSY::Fnt(this,&CTabGame::OnMouseLeave));
		}
		break;
	case ID_LOBBY_BTN_VIP_GISACHANCE:
		{
			m_pBtnVIPGisaChance		= (CChanceButton *)pObject;	bAddChild = true;
			m_pBtnVIPGisaChance->OnCreateChild(pProject->GetObject(ID_LOBBY_TXTBTN_VIP_GISACHANCE));
			pObject->AddHandler(LSY::EM_O_MOUSEOVER, LSY::Fnt(this,&CTabGame::OnMouseOver));
			pObject->AddHandler(LSY::EM_O_MOUSELEAVE, LSY::Fnt(this,&CTabGame::OnMouseLeave));
		}
		break;
		*/
	//규제안 : 입장제한 안내 버튼
	case ID_LOBBY_BTN_USELIMIT:
		m_pBtnUseLimit = (LSY::CButton *)pObject;
		bAddChild = true;
		break;
	case ID_LOBBY_IMG_USELIMIT:
		m_pUseLimit = (LSY::CImage *)pObject;
		bAddChild = true;
		break;
	// 황금 복주머니 이벤트 버튼
	case ID_LOBBY_BUTTON_GOLDBAG:
		m_pBtnGoldBack = (LSY::CButton *)pObject;
		bAddChild = true;
		break;
	case ID_LOBBY_BUTTON_CHUSEOK_ATTENDANCE:
		m_pChuSeokAttendance = (LSY::CButton *)pObject;
		bAddChild = true;
		break;
	case ID_LOBBY_BTN_FREE_CHARGE:
		bAddChild = true;
		break;

	}
	
	if(bAddChild)
	{
		pObject->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CTabGame::OnClickButton));
	}
	
	__super::OnCreateChild(pObject);
}

void CTabGame::OnCreated(void)
{
	//yoo tab 버튼 child만 호출
	LSY::CObjectProject *pProject = GetObjectMan()->GetProjectFromID(ID_LOBBY);
	OnCreateChild(pProject->GetObject(ID_LOBBY_BTN_AUTOSTART_7POKER));
	OnCreateChild(pProject->GetObject(ID_LOBBY_BTN_AUTOSTART_LOWBD));
	OnCreateChild(pProject->GetObject(ID_LOBBY_BTN_AUTOSTART_NEWPOKER));
	OnCreateChild(pProject->GetObject(ID_LOBBY_BTN_AUTOSTART_HIGHLOW));
	OnCreateChild(pProject->GetObject(ID_LOBBY_BTN_AUTOSTART_SUTDA));
	OnCreateChild(pProject->GetObject(ID_LOBBY_BTN_AUTOSTART_HOOLA_NORMAL));
	OnCreateChild(pProject->GetObject(ID_LOBBY_BTN_AUTOSTART_HOOLA_JOKER));
	OnCreateChild(pProject->GetObject(ID_LOBBY_BTN_SELECTSTART_7POKER));
	OnCreateChild(pProject->GetObject(ID_LOBBY_BTN_SELECTSTART_LOWBD));
	OnCreateChild(pProject->GetObject(ID_LOBBY_BTN_SELECTSTART_NEWPOKER));
	OnCreateChild(pProject->GetObject(ID_LOBBY_BTN_SELECTSTART_HIGHLOW));
	OnCreateChild(pProject->GetObject(ID_LOBBY_BTN_SELECTSTART_SUTDA));
	OnCreateChild(pProject->GetObject(ID_LOBBY_BTN_SELECTSTART_HOOLA));
	OnCreateChild(pProject->GetObject(ID_LOBBY_BTN_AUTOSTART_CONFIG_7POKER));
	OnCreateChild(pProject->GetObject(ID_LOBBY_BTN_AUTOSTART_CONFIG_LOWBD));
	OnCreateChild(pProject->GetObject(ID_LOBBY_BTN_AUTOSTART_CONFIG_NEWPOKER));
	OnCreateChild(pProject->GetObject(ID_LOBBY_BTN_AUTOSTART_CONFIG_HIGHLOW));
	OnCreateChild(pProject->GetObject(ID_LOBBY_BTN_AUTOSTART_CONFIG_SUTDA));
	OnCreateChild(pProject->GetObject(ID_LOBBY_BTN_AUTOSTART_CONFIG_HOOLA));

	LSY::CRadio *pRadio = NULL;
	for(int i = (int)IDX_GAME_SP; i < (int)MAX_GAME; ++i)
	{
		pRadio = (LSY::CRadio *)GetFirstChildFromID(i);
		if (pRadio == NULL)
			continue;

		m_listGameTab[(IDX_GAME)i] = pRadio;
		m_listGameMaintenance[(IDX_GAME)i] = false;

		m_listGameTabStatusIndex[(IDX_GAME)i].resize(LSY::EBS_MAX);
		for(int j=LSY::EBS_NORMAL;j<LSY::EBS_MAX;++j)
		{
			m_listGameTabStatusIndex[(IDX_GAME)i][j] = pRadio->GetButtonStatusIndex((LSY::ENUM_BUTTON_STATUS)j);
		}
	}

	AddHandler(LSY::EM_O_SELCHANGE, LSY::Fnt(this, &CTabGame::OnSelChangeGame));

	m_pBtnMade->SetEnable(false);
	m_pBtnRegen->SetEnable(false);

	m_pBtnPokerShop->SetShow(true);

	//규제안 : 입장제한 안내 버튼
	m_pBtnUseLimit->SetShow(false);

	m_pBtnGoldBack->SetShow(false);
	
	__super::OnCreated();
	
}
LRESULT CTabGame::OnMouseOver(LSY::CMessage *msg)
{
	LSY::CObjectProject *pProject = GetObjectMan()->GetProjectFromID(ID_LOBBY);

	CChanceTooltips	*pChanceTooltips = (CChanceTooltips*) pProject->GetObject(ID_LOBBY_GRP_CHANCE_TOOLTIPS);
	if (pChanceTooltips)
	{				
		LSY::CMO_MouseOver *pMsg = (LSY::CMO_MouseOver *)msg;
		CChanceButton* pButton = (CChanceButton*)pMsg->GetObject() ;
		
		/*pChanceTooltips->ShowToolTip( GM().GetCurrentGameCode() ,m_pBtnVIPChance->GetRewartItemCode());*/
		if (pButton)
		{	
			pChanceTooltips->ShowToolTip( GM().GetCurrentGameCode() ,pButton->GetRewartItemCode(), pButton->GetRewartItemType(), pButton->GetPos());
		}
	}

	return TRUE;
}
LRESULT CTabGame::OnMouseLeave(LSY::CMessage *msg)
{
	LSY::CObjectProject *pProject = GetObjectMan()->GetProjectFromID(ID_LOBBY);
	CChanceTooltips	*pChanceTooltips = (CChanceTooltips*) pProject->GetObject(ID_LOBBY_GRP_CHANCE_TOOLTIPS);
	if (pChanceTooltips)
	{		
		pChanceTooltips->SetShow(false);
	}

	return TRUE;
}


LRESULT CTabGame::OnClickButton(LSY::CMessage *msg)
{
	LSY::CMO_MouseLClick *pMsg = (LSY::CMO_MouseLClick *)msg;
	LSY::CButton *pBtn = (LSY::CButton *)pMsg->GetObject();

	switch(pBtn->GetID())
	{
	case ID_LOBBY_BTN_AUTOSTART_7POKER: 
	case ID_LOBBY_BTN_AUTOSTART_LOWBD: 
	case ID_LOBBY_BTN_AUTOSTART_NEWPOKER:
	case ID_LOBBY_BTN_AUTOSTART_SUTDA:
		NMBASE::SKINGDI::NMRESAM.PlayWav( "UI-SND_BUTTON_CLICK_B" );
		break;
	case ID_LOBBY_BTN_CREATE_ROOM:
	case ID_LOBBY_BTN_POKER_SHOP:
	case ID_LOBBY_BTN_SHOCKING_8:	
	case ID_LOBBY_BTN_USELIMIT: //규제안 안내 버튼
	case ID_LOBBY_BUTTON_GOLDBAG: // 황금 복주머니 이벤트 버튼
	case ID_LOBBY_BTN_FREE_CHARGE: // 무료 충전소 버튼
		NMBASE::SKINGDI::NMRESAM.PlayWav( "UI-SND_BUTTON_CLICK_A" );
		break;
	}

	CWnd *pWnd = GetObjectMan()->GetWnd();
	switch(pBtn->GetID())
	{
	case ID_LOBBY_BTN_CREATE_ROOM:
		//ShowUseLimitDlg();
		pWnd->SendMessage(LGT_BUTTON_ID_CREATEROOM);
		break;
	case ID_LOBBY_BTN_SHOCKING_8:
		{
			// 여기서 해당 웹페이지 호출.
			MM().Call(UPG_CMK_TIMELIMITITEM_DLG_SHOW);
		}
		break ;
	case ID_LOBBY_BTN_POKER_SHOP:
		#ifdef UPGRADE_10210619_ADD_CLICKLOG
			SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_USERLIST_POKERSHOP);
		#endif //UPGRADE_10210619_ADD_CLICKLOG
		pWnd->SendMessage(LGT_BUTTON_ID_SHOP);
		break;
	case ID_LOBBY_BTN_AUTOSTART_7POKER: 
	case ID_LOBBY_BTN_AUTOSTART_LOWBD: 
	case ID_LOBBY_BTN_AUTOSTART_NEWPOKER:
	case ID_LOBBY_BTN_AUTOSTART_HIGHLOW: 
	case ID_LOBBY_BTN_AUTOSTART_SUTDA:
		pWnd->SendMessage(LGT_BUTTON_ID_PREMQUICKJOIN);
		break;
	case ID_LOBBY_BTN_AUTOSTART_HOOLA_NORMAL:
		pWnd->SendMessage(LGT_BUTTON_ID_AUTOENTERROOM_HA_NORMAL);
		break;
	case ID_LOBBY_BTN_AUTOSTART_HOOLA_JOKER:
		pWnd->SendMessage(LGT_BUTTON_ID_AUTOENTERROOM_HA_JOKER);
		break;
	case ID_LOBBY_BTN_SELECTSTART_7POKER: 
	case ID_LOBBY_BTN_SELECTSTART_LOWBD:
	case ID_LOBBY_BTN_SELECTSTART_NEWPOKER:
	case ID_LOBBY_BTN_SELECTSTART_HIGHLOW:
	case ID_LOBBY_BTN_SELECTSTART_SUTDA:
	case ID_LOBBY_BTN_SELECTSTART_HOOLA:
		pWnd->SendMessage(LGT_BUTTON_ID_SELECTSTART);
		break;
	case ID_LOBBY_BTN_AUTOSTART_CONFIG_7POKER:
	case ID_LOBBY_BTN_AUTOSTART_CONFIG_LOWBD:
	case ID_LOBBY_BTN_AUTOSTART_CONFIG_NEWPOKER:
	case ID_LOBBY_BTN_AUTOSTART_CONFIG_HIGHLOW:
	case ID_LOBBY_BTN_AUTOSTART_CONFIG_SUTDA:
	case ID_LOBBY_BTN_AUTOSTART_CONFIG_HOOLA:
		#ifdef UPGRADE_10210619_ADD_CLICKLOG
			SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_ENTERBTN_AUTOENTER_OPTION);
		#endif //UPGRADE_10210619_ADD_CLICKLOG

		pWnd->SendMessage(LGT_BUTTON_ID_PREMQUICKJOIN_CONFIG);
		break;
	case ID_LOBBY_BUTTON_MADE:
		pWnd->SendMessage(LGT_BUTTON_ID_BUYCHANCEITEM, (WPARAM)m_pBtnMade->GetRewartItemCode());
		break;
	case ID_LOBBY_BUTTON_REGEN:
		pWnd->SendMessage(LGT_BUTTON_ID_BUYCHANCEITEM, (WPARAM)m_pBtnRegen->GetRewartItemCode());
		break;
		//규제안 : 입장제한 안내 버튼 클릭 -> 팝업오픈
	case ID_LOBBY_BTN_USELIMIT:
		{
			ShowUseLimitDlg();
		}
		break;
	// 황금 복주머니 이벤트 버튼
	case ID_LOBBY_BUTTON_GOLDBAG:
		MM().Call(UPG_CMK_SHOW_GOLDLUCKYBAG_POPUP);
		break;
	case ID_LOBBY_BUTTON_CHUSEOK_ATTENDANCE:
		MM().Call(UPG_CMK_SHOW_CHUSEOK_ATTENDANCE_DLG, (WPARAM)0, (LPARAM)0);
		break;		
	case ID_LOBBY_BTN_FREE_CHARGE:
		{
			// 무료 충전소 호출
			NMBASE::UTIL::CShellMan sm;
			sm.OpenNewBrowser(getGameURL(IDX_GAMEURL_FREECHARGE_SHOP));
		}
		break;

	}
	
	return TRUE;
}

void CTabGame::SetMaintenance(IDX_GAME nGame, bool bMaintenance, bool bAdmin)
{
	if(nGame == IDX_GAME_DP) return;	// 맞포커 제거로 맞포커 서버 점검 설정시 무시
	if(m_listGameMaintenance[nGame] == bMaintenance) return;
	m_listGameMaintenance[nGame] = bMaintenance;

	LSY::CRadio *pRadio = (LSY::CRadio *)GetFirstChildFromID((int)nGame);
	std::vector<int> &listStatusIndex = m_listGameTabStatusIndex[nGame];
	int listTempStatusIndex[LSY::EBS_MAX] = {-1,};

	if(bMaintenance)
	{
		for(int i=LSY::EBS_NORMAL;i<LSY::EBS_MAX;++i)
		{
			listTempStatusIndex[i] = listStatusIndex[LSY::EBS_DISABLE];
		}
	}
	else
	{
		for(int i=LSY::EBS_NORMAL;i<LSY::EBS_MAX;++i)
		{
			listTempStatusIndex[i] = listStatusIndex[i];
		}
	}

	pRadio->SetFreeStatusButton(listTempStatusIndex);
	m_bAdmin = bAdmin;
}

bool CTabGame::IsMaintenance(IDX_GAME nGame)
{
	return m_listGameMaintenance[nGame];
}


void CTabGame::SetChanceInfo(CRewardItemMan::REWARD_TYPE eRewardType, int nRewardItemCode, COleDateTime &timeExpire )
{
	if (eRewardType == CRewardItemMan::REWARD_TYPE_MADE)
	{
		m_pBtnMade->SetChanceInfo( eRewardType, nRewardItemCode, timeExpire );
	}
	else if (eRewardType == CRewardItemMan::REWARD_TYPE_REGEN)
	{
		m_pBtnRegen->SetChanceInfo( eRewardType, nRewardItemCode, timeExpire );
	}
}

void CTabGame::SetGameTab(IDX_GAME nGame)
{
	MM().Call(UPG_CMK_SHOW_CHUSEOK_ATTENDANCE_BTN, FALSE);

	if(m_nCurrentTab == nGame) return;

	//규제안 : 안내 버튼 - 게임별 속성이 달라져야 한다.
	m_nCurrentTab = nGame;
	m_listGameTab[nGame]->SetCheck(true);

	ChangeLimitGameTab();
}

bool CTabGame::ChangeLimitGameTab()
{
	int nGameType=GM().GetIDXToGameArray(m_nCurrentTab);
	int btnIndex = (int)m_nCurrentTab;
	if ( btnIndex >= IDX_GAME_HL ) btnIndex = (int)m_nCurrentTab - 1;

	//m_pBtnUseLimit->SetShow(true);
	m_pBtnUseLimit->SetFourStatusButton(btnIndex * 4);

	bool bReturn=GM().IsGetLimitMoney(nGameType) || GM().IsLimitOver();
	m_pBtnUseLimit->SetShow(bReturn);
	ChangeLimitGameBtns(bReturn);
	return bReturn;
}

void CTabGame::ChangeLimitGameBtns(bool bState)
{
	int nGameType=GM().GetIDXToGameArray(m_nCurrentTab);
	LSY::CObjectProject *pProject = GetObjectMan()->GetProjectFromID(ID_LOBBY);
	LSY::CButton *pButton=NULL;
	int nArray[4][6]={{ID_LOBBY_BTN_AUTOSTART_7POKER, ID_LOBBY_BTN_AUTOSTART_LOWBD, ID_LOBBY_BTN_AUTOSTART_NEWPOKER, ID_LOBBY_BTN_AUTOSTART_HIGHLOW, ID_LOBBY_BTN_AUTOSTART_SUTDA, ID_LOBBY_BTN_AUTOSTART_HOOLA_NORMAL},
		{-1, -1, -1, -1, -1, ID_LOBBY_BTN_AUTOSTART_HOOLA_JOKER},
		{ID_LOBBY_BTN_SELECTSTART_7POKER, ID_LOBBY_BTN_SELECTSTART_LOWBD, ID_LOBBY_BTN_SELECTSTART_NEWPOKER, ID_LOBBY_BTN_SELECTSTART_HIGHLOW, ID_LOBBY_BTN_SELECTSTART_SUTDA, ID_LOBBY_BTN_SELECTSTART_HOOLA},
		{ID_LOBBY_BTN_AUTOSTART_CONFIG_7POKER, ID_LOBBY_BTN_AUTOSTART_CONFIG_LOWBD, ID_LOBBY_BTN_AUTOSTART_CONFIG_NEWPOKER, ID_LOBBY_BTN_AUTOSTART_CONFIG_HIGHLOW, ID_LOBBY_BTN_AUTOSTART_CONFIG_SUTDA, ID_LOBBY_BTN_AUTOSTART_CONFIG_HOOLA}
	};
	if(true==bState){
		for(int i=0; i<4; i++){
			if(-1==nArray[i][nGameType]) continue;
			pButton=(LSY::CButton *)pProject->GetObject(nArray[i][nGameType]);
			pButton->SetEnable(false);
		}
		m_pBtnCreateRoom->SetEnable(false);
	}
	else{
		for(int i=0; i<4; i++){
			if(-1==nArray[i][nGameType]) continue;
			pButton=(LSY::CButton *)pProject->GetObject(nArray[i][nGameType]);
			pButton->SetEnable(true);
		}
		m_pBtnCreateRoom->SetEnable(true);
	}
}

LRESULT CTabGame::OnSelChangeGame(LSY::CMessage *msg)
{
	NMBASE::SKINGDI::NMRESAM.PlayWav( "UI-SND_BUTTON_CLICK_A" );

	LSY::CMO_SelChange *pMsg = (LSY::CMO_SelChange *)msg;
	int nIndex = pMsg->GetValue();
	LSY::CRadio *pRadio = GetRadioFromIndex(nIndex);
	IDX_GAME nGame = (IDX_GAME)pRadio->GetID();


	if(IsMaintenance(nGame) && !m_bAdmin)
	{
		m_listGameTab[m_nCurrentTab]->SetCheck(true);
		return TRUE;
	}

	IDX_GAME nBackupGame = m_nCurrentTab;
	m_nCurrentTab = nGame;

	CWnd *pWnd = GetObjectMan()->GetWnd();
	pWnd->SendMessage(ID_LOBBY_PROCESS_CHANGEGAME, (WPARAM)nGame, (LPARAM)nBackupGame);

	//규제안 : 안내 버튼 - 게임별 속성이 달라져야 한다.
	m_nCurrentTab = nGame;
	m_listGameTab[nGame]->SetCheck(true);

	ChangeLimitGameTab();

	return TRUE;
}

void CTabGame::ClearChance(CRewardItemMan::REWARD_TYPE eRewardType)
{
	if (eRewardType == CRewardItemMan::REWARD_TYPE_MADE)
	{
		m_pBtnMade->ClearChance();
	
	}else if (eRewardType == CRewardItemMan::REWARD_TYPE_REGEN)
	{
		m_pBtnRegen->ClearChance();

	}
}


//규제안 : 이용제한 안내 버튼 활성화 설정
void CTabGame::SetShowUseLimitBtn(bool bShow)
{	
	m_pBtnUseLimit->SetShow(bShow);
}

//규제안 안내 팝업 오픈
void CTabGame::ShowUseLimitDlg()
{
	if(true==GM().IsLimitOver()){
		CRestrictionOverPopup dlg;
		dlg.DoModal(GM().GetMainWnd());
	}
	else{
		CRestrictionPopup dlg;
		dlg.DoModal(GM().GetMainWnd(), m_nCurrentTab);
	}

}

//2018 설이벤트 버튼 노출
void CTabGame::SetShowGoldBackBtn(bool bShow)
{
	if(NULL != m_pBtnGoldBack)
	{
		m_pBtnGoldBack->SetShow(bShow);
		m_pBtnGoldBack->SetEnable(bShow);
	}
}

void CTabGame::ShowUseLimitOverDlg()
{
	CRestrictionPopup dlg;
	dlg.DoModal(GM().GetMainWnd(), m_nCurrentTab);
}

LRESULT CTabGame::OnShowChuseokAttendanceBtn(WPARAM &wParam, LPARAM &lParam)
{
	if(m_pChuSeokAttendance)
	{
		bool enable = false;

		if(wParam)
		{
			enable = true;
		}

		m_pChuSeokAttendance->SetShow(enable);
		m_pChuSeokAttendance->SetEnable(enable);

		return TRUE;
	}

	return FALSE;
}