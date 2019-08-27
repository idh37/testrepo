#include "StdAfx.h"
#include "PromotionLuckyWheel.h"
#include "PromotionManager.h"

#include "../Sound.h"


#pragma 

CPromotionLuckyWheel::CPromotionLuckyWheel( CPromotionManager *pManager, int nType )
: CPromotionBase(pManager, nType)
{
	string strDir = ".\\PokerCommon\\data\\Event\\LuckyWheel\\";
	_LOADSPRITEDEFINE(&m_sprBack, strDir, "evt_luckywheel.spr");
	memset(&m_PromotionInfo, 0, sizeof(m_PromotionInfo));
	m_cEffect_WheelPopup.SetSprite(&m_sprBack);
	m_bDrawTip=false;
	m_bJokboEffectStart=false;
	m_nPlayTime=0;

	MM().AddMsgH(UPG_CMK_SHOW_LUCKYWHEEL_POPUP, Fnt(this, &CPromotionLuckyWheel::ShowRewardPopup));
}

CPromotionLuckyWheel::~CPromotionLuckyWheel(void)
{
	MM().RemoveMsgH(UPG_CMK_SHOW_LUCKYWHEEL_POPUP);
}

LRESULT CPromotionLuckyWheel::ShowRewardPopup(WPARAM &wParam, LPARAM &lParam)
{
	if(5<=m_PromotionInfo.nWinCount){
		if(TRUE==m_cEffect_WheelPopup.IsShow()){
			return TRUE;
		}

		m_cEffect_WheelPopup.ResetEffect();
		if(IDX_GAME_HA != GM().GetCurrentGameType() && IDX_GAME_SD != GM().GetCurrentGameType()){
			m_cEffect_WheelPopup.ShowEffect(0, TRUE, FALSE);
		}
		else{
			m_cEffect_WheelPopup.ShowEffect(1, TRUE, FALSE);
		}
		m_cEffect_WheelPopup.ChangeScean(5, 10+GM().GetPromotionData()->nCnts[4]);

		m_nPlayTime=2;
	}

	return TRUE;
};

void CPromotionLuckyWheel::SetEventInfo( promotion::VISITE_PROMOTION_ALARM *pInfo )
{
	if (pInfo==NULL) 
		return;

	memcpy( &m_PromotionInfo, pInfo, sizeof( promotion::VISITE_PROMOTION_ALARM ) );

	GM().GetPromotionData()->nCnts[0] = m_PromotionInfo.nVisiteCount; // ¹æ¹® ¼ö
	GM().GetPromotionData()->nCnts[1] = m_PromotionInfo.nWinCount; // ÇöÀç ÆÇ¼ö
	GM().GetPromotionData()->nCnts[2] = m_PromotionInfo.nTodayRewardCount; // ¿À´Ã È¹µæ ¼ö
	GM().GetPromotionData()->nCnts[3] = 30;//m_PromotionInfo.nTotalRewardCount; // ÀüÃ¼ È¹µæ ¼ö
	// ´çÃ·±Ý ÀÌ¹ÌÁö index
	if (m_PromotionInfo.nRewardIndex > 6)
	{
		GM().GetPromotionData()->nCnts[4] = m_PromotionInfo.nRewardIndex + 19;
	}
	else
	{
		GM().GetPromotionData()->nCnts[4] = m_PromotionInfo.nRewardIndex;
	}

	if(5<=m_PromotionInfo.nWinCount){
		GM().GetPromotionData()->llEffectReserved = m_PromotionInfo.nRewardMoney;
	}
	else
	{
		GM().GetPromotionData()->llEffectReserved = 0ll;
	}
	GM().GetPromotionData()->llReserved = m_PromotionInfo.nRewardMoney;			// ´çÃ· ±Ý
	if(5<=GM().GetPromotionData()->nCnts[1]){
		GM().GetPromotionData()->nCnts[1] = 0; // ÇöÀç ÆÇ¼ö
	}
	if(true==IsEndEvent()){
		GM().GetPromotionData()->SetState(tPromotionData::PROMOTIONSTATE_END);
	}
	else{
		GM().GetPromotionData()->SetState(tPromotionData::PROMOTIONSTATE_ING);
	}
}

bool CPromotionLuckyWheel::IsEndEvent(void)
{
	return ((GM().GetPromotionData()->nCnts[3]<=GM().GetPromotionData()->nCnts[2])?(true):(false));
}

bool CPromotionLuckyWheel::IsEventRoom(void)
{
	return ((g_RI.llEnterLimitMoney>=m_PromotionInfo.nRunLowerLimit && g_RI.llEnterLimitMoney<=m_PromotionInfo.nRunUpperLimit)?(true):(false));
}

void CPromotionLuckyWheel::Accept_OutRoom()
{
}

void CPromotionLuckyWheel::Accept_CreateRoom()
{
}

void CPromotionLuckyWheel::Accept_EnterRoom()
{
}

void CPromotionLuckyWheel::OnEnterRoom()
{
}

void CPromotionLuckyWheel::OnEndGame()
{
}

void CPromotionLuckyWheel::DrawBottom(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage)
{
}

void CPromotionLuckyWheel::DrawMiddle(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage)
{
}

void CPromotionLuckyWheel::DrawTop(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage)
{
	if((CPromotionManager::ENUM_PROMOTION_TYPE)GM().GetPromotionData()->nCnts[0] == CPromotionManager::EPT_POKER_2018_CHUSEOK)
	{		
		return;
	}

	// °ñµå ¹æÀÏ °æ¿ì ·°Å° ÈÙ Á¦¿Ü
	if (g_RI.ChipKind == 1)
		return;
	
	if (IsEventRoom() == false)
		return;
	
	DWORD dwNow = timeGetTime();
	if (IDX_GAME_SD == GM().GetCurrentGameType())
	{
		if (IsEndEvent() == true)
		{
			pPage->DrawMultiScene(50, 0, &m_sprBack, 10, dwNow, TRUE);
		}
		else if (0 < m_PromotionInfo.nWinCount)
		{
			pPage->DrawMultiScene(50, 0, &m_sprBack, 9, dwNow, TRUE);
			pPage->DrawMultiScene(50, 0, &m_sprBack, 10+m_PromotionInfo.nWinCount, dwNow, TRUE);
		}
		else
		{
			pPage->DrawMultiScene(50, 0, &m_sprBack, 19, dwNow, TRUE);
			CurrentRewardTextDraw(pDC, 279, 66);
		}
		if (true == m_bDrawTip)
		{
			pPage->DrawMultiScene(50, 0, &m_sprBack, 17, dwNow, TRUE);
		}
	}
	else if(IDX_GAME_HA == GM().GetCurrentGameType())
	{
		if(IsEndEvent() == true)
		{
			pPage->DrawMultiScene(0, 0, &m_sprBack, 10, dwNow, TRUE);
		}
		else if (0 < m_PromotionInfo.nWinCount)
		{
			pPage->DrawMultiScene(0, 0, &m_sprBack, 9, dwNow, TRUE);
			pPage->DrawMultiScene(0, 0, &m_sprBack, 10 + m_PromotionInfo.nWinCount, dwNow, TRUE);
		}
		else
		{
			pPage->DrawMultiScene(0, 0, &m_sprBack, 19, dwNow, TRUE);
			CurrentRewardTextDraw(pDC, 229, 66);
		}
		if (true == m_bDrawTip)
		{
			pPage->DrawMultiScene(0, 0, &m_sprBack, 17, dwNow, TRUE);
		}
	}
	else
	{
		if (IsEndEvent() == true)
		{
			pPage->DrawMultiScene(0, 0, &m_sprBack, 3, dwNow, TRUE);
		}
		else if (0 < m_PromotionInfo.nWinCount)
		{
			pPage->DrawMultiScene(0, 0, &m_sprBack, 2, dwNow, TRUE);
			pPage->DrawMultiScene(0, 0, &m_sprBack, 3 + m_PromotionInfo.nWinCount, dwNow, TRUE);
		}
		else
		{
			pPage->DrawMultiScene(0, 0, &m_sprBack, 18, dwNow, TRUE);
			CurrentRewardTextDraw(pDC, 145, 482);
		}
		if (true == m_bDrawTip)
		{
			pPage->DrawMultiScene(0, 0, &m_sprBack, 16, dwNow, TRUE);
		}
	}
	if (true == m_cEffect_WheelPopup.Draw(pDC, pPage))
	{
		m_PromotionInfo.nWinCount=0;
	}
}

void CPromotionLuckyWheel::CurrentRewardTextDraw(CDC *pDC, int x, int y)
{
	CString strOutput;
	CFont *pOldFont = pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_16D_BOLD));
	COLORREF temp = pDC->GetTextColor();
	UINT oldFlags = pDC->SetTextAlign(TA_LEFT);
	pDC->SetTextColor( RGB(240, 163, 42) );
	strOutput.Format("%dÈ¸ / %dÈ¸ ÁøÇà", GM().GetPromotionData()->nCnts[2], GM().GetPromotionData()->nCnts[3]);
	pDC->TextOut(x, y, strOutput);
	pDC->SetTextColor( temp );
	pDC->SelectObject(pOldFont);
	pDC->SetTextAlign(oldFlags);
}

BOOL CPromotionLuckyWheel::OnLButtonDownMiddle(int &x , int &y)
{
	return FALSE;
}

BOOL CPromotionLuckyWheel::OnLButtonUpMiddle(int &x , int &y)
{
	return FALSE;
}

BOOL CPromotionLuckyWheel::OnMouseMoveMiddle(int &x , int &y)
{
	if((CPromotionManager::ENUM_PROMOTION_TYPE)GM().GetPromotionData()->nCnts[0] == CPromotionManager::EPT_POKER_2018_CHUSEOK)
	{		
		return FALSE;
	}

	if(IsEventRoom()==false)
		return FALSE;

	CRect crArea=CRect(8, 464, 8+288, 464+54);
	if(IDX_GAME_HA == GM().GetCurrentGameType()){
		crArea=CRect(104, 44, 104+288, 44+54);
	}
	if(IDX_GAME_SD == GM().GetCurrentGameType()){
		crArea=CRect(154, 44, 154+288, 44+54);
	}
	m_bDrawTip=false;
	if(TRUE==crArea.PtInRect(CPoint(x, y))){
		m_bDrawTip=true;
	}
	return FALSE;
}

void CPromotionLuckyWheel::OnTimer()
{
	if(0<m_nPlayTime )
	{
		m_nPlayTime--;
		if(m_nPlayTime<=0)
		{
			GM().GetPromotionData()->llEffectReserved = 0ll;
		}
	}
}


BOOL CPromotionLuckyWheel::OnPacketNotify(char* pName, int &Signal, int &TotSize, char *lpData)
{
	switch(Signal)
	{	
	case OPCODE_SV_VISITE_ALARM:
		{	
			packet::TSendPacket< promotion::_VISITE_PROMOTION_ALARM > mission_Packet;
			mission_Packet.Get( lpData, TotSize );
			promotion::_VISITE_PROMOTION_ALARM* pAlarm = static_cast< promotion::_VISITE_PROMOTION_ALARM* >( mission_Packet.Various() );
			SetEventInfo(pAlarm);
			// 2017.12 °ñµå°æ±âÀå ÆË¾÷À¸·Î º¯°æ
			GM().SetShowEndEventPopup(true);
		}
		break;
	}

	return FALSE;
}
