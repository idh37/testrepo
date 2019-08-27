#include "stdafx.h"
#include "BuyTimeLimitAvataDlg.h"
#include "GlobalBase.h"
#include <afxinet.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//[행운아바타] 2006.07.24
CBuyTimeLimitAvatarDlg::CBuyTimeLimitAvatarDlg(CWnd* pParent /*=NULL*/)
	: CUPGDlgBase(CBuyTimeLimitAvatarDlg::IDD, pParent)
{
	m_nItemCode = 0;
	m_dwStartTime=0;

	EnableAutomation();
}


void CBuyTimeLimitAvatarDlg::DoDataExchange(CDataExchange* pDX)
{
	CUPGDlgBase::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDCANCEL, m_CloseBtn);
}


BEGIN_MESSAGE_MAP(CBuyTimeLimitAvatarDlg, CUPGDlgBase)
	//{{AFX_MSG_MAP(CBuyLuckAvataDlg)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



BEGIN_DISPATCH_MAP(CBuyTimeLimitAvatarDlg, CUPGDlgBase)
DISP_FUNCTION( CBuyTimeLimitAvatarDlg, "UpdateMoney", UpdateMoney, VT_EMPTY, VTS_BOOL )
DISP_FUNCTION( CBuyTimeLimitAvatarDlg, "ClosePopup", ClosePopup, VT_EMPTY, VTS_I2 )
END_DISPATCH_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBuyLuckAvataDlg message handlers

BOOL CBuyTimeLimitAvatarDlg::OnInitDialog() 
{
	CUPGDlgBase::OnInitDialog();
	
	// TODO: Add extra initialization here

	NMBASE::UTIL::CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);


	Back.LoadBitmapFile(".\\common\\image\\Popup\\back_timelimit.BMP");	
	m_CloseBtn.Init( 497, 6, ".\\common\\image\\commonbtn\\Btn_X.bmp", 4 ); 

	m_Rgn.SetRgn( HBITMAP(Back), this, RGB( 255, 0, 255 ), &Back );

	RECT rc1;
	GetWindowRect( &rc1 );
	rc1.right = rc1.left + Back.GetWidth();
	rc1.bottom = rc1.top + Back.GetHeight();
	MoveWindow( &rc1 );

	CRect rc;
	rc.SetRect( 12, 32, 12+505, 32+638 );
	
	m_pWebGameItem.Create( WS_VISIBLE | WS_CHILD, rc, GetSafeHwnd(), 3434, GetIDispatch(FALSE) );	
	m_pWebGameItem.Navigate( "about:blank" );
	m_pWebGameItem.SetHideBorder( TRUE );
	m_pWebGameItem.SetCallback_OnBeforeNavigate2( SSO_OnBeforeNavigate2 );	
	m_pWebGameItem.SetCallback_OnQuit(WebClose);

	//아래 한줄 추가
	m_ChanceNavigateURL.Format( getGameURL(IDX_GAMEURL_TIMELIMITAVATAR) );	//%%% GameString

	m_pWebGameItem.Navigate( m_ChanceNavigateURL );
	m_pWebGameItem.ResetUserNavigatedCount();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CBuyTimeLimitAvatarDlg::OnCancel() 
{
	// TODO: Add extra cleanup here

	if( m_pWebGameItem.GetHWND() && m_pWebGameItem.GetUserNavigatedCount() > 0 )
	{
		if( m_nItemCode >= 0 ){
			// 머니 업데이트 과정. // 실제 아이템을 샀는지 아닌지 체크하고 샀다면 머니를 업데이트 한다.
			CSV_ASK_CHECK_REWARD msg;
			
			msg.Set( m_nItemCode, m_RecvTime );		
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
		}	
	}	
	
	if (GM().GetMainWnd()->GetSafeHwnd())
	{
		GM().GetMainWnd()->SetFocus();
	}
	/*

	//%%% 추가 작업
	if (g_pLobyDlg->IsShowItemZone())
	{
 		g_pLobyDlg->ShowWebItemZone(TRUE);
 		g_pLobyDlg->LobyItemNavigate(TRUE);
	}
	*/
	
	CUPGDlgBase::OnCancel();
}

void CBuyTimeLimitAvatarDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) return;
	CBitmap *pOldBmp = cdc.SelectObject(&Back);

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);

	cdc.SelectObject(pOldBmp);
	cdc.DeleteDC();		
}

int CBuyTimeLimitAvatarDlg::DoModal() 
{
	// TODO: Add your specialized code here and/or call the base class	
	return CUPGDlgBase::DoModal();
}



BOOL CBuyTimeLimitAvatarDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_pWebGameItem.Destroy();
	return CUPGDlgBase::DestroyWindow();
}

void CBuyTimeLimitAvatarDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, 0 );	
	CUPGDlgBase::OnLButtonDown(nFlags, point);
}

BOOL CBuyTimeLimitAvatarDlg::SafeClose()
{
	if(!GetSafeHwnd()) return FALSE;
	SendMessage(WM_COMMAND, IDCANCEL);
	return TRUE;
}

void CBuyTimeLimitAvatarDlg::UpdateMoney( BOOL bClosePopup )
{
	if ( m_dwStartTime == 0 ) // 처음 호출
	{
		CSV_ASK_GAME_CONTINUE msg;
		msg.Set(GM().GetMyInfo()->UI.ID, 0);		
		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());				
		
		if ( bClosePopup )
		{
			this->SendMessage( WM_CLOSE, 0, 0 );
		}
		
		m_dwStartTime = timeGetTime();
	}
	else
	{
		DWORD dwTime = timeGetTime() - m_dwStartTime;
		if ( dwTime >= 10000 ) // 10초가 지나야
		{
			CSV_ASK_GAME_CONTINUE msg;
			msg.Set(GM().GetMyInfo()->UI.ID, 0);
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());				
			
			if ( bClosePopup )
			{
				this->SendMessage( WM_CLOSE, 0, 0 );
			}
			
			m_dwStartTime = timeGetTime();
		}
	}
}

void CBuyTimeLimitAvatarDlg::ClosePopup( short i )
{
	OnCancel();
}

void CBuyTimeLimitAvatarDlg::WebClose()
{
	GM().PostMainWndMessage(UM_CLOSEALLPOPUP);
}