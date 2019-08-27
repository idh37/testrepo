// JackpotRankPopupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "JackpotRankPopupDlg.h"
#include "GlobalBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJackpotRankPopupDlg dialog


CJackpotRankPopupDlg::CJackpotRankPopupDlg(CWnd* pParent /*=NULL*/)
	: CUPGDlgBase(CJackpotRankPopupDlg::IDD, pParent)
{
	m_pBrowser = NULL;
	//{{AFX_DATA_INIT(CJackpotRankPopupDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT		
}


void CJackpotRankPopupDlg::DoDataExchange(CDataExchange* pDX)
{
	CUPGDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJackpotRankPopupDlg)
	DDX_Control(pDX, IDOK, m_OKBtn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CJackpotRankPopupDlg, CUPGDlgBase)
	//{{AFX_MSG_MAP(CJackpotRankPopupDlg)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJackpotRankPopupDlg message handlers

BOOL CJackpotRankPopupDlg::OnInitDialog() 
{
	CUPGDlgBase::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_OKBtn.Init( 361, 369, ".\\common\\Image\\Jackpot\\Btn_close.bmp", 4 );
	m_Back.LoadBitmapFile( ".\\common\\Image\\Jackpot\\back.bmp");
	
	m_Back.ResizeWnd( this, 0, 0, TRUE, m_pParentWnd );
	m_Rgn.SetRgn( m_Back, this, RGB(255, 0, 255), &m_Back );	
	CreateBrowser();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CJackpotRankPopupDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	m_Back.BitBlt( dc );
}

void CJackpotRankPopupDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CUPGDlgBase::OnOK();
}


void CJackpotRankPopupDlg::OnDestroy() 
{
	CUPGDlgBase::OnDestroy();
	
	// TODO: Add your message handler code here
	
	m_Back.DeleteObject();
	DestroyBrowser();
	
}


void CJackpotRankPopupDlg::DestroyBrowser()
{
	if ( m_pBrowser )
	{
		m_pBrowser->Destroy();
		delete m_pBrowser;
		m_pBrowser = NULL;
	}
}

BOOL CJackpotRankPopupDlg::CreateBrowser()
{
	DestroyBrowser();
	
	// CreateWebPage
	RECT rt = { 5, 5, 450+5, 350+5};
	m_pBrowser = new NMBASE::UTIL::CNMSSOWebBrowser();
	if ( !m_pBrowser->Create( WS_VISIBLE | WS_CHILD, rt, GetSafeHwnd(), 52512 ) )
	{
		if ( m_pBrowser )
		{
			delete m_pBrowser;
			m_pBrowser = NULL;
		}
		return FALSE;
	}

	m_pBrowser->Navigate( "about:blank" );
	m_pBrowser->SetHideBorder( TRUE );
	m_pBrowser->SetCallback_OnBeforeNavigate2(SSO_OnBeforeNavigate2);	
	
	m_pBrowser->Navigate(getGameURL(IDX_GAMEURL_JACKPOT_RANK));
	
	return TRUE;
}

