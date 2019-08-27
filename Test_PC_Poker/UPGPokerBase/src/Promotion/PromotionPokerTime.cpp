#include "stdafx.h"
#include "PromotionPokerTime.h"
#include <EventCommon.h>
#include "PromotionManager.h"
#include <CardRule.h>
#include "../src/GlobalBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CPromotionPokerTime::CPromotionPokerTime(CPromotionManager *pManager, int nType)
: CPromotionBase(pManager, nType), m_dwPokerTimeEffectStartTime(0)
{
	SetShowTab(true);
	string strDir = ".\\PokerCommon\\data\\Event\\PokerTime\\";

#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
	if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
	{
		_LOADSPRITEDEFINE(&m_sprEffect, strDir, "PokerTimeEffect_toctoc.spr");
		_LOADSPRITEDEFINE(&m_sprTab, strDir, "pokertime_tab_toctoc.spr");
	}
	else
	{
		_LOADSPRITEDEFINE(&m_sprEffect, strDir, "PokerTimeEffect.spr");
		_LOADSPRITEDEFINE(&m_sprTab, strDir, "pokertime_tab.spr");
	}
#else //UPGRADE_20120308_TOCTOC_CHANNELING
	_LOADSPRITEDEFINE(&m_sprEffect, strDir, "PokerTimeEffect.spr");
	_LOADSPRITEDEFINE(&m_sprTab, strDir, "pokertime_tab.spr");

#endif //UPGRADE_20120308_TOCTOC_CHANNELING
}

CPromotionPokerTime::~CPromotionPokerTime()
{

}

BOOL CPromotionPokerTime::OnLButtonDownBottom(int &x , int &y)
{
	if (x > 48 && x < 277 && y > 59 && y < 111)
	{
		if (g_RI.llEnterLimitMoney < 100000000 || g_RI.llEnterLimitMoney > 10000000000)
			return FALSE;

		if (IsActive() == false)
			return FALSE;

		// 포커타임 이벤트탭 활성화
		m_pManager->SetCurTabPromotion(CPromotionManager::EPT_POKERTIME);
		m_pManager->SetChatViewPage_ToEventTab();
		return TRUE;
	}

	return FALSE;
}

BOOL CPromotionPokerTime::OnLButtonUpBottom(int &x , int &y)
{
	return FALSE;
}

BOOL CPromotionPokerTime::OnMouseMoveBottom(int &x , int &y)
{
	return FALSE;
}

BOOL CPromotionPokerTime::OnLButtonDownMiddle(int &x , int &y)
{
	return FALSE;
}

BOOL CPromotionPokerTime::OnLButtonUpMiddle(int &x , int &y)
{
	return FALSE;
}

BOOL CPromotionPokerTime::OnMouseMoveMiddle(int &x , int &y)
{
	return FALSE;
}

BOOL CPromotionPokerTime::OnLButtonDownTop(int &x , int &y)
{
	return FALSE;
}

BOOL CPromotionPokerTime::OnLButtonUpTop(int &x , int &y)
{
	return FALSE;
}

BOOL CPromotionPokerTime::OnMouseMoveTop(int &x , int &y)
{
	return FALSE;
}

BOOL CPromotionPokerTime::OnEventTabLButtonDown(int &x , int &y)
{
	return FALSE;
}

BOOL CPromotionPokerTime::OnEventTabLButtonUp(int &x , int &y)
{
	return FALSE;
}

BOOL CPromotionPokerTime::OnEventTabMouseMove(int &x , int &y)
{
	return FALSE;
}


BOOL CPromotionPokerTime::OnPacketNotify(char* pName, int &Signal, int &TotSize, char *lpData)
{
	switch(Signal)
	{
	case OPCODE_SV_GTIME_START:
		{
			packet::TSendPacket< promotion::GTIME_ALRAM > GTime_Packet;
			GTime_Packet.Get( lpData, TotSize );
			promotion::GTIME_ALRAM* pAlaram = static_cast< promotion::GTIME_ALRAM* >( GTime_Packet.Various() );

			if( pAlaram->nTimeType == 0 )
			{
				SetEventState(EPTS_SEASON);
				SetActive(false);
			}
			else if (pAlaram->nTimeType == 1)
			{
				SetEventState(EPTS_POKERTIME);
				SetActive(true);

				// 포커타임 시작시에 시작 시간을 저장한다.
				m_dwPokerTimeEffectStartTime = timeGetTime();
			}
			else
				SetActive(false);
		}
		return TRUE;
	case OPCODE_SV_GTIME_ALRAM:
		{
			packet::TSendPacket< promotion::GTIME_ALRAM > GTime_Packet;
			GTime_Packet.Get( lpData, TotSize );
			promotion::GTIME_ALRAM* pAlaram = static_cast< promotion::GTIME_ALRAM* >( GTime_Packet.Various() );

			bool bOpen = MM().Call(UPG_CMK_GAME_RESULT_OPEN)?true:false;
			if( !bOpen ) 
			{
				return TRUE;
			}

			//당첨메세지
			if( GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME )
			{
				CString str;
				str.Format("포커타임 %s님 %s족보 승리 %s 지급\n", pAlaram->szNickName, GetRuleName(pAlaram->nGrade, pAlaram->nValue), NMBASE::UTIL::g_MoneyMark(pAlaram->llGiftMoney));
				AddGameInfoViewEdit(str, m_pManager->GetChatColor(ECC2_USEITEM));

				int nPNum = m_pManager->GetGameProcess()->GetPlayerPNum_ByNick(pAlaram->szNickName);
				//const int &nMaxPlayer = m_pManager->GetGameProcess()->GetMaxPlayer();
				int nMaxPlayer = m_pManager->GetGameProcess()->GetMaxPlayer();
				
				if(nPNum >= 0 && nPNum < nMaxPlayer)
				{
					ShowAniEffect(nPNum, GetRuleName(pAlaram->nGrade, pAlaram->nValue), NMBASE::UTIL::g_MoneyMark(pAlaram->llGiftMoney));

//					ShowMainMessageDlg( "머니지급");
				}

// 				char szMsg[ MAX_PATH ] ={ 0,};
// 
// 				sprintf( szMsg, "포커타임 - PNum[%d], MaxPlayer[%d]\n", nPNum, nMaxPlayer );
// 
// 				AddLobbyChatViewEdit( szMsg, RGB( 1, 1, 250 ) );
// 				AddGameChatViewEdit( szMsg , RGB(227, 227, 0) );

			}
		}
		return TRUE;
	case OPCODE_SV_GTIME_END:
		{
			packet::TSendPacket< promotion::GTIME_ALRAM > GTime_Packet;
			GTime_Packet.Get( lpData, TotSize );

			promotion::GTIME_ALRAM* pAlaram = 
				static_cast< promotion::GTIME_ALRAM* >( GTime_Packet.Various() );

			if( pAlaram->nTimeType == 0 )
			{
				SetEventState(EPTS_END);
				SetActive(false);
			}
			else
			{
				SetEventState(EPTS_SEASON);
				SetActive(true);
			}
		}
		return TRUE;
	}

	return FALSE;
}

CString CPromotionPokerTime::GetRuleName(int nJokbo, int nValue)
{
	//nValue가 1이면 하이족보 nValue가 2이면 로우족보
	CString strRet;

	if (nValue==2)
	{
		switch(nJokbo)
		{
		case 5: 
			strRet = "5탑";
			break;
		case 6: 
			strRet = "6탑";
			break;
		case 7: 
			strRet = "7탑";
			break;
		case 8: 
			strRet = "8탑";
			break;
		case 9: 
			strRet = "9탑";
			break;
		}
		return strRet;
	}

	switch(GM().GetCurrentGameType())
	{
	case IDX_GAME_BD:
		switch(nJokbo)
		{
		case 1:
			strRet = "골프";
			break;
		case 2:
			strRet = "쎄컨드";
			break;
		case 3:
			strRet = "써드";
			break;
		case 4:
			strRet.Format("%d메이드", nValue);
			break;
		}
		break;
	default:
		switch(nJokbo)
		{
		case CCardRule::N7C_SROYAL:
		case CCardRule::N7C_DROYAL:
		case CCardRule::N7C_HROYAL:
		case CCardRule::N7C_CROYAL:
			strRet = "로티플";
			break;
		case CCardRule::N7C_BSFLUSH:
		case CCardRule::N7C_SFULSH:
			strRet = "스티플";
			break;
		case CCardRule::N7C_QUADS:
			strRet = "포카드";
			break;
		case CCardRule::N7C_TITLE:
			strRet = "풀하우스";
			break;
		case CCardRule::N7C_FLUSH:
			strRet = "플러쉬";
			break;
		case CCardRule::N7C_MOUNT:
		case CCardRule::N7C_BACKS:
		case CCardRule::N7C_STRAIGHT:
			strRet = "스트레이트";
			break;
		}
		break;
	}

	return strRet;
}

void CPromotionPokerTime::ShowAniEffect(int p, CString strJokbo, CString strMoney)
{
	const int nMaxPlayer = m_pManager->GetGameProcess()->GetMaxPlayer();
	if(p<0 || p>=nMaxPlayer) return;

	EFFECT_PLAY_INFO info;
	info.m_dwStartTime = timeGetTime();
	info.m_dwEndTime = info.m_dwStartTime + m_sprEffect.GetMScenePlayTime(0);
	info.m_strJokbo = strJokbo;
	info.m_strMoney = strMoney;

	m_mapEffectPlayInfo[p] = info;
}

BOOL CPromotionPokerTime::OnCommand(WPARAM &wParam, LPARAM &lParam)
{
	return FALSE;
}

void CPromotionPokerTime::DrawBottom(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage)
{
	LONGLONG llTemp = g_RI.llEnterLimitMoney;
	if (g_RI.llEnterLimitMoney < 100000000 || g_RI.llEnterLimitMoney > 10000000000)
		return;

	if (IsActive() == false)
		return;

	DWORD dwCurTime = timeGetTime();

	// 포커타임 알림 이벤트를 보여준다.
	DWORD dwShowTimeEventBanner = dwCurTime - m_dwPokerTimeEffectStartTime;

	if (dwShowTimeEventBanner <= 3000)
		pPage->DrawMultiScene(0, 0, &m_sprEffect, 1, dwCurTime - m_dwPokerTimeEffectStartTime, FALSE);
	else
		pPage->DrawMultiScene(0, 0, &m_sprEffect, 1, 3000, FALSE);
}

void CPromotionPokerTime::DrawMiddle(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage)
{
}

void CPromotionPokerTime::DrawTop(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage)
{
	CFont *pOldFont = pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));
	COLORREF clrOld = pDC->SetTextColor(RGB(188, 149, 74));

	DWORD dwCurTime = timeGetTime();
/*
	// 포커타임 알림 이벤트를 보여준다.
	DWORD dwShowTimeEventBanner = dwCurTime - m_dwPokerTimeEffectStartTime;

	if (dwShowTimeEventBanner <= 3000)
		pPage->DrawMultiScene(0, 0, &m_sprEffect, 1, dwCurTime - m_dwPokerTimeEffectStartTime, FALSE);
	else
		pPage->DrawMultiScene(0, 0, &m_sprEffect, 1, 3000, FALSE);
*/
	if(m_mapEffectPlayInfo.size())
	{
		CUIInfoData *pUIInfoData = m_pManager->GetUIInfoData();

		map<int, EFFECT_PLAY_INFO>::iterator start = m_mapEffectPlayInfo.begin();
		map<int, EFFECT_PLAY_INFO>::iterator end = m_mapEffectPlayInfo.end();
		while(start != end)
		{
			if(start->second.m_dwStartTime <= dwCurTime)
			{
				if(start->second.m_dwEndTime <= dwCurTime)
				{
					start = m_mapEffectPlayInfo.erase(start);
					continue;
				}

				CPoint ptPos = pUIInfoData->m_listPokerTimeEffectPos[start->first];
				pPage->DrawMultiScene(ptPos.x, ptPos.y, &m_sprEffect, 0, dwCurTime - start->second.m_dwStartTime, FALSE);

				CRect rtRect = pUIInfoData->m_rtPokerTimeEffect;

				CString strJokboMoney;
				CSize sizeJokboMoney;
				strJokboMoney.Format("%s %s", start->second.m_strJokbo, start->second.m_strMoney);

				pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_12D_NORMAL));
				::GetTextExtentPoint(pDC->m_hDC, strJokboMoney, strJokboMoney.GetLength(), &sizeJokboMoney);

				int nX = (rtRect.Width() - (sizeJokboMoney.cx))/2;
				int nY = (rtRect.Height() - sizeJokboMoney.cy)/2;

				pDC->TextOut(ptPos.x + nX + 29, ptPos.y + nY + 21, strJokboMoney);
				
				/*
				CSize sizeJokbo, sizeMoney;

				pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));
				::GetTextExtentPoint(pDC->m_hDC, start->second.m_strJokbo, start->second.m_strJokbo.GetLength(), &sizeJokbo);
				pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_BOLD));
				::GetTextExtentPoint(pDC->m_hDC, start->second.m_strMoney, start->second.m_strMoney.GetLength(), &sizeMoney);

				int nX = (rtRect.Width() - (sizeJokbo.cx + sizeMoney.cx + 6))/2;
				int nY = (rtRect.Height() - sizeJokbo.cy)/2;

				pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));
				pDC->TextOut(ptPos.x + nX, ptPos.y + nY, start->second.m_strJokbo);

				nX += (sizeJokbo.cx + 6);
				nY = (rtRect.Height() - sizeMoney.cy)/2;

				pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_BOLD));
				pDC->TextOut(ptPos.x + nX, ptPos.y + nY, start->second.m_strMoney);
				*/
			}
			start++;
		}
	}

	pDC->SetTextColor(clrOld);
	pDC->SelectObject(pOldFont);
}

void CPromotionPokerTime::DrawEventTab(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage, const CPoint &ptStart)
{
	pPage->PutSprAuto(ptStart.x, ptStart.y, &m_sprTab, GM().GetCurrentGameType()) ; 	
}

int CPromotionPokerTime::GetPriorities(void)
{
	return 2;
}

void CPromotionPokerTime::Accept_CreateRoom()
{
	m_dwPokerTimeEffectStartTime = timeGetTime();
}

void CPromotionPokerTime::Accept_EnterRoom()
{
	m_dwPokerTimeEffectStartTime = timeGetTime();
}