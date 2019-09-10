// D:\Data\game\Poker\UPGPokerBase\src\Promotion\PromotionComeBackDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "PromotionComeBackDlg.h"
#include "../GlobalPokerBase.h"


// CPromotionComeBackDlg ��ȭ �����Դϴ�.


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


// CPromotionComeBackDlg �޽��� ó�����Դϴ�.

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
	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CPromotionComeBackDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	// �׸��� �޽����� ���ؼ��� CUPGDlgBase::OnPaint()��(��) ȣ������ ���ʽÿ�.
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
	CUPGDlgBase::OnLButtonDown(nFlags, point);
}


void CPromotionComeBackDlg::OnBnClickedComebackOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	
	if(true == GM().GetRETENTION_ALRAM()->bToday){
		CString strOutput;
		strOutput.Format("������ �⼮ �Ϸ��Ͽ����ϴ�.\n���ϵ� �⼮���ּ���.\n");
		ShowMainMessageDlg(strOutput);
	}
	else if(true == GM().IsEnableComeBackPromotion()){
		CSV_ASK_RETENTION_REWARD msg;
		msg.Set(0);
		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());	
	}
	else{
		CString strOutput;
		strOutput.Format("�⼮ �̼��� �Ϸ����ּ���.\n%d���� ���ҽ��ϴ�.\n", GM().GetRETENTION_ALRAM()->nTotalWinCnt-GM().GetRETENTION_ALRAM()->nTodayWinCnt);
		ShowMainMessageDlg(strOutput);
	}
}

void CPromotionComeBackDlg::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	EndDialog(IDCANCEL);
}

BOOL CPromotionComeBackDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	return TRUE;
	//return CUPGDlgBase::OnEraseBkgnd(pDC);
}
