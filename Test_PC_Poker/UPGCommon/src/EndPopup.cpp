#include "stdafx.h"
#include "EndPopup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PATH_IAMGEFOLDER_ENDPOPUP ".\\common\\image\\EndPopupDlg\\"

#define TIME_TO_CLOSE (1000*10)			//10초후에 닫힘



CEndPopup::CEndPopup(CWnd* pParent /*=NULL*/)
	: CUPGDlgBase(CEndPopup::IDD, pParent)
{
	m_pBrowser=NULL;
	m_pCustomHtml=NULL;
	m_llEarnedMoney=0;
	m_nPremNo = 0;
	m_dwStartTime=0;
	m_eEndpopupType=TYPE_ENDPOPUP_NO_EVENT;	
	m_nElapsedTime=0;

	EnableAutomation();
}

CEndPopup::~CEndPopup()
{
	
}


void CEndPopup::DoDataExchange(CDataExchange* pDX)
{
	CUPGDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEndPopup)	
	DDX_Control(pDX, IDC_BTN_EVENT1, m_btnEvent1);
	DDX_Control(pDX, IDC_BTN_EVENT2, m_btnEvent2);
	DDX_Control(pDX, IDC_BTN_EVENT3, m_btnEvent3);
	DDX_Control(pDX, IDOK, m_btnEndGame);		
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEndPopup, CUPGDlgBase)
	//{{AFX_MSG_MAP(CEndPopup)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_CLOSE()	
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_EVENT1, OnClickEvent1)
	ON_BN_CLICKED(IDC_BTN_EVENT2, OnClickEvent2)
	ON_BN_CLICKED(IDC_BTN_EVENT3, OnClickEvent3)
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEndPopup message handlers

BEGIN_DISPATCH_MAP(CEndPopup , CUPGDlgBase)
	/*
	외부 ActiveX 사용 할때
	DISP_FUNCTION(클래스명, 외부함수명, 맴버함수명, 리턴타입, 인자값1 인자값2) 
	참고 ) http://msdn.microsoft.com/ko-kr/library/cc451431(VS.71).aspx
	*/
 	DISP_FUNCTION( CEndPopup , "ClosePopup", ClosePopup, VT_EMPTY, VTS_NONE )
// 	DISP_FUNCTION( CEndPopup , "Cancel", Cancel, VT_EMPTY, VTS_NONE )
END_DISPATCH_MAP()

void CEndPopup::ClosePopup()
{
	OnOK();
}
// void CEndPopup::Cancel()
// {	
// 	OnCancel();
// }

BOOL CEndPopup::OnInitDialog() 
{
	CUPGDlgBase::OnInitDialog();
	
	// TODO: Add extra initialization here	
	
	CString strPathBack;
	m_eEndpopupType = (TYPE_ENDPOPUP) MM().Call(UPG_CMK_GET_ENDPOPUP_PROMOTION);
	
	strPathBack =  PATH_IAMGEFOLDER_ENDPOPUP "EndPopup.bmp";

	m_btnEndGame.Init( 861, 679, PATH_IAMGEFOLDER_ENDPOPUP "BtnEndGame.bmp", 4 );
	m_bitProgBar.LoadBitmapFile(PATH_IAMGEFOLDER_ENDPOPUP "bar.bmp");
	

	m_ptPromotionBanner = CPoint(12, 400);
	m_btnEvent1.ShowWindow(SW_HIDE);
	m_btnEvent2.ShowWindow(SW_HIDE);
	m_btnEvent3.ShowWindow(SW_HIDE);


	if(m_eEndpopupType != TYPE_ENDPOPUP_NO_EVENT)
	{
		if ( !MM().Call(UPG_CMK_INIT_ENDPOPUP_PROMOTION, (WPARAM)&m_ptPromotionBanner, (LPARAM)this))
		{
			OnOK();
			return FALSE;
		}		
	}

	if(!m_Back.LoadBitmapFile( strPathBack))
	{
		OnOK();
		return FALSE;
	}	

	m_Back.ResizeWnd( this, 0, 0, TRUE, m_pParentWnd );
	m_Rgn.SetRgn( m_Back, this, RGB(255, 0, 255), &m_Back );

	m_strMyNickname = GM().GetMyInfo()->UI.NickName;			//나의 닉네임
	
	
	SetURL(getGameURL(IDX_GAMEURL_ENDPOPUP));
	CreateBrowser();
		
	
	CenterWindow(GM().GetMainWnd());

	SetTimer(TIMER_ENDPOPUP_DRAW_PROGBAR, 40, NULL);
	m_dwStartTime = timeGetTime();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEndPopup::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	if( memDC.m_hDC == NULL ) return;
	
	memDC.SelectObject(&m_Back);

	
	float fElapsedTime = (float) m_nElapsedTime;
	if (m_nElapsedTime>TIME_TO_CLOSE)
	{
		fElapsedTime = (float) TIME_TO_CLOSE;
	}
	
	float fPercent = (fElapsedTime / (float)TIME_TO_CLOSE) ;
	
	int nTotalBarLength = m_bitProgBar.GetWidth();
	int nWidthBar = (int)((float)nTotalBarLength * fPercent);
	int nStartBar = nTotalBarLength - nWidthBar;

	m_bitProgBar.TransDraw(memDC.m_hDC, 57, 694, nWidthBar, 9,nStartBar, 0, 0);



	
	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &memDC, 0, 0, SRCCOPY);

	if(m_eEndpopupType!=TYPE_ENDPOPUP_NO_EVENT)
		MM().Call(UPG_CMK_PAINT_ENDPOPUP_PROMOTION, 0, (LPARAM)&dc);



	memDC.DeleteDC();

	// Do not call CUPGDlgBase::OnPaint() for painting messages
}

void CEndPopup::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	/*
	if( m_bEventRunning )
	{
		CRect rt;
		rt.SetRect(0,0,141,25);
		rt.OffsetRect(269,388);
		
		if(rt.PtInRect(point) )
		{				
			return;
		}
	}
	*/
// 
// 	CRect rect(20, 40, 20+408, 40+57); //상단배너영역	
// 	if(m_eEndpopupType==TYPE_ENDPOPUP_EVENT3 && rect.PtInRect(point))
// 	{
// 
// 		if (MM().Call(UPG_CMK_CLICK_ENDPOPUP_PROMOTION, IDC_BTN_EVENT1))
// 		{
// 			OnOK();
// 		}
// 		return;
// 	}
// 	PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, 0 );
	CUPGDlgBase::OnLButtonDown(nFlags, point);
}

void CEndPopup::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	//OnCancel();
	return;

	CUPGDlgBase::OnClose();
}

void CEndPopup::OnOK() 
{
	// TODO: Add extra validation here
	CUPGDlgBase::OnOK();
}

void CEndPopup::OnCancel() 
{
	// TODO: Add extra cleanup here
	return;
	CUPGDlgBase::OnCancel();
}


void CEndPopup::OnDestroy() 
{
	CUPGDlgBase::OnDestroy();

	// TODO: Add your message handler code here
	KillTimer(TIMER_ENDPOPUP_DRAW_PROGBAR);

	if( m_Back.m_hObject ) m_Back.DeleteObject();	
	if( m_bitProgBar.m_hObject ) m_bitProgBar.DeleteObject();		

	if(m_eEndpopupType!=TYPE_ENDPOPUP_NO_EVENT)
		MM().Call(UPG_CMK_DESTROY_ENDPOPUP_PROMOTION);

	DestroyBrowser();
}

void CEndPopup::OnTimer(UINT nIDEvent) 
{

	if (nIDEvent == TIMER_ENDPOPUP_DRAW_PROGBAR)
	{
		CRect rt(55, 692, 55+792, 692+12);
		InvalidateRect(&rt, FALSE);
		//Invalidate(FALSE);

		m_nElapsedTime = timeGetTime()-m_dwStartTime;

		if (m_nElapsedTime > TIME_TO_CLOSE)
		{
			KillTimer(TIMER_ENDPOPUP_DRAW_PROGBAR);
			OnOK();
		}
	}

	CWnd::OnTimer(nIDEvent);
}

void CEndPopup::DestroyBrowser()
{
	if ( m_pBrowser )
	{
		m_pBrowser->Destroy();
		delete m_pBrowser;
		m_pBrowser = NULL;
	}

}

BOOL CEndPopup::CreateBrowser()
{
	
	DestroyBrowser();

	
	// CreateWebPage
	//RECT rt = { 13, 102, 13+424, 102+297};
	RECT rt = { 514, 157, 514+421, 157+406};
	m_pBrowser = new NMBASE::UTIL::CNMSSOWebBrowser();
	if ( !m_pBrowser->Create( WS_VISIBLE | WS_CHILD, rt, GetSafeHwnd(), 52512, GetIDispatch(FALSE) ) ) 
	{
		if ( m_pBrowser )
		{
			delete m_pBrowser;
			m_pBrowser = NULL;
		}
		return FALSE;
	}
	m_pBrowser->Navigate( "about:blank" );
	m_pBrowser->SetHideBorder( TRUE );
	m_pBrowser->SetCallback_OnBeforeNavigate2( SSO_OnBeforeNavigate2 );
	m_pBrowser->Navigate(m_strURL);

	return TRUE;
}


void CEndPopup::SetURL(const char * pURL)
{
	m_strURL = pURL;
}


void CEndPopup::OnClickEvent1() 
{
	if((m_eEndpopupType!=TYPE_ENDPOPUP_NO_EVENT) && MM().Call(UPG_CMK_CLICK_ENDPOPUP_PROMOTION, IDC_BTN_EVENT1))
	{
		OnOK();
	}
}

void CEndPopup::OnClickEvent2() 
{
	if((m_eEndpopupType!=TYPE_ENDPOPUP_NO_EVENT) && MM().Call(UPG_CMK_CLICK_ENDPOPUP_PROMOTION, IDC_BTN_EVENT2))
	{
		OnOK();
	}
}

void CEndPopup::OnClickEvent3() 
{
	if((m_eEndpopupType!=TYPE_ENDPOPUP_NO_EVENT) && MM().Call(UPG_CMK_CLICK_ENDPOPUP_PROMOTION, IDC_BTN_EVENT3))
	{
		OnOK();
	}
}

void CEndPopup::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CUPGDlgBase::OnLButtonUp(nFlags, point);
}


INT_PTR CEndPopup::DoModal(INT64 llEarnedMoney)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_llEarnedMoney = llEarnedMoney;		//접속후 획득/감소 머니
	return CUPGDlgBase::DoModal();
}


BOOL CEndPopup::CreateCustomHtml()
{
	if (m_pCustomHtml)
		return FALSE;

	CRect rt;
	rt.SetRect(12, 32, 12+425, 32+54);


	CRuntimeClass *pRC = RUNTIME_CLASS(NMBASE::SKINGDI::CCustomHtml);
	m_pCustomHtml = (NMBASE::SKINGDI::CCustomHtml*) pRC->CreateObject();


	if (!m_pCustomHtml->Create(NULL, "CustomHtmlView", WS_CHILD|WS_VISIBLE, rt, this, 35932))
	{
		return FALSE;
	}



	CString strMoney;
	strMoney.Format("%c%s", m_llEarnedMoney>0?'+':' ', ((CString)NMBASE::UTIL::g_MoneyMark(m_llEarnedMoney)).GetString());


	CString strHtml, strBody;
	strBody.Format( "<center><b><font face='돋움' size='2' color='#6B6B6B'>"
		"<font color='#0039C9'>%s</font>님께서 이번 접속 시 획득한 금액은"
		"<br><font color='#BF0000'>%s</font> 입니다."
		"</b></font></center>",
		m_strMyNickname.GetString(), strMoney.GetString());	

	strHtml.Format("about:<body bgcolor='#D8DEE2' topmargin='17' oncontextmenu='return false' ondragstart='return false' onselectstart='return false' scroll=no style='{border:0}'>%s</body>", strBody.GetString());
	m_pCustomHtml->Navigate(strHtml);

	return TRUE;
}

void CEndPopup::DestoryCustomHtml()
{

	if ( m_pCustomHtml )
	{
		m_pCustomHtml->SendMessage(WM_DESTROY, 0,0);
		delete m_pCustomHtml;
		m_pCustomHtml = NULL;
	}
}

LRESULT CEndPopup::OnShowEndingURL(WPARAM &wParam, LPARAM &lParam)
{
	//string strURL = (char *)lParam;

	//NMBASE::UTIL::CShellMan sm;
	//sm.OpenNewBrowser(getGameURL());

	return TRUE;
}
