#include "stdafx.h"
#include "AskMoveGameDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAskMoveGameDlg::CAskMoveGameDlg(CWnd* pParent /*=NULL*/)
	: CUPGDlgBase(CAskMoveGameDlg::IDD, pParent)
{
	m_idxGame=IDX_GAME_NULL;
}

void CAskMoveGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CUPGDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAskMoveGameDlg)
	DDX_Control(pDX, IDCANCEL, m_CancelBtn);
	DDX_Control(pDX, IDOK, m_OkBtn);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAskMoveGameDlg, CUPGDlgBase)
	//{{AFX_MSG_MAP(CAskMoveGameDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAskMoveGameDlg message handlers

BOOL CAskMoveGameDlg::OnInitDialog() 
{
	CUPGDlgBase::OnInitDialog();
	
	// TODO: Add extra initialization here

	// 다이얼로그 컨트롤 포지션과 위치 재정렬
	NMBASE::UTIL::CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);


	m_OkBtn.Init(47,123,".\\common\\image\\commonbtn\\Btn_ok.bmp", 4);
	m_CancelBtn.Init(137,123,".\\common\\image\\commonbtn\\Btn_cancel.bmp", 4);
	
	NMBASE::SKINGDI::WindowResize2Bitmap(m_hWnd, ".\\common\\image\\Invite\\BackAskMoveGame.bmp");
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAskMoveGameDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CUPGDlgBase::OnOK();
}

void CAskMoveGameDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CUPGDlgBase::OnCancel();
}

int CAskMoveGameDlg::DoModal(IDX_GAME idxGame) 
{
	// TODO: Add your specialized code here and/or call the base class

	AfxGetMainWnd()->SendMessageToDescendants(WM_CANCELMODE);
	
	m_idxGame = idxGame;
	
	return CUPGDlgBase::DoModal();
}

void CAskMoveGameDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) return;
	
	NMBASE::SKINGDI::CMyBitmap Back;
	Back.LoadBitmapFile(".\\common\\image\\Invite\\BackAskMoveGame.bmp");


// 	HRGN h = NMBASE::SKINGDI::BitmapToRegion( Back.operator HBITMAP(), RGB(255,0,255));
// 	if(h) SetWindowRgn(h,TRUE);// 사용방법

	CBitmap *pOldBmp = cdc.SelectObject(&Back);

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);

	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(COLOR_DEFAULT_POPUP);
	dc.SelectObject(FONT_DEFAULT);
	

	CFont* pOldFont = dc.SelectObject(FONT_DEFAULT);	
	
	rect.SetRect(0,0,59,15);	
	rect.OffsetRect(25,46);

	//게임위치
	DrawGameText(dc, m_idxGame, &rect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);		

	rect.SetRect(0,0,59,15);	
	rect.OffsetRect(25,82);
	DrawGameText(dc, m_idxGame, &rect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);				


	cdc.SelectObject(pOldFont);
	cdc.SelectObject(pOldBmp);
	cdc.DeleteDC();
	Back.DeleteObject();

//	::DeleteObject(h);
//	h = NULL;
	
	// Do not call CUPGDlgBase::OnPaint() for painting messages
}
