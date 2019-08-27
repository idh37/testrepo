// KisaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KisaDlg.h"
#include <CommMsgDef.h>
#include "GlobalBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKisaDlg dialog

CKisaDlg *CKisaDlg::s_pKisaDlg = NULL;


CKisaDlg::CKisaDlg(CWnd* pParent /*=NULL*/)
	: CUPGDlgBase(CKisaDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKisaDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pBrowser = NULL;

	
}


void CKisaDlg::DoDataExchange(CDataExchange* pDX)
{
	CUPGDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKisaDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKisaDlg, CUPGDlgBase)
	//{{AFX_MSG_MAP(CKisaDlg)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKisaDlg message handlers



void CKisaDlg::DestroyBrowser()
{
	if ( m_pBrowser )
	{
		m_pBrowser->Destroy();
		delete m_pBrowser;
		m_pBrowser = NULL;
	}
}

BOOL CKisaDlg::CreateBrowser()
{
	DestroyBrowser();
	
	// CreateWebPage
	RECT rt = { 2, 2, 309, 289};
	m_pBrowser = new NMBASE::UTIL::CNMSSOWebBrowser();
	if ( !m_pBrowser->Create( WS_VISIBLE | WS_CHILD, rt, GetSafeHwnd(), 52361 ) )
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
	m_pBrowser->SetCallback_OnBeforeNavigate2( OnBeforeKisaNavigate );

	
	//strURL="http://kr.yahoo.com";
	m_pBrowser->Navigate(getGameURL(IDX_GAMEURL_KISA));

	return TRUE;
}



BOOL CKisaDlg::OnInitDialog() 
{
	CUPGDlgBase::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_BackBit.LoadBitmap(".\\common\\Image\\RecoveryCharge\\kisa.bmp");

	MoveWindow(0,0,311,291);
	CenterWindow(m_pParentWnd);
	CreateBrowser();
	

	s_pKisaDlg=this;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CKisaDlg::OnBeforeKisaNavigate(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, LPCTSTR lpszPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel)
{

	CString strURL = lpszURL;
	
	if (strURL.Find("_충전완료_") > 0)
	{
		if (pbCancel)
			*pbCancel = TRUE;

		//기사 회생을 받기 완료했다고 서버에 알려준다. 
		CCL_REPLY_KISACHARGE msg;
		msg.Set(TRUE); //기사 회생 충전완료
		NMBASE::SOCK::GSOCKMAN().SendData( g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize() );

		if (s_pKisaDlg)
				s_pKisaDlg->SendMessage(WM_COMMAND, IDOK, 0);
	
	}
	else if (strURL.Find("_충전취소_") > 0)
	{
		if (pbCancel)
			*pbCancel = TRUE;		

		if (s_pKisaDlg)
				s_pKisaDlg->SendMessage(WM_COMMAND, IDCANCEL, 0);
	}


	

}

void CKisaDlg::OnDestroy() 
{
	CUPGDlgBase::OnDestroy();
	
	// TODO: Add your message handler code here
	DestroyBrowser();

	if (m_BackBit.m_hObject) m_BackBit.DeleteObject();
}

void CKisaDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) return;
	CBitmap *pOldBmp = cdc.SelectObject(&m_BackBit);

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);

	cdc.SelectObject(pOldBmp);
	cdc.DeleteDC();		
	
	// Do not call CUPGDlgBase::OnPaint() for painting messages
}

void CKisaDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	//기사 회생을 받기 취소했다고 서버에 알려준다. 
	CCL_REPLY_KISACHARGE msg;
	msg.Set(FALSE); //기사 회생 충전완료
	NMBASE::SOCK::GSOCKMAN().SendData( g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize() );	
	
	CUPGDlgBase::OnCancel();
}

void CKisaDlg::OnOK() 
{
	// TODO: Add extra validation here

	CUPGDlgBase::OnOK();
}

BOOL CKisaDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN)
	{
		return TRUE;
	}
		
	return CUPGDlgBase::PreTranslateMessage(pMsg);
}
