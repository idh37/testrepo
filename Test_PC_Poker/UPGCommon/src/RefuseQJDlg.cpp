#include "stdafx.h"
#include "RefuseQJDlg.h"
#include "GlobalBase.h"
#define PATH_IAMGEFOLDER_QUICKJOINER ".\\common\\image\\QuickJoiner\\"
#define QJ_MYNICKNAME (GM().GetMyInfo()->UI.NickName)		//내 닉네임

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CRefuseQJDlg::CRefuseQJDlg(CWnd* pParent /*=NULL*/)
	: CUPGDlgBase(CRefuseQJDlg::IDD, pParent)
{
}


void CRefuseQJDlg::DoDataExchange(CDataExchange* pDX)
{
	CUPGDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRefuseQJDlg)
	DDX_Control(pDX, IDOK, m_BtnReConfig);
	DDX_Control(pDX, IDCANCEL, m_BtnCreateRoom);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_BtnClose);
	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRefuseQJDlg, CUPGDlgBase)
	//{{AFX_MSG_MAP(CRefuseQJDlg)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CRefuseQJDlg::OnBnClickedButtonClose)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRefuseQJDlg message handlers

BOOL CRefuseQJDlg::OnInitDialog() 
{
	CUPGDlgBase::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_BtnReConfig.Init(76, 132, PATH_IAMGEFOLDER_QUICKJOINER "QJ_ReCfgBtn.bmp", 4);
	m_BtnCreateRoom.Init(154, 132, PATH_IAMGEFOLDER_QUICKJOINER "QJ_CreateBtn.bmp", 4);
	m_BtnClose.Init(280, 5, PATH_IAMGEFOLDER_QUICKJOINER "Btn_X.bmp", 4);

	m_bClose = false ;	

	if (!m_BackBit.LoadBitmapFile(PATH_IAMGEFOLDER_QUICKJOINER "QJ_BG2.bmp"))
	{
		return FALSE;
	}
	
	//m_Rgn.SetRgn( (HBITMAP)m_BackBit.GetSafeHandle(), this, RGB( 255, 0, 255 ), &m_BackBit);
	NMBASE::SKINGDI::WindowResize2Bitmap(m_hWnd, (HBITMAP)m_BackBit.GetSafeHandle());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRefuseQJDlg::OnDestroy() 
{
	CUPGDlgBase::OnDestroy();
	
	// TODO: Add your message handler code here
	if (m_BackBit.m_hObject) m_BackBit.DeleteObject();
}

void CRefuseQJDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) return;

	CBitmap *pOldBmp = cdc.SelectObject(&m_BackBit);

// 	cdc.SetBkMode(TRANSPARENT);
// 	cdc.SetBkColor(0);
// 	cdc.SelectObject(NMBASE::UTIL::GetMyFont(FONT_12_BOLD));
// 	RECT rt = { 143, 106, 243, 120};
// 	cdc.DrawText(QJ_MYNICKNAME, strlen(QJ_MYNICKNAME), &rt, DT_CENTER);

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);
}

void CRefuseQJDlg::OnBnClickedButtonClose()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bClose = true ;

	OnCancel() ;
}
