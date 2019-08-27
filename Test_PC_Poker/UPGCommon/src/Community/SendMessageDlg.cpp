#include "stdafx.h"
#include "SendMessageDlg.h"
#include "SendMessageDlgID.h"
#include "AddUserSelectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static int s_nSendMessageDlgSeed = 0;

CSendMessageDlg::CSendMessageDlg(CWnd* pParent/* = NULL*/)
:	LSY::CLSYLibDialogBase(pParent),
	m_strNickName(""),
	m_strID(""),
	m_pMessage(NULL),
	m_pTotalByte(NULL),
	m_pSendUser(NULL),
	m_bFirst(true)
{
	m_nID = s_nSendMessageDlgSeed;
	++s_nSendMessageDlgSeed;
}

BEGIN_MESSAGE_MAP(CSendMessageDlg, LSY::CLSYLibDialogBase)
END_MESSAGE_MAP()

INT_PTR CSendMessageDlg::DoModal(std::string &strID, std::string &strNickName, CWnd *pParent)
{
	m_strNickName = strNickName;
	m_strID = strID;

	m_pParentWnd = pParent;

	LSY::CWindows *pWindows = (LSY::CWindows *)(GetObjectMan()->GetProjectFromID(ID_SENDMESSAGE)->GetMainObject());
	if (pWindows) pWindows->SetIndex(13);

	return CLSYLibDialogBase::DoModal(GetObjectMan(), ID_SENDMESSAGE);
}

bool CSendMessageDlg::OnCreatedProject(void)
{
	LSY::CImage *pImage = (LSY::CImage *)GetObject(ID_SENDMESSAGE_IMAGE_BACK);
	if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
		pImage->SetIndex(13);
	else
		pImage->SetIndex(0);

	LSY::CButton *pButton = (LSY::CButton *)GetObject(ID_SENDMESSAGE_BTN_SEND);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CSendMessageDlg::OnClickButton));

	pButton = (LSY::CButton *)GetObject(ID_SENDMESSAGE_BTN_CLOSE);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CSendMessageDlg::OnClickButton));

	pButton = (LSY::CButton *)GetObject(ID_SENDMESSAGE_BTN_ADD_BUDDY);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CSendMessageDlg::OnClickButton));

	pButton = (LSY::CButton *)GetObject(ID_SENDMESSAGE_BTN_ADD_CONNECT_USER);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CSendMessageDlg::OnClickButton));

	pButton = (LSY::CButton *)GetObject(ID_SENDMESSAGE_BTN_FIND_ADD);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CSendMessageDlg::OnClickButton));

	LSY::CEdit *pEdit = (LSY::CEdit *)GetObject(ID_SENDMESSAGE_EDIT_RECEIVE_ID);
	pEdit->AddHandler(LSY::EM_O_RETURN, LSY::Fnt(this, &CSendMessageDlg::OnEditReturn));
	pEdit->AddHandler(LSY::EM_O_FOCUSIN, LSY::Fnt(this, &CSendMessageDlg::OnFocusIn));
	pEdit->SetUseClipboard(false) ;
	pEdit->SetText("닉네임/아이디를 입력하세요.");

	m_pMessage = (LSY::CEdit *)GetObject(ID_SENDMESSAGE_EDIT_MESSAGE);
	m_pMessage->SetUseClipboard(false) ;
	m_pMessage->SetText("");
	m_pObjectMan->SetFocus(m_pMessage);

	m_pTotalByte = (LSY::CTextNumber *)GetObject(ID_SENDMESSAGE_TXTNB_TOTAL_BYTE);
	m_pTotalByte->SetNumber(0);
	
	CString strFormat;
	strFormat.Format("/%dbyte)", m_pMessage->GetLimitText());

	m_pTotalByte->SetFirstUnit(strFormat.GetString());

	m_pMessage->AddHandler(LSY::EM_O_KEYMESSAGEPROC, LSY::Fnt(this, &CSendMessageDlg::OnKeyProc));

	m_pSendUser = (LSY::CRichText *)GetObject(ID_SENDMESSAGE_RICHTEXT_SEND_USER);
	m_pSendUser->CreateEmoticon(".\\common\\Data\\Community\\emoticon.spr");
	m_pSendUser->SetEmoticonText("삭제", 0);
	m_pSendUser->AddHandler(LSY::EM_O_LBUTTONDOWNITEM, LSY::Fnt(this, &CSendMessageDlg::OnRichEditLButtonDown));

	m_bFirst = true;

	m_mapIDAndNickName.clear();

	if(m_strNickName == "@#;;!,/*그룹채팅*/,@#;;!") CCommunityManager::Instance()->GetGroupInIDAndNickName(m_strID, m_mapIDAndNickName);
	else if(m_strID != "") m_mapIDAndNickName[m_strID] = m_strNickName;

	//if(m_mapIDAndNickName.size()) OnResetRichEdit();

	return TRUE;
}

LRESULT CSendMessageDlg::OnRichEditLButtonDown(LSY::CMessage *pMsg)
{
	LSY::CMO_RichItemLButtonDown *msg = (LSY::CMO_RichItemLButtonDown *)pMsg;
	LSY::CRichItem *pItem = msg->GetItem();
	if(pItem->m_lParam != NULL)
	{
		std::string *pStrID = (std::string *)pItem->m_lParam;
		MAPSTRING::iterator findIt = m_mapIDAndNickName.find(*pStrID);
		if(findIt != m_mapIDAndNickName.end())
		{
			m_mapIDAndNickName.erase(findIt);
			OnResetRichEdit();
		}
	}
	return TRUE;
}

LRESULT CSendMessageDlg::OnEditReturn(LSY::CMessage *pMsg)
{
	if(m_bFirst) return TRUE;

	LSY::CEdit *pEdit = (LSY::CEdit *)GetObject(ID_SENDMESSAGE_EDIT_RECEIVE_ID);
	CString strTemp = pEdit->GetText().c_str();
	strTemp.Trim();
	pEdit->SetText("");
	if(strTemp == "") return TRUE;

	CPlayer *pPlayer = GM().GetMyInfo();
	if(pPlayer == NULL) return TRUE;

	if(strTemp == pPlayer->UI.ID || strTemp == pPlayer->UI.NickName)
	{
		CMsgBoxDlg dlg(this);
		dlg.DoModal(CMsgBoxDlg::EMBT_NOTICE, "자기 자신에게 쪽지를 보낼수 없습니다.");
		return TRUE;
	}

	LISTSTRING listIDorNickName;;
	listIDorNickName.push_back(strTemp.GetString());

	CCommunityManager::Instance()->SendGetUserInfo(CCommunityUserInfo::ECUIT_FINDMEMOADDUSER, m_nID, listIDorNickName, "");
	//사용자 검색
	return TRUE;
}

void CSendMessageDlg::OnNotFind(const int &nID)
{
	if(m_nID != nID) return;
	if(!IsWindow(GetSafeHwnd()) || !IsWindowVisible()) return;

	CMsgBoxDlg dlg(this);
	dlg.DoModal(CMsgBoxDlg::EMBT_NOTICE, "유저정보를 찾지 못하여 받는사람 리스트에 추가하지 못했습니다.");
}

void CSendMessageDlg::OnFind(const int &nID, const std::string &strID, const std::string &strNickName)
{
	if(m_nID != nID) return;
	if(!IsWindow(GetSafeHwnd()) || !IsWindowVisible()) return;

	// 쪽지보내기 한명만 가능하도록 수정 작업.
	m_mapIDAndNickName.clear() ;
	OnResetRichEdit() ;

	OnAddIDAndNickName(strID, strNickName);
}

LRESULT CSendMessageDlg::OnFocusIn(LSY::CMessage *pMsg)
{
	if(m_bFirst)
	{
		m_bFirst = false;
		LSY::CEdit *pEdit = (LSY::CEdit *)GetObject(ID_SENDMESSAGE_EDIT_RECEIVE_ID);
		pEdit->SetText("");
		pEdit->SetSpaceBetweenLetters(0);
	}
	return NULL;
}

LRESULT CSendMessageDlg::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;
	int nID = msg->GetObject()->GetID();
	switch(nID)
	{
	case ID_SENDMESSAGE_BTN_SEND:
		if(OnSendMessage())
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_PERSONAL_AREA_POKER_MEMOSEND_SEND);
			#endif //UPGRADE_10210619_ADD_CLICKLOG
			
			OnOK();
		}
		break;
	case ID_SENDMESSAGE_BTN_CLOSE:
		#ifdef UPGRADE_10210619_ADD_CLICKLOG
			SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_PERSONAL_AREA_POKER_MEMOSEND_CANCEL);
		#endif //UPGRADE_10210619_ADD_CLICKLOG

		OnCancel();
		break;
	case ID_SENDMESSAGE_BTN_FIND_ADD:
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_PERSONAL_AREA_POKER_MEMOSEND_RECEIVER_ENTER);
			#endif //UPGRADE_10210619_ADD_CLICKLOG

			OnEditReturn(NULL);
		}
		break;
	case ID_SENDMESSAGE_BTN_ADD_BUDDY:
	case ID_SENDMESSAGE_BTN_ADD_CONNECT_USER:
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				if (nID == ID_SENDMESSAGE_BTN_ADD_BUDDY)
					SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_PERSONAL_AREA_POKER_MEMOSEND_FRIEND_LIST);
				else if (nID == ID_SENDMESSAGE_BTN_ADD_CONNECT_USER)
					SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_PERSONAL_AREA_POKER_MEMOSEND_USER_LIST);
			#endif //UPGRADE_10210619_ADD_CLICKLOG

			CAddUserSelectDlg dlg((nID == ID_SENDMESSAGE_BTN_ADD_BUDDY)?true:false);
			if(dlg.DoModal(this) == IDOK)
			{
				const MAPSTRING &mapIDAndNickName = dlg.GetSelectIDAndNickName();

				// 쪽지보내기 한명만 가능하도록 수정 작업.
				m_mapIDAndNickName.clear() ;
				OnResetRichEdit() ;

				MAPSTRING::const_iterator start, end;
				start = mapIDAndNickName.begin();
				end = mapIDAndNickName.end();
				for(;start != end;++start)
				{
					OnAddIDAndNickName(start->first, start->second);
				}
			}
		}
		break;
	}
	return TRUE;
}

void CSendMessageDlg::OnAddIDAndNickName(const std::string &strID, const std::string &strNickName)
{
	MAPSTRING::iterator findIt = m_mapIDAndNickName.find(strID);
	if(findIt != m_mapIDAndNickName.end()) return;
	m_mapIDAndNickName[strID] = strNickName;

	findIt = m_mapIDAndNickName.find(strID);

	if(m_mapIDAndNickName.size() >= 2) m_pSendUser->AddText(", ");
	m_pSendUser->AddText(strNickName);
	m_pSendUser->AddText("/삭제/", (LPARAM)(&findIt->first));
}

void CSendMessageDlg::OnResetRichEdit(void)
{
	m_pSendUser->Clear();

	MAPSTRING::iterator start = m_mapIDAndNickName.begin();
	MAPSTRING::iterator end = m_mapIDAndNickName.end();

	bool bFirst = true;
	for(;start != end;++start)
	{
		if(bFirst) bFirst = false;
		else m_pSendUser->AddText(", ");

		m_pSendUser->AddText(start->second);
		m_pSendUser->AddText("/삭제/", (LPARAM)(&start->first));
	}
}

bool CSendMessageDlg::OnSendMessage(void)
{
	CMsgBoxDlg dlg(this);

	LSY::CEdit *pEdit = (LSY::CEdit *)GetObject(ID_SENDMESSAGE_EDIT_RECEIVE_ID);

	size_t nSize = m_mapIDAndNickName.size();
	if(nSize == 0)
	{
		dlg.DoModal(CMsgBoxDlg::EMBT_NOTICE, "쪽지를 보낼 사용자를 입력해 주십시요!\n본인에게 쪽지를 보내려 하거나 쪽지를 보낼수 있는 사용자가 없습니다.");
		return false;
	}
	else if(nSize > 10)
	{
		dlg.DoModal(CMsgBoxDlg::EMBT_NOTICE, "쪽지는 최대 10명까지 발송 가능합니다.");
		return false;
	}

	std::string strMessage = m_pMessage->GetText();
	if(strMessage == "")
	{
		dlg.DoModal(CMsgBoxDlg::EMBT_NOTICE, "쪽지의 내용을 입력해 주세요!");
		return false;
	}

	const int nMaxText = m_pMessage->GetLimitText() + 1;
	char *szTChat = new char[nMaxText];
	memset(szTChat, 0, nMaxText);
	NMBASE::UTIL::NMIsBadToRep( (char *)strMessage.c_str(), szTChat, nMaxText );
	if(strMessage != szTChat)
	{
		m_pMessage->SetText(szTChat);
		dlg.DoModal(CMsgBoxDlg::EMBT_NOTICE, "쪽지 내용에 사용할 수 없는 문구가 포함되어 있습니다. 사용할 수 없는 문구는 *로 표시 됩니다. 해당 부분을 수정 하고 다시 보내주시기 바랍니다.");
		return false;
	}

	LISTSTRING listNickName;

	MAPSTRING::iterator start = m_mapIDAndNickName.begin();
	MAPSTRING::iterator end = m_mapIDAndNickName.end();
	for(;start != end;++start)
	{
		listNickName.push_back(start->first);
	}

	CCommunityManager::Instance()->SendGetUserInfo(CCommunityUserInfo::ECUIT_SENDMEMO, (LPARAM)NULL, listNickName, strMessage);
	return true;
}

LRESULT CSendMessageDlg::OnKeyProc(LSY::CMessage *pMsg)
{
	m_pTotalByte->SetNumber(m_pMessage->GetLength());
	return TRUE;
}