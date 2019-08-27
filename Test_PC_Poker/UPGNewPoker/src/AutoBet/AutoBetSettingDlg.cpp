// AutoBetSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AutoBetSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define S_X	172
#define S_Y 75
#define XW	38
#define YH  23
/////////////////////////////////////////////////////////////////////////////
// CAutoBetSettingDlg dialog
char strBetRuleTip[40][8] = {
	{"591"},{"592"},{"593"},{"594"},{"595"},
	{"596"},{"597"},{"598"},{"599"},{"59F"},
	{""},{""},{""},{""},{""},

	{"691"},{"692"},{"693"},{"694"},{"695"},
	{"696"},{"697"},{"698"},{"699"},{"69F"},
	{""},{""},{""},{""},{""},

	{"791"},{"792"},{"793"},{"794"},{"795"},
	{"796"},{"797"},{"798"},{"799"},{"79F"},	
};

CAutoBetSettingDlg::CAutoBetSettingDlg(char *ID,int betkind,BOOL nodie,BOOL bnoedit,BOOL bmy,int gamemonde,CWnd* pParent /*=NULL*/)
	: CUPGDlgBase(CAutoBetSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAutoBetSettingDlg)
	//}}AFX_DATA_INIT

	m_bNoEdit = bnoedit; // 방설정인지 자기 설정인지 방이면 설정된값 이상만 설정 가능하게 자기면 맘껏 바꾸게.	0 에디팅 가능 1 에디팅 불가능
	m_nBetRule = betkind;
	m_bNodie = nodie;
	m_strMyID = ID;
	m_bMy = bmy;
	m_nGameMode = gamemonde;

	if(m_nGameMode <= 1)
	{
		m_nMaxBetNum = MAX_BETNUM - 15;
	}
	else
	{
		m_nMaxBetNum = MAX_BETNUM;
	}

	if(m_nBetRule >= m_nMaxBetNum)
	{
		m_nBetRule = m_nMaxBetNum;
	}
}


void CAutoBetSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CUPGDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutoBetSettingDlg)
	DDX_Control(pDX, IDC_COMBO_BETLIST, m_cmbBetList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAutoBetSettingDlg, CUPGDlgBase)
	//{{AFX_MSG_MAP(CAutoBetSettingDlg)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_BETLIST, OnSelchangeComboBetlist)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoBetSettingDlg message handlers

char betliset[6][8] = {
	{"595"},{"59F"},{"695"},{"69F"},{"795"},{"79F"}
};

BOOL CAutoBetSettingDlg::OnInitDialog() 
{
	CUPGDlgBase::OnInitDialog();

	m_cPage.Init(468,293,16);

	m_ctrlOKBtn.Init(this, &m_cPage, 150, 253, &g_sprAutoBetting, 0, IDM_OKBTN);
	m_ctrlNoBtn.Init(this, &m_cPage, 240, 253, &g_sprAutoBetting, 1, IDM_NOBTN);
	

	ZeroMemory(m_listBetDect,sizeof(m_listBetDect));

	m_nTimeEvent = (UINT)SetTimer(BET_TIME,(UINT)(1000.f/10),0);

	m_nRoomBetRule = g_RI.nAutoBetKind;				// 내 배팅 설정 정보

	if(m_nRoomBetRule >= m_nMaxBetNum){
		m_nRoomBetRule = m_nMaxBetNum;
	}

	m_bRoomNodie = g_RI.bNodie;				// 내 설정

	m_nBetClickIndex = 0;	

	SetBettingDeck(m_nBetRule);
	if(m_bNodie){
		int tnum = m_nBetRule;
		if(m_nRoomBetRule > 0 ){
			tnum = m_nRoomBetRule;
		}		
		SetComboIndex(tnum);
		m_bNodie = 1;
		int num = GetGuTotalNum(tnum);
		//노다이일때 콜 셋팅
		SetBettingDeckPutCall(num);		
	}
	else{
		// 콤도 박스를 셋팅한다
		SetComboIndex(m_nBetRule);	
	}

		/////////////////////////////////////////
	
	
	CString strM;
	// 당분간 지존 부터 도신 까지 수동배팅 활성화 한다. 02.21

	char str[6][12]={
		"수동베팅",
		"595",
		"59F",
		"695",
		"69F",
		"795",
	};
	
	for(int i=0; i<6; i++)
	{		
		strM =str[i];
		m_cmbBetList.AddString(strM.operator LPCTSTR());
	}
	m_cmbBetList.SetCurSel(0);
	

	if(m_bNoEdit){
		m_cmbBetList.EnableWindow(FALSE);
	}

	m_strSettingRule = GetAutoBettingRuleName(GAME()->GetGameType(), m_nBetRule, (BETRULE_KIND)g_RI.sBetRuleKind );

	m_bNoEditClick = FALSE;

	RECT rc1;
	GetWindowRect( &rc1 );
	rc1.right = rc1.left + 468;
	rc1.bottom = rc1.top + 293;
	MoveWindow( &rc1 );

	OnInvalid();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAutoBetSettingDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CDC cdc;
	cdc.CreateCompatibleDC(&dc);	

//	HRGN h = BitmapToRegion( Bmp.operator HBITMAP(), RGB(255,0,255));
//	if(h) SetWindowRgn(h,TRUE);// 사용방법

	cdc.SelectObject(m_cPage.hBmp);
	cdc.SetBkMode(TRANSPARENT);
	cdc.SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_12_NORMAL));
	cdc.SetTextColor(RGB(0,0,0));

	Draw(&cdc);

	CRect rect;
	GetClientRect(&rect);

	dc.BitBlt(0,0,rect.Width(),rect.Height(),&cdc,0,0,SRCCOPY);

//	::DeleteObject(h);
	cdc.DeleteDC();	
	
	// TODO: Add your message handler code here
	
	// Do not call CUPGDlgBase::OnPaint() for painting messages
}

void CAutoBetSettingDlg::Draw(CDC *dc)
{		
	m_cPage.PutSprAuto(0,0,&g_sprAutoBetting,8);	

	for(int i = 0 ; i < BETLIST_W ; i ++){
		
		for(int j = 0 ; j < BETLIST_H ; j ++){

			int index = (i*5)+j;
			if( m_listBetDect[index] == 1){
				if((index >= 10 && index <= 14)||(index >= 25 && index <= 29) || (index >= 40 && index < MAX_BETNUM))
				{
					m_cPage.PutSprAuto(S_X+(i*XW),S_Y+(j*YH),&g_sprAutoBetting,10);
				}
				else
					m_cPage.PutSprAuto(S_X+(i*XW),S_Y+(j*YH),&g_sprAutoBetting,9);
			}
			else if(m_listBetDect[index] == 2){
				m_cPage.PutSprAuto(S_X+(i*XW),S_Y+(j*YH),&g_sprAutoBetting,10);
			}
		}
	}

	if(m_bNodie){
		m_cPage.PutSprAuto(369,19,&g_sprAutoBetting,12);
	}

	if(m_bMy)
	{		
		m_cPage.PutSprAuto(171,20,&g_sprAutoBetting,14);
	}
	else	
	{
		m_cPage.PutSprAuto(171,20,&g_sprAutoBetting,14);
	}

	if(m_bNoEditClick)
	{
		m_cPage.PutSprAuto(25,199,&g_sprAutoBetting,15);
	}
	else
	{
		m_cPage.PutSprAuto(25,199,&g_sprAutoBetting,16);
	}

	if(m_strSettingRule.GetLength()>0)
	{
		CRect rt;
		rt.SetRect(0,0,75,18);
		rt.OffsetRect(84,22);
		
		dc->DrawText(m_strSettingRule,&rt,DT_CENTER | DT_WORDBREAK);
	}

	m_ctrlOKBtn.Draw(dc);
	m_ctrlNoBtn.Draw(dc);	
}

void CAutoBetSettingDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_nTimeEvent == nIDEvent){
		//화면 갱신		
		CRect rt;
		rt.SetRect(0,0,200,29);
		rt.OffsetRect(150,253);
		InvalidateRect(&rt,FALSE);				
	}
	
	CUPGDlgBase::OnTimer(nIDEvent);
}

void CAutoBetSettingDlg::OnOK() 
{
	// TODO: Add extra validation here	
	GetBettingTotalNum();
	CUPGDlgBase::OnOK();
}

void CAutoBetSettingDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_ctrlOKBtn.OnLButtonDown(point.x, point.y);			//수호천사 버튼	
	m_ctrlNoBtn.OnLButtonDown(point.x, point.y);			//

	CPoint pt;
	pt = point;		

	// 배팅 종류 클릭 부분
	if(pt.x >= 173 && pt.x <= 269+173 && pt.y >= 75 && pt.y <= 114+75){
		
		m_nBetClickIndex = (((pt.x - S_X) / XW) * 5) + ((pt.y - S_Y) / YH);	
		if(m_nBetClickIndex >= 35 )goto RETURN;		
		
		// 콜 위치는 클릭 못하게 한다.
		if(m_nBetClickIndex >= 10 && m_nBetClickIndex < 15 )goto RETURN;
		else if(m_nBetClickIndex >= 25 && m_nBetClickIndex < 30)goto RETURN;		
		else if(m_nBetClickIndex >=40 && m_nBetClickIndex <m_nMaxBetNum)goto RETURN;

		if(m_nBetClickIndex < 40)SetCursor(GM().GetCurHandCursor());
		
		if(m_nBetClickIndex >= 0 && m_nBetClickIndex < m_nMaxBetNum){
			
			if(m_bNoEdit == FALSE)
			{
				if(m_nBetClickIndex >= 0 && m_nBetClickIndex < 45){										
					
					if(m_listBetDect[m_nBetClickIndex+1] == 0 && m_listBetDect[m_nBetClickIndex-1] == 1)
					{
						m_listBetDect[m_nBetClickIndex] = m_listBetDect[m_nBetClickIndex] == 0 ? 1:0;
					}
					else if(m_nBetClickIndex == 0 && m_listBetDect[m_nBetClickIndex+1] == 0)
					{
						m_listBetDect[m_nBetClickIndex] = m_listBetDect[m_nBetClickIndex] == 0 ? 1:0;
					}
					else if(m_nBetClickIndex == 44 && m_listBetDect[m_nBetClickIndex-1] == 1)
					{
						m_listBetDect[m_nBetClickIndex] = m_listBetDect[m_nBetClickIndex] == 0 ? 1:0;
					}					
					else SetBettingDeck(m_nBetClickIndex+1);										
				}
			}
			// 콤도 박스를 셋팅한다		
			int cnt = 0;
			for(int i =0 ; i < MAX_BETNUM; i ++){
				if(m_listBetDect[i] == 1){
					cnt++;
				}
			}	
			
			SetComboIndex(cnt);

			m_strSettingRule = GetAutoBettingRuleName(GAME()->GetGameType(), cnt, (BETRULE_KIND)g_RI.sBetRuleKind );
		}	
	}
	else
	{
RETURN:
		m_bNoEditClick = FALSE;		
		SetCursor(GM().GetCurSelCursor());
	}

	// 노다이 채크 부분
	if(pt.x >= 369 && pt.x <= 369+61 && pt.y >= 19 && pt.y <= 19+16){
		if(m_nBetClickIndex >= 0 && m_bNoEdit == FALSE)
		{
			SetCursor(GM().GetCurHandCursor());

			int cnt = 0;
			int i=0;
			for(i =0 ; i < MAX_BETNUM; i ++){
				if(m_listBetDect[i] == 1){
					cnt++;
				}
			}			

			if(cnt >= 30)return;

			m_bNodie = m_bNodie == 0 ? 1:0;
			int ind = m_cmbBetList.GetCurSel();

			
			if(m_bNodie)
			{			
				// 콜 그린다
				switch(ind)
				{
				case 1:
					SetBettingDeckPutCall(15,0);
					break;
				case 2:
					SetBettingDeckPutCall(15,0,TRUE);
					break;
				case 3:
					SetBettingDeckPutCall(30,0);
					break;
				case 4:
					SetBettingDeckPutCall(30,0,TRUE);
					break;
				case 5:
					SetBettingDeckPutCall(45,0);
					break;
				case 6:
					SetBettingDeckPutCall(45,0,TRUE);
					break;
				}
			}
			else 
			{
				// 지운다 
				switch(ind)
				{
				case 1:
				case 2:
					SetBettingDeckPutCall(15,2);
					break;
				case 3:
				case 4:
					SetBettingDeckPutCall(30,2);
					break;
				case 5:
				case 6:
					SetBettingDeckPutCall(45,2);
					break;
				}
			}

			cnt = 0;
			for( i =0 ; i < MAX_BETNUM; i ++){
				if(m_listBetDect[i] == 1){
					cnt++;
				}
			}			

			m_strSettingRule = GetAutoBettingRuleName(GAME()->GetGameType(), cnt, (BETRULE_KIND)g_RI.sBetRuleKind);
		}		
		else
		{
			SetCursor(GM().GetCurSelCursor());
		}
	}

	OnInvalid();

	if(point.y <= 114+75)
	{
		PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
	}
	
	CUPGDlgBase::OnLButtonDown(nFlags, point);
}

void CAutoBetSettingDlg::OnSelchangeComboBetlist() 
{
	// TODO: Add your control notification handler code here	
	int Num = m_cmbBetList.GetCurSel();		
	if(g_RI.State == 1 && Num > 5)
	{
		m_cmbBetList.SetCurSel(5);
		Num = 5;
	}
	
	// 당분간 지존 부터 도신 까지 수동배팅 활성화 한다. 02.21	
	if(m_bNoEdit)
	{		
		switch(Num)
		{
		case 1:if(m_nRoomBetRule > 5)SetComboIndex(m_nRoomBetRule);break;
		case 2:if(m_nRoomBetRule > 15)SetComboIndex(m_nRoomBetRule);break;			
		case 3:if(m_nRoomBetRule > 20)SetComboIndex(m_nRoomBetRule);break;
		case 4:if(m_nRoomBetRule > 30)SetComboIndex(m_nRoomBetRule);break;		
		case 5:if(m_nRoomBetRule > 35)SetComboIndex(m_nRoomBetRule);break;
		case 6:if(m_nRoomBetRule > 45)SetComboIndex(m_nRoomBetRule);break;		
		}

		m_strSettingRule = GetAutoBettingRuleName(GAME()->GetGameType(), m_nRoomBetRule, (BETRULE_KIND)g_RI.sBetRuleKind );
	}
	else
	{
		int i = 0;	
		m_bNodie = FALSE;
		switch(Num)
		{
		case 1:
			SetBettingDeck(5);
			i = 5;
			break;
		case 2:
			SetBettingDeck(15);
			m_bNodie = TRUE;
			i = 15;
			break;
			
		case 3:
			SetBettingDeck(20);
			i = 20;
			break;
		case 4:
			SetBettingDeck(30);
			m_bNodie = TRUE;
			i = 30;
			break;		
		case 5:
			{
				if(m_nGameMode > 1){
					SetBettingDeck(35);
					i = 35;
				}
				else
				{
					SetComboIndex(m_nMaxBetNum);
					i = m_nMaxBetNum;
				}
			}
			break;
			/*
		case 6:
			{
				if(m_nGameMode > 1){
					SetBettingDeck(45);
					i = 45;
					m_bNodie = FALSE;
				}
				else
				{
					SetComboIndex(m_nMaxBetNum);
					i = m_nMaxBetNum;
				}
			}			
			break;
			*/
		default:
			ZeroMemory(m_listBetDect,sizeof(m_listBetDect));
			m_bNodie = FALSE;
			//		m_nBetRule = 0;
			break;
		}

		m_strSettingRule = GetAutoBettingRuleName(GAME()->GetGameType(), i, (BETRULE_KIND)g_RI.sBetRuleKind );
	}

	OnInvalid();
}

void CAutoBetSettingDlg::SetComboIndex(int num)
{
	int index = 0 ;	
	
	if(num > 0 && num <=9){
		m_bNodie = FALSE;
		index = 1;	
	}
	else if(num >= 10 && num <= 15 )
	{
		if(num == 15 && !m_bNoEdit)m_bNodie = TRUE;
		else if( num == 10 && !m_bNoEdit)m_bNodie = FALSE;
		index = 2;
	}
	else if(num > 15 && num <= 24)
	{
	//	if(num >= 16 && !m_bNoEdit)m_bNodie = FALSE;
		m_bNodie = FALSE;
		index = 3;
	}
	else if(num >= 25 && num <= 30)
	{
		if(num == 30)m_bNodie = TRUE;
		else if( num == 25 && !m_bNoEdit)m_bNodie = FALSE;
		index = 4;		
	}
	else if(num > 30 && num <=39)
	{
	//	if(num >= 31 && !m_bNoEdit)m_bNodie = FALSE;
		m_bNodie = FALSE;
		index = 5;
	}
	else if(num >=40 && num <=MAX_BETNUM)
	{
		if(num == 45 && !m_bNoEdit)m_bNodie = TRUE;
		else if( num == 40 && !m_bNoEdit)m_bNodie = FALSE;
		index = 6;
	}
	
	m_cmbBetList.SetCurSel(index);	

}

void CAutoBetSettingDlg::SetBettingDeck(int num)
{
	ZeroMemory(m_listBetDect,sizeof(m_listBetDect));

	for(int i =0 ; i < num; i ++){
		m_listBetDect[i] = 1;
	}	
}

int CAutoBetSettingDlg::GetBettingTotalNum()
{	
	m_nBetRule = 0;
	for(int i =0 ; i < m_nMaxBetNum; i ++){
		if(m_listBetDect[i] == 1){
			m_nBetRule++;
		}
	}	
	return m_nBetRule;
}

void CAutoBetSettingDlg::SetBettingDeckPutCall(int num,int cnum,BOOL bguf)
{
	if( cnum == 2)
	{
		m_listBetDect[num-1] = 2;
		m_listBetDect[num-2] = 2;
		m_listBetDect[num-3] = 2;
		m_listBetDect[num-4] = 2;
		m_listBetDect[num-5] = 2;
	}

	for(int i =0 ; i < num; i ++){
		if(cnum == 2)
		{
			if(m_listBetDect[i] == cnum){
				m_listBetDect[i] = 0;
			}
		}
		else
		{
			if(m_listBetDect[i] == cnum){
				if(bguf)m_listBetDect[i] = 1;
				else m_listBetDect[i] = 2;				
			}
		}		
	}	
}

int CAutoBetSettingDlg::GetGuTotalNum(int num)
{
	int tgu = 0;	

	if(num > 0 && num <=9)tgu = 15;
	else if(num >= 10 && num <= 15 )tgu = 15;
	else if(num > 15 && num <= 24)tgu = 30;
	else if(num >= 25 && num <= 30)tgu = 30;
	else if(num > 30 && num <=39)tgu = 45;
	else if(num >=40 && num <=MAX_BETNUM)tgu = 45;

	return tgu;
}

void CAutoBetSettingDlg::SetNoEditModeBettingDeck(int num)
{
	SetBettingDeck(num);
	int cnum = GetGuTotalNum(m_nRoomBetRule);
		//노다이일때 콜 셋팅
	SetBettingDeckPutCall(cnum);	
}

void CAutoBetSettingDlg::OnInvalid()
{
	CRect rt;	
	rt.SetRect(0,0,468,293);
	
	this->InvalidateRect(&rt,FALSE);		
}

void CAutoBetSettingDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	if(point.x >= 173 && point.x <= 269+173 && point.y >= 75 && point.y <= 114+75){
		
		int index  = (((point.x - S_X) / XW) * 5) + ((point.y - S_Y) / YH);	
		if(index >= m_nMaxBetNum )return;		
		// 콜 위치는 클릭 못하게 한다.
		if(index >= 10 && index < 15 )return;
		else if(index >= 25 && index < 30)return;		
		else if(index >=40)return;
		

		SetCursor(GM().GetCurHandCursor());

	}
	else if(point.x >= 369 && point.x <= 369+61 && point.y >= 19 && point.y <= 19+16){
		SetCursor(GM().GetCurHandCursor());		
	}
	else
	{		
		SetCursor(GM().GetCurSelCursor());
	}
	
	m_ctrlOKBtn.OnMouseMove(point.x, point.y);	
	m_ctrlNoBtn.OnMouseMove(point.x, point.y);	

	CUPGDlgBase::OnMouseMove(nFlags, point);
}

BOOL CAutoBetSettingDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class

	switch(wParam)
	{
	case IDM_OKBTN:		
		OnOK();
		break;
	case IDM_NOBTN:
		CUPGDlgBase::OnCancel();
		break;
	}
	
	
	return CUPGDlgBase::OnCommand(wParam, lParam);
}

void CAutoBetSettingDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_ctrlOKBtn.OnLButtonUp(point.x, point.y);			//수호천사 버튼	
	m_ctrlNoBtn.OnLButtonUp(point.x, point.y);			//
	
	CUPGDlgBase::OnLButtonUp(nFlags, point);
}

BOOL CAutoBetSettingDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	if( pMsg->message == WM_KEYDOWN )	
	{
		if( pMsg->wParam == VK_ESCAPE)
		{
			CUPGDlgBase::OnCancel();
			return 1;
		}
	}
	
	return CUPGDlgBase::PreTranslateMessage(pMsg);
}
