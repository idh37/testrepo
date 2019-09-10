// AniSprClassList.cpp: implementation of the AniSprClassList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "AniSprClassList.h"
#include "mmsystem.h"
#include "GlobalGame.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////
//���� Ŀ��Ʈ ǥ��


CBettingEffect::CBettingEffect()
{	
	m_nStartTime = 0;
	m_nEndTime = 0 ;
	Xp = 0 ;
	Yp = 0 ;
	m_nDestroyDelTime = 0;
	m_PlayerNumber  = -1;
	m_pEffectScene = NULL;
	m_nBetEffectBigPos = 0;
	m_nBetEffectSmallPos = 0;

	
	m_sEffectPos[ ANF_BET_HALF - ANF_BET_CMD - 1 ].nBigPos	 = 0;	// ����
	m_sEffectPos[ ANF_BET_HALF - ANF_BET_CMD - 1 ].nSmallPos= 7; 

	m_sEffectPos[ ANF_BET_CALL - ANF_BET_CMD - 1 ].nBigPos	 = 1;	// ��
	m_sEffectPos[ ANF_BET_CALL - ANF_BET_CMD - 1 ].nSmallPos= 8; 
	
	m_sEffectPos[ ANF_BET_CHECK - ANF_BET_CMD - 1 ].nBigPos	 = 2;	// üũ
	m_sEffectPos[ ANF_BET_CHECK - ANF_BET_CMD - 1 ].nSmallPos= 9; 
	
	m_sEffectPos[ ANF_BET_DADANG - ANF_BET_CMD - 1 ].nBigPos	 = 3;	// ����
	m_sEffectPos[ ANF_BET_DADANG - ANF_BET_CMD - 1 ].nSmallPos= 10; 
	
	m_sEffectPos[ ANF_BET_PING - ANF_BET_CMD - 1 ].nBigPos	 = 4;	// ��
	m_sEffectPos[ ANF_BET_PING - ANF_BET_CMD - 1 ].nSmallPos= 11; 
	
	m_sEffectPos[ ANF_BET_DIE - ANF_BET_CMD - 1 ].nBigPos	 = 5;	// ����
	m_sEffectPos[ ANF_BET_DIE - ANF_BET_CMD - 1 ].nSmallPos= 12; 

	m_sEffectPos[ ANF_BET_ALLIN - ANF_BET_CMD - 1 ].nBigPos	 = 13;	// ����
	m_sEffectPos[ ANF_BET_ALLIN - ANF_BET_CMD - 1 ].nSmallPos= 14; 

	m_sEffectPos[ ANF_BET_RAISE_X1 - ANF_BET_CMD - 1 ].nBigPos	 = 16;	// _X1
	m_sEffectPos[ ANF_BET_RAISE_X1 - ANF_BET_CMD - 1 ].nSmallPos= 19; 
	
	m_sEffectPos[ ANF_BET_RAISE_X2 - ANF_BET_CMD - 1 ].nBigPos	 = 17;	// _X2
	m_sEffectPos[ ANF_BET_RAISE_X2 - ANF_BET_CMD - 1 ].nSmallPos= 20; 

	m_sEffectPos[ ANF_BET_RAISE_X3 - ANF_BET_CMD - 1 ].nBigPos	 = 18;	// _X5
	m_sEffectPos[ ANF_BET_RAISE_X3 - ANF_BET_CMD - 1 ].nSmallPos= 21; 


// 	m_sEffectPos[ ANF_BET_LIMIT_X10 - ANF_BET_CMD - 1 ].nBigPos	 = 18;	// _X10
// 	m_sEffectPos[ ANF_BET_LIMIT_X10 - ANF_BET_CMD - 1 ].nSmallPos= 23; 
// 
// 	m_sEffectPos[ ANF_BET_LIMIT_X20 - ANF_BET_CMD - 1 ].nBigPos	 = 19;	// _X20
// 	m_sEffectPos[ ANF_BET_LIMIT_X20 - ANF_BET_CMD - 1 ].nSmallPos= 24; 

}

CBettingEffect::~CBettingEffect()
{	
	m_nStartTime = 0;	
	m_PlayerNumber  = -1;
	m_nDestroyDelTime = 0;
	//	LastBetEftSpr.Remove();

	m_nBetEffectBigPos = -1;		// ū ����Ʈ ��ġ
	m_nBetEffectSmallPos = -1;		// ���� ����Ʈ ��ġ

	if ( m_pEffectScene != NULL)
	{
		delete m_pEffectScene;
	}

	m_pEffectScene = NULL;
}


BOOL CBettingEffect::Init(int nPlayerNum, ENUM_ANIFILEID eAniType, int nEffectType, BOOL bLastBetMax)
{
	//if ( eAniType < ANF_BET_CMD || eAniType > ANF_BET_ALLIN )
	if ( eAniType < ANF_BET_CMD || eAniType > ANF_BET_RAISE_X3 )
	{
		return FALSE;
	}
	
	// ����Ʈ�� ��ġ�� ã�� �ش�.
	// -1�� ���ִ� ������ ���� 0���� ���� �ϱ� �����̴�.
	// g_GAMEPROCESS()->GetMaxPlayer()�� �����ִ� ������ ��ư�� Gap�� �÷��̾� ����ŭ�̱� ����
	
	//������ : �ƽ� ����Ʈ �߰�
	int nBetEffectStartPos = 0;

	if (nEffectType == ANF_BET_MAX)
	{
		nBetEffectStartPos = 130;
		m_nBetEffectBigPos   = 130 + nPlayerNum;
		m_nBetEffectSmallPos = 135 + nPlayerNum;
	}
#if defined(BUTTON_CHANGE_FULLTODADANG)
	else if (nEffectType==ANF_BET_FULL)
	{
		nBetEffectStartPos = 140;
		m_nBetEffectBigPos   = 140 + nPlayerNum;
		m_nBetEffectSmallPos = 145 + nPlayerNum;
	}
#endif
	else
	{
		nBetEffectStartPos = nBetEffectStartPos = nEffectType - ANF_BET_CMD - 1;
	}
	
	if ( nBetEffectStartPos >= 0 && nBetEffectStartPos < 10 && nBetEffectStartPos != 130 )
	{
		m_nBetEffectBigPos   = (m_sEffectPos[nBetEffectStartPos].nBigPos   * g_GAMEPROCESS()->GetMaxPlayer()) + nPlayerNum;
		m_nBetEffectSmallPos = (m_sEffectPos[nBetEffectStartPos].nSmallPos * g_GAMEPROCESS()->GetMaxPlayer()) + nPlayerNum;
	}
	else if ( nBetEffectStartPos != 130 )
	{
		//// ���� �� �� �����ϴ�.
		//__asm int 3
	}

	m_nObjID			= eAniType;
	DrawFaceKind		= DRAW_BOTTOM;
	m_nDestroyDelTime	= 0;
	m_nEndTime			= g_sprBettingEffect.GetMScenePlayTime(m_nBetEffectBigPos);
	m_nStartTime		= timeGetTime();
	m_PlayerNumber		= nPlayerNum;
	bDifferntBetStyle	= FALSE;
	
	m_pEffectScene = new NMBASE::SKINGDI::CNMultiScene(0, 0, g_pGameView, &g_pGameView->Page, &g_sprBettingEffect, m_nBetEffectBigPos);
	m_pEffectScene->StartMScene(1);

#ifdef TRACE_OUTPUT
	char aaa[256] = {0, };
	sprintf(aaa, "%d ����", m_PlayerNumber);
	TTrace::Debug()->Send ("CBettingEffect::Init()", aaa); // single byte string
#endif
	return TRUE;
}

CPoint CBettingEffect::GetEffectPos(void)
{
	if (NULL == g_sprBettingEffect.multiscene.ppMScene[m_nBetEffectBigPos])
		return CPoint(0, 0);
	
	NMBASE::GRAPHICGDI::TRACK_SCENE* pEffect = g_sprBettingEffect.multiscene.ppMScene[m_nBetEffectBigPos]->ppTrack[0]->GetDataScene();

	if (NULL == pEffect)
		return CPoint(0, 0);
	
	return pEffect->Pos;
}

//void CBettingEffect::MakeVIPJackpotPoint(int nPoint)
//{
//	int nMScene=0;
//	if (10>nPoint){
//		nMScene=1;
//	}
//	int nPoint10 = nPoint / 10;
//	int nPoint01 = nPoint % 10;
//	ErrorLogConsole(" MakeVIPJackpotPoint  = %d = %d = %d", nPoint, m_nObjID, nMScene);
//#if defined(_VIPJACKPOT)
//	if (0==nMScene){
//		NMBASE::GRAPHICGDI::TRACK_SCENE* pScnData1 = (NMBASE::GRAPHICGDI::TRACK_SCENE*)g_SprVIPJackpot_Bet.multiscene.ppMScene[nMScene]->ppTrack[2]->GetDataScene();
//		NMBASE::GRAPHICGDI::TRACK_SCENE* pScnData2 = (NMBASE::GRAPHICGDI::TRACK_SCENE*)g_SprVIPJackpot_Bet.multiscene.ppMScene[nMScene]->ppTrack[3]->GetDataScene();
//		if (NULL!=pScnData1){
//			pScnData1->SceneNo = 4+nPoint10;
//		}
//		if (NULL!=pScnData2){
//			pScnData2->SceneNo = 4+nPoint01;
//		}
//		ErrorLogConsole(" MakeVIPJackpotPoint2 %d = %d = %d = %d", nPoint10, nPoint01, pScnData1->SceneNo, pScnData2->SceneNo);
//	}
//	else{
//		NMBASE::GRAPHICGDI::TRACK_SCENE* pScnData = (NMBASE::GRAPHICGDI::TRACK_SCENE*)g_SprVIPJackpot_Bet.multiscene.ppMScene[nMScene]->ppTrack[2]->GetDataScene();
//		if (NULL!=pScnData){
//			pScnData->SceneNo = 4+nPoint01;
//		}
//		ErrorLogConsole(" MakeVIPJackpotPoint1 = %d = %d", nPoint01, pScnData->SceneNo);
//	}
//	NMBASE::GRAPHICGDI::TRACK_SCENE* pEffect = g_sprBettingEffect.multiscene.ppMScene[m_nBetEffectBigPos]->ppTrack[0]->GetDataScene();
//
//	m_pVIPJackPotScene = new NMBASE::SKINGDI::CNMultiScene(pEffect->Pos.x, pEffect->Pos.y, g_pGameView, &g_pGameView->Page, &g_SprVIPJackpot_Bet, nMScene);
//	m_pVIPJackPotScene->StartMScene(1);
//#endif
//}

void CBettingEffect::Destroy()
{
	m_nEndTime = 0;
	m_nStartTime = 0;
	m_nDestroyDelTime = 0;

	if ( m_pEffectScene != NULL ) 
	{
		delete m_pEffectScene;
	}

	m_pEffectScene = NULL;


#ifdef TRACE_OUTPUT		
	char aaa[256] = {0, };
	sprintf(aaa, "%d ����", m_PlayerNumber);
	TTrace::Debug()->Send ("CBettingEffect::Destroy()", aaa);   // single byte string	
#endif
}

void CBettingEffect::SetSmallCmd()
{
	if (bDifferntBetStyle) 
		return;

	bDifferntBetStyle = TRUE;

	if ( m_pEffectScene != NULL ) 
	{		
		if ( bDifferntBetStyle == FALSE )
		{
			m_pEffectScene->SetDrawIndex(m_nBetEffectBigPos);

#ifdef TRACE_OUTPUT		
			char aaa[256] = {0, };
			sprintf(aaa, "%d ����", m_PlayerNumber);
			TTrace::Debug()->Send ("CBettingEffect::SetSmallCmd()-big", aaa);   // single byte string	
#endif
		}
		else
		{			
			m_pEffectScene->SetDrawIndex(m_nBetEffectSmallPos);

#ifdef TRACE_OUTPUT		
			char aaa[256] = {0, };
			sprintf(aaa, "%d ����", m_PlayerNumber);
			TTrace::Debug()->Send ("CBettingEffect::SetSmallCmd()-small", aaa);   // single byte string	
#endif
		}


	}
}

int CBettingEffect::ProcessObject()
{	
	if (m_nDestroyDelTime > 0)
	{
		m_nDestroyDelTime--;
		if (m_nDestroyDelTime <= 0)
		{
			m_nDestroyDelTime = 0;
			m_nStartTime = 0;
			m_nEndTime = 0;
			return PROCESS_KILLME;
		}
	}	

	if ( m_pEffectScene != NULL )
	{
		if ( m_pEffectScene->m_stSceneState.bScenePlay == TRUE )
		{
			m_pEffectScene->OnTimer();
		}
		else
		{
			Destroy();
			return PROCESS_KILLME;
		}
	}

	if (m_nStartTime == 0) 
	{		
		m_nStartTime = 0;
		m_nEndTime = 0;
		m_nDestroyDelTime = 0;
		return PROCESS_KILLME;				
	}
	return PROCESS_OK;
}

void CBettingEffect::DrawObject(CDC *pDC)
{	
	if ( m_pEffectScene != NULL )
	{
		m_pEffectScene->DrawMScene(NULL);
	}
}

//=========================================================================//

CJokboEffect::CJokboEffect()
{		
	m_pMultiScene	= NULL;
	m_nDelTime		= 0;
}

CJokboEffect::~CJokboEffect()
{	
	Destroy();
}


//int pnum, int wincase , int valuekind , CString str
void CJokboEffect::Init( GAMERESULTDATA *pData)
{
	if ( pData == NULL )
		return;
	/*
	wincase = 0 �� , 1 �� 
	*/
	memcpy(&m_sResultData, pData, sizeof(m_sResultData));

	m_nObjID	 = ANF_PLAYER_JOKBOEFFECT;
	DrawFaceKind = DRAW_TOP;
	m_bJokboShow = FALSE;
	m_nDelTime	 = 15;

	if ( m_pMultiScene != NULL)
	{
		delete m_pMultiScene;
		m_pMultiScene = NULL;
	}

	m_dwWinDrawTime = 0;
	m_nSprindex = -1;
	
	// RoyalST(0), RoyalST&Jackpot(1), BackST(2), ST(3), 4card(4)	
	switch( m_sResultData.valuekind )
	{			
		case CCardRule::N7C_CROYAL:		// �ξ⽺Ʈ����Ʈ �÷���
		case CCardRule::N7C_HROYAL:
		case CCardRule::N7C_DROYAL:
		case CCardRule::N7C_SROYAL:	
			{
				//��Ƽ��
				if (g_GAMEPROCESS()->m_i64JackPotActive > 0)
				{
					m_nSprindex = 1;
				}
				else
				{
					m_nSprindex = 0;
				}
			}
			break;

		case CCardRule::N7C_BSFLUSH:	// �齺Ʈ����Ʈ �÷���
			{
				m_nSprindex = 2;
			}
			break;

		case CCardRule::N7C_SFULSH:		// ��Ʈ����Ʈ �÷���
			{
				m_nSprindex = 3;
			}
			break;

		case CCardRule::N7C_QUADS:		// ��ī��
			{
				m_nSprindex = 4;
			}
			break;	
		case CCardRule::N7C_TITLE:				// Ǯ�Ͽ콺
			{
				m_nSprindex = 5;
			}break;

		case CCardRule::N7C_FLUSH:				// Ǯ����
			{
				m_nSprindex = 6;
			}break;

		default:
			break;
	}
	

	if ( m_nSprindex > -1 )
	{
		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(0, 0, NULL, &g_pGameView->Page, &g_sprJokboEffect, m_nSprindex);
		m_pMultiScene->StartMScene(1);
	}

	if ( m_sResultData.nResultMoney <= 0 )
	{
		return;
	}

	ZeroMemory(m_nMoneyDeck,sizeof(m_nMoneyDeck));
	m_nTotalMoneyIndex = 0;

	INT64 tmoney = m_sResultData.nResultMoney;
	if (tmoney < 0)
	{
		tmoney = tmoney * -1;
	}


	CString str = _T("");		
	str.Format("%I64d",tmoney);
	int len = 0;
	len = str.GetLength();	

	if (len-1 > 30)
	{
		return;
	}

	int cnt = 0 ;
	for (int j = len-1; j >= 0 ; j--)
	{
		char tnum[4] = {0,};
		tnum[0] = str.GetAt(j);

		cnt++;
		if ( cnt % 5 == 0 )
		{ 
			m_nMoneyDeck[m_nTotalMoneyIndex] = -100;
			m_nTotalMoneyIndex++;
			m_nMoneyDeck[m_nTotalMoneyIndex] = atoi(tnum);
			m_nTotalMoneyIndex++; 
			cnt = 1; 
		}
		else
		{
			m_nMoneyDeck[m_nTotalMoneyIndex] = atoi(tnum);
			m_nTotalMoneyIndex++;	
		}	
	}

	m_dwWinDrawTime = timeGetTime();

	// ���� ����Ʈ�� �����ش�.
	MM().Call(UPG_CMK_SET_JACKPOTEFFECT_SHOW, (WPARAM)0, (LPARAM)TRUE);
}

void CJokboEffect::Destroy()
{	
	if ( m_pMultiScene != NULL )
	{
		delete m_pMultiScene;		
	}
	m_pMultiScene= NULL;
	m_bJokboShow = FALSE;
}

int CJokboEffect::ProcessObject()
{	
	if (m_nDelTime>0)
	{
		m_nDelTime--;
		if ( m_nDelTime <= 0 )
		{
			m_nDelTime = 0;			
		}
		return PROCESS_OK;
	}

	if ( m_pMultiScene != NULL )
	{
		if ( m_pMultiScene->m_stSceneState.bScenePlay == TRUE )
		{
			NMBASE::GRAPHICGDI::QUERYMSCENE_SCRIPT *SCRIPT = m_pMultiScene->OnTimer();
			if ( SCRIPT == NULL )
			{
				m_bJokboShow = TRUE;
			}
		}
		else
		{
			Destroy();
			return PROCESS_KILLME;
		}
	}
	else
	{
		Destroy();
		return PROCESS_KILLME;
	}

	return PROCESS_OK;
}

void CJokboEffect::DrawObject(CDC *pDC)
{
	if ( m_pMultiScene != NULL )
	{
		// ���� ����Ʈ�� ����Ѵ�.
		m_pMultiScene->DrawMScene(NULL);

		// ���� ������ ����� ���� �̸��� 0.6���� �Ӵϸ� ǥ���Ѵ�.
		if ( pDC && m_bJokboShow )
		{
			DWORD dwCurTime = timeGetTime();
			dwCurTime = dwCurTime - m_dwWinDrawTime; 	

			//���̵�/�г��� ���
			CRect rtRect = g_cUIData.m_rtHighJokboUserName;
			rtRect.OffsetRect(g_cUIData.m_ptHighJokboUserName);
			CFont *pOldFont = pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));
			COLORREF clrOld = pDC->SetTextColor(g_cUIData.m_clrHighJokboUserName);

			pDC->DrawText(g_PLAYER(m_sResultData.pnum)->UI.NickName, &rtRect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

			//���� ���
			CString strJokboName;
			rtRect = g_cUIData.m_rtHighJokboName;
			rtRect.OffsetRect(g_cUIData.m_ptHighJokboName);
			pDC->SetTextColor(g_cUIData.m_clrHighJokboName);
			MM().Call(UPG_CMK_GET_JOKBO_NAME, (WPARAM)m_sResultData.pnum, (LPARAM)&strJokboName);
			pDC->DrawText( strJokboName, &rtRect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

			//�ݾ� ǥ��
			CString strMoney = (CString)NMBASE::UTIL::g_MoneyMark(m_sResultData.nResultMoney);
			rtRect = g_cUIData.m_rtHighJokboWinMoney;
			rtRect.OffsetRect(g_cUIData.m_ptHighJokboWinMoney);
			pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_14D_BOLD));
			pDC->SetTextColor(g_cUIData.m_clrHighJokboWinMoney);
			pDC->DrawText(strMoney, &rtRect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

			////���� �Ӵϸ� ǥ���Ѵ�. 1.1���� ���� �Ӵϸ� ǥ���Ѵ�.
			//if (m_nSprindex == 1 && dwCurTime >= 1100 && dwCurTime <= 3600)
			//{
			//	rtRect = g_cUIData.m_rtHighJokboJacpotMoney;
			//	rtRect.OffsetRect(g_cUIData.m_ptHighJokboJacpotMoney);
			//	strMoney = NMBASE::UTIL::g_MoneyMark(g_GAMEPROCESS()->m_i64JackPotActive);
			//	pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_12D_BOLD));
			//	pDC->SetTextColor(g_cUIData.m_clrHighJokboJacpotMoney);
			//	pDC->DrawText(strMoney, &rtRect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
			//}
			if (dwCurTime >= 2800)
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

// 			CFont *oldf = pDC->SelectObject( &NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_12D_BOLD) );
// 			COLORREF color = RGB(255, 255, 255); 		
// 
// 			//���� ���� �϶� �ݾ�, ī�� , ���̵� �׷���
// 			if ( (m_sResultData.valuekind >= CCardRule::N7C_QUADS ))				
// 			{
// 				NMBASE::GRAPHICGDI::CPage *pPage = &g_pGameView->Page;
// 				if ( pPage == NULL )
// 					return;
// 
// 				color = pDC->SetTextColor(RGB(177,167,152));
// 
// 				CPoint Basept(354,113);
// 				//ī�� �׸���
// 				CPoint pt(60,93);
// 
// 				if ( m_sResultData.nTotalCardNum == 4 )
// 				{
// 					pt.x = 74;
// 				}
// 
// 				for ( int i = 0 ; i < m_sResultData.nTotalCardNum ; i ++ )
// 				{
// 					int cno = m_sResultData.nCard[i];
// 					if ( cno > -1 )
// 					{
// 
// 						pPage->PutSprAuto(Basept.x + pt.x + ( i * 20),Basept.y + pt.y,&g_sprBigCard,cno);
// 
// 					}
// 				}
// 
// 				// �ݾ� �׸���
// 				if ( m_sResultData.nResultMoney <= 0 )
// 					return;
// 
// 				CPoint npt(252,239);
// 				int cnt = 0 ;
// 				int s = 0;	
// 				int ngap = 0;
// 				const int number_w = 12;
// 				for ( int i=0; i<m_nTotalMoneyIndex;i++)
// 				{
// 					if ( i + 1 == m_nTotalMoneyIndex && m_nMoneyDeck[i] == 0 )
// 					{
// 						continue;
// 					}
// 
// 					if ( m_nMoneyDeck[i] >= -100 && m_nMoneyDeck[i] < 500 )
// 					{
// 						int tmp = m_nMoneyDeck[i];
// 
// 						if (tmp == -100)
// 						{				
// 							switch(s) 
// 							{
// 							case 0:
// 								ngap += 30 - number_w;//�� 
// 								break;
// 							case 1:
// 								ngap += 28 - number_w;//��
// 								break;
// 							case 2:
// 								ngap += 29 - number_w;//��
// 								break;
// 							}							
// 							s ++;
// 						}
// 						else
// 						{				
// 							if ( tmp != -1)
// 							{
// 								pPage->PutSprAuto( Basept.x + npt.x  - (cnt * number_w) - ngap,Basept.y + npt.y, &g_sprJokboEffect, tmp );
// 								cnt++;
// 							}
// 						}
// 					}
//				}
// 
// 				CRect rt;
// 				rt.SetRect(0,0,129,13);
// 				rt.OffsetRect(Basept.x + 84,Basept.y + 219);
// 				//���̵� 
// 				pDC->DrawText(m_sResultData.ID,rt,DT_CENTER);
// 			}
// 
// 			pDC->SelectObject(oldf);
// 			pDC->SetTextColor(color);
		}
	}
}


//////////////////////////////////////////////////////////////////////////

//ANF_SDLG_GRADE
CGradeSpr::CGradeSpr()
{
	m_pMultiScene = NULL;	
	m_pPage = NULL;
}

CGradeSpr::~CGradeSpr()
{
	Destroy();
}

BOOL CGradeSpr::Init( int index , NMBASE::GRAPHICGDI::CPage *pPage)
{
	m_nObjID = ANF_SDLG_GRADE;	//����	
	DrawFaceKind = DRAW_TOP;
	Destroy();

	if ( pPage == NULL )
	{
		nResult = PROCESS_KILLME;
		return FALSE;
	}
	m_pPage = pPage ;
	POINT pt;
	if ( index == 0 )
	{
		pt.x = 361;
		pt.y = 200;
	}
	else if ( index == 1 )
	{
		pt.x = 5;
		pt.y = 10;	
	}
	else if ( index == 2 )
	{
		pt.x = 5;
		pt.y = 69;
	}

	m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(pt.x,pt.y,g_pGameView,m_pPage,&GradeSpr,index);
	m_pMultiScene->StartMScene(1);

	return TRUE;
}

void CGradeSpr::DrawObject(CDC *pDC)
{
	if (pDC == NULL)
	{
		nResult = PROCESS_KILLME;
	}

	if ( m_pMultiScene != NULL)
	{
		m_pMultiScene->DrawMScene(pDC);
	}
}

void CGradeSpr::Destroy()
{	
	if ( m_pMultiScene != NULL )
	{
		delete m_pMultiScene;
	}
	m_pMultiScene = NULL;
}

int  CGradeSpr::ProcessObject()
{	
	if ( m_pMultiScene != NULL )
	{
		if ( m_pMultiScene->m_stSceneState.bScenePlay == TRUE ){
			m_pMultiScene->OnTimer();		
		}
		else
		{
			Destroy();
			return PROCESS_KILLME;
		}
	}
	return PROCESS_OK;
}





//ANF_PLAYER_CHANCEITEM_MSG ���� ������ ȿ��
//ä��â ���� �޼��� ���� ������ ���� �����ش�.

CChance_Msg_EffectSpr::CChance_Msg_EffectSpr()
{		
	m_pMultiScene = NULL;	
	ZeroMemory(m_ChanceItemText,sizeof(m_ChanceItemText));
}

CChance_Msg_EffectSpr::~CChance_Msg_EffectSpr()
{	
	Destroy();
}

void CChance_Msg_EffectSpr::Init( int itemcode )
{
	m_nObjID = ANF_PLAYER_CHANCEITEM;	
	nDelTime = 5;
	DrawFaceKind = DRAW_TOP;

	if ( m_pMultiScene != NULL)
	{
		delete m_pMultiScene;
		m_pMultiScene = NULL;
	}	

	/*
	ITEM_SCODE_REWARD_MADE=58,			// ���̵�
	ITEM_SCODE_REWARD_RSF=54,			//��Ƽ��	//���� ������
	ITEM_SCODE_REWARD_SF=53,			//��Ƽ��
	ITEM_SCODE_REWARD_FCARD=52,			//��ī��
	ITEM_SCODE_REWARD_FH=51,			//Ǯ�Ͽ콺
	ITEM_SCODE_REWARD_LUCKAVATA=109,	//[���ƹ�Ÿ] 2006.07.24
	ITEM_SCODE_REWARD_LOWGRADE=123,		//�ºλ� �Թ� [�Թ�������]
	ITEM_SCODE_REWARD_HIGHGRADE=124,	//�ºλ� ���
	ITEM_SCODE_REWARD_ALLINKING=125,	//���ο�����	
	ITEM_SCODE_REWARD_SPECIALLUCKITEM = 132, //[�����]2007.10.17
	ITEM_SCODE_REWARD_ALLINCHANCE=134,	//[���������ƹ�Ÿ] 2008.07
	*/

	switch(itemcode) 
	{
	case ITEM_SCODE_REWARD_HIGHGRADE:
		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&ChanceEffectSpr,0);
		break;
		// 	case ITEM_SCODE_REWARD_MADE:
		// 		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&ChanceEffectSpr,1);
		// 		break;
	case ITEM_SCODE_REWARD_RSF:
		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&ChanceEffectSpr,2);
		break;
	case ITEM_SCODE_REWARD_SF:
		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&ChanceEffectSpr,3);
		break;
	case ITEM_SCODE_REWARD_FCARD:
		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&ChanceEffectSpr,4);
		break;
	case ITEM_SCODE_REWARD_FH:
		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&ChanceEffectSpr,5);
		break;	
// 	case ITEM_SCODE_REWARD_CHANCEAVATA:	//�̺�Ʈ�����ƹ�Ÿ 11.08.01
// 		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&ChanceEffectSpr,6);
// 		break;	
	case ITEM_SCODE_REWARD_VIP :	//VIP ���� 11.08.28
		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&ChanceEffectSpr,7);
		break;	
	case ITEM_SCODE_REWARD_VVIP :	//VVIP ���� 11.08.28
		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&ChanceEffectSpr,8);
		break;
	case ITEM_SCODE_REWARD_BADBEAT_SF_VIP:
	case ITEM_SCODE_REWARD_BADBEAT_RSF_VIP:
		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&ChanceEffectSpr,9);
		break;
	case ITEM_SCODE_REWARD_BADBEAT_FCARD_VIP:
		{
			if (IDX_GAME_SP != GM().GetCurrentGameType())
				m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&ChanceEffectSpr,10);
			else										
				m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&ChanceEffectSpr,9);
		}
		break;	
	case ITEM_SCODE_REWARD_BADBEAT_GOLF_VIP:
	case ITEM_SCODE_REWARD_BADBEAT_SECOND_VIP:
	case ITEM_SCODE_REWARD_BADBEAT_THIRD_VIP:
	case ITEM_SCODE_REWARD_BADBEAT_5MADE_VIP:
	case ITEM_SCODE_REWARD_BADBEAT_6MADE_VIP:
	case ITEM_SCODE_REWARD_BADBEAT_FLUSH_VIP:
	case ITEM_SCODE_REWARD_BADBEAT_FH_VIP:
		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&ChanceEffectSpr,10);
		break;
	case ITEM_SCODE_REWARD_BADBEAT_FH:
	case ITEM_SCODE_REWARD_BADBEAT_FCARD:
	case ITEM_SCODE_REWARD_BADBEAT_SF:
	case ITEM_SCODE_REWARD_BADBEAT_RSF:
	case ITEM_SCODE_REWARD_BADBEAT_GOLF:
	case ITEM_SCODE_REWARD_BADBEAT_SECOND:
	case ITEM_SCODE_REWARD_BADBEAT_THIRD:
	case ITEM_SCODE_REWARD_BADBEAT_5MADE:
	case ITEM_SCODE_REWARD_BADBEAT_6MADE:
		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&ChanceEffectSpr,11);
		break;
	}

	//get_ChanceText(itemcode);
}

void CChance_Msg_EffectSpr::get_ChanceText(int itemCode)
{

// 	ZeroMemory(m_ChanceItemText,sizeof(m_ChanceItemText));
// 	CString str;
// 
// 	//UGP�۾� �Ʒ������� ��ü
// 	// 	str.Format( "[%s]�� %s \n�����ڰ� ȹ��.\n", 
// 	// 		GM().GetMyInfo()->UI.NickName, g_pMainDlg->m_RewardItemMan.GetRewardItemName( itemCode ));
// 
// 	str.Format( "��[%s]�� %s \n�����ڰ� ȹ��.\n", GM().GetMyInfo()->UI.NickName, RewardItemMan().GetRewardItemName( itemCode ).GetBuffer());
// 
// 	strncpy(m_ChanceItemText,(char*)(LPCTSTR)str,str.GetLength());	
}

void CChance_Msg_EffectSpr::Destroy()
{	
	if ( m_pMultiScene != NULL )
	{
		delete m_pMultiScene;		
	}
	m_pMultiScene= NULL;
}

int CChance_Msg_EffectSpr::ProcessObject()
{	
	if (nDelTime>0)
	{
		nDelTime--;
		if ( nDelTime == 0 )
		{
			nDelTime = 0;
			if ( m_pMultiScene != NULL )
			{
				m_pMultiScene->StartMScene(1);
// 				if ( strlen(m_ChanceItemText) > 0 )
// 				{
// 					AddGameInfoViewEdit(m_ChanceItemText, RGB(255, 249, 73));
// 				}				
			}
		}
		return PROCESS_OK;
	}

	if ( m_pMultiScene != NULL )
	{
		if ( m_pMultiScene->m_stSceneState.bScenePlay == TRUE )
		{
			m_pMultiScene->OnTimer();
		}
		else
		{
			Destroy();
			return PROCESS_KILLME;
		}		
	}
	return PROCESS_OK;
}

void CChance_Msg_EffectSpr::DrawObject(CDC *pDC)
{
	if ( m_pMultiScene != NULL )
	{

		//		if ( g_pChatWinMan->IsShow() && g_pChatWinMan->GetViewMode() == eCHATVIEW::eCHAT){
		//			m_pMultiScene->DrawScene(NULL);
		//		}

		m_pMultiScene->DrawMScene(NULL);		
	}
}




CMadeEffect::CMadeEffect()
:m_pSpr( NULL )
{
	m_pMultiScene = NULL;	
	m_nCardCnt=0;
}

CMadeEffect::~CMadeEffect()
{
	Destroy();
	m_pSpr = NULL;
}

BOOL CMadeEffect::Init(int nCardCnt)
{
	m_nObjID = ANF_PLAYER_COMPLETEMADE_EFT;	//����	
	DrawFaceKind = DRAW_TOP;
	m_nCardCnt = nCardCnt;
	Destroy();

	m_pSpr = &g_sprGLOBAL[ spr::GSPR_ET_MADE ];

	m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene( 0, 0, g_pGameView,&g_pGameView->Page, m_pSpr ,2 );
	m_pMultiScene->StartMScene(1);
	PBPlayEffectSound(SND_MADE);

	return TRUE;
}

void CMadeEffect::DrawObject(CDC *pDC)
{
	if (pDC == NULL){nResult = PROCESS_KILLME;}

	if ( m_pMultiScene != NULL){
		m_pMultiScene->DrawMScene(pDC);


		//ī������Ʈ (ī�尹����ŭ�׸���)		
		for (int i=0; i<m_nCardCnt; i++) 
		{			
			CPoint ptCard = g_PLAYER(0)->m_cMyCard[i].GetPos();
			g_pGameView->Page.DrawMultiScene(ptCard.x, ptCard.y, m_pSpr, 0, m_pMultiScene->m_stSceneState.nCurPlayTime, FALSE);		
		}
			
		
	}
}

void CMadeEffect::Destroy()
{	
	if ( m_pMultiScene != NULL ){
		delete m_pMultiScene;
	}
	m_pMultiScene = NULL;
}

int  CMadeEffect::ProcessObject()
{	
	if ( m_pSpr && m_pMultiScene != NULL )
	{	
		
		
		if ( m_pMultiScene->m_stSceneState.bScenePlay == TRUE ){

			DWORD curtime = m_pMultiScene->m_stSceneState.nCurPlayTime;

			m_pMultiScene->OnTimer();

			DWORD NowPlayTime = m_pMultiScene->m_stSceneState.nCurPlayTime;

			NMBASE::GRAPHICGDI::QUERYMSCENE_SOUND sndquery;
			sndquery.Reset();
			while ( m_pSpr->QueryMSceneSound( &sndquery, m_pMultiScene->m_nSceneNum, curtime , NowPlayTime, TRUE))
			{
				// ��ϵ��� ���� ���� �����̸� ���
				if (!NMBASE::SOUND::g_Sound.IsRegistered(sndquery.SndFile)) 
				{
					NMBASE::SOUND::g_Sound.RegistWav(sndquery.SndFile, NMBASE::SOUND::SNDPRIORITY_MID, 1, TRUE);
				}

				// ���� ����
				NMBASE::SOUND::g_Sound.PlayWav(sndquery.SndFile, FALSE, sndquery.Volume);
			}


			

		}
		else{
			Destroy();
			return PROCESS_KILLME;
		}
	}
	return PROCESS_OK;
}