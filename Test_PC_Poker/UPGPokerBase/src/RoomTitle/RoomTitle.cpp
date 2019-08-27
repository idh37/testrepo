#include "stdafx.h"
#include "RoomTitle.h"
#include "../UIInfoData.h"

#include "../GlobalPokerBase.h"

CRoomTitle::CRoomTitle()
{
	m_pUIInfoData = NULL;
	m_pRoomTitle = NULL;
	m_pRoomInfoMinimumMoney = NULL;
	m_pRoomInfoRuleType = NULL;
	m_pRoomInfoPokerKind = NULL;
}

CRoomTitle::~CRoomTitle()
{
	if(m_pRoomTitle) delete m_pRoomTitle;
	m_pRoomTitle = NULL;

	if(m_pRoomInfoMinimumMoney) delete m_pRoomInfoMinimumMoney;
	m_pRoomInfoMinimumMoney = NULL;

	if(m_pRoomInfoRuleType) delete m_pRoomInfoRuleType;
	m_pRoomInfoRuleType = NULL;

	if(m_pRoomInfoPokerKind) delete m_pRoomInfoPokerKind;
	m_pRoomInfoPokerKind = NULL;
}

void CRoomTitle::LoadImage(IDX_GAME nGame)
{
	m_nGameType = nGame;

	string strPath;
	switch(nGame)
	{
	case IDX_GAME_NP:
	case IDX_GAME_SP:
	case IDX_GAME_HL:
	case IDX_GAME_SD:
		strPath = ".\\PokerCommon\\data\\roominfo_sp_np.spr";
		break;
	case IDX_GAME_BD:
		strPath = ".\\PokerCommon\\data\\roominfo_bd.spr";
		break;
	case IDX_GAME_HA:
		strPath = ".\\PokerCommon\\data\\roominfo_ha.spr";
		break;
	default:
		strPath = ".\\PokerCommon\\data\\roominfo_sp_np.spr";
		break;
	}
	AddLoadImageList(&m_sprRoomInfo,strPath.c_str(),true);
}

void CRoomTitle::Init(CUIInfoData *pUIInfoData, CGameViewBase *pGameView)
{
	m_pUIInfoData = pUIInfoData;

	if(m_pRoomTitle == NULL)
	{
		CRect rtTitle = m_pUIInfoData->m_rtRoomTitle;
		rtTitle.OffsetRect(m_pUIInfoData->m_ptRoomTitle);
		m_pRoomTitle = new NMBASE::SKINGDI::CNText(rtTitle, "", 0, &pGameView->Page, pGameView->m_pDC);
		m_pRoomTitle->SetFont(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));
		m_pRoomTitle->SetColor(m_pUIInfoData->m_clrRoomTitle);
		m_pRoomTitle->SetOutLine(m_pUIInfoData->m_clrRoomTitleOutLine);
		m_pRoomTitle->SetAlign(DT_SINGLELINE | DT_CENTER | DT_TOP);
	}

	if(m_pRoomInfoMinimumMoney == NULL)
	{
		CRect rtRect = m_pUIInfoData->m_listRoomInfoMinimumMoney[m_nGameType];

		m_pRoomInfoMinimumMoney = new NMBASE::SKINGDI::CNText(rtRect, "", 0, &pGameView->Page, pGameView->m_pDC);
		m_pRoomInfoMinimumMoney->SetFont(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_BOLD));
		m_pRoomInfoMinimumMoney->SetColor(m_pUIInfoData->m_clrRoomInfoMinimumMoney);

		m_pRoomInfoMinimumMoney->SetAlign(DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	}

	if(m_pRoomInfoRuleType == NULL)
	{
		CRect rtRect = m_pUIInfoData->m_listRoomInfoRule[m_nGameType];
		COLORREF clrText = m_pUIInfoData->m_listRoomInfoRuleColor[m_nGameType];

		m_pRoomInfoRuleType = new NMBASE::SKINGDI::CNText(rtRect, "", 0, &pGameView->Page, pGameView->m_pDC);
		m_pRoomInfoRuleType->SetFont(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));
		m_pRoomInfoRuleType->SetColor(clrText);
		m_pRoomInfoRuleType->SetOutLine(m_pUIInfoData->m_clrRoomInfoOutline);

		m_pRoomInfoRuleType->SetAlign(DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	}

	if(m_pRoomInfoPokerKind)
	{
		delete m_pRoomInfoPokerKind;
		m_pRoomInfoPokerKind = NULL;
	}
}

void CRoomTitle::Draw(NMBASE::GRAPHICGDI::CPage *pPage, CDC *pDC)
{
	pPage->DrawScene(0,0,&m_sprRoomInfo,0,0);

	if(m_pRoomTitle) m_pRoomTitle->DrawText(pDC);
	if(m_pRoomInfoMinimumMoney) m_pRoomInfoMinimumMoney->DrawText(pDC);
	if(m_pRoomInfoRuleType) m_pRoomInfoRuleType->DrawText(pDC);
	if(m_pRoomInfoPokerKind) m_pRoomInfoPokerKind->DrawText(pDC);
}

//경기장에서 방정보 설정
void CRoomTitle::MakeTitle(void)
{
	INT64 ResultSeedMoney = g_RI.CurSeedMoney ;		

	/////////////////////////
	CString rk,str,rt;
	CString rule, ruleStr;
// 	switch(g_RI.FormKind)
// 	{
// 	case 1:rk = "초이스 | ";break;			
// 	case 2:rk = "폭탄 | ";break;
// 	case 3:rk = "조커 | ";break;
// 	default:rk = "일반 | ";break;
// 	}
// 

	str.Format("%d번방 - %s", g_RI.RoomNum, g_RI.Title);

	m_strRoomTitle = str;
	if(m_pRoomTitle) m_pRoomTitle->SetText(m_strRoomTitle);

	if ( m_nGameType == IDX_GAME_HA)
	{
		str = g_GetSeedMoneyToText( m_nGameType, ResultSeedMoney );

		if(m_pRoomInfoMinimumMoney) m_pRoomInfoMinimumMoney->SetText(str);

		CString strLimitMoney;

		if ( g_RI.bIsFriendRoom )
		{
			str.Format("친구");
		}
		else if ( g_RI.llEnterLimitMoney <= 0)
		{
			strLimitMoney.Format("제한없음");
		}
		else{
			if(1==g_RI.cMemberRoom){
				rt = (CString)NMBASE::UTIL::g_NumberToHangul(g_RI.llEnterLimitMoney);
				strLimitMoney.Format("%s 다이아방", (CString)NMBASE::UTIL::g_NumberToHangul(g_RI.llEnterLimitMoney ));
			}
			else{
				strLimitMoney = (CString)NMBASE::UTIL::g_NumberToHangul(g_RI.llEnterLimitMoney );
			}
		}
		str.Format( "%s | %d인 | %s | %s",strLimitMoney, g_RI.MaxUserNum, g_GetGameKindName(m_nGameType, g_RI.cRuleType), g_GetFormName(m_nGameType, g_RI.FormKind ) );

		if ( g_RI.MaxUserNum == 3)
		{
			TRACE("g_RI.MaxUserNum == 3\n");
		}
		if(m_pRoomInfoRuleType) m_pRoomInfoRuleType->SetText(str);

		return;
	}


 	if(m_nGameType != IDX_GAME_BD) 
	{
		rk = g_GetFormName(m_nGameType, g_RI.FormKind);
		rk += " | ";
	}

	/*
	//맥스방, 입장제한(buy in) 방의 경우 타이틀 변경
	if ( g_RI.bIsMaxRoom )
	{
		//7포커인 경우
		if ( m_nGameType == IDX_GAME_SP )
		{
			if(g_RI.llEnterLimitMoney != 0)
				str.Format("%d번방 - [입장머니방]%s", g_RI.RoomNum, g_RI.Title);
		}
	}
	*/
	
	//방제목 설정
	m_strRoomTitle = str;
	if(m_pRoomTitle) m_pRoomTitle->SetText(m_strRoomTitle);
	CString seedMoneyStr;
	seedMoneyStr.Format("%s%s", NMBASE::UTIL::g_NumberToHangul(ResultSeedMoney), strChip_Count[g_RI.ChipKind]);

	rt.Format("%s/%s", seedMoneyStr, UPGGetAutoBettingRuleName(GM().GetCurrentGameType(), g_RI.nAutoBetKind,  (BETRULE_KIND)g_RI.sBetRuleKind, (RULETYPE)g_RI.cRuleType).GetString());

	//규제안 카드세팅 작업으로 베팅룰 대신 카드수와 방정원을 표현
	if(m_nGameType != IDX_GAME_HA){
		CString tmpStr;
		CString tmpRule;
		if(m_nGameType != IDX_GAME_BD)
		{
			if(m_nGameType == IDX_GAME_SD)
			{
				tmpStr.Format("%i인", g_RI.MaxUserNum);
				tmpRule.Format("%i인", g_RI.MaxUserNum);
			
			}else if(m_nGameType == IDX_GAME_NP)
			{
				if(g_RI.cCardSetting > 5){
					tmpStr.Format("%i구 | %i인", g_RI.cCardSetting, g_RI.MaxUserNum);
					tmpRule.Format("%i구", g_RI.cCardSetting);
				}
				else{
					tmpStr.Format("수동 | %i인", g_RI.MaxUserNum);	
					tmpRule.Format("수동");	
				}
			}else{

				if(g_RI.cCardSetting > 4){
					tmpStr.Format("%i구 | %i인", g_RI.cCardSetting, g_RI.MaxUserNum);
					tmpRule.Format("%i구", g_RI.cCardSetting);
				}
				else{
					tmpStr.Format("수동 | %i인", g_RI.MaxUserNum);				
					tmpRule.Format("수동");	
				}
			}
		}
		else
		{
			if(g_RI.cCardSetting == 0) // 기본
			{
				/*tmpStr.Format("기본 | %i인", g_RI.MaxUserNum);*/
				tmpStr.Format("수동 | %i인", g_RI.MaxUserNum);
				tmpRule.Format("수동");

			}else // 1일듯 // 아침
			{
				tmpStr.Format("아침 | %i인", g_RI.MaxUserNum);
				tmpRule.Format("아침");
			}
		}		
			
		rt.Format("%s/%s", seedMoneyStr, tmpStr);
		rule.Format("%s/%s", seedMoneyStr, tmpRule);
	}

	str.Format("%s%s",rk.GetString(), rt.GetString());
	ruleStr.Format("%s%s",rk.GetString(), rule.GetString());
	if(m_pRoomInfoRuleType) m_pRoomInfoRuleType->SetText(str);
	
	if( g_RI.bIsMaxRoom  )
	{
		if ( m_nGameType == IDX_GAME_SP )
		{
			if(g_RI.llEnterLimitMoney != 0)
			{
				rt = (CString)NMBASE::UTIL::g_NumberToHangul(g_RI.llEnterLimitMoney);

				if(1==g_RI.cMemberRoom){
					str.Format("%s 다이아방", rt);
				}
				else{
					str.Format("%s%s방", rt, strChip_Count[g_RI.ChipKind]);
				}
			}else
			{
				if(1==g_RI.cMemberRoom){
					str.Format("MAX 다이아방");
				}
				else{
					str.Format("MAX방");
				}
			}
		} 
		else
		{
			if(1==g_RI.cMemberRoom){
				str.Format("MAX 다이아방");
			}
			else{
				str.Format("MAX방");
			}
		}		
	}
	else if ( g_RI.bIsFriendRoom )
	{
		str.Format("친구");
	}
	else if(g_RI.llEnterLimitMoney == 0)
	{
		str.Format("제한없음");
	}
	else
	{
		rt = (CString)NMBASE::UTIL::g_NumberToHangul(g_RI.llEnterLimitMoney);
		if(1==g_RI.cMemberRoom){
			str.Format("%s 다이아방", rt);
		}
		else{
			str.Format("%s%s방", rt, strChip_Count[g_RI.ChipKind]);
		}
	}
	if(m_pRoomInfoMinimumMoney) m_pRoomInfoMinimumMoney->SetText(str);

	g_cVipJackpot.SetMininumMoney(GetRoomInfoMininumMoney());
	g_cVipJackpot.SetRuleType((char*)ruleStr.GetString());
}


char* CRoomTitle::GetRoomInfoMininumMoney()
{
	if(NULL!=m_pRoomInfoMinimumMoney){
		return m_pRoomInfoMinimumMoney->GetText();
	}
	return NULL;
}

char* CRoomTitle::GetRoomInfoRuleType()
{
	if(NULL!=m_pRoomInfoRuleType){
		return m_pRoomInfoRuleType->GetText();
	}
	return NULL;
}