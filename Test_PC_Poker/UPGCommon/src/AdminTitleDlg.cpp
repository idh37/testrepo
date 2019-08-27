#include "stdafx.h"
#include "AdminTitleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAdminTitleDlg::CAdminTitleDlg(CString strName, CWnd* pParent /*=NULL*/)
	: CUPGDlgBase(CAdminTitleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAdminTitleDlg)
	m_strPrevTitle = _T("");
	m_strName = _T("");
	//}}AFX_DATA_INIT
}


void CAdminTitleDlg::DoDataExchange(CDataExchange* pDX)
{
	CUPGDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdminTitleDlg)
	DDX_Control(pDX, IDC_COMBO_NAME, m_ctrlTitle);
	DDX_Text(pDX, IDC_EDIT_ROOMNAME, m_strPrevTitle);
	DDX_CBString(pDX, IDC_COMBO_NAME, m_strName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdminTitleDlg, CUPGDlgBase)
	//{{AFX_MSG_MAP(CAdminTitleDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdminTitleDlg message handlers

BOOL CAdminTitleDlg::OnInitDialog() 
{
	CUPGDlgBase::OnInitDialog();
	

	
	// TODO: Add extra initialization here
	m_ctrlTitle.AddString("이기는자가 강한자이다");	
	//m_ctrlTitle.SelectString( 0, (LPCTSTR)g_StrMan.Get(_T("RNAME9")) );
	

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CAdminTitleDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CUPGDlgBase::OnCancel();
}

void CAdminTitleDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	if(m_strName.GetLength()==0 || m_strName.GetLength()>30){
		AfxMessageBox("이기는자가 강한자이다");
		m_ctrlTitle.SetFocus();
		return;
	}	
	
	CUPGDlgBase::OnOK();
}

