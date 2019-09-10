#include "stdafx.h"
#include "AdminRemoveRoomDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAdminRemoveRoomDlg::CAdminRemoveRoomDlg(CWnd* pParent /*=NULL*/)
	: CUPGDlgBase(CAdminRemoveRoomDlg::IDD, pParent)
{
	m_strMsg = _T("");
	m_text = _T("");
}


void CAdminRemoveRoomDlg::DoDataExchange(CDataExchange* pDX)
{
	CUPGDlgBase::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO_REMOVEMSG, m_cmbMsg);
	DDX_CBString(pDX, IDC_COMBO_REMOVEMSG, m_strMsg);
	DDV_MaxChars(pDX, m_strMsg, 254);
	DDX_Text(pDX, IDC_EDIT_TEXT, m_text);
}


BEGIN_MESSAGE_MAP(CAdminRemoveRoomDlg, CUPGDlgBase)
	//{{AFX_MSG_MAP(CAdminRemoveRoomDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdminRemoveRoomDlg message handlers

BOOL CAdminRemoveRoomDlg::OnInitDialog() 
{
	CUPGDlgBase::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetWindowText("�����");
	m_text.Format(" [%d]���� ������ [%s]\r\n �� ���� ������ ���� �ұ��?\r\n\r\n �Ʒ� �����޼����� ���� Ȥ�� �ۼ��ϼ���.",m_RoomNo, m_strRoomTitle.GetString());


	m_cmbMsg.AddString("���� ���� �ǻ簡 �����Ƿ� �ڵ� ���� �˴ϴ�.");
	m_cmbMsg.AddString("�����ڿ����� ���� �����Ǿ����ϴ�.");
	
	

	//m_strMsg = "�����ڿ����� ���� �����Ǿ����ϴ�.";
	m_strMsg = "";
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CAdminRemoveRoomDlg::DoModal(int RoomNo, const char* szRoomTitle) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_RoomNo	= RoomNo;
	m_strRoomTitle = szRoomTitle;
	return CUPGDlgBase::DoModal();
}

void CAdminRemoveRoomDlg::OnOK() 
{
	// TODO: Add extra validation here

	UpdateData();	
	if(m_strMsg.IsEmpty()) {
		m_strMsg = "�����ڿ����� ���� �����Ǿ����ϴ�.";
		UpdateData(FALSE);
	}


	CUPGDlgBase::OnOK();
}
