// VipCloseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VipCloseDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVipCloseDlg dialog


CVipCloseDlg::CVipCloseDlg(CWnd* pParent /*=NULL*/)
	: CUPGDlgBase(CVipCloseDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVipCloseDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CVipCloseDlg::DoDataExchange(CDataExchange* pDX)
{
	CUPGDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVipCloseDlg)
	DDX_Control(pDX, IDCANCEL, m_NoBtn);
	DDX_Control(pDX, IDOK, m_YesBtn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVipCloseDlg, CUPGDlgBase)
	//{{AFX_MSG_MAP(CVipCloseDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVipCloseDlg message handlers

BOOL CVipCloseDlg::OnInitDialog() 
{
	CUPGDlgBase::OnInitDialog();
	
	// TODO: Add extra initialization here
	NMBASE::SKINGDI::WindowResize2Bitmap(m_hWnd, ".\\common\\image\\vip\\vip_back_yesno.bmp");

	m_YesBtn.Init( 65, 120, ".\\common\\image\\vip\\vip_btn_yes.bmp", 4 );
	m_NoBtn.Init( 155, 120, ".\\common\\image\\vip\\vip_btn_no.bmp", 4 );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CVipCloseDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);

	NMBASE::SKINGDI::CMyBitmap Back;

	Back.LoadBitmapFile(".\\common\\image\\vip\\vip_back_yesno.bmp");	

	CBitmap *pOldBmp = cdc.SelectObject(&Back);

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);

	cdc.SelectObject(pOldBmp);
	Back.DeleteObject();
	
	// Do not call CUPGDlgBase::OnPaint() for painting messages
}
