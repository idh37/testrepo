#include "stdafx.h"
#include "ChattingRoomAddUserDlg.h"
#include "ChattingRoomAddUserDlgID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static int s_nChattingRoomIDSeed = 0;
static std::map<int, CChattingRoomAddUserDlg *> s_mapChattingRoomAddUserDlg;

CChattingRoomAddUserDlg::CChattingRoomAddUserDlg(CWnd* pParent/* = NULL*/)
:	m_bFirst(true),
	m_pInviteUser(NULL)
{
	m_nID = s_nChattingRoomIDSeed;
	++s_nChattingRoomIDSeed;

	s_mapChattingRoomAddUserDlg[m_nID] = this;
}

BEGIN_MESSAGE_MAP(CChattingRoomAddUserDlg, LSY::CLSYLibDialogBase)
END_MESSAGE_MAP()

INT_PTR CChattingRoomAddUserDlg::DoModal(CWnd *pParent)
{
	m_pParentWnd = pParent;
	return CLSYLibDialogBase::DoModal(GetObjectMan(), ID_CHATTINGUSERADD);
}

bool CChattingRoomAddUserDlg::OnCreatedProject(void)
{
	LSY::CEdit *pEdit = (LSY::CEdit *)GetObject(ID_CHATTINGUSERADD_EDIT_NICKNAME);
	pEdit->AddHandler(LSY::EM_O_RETURN, LSY::Fnt(this, &CChattingRoomAddUserDlg::OnEditReturn));
	pEdit->AddHandler(LSY::EM_O_FOCUSIN, LSY::Fnt(this, &CChattingRoomAddUserDlg::OnFocusIn));
	pEdit->SetText("닉네임/아이디를 입력하세요.");

	LSY::CButton *pButton = (LSY::CButton *)GetObject(ID_CHATTINGUSERADD_BTN_OK);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CChattingRoomAddUserDlg::OnClickButton));

	pButton = (LSY::CButton *)GetObject(ID_CHATTINGUSERADD_BTN_CANCEL);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CChattingRoomAddUserDlg::OnClickButton));

	pButton = (LSY::CButton *)GetObject(ID_CHATTINGUSERADD_BTN_ADD_BUDDY);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CChattingRoomAddUserDlg::OnClickButton));

	pButton = (LSY::CButton *)GetObject(ID_CHATTINGUSERADD_BTN_ADD_CONNECT_USER);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CChattingRoomAddUserDlg::OnClickButton));

	pButton = (LSY::CButton *)GetObject(ID_CHATTINGUSERADD_BTN_FIND_ADD);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CChattingRoomAddUserDlg::OnClickButton));

	m_pInviteUser = (LSY::CRichText *)GetObject(ID_CHATTINGUSERADD_RICHTEXT_INVITE_USER);
	m_pInviteUser->CreateEmoticon(".\\common\\Data\\Community\\emoticon.spr");
	m_pInviteUser->SetEmoticonText("삭제", 0);
	m_pInviteUser->AddHandler(LSY::EM_O_LBUTTONDOWNITEM, LSY::Fnt(this, &CChattingRoomAddUserDlg::OnRichEditLButtonDown));

	return true;
}

LRESULT CChattingRoomAddUserDlg::OnRichEditLButtonDown(LSY::CMessage *pMsg)
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

LRESULT CChattingRoomAddUserDlg::OnEditReturn(LSY::CMessage *pMsg)
{
	if(m_bFirst) return TRUE;

	LSY::CEdit *pEdit = (LSY::CEdit *)GetObject(ID_CHATTINGUSERADD_EDIT_NICKNAME);
	CString strTemp = pEdit->GetText().c_str();
	strTemp.Trim();
	pEdit->SetText("");
	if(strTemp == "") return TRUE;

	CPlayer *pPlayer = GM().GetMyInfo();
	if(pPlayer == NULL) return TRUE;

	if(strTemp == pPlayer->UI.ID || strTemp == pPlayer->UI.NickName)
	{
		CMsgBoxDlg dlg(this);
		dlg.DoModal(CMsgBoxDlg::EMBT_NOTICE, "자기 자신을 초대할 수 없습니다.");
		return TRUE;
	}

	LISTSTRING listIDorNickName;;
	listIDorNickName.push_back(strTemp.GetString());

	CCommunityManager::Instance()->SendGetUserInfo(CCommunityUserInfo::ECUIT_FINDCHATADDUSER, m_nID, listIDorNickName, "");
	//사용자 검색
	return TRUE;
}

void CChattingRoomAddUserDlg::OnNotFind(void)
{
	CMsgBoxDlg dlg(this);
	dlg.DoModal(CMsgBoxDlg::EMBT_NOTICE, "유저정보를 찾지 못하여 초대리스트에 추가하지 못했습니다.");
}

void CChattingRoomAddUserDlg::OnFind(const std::string &strID, const std::string &strNickName)
{
	OnAddIDAndNickName(strID, strNickName);
}

LRESULT CChattingRoomAddUserDlg::OnFocusIn(LSY::CMessage *pMsg)
{
	if(m_bFirst)
	{
		m_bFirst = false;
		LSY::CEdit *pEdit = (LSY::CEdit *)GetObject(ID_CHATTINGUSERADD_EDIT_NICKNAME);
		pEdit->SetText("");
		pEdit->SetSpaceBetweenLetters(0);
	}
	return NULL;
}

LRESULT CChattingRoomAddUserDlg::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;
	int nID = msg->GetObject()->GetID();
	switch(nID)
	{
	case ID_CHATTINGUSERADD_BTN_OK:
		OnOK();
		break;
	case ID_CHATTINGUSERADD_BTN_ADD_BUDDY:
	case ID_CHATTINGUSERADD_BTN_ADD_CONNECT_USER:
		{
			CAddUserSelectDlg dlg((nID == ID_CHATTINGUSERADD_BTN_ADD_BUDDY)?true:false);
			if(dlg.DoModal(this) == IDOK)
			{
				const MAPSTRING &mapIDAndNickName = dlg.GetSelectIDAndNickName();

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
	case ID_CHATTINGUSERADD_BTN_CANCEL:
		OnCancel();
		break;
	case ID_CHATTINGUSERADD_BTN_FIND_ADD:
		OnEditReturn(NULL);
		break;
	}
	return TRUE;
}

void CChattingRoomAddUserDlg::OnAddIDAndNickName(const std::string &strID, const std::string &strNickName)
{
	MAPSTRING::iterator findIt = m_mapIDAndNickName.find(strID);
	if(findIt != m_mapIDAndNickName.end()) return;
	m_mapIDAndNickName[strID] = strNickName;

	findIt = m_mapIDAndNickName.find(strID);

	if(m_mapIDAndNickName.size() >= 2) m_pInviteUser->AddText(", ");
	m_pInviteUser->AddText(strNickName);
	m_pInviteUser->AddText("/삭제/", (LPARAM)(&findIt->first));
}

void CChattingRoomAddUserDlg::OnOK()
{
	CMsgBoxDlg dlg(this);

	if((int)m_mapIDAndNickName.size() == 0)
	{
		dlg.DoModal(CMsgBoxDlg::EMBT_NOTICE, "초대할 수 있는 대화상대가 없습니다. 초대할 대화상대를 입력해 주세요.");
		return;
	}

	s_mapChattingRoomAddUserDlg.erase(m_nID);
	__super::OnOK();
}

void CChattingRoomAddUserDlg::OnCancel()
{
	s_mapChattingRoomAddUserDlg.erase(m_nID);
	__super::OnCancel();
}

void CChattingRoomAddUserDlg::OnResetRichEdit(void)
{
	m_pInviteUser->Clear();

	MAPSTRING::iterator start = m_mapIDAndNickName.begin();
	MAPSTRING::iterator end = m_mapIDAndNickName.end();

	bool bFirst = true;
	for(;start != end;++start)
	{
		if(bFirst) bFirst = false;
		else m_pInviteUser->AddText(", ");

		m_pInviteUser->AddText(start->second);
		m_pInviteUser->AddText("/삭제/", (LPARAM)(&start->first));
	}
}

CChattingRoomAddUserDlg *GetIDToChattingRoomAddUserDlg(const int &nID)
{
	std::map<int, CChattingRoomAddUserDlg *>::iterator findIt = s_mapChattingRoomAddUserDlg.find(nID);
	if(findIt == s_mapChattingRoomAddUserDlg.end()) return NULL;
	return findIt->second;
}