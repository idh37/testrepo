#include "stdafx.h"
#include "NewGameItemDlg.h"
#include "GlobalBase.h"
#include <afxinet.h>
#include "UniversalItemDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define RECT_SSOWEB (CRect( 11, 71, 11+506, 71+599 ))



CNewGameItemDlg::CNewGameItemDlg(CWnd* pParent /*=NULL*/)
	: CUPGDlgBase(CNewGameItemDlg::IDD, pParent)
{
	m_bModal = FALSE;
	m_nTab = 0;
	m_bRefreshList = FALSE;
	
	// [아이템샵 Tab 추가 수정]
	m_AvatarIndex = -1;
	m_bMyItemRefresh = FALSE;
	m_nDefaultItem = 0;
	EnableAutomation();

	m_dwStartTime = 0;
	
}

void CNewGameItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CUPGDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewGameItemDlg)
	DDX_Control(pDX, IDC_BUTTON_FAMILYAVATAR, m_FamilyAvatarBtn);
	DDX_Control(pDX, IDC_BUTTON_POKERAVATAR, m_PokerAvatarBtn);
	DDX_Control(pDX, IDC_BUTTON_LUCKAVATAR, m_LuckAvatarBtn);
	DDX_Control(pDX, IDC_BUTTON_POKERITEM, m_PokerItemBtn);
	DDX_Control(pDX, IDC_BUTTON_MYITEM, m_MyItemBtn);
	DDX_Control(pDX, IDC_BUTTON_CHARGECASH, m_ChargeCashBtn);
	DDX_Control(pDX, IDCANCEL, m_CloseBtn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewGameItemDlg, CUPGDlgBase)
	//{{AFX_MSG_MAP(CNewGameItemDlg)
	ON_BN_CLICKED(IDC_BUTTON_FAMILYAVATAR, OnButtonFamilyavatar)
	ON_BN_CLICKED(IDC_BUTTON_POKERAVATAR, OnButtonPokeravatar)
	ON_BN_CLICKED(IDC_BUTTON_LUCKAVATAR, OnButtonLuckavatar)
	ON_BN_CLICKED(IDC_BUTTON_POKERITEM, OnButtonPokeritem)
	ON_BN_CLICKED(IDC_BUTTON_MYITEM, OnButtonMyitem)
	ON_BN_CLICKED(IDC_BUTTON_CHARGECASH, OnButtonChargecash)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
// 	ON_BN_CLICKED(IDC_BUTTON_GETGAMEMONEY, OnButtonGetgamemoney)
// 	ON_BN_CLICKED(IDC_BUTTON_FUNCTIONALITEM, OnButtonFunctionalitem)
// 	ON_BN_CLICKED(IDC_BUTTON_FAMILY, OnButtonFamily)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
// 아이템 객체 대응 함수// 초이스 아바타
BEGIN_DISPATCH_MAP(CNewGameItemDlg, CUPGDlgBase)
/*
외부 ActiveX 사용 할때
DISP_FUNCTION(클래스명, 외부함수명, 맴버함수명, 리턴타입, 인자값1 인자값2) 
참고 ) http://msdn.microsoft.com/ko-kr/library/cc451431(VS.71).aspx
*/
DISP_FUNCTION( CNewGameItemDlg, "ShowItemPopup", ShowItemPopup, VT_EMPTY, VTS_I4 VTS_BSTR )
DISP_FUNCTION( CNewGameItemDlg, "UpdateMoney", UpdateMoney, VT_EMPTY, VTS_BOOL )
DISP_FUNCTION( CNewGameItemDlg, "ClosePopup", ClosePopup, VT_EMPTY, VTS_I2 )
DISP_FUNCTION( CNewGameItemDlg, "ChangeTab", ChangeTab, VT_EMPTY, VTS_I2 )
DISP_FUNCTION( CNewGameItemDlg, "OnCompleteUseItem", OnCompleteUseItem, VT_EMPTY, VTS_I4 )
END_DISPATCH_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewGameItemDlg message handlers

void CNewGameItemDlg::OnOK()
{

}

void CNewGameItemDlg::OnCancel() 
{
	// TODO: Add extra cleanup here

	if(GM().GetMainWnd()->GetSafeHwnd()) {
		GM().GetMainWnd()->SetFocus();
	}

	if ( !m_bModal ) {
		DestroyWindow();
		return;
	}
	
	CUPGDlgBase::OnCancel();
}

void CNewGameItemDlg::SetMyParent( CWnd* pWnd )
{
	m_pParentWnd = pWnd;
}


// void CNewGameItemDlg::OnButtonGetgamemoney() 
// {	
// 	// TODO: Add your control notification handler code here	
// 	m_FamilyBtn.EnableWindow( TRUE );
// 	m_GetGameMoneyBtn.EnableWindow( FALSE );
// 	m_FunctionalItemBtn.EnableWindow( TRUE );
// 	m_MyItemBtn.EnableWindow( TRUE );;
// 	m_ChargeCashBtn.EnableWindow( TRUE );
// 
// 	m_nTab = 0;
// 	ShowTab( 0 );
// }

// void CNewGameItemDlg::OnButtonFunctionalitem() 
// {	
// 	// TODO: Add your control notification handler code here
// 	m_FamilyBtn.EnableWindow( TRUE );
// 	m_GetGameMoneyBtn.EnableWindow( TRUE );
// 	m_FunctionalItemBtn.EnableWindow( FALSE );
// 	m_MyItemBtn.EnableWindow( TRUE );;
// 	m_ChargeCashBtn.EnableWindow( TRUE );
// 
// 	m_nTab = 1;
// 	ShowTab( 1 );
// }

// -> 아래 함수 추가
// int CNewGameItemDlg::BGDoModal( CString& sURL ) 
// {
// 	// TODO: Add your specialized code here and/or call the base class
// 	Reset();
// 
// 	m_bModal = TRUE;
// 
// 	m_nTab = 1;
// 	m_bRefreshList = FALSE;
// 	m_nDefaultItem = 0;
// 
// 	m_bBodyGuard = TRUE;
// 	m_BodyGuardURL = sURL;
// 
// 	return CUPGDlgBase::DoModal();
// }	



void CNewGameItemDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) return;
	CBitmap *pOldBmp = cdc.SelectObject(&Back);

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);

	cdc.SelectObject(pOldBmp);
	cdc.DeleteDC();		
	// Do not call CUPGDlgBase::OnPaint() for painting messages
}

BOOL CNewGameItemDlg::OnInitDialog() 
{
	CUPGDlgBase::OnInitDialog();
	
	// TODO: Add extra initialization here
	NMBASE::UTIL::CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);

	// 배경 그림 읽기
	Back.LoadBitmapFile(".\\common\\image\\GameItem\\GameItemBack.bmp");

	m_FamilyAvatarBtn.Init( 11, 31, ".\\common\\Image\\GameItem\\Btn_Familyavata.bmp", 4 );
	m_PokerAvatarBtn.Init( 115, 31, ".\\common\\image\\GameItem\\Btn_pokeravata.bmp", 4 );
	m_LuckAvatarBtn.Init( 181, 31, ".\\common\\image\\GameItem\\Btn_luckyavata.bmp", 4 );
	m_PokerItemBtn.Init( 215, 31, ".\\common\\image\\GameItem\\Btn_pokeritem.bmp", 4 );
	m_MyItemBtn.Init( 317, 31, ".\\common\\image\\GameItem\\Btn_myitem.bmp", 4 );
	m_ChargeCashBtn.Init( 418, 31, ".\\common\\image\\GameItem\\Btn_ChargeCash.bmp", 4 );
	m_CloseBtn.Init( 494, 4, ".\\common\\image\\commonbtn\\Btn_X.bmp", 4 );

	RECT rc1;
	GetWindowRect( &rc1 ); 
	rc1.right = rc1.left + Back.GetWidth();
	rc1.bottom = rc1.top + Back.GetHeight();
	MoveWindow( &rc1 );

	m_Rgn.SetRgn( HBITMAP(Back), this, RGB( 255, 0, 255 ), &Back );

	for ( int i = 0; i < DEF_MAX_URLs; i++ )
	{
		m_pWebGameItem[i] = NULL;
		CreateSSOWebGameItem( i );
	}

	m_LuckAvatarBtn.ShowWindow(SW_HIDE);

	
	ShowTab( m_nTab );
	CenterWindow();

	m_Rgn.SetRgn( Back, this, RGB(255, 0, 255), &Back );

	SetTimer(TIMER_NEWGAMEITEM_RESIZE,1000, NULL);

	ModifyStyle(0, WS_CLIPCHILDREN);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNewGameItemDlg::OnDestroy() 
{
	CUPGDlgBase::OnDestroy();
	
	// TODO: Add your message handler code here
	Back.DeleteObject();	

	for ( int i = 0; i < DEF_MAX_URLs; i++ )
	{
		if ( m_pWebGameItem[i] )
		{
			delete m_pWebGameItem[i];
			m_pWebGameItem[i] = NULL;
		}
	}

	KillTimer(TIMER_NEWGAMEITEM_RESIZE);
}

BOOL CNewGameItemDlg::SafeClose()
{
	if(!GetSafeHwnd()) return FALSE;
	PostMessage(WM_COMMAND, IDCANCEL);
	return TRUE;
}

void CNewGameItemDlg::CreateSSOWebGameItem( int nIndex ) 
{
	if ( nIndex < 0 || nIndex >= DEF_MAX_URLs )
	{
		return;
	}

	if ( m_pWebGameItem[nIndex] ) 
		return;

			

	if ( !m_pWebGameItem[nIndex] )
		m_pWebGameItem[nIndex] = new NMBASE::UTIL::CNMSSOWebBrowser();

	if ( !m_pWebGameItem[nIndex]->Create( WS_VISIBLE | WS_CHILD, RECT_SSOWEB, GetSafeHwnd(), 3321+nIndex, GetIDispatch(FALSE) ) )
	{
		if ( m_pWebGameItem[nIndex] )
		{
			delete m_pWebGameItem[nIndex];
			m_pWebGameItem[nIndex] = NULL;
		}

		return;
	}

	m_pWebGameItem[nIndex]->Navigate( "about:blank" );
	m_pWebGameItem[nIndex]->SetHideBorder( FALSE );
	m_pWebGameItem[nIndex]->SetCallback_OnQuit(WebClose);


	m_pWebGameItem[nIndex]->SetCallback_OnBeforeNavigate2( SSO_OnBeforeNavigate2 );	
		
	
	if ( !m_URLs[nIndex].IsEmpty() )
	{
		m_pWebGameItem[nIndex]->Navigate( m_URLs[nIndex] );
		m_pWebGameItem[nIndex]->ResetUserNavigatedCount();
	}
}


void CNewGameItemDlg::ItemShopNavigate( CString &URL, int nIndex, BOOL bRefresh )
{
	if ( nIndex < 0 || nIndex >= DEF_MAX_URLs )
	{
		return;
	}

	if ( !m_pWebGameItem[nIndex] ) 
	{
		CreateSSOWebGameItem( nIndex );
	}

	if ( m_pWebGameItem[nIndex] )
	{
		if ( /*m_URLs[nIndex] != URL ||*/ bRefresh /*|| ( nIndex == 2 && m_bMyItemRefresh )*/ )
		{
//			m_URLs[nIndex] = URL;
			if ( URL.GetLength() > 0 )
				m_pWebGameItem[nIndex]->Navigate( URL );
			else
				m_pWebGameItem[nIndex]->Navigate( m_URLs[nIndex] );
			m_pWebGameItem[nIndex]->ResetUserNavigatedCount();

			m_bMyItemRefresh = FALSE;
		}
	}			
}

void CNewGameItemDlg::EnableTabBtn(int i)
{
	
	m_FamilyAvatarBtn.EnableWindow(TRUE);
	m_PokerAvatarBtn.EnableWindow( TRUE );
	//m_LuckAvatarBtn.EnableWindow( TRUE );
	m_PokerItemBtn.EnableWindow( TRUE );
	m_MyItemBtn.EnableWindow( TRUE );
	m_ChargeCashBtn.EnableWindow( TRUE );
	
	
	
	switch ( i )
	{
	case 0:
		{
			m_FamilyAvatarBtn.EnableWindow( FALSE );
		}
		break;
		
	case 1:
		{
			m_PokerAvatarBtn.EnableWindow( FALSE );
		}
		break;
		
	case 2:
		{				
			//m_LuckAvatarBtn.EnableWindow( FALSE );		
		}
		break;
		
	case 3:
		{
			m_PokerItemBtn.EnableWindow( FALSE );
		}		
		break;
		
	case 4:
		{
			m_MyItemBtn.EnableWindow( FALSE );
		}
		break;
		
	case 5:
		{
			m_ChargeCashBtn.EnableWindow( FALSE );
		}
		break;
	}
	
}
void CNewGameItemDlg::ShowTab( int nIndex, BOOL bRefresh )
{
	if ( nIndex < 0 || nIndex >= DEF_MAX_URLs )
	{
		return;
	}

	if ( !m_pWebGameItem[nIndex] ) 
	{
		CreateSSOWebGameItem( nIndex );
	}	
	


	for ( int i = 0; i < DEF_MAX_URLs; i++ )
	{
		if ( m_pWebGameItem[i] )
		{
			m_pWebGameItem[i]->ShowWindow( SW_HIDE );
		}
	}

	if ( m_pWebGameItem[nIndex] )
	{
		m_pWebGameItem[nIndex]->ShowWindow( SW_SHOW );
	}	

	EnableTabBtn(nIndex);


	// [아이템샵 Tab 추가 수정]
	CString sURL = _T("");
	
	// [아이템샵 Tab 추가 수정]
	if ( m_AvatarIndex == nIndex )
	{
		ItemShopNavigate( m_AvatarURL, nIndex, TRUE );
		m_AvatarIndex = -1;
	}
	else
	{
		ItemShopNavigate( sURL, nIndex, TRUE );
	}

}

void CNewGameItemDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
	
	CUPGDlgBase::OnLButtonDown(nFlags, point);
}


// [아이템구매창 쓰레드] 2005-12-27
BOOL CNewGameItemDlg::DoModaless( int nTab, BOOL bRefreshList, int iDefSelItemKind, BOOL bThread )
{
	if(GetSafeHwnd()) 
		return FALSE;

	Reset();

	m_bModal = FALSE;

	m_nTab = nTab;
	m_bRefreshList = bRefreshList;


// [아이템구매창 쓰레드] 2005-12-27
	BOOL rtn = FALSE;

	/*
	if(bThread == TRUE){
		rtn = Create(IDD,NULL)
		if(rtn) ShowWindow(SW_SHOW);
	}
	else {
		rtn = Create(IDD,GM().GetMainWnd());
		if(rtn) ShowWindow(SW_SHOW);
	}
	*/

	rtn = Create(IDD,GM().GetMainWnd());
		if(rtn) ShowWindow(SW_SHOW);

	if ( rtn && nTab >= 0 && nTab < DEF_MAX_URLs )
	{
		ShowTab( nTab );
	}

	return rtn;
}

int CNewGameItemDlg::DoModal( int nTab, BOOL bRefreshList, int iDefSelItemKind ) 
{
	// TODO: Add your specialized code here and/or call the base class
	Reset();

	m_bModal = TRUE;

	m_nTab = nTab;
	m_bRefreshList = bRefreshList;
	m_nDefaultItem = iDefSelItemKind;


	return CUPGDlgBase::DoModal();
}

int CNewGameItemDlg::DoModal( char* cookiedata, int ItemCode, int GameCode, SYSTEMTIME ItemRecvTime ) 
{
	// TODO: Add your specialized code here and/or call the base class
	Reset();	

	m_bModal = TRUE;

	m_nTab = 0;
	m_bRefreshList = FALSE;
	m_nDefaultItem = 0;

	if( cookiedata ) m_CookieData.Format( "%s", cookiedata );
	
	m_ItemCode = ItemCode;
	m_GameCode = GameCode;
	memcpy( &m_RecvTime, &ItemRecvTime, sizeof(SYSTEMTIME) );

	return CUPGDlgBase::DoModal();
}

int CNewGameItemDlg::AvatarDoModal( const char *pURL,int Tab ) 
{
	// TODO: Add your specialized code here and/or call the base class
	Reset();

	m_bModal = TRUE;

	m_nTab = Tab;
	m_bRefreshList = FALSE;
	m_nDefaultItem = 0;

	// [아이템샵 Tab 추가 수정]
	m_AvatarIndex = Tab;	
	m_AvatarURL = pURL;

	return CUPGDlgBase::DoModal();
}

void CNewGameItemDlg::Refresh()
{
	ShowTab( m_nTab, TRUE );
}

void CNewGameItemDlg::Reset()
{
	m_bModal = FALSE;
	m_nTab = 0;
	m_bRefreshList = FALSE;
	// [아이템샵 Tab 추가 수정]
	m_AvatarIndex = -1;
	m_bMyItemRefresh = FALSE;

	SetURL();
}

// -> 아래 함수 교체
void CNewGameItemDlg::RefreshMyItem()
{
	ItemShopNavigate( CString(""), 4, TRUE );
}

// 초이스 아바타
void CNewGameItemDlg::ShowItemPopup( int nItemType, LPCTSTR url )
{ 
	static BOOL bProcessed = FALSE;
	
	if ( bProcessed == FALSE )
	{
		bProcessed = TRUE;
		
		switch ( nItemType )
		{
		case TYPE_CHOICEAVATAR:
		case TYPE_DICEAVATAR:			//주사위 아바타 (신규상품추가)
		case TYPE_CHALLENGE99AVATAR:	//도전99 아바타 (신규상품추가)
		case TYPE_CARIBBEANAVATAR:		//케리비언아바타 (신규상품추가)
		case TYPE_CARD_BINGO:			//행운의 카드빙고
		case TYPE_CARD_PINBAL:			//도전! 핀볼 : 2012.10.30 YS.Jeong
		case TYPE_CARD_PINBAL2:		//도전! 핀볼 : 2012.10.30 YS.Jeong
		case TYPE_BIGWHEEL:
		case TYPE_SCISSORS:
			{
				CUniversalItemDlg dlg;				
				dlg.DoModal( nItemType, CString(url) );  

				int nDefaultItem = dlg.GetDefaultItem();
				
				if ( nDefaultItem )
				{
					m_nTab = 2;
					m_nDefaultItem = 0;
					
					ShowTab( 2, TRUE );
				}
				else
				{
					SafeClose();
				}
			}
			break;
		}
		
		bProcessed = FALSE;
	}
}

void CNewGameItemDlg::OnButtonFamilyavatar() 
{
	// TODO: Add your control notification handler code here
	m_FamilyAvatarBtn.EnableWindow( FALSE );
	m_PokerAvatarBtn.EnableWindow( TRUE );
	//m_LuckAvatarBtn.EnableWindow( TRUE );
	m_PokerItemBtn.EnableWindow( TRUE );
	m_MyItemBtn.EnableWindow( TRUE );
	m_ChargeCashBtn.EnableWindow( TRUE );
	
	m_nTab = 0;
	ShowTab( 0 );
	
}

void CNewGameItemDlg::OnButtonPokeravatar() 
{
	// TODO: Add your control notification handler code here
	m_FamilyAvatarBtn.EnableWindow( TRUE );
	m_PokerAvatarBtn.EnableWindow( FALSE );
	//m_LuckAvatarBtn.EnableWindow( TRUE );
	m_PokerItemBtn.EnableWindow( TRUE );
	m_MyItemBtn.EnableWindow( TRUE );;
	m_ChargeCashBtn.EnableWindow( TRUE );
	
	m_nTab = 1;
	ShowTab( 1 );
	
}

void CNewGameItemDlg::OnButtonLuckavatar() 
{
	// TODO: Add your control notification handler code here
	m_FamilyAvatarBtn.EnableWindow( TRUE );
	m_PokerAvatarBtn.EnableWindow( TRUE );
	//m_LuckAvatarBtn.EnableWindow( FALSE );
	m_PokerItemBtn.EnableWindow( TRUE );
	m_MyItemBtn.EnableWindow( TRUE );
	m_ChargeCashBtn.EnableWindow( TRUE );
	
	m_nTab = 2;
	ShowTab( 2 );
	
}

void CNewGameItemDlg::OnButtonPokeritem() 
{
	// TODO: Add your control notification handler code here
	m_FamilyAvatarBtn.EnableWindow( TRUE );
	m_PokerAvatarBtn.EnableWindow( TRUE );
	//m_LuckAvatarBtn.EnableWindow( TRUE );
	m_PokerItemBtn.EnableWindow( FALSE );
	m_MyItemBtn.EnableWindow( TRUE );;
	m_ChargeCashBtn.EnableWindow( TRUE );
	
	m_nTab = 3;
	ShowTab( 3 );
	
}

void CNewGameItemDlg::OnButtonMyitem() 
{
	// TODO: Add your control notification handler code here
	m_FamilyAvatarBtn.EnableWindow( TRUE );
	m_PokerAvatarBtn.EnableWindow( TRUE );
	//m_LuckAvatarBtn.EnableWindow( TRUE );
	m_PokerItemBtn.EnableWindow( TRUE );
	m_MyItemBtn.EnableWindow( FALSE );;
	m_ChargeCashBtn.EnableWindow( TRUE );
	
	m_nTab = 4;
	ShowTab( 4 );
}

void CNewGameItemDlg::OnButtonChargecash() 
{	
	// TODO: Add your control notification handler code here
	m_FamilyAvatarBtn.EnableWindow( TRUE );
	m_PokerAvatarBtn.EnableWindow( TRUE );
	//m_LuckAvatarBtn.EnableWindow( TRUE );
	m_PokerItemBtn.EnableWindow( TRUE );
	m_MyItemBtn.EnableWindow( TRUE );;
	m_ChargeCashBtn.EnableWindow( FALSE );
	
	m_nTab = 5;
	ShowTab( 5 );
}

void CNewGameItemDlg::SetURL()
{
	m_URLs[0] = getGameURL(IDX_GAMEURL_FAMILYAVATARLIST);
	m_URLs[1] = getGameURL(IDX_GAMEURL_GAMEAVATARLIST);
	m_URLs[2] = getGameURL(IDX_GAMEURL_LOTTOAVATARLIST);
	m_URLs[3] = getGameURL(IDX_GAMEURL_GAMEITEMLIST);
	m_URLs[4] = getGameURL(IDX_GAMEURL_MYITEMLIST);
	m_URLs[5] = getGameURL(IDX_GAMEURL_REFILL);
}

void CNewGameItemDlg::UpdateMoney( BOOL bClosePopup )
{
	BOOL bCanSend = FALSE;
	if ( m_dwStartTime == 0 ) // 처음 호출
	{
		bCanSend = TRUE;
	}
	else
	{
		DWORD dwTime = timeGetTime() - m_dwStartTime;
		if ( dwTime >= 2000 ) // 10초가 지나야
		{
			bCanSend = TRUE;
		}
	}
	
	if ( bCanSend )
	{
		CSV_ASK_GAME_CONTINUE msg;
		msg.Set(GM().GetMyInfo()->UI.ID, 0);
		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());				
		
		if ( bClosePopup )
		{
			this->SendMessage( WM_CLOSE, 0, 0 );
		}
		
		m_dwStartTime = timeGetTime();
	}
}


void CNewGameItemDlg::ClosePopup( short wParam )
{
	//	CUPGDlgBase::OnCancel();
	if(!GetSafeHwnd()) return;
	PostMessage(WM_COMMAND, IDCANCEL);
}


void CNewGameItemDlg::ChangeTab( short wParam )
{
	//	CUPGDlgBase::OnCancel();
	
	EnableTabBtn(wParam);
}

void CNewGameItemDlg::OnCompleteUseItem(int nItemCode)
{
	//기능아이템을 사용하였을 때

	//실전 채널 입장권
	CSV_ASK_GAMEITEMLIST agmsg;
	agmsg.Set();
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), agmsg.pData, agmsg.GetTotalSize());

}

void CNewGameItemDlg::OnTimer(UINT nIDEvent)
{
	if (TIMER_NEWGAMEITEM_RESIZE == nIDEvent)
	{
		//IE6.0에서 캐쉬충전웹이 리사이징이되어 Timer로 사이즈를 복구시켜준다.
		if (m_pWebGameItem[INDEX_TAB_CASHCHARGE])
		{
			HWND hWnd = m_pWebGameItem[INDEX_TAB_CASHCHARGE]->GetHWND();

			if (hWnd && ::IsWindowVisible(hWnd))
			{				
				CRect rtCashWeb;
				::GetWindowRect(hWnd, &rtCashWeb);

				if (rtCashWeb.EqualRect(RECT_SSOWEB)==FALSE)
				{
					m_pWebGameItem[INDEX_TAB_CASHCHARGE]->MoveWindow(RECT_SSOWEB);
				}
			}
		}
	}
	
	CUPGDlgBase::OnTimer(nIDEvent);
}

void CNewGameItemDlg::WebClose()
{
	GM().PostMainWndMessage(UM_CLOSEALLPOPUP);	//팝업(상점, 찬스구매창)을 닫으라고 메인다이얼로그에 알린다.
}
