// MyFamilyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyFamilyDlg.h"
#include "GlobalBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyFamilyDlg dialog

 
CMyFamilyDlg::CMyFamilyDlg(CWnd* pParent /*=NULL*/)
	: CUPGDlgBase(CMyFamilyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyFamilyDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
}


void CMyFamilyDlg::DoDataExchange(CDataExchange* pDX)
{
	CUPGDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyFamilyDlg)
	DDX_Control(pDX, IDCANCEL, m_CloseBtn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyFamilyDlg, CUPGDlgBase)
	//{{AFX_MSG_MAP(CMyFamilyDlg)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyFamilyDlg message handlers

CMyFamilyDlg* CMyFamilyDlg::s_pFamilyDlg = NULL;


void CMyFamilyDlg::OnOk() 
{
	// TODO: Add your control notification handler code here
	EndDialog(IDCANCEL);
	//CUPGDlgBase::OnOK();
}

void CMyFamilyDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	EndDialog(IDCANCEL);
	//CUPGDlgBase::OnCancel();
}

int CMyFamilyDlg::DoModal( int PremMenuNo, int GameCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CString str;
	if( CODE_NEW_FAMILY_NONE == PremMenuNo )
	{
		return IDOK;
	}

	m_URL = getGameURL(IDX_GAMEURL_MYFAMILY);
	
	return CUPGDlgBase::DoModal();
}

BOOL CMyFamilyDlg::OnInitDialog() 
{
	CUPGDlgBase::OnInitDialog();
	
	// TODO: Add extra initialization here
	// 배경 그림 읽기
	m_Back.LoadBitmapFile(".\\common\\image\\MyFamily\\Back.bmp");
	m_Back.ResizeWnd( this, 0, 0, TRUE, GM().GetMainWnd() );
	m_CloseBtn.Init( 405, 5, ".\\common\\image\\commonbtn\\Btn_X.bmp", 4 );	

	CRect rc;
	rc.SetRect( 0, 0, 416, 530 );
	rc.OffsetRect( 12, 32 );
	m_Web.Create( WS_VISIBLE | WS_CHILD, rc, GetSafeHwnd(), 3432 );	
	
	m_Web.SetHideBorder( TRUE );	
	m_Web.SetCallback_OnBeforeNavigate2( SSO_OnBeforeNavigate2);
	m_Web.ResetUserNavigatedCount();
	m_Web.Navigate( m_URL );

	s_pFamilyDlg = this;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMyFamilyDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	SendMessage( WM_NCLBUTTONDOWN, HTCAPTION, 0 );
	
	CUPGDlgBase::OnLButtonDown(nFlags, point);
}

void CMyFamilyDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	m_Back.BitBlt( dc );
	// Do not call CUPGDlgBase::OnPaint() for painting messages
}

void CMyFamilyDlg::OnBeforeMyFamilyNavigate(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, LPCTSTR lpszPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel)
{
	CString strURL = lpszURL;
	
	if (strURL.Find("_충전완료_") > 0)
	{
		if (pbCancel)
			*pbCancel = TRUE;

		CSV_ASK_GAME_CONTINUE msg;
		msg.Set(GM().GetMyInfo()->UI.ID, 0);
		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());

		if (s_pFamilyDlg)
				s_pFamilyDlg->SendMessage(WM_COMMAND, IDOK, 0);
	
	}

}

BOOL CMyFamilyDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_Web.Destroy();
	
	return CUPGDlgBase::DestroyWindow();
}
