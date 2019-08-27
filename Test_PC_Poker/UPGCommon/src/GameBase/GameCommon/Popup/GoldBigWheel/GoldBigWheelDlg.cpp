#include "stdafx.h"
#include "GoldBigWheelDlg.h"
#include "goldbigwheelid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CGoldBigWheelDlg::CGoldBigWheelDlg(CWnd* pParent/* = NULL*/)
:	LSY::CLSYLibDialogBase(pParent),
	m_pWebBigWheel(NULL),
	m_pBtnStart(NULL),
	m_pBtnStop(NULL),
	m_pBtnMinus(NULL),
	m_pBtnPlus(NULL),
	m_pBtnClose(NULL),
	m_pTxtMoney(NULL),
	m_pTxtGold(NULL),
	m_pImgEnterGold(NULL),
	m_pImgAutoCnt(NULL),
	m_pImgRemainCnt(NULL),
	m_enterGold(0),
	m_autoCnt(1),
	m_remainCnt(0)
{
}

BEGIN_MESSAGE_MAP(CGoldBigWheelDlg, LSY::CLSYLibDialogBase)
	ON_WM_TIMER()
END_MESSAGE_MAP()

INT_PTR CGoldBigWheelDlg::DoModal(int remainCnt, INT64 useGold, CWnd *pParent)
{
	m_remainCnt = remainCnt;
	m_enterGold = useGold;
	m_pParentWnd = pParent;
	return CLSYLibDialogBase::DoModal(GetObjectMan(), ID_GOLDBIGWHEELDLG);
}

LRESULT CGoldBigWheelDlg::OnInit(WPARAM &wParam, LPARAM &lParam)
{
	Init();

	return (LRESULT)wParam;
}

void CGoldBigWheelDlg::Init()
{
	m_pWebBigWheel->SetCallback_OnBeforeNavigate2(SSO_OnBeforeNavigate2);
	m_pWebBigWheel->SetURL(getGameURL(IDX_GAMEURL_GOLD_BIGWHEEL).GetString(), true);

	m_pBtnStart->SetShow(true);
	m_pBtnStart->SetEnable(false);
	m_pBtnStop->SetShow(false);

	m_pTxtMoney->SetNumber(GM().GetMyInfo()->UI.GetRealPMoney());
	m_pTxtGold->SetNumber(GM().GetMyGold());

	m_pImgEnterGold->SetNumber(m_enterGold);
	m_pImgRemainCnt->SetNumber(m_remainCnt);
	SetAutoCnt(1);
}

bool CGoldBigWheelDlg::OnCreatedProject(void)
{	
	m_pWebBigWheel = (LSY::CWeb *)GetObject(ID_GOLDBIGWHEELDLG_WEB);

	m_pBtnStart = (LSY::CButton *)GetObject(ID_GOLDBIGWHEELDLG_BUTTON_START);
	m_pBtnStart->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldBigWheelDlg::OnClickButton));

	m_pBtnStop = (LSY::CButton *)GetObject(ID_GOLDBIGWHEELDLG_BUTTON_STOP);
	m_pBtnStop->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldBigWheelDlg::OnClickButton));

	m_pBtnMinus = (LSY::CButton *)GetObject(ID_GOLDBIGWHEELDLG_BUTTON_MINUS);
	m_pBtnMinus->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldBigWheelDlg::OnClickButton));

	m_pBtnPlus = (LSY::CButton *)GetObject(ID_GOLDBIGWHEELDLG_BUTTON_PLUS);
	m_pBtnPlus->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldBigWheelDlg::OnClickButton));

	m_pBtnClose = (LSY::CButton *)GetObject(ID_GOLDBIGWHEELDLG_BUTTON_CLOSE);
	m_pBtnClose->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldBigWheelDlg::OnClickButton));

	m_pTxtMoney = (LSY::CTextNumber *)GetObject(ID_GOLDBIGWHEELDLG_TEXTNUMBER_MONEY);
	m_pTxtGold = (LSY::CTextNumber *)GetObject(ID_GOLDBIGWHEELDLG_TEXTNUMBER_GOLD);

	m_pImgEnterGold = (LSY::CImageNumber *)GetObject(ID_GOLDBIGWHEELDLG_IMAGENUM_ENTER_GOLD);
	m_pImgAutoCnt = (LSY::CImageNumber *)GetObject(ID_GOLDBIGWHEELDLG_IMAGENUM_AUTOCOUNT);
	m_pImgRemainCnt = (LSY::CImageNumber *)GetObject(ID_GOLDBIGWHEELDLG_IMAGENUM_REMAINCOUNT);

	m_pTxtRemainCnt = (LSY::CText *)GetObject(ID_GOLDBIGWHEELDLG_TEXT_REMAIN);

	Init();
	
	return TRUE;
}

LRESULT CGoldBigWheelDlg::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	case ID_GOLDBIGWHEELDLG_BUTTON_START:
		NMBASE::SOUND::g_Sound.PlayWav(SND_BUTTON_CLICK_D);
		m_remainAutoCnt = m_autoCnt;
		Start();
		break;
	case ID_GOLDBIGWHEELDLG_BUTTON_STOP:
		NMBASE::SOUND::g_Sound.PlayWav(SND_BUTTON_CLICK_D);
		m_remainAutoCnt = 0;
		m_pBtnStop->SetShow(false);
		break;
	case ID_GOLDBIGWHEELDLG_BUTTON_CLOSE:
		NMBASE::SOUND::g_Sound.PlayWav(SND_BUTTON_CLICK_C);
		ClosePopup();
		break;
	case ID_GOLDBIGWHEELDLG_BUTTON_MINUS:
		NMBASE::SOUND::g_Sound.PlayWav(SND_BUTTON_CLICK_E);
		SetAutoCnt(m_autoCnt-1);
		break;
	case ID_GOLDBIGWHEELDLG_BUTTON_PLUS:
		NMBASE::SOUND::g_Sound.PlayWav(SND_BUTTON_CLICK_E);
		SetAutoCnt(m_autoCnt+1);
		break;
	}
	return TRUE;
}

BOOL CGoldBigWheelDlg::PreTranslateMessage( MSG* pMsg )
{
	switch( pMsg->message )
	{
		//ALT + F4 방지
		case WM_SYSKEYDOWN:
		{
			if(pMsg->wParam == VK_F4)
				return TRUE;
		} break;
	}

	if ( GM().GetPlayGoldBigWheel() == true )
	{
		switch( pMsg->message )
		{
			case WM_KEYDOWN:
			{
				switch(pMsg->wParam)
				{
					case VK_ESCAPE:
						return TRUE;
				}			
			} break;
		}
	}

	return CLSYLibDialogBase::PreTranslateMessage( pMsg );
}

void CGoldBigWheelDlg::WheelLoadCompleted()
{
	m_nTimeEvent = (UINT)this->SetTimer(111, 500, NULL);
}

void CGoldBigWheelDlg::WheelCompleted()
{
	GM().SetPlayGoldBigWheel(false);
	MM().Call(UPG_CMK_REFRESH_MYMONEY);
	m_pTxtMoney->SetNumber(GM().GetMyInfo()->UI.GetMoney());
}

void CGoldBigWheelDlg::WheelNext()
{
	// 한도 초과 팝업을 딜레이 시켜야 할 경우
	if (GM().GetWaitOwnLimitPopup() == true)
	{
		ClosePopup();
		MM().Call(UPG_CMK_SHOW_GOLD_OWNLIMIT_POPUP);
	}
	else if(m_remainAutoCnt > 0 && IsPossibleStart())
	{
		Start();
	}
	else
	{
		m_pWebBigWheel->ExecJavascript("init()");
		m_pBtnClose->SetEnable(true);
		m_pBtnStop->SetShow(false);
		m_pBtnStart->SetEnable(true);
		m_pBtnMinus->SetEnable(IsPossibleStart());
		m_pBtnPlus->SetEnable(IsPossibleStart());
		SetAutoCnt(1);
	}
}

void CGoldBigWheelDlg::Start()
{
	CCL_PM_BIGWHEEL_PLAY_GAME cMsg;
	cMsg.Set(m_enterGold);
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), cMsg.pData, cMsg.GetTotalSize());
	m_pBtnStart->SetEnable(false);
	m_pBtnMinus->SetEnable(false);
	m_pBtnPlus->SetEnable(false);
	m_pBtnClose->SetEnable(false);

	GM().SetPlayGoldBigWheel(true);
}

void CGoldBigWheelDlg::StartWheel(int remainCnt, INT64 useGold, INT64 getMoney)
{
	CString changeVal;
	changeVal.Format("changeVal(""%I64d"")", getMoney);
	m_pWebBigWheel->ExecJavascript("init()");
	m_pWebBigWheel->ExecJavascript(changeVal.GetString());
	m_pWebBigWheel->ExecJavascript("start()");
	m_remainCnt = remainCnt;
	m_pImgRemainCnt->SetNumber(m_remainCnt);

	CString remainVal;
	m_remainAutoCnt--;
	remainVal.Format("%d회 남음", m_remainAutoCnt);
	m_pTxtRemainCnt->SetText(remainVal.GetString());
	m_pBtnStop->SetShow(m_remainAutoCnt > 0);
	m_pTxtGold->SetNumber(GM().GetMyGold());
}

void CGoldBigWheelDlg::SetAutoCnt(int autoCnt)
{
	m_autoCnt = autoCnt;
	if (m_autoCnt <= 0)				m_autoCnt = static_cast<int>(m_remainCnt);
	if (m_autoCnt > m_remainCnt)	m_autoCnt = 1;

	m_autoCnt = min(m_autoCnt, static_cast<int>(GM().GetMyGold()/m_enterGold));

	if(m_autoCnt < 1)
	{
		m_pBtnStart->SetEnable(false);
		m_pBtnMinus->SetEnable(false);
		m_pBtnPlus->SetEnable(false);
		m_autoCnt = 1;
	}

	m_pImgAutoCnt->SetNumber(m_autoCnt);
}

void CGoldBigWheelDlg::ClosePopup()
{
	OnCancel();
}

bool CGoldBigWheelDlg::IsPossibleStart()
{
	return GM().GetMyGold()>=m_enterGold && GM().GetMyInfo()->UI.GetMoney() < g_MaxServerMoney && m_remainCnt > 0;
}

void CGoldBigWheelDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_nTimeEvent == nIDEvent){
		//화면 갱신		
		m_pBtnStart->SetEnable(true);	
		KillTimer(nIDEvent);
	}

	CLSYLibDialogBase::OnTimer(nIDEvent);
}