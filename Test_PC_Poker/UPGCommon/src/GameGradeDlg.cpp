#include "stdafx.h"
#include "GameGradeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CGameGradeDlg::CGameGradeDlg(CWnd* pParent /*=NULL*/)
	: CUPGDlgBase(CGameGradeDlg::IDD, pParent)
{
}


void CGameGradeDlg::DoDataExchange(CDataExchange* pDX)
{
	CUPGDlgBase::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGameGradeDlg, CUPGDlgBase)
	//{{AFX_MSG_MAP(CGameGradeDlg)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGameGradeDlg message handlers

void CGameGradeDlg::DoModal(CWnd* pParent, int Mode)
{
	m_UseMode = Mode;

	m_pParent = pParent;
	CUPGDlgBase::DoModal();
}

void CGameGradeDlg::OnOK() 
{
	// TODO: Add extra validation here

	//DestroyWindow();
	//PostMessage(WM_COMMAND, IDCANCEL);

	CUPGDlgBase::OnOK();
}

void CGameGradeDlg::OnCancel() 
{
	// TODO: Add extra cleanup here

	return;
	//CUPGDlgBase::OnCancel();
}


BOOL CGameGradeDlg::OnInitDialog() 
{
	CUPGDlgBase::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(m_Back.m_hObject == NULL) {
		if(m_UseMode == 1) {		// 이용등급표시
			m_Back.LoadBitmapFile(".\\common\\image\\game_grade.bmp");	
		}
		else if(m_UseMode == 2) {	// 게임내용 표시(사행성)
			m_Back.LoadBitmapFile(".\\common\\image\\game_contents.bmp");	
		}
		else if(m_UseMode == 3) {	// 팝업창
			m_Back.LoadBitmapFile(".\\common\\image\\GradeWin.bmp");	
		}
	}

	if(m_Back.m_hObject != NULL) {

		RECT rc1;
			
		if(m_UseMode != 3) {
			GetWindowRect( &rc1 );
			rc1.left = rc1.left+m_PosX;
			rc1.top = rc1.top+m_PosY;
			rc1.right = rc1.left + m_Back.GetWidth();
			rc1.bottom = rc1.top + m_Back.GetHeight();
			MoveWindow( &rc1 );
		}
		else {
			::GetWindowRect( m_pParent->m_hWnd, &rc1 );
			rc1.left = rc1.left+((rc1.right-rc1.left)-m_Back.GetWidth())/2;
			rc1.top = rc1.top+((rc1.bottom-rc1.top)-m_Back.GetHeight())/2;
			rc1.right = rc1.left + m_Back.GetWidth();
			rc1.bottom = rc1.top + m_Back.GetHeight();
			MoveWindow( &rc1 );	
		}
	}

	CRect rect;
	rect.SetRect(m_PosX, m_PosY, m_PosX+m_Back.GetWidth(), m_PosY+m_Back.GetHeight());

	SetTimer(TIMER_GAMEGRADE_COMMAND, 3000, NULL);

	// 최상위 윈도우로 설정
//	SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGameGradeDlg::MoveDlg(int x, int y)
{

	if(!GetSafeHwnd()) 
		return;

	if(m_UseMode == 3)
		return;

	if(m_Back.m_hObject != NULL) {
		RECT rc1;
		
		rc1.left = x + m_PosX;
		rc1.top = y + m_PosY;
		rc1.right = rc1.left + m_Back.GetWidth();
		rc1.bottom = rc1.top + m_Back.GetHeight();
		MoveWindow( &rc1 );
	}
}

BOOL CGameGradeDlg::Create(CWnd *pParentWnd, int x, int y, int usemode)
{

	m_UseMode = usemode;
	
	m_PosX = x;
	m_PosY = y;

	return Create( pParentWnd) ;
}

BOOL CGameGradeDlg::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class


	if(GetSafeHwnd()) 
		return FALSE;

	return CUPGDlgBase::Create(IDD, pParentWnd);
}


void CGameGradeDlg::OnPaint() 
{

	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);

	if(m_UseMode == 3) {	// 팝업창
		HRGN h = NMBASE::SKINGDI::BitmapToRegion( m_Back.operator HBITMAP(), RGB(255,0,255));
		if(h) SetWindowRgn(h,TRUE);// 사용방법
	}

	CBitmap *pOldBmp = cdc.SelectObject(&m_Back);

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);



	cdc.SelectObject(pOldBmp);
	cdc.DeleteDC();
	
	// TODO: Add your message handler code here
	
	// Do not call CUPGDlgBase::OnPaint() for painting messages
}

void CGameGradeDlg::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == TIMER_GAMEGRADE_COMMAND)
	{
		KillTimer(TIMER_GAMEGRADE_COMMAND);

		PostMessage(WM_COMMAND, IDOK);
	}
	
	CUPGDlgBase::OnTimer(nIDEvent);
}

void CGameGradeDlg::OnDestroy() 
{
	CUPGDlgBase::OnDestroy();

	// TODO: Add your message handler code here
	KillTimer(TIMER_GAMEGRADE_COMMAND);
	m_Back.DeleteObject();
}

BOOL CGameGradeDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	if(pMsg->message == WM_KEYDOWN) {
		return TRUE;
	}
	
	return CUPGDlgBase::PreTranslateMessage(pMsg);
}
