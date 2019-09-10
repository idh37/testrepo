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
	m_nSelectContent = -1;				//deque�� �ε����� �˰� �ִ´�.
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
		��--- ��Ģ ---��
		1 : �޺� �ڽ� �̹����� �Ϲ� ���콺 ���� 2������ ���� �ȴ�.
		2 : spr 0��°�� �׻� ���� ������ ����. 
		3 : �޺� �ڽ� �׵θ�(1�ȼ�)�� ����Ʈ �� ����Ʈ ���̿� ����(1�ȼ�)�� �̹����� ����.
		4 : ���õ� ����Ʈ �̹���� bmp ���Ϸ� ���� �����( page �� spr �� ������ �־ �������)
		5 : �� ���� ����Ű�� �� �� ���� spr (���� ��Ŀ data �ȿ� crm_cb_moneyoption.spr ���� ���� �Ұ�
		6 : �뷫 ���� ���¿��� ���Ŷ� �˾Ƽ� �ϻ�.
	*/

	/*
		text �� �޺� ������ ������ �ְ� ����..���� ����� �˾Ƽ�..
		�� 1, 3 ������ ���Ѿ� ��
	*/
	
	// TODO: Add extra initialization here
	m_nSelectContent = -1; //deque�� �ε����� �˰� �ִ´�.	
	SetTimer(TIMER_COMBOIMG_DRAW, (UINT)(1000.f/20), 0);

	//DLG�� Rect �� ���ϱ�
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
					
					//�޺��ڽ��� spr�� 0��°�� ����Ʈ ����̹����� ���ش�..
					With = m_ComboBoxInfo.pSpr->spr[pData->sprindex].xl;
					height = m_ComboBoxInfo.pSpr->spr[pData->sprindex].yl;
				}				
			}		
		}
		break;	
	}

	int total = m_deque_ComboBoxDeck.size();

	//(total + 1) �Ʒ� �� 1�ȼ��� ���� �̹��� ������
	int h = h = total * height + (total + 1);
	
	if( h <= 0 ){
		CUPGDlgBase::OnCancel();
	}
	
	// with �� ������ �ϸ� ���� �� �Ʒ� 1�ȼ��� ����� ���ϴ°� ���� + ���� ���� 2 ���� 1
	CRect rt(0,0,With,h);
	rt.OffsetRect(m_ComboBoxInfo.BasePT.x,m_ComboBoxInfo.BasePT.y);
	MoveWindow(rt);

	//page�� �����ش�.
	m_CombolistWidth = With;
	m_CombolistHeight = h;
	m_Page.Init(With,h,16);


	CRect rect;
	rect.SetRect(0,0,m_CombolistWidth,height);	
	deque<COMBOBOX_LIST>::iterator iter;	
	for(iter = m_deque_ComboBoxDeck.begin(); iter != m_deque_ComboBoxDeck.end(); ++iter) { //�ߺ��˻�
		COMBOBOX_LIST *pData = &(*iter);
		if( pData == NULL )continue;		
		pData->Rect = rect;
		//+1�� 1�ȼ��� ������ ���� ���� ������ ��� �д�.
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
	for(iter = m_deque_ComboBoxDeck.begin(); iter != m_deque_ComboBoxDeck.end(); ++iter) { //�ߺ��˻�
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
	for(iter = m_deque_ComboBoxDeck.begin(); iter != m_deque_ComboBoxDeck.end(); ++iter) { //�ߺ��˻�
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
	for(iter = m_deque_ComboBoxDeck.begin(); iter != m_deque_ComboBoxDeck.end(); ++iter) { //�ߺ��˻�
		COMBOBOX_LIST *pData = &(*iter);
		if( pData == NULL )continue;
		//����� ��¼��..�ѤѤ�
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
	for(iter = m_deque_ComboBoxDeck.begin(); iter != m_deque_ComboBoxDeck.end(); ++iter) { //�ߺ��˻�
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
			//���� ��ο� 
			Draw_Number(pDC,&m_Page,pData);
		}	
		
		if( m_ComboBoxInfo.d_type == enum_drawtype_IMG ){
			m_Page.PutSprAuto(rt.left,rt.top,m_ComboBoxInfo.pSpr, (int) pData->nContentData);
		}

		cnt ++;
		if( cnt != total){
			//1���� ����Ʈ ���� 1�ȼ� ���� ������ 1�ȼ��� ���� �̹��� ��ο�
			m_Page.PutSprAuto(rt.left,rt.bottom,m_ComboBoxInfo.pSpr,m_ComboBoxInfo.nUnderLineSprIndex);
		}	
	}
	
	GetClientRect(&rt);	

	//�޺� �ڽ� ��ܿ� 1�ȼ� ���� ������ 1�ȼ��� ���� �̹��� ��ο�	
	m_Page.PutSprAuto(rt.left,rt.top,m_ComboBoxInfo.pSpr,m_ComboBoxInfo.nOutLineSprIndex);

	//�޺� �ڽ� �ϴܿ� 1�ȼ� ���� ������ 1�ȼ��� ���� �̹��� ��ο�
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

	//	pPage->PutSprAuto(rt.left+(i*(With-6))+12 + x,rt.top+3 + y,m_ComboBoxInfo.pSpr,index); �տ��� 12 �����̵��� �׸���
		pPage->PutSprAuto(rt.left+(i*(With-6)) + x,rt.top+3 + y,m_ComboBoxInfo.pSpr,index);
	}

//	m_Page.PutSprAuto(rt.left,rt.bottom,m_ComboBoxInfo.pSpr,0);
}

//������ �����͸� �����Ѵ�.
COMBOBOX_LIST* CComboBoxImageDlg::GetSelectComboBoxContent()
{
	COMBOBOX_LIST* pResultData = NULL;
	deque<COMBOBOX_LIST>::iterator iter;	
	for(iter = m_deque_ComboBoxDeck.begin(); iter != m_deque_ComboBoxDeck.end(); ++iter) { //�ߺ��˻�
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
	CString str; // �ѱ�ǥ��
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
	//��
	else if( str.Compare( "M" ) == 0 ){
		return 11;
	}
	//��
	else if( str.Compare( "Y" ) == 0 ){
		return 12;
	}
	//��
	else if( str.Compare( "J" ) == 0 ){
		return 13;
	}
	//���� ����
	else if( str.Compare( "N" ) == 0 )
	{
		return 18;
	}
	//�� �̻�
	else if( str.Compare( "O" ) == 0 )
	{
		return 19;
	}
	// ����
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
