#include "StdAfx.h"
#include "InGameBadUserComplainMsgBox.h"

IMPLEMENT_DYNAMIC(CInGameBadUserComplainMsgBox, CUPGDlgBase)

CInGameBadUserComplainMsgBox::CInGameBadUserComplainMsgBox(CWnd* pParent /*=NULL*/)
: CUPGDlgBase(CInGameBadUserComplainMsgBox::IDD, pParent)
{
	EnableAutomation();
}

CInGameBadUserComplainMsgBox::~CInGameBadUserComplainMsgBox(void)
{
}

void CInGameBadUserComplainMsgBox::DoDataExchange(CDataExchange* pDX)
{
	CUPGDlgBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_OKBtn);
	DDX_Control(pDX, IDCANCEL, m_CloseBtn);
	DDX_Control(pDX, IDC_BTN_OPEN, m_OpenBtn);
	
}

BEGIN_MESSAGE_MAP(CInGameBadUserComplainMsgBox, CUPGDlgBase)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDOK, &CInGameBadUserComplainMsgBox::OnBnClickedBtnOk)
	ON_BN_CLICKED(IDC_BTN_OPEN, &CInGameBadUserComplainMsgBox::OnBnClickedBtnOpen)
	ON_BN_CLICKED(IDOK, &CInGameBadUserComplainMsgBox::OnBnClickedBtnOk)
END_MESSAGE_MAP()

BOOL CInGameBadUserComplainMsgBox::OnInitDialog()
{
	CUPGDlgBase::OnInitDialog();

	m_BackBit.LoadBitmapFile(".\\PokerCommon\\image\\Popup_complain\\Popup_complain.bmp");
	m_OKBtn.Init( 70, 125, ".\\PokerCommon\\image\\Popup_complain\\m_OKBtn.bmp", 4 );
	m_OpenBtn.Init( 170, 125, ".\\PokerCommon\\image\\Popup_complain\\Btn_folder.bmp", 4 );
	m_CloseBtn.ShowWindow(FALSE);

	MoveWindow(0,0,m_BackBit.GetWidth() ,m_BackBit.GetHeight());
	m_BackBit.ResizeWnd(this, 0, 0, TRUE, GM().GetMainWnd());
	return TRUE;
}

void CInGameBadUserComplainMsgBox::OnPaint()
{
	CPaintDC dc(this);
	dc.SetBkMode(TRANSPARENT);

	m_BackBit.BitBlt( dc );
}


void CInGameBadUserComplainMsgBox::OnDestroy()
{
	CUPGDlgBase::OnDestroy();

	if (m_BackBit.m_hObject) m_BackBit.DeleteObject();
}

void CInGameBadUserComplainMsgBox::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
	CUPGDlgBase::OnLButtonDown(nFlags, point);
}

int CInGameBadUserComplainMsgBox::DoModal()
{
	return CUPGDlgBase::DoModal();
}

void CInGameBadUserComplainMsgBox::OnBnClickedBtnOk()
{
// 	CString fullpath;
// 	char lpszPath[MAX_PATH] = {0,};
// 	SHGetSpecialFolderPath(NULL, lpszPath, CSIDL_PERSONAL, FALSE);
// 	fullpath = lpszPath;
// 	fullpath += "\\";
// 	fullpath += m_strName;
// 	ShellExecute(NULL,  "open", "explorer", "/select," + fullpath, NULL, SW_SHOW);

	NMBASE::UTIL::CShellMan sm;
	sm.OpenNewBrowser(getGameURL(IDX_GAMEURL_BADUSER));
	CUPGDlgBase::EndDialog(IDOK);
}

void CInGameBadUserComplainMsgBox::OnBnClickedBtnOpen()
{
	CString fullpath;
	char lpszPath[MAX_PATH] = {0,};
	SHGetSpecialFolderPath(NULL, lpszPath, CSIDL_PERSONAL, FALSE);
	fullpath = lpszPath;
	fullpath += "\\";
	fullpath += m_strName;
	ShellExecute(NULL,  "open", "explorer", "/select," + fullpath, NULL, SW_SHOW);
}

void CInGameBadUserComplainMsgBox::OnBnClickedBtnCancel()
{

}
