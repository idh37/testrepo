// GameViewButtonMan.cpp: implementation of the CGameButtonHL class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameButtonHL.h"
#include "UIInfoDataHighLow.h"
#include "SpriteList.h"

CGameButtonHL::CGameButtonHL()
{

	
}

CGameButtonHL::~CGameButtonHL()
{

}

void CGameButtonHL::Init( CGameViewBase *pView, CUIInfoData *pUIInfoData, CChattingWindowMan *pChatWndMan )
{	
	CGameViewButtonMan::Init( pView,  pUIInfoData, pChatWndMan );

	CUIInfoDataHighLow *pHighLowUIData = dynamic_cast<CUIInfoDataHighLow*>(m_pUIInfoData);

	ASSERT(pHighLowUIData);
	if (pHighLowUIData)
	{		
		CPoint pt;
		pt = pHighLowUIData->GetHighLowBtnPos(EBT_HILO_HIGH);
		m_btnHigh.Init(m_pParent, m_pPage, pt.x, pt.y, &g_sprHighLowSwingBtn, 0, IDM_HIGH);
		m_ctrlHilowBtnList.Add(&m_btnHigh);

		pt = pHighLowUIData->GetHighLowBtnPos(EBT_HILO_LOW);
		m_btnLow.Init(m_pParent, m_pPage, pt.x, pt.y, &g_sprHighLowSwingBtn, 1, IDM_LOW);
		m_ctrlHilowBtnList.Add(&m_btnLow);

		pt = pHighLowUIData->GetHighLowBtnPos(EBT_HILO_SWING);
		m_btnSwing.Init(m_pParent, m_pPage, pt.x, pt.y, &g_sprHighLowSwingBtn, 2, IDM_SWING);	
		m_ctrlHilowBtnList.Add(&m_btnSwing);
	}


// 	// 바둑이 폰트 컬러
// 	for ( int i = 0; i < NT_ALL; i++ )
// 	{
// 		if ( m_pNTMoney[ i ] )
// 		{
// 			m_pNTMoney[ i ]->SetColor( static_cast< ui::CUIInfoData_BD* >( pUIInfoData )->GetRGBBD( ui::ERGB_BETBTNTEXT ) );
// 			m_pNTMoney[ i ]->SetOutLine( static_cast< ui::CUIInfoData_BD* >( pUIInfoData )->GetRGBBD( ui::ERGB_BETBTNTEXT_SHADOW ) );
// 		}
// 	}
// 	

}


void CGameButtonHL::ClearFlag()
{
	if ( GetCurrentWhere() != IDX_GAMEWHERE_GAME )
	{
		return;
	}

	CGameViewButtonMan::ClearFlag();
	
	SetHilowSelBtnShowAll(false);

}




void CGameButtonHL::SetHilowSelBtnShowAll( bool bShow ) // 모든 하이/로우/스윙 선택 버튼 보이기
{
	m_btnHigh.Show(bShow);
	m_btnLow.Show(bShow);
	m_btnSwing.Show(bShow);
	
	
	
}

// 사용되어질 키 셋팅
void CGameButtonHL::InitializeKeyValue( CONFIG_BETBTN_TYPE eBtnDirectionType )
{
	CGameViewButtonMan::InitializeKeyValue( eBtnDirectionType );
	
// 	//카드선택  Cutting
// 	if ( g_Config.bCutStyle )
// 	{
// 		// F1 ~ F4
// 		G_KEYDEF[ KEY_CUT_1 ][ 0 ] = SET_VKEY_NOR( rule::SKEY_F1 );
// 		G_KEYDEF[ KEY_CUT_2 ][ 0 ] = SET_VKEY_NOR( rule::SKEY_F2 );
// 		G_KEYDEF[ KEY_CUT_3 ][ 0 ] = SET_VKEY_NOR( rule::SKEY_F3 );
// 		G_KEYDEF[ KEY_CUT_4 ][ 0 ] = SET_VKEY_NOR( rule::SKEY_F4 );
// 	}
// 	else
// 	{
// 		// 1 ~ 4
// 		G_KEYDEF[ KEY_CUT_1 ][ 1 ] = SET_VKEY_NOR( rule::SKEY_1 );
// 		G_KEYDEF[ KEY_CUT_2 ][ 1 ] = SET_VKEY_NOR( rule::SKEY_2 );
// 		G_KEYDEF[ KEY_CUT_3 ][ 1 ] = SET_VKEY_NOR( rule::SKEY_3 );
// 		G_KEYDEF[ KEY_CUT_4 ][ 1 ] = SET_VKEY_NOR( rule::SKEY_4 );
// 
// 	
// 		G_KEYDEF[ KEY_CUT_1 ][ 2 ] = SET_VKEY_EXT( rule::SKEY_GRAY_DEL );
// 		G_KEYDEF[ KEY_CUT_1 ][ 3 ] = SET_VKEY_NOR( rule::SKEY_GRAY_END );   //VK_NUMPAD1
// 
// 		G_KEYDEF[ KEY_CUT_2 ][ 2 ] = SET_VKEY_EXT( rule::SKEY_GRAY_END );
// 		G_KEYDEF[ KEY_CUT_2 ][ 3 ] = SET_VKEY_NOR( rule::SKEY_GRAY_DOWN );  //VK_NUMPAD2
// 
// 		G_KEYDEF[ KEY_CUT_3 ][ 2 ] = SET_VKEY_EXT( rule::SKEY_GRAY_PGDN );
// 		G_KEYDEF[ KEY_CUT_3 ][ 3 ] = SET_VKEY_NOR( rule::SKEY_GRAY_PGDN );  //VK_NUMPAD3 
// 
// 		G_KEYDEF[ KEY_CUT_4 ][ 2 ] = SET_VKEY_EXT( rule::SKEY_GRAY_INS );
// 		G_KEYDEF[ KEY_CUT_4 ][ 3 ] = SET_VKEY_NOR( rule::SKEY_GRAY_LEFT );  //VK_NUMPAD4
// 
// 	}
// 
// 	// 바꾸기, 패스 단축키
// 	G_KEYDEF[ KEY_CUT_CHANGE ][ 0 ] = SET_VKEY_EXT( rule::SKEY_GRAY_HOME ); 
// 	G_KEYDEF[ KEY_CUT_CHANGE ][ 1 ] = SET_VKEY_NOR( rule::SKEY_GRAY_CENTER );//VK_NUMPAD5
// 	G_KEYDEF[ KEY_CUT_CHANGE ][ 2 ] = SET_VKEY_NOR( rule::SKEY_5 );          //5
// 
// 	G_KEYDEF[ KEY_CUT_PASS ][ 0 ] = SET_VKEY_EXT( rule::SKEY_GRAY_PGUP );
// 	G_KEYDEF[ KEY_CUT_PASS ][ 1 ] = SET_VKEY_NOR( rule::SKEY_GRAY_RIGHT ); //VK_NUMPAD6;
// 	G_KEYDEF[ KEY_CUT_PASS ][ 2 ] = SET_VKEY_NOR( rule::SKEY_6 ); //6;



	//하이로우 전용
	//하이/로우/스윙 선택
	G_KEYDEF[ KEY_HILOWSEL_HIGH ][ 0 ] = SET_VKEY_EXT( rule::SKEY_GRAY_DEL );
	G_KEYDEF[ KEY_HILOWSEL_HIGH ][ 1 ] = SET_VKEY_NOR( rule::SKEY_GRAY_END );
	G_KEYDEF[ KEY_HILOWSEL_HIGH ][ 2 ] = SET_VKEY_NOR( rule::SKEY_1 );

	G_KEYDEF[ KEY_HILOWSEL_LOW ][ 0 ] = SET_VKEY_EXT( rule::SKEY_GRAY_END );
	G_KEYDEF[ KEY_HILOWSEL_LOW ][ 1 ] = SET_VKEY_NOR( rule::SKEY_GRAY_DOWN );  
	G_KEYDEF[ KEY_HILOWSEL_LOW ][ 2 ] = SET_VKEY_NOR( rule::SKEY_2 );

	G_KEYDEF[ KEY_HILOWSEL_SWING ][ 0 ] = SET_VKEY_EXT( rule::SKEY_GRAY_PGDN );
	G_KEYDEF[ KEY_HILOWSEL_SWING ][ 1 ] = SET_VKEY_NOR( rule::SKEY_GRAY_PGDN );
	G_KEYDEF[ KEY_HILOWSEL_SWING ][ 2 ] = SET_VKEY_NOR( rule::SKEY_3 );	

}






void CGameButtonHL::ShowWincaseBtn_HL(bool bShow, bool bEnableOnlySwingBtn)
{
	m_btnHigh.Show(bShow);
	m_btnLow.Show(bShow);
	m_btnSwing.Show(bShow);

	m_btnHigh.Enable(bShow);
	m_btnLow.Enable(bShow);
	m_btnSwing.Enable(bShow);
	
	
	//스윙
	if( bEnableOnlySwingBtn  )
	{
		m_btnHigh.Enable(FALSE);
		m_btnLow.Enable(FALSE);
		m_btnSwing.Enable(TRUE);
	}

	
}


void CGameButtonHL::EnableWincaseBtn_HL(bool bEnable)
{

	m_btnHigh.Enable(bEnable);
	m_btnLow.Enable(bEnable);
	m_btnSwing.Enable(bEnable);
}



void CGameButtonHL::DrawBtn(CDC *pDC)
{
	CGameViewButtonMan::DrawBtn(pDC);

	m_ctrlHilowBtnList.Draw(pDC);
	

}
BOOL CGameButtonHL::OnLButtonDown(int x , int y)
{
	if (CGameViewButtonMan::OnLButtonDown(x, y))
		return TRUE;

	return m_ctrlHilowBtnList.OnLButtonDown(x,y);



}
BOOL CGameButtonHL::OnLButtonUp(int x , int y)
{
	if (CGameViewButtonMan::OnLButtonUp(x, y))
		return TRUE;

	return m_ctrlHilowBtnList.OnLButtonUp(x,y);
}
BOOL CGameButtonHL::OnMouseMove(int x , int y)
{
	if (CGameViewButtonMan::OnMouseMove(x, y))
		return TRUE;

	return m_ctrlHilowBtnList.OnMouseMove(x,y);
}