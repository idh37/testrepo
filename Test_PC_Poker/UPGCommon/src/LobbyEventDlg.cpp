// LobbyEventDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "LobbyEventDlg.h"
#include <afxinet.h>


// CLobbyEventDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLobbyEventDlg, CUPGDlgBase)

CLobbyEventDlg::CLobbyEventDlg(CWnd* pParent /*=NULL*/, LobbyEventType type)
	: CUPGDlgBase(CLobbyEventDlg::IDD, pParent), m_eventType(type)
{
	m_pBrowser=NULL;
	
	EnableAutomation();

}

CLobbyEventDlg::~CLobbyEventDlg()
{
}

void CLobbyEventDlg::DoDataExchange(CDataExchange* pDX)
{
	CUPGDlgBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDCANCEL, m_CloseBtn);
}


BEGIN_MESSAGE_MAP(CLobbyEventDlg, CUPGDlgBase)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CLobbyEventDlg, CUPGDlgBase)
	/*
	외부 ActiveX 사용 할때
	DISP_FUNCTION(클래스명, 외부함수명, 맴버함수명, 리턴타입, 인자값1 인자값2) 
	참고 ) http://msdn.microsoft.com/ko-kr/library/cc451431(VS.71).aspx
	*/
	DISP_FUNCTION( CLobbyEventDlg, "ClosePopup", ClosePopup, VT_EMPTY, VTS_I2 )
END_DISPATCH_MAP()

// CLobbyEventDlg 메시지 처리기입니다.

void CLobbyEventDlg::DestroyBrowser()
{
	if ( m_pBrowser )
	{
		m_pBrowser->Destroy();
		delete m_pBrowser;
		m_pBrowser = NULL;
	}
}

BOOL CLobbyEventDlg::CreateBrowser()
{
	DestroyBrowser();

	if (EVENT_CHICKENBIG == m_eventType)
		return TRUE;

	// CreateWebPage
	long x1 = 0;
	long x2 = m_BackBit.GetWidth();
	long y1 = 0;
	long y2 = m_BackBit.GetHeight();
	if (EVENT_NORMAL == m_eventType)
	{
		x1 = 11;
		x2 = 11 + 506;
		y1 = 31;
		y2 = 31 + 645;
	}
	RECT rt = {x1, y1, x2, y2};
	
	m_pBrowser = new NMBASE::UTIL::CNMSSOWebBrowser();
	if ( !m_pBrowser->Create( WS_VISIBLE | WS_CHILD, rt, GetSafeHwnd(), 52361, GetIDispatch(FALSE) ) )
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


	
	
	m_pBrowser->Navigate(m_strURL.GetString());

	return TRUE;
}



BOOL CLobbyEventDlg::OnInitDialog()
{
	CUPGDlgBase::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	if (EVENT_NORMAL == m_eventType)
	{
		if (m_BackBit.LoadBitmapFile(".\\Lobby\\Image\\LobbyEventDlg\\BackLobbyEvent.bmp")==FALSE)
			return FALSE;
		m_CloseBtn.Init( 494, 4, ".\\Lobby\\image\\LobbyEventDlg\\Btn_Close.bmp", 4 ); 
	}
	else if (EVENT_CHICKEN == m_eventType || EVENT_CHICKENREWARD == m_eventType)
	{
		if (m_BackBit.LoadBitmapFile(".\\Lobby\\Image\\Chicken\\ChickenBG.bmp")==FALSE)
			return FALSE;
		m_CloseBtn.Init( 587, 8, ".\\Lobby\\Image\\Chicken\\Btn_chickenclose.bmp", 4 ); 
	}
	else if (EVENT_CHICKENBIG == m_eventType)
	{
		if (m_BackBit.LoadBitmapFile(".\\Lobby\\Image\\Chicken\\ChickenBigBG.bmp")==FALSE)
			return FALSE;
		m_CloseBtn.Init( 587, 8, ".\\Lobby\\Image\\Chicken\\Btn_chickenclose.bmp", 4 ); 
	}

	MoveWindow(0,0,m_BackBit.GetWidth() ,m_BackBit.GetHeight());
	CenterWindow(m_pParentWnd);
	CreateBrowser();

	ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CLobbyEventDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CUPGDlgBase::OnPaint()을(를) 호출하지 마십시오.

	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) return;
	CBitmap *pOldBmp = cdc.SelectObject(&m_BackBit);

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);

	cdc.SelectObject(pOldBmp);
	cdc.DeleteDC();		
}

void CLobbyEventDlg::OnDestroy()
{
	CUPGDlgBase::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	DestroyBrowser();
	if (m_BackBit.m_hObject) m_BackBit.DeleteObject();
}

BOOL CLobbyEventDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN)
	{
		return TRUE;
	}

	return CUPGDlgBase::PreTranslateMessage(pMsg);
}

void CLobbyEventDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	

	PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
	CUPGDlgBase::OnLButtonDown(nFlags, point);
}


int CLobbyEventDlg::DoModal(LPCTSTR szURL)
{

	m_strURL = szURL;

	return CUPGDlgBase::DoModal();
}


void CLobbyEventDlg::ClosePopup( short i )
{

	CUPGDlgBase::EndDialog(IDOK);

}
