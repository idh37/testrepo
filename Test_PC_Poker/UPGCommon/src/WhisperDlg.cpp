// WhisperDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WhisperDlg.h"
#include "whisperdlgid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CWhisperDlg::CWhisperDlg(CWnd* pParent /*=NULL*/)
:	LSY::CLSYLibDialogBase(pParent),
	m_pWhisperEdit(NULL),
	m_llTargetUKey(-1),
	m_strNickName(""),
	m_strID("")
{
}

BEGIN_MESSAGE_MAP(CWhisperDlg, LSY::CLSYLibDialogBase)
END_MESSAGE_MAP()

bool CWhisperDlg::OnCreatedProject(void)
{
	m_pNickName = (LSY::CText *)GetObject(ID_WHISPER_TXT_NICKNAME);
	m_pWhisperEdit = (LSY::CEdit *)GetObject(ID_WHISPER_EDIT_WHISPER);

	// 복사 붙여넣기 막기.
	m_pWhisperEdit->SetUseClipboard(false) ;
	m_pWhisperEdit->AddHandler(LSY::EM_O_RETURN, LSY::Fnt(this, &CWhisperDlg::OnEditReturn));

	LSY::CButton *pButton = (LSY::CButton *)GetObject(ID_WHISPER_BTN_SEND);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CWhisperDlg::OnClickButton));

	pButton = (LSY::CButton *)GetObject(ID_WHISPER_BTN_CANCEL);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CWhisperDlg::OnClickButton));

	m_pWhisperEdit->SetText("");
	m_pNickName->SetText(m_strNickName.GetString());

	return TRUE;
}

LRESULT CWhisperDlg::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CMO_MouseLClick *msg = (LSY::CMO_MouseLClick *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	case ID_WHISPER_BTN_CANCEL:
		OnCancel();
		break;
	case ID_WHISPER_BTN_SEND:
		SendWhisper();
		OnOK();
		break;
	}
	return TRUE;
}

LRESULT CWhisperDlg::OnEditReturn(LSY::CMessage *pMsg)
{
	SendWhisper();
	OnOK();
	return TRUE;
}

void CWhisperDlg::SendWhisper(void)
{
	CString strChat = m_pWhisperEdit->GetText().c_str();

	if(strChat.GetLength() == 0)
	{
		return;
	}

	int DestUNum=-1;
	int num=-1;	
	
	CString outstr;
	// 대화 도배 검사
	if(NoChat.CheckNoChat(strChat))
	{
		CMsgBoxDlg msgdlg(this);
		msgdlg.DoModal(CMsgBoxDlg::EMBT_NOTICE, "대화 도배로 인하여 채팅 금지중입니다.");
		return;
	}

	int slen = strChat.GetLength();
	char *str= (char *)(LPCTSTR)strChat;
	for(int s=0; s<slen-1; s++) {
		if(str[s] == 8 || str[s] == 9/* || str[s] == 10*/ || str[s] == 13) {
			str[s] = ' ';
		}
	}

	AddChattingWhisperList(m_strNickName.GetString(), true, false, false);

	if(GM().GetCurrentWhere() == IDX_GAMEWHERE_LOBY) // 대기실이면
	{	
		char szTChat[512];
		
		ZeroMemory(szTChat, sizeof(szTChat));
		NMBASE::UTIL::NMIsBadToRep( (char *)(LPCTSTR)strChat, szTChat, 512 );

		if (!g_bAdminUser)
		{
			outstr.Format("[%s]님에게 귓말 : %s\n", (char*)(LPCTSTR)m_strNickName, szTChat);
			AddLobbyChatViewEdit(outstr, RGB(90, 0, 167));

			GM().SendWhisper(strChat.GetString(), m_strNickName, m_llTargetUKey, m_strID);
		}
		else
		{
			CNM_ADMIN_WHISPERCHAT wcmsg;
			wcmsg.Set(GM().GetMyInfo()->UI.ID, DestUNum, m_strNickName,  (char*)(LPCTSTR)strChat);
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), wcmsg.pData, wcmsg.GetTotalSize());
		}

		CString  str_1;
		str_1.Format("[%s]님에게 귓말 : %s\n", (char*)(LPCTSTR)m_strNickName , szTChat);
		AddLobbyHideChatText(&str_1);
	}

	if(GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME) // 게임중이면
	{
		BOOL bDont = FALSE;
		if(g_RI.State==1)
		{
			const int nMaxPlayer = GM().GetMaxPlayer();
			CGameProcess *pGameProcess = GM().GetCurrentProcess();
			CPlayer *pPlayer = NULL;
			for(int i=0; i<nMaxPlayer; i++)
			{
				pPlayer = pGameProcess->GetPlayer(i);
				if(m_strNickName.Compare(pPlayer->UI.NickName)==0) { bDont=TRUE; break; }
			}
		}

		if (g_bAdminUser) bDont = FALSE;

		if(bDont)
		{	
			outstr.Format( g_StrMan.Get(_T("NO_WHISPER")) );
			AddGameChatViewEdit(outstr, RGB(143,141,102));
		}
		else
		{
			char szTChat[512];

			ZeroMemory(szTChat, sizeof(szTChat));
			NMBASE::UTIL::NMIsBadToRep( (char *)(LPCTSTR)strChat, szTChat, 512 );

			if (!g_bAdminUser)
			{
				outstr.Format("[%s]님에게 귓말 : %s\n", (char*)(LPCTSTR)m_strNickName, szTChat);
				if(m_llTargetUKey != -1) GM().SendWhisper(strChat.GetString(), m_strNickName, m_llTargetUKey, m_strID);			
				else GM().SendWhisper(strChat.GetString(), m_strNickName);
			}
			else
			{
				CNM_ADMIN_WHISPERCHAT wcmsg;
				wcmsg.Set(GM().GetMyInfo()->UI.ID, DestUNum, m_strNickName,  (char*)(LPCTSTR)strChat);
				NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), wcmsg.pData, wcmsg.GetTotalSize());					
			}

			CString  str_1;
			str_1.Format("[%s]님에게 귓말 : %s\n", (char*)(LPCTSTR)m_strNickName , szTChat);
			AddLobbyHideChatText(&str_1);
		}
	}

	m_pWhisperEdit->SetText("");
}

INT_PTR CWhisperDlg::DoModal(const char *pNickname, const char *pID, UINT64 llUserKey, CWnd *pParent)
{
	m_pParentWnd = pParent;
	m_strNickName = pNickname;
	m_strID = pID;
	m_llTargetUKey = llUserKey;

	return LSY::CLSYLibDialogBase::DoModal(GetObjectMan(), ID_WHISPER, true);
}