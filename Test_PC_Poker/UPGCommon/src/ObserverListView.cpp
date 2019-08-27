// ObserverListView.cpp: implementation of the CObserverListView class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ObserverListView.h"
#include "GlobalBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif





//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define IDM_UP			197
#define IDM_DOWN		196
#define IDM_THUMB		195

BEGIN_MESSAGE_MAP(CObserverListView, CWnd)
	//{{AFX_MSG_MAP(CObserverListView)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_TIMER()	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CObserverListView, CWnd)

CObserverListView::CObserverListView()
{
	m_hTimer = 0;
	m_pBackSpr = NULL;
	m_pButtonSpr = NULL;

	m_NowIndex = 0;
	m_MouseOverIndex = -1;	//	오버된 인덱스
	m_SelectIndex =-1;		//	선택된 인덱스
	m_TimeCnt = 0;	

	
	m_pLevelSpr=m_pEtcSpr=NULL;
	m_bEnableBanish=FALSE;

	ZeroMemory( m_pUserInfoList, sizeof( m_pUserInfoList ) );
}

CObserverListView::~CObserverListView()
{
	
}


/////////////////////////////////////////////////////////////////////////////
// CObserverListView drawing
void CObserverListView::Create( CWnd* pParentWnd, const RECT& rect, UINT nID, int UseMode )
{
	CWnd::Create( NULL, NULL, WS_CHILD | WS_VISIBLE, rect, pParentWnd, nID );
	ShowWindow(SW_SHOW);

	m_pParentWnd = pParentWnd;
	m_Page.Init( rect.right - rect.left, rect.bottom - rect.top, 16 );

	if( m_hTimer != 0 )		KillTimer( m_hTimer );
	m_hTimer = SetTimer( nID, 200, NULL );


}

void CObserverListView::Init( NMBASE::GRAPHICGDI::xSprite *pBackSpr, NMBASE::GRAPHICGDI::xSprite *pButtonSpr , NMBASE::GRAPHICGDI::xSprite *pLevelSpr, NMBASE::GRAPHICGDI::xSprite *pEtcSpr)
{
	m_pBackSpr = pBackSpr;
	m_pButtonSpr = pButtonSpr;

	m_pLevelSpr = pLevelSpr;
	m_pEtcSpr = pEtcSpr;

	m_NowIndex = 0;
	m_MouseOverIndex = -1;	//	오버된 인덱스
	m_SelectIndex = -1;		//	선택된 인덱스	



	if (m_pButtonSpr)
	{
		m_UpBtn.Init( this, &m_Page, 331, 12, m_pButtonSpr, 0, IDM_UP,3 );
		m_DownBtn.Init( this, &m_Page, 331, 152, m_pButtonSpr, 1, IDM_DOWN ,3);
		m_ThumbBtn.Init( this, &m_Page, 331, 28, m_pButtonSpr, 2, IDM_THUMB ,3);		
	}
	m_ScrollBaseRect.SetRect( 0, 0, 16, 121);	//	스크롤바 영역
	m_ScrollBaseRect.OffsetRect( 336, 30);
	
	//	체크할 위치 설정
	int nUnitHeight = m_Page.height / MAX_OBSERVERCHECK;
	nUnitHeight+=1;
	for( int i = 0; i < MAX_OBSERVERCHECK; i++ )
	{		
		m_UserCheckRect_Max[ i ].SetRect( 0, 0, m_Page.width, nUnitHeight);
		m_UserCheckRect_Max[ i ].OffsetRect(0,i*nUnitHeight);
	}	

	//	정렬하기 - 스클로바용으로
	ResetControlls();

	//	리스트 초기화
	DelAllList();
	return;
}


void CObserverListView::OnDestroy() 
{
	if( m_hTimer != 0 )		KillTimer( m_hTimer );
	//	리시트 지우기
	DelAllList();
	CWnd::OnDestroy();
}


LRESULT CObserverListView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch( message )
	{
		case WM_COMMAND:
		{
			switch( wParam )
			{
				case IDM_UP:
				{
					m_NowIndex--;
					if( m_NowIndex < 0 )		m_NowIndex = 0;
					m_MouseOverIndex = -1;	//	오버된 인덱스

					//	스클로바 텀브바 설정
					ResetControlls();

					return 1;
				}
				case IDM_DOWN:
				{
					//int totcnt = m_List.GetCount();
					int totcnt = m_deque_ObUser.size();					
					
					if( totcnt <= MAX_OBSERVERCHECK )		return 1;
					m_NowIndex++;
					if( m_NowIndex >= totcnt - MAX_OBSERVERCHECK )		m_NowIndex = totcnt - MAX_OBSERVERCHECK;					
					
					if( m_NowIndex < 0 )		m_NowIndex = 0;
					m_MouseOverIndex = -1;	//	오버된 인덱스

					//	스클로바 텀브바 설정
					ResetControlls();

					return 1;
				}
				case IDM_THUMB:
				{
					return 1;
				}
			}

			break;
		}

		case WM_SETCURSOR:			return 1;
		case WM_RBUTTONUP:
		case WM_RBUTTONDBLCLK:
		case WM_MBUTTONDBLCLK:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:			return 1;
								
	}
	return CWnd::WindowProc(message, wParam, lParam);
}

void CObserverListView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	BOOL bOver = false;
	if( m_UpBtn.OnMouseMove( point.x, point.y ) )	bOver = true;
	if( m_DownBtn.OnMouseMove( point.x, point.y ) )	bOver = true;
	if( m_ThumbBtn.OnMouseMove( point.x, point.y ) )	bOver = true;
	SetCursor( GM().GetCurHandCursor() );
	if( bOver )		return;


	SetCursor( GM().GetCurSelCursor() );
	int nMaxObserver = GM().GetMaxObserver();
	for( int i = 0; i < MAX_OBSERVERCHECK; i++ )
	{
		if( !m_UserCheckRect_Max[ i ].PtInRect( point ) )		continue;
		int index = m_NowIndex + i;
		if( index >= nMaxObserver || index >= MAX_OBSERVERCHECK)continue;
		if( m_pUserInfoList[ index ] == NULL )		return;
		if( strlen(m_pUserInfoList[ index ]->strID) <= 0 )		return;
				
		m_SelectIndex = index;
		OnButtonViewprofile();
		//m_pParentWnd->PostMessage(WM_COMMAND,IDC_BTN_PROFILE,0);
		return;
	}

	CWnd::OnLButtonDblClk(nFlags, point);
}

void CObserverListView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	SetFocus();
	SetCapture();

	m_LButtonDownThumbPos = point;
	SetCursor( GM().GetCurHandCursor() );

	if( m_UpBtn.OnLButtonDown( point.x, point.y ) )			return;
	if( m_DownBtn.OnLButtonDown( point.x, point.y ) )		return;
	if( m_ThumbBtn.OnLButtonDown( point.x, point.y ) ) {
		m_LButtonDownThumbPos.x = point.x - m_ThumbBtn.m_Xp;
		m_LButtonDownThumbPos.y = point.y - m_ThumbBtn.m_Yp;
		return;
	}

	m_LButtonDownThumbPos = 0;
	SetCursor( GM().GetCurSelCursor() );

	
	
	
	int nMaxObserver = GM().GetMaxObserver();
	for( int i = 0; i < MAX_OBSERVERCHECK; i++ ) 
	{
		if( !m_UserCheckRect_Max[ i ].PtInRect( point ) )
			continue;
		int index = m_NowIndex + i;
		if( index >= nMaxObserver || index >= MAX_OBSERVERCHECK)
			continue;

		if( m_pUserInfoList[ index ] == NULL || strlen(m_pUserInfoList[ index ]->strID) <= 0 ) 
		{
			m_SelectIndex = -1;		
			return;		 
		}

		m_pUserInfoList[index]->bSelected = !m_pUserInfoList[index]->bSelected; 
		m_SelectIndex = index;	

		return;
	}
	

	//	스롤바에서 페이지 다운이나 업일때
	if( m_ScrollBaseRect.PtInRect( point ) ) {
		if( point.y < m_ThumbBtn.m_Yp ) {
			m_NowIndex -= MAX_OBSERVERCHECK;
		}
		else {
			int totcnt = m_deque_ObUser.size();
			if( totcnt <= MAX_OBSERVERCHECK )		return;
			m_NowIndex += MAX_OBSERVERCHECK;
			if( m_NowIndex >= totcnt - MAX_OBSERVERCHECK )		m_NowIndex = totcnt - MAX_OBSERVERCHECK;
		}

		if( m_NowIndex < 0 )		m_NowIndex = 0;
		m_MouseOverIndex = -1;	//	오버된 인덱스
	}

	//	스클로바 위치조정
	ResetControlls();
	CWnd::OnLButtonDown(nFlags, point);
}

void CObserverListView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	ReleaseCapture();

	if( m_UpBtn.OnLButtonUp( point.x, point.y ) )		return;
	if( m_DownBtn.OnLButtonUp( point.x, point.y ) )		return;
	if( m_ThumbBtn.OnLButtonUp( point.x, point.y ) )	return;

	SetCursor( GM().GetCurSelCursor() );

	Invalidate(FALSE);
	CWnd::OnLButtonUp(nFlags, point);
}

void CObserverListView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
//	CWnd *pWnd = GetFocus();
//	SetFocus();

	if( m_ThumbBtn.m_bClick )
	{		
		int totcnt = m_deque_ObUser.size();
		int index = 0;
		
		if( totcnt <= MAX_OBSERVERCHECK )	return;
		
		m_ThumbBtn.m_Yp = point.y - m_LButtonDownThumbPos.y;
		if( m_ThumbBtn.m_Yp < m_ScrollBaseRect.top )		m_ThumbBtn.m_Yp = m_ScrollBaseRect.top;
		else if( m_ThumbBtn.m_Yp > m_ScrollBaseRect.bottom - m_ThumbBtn.m_Height )
			m_ThumbBtn.m_Yp = m_ScrollBaseRect.bottom - m_ThumbBtn.m_Height;
		
		//	인덱스 구해오기
		index = MAX_OBSERVERCHECK;
		
		
		float ftt = ( float )( m_ThumbBtn.m_Yp - m_ScrollBaseRect.top )/ ( m_ScrollBaseRect.Height() - m_ThumbBtn.m_Height );
		int per = ( int )( ftt * 100 );
		m_NowIndex = ( ( totcnt - index ) * per ) / 100;
		
		return;
	}

	
	BOOL bOver = false;

	if( m_UpBtn.OnMouseMove( point.x, point.y ) )	bOver = true;
	if( m_DownBtn.OnMouseMove( point.x, point.y ) )	bOver = true;
	if( m_ThumbBtn.OnMouseMove( point.x, point.y ) ){
		bOver = true;
		m_MouseOverIndex = -1;
	}

	if( bOver )		return;

	m_MouseOverIndex = -1;
	
	for( int i = 0; i < MAX_OBSERVERCHECK; i++ )
	{
		if( !m_UserCheckRect_Max[ i ].PtInRect( point ) )		continue;
		m_MouseOverIndex = i;
		break;
	}

//	if( m_MouseOverIndex < 0 && pWnd != NULL )	pWnd->SetFocus();
	SetCursor( GM().GetCurSelCursor() );
	CWnd::OnMouseMove(nFlags, point);
}

void CObserverListView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if( m_UpBtn.m_bClick || m_DownBtn.m_bClick )
	{
		m_TimeCnt++;
		if( m_TimeCnt % 4 == 0 )
		{
			if( m_UpBtn.m_bClick )		SendMessage( WM_COMMAND, IDM_UP );
			if( m_DownBtn.m_bClick )	SendMessage( WM_COMMAND, IDM_DOWN );
			m_TimeCnt = 0;
		}
	}

	POINT mpos;
	GetCursorPos(&mpos);
	ScreenToClient(&mpos);

	if( GetFocus() != this || mpos.y<0 || mpos.x<0 || mpos.y>379 || mpos.x> 316)
	{	
		m_UpBtn.m_bOver = false;
		m_DownBtn.m_bOver = false;
		m_ThumbBtn.m_bOver = false;
	}

	Invalidate( false );

	CWnd::OnTimer(nIDEvent);
}

void CObserverListView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CDC MemDC;
	MemDC.CreateCompatibleDC( &dc );
	if( MemDC.m_hDC == NULL )		return;
	MemDC.SelectObject( m_Page.hBmp );
	MemDC.SetBkMode( TRANSPARENT );

	Draw( &MemDC );

	CRect rect;
	GetWindowRect( &rect );
	dc.BitBlt( 0, 0, rect.Width(), rect.Height(), &MemDC, 0, 0, SRCCOPY );
	MemDC.DeleteDC();
}

void CObserverListView::Draw( CDC *pDC )
{
	//if( m_pBackSpr == NULL )		return;

	//m_Page.PutSprAuto(0,0,m_pBackSpr,1);
	m_Page.DrawFillBox(0,0,m_Page.width, m_Page.height, NMBASE::GRAPHICGDI::xSprite::RGB555(227,232,236));

	//선그리기
	for( int i = 0; i < MAX_OBSERVERCHECK; ++i )
	{
		m_Page.DrawLine(m_UserCheckRect_Max[i].left + 3, m_UserCheckRect_Max[i].bottom-1, m_UserCheckRect_Max[i].right-3, m_UserCheckRect_Max[i].bottom-1, NMBASE::GRAPHICGDI::xSprite::RGB555(210,214,217));
	}

	//	룸일때
	DrawRoom( pDC );
	DrawButton( pDC );	
}

//	룸일때
void CObserverListView::DrawRoom( CDC *pDC )
{
	pDC->SelectObject( FONT_DEFAULT);
	pDC->SetTextColor( COLOR_DEFAULT_POPUP );	

	int imgnum = 0;

	CString strTemp;
	int index = m_NowIndex;
	CRect rect;
	OBSERVER_DATA *pUi = NULL;
	int nMaxObserver = GM().GetMaxObserver();

	for( int i = 0; i < MAX_OBSERVERCHECK; ++i )
	{
		index = m_NowIndex + i;

		if(index >= nMaxObserver || index >= MAX_OBSERVERCHECK)
			continue;

		if( m_pUserInfoList[ index ] == NULL )
			continue;

		pUi = m_pUserInfoList[ index ];
		if(pUi == NULL )
		{
			TRACE("pRI == NULL");
			continue;
		}		

		if (m_bEnableBanish)
		{
			//체크박스
			m_Page.PutSprAuto(m_UserCheckRect_Max[i].left + 11,m_UserCheckRect_Max[i].top+6, m_pEtcSpr, pUi->bSelected?1:0);

			rect.SetRect(m_UserCheckRect_Max[i].left + 32,m_UserCheckRect_Max[i].top, m_UserCheckRect_Max[i].left +121, m_UserCheckRect_Max[i].bottom);		
		}
		else
		{
			rect.SetRect(m_UserCheckRect_Max[i].left + 24,m_UserCheckRect_Max[i].top, m_UserCheckRect_Max[i].left + 113, m_UserCheckRect_Max[i].bottom);
		}

		strTemp.Format( "%s", pUi->strNickName );		
		pDC->DrawText( strTemp, &rect, DT_LEFT|DT_VCENTER|DT_SINGLELINE );

// 		int nIcon = GetMoneyGrade( pUi->PMoney );
// 
// 		m_Page.PutSprAuto( 158, m_UserCheckRect_Max[i].top, m_pLevelSpr, nIcon );
// 		m_Page.PutSprAuto( 194, m_UserCheckRect_Max[i].top, m_pEtcSpr, 11 + pUi->Sex );		
		

		//보유머니
		CString strM = NMBASE::UTIL::g_MakeShortMoneyStrEx(pUi->PMoney);
		strTemp.Format( "%s", strM.GetString() );
		strTemp.Append(strChip_Count[g_RI.ChipKind]);

		rect.SetRect(m_UserCheckRect_Max[i].left + 138,m_UserCheckRect_Max[i].top, m_UserCheckRect_Max[i].left + 216, m_UserCheckRect_Max[i].bottom);
		pDC->DrawText( strTemp, &rect, DT_RIGHT|DT_VCENTER|DT_SINGLELINE );

		CString strR;
		if( pUi->Seq == 0 ){
			strR="관전중";
		}
		else{
			strR.Format("%d순위대기",pUi->Seq);
		}

		if( strR.GetLength() > 0 )
		{
			rect.SetRect(m_UserCheckRect_Max[i].left + 225,m_UserCheckRect_Max[i].top, m_UserCheckRect_Max[i].left + 291, m_UserCheckRect_Max[i].bottom);
			pDC->DrawText( strR, &rect, DT_CENTER|DT_VCENTER|DT_SINGLELINE );
		}

		//%%%추후 수정
		/*
		// 체크 된 것 그리기 (내가 방장인경우에만 노출)
		if (strcmp(g_RI.ID, g_MyInfo.UI.ID)==0)
		{
			if ( index == m_SelectIndex )
			{
				m_Page.PutSprAuto( m_UserCheckRect_Max[i].left+1,m_UserCheckRect_Max[i].top, m_pButtonSpr, 10 );
			}
			else
			{
				m_Page.PutSprAuto( m_UserCheckRect_Max[i].left+1,m_UserCheckRect_Max[i].top, m_pButtonSpr, 9 );
			}		
		}
		*/
	}

	return;
}


void CObserverListView::DrawButton( CDC *pDC )
{
	m_UpBtn.Draw(pDC);
	m_DownBtn.Draw(pDC);
	m_ThumbBtn.Draw(pDC);	
}

//	============= 리스트 관련 ========================//
//	추가하기
bool CObserverListView::InsertList( OBSERVER_DATA *pList )
{	
//	if( g_Where != WH_GAME ) return false;
	deque<OBSERVER_DATA>::iterator iter;
	int count = m_deque_ObUser.size();
	for(iter = m_deque_ObUser.begin(); iter != m_deque_ObUser.end(); ++iter) { //중복검사
		OBSERVER_DATA *pData = &(*iter);
		if( pData == NULL )	continue;

		if( strcmp( pData->strID, pList->strID ) == 0 && pData->UNum == pList->UNum) return false;
	}
		
	m_deque_ObUser.push_back( *((OBSERVER_DATA*)pList) );	
	//	정보 갱신 정렬하기
	RenewalShortList();
	return true;
}

//  선택한 넘 아이디 
OBSERVER_DATA	*CObserverListView::GetSelectIndex_strID()
{
	if(m_deque_ObUser.size() == 0) return NULL;
	
//	NMBASE::SOUND::g_Sound.PlayWav(SND_BUTTON_CLICK);
	if(m_SelectIndex < 0)return NULL;
	
	int index = m_SelectIndex;

	if( index >= GM().GetMaxObserver() ) 
		return NULL;

	if(m_pUserInfoList[ index ] == NULL) 
		return NULL;	
	
	CString szID;
	szID = m_pUserInfoList[ index ]->strID;	

	if(szID.GetLength() == 0) 
		return NULL;	

	return m_pUserInfoList[ index ];
}

void CObserverListView::ResetReservation()
{
	deque<OBSERVER_DATA>::iterator iter;	
	for(iter = m_deque_ObUser.begin(); iter != m_deque_ObUser.end(); iter++) 
	{
		OBSERVER_DATA *pData = &(*iter);
		if( pData == NULL )	continue;		
		pData->Seq = 0;			
	}
}

void CObserverListView::SetList( int unum, char *id, int seq )
{
	deque<OBSERVER_DATA>::iterator iter;
	int count = m_deque_ObUser.size();
	for(iter = m_deque_ObUser.begin(); iter != m_deque_ObUser.end(); iter++) { //중복검사
		OBSERVER_DATA *pData = &(*iter);
		if( pData == NULL )	continue;

		if( strcmp( pData->strID , id ) == 0 && pData->UNum == unum ){
			pData->Seq = seq;
			break;
		}
	}
}

//	지우기
void CObserverListView::DelList( int DelIndex, char *pID )
{
	if(m_deque_ObUser.size() == 0) return;
	if( pID == NULL )return;
	if( DelIndex < 0 )return;
	if( strlen(pID) == 0 )return;

	deque<OBSERVER_DATA>::iterator iter;
	int count = m_deque_ObUser.size();
	for(iter = m_deque_ObUser.begin(); iter != m_deque_ObUser.end(); iter++) { //중복검사
		OBSERVER_DATA *pData = &(*iter);
		if( pData == NULL )	continue;

		if( strcmp( pData->strID , pID ) == 0 && pData->UNum == DelIndex ){
			m_deque_ObUser.erase(iter);
			m_SelectIndex = -1;			
			break;
		}		
	}	
	//	정보 갱신 정렬하기
	RenewalShortList();	

	return;
}

//	모두 초기화
void CObserverListView::DelAllList( void )
{
	m_deque_ObUser.clear();
	ZeroMemory( m_pUserInfoList, sizeof( m_pUserInfoList) );		//	방리스트
	m_NowIndex = 0;			//	현재 인덱스
	m_MouseOverIndex = -1;	//	오버된 인덱스
	m_SelectIndex = -1;		//	선택된 인덱스
	m_ThumbBtn.m_Yp = 32;
	return;
}

//	인덱스 설정
void CObserverListView::SetIndex( int Index )
{
	m_NowIndex = Index;
	ResetControlls();
	return;
}

//	선택된 인덱스 찾아오기
int	CObserverListView::GetSelectIndex( void )
{
	return m_SelectIndex;
}

//	정보 갱신
void CObserverListView::RenewalShortList( void )
{
// 	BOOL bScrollBarShow = false;
// 
// 	if(m_deque_ObUser.size()>= MAX_OBSERVERCHECK) bScrollBarShow = true;
// 	ZeroMemory( m_pUserInfoList, sizeof( OBSERVER_DATA * ) * GM().GetMaxObserver());
// 
// 	deque<OBSERVER_DATA>::iterator iter;
// 	int nIndex = 0;
// 	for(iter = m_deque_ObUser.begin(); iter!=m_deque_ObUser.end(); ++iter) {			
// 		m_pUserInfoList[nIndex++] = &(*iter);
// 		if( nIndex >= MAX_OBSERVERCHECK )break;
// 	}
// 
// 	if( m_NowIndex >= m_deque_ObUser.size() - MAX_OBSERVERCHECK )	m_NowIndex = m_deque_ObUser.size() - MAX_OBSERVERCHECK;
// 
// 	if( m_NowIndex < 0 )		m_NowIndex = 0;
// 	m_UpBtn.Show( bScrollBarShow );
// 	m_DownBtn.Show( bScrollBarShow );
// 	m_ThumbBtn.Show( bScrollBarShow );
// 
// 	if( !bScrollBarShow )
// 	{
// 		m_NowIndex = 0;
// 		m_MouseOverIndex = -1;	//	오버된 인덱스
// 	}
// 	return;

	BOOL bScrollBarShow = false;
	
//	if(m_deque_ObUser.size()>= MAX_OBSERVERCHECK) bScrollBarShow = true;
	ZeroMemory( m_pUserInfoList, sizeof( m_pUserInfoList) );
	
	deque<OBSERVER_DATA>::iterator iter;
	int nIndex = 0;
	int nMaxObserver = GM().GetMaxObserver();
	for(iter = m_deque_ObUser.begin(); iter!=m_deque_ObUser.end(); ++iter) {			
		m_pUserInfoList[nIndex++] = &(*iter);
		if( nIndex >= nMaxObserver )break;
	}
	if( nIndex > MAX_OBSERVERCHECK) 
	{
		bScrollBarShow = true;
	}
	
	
	if( m_NowIndex >= int (m_deque_ObUser.size() - MAX_OBSERVERCHECK) )	
		m_NowIndex = (int) (m_deque_ObUser.size() - MAX_OBSERVERCHECK);

	//if( m_NowIndex >= nIndex - MAX_OBSERVERCHECK )	m_NowIndex = nIndex - MAX_OBSERVERCHECK;
	if( m_NowIndex < 0 )		m_NowIndex = 0;

	m_UpBtn.Show( bScrollBarShow );
	m_DownBtn.Show( bScrollBarShow );
	m_ThumbBtn.Show( bScrollBarShow );
	
	if( !bScrollBarShow )
	{
		m_NowIndex = 0;
		m_MouseOverIndex = -1;	//	오버된 인덱스
	}
	return;

}

//	==================================================

//	정렬하기 - 스크롤기능
void CObserverListView::ResetControlls( void )
{
	int totcnt = m_deque_ObUser.size();
	int index = 0;
	index = MAX_OBSERVERCHECK;

	int tt = totcnt - index;
	if( tt <= 0 )	tt = 1;

	float ftt = ( float )m_NowIndex / tt;
	int per = ( int )( ftt * 100 );
	int bb = m_ScrollBaseRect.Height() - m_ThumbBtn.m_Height;
	int dd = ( bb * per ) / 100;
	int xx = 0 ;
	if(dd>=bb)xx = bb;
	else xx =dd;
		
	m_ThumbBtn.m_Yp = m_ScrollBaseRect.top + xx;

	return;
}

void CObserverListView::OnButtonViewprofile()
{
//	if(g_Wait == TRUE) return;
//	NMBASE::SOUND::g_Sound.PlayWav(SND_BUTTON_CLICK);
	if(m_SelectIndex < 0 || m_SelectIndex >= MAX_OBSERVERCHECK)
		return;
		
	if(m_pUserInfoList[ m_SelectIndex ] == NULL)
		return;	
	
	if( strlen(m_pUserInfoList[ m_SelectIndex ]->strID) <= 0 )
		return;

	CString szID;
	szID = m_pUserInfoList[ m_SelectIndex ]->strID;
	
	if(szID.GetLength() == 0) 
		return;			
//	g_Wait = TRUE;	
// 
// 	CSV_ASK_USERINFO aumsg;
// 	aumsg.Set(m_pUserInfoList[ m_SelectIndex ]->llUserKey, (char*)szID.operator LPCTSTR());
// 	NMBASE::SOCK::GSOCKMAN().SendData(g_MainSrvSID, aumsg.pData, aumsg.GetTotalSize());


 	int pno = GM().GetCurrentGame()->GetGameProcess()->GetPlayerPNum(szID);
	if (pno<0)
		return;
 	CPlayer *pPlayer = GM().GetCurrentGame()->GetGameProcess()->GetPlayer(pno);
	if (pPlayer)
	{
		ShowProfileDialog(&pPlayer->UI, GetParent());
	}
}
 


void CObserverListView::SetEnableBanish(BOOL bBanish)
{
	m_bEnableBanish = bBanish;
}

void CObserverListView::KickOutSelectedUser()
{
	//체크된 유저 모두 강제 추방

	int nMaxObserver = GM().GetMaxObserver();
	for(int i=0; i<nMaxObserver; i++ )
	{
		if( m_pUserInfoList[ i ] == NULL || strlen(m_pUserInfoList[ i ]->strID) <= 0 ) 
			continue;		 

		if (m_pUserInfoList[i]->bSelected )
		{
			// 강제 추방 요청
			CSV_ASK_FORCEBANISH fbmsg;
			fbmsg.Set(m_pUserInfoList[i]->strID);
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), fbmsg.pData, fbmsg.GetTotalSize());
		}
	}
}

int CObserverListView::GetSelectedUserCount()
{
	int cnt = 0;
	int nMaxObserver = GM().GetMaxObserver();
	for(int i=0; i<nMaxObserver; i++ )
	{
		if( m_pUserInfoList[ i ] == NULL || strlen(m_pUserInfoList[ i ]->strID) <= 0 ) 
			continue;		 

		if (m_pUserInfoList[i]->bSelected )
		{
			cnt++;			
		}
	}

	return cnt;

}


void CObserverListView::SetCheckAll(BOOL bCheck)
{
	//전체 선택 체크박스 클릭여부 체크
	
	int nMaxObserver = GM().GetMaxObserver();
	for(int i=0; i<nMaxObserver; i++ )
	{
		if( m_pUserInfoList[ i ] == NULL || strlen(m_pUserInfoList[ i ]->strID) <= 0 ) 
			continue;		 
		
		m_pUserInfoList[i]->bSelected = 	bCheck;
	}

	
}