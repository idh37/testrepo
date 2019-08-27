#include "stdafx.h"
#include "ComboBoxImageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DEFAULT_COMBOLIST_W 100
#define DEFAULT_COMBOLIST_H 15

CComboBoxImageDlg::CComboBoxImageDlg(CWnd* pParent /*=NULL*/)
	: CUPGDlgBase(CComboBoxImageDlg::IDD, pParent)
{
	m_ComboBoxInfo.Clear();
	m_nSelectContent = -1;				//deque에 인덱스를 알고 있는다.
	m_deque_ComboBoxDeck.clear();
	m_ComboBoxInfo.Clear();
	m_CombolistWidth = 0;
	m_CombolistHeight = 0;
}


void CComboBoxImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CUPGDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CComboBoxImageDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CComboBoxImageDlg, CUPGDlgBase)
	//{{AFX_MSG_MAP(CComboBoxImageDlg)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_KILLFOCUS()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComboBoxImageDlg message handlers

BOOL CComboBoxImageDlg::OnInitDialog() 
{
	CUPGDlgBase::OnInitDialog();
	/*
		★--- 규칙 ---★
		1 : 콤보 박스 이미지는 일반 마우스 오버 2장으로 구성 된다.
		2 : spr 0번째는 항상 선택 없음이 들어간다. 
		3 : 콤보 박스 테두리(1픽셀)와 리스트 와 리스트 사이에 한줄(1픽셀)의 이미지가 들어간다.
		4 : 선택된 리스트 이미즈는 bmp 파일로 따로 찍어줌( page 면 spr 로 가지고 있어도 상관없음)
		5 : 위 사항 안지키면 난 모름 참고 spr (통합 포커 data 안에 crm_cb_moneyoption.spr 파일 참고 할것
		6 : 대략 멍한 상태에서 쓴거라 알아서 하삼.
	*/

	/*
		text 로 콤보 내용을 넣을수 있게 수정..쓰는 방법은 알아서..
		위 1, 3 내용은 지켜야 함
	*/
	
	// TODO: Add extra initialization here
	m_nSelectContent = -1; //deque에 인덱스를 알고 있는다.	
	SetTimer(TIMER_COMBOIMG_DRAW, (UINT)(1000.f/20), 0);

	//DLG의 Rect 값 구하기
	int With = DEFAULT_COMBOLIST_W;
	int height = DEFAULT_COMBOLIST_H;

	switch(m_ComboBoxInfo.d_type) {
	case enum_drawtype_TEXT:
		With = DEFAULT_COMBOLIST_W;
		height = DEFAULT_COMBOLIST_H;
		break;
	case enum_drawtype_IMG:
	case enum_drawtype_TEXT_IMG:
	case enum_drawtype_IMG_INDEX:
		{			
			if( m_ComboBoxInfo.pSpr == NULL ){
				With = DEFAULT_COMBOLIST_W;
				height = DEFAULT_COMBOLIST_H;
			}
			else{
				if( m_deque_ComboBoxDeck.size() > 0 ){
					COMBOBOX_LIST *pData = &m_deque_ComboBoxDeck.front();
					
					//콤보박스용 spr에 0번째는 리스트 배경이미지로 해준다..
					With = m_ComboBoxInfo.pSpr->spr[pData->sprindex].xl;
					height = m_ComboBoxInfo.pSpr->spr[pData->sprindex].yl;
				}				
			}		
		}
		break;	
	}

	int total = m_deque_ComboBoxDeck.size();

	//(total + 1) 아래 위 1픽셀의 라인 이미지 때문에
	int h = h = total * height + (total + 1);
	
	if( h <= 0 ){
		CUPGDlgBase::OnCancel();
	}
	
	// with 로 생성을 하면 양쪽 끝 아래 1픽셀씩 사용을 못하는거 같아 + 해줌 가로 2 세로 1
	CRect rt(0,0,With,h);
	rt.OffsetRect(m_ComboBoxInfo.BasePT.x,m_ComboBoxInfo.BasePT.y);
	MoveWindow(rt);

	//page는 안해준다.
	m_CombolistWidth = With;
	m_CombolistHeight = h;
	m_Page.Init(With,h,16);


	CRect rect;
	rect.SetRect(0,0,m_CombolistWidth,height);	
	deque<COMBOBOX_LIST>::iterator iter;	
	for(iter = m_deque_ComboBoxDeck.begin(); iter != m_deque_ComboBoxDeck.end(); ++iter) { //중복검사
		COMBOBOX_LIST *pData = &(*iter);
		if( pData == NULL )continue;		
		pData->Rect = rect;
		//+1은 1픽셀의 라인이 들어가기 위해 공간을 비워 둔다.
		pData->Rect.OffsetRect(0,1);
		rect.OffsetRect(0,height+1);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CComboBoxImageDlg::OnDestroy() 
{
	CUPGDlgBase::OnDestroy();

	// TODO: Add your message handler code here	
	m_Page.Destroy();	
	m_deque_ComboBoxDeck.clear();
	KillTimer(TIMER_COMBOIMG_DRAW);
}

void CComboBoxImageDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	// TODO: Add your message handler code here and/or call default
	if( m_ComboBoxInfo.BtnCmd < 0 )return;
	deque<COMBOBOX_LIST>::iterator iter;	
	for(iter = m_deque_ComboBoxDeck.begin(); iter != m_deque_ComboBoxDeck.end(); ++iter) { //중복검사
		COMBOBOX_LIST *pData = &(*iter);
		if( pData == NULL )continue;

		if( pData->Rect.PtInRect(point) ){			 
			m_nSelectContent = pData->Index;
			if( m_pParent != NULL ){				
				m_pParent->SendMessage(WM_COMMAND,m_ComboBoxInfo.BtnCmd , 0);
			}			
			return;
		}
	}
	
	CUPGDlgBase::OnLButtonDown(nFlags, point);
}

void CComboBoxImageDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	deque<COMBOBOX_LIST>::iterator iter;	
	for(iter = m_deque_ComboBoxDeck.begin(); iter != m_deque_ComboBoxDeck.end(); ++iter) { //중복검사
		COMBOBOX_LIST *pData = &(*iter);
		if( pData == NULL )continue;

		if( pData->Rect.PtInRect(point) ){
			m_MouseOverIndex = pData->Index;
			return;
		}
	}
	
	CUPGDlgBase::OnMouseMove(nFlags, point);
}

void CComboBoxImageDlg::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == TIMER_COMBOIMG_DRAW)
	{
		Invalidate(FALSE);
	}
	
	CUPGDlgBase::OnTimer(nIDEvent);
}

void CComboBoxImageDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	cdc.SelectObject(m_Page.hBmp);
	CFont *oldf = cdc.SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_12_BOLD));

	cdc.SetBkMode(TRANSPARENT);
	cdc.SetTextColor(RGB(0,0,0));

	Draw(&cdc);
	
	CRect rt;
	GetClientRect(&rt);
	dc.BitBlt(0,0,rt.Width(),rt.Height(),&cdc,0,0,SRCCOPY);
	cdc.DeleteDC();	
	// Do not call CUPGDlgBase::OnPaint() for painting messages
}

int CComboBoxImageDlg::DoModal(int nlistNum , COMBOBOX_LIST data[] ,COMBOBOX_INFO *comboboxinfo,CWnd* pParent) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( nlistNum <= 0 ){
		CUPGDlgBase::OnClose();
		return 0;
	}
	if( comboboxinfo == NULL ){
		CUPGDlgBase::OnCancel();
		return 0;
	}	

	if( pParent == NULL ){
		CUPGDlgBase::OnCancel();
		return 0;
	}

	m_pParent = pParent;

	for( int i = 0 ; i < nlistNum ; i++ ){

		m_deque_ComboBoxDeck.push_back(data[i]);
	}

	memcpy( &m_ComboBoxInfo, comboboxinfo ,sizeof( COMBOBOX_INFO ));
	return CUPGDlgBase::DoModal();
}

void CComboBoxImageDlg::Draw(CDC *pDC)
{
	if( m_deque_ComboBoxDeck.size() == 0 )
		return;
	if( m_ComboBoxInfo.d_type == enum_drawtype_TEXT ){
		Draw_Text(pDC);
	}
	else{
		Draw_Image(pDC);
	}	
}

void CComboBoxImageDlg::Draw_Text(CDC *pDC)
{
	if( m_ComboBoxInfo.d_type != enum_drawtype_TEXT )return;
	if( pDC == NULL )return;

	deque<COMBOBOX_LIST>::iterator iter;	
	for(iter = m_deque_ComboBoxDeck.begin(); iter != m_deque_ComboBoxDeck.end(); ++iter) { //중복검사
		COMBOBOX_LIST *pData = &(*iter);
		if( pData == NULL )continue;
		//배경을 어쩌냥..ㅡㅡㅋ
		pDC->DrawText(pData->strContent,pData->Rect,DT_LEFT);		
	}
}

void CComboBoxImageDlg::Draw_Image(CDC *pDC)
{
	if( m_ComboBoxInfo.d_type == enum_drawtype_TEXT )return;

	if( pDC == NULL )return;

	int total = m_deque_ComboBoxDeck.size();

	CRect rt;
	int cnt = 0 ;
	deque<COMBOBOX_LIST>::iterator iter;	
	for(iter = m_deque_ComboBoxDeck.begin(); iter != m_deque_ComboBoxDeck.end(); ++iter) { //중복검사
		COMBOBOX_LIST *pData = &(*iter);
		if( pData == NULL )continue;
		rt = pData->Rect;
		if( pData->sprindex > -1 ){			
			m_Page.PutSprAuto(rt.left,rt.top,m_ComboBoxInfo.pSpr,pData->sprindex);			
		}
		
		if( pData->MouseOversprindex > -1 && pData->Index == m_MouseOverIndex ){
			m_Page.PutSprAuto(rt.left,rt.top,m_ComboBoxInfo.pSpr,pData->MouseOversprindex);
		}	

		if( m_ComboBoxInfo.d_type == enum_drawtype_TEXT_IMG ){

			pDC->SetTextColor(RGB(198,214,222));

			CPoint gab(3,5);
			CRect rtText = pData->Rect;
			rtText.left += gab.x;
			rtText.top  += gab.y;

			pDC->DrawText(pData->strContent, &rtText, DT_LEFT);
		}
		
		if(m_ComboBoxInfo.d_type == enum_drawtype_IMG_INDEX ){
			//숫자 드로우 
			Draw_Number(pDC,&m_Page,pData);
		}	
		
		if( m_ComboBoxInfo.d_type == enum_drawtype_IMG ){
			m_Page.PutSprAuto(rt.left,rt.top,m_ComboBoxInfo.pSpr, (int) pData->nContentData);
		}

		cnt ++;
		if( cnt != total){
			//1개의 리스트 다음 1픽셀 여유 공간에 1픽셀의 라인 이미지 드로우
			m_Page.PutSprAuto(rt.left,rt.bottom,m_ComboBoxInfo.pSpr,m_ComboBoxInfo.nUnderLineSprIndex);
		}	
	}
	
	GetClientRect(&rt);	

	//콤보 박스 상단에 1픽셀 여유 공간에 1픽셀의 라인 이미지 드로우	
	m_Page.PutSprAuto(rt.left,rt.top,m_ComboBoxInfo.pSpr,m_ComboBoxInfo.nOutLineSprIndex);

	//콤보 박스 하단에 1픽셀 여유 공간에 1픽셀의 라인 이미지 드로우
	m_Page.PutSprAuto(rt.left,rt.bottom-1,m_ComboBoxInfo.pSpr,m_ComboBoxInfo.nOutLineSprIndex);		

}

void CComboBoxImageDlg::Draw_Number(CDC *pDC,NMBASE::GRAPHICGDI::CPage *pPage,COMBOBOX_LIST *pData)
{
	if( pData == NULL )return;
	CString str;
	
	str = get_Numberstring(pData->nContentData);

	if( strlen( pData->strContent ) > 0 ){
		str+=pData->strContent;
	}

	int With = m_ComboBoxInfo.pSpr->spr[0].xl;

	CRect rt;
	rt = pData->Rect;

	int x = 0;
	int y = 0;
	for( int i = 0 ; i < str.GetLength() ; i++ )
	{		
		int index = get_MonetaryUnit(str.GetAt(i));
		if( index > 9 )
		{
			x = 2;
			y = 0;
		}

	//	pPage->PutSprAuto(rt.left+(i*(With-6))+12 + x,rt.top+3 + y,m_ComboBoxInfo.pSpr,index); 앞에서 12 정도이동후 그리기
		pPage->PutSprAuto(rt.left+(i*(With-6)) + x,rt.top+3 + y,m_ComboBoxInfo.pSpr,index);
	}

//	m_Page.PutSprAuto(rt.left,rt.bottom,m_ComboBoxInfo.pSpr,0);
}

//선택한 데이터를 리턴한다.
COMBOBOX_LIST* CComboBoxImageDlg::GetSelectComboBoxContent()
{
	COMBOBOX_LIST* pResultData = NULL;
	deque<COMBOBOX_LIST>::iterator iter;	
	for(iter = m_deque_ComboBoxDeck.begin(); iter != m_deque_ComboBoxDeck.end(); ++iter) { //중복검사
		COMBOBOX_LIST *pData = &(*iter);
		if( pData == NULL )continue;

		if( m_nSelectContent == pData->Index ){			 
			return pData;		
		}
	}
	return NULL;
}

BOOL CComboBoxImageDlg::DoModaless(  int nlistNum , COMBOBOX_LIST data[] ,COMBOBOX_INFO *comboboxinfo ,CWnd* pParent)
{	
	if(GetSafeHwnd()) 
		return FALSE;	

	if( nlistNum <= 0 ){
		CUPGDlgBase::OnCancel();
	}
	if( comboboxinfo == NULL ){
		CUPGDlgBase::OnCancel();
	}	

	m_pParent = pParent;

	if( m_pParent == NULL )CUPGDlgBase::OnCancel();


	for( int i = 0 ; i < nlistNum ; i++ ){

		m_deque_ComboBoxDeck.push_back(data[i]);
	}

	memcpy( &m_ComboBoxInfo, comboboxinfo ,sizeof( COMBOBOX_INFO ));

	BOOL rtn = FALSE;	
	rtn = Create(IDD,pParent);
	if(rtn) ShowWindow(SW_SHOW);

	return rtn;
}

BOOL CComboBoxImageDlg::DoModaless( CWnd* pParent )
{
	if(GetSafeHwnd()) 
		return FALSE;	
	
	m_pParent = pParent;

	if( m_pParent == NULL )CUPGDlgBase::OnCancel();

	BOOL rtn = FALSE;
	rtn = Create(IDD,pParent);
	if(rtn) ShowWindow(SW_SHOW);

	return rtn;
}

BOOL CComboBoxImageDlg::set_ComboBoxInfo(COMBOBOX_INFO *comboboxinfo)
{
	if( comboboxinfo == NULL ){
		return FALSE;
	}
	
	memcpy( &m_ComboBoxInfo, comboboxinfo ,sizeof( COMBOBOX_INFO ));

	return TRUE;
}

void CComboBoxImageDlg::set_List(COMBOBOX_LIST data)
{	
	if( data.Index == -1 )return;
	m_deque_ComboBoxDeck.push_back(data);
}

void CComboBoxImageDlg::OnCancel() 
{
	// TODO: Add extra cleanup here	
	
	CUPGDlgBase::OnCancel();
	DestroyWindow();
}

BOOL CComboBoxImageDlg::SafeClose()
{
	if(!GetSafeHwnd()) return FALSE;
	SendMessage(WM_COMMAND, IDCANCEL);
	return TRUE;
}

BOOL CComboBoxImageDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CUPGDlgBase::DestroyWindow();
}


CString	CComboBoxImageDlg::get_Numberstring(INT64 number)
{
	CString str; // 한글표시
	CString str1;
	INT64 t = number; 
	INT64 Money = number;		
	
	int num[4] = {0,};	
	NMBASE::UTIL::g_NumberToParse(Money, num[3], num[2], num[1], num[0]);

	if (num[3] > 0){
		str.Format("%dJ",num[3]);		
	}
	if (num[2] > 0){
		str.Format("%dY",num[2]);		
	}
	if (num[1] > 0){
		str.Format("%dM",num[1]);
	}
	if (num[0] > 0){
		str.Format("%d",num[0]);		
	}
	
	return str;	
}

int CComboBoxImageDlg::get_MonetaryUnit(char ch)
{
	CString str;
	str.Format("%c", ch);

	if( str.GetLength() == 0 ) 
		return -1;
	
	if( str.Compare( "W" ) == 0 ){
		return 10;
	}
	//만
	else if( str.Compare( "M" ) == 0 ){
		return 11;
	}
	//억
	else if( str.Compare( "Y" ) == 0 ){
		return 12;
	}
	//조
	else if( str.Compare( "J" ) == 0 ){
		return 13;
	}
	//제한 없음
	else if( str.Compare( "N" ) == 0 )
	{
		return 18;
	}
	//조 이상
	else if( str.Compare( "O" ) == 0 )
	{
		return 19;
	}
	// 수동
	else if( str.Compare( "A" ) == 0 )
	{
		return 20;
	}
	else{
		return atoi(str.operator LPCTSTR());
	}

	return -1;
}

void CComboBoxImageDlg::OnKillFocus(CWnd* pNewWnd) 
{
	CUPGDlgBase::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
	ShowWindow(SW_HIDE);
	SafeClose();
}

BOOL CComboBoxImageDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rt2;
	GetClientRect(&rt2);
	pDC->FillSolidRect(&rt2,RGB(0,0,0));

 	return TRUE;
	return CUPGDlgBase::OnEraseBkgnd(pDC);
}
