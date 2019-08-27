// ProfileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SelectQuickstarterDlg.h"
#include "GlobalBase.h"
#include "selectquickstarterdlgid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSelectQuickstarterDlg::CSelectQuickstarterDlg(CWnd* pParent /*=NULL*/)
	: LSY::CLSYLibDialogBase(pParent)
{
	m_i64DefaultSeed = 0;
}

BEGIN_MESSAGE_MAP(CSelectQuickstarterDlg, LSY::CLSYLibDialogBase)
END_MESSAGE_MAP()

bool CSelectQuickstarterDlg::OnCreatedProject(void)
{
	LSY::CObjectProject *pProject = m_pObjectMan->GetProjectFromID(ID_SELECTQUICKSTARTERDLG);
	m_pWndBack = (LSY::CWindows *)pProject->GetMainObject();

	m_pComboLimit = (LSY::CCombo *)GetObject(ID_SELECTQUICKSTARTERDLG_COMBO_LIMITMONEY);
	m_pComboMode = (LSY::CCombo *)GetObject(ID_SELECTQUICKSTARTERDLG_COMBO_GAMEMODE);
	m_pComboRule = (LSY::CCombo *)GetObject(ID_SELECTQUICKSTARTERDLG_COMBO_GAMERULE);
	m_pComboSeedMoney = (LSY::CCombo *)GetObject(ID_SELECTQUICKSTARTERDLG_COMBO_SEEDMONEY);
	m_pComboBlind = (LSY::CCombo *)GetObject(ID_SELECTQUICKSTARTERDLG_COMBO_BLINDRULE);

	m_pComboLimit->AddHandler(LSY::EM_O_SELCHANGE, LSY::Fnt(this, &CSelectQuickstarterDlg::OnSelChangeLimit));
	m_pComboMode->AddHandler(LSY::EM_O_SELCHANGE, LSY::Fnt(this, &CSelectQuickstarterDlg::OnSelChangeMode));
	m_pComboRule->AddHandler(LSY::EM_O_SELCHANGE, LSY::Fnt(this, &CSelectQuickstarterDlg::OnSelChangeRule));

	m_pImgLimitBack = (LSY::CImage*)GetObject(ID_SELECTQUICKSTARTERDLG_IMAGE_LIMITBACK);
	m_pImgModeBack = (LSY::CImage*)GetObject(ID_SELECTQUICKSTARTERDLG_IMAGE_GAMEMODEBACK);
	m_pImgRuleBack = (LSY::CImage*)GetObject(ID_SELECTQUICKSTARTERDLG_IMAGE_GAMERULEBACK);
	m_pImgSeedMoneyBack = (LSY::CImage*)GetObject(ID_SELECTQUICKSTARTERDLG_IMAGE_SEEDMONEYBACK);
	m_pImgBlindBack = (LSY::CImage*)GetObject(ID_SELECTQUICKSTARTERDLG_IMAGE_BLINDRULEBACK);

	m_pRandomCheckbox = (LSY::CCheckBox*)GetObject(ID_SELECTQUICKSTARTERDLG_CHECKBOX_RANDOM);
	m_pDiaCheckbox = (LSY::CCheckBox*)GetObject(ID_SELECTQUICKSTARTERDLG_CHECKBOX_DIA);

	m_pTextRule = (LSY::CText*)GetObject(ID_SELECTQUICKSTARTERDLG_TXT_GAMERULE);	

	LSY::CButton *pButton = (LSY::CButton *)GetObject(ID_SELECTQUICKSTARTERDLG_BTN_CLOSE);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CSelectQuickstarterDlg::OnClickButton));

	pButton = (LSY::CButton *)GetObject(ID_SELECTQUICKSTARTERDLG_BTN_QUICKSTART);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CSelectQuickstarterDlg::OnClickButton));

	return true;
}

BOOL CSelectQuickstarterDlg::OnInitDialog() 
{
	CLSYLibDialogBase::OnInitDialog();
	// TODO: Add extra initialization here

	SetComboData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CSelectQuickstarterDlg::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CMO_MouseLClick *msg = (LSY::CMO_MouseLClick *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	case ID_SELECTQUICKSTARTERDLG_BTN_CLOSE:
		{
			OnCancel() ;
		}
		break;
	case ID_SELECTQUICKSTARTERDLG_BTN_QUICKSTART:
		{
			SaveOption();
			
			GM().SetSelectQuickJoin(true);
			GM().SendSelectQuickJoiner();

			OnCancel() ;
		}		
		break;
	}

	return TRUE;
}

INT_PTR CSelectQuickstarterDlg::DoModal(CWnd *pParent)
{
	m_pParentWnd = pParent;

	return CLSYLibDialogBase::DoModal(GetObjectMan(), ID_SELECTQUICKSTARTERDLG);
}


void CSelectQuickstarterDlg::SetComboData()
{
	m_pImgLimitBack->SetShow(true);
	m_pImgModeBack->SetShow(true);
	m_pImgRuleBack->SetShow(true);
	m_pImgSeedMoneyBack->SetShow(true);
	m_pImgBlindBack->SetShow(false);

	CDataRoomOptionList *pDataRoomOptionList = GM().GetDataRoomOptionList(); //게임 메니저에 있는 룸옵션 리스트를 받아온다, 만약 없을 경우 리턴
	if(pDataRoomOptionList == NULL) return ;

	ROOM_CREATEOPTION *pRoomCreationOption = pDataRoomOptionList->GetRoomCreationOption();
	if(pRoomCreationOption == NULL) return ;

	ROOMOPTION *pDefaultRoomOption = pDataRoomOptionList->GetDefaultRoomOption();//디폴트 룸옵션 받기
	if(pDefaultRoomOption == NULL) return ;

	ROOMOPTION *pRoomOption = NULL;
	CString strMoney;

	int nIndex = 0;
	int nDefault = 0;
	int nCount = 1;

	bool bBigRoom = false ;

	//
	MAP_LIST_RULE_ITER iter = m_listRule.begin();
	while ( iter != m_listRule.end() )
	{
		map< BetRule*, RULE >::iterator iterRule = ( iter->second ).begin();

		while( iterRule !=  ( iter->second ).end() )
		{
			delete iterRule->first;

			iterRule++;
		}

		++iter;
	}

	m_listRule.clear();

	short sDefaultAutoBet = -1;
	char cCardSetting = 0;

	m_listMinimumMoney.clear();
	m_vecMinimumMoney.clear();

	bool bDefaultCheck = false ;
	for(int i=0;i<pRoomCreationOption->nArrayCnt;i++)
	{
		pRoomOption = &(pRoomCreationOption->arrRoomOption[i]);

		//if(pRoomOption->llEnterLimitMoney == pDefaultRoomOption->llEnterLimitMoney)
		if(0<pRoomOption->llEnterLimitMoney)
		{
			RULE rule;
			rule.i64Seed = pRoomOption->llSeedMoney;
			rule.sAutoBet = pRoomOption->sAutoBet;
			rule.sBetRuleKind = pRoomOption->sBetRuleKind;	//리미트배팅룰
			rule.cCardSetting = pRoomOption->cCardSetting;
			rule.fBetMoneyMulti = pRoomOption->fBetMoneyMulti;
			rule.llEnterLimitMoney = pRoomOption->llEnterLimitMoney;

			BetRule* pBetRuleKey = NULL;
			pBetRuleKey = new BetRule;

			pBetRuleKey->sAutoBet = pRoomOption->sAutoBet;

			m_listRule[ pRoomOption->llSeedMoney ][ pBetRuleKey ] = rule;

			if(pRoomOption->cDefault)
			{
				sDefaultAutoBet = pRoomOption->sAutoBet;
				m_i64DefaultSeed = pRoomOption->llSeedMoney;
				cCardSetting = pRoomOption->cCardSetting;
			}

			m_listMinimumMoney.insert(pRoomOption->llEnterLimitMoney);
		}
	}

	m_pComboLimit->RemoveAll();
	m_pComboLimit->AddText("상관없음");
	m_pComboLimit->SetItemData(0, -1);

	set<INT64>::iterator start = m_listMinimumMoney.begin();
	set<INT64>::iterator end = m_listMinimumMoney.end();

	bool bZero = false;
	set<INT64>::iterator itZero;

	for(;start != end;start++)
	{
		//입장제한 없음 주석 처리
		if(*start == 0)
		{
			bZero = true;
			itZero = start;
			continue;
		}
		else strMoney.Format("%s", NMBASE::UTIL::g_NumberToHangul(*start));
		m_vecMinimumMoney.push_back(*start);
		nIndex = m_pComboLimit->AddText(strMoney.GetString());
		m_pComboLimit->SetItemData(nIndex, nIndex-1);

		//if(pDefaultRoomOption->llEnterLimitMoney == *start) 
		if(GM().GetRoomOptionLimitMoney(pDefaultRoomOption, false) == *start) 
			nDefault = nIndex;

		nCount++;
	}

	if(bZero)
	{
		strMoney = "자유";

		nIndex = m_pComboLimit->AddText(strMoney.GetString());
		m_vecMinimumMoney.push_back(*itZero);
		m_pComboLimit->SetItemData(nIndex, nIndex-1);

		//if(pDefaultRoomOption->llEnterLimitMoney == 0) 
		if(GM().GetRoomOptionLimitMoney(pDefaultRoomOption, false) == 0) 
			nDefault = nIndex;

		nCount++;
	}

	m_pComboLimit->SetCurSelectItem(nDefault);
	m_pComboLimit->SetTotalHeightUseLineHeight(nCount);

	m_i64DefaultSeed = GM().GetRoomOptionSeedMoney(pDefaultRoomOption, false);

	// m_pComboSeedMoney 변경은 SetComboSeedMoney 함수로 함. 2019.4.1 패치 shisky
	/*
	m_pComboSeedMoney->RemoveAll();
	m_pComboSeedMoney->AddText("상관없음");
	m_pComboSeedMoney->SetItemData(0, -1);

	nCount = 1 ;
	m_vecSeedMoney.clear();

	MAP_LIST_RULE_ITER startSeed = m_listRule.begin();
	MAP_LIST_RULE_ITER endSeed = m_listRule.end();

	map< BetRule*, RULE >::iterator start_auto, end_auto;
	nDefault = -1;
	for(;startSeed != endSeed;startSeed++)
	{
		start_auto = startSeed->second.begin();
		end_auto = startSeed->second.end();

		for(;start_auto != end_auto;start_auto++)
		{
			if( startSeed->first > 0)
			{
				vector<INT64>::iterator _start = m_vecSeedMoney.begin();
				vector<INT64>::iterator _end = m_vecSeedMoney.end();

				bool bInsert = false ;
				if(start_auto->second.llEnterLimitMoney == GM().GetRoomOptionLimitMoney(pDefaultRoomOption, false))
				{
					bInsert = true ;
				}

				for(;_start != _end;_start++)
				{
					if(*_start == startSeed->first)
					{
						bInsert = false ;
					}					
				}

				if(true == bInsert)
				{
					strMoney.Format("%s", NMBASE::UTIL::g_NumberToHangul(startSeed->first));
					m_vecSeedMoney.push_back(startSeed->first);
					nCount++;

					nIndex = m_pComboSeedMoney->AddText(strMoney.GetString());

					m_pComboSeedMoney->SetItemData(nIndex, nIndex-1);

					// 디폴트 옵션 선택.
					if(m_i64DefaultSeed == startSeed->first) {
						nDefault = nIndex;
					}
					else if(-1==nDefault){
						nDefault=nIndex;
					}
				}
			}
		}
	}

	m_pComboSeedMoney->SetCurSelectItem(nDefault);
	m_pComboSeedMoney->SetTotalHeightUseLineHeight(nCount);
	*/

	m_vecSeedMoney.clear();

	MAP_LIST_RULE_ITER startSeed = m_listRule.begin();
	MAP_LIST_RULE_ITER endSeed = m_listRule.end();

	map< BetRule*, RULE >::iterator start_auto, end_auto;
	for(;startSeed != endSeed;startSeed++)
	{
		start_auto = startSeed->second.begin();
		end_auto = startSeed->second.end();

		for(;start_auto != end_auto;start_auto++)
		{
			if( startSeed->first > 0)
			{
				vector<INT64>::iterator _start = m_vecSeedMoney.begin();
				vector<INT64>::iterator _end = m_vecSeedMoney.end();

				bool bInsert = false ;
				if(start_auto->second.llEnterLimitMoney == GM().GetRoomOptionLimitMoney(pDefaultRoomOption, false))
				{
					bInsert = true ;
				}

				for(;_start != _end;_start++)
				{
					if(*_start == startSeed->first)
					{
						bInsert = false ;
					}					
				}

				if(true == bInsert)
				{
					m_vecSeedMoney.push_back(startSeed->first);
				}
			}
		}
	}

	int nCur = m_pComboLimit->GetCurSelectItem();
	nCount =m_pComboLimit->GetItemData(nCur);
	INT64 llEnterLimitMoney = -1;
	if(-1 != nCount)
	{
		llEnterLimitMoney = m_vecMinimumMoney[nCount];
	}
	if(true==GM().IsDiaMemberRooms(llEnterLimitMoney)){
		m_pWndBack->SetIndex(38);
		m_pDiaCheckbox->SetShow(true);
		m_pDiaCheckbox->SetEnable(true);
	}
	else{
		m_pWndBack->SetIndex(36);
		m_pDiaCheckbox->SetShow(false);
		m_pDiaCheckbox->SetEnable(false);
	}

	

	int nGame = (int)GM().GetCurrentGameType();
	switch(nGame)
	{
	case IDX_GAME_HA:
		m_pImgLimitBack->SetPos(27, 95);
		m_pImgModeBack->SetPos(131, 95);
		m_pImgRuleBack->SetPos(235, 95);
		m_pImgSeedMoneyBack->SetPos(339, 95);

		m_pTextRule->SetText("게임옵션");

		m_pComboMode->RemoveAll();
		m_pComboMode->AddText("상관없음");
		m_pComboMode->SetItemData(0, -1);

		m_pComboMode->AddText("일반");
		m_pComboMode->SetItemData(1, 1);

		m_pComboMode->AddText("조커");
		m_pComboMode->SetItemData(2, 2);

		if(GM().GetMyInfo()->UI.GetRealPMoney() >= (INT64)1000000000000)
		{
			m_pComboMode->SetCurSelectItem(1);
		}else
		{
			m_pComboMode->SetCurSelectItem(2);
		}

		m_pComboMode->SetTotalHeightUseLineHeight(3);

		m_pComboRule->RemoveAll();
		m_pComboRule->AddText("상관없음");
		m_pComboRule->SetItemData(0, -1);

		m_pComboRule->AddText("스톱on");
		m_pComboRule->SetItemData(1, 2);

		m_pComboRule->AddText("스톱off");
		m_pComboRule->SetItemData(2, 1);

		m_pComboRule->SetCurSelectItem(1);

		m_pComboRule->SetTotalHeightUseLineHeight(3);

		break;
	case IDX_GAME_NP:
	case IDX_GAME_SP:
	case IDX_GAME_HL:
		{
			m_pImgLimitBack->SetPos(27, 95);
			m_pImgModeBack->SetPos(131, 95);
			m_pImgRuleBack->SetPos(235, 95);
			m_pImgSeedMoneyBack->SetPos(339, 95);

			m_pTextRule->SetText("게임룰");

			m_pComboMode->RemoveAll();
			m_pComboMode->AddText("상관없음");
			m_pComboMode->SetItemData(0, -1);

			m_pComboRule->RemoveAll();
			m_pComboRule->AddText("상관없음");
			m_pComboRule->SetItemData(0, -1);

			if(nGame == IDX_GAME_SP || nGame == IDX_GAME_HL)
			{
				m_pComboMode->AddText("초이스");
				m_pComboMode->SetItemData(1, 1);
				m_pComboMode->AddText("일반");
				m_pComboMode->SetItemData(2, 0);

				m_pComboMode->SetCurSelectItem(1);

				m_pComboMode->SetTotalHeightUseLineHeight(3);

				if(nGame == IDX_GAME_SP)
				{
					int nCur=1;

					if(llEnterLimitMoney >= (INT64)100ll*MONEY_UK){
					//if(GM().GetMyInfo()->UI.GetRealPMoney() >= (INT64)500000000000){
						m_pComboRule->AddText("수동");
						m_pComboRule->SetItemData(nCur, 4);
						nCur++;
						m_pComboRule->SetCurSelectItem(1);
					}
					if(llEnterLimitMoney < (INT64)100ll*MONEY_UK){
					//if(GM().GetMyInfo()->UI.GetRealPMoney() < (INT64)1000000000000){
						m_pComboRule->AddText("5구");
						m_pComboRule->SetItemData(nCur, 5);
						if(1==nCur){
							m_pComboRule->SetCurSelectItem(1);
						}
						nCur++;
					}
					m_pComboRule->SetTotalHeightUseLineHeight(nCur);

					if(GM().GetMyInfo()->UI.GetRealPMoney() >= (INT64)30000000000000)
					{
						bBigRoom = true ;
					}
				}else
				{
#if defined(_GAMECHANNEL_CHANGE)
					int nCur=1;
					if(llEnterLimitMoney >= (INT64)1000ll*MONEY_UK){
						m_pComboRule->AddText("수동");
						m_pComboRule->SetItemData(nCur, 4);
						nCur++;
						m_pComboRule->SetCurSelectItem(1);
					}
					if(llEnterLimitMoney < (INT64)1000ll*MONEY_UK){
						m_pComboRule->AddText("5구");
						m_pComboRule->SetItemData(nCur, 5);
						nCur++;
						m_pComboRule->SetCurSelectItem(1);
					}
					m_pComboRule->SetTotalHeightUseLineHeight(nCur);
#else

					m_pComboRule->AddText("수동");
					m_pComboRule->SetItemData(1, 4);

					if(GM().GetMyInfo()->UI.GetRealPMoney() >= (INT64)3000000000000)
					{
						m_pComboRule->AddText("5구");
						m_pComboRule->SetItemData(2, 5);
						m_pComboRule->SetCurSelectItem(2);
					
					}else if(GM().GetMyInfo()->UI.GetRealPMoney() < (INT64)100000000)
					{
						m_pComboRule->AddText("5구");
						m_pComboRule->SetItemData(2, 5);

						m_pComboRule->SetCurSelectItem(1);

					}else
					{
						m_pComboRule->AddText("6구");
						m_pComboRule->SetItemData(2, 6);
						m_pComboRule->SetCurSelectItem(2);
					}
					m_pComboRule->SetTotalHeightUseLineHeight(3);	
#endif

					if(GM().GetMyInfo()->UI.GetRealPMoney() >= (INT64)100000000000000)
					{
						bBigRoom = true ;
					}				
				}


			}else if(nGame == IDX_GAME_NP )
			{
				m_pComboMode->AddText("폭탄");
				m_pComboMode->SetItemData(1, 2);
				m_pComboMode->AddText("조커");
				m_pComboMode->SetItemData(2, 3);

				if(GM().GetMyInfo()->UI.GetRealPMoney() < (INT64)3000000000000)
				{
					m_pComboMode->SetCurSelectItem(2);
				}else
				{
					m_pComboMode->SetCurSelectItem(1);
				}				

				m_pComboMode->SetTotalHeightUseLineHeight(3);
#if defined(_GAMECHANNEL_CHANGE)
				if(llEnterLimitMoney >= (INT64)1000ll*MONEY_UK){
					m_pComboRule->AddText("수동");
					m_pComboRule->SetItemData(2, 5);
					m_pComboRule->SetCurSelectItem(1);
					m_pComboRule->SetTotalHeightUseLineHeight(2);
				}
				if(llEnterLimitMoney < (INT64)1000ll*MONEY_UK){
					m_pComboRule->AddText("6구");
					m_pComboRule->SetItemData(2, 6);
					m_pComboRule->SetCurSelectItem(1);
					m_pComboRule->SetTotalHeightUseLineHeight(2);
				}
#else
				m_pComboRule->AddText("수동");
				m_pComboRule->SetItemData(1, 5);
				m_pComboRule->AddText("6구");
				m_pComboRule->SetItemData(2, 6);

				if(GM().GetMyInfo()->UI.GetRealPMoney() < (INT64)100000000 || GM().GetMyInfo()->UI.GetRealPMoney() >= (INT64)3000000000000)
				{
					if(GM().GetMyInfo()->UI.GetRealPMoney() >= (INT64)30000000000000){
						m_pComboRule->SetCurSelectItem(2);
					}else{
						m_pComboRule->SetCurSelectItem(1);
					}
				}else
				{
					m_pComboRule->SetCurSelectItem(2);
				}
				m_pComboRule->SetTotalHeightUseLineHeight(3);
#endif

				if(GM().GetMyInfo()->UI.GetRealPMoney() >= (INT64)50000000000000)
				{
					bBigRoom = true ;
				}


			}else
			{
				m_pComboMode->SetCurSelectItem(0);
				m_pComboMode->SetTotalHeightUseLineHeight(1);

				m_pComboRule->SetCurSelectItem(0);
				m_pComboRule->SetTotalHeightUseLineHeight(1);
			}
		}
		break;
	case IDX_GAME_SD:
		{
			m_pImgLimitBack->SetPos(27, 95);
			m_pImgModeBack->SetPos(131, 95);
			m_pImgSeedMoneyBack->SetPos(235, 95);

			m_pImgRuleBack->SetShow(false);

			m_pComboMode->RemoveAll();
			m_pComboMode->AddText("상관없음");
			m_pComboMode->SetItemData(0, -1);

			m_pComboMode->AddText("2장");
			m_pComboMode->SetItemData(1, 0);

			m_pComboMode->AddText("3장");
			m_pComboMode->SetItemData(2, 1);

			if(GM().GetMyInfo()->UI.GetRealPMoney() < (INT64)1000000000000)
			{
				m_pComboMode->SetCurSelectItem(2);			
			}else
			{
				
				m_pComboMode->SetCurSelectItem(1);
			}

			m_pComboMode->SetTotalHeightUseLineHeight(3);

			m_pComboRule->RemoveAll();
			m_pComboRule->AddText("상관없음");
			m_pComboRule->SetItemData(0, -1);

			m_pComboRule->AddText("수동");
			m_pComboRule->SetItemData(1, 4);

			m_pComboRule->SetCurSelectItem(1);

			m_pComboRule->SetTotalHeightUseLineHeight(2);
		}
		break;
	case IDX_GAME_BD:
		{
			m_pImgLimitBack->SetPos(27, 95);
			m_pImgRuleBack->SetPos(131, 95);
			m_pImgSeedMoneyBack->SetPos(235, 95);

			m_pImgModeBack->SetShow(false);

			m_pTextRule->SetText("게임룰");

			m_pComboRule->RemoveAll();
			m_pComboRule->AddText("상관없음");
			m_pComboRule->SetItemData(0, -1);

			int nCur=1;

			if(llEnterLimitMoney > (INT64)100ll*MONEY_UK){
			//if(GM().GetMyInfo()->UI.GetRealPMoney() > (INT64)1000000000000){
				m_pComboRule->AddText("수동");
				m_pComboRule->SetItemData(nCur, 0);
				nCur++;
				m_pComboRule->SetCurSelectItem(1);
			}
			if(llEnterLimitMoney <= (INT64)100ll*MONEY_UK){
			//if(GM().GetMyInfo()->UI.GetRealPMoney() <= (INT64)1000000000000){
				m_pComboRule->AddText("아침");
				m_pComboRule->SetItemData(nCur, 1);
				if(1==nCur){
					m_pComboRule->SetCurSelectItem(1);
				}
				nCur++;
			}
			m_pComboRule->SetTotalHeightUseLineHeight(nCur);

			INT64 ss = m_pComboLimit->GetItemData(m_pComboLimit->GetCurSelectItem());
			m_pComboBlind->RemoveAll();
			m_pComboBlind->SetText("On");
			m_pComboBlind->AddText("On");
			m_pComboBlind->SetItemData(0, 5);
			m_pComboBlind->AddText("Off");
			m_pComboBlind->SetItemData(1, -1);
			m_pComboBlind->SetCurSelectItem(0);
			m_pImgBlindBack->SetShow(llEnterLimitMoney >= (INT64)100000ll*MONEY_UK);

			m_pComboBlind->SetTotalHeightUseLineHeight(2);

			if(GM().GetMyInfo()->UI.GetRealPMoney() >= (INT64)50000000000000)
			{
				bBigRoom = true ;
			}

			//m_pComboRule->SetTotalHeightUseLineHeight(3);
		}

		break;
	}

	SetComboSeedMoney();
}

LRESULT CSelectQuickstarterDlg::OnSelChangeLimit(LSY::CMessage *msg)
{
	LSY::CMO_SelChange *pMsg = (LSY::CMO_SelChange *)msg;
	int nValue = pMsg->GetValue();
	INT64 llEnterLimitMoney = -1;
	int nCount = 1;
	int nCur = m_pComboLimit->GetCurSelectItem();
	nCount =m_pComboLimit->GetItemData(nCur);
	llEnterLimitMoney = -1;
	if(-1 != nCount)
	{
		llEnterLimitMoney = m_vecMinimumMoney[nCount];
	}

	if(true==GM().IsDiaMemberRooms(llEnterLimitMoney)){
		m_pWndBack->SetIndex(38);
		m_pDiaCheckbox->SetShow(true);
		m_pDiaCheckbox->SetEnable(true);
	}
	else{
		m_pWndBack->SetIndex(36);
		m_pDiaCheckbox->SetShow(false);
		m_pDiaCheckbox->SetEnable(false);
	}
	if(0 == nValue)
	{
		m_pImgBlindBack->SetShow(false);
		m_pComboSeedMoney->RemoveAll();
		m_pComboSeedMoney->AddText("상관없음");
		m_pComboSeedMoney->SetItemData(0, -1);
		m_pComboSeedMoney->SetCurSelectItem(nValue);
		m_pComboSeedMoney->SetTotalHeightUseLineHeight(1);
		return TRUE ;
	}

	if(-1 != nValue)
	{
		llEnterLimitMoney = m_vecMinimumMoney[nValue-1];
	}

	m_pImgLimitBack->SetShow(true);
	m_pImgModeBack->SetShow(true);
	m_pImgRuleBack->SetShow(true);
	m_pImgSeedMoneyBack->SetShow(true);

	CDataRoomOptionList *pDataRoomOptionList = GM().GetDataRoomOptionList(); //게임 메니저에 있는 룸옵션 리스트를 받아온다, 만약 없을 경우 리턴
	if(pDataRoomOptionList == NULL) return TRUE;

	ROOM_CREATEOPTION *pRoomCreationOption = pDataRoomOptionList->GetRoomCreationOption();
	if(pRoomCreationOption == NULL) return TRUE;

	ROOMOPTION *pRoomOption = NULL;
	int i=0;
	CString strMoney;

	int nIndex = 0;
	int nDefault = -1;

	short sDefaultAutoBet = -1;
	char cCardSetting = 0;

	for(i=0;i<pRoomCreationOption->nArrayCnt;i++)
	{
		pRoomOption = &(pRoomCreationOption->arrRoomOption[i]);
		if(pRoomOption->llEnterLimitMoney == llEnterLimitMoney)
		{
			if(pRoomOption->cDefault)
			{
				sDefaultAutoBet = pRoomOption->sAutoBet;
				m_i64DefaultSeed = pRoomOption->llSeedMoney;
				cCardSetting = pRoomOption->cCardSetting;
			}
		}
	}

	bool bBigRoom = false ;

	int nGame = (int)GM().GetCurrentGameType();
	switch(nGame)
	{
	case IDX_GAME_HA:
		m_pImgLimitBack->SetPos(27, 95);
		m_pImgModeBack->SetPos(131, 95);
		m_pImgRuleBack->SetPos(235, 95);
		m_pImgSeedMoneyBack->SetPos(339, 95);

		m_pTextRule->SetText("게임옵션");

		m_pComboMode->RemoveAll();
		m_pComboMode->AddText("상관없음");
		m_pComboMode->SetItemData(0, -1);

		m_pComboMode->AddText("일반");
		m_pComboMode->SetItemData(1, 1);

		m_pComboMode->AddText("조커");
		m_pComboMode->SetItemData(2, 2);

		if(GM().GetMyInfo()->UI.GetRealPMoney() >= (INT64)1000000000000)
		{
			m_pComboMode->SetCurSelectItem(1);
		}else
		{
			m_pComboMode->SetCurSelectItem(2);
		}

		m_pComboMode->SetTotalHeightUseLineHeight(3);

		m_pComboRule->RemoveAll();
		m_pComboRule->AddText("상관없음");
		m_pComboRule->SetItemData(0, -1);

		m_pComboRule->AddText("스톱on");
		m_pComboRule->SetItemData(1, 2);

		m_pComboRule->AddText("스톱off");
		m_pComboRule->SetItemData(2, 1);

		m_pComboRule->SetCurSelectItem(1);

		m_pComboRule->SetTotalHeightUseLineHeight(3);

		break;
	case IDX_GAME_NP:
	case IDX_GAME_SP:
	case IDX_GAME_HL:
		{
			m_pImgLimitBack->SetPos(27, 95);
			m_pImgModeBack->SetPos(131, 95);
			m_pImgRuleBack->SetPos(235, 95);
			m_pImgSeedMoneyBack->SetPos(339, 95);

			m_pTextRule->SetText("게임룰");

			m_pComboMode->RemoveAll();
			m_pComboMode->AddText("상관없음");
			m_pComboMode->SetItemData(0, -1);

			m_pComboRule->RemoveAll();
			m_pComboRule->AddText("상관없음");
			m_pComboRule->SetItemData(0, -1);

			if(nGame == IDX_GAME_SP || nGame == IDX_GAME_HL)
			{
				m_pComboMode->AddText("초이스");
				m_pComboMode->SetItemData(1, 1);
				m_pComboMode->AddText("일반");
				m_pComboMode->SetItemData(2, 0);

				m_pComboMode->SetCurSelectItem(1);

				m_pComboMode->SetTotalHeightUseLineHeight(3);


				if(nGame == IDX_GAME_SP)
				{
					int nCur=1;

					if(llEnterLimitMoney >= (INT64)5000ll*MONEY_UK){
					//if(GM().GetMyInfo()->UI.GetRealPMoney() >= (INT64)500000000000){
						m_pComboRule->AddText("수동");
						m_pComboRule->SetItemData(1, 4);
						nCur++;
						m_pComboRule->SetCurSelectItem(1);
					}
					if(llEnterLimitMoney < (INT64)5000ll*MONEY_UK){
					//if(GM().GetMyInfo()->UI.GetRealPMoney() < (INT64)1000000000000){
						m_pComboRule->AddText("5구");
						m_pComboRule->SetItemData(2, 5);
						if(1==nCur){
							m_pComboRule->SetCurSelectItem(1);
						}
						nCur++;
					}
					m_pComboRule->SetTotalHeightUseLineHeight(nCur);

					if(GM().GetMyInfo()->UI.GetRealPMoney() >= (INT64)30000000000000)
					{
						bBigRoom = true ;
					}
					//m_pComboRule->SetCurSelectItem(2);
					//m_pComboRule->SetTotalHeightUseLineHeight(3);
				}else
				{
#if defined(_GAMECHANNEL_CHANGE)
					int nCur=1;
					if(llEnterLimitMoney >= (INT64)1000ll*MONEY_UK){
						m_pComboRule->AddText("수동");
						m_pComboRule->SetItemData(nCur, 4);
						nCur++;
						m_pComboRule->SetCurSelectItem(1);
					}
					if(llEnterLimitMoney < (INT64)1000ll*MONEY_UK){
						m_pComboRule->AddText("5구");
						m_pComboRule->SetItemData(nCur, 5);
						nCur++;
						m_pComboRule->SetCurSelectItem(1);
					}
					m_pComboRule->SetTotalHeightUseLineHeight(nCur);
#else
					m_pComboRule->AddText("수동");
					m_pComboRule->SetItemData(1, 4);
					if(GM().GetMyInfo()->UI.GetRealPMoney() >= (INT64)3000000000000)
					{
						m_pComboRule->AddText("5구");
						m_pComboRule->SetItemData(2, 5);
						m_pComboRule->SetCurSelectItem(2);
					
					}else if(GM().GetMyInfo()->UI.GetRealPMoney() >= (INT64)100000000)
					{
						m_pComboRule->AddText("5구");
						m_pComboRule->SetItemData(2, 5);

						m_pComboRule->SetCurSelectItem(1);
					
					}else
					{
						m_pComboRule->AddText("6구");
						m_pComboRule->SetItemData(2, 6);
						m_pComboRule->SetCurSelectItem(2);
					}
					m_pComboRule->SetTotalHeightUseLineHeight(3);
#endif

					if(GM().GetMyInfo()->UI.GetRealPMoney() >= (INT64)100000000000000)
					{
						bBigRoom = true ;
					}		
				}


			}
			else if(nGame == IDX_GAME_NP )
			{
				m_pComboMode->AddText("폭탄");
				m_pComboMode->SetItemData(1, 2);
				m_pComboMode->AddText("조커");
				m_pComboMode->SetItemData(2, 3);

				if(GM().GetMyInfo()->UI.GetRealPMoney() < (INT64)3000000000000)
				{
					m_pComboMode->SetCurSelectItem(2);
				}else
				{
					m_pComboMode->SetCurSelectItem(1);
				}				

				m_pComboMode->SetTotalHeightUseLineHeight(3);

#if defined(_GAMECHANNEL_CHANGE)
				if(llEnterLimitMoney >= (INT64)1000ll*MONEY_UK){
					m_pComboRule->AddText("수동");
					m_pComboRule->SetItemData(2, 5);
					m_pComboRule->SetCurSelectItem(1);
					m_pComboRule->SetTotalHeightUseLineHeight(2);
				}
				if(llEnterLimitMoney < (INT64)1000ll*MONEY_UK){
					m_pComboRule->AddText("6구");
					m_pComboRule->SetItemData(2, 6);
					m_pComboRule->SetCurSelectItem(1);
					m_pComboRule->SetTotalHeightUseLineHeight(2);
				}
#else
				m_pComboRule->AddText("수동");
				m_pComboRule->SetItemData(1, 5);
				m_pComboRule->AddText("6구");
				m_pComboRule->SetItemData(2, 6);

				if(GM().GetMyInfo()->UI.GetRealPMoney() < (INT64)100000000 || GM().GetMyInfo()->UI.GetRealPMoney() >= (INT64)3000000000000)
				{
					m_pComboRule->SetCurSelectItem(1);
				}else
				{
					m_pComboRule->SetCurSelectItem(2);
				}
				m_pComboRule->SetTotalHeightUseLineHeight(3);
#endif
				if(GM().GetMyInfo()->UI.GetRealPMoney() >= (INT64)50000000000000)
				{
					bBigRoom = true ;
				}


			}else
			{
				m_pComboMode->SetCurSelectItem(0);
				m_pComboMode->SetTotalHeightUseLineHeight(1);

				m_pComboRule->SetCurSelectItem(0);
				m_pComboRule->SetTotalHeightUseLineHeight(1);
			}
		}
		break;
	case IDX_GAME_SD:
		{
			m_pImgLimitBack->SetPos(27, 95);
			m_pImgModeBack->SetPos(131, 95);
			m_pImgSeedMoneyBack->SetPos(235, 95);

			m_pImgRuleBack->SetShow(false);

			m_pComboMode->RemoveAll();
			m_pComboMode->AddText("상관없음");
			m_pComboMode->SetItemData(0, -1);

			m_pComboMode->AddText("2장");
			m_pComboMode->SetItemData(1, 0);

			m_pComboMode->AddText("3장");
			m_pComboMode->SetItemData(2, 1);

			if(GM().GetMyInfo()->UI.GetRealPMoney() < (INT64)1000000000000)
			{
				m_pComboMode->SetCurSelectItem(2);			
			}else
			{
				
				m_pComboMode->SetCurSelectItem(1);
			}

			m_pComboMode->SetTotalHeightUseLineHeight(3);

			m_pComboRule->RemoveAll();
			m_pComboRule->AddText("상관없음");
			m_pComboRule->SetItemData(0, -1);

			m_pComboRule->AddText("수동");
			m_pComboRule->SetItemData(1, 4);

			m_pComboRule->SetCurSelectItem(1);

			m_pComboRule->SetTotalHeightUseLineHeight(2);
		}
		break;
	case IDX_GAME_BD:
		{
			m_pImgLimitBack->SetPos(27, 95);
			m_pImgRuleBack->SetPos(131, 95);
			m_pImgSeedMoneyBack->SetPos(235, 95);

			m_pImgModeBack->SetShow(false);

			m_pTextRule->SetText("게임룰");

			m_pComboRule->RemoveAll();
			m_pComboRule->AddText("상관없음");
			m_pComboRule->SetItemData(0, -1);
			int nCur=1;

			if(llEnterLimitMoney > (INT64)1000ll*MONEY_UK){
				m_pComboRule->AddText("수동");
				m_pComboRule->SetItemData(nCur, 0);
				nCur++;
				m_pComboRule->SetCurSelectItem(1);
			}
			if(llEnterLimitMoney <= (INT64)1000ll*MONEY_UK){
				m_pComboRule->AddText("아침");
				m_pComboRule->SetItemData(nCur, 1);
				if(1==nCur){
					m_pComboRule->SetCurSelectItem(1);
				}
				nCur++;
			}
			m_pComboRule->SetTotalHeightUseLineHeight(nCur);

			m_pImgBlindBack->SetShow(llEnterLimitMoney >= (INT64)10ll*MONEY_JO);

			if(GM().GetMyInfo()->UI.GetRealPMoney() >= (INT64)50000000000000)
			{
				bBigRoom = true ;
			}

			//m_pComboRule->SetTotalHeightUseLineHeight(3);
		}

		break;
	}
	
	// m_pComboSeedMoney 변경은 SetComboSeedMoney 함수로 함. 2019.4.1 패치 shisky
	/*
	m_pComboSeedMoney->RemoveAll();
	m_pComboSeedMoney->AddText("상관없음");
	m_pComboSeedMoney->SetItemData(0, -1);

	nCount = 1 ;
	m_vecSeedMoney.clear();

	MAP_LIST_RULE_ITER startSeed = m_listRule.begin();
	MAP_LIST_RULE_ITER endSeed = m_listRule.end();

	map< BetRule*, RULE >::iterator start_auto, end_auto;

	for(;startSeed != endSeed;startSeed++)
	{
		start_auto = startSeed->second.begin();
		end_auto = startSeed->second.end();

		for(;start_auto != end_auto;start_auto++)
		{
			if( startSeed->first > 0)
			{
				vector<INT64>::iterator _start = m_vecSeedMoney.begin();
				vector<INT64>::iterator _end = m_vecSeedMoney.end();

				bool bInsert = false ;
				if(start_auto->second.llEnterLimitMoney == llEnterLimitMoney)
				{
					bInsert = true ;
				}

				for(;_start != _end;_start++)
				{
					if(*_start == startSeed->first)
					{
						bInsert = false ;
					}					
				}

				if(true == bInsert)
				{
					strMoney.Format("%s", NMBASE::UTIL::g_NumberToHangul(startSeed->first));
					m_vecSeedMoney.push_back(startSeed->first);
					nCount++;

					nIndex = m_pComboSeedMoney->AddText(strMoney.GetString());

					m_pComboSeedMoney->SetItemData(nIndex, nIndex-1);

					// 디폴트 옵션 선택.
					if(m_i64DefaultSeed == startSeed->first) 
						nDefault = nIndex;
				}
			}
		}
	}

	if(true == bBigRoom)
	{
		if(-1 == nDefault)
		{
			m_pComboSeedMoney->SetCurSelectItem(nCount-1);
		}else
		{
			m_pComboSeedMoney->SetCurSelectItem(nDefault);
		}		
	}else
	{
		m_pComboSeedMoney->SetCurSelectItem(nDefault);
	}
	
	m_pComboSeedMoney->SetTotalHeightUseLineHeight(nCount);
	*/

	m_vecSeedMoney.clear();

	MAP_LIST_RULE_ITER startSeed = m_listRule.begin();
	MAP_LIST_RULE_ITER endSeed = m_listRule.end();

	map< BetRule*, RULE >::iterator start_auto, end_auto;

	for(;startSeed != endSeed;startSeed++)
	{
		start_auto = startSeed->second.begin();
		end_auto = startSeed->second.end();

		for(;start_auto != end_auto;start_auto++)
		{
			if( startSeed->first > 0)
			{
				vector<INT64>::iterator _start = m_vecSeedMoney.begin();
				vector<INT64>::iterator _end = m_vecSeedMoney.end();

				bool bInsert = false ;
				if(start_auto->second.llEnterLimitMoney == llEnterLimitMoney)
				{
					bInsert = true ;
				}

				for(;_start != _end;_start++)
				{
					if(*_start == startSeed->first)
					{
						bInsert = false ;
					}					
				}

				if(true == bInsert)
				{
					m_vecSeedMoney.push_back(startSeed->first);
				}
			}
		}
	}
	
	SetComboSeedMoney();

	return TRUE;
}

LRESULT CSelectQuickstarterDlg::OnSelChangeMode(LSY::CMessage *msg)
{
	SetComboSeedMoney();

	return TRUE;
}

LRESULT CSelectQuickstarterDlg::OnSelChangeRule(LSY::CMessage *msg)
{
	SetComboSeedMoney();

	return TRUE;
}

void CSelectQuickstarterDlg::SetComboSeedMoney()
{
	m_pComboSeedMoney->RemoveAll();
	m_pComboSeedMoney->AddText("상관없음");
	m_pComboSeedMoney->SetItemData(0, -1);
	CString strMoney;

	int nDefault = -1;
	int nIndex = 0;
	INT64 i64GiveUpSeed = 6000 * MONEY_UK;
	if (GM().GetCurrentGameType() == IDX_GAME_HA)
	{
		i64GiveUpSeed = 4000 * MONEY_UK;
	}
	for (int i = 0; i < m_vecSeedMoney.size(); i++)
	{
		if (m_vecSeedMoney[i] == i64GiveUpSeed && IsGiveUpRoom() == false)
			continue;

		strMoney.Format("%s", NMBASE::UTIL::g_NumberToHangul(m_vecSeedMoney[i]));

		nIndex = m_pComboSeedMoney->AddText(strMoney.GetString());
		m_pComboSeedMoney->SetItemData(nIndex, i);

		// 디폴트 옵션 선택.
		if(m_i64DefaultSeed == m_vecSeedMoney[i]) {
			nDefault = nIndex;
		}
		else if(-1==nDefault){
			nDefault = nIndex;
		}
	}
	m_pComboSeedMoney->SetCurSelectItem(nDefault);
	m_pComboSeedMoney->SetTotalHeightUseLineHeight(nIndex + 1);
}

bool CSelectQuickstarterDlg::IsGiveUpRoom()
{
	// ### 기권방 조건 ###	
	int nCount = m_pComboLimit->GetCurSelectItem();
	INT64 llEnterLimitMoney = -1;
	if(-1 != nCount)
	{
		llEnterLimitMoney = m_vecMinimumMoney[nCount-1];
	}

	int nGameMode = m_pComboMode->GetCurSelectItem();
	int nGameRule = m_pComboRule->GetCurSelectItem();
		
	switch((int)GM().GetCurrentGameType())
	{
		case IDX_GAME_SP:
		case IDX_GAME_HL:
			if (nGameMode == 2 && // 게임 모드 - 일반
				(llEnterLimitMoney == 10 * MONEY_JO || llEnterLimitMoney == 30 * MONEY_JO)) // 입장 제한 - 10조 or 30조
			{
				return true;
			}
			break;
		case IDX_GAME_BD:
			if (llEnterLimitMoney == 30 * MONEY_JO || llEnterLimitMoney == 50 * MONEY_JO) // 입장 제한 - 30조 or 50조
			{
				return true;
			}
			break;
		case IDX_GAME_NP:
			if (nGameMode == 2 && // 게임 모드 - 조커
				(llEnterLimitMoney == 30 * MONEY_JO || llEnterLimitMoney == 50 * MONEY_JO)) // 입장 제한 - 30조 or 50조
			{
				return true;
			}
			break;
		case IDX_GAME_SD:
			if (nGameMode == 1 && // 게임 모드 - 2장
				(llEnterLimitMoney == 50 * MONEY_JO || llEnterLimitMoney == 100 * MONEY_JO)) // 입장 제한 - 50조 or 100조
			{
				return true;
			}
			break;
		case IDX_GAME_HA:
			if (nGameMode == 2 && // 게임 모드 - 조커
				nGameRule == 2 && // 게임 옵션 - 스톱 off 
				(llEnterLimitMoney == 30 * MONEY_JO || llEnterLimitMoney == 100 * MONEY_JO)) // 입장 제한 - 30조 or 100조
			{
				return true;
			}
			break;
	}
	return false;
}

void CSelectQuickstarterDlg::SaveOption()
{
	bool bCheck = m_pRandomCheckbox->IsCheck() ;

	int nCur = m_pComboLimit->GetCurSelectItem();
	int nCount =m_pComboLimit->GetItemData(nCur);

	INT64 llEnterLimitMoney = -1;
	if(-1 != nCount)
	{
		llEnterLimitMoney = m_vecMinimumMoney[nCount];
	}

	nCur = m_pComboMode->GetCurSelectItem();
	int nGameMode = m_pComboMode->GetItemData(nCur);

	nCur = m_pComboRule->GetCurSelectItem();
	int nGameRule = m_pComboRule->GetItemData(nCur);

	nCur = m_pComboSeedMoney->GetCurSelectItem();
	nCount = m_pComboSeedMoney->GetItemData(nCur);

	nCur = m_pComboBlind->GetCurSelectItem();
	int nBlind = FORMKIND_CHOICE;
	if(llEnterLimitMoney >= (INT64)10ll*MONEY_JO)
	{
		nBlind = m_pComboBlind->GetItemData(nCur);
	}

	INT64 llSeedMoney = -1;
	if(-1 != nCount)
	{
		llSeedMoney = m_vecSeedMoney[nCount];
	}
	int nGame = (int)GM().GetCurrentGameType();

	bool bDiaOption = false;
	if(true==GM().IsDiaMemberRooms(llEnterLimitMoney)){
		bDiaOption = m_pDiaCheckbox->IsCheck();
	}

	switch(nGame)
	{
	case IDX_GAME_HA:		
	case IDX_GAME_NP:
	case IDX_GAME_SP:
	case IDX_GAME_HL:
		GM().SaveSelectQuickStartOption(bCheck, llEnterLimitMoney, nGameMode, nGameRule, llSeedMoney, bDiaOption);
		break;
	case IDX_GAME_SD:
		GM().SaveSelectQuickStartOption(bCheck, llEnterLimitMoney, nGameMode, -1, llSeedMoney, bDiaOption);
		break;
	case IDX_GAME_BD:
		GM().SaveSelectQuickStartOption(bCheck, llEnterLimitMoney, nBlind, nGameRule, llSeedMoney, bDiaOption);
		break;
	}
}

