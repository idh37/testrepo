// VIPChanceAlert.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "VIPChanceAlert.h"


// CVIPChanceAlert 대화 상자입니다.

IMPLEMENT_DYNAMIC(CVIPChanceAlert, CUPGDlgBase)

CVIPChanceAlert::CVIPChanceAlert(CWnd* pParent /*=NULL*/)
	: CUPGDlgBase(CVIPChanceAlert::IDD, pParent)
{

}

CVIPChanceAlert::~CVIPChanceAlert()
{
}

void CVIPChanceAlert::DoDataExchange(CDataExchange* pDX)
{
	CUPGDlgBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDCANCEL, m_CancelBtn);
	DDX_Control(pDX, IDOK, m_OkBtn);
}


BEGIN_MESSAGE_MAP(CVIPChanceAlert, CDialog)
	ON_WM_DESTROY()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CVIPChanceAlert 메시지 처리기입니다.

BOOL CVIPChanceAlert::OnInitDialog()
{
	CUPGDlgBase::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CString strBGPath = ".\\Common\\image\\gameitem\\VIPChanceAlert.bmp";
	m_Back.LoadBitmapFile(strBGPath.GetString());

	NMBASE::SKINGDI::WindowResize2Bitmap(m_hWnd, strBGPath.GetString());


	m_OkBtn.Init(69,126,".\\common\\image\\commonbtn\\Btn_joinmembers.bmp", 4);
	m_CancelBtn.Init(159,126,".\\common\\image\\commonbtn\\Btn_GreyOk.bmp", 4);

	
	CenterWindow(m_pParentWnd);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CVIPChanceAlert::OnDestroy()
{
	CUPGDlgBase::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_Back.m_hObject)
		m_Back.DeleteObject();
}

void CVIPChanceAlert::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CUPGDlgBase::OnPaint()을(를) 호출하지 마십시오.

	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) return;

	


	// 	HRGN h = NMBASE::SKINGDI::BitmapToRegion( Back.operator HBITMAP(), RGB(255,0,255));
	// 	if(h) SetWindowRgn(h,TRUE);// 사용방법

	CBitmap *pOldBmp = cdc.SelectObject(&m_Back);

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);


}

void CVIPChanceAlert::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	NMBASE::UTIL::CShellMan sm;
	sm.OpenNewBrowser("http://family.netmarble.net/poker/introduce/?members=gold");

	CUPGDlgBase::OnOK();
}

void CVIPChanceAlert::OnCancel()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CUPGDlgBase::OnCancel();
}
