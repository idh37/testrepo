#include "stdafx.h"
#include "ConfirmVote.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CConfirmVote::CConfirmVote(CWnd* pParent /*=NULL*/)
	: CUPGDlgBase(CConfirmVote::IDD, pParent)
{
}

void CConfirmVote::DoDataExchange(CDataExchange* pDX)
{
	CUPGDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfirmVote)
	DDX_Control(pDX, IDCANCEL, m_CancelBtn);
	DDX_Control(pDX, IDOK, m_OkBtn);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CConfirmVote, CUPGDlgBase)
	//{{AFX_MSG_MAP(CConfirmVote)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfirmVote message handlers

BOOL CConfirmVote::OnInitDialog() 
{
	CUPGDlgBase::OnInitDialog();
		NMBASE::UTIL::CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);
	// TODO: Add extra initialization here
	// 다이얼로그 컨트롤 포지션과 위치 재정렬
	Back.LoadBitmapFile(".\\common\\image\\vote\\banishok.BMP");
	// 버튼 초기화
	m_OkBtn.Init( 28, 98, ".\\common\\image\\commonbtn\\Btn_yes.bmp", 4 );
	m_CancelBtn.Init( 130, 98, ".\\common\\image\\commonbtn\\Btn_no.bmp", 4 );

	//이미지 사이즈에맞게 변환dlg
	NMBASE::SKINGDI::WindowResize2Bitmap(m_hWnd, ".\\common\\image\\vote\\banishok.BMP");	


	SetForegroundWindow();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfirmVote::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) return;

	if(Back.GetSafeHandle() == NULL)return;
	HRGN h = NMBASE::SKINGDI::BitmapToRegion( Back.operator HBITMAP(), RGB(255,0,255));
	if(h) SetWindowRgn(h,TRUE);// 사용방법
	CBitmap *pOldBmp = cdc.SelectObject(&Back);

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);

	dc.SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_12_BOLD));
	dc.SetBkMode(TRANSPARENT);

	dc.SetTextColor( RGB( 66, 46, 13) );

	rect.SetRect(109,48,225,62);
	dc.DrawText( m_ID, &rect, DT_CENTER | DT_WORDBREAK );

	cdc.SelectObject(pOldBmp);
	cdc.DeleteDC();	
	// Do not call CUPGDlgBase::OnPaint() for painting messages
}

void CConfirmVote::OnDestroy() 
{
	CUPGDlgBase::OnDestroy();
	
	// TODO: Add your message handler code here
	Back.DeleteObject();
	
}

void CConfirmVote::SetID(char *pID)
{
	if ( strlen( pID ) > 0 )
	{
		ZeroMemory( m_ID, 16 );
		strncpy( m_ID, pID, 15 );
	}
}
