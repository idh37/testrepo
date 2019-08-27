#include "stdafx.h"
#include "BanishmentDlg.h"
#include "MsgBoxDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CBanishmentDlg::CBanishmentDlg(CWnd* pParent /*=NULL*/)
	: CUPGDlgBase(CBanishmentDlg::IDD, pParent)
{
	m_BanishmentID = _T("");
}


void CBanishmentDlg::DoDataExchange(CDataExchange* pDX)
{
	CUPGDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBanishmentDlg)
	DDX_Control(pDX, IDCANCEL, m_CloseBtn);
	DDX_Control(pDX, IDOK, m_BanismentBtn);
	DDX_Control(pDX, IDC_EDIT_WHO, m_BanismentEdit);
	DDX_Text(pDX,IDC_EDIT_WHO , m_BanishmentID);
	DDV_MaxChars(pDX, m_BanishmentID, 19);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBanishmentDlg, CUPGDlgBase)
	//{{AFX_MSG_MAP(CBanishmentDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBanishmentDlg message handlers

BOOL CBanishmentDlg::OnInitDialog() 
{
	CUPGDlgBase::OnInitDialog();
	
	// TODO: Add extra initialization here
	NMBASE::UTIL::CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);

	Back.LoadBitmapFile(".\\common\\image\\vote\\banishment.BMP");
	// 버튼 초기화
	m_BanismentBtn.Init( 69, 159, ".\\common\\image\\vote\\btn_banishment.BMP", 4 );
	m_CloseBtn.Init( 164, 159, ".\\common\\image\\commonbtn\\Btn_cancel.bmp", 4 );	

	CRect rect;
	rect.SetRect(0,0,134,18);
	rect.OffsetRect(60,58);
	GetDlgItem(IDC_EDIT_WHO)->MoveWindow( &rect );
	GetDlgItem(IDC_EDIT_WHO)->SetWindowText(m_BanishmentID);	

	m_BanismentEdit.OnCreateBrush(RGB(45,45,45));
	m_BanismentEdit.OnSetTextColor(RGB(195, 207, 221));		
	m_BanismentEdit.SetFont(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_13_BOLD));	

	//이미지 사이즈에맞게 변환dlg
	NMBASE::SKINGDI::WindowResize2Bitmap(m_hWnd, ".\\common\\image\\vote\\banishment.BMP");			

	CenterWindow(m_pParentWnd);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBanishmentDlg::OnPaint() 
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

	cdc.SetTextColor(RGB(195, 207, 221));

	CRect rect;		
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);

	dc.SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_13_BOLD));
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(195, 207, 221));
	rect.SetRect(0,0,30,21);
	rect.OffsetRect(163,119);	

	dc.DrawText(BanisCnt,rect,DT_CENTER | DT_WORDBREAK);

	cdc.SelectObject(pOldBmp);
	cdc.DeleteDC();
	
	// Do not call CUPGDlgBase::OnPaint() for painting messages
}

int CBanishmentDlg::DoModal(CString id) 
{
	// TODO: Add your specialized code here and/or call the base class
//	if( g_Where != WH_GAME )return IDCANCEL;
	if( strlen(id) == 0 )
	{
		CMsgBoxDlg msgdlg(this);
		msgdlg.DoModal(CMsgBoxDlg::EMBT_NOTICE, "추방 할 NickName 없습니다.");
		return IDCANCEL;
	}
	
	m_BanishmentID = id;
//	BanisCnt.Format("%d",GM().GetCurrentProcess()->m_BanishmentMan.GetBanishmentCount());	
	return CUPGDlgBase::DoModal();
}
