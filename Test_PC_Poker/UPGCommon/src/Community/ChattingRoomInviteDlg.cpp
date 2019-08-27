#include "stdafx.h"
#include "ChattingRoomInviteDlg.h"
#include "ChattingRoomInviteDlgID.h"

#define _DEF_CHAT_INVITE_DELAY	10000
#define TIMER_CHATROOMINVITE_PROGRESS  123456

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CChattingRoomInviteDlg::CChattingRoomInviteDlg(CWnd* pParent/* = NULL*/)
:	m_dwStartTime(0),
	m_dwEndTime(0),
	m_pProg(NULL),
	m_bFirst(true)
{
}

BEGIN_MESSAGE_MAP(CChattingRoomInviteDlg, LSY::CLSYLibDialogBase)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


void CChattingRoomInviteDlg::Create(CWnd *pParent, const std::string &strRoomID, const std::string &strSenderID)
{
	m_strRoomID		= strRoomID;
	m_strID			= strSenderID;
	m_bFirst		= true;

	LSY::CLSYLibDialogBase::Create(GetObjectMan(), ID_CHATTINGREQUEST, pParent, true);
}

BOOL CChattingRoomInviteDlg::OnInitDialog()
{
	BOOL bRet = __super::OnInitDialog();

	SetTimer(TIMER_CHATROOMINVITE_PROGRESS, 1, NULL);
	return bRet;
}

bool CChattingRoomInviteDlg::OnCreatedProject(void)
{
	LSY::CButton *pButton = (LSY::CButton *)GetObject(ID_CHATTINGREQUEST_BTN_ACCEPT);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CChattingRoomInviteDlg::OnClickButton));

	pButton = (LSY::CButton *)GetObject(ID_CHATTINGREQUEST_BTN_REJECTION);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CChattingRoomInviteDlg::OnClickButton));

	LSY::CText *pText = (LSY::CText *)GetObject(ID_CHATTINGREQUEST_TXT_NICKNAME);
	pText->SetText(m_strID);

	m_pProg = (LSY::CProgressiveBar *)GetObject(ID_CHATTINGREQUEST_PROG_TIMER);
	m_pProg->SetRangeValue(0.0f, (float)_DEF_CHAT_INVITE_DELAY);
	m_pProg->SetCurValue((float)_DEF_CHAT_INVITE_DELAY);

	LSY::CCheckBox *pCheckBox = (LSY::CCheckBox *)GetObject(ID_CHATTINGREQUEST_CHK_NEXT_REJECTION);
	pCheckBox->SetCheck(false);

	return true;
}

LRESULT CChattingRoomInviteDlg::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	case ID_CHATTINGREQUEST_BTN_ACCEPT:
		OnOK();
		break;
	case ID_CHATTINGREQUEST_BTN_REJECTION:
		OnCancel();
		break;
	}
	return TRUE;
}

void CChattingRoomInviteDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == TIMER_CHATROOMINVITE_PROGRESS)
	{
		if(m_bFirst)
		{
			m_dwStartTime	= timeGetTime();
			m_dwEndTime		= m_dwStartTime + _DEF_CHAT_INVITE_DELAY;
			m_bFirst = false;
		}

		DWORD dwCurTime = timeGetTime();

		if(dwCurTime >= m_dwEndTime)
		{
			m_pProg->SetCurValue(0.0f);
			OnCancel();
		}
		else
		{
			float fRate = 1.0f - float(dwCurTime - m_dwStartTime)/float(_DEF_CHAT_INVITE_DELAY);
			float fValue = float(_DEF_CHAT_INVITE_DELAY) * fRate;
			m_pProg->SetCurValue(fValue);
		}
	}
	__super::OnTimer(nIDEvent);
}

void CChattingRoomInviteDlg::OnDestroy()
{
	CLSYLibDialogBase::OnDestroy();

	// TODO: Add your message handler code here	
	KillTimer(TIMER_CHATROOMINVITE_PROGRESS);
}

void CChattingRoomInviteDlg::OnCancel()
{
	KillTimer(TIMER_CHATROOMINVITE_PROGRESS);
	GM().GetMainWnd()->PostMessage(CM_CLOSE_INVITE_CHATTING_ROOM, IDCANCEL, (LPARAM)this);

	LSY::CCheckBox *pCheckBox = (LSY::CCheckBox *)GetObject(ID_CHATTINGREQUEST_CHK_NEXT_REJECTION);
	if(pCheckBox->IsCheck())
	{
		if(CCommunityManager::Instance()->IsConnect())
		{
			if(CCommunityManager::Instance()->GetOption(COM_ECOT_CHAT) != COM_ECOPT_OFF) CCommunityManager::Instance()->SetOption(COM_ECOT_CHAT, COM_ECOPT_OFF);
		}
		else
		{
			ShowMainMessageDlg("커뮤니티 서버와 접속되어있지 않아 대화 수신 거부가 적용되지 않았습니다.");
		}
	}

	__super::OnCancel();
}

void CChattingRoomInviteDlg::OnOK()
{
	KillTimer(TIMER_CHATROOMINVITE_PROGRESS);
	GM().GetMainWnd()->PostMessage(CM_CLOSE_INVITE_CHATTING_ROOM, IDOK, (LPARAM)this);

	LSY::CCheckBox *pCheckBox = (LSY::CCheckBox *)GetObject(ID_CHATTINGREQUEST_CHK_NEXT_REJECTION);
	if(pCheckBox->IsCheck())
	{
		if(CCommunityManager::Instance()->IsConnect())
		{
			if(CCommunityManager::Instance()->GetOption(COM_ECOT_CHAT) != COM_ECOPT_OFF) CCommunityManager::Instance()->SetOption(COM_ECOT_CHAT, COM_ECOPT_OFF);
		}
		else
		{
			ShowMainMessageDlg("커뮤니티 서버와 접속되어있지 않아 대화 수신 거부가 적용되지 않았습니다.");
		}
	}

	__super::OnOK();
}