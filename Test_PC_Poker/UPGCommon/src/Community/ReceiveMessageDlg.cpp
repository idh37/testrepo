#include "stdafx.h"
#include "ReceiveMessageDlg.h"
#include "ReceiveMessageDlgID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CReceiveMessageDlg::CReceiveMessageDlg(CWnd* pParent/* = NULL*/)
:	LSY::CLSYLibDialogBase(pParent)
{
	m_cMemo.Clear();
}

BEGIN_MESSAGE_MAP(CReceiveMessageDlg, LSY::CLSYLibDialogBase)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

INT_PTR CReceiveMessageDlg::DoModal(Camel::Dar2::SS::Memo *pMemo, CWnd *pParent)
{
	m_pParentWnd = pParent;
	m_cMemo.Copy(*pMemo);
	return CLSYLibDialogBase::DoModal(GetObjectMan(), ID_RECEIVEMESSAGE);
}

bool CReceiveMessageDlg::OnCreatedProject(void)
{
	LSY::CAvatar *pAvatar = (LSY::CAvatar *)GetObject(ID_RECEIVEMESSAGE_AVATAR);
	pAvatar->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CReceiveMessageDlg::OnClickButton));

	LSY::CButton *pButton = (LSY::CButton *)GetObject(ID_RECEIVEMESSAGE_BTN_PROFILE);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CReceiveMessageDlg::OnClickButton));

	pButton = (LSY::CButton *)GetObject(ID_RECEIVEMESSAGE_BTN_SEND_WHISPER);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CReceiveMessageDlg::OnClickButton));

	pButton = (LSY::CButton *)GetObject(ID_RECEIVEMESSAGE_BTN_CHAT);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CReceiveMessageDlg::OnClickButton));

	pButton = (LSY::CButton *)GetObject(ID_RECEIVEMESSAGE_BTN_MESSAGE_DELETE);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CReceiveMessageDlg::OnClickButton));

	pButton = (LSY::CButton *)GetObject(ID_RECEIVEMESSAGE_BTN_MESSAGE_SAVE);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CReceiveMessageDlg::OnClickButton));

	pButton = (LSY::CButton *)GetObject(ID_RECEIVEMESSAGE_BTN_MESSAGE_NOTIFY);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CReceiveMessageDlg::OnClickButton));

	LSY::CCheckBox *pCheckBox = (LSY::CCheckBox *)GetObject(ID_RECEIVEMESSAGE_CHECK_NO_RECEIVE_MESSAGE);
	pCheckBox->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CReceiveMessageDlg::OnClickButton));
	pCheckBox->SetCheck(false);
	
	pButton = (LSY::CButton *)GetObject(ID_RECEIVEMESSAGE_BTN_RESPONSE);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CReceiveMessageDlg::OnClickButton));

	pButton = (LSY::CButton *)GetObject(ID_RECEIVEMESSAGE_BTN_CLOSE);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CReceiveMessageDlg::OnClickButton));

	ChangeMemo(&m_cMemo);

	return TRUE;
}

LRESULT CReceiveMessageDlg::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	case ID_RECEIVEMESSAGE_BTN_PROFILE:
	case ID_RECEIVEMESSAGE_AVATAR:
		{
			SMALLUSERINFO *pUserInfo = GM().GetDataUserList()->FindData(m_cMemo.senderID);
			if(pUserInfo) AskProfileToServer(pUserInfo->llUserKey, pUserInfo->ID);
			else AskProfileToServer(0, m_cMemo.senderID.c_str(), m_cMemo.senderCN.c_str());
		}
		break;
	case ID_RECEIVEMESSAGE_BTN_SEND_WHISPER:
		{
			SMALLUSERINFO *pUserInfo = GM().GetDataUserList()->FindData(m_cMemo.senderID);
			if(pUserInfo)
			{
				ShowWhisperDialog(pUserInfo, this);
			}
			else
			{
				SMALLUSERINFO userinfo;
				strcpy(userinfo.NickName, m_cMemo.senderNickname.c_str());
				strcpy(userinfo.ID, m_cMemo.senderID.c_str());
				userinfo.llUserKey = 0;
				ShowWhisperDialog(&userinfo, this);
			}
		}
		break;
	case ID_RECEIVEMESSAGE_BTN_CHAT:
		{
			OnOK();
			std::list<std::string> listInviteID;
			listInviteID.push_back(m_cMemo.senderID);
			SendCreateChatting(&listInviteID, "커뮤니티 서버와 접속이 되어 있지 않아 대화창을 열수 없습니다.");
		}
		break;
	case ID_RECEIVEMESSAGE_BTN_MESSAGE_DELETE:
		if(!CCommunityManager::Instance()->IsConnect())
		{
			CMsgBoxDlg dlg(this);
			dlg.DoModal(CMsgBoxDlg::EMBT_NOTICE, "커뮤니티 서버와 접속이 되어 있지 않아 쪽지를 삭제할 수 없습니다.");
			break;
		}
		else
		{
			CMemoNotifyDlg dlgNotify;
			if(dlgNotify.DoModal(this, CMemoNotifyDlg::EMNT_MEMO_DELETE) == IDCANCEL) break;			

			list<std::string> listMemoNode;
			listMemoNode.push_back(m_cMemo.node);
			CCommunityManager::Instance()->SendRemoveMemo(listMemoNode);
			OnCancel();
		}
		break;
	case ID_RECEIVEMESSAGE_BTN_MESSAGE_SAVE:
		if(!CCommunityManager::Instance()->IsConnect())
		{
			CMsgBoxDlg dlg(this);
			dlg.DoModal(CMsgBoxDlg::EMBT_NOTICE, "커뮤니티 서버와 접속이 되어 있지 않아 쪽지를 쪽지보관함으로 이동할 수 없습니다.");
		}
		else
		{
			int nStorageCount = CCommunityManager::Instance()->GetTotalStorageMemoCount(); 
			if(nStorageCount >= CCommunityManager::Instance()->GetMaxStorageMemo())
			{
				CMemoNotifyDlg dlgNotify;
				dlgNotify.DoModal(this, CMemoNotifyDlg::EMNT_MEMO_STORAGE_FULL);
				break;
			}

			list<std::string> listMemoNode;
			listMemoNode.push_back(m_cMemo.node);
			CCommunityManager::Instance()->SendStorageMoveMemo(listMemoNode);
			OnCancel();
		}
		break;
	case ID_RECEIVEMESSAGE_BTN_MESSAGE_NOTIFY:
		GM().BadUserReportRecevieMemo(m_cMemo);
		break;
	case ID_RECEIVEMESSAGE_BTN_RESPONSE:
		OnOK();
		//if(CCommunityManager::Instance()->IsConnect())
		{
			ShowSendMessageDialog(  m_cMemo.senderNickname.c_str() );
		}
//		else
// 		{
// 			ShowMainMessageDlg("커뮤니티 서버와 접속이 되어 있지 않아 답장을 보낼수 없습니다.");
// 		}
		break;
	case ID_RECEIVEMESSAGE_BTN_CLOSE:
		OnCancel();
		break;
	}
	return TRUE;
}

void CReceiveMessageDlg::OnDestroy()
{
	LSY::CCheckBox *pCheckBox = (LSY::CCheckBox *)GetObject(ID_RECEIVEMESSAGE_CHECK_NO_RECEIVE_MESSAGE);
	if(pCheckBox->IsCheck())
	{
		if(CCommunityManager::Instance()->IsConnect())
		{
			if(CCommunityManager::Instance()->GetOption(COM_ECOT_MEMO) != COM_ECOPT_OFF) CCommunityManager::Instance()->SetOption(COM_ECOT_MEMO, COM_ECOPT_OFF);
		}
		else
		{
			ShowMainMessageDlg("커뮤니티 서버와 접속되어있지 않아 쪽지 수신 거부가 적용되지 않았습니다.");
		}
	}

	__super::OnDestroy();
}

void CReceiveMessageDlg::ChangeMemo(Camel::Dar2::SS::Memo *pMemo)
{
	if(&m_cMemo != pMemo)
		m_cMemo.Copy(*pMemo);

#ifdef _DEBUG
	CString strFormat2;
	strFormat2.Format("\n[CReceiveMessageDlg : ChangeMemo] id = %s, nickname = %s, cn = %s", pMemo->senderID.c_str(), pMemo->senderNickname.c_str(), pMemo->senderCN.c_str());
	OutputDebugString(strFormat2);
#endif

	LSY::CAvatar *pAvatar = (LSY::CAvatar *)GetObject(ID_RECEIVEMESSAGE_AVATAR);	
	pAvatar->SetAvatar(pMemo->senderAvatarInfo);
	
	//**귓속말/채팅 셋팅 필요
	LSY::CButton *pButton = (LSY::CButton *)GetObject(ID_RECEIVEMESSAGE_BTN_SEND_WHISPER);
	pButton = (LSY::CButton *)GetObject(ID_RECEIVEMESSAGE_BTN_CHAT);

	LSY::CText *pText = (LSY::CText *)GetObject(ID_RECEIVEMESSAGE_TXT_SENDER_NICKNAME);
	pText->SetText(m_cMemo.senderNickname);

	CString strFormat;
	strFormat.Format("(%s)", m_cMemo.senderID.c_str());
	pText = (LSY::CText *)GetObject(ID_RECEIVEMESSAGE_TXT_SENDER_ID);
	pText->SetText(strFormat.GetString());

	LSY::CRichText *pRich = (LSY::CRichText *)GetObject(ID_RECEIVEMESSAGE_RICHTEXT_MESSAGE);
	pRich->AddText(m_cMemo.content);

	pText = (LSY::CText *)GetObject(ID_RECEIVEMESSAGE_TXT_SEND_TIME);
	pText->SetText(m_cMemo.recvTime);

	pButton = (LSY::CButton *)GetObject(ID_RECEIVEMESSAGE_BTN_MESSAGE_SAVE);
	pButton->SetEnable((pMemo->boxType == Camel::Dar2::SS::IN_BOX)?true:false);
}

void CReceiveMessageDlg::OnChangeUser(SMALLUSERINFO *pSUI)
{
	if(pSUI == NULL) return;
	if(GetSafeHwnd() == NULL) return;
	if(m_cMemo.senderID != pSUI->ID) return;

	//LSY::CText *pText = (LSY::CText *)GetObject(ID_RECEIVEMESSAGE_TXT_GAME_LOCATION);
	//LSY::CText *pRoomNumber = (LSY::CText *)GetObject(ID_RECEIVEMESSAGE_TXT_ROOM_NUMBER);
	//
	//CGame *pGame = GM().GetGame((IDX_GAME)pSUI->nGameIndex);;
	//if(pGame)
	//{
	//	pText->SetTextColor(pGame->GetGameColor());
	//	pText->SetText(pGame->GetGameName().GetString());
	//	pRoomNumber->SetShow(true);
	//}
	//else
	//{
	//	pText->SetTextColor(COLOR_GAMENAME_OFFLINE);
	//	pText->SetText("오프라인");
	//	pRoomNumber->SetShow(false);
	//}

	//if(pRoomNumber->IsShow())
	//{
	//	CString strTemp;
	//	if(pSUI->RoomNum == 0)
	//	{
	//		strTemp = "대기실";
	//	}
	//	else
	//	{
	//		strTemp.Format("[%d번방]", pSUI->RoomNum);
	//	}

	//	pRoomNumber->SetText(strTemp.GetString());
	//}
}