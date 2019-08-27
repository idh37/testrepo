#include "StdAfx.h"
#include "PlayerDraw_HL.h"
#include "UIInfoDataHighLow.h"
#include "CardRule.h"
#include "GlobalGame.h"

CPlayerDraw_HL::CPlayerDraw_HL(void)
{

	m_pLowJokboName			= NULL;
	m_bSelectedHLS			= false;
}

CPlayerDraw_HL::~CPlayerDraw_HL(void)
{

	if (m_pLowJokboName)
	{
		delete m_pLowJokboName;
		m_pLowJokboName=NULL;
	}


}


void CPlayerDraw_HL::Reset(void)
{
	CPlayerDraw::Reset();
	
	if(m_pLowJokboName != NULL)
	{		
		m_pLowJokboName->SetText("");
	}
	
	m_bSelectedHLS = false;

}


void CPlayerDraw_HL::Init(CPlayer *pPlayer, int nNum, NMBASE::GRAPHICGDI::CPage *pPage, CPlayerDrawManager *pManager)
{
	CPlayerDraw::Init(pPlayer, nNum, pPage, pManager);


	if(m_nPosType == EPT_ME)
	{	
		if (m_pJokboName)	//하이or 스윙 족보명
		{
			m_pJokboName->SetAlign(DT_RIGHT | DT_WORDBREAK);
			
		}

		if(m_pLowJokboName == NULL)	//로우 족보명
		{
			CRect rtRect = ((CUIInfoDataHighLow*)m_pUIInfoData)->m_rtLowJokoName;

			m_pLowJokboName = new NMBASE::SKINGDI::CNText(rtRect, "", 0, pPage, pManager->m_pGameView->m_pDC);
			m_pLowJokboName->SetFont(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));  
			m_pLowJokboName->SetColor(((CUIInfoDataHighLow*)m_pUIInfoData)->m_clrLowJokboNameColor);
			m_pLowJokboName->SetOutLine(m_pUIInfoData->m_clrJokboNameShadowColor);
			m_pLowJokboName->SetAlign(DT_LEFT | DT_WORDBREAK);
			m_pLowJokboName->SetPermitSameText(TRUE);	//족보
		}
	}

}

void CPlayerDraw_HL::InitUIData(void)
{
	CPlayerDraw::InitUIData();

	if(m_pLowJokboName) 
		m_pLowJokboName->ReSetPostion(((CUIInfoDataHighLow*)m_pUIInfoData)->m_rtLowJokoName);
	
}

void CPlayerDraw_HL::DrawJokboName(CDC *pDC)
{
	CPlayerDraw::DrawJokboName(pDC);

	//로우 족보 그리기
	if(m_pLowJokboName != NULL && !g_bMyObserver && m_pPlayer->JoinState != 0)
	{	
		m_pLowJokboName->DrawText(pDC);
	}	
}

void CPlayerDraw_HL::UpdateJokboName()
{
	BOOL bOnlySwing=FALSE;	//포카드 이상일 경우 스윙만 가능 (족보도 스윙만 표시해줌)

	//하이족보 업데이트
	if( m_pJokboName != NULL)
	{		
		CString strJokbo="";
		int nSevenValue=-1;
		MM().Call(UPG_CMK_GET_MYJOKBO_NAME, (WPARAM)&nSevenValue, (LPARAM)&strJokbo);
		bOnlySwing = (nSevenValue >= CCardRule::N7C_QUADS);

		if(!strJokbo.IsEmpty())
		{
			if (bOnlySwing)
			{
				strJokbo = "▲▼ " + strJokbo;
				m_pJokboName->SetColor(((CUIInfoDataHighLow*)m_pUIInfoData)->m_clrSwingJokboNameColor);
			}
			else
			{
				strJokbo = "▲ " + strJokbo;
				m_pJokboName->SetColor(((CUIInfoDataHighLow*)m_pUIInfoData)->m_clrJokboNameColor);
			}

			m_pJokboName->SetText(strJokbo);
		}
	}

	//로우 족보 업데이트
	if (bOnlySwing)
	{
		//스윙족보이면 로우 족보는 표시하지 않는다.
		m_pLowJokboName->SetText("");
	}
	else if(m_pLowJokboName != NULL)
	{		
		CString strJokbo="";
		MM().Call(UPG_CMK_GET_MY_LOWJOKBO_NAME, (WPARAM)m_nPNum, (LPARAM)&strJokbo);

		if(!strJokbo.IsEmpty())
		{
			strJokbo = "▼ " + strJokbo;
			m_pLowJokboName->SetText(strJokbo);
		}		
	}

	
}


void CPlayerDraw_HL::DrawResult(CDC *pDC)
{
	//확률 퍼센트 그리기
	CFont *oldf = pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));

	CPlayerDrawManager_HL *pManager_HL = (CPlayerDrawManager_HL*) m_pManager;
	if (pManager_HL==NULL) return;


	if(IsGameOver() && m_pManager->IsGameOver() && m_pPlayer->JoinState == 1)
	{
		// 내가 하이/ 로우 / 스윙 어떤걸로 이겼는지 찾는다.
		EHIGHLOWSWINGCASE eHLSCase = CASE_NULL;
		int nServWinner = pManager_HL->GetHighWinnerPNum();
		if (nServWinner>=0)
		{		
			if (m_nPNum == m_pGameProcess->GetPNum_ByServPN(nServWinner))
			{
				eHLSCase = CASE_HIGH;
			}
		}

		nServWinner = pManager_HL->GetLowWinnerPNum();
		if (nServWinner>=0)
		{		
			if (m_nPNum == m_pGameProcess->GetPNum_ByServPN(nServWinner))
			{
				eHLSCase = CASE_LOW;
			}
		}

		nServWinner = pManager_HL->GetSwingWinnerPNum();
		if (nServWinner>=0)
		{		
			if (m_nPNum == m_pGameProcess->GetPNum_ByServPN(nServWinner))
			{
				eHLSCase = CASE_SWING;
			}
		}

		const int OFFSET_MSCENE_NO_FOLDWIN = 0;		//기권승 멀티신 번호 오프셋
		const int OFFSET_MSCENE_NO_LOSE = 5;		//패배 멀티신 번호 오프셋
		const int OFFSET_MSCENE_NO_ALLIN = 10;		//올인패배 멀티신 번호 오프셋
		const int OFFSET_MSCENE_NO_LOW = 15;		//로우승 멀티신 번호 오프셋
		const int OFFSET_MSCENE_NO_HIGH = 20;		//하이승 멀티신 번호 오프셋
		const int OFFSET_MSCENE_NO_SWING = 25;		//스윙승 멀티신 번호 오프셋

		///eHLSCase에 어떤 걸로 이겼거나 졌는지 확인
		//eHLSCase가 CASE_NULL인 경우는 패배


		//각각의 경우에 따른 멀티씬 번호 찾기
		int nMSceneNo = -1;
		bool bWin = true;	//승인가? 패인가?
		if (eHLSCase == CASE_NULL) // 패배
		{
			bWin = false;
			if(m_pPlayer->UI.GetMoney() < 100)	//올인
			{
				nMSceneNo = m_nPNum + OFFSET_MSCENE_NO_ALLIN;
			}
			else
			{
				nMSceneNo = m_nPNum + OFFSET_MSCENE_NO_LOSE;
			}
		}
		else if (pManager_HL->IsOpen()==FALSE)	//기권승
		{
			nMSceneNo = m_nPNum + OFFSET_MSCENE_NO_FOLDWIN;
		}
		else if (eHLSCase == CASE_HIGH)			//하이승
		{
			nMSceneNo = m_nPNum + OFFSET_MSCENE_NO_HIGH;
		}
		else if (eHLSCase == CASE_LOW)			//로우승
		{
			nMSceneNo = m_nPNum + OFFSET_MSCENE_NO_LOW;
		}
		else if (eHLSCase == CASE_SWING)		//스윙승
		{
			nMSceneNo = m_nPNum + OFFSET_MSCENE_NO_SWING;
		}
			
		DWORD dwDeltaGab = timeGetTime() - m_pManager->GetGameOverStartTime();

		m_pPage->DrawMultiScene(0, 0, m_pManager->m_pWinLoseBackSpr, nMSceneNo, dwDeltaGab, FALSE);		


		CPoint ptBack = m_ptCardStartPos;

		if(m_nPNum == 0) ptBack += m_pUIInfoData->m_ptResultBackMyGab;
		else ptBack += m_pUIInfoData->m_ptResultBackYouGab;

		if(dwDeltaGab >= 200)
		{
			CPoint ptJokbo = ptBack + m_pUIInfoData->m_ptResultJokboNameGab;
			CRect rect = m_pUIInfoData->m_rtResultJokboYouSize;
			if(m_nPNum == 0) rect = m_pUIInfoData->m_rtResultJokboMySize;

			rect.OffsetRect(ptJokbo);

			if(bWin)
			{			
				pDC->SetTextColor(m_pUIInfoData->m_clrResultJokboNameWinColor);
			}
			else 
			{				
				pDC->SetTextColor(m_pUIInfoData->m_clrResultJokboNameLoseColor);
			}

			int nServPNum = GM().GetCurrentProcess()->GetServPNum_ByPN(m_nPNum);
			CString strJokbo= m_pManager->m_listResultJokbo[nServPNum];

			if(strJokbo == "")
			{
				if(bWin) strJokbo = "기권승";
				else strJokbo = "기권패";
			}

			pDC->DrawText(strJokbo, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

			CString rmstr = "";
			INT64 i64ResultMoney = m_pManager->m_listResultMoney[nServPNum];

			if(bWin)
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
			if(m_nPNum == 0) rect = m_pUIInfoData->m_rtResultMoneyMySize;
			rect.OffsetRect(ptResult);

			pDC->DrawText(rmstr, &rect,  DT_SINGLELINE | DT_CENTER | DT_VCENTER);

			pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));
		}
	}

	pDC->SelectObject(oldf);
}



//타이머 그리기
void CPlayerDraw_HL::DrawTimer(CDC *pDC)
{
	int nTestState = g_Poker.nState;
	int test = nTestState +1;
	
	if (g_Poker.nState==RSTATE_HIGHLOW_SELECT)
	{
		
		if ( m_bSelectedHLS==false)
		{		
			//하이로우 선택시 타이머
			
			//게임중 일 때만 그린다.
			if(g_RI.State != 1) return;

			m_pManager->m_dwTimerCurTime = timeGetTime();

			bool bTimer = false;

			if( m_pPlayer->PlayState==1 )
			{
				bTimer = true;
			}

			if(!bTimer) return;

			DWORD dwTimeGab = m_pManager->m_dwTimerCurTime - m_pManager->m_dwTimerStartTime;
			DWORD dwTimeLimit = m_pManager->m_sprTimer.GetMScenePlayTime(m_nPNum);

			if( dwTimeGab >= TURN_TIMER_WAIT_TIME && dwTimeGab <= dwTimeLimit + TURN_TIMER_WAIT_TIME )
			{
				int nIndex = (dwTimeGab - TURN_TIMER_WAIT_TIME) / 1000;
				if(!m_pManager->m_listClockFlag[nIndex])
				{
					m_pManager->m_listClockFlag[nIndex] = TRUE;
					PBPlayEffectSound(SND_TIMER);
				}

				m_pPage->DrawMultiScene(m_pUIInfoData->m_ptTimer.x,m_pUIInfoData->m_ptTimer.y,&m_pManager->m_sprTimer, m_nPNum, dwTimeGab - TURN_TIMER_WAIT_TIME);
			}

		}

		return;
	}

	
	
	


	CPlayerDraw::DrawTimer(pDC);

	
}

