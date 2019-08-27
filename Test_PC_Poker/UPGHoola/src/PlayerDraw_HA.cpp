#include "StdAfx.h"
#include "PlayerDraw_HA.h"
#include "CardRule.h"
#include "GlobalGame.h"

CPlayerDraw_HA::CPlayerDraw_HA(void)
{
	m_pMoneyStr = NULL;
}

CPlayerDraw_HA::~CPlayerDraw_HA(void)
{
	SAFE_DELETE( m_pMoneyStr );
//	bmpNickNameBack.DeleteObject();
}


void CPlayerDraw_HA::Reset(void)
{
	CPlayerDraw::Reset();
}


void CPlayerDraw_HA::Init(CPlayer *pPlayer, int nNum, NMBASE::GRAPHICGDI::CPage *pPage, CPlayerDrawManager *pManager)
{
	CPlayerDraw::Init(pPlayer, nNum, pPage, pManager);
//	bmpNickNameBack.LoadBitmapFile(".\\hoola\\data\\image\\nickNameback.bmp");

	switch(m_nPNum)
	{
	case 0:
		m_ptLevel = CPoint( 323, 582);
		m_ptMaster = CPoint( 345, 582);
		break;
	case 1:
		m_ptLevel = CPoint( 108, 371);
		m_ptMaster = CPoint( 130, 371);
		break;
	case 2:
		m_ptLevel = CPoint( 108, 180);
		m_ptMaster = CPoint( 130, 180);
		break;
	case 3:
		m_ptLevel = CPoint( 856, 180);
		m_ptMaster = CPoint( 878, 180);
		break;
	case 4:
		m_ptLevel = CPoint( 856, 371);
		m_ptMaster = CPoint( 878, 371);
		break;
	}
}

void CPlayerDraw_HA::InitUIData(void)
{
	CPlayerDraw::InitUIData();
}

void CPlayerDraw_HA::DrawResult(CDC *pDC)
{
	CPlayerDraw::DrawResult(pDC);
}

BOOL CPlayerDraw_HA::OnMouseMove(int x, int y)
{
	if ( FALSE == IsPracticUser() )
	{
		return CPlayerDraw::OnMouseMove(x, y);
	}
	return FALSE;
}
BOOL CPlayerDraw_HA::OnLButtonDown(int x, int y)
{
	if ( FALSE == IsPracticUser() )
	{
		return CPlayerDraw::OnLButtonDown(x, y);
	}
	return FALSE;
}
BOOL CPlayerDraw_HA::OnLButtonUp(int x, int y)
{
	if ( FALSE == IsPracticUser() )
	{
		return CPlayerDraw::OnLButtonUp(x, y);
	}
	return FALSE;
}


// void CPlayerDraw_HA::DrawBottom(CDC *pDC)
// {
// 	CPlayerDraw::DrawBottom(pDC);
// }
// void CPlayerDraw_HA::DrawMiddle(CDC *pDC)
// {
// 	CPlayerDraw::DrawMiddle(pDC);
// }
// void CPlayerDraw_HA::DrawTop(CDC *pDC)
// {
// 	CPlayerDraw::DrawTop(pDC);
// }
// void CPlayerDraw_HA::DrawTopMost(CDC *pDC)
// {
// 	CPlayerDraw::DrawTopMost(pDC);
// }

void CPlayerDraw_HA::DrawLevel(CDC *pDC)
{
	if( !IsPracticUser() )
	{		
		CPlayerDraw::DrawLevel(pDC);
	}
}
void CPlayerDraw_HA::DrawAvatar(CDC *pDC)
{
	if( IsPracticUser() )
	{
		m_pPage->DrawMultiScene( 0, 0, &g_sprPracticsAvatar, m_pPlayer->PNum-1, timeGetTime() );
	}
	else
	{
		CPlayerDraw::DrawAvatar(pDC);
	}
}

void CPlayerDraw_HA::DrawUserBack(CDC *pDC)
{
	if(m_nPNum != 0)
	{
		m_pPage->PutSprAuto(m_ptBack.x, m_ptBack.y, &g_SprTurnEffect, 2); //2번은 닉네임 Back 이미지	
	}
}
void CPlayerDraw_HA::DrawTurn(CDC *pDC)
{
	if ( FALSE == g_ObserverMan.IsMyObserver() )
	{
		if ( ( g_PLAYER(0)->PlayState == EPLAYER_GAMEEND ) && ( TRUE == g_GAMEPROCESS()->GetPlayScr()->m_bPracticsGame ) )
		{
			return;
		}		
	}
	else
	{
		return;
	}

	if(!IsGameOver() && m_bTurn)
	{
		if(m_nPNum != 0)
		{
			m_pPage->DrawMultiScene(m_ptBack.x, m_ptBack.y, &g_SprTurnEffect, 0, timeGetTime());
		}
		else
		{
			m_pPage->DrawMultiScene(m_ptBack.x, m_ptBack.y, &g_SprTurnEffect, 1, timeGetTime());
		}
	}
}

void CPlayerDraw_HA::DrawNickName(CDC *pDC)
{
	if ( FALSE == IsPracticUser() )
	{
		if(m_nPNum != 0)
		{
			CString str= "";

			str.Format("%s", m_pPlayer->UI.NickName);

			if(m_pName == NULL)
			{
				CRect rtRect = m_pUIInfoData->m_rtUserName;

				//기존 아바타 시작위치로부터 사용자 이름의 갭으로 계산한 위치를 사용할 수 없음
				rtRect.OffsetRect(m_ptBack.x + 4, m_ptBack.y + 4);

				m_pName = new NMBASE::SKINGDI::CNText(rtRect, (char *)(LPCTSTR)"", 0, m_pPage, pDC);
				m_pName->SetAlign(DT_SINGLELINE | DT_LEFT);
				m_pName->SetColor(m_pUIInfoData->m_clrUserNameColor);
				m_pName->SetOutLine(m_pUIInfoData->m_clrUserNameShadowColor);
				m_pName->SetFont(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));
				m_pName->SetText(str);
			}
			else
			{
				if( str != m_pName->GetText() )
					m_pName->SetText(str);
			}

			m_pName->DrawText(pDC);
		}
		else
		{
			CPlayerDraw::DrawNickName(pDC);
		}
	}
}

BOOL CPlayerDraw_HA::IsPracticUser()
{
	if (strcmp( m_pPlayer->UI.ID, "_PRACTICSUSER_") == 0)
	{
		return TRUE;
	}
	return FALSE;
}
void CPlayerDraw_HA::DrawMoney(CDC *pDC)
{
	if ( FALSE == IsPracticUser())
	{
		if ( m_pPlayer->PNum == 0)
		{
			CPlayerDraw::DrawMoney(pDC);
		}
		else
		{
			if ( m_pMoneyStr == NULL )
			{
				CRect rtRect;
				//닉네임 배경좌표 기준으로 위치를 조정함. 차후에라도 값을 UIInfodata에 넣을것
				//닉네임 상자의 크기는 129,19
				//m_rtUserName(100,14) -> width,height 값이 기존값에서 변경되면 텍스트가 일그러지는데 원인 파악해야할듯

				int nMoneyLabelWidth = 100;
				int nMoneyLabelHeight = 14;
				
				rtRect.SetRect(m_ptBack.x + 29, m_ptBack.y + 22, m_ptBack.x + 29 + nMoneyLabelWidth , m_ptBack.y + 22 + nMoneyLabelHeight);
				m_pMoneyStr = new NMBASE::SKINGDI::CNText(rtRect, (char *)(LPCTSTR)"", 0, m_pPage, pDC);
				m_pMoneyStr->SetAlign(DT_SINGLELINE | DT_RIGHT);
				m_pMoneyStr->SetColor(m_pUIInfoData->m_clrUserNameColor);
				m_pMoneyStr->SetOutLine(m_pUIInfoData->m_clrUserNameShadowColor);
				m_pMoneyStr->SetFont(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));
			}

			INT64 roundingoff = 0;
			roundingoff = m_pPlayer->UI.GetMoney();
			CString str= "";
//			str = NMBASE::UTIL::g_HistoryMoneyMarkEx(roundingoff);
			str = (CString)NMBASE::UTIL::g_MakeShortMoneyStrEx( roundingoff );

			m_pMoneyStr->SetText(str);
			m_pMoneyStr->DrawText(pDC);
		}
		
	}

}

void CPlayerDraw_HA::DrawBossMark(CDC *pDC)
{
	//방장 표시를 없애기 위해 상속받음
}