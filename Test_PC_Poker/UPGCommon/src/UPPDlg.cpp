// UPPDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "UPPDlg.h"
#include "GlobalBase.h"
#include "upppopupid.h"
#include "upptooltipdlgid.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CUPPDlg::CUPPDlg(CWnd* pParent /*=NULL*/)
	: LSY::CLSYLibDialogBase(pParent)
{
	bCreatedTooltip=false;
}

BEGIN_MESSAGE_MAP(CUPPDlg, LSY::CLSYLibDialogBase)
	ON_WM_MOVE()
END_MESSAGE_MAP()

bool CUPPDlg::OnCreatedProject(void)
{
	LSY::CObjectProject *pProject = m_pObjectMan->GetProjectFromID(ID_UPPPOPUP);
	m_pWndBack = (LSY::CWindows *)pProject->GetMainObject();

	LSY::CButton *pButton = NULL;
	
	SetButton((LSY::CButton *)GetObject(ID_UPPPOPUP_BUTTON_INFO), true);

	LSY::CText *pText = (LSY::CText *)GetObject(ID_UPPPOPUP_TEXT_CLEAR_TIME);
	pText->SetText("");
	CString strText;

	switch (nLevel)
	{
	case 1:
		m_pWndBack->SetIndex(0);
		pButton = (LSY::CButton *)GetObject(ID_UPPPOPUP_BUTTON_INFO);
		pButton->SetPos(242, 406);
		SetButton((LSY::CButton *)GetObject(ID_UPPPOPUP_BUTTON_OK), true);
		break;
	case 2:
		m_pWndBack->SetIndex(9);
		SetButton((LSY::CButton *)GetObject(ID_UPPPOPUP_BUTTON_OK), true);
		ShowTxt();
		break;
	case 3:
		m_pWndBack->SetIndex(10);
		SetButton((LSY::CButton *)GetObject(ID_UPPPOPUP_BUTTON_TEST), true);
		SetButton((LSY::CButton *)GetObject(ID_UPPPOPUP_BUTTON_NEXT), true);
		ShowTxt();
		if (pDlgTooltip.GetSafeHwnd())	
			break;
		bCreatedTooltip=pDlgTooltip.Create(this);
		pDlgTooltip.EnableWindow(false);
		break;
	case 4:
		m_pWndBack->SetIndex(20);
		pButton = (LSY::CButton *)GetObject(ID_UPPPOPUP_BUTTON_INFO);
		pButton->SetPos(242, 406);
		SetButton((LSY::CButton *)GetObject(ID_UPPPOPUP_BUTTON_CLOSE), true);
		strText.Format("~ %s까지", GM().GetClearDateStr().c_str());
		pText->SetText(strText.GetString());
		break;
	case 5:
		m_pWndBack->SetIndex(21);
		pButton = (LSY::CButton *)GetObject(ID_UPPPOPUP_BUTTON_INFO);
		pButton->SetPos(242, 406);
		SetButton((LSY::CButton *)GetObject(ID_UPPPOPUP_BUTTON_CLOSE), true);
		strText.Format("~ %s까지", GM().GetClearDateStr().c_str());
		pText->SetText(strText.GetString());
		break;
	default:
		break;
	}

	return true;
}

void CUPPDlg::ShowTxt()
{
	LSY::CText *pTxt = (LSY::CText *)GetObject(ID_UPPPOPUP_TEXT_TIME);
	CString strText;

	strText.Format("%d시간 %d분", nPlayTime/3600, (nPlayTime%3600)/60);
	pTxt->SetText(strText.GetString());
	pTxt->SetShow(true);

	pTxt = (LSY::CText *)GetObject(ID_UPPPOPUP_TEXT_MONEY);
	strText.Format("%s원", NMBASE::UTIL::g_MakeShortMoneyStrEx(nBillingMoney));
	pTxt->SetText(strText.GetString());
	pTxt->SetShow(true);
}

void CUPPDlg::SetButton(LSY::CButton * pButton, bool bShow)
{
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CUPPDlg::OnClickButton));
	pButton->SetShow(bShow);
}

BOOL CUPPDlg::OnInitDialog() 
{
	CLSYLibDialogBase::OnInitDialog();
	return TRUE;  
}

LRESULT CUPPDlg::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CMO_MouseLClick *msg = (LSY::CMO_MouseLClick *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	case ID_UPPPOPUP_BUTTON_INFO:
		{
			NMBASE::UTIL::CShellMan ShellMan;

			ShellMan.OpenNewBrowser("http://helpdesk.netmarble.net/HelpUPPInfo.asp");
		}
		break;

	case ID_UPPPOPUP_BUTTON_OK:
		{
			OnCancel() ;
		}
		break;
		
	case ID_UPPPOPUP_BUTTON_TEST:
		{
			NMBASE::UTIL::CShellMan ShellMan;
			CString strRet ="";
#if defined(_DEBUG)
			strRet.Format( "http://10.109.25.96:12000/upp?pid=%s&gameCode=42",m_strUserCN );			

#else
			if(g_strConnectecMasterServerName == "183.110.61.192:12000"){
				strRet.Format( "http://10.109.25.96:12000/upp?pid=%s&gameCode=42",m_strUserCN );
			}
			else{
				strRet.Format( "http://uppservey.netmarble.net:12000/upp?pid=%s&gameCode=42",m_strUserCN );
			}
#endif
			ShellMan.OpenNewBrowser(strRet);
			ExitClient();
		}
		break;
		
	case ID_UPPPOPUP_BUTTON_CLOSE:
	case ID_UPPPOPUP_BUTTON_NEXT:
		{
			ExitClient();
		}
		break;
	}

	return TRUE;
}

void CUPPDlg::ExitClient()
{
	if(nLevel==3)
	{
		pDlgTooltip.Close();
	}
	OnCancel() ;

	ExitGameClient(true);
}

BOOL CUPPDlg::PreTranslateMessage(MSG* pMsg)
{
	if(m_pObjectMan)
	{
		switch(pMsg->message)
		{
		case LSY_UM_EVENT_DRAW:
			if(m_pObjectMan->IsFirstDraw())
				return TRUE;
		}
	}

	return CWnd::PreTranslateMessage(pMsg);
}

INT_PTR CUPPDlg::DoModal(CWnd *pParent, int *param, char* UserCN)
{
	m_pParentWnd = pParent;
	nLevel = param[0];
	nPlayTime = param[1];
	nBillingMoney = param[2];
	m_strUserCN = UserCN;

	return CLSYLibDialogBase::DoModal(GetObjectMan(), ID_UPPPOPUP);
}

void CUPPDlg::OnMove(int x, int y)
{
	CLSYLibDialogBase::OnMove(x, y);
	if(bCreatedTooltip==false)
		return;
	CRect rect;
	pDlgTooltip.GetWindowRect(rect);
	pDlgTooltip.MoveWindow(x+85, y+507, rect.Width(), rect.Height());

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

CUPPTooltipDlg::CUPPTooltipDlg(CWnd* pParent /*=NULL*/)
	: LSY::CLSYLibDialogBase(pParent)
{
}

BEGIN_MESSAGE_MAP(CUPPTooltipDlg, LSY::CLSYLibDialogBase)
END_MESSAGE_MAP()

bool CUPPTooltipDlg::OnCreatedProject(void)
{
	LSY::CObjectProject *pProject = m_pObjectMan->GetProjectFromID(ID_UPPTOOLTIPDLG);
	m_pWndBack = (LSY::CWindows *)pProject->GetMainObject();
	return true;
}

BOOL CUPPTooltipDlg::OnInitDialog() 
{
	CLSYLibDialogBase::OnInitDialog();
	return TRUE;  
}

BOOL CUPPTooltipDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWnd::PreTranslateMessage(pMsg);
}

INT_PTR CUPPTooltipDlg::Create(CWnd *pParent)
{
	m_pParentWnd = pParent;

	return CLSYLibDialogBase::Create(GetObjectMan(), ID_UPPTOOLTIPDLG, pParent);
}

void CUPPTooltipDlg::Close(void)
{
	OnCancel();
}
