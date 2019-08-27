#include "stdafx.h"
#include "JackpotTimepopup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PATH_IAMGEFOLDER_POPUP ".\\pokercommon\\image\\upp\\"


bool CJackpotTimepopup::gbOpen=false;
//규제안 : 최초 접속유저 안내 팝업
CJackpotTimepopup ::CJackpotTimepopup (CWnd* pParent /*=NULL*/)
	: CUPGDlgBase(CJackpotTimepopup ::IDD, pParent)
{

}

CJackpotTimepopup ::~CJackpotTimepopup ()
{
}

void CJackpotTimepopup ::DoDataExchange(CDataExchange* pDX)
{
	CUPGDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRestrictionPopup )	
 	DDX_Control(pDX, IDC_BUTTON_CLOSEX, m_btnOK);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CJackpotTimepopup , CUPGDlgBase)
	//{{AFX_MSG_MAP(CRestrictionPopup )
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_BUTTON_CLOSEX, OnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUPPInfoPopup  message handlers


BOOL CJackpotTimepopup ::OnInitDialog() 
{
	CUPGDlgBase::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString strPathBack;
	strPathBack = ".\\Common\\image\\Jackpot\\Jackpottimepopup.bmp";
	m_btnOK.Init( 882-34, 4, ".\\Common\\image\\commonbtn\\Btn_X.bmp", 4 );
	m_btnOK.ShowWindow(SW_SHOW);


	m_Back.LoadBitmapFile(strPathBack);
	m_Back.ResizeWnd( this, 0, 0, TRUE, m_pParentWnd );

	//if(GM().GetMyInfo()->UI.PremMenuNo == CODE_NEW_FAMILY_DIAMOND){

	//}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CJackpotTimepopup ::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	if( memDC.m_hDC == NULL ) return;
	
	memDC.SelectObject(&m_Back);
	memDC.SetBkMode(TRANSPARENT);

	//memDC.SetTextColor(RGB(50,50,50));
	//memDC.SelectObject(NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_14D_BOLD));

	//memDC.DrawText(m_strUserID, &m_rtID, DT_VCENTER|DT_SINGLELINE|DT_CENTER);

	//if ( m_ePopupType == TYPE_UPPPOPUP_GRADE1_STEP2 )
	//{
	//	memDC.SetTextColor(RGB(20,90,160));
	//	memDC.SelectObject(NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_14D_BOLD));

	//	memDC.DrawText(m_strRemainTime, &m_rtRemainTime, DT_VCENTER|DT_SINGLELINE|DT_LEFT);
	//	memDC.DrawText(m_strRemainRound, &m_rtRemainRound, DT_VCENTER|DT_SINGLELINE|DT_LEFT);
	//}

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &memDC, 0, 0, SRCCOPY);

	memDC.DeleteDC();

	// Do not call CUPGDlgBase::OnPaint() for painting messages
}

BOOL CJackpotTimepopup::PreTranslateMessage( MSG* pMsg )
{
	switch( pMsg->message )
	{
	case WM_SETCURSOR:// 메세지를 집어 삼킨다
		{
		} return TRUE;
	case WM_KEYDOWN:
		{
			switch(pMsg->wParam)
			{
			case VK_RETURN:
			case VK_ESCAPE:
				return TRUE;
			}			
		} break;
	}

	return CWnd::PreTranslateMessage( pMsg );
}

void CJackpotTimepopup::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, 0 );
	CUPGDlgBase::OnLButtonDown(nFlags, point);
}

void CJackpotTimepopup ::OnOK() 
{
	// TODO: Add extra validation here
	CUPGDlgBase::OnOK();
}

void CJackpotTimepopup::OnCancel() 
{
	// TODO: Add extra cleanup here

	CUPGDlgBase::OnCancel();
}

//void CJackpotTimepopup::OnBnClickedExplain()
//{
//	::ShellExecute(NULL,  "open", "http://game1.netmarble.net/event/2016/0322/index.asp", NULL, NULL, SW_SHOWNORMAL);
//	//::ShellExecute(NULL,  "open", "http://game1.netmarble.net/event/2014/02/gopo/", NULL, NULL, SW_SHOWNORMAL);
//}

void CJackpotTimepopup::OnDestroy() 
{
	gbOpen=false;
	CUPGDlgBase::OnDestroy();

	// TODO: Add your message handler code here

	if( m_Back.m_hObject ) m_Back.DeleteObject();
}

void CJackpotTimepopup::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CUPGDlgBase::OnLButtonUp(nFlags, point);
}


INT_PTR CJackpotTimepopup::DoModal()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
//#if defined(_DEBUG)
//	return IDOK;
//#endif
#if !defined(_REGULATION)
	if(1 == NMBASE::UTIL::GetProfile_int(GM().GetMyInfo()->UI.ID, "restrictionPopup",-1))
	{
		return IDOK;
	}
#endif
	if(true==gbOpen){
		return IDOK;
	}
	char* pBuffer=NMBASE::UTIL::GetProfile_string(GM().GetMyInfo()->UI.ID, "JackpotTimepopupday","20000101000000");
	int nCnt=NMBASE::UTIL::GetProfile_int(GM().GetMyInfo()->UI.ID, "JackpotTimepopupcnt", 0);
	if(false==GM().CheckDiffTime(string(pBuffer))){
		if(3<=nCnt){
			return IDOK;
		}
		nCnt++;
	}
	else{
		nCnt=1;
	}
	NMBASE::UTIL::WriteProfile_int(GM().GetMyInfo()->UI.ID, "JackpotTimepopupcnt", nCnt);
	NMBASE::UTIL::WriteProfile_string(GM().GetMyInfo()->UI.ID, "JackpotTimepopupday", GM().GetCurrentTimeString().c_str());
	INT_PTR nReturn=CUPGDlgBase::DoModal();
	gbOpen=true;
	return nReturn;
}