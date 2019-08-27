// ResultFocus.cpp: implementation of the CResultFocus class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "ResultFocus.h"
#include "./Etc/spline.h"

#include "./Etc/CurveControl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
typedef struct{
		POINT pt[5];
}CCP;	

CResultFocus::CResultFocus()
{
	m_pPage = NULL;
}

CResultFocus::~CResultFocus()
{
	
}	

void CResultFocus::Init(NMBASE::GRAPHICGDI::CPage *page)
{
	ReSet();

	m_pPage = page;		
}

void CResultFocus::Clear()
{
	ReSet();
}

void CResultFocus::ReSet()
{
	m_nStarCnt = 0;
	m_bShow = FALSE;

	m_bMyHighRule = FALSE;
	m_nWinPNum = -1;
	m_dwWinDrawTime = 0;
	m_nMyhighrule = 0;
	m_dwChanceDrawTime = 0;
}


void CResultFocus::SetRFocus(int wpnum,int myhighrule)
{	
	ReSet();

	m_bShow = TRUE;	

	m_nWinPNum = wpnum;

	m_dwChanceDrawTime = timeGetTime();	

	//승자족보이팩트
	if( g_pGameView->m_cPlayerDrawManager.IsOpen() ) 
	{
		m_dwWinDrawTime = timeGetTime();

		m_nMyhighrule = myhighrule;

		if(myhighrule >= CCardRule::N7C_FLUSH)
		{
			m_bMyHighRule = TRUE;
		}
		else
		{
			if((CPromotionManager::ENUM_PROMOTION_TYPE)GM().GetPromotionData()->nCnts[0] == CPromotionManager::EPT_POKER_2018_CHUSEOK)
			{
				MM().Call(UPG_CMK_SHOW_CHUSEOK_GOLDCOIN, WPARAM(0), LPARAM(0));
			}
			else
			{			
				MM().Call(UPG_CMK_SHOW_LUCKYWHEEL_POPUP, (WPARAM)0, (LPARAM)TRUE);
			}
			m_bMyHighRule = FALSE;
		}

		MM().Call(UPG_CMK_SET_JACKPOTEFFECT_SHOW, (WPARAM)0, (LPARAM)TRUE);
	}
	else
	{
		if((CPromotionManager::ENUM_PROMOTION_TYPE)GM().GetPromotionData()->nCnts[0] == CPromotionManager::EPT_POKER_2018_CHUSEOK)
		{
			MM().Call(UPG_CMK_SHOW_CHUSEOK_GOLDCOIN, WPARAM(0), LPARAM(0));
		}
		else
		{	
			MM().Call(UPG_CMK_SHOW_LUCKYWHEEL_POPUP, (WPARAM)0, (LPARAM)TRUE);
		}
	}
}

void CResultFocus::OnTimer()
{
	if(m_bShow== FALSE) return;
}

void CResultFocus::Draw(CDC *pDC)
{
	if(m_bShow == FALSE) return;
	
	if(m_nWinPNum > -1 && m_nWinPNum < 5 )
	{	
		if(m_dwWinDrawTime>0)
		{
			DWORD CurTime = timeGetTime();
			CurTime = CurTime - m_dwWinDrawTime; 								
			
			if(m_bMyHighRule == TRUE)
			{
				int nPlayIndex = 0;
				DWORD dwPlayTime = 0;

				if(m_nMyhighrule == CCardRule::N7C_FLUSH)
				{
					// 플러쉬
					nPlayIndex = 6;				
				}
				else if(m_nMyhighrule == CCardRule::N7C_TITLE)
				{
					// 풀하우스
					nPlayIndex = 5;				
				}
				else if(m_nMyhighrule == CCardRule::N7C_QUADS)
				{
					//포카드
					nPlayIndex = 4;				
				}
				else if(m_nMyhighrule == CCardRule::N7C_SFULSH)
				{
					//스티플
					nPlayIndex = 3;
				}
				else if(m_nMyhighrule == CCardRule::N7C_BSFLUSH)
				{
					//백스티플
					nPlayIndex = 2;
				}
				else if(m_nMyhighrule >= CCardRule::N7C_CROYAL && m_nMyhighrule <= CCardRule::N7C_SROYAL){
					//로티플
					if(GAME()->m_i64JackPotActive > 0)
					{
						nPlayIndex = 1;
					}
					else
					{
						nPlayIndex = 0;
					}
				}

				dwPlayTime = g_sprJokboEffect.GetMScenePlayTime(nPlayIndex);
				m_pPage->DrawMultiScene(0,0,&g_sprJokboEffect,nPlayIndex,CurTime, FALSE);

				//하이 족보로 우승한 유저 이름과
				//0.81초후 머니를 표시한다.
				if(CurTime >= 810 && CurTime <= dwPlayTime)
				{
					//아이디/닉네임 찍기
					CRect rtRect = g_cUIData.m_rtHighJokboUserName;
					rtRect.OffsetRect(g_cUIData.m_ptHighJokboUserName);
					CFont *pOldFont = pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));
					COLORREF clrOld = pDC->SetTextColor(g_cUIData.m_clrHighJokboUserName);

					CPlayer_NP *pPlayer = (CPlayer_NP *)GAME()->GetPlayer(m_nWinPNum);
					pDC->DrawText(pPlayer->UI.NickName, &rtRect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

					//족보 찍기
					CString strJokboName;
					rtRect = g_cUIData.m_rtHighJokboName;
					rtRect.OffsetRect(g_cUIData.m_ptHighJokboName);
					pDC->SetTextColor(g_cUIData.m_clrHighJokboName);
					MM().Call(UPG_CMK_GET_JOKBO_NAME, (WPARAM)m_nWinPNum, (LPARAM)&strJokboName);
					pDC->DrawText( strJokboName, &rtRect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

					//금액 표시
					CString strMoney = (CString)NMBASE::UTIL::g_MoneyMark(pPlayer->m_i64ResultMoney);
					rtRect = g_cUIData.m_rtHighJokboWinMoney;
					rtRect.OffsetRect(g_cUIData.m_ptHighJokboWinMoney);
					pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_14D_BOLD));
					pDC->SetTextColor(g_cUIData.m_clrHighJokboWinMoney);
					pDC->DrawText(strMoney, &rtRect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

					////잭팟 머니를 표시한다. 1.1초후 잭팟 머니를 표시한다.
					//if(nPlayIndex == 1 && CurTime >= 1100 && CurTime <= 3600)
					//{
					//	rtRect = g_cUIData.m_rtHighJokboJacpotMoney;
					//	rtRect.OffsetRect(g_cUIData.m_ptHighJokboJacpotMoney);
					//	strMoney = NMBASE::UTIL::g_MoneyMark(GAME()->m_i64JackPotActive);
					//	pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_12D_BOLD));
					//	pDC->SetTextColor(g_cUIData.m_clrHighJokboJacpotMoney);
					//	pDC->DrawText(strMoney, &rtRect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					//}

					if(CurTime >= 3300)
				    {
						if((CPromotionManager::ENUM_PROMOTION_TYPE)GM().GetPromotionData()->nCnts[0] == CPromotionManager::EPT_POKER_2018_CHUSEOK)
						{
							MM().Call(UPG_CMK_SHOW_CHUSEOK_GOLDCOIN, WPARAM(0), LPARAM(0));
						}
						else
						{						
							MM().Call(UPG_CMK_SHOW_LUCKYWHEEL_POPUP, (WPARAM)0, (LPARAM)TRUE);
						}
				    }

					pDC->SetTextColor(clrOld);
					pDC->SelectObject(pOldFont);
				}		
			}		
		}
	}
	
// 	POINT pt[5] = {{273,602}, {9,394}, {9,189}, {843,189}, {843,394}};
// 	DWORD time = timeGetTime() - m_dwChanceDrawTime;
// 	
// #ifdef _DEBUG 
// 	if( m_dwChanceDrawTime > 0){
// 		
// 		if(RewardItemMan().m_RewardItemData.itemcode[0] >= ITEM_SCODE_REWARD_FCARD &&
// 			RewardItemMan().m_RewardItemData.itemcode[0] <= ITEM_SCODE_REWARD_RSF)
// 		{			
// 			int scene = RewardItemMan().m_RewardItemData.itemcode[0] - ITEM_SCODE_REWARD_FCARD;
// 			if(scene < 0) scene = 0;
// 			
// 			DWORD endtime = g_sprChance.GetScenePlayTime(scene);
// 			
// 			if( time >= endtime ){
// 				m_dwChanceDrawTime = 0;
// 				return;
// 			}
// 			m_pPage->DrawScene(pt[0].x, pt[0].y, &g_sprChance, scene, time);		
// 		}
// 	}
// #else
// 	if(g_pGameView->m_cPlayerDrawManager.IsGameOver() && m_dwChanceDrawTime > 0) 
// 	{
// 		if(RewardItemMan().m_RewardItemData.itemcode[0] >= ITEM_SCODE_REWARD_FCARD &&
// 			RewardItemMan().m_RewardItemData.itemcode[0] <= ITEM_SCODE_REWARD_RSF) 
// 		{			
// 		
// 			int scene = RewardItemMan().m_RewardItemData.itemcode[0] - ITEM_SCODE_REWARD_FCARD;
// 			if(scene < 0) scene = 0;
// 
// 			DWORD endtime = g_sprChance.GetScenePlayTime(scene);
// 			
// 			if( time >= endtime ){
// 				m_dwChanceDrawTime = 0;
// 				return;
// 			}
// 			m_pPage->DrawScene(pt[0].x, pt[0].y, &g_sprChance, scene, time);		
// 		}
// 	}
// #endif
}