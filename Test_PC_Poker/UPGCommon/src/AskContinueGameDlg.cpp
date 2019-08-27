#include "stdafx.h"
#include "AskContinueGameDlg.h"
#include "MsgBoxDlgID.h"
#include "GlobalBase.h"


CAskContinueGameDlg::CAskContinueGameDlg(CWnd* pParent /*=NULL*/)
	: LSY::CLSYLibDialogBase(pParent),
	m_bClearMessage(false),
	m_pImgBack(NULL),
	m_pTextMessage(NULL)
{

}

BEGIN_MESSAGE_MAP(CAskContinueGameDlg, LSY::CLSYLibDialogBase)
END_MESSAGE_MAP()

INT_PTR CAskContinueGameDlg::DoModal( LPCTSTR strMsg)
{
	m_listMessage.push_back(strMsg);


	if(GetSafeHwnd())
	{
		LSY::CText *pText = (LSY::CText *)GetObject(ID_MSGBOX_TXT_MSG);
		pText->SetText(strMsg);
		m_listMessage.clear();
		m_bClearMessage = false;

		return IDCANCEL;
	}

	return __super::DoModal(GetObjectMan(), ID_MSGBOX);
}

bool CAskContinueGameDlg::OnCreatedProject(void)
{
	//사용하지 않는 버튼은 visible = false처리
	LSY::CButton *pButton = (LSY::CButton *)GetObject(ID_MSGBOX_BTN_YES);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CAskContinueGameDlg::OnClickButton));
	pButton->SetShow(false);

	pButton = (LSY::CButton *)GetObject(ID_MSGBOX_BTN_NO);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CAskContinueGameDlg::OnClickButton));
	pButton->SetShow(false);

	pButton = (LSY::CButton *)GetObject(ID_MSGBOX_BTN_ENTER_ROOM);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CAskContinueGameDlg::OnClickButton));
	pButton->SetShow(false);

	pButton = (LSY::CButton *)GetObject(ID_MSGBOX_BTN_CREATE_ROOM);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CAskContinueGameDlg::OnClickButton));
	pButton->SetShow(false);
	
	//버튼 설정
	pButton = (LSY::CButton *)GetObject(ID_MSGBOX_BTN_OK);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CAskContinueGameDlg::OnClickButton));
	pButton->SetPos(70,78);
	pButton->SetShow(true);


	pButton = (LSY::CButton *)GetObject(ID_MSGBOX_BTN_CANCEL);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CAskContinueGameDlg::OnClickButton));
	pButton->SetShow(true);

	
	m_pTextMessage = (LSY::CText *)GetObject(ID_MSGBOX_TXT_MSG);
	m_pTextMessage->SetText(m_listMessage.front());
	m_pTextMessage->AddHandler(LSY::EM_O_SETTXTLINECOUNT, LSY::Fnt(this, &CAskContinueGameDlg::OnSetTextLineCount));
	m_listMessage.pop_front();

	m_pImgBack = (LSY::CImage *)GetObject(ID_MSGBOX_IMG_BACK_TOP);
	return TRUE;

	return true;
}


LRESULT CAskContinueGameDlg::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CMO_MouseLClick *msg = (LSY::CMO_MouseLClick *)pMsg;

	
	switch(msg->GetObject()->GetID())
	{
	case ID_MSGBOX_BTN_OK:
		OnOK();
		break;

	case ID_MSGBOX_BTN_CANCEL:
		OnCancel();
		break;
	}
	return TRUE;
}


LRESULT CAskContinueGameDlg::OnSetTextLineCount(LSY::CMessage *pMsg)
{
	LSY::CMO_SetTextLineCount *msg = (LSY::CMO_SetTextLineCount *)pMsg;
	int nLine = msg->GetValue();
	if(nLine < 2) nLine = 2;

	int nOldHeight = (int)m_pTextMessage->GetHeight();
	int nNewHeight = nLine * 13;

	if(nOldHeight != nNewHeight)
	{
		int nWidth = (int)m_pTextMessage->GetWidth();
		m_pTextMessage->Create(nWidth, nNewHeight);
		m_pTextMessage->SetRedraw(false);

		m_pImgBack->SetMiddleCount(nLine);
		m_pObjectMan->ReSize(m_pImgBack->GetLocalRect().GetSize());

		CRect rtRect;
		GetWindowRect(&rtRect);

		int nPosY = rtRect.top - (nNewHeight - nOldHeight)/2;
		if(nPosY < 0) nPosY = 0;

		SetWindowPos(NULL, rtRect.left, nPosY, 0, 0, SWP_NOZORDER|SWP_NOSIZE|SWP_NOREDRAW);
		InvalidateRect(NULL, FALSE);

		return TRUE;
	}
	return FALSE;
}

void CAskContinueGameDlg::OnOK() 
{
	if(GetCurrentWhere() == IDX_GAMEWHERE_GAME) 
	{
		//관전중에서 방참여 요청한다.
		CPlayer *pPlayer = GM().GetMyInfo();
		CSV_ASK_PARTICIPATION parti;
		parti.Set(g_RI.RoomNum,  pPlayer->UI.UNum, pPlayer->ServPNum, pPlayer->UI.ID, pPlayer->nSndFxKind);
		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), parti.pData, parti.GetTotalSize() );
	
	}else
	{
		CMsgBoxDlg dlg(GM().GetMainWnd());
		dlg.DoModal(CMsgBoxDlg::EMBT_NOTICE, "이전 방이 없어 참가할 수 없습니다.");
	}

	__super::OnOK();
}

void CAskContinueGameDlg::OnCancel()
{
	//대기실로 이동요청한다.
	if(GetCurrentWhere() == IDX_GAMEWHERE_GAME) 
		ExitGameRequest();

	__super::OnCancel();
}
