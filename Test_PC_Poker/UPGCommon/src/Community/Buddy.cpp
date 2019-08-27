#include "stdafx.h"
#include "Buddy.h"
#include "gameinfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

bool BuddyGroupEntryListCompareFunction(LSY::CListItemData *pItem1, LSY::CListItemData *pItem2, bool bIncrement, LPARAM lParam)
{
	CBuddyBase *pBuddy1 = (CBuddyBase *)pItem1->GetItemData();
	CBuddyBase *pBuddy2 = (CBuddyBase *)pItem2->GetItemData();

	if(pBuddy1->IsGroup() && pBuddy2->IsGroup())
	{
		CBuddyGroup *pGroup1 = (CBuddyGroup *)pBuddy1;
		CBuddyGroup *pGroup2 = (CBuddyGroup *)pBuddy2;

		if(pGroup1->IsSendChangeName() && pGroup2->IsSendChangeName())
		{
			return strcmp(pGroup1->GetEditName().c_str(), pGroup2->GetEditName().c_str()) < 0;	
		}
		else if(pGroup1->IsSendChangeName())
		{
			return strcmp(pGroup1->GetEditName().c_str(), pGroup2->GetName().c_str()) < 0;
		}
		else if(pGroup2->IsSendChangeName())
		{
			return strcmp(pGroup1->GetName().c_str(), pGroup2->GetEditName().c_str()) < 0;
		}

		return strcmp(pGroup1->GetName().c_str(), pGroup2->GetName().c_str()) < 0;
	}
	else if(pBuddy1->IsGroup())
	{
		CBuddyGroup *pGroup1 = (CBuddyGroup *)pBuddy1;
		CBuddyGroup *pGroup2 = ((CBuddyFriend *)pBuddy2)->GetGroup();

		if(pGroup1->IsSendChangeName() && pGroup2->IsSendChangeName())
		{
			return strcmp(pGroup1->GetEditName().c_str(), pGroup2->GetEditName().c_str()) < 0;	
		}
		else if(pGroup1->IsSendChangeName())
		{
			return strcmp(pGroup1->GetEditName().c_str(), pGroup2->GetName().c_str()) < 0;
		}
		else if(pGroup2->IsSendChangeName())
		{
			return strcmp(pGroup1->GetName().c_str(), pGroup2->GetEditName().c_str()) < 0;
		}

		return strcmp(pGroup1->GetName().c_str(), pGroup2->GetName().c_str()) < 0;
	}
	else if(pBuddy2->IsGroup())
	{
		CBuddyGroup *pGroup1 = ((CBuddyFriend *)pBuddy1)->GetGroup();
		CBuddyGroup *pGroup2 = (CBuddyGroup *)pBuddy2;

		if(pGroup1->IsSendChangeName() && pGroup2->IsSendChangeName())
		{
			return strcmp(pGroup1->GetEditName().c_str(), pGroup2->GetEditName().c_str()) < 0;	
		}
		else if(pGroup1->IsSendChangeName())
		{
			return strcmp(pGroup1->GetEditName().c_str(), pGroup2->GetName().c_str()) < 0;
		}
		else if(pGroup2->IsSendChangeName())
		{
			return strcmp(pGroup1->GetName().c_str(), pGroup2->GetEditName().c_str()) < 0;
		}

		return strcmp(pGroup1->GetName().c_str(), pGroup2->GetName().c_str()) < 0;
	}

	CBuddyGroup *pGroup1 = ((CBuddyFriend *)pBuddy1)->GetGroup();
	CBuddyGroup *pGroup2 = ((CBuddyFriend *)pBuddy2)->GetGroup();

	if(pGroup1 == pGroup2)
	{
		CBuddyFriend *pFriend1 = (CBuddyFriend *)pBuddy1;
		CBuddyFriend *pFriend2 = (CBuddyFriend *)pBuddy2;

		return strcmp(pFriend1->GetNickName().c_str(), pFriend2->GetNickName().c_str()) < 0;
	}
	else
	{
		if(pGroup1->IsSendChangeName() && pGroup2->IsSendChangeName())
		{
			return strcmp(pGroup1->GetEditName().c_str(), pGroup2->GetEditName().c_str()) < 0;	
		}
		else if(pGroup1->IsSendChangeName())
		{
			return strcmp(pGroup1->GetEditName().c_str(), pGroup2->GetName().c_str()) < 0;
		}
		else if(pGroup2->IsSendChangeName())
		{
			return strcmp(pGroup1->GetName().c_str(), pGroup2->GetEditName().c_str()) < 0;
		}

		return strcmp(pGroup1->GetName().c_str(), pGroup2->GetName().c_str()) < 0;
	}

	return false;
}

CBuddyBase::CBuddyBase(const bool &bGroup)
:	m_bGroup(bGroup)
{
}

CBuddyFriend::CBuddyFriend()
:	CBuddyBase(false),
	m_strID(""),
	m_strNickName(""),
	m_strCN(""),
	m_nStatus(Camel::Dar2::SS::SHOW_XA),
	m_strStatusInfo(""),
	m_nGame(IDX_GAME_NULL),
	m_nRoomNumber(0),
	m_i64Money(0),
	m_pGroup(NULL)
{
	ZeroMemory(m_pAvatarInfo, sizeof(int) * MAX_AVATARLAYER);
}

void CBuddyFriend::Clear(void)
{
	m_strID = "";
	m_strNickName = "";
	m_strCN = "";
	m_nStatus = Camel::Dar2::SS::SHOW_XA;
	m_strStatusInfo = "";
	m_nGame = IDX_GAME_NULL;
	m_nRoomNumber = 0;
	m_i64Money = 0;
	m_pGroup = NULL;
	ZeroMemory(m_pAvatarInfo, sizeof(int) * MAX_AVATARLAYER);
}

void CBuddyFriend::SetBuddy(const CBuddyFriend *pBuddy)
{
	m_strID = pBuddy->m_strID;
	m_strNickName = pBuddy->m_strNickName;
	m_strCN = pBuddy->m_strCN;
	m_nStatus = pBuddy->m_nStatus;
	m_strStatusInfo = pBuddy->m_strStatusInfo;
	m_nGame = pBuddy->m_nGame;
	m_nRoomNumber = pBuddy->m_nRoomNumber;
	m_i64Money = pBuddy->m_i64Money;
	m_pGroup = pBuddy->m_pGroup;
	memcpy(m_pAvatarInfo, pBuddy->m_pAvatarInfo, sizeof(int) * MAX_AVATARLAYER);
}

void CBuddyFriend::SetBuddy(const Camel::Dar2::SS::BuddyInfo *pBuddy)
{
	ASSERT(pBuddy != NULL);

	m_strID = pBuddy->userID;
	m_strNickName = pBuddy->profile.nickname;
	m_strCN = pBuddy->profile.userCN;
	memcpy(m_pAvatarInfo, pBuddy->profile.avatarInfo, sizeof(int) * MAX_AVATARLAYER);
	m_nStatus = pBuddy->presence.show;
	m_strStatusInfo = pBuddy->presence.status;

	if(pBuddy->presence.gameinfo == "")
	{
		m_nGame = IDX_GAME_NULL;
		m_nRoomNumber = 0;
		m_i64Money = 0;
	}
	else
	{
		Camel::Dar2::gameinfo clGameinfo;
		clGameinfo.LoadJSON(pBuddy->presence.gameinfo);

		m_nGame = (IDX_GAME)clGameinfo.gameid;
		m_nRoomNumber = clGameinfo.roomnum;
		m_i64Money = clGameinfo.money;
	}
}

void CBuddyFriend::SetChangeInfo(const Camel::Dar2::SS::PresenceInfo &presenceInfo)
{
	m_nStatus = presenceInfo.show;
	m_strStatusInfo = presenceInfo.status;

	if(presenceInfo.gameinfo == "")
	{
		m_nGame = IDX_GAME_NULL;
		m_nRoomNumber = 0;
		m_i64Money = 0;
	}
	else
	{
		Camel::Dar2::gameinfo clGameinfo;
		clGameinfo.LoadJSON(presenceInfo.gameinfo);

		m_nGame = (IDX_GAME)clGameinfo.gameid;
		m_nRoomNumber = clGameinfo.roomnum;
		m_i64Money = clGameinfo.money;
	}
}

const bool CBuddyFriend::IsOffline(void)
{
	if(m_nStatus == Camel::Dar2::SS::SHOW_XA || (m_nGame < 0 || m_nGame >= MAX_GAME)) return true;
	return false;
}

CBuddyGroup::CBuddyGroup()
:	CBuddyBase(true),
	m_strName(""),
	m_bExpand(true),
	m_bEdit(false),
	m_strEditName(""),
	m_bSendChangeName(false)
{
}

CBuddyGroup::~CBuddyGroup()
{
	Clear();
}

void CBuddyGroup::Clear(void)
{
	LISTBUDDY::iterator start = m_listFriend.begin();
	LISTBUDDY::iterator end = m_listFriend.end();

	CBuddyFriend *pFriend = NULL;
	for(;start != end;++start)
	{
		pFriend = *start;
		delete pFriend;
	}

	m_listFriend.clear();
}

void CBuddyGroup::SetBuddyGroup(Camel::Dar2::SS::BuddyGroupEntry *pGroup)
{
	ASSERT(pGroup != NULL);

	Clear();

	m_strName = pGroup->groupName;

	std::list<Camel::Dar2::SS::BuddyInfo *>::iterator start = pGroup->buddies.begin();
	std::list<Camel::Dar2::SS::BuddyInfo *>::iterator end = pGroup->buddies.end();
	CBuddyFriend *pBuddy = NULL;

	for(;start != end;++start)
	{
		pBuddy = new CBuddyFriend;
		pBuddy->SetBuddy(*start);
		pBuddy->SetGroup(this);

		m_listFriend.push_back(pBuddy);
	}
}

void CBuddyGroup::AddBuddy(CBuddyFriend *pBuddy)
{
	pBuddy->SetGroup(this);
	m_listFriend.push_back(pBuddy);
}

CBuddyFriend *CBuddyGroup::GetBuddy(std::string strID)
{
	LISTBUDDY::iterator start = m_listFriend.begin();
	LISTBUDDY::iterator end = m_listFriend.end();

	CBuddyFriend *pBuddy = NULL;
	for(;start != end;++start)
	{
		pBuddy = *start;
		if(pBuddy->GetID() == strID)
			break;
	}

	if(start != end)
	{
		m_listFriend.erase(start);
		return pBuddy;
	}

	return NULL;
}

int CBuddyGroup::GetOnlineCount(void)
{
	int nRet = 0;

	LISTBUDDY::iterator start = m_listFriend.begin();
	LISTBUDDY::iterator end = m_listFriend.end();

	CBuddyFriend *pBuddy = NULL;
	for(;start != end;++start)
	{
		pBuddy = *start;
		if(!pBuddy->IsOffline()) ++nRet;
	}

	return nRet;
}

const bool CBuddyGroup::IsSameName(const std::string &strName)
{
	std::string strCurName = IsSendChangeName()?GetEditName():GetName();
	return (strCurName == strName)?true:false;
}

void CBuddyGroup::SetExpand(const bool &bExpand)
{
	if(m_bExpand == bExpand) return;
	m_bExpand = bExpand;
}

void CBuddyGroup::SetEdit(const bool &bEdit)
{
	if(m_bEdit == bEdit) return;
	m_bEdit = bEdit;
	if(m_bEdit) m_strEditName = m_strName;
}

void CBuddyGroup::AllDeleteFriend(LSY::CList *pList)
{
	LISTBUDDY::iterator start = m_listFriend.begin();
	LISTBUDDY::iterator end = m_listFriend.end();

	CBuddyFriend *pFriend = NULL;
	for(;start != end;++start)
	{
		pFriend = *start;
		pList->DeleteItem((LPARAM)pFriend);
	}
}

void CBuddyGroup::AllAddFriend(LSY::CList *pList)
{
	LISTBUDDY::iterator start = m_listFriend.begin();
	LISTBUDDY::iterator end = m_listFriend.end();

	CBuddyFriend *pFriend = NULL;
	bool bIsShowConnect = CCommunityManager::Instance()->IsShowConnectBuddy();
	for(;start != end;++start)
	{
		pFriend = *start;
		if(!bIsShowConnect || !pFriend->IsOffline()) pList->AddItem((LPARAM)pFriend);
	}
}

CBlackBuddy::CBlackBuddy()
:	m_strID(""),
	m_strNickName("")
{
}

void CBlackBuddy::SetBuddy(const Camel::Dar2::SS::BlackBuddy *pBuddy)
{
	ASSERT(pBuddy != NULL);
	m_strID = pBuddy->buddyID;
	m_strNickName = pBuddy->buddyNickname;
}

void CBlackBuddy::SetBuddy(const std::string &strID, const std::string &strNickName)
{
	m_strID = strID;
	m_strNickName = strNickName;
}

CReverseBuddy::CReverseBuddy()
:	m_strID(""),
	m_strNickName("")
{
}

void CReverseBuddy::SetBuddy(const Camel::Dar2::SS::BuddyInfo *pBuddy)
{
	ASSERT(pBuddy != NULL);
	m_strID = pBuddy->userID;
	m_strNickName = pBuddy->profile.nickname;
}