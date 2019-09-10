// VIPChanceAlert.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "VIPChanceAlert.h"


// CVIPChanceAlert ��ȭ �����Դϴ�.

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


// CVIPChanceAlert �޽��� ó�����Դϴ�.

BOOL CVIPChanceAlert::OnInitDialog()
{
	CUPGDlgBase::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	CString strBGPath = ".\\Common\\image\\gameitem\\VIPChanceAlert.bmp";
	m_Back.LoadBitmapFile(strBGPath.GetString());

	NMBASE::SKINGDI::WindowResize2Bitmap(m_hWnd, strBGPath.GetString());


	m_OkBtn.Init(69,126,".\\common\\image\\commonbtn\\Btn_joinmembers.bmp", 4);
	m_CancelBtn.Init(159,126,".\\common\\image\\commonbtn\\Btn_GreyOk.bmp", 4);

	
	CenterWindow(m_pParentWnd);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CVIPChanceAlert::OnDestroy()
{
	CUPGDlgBase::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (m_Back.m_hObject)
		m_Back.DeleteObject();
}

void CVIPChanceAlert::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	// �׸��� �޽����� ���ؼ��� CUPGDlgBase::OnPaint()��(��) ȣ������ ���ʽÿ�.

	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) return;

	


	// 	HRGN h = NMBASE::SKINGDI::BitmapToRegion( Back.operator HBITMAP(), RGB(255,0,255));
	// 	if(h) SetWindowRgn(h,TRUE);// �����

	CBitmap *pOldBmp = cdc.SelectObject(&m_Back);

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);


}

void CVIPChanceAlert::OnOK()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	NMBASE::UTIL::CShellMan sm;
	sm.OpenNewBrowser("http://family.netmarble.net/poker/introduce/?members=gold");

	CUPGDlgBase::OnOK();
}

void CVIPChanceAlert::OnCancel()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	CUPGDlgBase::OnCancel();
}
