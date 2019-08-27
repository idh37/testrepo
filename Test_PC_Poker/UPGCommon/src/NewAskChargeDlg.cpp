// NewAskChargeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NewAskChargeDlg.h"
#include  "GlobalBase.h"
//#include "..\include\UPGCommon.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//CNewAskChargeDlg *CNewAskChargeDlg::m_pNewAskChargeDlg=NULL;

#define WIDTH_NEWASKCHARGE_DIALOG (450)
#define HEIGHT_NEWASKCHARGE_DIALOG (585)

/////////////////////////////////////////////////////////////////////////////
// CNewAskChargeDlg dialog


CNewAskChargeDlg::CNewAskChargeDlg(CWnd* pParent /*=NULL*/)
	: CUPGDlgBase(CNewAskChargeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewAskChargeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	EnableAutomation();


	m_pBrowser= NULL;
	m_dwStartTime = 0;

	m_nKind = 0;
}


void CNewAskChargeDlg::DoDataExchange(CDataExchange* pDX)
{
	CUPGDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewAskChargeDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_BUTTON_X, m_BtnX);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewAskChargeDlg, CUPGDlgBase)
	//{{AFX_MSG_MAP(CNewAskChargeDlg)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_X, &CNewAskChargeDlg::OnBnClickedButtonX)	
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CNewAskChargeDlg, CUPGDlgBase)
DISP_FUNCTION( CNewAskChargeDlg, "UpdateMoney", UpdateMoney, VT_EMPTY, VTS_BOOL )
DISP_FUNCTION( CNewAskChargeDlg, "ClosePopup", ClosePopup, VT_EMPTY, VTS_I2 )
DISP_FUNCTION( CNewAskChargeDlg, "OpenMarket", OpenMarket, VT_EMPTY, VTS_NONE )
DISP_FUNCTION( CNewAskChargeDlg, "JoinRequest", JoinInsuRequest, VT_EMPTY, VTS_NONE )
DISP_FUNCTION( CNewAskChargeDlg, "ShowItemPopup", ShowItemPopup, VT_EMPTY, VTS_I4 VTS_BSTR )
END_DISPATCH_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewAskChargeDlg message handlers

BOOL CNewAskChargeDlg::OnInitDialog() 
{
	CUPGDlgBase::OnInitDialog();

//	::SetWindowPos(m_hWnd,HWND_TOP,0,0,WIDTH_NEWASKCHARGE_DIALOG,HEIGHT_NEWASKCHARGE_DIALOG,SWP_NOZORDER);

	if ( m_Back.LoadBitmapFile( ".\\common\\image\\Popup\\ChargeBG.BMP" ) == FALSE )
	{
		return FALSE;
	}	

	m_BtnX.Init( 468, 6, ".\\common\\image\\commonbtn\\Btn_X.bmp", 4 );

	NMBASE::SKINGDI::WindowResize2Bitmap( m_hWnd, ".\\common\\image\\Popup\\ChargeBG.BMP" );
	
	CreateBrowser();

	CenterWindow(GM().GetMainWnd());

	m_Rgn.SetRgn( HBITMAP(m_Back), this, RGB( 255, 0, 255 ), &m_Back );

	return TRUE;
}


void CNewAskChargeDlg::OnDestroy() 
{	
	CUPGDlgBase::OnDestroy();
	// TODO: Add your message handler code here

	if (m_pBrowser)
	{
		m_pBrowser->Destroy();
		delete m_pBrowser;
		m_pBrowser  = NULL;
	}	

}

void CNewAskChargeDlg::DestroyBrowser()
{

}


void CNewAskChargeDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) return;
	
	cdc.SelectObject( &m_Back );
	
	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);
}

void CNewAskChargeDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	SendMessage( WM_NCLBUTTONDOWN, HTCAPTION, 0);
	CUPGDlgBase::OnLButtonDown(nFlags, point);
}


BOOL CNewAskChargeDlg::CreateBrowser()
{
	CString strURL;
	
	if ( m_strURL.GetLength() == 0 )
	{
		if (m_nKind == NEW_ASK_CHARGE_DLG_TYPE_FREE_CHARGE_INSU)	//등급보험 우선 무료충전
		{
			m_strURL = getGameURL(IDX_GAMEURL_FREECHARGEINSURANCE);
		}		
		else if (m_nKind == NEW_ASK_CHARGE_DLG_TYPE_INSU ) // 보험
		{
			m_strURL = getGameURL(IDX_GAMEURL_INSURANCE);
		}
		else if (m_nKind == NEW_ASK_CHARGE_DLG_TYPE_INSU_GOLD ) // 골드 보험
		{
			m_strURL = getGameURL(IDX_GAMEURL_INSURANCE_GOLD);
		}
		else if (m_nKind == NEW_ASK_CHARGE_DLG_TYPE_INSU_GOLD2 ) // 골드 보험 지급
		{
			m_strURL = getGameURL(IDX_GAMEURL_INSURANCE_GOLD2);
		}
		else if ( m_nKind == NEW_ASK_CHARGE_DLG_TYPE_FREE_CHARGE )
		{
			//-> STRING_GAME_INITIAL은 영문 게임명 없다면 Struct.h에 자기 게임명 디파인 #define STRING_GAME_INITIAL ("spoker")
			m_strURL = getGameURL(IDX_GAMEURL_FREECHARGE);
		}
		else
		{
			return FALSE;
		}
	}

	if (m_pBrowser)
	{
		m_pBrowser->Destroy();
		delete m_pBrowser;		
		m_pBrowser=NULL;
	}	

	

	CRect rect;
	rect.SetRect( 12, 32, 12+480, 32+600 );

	// 웹브라우저 생성 
	m_pBrowser  = new NMBASE::UTIL::CNMSSOWebBrowser;
	if ( m_pBrowser->Create( WS_VISIBLE | WS_CHILD, rect, GetSafeHwnd(), 32327, GetIDispatch(FALSE) ) == FALSE )
	{
		return FALSE;
	}
	m_pBrowser->SetHideBorder( TRUE );


	m_pBrowser->Navigate( m_strURL );
	

	return TRUE;
}

BOOL CNewAskChargeDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message==WM_KEYDOWN)
	{
		if (pMsg->wParam==VK_RETURN)
		{
			return TRUE;
		}
		else if (pMsg->wParam==VK_ESCAPE)
		{

			return TRUE;
		}
	}
	return CUPGDlgBase::PreTranslateMessage(pMsg);
}

void CNewAskChargeDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	//무료 충전 창이 종료 되면 게임 클라리언트를 종료 시킨다.


	CUPGDlgBase::OnClose();
}

void CNewAskChargeDlg::ClosePopup( short wParam )
{
//	CUPGDlgBase::OnCancel();
	if(!GetSafeHwnd()) return;
	SendMessage(WM_COMMAND, IDCANCEL);
}

void CNewAskChargeDlg::UpdateMoney( BOOL bClosePopup )
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

void CNewAskChargeDlg::OpenMarket()
{
	ShowGameItemDlg(false);
}

void CNewAskChargeDlg::JoinInsuRequest()
{
	// 기능 없어짐 삭제 
	/*
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
		CSV_AIN_INSURANCE_JOIN_REQUEST msg;
		AIN_INSURANCE_JOIN_REQUEST data;
		ZeroMemory(&data,sizeof(data));
		strcpy(data.ID,GM().GetMyInfo()->UI.ID);
		data.UNum = GM().GetMyInfo()->UI.UNum;
		data.nAIN_InsuranceRequest = 0;
		
		msg.Set(&data);
		NMBASE::SOCK::GSOCKMAN().SendData(g_MainSrvSID,msg.pData,msg.GetTotalSize());
		
		m_dwStartTime = timeGetTime();
	}

	*/
}

int CNewAskChargeDlg::DoModal( int nKind , const char *pStrURL) 
{
	// TODO: Add your specialized code here and/or call the base class

	if (GM().GetPlayDailyGoldEffect() == true || GM().GetPlayGoldBigWheel() == true)
	{
		GM().SetWaitGoldInsuPopup(true);

		return IDCANCEL;
	}

	m_nKind = nKind;

	if (pStrURL)
		m_strURL = pStrURL;
	else
		m_strURL.Empty();

	return CUPGDlgBase::DoModal();
}

void CNewAskChargeDlg::Navigate( CString& strURL )
{	
	if (GetSafeHwnd()==NULL)
		return;

	if (m_pBrowser)
	{
		m_pBrowser->Navigate(strURL);
	}
// 
// 	if ( m_pBrowser->GetHWND() )
// 	{
// 		m_Browser.Destroy();
// 		m_strURL = strURL;
// 		CreateBrowser();		
// 	}
}

void CNewAskChargeDlg::ShowItemPopup( int nTab, LPCTSTR sURL )
{ 
	
	MM().Call(UPG_CMK_AVATAR_SHOP, nTab, (LPARAM) sURL );

	if(!GetSafeHwnd()) return;
	SendMessage(WM_COMMAND, IDCANCEL);
}

void CNewAskChargeDlg::OnBnClickedButtonX()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CUPGDlgBase::OnCancel();
}
