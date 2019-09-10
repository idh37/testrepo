#include "StdAfx.h"
#include "PromotionTotalDdengab.h"
#include "PromotionManager.h"
#include "CardRule.h"

#include "../Sound.h"

CPromotionTotalDdengab::CPromotionTotalDdengab( CPromotionManager *pManager, int nType )
: CPromotionBase(pManager, nType)
{
	SetShowTab(true);
	string strDir = ".\\PokerCommon\\data\\Event\\TotalDdengab\\";
	_LOADSPRITEDEFINE(&m_sprBack, strDir, "evt_ddengab.spr");

	m_roomIdx = 0;
	m_isOnBtn = FALSE;
	m_isClickBtn = FALSE;
	m_isRightRoom = FALSE;
	m_isShowReward = FALSE;
	m_isMyReward = FALSE;

	m_listRewardPos.clear();
	m_listRewardPos.push_back(CPoint(447, 663));
	m_listRewardPos.push_back(CPoint(120, 447));
	m_listRewardPos.push_back(CPoint(120, 252));
	m_listRewardPos.push_back(CPoint(666, 252));
	m_listRewardPos.push_back(CPoint(666, 447));
	
	m_curRewardIdx = 0;
	m_scoreCode = 0;	// ��÷ ���ھ�. ����(CommMsgDef.h)�� m_scoreCode�ʹ� �ٸ�
	m_rewardMoney = 0;
	m_remainCnt = 0;
	m_premState = 0;
}

CPromotionTotalDdengab::~CPromotionTotalDdengab(void)
{
}

void CPromotionTotalDdengab::Accept_OutRoom()
{
	m_isShowReward = FALSE;
}

void CPromotionTotalDdengab::Accept_CreateRoom()
{
	SetCurrentRoomInfo();
}

void CPromotionTotalDdengab::Accept_EnterRoom()
{
	SetCurrentRoomInfo();
}

void CPromotionTotalDdengab::OnEnterRoom()
{
}

void CPromotionTotalDdengab::OnEndGame()
{
	m_isShowReward = FALSE;
}

void CPromotionTotalDdengab::DrawBottom(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage)
{
	// �ΰ��� ���� ���� ���� ��� ǥ��

	// ��ó�� �̺�Ʈ�� �ش��ϴ� ������ Ȯ��
	if(m_isRightRoom)
	{
		// ���� ���� Ÿ���� ������Ŀ �� ���
		if ( GM().GetCurrentGameType() == IDX_GAME_SP )
		{
			pPage->PutSprAuto(23, 40, &m_sprBack, m_isOnBtn ? (m_isClickBtn ? 17 : 16) : 15, AB);
		}
		// ���� ���� Ÿ���� ����Ŀ �� ���
		else if ( GM().GetCurrentGameType() == IDX_GAME_NP )
		{
			pPage->PutSprAuto(23, 40, &m_sprBack, m_isOnBtn ? (m_isClickBtn ? 9 : 8) : 7, AB);
		}
		// ���� ���� Ÿ���� ���̷ο� �� ���
		else if ( GM().GetCurrentGameType() == IDX_GAME_HL )
		{
			pPage->PutSprAuto(23, 40, &m_sprBack, m_isOnBtn ? (m_isClickBtn ? 25 : 24) : 23, AB);
		}
	}
}

void CPromotionTotalDdengab::DrawMiddle(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage)
{
}

void CPromotionTotalDdengab::DrawTop(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage)
{
	// ������ ��÷�Ǿ��� ���
	if(m_isShowReward)
	{
		// ���� ���� Ÿ���� ������Ŀ �� ���
		if ( GM().GetCurrentGameType() == IDX_GAME_SP )
		{
			// ���� ��÷ ��� ǥ��
			pPage->PutSprAuto(m_listRewardPos[m_curRewardIdx].x, m_listRewardPos[m_curRewardIdx].y, &m_sprBack, 14, AB);
			TextSetting(pDC, m_listRewardPos[m_curRewardIdx].x+102, m_listRewardPos[m_curRewardIdx].y+21);
		}
		// ���� ���� Ÿ���� ����Ŀ �� ���
		else if ( GM().GetCurrentGameType() == IDX_GAME_NP )
		{
			// ���� ��÷ ��� ǥ��
			pPage->PutSprAuto(m_listRewardPos[m_curRewardIdx].x, m_listRewardPos[m_curRewardIdx].y, &m_sprBack, 4, AB);
			TextSetting(pDC, m_listRewardPos[m_curRewardIdx].x+102, m_listRewardPos[m_curRewardIdx].y+21);
		}
		// ���� ���� Ÿ���� ���̷ο� �� ���
		else if ( GM().GetCurrentGameType() == IDX_GAME_HL )
		{
			// ���� ��÷ ��� ǥ��
			pPage->PutSprAuto(m_listRewardPos[m_curRewardIdx].x, m_listRewardPos[m_curRewardIdx].y, &m_sprBack, 22, AB);
			TextSetting(pDC, m_listRewardPos[m_curRewardIdx].x+102, m_listRewardPos[m_curRewardIdx].y+21);
		}
	}
}

BOOL CPromotionTotalDdengab::OnLButtonDownMiddle(int &x , int &y)
{
	if(m_isRightRoom)
	{
		m_isClickBtn = (x>23&&x<23+221&&y>40&&y<40+57);
	}
	return FALSE;
}

BOOL CPromotionTotalDdengab::OnLButtonUpMiddle(int &x , int &y)
{
	// �� �κ�
	if(y>597&&y<619)
	{
		// ���� ���� Ÿ���� ������Ŀ �� ���
		if ( GM().GetCurrentGameType() == IDX_GAME_SP )
		{
			// ù��° ��(�� �κ��� �ΰ��� ����)
			if(x>18&&x<153)
			{
				m_roomIdx = 0;
			}
			// �ι�° ��
			else if(x>152&&x<287)
			{
				m_roomIdx = 1;
			}
		}
		// ���� ���� Ÿ���� ����Ŀ �� ���
		else if ( GM().GetCurrentGameType() == IDX_GAME_NP )
		{
			// ù��° ��(�� �κ��� ������ ����)
			if(x>18&&x<109)
			{
				m_roomIdx = 0;
			}
			// �ι�° ��
			else if(x>108&&x<198)
			{
				m_roomIdx = 1;
			}
			// ����° ��
			else if(x>197&&x<287)
			{
				m_roomIdx = 2;
			}
		}
		// ���� ���� Ÿ���� ���̷ο� �� ���
		else if ( GM().GetCurrentGameType() == IDX_GAME_HL )
		{
			// ù��° ��(�� �κ��� �ΰ��� ����)
			if(x>18&&x<153)
			{
				m_roomIdx = 0;
			}
			// �ι�° ��
			else if(x>152&&x<287)
			{
				m_roomIdx = 1;
			}
		}
	}
	if(m_isRightRoom)
	{
		if(x>23&&x<23+221&&y>40&&y<40+57)
		{
			m_pManager->SetChatViewPage_ToEventTab();
			m_pManager->SetCurTabPromotion(CPromotionManager::EPT_POKER_REWARD_NEWPOKER);
		}
		m_isClickBtn = FALSE;
	}
	
	return FALSE;
}

BOOL CPromotionTotalDdengab::OnMouseMoveMiddle(int &x , int &y)
{
	if(m_isRightRoom)
	{
		m_isOnBtn = (x>23&&x<23+221&&y>40&&y<40+57);
	}
	return FALSE;
}

// �ΰ��� ���� �Ʒ� �̺�Ʈ �ǿ� ���� �̺�Ʈ �� �߰�
void CPromotionTotalDdengab::DrawEventTab(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage, const CPoint &ptStart)
{
	// ���� ���� Ÿ���� ������Ŀ �� ���
	if ( GM().GetCurrentGameType() == IDX_GAME_SP )
	{
		pPage->PutSprAuto(ptStart.x, ptStart.y, &m_sprBack, 11);
		pPage->PutSprAuto(ptStart.x+7, ptStart.y+61, &m_sprBack, 11+m_roomIdx+1);
	}
	// ���� ���� Ÿ���� ����Ŀ �� ���
	else if ( GM().GetCurrentGameType() == IDX_GAME_NP )
	{
		pPage->PutSprAuto(ptStart.x, ptStart.y, &m_sprBack, 0);
		pPage->PutSprAuto(ptStart.x+7, ptStart.y+61, &m_sprBack, m_roomIdx+1);
	}
	// ���� ���� Ÿ���� ���̷ο� �� ���
	if ( GM().GetCurrentGameType() == IDX_GAME_HL )
	{
		pPage->PutSprAuto(ptStart.x, ptStart.y, &m_sprBack, 19);
		pPage->PutSprAuto(ptStart.x+7, ptStart.y+61, &m_sprBack, 19+m_roomIdx+1);
	}
	
	// ���� ���� ��÷ �Ǿ��� ���
	if(m_isShowReward&&m_isMyReward)
	{
		if(m_scoreCode<1)
		{
			pPage->PutSprAuto(71, 636+20*m_premState, &m_sprBack, 6);
		}
		else if(m_scoreCode<4)
		{
			pPage->PutSprAuto(55+57*m_scoreCode, 636+20*m_premState, &m_sprBack, 5);
		}
	}
}

BOOL CPromotionTotalDdengab::OnPacketNotify(char* pName, int &Signal, int &TotSize, char *lpData)
{
	switch(Signal)
	{
	case SV_REWARD_TOTALPOKER_SUPERSCORE:
		{	
			CSV_REWARD_TOTALPOKER_SUPERSCORE msg;
			msg.Get( lpData, TotSize );
			m_isShowReward = TRUE;
			m_curRewardIdx = m_pManager->GetGameProcess()->GetPNum_ByServPN(*msg.m_UNum);
			m_scoreCode = GetScoreIdx(*msg.m_ScoreCode, *msg.m_ScoreSubCode);
			m_rewardMoney = *msg.m_RewardMoney;
			m_remainCnt = *msg.m_RemainDailyRewardCount;
			m_premState = GetPremIdx(*msg.m_UserGrade);
			m_isMyReward = FALSE;
			if(GM().GetMyInfo()->ServPNum==*msg.m_UNum)
			{
				m_isMyReward = TRUE;
				m_pManager->SetCurTabPromotion(CPromotionManager::EPT_POKER_REWARD_NEWPOKER);
			}
		}
		break;
	case SV_ACCEPT_EMERAGE:
		{
			m_isShowReward = FALSE;
		}
	}

	return FALSE;
}

void CPromotionTotalDdengab::SetCurrentRoomInfo()
{
	m_isShowReward = FALSE;

	if(g_RI.llEnterLimitMoney == 30000000000000)
	{
		m_roomIdx = 1;
		m_isRightRoom = TRUE;
	}
	else if(g_RI.llEnterLimitMoney == 50000000000000)
	{
		m_roomIdx = 2;
		m_isRightRoom = TRUE;
	}
	else
	{
		m_roomIdx = 0;
		if(g_RI.llEnterLimitMoney == 10000000000000)
		{
			m_isRightRoom = TRUE;
		}
		else
		{
			m_isRightRoom = FALSE;
		}
	}
}

void CPromotionTotalDdengab::TextSetting(CDC *pDC, int x, int y)
{
	CString strOutput;
	CFont *pOldFont = pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_BOLD));
	COLORREF temp = pDC->GetTextColor();
	UINT oldFlags = pDC->SetTextAlign(TA_CENTER);
	pDC->SetTextColor( RGB(188, 174, 138) );
	strOutput = NMBASE::UTIL::g_NumberToHangul(m_rewardMoney);
	pDC->TextOut(x, y, strOutput);
	int length = strOutput.GetLength();
	pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));
	pDC->SetTextAlign(TA_RIGHT);
	strOutput = GetScoreName(m_scoreCode);
	pDC->TextOut(x - 4*length, y, strOutput);
	pDC->SetTextAlign(TA_LEFT);
	strOutput.Format("(%dȸ����)", m_remainCnt);
	pDC->TextOut(x + 4*length, y, strOutput);
	pDC->SetTextColor( temp );
	pDC->SelectObject(pOldFont);
	pDC->SetTextAlign(oldFlags);
}

CString CPromotionTotalDdengab::GetScoreName(int nValue)
{
	CString strRet;

	// ���� ���� Ÿ���� ������Ŀ �� ���
	if ( GM().GetCurrentGameType() == IDX_GAME_SP )
	{
		switch(nValue)
		{
			case 0: 
				strRet = "�÷���";
				break;
			case 1: 
				strRet = "Ǯ�Ͽ콺";
				break;
			case 2: 
				strRet = "��ī��";
				break;
			case 3:
				strRet = "��Ƽ��";
				break;
		}
	}
	// ���� ���� Ÿ���� ����Ŀ �� ���
	else if ( GM().GetCurrentGameType() == IDX_GAME_NP )
	{
		switch(nValue)
		{
			case 0: 
				strRet = "Ǯ�Ͽ콺";
				break;
			case 1: 
				strRet = "��ī��";
				break;
			case 2: 
				strRet = "��Ƽ��";
				break;
			case 3:
				strRet = "��Ƽ��";
				break;
		}
	}
	// ���� ���� Ÿ���� ���̷ο� �� ���
	else if ( GM().GetCurrentGameType() == IDX_GAME_HL )
	{
		switch(nValue)
		{
			case 0: 
				strRet = "�÷���";
				break;
			case 1: 
				strRet = "Ǯ�Ͽ콺/6ž";
				break;
			case 2: 
				strRet = "��ī��/5ž";
				break;
			case 3:
				strRet = "��Ƽ��";
				break;
		}
	}

	return strRet;
}

/// <summary>
/// ��� ���� �ڵ带 ������� ��÷ ��ȣ ��ȯ
/// <summary>
/// <param name="nScore">��� ���� �ڵ�(��Ƽ��, ��Ƽ��, ��ī�� ���..)</param>
/// <param name="nSubScore">��� ���� ���� �ڵ�(Až, 2ž, 3ž ���..)</param>
/// <returns>��÷ ��ȣ</returns>
int CPromotionTotalDdengab::GetScoreIdx(int nScore, int nSubScore)
{
	int ret = 0;
	
	// ���� ���� Ÿ���� ������Ŀ �� ���
	if ( GM().GetCurrentGameType() == IDX_GAME_SP )
	{
		switch(nScore)
		{
		case CCardRule::N7C_FLUSH:		// �÷���
			ret = 0;
			break;
		case CCardRule::N7C_TITLE:		// Ǯ�Ͽ콺
			ret = 1;
			break;
		case CCardRule::N7C_QUADS:		// ��ī��
			ret = 2;
			break;
		case CCardRule::N7C_SFULSH:		// ��Ƽ�� �̻�
		case CCardRule::N7C_BSFLUSH:
		case CCardRule::N7C_CROYAL:		// ��Ƽ��
		case CCardRule::N7C_HROYAL:
		case CCardRule::N7C_DROYAL:
		case CCardRule::N7C_SROYAL:
			ret = 3;
			break;
		}
	}
	// ���� ���� Ÿ���� ����Ŀ �� ���
	else if ( GM().GetCurrentGameType() == IDX_GAME_NP )
	{
		switch(nScore)
		{
		case CCardRule::N7C_FLUSH:		// �÷���
			ret = 0;
			break;
		case CCardRule::N7C_QUADS:		// ��ī��
			ret = 1;
			break;
		case CCardRule::N7C_SFULSH:		// ��Ƽ�� �̻�
		case CCardRule::N7C_BSFLUSH:
			ret = 2;
			break;
		case CCardRule::N7C_CROYAL:		// ��Ƽ��
		case CCardRule::N7C_HROYAL:
		case CCardRule::N7C_DROYAL:
		case CCardRule::N7C_SROYAL:
			ret = 3;
			break;
		}
	}
	// ���� ���� Ÿ���� ���̷ο� �� ���
	else if ( GM().GetCurrentGameType() == IDX_GAME_HL )
	{
		switch(nScore)
		{
		case 0:		// �ο�
			{
				switch(nSubScore)
				{
				case 6:		// 6ž
					ret = 1;
					break;
				case 5:		// 5ž
					ret = 2;
					break;
				}
			}
			break;
		case CCardRule::N7C_FLUSH:		// �÷���
			ret = 0;
			break;
		case CCardRule::N7C_TITLE:		// Ǯ�Ͽ콺
			ret = 1;
			break;
		case CCardRule::N7C_QUADS:		// ��ī��
			ret = 2;
			break;
		case CCardRule::N7C_SFULSH:		// ��Ƽ�� �̻�
		case CCardRule::N7C_BSFLUSH:
		case CCardRule::N7C_CROYAL:		// ��Ƽ��
		case CCardRule::N7C_HROYAL:
		case CCardRule::N7C_DROYAL:
		case CCardRule::N7C_SROYAL:
			ret = 3;
			break;
		}
	}

	return ret;
}

int CPromotionTotalDdengab::GetPremIdx(int nValue)
{
	int ret = 0;

	switch(nValue)
	{
	case CODE_NEW_FAMILY_NONE:		// �Ϲ� �̻�
	case CODE_NEW_FAMILY_MINI:
	case CODE_NEW_FAMILY_SILVER:
		ret = 0;
		break;
	case CODE_NEW_FAMILY_GOLD:		// ��� �̻�
		ret = 1;
		break;
	case CODE_NEW_FAMILY_DIAMOND:	// ���̾Ƹ�� �̻�
		ret = 2;
		break;
	}
	return ret;
}