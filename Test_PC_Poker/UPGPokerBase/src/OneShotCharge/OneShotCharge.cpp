#include "stdafx.h"
#include "OneShotCharge.h"
#include "../UIInfoData.h"
#include "../SeatKeeper/SeatKeeper.h"
#ifndef BUILD_NEWPOKER
#include "CommMsgDef_Game.h"
#else
#include "CommMsgDef.h"
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

COneShotCharge::COneShotCharge()
{
	m_pUIInfoData = NULL;
	m_pGameProcess = NULL;
	m_pGameView = NULL;
	m_pPage = NULL;
	m_pSeatKeeper = NULL;
	m_pCloseBtnSpr = NULL;
	m_bShowAskDlg = false;
	m_bWaitResult = false;
	m_strItemName = "";
	m_bShowResultDlg = false;

	m_pPossibilityMoney = NULL;
	m_pOnceMoney = NULL;
	m_pTwiceMoney = NULL;
	m_bRevervation = false;
	m_pErrorMsg = NULL;

	m_pSuccessMsg1 = NULL;
	m_pSuccessMsg2 = NULL;

	m_pReservationMsg1	= NULL;
	m_pReservationMsg2	= NULL;
	m_pReservationMsg3	= NULL;
	m_bShowBtn			= true;
}

COneShotCharge::~COneShotCharge()
{
	if(m_pPossibilityMoney) delete m_pPossibilityMoney; m_pPossibilityMoney = NULL;
	if(m_pOnceMoney) delete m_pOnceMoney; m_pOnceMoney = NULL;
	if(m_pTwiceMoney) delete m_pTwiceMoney; m_pTwiceMoney = NULL;
	if(m_pErrorMsg) delete m_pErrorMsg; m_pErrorMsg = NULL;
	if(m_pSuccessMsg1) delete m_pSuccessMsg1; m_pSuccessMsg1 = NULL;
	if(m_pSuccessMsg2) delete m_pSuccessMsg2; m_pSuccessMsg2 = NULL;
	if(m_pReservationMsg1) delete m_pReservationMsg1; m_pReservationMsg1 = NULL;
	if(m_pReservationMsg2) delete m_pReservationMsg2; m_pReservationMsg2 = NULL;
	if(m_pReservationMsg3) delete m_pReservationMsg3; m_pReservationMsg3 = NULL;
}

void COneShotCharge::LoadImage(void)
{
	AddLoadImageList(&m_sprOneShotCharge,".\\PokerCommon\\data\\sp_oneshotCharge.spr", true);
	AddLoadImageList(&m_sprOneShotChargeBtn,".\\PokerCommon\\data\\oneshotChargeBtn.spr",true);
}

void COneShotCharge::OnEnterRoom(void)
{
	m_bShowAskDlg = false;
	m_bRevervation = false;
	m_bWaitResult = false;
	m_bShowResultDlg = false;
	m_bFail = false;
	CheckStatus();
}

void COneShotCharge::Init(CUIInfoData *pUIInfoData, CGameProcess *pProcess, CSeatKeeper *pSeatKeeper, NMBASE::GRAPHICGDI::xSprite *pCloseBtnSpr)
{
	m_pUIInfoData = pUIInfoData;
	m_pGameProcess = pProcess;
	m_pGameView = pProcess->GetGame()->GetGameView();
	m_pPage = &m_pGameView->Page;
	m_pSeatKeeper = pSeatKeeper;
	m_pCloseBtnSpr = pCloseBtnSpr;

	m_rtRect.SetRect(0,0,m_sprOneShotCharge.spr[8].xl, m_sprOneShotCharge.spr[8].yl);
	m_rtRect.OffsetRect(pUIInfoData->m_ptOneShotDlg);

	m_btnOneShotCharge.Init(m_pGameView, m_pPage, pUIInfoData->m_listButtonPos[EBT_ONESHOTCHARGE].x, pUIInfoData->m_listButtonPos[EBT_ONESHOTCHARGE].y, &m_sprOneShotChargeBtn, 0, IDM_ONESHOT_CHARGE);
	m_btnOnceCharge.Init(m_pGameView, m_pPage, pUIInfoData->m_ptOneShotDlg.x + pUIInfoData->m_ptOneShotDlg_btnOnceGab.x, pUIInfoData->m_ptOneShotDlg.y + pUIInfoData->m_ptOneShotDlg_btnOnceGab.y, &m_sprOneShotCharge, 0, IDM_ONESHOT_ONCE);
	m_btnTwiceCharge.Init(m_pGameView, m_pPage, pUIInfoData->m_ptOneShotDlg.x + pUIInfoData->m_ptOneShotDlg_btnTwiceGab.x, pUIInfoData->m_ptOneShotDlg.y + pUIInfoData->m_ptOneShotDlg_btnTwiceGab.y, &m_sprOneShotCharge, 1, IDM_ONESHOT_TWICE);
	m_btnClose.Init(m_pGameView, m_pPage, pUIInfoData->m_ptOneShotDlg.x + pUIInfoData->m_ptOneShotDlg_btnCloseGab.x, pUIInfoData->m_ptOneShotDlg.y + pUIInfoData->m_ptOneShotDlg_btnCloseGab.y, m_pCloseBtnSpr, 0, IDM_ONESHOT_CLOSE);

	CRect rtRect = m_pUIInfoData->m_rtOneShotPossibilityMoneySize;
	rtRect.OffsetRect(m_pUIInfoData->m_ptOneShotPossibilityMoneyGab + pUIInfoData->m_ptOneShotDlg);
	m_pPossibilityMoney = new NMBASE::SKINGDI::CNText(rtRect, "", 0, m_pPage, m_pGameView->m_pDC);
	m_pPossibilityMoney->SetColor(m_pUIInfoData->m_clrPossibilityMoney);
	m_pPossibilityMoney->SetOutLine(m_pUIInfoData->m_clrPossibilityMoneyOutline);
	m_pPossibilityMoney->SetFont(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));
	m_pPossibilityMoney->SetAlign(DT_SINGLELINE | DT_RIGHT | DT_VCENTER);

	rtRect = m_pUIInfoData->m_rtOneShotOnceTextSize;
	rtRect.OffsetRect(m_pUIInfoData->m_ptOneShotOnceTextGab + pUIInfoData->m_ptOneShotDlg);
	m_pOnceMoney = new NMBASE::SKINGDI::CNText(rtRect, "", 1, m_pPage, m_pGameView->m_pDC);
	m_pOnceMoney->SetColor(m_pUIInfoData->m_clrText);
	m_pOnceMoney->SetOutLine(m_pUIInfoData->m_clrTextOutline);
	m_pOnceMoney->SetFont(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_BOLD));
	m_pOnceMoney->SetAlign(DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	rtRect = m_pUIInfoData->m_rtOneShotTwiceTextSize;
	rtRect.OffsetRect(m_pUIInfoData->m_ptOneShotTwiceTextGab + pUIInfoData->m_ptOneShotDlg);
	m_pTwiceMoney = new NMBASE::SKINGDI::CNText(rtRect, "", 2, m_pPage, m_pGameView->m_pDC);	
	m_pTwiceMoney->SetColor(m_pUIInfoData->m_clrText);
	m_pTwiceMoney->SetOutLine(m_pUIInfoData->m_clrTextOutline);
	m_pTwiceMoney->SetFont(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_BOLD));
	m_pTwiceMoney->SetAlign(DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	rtRect = m_pUIInfoData->m_rtOneShotError;
	rtRect.OffsetRect(m_pUIInfoData->m_ptOneShotDlg + m_pUIInfoData->m_ptOneShotErrorGab);
	m_pErrorMsg = new NMBASE::SKINGDI::CNText(rtRect, "", 3, m_pPage, m_pGameView->m_pDC);	
	m_pErrorMsg->SetColor(m_pUIInfoData->m_clrResultNormal);
	m_pErrorMsg->SetOutLine(m_pUIInfoData->m_clrResultOutline);
	m_pErrorMsg->SetFont(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));
	m_pErrorMsg->SetAlign(DT_WORDBREAK | DT_LEFT | DT_TOP);

	rtRect = m_pUIInfoData->m_rtMsgSize;
	rtRect.OffsetRect(m_pUIInfoData->m_ptOneShotDlg + m_pUIInfoData->m_ptSuccessMoneyGab);
	m_pSuccessMsg1 = new NMBASE::SKINGDI::CNText(rtRect, "", 4, m_pPage, m_pGameView->m_pDC);	
	m_pSuccessMsg1->SetColor(m_pUIInfoData->m_clrResultEmphasis);
	m_pSuccessMsg1->SetOutLine(m_pUIInfoData->m_clrResultOutline);
	m_pSuccessMsg1->SetFont(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_BOLD));
	m_pSuccessMsg1->SetAlign(DT_WORDBREAK | DT_LEFT | DT_TOP);

	rtRect.OffsetRect(CPoint(0,m_pUIInfoData->m_nLineGab));
	m_pSuccessMsg2 = new NMBASE::SKINGDI::CNText(rtRect, "", 5, m_pPage, m_pGameView->m_pDC);	
	m_pSuccessMsg2->SetColor(m_pUIInfoData->m_clrResultNormal);
	m_pSuccessMsg2->SetOutLine(m_pUIInfoData->m_clrResultOutline);
	m_pSuccessMsg2->SetFont(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));
	m_pSuccessMsg2->SetAlign(DT_WORDBREAK | DT_LEFT | DT_TOP);

	m_pSuccessMsg2->SetText("지급이 완료 되었습니다.");

	rtRect = m_pUIInfoData->m_rtMsgSize;
	rtRect.OffsetRect(m_pUIInfoData->m_ptOneShotDlg + m_pUIInfoData->m_ptReservationGab);
	m_pReservationMsg1 = new NMBASE::SKINGDI::CNText(rtRect, "", 6, m_pPage, m_pGameView->m_pDC);	
	m_pReservationMsg1->SetColor(m_pUIInfoData->m_clrResultNormal);
	m_pReservationMsg1->SetOutLine(m_pUIInfoData->m_clrResultOutline);
	m_pReservationMsg1->SetFont(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));
	m_pReservationMsg1->SetAlign(DT_WORDBREAK | DT_LEFT | DT_TOP);

	m_pReservationMsg1->SetText("이번판 종료후");

	rtRect.OffsetRect(CPoint(0,m_pUIInfoData->m_nLineGab));
	m_pReservationMsg2 = new NMBASE::SKINGDI::CNText(rtRect, "", 7, m_pPage, m_pGameView->m_pDC);	
	m_pReservationMsg2->SetColor(m_pUIInfoData->m_clrResultEmphasis);
	m_pReservationMsg2->SetOutLine(m_pUIInfoData->m_clrResultOutline);
	m_pReservationMsg2->SetFont(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_BOLD));
	m_pReservationMsg2->SetAlign(DT_WORDBREAK | DT_LEFT | DT_TOP);

	rtRect.OffsetRect(CPoint(0,m_pUIInfoData->m_nLineGab));
	m_pReservationMsg3 = new NMBASE::SKINGDI::CNText(rtRect, "", 7, m_pPage, m_pGameView->m_pDC);	
	m_pReservationMsg3->SetColor(m_pUIInfoData->m_clrResultNormal);
	m_pReservationMsg3->SetOutLine(m_pUIInfoData->m_clrResultOutline);
	m_pReservationMsg3->SetFont(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));
	m_pReservationMsg3->SetAlign(DT_WORDBREAK | DT_LEFT | DT_TOP);

	m_pReservationMsg3->SetText("지급 예정입니다.");
}

void COneShotCharge::CheckStatus(void)
{

#ifdef _BETAVER
	return;
#endif

	bool bHave = g_PremOption.HaveOption( PREMOPTION::CODE_OPTION_JEKI )?true:false;
	int nCount = g_PremOption.GetRemainCount( PREMOPTION::CODE_OPTION_JEKI );

	if(bHave)
	{
		if(m_bRevervation) m_btnOneShotCharge.Enable(FALSE);
		else if(nCount > 0) m_btnOneShotCharge.Enable(TRUE);
		else m_btnOneShotCharge.Enable(FALSE);

		switch(nCount)
		{
		case 0:
			m_btnOnceCharge.Enable(FALSE);
			m_btnTwiceCharge.Enable(FALSE);
			break;
		case 1:
			m_btnOnceCharge.Enable(FALSE);
			m_btnTwiceCharge.Enable(TRUE);
			break;
		default:
			m_btnOnceCharge.Enable(TRUE);
			m_btnTwiceCharge.Enable(TRUE);
			break;
		}

		m_btnOneShotCharge.Show(TRUE);
	}
	else
	{
		m_btnOnceCharge.Enable(FALSE);
		m_btnTwiceCharge.Enable(FALSE);
		m_btnOneShotCharge.Enable(FALSE);

		m_btnOneShotCharge.Show(FALSE);
	}

	// Test LDH
	//m_btnOneShotCharge.Enable(TRUE);
	//m_btnOneShotCharge.Show(TRUE);
}

void COneShotCharge::SetError(LPCTSTR strErrMsg)
{
	ASSERT(strErrMsg != NULL);
	m_bWaitResult = false;
	m_bShowAskDlg = false;
	m_bShowResultDlg = true;
	m_bRevervation = false;
	m_bFail = true;

	m_pErrorMsg->SetText(strErrMsg);
}

void COneShotCharge::SetReservation(void)
{
	m_bWaitResult = false;
	m_bShowAskDlg = false;
	m_bShowResultDlg = true;
	m_bRevervation = true;
	m_bFail = false;

	m_pReservationMsg2->SetText(NMBASE::UTIL::g_NumberToHangul(m_i64RecvMoney));
}

void COneShotCharge::SetUseItem(INT64 i64Money)
{
	m_bWaitResult = false;
	m_bShowResultDlg = true;
	m_bRevervation = false;
	m_bFail = false;

	m_pSuccessMsg1->SetText((CString)NMBASE::UTIL::g_NumberToHangul(i64Money));
}


BOOL COneShotCharge::OnPacketNotify(char* pName, int Signal, int TotSize, char *lpData)
{

#ifdef _BETAVER
	return FALSE;
#endif

	switch(Signal)
	{
		// [재기충전 작업] [2009-02-12]
		case SV_PREM_OPTION_INFO:
			{
				CSV_PREM_OPTION_INFO msg;
				msg.Get( lpData, TotSize );
				
				memcpy( &g_PremOption, msg.OptionInfo, sizeof(g_PremOption) );
				CheckStatus();
			}
			return TRUE;
		// [재기충전 작업] [2009-02-19]
		// 원샷 충전 예약 결과를 받는다.
		case CL_RECOVERY_CHARGE_INFO:
			{
				CCL_RECOVERY_CHARGE_INFO msg;
				msg.Get( lpData, TotSize );
						
				switch(*msg.Result)
				{
				case CCL_RECOVERY_CHARGE_INFO::PROHIBITED_TERM:
					SetError("결제 진행 기간중일때는 옵션상품을 사용하실 수 없습니다.");
					break;
				//예약 성공
				case CCL_RECOVERY_CHARGE_INFO::RESERVED_SUCCESS:
					SetReservation();
					break;
				//지급
				case CCL_RECOVERY_CHARGE_INFO::SUCCESS:
					break;
				case CCL_RECOVERY_CHARGE_INFO::FAILED_OVERMONEY:
					SetError("지급시 회원님의 보유 한도를 초과할 수 있어 사용하실 수 없습니다.");
					break;
				}
			}
			return TRUE;
	}
	return FALSE;
}

void COneShotCharge::OnClickOneShotCharge()
{
	CPlayer *pPlayer = m_pGameProcess->GetPlayer(0);
	if(m_pSeatKeeper->IsUsingSeatKeeper() //내가 자리지킴이 사용중일 때
		|| m_pSeatKeeper->GetReserveUser() == pPlayer->ServPNum ) //내가 자리지킴이 예약중일때?
	{
		SetError("자리지킴이 예약 또는 사용시에는 원샷충전을 사용할 수 없습니다.");
		return;
	}

	if( g_PremOption.HaveOption( PREMOPTION::CODE_OPTION_JEKI ))
	{
		if( g_PremOption.GetRemainCount( PREMOPTION::CODE_OPTION_JEKI ) > 0 )
		{
			if( m_bRevervation )
			{
				SetError("이미 원샷충전을 예약하셨습니다.");
			}
			else
			{
				m_btnOneShotCharge.Enable(FALSE);
				m_bShowAskDlg = true;

				m_nRemainCnt = g_PremOption.GetRemainCount(PREMOPTION::CODE_OPTION_JEKI);
				m_i64RecvMoney = g_PremOption.GetMoney(PREMOPTION::CODE_OPTION_JEKI);
				GetJekiOptionName( g_PremOption.GetOptionGoodsCode(PREMOPTION::CODE_OPTION_JEKI) );

				m_pPossibilityMoney->SetText((CString)NMBASE::UTIL::g_NumberToHangul(m_i64RecvMoney));
				m_pOnceMoney->SetText((CString)NMBASE::UTIL::g_NumberToHangul(m_i64RecvMoney));
				m_pTwiceMoney->SetText((CString)NMBASE::UTIL::g_NumberToHangul(m_i64RecvMoney/2));
			}
		}
		else
		{
			m_nItemKind = -1;
			SetError("원샷충전 상품을 모두 사용하셨습니다.");
		}
	}
}

INT64 COneShotCharge::GetCompareMoney(void)
{
	// 		1. 얼럿 노출 기준  
	// 		- 원샷 충전 3만,5만 지급액의 경우 회원제 가입 시점 
	// 		일할 계산되므로 고정액 아님 
	// 		- 원샷 충전 받기 클릭 시점
	// 		1) 게임 중인 경우 
	// 		현재 판이 아닌 전판 보유 자산을 기준으로
	// 		클릭한 금액을 받는 경우 초과금 발생할 때 노출 
	// 		* 이 경우 진행중인 판 결과 반영 후 상황이 달라질 수 
	// 		있지만 우선 체크하여 노출함 
	// 		2) 게임 시작 전인 경우 : 현재 보유 자산 기준으로 
	// 		클릭한 금액을 받는 경우 초과금 발생할 때 노출 
	
	if( g_RI.State == 0 )
	{
		//return GM().GetMyInfo()->UI.GetMoney(IsTournament()); //현재머니
		return GM().GetMyInfo()->UI.GetMoney(); //현재머니
	}

	CPlayer *pPlayer = m_pGameProcess->GetPlayer(0);
	return pPlayer->nRoundPrevMoney;  //게임시작전 머니	
}

void COneShotCharge::GetJekiOptionName( const char* OptionGoodsCode)
{
	if(NULL == OptionGoodsCode ) return;
	m_nItemKind = atoi( OptionGoodsCode ) - 1;
}

void COneShotCharge::OnClickOneShotOnceCharge()
{
	//보유한도초과금
	if (GetCompareMoney() + m_i64RecvMoney > g_MaxServerMoney)
	{
		SetError("지급시 회원님의 보유 한도를 초과할 수 있어 사용하실 수 없습니다.");
		return;
	}

	if( m_bWaitResult ) return;

	m_bWaitResult = true;
	m_bShowAskDlg = false;

	CCL_ASK_RECOVERY_CHARGE msg;
	msg.Set( CCL_ASK_RECOVERY_CHARGE::ALLIN );
	NMBASE::SOCK::GSOCKMAN().SendData( g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize() );
}

void COneShotCharge::OnClickOneShotTwiceCharge()
{
	//보유한도초과금
	if (GetCompareMoney() + (m_i64RecvMoney/2) > g_MaxServerMoney)
	{
		SetError("지급시 회원님의 보유 한도를 초과할 수 있어 사용하실 수 없습니다.");
		return;
	}
	
	// TODO: Add your control notification handler code here
	if( m_bWaitResult ) return;

	m_bWaitResult = true;
	m_bShowAskDlg = false;
	m_i64RecvMoney = m_i64RecvMoney/2;

	CCL_ASK_RECOVERY_CHARGE msg;
	msg.Set( CCL_ASK_RECOVERY_CHARGE::HALF );
	NMBASE::SOCK::GSOCKMAN().SendData( g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize() );
}

void COneShotCharge::OnClickOneShotClose()
{
	if(m_bShowAskDlg)
	{
		m_bShowAskDlg = false;
	}
	else if(m_bShowResultDlg)
	{
		m_bShowResultDlg = false;
	}

	CheckStatus();
}

BOOL COneShotCharge::OnCommand(WPARAM wParam, LPARAM lParam)
{

#ifdef _BETAVER
	return FALSE;
#endif

	switch(wParam)
	{
	case IDM_ONESHOT_CHARGE:
		if(!m_bShowBtn || m_bShowAskDlg || m_bWaitResult || m_bShowResultDlg) break;
		OnClickOneShotCharge();
		return TRUE;
	case IDM_ONESHOT_ONCE:
		if(!m_bShowAskDlg || m_bWaitResult || m_bShowResultDlg) break;
		OnClickOneShotOnceCharge();
		return TRUE;
	case IDM_ONESHOT_TWICE:
		if(!m_bShowAskDlg || m_bWaitResult || m_bShowResultDlg) break;
		OnClickOneShotTwiceCharge();
		return TRUE;
	case IDM_ONESHOT_CLOSE:
		if(!m_bShowAskDlg && !m_bShowResultDlg) break;
		OnClickOneShotClose();
		return TRUE;
	}

	return FALSE;
}

BOOL COneShotCharge::OnLButtonDown(int x , int y)
{

#ifdef _BETAVER
	return FALSE;
#endif

	if(g_bMyObserver) return FALSE;

	if(m_bShowBtn && m_btnOneShotCharge.OnLButtonDown(x, y)) return TRUE;
	if((m_bShowAskDlg || m_bShowResultDlg) && m_rtRect.PtInRect(CPoint(x,y)))
	{
		if(m_bShowAskDlg)
		{
			if(m_btnOnceCharge.OnLButtonDown(x, y)) return TRUE;
			if(m_btnTwiceCharge.OnLButtonDown(x, y)) return TRUE;
		}
		if(m_btnClose.OnLButtonDown(x, y)) return TRUE;
		return TRUE;
	}
	return FALSE;
}

BOOL COneShotCharge::OnLButtonUp(int x , int y)
{

#ifdef _BETAVER
	return FALSE;
#endif

	if(g_bMyObserver) return FALSE;

	if(m_bShowBtn && m_btnOneShotCharge.OnLButtonUp(x, y)) return TRUE;
	if((m_bShowAskDlg || m_bShowResultDlg) && m_rtRect.PtInRect(CPoint(x,y)))
	{
		if(m_bShowAskDlg)
		{
			if(m_btnOnceCharge.OnLButtonUp(x, y)) return TRUE;
			if(m_btnTwiceCharge.OnLButtonUp(x, y)) return TRUE;
		}
		if(m_btnClose.OnLButtonUp(x, y)) return TRUE;
		return TRUE;
	}
	return FALSE;
}

BOOL COneShotCharge::OnMouseMove(int x , int y)
{

#ifdef _BETAVER
	return FALSE;
#endif

	if(g_bMyObserver) return FALSE;

	if(m_bShowBtn && m_btnOneShotCharge.OnMouseMove(x, y))
	{
		SetCursor(GM().GetCurHandCursor());
		return TRUE;
	}

	if((m_bShowAskDlg || m_bShowResultDlg) && m_rtRect.PtInRect(CPoint(x,y)))
	{
		if(m_bShowAskDlg)
		{
			if(m_btnOnceCharge.OnMouseMove(x, y))
			{
				SetCursor(GM().GetCurHandCursor());
				return TRUE;
			}
			if(m_btnTwiceCharge.OnMouseMove(x, y))
			{
				SetCursor(GM().GetCurHandCursor());
				return TRUE;
			}
		}
		if(m_btnClose.OnMouseMove(x, y))
		{
			SetCursor(GM().GetCurHandCursor());
			return TRUE;
		}
		return TRUE;
	}
	return FALSE;
}

void COneShotCharge::Draw(CDC *pDC)
{

#ifdef _BETAVER
	return;
#endif

	if(g_bMyObserver) return;

	if(m_bShowResultDlg)
	{
		int nBackIndex = 0;
		switch(m_nItemKind)
		{
		case -1:
			nBackIndex = 11;
			break;
		case 0:
			nBackIndex = 9;
			break;
		case 1:
			nBackIndex = 10;
			break;
		case 2:
			nBackIndex = 12;
			break;
		default: return;
		}

		if(m_bShowBtn) m_btnOneShotCharge.Draw(pDC, 0, 31);
		m_pPage->PutSprAuto(m_rtRect.left, m_rtRect.top, &m_sprOneShotCharge, nBackIndex);


		if(m_bFail)
		{
			if(m_nItemKind != -1) m_pErrorMsg->DrawText(pDC);
		}
		else if(m_bRevervation)
		{
			m_pReservationMsg1->DrawText(pDC);
			m_pReservationMsg2->DrawText(pDC);
			m_pReservationMsg3->DrawText(pDC);
		}
		else
		{
			m_pSuccessMsg1->DrawText(pDC);
			m_pSuccessMsg2->DrawText(pDC);
		}

		m_btnClose.Draw(pDC, 0, 31);
	}
	else
	{
		if(m_bShowBtn) m_btnOneShotCharge.Draw(pDC, 0, 31);
		if(m_bShowAskDlg)
		{
			m_pPage->PutSprAuto(m_rtRect.left, m_rtRect.top, &m_sprOneShotCharge, 8);

			m_btnOnceCharge.Draw(pDC, 0, 31);
			m_btnTwiceCharge.Draw(pDC, 0, 31);
			m_btnClose.Draw(pDC, 0, 31);

			m_pPossibilityMoney->DrawText(pDC);
			m_pOnceMoney->DrawText(pDC);
			m_pTwiceMoney->DrawText(pDC);
		}
	}
}

void COneShotCharge::OnChargeMoney(CSV_CHANGE_MONEY& msg)
{
	if(g_bMyObserver) return;

	int PNum = m_pGameProcess->GetPlayerPNum(msg.ID);
	int LeftCnt = *msg.SmallData_1;
	int TotalLeftCnt = *msg.SmallData_2;
	INT64 addmoney = (INT64)*msg.ChangeMoney;
	
	if( 0 == PNum )
	{			
		g_PremOption.SetCount( PREMOPTION::CODE_OPTION_JEKI, LeftCnt );

		CString optioncode_name;
		CString optioncode_recv;
		optioncode_recv.Format( "%3.d", TotalLeftCnt );
		GetJekiOptionName( optioncode_recv );

		SetUseItem(addmoney);
	}

	CString strOutput;
	strOutput.Format("[%s]님이 원샷충전으로 %s을(를) 지급받으셨습니다.\n", msg.ID, NMBASE::UTIL::g_NumberToHangul(addmoney));
	AddGameInfoViewEdit(strOutput, m_pUIInfoData->m_listChatColor[ECC2_USEITEM]);
}