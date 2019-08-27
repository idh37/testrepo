// WebEventDisplayer.cpp : implementation file
//

#include "stdafx.h"
#include "WebEventDisplayer.h"
//#include <afxinet.h>
#include <Exdispid.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWebEventDisplayer dialog


CWebEventDisplayer::CWebEventDisplayer(CWnd* pParent /*=NULL*/)
	: CDialog(CWebEventDisplayer::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWebEventDisplayer)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_WebW = 380;
	m_WebH = 470;
}


void CWebEventDisplayer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWebEventDisplayer)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWebEventDisplayer, CDialog)
	//{{AFX_MSG_MAP(CWebEventDisplayer)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BEGIN_EVENTSINK_MAP(CWebEventDisplayer, CDialog)
 //{{AFX_EVENTSINK_MAP(CWebEventDisplayer)
//	ON_EVENT(CWebEventDisplayer, 61328, DISPID_WINDOWSETWIDTH, OnWindowSetWidth, VTS_I4) 
//	ON_EVENT(CWebEventDisplayer, 61328, DISPID_WINDOWSETHEIGHT, OnWindowSetHeight, VTS_I4)
//	ON_EVENT(CWebEventDisplayer, 61328, DISPID_DOCUMENTCOMPLETE,WbDocumentComplete,VTS_DISPATCH VTS_VARIANT)
//	ON_EVENT(CWebEventDisplayer, 61328, DISPID_WINDOWCLOSING, OnClosing, VTS_BOOL VTS_PBOOL)
//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()
/////////////////////////////////////////////////////////////////////////////
// CWebEventDisplayer message handlers

void CWebEventDisplayer::OnClosing(BOOL IsChildWindow,BOOL *pCancel)
{
	*pCancel = TRUE;
	::PostMessage(m_hWnd,WM_CLOSE,0,0);
}

void CWebEventDisplayer::OnWindowSetWidth(long width)
{
	m_WebW = width;
}
void CWebEventDisplayer::OnWindowSetHeight(long height)
{
	m_WebH = height;
}



void CWebEventDisplayer::WbDocumentComplete(LPDISPATCH pDisp,VARIANT FAR* URL)
{
	int BorderX = GetSystemMetrics(SM_CXBORDER);
	int BorderY = GetSystemMetrics(SM_CYBORDER);
	int Caption = GetSystemMetrics(SM_CYCAPTION);


	SetWindowPos(&wndTop,0,0,m_WebW,m_WebH,SWP_SHOWWINDOW | SWP_NOMOVE); //m_WebH
	CenterWindow(GetDesktopWindow());
	m_pLocWebCtrl.MoveWindow(0,0,m_WebW,m_WebH);
}

BOOL CWebEventDisplayer::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect rc(0,0,5,5);
	m_pLocWebCtrl.Create(NULL,"WebControlX2",WS_VISIBLE | WS_CHILD,rc, this, 61328);
	CString strURL = "http://norazo.humoruniv.com/event_voice/index_popup.html";
	m_pLocWebCtrl.Navigate(strURL);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWebEventDisplayer::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	delete m_pLocWebCtrl;	
}

BOOL CWebEventDisplayer::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_RBUTTONUP) pMsg->message = 0;
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) pMsg->wParam = 0;	
	return CDialog::PreTranslateMessage(pMsg);
}
