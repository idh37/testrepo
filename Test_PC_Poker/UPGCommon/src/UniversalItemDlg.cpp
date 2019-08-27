// UniversalItemDlg.cpp : implementation file
//


#include "stdafx.h"
#include "UniversalItemDlg.h"
#include "GlobalBase.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUniversalItemDlg dialog

CUniversalItemDlg::CUniversalItemDlg(CWnd* pParent /*=NULL*/)
	: CUPGDlgBase(CUniversalItemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUniversalItemDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	EnableAutomation();

	m_nDefaultItem = 0;

	m_dwStartTime = 0;
}


void CUniversalItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CUPGDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUniversalItemDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUniversalItemDlg, CUPGDlgBase)
	//{{AFX_MSG_MAP(CUniversalItemDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CUniversalItemDlg, CUPGDlgBase)
	DISP_FUNCTION( CUniversalItemDlg, "UpdateMoney", UpdateMoney, VT_EMPTY, VTS_BOOL )
	DISP_FUNCTION( CUniversalItemDlg, "ClosePopup", ClosePopup, VT_EMPTY, VTS_I2 )
END_DISPATCH_MAP()
/////////////////////////////////////////////////////////////////////////////
// CUniversalItemDlg message handlers

BOOL CUniversalItemDlg::OnInitDialog() 
{
	CUPGDlgBase::OnInitDialog();
	
	// TODO: Add extra initialization here
	if ( m_Back.LoadBitmapFile( ".\\common\\image\\GameItem\\UBack.bmp" ) == FALSE )
	{
		return TRUE;
	}

	CRect rect;

	rect.SetRect( 11, 31, 800+11, 600+31 );

	switch ( m_nItemType )
	{			
	case TYPE_CHOICEAVATAR:
		//		m_BmpSubTitle.LoadBitmapFile( ".\\common\\image\\GameItem\\Choice.bmp" );
		m_strSubtitle.Format(_T("> 윈조이 - 리얼한 승부, 도전! 초이스"));
		break;
	case TYPE_DICEAVATAR:			//주사위 아바타 (신규상품추가)
		//		m_BmpSubTitle.LoadBitmapFile( ".\\common\\image\\GameItem\\dice.bmp" );
		m_strSubtitle.Format(_T("> 도전!주사위"));
		break;
	case TYPE_CHALLENGE99AVATAR:	//도전99 아바타 (신규상품추가)		
		//		m_BmpSubTitle.LoadBitmapFile( ".\\common\\image\\GameItem\\challenge99.bmp" );		
		m_strSubtitle.Format(_T("> 도전!99"));
		break;
	case TYPE_CARIBBEANAVATAR:
		m_strSubtitle.Format(_T("> 도전 캐리비안!"));
		break;

	case TYPE_CARD_BINGO:
		m_strSubtitle.Format(_T("> 행운의 카드빙고"));
		break;

	//도전! 핀볼 : 2012.10.30 YS.Jeong	
	case TYPE_CARD_PINBAL:
	case TYPE_CARD_PINBAL2:
		m_strSubtitle.Format(_T("> 도전! 핀볼"));
		break;

	case TYPE_BIGWHEEL:
			m_strSubtitle.Format(_T("> 돌려라! BIG WHEEL"));
		break;

	case TYPE_SCISSORS:
		m_strSubtitle.Format(_T("> 이겨라! 가위바위보"));
		break;

	default:
		return TRUE;
	}

	NMBASE::SKINGDI::WindowResize2Bitmap( GetSafeHwnd(), ".\\common\\image\\GameItem\\UBack.bmp" );
	
	// 웹브라우저 생성 
	if ( m_Browser.Create( WS_VISIBLE | WS_CHILD, rect, GetSafeHwnd(), 2352, GetIDispatch(FALSE) ) == FALSE )
	{
		return FALSE;
	}
	m_Browser.SetHideBorder( TRUE );

	//AfxMessageBox(m_URL);
	m_Browser.Navigate( m_URL );

	m_Rgn.SetRgn( HBITMAP(m_Back), this, RGB( 255, 0, 255 ), &m_Back );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUniversalItemDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) return;

	cdc.SelectObject( &m_Back );

	//	m_BmpSubTitle.BitBlt( cdc, 19, 11 );

	if (!m_strSubtitle.IsEmpty())
	{

		CFont *pFont = &NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_12D_BOLD);
		CFont *pOldFont = cdc.SelectObject(pFont);
		cdc.SetBkMode(TRANSPARENT);


		cdc.SetTextColor(RGB(206, 211, 215));
		cdc.TextOut(12,9,m_strSubtitle);
		cdc.TextOut(14,11,m_strSubtitle);
		cdc.TextOut(12,11,m_strSubtitle);
		cdc.TextOut(14,9,m_strSubtitle);
		COLORREF oldcolor = cdc.SetTextColor(RGB(63, 63, 63));
		cdc.TextOut(13,10,m_strSubtitle);
		cdc.SelectObject(pOldFont);
		cdc.SetTextColor(oldcolor);
	}

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);
	// Do not call CUPGDlgBase::OnPaint() for painting messages
}

int CUniversalItemDlg::DoModal( int nItemType, CString& sURL) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_nItemType = nItemType;
	m_URL = sURL;
	
	
	return CUPGDlgBase::DoModal();
}

void CUniversalItemDlg::UpdateMoney( BOOL bClosePopup )
{
	TRACE("UpdateMoney run");
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

void CUniversalItemDlg::ClosePopup( int nOpenShopItemIndex )
{
	if ( nOpenShopItemIndex )
	{
		m_nDefaultItem = nOpenShopItemIndex;
	}

	CUPGDlgBase::OnCancel();
}

void CUniversalItemDlg::OnOK() 
{
	// TODO: Add extra validation here
	return;
}

void CUniversalItemDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	return;
}

BOOL CUniversalItemDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	return CUPGDlgBase::PreTranslateMessage(pMsg);
}
