// LoadingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LoadingDlg.h"
#include "GlobalBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CPoint	listLodingArrowPos[9] = 
{
	CPoint(599, 308),
	CPoint(612, 308),
	CPoint(625, 308),
	CPoint(638, 308),
	CPoint(651, 308),
	CPoint(764, 308),
	CPoint(777, 308),
	CPoint(790, 308),
	CPoint(803, 308),
};

DWORD WINAPI CLoadingDlg::stLoadThreadProc( LPVOID pArg )
{
	CLoadingDlg *pInst = (CLoadingDlg *)pArg;
	SetEvent(pInst->m_hCreate);

	while(pInst->m_bContinue)
	{
		pInst->m_loadman.LoadAllRes(pInst);
		Sleep(1);
	}

	SetEvent(pInst->m_hDelete);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CLoadingDlg dialog

void CLoadManager::LoadAllRes(CLoadingDlg *pDlg)
{		
	if (m_bComplete)
	{
		return;
	}

	m_Prog=0;
	int i=0;
	int Prog = 0;
	for( vector<stRESOURCE>::iterator iter = m_vecResList.begin(); iter != m_vecResList.end() ; iter++ )
	{
		NMBASE::GRAPHICGDI::xSprite *pSpr = (NMBASE::GRAPHICGDI::xSprite*)(*iter).pResource;

		if( pSpr->Load( (char *)(*iter).szPath.c_str(), 555) == FALSE )
		{
			CString strMsg;
			strMsg.Format("%s 파일을 읽을 수 없습니다.", PathFindFileName((char *)(*iter).szPath.c_str()));
			::MessageBox(NULL, strMsg, "경고", MB_OK);
		}

		i++;
		Prog =  (i*100) / GetResSize();			
		if (Prog>100) Prog=100;
		
		if (Prog != m_Prog)
		{
			m_Prog = Prog;
			pDlg->PostMessage(UM_CHANGE_LODING, m_Prog);
		}
	}

	m_bComplete= TRUE;
	m_vecResList.clear();

	pDlg->PostMessage(UM_END_LODING, NULL, NULL);
}


CLoadingDlg::CLoadingDlg(CWnd* pParent /*=NULL*/)
	: CUPGDlgBase(CLoadingDlg::IDD, pParent),
	m_hCreate(INVALID_HANDLE_VALUE),
	m_hThread(INVALID_HANDLE_VALUE),
	m_hDelete(INVALID_HANDLE_VALUE),
	m_dwThread(NULL),
	m_bContinue(false)
{
	m_dwMinShowTime=0;
	m_dwCreationTime=0;

	m_bFirstLoading = true;
	m_bReserveDestroy = false;
	m_nDestroyCount = 0;
}

void CLoadingDlg::DoDataExchange(CDataExchange* pDX)
{
	CUPGDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoadingDlg)	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLoadingDlg, CUPGDlgBase)
	//{{AFX_MSG_MAP(CLoadingDlg)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_MESSAGE(UM_START_LODING, OnStartLoading)
	ON_MESSAGE(UM_END_LODING, OnEndLoading)
	ON_MESSAGE(UM_CHANGE_LODING, OnChangeLoading)
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoadingDlg message handlers
BOOL CLoadingDlg::OnInitDialog() 
{
	CUPGDlgBase::OnInitDialog();
	// 다이얼로그 컨트롤 포지션과 위치 재정렬
	
	m_imgArrow.LoadBitmapFile(".\\common\\image\\Loading\\LoadingArrow.bmp");

	CString strLodingBack = GM().GetCurrentGameLoadingBackPath();

	m_imgBackBit.LoadBitmapFile(strLodingBack);
	m_imgPage.LoadBitmapFile(strLodingBack);

	RECT rc1;
	GetWindowRect( &rc1 );
	rc1.right = rc1.left + m_imgBackBit.GetWidth();
	rc1.bottom = rc1.top + m_imgBackBit.GetHeight();
	MoveWindow( &rc1 );

	CenterWindow();

	m_bRedarw = false;
	
	m_dwCreationTime  = timeGetTime();
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);
	m_bReserveClose = FALSE;

#ifndef _DEBUG
	SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
#endif
	PostMessage(UM_START_LODING);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLoadingDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

	Draw(&dc);		
	// Do not call CUPGDlgBase::OnPaint() for painting messages
	m_bRedarw = false;
}


void CLoadingDlg::OnOK() 
{
	// TODO: Add extra validation here
	if(m_hWnd==NULL) return;

	//닫을 시간이 아직 안됐으면 예약 한다. 
	if (m_dwMinShowTime > timeGetTime()-m_dwCreationTime)
	{
		m_bReserveClose=TRUE;
		SetTimer(TIMER_LODING_MSGBOX_SELFDESTROY, 100, NULL);
		return;
	}

	MM().Call(UPG_CMK_ONDESTROY_LOADINGDIALOG);
	DestroyWindow();
	m_hWnd = NULL;
}

void CLoadingDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	// 이미 대화창이 파괴된 경우라면 리턴
	bool bFirstLoading = m_bFirstLoading;
	m_bFirstLoading = false;
	if(m_hWnd==NULL) return;

	if(bFirstLoading)
	{
		GM().PostMainWndMessage(WM_COMMAND, IDCANCEL);

		DestroyWindow();
		m_hWnd = NULL;
	}
}

void CLoadingDlg::LoadAllRes(void)
{
	m_hCreate = CreateEvent(NULL,FALSE,FALSE,NULL);
	m_hDelete = CreateEvent(NULL,FALSE,FALSE,NULL);
	m_bContinue = true;
	m_hThread = CreateThread(NULL, 0, stLoadThreadProc,(LPVOID)this, 0, &m_dwThread );
	WaitForSingleObject(m_hCreate, INFINITE );
}

bool CLoadingDlg::IsCheckClose(void) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_bReserveClose && m_dwMinShowTime <= timeGetTime()-m_dwCreationTime)
	{
		m_bReserveDestroy = true;
		m_nDestroyCount = 0;
		SetWindowPos(NULL, 0,0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_HIDEWINDOW | SWP_NOZORDER);
		//MM().Call(UPG_CMK_ONDESTROY_LOADINGDIALOG);

		return true;
	}

	return false;
}

BOOL CLoadingDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CUPGDlgBase::OnCommand(wParam, lParam);
}

void CLoadingDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	


	CUPGDlgBase::OnLButtonDown(nFlags, point);
}

void CLoadingDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	
	CUPGDlgBase::OnMouseMove(nFlags, point);
}

void CLoadingDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CUPGDlgBase::OnLButtonUp(nFlags, point);
}

BOOL CLoadingDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return FALSE;
}


void CLoadingDlg::Draw(CDC *pDC)
{
	CDC pageDC;
	pageDC.CreateCompatibleDC(pDC);	
	pageDC.SelectObject(m_imgPage);

	CDC cdc;
	cdc.CreateCompatibleDC(pDC);	
	cdc.SelectObject(m_imgBackBit);

	//CDC cdcprog;
	//cdcprog.CreateCompatibleDC(pDC);
	//cdcprog.SelectObject(m_imgArrow);
	
	float fPercent = 0.0f;
	if (m_bBasedOnTime && m_dwMinShowTime>0)
	{	
		fPercent =  ((float)(timeGetTime() - m_dwCreationTime)) / ((float)m_dwMinShowTime) ;
		fPercent *= 100.0f;
	}
	else
	{
		fPercent = (float)m_loadman.m_Prog;
	}

	if (fPercent > 100.0f)  fPercent = 100.0f;

	int nCurBarWidth = -1;
	if(fPercent > 0.0f) nCurBarWidth = (int)(fPercent * 0.09f);

	CRect rt;
	GetClientRect(&rt);

	pageDC.BitBlt(0,0,rt.Width(),rt.Height(),&cdc,0,0,SRCCOPY);
	for(int i=0;i<nCurBarWidth;i++)
	{
		m_imgArrow.TransDraw(pageDC, listLodingArrowPos[i].x,listLodingArrowPos[i].y, RGB(255,0,255));
	}
	
	pDC->BitBlt(0,0,rt.Width(),rt.Height(),&pageDC,0,0,SRCCOPY);
}

void CLoadingDlg::OnDestroy() 
{
	KillTimer(TIMER_LODING_MSGBOX_SELFDESTROY);

	if(m_hCreate != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hCreate);
		m_hCreate = INVALID_HANDLE_VALUE;
	}

	if(m_bContinue)
	{
		m_bContinue = false;
		WaitForSingleObject( m_hDelete, INFINITE );
	}

	if(m_hDelete != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hDelete);
		m_hDelete = INVALID_HANDLE_VALUE;
	}

	if(m_hThread != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hThread);
		m_hThread = INVALID_HANDLE_VALUE;
	}

	CUPGDlgBase::OnDestroy();
	
	if (m_imgBackBit.m_hObject) m_imgBackBit.DeleteObject();
	if (m_imgArrow.m_hObject) m_imgArrow.DeleteObject();
	if (m_imgPage.m_hObject) m_imgPage.DeleteObject();
}

LRESULT CLoadingDlg::OnStartLoading(WPARAM wParam, LPARAM lParam)
{
	m_bReserveDestroy = false;
	m_nDestroyCount = 0;
	SetWindowPos(&CWnd::wndTop, 0,0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	GetParent()->SetWindowPos(NULL, 0,0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_HIDEWINDOW | SWP_NOZORDER);
	SetActiveWindow();
	LoadAllRes();
	return TRUE;
}

LRESULT CLoadingDlg::OnEndLoading(WPARAM wParam, LPARAM lParam)
{
	if(m_hCreate != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hCreate);
		m_hCreate = INVALID_HANDLE_VALUE;
	}

	if(m_bContinue)
	{
		m_bContinue = false;
		WaitForSingleObject( m_hDelete, INFINITE );
	}

	if(m_hDelete != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hDelete);
		m_hDelete = INVALID_HANDLE_VALUE;
	}

	if(m_hThread != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hThread);
		m_hThread = INVALID_HANDLE_VALUE;
	}

	MM().Call(UPG_CMK_SPRITE_LOADING_END);
	return TRUE;
}

LRESULT CLoadingDlg::OnChangeLoading(WPARAM wParam, LPARAM lParam)
{
	if(m_bRedarw) return TRUE;
	m_bRedarw = true;
	Invalidate(FALSE);
	return TRUE;
}

void CLoadingDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
	case TIMER_LODING_MSGBOX_SELFDESTROY:
		if(IsCheckClose())
		{
			KillTimer(TIMER_LODING_MSGBOX_SELFDESTROY);
			OnOK();
			return;
		}
		break;
	}
	CUPGDlgBase::OnTimer(nIDEvent);
}
