// OwnLimitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OwnLimitDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COwnLimitDlg dialog


COwnLimitDlg::COwnLimitDlg(CWnd* pParent /*=NULL*/)
	: CUPGDlgBase(COwnLimitDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COwnLimitDlg)
	//}}AFX_DATA_INIT

	m_pBrowser=NULL;
	m_HandoPopup=HANDOPOPUP_NULL;
	EnableAutomation();
	MM().AddMsgH(UPG_CMK_SHOW_GOLD_OWNLIMIT_POPUP, Fnt(this, &COwnLimitDlg::OnDoModal));
}


void COwnLimitDlg::DoDataExchange(CDataExchange* pDX)
{
	CUPGDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COwnLimitDlg)
	DDX_Control(pDX, IDCANCEL, m_BtnCancel);
	DDX_Control(pDX, IDOK, m_BtnOk);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COwnLimitDlg, CUPGDlgBase)
	//{{AFX_MSG_MAP(COwnLimitDlg)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// 초이스 아바타
BEGIN_DISPATCH_MAP(COwnLimitDlg, CUPGDlgBase)
DISP_FUNCTION( COwnLimitDlg, "ClosePopup", ClosePopup, VT_EMPTY, VTS_I2 )
END_DISPATCH_MAP()

/////////////////////////////////////////////////////////////////////////////
// COwnLimitDlg message handlers

BOOL COwnLimitDlg::OnInitDialog() 
{
	CUPGDlgBase::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	NMBASE::UTIL::CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);

	m_Back.LoadBitmapFile(".\\common\\image\\Popup\\Hando_back.BMP");

	m_BtnOk.Init( 24, 517, ".\\common\\Image\\Popup\\btn_today.BMP", 4);
	m_BtnCancel.Init( 385, 510, ".\\common\\Image\\Popup\\btn_close.bmp", 4 );

	NMBASE::SKINGDI::WindowResize2Bitmap(m_hWnd, ".\\common\\image\\Popup\\Hando_back.BMP");

	CreateBrowser();
	Navigate();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int COwnLimitDlg::DoModal(HANDOPOPUP HandoPopup) 
{
	// TODO: Add your specialized code here and/or call the base class
	//m_pParentWnd = GM().GetMainWnd();
	
	m_HandoPopup = HandoPopup;

	COleDateTime time = COleDateTime::GetCurrentTime();
	int today = time.GetDay();

	if( today == NMBASE::UTIL::GetProfile_int( GM().GetMyInfo()->UI.ID, GetNotTodayKeyString(), 0 ) )
	{
		return IDCANCEL;
	}

	return CUPGDlgBase::DoModal();
}

CString COwnLimitDlg::GetNotTodayKeyString()
{
	CString str;
	str.Format("DoNotShowHandoThisDay_%d", m_HandoPopup);
	return str;
}

void COwnLimitDlg::OnDestroy() 
{
	CUPGDlgBase::OnDestroy();
	
	// TODO: Add your message handler code here
	DestroyBrowser();
}

void COwnLimitDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) return;

	CRect rect;
	GetClientRect(&rect);

	cdc.SelectObject(&m_Back);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);	
	
	cdc.DeleteDC();

	// Do not call CUPGDlgBase::OnPaint() for painting messages
}

void COwnLimitDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	COleDateTime time = COleDateTime::GetCurrentTime();	
	NMBASE::UTIL::WriteProfile_int( GM().GetMyInfo()->UI.ID, GetNotTodayKeyString(), time.GetDay() );
	CUPGDlgBase::OnOK();
}

void COwnLimitDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CUPGDlgBase::OnCancel();
}


BOOL COwnLimitDlg::CreateBrowser()
{
	DestroyBrowser();
	
	// CreateWebPage
	RECT rt = { 0, 0, 449, 504};
	m_pBrowser = new NMBASE::UTIL::CNMSSOWebBrowser();
	if ( !m_pBrowser->Create( WS_VISIBLE | WS_CHILD, rt, GetSafeHwnd(), 52514, GetIDispatch(FALSE) )   )
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
	
	return TRUE;
}
void COwnLimitDlg::DestroyBrowser()
{
	if ( m_pBrowser )
	{
		m_pBrowser->Destroy();
		delete m_pBrowser;
		m_pBrowser = NULL;
	}
}


void COwnLimitDlg::ClosePopup(short i)
{
	OnCancel();
}



void COwnLimitDlg::Navigate()
{
	if (m_pBrowser==NULL)
		return;

	if (m_pBrowser->GetHWND()==NULL)
		return;


	CString strURL;	
	switch(m_HandoPopup)
	{
	case HANDOPOPUP_COLLECT_OVER_MONEY:
		strURL = getGameURL(IDX_GAMEURL_COLLECTOVERMONEY);
		break;
	case HANDOPOPUP_ACHIEVE_LIMIT_MONEY:
		strURL = getGameURL(IDX_GAMEURL_ACHIEVELIMITMONEY);
		m_BtnOk.ShowWindow(SW_HIDE);
		break;
	case HANDOPOPUP_COLLECT_REVISION_MONEY:
		strURL = getGameURL(IDX_GAMEURL_COLLECTREVISIONMONEY);
		break;
	case HANDOPOPUP_COLLECT_BOTH_MONEY:
		strURL = getGameURL(IDX_GAMEURL_COLLECTBOTHMONEY);
		break;
	case HANDOPOPUP_COLLECT_OVER_GOLDBANK:
		strURL = getGameURL(IDX_GAMEURL_COLLECTOVERGOLDBANK);
		break;
	}

	m_pBrowser->Navigate(strURL);

}

LRESULT COwnLimitDlg::OnDoModal(WPARAM &wParam, LPARAM &lParam)
{
	GM().SetWaitOwnLimitPopup(false);

	return CUPGDlgBase::DoModal();
}
