// LoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LoginDlg.h"
#include "GlobalBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg dialog


CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CUPGDlgBase(CLoginDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoginDlg)
	m_ID = _T("");
	m_Pass = _T("");
	m_ServerIP = _T("");
	m_ServerPort = 0;
	m_Group = 0;
	//}}AFX_DATA_INIT
}


void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CUPGDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoginDlg)
	DDX_Text(pDX, IDC_EDIT_ID, m_ID);
	DDV_MaxChars(pDX, m_ID, 15);
	DDX_Text(pDX, IDC_EDIT_PASS, m_Pass);
	DDV_MaxChars(pDX, m_Pass, 12);				// [SSO 작업] - 수정
	DDX_Text(pDX, IDC_EDIT_SERVERIP, m_ServerIP);
	DDV_MaxChars(pDX, m_ServerIP, 20);
	DDX_Text(pDX, IDC_EDIT_SERVERPORT, m_ServerPort);
	DDX_Text(pDX, IDC_EDIT_GROUP, m_Group);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLoginDlg, CUPGDlgBase)
	//{{AFX_MSG_MAP(CLoginDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(ID_Sel_7Poker, &CLoginDlg::OnBnClickedSel7poker)
	ON_BN_CLICKED(ID_Sel_LowBD, &CLoginDlg::OnBnClickedSelLowbd)
	ON_BN_CLICKED(ID_Sel_NewPoker, &CLoginDlg::OnBnClickedSelNewpoker)
	ON_BN_CLICKED(ID_Sel_HighLow, &CLoginDlg::OnBnClickedSelHighlow)
	ON_BN_CLICKED(ID_Sel_Sut, &CLoginDlg::OnBnClickedSelSut)
	ON_BN_CLICKED(ID_Sel_Hoola, &CLoginDlg::OnBnClickedSelHoola)
	ON_BN_CLICKED(ID_Sel_DualPoker, &CLoginDlg::OnBnClickedSelDualpoker)
	ON_BN_CLICKED(ID_Sel_Tournament, &CLoginDlg::OnBnClickedSelTournament)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg message handlers

BOOL CLoginDlg::OnInitDialog() 
{
	CUPGDlgBase::OnInitDialog();
	
	// TODO: Add extra initialization here

// 	m_ServerIP =NMBASE::UTIL::GetProfile_string("debuginfo", "ip" ,m_ServerIP);
// 	m_ServerPort=NMBASE::UTIL::GetProfile_int("debuginfo", "port", m_ServerPort);
// 	m_Group=NMBASE::UTIL::GetProfile_int("debuginfo", "group", m_Group);
	
	UpdateData(FALSE);

	GetDlgItem(IDC_EDIT_ID)->SetFocus();
	//yoo
	::SetWindowPos ( this->GetSafeHwnd(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE );
	string sMap;

	NMBASE::UTIL::CShellMan ShellMan;
	int nCnt=ShellMan.EnumProcessCnt(NULL);
	NMBASE::SOCK::CSockMan::SetSocketTxt(nCnt);
	//로그인아이디 결정하기
	char PCName[200]={0,};
	DWORD BufSize = sizeof(PCName)-1;
	GetComputerName(PCName, &BufSize);		
	int nMyIDNO=0;		int i=0;
	while(PCName[i]!=0) nMyIDNO+= (PCName[i++]^0x55);	
	nMyIDNO = ( ((nMyIDNO) %100) *10 ) + nCnt +1; 		

	m_ID.Format("t_test%05d", nMyIDNO);		
	m_Pass = "a12a12";

	m_ID.Format("t_test%05d", nMyIDNO);		
	m_Pass = "a12a12";

	//qa용
	nMyIDNO = (nMyIDNO % 89) +10;	

	//게임개발팀 할당 받은 테스트 아이디 : qa_pk100~110 사용권장(qa_pk90~99까지는 사용금지)

	if(false==NMBASE::UTIL::CFileMapping::GetInstance()->MappingRead("PokerD.exe", sMap)){
		m_ID.Format("qa_pk%02d", nMyIDNO+4);
	}
	else{
		sscanf(sMap.c_str(), "qa_pk%02d", &nMyIDNO);
		m_ID.Format("qa_pk%02d", nMyIDNO+1);
	}
	m_Pass = "qa1234";

	//LoginDlg.m_ID.Format("clientdev_");		
	//LoginDlg.m_Pass = "a12a12";

	//LoginDlg.m_ID.Format("qwertyuiopas");		
	//LoginDlg.m_Pass = "a12a12a12";

	//QA존 서버
	//LoginDlg.m_ServerIP= "192.168.62.31";
	//LoginDlg.m_ServerIP= "192.168.62.78";
	//LoginDlg.m_ServerIP= "192.168.62.31";	//훌다 QA서버
	//LoginDlg.m_ServerPort= 24200;			//섯다 QA서버
	//LoginDlg.m_ServerIP= "192.168.62.31";	//하이로우 QA서버
	m_ServerIP= "183.110.61.192";
	m_ServerPort = 12000;

	//LoginDlg.m_ServerIP= "192.168.62.230"; // QA존 테스트 IP
	//LoginDlg.m_ServerIP= "192.168.62.232"; // 규제안 작업 서버
	//LoginDlg.m_ServerIP= "218.145.96.60"; // 재경님 로컬
	//LoginDlg.m_ServerIP= "218.145.96.201"; // 병인님 로컬

	//리얼서버 
	//LoginDlg.m_ServerIP= "220.73.221.235";
	//LoginDlg.m_ServerPort = 24200;

	//로컬서버
	//LoginDlg.m_ServerIP= "218.145.96.57";	//정중호님PC 접속
	//LoginDlg.m_ServerIP= "218.153.63.234";	//최연식님PC 접속		
	//LoginDlg.m_ServerIP= "218.145.96.103";	//박청룡님PC 접속
	//LoginDlg.m_ServerIP= "218.145.96.102";	//고영준님PC 접속
	//LoginDlg.m_ServerIP= "218.145.96.60";	//박재경님PC 접속

	m_Group = 110;
	UpdateData(FALSE);
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CLoginDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	if(m_ID.GetLength()==0) { AfxMessageBox("아이디를 입력하세요"); return; }
	if(m_Pass.GetLength()==0) { AfxMessageBox("비밀번호를 입력하세요"); return; }
	string sStr = NMBASE::UTIL::format("socket_%s.txt", m_ID.GetString());
	NMBASE::SOCK::CSockMan::SetSocketTxt(sStr);

	NMBASE::UTIL::WriteProfile_string("debuginfo", "id", m_ID);
	NMBASE::UTIL::WriteProfile_string("debuginfo", "pass", m_Pass);
	NMBASE::UTIL::WriteProfile_string("debuginfo", "ip", m_ServerIP);
	NMBASE::UTIL::WriteProfile_int("debuginfo", "port", m_ServerPort);
	NMBASE::UTIL::WriteProfile_int("debuginfo", "group", m_Group);


	NMBASE::UTIL::CFileMapping::GetInstance()->MappingWrite("PokerD.exe", m_ID.GetBuffer(m_ID.GetLength()));
	CUPGDlgBase::OnOK();
}

void CLoginDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	CUPGDlgBase::OnCancel();
}


BOOL CLoginDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->wParam==VK_RETURN && pMsg->message==WM_KEYDOWN)
	{	
		
		if ((GetKeyState(VK_CONTROL) & 0x8000 ) >0 )
		{
			m_ID=NMBASE::UTIL::GetProfile_string("debuginfo", "id", "");
			m_Pass=NMBASE::UTIL::GetProfile_string("debuginfo", "pass", "" );
			m_ServerIP =NMBASE::UTIL::GetProfile_string("debuginfo", "ip" ,"");
			m_ServerPort=NMBASE::UTIL::GetProfile_int("debuginfo", "port", 0);
			m_Group=NMBASE::UTIL::GetProfile_int("debuginfo", "group", 0);
			UpdateData(FALSE);
			CUPGDlgBase::OnOK();
			return FALSE;
			
		}	

	}
	return CUPGDlgBase::PreTranslateMessage(pMsg);
}

void CLoginDlg::OnBnClickedSel7poker()
{
	UpdateData(TRUE);
	m_Group = 110;
	UpdateData(FALSE);
	OnOK();
}

void CLoginDlg::OnBnClickedSelLowbd()
{
	UpdateData(TRUE);
	m_Group = 120;
	UpdateData(FALSE);
	OnOK();

}

void CLoginDlg::OnBnClickedSelNewpoker()
{
	UpdateData(TRUE);
	m_Group = 130;
	UpdateData(FALSE);
	OnOK();
}

void CLoginDlg::OnBnClickedSelHighlow()
{
	UpdateData(TRUE);
	m_Group = 150;
	UpdateData(FALSE);
	OnOK();
}

void CLoginDlg::OnBnClickedSelSut()
{
	UpdateData(TRUE);
	m_Group = 160;
	UpdateData(FALSE);
	OnOK();
}

void CLoginDlg::OnBnClickedSelHoola()
{
	UpdateData(TRUE);
	m_Group = 170;
	UpdateData(FALSE);
	OnOK();
}

void CLoginDlg::OnBnClickedSelDualpoker()
{
	UpdateData(TRUE);
	m_Group = 140;
	UpdateData(FALSE);
	OnOK();
}

void CLoginDlg::OnBnClickedSelTournament()
{
	UpdateData(TRUE);
	m_Group = 180;
	UpdateData(FALSE);
	OnOK();
}
