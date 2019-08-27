#include "stdafx.h"
#include "ChattingRoomDlg.h"
#include "ChattingRoomDlgID.h"
#include "ChattingRoomAddUserDlg.h"
#include "ChattingPopupMemu.h"
#include "ChattingPopupMemuID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define _DEF_FONTCOLOR_NOTIFY	0
#define _DEF_FONTCOLOR_MYID		2
#define _DEF_FONTCOLOR_MYCHAT	1
#define _DEF_FONTCOLOR_YOUID	4
#define _DEF_FONTCOLOR_YOUCHAT	3
#define _DEF_FONTCOLOR_NOTIFY2	5

CChattingRoomDlg::CChatData::CChatData(const std::string &strID, const std::string &strNick, const std::string &strMessage)
:	m_strID(strID),
	m_strNickName(strNick),
	m_strChatting(strMessage),
	m_nLen(0)
{
	CalculationLen();
}

void CChattingRoomDlg::CChatData::CalculationLen(void)
{
	m_nLen += 1; //[
	m_nLen += m_strID.size();
	m_nLen += 1; //:
	m_nLen += m_strNickName.size();
	m_nLen += 10; //]님의 말:\n
	m_nLen += m_strChatting.size();
}

std::string CChattingRoomDlg::CChatData::GetText(void)
{
	std::string strRet = "[";
	strRet += m_strID;
	strRet += ":";
	strRet += m_strNickName;
	strRet += "]님의 말:\n";
	strRet += m_strChatting;
	return strRet;
}

CChattingRoomDlg::CChattingRoomDlg()
:	LSY::CLSYLibWindowsBase(),
	m_pAvatarYou(NULL),
	m_pAvatarMy(NULL),
	m_pUserList(NULL),
	m_pChattingList(NULL),
	m_pEdit(NULL),
	m_bMaster(false),
	m_nChatLength(0),
	m_pLastMessageTime(NULL),
	m_bDisconnect(false),
	m_pPopupMenu(NULL),
	m_bSendClose(false)
{
}

CChattingRoomDlg::~CChattingRoomDlg()
{
	if(m_pPopupMenu)
	{
		m_pPopupMenu->DestroyWindow();
		delete m_pPopupMenu;
		m_pPopupMenu = NULL;
	}
}

BEGIN_MESSAGE_MAP(CChattingRoomDlg, LSY::CLSYLibWindowsBase)
	ON_WM_DESTROY()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

bool CChattingRoomDlg::Create(CWnd *pParent, const std::string &strRoomID, const std::list<std::string> &listInviteID, const bool &bMaster)
{
	ASSERT(listInviteID.size() != 0);

	m_listInviateID = listInviteID;
	m_strRoomID = strRoomID;
	m_strID = *m_listInviateID.begin();
	m_bMaster = bMaster;

	ASSERT(pParent != NULL);
	Init(GetObjectMan(), ID_CHATTING, true);
	if(!CLSYLibWindowsBase::CreateEx(0, AfxRegisterWndClass(CS_DBLCLKS), "ChattingRoom", WS_POPUP | WS_OVERLAPPED | WS_CLIPCHILDREN, 0, 0, 100, 100, NULL, NULL))
	{
		return false;
	}

	m_pObjectMan->SetCaptionHeight(28);
	if(pParent) CenterWindow(pParent);
	return true;
}

bool CChattingRoomDlg::OnCreatedProject(void)
{
	m_pAvatarYou = (LSY::CAvatar *)GetObject(ID_CHATTING_AVATAR_YOU);
	m_pAvatarYou->SetShow(false);

	m_pAvatarMy = (LSY::CAvatar *)GetObject(ID_CHATTING_AVATAR_MY);
	m_pAvatarMy->SetAvatar(GM().GetMyInfo()->UI.AvatarInfo);

	m_pUserList = (LSY::CTextList *)GetObject(ID_CHATTING_TEXTLIST_USER);
	m_pUserList->AddHandler(LSY::EM_O_MOUSERCLICK, LSY::Fnt(this, &CChattingRoomDlg::OnUserListRClick));

	m_pChattingList = (LSY::CRichText *)GetObject(ID_CHATTING_RICH_CHATTING);
	m_pEdit = (LSY::CEdit *)GetObject(ID_CHATTING_EDIT_MESSAGE);

	m_pEdit->SetText("");
	m_pEdit->SetEnable(false);

	m_pEdit->AddHandler(LSY::EM_O_RETURN, LSY::Fnt(this, &CChattingRoomDlg::OnReturn));

	LSY::CButton *pButton = (LSY::CButton *)GetObject(ID_CHATTING_BTN_ADDUSER);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CChattingRoomDlg::OnClickButton));

	pButton = (LSY::CButton *)GetObject(ID_CHATTING_BTN_NOTIFY);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CChattingRoomDlg::OnClickButton));

	pButton = (LSY::CButton *)GetObject(ID_CHATTING_BTN_SEND);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CChattingRoomDlg::OnClickButton));

	pButton->SetEnable(false);

	pButton = (LSY::CButton *)GetObject(ID_CHATTING_BTN_CLOSE);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CChattingRoomDlg::OnClickButton));

	m_pLastMessageTime = (LSY::CText *)GetObject(ID_CHATTING_TXT_LAST_MESSAGE_TIME);
	m_pLastMessageTime->SetText("");

	m_pChattingList->SetCurrentFont(_DEF_FONTCOLOR_NOTIFY);

#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
	if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
		m_pChattingList->AddText("톡톡 운영자는 어떤한 경우라도 회원님의 개인정보를 묻지 않습니다.");
	else
		m_pChattingList->AddText("윈조이 운영자는 어떤한 경우라도 회원님의 개인정보를 묻지 않습니다.");
#else //UPGRADE_20120308_TOCTOC_CHANNELING
	m_pChattingList->AddText("윈조이 운영자는 어떤한 경우라도 회원님의 개인정보를 묻지 않습니다.");
#endif //UPGRADE_20120308_TOCTOC_CHANNELING

	m_pChattingList->AddText("\n운영자 사칭 및 욕설 등은 신고버튼을 통해서 신고해 주시기 바랍니다.");
	m_pChattingList->AddText("\n단체 대화는 최대 10명까지 가능합니다.");

	if(m_bMaster) OnJoinSuccessRoom();
	else CCommunityManager::Instance()->SendJoinChatRoom(m_strRoomID, m_bMaster);

	CheckChangeCaption();

	return true;
}

void CChattingRoomDlg::CheckChangeCaption(void)
{
	CString strFormat;
	int nIndex = GetIDToListIndex(m_strID);
	std::string strNIckName = m_strID;
	if(nIndex != -1)
	{
		CChatUser *pUser = (CChatUser *)m_pUserList->GetItemData(nIndex);
		strNIckName = pUser->GetNickName();
	}

	if(m_pUserList->GetItemCount() > 2)
	{
		strFormat.Format("%s님외 %d명과 대화 - 윈조이 %s", strNIckName.c_str(), m_pUserList->GetItemCount() - 2, GM().GetCurrentGameName().GetString());
	}
	else
	{
		strFormat.Format("%s님과의 대화 - 윈조이 %s", strNIckName.c_str(), GM().GetCurrentGameName().GetString());
	}

	SetWindowText(strFormat);
}

void CChattingRoomDlg::OnCancel()
{
	if(m_bSendClose) return;
	ShowWindow(SW_HIDE);
	GM().GetMainWnd()->PostMessage(CM_CLOSE_CHATTING_ROOM, 0, (LPARAM)this);
	m_bSendClose = true;
}

LRESULT CChattingRoomDlg::OnReturn(LSY::CMessage *pMsg)
{
	SendChat();
	return TRUE;
}

LRESULT CChattingRoomDlg::OnUserListRClick(LSY::CMessage *pMsg)
{
	LSY::CMO_MouseRClick *msg = (LSY::CMO_MouseRClick *)pMsg;
	LSY::CPoint ptClick = msg->GetPos();
	int nHitIndex = m_pUserList->HitTest(ptClick);
	if(nHitIndex != -1)
	{
		CChatUser *pUser = (CChatUser *)m_pUserList->GetItemData(nHitIndex);
		if(pUser)
		{
			m_strSelectNickName = pUser->GetNickName();
			m_strSelectID = pUser->GetID();

			LSY::CPoint ptPos = m_pObjectMan->GetCurMousePos();
			POINT ptTempPos = ptPos.GetPoint();
			ClientToScreen(&ptTempPos);

			bool bBuddy = true;
			bool bBlackList = true;

			if(CCommunityManager::Instance()->GetMyNickName() != m_strSelectNickName)
			{
				bBuddy = CCommunityManager::Instance()->IsExistBuddy(m_strSelectNickName)?true:false;
				bBlackList = CCommunityManager::Instance()->IsExistBlackBuddy(m_strSelectNickName)?true:false;
			}

			CChattingPopupMemu *pPopupMemu = new CChattingPopupMemu;
			pPopupMemu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_PROFILE, "프로필보기", true);
			pPopupMemu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_ADD_BUDDY, "친구추가", bBuddy?false:true);
			pPopupMemu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_ADD_BLACKLIST, "블랙리스트추가", bBlackList?false:true);

			pPopupMemu->Create(ptTempPos.x, ptTempPos.y, this);

			PostMessage(UPGM_POPUPMEMU_OPEN, 0, (LPARAM)pPopupMemu);
		}
	}
	return TRUE;
}

void CChattingRoomDlg::SendChat(void)
{
	std::string strTemp = m_pEdit->GetText();
	if(strTemp == "") return;
	CCommunityManager::Instance()->SendChat(m_strRoomID, strTemp);
	m_pEdit->SetText("");
}

LRESULT CChattingRoomDlg::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	case ID_CHATTING_BTN_SEND:
		SendChat();
		break;
	case ID_CHATTING_BTN_CLOSE:
		OnCancel();
		break;
	case ID_CHATTING_BTN_ADDUSER:
		{
			CChattingRoomAddUserDlg dlg;
			if(dlg.DoModal(this) == IDOK)
			{
				CChatUser *pUser = NULL;

				MAPSTRING::const_iterator start = dlg.GetIDAndNickName().begin();
				MAPSTRING::const_iterator end = dlg.GetIDAndNickName().end();

				MAPSTRING mapExistIDorNickName;
				MAPSTRING mapSendIDorNickName;

				bool bExistID = false;
				for(;start != end;++start)
				{
					pUser = FindUserFromID(start->first);

					if(pUser)
					{
						mapExistIDorNickName[start->first] = start->second;
					}
					else
					{
						mapSendIDorNickName[start->first] = start->second;
					}
				}

				CString strTemp;
				CString strFormat;
				m_pChattingList->SetCurrentFont(_DEF_FONTCOLOR_NOTIFY2);
				if(mapExistIDorNickName.size())
				{
					MAPSTRING::iterator start = mapExistIDorNickName.begin();
					MAPSTRING::iterator end = mapExistIDorNickName.end();

					bool bFirst = true;
					for(;start != end;++start)
					{
						if(bFirst) bFirst = false;
						else strTemp += ", ";

						strTemp += start->second.c_str();
					}

					strFormat.Format("\n[%s]님은 이미 대화에 참여중입니다.", strTemp.GetString());
					m_pChattingList->AddText(strFormat.GetString());
				}
				
				if(mapSendIDorNickName.size())
				{
					LISTSTRING listSendIDorNickName;
					MAPSTRING::iterator start = mapSendIDorNickName.begin();
					MAPSTRING::iterator end = mapSendIDorNickName.end();
					for(;start != end;++start)
					{
						listSendIDorNickName.push_back(start->first);
					}

					CCommunityManager::Instance()->SendGetUserInfo(CCommunityUserInfo::ECUIT_INVITECHAT, (LPARAM)this, listSendIDorNickName, "");
				}
			}
		}
		break;
	case ID_CHATTING_BTN_NOTIFY:
		OnNotifyUser();
		break;
	}
	return TRUE;
}

void CChattingRoomDlg::SendInviteCahtRoom(std::string strID, std::string strNickName, bool bSendResult, bool bReceiveResult)
{
	CString strFormat;
	m_pChattingList->SetCurrentFont(_DEF_FONTCOLOR_NOTIFY2);

	if(bSendResult)
	{
		if(bReceiveResult)
		{
			CCommunityManager::Instance()->SendInviteChatRoom(m_strRoomID, strID);

			strFormat.Format("\n[%s]님께 대화 신청을 하였습니다.", strNickName.c_str());
			m_pChattingList->AddText(strFormat.GetString());
		}
		else
		{
			m_pChattingList->AddText("\n[%s]님께 초대 메시지를 보내지 못했습니다. 아이디/닉네임이 존재하지 않거나 오프라인일 수 있습니다.");
		}
	}
	else
	{
		m_pChattingList->AddText("\n[%s]님께 초대 메시지를 보내지 못했습니다.");
	}
}

void CChattingRoomDlg::OnDestroy()
{
	if(!m_bSendClose)
		OnCancel();

	int nCount = m_pUserList->GetItemCount();
	CChatUser *pUser = NULL;
	for(int i=0;i<nCount;++i)
	{
		pUser = (CChatUser *)m_pUserList->GetItemData(i);
		delete pUser;
	}

	m_pUserList->RemoveAll();

	if(!m_bDisconnect) CCommunityManager::Instance()->SendLeaveChatRoom(m_strRoomID);
	__super::OnDestroy();
}

void CChattingRoomDlg::AddUser(CChatUser *pUser)
{
	ASSERT(m_pUserList->FirstFind((LPARAM)pUser->GetNickName().c_str()) == -1);
	int nIndex = m_pUserList->AddText(pUser->GetNickName());
	m_pUserList->SetItemData(nIndex, (LPARAM)pUser);
	CheckChangeCaption();
}

void CChattingRoomDlg::SendInviteRoom(void)
{
	if(!m_bMaster) return;

	std::list<std::string>::iterator start = m_listInviateID.begin();
	std::list<std::string>::iterator end = m_listInviateID.end();

	std::string strID;
	bool bFirst = true;
	for(;start != end;++start)
	{
		if(bFirst) bFirst = false;
		else strID += ", ";

		strID += *start;
		CCommunityManager::Instance()->SendInviteChatRoom(m_strRoomID, *start);
	}

	m_pChattingList->SetCurrentFont(_DEF_FONTCOLOR_NOTIFY2);
	CString strFormat;
	strFormat.Format("\n[%s]님께 대화 신청을 하였습니다.", strID.c_str());
	m_pChattingList->AddText(strFormat.GetString());
	m_pChattingList->AddText("\n상대방이 수락할때까지 기다려 주시기 바랍니다.");
}

void CChattingRoomDlg::OnJoinSuccessRoom(void)
{
	m_pChattingList->SetCurrentFont(_DEF_FONTCOLOR_NOTIFY2);
	m_pChattingList->AddText("\n대화방에 입장하였습니다.");

	m_pEdit->SetEnable(true);
	LSY::CButton *pButton = (LSY::CButton *)GetObject(ID_CHATTING_BTN_SEND);
	pButton->SetEnable(true);

	SendInviteRoom();
}

void CChattingRoomDlg::OnJoinFaileRoom(void)
{
	m_pChattingList->SetCurrentFont(_DEF_FONTCOLOR_NOTIFY2);
	m_pChattingList->AddText("\n대화방에 입장하지 못했습니다.");
}

void CChattingRoomDlg::OnEnterRoom(CChatUser *pUser)
{
	AddUser(pUser);
	m_pChattingList->SetCurrentFont(_DEF_FONTCOLOR_NOTIFY2);

	CString strFormat;
	strFormat.Format("\n[%s]님께서 대화에 참여 하셨습니다.", pUser->GetNickName().c_str());
	m_pChattingList->AddText(strFormat.GetString());
}

void CChattingRoomDlg::OnLeaveRoom(const std::string &strLeaveID)
{
	m_pChattingList->SetCurrentFont(_DEF_FONTCOLOR_NOTIFY2);

	int nIndex = GetIDToListIndex(strLeaveID);
	if(nIndex != -1)
	{
		CChatUser *pUser = (CChatUser *)m_pUserList->GetItemData(nIndex);

		CString strFormat;
		strFormat.Format("\n[%s]님께서 대화를 종료 하셨습니다.", pUser->GetNickName().c_str());
		m_pChattingList->AddText(strFormat.GetString());

		delete pUser;
		m_pUserList->RemoveText(nIndex);

		if(m_strID == strLeaveID)
		{
			pUser = (CChatUser *)m_pUserList->GetItemData(0);
			std::string strMyID = GM().GetMyInfo()->UI.ID;
			if(pUser && pUser->GetID() != m_strID && pUser->GetID() != strMyID)
			{
				m_strID = pUser->GetID();
			}
			else
			{
				pUser = (CChatUser *)m_pUserList->GetItemData(1);
				if(pUser && pUser->GetID() != m_strID && pUser->GetID() != strMyID)
				{
					m_strID = pUser->GetID();
				}
			}
		}

		CheckChangeCaption();
	}
}

void CChattingRoomDlg::OnSendFaileChat(const std::string &strMessage)
{
	m_pChattingList->SetCurrentFont(_DEF_FONTCOLOR_NOTIFY2);
	CString strFormat;
	strFormat.Format("\n[%s] 메시지가 전달되지 않았습니다.", strMessage.c_str());
	m_pChattingList->AddText(strFormat.GetString());
}

void CChattingRoomDlg::OnReceiveChat(const std::string &strID, const std::string &strMessage)
{
	CString strFormat;
	bool bMy = false;
	CChatUser *pUser = FindUserFromID(strID);

	ASSERT(pUser != NULL);

	if(pUser->GetID() == GM().GetMyInfo()->UI.ID)
	{
		bMy = true;
	}
	else
	{
		FlashWindowEx(FLASHW_TRAY | FLASHW_TIMERNOFG, 0, 500);

		SYSTEMTIME time;
		GetLocalTime(&time);
		CString strFormat;
		strFormat.Format("마지막 메시지 받은 시각 : %04d-%02d-%02d %02d:%02d:%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
		m_pLastMessageTime->SetText(strFormat.GetString());
	}

	AddChatData(pUser, strMessage);

	m_pAvatarYou->SetAvatar(pUser->GetAvatarInfo());
	m_pAvatarYou->SetShow(true);

	strFormat.Format("\n[%s]님의 말:", pUser->GetNickName().c_str());
	m_pChattingList->SetCurrentFont(bMy?_DEF_FONTCOLOR_MYID:_DEF_FONTCOLOR_YOUID);
	m_pChattingList->AddText(strFormat.GetString());

	char szTBuf[512] = {0, };
	NMBASE::UTIL::NMIsBadToRep((char *)strMessage.c_str(), szTBuf, 512 );
	strFormat.Format("\n%s", szTBuf);
	m_pChattingList->SetCurrentFont(bMy?_DEF_FONTCOLOR_MYCHAT:_DEF_FONTCOLOR_YOUCHAT);
	m_pChattingList->AddText(strFormat.GetString());
}

CChatUser *CChattingRoomDlg::FindUserFromNickName(const std::string &strNIckName)
{
	int nCount = m_pUserList->GetItemCount();
	CChatUser *pUser = NULL;
	for(int i=0;i<nCount;++i)
	{
		pUser = (CChatUser *)m_pUserList->GetItemData(i);
		if(pUser->GetNickName() == strNIckName) return pUser;
	}
	return NULL;
}

CChatUser *CChattingRoomDlg::FindUserFromID(const std::string &strID)
{
	int nCount = m_pUserList->GetItemCount();
	CChatUser *pUser = NULL;
	for(int i=0;i<nCount;++i)
	{
		pUser = (CChatUser *)m_pUserList->GetItemData(i);
		if(pUser->GetID() == strID) return pUser;
	}
	return NULL;
}

int	CChattingRoomDlg::GetNickNameToListIndex(const std::string &strNickName)
{
	int nCount = m_pUserList->GetItemCount();
	CChatUser *pUser = NULL;
	for(int i=0;i<nCount;++i)
	{
		pUser = (CChatUser *)m_pUserList->GetItemData(i);
		if(pUser->GetNickName() == strNickName) return i;
	}
	return -1;
}

int	CChattingRoomDlg::GetIDToListIndex(const std::string &strID)
{
	int nCount = m_pUserList->GetItemCount();
	CChatUser *pUser = NULL;
	for(int i=0;i<nCount;++i)
	{
		pUser = (CChatUser *)m_pUserList->GetItemData(i);
		if(pUser->GetID() == strID) return i;
	}
	return -1;
}

void CChattingRoomDlg::AddChatData(CChatUser *pUser, const std::string &strMessage)
{
	if(m_nChatLength != 0)
	{
		++m_nChatLength;
	}

	CChatData data(pUser->GetID(), pUser->GetNickName(), strMessage);
	m_listChat.push_back(data);
	m_nChatLength += data.GetLength();

	CheckMaxNotifyMsg();
}

void CChattingRoomDlg::CheckMaxNotifyMsg(void)
{
	// 4000자 이상은 잘라냄
	if(m_nChatLength <= 4000) return;

	while(m_nChatLength <= 4000)
	{
		if(m_listChat.size() != 1)
			--m_nChatLength;

		CChatData data = m_listChat.front();
		m_listChat.pop_front();

		m_nChatLength -= data.GetLength();
	}
}

void CChattingRoomDlg::OnNotifyUser(void)
{
	std::list<std::pair<std::string, std::string> > listIDNickName;

	std::list<CChatData>::iterator start = m_listChat.begin();
	std::list<CChatData>::iterator end = m_listChat.end();
	bool bFirst = true;
	std::string strMessage;

	std::string strMyID = GM().GetMyInfo()->UI.ID;

	for(;start != end;++start)
	{
		if(bFirst) bFirst = false;
		else strMessage += "\n";

		strMessage += start->GetText();

		if(strMyID != start->GetID())
			listIDNickName.push_back(make_pair(start->GetID(), start->GetNickName()));
	}

	GM().BadUserReportMUChat(this, listIDNickName, strMessage);
}

void CChattingRoomDlg::OnInvateFaileUser(const std::string &strID)
{
	m_pChattingList->SetCurrentFont(_DEF_FONTCOLOR_NOTIFY2);
	CString strFormat;
	strFormat.Format("\n[%s]님을 초대하지 못했습니다.", strID.c_str());
	m_pChattingList->AddText(strFormat.GetString());
	m_pChattingList->AddText("\n존재하지 않는 아이디/닉네임이거나 오프라인 사용자일 수 있습니다.");
}

void CChattingRoomDlg::DisconnectDisable(void)
{
	m_pChattingList->SetCurrentFont(_DEF_FONTCOLOR_NOTIFY);
	m_pChattingList->AddText("\n커뮤니티 서버와의 접속이 끊어졌습니다.");
	m_bDisconnect = true;

	m_pEdit->SetText("");
	m_pEdit->SetEnable(false);
	LSY::CButton *pButton = (LSY::CButton *)GetObject(ID_CHATTING_BTN_SEND);
	pButton->SetEnable(false);
}

void CChattingRoomDlg::OnKillFocus(CWnd* pNewWnd)
{
	__super::OnKillFocus(pNewWnd);

	FlashWindowEx(FLASHW_STOP, 0, 0);
}
LRESULT CChattingRoomDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	switch(message)
	{
	case UPGM_POPUPMEMU_OPEN:
		if(m_pPopupMenu)
		{
			m_pPopupMenu->DestroyWindow();
			delete m_pPopupMenu;
			m_pPopupMenu = NULL;
		}

		m_pPopupMenu = (CChattingPopupMemu *)lParam;
		break;
	case UPGM_POPUPMEMU_CLOSE:
		if(m_pPopupMenu)
		{
			m_pPopupMenu->DestroyWindow();
			delete m_pPopupMenu;
			m_pPopupMenu = NULL;
		}
		switch(wParam)
		{
		case ID_LOBBYPOPUPMENU_TXTBTN_ADD_BLACKLIST:
			{
				CBlackBuddyAddDlg dlg;

				if(dlg.DoModal(CBlackBuddyAddDlg::BBADT_QUESTION, "", this) == IDCANCEL)
				{
					break;
				}

				if(CCommunityManager::Instance()->IsConnect())
				{
					CCommunityManager::Instance()->SendAddBlackBuddy(m_strSelectID);
				}
				else
				{
					CMsgBoxDlg dlg(this);
					dlg.DoModal(CMsgBoxDlg::EMBT_NOTICE, "커뮤니티 서버와 연결되어 있지 않아 블랙리스트 목록에 추가할 수 없습니다.");
				}
			}
			break;
		case ID_LOBBYPOPUPMENU_TXTBTN_PROFILE:
			{
				std::list<std::string> listNIckName;
				listNIckName.push_back(m_strSelectID);
				CCommunityManager::Instance()->SendGetUserInfo(CCommunityUserInfo::ECUIT_SHOWPROFILE, NULL, listNIckName,"");
			}
			break;
		case ID_LOBBYPOPUPMENU_TXTBTN_ADD_BUDDY:
			{
				CBuddyAddDlg dlg;

				if(dlg.DoModal(m_strSelectNickName, this) == IDCANCEL)
				{
					break;
				}

				if(!CCommunityManager::Instance()->IsConnect())
				{
					CMsgBoxDlg dlg(this);
					dlg.DoModal(CMsgBoxDlg::EMBT_NOTICE, "커뮤니티 서버와 연결되어 있지 않아 내친구 목록에 추가할 수 없습니다.");
				}
			}
			break;
		}
		break;
	}

	return CLSYLibWindowsBase::WindowProc(message, wParam, lParam);
}
