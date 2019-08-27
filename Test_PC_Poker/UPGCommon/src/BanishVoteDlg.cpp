#include "stdafx.h"
#include "BanishVoteDlg.h"
#include "ConfirmVote.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CBanishVoteDlg::CBanishVoteDlg(CWnd* pParent /*=NULL*/)
	: CUPGDlgBase(CBanishVoteDlg::IDD, pParent)
{
	m_Reson = _T("");
	m_HollowBrush.CreateStockObject(HOLLOW_BRUSH);
	for( int i = 0 ; i < 30 ; i++ )
		TarNickName[i] = "";

	Direct_TarID = "";

	m_IDcnt=0;
	m_Index_Tarid = -1;
}

void CBanishVoteDlg::DoDataExchange(CDataExchange* pDX)
{
	CUPGDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBanishVoteDlg)
	DDX_Control(pDX, IDCANCEL, m_CancelBtn);
	DDX_Control(pDX, IDOK, m_OkBtn);
	DDX_Control(pDX, IDC_COMBO_TARGETID, m_TargetIDCombo);
	DDX_CBString(pDX, IDC_COMBO_RESON, m_Reson);
	DDV_MaxChars(pDX, m_Reson, 40);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBanishVoteDlg, CUPGDlgBase)
	//{{AFX_MSG_MAP(CBanishVoteDlg)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_COMBO_RESON, &CBanishVoteDlg::OnCbnSelchangeComboReson)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBanishVoteDlg message handlers

BOOL CBanishVoteDlg::OnInitDialog() 
{
	CUPGDlgBase::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_OkBtn.Init(68,218,".\\common\\image\\vote\\btn_banishment.BMP", 4);
	m_CancelBtn.Init(163,218,".\\common\\image\\commonbtn\\Btn_cancel.bmp",4);

	NMBASE::SKINGDI::WindowResize2Bitmap(m_hWnd, ".\\common\\image\\vote\\banishvote.BMP");

	CRect rt;
	rt.SetRect(43,56,178,300);
	GetDlgItem(IDC_COMBO_TARGETID)->MoveWindow(&rt);


	rt.SetRect(42,170,278,190);
	GetDlgItem(IDC_COMBO_RESON)->MoveWindow(&rt);
	
	if( m_Index_Tarid > -1 ){
		m_TargetIDCombo.SetCurSel(m_Index_Tarid);
	}	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBanishVoteDlg::AddUser(const char *pID, const char *pNick)
{
	if (MAX_BANISHVOTE_USER<=m_IDcnt || m_IDcnt<0)
		return;

	m_TargetIDCombo.AddString(pNick);
	TarID[m_IDcnt] = pID;
	TarNickName[m_IDcnt] = pNick;

	if( !Direct_TarID.IsEmpty() )
	{
		if( Direct_TarID.Compare(TarID[m_IDcnt]) == 0 )
		{
			m_Index_Tarid = m_IDcnt;
		}
	}

	m_IDcnt++;

}
void CBanishVoteDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
/* 다이얼로그 밖으로 뺌
	int nowsel = m_TargetIDCombo.GetCurSel();
	if(nowsel >= 0)
	{
		if(m_Reson.GetLength()==0)
		{
			ShowMainMessageDlg(false , "추방할 이유를 직접 기입하거나 리스트에서 선택하세요");
			
			return;
		}
		else if(GM().GetCurrentProcess()->GetPlayerPNum((char*)TarID[nowsel].operator LPCTSTR()) == -1)
		{
			ShowMainMessageDlg("존재하지 않는 사용자입니다");
		}
		else if(g_RI.NowUserNum < LIMITGAMEPLAY_PLAYERNUM)
		{
			ShowMainMessageDlg("3명 이상인 경우만 추방 투표가 가능합니다");
			return;
		}
		else
		{

			if(g_RI.bSuperMaster ) {
				if(strncmp(g_RI.ID, TarID[nowsel], 15)==0) {
					ShowMainMessageDlg("슈퍼방장은 추방 할 수 없습니다.");
					return;
				}
			}		

			BANISHVOTEINFO bv = {0,};
			bv.RoomNum = g_RI.RoomNum;
			strncpy(bv.MyID, PLAYER(0)->UI.ID, 15);
			strncpy(bv.TarID, (LPCTSTR)TarID[nowsel], 15);
			strncpy(bv.Reson, (LPCTSTR)m_Reson, 41);

			CSV_ASK_BANISHVOTE abvmsg;
			abvmsg.Set(&bv);
			NMBASE::SOCK::GSOCKMAN().SendData(g_MainSrvSID, abvmsg.pData, abvmsg.GetTotalSize());
		}
	}
	else{
		ShowMainMessageDlg(false ,"추방을 원하는 아이디를 선택하세요");	
		return;
	}
*/
	CUPGDlgBase::OnOK();
}

void CBanishVoteDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CUPGDlgBase::OnCancel();
}

void CBanishVoteDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);

	NMBASE::SKINGDI::CMyBitmap Back;
	Back.LoadBitmapFile(".\\common\\image\\vote\\banishvote.BMP");

	HRGN h = NMBASE::SKINGDI::BitmapToRegion( Back.operator HBITMAP(), RGB(255,0,255));
	if(h) SetWindowRgn(h,TRUE);// 사용방법

	CBitmap *pOldBmp = cdc.SelectObject(&Back);	

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);

	cdc.SelectObject(pOldBmp);
	if(h)::DeleteObject(h);
	h = NULL;
	cdc.DeleteDC();
	Back.DeleteObject();

	// TODO: Add your message handler code here
	
	// Do not call CUPGDlgBase::OnPaint() for painting messages
}

void CBanishVoteDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
	CUPGDlgBase::OnLButtonDown(nFlags, point);	
}

HBRUSH CBanishVoteDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CUPGDlgBase::OnCtlColor(pDC, pWnd, nCtlColor);
	
	/*
	// TODO: Change any attributes of the DC here	
	switch(nCtlColor) {
		case CTLCOLOR_EDIT:
		//	pDC->SetBkMode(TRANSPARENT);
			pDC->SetBkColor(RGB(45,45,45));
			pDC->SetTextColor(RGB(255,255,255));
			break;		
		default:
			break;
	}
	// TODO: Return a different brush if the default is not desired
	*/
	return hbr;
}

int CBanishVoteDlg::DoModal(char *id) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( id == NULL )return 0;
	Direct_TarID = id;	
	return CUPGDlgBase::DoModal();
}

void CBanishVoteDlg::OnCbnSelchangeComboReson()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
