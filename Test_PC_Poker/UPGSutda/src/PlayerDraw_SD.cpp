#include "StdAfx.h"
#include "PlayerDraw_SD.h"
#include "UIInfoDataSutda.h"
#include "CardRule.h"
#include "GlobalGame.h"

CPlayerDraw_SD::CPlayerDraw_SD(void)
{
	m_bShowPenaltyMoney = 0;
	m_TangDy = 0;
	m_TangDelayFlag = 0;
	
}

CPlayerDraw_SD::~CPlayerDraw_SD(void)
{

	


}


void CPlayerDraw_SD::Reset(void)
{
	CPlayerDraw::Reset();
	

	m_cJokboTable.ReSetJokboTable();
	
	m_bShowPenaltyMoney = 0;
	m_TangDy = 0;
	m_TangDelayFlag = 0;
}


void CPlayerDraw_SD::Init(CPlayer *pPlayer, int nNum, NMBASE::GRAPHICGDI::CPage *pPage, CPlayerDrawManager *pManager)
{
	CPlayerDraw::Init(pPlayer, nNum, pPage, pManager);


	if (nNum <0 || nNum>=MAX_PLAYER)
		return;
	

	m_cJokboTable.Init(&((CPlayerDrawManager_SD*) pManager)->m_JokboTableSpr);

	
	//나말고 상대 플레이어들의 족보텍스트 생성
	if(m_pJokboName == NULL)
	{
		CRect rtRect = ((CUIInfoDataSutda*)m_pUIInfoData)->m_listJokboNameRect[nNum];
	
		m_pJokboName = new NMBASE::SKINGDI::CNText(rtRect, "", 0, pPage, pManager->m_pGameView->m_pDC);
		m_pJokboName->SetFont(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));
		m_pJokboName->SetColor(m_pUIInfoData->m_clrJokboNameColor);
		m_pJokboName->SetOutLine(m_pUIInfoData->m_clrJokboNameShadowColor);
		m_pJokboName->SetPermitSameText(TRUE);	//족보
		
		if (m_nPosType==EPT_RIGHT)
			m_pJokboName->SetAlign(DT_LEFT | DT_WORDBREAK);
		else
			m_pJokboName->SetAlign(DT_RIGHT | DT_WORDBREAK);

		
		
	}

}

void CPlayerDraw_SD::InitUIData(void)
{
	CPlayerDraw::InitUIData();


	
}

void CPlayerDraw_SD::DrawJokboName(CDC *pDC)
{

// 	void CPlayerDraw::DrawJokboName(CDC *pDC)

	if(m_pJokboName != NULL && m_pPlayer->JoinState == 1)
	{	
		m_pJokboName->DrawText(pDC);
	}
	
}


void CPlayerDraw_SD::UpdateJokboName()
{
	
	//족보계산
	CPlayerSutda *pPlayer = (CPlayerSutda*)((CGameProcessSutda*) m_pGameProcess)->GetPlayer(m_nPNum);
	if (pPlayer==NULL)
		return;

	pPlayer->CalcJokbo();

	
	if( m_pJokboName != NULL)
	{		
		if (m_nPNum!=0)
			ClearBettingMoney();		//베팅머니가 찍히는곳에 족보가 찍히기때문에 베팅머니는 지워준다.

		m_pJokboName->SetText(pPlayer->GetJokboName());	
	}


	//CPlayerDraw::UpdateJokboName();

	if (m_nPNum==0)
	{
		//나의 족보명이 업뎃될 때 족보테이블도 같이 업뎃해준다.
		m_cJokboTable.ReSetJokboTable();
		vector<int> vecJokboIndex  = pPlayer->GetJokboIndex();
		for(int i=0; i<(int)vecJokboIndex.size(); i++)
		{
			m_cJokboTable.SetJokboTableIndex(vecJokboIndex[i]);
		}
	}
	
	
}


void CPlayerDraw_SD::DrawResult(CDC *pDC)
{

	//CPlayerDraw::DrawResult(pDC);
	
}


BOOL CPlayerDraw_SD::OnMouseMove(int x, int y)
{

	m_cJokboTable.OnMouseMove(x,y);

	return CPlayerDraw::OnMouseMove(x, y);

}


void CPlayerDraw_SD::DrawBottom(CDC *pDC)
{
	if(m_bShow == false) return;
	if(strlen(m_pPlayer->UI.ID) == 0) return;

	CPlayerDraw::DrawBottom(pDC);


	

	if (m_nPNum==0)
		m_cJokboTable.DrawBottom(m_pPage, pDC);

}
void CPlayerDraw_SD::DrawMiddle(CDC *pDC)
{
	if(m_bShow == false) return;
	if(strlen(m_pPlayer->UI.ID) == 0) return;
	
	CPlayerDraw::DrawMiddle(pDC);
}
void CPlayerDraw_SD::DrawTop(CDC *pDC)
{
	if(m_bShow == false) return;
	if(strlen(m_pPlayer->UI.ID) == 0) return;


	if (m_nPNum==0)
		m_cJokboTable.DrawTop(m_pPage, pDC);


	if (m_bShowPenaltyMoney)
		DrawPenaltyMoney(pDC);	//땡값

	CPlayerDraw::DrawTop(pDC);
}
void CPlayerDraw_SD::DrawTopMost(CDC *pDC)
{
	if(m_bShow == false) return;
	if(strlen(m_pPlayer->UI.ID) == 0) return;

	CPlayerDraw::DrawTopMost(pDC);

}

void CPlayerDraw_SD::DrawPenaltyMoney(CDC *pDC)	//땡값그리기
{

	if(m_pPage == NULL) return;
	if(m_bShow == false) return;
	if (m_bShowPenaltyMoney==false) return;
	if(m_nPNum < 0) return;
	
	m_TangDelayFlag ^= 1;
	if(m_TangDelayFlag) m_TangDy++;

	INT64 tang = ((CPlayerSutda*)m_pPlayer)->m_llPenaltyMoney;

	if(m_bShowPenaltyMoney == false || tang == 0 || m_TangDy > 100 || strlen(m_pPlayer->UI.ID) == 0)
	{
		m_TangDy = 0;
		m_bShowPenaltyMoney = FALSE;
		return;
	}

	int xp = ((CUIInfoDataSutda*)m_pUIInfoData)->m_listPenaltyMoneyPos[m_nPNum].x;	//땡값 좌표
	int yp = ((CUIInfoDataSutda*)m_pUIInfoData)->m_listPenaltyMoneyPos[m_nPNum].y;


	char tstr[30]={0,};
	if(tang<=0) sprintf(tstr, "%I64d", tang);
	if(tang>0) sprintf(tstr, "+%I64d", tang);

	int slen = strlen(tstr);
	int totwide = slen * 18;
	int sx = xp-totwide/2, sy=yp;
//	int grade = max(0, min(31, 31-(TangDy)/5));
	int strXp = 160;
	if(g_RI.ChipKind == (int)ROOM_CHIPKIND_GOLD)
		strXp = 120;

	if(tang<0)
	{
		m_pPage->PutSprAuto( xp, yp, &g_sprPenaltyRed , 13, HB);
		m_pPage->PutSprAuto( xp + 35, yp + 5, &g_sprPenaltyRed , 14) ;
		//pPage->PutSprAuto( xp + 160, yp+ 25, &RedNumSpr , 12) ; 	//원단위 삭제
		if(g_RI.ChipKind == (int)ROOM_CHIPKIND_GOLD)
			m_pPage->PutSprAuto( xp + strXp, yp+ 25, &g_sprPenaltyRed , 15); 
	}
	else
	{
		m_pPage->PutSprAuto( xp, yp, &g_sprPenaltyBlue , 13, HB) ;
		m_pPage->PutSprAuto( xp+ 35, yp+ 5, &g_sprPenaltyBlue , 14) ;
		//pPage->PutSprAuto( xp+ 160, yp+ 25, &BlueNumSpr , 12) ;		//원단위 삭제
		if(g_RI.ChipKind == (int)ROOM_CHIPKIND_GOLD)
			m_pPage->PutSprAuto( xp + strXp, yp+ 25, &g_sprPenaltyBlue , 16); 	//원단위 삭제
	}

	int kk=0;	//콤마찍어주기 
	//for( int i = tstr.GetLength()-1; i >= 0; i--)
	//for(int i=0; i<slen; i++)
	int addppint = 0;
	for(int i=slen-1; i>=0; i--)
	{
		int spn = tstr[i] - '0';
		if(tstr[i] == '+' || tstr[i] == '-') spn = 10;

		//콤마찍기
		if( kk >= 3 && tstr[i] != '+' && tstr[i] != '-' )
		{
			kk = 0;
			addppint += g_sprPenaltyRed.spr[11].xl;
			if(tang<0) m_pPage->PutSprAuto((xp+strXp) - addppint, (yp+26), &g_sprPenaltyRed, 11);
			if(tang>0) m_pPage->PutSprAuto((xp+strXp) - addppint, (yp+26), &g_sprPenaltyBlue, 11);
		}

		if(tang<0)
		{
			addppint += g_sprPenaltyRed.spr[0].xl;
			m_pPage->PutSprAuto((xp+strXp) - addppint, (yp+26), &g_sprPenaltyRed, spn);
		}
		if(tang>0)
		{
			addppint += g_sprPenaltyBlue.spr[0].xl;
			m_pPage->PutSprAuto((xp+strXp) - addppint, (yp+26), &g_sprPenaltyBlue, spn);
		}
		
		kk++;
		/*
	//	xp-=18;
		if(tstr[i] != '+' && tstr[i] != '-' && tstr[i] != '원' )
		{
			
		}
		*/
	}



}


//타이머 그리기
void CPlayerDraw_SD::DrawTimer(CDC *pDC)
{
	//게임중 일 때만 그린다.
	if(g_RI.State != 1) return;

	m_pManager->m_dwTimerCurTime = timeGetTime();

	bool bTimer = false;

	if( m_bSelectCard == true && m_pPlayer->PlayState==1 )
	{
		bTimer = true;
	}
	// 턴타이머
	else if(m_pPlayer->PlayState==1 && m_pGameProcess->m_nCurPlayer == m_pPlayer->ServPNum && !m_pManager->m_bFlyWindCard ) // 카드 날리는 중에는 그리지 않는다.
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
			NMBASE::SOUND::g_Sound.PlayWav(SND10);
		}

		m_pPage->DrawMultiScene(m_pUIInfoData->m_ptTimer.x,m_pUIInfoData->m_ptTimer.y,&m_pManager->m_sprTimer, m_nPNum, dwTimeGab - TURN_TIMER_WAIT_TIME);
	}
}