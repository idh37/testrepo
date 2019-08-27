#include "stdafx.h"
#include "MyVoteDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMyVoteDlg *g_pMyVoteDlg=NULL;

void CALLBACK VoteTimerProc(UINT uTimerID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2) 
{ 
	if (g_pMyVoteDlg==NULL)
		return;

	if(g_pMyVoteDlg->GetSafeHwnd()) 
	{		
		g_pMyVoteDlg->NowLeftTime--;
		g_pMyVoteDlg->m_TimeProgress.SetPosEx(g_pMyVoteDlg->NowLeftTime);
		if(g_pMyVoteDlg->NowLeftTime<=0)
		{
			g_pMyVoteDlg->PostMessage(WM_COMMAND,IDCANCEL);
			return;
		}	
	}
} 
 
CMyVoteDlg::CMyVoteDlg(CWnd* pParent /*=NULL*/)
	: CUPGDlgBase(CMyVoteDlg::IDD, pParent)
{
	hTimer = NULL;
	NowLeftTime = 15;
	m_BanishReson = _T("");
	m_NickName = _T("");
	g_pMyVoteDlg = this;
}

void CMyVoteDlg::DoDataExchange(CDataExchange* pDX)
{
	CUPGDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyVoteDlg)
	DDX_Control(pDX, IDOK, m_btnAssent);
	DDX_Control(pDX, IDCANCEL, m_btnConcept);
	DDX_Control(pDX, IDC_PROGRESS, m_TimeProgress);
	DDX_Text(pDX, IDC_EDIT_BANISHRESON, m_BanishReson);
		// 닉네임수정 	
	//DDX_Text(pDX, IDC_EDIT_TARGETID, m_TargetID);
	DDX_Text(pDX, IDC_EDIT_TARGETID, m_NickName);
// 여기까지
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyVoteDlg, CUPGDlgBase)
	//{{AFX_MSG_MAP(CMyVoteDlg)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyVoteDlg message handlers

BOOL CMyVoteDlg::OnInitDialog() 
{
	CUPGDlgBase::OnInitDialog();
	
	NMBASE::UTIL::CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);
	// TODO: Add extra initialization here

	Back.LoadBitmapFile(".\\common\\image\\vote\\voteback.BMP");		

	m_btnAssent.Init(63,218, ".\\common\\image\\vote\\btn_ok.BMP", 4);
	m_btnConcept.Init(164,218, ".\\common\\image\\vote\\btn_cancel.BMP", 4);

	UpdateData(FALSE);
	
	CRect trt;
	trt.SetRect(27,184,294,192);	
	m_TimeProgress.MoveWindow(&trt);
	m_TimeProgress.SetBkColor(RGB(45,45,45));
	m_TimeProgress.SetBarColor(RGB(172,172,172));

	//
	//hTimer = SetTimer(MYVOTE_TIMER1, 1000, NULL);
	hTimer = timeSetEvent((int)1000, 1000, VoteTimerProc, 0, TIME_PERIODIC|TIME_CALLBACK_FUNCTION); 
	NowLeftTime = 15;
	m_TimeProgress.SetRangeEx(15, NowLeftTime);
//	StartTime = GM().GetCurrentProcess()->m_nGameCount;

/*
	RECT rc1;	
	GetWindowRect( &rc1 );
	rc1.right = rc1.left + Back.GetWidth();
	rc1.bottom = rc1.top + Back.GetHeight();
	MoveWindow( &rc1 );
*/
	NMBASE::SKINGDI::WindowResize2Bitmap(m_hWnd, ".\\common\\image\\vote\\voteback.bmp");

	CRect rt;
	

	rt.SetRect(0,0,130,13);
	rt.OffsetRect(45,62);
	GetDlgItem(IDC_EDIT_TARGETID)->MoveWindow(&rt);
	
	rt.SetRect(0,0,212,32);
	rt.OffsetRect(83,109);
	GetDlgItem(IDC_EDIT_BANISHRESON)->MoveWindow(&rt);	

	SetFocus();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMyVoteDlg::OnOK() 
{
	// TODO: Add extra validation here
	/*삭제 : 다이얼로그 밖에서 처리
	CSV_MYBANISHVOTE mvmsg;
	mvmsg.Set(TRUE);
	NMBASE::SOCK::GSOCKMAN().SendData(g_MainSrvSID, mvmsg.pData, mvmsg.GetTotalSize());
	*/
	CUPGDlgBase::OnOK();
}

void CMyVoteDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	/*삭제 : 다이얼로그 밖에서 처리

	CSV_MYBANISHVOTE mvmsg;
	mvmsg.Set(FALSE);
	NMBASE::SOCK::GSOCKMAN().SendData(g_MainSrvSID, mvmsg.pData, mvmsg.GetTotalSize());
	*/

//	DestroyWindow();
//	return;
	CUPGDlgBase::OnCancel();
}

void CMyVoteDlg::OnDestroy() 
{
	CUPGDlgBase::OnDestroy();
	Back.DeleteObject();
	
	// TODO: Add your message handler code here
	if(hTimer) KillTimer(hTimer);
	hTimer = NULL;
}

void CMyVoteDlg::OnTimer(UINT nIDEvent) 
{

	CUPGDlgBase::OnTimer(nIDEvent);
}

BOOL CMyVoteDlg::DoModaless(char *ID,char *Reson)
{
	if(GetSafeHwnd()) 
		return FALSE;

	m_NickName = ID;
	m_BanishReson = Reson;

	BOOL rtn = Create(IDD,NULL);
	if(rtn) ShowWindow(SW_SHOW);

		return rtn;
}

void CMyVoteDlg::OnPaint() 
{
	
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) return;	

	HRGN h = NMBASE::SKINGDI::BitmapToRegion( Back.operator HBITMAP(), RGB(255,0,255));
	if(h) SetWindowRgn(h,TRUE);// 사용방법

	CBitmap *pOldBmp = cdc.SelectObject(&Back);

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);

	cdc.SelectObject(pOldBmp);
	if(h) 
	{
		::DeleteObject(h);	
	}
	cdc.DeleteDC();
	
	// Do not call CUPGDlgBase::OnPaint() for painting messages
}

HBRUSH CMyVoteDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CUPGDlgBase::OnCtlColor(pDC, pWnd, nCtlColor);
	
	UINT nID = pWnd->GetDlgCtrlID(); //스태틱텍스트박스 투명하게 하기박스
	if(IDC_EDIT_TARGETID == nID || IDC_EDIT_BANISHRESON == nID )
	{
		pDC->SetBkMode(TRANSPARENT);		
		//	pDC->SetBkColor(RGB(171, 182, 195));
			pDC->SetTextColor(RGB(196,208,223));
			return CreateSolidBrush(RGB(45,45,45));                		
	}
	
//	m_TimeProgress.OnCtlColor(pDC,pWnd,nCtlColor);
//	((*CProgressCtrl)GetDlgItem(IDC_PROGRESS))->OnCtlColor(pDC,pWnd,nCtlColor);


	
	return hbr;	
}
