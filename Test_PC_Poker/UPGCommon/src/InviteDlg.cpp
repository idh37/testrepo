#include "stdafx.h"
#include "InviteDlg.h"
#include "WhisperDlg.h"
#include "MsgBoxDlg.h"
#include "GlobalBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CInviteDlg::CInviteDlg(CWnd* pParent /*=NULL*/)
	: CUPGDlgBase(CInviteDlg::IDD, pParent)
{
	m_pCustomHtml=NULL;
}

void CInviteDlg::DoDataExchange(CDataExchange* pDX)
{
	CUPGDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInviteDlg)
	DDX_Control(pDX, IDC_BUTTON_INVITE, m_InviteBtn);
	DDX_Control(pDX, IDCANCEL, m_CancelBtn);	
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CInviteDlg, CUPGDlgBase)
	//{{AFX_MSG_MAP(CInviteDlg)
	ON_BN_CLICKED(IDC_BUTTON_INVITE, OnButtonInvite)	
	ON_WM_PAINT()
	ON_WM_DESTROY()	
	ON_WM_LBUTTONDOWN()		
	//}}AFX_MSG_MAP
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInviteDlg message handlers

BOOL CInviteDlg::OnInitDialog() 
{
	CUPGDlgBase::OnInitDialog();
	
	// TODO: Add extra initialization here

	// 다이얼로그 컨트롤 포지션과 위치 재정렬
	NMBASE::UTIL::CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);

	// 사이즈 저장
	GetWindowRect(&OrgWindRect);


	Back.LoadBitmapFile(".\\common\\image\\Invite\\inviteback.bmp");	

	m_InviteBtn.Init( 104, 405, ".\\common\\image\\Invite\\Btn_Invite.bmp", 4 );
	m_CancelBtn.Init( 194, 405, ".\\common\\image\\Invite\\Btn_Cancel.bmp", 4 );

	//뷰생성
	CRect rect(11, 31, 11+355, 32+296 +65 );
	m_UserListView.Create(NULL, "InviteView", WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, rect, this, 12347);		


	CRect rc1;
	GetWindowRect( &rc1 );
	rc1.right = rc1.left + Back.GetWidth();
	rc1.bottom = rc1.top + Back.GetHeight();
	MoveWindow( &rc1 );

	return TRUE;
}

void CInviteDlg::OnCancel() 
{
	#ifdef UPGRADE_10210619_ADD_CLICKLOG
	if (m_UserListView.GetCurSelIndex() == 0)
		SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_BETAREA_INVITE_NOWGAME_CANCEL);
	else if (m_UserListView.GetCurSelIndex() == 1)
		SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_BETAREA_INVITE_ALLGAME_CANCEL);
	#endif //UPGRADE_10210619_ADD_CLICKLOG

	ShowWindow(SW_HIDE);
}


void CInviteDlg::OnButtonInvite() 
{
	#ifdef UPGRADE_10210619_ADD_CLICKLOG
		if (m_UserListView.GetCurSelIndex() == 0)
			SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_BETAREA_INVITE_NOWGAME_INVITE);
		else if (m_UserListView.GetCurSelIndex() == 1)
			SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_BETAREA_INVITE_ALLGAME_INVITE);
	#endif //UPGRADE_10210619_ADD_CLICKLOG

	map<string, UINT64> *pMapCheckedUser = m_UserListView.GetCheckedUser();
	if (pMapCheckedUser)
	{
		CSV_ASK_INVITE aimsg;
		SMALLROOMINFO smi;
		ZeroMemory(&smi, sizeof(smi));
		smi.llRoomKey = g_RI.llRoomKey; 
		smi.sChanNo = GM().GetCurrentChanNo();
		smi.sRoomNum = g_RI.RoomNum;
		smi.llSeedMoney = g_RI.BetMoney; 
		smi.nGameIndex = g_RI.nGameIndex;
		smi.sAutoBetKind = g_RI.nAutoBetKind;
		smi.cFormKind = g_RI.FormKind;
		strncpy(smi.szTitle, g_RI.Title, sizeof(smi.szTitle)-1);
	
		int nCount = 0;

		map<string, UINT64>::iterator it = pMapCheckedUser->begin();
		for(; it!=pMapCheckedUser->end(); it++, nCount++)
		{	
			CString strMsg=" ";			
			

			aimsg.Set(it->second, (char*)it->first.c_str(), &smi, &GM().GetMyInfo()->UI,strMsg);
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), aimsg.pData, aimsg.GetTotalSize());

			if (nCount>10)
				break;
		}
		pMapCheckedUser->clear();
	}

	ShowWindow(SW_HIDE);
}

void CInviteDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) return;
	
	CBitmap *pOldBmp;
	pOldBmp = cdc.SelectObject(&Back);

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);

	dc.SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11_BOLD));

	cdc.SelectObject(pOldBmp);
	cdc.DeleteDC();
}


void CInviteDlg::OnDestroy() 
{
	CUPGDlgBase::OnDestroy();
	
	// TODO: Add your message handler code here
	Back.DeleteObject();
	DestoryCustomHtml();
	m_pCustomHtml=NULL;
}

void CInviteDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your messagez handler code here and/or call default
	PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
	CUPGDlgBase::OnLButtonDown(nFlags, point);
}

void CInviteDlg::PopUp()
{
	if(!::IsWindow(m_hWnd))
	{
		Create(IDD_INVITE_DIALOG, NULL);
	}

	ShowWindow(SW_SHOW);
	CenterWindow(GM().GetMainWnd());

	m_UserListView.RefreshUserList();
}

void CInviteDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CUPGDlgBase::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (bShow)
	{	
		//초대 유저리스트 요청
		CCL_ASK_INVITEUSERLIST msg;
		msg.Set();
		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());	
		
		m_UserListView.SetFocus();
		m_UserListView.OnAskInviteUserList();
	}
	else
	{	
	}
}

BOOL CInviteDlg::CreateCustomHtml()
{
	if (m_pCustomHtml)
	{
		DestoryCustomHtml();
		m_pCustomHtml=NULL;
	}

	CRect rt;
	rt.SetRect(16, 330, 16+338, 330+61);

	CRuntimeClass *pRC = RUNTIME_CLASS(NMBASE::SKINGDI::CCustomHtml);
	m_pCustomHtml = (NMBASE::SKINGDI::CCustomHtml*) pRC->CreateObject();

	if (!m_pCustomHtml->Create(NULL, "CustomHtmlView", WS_CHILD|WS_VISIBLE, rt, this, 35933))
	{
		return FALSE;
	}

	CString strMoneyRange=g_RI.MoneyRange.GetStrMoneyRange(); 
	CString strWholeGames;
	
	for(int idxGame=0; idxGame<MAX_GAME; idxGame++)
	{
		if (idxGame>0)
		{
			strWholeGames += ", ";
		}

		CGame *pGame = GM().GetGame((IDX_GAME)idxGame);
		if (pGame)
		{
			strWholeGames += pGame->GetGameName();
		}
	}

	CString strHtml, strBody;
	strBody.Format( "<font face='돋움' color='#6B6B6B' style='font-size:11px'>"
		"<li>보유머니가 <b><font color='#9C1010'>%s</font></b> 사용자만 표시됩니다.<br>"
		"<li>게임초대 수신을 설정하지 않은 사용자는 초대하지 않습니다.<br>"
		"<li>전체게임: %s"
		"</font>",
		strMoneyRange.GetString(), strWholeGames.GetString());	

	strHtml.Format("about:<body bgcolor='#D8DEE2' topmargin='0' oncontextmenu='return false' ondragstart='return false' onselectstart='return false' scroll=no style='{border:0}'>%s</body>", strBody.GetString());
	m_pCustomHtml->Navigate(strHtml);

	return TRUE;
}

void CInviteDlg::DestoryCustomHtml()
{
	if ( m_pCustomHtml )
	{
		m_pCustomHtml->SendMessage(WM_DESTROY, 0,0);
		delete m_pCustomHtml;
		m_pCustomHtml = NULL;
	}
}

void CInviteDlg::RefreshUserList()
{
	m_UserListView.RefreshUserList();
}


void CInviteDlg::SetGameFilter(CDataUserList::KEY_FILTER KeyFilter)
{
	m_UserListView.SetGameFilter(KeyFilter);
}
