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
	m_scoreCode = 0;	// 당첨 스코어. 서버(CommMsgDef.h)의 m_scoreCode와는 다름
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
	// 인게임 왼쪽 위에 땡값 배너 표시

	// 땡처리 이벤트에 해당하는 방인지 확인
	if(m_isRightRoom)
	{
		// 현재 게임 타입이 세븐포커 일 경우
		if ( GM().GetCurrentGameType() == IDX_GAME_SP )
		{
			pPage->PutSprAuto(23, 40, &m_sprBack, m_isOnBtn ? (m_isClickBtn ? 17 : 16) : 15, AB);
		}
		// 현재 게임 타입이 뉴포커 일 경우
		else if ( GM().GetCurrentGameType() == IDX_GAME_NP )
		{
			pPage->PutSprAuto(23, 40, &m_sprBack, m_isOnBtn ? (m_isClickBtn ? 9 : 8) : 7, AB);
		}
		// 현재 게임 타입이 하이로우 일 경우
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
	// 땡값에 당첨되었을 경우
	if(m_isShowReward)
	{
		// 현재 게임 타입이 세븐포커 일 경우
		if ( GM().GetCurrentGameType() == IDX_GAME_SP )
		{
			// 땡값 당첨 배너 표시
			pPage->PutSprAuto(m_listRewardPos[m_curRewardIdx].x, m_listRewardPos[m_curRewardIdx].y, &m_sprBack, 14, AB);
			TextSetting(pDC, m_listRewardPos[m_curRewardIdx].x+102, m_listRewardPos[m_curRewardIdx].y+21);
		}
		// 현재 게임 타입이 뉴포커 일 경우
		else if ( GM().GetCurrentGameType() == IDX_GAME_NP )
		{
			// 땡값 당첨 배너 표시
			pPage->PutSprAuto(m_listRewardPos[m_curRewardIdx].x, m_listRewardPos[m_curRewardIdx].y, &m_sprBack, 4, AB);
			TextSetting(pDC, m_listRewardPos[m_curRewardIdx].x+102, m_listRewardPos[m_curRewardIdx].y+21);
		}
		// 현재 게임 타입이 하이로우 일 경우
		else if ( GM().GetCurrentGameType() == IDX_GAME_HL )
		{
			// 땡값 당첨 배너 표시
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
	// 탭 부분
	if(y>597&&y<619)
	{
		// 현재 게임 타입이 세븐포커 일 경우
		if ( GM().GetCurrentGameType() == IDX_GAME_SP )
		{
			// 첫번째 탭(탭 부분을 두개로 나눔)
			if(x>18&&x<153)
			{
				m_roomIdx = 0;
			}
			// 두번째 탭
			else if(x>152&&x<287)
			{
				m_roomIdx = 1;
			}
		}
		// 현재 게임 타입이 뉴포커 일 경우
		else if ( GM().GetCurrentGameType() == IDX_GAME_NP )
		{
			// 첫번째 탭(탭 부분을 세개로 나눔)
			if(x>18&&x<109)
			{
				m_roomIdx = 0;
			}
			// 두번째 탭
			else if(x>108&&x<198)
			{
				m_roomIdx = 1;
			}
			// 세번째 탭
			else if(x>197&&x<287)
			{
				m_roomIdx = 2;
			}
		}
		// 현재 게임 타입이 하이로우 일 경우
		else if ( GM().GetCurrentGameType() == IDX_GAME_HL )
		{
			// 첫번째 탭(탭 부분을 두개로 나눔)
			if(x>18&&x<153)
			{
				m_roomIdx = 0;
			}
			// 두번째 탭
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

// 인게임 왼쪽 아래 이벤트 탭에 땡값 이벤트 탭 추가
void CPromotionTotalDdengab::DrawEventTab(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage, const CPoint &ptStart)
{
	// 현재 게임 타입이 세븐포커 일 경우
	if ( GM().GetCurrentGameType() == IDX_GAME_SP )
	{
		pPage->PutSprAuto(ptStart.x, ptStart.y, &m_sprBack, 11);
		pPage->PutSprAuto(ptStart.x+7, ptStart.y+61, &m_sprBack, 11+m_roomIdx+1);
	}
	// 현재 게임 타입이 뉴포커 일 경우
	else if ( GM().GetCurrentGameType() == IDX_GAME_NP )
	{
		pPage->PutSprAuto(ptStart.x, ptStart.y, &m_sprBack, 0);
		pPage->PutSprAuto(ptStart.x+7, ptStart.y+61, &m_sprBack, m_roomIdx+1);
	}
	// 현재 게임 타입이 하이로우 일 경우
	if ( GM().GetCurrentGameType() == IDX_GAME_HL )
	{
		pPage->PutSprAuto(ptStart.x, ptStart.y, &m_sprBack, 19);
		pPage->PutSprAuto(ptStart.x+7, ptStart.y+61, &m_sprBack, 19+m_roomIdx+1);
	}
	
	// 내가 땡값 당첨 되었을 경우
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
	strOutput.Format("(%d회남음)", m_remainCnt);
	pDC->TextOut(x + 4*length, y, strOutput);
	pDC->SetTextColor( temp );
	pDC->SelectObject(pOldFont);
	pDC->SetTextAlign(oldFlags);
}

CString CPromotionTotalDdengab::GetScoreName(int nValue)
{
	CString strRet;

	// 현재 게임 타입이 세븐포커 일 경우
	if ( GM().GetCurrentGameType() == IDX_GAME_SP )
	{
		switch(nValue)
		{
			case 0: 
				strRet = "플러쉬";
				break;
			case 1: 
				strRet = "풀하우스";
				break;
			case 2: 
				strRet = "포카드";
				break;
			case 3:
				strRet = "스티플";
				break;
		}
	}
	// 현재 게임 타입이 뉴포커 일 경우
	else if ( GM().GetCurrentGameType() == IDX_GAME_NP )
	{
		switch(nValue)
		{
			case 0: 
				strRet = "풀하우스";
				break;
			case 1: 
				strRet = "포카드";
				break;
			case 2: 
				strRet = "스티플";
				break;
			case 3:
				strRet = "로티플";
				break;
		}
	}
	// 현재 게임 타입이 하이로우 일 경우
	else if ( GM().GetCurrentGameType() == IDX_GAME_HL )
	{
		switch(nValue)
		{
			case 0: 
				strRet = "플러쉬";
				break;
			case 1: 
				strRet = "풀하우스/6탑";
				break;
			case 2: 
				strRet = "포카드/5탑";
				break;
			case 3:
				strRet = "스티플";
				break;
		}
	}

	return strRet;
}

/// <summary>
/// 결과 족보 코드를 기반으로 당첨 번호 반환
/// <summary>
/// <param name="nScore">결과 족보 코드(로티플, 스티플, 포카드 등등..)</param>
/// <param name="nSubScore">결과 족보 서브 코드(A탑, 2탑, 3탑 등등..)</param>
/// <returns>당첨 번호</returns>
int CPromotionTotalDdengab::GetScoreIdx(int nScore, int nSubScore)
{
	int ret = 0;
	
	// 현재 게임 타입이 세븐포커 일 경우
	if ( GM().GetCurrentGameType() == IDX_GAME_SP )
	{
		switch(nScore)
		{
		case CCardRule::N7C_FLUSH:		// 플러쉬
			ret = 0;
			break;
		case CCardRule::N7C_TITLE:		// 풀하우스
			ret = 1;
			break;
		case CCardRule::N7C_QUADS:		// 포카드
			ret = 2;
			break;
		case CCardRule::N7C_SFULSH:		// 스티플 이상
		case CCardRule::N7C_BSFLUSH:
		case CCardRule::N7C_CROYAL:		// 로티플
		case CCardRule::N7C_HROYAL:
		case CCardRule::N7C_DROYAL:
		case CCardRule::N7C_SROYAL:
			ret = 3;
			break;
		}
	}
	// 현재 게임 타입이 뉴포커 일 경우
	else if ( GM().GetCurrentGameType() == IDX_GAME_NP )
	{
		switch(nScore)
		{
		case CCardRule::N7C_FLUSH:		// 플러쉬
			ret = 0;
			break;
		case CCardRule::N7C_QUADS:		// 포카드
			ret = 1;
			break;
		case CCardRule::N7C_SFULSH:		// 스티플 이상
		case CCardRule::N7C_BSFLUSH:
			ret = 2;
			break;
		case CCardRule::N7C_CROYAL:		// 로티플
		case CCardRule::N7C_HROYAL:
		case CCardRule::N7C_DROYAL:
		case CCardRule::N7C_SROYAL:
			ret = 3;
			break;
		}
	}
	// 현재 게임 타입이 하이로우 일 경우
	else if ( GM().GetCurrentGameType() == IDX_GAME_HL )
	{
		switch(nScore)
		{
		case 0:		// 로우
			{
				switch(nSubScore)
				{
				case 6:		// 6탑
					ret = 1;
					break;
				case 5:		// 5탑
					ret = 2;
					break;
				}
			}
			break;
		case CCardRule::N7C_FLUSH:		// 플러쉬
			ret = 0;
			break;
		case CCardRule::N7C_TITLE:		// 풀하우스
			ret = 1;
			break;
		case CCardRule::N7C_QUADS:		// 포카드
			ret = 2;
			break;
		case CCardRule::N7C_SFULSH:		// 스티플 이상
		case CCardRule::N7C_BSFLUSH:
		case CCardRule::N7C_CROYAL:		// 로티플
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
	case CODE_NEW_FAMILY_NONE:		// 일반 이상
	case CODE_NEW_FAMILY_MINI:
	case CODE_NEW_FAMILY_SILVER:
		ret = 0;
		break;
	case CODE_NEW_FAMILY_GOLD:		// 골드 이상
		ret = 1;
		break;
	case CODE_NEW_FAMILY_DIAMOND:	// 다이아몬드 이상
		ret = 2;
		break;
	}
	return ret;
}