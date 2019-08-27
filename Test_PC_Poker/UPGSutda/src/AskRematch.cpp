#include "StdAfx.h"
#include "AskRematch.h"
#include "GlobalGame.h"

#define MAX_ASKREMATCH_TIME 400

CAskRematch::CAskRematch(void)
{

	m_pPage = NULL;
	m_bAsk_Rematch = FALSE;
	m_nAsk_RematchCnt= 0;
	m_llPayment = 0;
}

CAskRematch::~CAskRematch(void)
{
}

void CAskRematch::LoadImage()
{
	//카드애니메이션(카드커지는거)
	AddLoadImageList(&m_YesBtnSpr, ".\\sutda\\data\\Pop\\YesBtn.spr", true);
	AddLoadImageList(&m_NoBtnSpr, ".\\sutda\\data\\Pop\\NoBtn.spr", true);
	AddLoadImageList(&m_RematchGameDlgSpr, ".\\sutda\\data\\Pop\\pop_1.spr", true);
	
}

void CAskRematch::Init(CWnd *pParent, NMBASE::GRAPHICGDI::CPage *pPage)
{

	m_pPage = pPage;
	
	//뉴재대결참여
	m_BtnAccept_Rematch.Init(pParent, m_pPage, 353 + 64, 238 + 80, &m_YesBtnSpr, 0, IDM_ACCEPT_REMATCH);
	m_BtnRefusal_Rematch.Init(pParent, m_pPage, 353 + 159, 238 + 80, &m_NoBtnSpr, 0, IDM_REFUSAL_REMATCH);


}
void CAskRematch::OnTimer()
{

	//뉴재대결참여
	if( m_bAsk_Rematch )
	{
		m_nAsk_RematchCnt--;
		
		if( m_nAsk_RematchCnt <= 0 )
		{
			DoAskRematch(false);
			
		}
	}

}

void CAskRematch::Show( INT64 llPayment)
{
	m_bAsk_Rematch = TRUE;
	m_nAsk_RematchCnt = MAX_ASKREMATCH_TIME;
	m_llPayment = llPayment;


	m_BtnAccept_Rematch.Show( TRUE );
	m_BtnRefusal_Rematch.Show( TRUE );

}


void CAskRematch::Hide()
{
	m_bAsk_Rematch = FALSE;
	m_nAsk_RematchCnt = 0;

	m_BtnAccept_Rematch.Show( FALSE );
	m_BtnRefusal_Rematch.Show( FALSE );

}



void CAskRematch::Draw(CDC *pDC)
{

	if( !m_bAsk_Rematch ) 
		return;

	int org_x = 353 + 14;
	int org_y = 238 + 12;

	m_pPage->PutSprAuto( org_x, org_y, &m_RematchGameDlgSpr, 0, HB );
	m_pPage->PutSprAuto( org_x, org_y, &m_RematchGameDlgSpr, 1 );
	m_pPage->PutSprAuto( org_x, org_y, &m_RematchGameDlgSpr, 2 );

	//참가비용
	CString str;
	str.Format("%s", NMBASE::UTIL::g_MakeShortMoneyStrEx( m_llPayment ) );
	str.Append(strChip_Name[g_RI.ChipKind]);

	CRect rc;
	rc.SetRect(0, 0, 125, 15); 
	rc.OffsetRect(org_x + 98, org_y + 43);
	pDC->DrawText(str, &rc, DT_CENTER);


	m_BtnAccept_Rematch.Draw(pDC);
	m_BtnRefusal_Rematch.Draw(pDC);

}

BOOL CAskRematch::OnLButtonDown(int x, int y)
{
	if( !m_bAsk_Rematch ) 
		return FALSE;
		

	if (m_BtnAccept_Rematch.OnLButtonDown(x,y))
		return TRUE;
	if (m_BtnRefusal_Rematch.OnLButtonDown(x,y))
		return TRUE;
	
	return FALSE;
}


BOOL CAskRematch::OnLButtonUp(int x, int y)
{
	if( !m_bAsk_Rematch ) 
		return FALSE;

	if (m_BtnAccept_Rematch.OnLButtonUp(x,y))
	{
		DoAskRematch(true);
		return TRUE;
	}
	if (m_BtnRefusal_Rematch.OnLButtonUp(x,y))
	{
		DoAskRematch(false);
		return TRUE;
	}

	return FALSE;

}

BOOL CAskRematch::OnMouseMove(int x, int y)
{
	if( !m_bAsk_Rematch ) 
		return FALSE;

	if (m_BtnAccept_Rematch.OnMouseMove(x,y))
		return TRUE;
	if (m_BtnRefusal_Rematch.OnMouseMove(x,y))
		return TRUE;

	return FALSE;
}


void CAskRematch::DoAskRematch(bool bAcceptToJoin)
{
	Hide();

	INT64 llJoinFee = 0;
	if( bAcceptToJoin )
	{
		
		llJoinFee = m_llPayment;
	}
	

	CCL_SD_ASK_49REMATCH msg;
	msg.Set( g_RI.RoomNum, GM().GetMyInfo()->UI.UNum, GM().GetMyInfo()->UI.ID, bAcceptToJoin, llJoinFee);
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());

}

BOOL CAskRematch::OnGameKey( int nVKey, int nextended, int repeat )
{
	if( !m_bAsk_Rematch ) 
		return FALSE;
	
	if( nVKey == rule::SKEY_ENTER )
	{
		DoAskRematch(true);
		return FALSE;
	}

	return FALSE;
}