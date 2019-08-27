// ProfileDlg.cpp : implementation file
//
#include "stdafx.h"
#include "RestrictionPopup.h"
#include "GlobalBase.h"
#include "restrictionpopupid.h"


CRestrictionPopup::CRestrictionPopup(CWnd* pParent /*=NULL*/)
	: LSY::CLSYLibDialogBase(pParent)
{
	for(int i=0; i<5; i++){
		m_pTxt_Game[i]=NULL;
		m_pTxt_Money[i]=NULL;
		m_pImg_State[i]=NULL;
	}
}

BEGIN_MESSAGE_MAP(CRestrictionPopup, LSY::CLSYLibDialogBase)
	ON_COMMAND(IDCANCEL, OnCancel)
END_MESSAGE_MAP()

bool CRestrictionPopup::OnCreatedProject(void)
{
	LSY::CObjectProject *pProject = m_pObjectMan->GetProjectFromID(ID_RESTRICTIONPOPUP);
	m_pWndBack = (LSY::CWindows *)pProject->GetMainObject();
#if defined(RESTRICTION_LEVEL)
	m_pWndBack->SetIndex(0);
#else
	m_pWndBack->SetIndex(0);
#endif

	m_pBtnOK = (LSY::CButton *)GetObject(ID_RESTRICTIONPOPUP_BTN_OK);
	m_pBtnOK->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CRestrictionPopup::OnClickButton));
	m_pBtnPlay = (LSY::CButton *)GetObject(ID_RESTRICTIONPOPUP_BTN_PLAY);
	m_pBtnPlay->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CRestrictionPopup::OnClickButton));

	LSY::CButton *pButton = (LSY::CButton *)GetObject(ID_RESTRICTIONPOPUP_BTN_CLOSE);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CRestrictionPopup::OnClickButton));

	m_pImgGameName = (LSY::CImage *)GetObject(ID_RESTRICTIONPOPUP_IMG_GAMENAME);
	m_pImgGameName->AddHandler(LSY::EM_O_MOUSEOVER, LSY::Fnt(this, &CRestrictionPopup::OnMouseOver)) ;
	m_pImgGameName->AddHandler(LSY::EM_O_MOUSELEAVE, LSY::Fnt(this, &CRestrictionPopup::OnMouseLeave)) ;
	m_pImgExplainlossmoney = (LSY::CImage *)GetObject(ID_RESTRICTIONPOPUP_IMG_EXPLAIN_LOSSMONEY);

	m_pTxtLossMoney = (LSY::CText *)GetObject(ID_RESTRICTIONPOPUP_TXT_LOSSMONEY);

	int nGameID[5]={ID_RESTRICTIONPOPUP_TXT_GAME1, ID_RESTRICTIONPOPUP_TXT_GAME2, ID_RESTRICTIONPOPUP_TXT_GAME3, 
		ID_RESTRICTIONPOPUP_TXT_GAME4, ID_RESTRICTIONPOPUP_TXT_GAME5};
	int nMoneyID[5]={ID_RESTRICTIONPOPUP_TXT_GAMEMONEY1, ID_RESTRICTIONPOPUP_TXT_GAMEMONEY2, ID_RESTRICTIONPOPUP_TXT_GAMEMONEY3, 
		ID_RESTRICTIONPOPUP_TXT_GAMEMONEY4, ID_RESTRICTIONPOPUP_TXT_GAMEMONEY5};
	int nStateID[5]={ID_RESTRICTIONPOPUP_IMG_GAMESTATE1, ID_RESTRICTIONPOPUP_IMG_GAMESTATE2, ID_RESTRICTIONPOPUP_IMG_GAMESTATE3, 
		ID_RESTRICTIONPOPUP_IMG_GAMESTATE4, ID_RESTRICTIONPOPUP_IMG_GAMESTATE5};
	for(int i=0; i<5; i++){
		m_pTxt_Game[i]=(LSY::CText *)GetObject(nGameID[i]);
		m_pTxt_Money[i]=(LSY::CText *)GetObject(nMoneyID[i]);
		m_pImg_State[i]=(LSY::CImage *)GetObject(nStateID[i]);
	}

	return true;
}

BOOL CRestrictionPopup::OnInitDialog() 
{
	CLSYLibDialogBase::OnInitDialog();
	// TODO: Add extra initialization here

	DAILYLOSS_STATE eState = GM().GetLossMoneyState() ;
	INT64 nLossmoney = GM().GetLossMoney() ;
	time_t	blockTime = GM().GetBlockTime() ;

	int nGameIndex[7] = {1, 4, 5, -1, 2, 6, 3 } ;
	int nGameType = -1;
	/*
		IDX_GAME_SP=0,
		IDX_GAME_BD 1,
		IDX_GAME_NP 2, 
		IDX_GAME_DP 3, -
		IDX_GAME_HL 4,
		IDX_GAME_SD 5,
		IDX_GAME_HA 6,
		*/
	nGameType=GM().GetIDXToGameArray(m_nCurrentGame);
	int nCur=0;
	string sGameName[6]={"7포커", "로우바둑이", "뉴포커", "하이로우", "섯다", "훌라"};
	
	long long llLimit=0ll;
	for(int i=0; i<6; i++){
		llLimit=((0ll<GM().GetLimitRestMoney(i))?(0ll):(-GM().GetLimitRestMoney(i)));
		if((i)==nGameType){
			m_pImgGameName->SetIndex(1+nGameType) ;
			m_pTxtLossMoney->SetText(g_NumberToOrientalString( llLimit, 3).GetString()) ;
			m_pBtnPlay->SetFourStatusButton(32+i*8);
			m_pBtnOK->SetFourStatusButton(36+i*8);
		}
		else{			
			m_pTxt_Game[nCur]->SetText(sGameName[i]);
			m_pTxt_Money[nCur]->SetText(g_NumberToOrientalString( llLimit, 3).GetString());

			m_pImg_State[nCur]->SetIndex(18+GM().GetLevelLimitMoney(i));
			nCur++;
		}
	}
	
	//	////프로그래세브바 90%이상으로 설정(528(70%)->588(100%))
	//	//if (tmp < 70 )
	//	//{
	//	//	tmp = 70;
	//	//} 
	//	//else if (tmp > 100)
	//	//{
	//	//	tmp = 100;
	//	//}
	//	//int tmpX = tmp - 70;
	//	//m_pImgGauge->SetPos(527 + (tmpX * 2), 187);
	//	//프로그래세브바 90%이상으로 설정(528(70%)->588(100%))
	//	m_pImgGauge->SetShow(true) ;
	//	if (tmp < 70 )
	//	{
	//		tmp = 70;
	//	} 
	//	else if (tmp > 100)
	//	{
	//		tmp = 100;
	//	}
	//	int tmpX = tmp - 70;
	//	m_pImgGauge->SetPos(286 + (tmpX * 6), 187);
	//}
	//
	////도달 상태(자정까지 정지)
	//else if (eState == DAILYLOSS_STATE_BLOCKED )
	//{
	//	m_pImgGameName->SetShow(false) ;

	//	m_pImgLimitMoneyover->SetShow(true) ;

	//	m_pImgLimitMoneyover->SetIndex(52+nGameType) ;
	//	
	//	m_pImgLossMoney->SetShow(false);

	//	m_pImgPercent->SetShow(false) ;

	//	m_pImgLimitMoneytime->SetShow(true) ;
	//	
	//	m_pImgGameLimittime->SetShow(true) ;
	//	m_pImgGameLimittime->SetIndex(32+nGameType) ;

	//	m_pImgGauge->SetShow(false) ;

	//	//if(blockTime == 0 )
	//	//	return TRUE;

	//	// 시간값을 얻어온다.
	//	COleDateTime time = COleDateTime::GetCurrentTime() - g_CSTimeGap;
	//	
	//	SYSTEMTIME st; 
	//	time.GetAsSystemTime(st);

	//	COleDateTime CurTime(st);
	//	//COleDateTime EndTime(blockTime) ;

	//	/*
	//	if(CurTime.GetDay() != EndTime.GetDay())
	//	{
	//		m_pImgToday_Tomorrow->SetIndex(7) ; // 날이 다르면 과거는 나올 수 없으므로 내일.
	//	}else
	//	{
	//		m_pImgToday_Tomorrow->SetIndex(8) ; // 오늘
	//	}
	//	*/ 
	//	m_pImgToday_Tomorrow->SetIndex(8);

	//	m_pImgNumMonth->SetNumber(CurTime.GetMonth());
	//	m_pImgNumDay->SetNumber(CurTime.GetDay());
	//	
	//	m_pImgNumHour->SetNumber(24);		 
	//	m_pImgNumMin->SetNumber(0);

	//	m_pImgNumMin0->SetShow(true) ;

	//	//24시간 정지
	//}else if (eState == DAILYLOSS_STATE_BLOCKED_24H)
	//{
	//	m_pImgGameName->SetShow(false) ;

	//	m_pImgLimitMoneyover->SetShow(true) ;

	//	/*m_pImgLimitMoneyover->SetIndex(26+nGameType) ;*/
	//	m_pImgLimitMoneyover->SetIndex(32) ;

	//	m_pImgLossMoney->SetShow(false);

	//	m_pImgPercent->SetShow(false) ;

	//	m_pImgGauge->SetShow(false) ;

	//	m_pImgLimitMoneytime->SetShow(true) ;

	//	m_pImgGameLimittime->SetShow(true) ;
	//	m_pImgGameLimittime->SetIndex(60) ;

	//	if(blockTime == 0 )
	//		return TRUE;

	//	// 시간값을 얻어온다.
	//	COleDateTime time = COleDateTime::GetCurrentTime() - g_CSTimeGap;

	//	SYSTEMTIME st; 
	//	time.GetAsSystemTime(st);

	//	COleDateTime CurTime(st) ;
	//	COleDateTime EndTime(blockTime) ;

	//	if(CurTime.GetDay() != EndTime.GetDay())
	//	{
	//		m_pImgToday_Tomorrow->SetIndex(7) ; // 날이 다르면 과거는 나올 수 없으므로 내일.
	//	}else
	//	{
	//		m_pImgToday_Tomorrow->SetIndex(8) ; // 오늘
	//	}

	//	m_pImgNumMonth->SetNumber(EndTime.GetMonth());
	//	m_pImgNumDay->SetNumber(EndTime.GetDay());

	//	m_pImgNumHour->SetNumber(EndTime.GetHour());		
	//	m_pImgNumMin->SetNumber(EndTime.GetMinute());

	//	if(10 <= EndTime.GetMinute())
	//	{
	//		m_pImgNumMin0->SetShow(false) ;
	//	}else
	//	{
	//		m_pImgNumMin0->SetShow(true) ;
	//	}
	//}
	//]yoo
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CRestrictionPopup::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CMO_MouseLClick *msg = (LSY::CMO_MouseLClick *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	case ID_RESTRICTIONPOPUP_BTN_PLAY:
		{
			CCL_SET_DAILYLOSSLIMIT_INFO sendmsg;
			sendmsg.Set(GM().GetCurrentGameType(), true);
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), sendmsg.pData, sendmsg.GetTotalSize());
			OnCancel() ;
		}
		break;
	case ID_RESTRICTIONPOPUP_BTN_OK:
	case ID_RESTRICTIONPOPUP_BTN_CLOSE:
		{
			CCL_SET_DAILYLOSSLIMIT_INFO sendmsg;
			sendmsg.Set(GM().GetCurrentGameType(), false);
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), sendmsg.pData, sendmsg.GetTotalSize());
			OnCancel() ;
		}
		break;
	}

	return TRUE;
}

LRESULT CRestrictionPopup::OnMouseOver(LSY::CMessage *pMsg)
{
	m_pImgExplainlossmoney->SetShow(true) ;
	return TRUE;
} 

LRESULT CRestrictionPopup::OnMouseLeave(LSY::CMessage *pMsg)
{
	m_pImgExplainlossmoney->SetShow(false) ;
	return TRUE;
}

INT_PTR CRestrictionPopup::DoModal(CWnd *pParent, IDX_GAME nCurrentTab)
{
	m_pParentWnd = pParent;
	m_nCurrentGame = nCurrentTab;

	return CLSYLibDialogBase::DoModal(GetObjectMan(), ID_RESTRICTIONPOPUP);
}

void CRestrictionPopup::OnCancel()
{
	EndDialog(IDCANCEL);
}
