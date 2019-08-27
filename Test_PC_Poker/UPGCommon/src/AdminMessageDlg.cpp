#include "stdafx.h"
#include "AdminMessageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAdminMessageDlg::CAdminMessageDlg(CWnd* pParent /*=NULL*/)
	: CUPGDlgBase(CAdminMessageDlg::IDD, pParent)
{
	m_bShow  = FALSE;

	m_strMessChat = _T("");
	m_strMessID = _T("");
	m_nTime = -1;
}


void CAdminMessageDlg::DoDataExchange(CDataExchange* pDX)
{
	CUPGDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdminMessageDlg)
	DDX_Text(pDX, IDC_MES_CHAT, m_strMessChat);
	DDX_Text(pDX, IDC_MES_ID, m_strMessID);
	DDX_Radio(pDX, IDC_RADIO1, m_nTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdminMessageDlg, CUPGDlgBase)
	//{{AFX_MSG_MAP(CAdminMessageDlg)
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBtnExit)
	ON_BN_CLICKED(IDC_BTN_HIDE, OnBtnHide)
	ON_BN_CLICKED(IDC_BTN_SEND, OnBtnSend)
	ON_BN_CLICKED(IDC_BUTTON_WAR, OnButtonWar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdminMessageDlg message handlers

BOOL CAdminMessageDlg::OnInitDialog() 
{
	CUPGDlgBase::OnInitDialog();
	
	// TODO: Add extra initialization here
	CenterWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAdminMessageDlg::OnOK() 
{
}

void CAdminMessageDlg::OnCancel() 
{
	Hide();
}


void CAdminMessageDlg::OnChatSend() 
{
	UpdateData();

	if(m_strMessID==_T("")){
		GetDlgItem(IDC_MES_ID)->SetFocus();
		//m_ctrIDEdit.SetFocus();
		return;
	}

	if (m_strMessChat==_T("")){
		GetDlgItem(IDC_MES_CHAT)->SetFocus();
		//m_ctrChatEdit.SetFocus();
		return;
	}

	//%%%추후 수정
	/*
	SMALLUSERINFO	*pUI = NULL;
	pUI = g_pLobyDlg->m_UserListView.m_UserView.GetUserInfo((char*)m_strMessID.operator LPCTSTR());

	int DestUNum = -1;
	if(pUI == NULL)
	{
		if(DestUNum == -1)
		{
			CString str;
			
			if(g_Where==WH_LOBY)
			{
				str.Format("[%s]님은 현재 대기실에 없습니다. \n", m_strMessID.operator LPCTSTR() );
				AddLobbyChatViewEdit(str.GetString(), RGB(200,100,100));
			}
			if(g_Where==WH_GAME)
			{
				str.Format("[%s]님은 현재 방에 없습니다. \n", m_strMessID.operator LPCTSTR() );
				g_ChatWinMan->m_pChatting->AddText(&str, RGB(250,250,250));
				
			}
			return;
		}
	}

	DestUNum = pUI->UNum;

	if(DestUNum == -1)
	{
		CString str;
		
		if(g_Where==WH_LOBY)
		{
			str.Format("[%s]님은 현재 대기실에 없습니다. \n", m_strMessID.operator LPCTSTR() );
			AddLobbyChatViewEdit(str.GetString(), RGB(200,100,100));
		}
		if(g_Where==WH_GAME)
		{
			str.Format("[%s]님은 현재 방에 없습니다. \n", m_strMessID.operator LPCTSTR() );
			g_ChatWinMan->m_pChatting->AddText(&str, RGB(250,250,250));
			
		}
		return;
	}


	CNM_ADMIN_WHISPERCHAT wcmsg;
	wcmsg.Set(GM().GetMyInfo()->UI.ID, DestUNum, pUI->NickName, (char*)m_strMessChat.operator LPCTSTR());
	NMBASE::SOCK::GSOCKMAN().SendData(g_MainSrvSID, wcmsg.pData, wcmsg.GetTotalSize());

	CString str;
	str.Format("[%s]님에게 메세지를 보냈습니다.\n", m_strMessID.operator LPCTSTR() );
	if(g_Where==WH_LOBY)
	{
		AddLobbyChatViewEdit(str.GetString(), RGB(0,105,0));
	}
	if(g_Where==WH_GAME)
	{
		g_ChatWinMan->m_pChatting->AddText(&str, RGB(250,250,250));
		
	}
*/
	m_strMessChat = ""; //_T("");
	GetDlgItem(IDC_MES_CHAT)->SetFocus();
}

void CAdminMessageDlg::OnBtnExit() 
{
	// TODO: Add your control notification handler code here

	if (AfxMessageBox("해당 계정을 강제로 종료시키겠습니까?", MB_YESNO)!=IDYES)
		return;
	
	UpdateData();
/* //%%% 추후 수정
	SMALLUSERINFO	*pUI = NULL;
	pUI = g_pLobyDlg->m_UserListView.m_UserView.GetUserInfo((char*)m_strMessID.operator LPCTSTR());

	if(pUI == NULL){m_UNum  = -1;}
	else{m_UNum = pUI->UNum;}	

//	m_UNum = g_pLobyDlg->GetUserNum(m_strMessID);

	if(m_strMessID==_T("") || m_UNum <= 0 ){
		GetDlgItem(IDC_MES_ID)->SetFocus();
		return;
	}

	int nRes = AfxMessageBox(g_StrMan.Get(_T("ADMIN_DELUSER")),MB_YESNO);
	if(nRes==IDNO) return;


	ADMINMESSAGE AdminMsg={0,};
	AdminMsg.AdminUNum = GM().GetMyInfo()->UI.UNum;
	strncpy(AdminMsg.AdminID, GM().GetMyInfo()->UI.ID, 15);

	AdminMsg.TarUNum = m_UNum;
	strncpy(AdminMsg.TarID, m_strMessID, 15);

	CNM_ADMIN_OUTUSER outmsg;
	outmsg.Set(&AdminMsg);
	NMBASE::SOCK::GSOCKMAN().SendData(g_MainSrvSID, outmsg.pData, outmsg.GetTotalSize());

	m_strMessID==_T("");
	m_UNum = 0;
*/
}

void CAdminMessageDlg::OnBtnHide() 
{
	// TODO: Add your control notification handler code here
	Hide();
	
}

void CAdminMessageDlg::OnBtnSend() 
{
	// TODO: Add your control notification handler code here


	if (AfxMessageBox("메세지를 보내시겠습니까?", MB_YESNO)!=IDYES)
		return;

	OnChatSend();
	
}

// 보인다
void CAdminMessageDlg::Show(int unum, CString strID/*=_T("")*/)
{
	m_strMessChat = _T("");
	m_strMessID   = _T("");
	if(strID!=_T("")){
		m_strMessID = strID;
		GetDlgItem(IDC_MES_CHAT)->SetFocus();
		m_UNum = unum;
		//m_ctrChatEdit.SetFocus();
	}
	else{
		m_strMessID =_T("");
		m_UNum = 0;
		//m_ctrIDEdit.SetFocus();
		GetDlgItem(IDC_MES_ID)->SetFocus();
	}

	if(!IsShow()){
		ShowWindow(SW_SHOW);
		m_bShow = TRUE;
		CenterWindow(); 
	//	if(g_bFullScreen) CenterWindow(); // 풀스크린시 중앙에 위치
	}

	UpdateData(FALSE);
}


// 감춘다
void CAdminMessageDlg::Hide()
{
	ShowWindow(SW_HIDE);
	m_bShow = FALSE;
	m_strMessChat = _T("");
	m_strMessID   = _T("");
	m_UNum = 0;
	UpdateData(FALSE);
}

void CAdminMessageDlg::OnButtonWar() 
{
	// TODO: Add your control notification handler code here
	//%%% 추후 수정
/*
	if(GM().GetMyInfo()->UI.AdminGrade == 0) return;
	UpdateData();

	SMALLUSERINFO	*pUI = NULL;
	pUI = g_pLobyDlg->m_UserListView.m_UserView.GetUserInfo((char*)m_strMessID.operator LPCTSTR());

	if(pUI == NULL){m_UNum  = -1;}
	else{m_UNum = pUI->UNum;}

	//m_UNum = g_pLobyDlg->GetUserNum(m_strMessID);;
	if(m_strMessID==_T("") || m_UNum <= 0 ){
		GetDlgItem(IDC_MES_ID)->SetFocus();
		return;
	}

	int nRes = AfxMessageBox("사용하면 안되요 절대로~~~~ ",MB_YESNO);
	if(nRes==IDNO) return;


	ADMINMESSAGE AdminMsg={0,};
	AdminMsg.AdminUNum = GM().GetMyInfo()->UI.UNum;
	strncpy(AdminMsg.AdminID, GM().GetMyInfo()->UI.ID, 15);

	AdminMsg.TarUNum = m_UNum;
	strncpy(AdminMsg.TarID, m_strMessID, 15);

	CNM_SPECIALMESSAGE shokm;
	shokm.Set(&AdminMsg);
	NMBASE::SOCK::GSOCKMAN().SendData(g_MainSrvSID, shokm.pData, shokm.GetTotalSize());

	m_strMessID==_T("");
	m_UNum = 0;
	*/
	
}
