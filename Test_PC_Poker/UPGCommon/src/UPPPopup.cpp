#include "stdafx.h"
#include "UPPPopup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PATH_IAMGEFOLDER_ENDPOPUP ".\\common\\image\\EndPopupDlg\\"

#define STR_UPP_POPUPTYPE01 "http://game1.netmarble.net/upp/?type=1"
#define STR_UPP_POPUPTYPE02 "http://game1.netmarble.net/upp/?type=2"
#define STR_UPP_POPUPTYPE03 "http://game1.netmarble.net/upp/?type=3"

#define EVENT_NONE 0
#define EVENT_GMARKET 1
#define EVENT_RICH 2
#define EVENT_NATE 3	//추가

CUPPPopup::CUPPPopup (CWnd* pParent /*=NULL*/)
	: CUPGDlgBase(CUPPPopup::IDD, pParent)
{
	m_pBrowser=NULL;
	m_pCustomHtml=NULL;
	m_pParent = pParent;
	m_nUPPPopupType = 0;
	EnableAutomation();
}

CUPPPopup::~CUPPPopup ()
{
}


void CUPPPopup::DoDataExchange(CDataExchange* pDX)
{
	CUPGDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUPPPopup )	
 	DDX_Control(pDX, IDOK, m_btnOK);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUPPPopup , CUPGDlgBase)
	//{{AFX_MSG_MAP(CUPPPopup )
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_CLOSE()	
	ON_WM_DESTROY()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUPPPopup  message handlers

BEGIN_DISPATCH_MAP(CUPPPopup , CUPGDlgBase)
	/*
	외부 ActiveX 사용 할때
	DISP_FUNCTION(클래스명, 외부함수명, 맴버함수명, 리턴타입, 인자값1 인자값2) 
	참고 ) http://msdn.microsoft.com/ko-kr/library/cc451431(VS.71).aspx
	*/
 	DISP_FUNCTION( CUPPPopup , "ClosePopup", ClosePopup, VT_EMPTY, VTS_NONE )
 	DISP_FUNCTION( CUPPPopup , "Cancel", Cancel, VT_EMPTY, VTS_NONE )
// 	DISP_FUNCTION( CUPPPopup , "ClosePopup", ClosePopup, VT_EMPTY, VTS_I2 )
// 	DISP_FUNCTION( CUPPPopup , "ChangeTab", ChangeTab, VT_EMPTY, VTS_I2 )
// 	DISP_FUNCTION( CUPPPopup , "OnCompleteUseItem", OnCompleteUseItem, VT_EMPTY, VTS_I4 )
END_DISPATCH_MAP()

// 확인 부분을 window.external.ClosePopup();
// 취소부분을 window.external.Cancel();
void CUPPPopup::ClosePopup()
{
	CCL_ASK_UPPMSG msg;
	msg.Set((int)CCL_ASK_UPPMSG::AGREE, 0, 0, 0);
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
	OnOK();
}
void CUPPPopup::Cancel()
{
	CMsgBoxDlg dlg(this);
	if(dlg.DoModal(CMsgBoxDlg::EMBT_QUESTION, "취소하시겠습니까? 동의 하지 않으시면 게임 이용이 불가능 합니다.") == IDOK) 
	{
		OnCancel();
		return;
	}	
}
BOOL CUPPPopup::OnInitDialog() 
{
	CUPGDlgBase::OnInitDialog();
	
	CRect rc;
	GetClientRect( rc );
	rc.right = rc.left + 480;
	rc.bottom = rc.top + 456;
	MoveWindow(rc, TRUE);

	CenterWindow(GM().GetMainWnd());


	m_btnOK.ShowWindow(SW_HIDE);

	//CUPPPopup은 정책 동의 관련 웹인클라만 띄운다
	if ( m_nUPPPopupType == 2 )
	{
		SetURL( STR_UPP_POPUPTYPE02 );
	}
	else if ( m_nUPPPopupType == 1 )
	{
		SetURL( STR_UPP_POPUPTYPE01 );
	}
	else if ( m_nUPPPopupType == 3 )
	{
		SetURL( STR_UPP_POPUPTYPE03 );
	}
	
	CreateBrowser();	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUPPPopup::OnPaint()
{
	CPaintDC dc(this); // device context for painting	
	// TODO: Add your message handler code here
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	if( memDC.m_hDC == NULL ) return;
	
	memDC.SelectObject(&m_Back);

	
	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &memDC, 0, 0, SRCCOPY);

	memDC.DeleteDC();

	// Do not call CUPGDlgBase::OnPaint() for painting messages
}

void CUPPPopup::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CUPGDlgBase::OnLButtonDown(nFlags, point);
}

void CUPPPopup::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
// 	OnCancel();
// 
// 	CUPGDlgBase::OnClose();
}

void CUPPPopup::OnOK() 
{
	// TODO: Add extra validation here
	CUPGDlgBase::OnOK();
}

void CUPPPopup::OnCancel() 
{
	// TODO: Add extra cleanup here
	CUPGDlgBase::OnCancel();
}

void CUPPPopup::OnDestroy() 
{
	CUPGDlgBase::OnDestroy();

	// TODO: Add your message handler code here


	DestroyBrowser();
	

} 
void CUPPPopup::DestroyBrowser()
{
	if ( m_pBrowser )
	{
		m_pBrowser->Destroy();
		delete m_pBrowser;
		m_pBrowser = NULL;
	}

}

BOOL CUPPPopup::PreTranslateMessage( MSG* pMsg )
{
	switch( pMsg->message )
	{
	case WM_SETCURSOR:// 메세지를 집어 삼킨다
		{
		} return TRUE;
	case WM_KEYDOWN:
		{
			switch(pMsg->wParam)
			{
			case VK_RETURN:
			case VK_ESCAPE:
				return TRUE;
			}			
		} break;
	}

	return CWnd::PreTranslateMessage( pMsg );
}

 
BOOL CUPPPopup::CreateBrowser()
{
	
	DestroyBrowser();

	
	// CreateWebPage
	RECT rt = { 0, 0, 480, 456};
	m_pBrowser = new NMBASE::UTIL::CNMSSOWebBrowser();
	if ( !m_pBrowser->Create( WS_VISIBLE | WS_CHILD, rt, GetSafeHwnd(), 52512, GetIDispatch(FALSE) ) )
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
	m_pBrowser->SetCallback_OnBeforeNavigate2( SSO_OnBeforeNavigate2 );
	m_pBrowser->Navigate(m_strURL);

	return TRUE;
}

void CUPPPopup::SetURL(const char * pURL)
{
	m_strURL = pURL;
}
void CUPPPopup::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CUPGDlgBase::OnLButtonUp(nFlags, point);
}

INT_PTR CUPPPopup::DoModal(int nPopupType)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_nUPPPopupType = nPopupType;

	return CUPGDlgBase::DoModal();
}

BOOL CUPPPopup::CreateCustomHtml()
{
	if (m_pCustomHtml)
		return FALSE;
	
	return TRUE;
}

void CUPPPopup::DestoryCustomHtml()
{

	if ( m_pCustomHtml )
	{
		m_pCustomHtml->SendMessage(WM_DESTROY, 0,0);
		delete m_pCustomHtml;
		m_pCustomHtml = NULL;
	}
}
