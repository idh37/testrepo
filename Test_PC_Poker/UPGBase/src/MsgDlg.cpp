// MsgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MsgDlg.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMsgDlg dialog


CMsgDlg::CMsgDlg(CWnd* pParent /*=NULL*/)
	: CUPGDlgBase(CMsgDlg::IDD, pParent)
{
	m_nMode = 0;
	m_nTimerWait = NULL;
	m_dbTick = 0.0f;
	//{{AFX_DATA_INIT(CMsgDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CUPGDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMsgDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMsgDlg, CUPGDlgBase)
	//{{AFX_MSG_MAP(CMsgDlg)
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsgDlg message handlers

void CMsgDlg::MsgBox(int nMode, char *pMsg)
{
	m_nMode = nMode;
	m_strMsg.Format(pMsg);
	Create(IDD_MSG_DIALOG, AfxGetMainWnd());
	ShowWindow(SW_SHOW);
}

BOOL CMsgDlg::OnInitDialog() 
{
	CUPGDlgBase::OnInitDialog();
	
	// TODO: Add extra initialization here
	//GetDlgItem(IDC_STATIC_MSG)->SetWindowText(m_strMsg);

	m_dbTick = 0.0f;
	if( m_nMode == 0 )
	{
		if(!m_sprBack.Load(".\\common\\data\\load.spr",555)){
			//AfxMessageBox("load.spr 파일을 찾을수 없습니다.");
			CMsgDlg::OnCancel();
		}

		int index = 4;
		if( index < 0 || index >= m_sprBack.GetTotalSpr() )
		{
			CMsgDlg::OnCancel();
		}

		int w = m_sprBack.spr[index].xl;
		int h = m_sprBack.spr[index].yl;
		
		m_cPage.Init(w, h, 16);

		m_ctrlCancel.Enable(FALSE);
		m_dwStartTime = timeGetTime();
		m_dwNowTime = timeGetTime();

		GetDlgItem(IDC_STATIC_MSG)->ShowWindow(SW_HIDE);

		LONG style = GetWindowLong( m_hWnd , GWL_EXSTYLE );
		long style2 = WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE;
		if((style & style2) !=0 )
		{
			int a =10;
		}
		style &= ~style2;
		SetWindowLong( m_hWnd , GWL_EXSTYLE , style);

		RECT rc1;
		GetWindowRect( &rc1 );
		rc1.right = rc1.left + w;
		rc1.bottom = rc1.top + h;
		MoveWindow( &rc1 );
		CenterWindow(this);

		if(m_nTimerAni!=NULL) KillTimer(m_nTimerAni);
		m_nTimerAni = SetTimer(MSGBOX_ANI_TIMER, 100, NULL); // 0.1초
	}
	else
	{
		GetDlgItem(IDC_STATIC_MSG)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_MSG)->SetWindowText(m_strMsg);
	}


	switch(m_nMode)
	{
	case 0:
		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		if(m_nTimerWait!=NULL) KillTimer(m_nTimerWait);
		m_nTimerWait = SetTimer(MSGBOX_WAIT_TIMER, 25000, NULL); // 25초
		break;
	case 1:
		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		if(m_nTimerWait!=NULL) KillTimer(m_nTimerWait);
		m_nTimerWait = SetTimer(MSGBOX_WAIT_TIMER, 25000, NULL); // 25초
		break;
	case 2:
		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		if(m_nTimerWait!=NULL) KillTimer(m_nTimerWait);
		m_nTimerWait = SetTimer(MSGBOX_WAIT_TIMER, 15000, NULL); // 15초
		break;
	case 3:
		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);
		break;
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CMsgDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
//	CDC* dc =  GetDC();
	Draw( &dc );
//	ReleaseDC( dc );
		// Do not call CUPGDlgBase::OnPaint() for painting messages
}

void CMsgDlg::Draw(CDC *pDC)
{
	

	if( m_nMode == 0 )
	{
		CDC cdc;
		cdc.CreateCompatibleDC(pDC);
		cdc.SelectObject(m_cPage.hBmp);

		cdc.SetBkMode(TRANSPARENT);
		cdc.SetTextColor(RGB(0,0,0));		

		if( m_dbTick < 0.00f )m_dbTick = 0.00f;
		else if( m_dbTick > 1 ){
	//		AfxMessageBox("sss");
			m_dbTick = 1;		
		}

		m_cPage.PutSprAuto(0,0,&m_sprBack,4);
	//	m_cPage.SetTempClip(13, 168, 
	//		13+(m_sprBack.spr[5].xl*m_dbTick), 168 + m_sprBack.spr[5].yl);

		m_cPage.SetTempClip(232, 538, 
			(int)(232+(350*m_dbTick)), 538 + 14);

		m_cPage.PutSprAuto(232, 538, &m_sprBack, 5, TEMPCLIP); 


//		m_cPage.DrawScene( 0, 0, &m_sprBack, 0, timeGetTime(), TRUE );
//		m_ctrlCancel.Draw(&cdc);

		CRect rt;
		GetClientRect(&rt);
		pDC->BitBlt(0,0,rt.Width(),rt.Height(),&cdc,0,0,SRCCOPY);
		cdc.DeleteDC();
		
	}
}


BOOL CMsgDlg::OnEraseBkgnd(CDC* pDC) 
{
// TODO: Add your message handler code here and/or call default

//	Draw(pDC);
	return 1;

	return CUPGDlgBase::OnEraseBkgnd(pDC);
}

void CMsgDlg::OnOK() 
{
	// TODO: Add extra validation here
	if(m_nTimerWait!=NULL) { KillTimer(m_nTimerWait); m_nTimerWait=NULL; }
	if(m_nTimerAni!=NULL) { KillTimer(m_nTimerAni); m_nTimerAni=NULL; }

	GM().SetWaitResponse(false);

	// 이미 대화창이 파괴된 경우라면 리턴
	if(m_hWnd==NULL) return;
	
	switch(m_nMode)
	{
	case 0:
		break;
	case 1:
		break;
	case 2:
		break;
	}

	DestroyWindow();
	m_hWnd = NULL;
	//CUPGDlgBase::OnOK();
}

void CMsgDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	if(m_nTimerWait!=NULL) { KillTimer(m_nTimerWait); m_nTimerWait=NULL; }
	if(m_nTimerAni!=NULL) { KillTimer(m_nTimerAni); m_nTimerAni=NULL; }

	GM().SetWaitResponse(false);
	
	// 이미 대화창이 파괴된 경우라면 리턴
	if(m_hWnd==NULL) return;

	switch(m_nMode)
	{
	case 0:
		ExitGameClient(false);
		return;
	case 1:
		SetAccessRoomState(false);
	}

	DestroyWindow();
	m_hWnd = NULL;
	//CUPGDlgBase::OnCancel();
}

void CMsgDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == MSGBOX_WAIT_TIMER)
	{
		if(m_nTimerWait!=NULL) { KillTimer(m_nTimerWait); m_nTimerWait=NULL; }
		GetDlgItem(IDCANCEL)->EnableWindow(TRUE);

		switch(m_nMode)
		{
		case 0:
		case 1:
			GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
			break;
		case 2:
			GM().SetNowLogout(true);
			ExitGameClient(false);
			return;
		}
	}

	
	if( nIDEvent == MSGBOX_ANI_TIMER )
	{
		m_dwNowTime = timeGetTime();
		Invalidate(FALSE);
	}
	
	CUPGDlgBase::OnTimer(nIDEvent);
}

BOOL CMsgDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	switch( wParam )
	{
		case IDM_BTN_LODINGCANCEL:
		{
			CUPGDlgBase::OnCancel();			
		}break;

	}
	return CUPGDlgBase::OnCommand(wParam, lParam);
}

void CMsgDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	

	if( m_ctrlCancel.OnLButtonDown(point.x, point.y) == FALSE ) 
	{
		PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
	}

	CUPGDlgBase::OnLButtonDown(nFlags, point);
}

void CMsgDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_ctrlCancel.OnMouseMove(point.x, point.y);
	
	CUPGDlgBase::OnMouseMove(nFlags, point);
}

void CMsgDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	m_ctrlCancel.OnLButtonUp(point.x, point.y);
	
	CUPGDlgBase::OnLButtonUp(nFlags, point);
}
