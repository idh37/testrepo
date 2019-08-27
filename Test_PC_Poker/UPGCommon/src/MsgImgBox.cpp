// MsgImgBox.cpp : implementation file
//

#include "stdafx.h"
#include "MsgImgBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMsgImgBox dialog


CMsgImgBox::CMsgImgBox(CWnd* pParent /*=NULL*/)
	: CUPGDlgBase(CMsgImgBox::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMsgImgBox)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMsgImgBox::DoDataExchange(CDataExchange* pDX)
{
	CUPGDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMsgImgBox)
	DDX_Control(pDX, IDCANCEL, m_CancelBtn);
	DDX_Control(pDX, IDOK, m_OKBtn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMsgImgBox, CUPGDlgBase)
	//{{AFX_MSG_MAP(CMsgImgBox)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsgImgBox message handlers

#define BTN_W 88
#define BTN_H 35
#define BTN_BETWEEN 10

BOOL CMsgImgBox::OnInitDialog() 
{
	CUPGDlgBase::OnInitDialog();
	
	// TODO: Add extra initialization here
	NMBASE::UTIL::CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);		

	NMBASE::SKINGDI::WindowResize2Bitmap(m_hWnd, m_BgPath);
	
	CRect rt;
	GetClientRect(&rt);
	
	if( m_nBtn_type == TPYE_YES || m_nBtn_type == TYPE_OK ){
		int x = (rt.Width()/2) - (BTN_W/2);
		int y = rt.Height()/BTN_H;
		
		y = ((y-1) * BTN_H) - 4;
		//88,31픽셀
		m_OKBtn.Init(x, y, ".\\common\\image\\commonbtn\\Btn_ok.bmp", 4);
	}
	else if( m_nBtn_type == TYPE_YESNO || m_nBtn_type == TYPE_OKNO ){
		int x = (rt.Width()/2) - (BTN_W) - (BTN_BETWEEN/2);
		int y = rt.Height()/BTN_H;
		
		y = ((y-1) * BTN_H) - 4;
		m_OKBtn.Init(x, y, ".\\common\\image\\commonbtn\\Btn_ok.bmp", 4);
		
		x = x + BTN_BETWEEN + BTN_W;
		m_CancelBtn.Init(x, y, ".\\common\\image\\commonbtn\\Btn_cancel.bmp", 4);
	}	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMsgImgBox::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) return;

	NMBASE::SKINGDI::CMyBitmap Back;
	Back.LoadBitmapFile(m_BgPath);

	HRGN h = NMBASE::SKINGDI::BitmapToRegion( Back.operator HBITMAP(), RGB(255,0,255));
	if(h) SetWindowRgn(h,TRUE);// 사용방법

	CBitmap *pOldBmp = cdc.SelectObject(&Back);
	
	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);
	dc.SetBkMode(TRANSPARENT);


	if( m_bText ){

		dc.SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_12_BOLD));
		dc.SetTextColor(RGB(57,57,57));
	//	CRect rt;
	//	rt.SetRect(0,0,291,59);
	//	rt.OffsetRect(14,53);

		CRect rt;
		rt = m_TextRect;		
		rt.OffsetRect(-1, 0);
		dc.DrawText(m_Msg,rt,DT_LEFT|DT_WORDBREAK);
		rt = m_TextRect;
		rt.OffsetRect(0, -1);
		dc.DrawText(m_Msg,rt,DT_LEFT|DT_WORDBREAK);
		rt = m_TextRect;
		rt.OffsetRect(1, 0);
		dc.DrawText(m_Msg,rt,DT_LEFT|DT_WORDBREAK);
		rt = m_TextRect;
		rt.OffsetRect(0, 1);
		dc.DrawText(m_Msg,rt,DT_LEFT|DT_WORDBREAK);

		dc.SetTextColor(RGB(172,172,172));
		dc.DrawText(m_Msg,m_TextRect,DT_LEFT|DT_WORDBREAK);
	}
	
	
	cdc.DeleteDC();
	Back.DeleteObject();	
}

int CMsgImgBox::DoModal(CString bgpath , int btntype) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_BgPath = bgpath;
	m_nBtn_type = btntype;	
	return CUPGDlgBase::DoModal();
}

int CMsgImgBox::DoModal(CString bgpath,CString Msg ,CRect rt, int btntype,BOOL bText)
{
	m_BgPath = bgpath;
	m_nBtn_type = btntype;	
	m_Msg = Msg;
	m_bText = bText;
	m_TextRect = rt;
	return CUPGDlgBase::DoModal();
}
