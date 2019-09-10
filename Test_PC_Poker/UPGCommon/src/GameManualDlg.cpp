// GameManualDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "GameManualDlg.h"


// CGameManualDlg ��ȭ �����Դϴ�.

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


// CGameManualDlg �޽��� ó�����Դϴ�.



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

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	if (m_BackBit.LoadBitmapFile(".\\Lobby\\Image\\GameManual\\BackGameManual.bmp")==FALSE)
		return FALSE;

	m_CloseBtn.Init( 960, 6, ".\\Lobby\\image\\GameManual\\Btn_Close.bmp", 4 ); 

	MoveWindow(0,0,m_BackBit.GetWidth() ,m_BackBit.GetHeight());
	CenterWindow(m_pParentWnd);
	CreateBrowser();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CGameManualDlg::OnDestroy()
{
	CUPGDlgBase::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	DestroyBrowser();
	if (m_BackBit.m_hObject) m_BackBit.DeleteObject();
}

void CGameManualDlg::OnPaint()
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

