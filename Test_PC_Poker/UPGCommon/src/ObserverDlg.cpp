#include "stdafx.h"
#include "GlobalBase.h"
#include "ObserverDlg.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CObserverDlg::CObserverDlg(CWnd* pParent /*=NULL*/)
	: CUPGDlgBase(CObserverDlg::IDD, pParent)
{
	ViewMode = 0;
	nUserSel = -1;

	m_pInviteUserListBackSpr=m_pInviteUserListBtnSpr=NULL;
	m_pLevelSpr=m_pEtcSpr=NULL;
}

CObserverDlg::~CObserverDlg()
{
}


void CObserverDlg::DoDataExchange(CDataExchange* pDX)
{
	CUPGDlgBase::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BUTTON_KICKOUT, m_KickOutBtn);
	DDX_Control(pDX, IDC_BTN_CLOSE, m_CloseBtn);	
	DDX_Control(pDX, IDC_BUTTON_CHECK_ALL, m_CheckAll);
}


BEGIN_MESSAGE_MAP(CObserverDlg, CUPGDlgBase)
	//{{AFX_MSG_MAP(CObserverDlg)	
	ON_BN_CLICKED(IDC_BTN_PROFILE, OnBtnProfile)
	ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_KICKOUT, OnButtonKickOut)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObserverDlg message handlers


BOOL CObserverDlg::IsAbleToBanish()
{
	if (GM().GetMyInfo()==NULL)
		return FALSE;
	return  (strcmp( g_RI.ID ,GM().GetMyInfo()->UI.ID ) ==0 ); //내가 방장이면 추방이 가능하다.

}
void CObserverDlg::RefreshButton() 
{
	// [게임 아이템 작업]
	if( IsAbleToBanish() )
	{		
		//추방이가능하다.
		m_CloseBtn.SetPos(163, 307);
		m_KickOutBtn.ShowWindow(SW_SHOW);
		m_KickOutBtn.EnableWindow(TRUE);

		m_CheckAll.ShowWindow(SW_SHOW);
		m_CheckAll.SetCheck(FALSE);
		m_ObserverListView.SetCheckAll(FALSE);
	}
	else
	{
		m_CloseBtn.SetPos(117, 307);
		m_KickOutBtn.ShowWindow(SW_HIDE);
		m_KickOutBtn.EnableWindow(FALSE);
		m_CheckAll.ShowWindow(SW_HIDE);
		
	}

	m_ObserverListView.SetEnableBanish(IsAbleToBanish());
	
}
	

BOOL CObserverDlg::OnInitDialog() 
{
	CUPGDlgBase::OnInitDialog();
	
	// TODO: Add extra initialization here
	// 다이얼로그 컨트롤 포지션과 위치 재정렬
	NMBASE::UTIL::CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);	

	m_CloseBtn.Init(163,307,".\\common\\image\\CommonBtn\\Btn_close.bmp", 4);
	m_KickOutBtn.Init(73, 307, ".\\common\\image\\CommonBtn\\Btn_Banish.bmp", 4);	//추방
	m_CheckAll.Init(23, 37, ".\\common\\image\\Observer\\btn_check.bmp", ".\\common\\image\\Observer\\btn_uncheck.bmp", this);

	m_CheckSpr.Load(".\\common\\data\\Check.spr", 555);

	User_InitList();
	
	NMBASE::SKINGDI::WindowResize2Bitmap(m_hWnd,".\\common\\image\\Observer\\ObserverBack.bmp");	

	CenterWindow(GM().GetMainWnd());

	RefreshButton();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CObserverDlg::User_DelAll()
{
	m_ObserverListView.DelAllList();
}

 void CObserverDlg::OnBtnProfile() 
 {
 	// TODO: Add your control notification handler code here
 	//NMBASE::SOUND::g_Sound.PlayWav(SND_BUTTON_CLICK);
 	
 	OBSERVER_DATA *pDATA = m_ObserverListView.GetSelectIndex_strID();
 	if( pDATA == NULL )return;		
 	
 	CString szID;
 	szID = pDATA->strID;
 	if(szID.GetLength() == 0) return;	

	GM().SetWaitResponse(false);
 }

void CObserverDlg::OnBtnClose() 
{
	// TODO: Add your control notification handler code here
	//NMBASE::SOUND::g_Sound.PlayWav(SND_BUTTON_CLICK);

	#ifdef UPGRADE_10210619_ADD_CLICKLOG
		SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_BETAREA_OBSERVE_STATUS_CLOSE);
	#endif //UPGRADE_10210619_ADD_CLICKLOG

	PostMessage(WM_COMMAND, IDCANCEL, 0);
}

void CObserverDlg::OnOK() 
{
	// TODO: Add extra validation here
//	m_ObserverListView.DelAllList();
	
	//CUPGDlgBase::OnOK();
}

void CObserverDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
//	m_ObserverListView.DelAllList();
	ShowWindow(SW_HIDE);
//	CUPGDlgBase::OnCancel();
}

void CObserverDlg::SetSprite(NMBASE::GRAPHICGDI::xSprite *pBackSpr, NMBASE::GRAPHICGDI::xSprite *pButtonSpr , NMBASE::GRAPHICGDI::xSprite *pLevelSpr, NMBASE::GRAPHICGDI::xSprite *pEtcSpr)
{
	m_pInviteUserListBackSpr = pBackSpr;
	m_pInviteUserListBtnSpr = pButtonSpr;

	m_pLevelSpr = pLevelSpr;
	m_pEtcSpr = pEtcSpr;
}

void CObserverDlg::User_InitList()
{	

	if (m_ObserverListView.GetSafeHwnd())
	{
		return;
	}


	CRect rc;
	rc.SetRect( 0, 0, 292, 238);
	rc.OffsetRect( 12, 55);

	m_ObserverListView.Create( this, rc, 2141, 0 );
	m_ObserverListView.Init( NULL, NULL , NULL, &m_CheckSpr);	
}

void CObserverDlg::User_InsItem(int unum, char *id,char *nick, BOOL sex, int level, INT64 money, int seq)
{
	OBSERVER_DATA data;
	data.Clear();

	data.UNum = unum;
	data.Sex = sex;
	data.PMoney = money;
	data.Seq = seq;	

	strncpy(data.strID,id,sizeof(data.strID)-1);
	strncpy(data.strNickName,nick,sizeof(data.strNickName)-1);

	m_ObserverListView.InsertList(&data);

}

void CObserverDlg::User_SetItem(int unum, char *id,char *nick, BOOL sex, int level, INT64 money, int seq)
{	
	OBSERVER_DATA data;
	data.Clear();

	data.UNum = unum;
	data.Sex = sex;
	data.PMoney = money;
	data.Seq = seq;	
	
	strncpy(data.strID,id,sizeof(data.strID)-1);

	strncpy(data.strNickName,nick,sizeof(data.strNickName)-1);

	m_ObserverListView.InsertList(&data);	
}

void CObserverDlg::User_DelItem(int unum, char *id)
{
	m_ObserverListView.DelList(unum,id);
}

void CObserverDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) return;
	
	if(Back.m_hObject) Back.DeleteObject();
	Back.LoadBitmapFile(".\\common\\image\\Observer\\ObserverBack.bmp");

// 	if(GM().GetCurrentGameType() == IDX_GAME_NP)
// 	{
// 		HRGN h = NMBASE::SKINGDI::BitmapToRegion( Back.operator HBITMAP(), RGB(255,0,255));
// 		if(h) SetWindowRgn(h,TRUE);// 사용방법
// 	}

	CBitmap *pOldBmp = cdc.SelectObject(&Back);

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);


// 	dc.SetBkMode(TRANSPARENT);
// 	dc.SetTextColor(COLOR_DEFAULT_POPUP);
// 	dc.SelectObject(FONT_DEFAULT);
// 	CString str;
// 	str.Format("닉네임");
// 	dc.TextOut(59, 55, str);
// 
// 	str.Format("보유머니");
// 	dc.TextOut(165, 55, str);
// 
// 	str.Format("상태");
// 	dc.TextOut(245, 55, str);


	cdc.SelectObject(pOldBmp);
	cdc.DeleteDC();
	Back.DeleteObject();	
	
	// Do not call CUPGDlgBase::OnPaint() for painting messages
}

void CObserverDlg::OnDestroy() 
{
	CUPGDlgBase::OnDestroy();
	
	// TODO: Add your message handler code here
	Back.DeleteObject();
	
}

void CObserverDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
		
	CUPGDlgBase::OnLButtonDown(nFlags, point);
}

void CObserverDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CUPGDlgBase::OnShowWindow(bShow, nStatus);

	RefreshButton();	
}

void CObserverDlg::User_ResetReservation()
{
	m_ObserverListView.ResetReservation();
}

void CObserverDlg::User_SetItemReservation(int unum, char *id, int seq)
{
	m_ObserverListView.SetList(unum,id,seq+1);
}

void CObserverDlg::OnButtonKickOut() 
{
	// TODO: Add your control notification handler code here
	if( strcmp( g_RI.ID,GM().GetMyInfo()->UI.ID ) != 0 || g_bMyObserver )return;
	
// 	OBSERVER_DATA *pDATA = m_ObserverListView.GetSelectIndex_strID();
// 	if( pDATA == NULL )return;
	int nCountSelected = m_ObserverListView.GetSelectedUserCount();

	CMsgBoxDlg msgbox(this);
	CString strMsg;

	if (nCountSelected==0)
	{
		strMsg.Format("강제추방을 위해 선택된 유저가 없습니다.");	
		msgbox.DoModal(CMsgBoxDlg::EMBT_NOTICE, strMsg);
		return;
	}

	strMsg.Format("선택된 유저를 정말로 강제 추방하시겠습니까?");
	if( msgbox.DoModal(CMsgBoxDlg::EMBT_QUESTION, strMsg) == IDCANCEL )
	{
		return;
	}

	m_ObserverListView.KickOutSelectedUser();	//선택된 유저를 강제 추방 시킨다.
}
BOOL CObserverDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (wParam==IDC_BUTTON_CHECK_ALL)
	{
		m_ObserverListView.SetCheckAll(m_CheckAll.GetCheck());
	}

	return CUPGDlgBase::OnCommand(wParam, lParam);
}
