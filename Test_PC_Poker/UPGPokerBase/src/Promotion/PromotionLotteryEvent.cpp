#include "StdAfx.h"
#include "PromotionLotteryEvent.h"
#include "PromotionManager.h"

#define _TESTCODE_ 1
#define ID_BUTTON_CLICK (1111)

CPromotionLotteryEvent::CPromotionLotteryEvent( CPromotionManager *pManager, int nType )
: CPromotionBase(pManager, nType)
{

	memset( &m_PcRoomMact_Info, 0, sizeof( promotion::BETTING_ALRAM  ) );

	string strDir = ".\\PokerCommon\\data\\Event\\Lottery\\";
	_LOADSPRITEDEFINE(&m_sprBack, strDir, "EventLottery.spr");

	m_bDomantUser = FALSE;
	m_nIndex_Coupon = 0;
	m_nIndex_CouponTab = 0;

	m_bShowEft = FALSE;
	m_dwShowEftTimer = 0;

	m_bGetCoupon = FALSE;
	m_dwGetCouponTimer = 0;





	
}

CPromotionLotteryEvent::~CPromotionLotteryEvent(void)
{

}




void CPromotionLotteryEvent::Clear_Info()
{

}

void CPromotionLotteryEvent::ShowComplete( int nGetCoupon )
{
	m_bGetCoupon = TRUE;
	m_dwGetCouponTimer = timeGetTime();

	m_nIndex_Coupon=3;
}


void CPromotionLotteryEvent::SetEventInfo( promotion::CHUSEOK_ALRAM *pInfo )
{
	if (pInfo==NULL) 
		return;

	
	//ÀÀ¸ð±ÇÈ¹µæ
	if( pInfo->bComplete || pInfo->nCurApplyCnt > 0 )
	{
		
		ShowComplete( pInfo->nCurApplyCnt );

		//ÀÀ¸ð±Ç È¹µæ ½Ã ÀÌº¥Æ®ÅÇ ±ôºýÀÓ
		if (m_pManager)
		{
			m_pManager->SetEvent();
		}
		
	}

	memcpy( &m_PcRoomMact_Info, pInfo, sizeof( promotion::BETTING_ALRAM ) );
}

void CPromotionLotteryEvent::Accept_OutRoom()
{
	
}

void CPromotionLotteryEvent::Accept_CreateRoom()
{
	

}

void CPromotionLotteryEvent::Accept_EnterRoom()
{
	

}



void CPromotionLotteryEvent::DrawBottom(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage)
{
	if( !IsActive() ) return;

	//pPage->PutSprAuto( 11, 492, &m_sprBack, 3 );


}

void CPromotionLotteryEvent::DrawMiddle(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage)
{

}

//ÀÀ¸ð±ÇÈ¹µæ½Ã ±×·ÁÁØ´Ù.
void CPromotionLotteryEvent::DrawTop(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage)
{
	if( !IsActive() ) return;


	if( m_bGetCoupon )
	{
		DWORD dwAniTime = timeGetTime() - m_dwGetCouponTimer;
		pPage->DrawMultiScene( 0, 0, &m_sprBack, m_nIndex_Coupon, dwAniTime, FALSE );

		if( m_sprBack.IsMSceneTimeOver( m_nIndex_Coupon, dwAniTime) )
		{
			m_bGetCoupon = FALSE;
			m_dwGetCouponTimer = 0;
			m_nIndex_Coupon = 0;
		}
	}

	
}

void CPromotionLotteryEvent::DrawEventTab(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage, const CPoint &ptStart)
{

	//ÆùÆ®/ »ö±ò
	CFont *pOldFont = pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_15_BOLD));
	COLORREF clrOld = pDC->SetTextColor(RGB(255, 255, 255 ));




	const int COORD_X_EVENTTAB = 12;
	const int COORD_Y_EVENTTAB = 537;
	const int MAX_PLAYROUND = 20; //20ÆÇ ¸¶´Ù ÀÀ¸ð±Ç 1Àå


	//ÇÏ·ç ÃÑÀÀ¸ð±Ç È¹µæ·®¸¸Å­ È¹µæÇßÀ»°æ¿ì 
	//if( m_PcRoomMact_Info.nTodayApplyCnt == 100 && m_bGetCoupon == FALSE )
	if (m_bGetCoupon)
	{
		//ÀÀ¸ð±Ç ÀÌÆåÆ® ³ëÃâÁß
// 		if (m_PcRoomMact_Info.nCurApplyCnt == 10)
// 			pPage->PutSprAuto( COORD_X_EVENTTAB, COORD_Y_EVENTTAB, &m_sprBack, 6 );	///"10Àå È¹µæ!" Ç¥½Ã
// 		else
// 			pPage->PutSprAuto( COORD_X_EVENTTAB, COORD_Y_EVENTTAB, &m_sprBack, 5 ); ///"1Àå È¹µæ!" Ç¥½Ã
		pPage->DrawMultiScene( 0, 0, &m_sprBack, 1, 0, FALSE );
	}
	else if( m_PcRoomMact_Info.bApplyLimit )
	{
		//ÀÀ¸ð±Ç ´Ù ¸ð¾ÒÀ½
		pPage->DrawMultiScene( 0, 0, &m_sprBack, 2, 0, FALSE );
	}
	else
	{
		//±âº» Back È­¸é Âï±â
//		pPage->PutSprAuto( COORD_X_EVENTTAB, COORD_Y_EVENTTAB, &m_sprBack, 4 );
		pPage->DrawMultiScene( 0, 0, &m_sprBack, 0, 0, FALSE );

		//³²Àº ÆÇ¼ö ±×¸®±â
		CString str;
		str.Format("%d", MAX_PLAYROUND - m_PcRoomMact_Info.nPlayRoundCnt);
		CRect rtCnt(COORD_X_EVENTTAB + 151, COORD_Y_EVENTTAB + 109 , COORD_X_EVENTTAB + 151 + 26, COORD_Y_EVENTTAB+109 +16);
		pDC->DrawText(str, &rtCnt,  DT_VCENTER|DT_SINGLELINE|DT_CENTER );

	}

//m_PcRoomMact_Info.nTotalApplyCnt=5897;

	//ÇöÀç ÀÀ¸ð±Ç °¹¼ö ±×¸®±â
	CString stPointInfo;
	stPointInfo.Format("%d", m_PcRoomMact_Info.nTotalApplyCnt );
	CRect rt(COORD_X_EVENTTAB + 157, COORD_Y_EVENTTAB+58 , COORD_X_EVENTTAB + 157 + 67, COORD_Y_EVENTTAB+58 +16);
	pDC->DrawText(stPointInfo, &rt,  DT_VCENTER|DT_SINGLELINE|DT_CENTER );


	pDC->SetTextColor(clrOld);
	pDC->SelectObject(pOldFont);

	
}

BOOL CPromotionLotteryEvent::OnLButtonDownMiddle(int &x , int &y)
{
// 	m_BtnClose.OnLButtonDown( x, y ); 
// 	m_BtnOpen.OnLButtonDown( x, y );
	
	return FALSE;
}

BOOL CPromotionLotteryEvent::OnLButtonUpMiddle(int &x , int &y)
{
// 	m_BtnClose.OnLButtonUp( x, y );
// 	m_BtnOpen.OnLButtonUp( x, y );
	
	return FALSE;
}

BOOL CPromotionLotteryEvent::OnMouseMoveMiddle(int &x , int &y)
{
// 	m_BtnClose.OnMouseMove( x, y );
// 	m_BtnOpen.OnMouseMove( x, y );
	
	return FALSE;
}



BOOL CPromotionLotteryEvent::OnPacketNotify(char* pName, int &Signal, int &TotSize, char *lpData)
{
	switch(Signal)
	{
	
	// ½ÃÀÛ Á¤º¸
	case OPCODE_SV_CHUSEOK_START:
		{
			if( GM().GetCurrentGame()->GetGameType() == IDX_GAME_DP ) break;

			packet::TSendPacket< promotion::CHUSEOK_ALRAM > October_Packet;
			October_Packet.Get( lpData, TotSize );
			promotion::CHUSEOK_ALRAM* pAlaram = static_cast< promotion::CHUSEOK_ALRAM* >( October_Packet.Various() );

			SetActive(true);

			SetEventInfo( pAlaram );

//#ifdef _TESTCODE_
// 			char szMsg[ MAX_PATH ] ={ 0,};
// 
// 			sprintf( szMsg, "PC¹æ ÀÌº¥Æ®±â°£ ½ÃÀÛ - È¹µæ[%d], ÈÞ¸ÕÀ¯Àú[%d], ÇÃ·¹ÀÌÄ«¿îÆ®[%d], È¹µæÇÑÀÀ¸ð±Ç[%d]\n", 
// 			pAlaram->bComplete, pAlaram->bDomantUser, pAlaram->nPlayRoundCnt, pAlaram->nCurApplyCnt);
// 
// 			AddLobbyChatViewEdit( szMsg, RGB( 1, 1, 250 ) );
// 			AddGameChatViewEdit( szMsg , RGB(227, 227, 0) );
// 
// 			sprintf( szMsg, "¿À´ÃÃÑÀÀ¸ð±Ç[%d], ÃÑÀÀ¸ð±Ç[%d], È¹µæÁ¦ÇÑ[%d]\n", 
// 			pAlaram->nTodayApplyCnt, pAlaram->nTotalApplyCnt, pAlaram->bApplyLimit);
// 
// 			AddLobbyChatViewEdit( szMsg, RGB( 1, 1, 250 ) );
// 			AddGameChatViewEdit( szMsg , RGB(227, 227, 0) );
//#endif

		}break;

	//12½Ã ÃÊ±âÈ­
	case OPCODE_SV_CHUSEOK_INITIALIZE:
		{
			if( GM().GetCurrentGame()->GetGameType() == IDX_GAME_DP ) break;

			packet::TSendPacket< promotion::CHUSEOK_INITIALIZE > Bet_Packet;
			Bet_Packet.Get( lpData, TotSize );
			promotion::CHUSEOK_INITIALIZE* pAlaram = static_cast< promotion::CHUSEOK_INITIALIZE* >( Bet_Packet.Various() );

			m_PcRoomMact_Info.bApplyLimit = false;
			m_PcRoomMact_Info.nPlayRoundCnt = 0;
			m_PcRoomMact_Info.nTodayApplyCnt = 0;
			

// 			SetEventInfo( pAlaram );
// 			
// 			Clear_Info();

//#ifdef _TESTCODE_
// 			char szMsg[ MAX_PATH ] ={ 0,};
// 
// 			sprintf( szMsg, "PC¹æ ÀÌº¥Æ®±â°£ ½ÃÀÛ - È¹µæ[%d], ÈÞ¸ÕÀ¯Àú[%d], ÇÃ·¹ÀÌÄ«¿îÆ®[%d], È¹µæÇÑÀÀ¸ð±Ç[%d]\n", 
// 				m_PcRoomMact_Info.bComplete, m_PcRoomMact_Info.bDomantUser, m_PcRoomMact_Info.nPlayRoundCnt, m_PcRoomMact_Info.nCurApplyCnt);
// 
// 			AddLobbyChatViewEdit( szMsg, RGB( 1, 1, 250 ) );
// 			AddGameChatViewEdit( szMsg , RGB(227, 227, 0) );
// 
// 			sprintf( szMsg, "¿À´ÃÃÑÀÀ¸ð±Ç[%d], ÃÑÀÀ¸ð±Ç[%d], È¹µæÁ¦ÇÑ[%d]\n", 
// 				m_PcRoomMact_Info.nTodayApplyCnt, m_PcRoomMact_Info.nTotalApplyCnt, m_PcRoomMact_Info.bApplyLimit);
// 
// 			AddLobbyChatViewEdit( szMsg, RGB( 1, 1, 250 ) );
// 			AddGameChatViewEdit( szMsg , RGB(227, 227, 0) );
//#endif

		}break;
	case OPCODE_SV_CHUSEOK_ALRAM:
		{
			if( GM().GetCurrentGame()->GetGameType() == IDX_GAME_DP ) break;

			packet::TSendPacket< promotion::CHUSEOK_ALRAM > Bet_Packet;
			Bet_Packet.Get( lpData, TotSize );
			promotion::CHUSEOK_ALRAM* pAlaram = static_cast< promotion::CHUSEOK_ALRAM* >( Bet_Packet.Various() );

			SetActive( true );
			SetEventInfo( pAlaram );

//#ifdef _TESTCODE_
// 			char szMsg[ MAX_PATH ] ={ 0,};
// 
// 			sprintf( szMsg, "PC¹æ ÀÌº¥Æ®±â°£ ½ÃÀÛ - È¹µæ[%d], ÈÞ¸ÕÀ¯Àú[%d], ÇÃ·¹ÀÌÄ«¿îÆ®[%d], È¹µæÇÑÀÀ¸ð±Ç[%d]\n", 
// 				pAlaram->bComplete, pAlaram->bDomantUser, pAlaram->nPlayRoundCnt, pAlaram->nCurApplyCnt);
// 
// 			AddLobbyChatViewEdit( szMsg, RGB( 1, 1, 250 ) );
// 			AddGameChatViewEdit( szMsg , RGB(227, 227, 0) );
// 
// 			sprintf( szMsg, "¿À´ÃÃÑÀÀ¸ð±Ç[%d], ÃÑÀÀ¸ð±Ç[%d], È¹µæÁ¦ÇÑ[%d]\n", 
// 				pAlaram->nTodayApplyCnt, pAlaram->nTotalApplyCnt, pAlaram->bApplyLimit);
// 
// 			AddLobbyChatViewEdit( szMsg, RGB( 1, 1, 250 ) );
// 			AddGameChatViewEdit( szMsg , RGB(227, 227, 0) );
//#endif

		}break;
	case OPCODE_SV_CHUSEOK_END:
		{

			SetActive(false);

			//////////////////////////////////////////////////////////////////////////
#ifdef _TESTCODE_
// 			char szMsg[ MAX_PATH ] ={ 0,};
// 
// 			sprintf( szMsg, "PC¹æ ÀÌº¥Æ® ³¡ - \n");	
// 
// 			AddLobbyChatViewEdit( szMsg, RGB( 1, 1, 250 ) );
// 			AddGameChatViewEdit( szMsg , RGB(227, 227, 0) );
#endif
			//////////////////////////////////////////////////////////////////////////

		}break;
	}

	return FALSE;
}

void CPromotionLotteryEvent::SetActive(const bool &bActive)
{
	CPromotionBase::SetActive(bActive);

	//MM().Call(UPG_CMK_NOTIFY_ACTIVE_PCPROMOTION, (WPARAM)bActive);
}


LRESULT CPromotionLotteryEvent::OnIsActive(WPARAM &wParam, LPARAM &lParam)
{
	return IsActive();
}




bool CPromotionLotteryEvent::IsEndPopup(void)
{
	return true;

}
bool CPromotionLotteryEvent::InitEndPopup(CEndPopup *pEndPopup, CPoint ptPos)
{

	m_bitEndBanner.LoadBitmapFile("PokerCommon\\image\\Event\\PcRoom\\Endpopup.BMP");


	pEndPopup->m_btnEvent1.ShowWindow(SW_SHOW);
	pEndPopup->m_btnEvent1.EnableWindow();
	pEndPopup->m_btnEvent1.Init(171, 459, ".\\PokerCommon\\image\\Event\\PcRoom\\EndpopupBtn.BMP", 4);


	return true;
}
bool CPromotionLotteryEvent::OnPaintEndPopup(CPaintDC *pDC)
{

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	if( memDC.m_hDC == NULL ) 
		return false;

	memDC.SelectObject(&m_bitEndBanner);
	pDC->BitBlt(13, 400, m_bitEndBanner.GetWidth(), m_bitEndBanner.GetHeight(), &memDC, 0, 0, SRCCOPY);

	memDC.DeleteDC();
	return true;

}
bool CPromotionLotteryEvent::OnClickEndPopup(UINT nID)
{
	if (nID==IDC_BTN_EVENT1)
	{
		NMBASE::UTIL::CShellMan sm;
		sm.OpenNewBrowser("http://game1.netmarble.net/event/2011/_06/20110621_PokerBetting/" );
		return true;
	}

	return false;
	
}
void CPromotionLotteryEvent::OnDestroyEndPopup(void)
{
	if (m_bitEndBanner.m_hObject)
		m_bitEndBanner.DeleteObject();




}

