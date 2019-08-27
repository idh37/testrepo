// GH_CardValueMan.cpp: implementation of the CGameHelperMan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameHelperMan.h"
#include "../UIInfoData.h"
#include "JokboProbability.h"
#include "OtherUserJokboCheckMan.h"
#include "../GlobalPokerBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGameHelperMan::CGameHelperMan()
{
	m_nStatus = EGHS_NOITEM;

	m_pUIInfoData	= NULL;
	m_pGameView		= NULL;
	m_pGameProcess	= NULL;

	m_pOtherUserJokboCheckMan	= NULL;
	m_pJokboProbability			= NULL;
	m_plistProbabilityRect		= NULL;
	m_nMaxRule = 0;
	m_bUseResultDlg = false;
	m_bBuyInfoDlg = false;
	m_nInfo = -1;

	MM().AppendMsgH(UPG_CMK_UPDATE_ITEM_DATA, Fnt(this, &CGameHelperMan::OnUseItem));
}

CGameHelperMan::~CGameHelperMan()
{
	if(m_plistProbabilityRect)	delete [] m_plistProbabilityRect;
	m_plistProbabilityRect = NULL;

	if(m_pOtherUserJokboCheckMan) delete m_pOtherUserJokboCheckMan;
	m_pOtherUserJokboCheckMan = NULL;

	if(m_pJokboProbability) delete m_pJokboProbability;
	m_pJokboProbability = NULL;

	MM().RemoveAppendMsgH(UPG_CMK_UPDATE_ITEM_DATA, Fnt(this, &CGameHelperMan::OnUseItem));
}

void CGameHelperMan::Clear()
{	
	m_nStatus = EGHS_NOITEM;
	m_bUseResultDlg = false;
	m_bBuyInfoDlg = false;
	m_nInfo = -1;
	Reset();
}

void CGameHelperMan::ClearProbability()
{
	m_pJokboProbability->Clear();
}

void CGameHelperMan::LoadImage(LPCTSTR lpJokboBtnPath, LPCTSTR lpDoumiPath, LPCTSTR lpTablePath, LPCTSTR lpJokboInfo)
{
	AddLoadImageList(&m_sprBtn, lpJokboBtnPath, true);
	AddLoadImageList(&m_sprJBTable, lpTablePath, true);
	AddLoadImageList(&m_sprUseHelper, lpDoumiPath, true);
	AddLoadImageList(&m_sprInfo, lpJokboInfo, true);
	
	AddLoadImageList(&m_sprOnOff,".\\PokerCommon\\data\\jokbo_onoff.spr", true);
	AddLoadImageList(&m_sprArrow,".\\PokerCommon\\data\\jokbo_complete.spr", true);
	AddLoadImageList(&m_sprUseDlg,".\\PokerCommon\\data\\sp_doumi.spr", true);
	AddLoadImageList(&m_sprUseBtn,".\\PokerCommon\\data\\sp_useBtn.spr", true);
	AddLoadImageList(&m_sprUseCancelBtn,".\\PokerCommon\\data\\sp_cancelBtn.spr", true);
	AddLoadImageList(&m_sprUseOkBtn,".\\PokerCommon\\data\\sp_checkBtn.spr", true);
}

void CGameHelperMan::Init(CUIInfoData *pUIInfoData, CGame *pGame, int nMaxRule, COtherUserJokboCheckMan *pOtherUserJokboCheckMan, CJokboProbability *pJokboProbability)
{
	ASSERT(pUIInfoData != NULL && pGame != NULL && pOtherUserJokboCheckMan != NULL && pJokboProbability);
	
	m_pUIInfoData = pUIInfoData;
	m_pGameProcess = pGame->GetGameProcess();
	m_pGameView = pGame->GetGameView();
	m_pOtherUserJokboCheckMan = pOtherUserJokboCheckMan;
	m_pJokboProbability = pJokboProbability;
	m_nMaxRule = nMaxRule;
	m_nMaxPlayer = pGame->GetMaxPlayer();
	
	m_plistProbabilityRect = new CRect[m_nMaxRule];
	
	Clear();
	
	m_pJokboProbability->Init(m_pGameProcess->GetMaxPlayer(), m_nMaxRule);
	m_pJokboProbability->Clear();
	m_pOtherUserJokboCheckMan->Init(&m_sprArrow);
	
	//m_btnOn.Init(m_pGameView, &m_pGameView->Page, m_pUIInfoData->m_listButtonPos[EBT_JOKBOHELP_ON].x, m_pUIInfoData->m_listButtonPos[EBT_JOKBOHELP_ON].y, &m_sprOnOff, 0, UM_GH_RULE_ON);
	//m_btnOff.Init(m_pGameView, &m_pGameView->Page, m_pUIInfoData->m_listButtonPos[EBT_JOKBOHELP_OFF].x, m_pUIInfoData->m_listButtonPos[EBT_JOKBOHELP_OFF].y, &m_sprOnOff, 1, UM_GH_RULE_OFF);
	m_btnUse.Init(m_pGameView, &m_pGameView->Page, m_pUIInfoData->m_listButtonPos[EBT_JOKBOHELP_USE].x, m_pUIInfoData->m_listButtonPos[EBT_JOKBOHELP_USE].y, &m_sprOnOff, 2, UM_GH_USE);
	
	CPoint ptPos = m_pUIInfoData->m_ptJokboHelpUseDlg + m_pUIInfoData->m_ptJokboHelpUseDlgUseBtnGab;
	m_btnUseDlgUseBtn.Init(m_pGameView, &m_pGameView->Page, ptPos.x, ptPos.y, &m_sprUseBtn, 0, UM_GH_USEDLG_USE);
	
	ptPos = m_pUIInfoData->m_ptJokboHelpUseDlg + m_pUIInfoData->m_ptJokboHelpUseDlgCancelBtnGab;
	m_btnUseDlgCancelBtn.Init(m_pGameView, &m_pGameView->Page, ptPos.x, ptPos.y, &m_sprUseCancelBtn, 0, UM_GH_USEDLG_CANCEL);
	
	ptPos = m_pUIInfoData->m_ptJokboHelpUseDlg + m_pUIInfoData->m_ptJokboHelpUseDlgOK;
	m_btnUseDlgOKBtn.Init(m_pGameView, &m_pGameView->Page, ptPos.x, ptPos.y, &m_sprUseOkBtn, 0, UM_GH_USEDLG_OK);
	
	m_rtUseDlg = CRect(0,0,m_sprUseDlg.spr[0].xl, m_sprUseDlg.spr[0].yl);
	m_rtUseDlg.OffsetRect(m_pUIInfoData->m_ptJokboHelpUseDlg);
	
	m_btnOn.Enable(FALSE);
	m_btnOff.Enable(FALSE);
	m_btnUse.Enable(FALSE);
	m_btnUseDlgUseBtn.Enable(FALSE);
	m_btnUseDlgCancelBtn.Enable(FALSE);
	m_btnUseDlgOKBtn.Enable(FALSE);
	
	for(int i = 0 ; i < m_nMaxRule ; i++)
	{
		m_plistProbabilityRect[i] = m_pUIInfoData->m_rtJokboHelpPercent;
		m_plistProbabilityRect[i].OffsetRect(m_pUIInfoData->m_ptJokboHelpPercent.x,m_pUIInfoData->m_ptJokboHelpPercent.y+(((m_nMaxRule-1)-i)*m_pUIInfoData->m_nJokboHelpHightGab));				
	}
	m_pJokboProbability->Clear();
}

void CGameHelperMan::Reset()
{
	m_nMyCurJokboIndex = -1;
	m_pJokboProbability->Clear();
	m_pOtherUserJokboCheckMan->Clear();
}

void CGameHelperMan::CheckItemHaveAndUse(void)
{
	
	BOOL bHaveItem = FALSE;
	BOOL bUseItem = FALSE;

	//아이템이 있는지 확인한다.
	if( UPGIsGameItemUsing(ITEM_SCODE_POKER_HELP, &bHaveItem) )
	{
		bHaveItem = TRUE;
		bUseItem = TRUE;
	}	
	else{
		bUseItem = FALSE;
	}

	//블루 회원제 이상이거나 PC방 사용자 이면 무조건 무제한 사용가능
	if( UPGCheckPremiumState(&GM().GetMyInfo()->UI) > CODE_NEW_FAMILY_MINI || GM().GetMyInfo()->UI.nPCRoomIPType > 0 )
	{
		bHaveItem = TRUE;
		bUseItem = TRUE;
	}

	if(bHaveItem)
	{
		if(bUseItem)
		{
			m_nStatus = EGHS_USEITEM_ON;
		}
		else
		{
			m_nStatus = EGHS_NOTUSEITEM;
		}
	}
	else
	{
		m_nStatus = EGHS_NOITEM;
	}
}

void CGameHelperMan::CheckUseItemState(void)
{
	CheckItemHaveAndUse();

	m_btnUseDlgUseBtn.Enable(FALSE);
	m_btnUseDlgCancelBtn.Enable(FALSE);
	m_btnUseDlgOKBtn.Enable(FALSE);

	switch(m_nStatus)
	{
	case EGHS_NOITEM:
		m_btnOn.Enable(TRUE);
		m_btnOff.Enable(FALSE);
		m_btnUse.Enable(FALSE);
		break;
	case EGHS_USEITEM_ON:
		m_btnOn.Enable(FALSE);
		m_btnOff.Enable(TRUE);
		m_btnUse.Enable(FALSE);
		break;
	case EGHS_NOTUSEITEM:
		m_btnOn.Enable(FALSE);
		m_btnOff.Enable(FALSE);
		m_btnUse.Enable(TRUE);
		break;
	}
}

void CGameHelperMan::OnEnterRoom(void)
{
	Clear();	
	CheckUseItemState();

	switch(GM().GetCurrentGameType())
	{
	case IDX_GAME_BD:
		m_btnOn.Show(FALSE);
		m_btnOff.Show(FALSE);
		m_btnUse.Show(FALSE);
		m_nStatus = EGHS_USEITEM_OFF;
		break;
	}
}

void CGameHelperMan::SetAllUserCardInfo()
{
	if(g_bMyObserver || m_nStatus == EGHS_NOITEM) return;

	m_pJokboProbability->Clear();
	m_pJokboProbability->SetAllUserCardInfo();
}

void CGameHelperMan::CalculationMyJokbo()
{
	if(g_bMyObserver) return;
	m_nMyCurJokboIndex = m_pJokboProbability->GetMyCurJokboIndex();	
}

void CGameHelperMan::CalculationProbability()
{	
	if(g_bMyObserver) return;
	CalculationMyJokbo();
	m_pJokboProbability->CalculationProbability();
}

void CGameHelperMan::CheckOtherJokboCheck( int pnum )
{	
	if(g_bMyObserver || m_nStatus == EGHS_NOITEM) return;

	if( pnum < 0 || pnum >= m_nMaxPlayer ){
		m_pOtherUserJokboCheckMan->Clear();		
		return;
	}
	m_pOtherUserJokboCheckMan->CheckJokbo();
}

void CGameHelperMan::Draw( CDC *pDC , NMBASE::GRAPHICGDI::CPage *pPage )
{
	if(g_bMyObserver) return;
	if(pDC == NULL || pPage == NULL) return;

	CFont *pFont = pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));
	COLORREF color = pDC->GetTextColor();

	switch(m_nStatus)
	{
	case EGHS_USEITEM_ON:
		m_btnOn.Draw(pDC);
		m_btnOff.Draw(pDC);

		DrawProbability(pDC,pPage);
		break;
	case EGHS_NOITEM:
	case EGHS_USEITEM_OFF:
		m_btnOn.Draw(pDC);
		m_btnOff.Draw(pDC);
		DrawProbability(pDC,pPage);
		break;
	case EGHS_NOTUSEITEM:
		m_btnUse.Draw(pDC);
		DrawProbability(pDC,pPage);
		break;
	case EGHS_USEDLG:
		m_btnUse.Draw(pDC);
		DrawProbability(pDC,pPage);
		DrawUseDlg(pDC, pPage);
		break;
	}

	if(m_bUseResultDlg) DrawUseResultDlg(pDC, pPage);
	if(m_bBuyInfoDlg) DrawBuyInfoDlg(pDC, pPage);

	pDC->SetTextColor(color);
	pDC->SelectObject(pFont);
}

void CGameHelperMan::DrawUseDlg( CDC *pDC , NMBASE::GRAPHICGDI::CPage *pPage )
{
	pPage->PutSprAuto( m_pUIInfoData->m_ptJokboHelpUseDlg.x, m_pUIInfoData->m_ptJokboHelpUseDlg.y, &m_sprUseDlg, 0);
	m_btnUseDlgUseBtn.Draw(pDC);
	m_btnUseDlgCancelBtn.Draw(pDC);
}

void CGameHelperMan::DrawUseResultDlg( CDC *pDC , NMBASE::GRAPHICGDI::CPage *pPage )
{
	pPage->PutSprAuto( m_pUIInfoData->m_ptJokboHelpUseDlg.x, m_pUIInfoData->m_ptJokboHelpUseDlg.y, &m_sprUseDlg, 1);
	m_btnUseDlgOKBtn.Draw(pDC);
}

void CGameHelperMan::DrawBuyInfoDlg( CDC *pDC , NMBASE::GRAPHICGDI::CPage *pPage )
{
	pPage->PutSprAuto( m_pUIInfoData->m_ptJokboHelpUseDlg.x, m_pUIInfoData->m_ptJokboHelpUseDlg.y, &m_sprUseDlg, 2);
	m_btnUseDlgOKBtn.Draw(pDC);
}

void CGameHelperMan::DrawProbability( CDC *pDC , NMBASE::GRAPHICGDI::CPage *pPage )
{
	if( pDC == NULL || pPage == NULL )return;	

	bool bDrawProbability = (m_pGameProcess->GetPlayer(0)->PlayState == 1)?true:false;
	int index = m_nMyCurJokboIndex;
	if(bDrawProbability == false) index = -1;

	//족보 확률 표시
	CString str;
	CRect rt;

	int nCompleteIndex = 12;
	if(GM().GetCurrentGameType() == IDX_GAME_BD) nCompleteIndex = 13;
	
	for( int i= 0 ; i < m_nMaxRule ; i ++ )
	{
		pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));
		SetTextCharacterExtra(*pDC, -1);

		rt = m_plistProbabilityRect[i];

		if( i <= index )
		{
			if(i == index) 
			{
				pPage->PutSprAuto(m_plistProbabilityRect[i].left + m_pUIInfoData->m_ptJokboHelpCompleteGab.x,m_plistProbabilityRect[i].top + m_pUIInfoData->m_ptJokboHelpCompleteGab.y, &m_sprOnOff, nCompleteIndex, AB);
				pDC->SetTextColor(m_pUIInfoData->m_clrJokboHelpMyJokboName);
			}
			else
			{
				pDC->SetTextColor(m_pUIInfoData->m_clrJokboHelpNoPossibilityName);
			}
			pDC->DrawText(m_pUIInfoData->GetJokboName(i),&rt, DT_VCENTER | DT_CENTER | DT_WORDBREAK | DT_SINGLELINE);

			SetTextCharacterExtra(*pDC, 0);

			pDC->SetTextColor(m_pUIInfoData->m_clrJokboHelpPercent);
			pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));

			rt.OffsetRect(m_pUIInfoData->m_ptJokboHelpPercentGab);
			str.Format("-");
			pDC->DrawText(str,&rt, DT_VCENTER | DT_RIGHT | DT_WORDBREAK | DT_SINGLELINE);

			continue;
		}

		if(bDrawProbability && m_nStatus == EGHS_USEITEM_ON)
		{
			float pro = (float)m_pJokboProbability->GetProbability(i);
			if( pro <= 0.00f )
			{
				pDC->SetTextColor(m_pUIInfoData->m_clrJokboHelpNoPossibilityName);
				pDC->DrawText(m_pUIInfoData->GetJokboName(i),&rt, DT_VCENTER | DT_CENTER | DT_WORDBREAK | DT_SINGLELINE);
				continue;
			}
			else
			{
				if(pro < 0.1) pro = 0.1f;
				pDC->SetTextColor(m_pUIInfoData->m_clrJokboHelpPossibilityName);
				pDC->DrawText(m_pUIInfoData->GetJokboName(i),&rt, DT_VCENTER | DT_CENTER | DT_WORDBREAK | DT_SINGLELINE);
			}

			SetTextCharacterExtra(*pDC, 0);

			pDC->SetTextColor(m_pUIInfoData->m_clrJokboHelpPercent);
			pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));

			rt.OffsetRect(m_pUIInfoData->m_ptJokboHelpPercentGab);

			str.Format("%0.1f%%",pro);
			pDC->DrawText(str,&rt, DT_VCENTER | DT_RIGHT | DT_WORDBREAK | DT_SINGLELINE);
		}
		else
		{
			pDC->SetTextColor(m_pUIInfoData->m_clrJokboHelpNoPossibilityName);
			pDC->DrawText(m_pUIInfoData->GetJokboName(i),&rt, DT_VCENTER | DT_CENTER | DT_WORDBREAK | DT_SINGLELINE);
		}
	}

	pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));
	SetTextCharacterExtra(*pDC, 0);
	pDC->SetTextColor(RGB(150,150,150));

	if(m_nInfo > -1)
	{
		pPage->PutSprAuto(m_pUIInfoData->m_ptJokboInfo.x, m_pUIInfoData->m_ptJokboInfo.y, &m_sprInfo, m_nInfo);
	}
	else if(index > -1)
	{

		pPage->PutSprAuto(m_pUIInfoData->m_ptJokboInfo.x, m_pUIInfoData->m_ptJokboInfo.y, &m_sprInfo, index);
	}
}

void CGameHelperMan::DrawOtherUserJokboArrow( CDC *pDC , NMBASE::GRAPHICGDI::CPage *pPage )
{
	if( pDC == NULL || pPage == NULL )return;
	if( m_nStatus != EGHS_USEITEM_ON ) return;
	m_pOtherUserJokboCheckMan->Draw( pDC, pPage);
}

BOOL CGameHelperMan::OnLButtonDown( int x, int y)
{
	if(g_bMyObserver) return FALSE;

	if((m_bUseResultDlg || m_bBuyInfoDlg) && m_btnUseDlgOKBtn.OnLButtonDown(x,y)) return TRUE;

	switch(m_nStatus)
	{
	case EGHS_USEITEM_ON:
	case EGHS_USEITEM_OFF:
	case EGHS_NOITEM:
		if (m_btnOn.OnLButtonDown(x,y))	 return TRUE;
		if (m_btnOff.OnLButtonDown(x,y)) return TRUE;
	case EGHS_NOTUSEITEM:
		if (m_btnUse.OnLButtonDown(x,y)) return TRUE;
		break;
	case EGHS_USEDLG:
		if (m_btnUseDlgUseBtn.OnLButtonDown(x,y))	return TRUE;
		if (m_btnUseDlgCancelBtn.OnLButtonDown(x,y))return TRUE;
		break;
	}

	if(m_bUseResultDlg || m_nStatus == EGHS_USEDLG || m_bBuyInfoDlg)
	{
		if (m_rtUseDlg.PtInRect(CPoint(x,y))) return TRUE;
	}

	return FALSE;
}

BOOL CGameHelperMan::OnLButtonUp( int x, int y )
{
	if(g_bMyObserver) return FALSE;
	
	if((m_bUseResultDlg || m_bBuyInfoDlg) && m_btnUseDlgOKBtn.OnLButtonUp(x,y)) return TRUE;
	
	switch(m_nStatus)
	{
	case EGHS_USEITEM_ON:
	case EGHS_USEITEM_OFF:
	case EGHS_NOITEM:
		if (m_btnOn.OnLButtonUp(x,y))	return TRUE;
		if (m_btnOff.OnLButtonUp(x,y)) return TRUE;
	case EGHS_NOTUSEITEM:
		if (m_btnUse.OnLButtonUp(x,y)) return TRUE;
		break;
	case EGHS_USEDLG:
		if (m_btnUseDlgUseBtn.OnLButtonUp(x,y))	return TRUE;
		if (m_btnUseDlgCancelBtn.OnLButtonUp(x,y))return TRUE;
		break;
	}
	
	if(m_bUseResultDlg || m_nStatus == EGHS_USEDLG || m_bBuyInfoDlg)
	{
		if (m_rtUseDlg.PtInRect(CPoint(x,y))) return TRUE;
	}
	
	return FALSE;
}

BOOL CGameHelperMan::OnMouseMove( int x, int y )
{
	if(g_bMyObserver) return FALSE;

	if((m_bUseResultDlg || m_bBuyInfoDlg) && m_btnUseDlgOKBtn.OnMouseMove(x,y))
	{
		SetCursor(GM().GetCurHandCursor());
		return TRUE;
	}

	BOOL bRet = FALSE;

	switch(m_nStatus)
	{
	case EGHS_USEITEM_ON:
	case EGHS_USEITEM_OFF:
	case EGHS_NOITEM:
		if (m_btnOn.OnMouseMove(x,y))
		{
			bRet = TRUE;
			break;
		}
		if (m_btnOff.OnMouseMove(x,y))
		{
			bRet = TRUE;
			break;
		}
	case EGHS_NOTUSEITEM:
		if (m_btnUse.OnMouseMove(x,y))
		{
			bRet = TRUE;
			break;
		}
		break;
	case EGHS_USEDLG:
		if (m_btnUseDlgUseBtn.OnMouseMove(x,y))
		{
			bRet = TRUE;
			break;
		}
		if (m_btnUseDlgCancelBtn.OnMouseMove(x,y))
		{
			bRet = TRUE;
			break;
		}
		break;
	}

	m_nInfo = -1;
	for(int i = 0 ; i < m_nMaxRule ; i++)
	{
		if(m_plistProbabilityRect[i].PtInRect(CPoint(x,y)))
		{
			m_nInfo = i;
			bRet = TRUE;
			break;
		}
	}

	if(m_bUseResultDlg || m_nStatus == EGHS_USEDLG || m_bBuyInfoDlg)
	{
		if (m_rtUseDlg.PtInRect(CPoint(x,y)))
		{
			bRet = TRUE;
		}
	}

	if(bRet)
	{
		SetCursor(GM().GetCurHandCursor());
	}

	return bRet;
}

BOOL CGameHelperMan::OnCommand( WPARAM wParam, LPARAM lParam )
{
	if(g_bMyObserver) return FALSE;
	switch(wParam)
	{
	case UM_GH_RULE_ON:
	case UM_GH_RULE_OFF:
		GameHelperOnOff();
		return TRUE;
	case UM_GH_USE:
		m_nStatus = EGHS_USEDLG;
		m_btnUse.Enable(FALSE);
		m_btnUseDlgUseBtn.Enable(TRUE);
		m_btnUseDlgCancelBtn.Enable(TRUE);
		return TRUE;
	case UM_GH_USEDLG_USE:
		m_btnUseDlgUseBtn.Enable(FALSE);
		m_btnUseDlgCancelBtn.Enable(FALSE);
		SendUsePacket();
		return TRUE;
	case UM_GH_USEDLG_CANCEL:
		m_nStatus = EGHS_NOTUSEITEM;
		m_btnUse.Enable(TRUE);
		m_btnUseDlgUseBtn.Enable(FALSE);
		m_btnUseDlgCancelBtn.Enable(FALSE);
		return TRUE;
	case UM_GH_USEDLG_OK:
		if(m_nStatus == EGHS_NOITEM)
		{
			m_bBuyInfoDlg = false;
		}
		else
		{
			m_bUseResultDlg = false;		
		}
		m_btnUseDlgOKBtn.Enable(FALSE);
		return TRUE;
	}
	return FALSE;
}

LRESULT CGameHelperMan::OnUseItem(WPARAM &wParam, LPARAM &lParam)
{
	GAMEITEM *pItem = (GAMEITEM *)lParam;
	if(pItem == NULL) return FALSE;
	if(pItem->Code == ITEM_SCODE_POKER_HELP)
	{
		CheckItemHaveAndUse();
		OnUseGameHelper((m_nStatus == EGHS_USEITEM_ON)?true:false);
		return TRUE;
	}
	return FALSE;
}

void CGameHelperMan::OnUseGameHelper(bool bUse)
{
	if(bUse)
	{
		m_bUseResultDlg = true;
		m_btnUseDlgOKBtn.Enable(TRUE);
		m_nStatus = EGHS_USEITEM_ON;

		m_btnOn.Enable(FALSE);
		m_btnOff.Enable(TRUE);		
	}
	else
	{
		m_nStatus = EGHS_NOTUSEITEM;
		m_btnUse.Enable(TRUE);
	}
}

void CGameHelperMan::SendUsePacket(void)
{
	GAMEITEM *pItem = UPGNotUseGameItem(ITEM_SCODE_POKER_HELP);
	if(pItem == NULL) return;

	CPlayer *pPlayer = GM().GetMyInfo();
	USEGAMEITEMINFO info;
	memcpy(info.ID, pPlayer->UI.ID, ID_LEN);
	memcpy(&info.Item, pItem, sizeof(GAMEITEM));

	CSV_USEGAMEITEM msg;
	msg.Set(&info);

	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
}

void CGameHelperMan::GameHelperOnOff( void )
{
	if(m_nStatus == EGHS_NOITEM)
	{
		m_bBuyInfoDlg = true;
		m_btnUseDlgOKBtn.Enable(TRUE);
		return;
	}

	if( m_nStatus == EGHS_USEITEM_ON ){		
		m_nStatus = EGHS_USEITEM_OFF;

		m_btnOn.Enable(TRUE);
		m_btnOff.Enable(FALSE);
	}
	else{
		m_nStatus = EGHS_USEITEM_ON;

		m_btnOn.Enable(FALSE);
		m_btnOff.Enable(TRUE);
	}
}

BOOL CGameHelperMan::OnPacketNotify(char* pName, int Signal, int TotSize, char *lpData)
{
	return FALSE;
}