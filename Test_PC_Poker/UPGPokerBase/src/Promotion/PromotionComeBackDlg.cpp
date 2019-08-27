// D:\Data\game\Poker\UPGPokerBase\src\Promotion\PromotionComeBackDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "PromotionComeBackDlg.h"
#include "../GlobalPokerBase.h"


// CPromotionComeBackDlg 대화 상자입니다.


CPromotionComeBackDlg::CPromotionComeBackDlg(CWnd* pParent /*=NULL*/)
	: CUPGDlgBase(CPromotionComeBackDlg::IDD, pParent)
{

}

CPromotionComeBackDlg::~CPromotionComeBackDlg()
{
}

void CPromotionComeBackDlg::DoDataExchange(CDataExchange* pDX)
{
	CUPGDlgBase::DoDataExchange(pDX);
	DDX_Control(pDX, ID_COMEBACK_OK, m_btnComeBack);
	DDX_Control(pDX, IDCANCEL, m_btnClose);
}


BEGIN_MESSAGE_MAP(CPromotionComeBackDlg, CUPGDlgBase)
	ON_BN_CLICKED(ID_COMEBACK_OK, &CPromotionComeBackDlg::OnBnClickedComebackOk)
	ON_BN_CLICKED(IDCANCEL, &CPromotionComeBackDlg::OnBnClickedCancel)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CPromotionComeBackDlg 메시지 처리기입니다.

BOOL CPromotionComeBackDlg::OnInitDialog()
{
	CUPGDlgBase::OnInitDialog();
	//m_BackBit.LoadBitmapFile(".\\PokerCommon\\data\\Event\\ComeBack\\back.bmp");
	//m_BackBit.ResizeWnd( this, 0, 0, TRUE, m_pParentWnd );

	m_sprBack.Load(".\\PokerCommon\\data\\Event\\ComeBack\\comeback.spr");
	m_cPage.Init(m_sprBack.spr->xl, m_sprBack.spr->yl, 16);
	CRect rc;
	GetClientRect( rc );
	rc.right = rc.left + m_sprBack.spr->xl;
	rc.bottom = rc.top + m_sprBack.spr->yl;
	MoveWindow(rc, TRUE);

	CenterWindow(GM().GetMainWnd());

	m_btnComeBack.Init( 159, 378, ".\\PokerCommon\\data\\Event\\ComeBack\\back_button.bmp", 4 );
	m_btnClose.Init( 478, 8, ".\\PokerCommon\\data\\Event\\ComeBack\\back_X.bmp", 4 );
	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CPromotionComeBackDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CUPGDlgBase::OnPaint()을(를) 호출하지 마십시오.
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);	
	cdc.SelectObject(m_cPage.hBmp);
	cdc.SetBkMode(TRANSPARENT);
	cdc.SetTextColor(RGB(0,0,0));	
	m_cPage.PutSprAuto(0, 0, &m_sprBack, 0);
	CPoint cpStart(99, 219);
	CPoint cpDiff(20+58, 19+59);
	for(int i=0; i<MAX_RETENTION_MISSION_DAY; i++){
		if(0!=GM().GetRETENTION_ALRAM()->nMissionComplet[i]){
			m_cPage.PutSprAuto(cpStart.x+cpDiff.x*(i%5), cpStart.y+cpDiff.y*(i/5), &m_sprBack, 1, AB);
		}
	}
	CRect rt;
	GetClientRect(&rt);
	dc.BitBlt(0,0,rt.Width(),rt.Height(),&cdc,0,0,SRCCOPY);
	cdc.DeleteDC();
}

void CPromotionComeBackDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
	CUPGDlgBase::OnLButtonDown(nFlags, point);
}


void CPromotionComeBackDlg::OnBnClickedComebackOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	if(true == GM().GetRETENTION_ALRAM()->bToday){
		CString strOutput;
		strOutput.Format("오늘은 출석 완료하였습니다.\n내일도 출석해주세요.\n");
		ShowMainMessageDlg(strOutput);
	}
	else if(true == GM().IsEnableComeBackPromotion()){
		CSV_ASK_RETENTION_REWARD msg;
		msg.Set(0);
		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());	
	}
	else{
		CString strOutput;
		strOutput.Format("출석 미션을 완료해주세요.\n%d판이 남았습니다.\n", GM().GetRETENTION_ALRAM()->nTotalWinCnt-GM().GetRETENTION_ALRAM()->nTodayWinCnt);
		ShowMainMessageDlg(strOutput);
	}
}

void CPromotionComeBackDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	EndDialog(IDCANCEL);
}

BOOL CPromotionComeBackDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return TRUE;
	//return CUPGDlgBase::OnEraseBkgnd(pDC);
}
