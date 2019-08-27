#include "stdafx.h"
#include "RecvInviteDlg.h"
#include "AskMoveGameDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CRecvInviteDlg::CRecvInviteDlg(CWnd* pParent /*=NULL*/)
	: CUPGDlgBase(CRecvInviteDlg::IDD, pParent)
{
}


void CRecvInviteDlg::DoDataExchange(CDataExchange* pDX)
{
	CUPGDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecvInviteDlg)
	DDX_Control(pDX, IDCANCEL, m_CancelBtn);
	DDX_Control(pDX, IDOK, m_OkBtn);
	DDX_Control(pDX, IDC_BTN_CHECK, m_NoInviteCheckBtn);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRecvInviteDlg, CUPGDlgBase)
	//{{AFX_MSG_MAP(CRecvInviteDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecvInviteDlg message handlers

BOOL CRecvInviteDlg::OnInitDialog() 
{
	CUPGDlgBase::OnInitDialog();
	
	// TODO: Add extra initialization here
	// 다이얼로그 컨트롤 포지션과 위치 재정렬
	NMBASE::UTIL::CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);
	

	// 배경 그림 읽기
	Back.LoadBitmapFile(".\\common\\image\\Invite\\RecvInvite.bmp");		

	m_OkBtn.Init(70,265,".\\common\\image\\Invite\\Btn_Accept.bmp", 4);
	m_CancelBtn.Init(160,265,".\\common\\image\\Invite\\Btn_Refuse.bmp", 4);
	m_NoInviteCheckBtn.Init(70, 244, ".\\common\\image\\Invite\\btn_check.BMP", ".\\common\\image\\Invite\\btn_uncheck.BMP", this);

	// 초대 알림 사운드
	//NMBASE::SOUND::g_Sound.PlayWav(SND40);
	MoveWindow(0,0,Back.GetWidth(), Back.GetHeight());
	CenterWindow(m_pParentWnd);

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRecvInviteDlg::OnOK() 
{
	// TODO: Add extra validation here
	NMBASE::SOUND::g_Sound.PlayWav(SND_BUTTON_CLICK_A);

	if (m_NoInviteCheckBtn.GetCheck())
	{
		if(CCommunityManager::Instance()->GetOption(COM_ECOT_INVITE) != COM_ECOPT_OFF) CCommunityManager::Instance()->SetOption(COM_ECOT_INVITE, COM_ECOPT_OFF);
		SaveGameConfig(GM().GetMyInfo()->UI.ID);		
	}

	if (RoomInfo.nGameIndex!=GM().GetCurrentGameType())
	{
		CAskMoveGameDlg dlg(this);
		if (dlg.DoModal((IDX_GAME)RoomInfo.nGameIndex) == IDCANCEL)
		{
			CUPGDlgBase::OnCancel();
			return;
		}
	}

	CUPGDlgBase::OnOK();
}

void CRecvInviteDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	NMBASE::SOUND::g_Sound.PlayWav(SND_BUTTON_CLICK_A);

	if (m_NoInviteCheckBtn.GetCheck())
	{
		if(CCommunityManager::Instance()->GetOption(COM_ECOT_INVITE) != COM_ECOPT_OFF) CCommunityManager::Instance()->SetOption(COM_ECOT_INVITE, COM_ECOPT_OFF);
		SaveGameConfig(GM().GetMyInfo()->UI.ID);		
	}


	CUPGDlgBase::OnCancel();
}


//int CRecvInviteDlg::DoModal(int tarunum, char *tarid, int roomnum, char *strmsg) 
int CRecvInviteDlg::DoModal( USERINFO *pAskedUI, SMALLROOMINFO* pSRI) 
{
	// TODO: Add your specialized code here and/or call the base class

	CopyMemory( &AskedUI, pAskedUI, sizeof( USERINFO ) );
	if( pSRI != NULL ){
		CopyMemory( &RoomInfo, pSRI, sizeof( SMALLROOMINFO ) );
	}			
	return CUPGDlgBase::DoModal();
}
int CRecvInviteDlg::DoModal(int tarunum, char *tarid, int roomnum, char *strmsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	AfxGetMainWnd()->SendMessageToDescendants(WM_CANCELMODE);

	return CUPGDlgBase::DoModal();
}

void CRecvInviteDlg::DrawBack(CDC &DC)
{

	CDC cdc;
	cdc.CreateCompatibleDC(&DC);
	if(cdc.m_hDC == NULL) return;

	CBitmap *pOldBmp = cdc.SelectObject(&Back);

	CRect rect;
	GetClientRect(&rect);
	DC.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);

	DC.SetBkMode(TRANSPARENT);
	DC.SetTextColor(COLOR_DEFAULT_POPUP);
	DC.SelectObject( FONT_DEFAULT );

	CString str;	
	rect.SetRect(0,0,59,15);	
	rect.OffsetRect(95,44);

	//게임위치
	DrawGameText(DC, (IDX_GAME)RoomInfo.nGameIndex, &rect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);

	//방번호
	str.Format("%d번방", RoomInfo.sRoomNum);
	DC.TextOut(157, 45, str);

	//방제목
	str.Format("%s", RoomInfo.szTitle);
	DC.TextOut(95, 70, str);

	//입장제한	
	DC.TextOut(95, 94, g_GetRoomName( GM().GetCurrentGameType(), &RoomInfo));

	//시드머니/ 베팅룰
	str.Format("%s / %s", NMBASE::UTIL::g_MakeShortMoneyStrEx(RoomInfo.llSeedMoney) , GetAutoBettingRuleName((IDX_GAME)RoomInfo.nGameIndex, RoomInfo.sAutoBetKind, (BETRULE_KIND)RoomInfo.sBetRuleKind, (RULETYPE)RoomInfo.cRuleType));
	DC.TextOut(95, 118, str);

	//초대자
	DC.SelectObject(NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_BOLD));
	DC.TextOut(94, 168, AskedUI.ID, strlen(AskedUI.ID));


}


void CRecvInviteDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	DrawBack(dc);

	// Do not call CUPGDlgBase::OnPaint() for painting messages	
}
