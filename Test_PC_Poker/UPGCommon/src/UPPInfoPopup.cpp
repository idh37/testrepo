#include "stdafx.h"
#include "UPPInfoPopup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PATH_IAMGEFOLDER_POPUP ".\\pokercommon\\image\\upp\\"

CUPPInfoPopup ::CUPPInfoPopup (CWnd* pParent /*=NULL*/)
	: CUPGDlgBase(CUPPInfoPopup ::IDD, pParent)
{
	m_ePopupType=TYPE_UPPPOPUP_GRADE1_STEP1;
	m_strRemainTime = "";
	m_strRemainRound = "";
	m_strUserID = "";
}

CUPPInfoPopup ::~CUPPInfoPopup ()
{
}

void CUPPInfoPopup ::DoDataExchange(CDataExchange* pDX)
{
	CUPGDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUPPInfoPopup )	
 	DDX_Control(pDX, IDOK, m_btnOK);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUPPInfoPopup , CUPGDlgBase)
	//{{AFX_MSG_MAP(CUPPInfoPopup )
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_CLOSE()	
	ON_WM_DESTROY()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUPPInfoPopup  message handlers


BOOL CUPPInfoPopup ::OnInitDialog() 
{
	CUPGDlgBase::OnInitDialog();
	
	// TODO: Add extra initialization here	

	

	CString strPathBack;

	switch(m_ePopupType)
	{
	case TYPE_UPPPOPUP_GRADE1_STEP1:
		strPathBack = PATH_IAMGEFOLDER_POPUP "1grade_1.bmp";
		m_rtID = CRect( 71, 111, 71 + 110, 111 + 13 );
		break;
	case TYPE_UPPPOPUP_GRADE1_STEP2:
		strPathBack = PATH_IAMGEFOLDER_POPUP "1grade_2.bmp";
		m_rtID = CRect( 67, 104, 67 + 109, 104 + 13 );
		break;
	case TYPE_UPPPOPUP_GRADE2:
		strPathBack = PATH_IAMGEFOLDER_POPUP "2grade.bmp";
		m_rtID = CRect( 62, 136, 62 + 108, 136 + 13 );
		break;
	case TYPE_UPPPOPUP_GRADE3_STEP1:
		strPathBack = PATH_IAMGEFOLDER_POPUP "3grade_1.bmp";
		m_rtID = CRect( 74, 183, 74 + 110, 183 + 13 );
		break;
	case TYPE_UPPPOPUP_GRADE3_STEP2:
		strPathBack = PATH_IAMGEFOLDER_POPUP "3grade_2.bmp";
		m_rtID = CRect( 81, 183, 81 + 110, 183 + 13);
		break;
	default:break;
	}
	m_rtRemainTime =  CRect(219, 249, 219 + 94, 249 + 15);
	m_rtRemainRound = CRect(219, 223, 219 + 94, 223 + 15);


	m_btnOK.Init( 177, 396, PATH_IAMGEFOLDER_POPUP "btn_confirm.bmp", 4 );
	m_btnOK.ShowWindow(SW_SHOW);
	m_Back.LoadBitmapFile(strPathBack);
	m_Back.ResizeWnd( this, 0, 0, TRUE, m_pParentWnd );

	m_strUserID.Format("%s",GM().GetMyInfo()->UI.ID);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUPPInfoPopup ::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	if( memDC.m_hDC == NULL ) return;
	
	memDC.SelectObject(&m_Back);

	memDC.SetBkMode(TRANSPARENT);
	memDC.SetTextColor(RGB(50,50,50));
	memDC.SelectObject(NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_14D_BOLD));

	memDC.DrawText(m_strUserID, &m_rtID, DT_VCENTER|DT_SINGLELINE|DT_CENTER);

	if ( m_ePopupType == TYPE_UPPPOPUP_GRADE1_STEP2 )
	{
		memDC.SetTextColor(RGB(20,90,160));
		memDC.SelectObject(NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_14D_BOLD));

		memDC.DrawText(m_strRemainTime, &m_rtRemainTime, DT_VCENTER|DT_SINGLELINE|DT_LEFT);
		memDC.DrawText(m_strRemainRound, &m_rtRemainRound, DT_VCENTER|DT_SINGLELINE|DT_LEFT);
	}


// 	if (m_eEndpopupType!=TYPE_ENDPOPUP_EVENT2)
// 	{
// 
// 	
// 
// 		CString strText;
// 		strText.Format("%s님께서 이번 접속 시 획득한 금액은", m_strMyNickname.GetString() );
// 
// 		memDC.SetBkMode(TRANSPARENT);
// 		memDC.SetTextColor(RGB(255,255,255));
// 		memDC.SelectObject(NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_12D_BOLD));
// 		memDC.TextOut(131, 51, strText.GetString());
// 
// 
// 		strText.Format("입니다.");	
// 		memDC.TextOut(301, 71, strText.GetString());
// 
// 		
// 	//	m_llEarnedMoney = -329540402958593;
// 
// 		//머니
// 		strText.Format("%c%s", m_llEarnedMoney>0?'+':' ', ((CString)NMBASE::UTIL::g_MakeShortMoneyStrEx(m_llEarnedMoney)).GetString());
// 		
// 		CRect rt (119, 69, 119+177, 69+17);
// 		memDC.SelectObject(NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_14D_BOLD));
// 		memDC.SetTextColor(RGB(254,189,189));
// 		memDC.DrawText(strText, &rt, DT_VCENTER|DT_SINGLELINE|DT_RIGHT);
// 		
// 
// 
// 
// 		//멤버쉽 심볼 그리기
// 		if (m_bitPremSymbol.m_hObject)
// 		{
// 			m_bitPremSymbol.TransDraw(memDC.m_hDC, 38, 44, m_bitPremSymbol.GetWidth(), m_bitPremSymbol.GetHeight(), 0,0, RGB(255,0,255) );
// 		}
// 	}

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &memDC, 0, 0, SRCCOPY);

	memDC.DeleteDC();

	// Do not call CUPGDlgBase::OnPaint() for painting messages
}

BOOL CUPPInfoPopup::PreTranslateMessage( MSG* pMsg )
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

void CUPPInfoPopup ::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, 0 );
	CUPGDlgBase::OnLButtonDown(nFlags, point);
}

void CUPPInfoPopup ::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	OnCancel();

	CUPGDlgBase::OnClose();
}

void CUPPInfoPopup ::OnOK() 
{
	// TODO: Add extra validation here
	CUPGDlgBase::OnOK();
}

void CUPPInfoPopup ::OnCancel() 
{
	// TODO: Add extra cleanup here
	CUPGDlgBase::OnCancel();
}


void CUPPInfoPopup ::OnDestroy() 
{
	CUPGDlgBase::OnDestroy();

	// TODO: Add your message handler code here

	if( m_Back.m_hObject ) m_Back.DeleteObject();
}
void CUPPInfoPopup ::SetRemainTimeRound(int nRemainTimeSec, int nRemainRound)
{
	m_strRemainTime.Format("%d판", nRemainRound);
	int nHour = nRemainTimeSec / 3600;
	int nMin = ( nRemainTimeSec % 3600 ) / 60;
	m_strRemainRound.Format( "%02d시간 %02d분", nHour, nMin);	
}
void CUPPInfoPopup ::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CUPGDlgBase::OnLButtonUp(nFlags, point);
}


INT_PTR CUPPInfoPopup ::DoModal(TYPE_UPPPOPUP eType)
{
	m_ePopupType = eType;
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	return CUPGDlgBase::DoModal();
}