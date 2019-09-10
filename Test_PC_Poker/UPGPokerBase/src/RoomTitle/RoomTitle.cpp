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

//����忡�� ������ ����
void CRoomTitle::MakeTitle(void)
{
	INT64 ResultSeedMoney = g_RI.CurSeedMoney ;		

	/////////////////////////
	CString rk,str,rt;
	CString rule, ruleStr;
// 	switch(g_RI.FormKind)
// 	{
// 	case 1:rk = "���̽� | ";break;			
// 	case 2:rk = "��ź | ";break;
// 	case 3:rk = "��Ŀ | ";break;
// 	default:rk = "�Ϲ� | ";break;
// 	}
// 

	str.Format("%d���� - %s", g_RI.RoomNum, g_RI.Title);

	m_strRoomTitle = str;
	if(m_pRoomTitle) m_pRoomTitle->SetText(m_strRoomTitle);

	if ( m_nGameType == IDX_GAME_HA)
	{
		str = g_GetSeedMoneyToText( m_nGameType, ResultSeedMoney );

		if(m_pRoomInfoMinimumMoney) m_pRoomInfoMinimumMoney->SetText(str);

		CString strLimitMoney;

		if ( g_RI.bIsFriendRoom )
		{
			str.Format("ģ��");
		}
		else if ( g_RI.llEnterLimitMoney <= 0)
		{
			strLimitMoney.Format("���Ѿ���");
		}
		else{
			if(1==g_RI.cMemberRoom){
				rt = (CString)NMBASE::UTIL::g_NumberToHangul(g_RI.llEnterLimitMoney);
				strLimitMoney.Format("%s ���̾ƹ�", (CString)NMBASE::UTIL::g_NumberToHangul(g_RI.llEnterLimitMoney ));
			}
			else{
				strLimitMoney = (CString)NMBASE::UTIL::g_NumberToHangul(g_RI.llEnterLimitMoney );
			}
		}
		str.Format( "%s | %d�� | %s | %s",strLimitMoney, g_RI.MaxUserNum, g_GetGameKindName(m_nGameType, g_RI.cRuleType), g_GetFormName(m_nGameType, g_RI.FormKind ) );

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
	//�ƽ���, ��������(buy in) ���� ��� Ÿ��Ʋ ����
	if ( g_RI.bIsMaxRoom )
	{
		//7��Ŀ�� ���
		if ( m_nGameType == IDX_GAME_SP )
		{
			if(g_RI.llEnterLimitMoney != 0)
				str.Format("%d���� - [����ӴϹ�]%s", g_RI.RoomNum, g_RI.Title);
		}
	}
	*/
	
	//������ ����
	m_strRoomTitle = str;
	if(m_pRoomTitle) m_pRoomTitle->SetText(m_strRoomTitle);
	CString seedMoneyStr;
	seedMoneyStr.Format("%s%s", NMBASE::UTIL::g_NumberToHangul(ResultSeedMoney), strChip_Count[g_RI.ChipKind]);

	rt.Format("%s/%s", seedMoneyStr, UPGGetAutoBettingRuleName(GM().GetCurrentGameType(), g_RI.nAutoBetKind,  (BETRULE_KIND)g_RI.sBetRuleKind, (RULETYPE)g_RI.cRuleType).GetString());

	//������ ī�弼�� �۾����� ���÷� ��� ī����� �������� ǥ��
	if(m_nGameType != IDX_GAME_HA){
		CString tmpStr;
		CString tmpRule;
		if(m_nGameType != IDX_GAME_BD)
		{
			if(m_nGameType == IDX_GAME_SD)
			{
				tmpStr.Format("%i��", g_RI.MaxUserNum);
				tmpRule.Format("%i��", g_RI.MaxUserNum);
			
			}else if(m_nGameType == IDX_GAME_NP)
			{
				if(g_RI.cCardSetting > 5){
					tmpStr.Format("%i�� | %i��", g_RI.cCardSetting, g_RI.MaxUserNum);
					tmpRule.Format("%i��", g_RI.cCardSetting);
				}
				else{
					tmpStr.Format("���� | %i��", g_RI.MaxUserNum);	
					tmpRule.Format("����");	
				}
			}else{

				if(g_RI.cCardSetting > 4){
					tmpStr.Format("%i�� | %i��", g_RI.cCardSetting, g_RI.MaxUserNum);
					tmpRule.Format("%i��", g_RI.cCardSetting);
				}
				else{
					tmpStr.Format("���� | %i��", g_RI.MaxUserNum);				
					tmpRule.Format("����");	
				}
			}
		}
		else
		{
			if(g_RI.cCardSetting == 0) // �⺻
			{
				/*tmpStr.Format("�⺻ | %i��", g_RI.MaxUserNum);*/
				tmpStr.Format("���� | %i��", g_RI.MaxUserNum);
				tmpRule.Format("����");

			}else // 1�ϵ� // ��ħ
			{
				tmpStr.Format("��ħ | %i��", g_RI.MaxUserNum);
				tmpRule.Format("��ħ");
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
					str.Format("%s ���̾ƹ�", rt);
				}
				else{
					str.Format("%s%s��", rt, strChip_Count[g_RI.ChipKind]);
				}
			}else
			{
				if(1==g_RI.cMemberRoom){
					str.Format("MAX ���̾ƹ�");
				}
				else{
					str.Format("MAX��");
				}
			}
		} 
		else
		{
			if(1==g_RI.cMemberRoom){
				str.Format("MAX ���̾ƹ�");
			}
			else{
				str.Format("MAX��");
			}
		}		
	}
	else if ( g_RI.bIsFriendRoom )
	{
		str.Format("ģ��");
	}
	else if(g_RI.llEnterLimitMoney == 0)
	{
		str.Format("���Ѿ���");
	}
	else
	{
		rt = (CString)NMBASE::UTIL::g_NumberToHangul(g_RI.llEnterLimitMoney);
		if(1==g_RI.cMemberRoom){
			str.Format("%s ���̾ƹ�", rt);
		}
		else{
			str.Format("%s%s��", rt, strChip_Count[g_RI.ChipKind]);
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