#include "stdafx.h"
#include "PlayerDraw.h"
#include "PlayerDrawManager.h"
#include "../GlobalPokerBase.h"
#include "../Sound.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CPlayerDraw::CPlayerDraw()
{
	m_nPosType				= EPT_UNKNOWN;
	m_pPlayer				= NULL;
	m_nPNum					= -1;
	m_pUIInfoData			= NULL;
	m_bShow					= false;
	m_bLock					= false;
	m_pName					= NULL;
	m_ptLevel				= CPoint(0,0);
	m_ptChar				= CPoint(0,0);
	m_ptName				= CPoint(0,0);
	m_ptBack				= CPoint(0,0);
	m_ptBoss				= CPoint(0,0);
	m_ptMaster				= CPoint(0,0);
	m_ptReservation			= CPoint(0,0);
	m_ptChangeUserInfoBack	= CPoint(0,0);
	m_rtAvatar				= CRect(0,0,0,0);
	m_ptSameIPMark			= CPoint(0,0);
	m_bSameIPMarkRight		= false;
	m_bClickAvatar			= false;
	m_bShowChangeUserInfo	= false;
	m_nBackIndex			= 0;
	m_pManager				= NULL;
	m_bExitReservation		= false;
	m_bGameOver				= false;
	m_ptCardStartPos		= CPoint(0,0);
	m_dwAllinAniStartTime	= 0;
	m_dwAllinAngelStartTime = 0;
	m_bSelectCard			= false;
	m_pGameProcess			= NULL;
	m_bDrawMyValue			= false;
	m_bObserverReservation	= false;
	m_pTotalBet				= NULL;
	m_pJokboName			= NULL;
	m_pBetMoney				= NULL;
	m_pGold					= NULL;
	m_pRakeBackGold			= NULL;
	m_pRakeBackGoldEvent	= NULL;
	m_bTurn					= false;
	m_bOverHaveMoney		= false;
	m_bLimitMoney			= false;
	m_dwCongNotiStartTime	= 0;
	m_strRakeBackGold		= "";
	m_dwRakeBackGoldStartTime	= 0;
	m_dwRakeBackGoldEventStartTime	= 0;
	m_dwGoldRewardStartTime = 0;
	m_pGoldReward			= NULL;
	m_pGoldMileage			= NULL;
	m_bShowGoldMileageInfo	= false;
	m_pGoldNotiId			= NULL;
	m_pGoldNotiValue		= NULL;
	m_dwGoldNotiStartTime	= 0;
	m_dwGoldInsuNotiStartTime = 0;
	m_bShowCafeReservedInfo = false;
	m_bShowCafeReservedPoint = false;
	m_pCafeReservedPoint	= NULL;
	
	InitializeCriticalSection(&m_cLockQueue);
}

CPlayerDraw::~CPlayerDraw()
{
	if (m_pName) delete m_pName;
	m_pName = NULL;

	if (m_pTotalBet) delete m_pTotalBet;
	m_pTotalBet = NULL;

	if (m_pJokboName) delete m_pJokboName;
	m_pJokboName = NULL;

	if (m_pBetMoney) delete m_pBetMoney;
	m_pBetMoney = NULL;	

	DeleteCriticalSection(&m_cLockQueue);
}

ENUM_POSTYPE CPlayerDraw::GetPosType(int nPNum)
{
	ENUM_POSTYPE nPosType = EPT_UNKNOWN;
	switch(m_nPNum)
	{
	case 0:
		nPosType = EPT_ME;
		break;
		//좌측
	case 1:
	case 2:
		nPosType = EPT_LEFT;
		break;
	case 3:
	case 4:
		nPosType = EPT_RIGHT;
		break;
	default:
		ASSERT(FALSE);
		return nPosType;
	}
	return nPosType;
}

void CPlayerDraw::OnExitRoom(void)
{
	m_bExitReservation = false;
	m_bObserverReservation = false;
	m_bClickAvatar = false;
	m_bShowChangeUserInfo = false;
	m_bLimitMoney = false;
	SetShow(false);
	Reset();
}

void CPlayerDraw::OnEnterRoom(void)
{
	m_bExitReservation = false;
	m_bObserverReservation = false;
	m_bClickAvatar = false;
	m_bShowChangeUserInfo = false;
	m_bLimitMoney = false;
	SetShow(true);
	Reset();
}

void CPlayerDraw::Reset(void)
{
	m_bGameOver = false;
	m_dwAllinAniStartTime = 0;
	m_bSelectCard = false;
	m_bDrawMyValue = false;
	m_bTurn = false;
	m_bLimitMoney = false;

	if (m_pTotalBet != NULL)
	{
		m_pTotalBet->SetText("");
	}

	if (m_pJokboName != NULL)
	{		
		m_pJokboName->SetText("");
	}

	if (m_pBetMoney != NULL)
	{
		m_pBetMoney->SetText("");
	}
}

void CPlayerDraw::Init(CPlayer *pPlayer, int nNum, NMBASE::GRAPHICGDI::CPage *pPage, CPlayerDrawManager *pManager)
{
	m_pPlayer		= pPlayer;
	m_pPage			= pPage;
	m_nPNum			= nNum;
	m_pManager		= pManager;
	m_pUIInfoData	= pManager->m_pUIInfoData;
	m_pGameProcess	= pManager->m_pGameProcess;

	m_nPosType = GetPosType(nNum);

	m_ptCardStartPos = m_pUIInfoData->m_listCardStartPos[m_nPNum];
	m_ptChar = m_pUIInfoData->m_listAvatarPos[m_nPNum];
	m_ptReservation = m_ptChar + m_pUIInfoData->m_ptReservation;
	m_ptChangeUserInfoBack = m_pUIInfoData->m_listAvatarPos[m_nPNum] + m_pUIInfoData->m_listUserChangeGameInfoBackPos[m_nPosType];	
	m_ptBack = m_pUIInfoData->m_listPlayerBackPos[m_nPNum];
	m_ptBoss = m_ptCardStartPos + m_pUIInfoData->m_ptlistBossMarkGab[m_nPosType];
	m_ptMaster = m_ptChar + m_pUIInfoData->m_ptlistHostMarkGab[m_nPosType];
	m_ptSameIPMark = m_ptChar + m_pUIInfoData->m_listSameIPGab[m_nPosType];
	m_bSameIPMarkRight = m_pUIInfoData->m_listSameIPMarkRight[m_nPosType];

	m_ptName = m_ptChar + m_pUIInfoData->m_ptUserNameGab;
	m_ptLevel = m_ptChar + m_pUIInfoData->m_listLevelGab[m_nPosType];

	m_rtAvatar = CRect(0, 0, m_pUIInfoData->m_szAvatar.cx, m_pUIInfoData->m_szAvatar.cy);
	m_rtAvatar.OffsetRect(m_ptChar);

	CRect rtRect = m_pUIInfoData->m_listUserMoneyRect[m_nPNum];
	switch(m_nPosType)
	{
	case EPT_ME:
		m_cMoneyDraw.Init(m_pPage,&pManager->m_sprMyMoney,rtRect, DT_LEFT, 2, true, 0, &pManager->m_sprMyGold);
		break;
	case EPT_LEFT:
		m_cMoneyDraw.Init(m_pPage,&pManager->m_sprUserMoney,rtRect, DT_RIGHT, 2, true, 0, &pManager->m_sprUserGold);
		break;
	case EPT_RIGHT:
		m_cMoneyDraw.Init(m_pPage,&pManager->m_sprUserMoney,rtRect, DT_LEFT, 2, true, 0, &pManager->m_sprUserGold);
		break;
	}

	if (m_nPosType == EPT_ME)
	{
		if (m_pTotalBet == NULL)
		{
			CRect rtRect = m_pUIInfoData->m_rtMyTotalBettingMoney;
			rtRect.OffsetRect(m_pUIInfoData->m_ptMyTotalBettingMoney);
			m_pTotalBet = new NMBASE::SKINGDI::CNText(rtRect, "", 0, pPage, pManager->m_pGameView->m_pDC);
			m_pTotalBet->SetFont(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_BOLD));
			m_pTotalBet->SetColor(m_pUIInfoData->m_clrMyTotalBettingMoneyText);
			m_pTotalBet->SetOutLine(m_pUIInfoData->m_clrMyTotalBettingMoneyOutline);
			m_pTotalBet->SetAlign(DT_SINGLELINE | DT_LEFT | DT_TOP);
		}

		if (m_pJokboName == NULL)
		{
			CRect rtRect = m_pUIInfoData->m_rtJokoName;

			m_pJokboName = new NMBASE::SKINGDI::CNText(rtRect, "", 0, pPage, pManager->m_pGameView->m_pDC);
			m_pJokboName->SetFont(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));
			m_pJokboName->SetColor(m_pUIInfoData->m_clrJokboNameColor);
			m_pJokboName->SetOutLine(m_pUIInfoData->m_clrJokboNameShadowColor);
			m_pJokboName->SetAlign(DT_CENTER | DT_WORDBREAK);
			m_pJokboName->SetPermitSameText(TRUE);	//족보
		}

		if (m_pGold == NULL)
		{
			CRect rtRect = m_pUIInfoData->m_ptGold;

			m_pGold = new NMBASE::SKINGDI::CNText(rtRect, "", 0, pPage, pManager->m_pGameView->m_pDC);
			m_pGold->SetFont(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));
			m_pGold->SetColor(m_pUIInfoData->m_clrGold);
			m_pGold->SetAlign(DT_SINGLELINE | DT_LEFT | DT_TOP);
		}

		if (m_pRakeBackGold == NULL)
		{
			CRect rtRect = m_pUIInfoData->m_ptGold;

			m_pRakeBackGold = new NMBASE::SKINGDI::CNText(rtRect, "", 0, pPage, pManager->m_pGameView->m_pDC);
			m_pRakeBackGold->SetFont(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));
			m_pRakeBackGold->SetColor(m_pUIInfoData->m_clrGoldDiff);
			m_pRakeBackGold->SetAlign(DT_SINGLELINE | DT_LEFT | DT_TOP);
		}

		if (m_pGoldReward == NULL)
		{
			CRect rtRect = CRect(498, 689, 498+110, 689+13);

			m_pGoldReward = new NMBASE::SKINGDI::CNText(rtRect, "", 0, pPage, pManager->m_pGameView->m_pDC);
			m_pGoldReward->SetFont(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_12D_NORMAL));
			m_pGoldReward->SetColor(m_pUIInfoData->m_clrGoldReward);
			m_pGoldReward->SetAlign(DT_SINGLELINE | DT_CENTER | DT_TOP);
		}

		if (m_pGoldNotiId == NULL)
		{
			CRect rtRect = CRect(360, 54, 360+120, 54+30);

			m_pGoldNotiId = new NMBASE::SKINGDI::CNText(rtRect, "", 0, pPage, pManager->m_pGameView->m_pDC);
			m_pGoldNotiId->SetFont(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_15_BOLD));
			m_pGoldNotiId->SetColor(RGB(255, 255, 255));
			m_pGoldNotiId->SetAlign(DT_SINGLELINE | DT_RIGHT | DT_TOP);
		}

		if (m_pGoldNotiValue == NULL)
		{
			CRect rtRect = CRect(360, 81, 360+160, 81+18);

			m_pGoldNotiValue = new NMBASE::SKINGDI::CNText(rtRect, "", 0, pPage, pManager->m_pGameView->m_pDC);
			m_pGoldNotiValue->SetFont(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_18_BOLD));
			m_pGoldNotiValue->SetColor(RGB(252, 255, 0));
			m_pGoldNotiValue->SetAlign(DT_SINGLELINE | DT_RIGHT | DT_TOP);
		}
	}
	
	// 골드 마일리지 텍스트
	if (m_pGoldMileage == NULL)
	{
		CRect rtRect = CRect(531, 691, 531 + 130, 691 + 20);
		
		switch (GM().GetCurrentGameType())
		{
		case IDX_GAME_SP:
		case IDX_GAME_HL:
			rtRect = CRect(531, 692, 531 + 130, 692 + 20);
			break;
		case IDX_GAME_NP:
			rtRect = CRect(522, 692, 522 + 130, 692 + 20);
			break;
		case IDX_GAME_BD:
			rtRect = CRect(560, 691, 560 + 130, 691 + 20);
			break;
		case IDX_GAME_HA:
			rtRect = CRect(531, 695, 531 + 130, 695 + 20);
			break;
		case IDX_GAME_SD:
			rtRect = CRect(547, 692, 547 + 130, 692 + 20);
			break;
		}
		
		m_pGoldMileage = new NMBASE::SKINGDI::CNText(rtRect, "", 0, pPage, pManager->m_pGameView->m_pDC);
		m_pGoldMileage->SetFont(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_12D_NORMAL));
		m_pGoldMileage->SetColor(RGB(214, 181, 74));
		m_pGoldMileage->SetAlign(DT_SINGLELINE | DT_CENTER | DT_TOP);
	}
	
	if (m_pBetMoney == NULL)
	{
		CRect rtRect = m_pUIInfoData->m_rtBettingMoney;
		rtRect.OffsetRect(m_pUIInfoData->m_listBettingMoneyPos[m_nPNum]);

		m_pBetMoney = new NMBASE::SKINGDI::CNText(rtRect, "", 0, pPage, pManager->m_pGameView->m_pDC);
		m_pBetMoney->SetFont(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_12D_NORMAL));
		m_pBetMoney->SetColor(m_pUIInfoData->m_clrBettingMoneyText);
		m_pBetMoney->SetOutLine(m_pUIInfoData->m_clrBettingMoneyOutline);
		if (m_nPosType == EPT_LEFT)	m_pBetMoney->SetAlign(DT_SINGLELINE | DT_TOP | DT_RIGHT);
		else m_pBetMoney->SetAlign(DT_SINGLELINE | DT_TOP | DT_LEFT);
	}
	m_bLimitMoney = false;

	// 카페 미가입 유저의 포인트 적립 텍스트
	if (m_pCafeReservedPoint == NULL)
	{
		CRect rtRect = CRect(355, 667, 355 + 70, 667 + 13);

		switch (GM().GetCurrentGameType())
		{
		case IDX_GAME_SP:
			rtRect = CRect(355, 667, 355 + 70, 667 + 13);
			break;
		case IDX_GAME_NP:
			rtRect = CRect(346, 667, 346 + 68, 667 + 13);
			break;
		case IDX_GAME_BD:
			rtRect = CRect(357, 667, 357 + 68, 667 + 13);
			break;
		case IDX_GAME_SD:
			rtRect = CRect(376, 667, 376 + 68, 667 + 13);
			break;
		case IDX_GAME_HL:
			rtRect = CRect(356, 667, 356 + 68, 667 + 13);
			break;
		case IDX_GAME_HA:
			rtRect = CRect(327, 671, 327 + 68, 671 + 13);
			break;
		}

		m_pCafeReservedPoint = new NMBASE::SKINGDI::CNText(rtRect, "", 0, pPage, pManager->m_pGameView->m_pDC);
		m_pCafeReservedPoint->SetFont(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_12D_NORMAL));
		m_pCafeReservedPoint->SetColor(RGB(189, 142, 53));
		m_pCafeReservedPoint->SetAlign(DT_SINGLELINE | DT_CENTER | DT_TOP);
	}
}

void CPlayerDraw::InitUIData(void)
{
	if (m_pJokboName) m_pJokboName->ReSetPostion(m_pUIInfoData->m_rtJokoName);
	m_cMoneyDraw.SetRect(m_pUIInfoData->m_listUserMoneyRect[m_nPNum]);
}

void CPlayerDraw::SetBettingMoney(int nBet, INT64 i64Money, bool bRaise)
{
	if (m_pBetMoney)
	{
		CString strOutput;
		switch(nBet)
		{
		case RAISE_LAST_CMD_FOLD: //다이
			break;
		case RAISE_LAST_CMD_CHECK: //체크
			strOutput = "체크  ";
			break;
		case RAISE_LAST_CMD_CALL:
			strOutput = "콜  ";
			break;
		case RAISE_LAST_CMD_PING:
			strOutput = "삥  ";
			break;
		case RAISE_LAST_CMD_DADANG:
			strOutput = "따당  ";
			break;
		case RAISE_LAST_CMD_QUARTER:
		case RAISE_LAST_CMD_HALF:
			strOutput = "하프  ";
			break;
		case RAISE_LAST_CMD_FULL:
			strOutput = "풀(Full)  ";
			break;
		case RAISE_LAST_CMD_ALLIN:
			strOutput = "올인  ";
			break;
		}

		CString strMoney;
		if (nBet != RAISE_LAST_CMD_FOLD && nBet != RAISE_LAST_CMD_NULL)
		{ 
			strMoney = (CString)NMBASE::UTIL::g_NumberToHangul(i64Money);
			strOutput += strMoney;

			strOutput.Append(strChip_Name[g_RI.ChipKind]);

			//규제안 : 베팅금액이 0이면 노출안한다.
			if (nBet == RAISE_LAST_CMD_CALL && i64Money == 0){
				strOutput = "";
			}
		}
		
		//규제안 관련해서 9조9000억 이상이면 베팅금액을 노출하는 것이 아니고 안내문구를 노출한다 2014.01
		if (i64Money >= GM().GetLossMaxMoneyForGame()){
			strOutput = "";
		}

		if (g_Poker.RU[g_Poker.LastPlayer].bMaxBet && GM().g_bFirstMax[g_Poker.LastPlayer] == false)
		{
			strOutput.Format("베팅 %s", strMoney);
		}

		//리미트 베팅
		if (nBet>=RAISE_LAST_CMD_LIMIT_1X && nBet<RAISE_LAST_CMD_MAX)
		{
			if (bRaise)
			{
				strOutput.Format("받고 %s 더", strMoney);
			}
			else
			{
				strOutput.Format("베팅 %s", strMoney);
			}
		}


		m_pBetMoney->SetText(strOutput);
	}
}

void CPlayerDraw::ClearBettingMoney(void)
{
	m_pBetMoney->SetText("");
}

void CPlayerDraw::DrawUserBack(CDC *pDC)
{
	if (m_nPNum != 0) m_pPage->PutSprAuto(m_ptBack.x, m_ptBack.y, &m_pManager->m_sprUserBack, m_nBackIndex, AB);
}

void CPlayerDraw::DrawTurn(CDC *pDC)
{
	if (!IsGameOver() && m_bTurn)
		m_pPage->DrawScene(0, 0, m_pManager->m_pTurnSpr, m_nPNum, timeGetTime());
}


void CPlayerDraw::DrawAvatar(CDC *pDC)
{
	// 캐릭터 그리기 아바타	
	if (!g_Config.bAvataAni){
		m_pPlayer->Avatar.ResetAnimation();
		m_pPlayer->Avatar.SetAnimationTime(0);
	}

	m_pPlayer->Avatar.Draw(m_ptChar.x, m_ptChar.y, m_ptChar.x + m_pUIInfoData->m_szAvatar.cx, m_ptChar.y + m_pUIInfoData->m_szAvatar.cy, (WORD*)m_pPage->lpBit, m_pPage->lPitch, (m_nPosType == EPT_RIGHT)?FALSE:TRUE);

}

void CPlayerDraw::DrawNickName(CDC *pDC)
{
	CString str= "";

	str.Format("%s", m_pPlayer->UI.NickName);

	if (m_pName == NULL)
	{
		CRect rtRect = m_pUIInfoData->m_rtUserName;
		rtRect.OffsetRect(m_ptName.x, m_ptName.y);

		m_pName = new NMBASE::SKINGDI::CNText(rtRect, (char *)(LPCTSTR)"", 0, m_pPage, pDC);
		m_pName->SetAlign(DT_SINGLELINE | DT_CENTER);
		m_pName->SetColor(m_pUIInfoData->m_clrUserNameColor);
		m_pName->SetOutLine(m_pUIInfoData->m_clrUserNameShadowColor);
		m_pName->SetFont(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));
		m_pName->SetText(str);
	}
	else
	{
		if ( str != m_pName->GetText() )
			m_pName->SetText(str);
	}

	m_pName->DrawText(pDC);
}

void CPlayerDraw::DrawLevel(CDC *pDC)
{
	INT64 i64TotalMoney = m_pPlayer->UI.GetTotalMoney();
	int nIcon = MM().Call(UPG_CMK_GET_MONEY_GRADE,  0, (LPARAM)(&i64TotalMoney));
	
	if (m_pManager && m_pPage)
		m_pPage->PutSprAuto(m_ptLevel.x, m_ptLevel.y, &m_pManager->m_sprLevel, nIcon);
}

void CPlayerDraw::DrawMoney(CDC *pDC)
{
	INT64 roundingoff = 0;

	if ( g_RI.bIsMaxRoom )
		roundingoff = m_pPlayer->UI.GetInMoney();
	else if ( g_RI.ChipKind == (int)ROOM_CHIPKIND_GOLD )
		roundingoff = m_pPlayer->nPlayGold;
	else
		roundingoff = m_pPlayer->UI.GetMoney(); 

	if (GM().GetPromotionData()->IsIng())
	{
		roundingoff -= GM().GetPromotionData()->llEffectReserved;
	}
	m_cMoneyDraw.SetMoney(roundingoff, false);
	m_cMoneyDraw.Draw(g_RI.ChipKind);
}

//yoo 2015.01 규제안
void CPlayerDraw::DrawLimitIcon(CDC* pDC)
{
	if (false==GM().IsShowLimitICon()) return;
	//DWORD dwTimeGab = m_pManager->m_dwTimerCurTime - m_pManager->m_dwTimerStartTime;

	if (true == m_bLimitMoney && 1 != g_Poker.RU[m_pGameProcess->GetServPNum_ByPN(m_nPNum)].bRealOverBet){	
		int nArray=0;
		if (4==GM().GetCurrentGameArray()){
			nArray=1;
		}
		if (m_pManager && m_pPage){
			if (EPT_ME==m_nPosType){
				m_pPage->DrawMultiScene(m_pUIInfoData->m_listLimitIcon[nArray][m_nPNum].x,m_pUIInfoData->m_listLimitIcon[nArray][m_nPNum].y, &m_pManager->m_sprLimitIcon, 0, timeGetTime());
			}else{
				m_pPage->DrawMultiScene(m_pUIInfoData->m_listLimitIcon[nArray][m_nPNum].x,m_pUIInfoData->m_listLimitIcon[nArray][m_nPNum].y, &m_pManager->m_sprLimitIcon, 1, timeGetTime());
			}
		}
	}
}

void CPlayerDraw::DrawLimitOverIcon(CDC* pDC)
{
	if (false==GM().IsShowLimitICon()) return;

	if (1 == g_Poker.RU[m_pGameProcess->GetServPNum_ByPN(m_nPNum)].bRealOverBet){
		int nArray=0;
		if (4==GM().GetCurrentGameArray()){
			nArray=1;
		}
		if (m_pManager && m_pPage){
			if (EPT_ME==m_nPosType){
				if (0==nArray){
					m_pPage->DrawMultiScene(324, 635, &m_pManager->m_sprLimitOverIcon, 0, timeGetTime());
				}else{
					m_pPage->DrawMultiScene(366, 675, &m_pManager->m_sprLimitOverIcon, 0, timeGetTime());
				}
			}
			else{
				m_pPage->DrawMultiScene(m_pUIInfoData->m_listLimitIcon[nArray][m_nPNum].x,m_pUIInfoData->m_listLimitIcon[nArray][m_nPNum].y, &m_pManager->m_sprLimitIcon, 1, timeGetTime());
			}
		}
	}
}

void CPlayerDraw::DrawChangeInfo(CDC *pDC)
{
	CRect rect;
	CString strM = "";
	INT64 roundingoff = 0;
	
	if (m_bShowChangeUserInfo){
		CFont *oldf = pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));
		int nCharactorExtra = SetTextCharacterExtra(*pDC, 0);
		
		CHANGE_USERINFO_ROOM *pChangeInfo = m_pPlayer->GetChangeRoomInfo();
		
		//if (!IsTournamentGame() && GM().GetCurrentGameType() != IDX_GAME_HA)
		//{
		//	if (pChangeInfo->llMaxLossMoney < GM().GetLossMaxMoneyForGame())
		//	{
		//		m_pPage->PutSprAuto(m_ptChangeUserInfoBack.x, m_ptChangeUserInfoBack.y, &m_pManager->m_sprChangeGameInfoBack, 1);
		//	}else
		//	{
		//		m_pPage->PutSprAuto(m_ptChangeUserInfoBack.x, m_ptChangeUserInfoBack.y, &m_pManager->m_sprChangeGameInfoBack, 0);
		//	}
		//}
		
		m_pPage->PutSprAuto(m_ptChangeUserInfoBack.x, m_ptChangeUserInfoBack.y, &m_pManager->m_sprChangeGameInfoBack, 0);
		
		rect = m_pUIInfoData->m_rtUserChangeGameInfoMoney;
		rect.OffsetRect(m_ptChangeUserInfoBack);		
		
		roundingoff = pChangeInfo->m_llChangeMoney;
		//roundingoff = (roundingoff/100)*100;
		
		CString strAdd;
		strM = (CString)NMBASE::UTIL::g_MoneyMark(roundingoff, 2);
		strM.Append(strChip_Name[g_RI.ChipKind]);
		
		if (roundingoff > 0)
		{
			strM.Insert(0,"+");
		}
		
		if (roundingoff >= 0)
		{
			pDC->SetTextColor(m_pUIInfoData->m_clrUserChangeGameInfoMoneyWin);
		}
		else
		{
			pDC->SetTextColor(m_pUIInfoData->m_clrUserChangeGameInfoMoneyLose);
		}
		
		pDC->DrawText(strM, &rect, DT_SINGLELINE | DT_LEFT | DT_TOP);
		
		rect = m_pUIInfoData->m_rtUserChangeGameInfoInfo;
		rect.OffsetRect(m_ptChangeUserInfoBack);
		pDC->SetTextColor(m_pUIInfoData->m_clrUserChangeGameInfoInfo);
		
		UINT nTotalVs = pChangeInfo->m_stChangeRecord.GetTotalNum();
		UINT nWin = pChangeInfo->m_stChangeRecord.GetWinNum();
		UINT nLose = pChangeInfo->m_stChangeRecord.GetLoseNum();
		UINT nDraw = pChangeInfo->m_stChangeRecord.GetFoldNum();
		
		strM.Format("%ld판(%ld승%ld패)", nTotalVs, nWin, nDraw+nLose);
		pDC->DrawText(strM, &rect, DT_SINGLELINE | DT_LEFT | DT_TOP);
		
		//if (!IsTournamentGame() && GM().GetCurrentGameType() != IDX_GAME_HA)
		//{
		//	if (pChangeInfo->llMaxLossMoney < GM().GetLossMaxMoneyForGame())
		//	{
		//		strM.Format("베팅가능 %s", (CString)NMBASE::UTIL::g_MoneyMark(pChangeInfo->llMaxLossMoney, 2));
		//		pDC->SetTextColor(m_pUIInfoData->m_clrUserChangeGameInfoLimitMoney);
		
		//		rect = m_pUIInfoData->m_rtUserChangeGameInfoLimit;
		//		rect.OffsetRect(m_ptChangeUserInfoBack);
		
		//		pDC->DrawText(strM, &rect, DT_SINGLELINE | DT_LEFT | DT_TOP);
		//	}
		//}
		
		pDC->SelectObject(oldf);
		SetTextCharacterExtra(*pDC, nCharactorExtra);
	}
}

void CPlayerDraw::DrawIPCheck(CDC *pDC)
{
	// ### [ 중복된 IP가 ] ###
	if (m_pPlayer->MyIpCheck >= 0)
	{
		int imageIndex = m_bSameIPMarkRight?0:12 + m_pPlayer->MyIpCheck;
		m_pPage->PutSprAuto(m_ptSameIPMark.x, m_ptSameIPMark.y, &m_pManager->m_sprWarning, ( timeGetTime()/500 )%2 + imageIndex);
	}
}

void CPlayerDraw::DrawMasterMark(CDC *pDC)
{
	// 방장 마크 그리기
	if (strcmp(g_RI.ID, m_pPlayer->UI.ID)==0)
	{
		int x = 0;

		int master = g_RI.RoomMasterType;

		if ( g_RI.RoomMasterType == 1 )
		{
			x = 1;
		}
		
		m_pPage->PutSprAuto(m_ptMaster.x, m_ptMaster.y, &m_pManager->m_sprHostBossDealer, x);
	}
}

void CPlayerDraw::DrawObserverReservationMark(CDC *pDC)
{
	// 관전 예약 그리기
	if (m_bObserverReservation)
	{
		m_pPage->PutSprAuto(m_ptReservation.x, m_ptReservation.y, &m_pManager->m_sprReservation, 1); 
	}
}

void CPlayerDraw::DrawExitReservationMark(CDC *pDC)
{
	// 나가기 예약 그리기
	if (m_bExitReservation)
	{
		m_pPage->PutSprAuto(m_ptReservation.x, m_ptReservation.y, &m_pManager->m_sprReservation, 0); 
	}
}

void CPlayerDraw::DrawRealMoney(CDC *pDC)
{
	// 나가기 예약 그리기
	if (m_bOverHaveMoney)
	{
		CPoint ptPos = m_cMoneyDraw.GetTopLeftPos() + m_pUIInfoData->m_ptOverHaveMoneyGab;
		CRect rtRect(0,1,148,19);
		rtRect.OffsetRect(ptPos);

		CFont *pOldFont = pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));
		COLORREF clrOld = pDC->SetTextColor(RGB(198,194,127));

		/*CString strOutput =	(CString)NMBASE::UTIL::g_NumberToHangul(m_pPlayer->UI.GetMoney(IsTournament()));*/
		
		INT64 chipMoney = 0;
		if ( g_RI.bIsMaxRoom )
			chipMoney = m_pPlayer->UI.GetInMoney();
		else if ( g_RI.ChipKind == (int)ROOM_CHIPKIND_GOLD )
			chipMoney = m_pPlayer->nPlayGold * GM().GetGoldValue();
		else
			chipMoney = m_pPlayer->UI.GetMoney();

		CString strOutput =	(CString)NMBASE::UTIL::g_NumberToHangul(chipMoney);

		m_pPage->PutSprAuto(ptPos.x, ptPos.y, &m_pManager->m_sprHaveMoneyToolTip, 0); 
		pDC->DrawText(strOutput,&rtRect,DT_SINGLELINE|DT_CENTER|DT_VCENTER);

		pDC->SetTextColor(clrOld);
		pDC->SelectObject(pOldFont);
	}
}

void CPlayerDraw::DrawTotalBetting(CDC *pDC)
{
	if (m_pTotalBet && g_Poker.RU[m_pPlayer->ServPNum].nRealBat > 0)
	{
		CString strOutput;

		//규제안 관련하여 한판의 최대 베팅액에 도달한 경우의 문구 변경 2014.01
		//strOutput.Format("총베팅액  %s", NMBASE::UTIL::g_NumberToHangul(g_Poker.RU[m_pPlayer->ServPNum].nRealBat));
		if (g_Poker.RU[m_pPlayer->ServPNum].nRealBat >= GM().GetLossMaxMoneyForGame() && GM().GetCurrentGameType() != IDX_GAME_HA && g_RI.ChipKind != 1){
			/*strOutput.Format("총베팅액  9조9000억");*/
			/*strOutput.Format("총베팅액  11조 7000억");*/
			/*strOutput.Format("총베팅액  15조 6000억");*/
			strOutput.Format("총베팅액  %s", NMBASE::UTIL::g_NumberToHangul(GM().GetLossMaxMoneyForGame()));

			m_pBetMoney->SetText("최대 베팅 한도 도달!");
			m_pBetMoney->DrawText(pDC);

		}
		else
		{
			strOutput.Format("총베팅액  %s%s", NMBASE::UTIL::g_NumberToHangul(g_Poker.RU[m_pPlayer->ServPNum].nRealBat), strChip_Count[g_RI.ChipKind]);
		}
		
		m_pTotalBet->SetText(strOutput);
		m_pTotalBet->DrawText(pDC);
	}
}

void CPlayerDraw::DrawBetting(CDC *pDC)
{
	if (m_pBetMoney && !m_bGameOver)
	{
		m_pBetMoney->DrawText(pDC);
	}
}

void CPlayerDraw::DrawJokboName(CDC *pDC)
{
	if (m_pJokboName != NULL && !g_bMyObserver /*&& !IsGameOver() && !m_pManager->IsGameOver()*/ && m_pPlayer->JoinState == 1)
	{	
		m_pJokboName->DrawText(pDC);
	}	
}

void CPlayerDraw::DrawGold(CDC *pDC)
{
	// 골드방이거나 1000억 미만 방에서는 그려주지 않는다.
	if (g_RI.llEnterLimitMoney < ONE_HUNDRED_BILLION || g_RI.ChipKind == 1 || g_bMyObserver)
		return;
	
	if (m_pGold != NULL)
	{
		CString strOutput;
		strOutput.Format("%s", NMBASE::UTIL::g_NumberToHangul(GM().GetMyGold()));
		m_pGold->SetText(strOutput);
		m_pGold->DrawText(pDC);

		m_pPage->PutSprAuto((int)m_pUIInfoData->m_ptGold.left-17, (int)m_pUIInfoData->m_ptGold.top, &m_pManager->m_sprGold, 0);
	}
}

/// <summary>
/// 이번 게임에서 적립된 골드 적림금 표시
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CPlayerDraw::DrawGoldMileage(CDC *pDC)
{
	// 골드방이 아닐경우 그려주지 않는다.
	//if (g_RI.ChipKind != ROOM_CHIPKIND_GOLD)
	//	return;

	//m_pPage->PutSprAuto((int)m_pUIInfoData->m_ptGold.left-17, (int)m_pUIInfoData->m_ptGold.top, &m_pManager->m_sprGold, 0);
	int nSpridx = GetGoldMileageIdx();
	
	// 테두리 반짝이는 효과 애니메이션 실행 시간
	DWORD playTime = m_pManager->m_sprGoldMileage.GetMScenePlayTime(1);
	DWORD dwNow = timeGetTime();
	
	// 적립된 골드 표시 이미지 + 테두리 반짝이는 효과
    if (dwNow < m_dwRakeBackGoldStartTime + playTime)
    {
        DWORD dwCurTime = dwNow - m_dwRakeBackGoldStartTime;
		m_pPage->DrawMultiScene(0, 0, &m_pManager->m_sprGoldMileage, nSpridx + 1, dwCurTime);
	}
	// 적립된 골드 표시 이미지
	else
	{
		m_pPage->DrawMultiScene(0, 0, &m_pManager->m_sprGoldMileage, nSpridx, dwNow, TRUE);
	}
	
	int nSpridx2 = GetGoldMileageIdx2();
	
	// 한도 초과 이미지
	if (GM().GetOverMoneyGB())
	{
		m_pPage->DrawMultiScene(0, 0, &m_pManager->m_sprGoldMileage, nSpridx2 + 1, dwNow, TRUE);
	}
	
	// 골드를 0000만0000골드 형식으로 표기(이미지 위에)
	INT64 nCurTempGold = GM().GetCurTempGold();
	int nGold_Remain = GM().GetGoldRemain();
	CString strOutput;
	
	if (nCurTempGold >= 10000)
	{
		INT64 nCurTempGold1 = nCurTempGold / 10000;
		int nCurTempGold2 = nCurTempGold % 10000;
		strOutput.Format("%I64d만%04d%s %d%s", nCurTempGold1, nCurTempGold2, strChip_Count[(int)ROOM_CHIPKIND_GOLD], nGold_Remain, strChip_Count[2]);
	}
	else
	{
		strOutput.Format("%I64d%s %d%s", nCurTempGold, strChip_Count[(int)ROOM_CHIPKIND_GOLD], nGold_Remain, strChip_Count[2]);
	}

	m_pGoldMileage->SetText(strOutput);
	m_pGoldMileage->DrawText(pDC);
}

/// <summary>
/// 골드 적립 spr 애니메이션 관련 데이터 초기화
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CPlayerDraw::SetPlayRakeBackGold(INT64 nGoldInt, int nGoldPrime)
{
	m_strRakeBackGold.Format("+%I64d.%d", nGoldInt, nGoldPrime);
	m_dwRakeBackGoldStartTime = timeGetTime();
}

/// <summary>
/// 적립금 적립 애니메이션(숫자)
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CPlayerDraw::DrawRakeBackGold(CDC *pDC)
{
	DWORD playTime = m_pManager->m_sprGoldPlus.GetMScenePlayTime(0);
    if (timeGetTime()<m_dwRakeBackGoldStartTime+playTime)
	{
        DWORD curTime = timeGetTime() - m_dwRakeBackGoldStartTime;
        m_pRakeBackGold->SetText(m_strRakeBackGold);
        long goldDiffX = 0;
        if (curTime > 1200)
        {
            goldDiffX = (1700 - curTime) * 40 / 500 + 20;
        }
        else if (curTime < 1700)
        {
            goldDiffX = 60;
        }
		
		CRect rtRect;
		rtRect = m_pUIInfoData->m_ptGold;
		//if (g_RI.ChipKind == ROOM_CHIPKIND_GOLD)
		//{
			rtRect.OffsetRect(m_pGold->m_Rt.right - rtRect.left + goldDiffX, m_pGold->m_Rt.top - rtRect.top + GetGoldMileageIdx()==6?-10:-7);
			m_pRakeBackGold->ReSetPostion(rtRect);
			m_pRakeBackGold->DrawText(pDC);
			m_pPage->DrawMultiScene((int)m_pGold->m_Rt.left, (int)m_pGold->m_Rt.top - 3, &m_pManager->m_sprGoldPlus, 2, curTime);
		//}
		//else
		//{
		//	rtRect.OffsetRect(goldDiffX, 0);
		//	m_pRakeBackGold->ReSetPostion(rtRect);
		//	m_pRakeBackGold->DrawText(pDC);
		//	m_pPage->DrawMultiScene((int)m_pUIInfoData->m_ptGold.left, (int)m_pUIInfoData->m_ptGold.top, &m_pManager->m_sprGoldPlus, 0, curTime);
		//}
    }
}

void CPlayerDraw::DrawBottom(CDC *pDC)
{
	if (m_bShow == false) return;
	if (strlen(m_pPlayer->UI.ID) == 0) return;

	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));

	DrawUserBack(pDC);
	DrawTurn(pDC);
	DrawBetting(pDC);
	DrawTotalBetting(pDC);
	DrawAvatar(pDC);
	DrawNickName(pDC);
	DrawLevel(pDC);
	DrawMoney(pDC);
	//yoo 2015.01 규제안
	DrawLimitIcon(pDC);
	DrawLimitOverIcon(pDC);
	DrawIPCheck(pDC);
	DrawMasterMark(pDC);
	DrawObserverReservationMark(pDC);
	DrawExitReservationMark(pDC);
	DrawJokboName(pDC);
	//DrawGold(pDC);
	DrawGoldMileage(pDC);
}

//타이머 그리기
void CPlayerDraw::DrawTimer(CDC *pDC)
{
	//게임중 일 때만 그린다.
	if (g_RI.State != 1) return;

	m_pManager->m_dwTimerCurTime = timeGetTime();

	bool bTimer = false;

	if ( m_bSelectCard == true && m_pPlayer->PlayState==1 )
	{
		bTimer = true;
	}
	// 턴타이머
	else if (m_pPlayer->PlayState==1 && m_pGameProcess->m_nCurPlayer == m_pPlayer->ServPNum && !m_pManager->m_bFlyWindCard ) // 카드 날리는 중에는 그리지 않는다.
	{
		bTimer = true;
	}

	if (!bTimer) return;

	DWORD dwTimeGab = m_pManager->m_dwTimerCurTime - m_pManager->m_dwTimerStartTime;
	DWORD dwTimeLimit = m_pManager->m_sprTimer.GetMScenePlayTime(m_nPNum);

	DWORD dwTime_Wait = (DWORD)TURN_TIMER_WAIT_TIME;

	if ( dwTimeGab >= dwTime_Wait && dwTimeGab <= dwTimeLimit + dwTime_Wait )
	{
		int nIndex = (dwTimeGab - dwTime_Wait) / 1000;
		if (!m_pManager->m_listClockFlag[nIndex])
		{
			m_pManager->m_listClockFlag[nIndex] = TRUE;
			PBPlayEffectSound(SND_TIMER);
		}
		
		m_pPage->DrawMultiScene(m_pUIInfoData->m_ptTimer.x,m_pUIInfoData->m_ptTimer.y,&m_pManager->m_sprTimer, m_nPNum, dwTimeGab - dwTime_Wait);
	}
}

void CPlayerDraw::DrawBossMark(CDC *pDC)
{
	//게임중 일 때만 그린다.
	if (g_RI.State != 1) return;
	
	//내가 보스인지 검사해서 보스를 그린다.
	//m_pManager->m_nWinnerPNum == -1 결과 패킷을 받았음을 의미함
	IDX_GAME eGameType = GM().GetCurrentGameType();
	if (!m_pManager->m_bFlyWindCard && !m_bGameOver && m_pManager->m_nWinnerPNum == -1)
	{
		int pnum = m_pGameProcess->GetPNum_ByServPN( g_Poker.nBossPNum ); // 카드 날리는 순서
		if (m_pPlayer->PlayState && (pnum == m_nPNum))
		{
			m_pPage->PutSprAuto(m_ptBoss.x, m_ptBoss.y, &m_pManager->m_sprHostBossDealer, (eGameType==IDX_GAME_SD)?5:3);
		}
	}
}

void CPlayerDraw::DrawAllinAni(CDC *pDC)
{
	//올인 애니 표시 한다 
	if (!m_pManager->IsGameOver() && m_dwAllinAniStartTime > 0)
	{			
		DWORD time = timeGetTime() - m_dwAllinAniStartTime;
		m_pPage->DrawMultiScene(0, 0, &m_pManager->m_sprAllin , m_nPNum, time, FALSE);
	}
}

void CPlayerDraw::DrawAllinAngel(CDC *pDC)
{
	//올인 천사 애니 표시 한다 
	if (m_dwAllinAngelStartTime > 0)
	{
		DWORD time = timeGetTime() - m_dwAllinAngelStartTime;
		if (m_pPage->DrawMultiScene(0, 0, &m_pManager->m_sprAllinAngel, 0, time, FALSE) == FALSE)
		{
			m_dwAllinAngelStartTime = 0;
		}
	}
}

void CPlayerDraw::SetPlayAllinAngel(int remainCount, DWORD dwStartTime)
{
	m_dwAllinAngelStartTime = dwStartTime;
	m_pManager->m_cEffect_AllinAngel.ResetEffect();
	m_pManager->m_cEffect_AllinAngel.ChangeScean(1, 5 + remainCount);
}

void CPlayerDraw::SetPlayCongNoti(std::string id, std::string data, int playTime)
{
	CThreadLock lock(&m_cLockQueue);
	if (m_dwCongNotiStartTime<=0)
	{
		m_dwCongNotiStartTime = timeGetTime();
	}
	CongDrawData drawData;
	drawData.id.Format("%s", id.c_str());
	drawData.message.Format("%s", data.c_str());
	drawData.time = playTime;
	m_CongQueData.push(drawData);
}

void CPlayerDraw::DrawGoldReward(CDC *pDC)
{
	//올인 천사 애니 표시 한다 
	if (m_dwGoldRewardStartTime > 0)
	{			
		DWORD time = timeGetTime() - m_dwGoldRewardStartTime;
		if (m_pPage->DrawMultiScene(0, 0, &m_pManager->m_sprGoldReward, 0, time, FALSE) == FALSE)
		{
			m_dwGoldRewardStartTime = 0;
		}
		else if (time>200 && time<3400)
		{
			m_pGoldReward->DrawText(pDC);
		}
	}
}

void CPlayerDraw::SetPlayGoldReward(INT64 rewardGold, DWORD dwStartTime)
{
	m_dwGoldRewardStartTime = dwStartTime;
	m_pManager->m_cEffect_GoldReward.ResetEffect();

	CString strOutput;
	strOutput.Format("%s%s 지급", NMBASE::UTIL::g_NumberToHangul(rewardGold), strChip_Name[(int)ROOM_CHIPKIND_GOLD]);
	m_pGoldReward->SetText(strOutput);
}

void CPlayerDraw::DrawMiddle(CDC *pDC)
{
	if (m_bShow == false) return;
	if (strlen(m_pPlayer->UI.ID) == 0) return;
	DrawTimer(pDC);	//타이머를 그린다.
	DrawBossMark(pDC);
	DrawAllinAni(pDC);
	DrawAllinAngel(pDC);
	DrawGoldReward(pDC);
}

void CPlayerDraw::DrawResult(CDC *pDC)
{
	//확률 퍼센트 그리기
	CFont *oldf = pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));
	
	if (IsGameOver() && m_pManager->IsGameOver() && m_pPlayer->JoinState == 1)
	{
		bool bWin = false;
		int nWinner = m_pGameProcess->GetPNum_ByServPN( m_pManager->m_nWinnerPNum );
		if (nWinner == m_nPNum) bWin = true;

		CPoint ptBack = m_ptCardStartPos;
		
		if (m_nPNum == 0) ptBack += m_pUIInfoData->m_ptResultBackMyGab;
		else ptBack += m_pUIInfoData->m_ptResultBackYouGab;

		DWORD dwDeltaGab = timeGetTime() - m_pManager->GetGameOverStartTime();

		int nIndex = m_nPNum;
		if (bWin == false)
		{
			nIndex += m_pManager->m_nMaxPlayer;
			if (g_RI.bIsMaxRoom)
			{
				if (m_pPlayer->UI.GetInMoney() < 1)
					nIndex += m_pManager->m_nMaxPlayer;
			}
			else if (g_RI.ChipKind==0 && m_pPlayer->UI.GetMoney() < 100) 
			{
				nIndex += m_pManager->m_nMaxPlayer;
			}
			else if (g_RI.ChipKind==1 && m_pPlayer->nRealGold < 1)
			{
				nIndex += m_pManager->m_nMaxPlayer;
			}
		}

		m_pPage->DrawMultiScene(0, 0, m_pManager->m_pWinLoseBackSpr, nIndex, dwDeltaGab, FALSE);		

		if (dwDeltaGab >= 200)
		{
			CPoint ptJokbo = ptBack + m_pUIInfoData->m_ptResultJokboNameGab;
			CRect rect = m_pUIInfoData->m_rtResultJokboYouSize;
			if (m_nPNum == 0) rect = m_pUIInfoData->m_rtResultJokboMySize;

			rect.OffsetRect(ptJokbo);

			if (bWin)
			{			
				pDC->SetTextColor(m_pUIInfoData->m_clrResultJokboNameWinColor);
			}
			else 
			{				
				pDC->SetTextColor(m_pUIInfoData->m_clrResultJokboNameLoseColor);
			}

			int nServPNum = GM().GetCurrentProcess()->GetServPNum_ByPN(m_nPNum);
			CString strJokbo= m_pManager->m_listResultJokbo[nServPNum];

			if (strJokbo == "")
			{
				if (bWin) strJokbo = "기권승";
				else strJokbo = "기권패";
			}

			pDC->DrawText(strJokbo, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

			CString rmstr = "";
			INT64 i64ResultMoney = m_pManager->m_listResultMoney[nServPNum];

			if (bWin)
			{
				rmstr.Format("+%s",NMBASE::UTIL::g_MoneyMark(i64ResultMoney, 2));
				pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_12D_BOLD));
				pDC->SetTextColor(m_pUIInfoData->m_clrResultMoneyWinColor);
			}
			else 
			{
				rmstr.Format("%s",NMBASE::UTIL::g_MoneyMark(i64ResultMoney, 2));
				pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_12D_NORMAL));
				pDC->SetTextColor(m_pUIInfoData->m_clrResultMoneyLoseColor);
			}

			rmstr.Append(strChip_Name[g_RI.ChipKind]);

			CPoint ptResult = ptBack + m_pUIInfoData->m_ptResultMoneyGab;

			rect = m_pUIInfoData->m_rtResultMoneyYouSize;
			if (m_nPNum == 0) rect = m_pUIInfoData->m_rtResultMoneyMySize;
			rect.OffsetRect(ptResult);

			pDC->DrawText(rmstr, &rect,  DT_SINGLELINE | DT_CENTER | DT_VCENTER);

			pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));
		}
	}

	pDC->SelectObject(oldf);
}

void CPlayerDraw::DrawCongNoti(CDC *pDC)
{
	CThreadLock lock(&m_cLockQueue);
	CFont *oldf = pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_12D_BOLD));
	pDC->SetTextColor(RGB(185,168,68));
	if (m_dwCongNotiStartTime>0)
	{
		CongDrawData data = m_CongQueData.front();
		m_pPage->PutSprAuto(5, 587, &m_pManager->m_sprTargetNoti, 2);
		CRect rect = CRect(0, 0, 168, 12);
		rect.OffsetRect(CPoint(31, 621));
		pDC->DrawText(data.id, &rect,  DT_SINGLELINE|DT_TOP|DT_LEFT );
		rect = CRect(0, 0, 243, 67);
		rect.OffsetRect(CPoint(29, 645));
		pDC->DrawText(data.message, &rect, DT_TOP|DT_LEFT );

		if (m_dwCongNotiStartTime+(DWORD)data.time*1000 < timeGetTime())
		{
			m_CongQueData.pop();
			if (m_CongQueData.empty())
			{
				m_dwCongNotiStartTime = 0;
			}
			else
			{
				m_dwCongNotiStartTime = timeGetTime();
			}
		}
	}
	pDC->SelectObject(oldf);
}

/// <summary>
/// 골드 적립 플러스 spr 초기화
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CPlayerDraw::SetPlayRakeBackGoldEvent(int nGoldMultiply)
{
	// 최대 99배
	if (nGoldMultiply > 99 || nGoldMultiply < 2)
	{
		return;
	}
	
	int nUnits = 0;	// nGoldMultiply의 일의자리 숫자(0 ~ 9)
	int nTens = 0;	// nGoldMultiply가 두자리수 숫자일때 십의자리 숫자(0 ~ 9)
	
	nUnits = nGoldMultiply % 10;
	nTens = nGoldMultiply / 10;
	
	// 이펙트 초기화
	m_pManager->m_cEffect_GoldPlus.ResetEffect();
	if (g_RI.ChipKind == 1)
		m_pManager->m_cEffect_GoldPlus.ShowEffect(3, TRUE, FALSE);
	else
		m_pManager->m_cEffect_GoldPlus.ShowEffect(1, TRUE, FALSE);
	
	// 멀티신 2번트랙을 (4 + nUnits)번 그림으로 변경 - 일의 자리수 변경
	m_pManager->m_cEffect_GoldPlus.ChangeScean(2, 4 + nUnits);
	// 멀티신 3번트랙을 3번 그림으로 변경 - 십의 자리수 변경
	if (nTens == 0)
		m_pManager->m_cEffect_GoldPlus.ChangeScean(3, 3);
	else
		m_pManager->m_cEffect_GoldPlus.ChangeScean(3, 4 + nTens);
	
	m_dwRakeBackGoldEventStartTime = timeGetTime();
}

/// <summary>
/// 골드 n배 이벤트 spr 애니메이션
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CPlayerDraw::DrawGoldEvent(CDC *pDC)
{
	DWORD playTime = m_pManager->m_sprGoldPlus.GetMScenePlayTime(1);
	if (timeGetTime() < m_dwRakeBackGoldEventStartTime + playTime)
	{
		DWORD curTime = timeGetTime() - m_dwRakeBackGoldEventStartTime;

		if (g_RI.ChipKind == 1)
			m_pPage->DrawMultiScene(m_pGold->m_Rt.right-108, 689, &m_pManager->m_sprGoldPlus, 3, curTime);
		else
			m_pPage->DrawMultiScene(m_pGold->m_Rt.right-110, m_pGold->m_Rt.top, &m_pManager->m_sprGoldPlus, 1, curTime);
	}
}

/// <summary>
/// 골드 적립금 마우스 오버시 띄우는 툴팁
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CPlayerDraw::DrawGoldMileageTooltip(CDC *pDC)
{
	int nSpridx = -1;
	
	// 일반(머니)방일 경우
	if (g_RI.ChipKind == ROOM_CHIPKIND_NORMAL)
	{
		nSpridx = GetGoldMileageIdx2();
		
		// 골드 적립금 마우스 오버시 띄우는 툴팁
		if (m_bShowGoldMileageInfo)
			m_pPage->DrawMultiScene(0, 0, &m_pManager->m_sprGoldMileage, nSpridx, timeGetTime(), TRUE);
	}
	// 골드방일 경우
	else if (g_RI.ChipKind == ROOM_CHIPKIND_GOLD)
	{
		nSpridx = GetGoldMileageIdx2();
		
		// 골드 적립금 마우스 오버시 띄우는 툴팁
		if (m_bShowGoldMileageInfo)
		{
			m_pPage->DrawMultiScene(0, 0, &m_pManager->m_sprGoldMileage, nSpridx, timeGetTime(), TRUE);
			//m_pPage->DrawMultiScene(0, 0, &m_pManager->m_sprGoldMileage, nSpridx + 2, timeGetTime(), TRUE);
		}
	}
	
	int nSpridx2 = GetGoldMileageIdx2();
	
	// 골드 적립 한도 초과시 띄우는 툴팁
	if (GM().GetOverMoneyGB() && m_bShowOverMoneyGB)
		m_pPage->DrawMultiScene(0, 0, &m_pManager->m_sprGoldMileage, nSpridx2 + 2, timeGetTime(), TRUE);
}

void CPlayerDraw::DrawGoldNoti(CDC *pDC)
{
	if (m_dwGoldNotiStartTime > 0)
	{
		DWORD time = timeGetTime() - m_dwGoldNotiStartTime;
		if (m_pPage->DrawMultiScene(0, 0, &m_pManager->m_sprGoldNoti, 0, time, FALSE) == FALSE)
		{
			m_dwGoldNotiStartTime = 0;
		}
		else if (time>200 && time<4800)
		{
			m_pGoldNotiId->DrawText(pDC);
			m_pGoldNotiValue->DrawText(pDC);
		}
	}
}

void CPlayerDraw::SetPlayGoldNoti(std::string id, INT64 rewardGold, DWORD dwStartTime)
{
	if(g_RI.bIsMaxRoom == true)
		return;

	m_dwGoldNotiStartTime = dwStartTime;

	CString strOutput;
	strOutput.Format("%s", id.c_str());
	m_pGoldNotiId->SetText(strOutput);
	strOutput.Format("%s", NMBASE::UTIL::g_NumberToHangul(rewardGold));
	m_pGoldNotiValue->SetText(strOutput);
}

void CPlayerDraw::DrawGoldInsuNoti(CDC *pDC)
{
	m_dwGoldInsuNotiStartTime = GM().GetGoldInsuNotiStartTime();
	INT64 i64Gold = GM().GetGoldInsuChangeChip();
	//INT64 i64Gold = 100; // test

	m_pManager->m_cEffect_GoldInsuNoti.ResetEffect();
	m_pManager->m_cEffect_GoldInsuNoti.ShowEffect(0, TRUE, FALSE);

	int n4 = (i64Gold / 1000) % 10; // 천의 자리수
	int n3 = (i64Gold / 100) % 10; // 백의 자리수
	int n2 = (i64Gold / 10) % 10; // 십의 자리수
	int n1 = (i64Gold / 1) % 10; // 일의 자리수

	// 멀티신 n번트랙을 m번 그림으로 변경 - 천의 자리수 변경
	if (i64Gold < 1000)
		m_pManager->m_cEffect_GoldInsuNoti.ChangeScean(1, 1 + 10);
	else
		m_pManager->m_cEffect_GoldInsuNoti.ChangeScean(1, 1 + n4);
	// 멀티신 n번트랙을 m번 그림으로 변경 - 백의 자리수 변경
	if (i64Gold < 100)
		m_pManager->m_cEffect_GoldInsuNoti.ChangeScean(2, 1 + 10);
	else
		m_pManager->m_cEffect_GoldInsuNoti.ChangeScean(2, 1 + n3);
	// 멀티신 n번트랙을 m번 그림으로 변경 - 십의 자리수 변경
	if (i64Gold < 10)
		m_pManager->m_cEffect_GoldInsuNoti.ChangeScean(3, 1 + 10);
	else
		m_pManager->m_cEffect_GoldInsuNoti.ChangeScean(3, 1 + n2);
	// 멀티신 n번트랙을 m번 그림으로 변경 - 일의 자리수 변경
	m_pManager->m_cEffect_GoldInsuNoti.ChangeScean(4, 1 + n1);
	
	if (m_dwGoldInsuNotiStartTime > 0)
	{
		DWORD time = timeGetTime() - m_dwGoldInsuNotiStartTime;
		if (m_pPage->DrawMultiScene(0, 0, &m_pManager->m_sprGoldInsuNoti, 0, time, FALSE) == FALSE)
		{
			m_dwGoldNotiStartTime = 0;
		}
	}
}

void CPlayerDraw::DrawCafeReservedPoint(CDC *pDC)
{
	if (GM().GetCafeReservedPoint() == 0)
		return;

	int nSpridx = GetCafeReservedPointIdx();

	DWORD dwNow = timeGetTime();
	m_pPage->DrawMultiScene(0, 0, &m_pManager->m_sprGoldMileage, nSpridx, dwNow);

	if(m_bShowCafeReservedInfo)
	{
		m_pPage->DrawMultiScene(0, 0, &m_pManager->m_sprGoldMileage, nSpridx+2, dwNow);
	}

	if(m_bShowCafeReservedPoint)
	{
		m_pPage->DrawMultiScene(0, 0, &m_pManager->m_sprGoldMileage, nSpridx+1, dwNow);

		INT64 nCurTempGold = GM().GetCafeReservedPoint();
		CString strOutput;

		if (nCurTempGold >= 10000)
		{
			INT64 nCurTempGold1 = nCurTempGold / 10000;
			int nCurTempGold2 = nCurTempGold % 10000;
			strOutput.Format("%I64d만%04d%s", nCurTempGold1, nCurTempGold2, strChip_Name[(int)ROOM_CHIPKIND_GOLD]);
		}
		else
		{
			strOutput.Format("%I64d%s", nCurTempGold, strChip_Name[(int)ROOM_CHIPKIND_GOLD]);
		}

		m_pCafeReservedPoint->SetText(strOutput);
		m_pCafeReservedPoint->DrawText(pDC);
	}
}

int CPlayerDraw::GetCafeReservedPointIdx()
{
	int idx = 0;

	switch (GM().GetCurrentGameType())
	{
	case IDX_GAME_SP:
		idx = 36;
		break;
	case IDX_GAME_NP:
		idx = 39;
		break;
	case IDX_GAME_BD:
		idx = 42;
		break;
	case IDX_GAME_SD:
		idx = 45;
		break;
	case IDX_GAME_HL:
		idx = 48;
		break;
	case IDX_GAME_HA:
		idx = 51;
		break;
	}

	return idx;
}

void CPlayerDraw::DrawTop(CDC *pDC)
{
	if (m_bShow == false) return;
	if (strlen(m_pPlayer->UI.ID) == 0) return;
	DrawRealMoney(pDC);
	DrawResult(pDC);
	DrawCongNoti(pDC);
	DrawRakeBackGold(pDC);
	DrawGoldEvent(pDC);
	DrawGoldMileageTooltip(pDC);
	DrawGoldNoti(pDC);
	DrawGoldInsuNoti(pDC);
	DrawCafeReservedPoint(pDC);
}

void CPlayerDraw::DrawTopMost(CDC *pDC)
{
	if (m_bShow == false) return;
	if (strlen(m_pPlayer->UI.ID) == 0) return;

	DrawChangeInfo(pDC);
}

BOOL CPlayerDraw::OnLButtonDown(int x, int y)
{
	if (m_bShow == false) return FALSE;

	CPoint ptPos = CPoint(x, y);
	if (m_rtAvatar.PtInRect(ptPos))
	{ 
		m_bClickAvatar = true;
		return TRUE;
	}
	else
	{
		m_bClickAvatar = false;
	}

	return FALSE;
}

BOOL CPlayerDraw::OnLButtonUp(int x, int y)
{
	if (m_bShow == false) return FALSE;
	
	CPoint ptPos = CPoint(x, y);
	
	if (m_bClickAvatar && m_rtAvatar.PtInRect(ptPos))
	{
		m_bClickAvatar = false;
		if (strlen(m_pPlayer->UI.ID)==0)
			return FALSE;
		
		CPlayer *pMyPlayer = GM().GetMyInfo();
		bool bMyProfile = (strcmp(m_pPlayer->UI.ID, pMyPlayer->UI.ID)==0)?true:false;
		
		if (bMyProfile == true)
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_MYCARDAREA_PROFILE);
			#endif //UPGRADE_10210619_ADD_CLICKLOG
		}
		else
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_OTHER_USER_PROFILE);
			#endif //UPGRADE_10210619_ADD_CLICKLOG
		}
		
		ShowProfileDialog(&m_pPlayer->UI, NULL, EPOK_ROOM_CLICK_AVATAR);
		return TRUE;
	}
	
	m_bClickAvatar = false;
	
	return FALSE;
}

BOOL CPlayerDraw::OnMouseMove(int x, int y)
{
	if (m_bShow == false) return FALSE;
	if (strlen(m_pPlayer->UI.ID)==0) return FALSE;
	
	CPoint ptPos(x,y);
	
	m_bOverHaveMoney = m_cMoneyDraw.IsIn(ptPos);
	bool bOver = false;

	if (m_rtAvatar.PtInRect(CPoint(x, y)))
	{
		bOver = true;
		m_bShowChangeUserInfo = true;
	}
	else
	{
		m_bShowChangeUserInfo = false;
	}

	if (m_bOverHaveMoney || bOver)
	{
		SetCursor(GM().GetCurHandCursor());
		return TRUE;
	}
	else
	{
		SetCursor( GM().GetCurSelCursor() );
	}

	
	switch (GM().GetCurrentGameType())
	{
	case IDX_GAME_SP:
		m_bShowGoldMileageInfo = (x > 429 && y > 687 && x < 429 + 226 && y < 687 + 20);
		m_bShowOverMoneyGB = (x > 430 && y > 687 && x < 430 + 18 && y < 687 + 19);
		m_bShowCafeReservedInfo = (x > 343 && y > 687 && x < 343 + 22 && y < 687 + 20);
		m_bShowCafeReservedPoint = (x > 343 + 22 && y > 687 && x < 343 + 85 && y < 687 + 20);
		break;
	case IDX_GAME_NP:
		m_bShowGoldMileageInfo = (x > 421 && y > 687 && x < 421 + 244 && y < 687 + 20);
		m_bShowOverMoneyGB = (x > 422 && y > 687 && x < 422 + 19 && y < 687 + 19);
		m_bShowCafeReservedInfo = (x > 333 && y > 687 && x < 333 + 22 && y < 687 + 20);
		m_bShowCafeReservedPoint = (x > 333 + 22 && y > 687 && x < 333 + 85 && y < 687 + 20);
		break;
	case IDX_GAME_BD:
		m_bShowGoldMileageInfo = (x > 461 && y > 687 && x < 461 + 197 && y < 687 + 20);
		m_bShowOverMoneyGB = (x > 462 && y > 687 && x < 462 + 19 && y < 687 + 19);
		m_bShowCafeReservedInfo = (x > 343 && y > 687 && x < 343 + 22 && y < 687 + 20);
		m_bShowCafeReservedPoint = (x > 343 + 22 && y > 687 && x < 343 + 85 && y < 687 + 20);
		break;
	case IDX_GAME_SD:
		m_bShowGoldMileageInfo = (x > 447 && y > 687 && x < 447 + 208 && y < 687 + 20);
		m_bShowOverMoneyGB = (x > 448 && y > 687 && x < 430 + 19 && y < 687 + 19);
		m_bShowCafeReservedInfo = (x > 363 && y > 687 && x < 363 + 22 && y < 687 + 20);
		m_bShowCafeReservedPoint = (x > 363 + 22 && y > 687 && x < 363 + 85 && y < 687 + 20);
		break;
	case IDX_GAME_HL:
		m_bShowGoldMileageInfo = (x > 429 && y > 690 && x < 429 + 226 && y < 690 + 20);
		m_bShowOverMoneyGB = (x > 430 && y > 690 && x < 430 + 18 && y < 690 + 19);
		m_bShowCafeReservedInfo = (x > 343 && y > 687 && x < 343 + 22 && y < 687 + 20);
		m_bShowCafeReservedPoint = (x > 343 + 22 && y > 687 && x < 343 + 85 && y < 687 + 20);
		break;
	case IDX_GAME_HA:
		m_bShowGoldMileageInfo = (x > 429 && y > 690 && x < 429 + 226 && y < 690 + 20);
		m_bShowOverMoneyGB = (x > 430 && y > 690 && x < 430 + 18 && y < 690 + 19);
		m_bShowCafeReservedInfo = (x > 313 && y > 691 && x < 313 + 22 && y < 691 + 20);
		m_bShowCafeReservedPoint = (x > 313 + 22 && y > 691 && x < 313 + 85 && y < 691 + 20);
		break;
	}
	
	return FALSE;
}


void CPlayerDraw::UpdateJokboName()
{
	if ( m_pJokboName != NULL)
	{		
		CString strJokbo="";
		MM().Call(UPG_CMK_GET_MYJOKBO_NAME, (WPARAM)m_nPNum, (LPARAM)&strJokbo);

		if (!strJokbo.IsEmpty())
		{
			m_pJokboName->SetText(strJokbo);
		}		
	}
}

/// <summary>
/// 게임별 골드 적립금 spr 애니메이션 인덱스 구하기(골드방)
/// 골드 적립금
/// <summary>
/// <param name=""></param>
/// <returns></returns>
int CPlayerDraw::GetGoldMileageIdx()
{
	int idx = 0;

	switch (GM().GetCurrentGameType())
	{
	case IDX_GAME_SP:
		idx = 0;
		break;
	case IDX_GAME_NP:
		idx = 3;
		break;
	case IDX_GAME_BD:
		idx = 6;
		break;
	case IDX_GAME_SD:
		idx = 9;
		break;
	case IDX_GAME_HL:
		idx = 24;
		break;
	case IDX_GAME_HA:
		idx = 30;
		break;
	}

	return idx;
}

/// <summary>
/// 게임별 골드 적립금 spr 애니메이션 인덱스 구하기
/// 툴팁, 적립 한도 초과
/// <summary>
/// <param name=""></param>
/// <returns></returns>
int CPlayerDraw::GetGoldMileageIdx2()
{
	int idx = 0;

	switch (GM().GetCurrentGameType())
	{
	case IDX_GAME_SP:
		idx = 21;
		break;
	case IDX_GAME_NP:
		idx = 18;
		break;
	case IDX_GAME_BD:
		idx = 15;
		break;
	case IDX_GAME_SD:
		idx = 12;
		break;
	case IDX_GAME_HL:
		idx = 27;
		break;
	case IDX_GAME_HA:
		idx = 33;
		break;
	}

	return idx;
}