// GameManualDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GameManualDlg.h"


// CGameManualDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CGameManualDlg, CUPGDlgBase)

CGameManualDlg::CGameManualDlg(CWnd* pParent /*=NULL*/)
	: CUPGDlgBase(CGameManualDlg::IDD, pParent)
{
	m_pBrowser=NULL;
}

CGameManualDlg::~CGameManualDlg()
{
}

void CGameManualDlg::DoDataExchange(CDataExchange* pDX)
{
	CUPGDlgBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDCANCEL, m_CloseBtn);
}


BEGIN_MESSAGE_MAP(CGameManualDlg, CUPGDlgBase)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CGameManualDlg 메시지 처리기입니다.



void CGameManualDlg::DestroyBrowser()
{
	if ( m_pBrowser )
	{
		m_pBrowser->Destroy();
		delete m_pBrowser;
		m_pBrowser = NULL;
	}
}

BOOL CGameManualDlg::CreateBrowser()
{
	DestroyBrowser();

	// CreateWebPage
	RECT rt = { 5, 38, 5+980, 38+681};
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
	
	m_pBrowser->Navigate(getGameURL(IDX_GAMEURL_GAMEMANUAL));

	return TRUE;
}




BOOL CGameManualDlg::OnInitDialog()
{
	CUPGDlgBase::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	if (m_BackBit.LoadBitmapFile(".\\Lobby\\Image\\GameManual\\BackGameManual.bmp")==FALSE)
		return FALSE;

	m_CloseBtn.Init( 960, 6, ".\\Lobby\\image\\GameManual\\Btn_Close.bmp", 4 ); 

	MoveWindow(0,0,m_BackBit.GetWidth() ,m_BackBit.GetHeight());
	CenterWindow(m_pParentWnd);
	CreateBrowser();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CGameManualDlg::OnDestroy()
{
	CUPGDlgBase::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	DestroyBrowser();
	if (m_BackBit.m_hObject) m_BackBit.DeleteObject();
}

void CGameManualDlg::OnPaint()
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


BOOL CGameManualDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN)
	{
		return TRUE;
	}

	return CUPGDlgBase::PreTranslateMessage(pMsg);
}

void CGameManualDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	


	//PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
	CUPGDlgBase::OnLButtonDown(nFlags, point);
}

