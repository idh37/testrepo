// LobbyEventDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "LobbyEventDlg.h"
#include <afxinet.h>


// CLobbyEventDlg ��ȭ �����Դϴ�.

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
	�ܺ� ActiveX ��� �Ҷ�
	DISP_FUNCTION(Ŭ������, �ܺ��Լ���, �ɹ��Լ���, ����Ÿ��, ���ڰ�1 ���ڰ�2) 
	���� ) http://msdn.microsoft.com/ko-kr/library/cc451431(VS.71).aspx
	*/
	DISP_FUNCTION( CLobbyEventDlg, "ClosePopup", ClosePopup, VT_EMPTY, VTS_I2 )
END_DISPATCH_MAP()

// CLobbyEventDlg �޽��� ó�����Դϴ�.

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

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
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
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CLobbyEventDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	// �׸��� �޽����� ���ؼ��� CUPGDlgBase::OnPaint()��(��) ȣ������ ���ʽÿ�.

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

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
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
