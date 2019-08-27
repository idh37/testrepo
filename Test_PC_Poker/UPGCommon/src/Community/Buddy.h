#pragma once

UPG_COMMON_EXPORT extern bool BuddyGroupEntryListCompareFunction(LSY::CListItemData *pItem1, LSY::CListItemData *pItem2, bool bIncrement, LPARAM lParam);

class UPG_COMMON_EXPORT CBuddyBase
{
protected:
	bool			m_bGroup;
public:
	CBuddyBase(const bool &bGroup);
	virtual ~CBuddyBase(){}

	const bool &IsGroup(void){return m_bGroup;}
};

class CBuddyGroup;
#ifndef _DEF_LISTBUDDY
#define _DEF_LISTBUDDY
typedef std::list<CBuddyFriend *>	LISTBUDDY;
#endif

class UPG_COMMON_EXPORT CBuddyFriend : public CBuddyBase
{
protected:
	std::string				m_strID;
	std::string				m_strNickName;
	std::string				m_strCN;
	int					m_pAvatarInfo[MAX_AVATARLAYER];
	Camel::Dar2::SS::Show	m_nStatus;
	std::string				m_strStatusInfo;
	INT64					m_i64Money;
	IDX_GAME				m_nGame;
	int						m_nRoomNumber;
	CBuddyGroup				*m_pGroup;
public:
	CBuddyFriend();
	virtual ~CBuddyFriend(){}

	void SetBuddy(const Camel::Dar2::SS::BuddyInfo *pBuddy);
	void SetChangeInfo(const Camel::Dar2::SS::PresenceInfo &presenceInfo);
	void SetGroup(CBuddyGroup *pGroup){m_pGroup = pGroup;}
	void SetBuddy(const CBuddyFriend *pBuddy);

	const std::string &GetID(void){return m_strID;}
	const std::string &GetNickName(void){return m_strNickName;}
	const std::string &GetCN(void){return m_strCN;}
	const int *GetAvatarInfo(void){return m_pAvatarInfo;}
	const Camel::Dar2::SS::Show &GetStatus(void){return m_nStatus;}
	const std::string &GetStatusInfo(void){return m_strStatusInfo;}
	const INT64 &GetMoney(void){return m_i64Money;}
	const IDX_GAME &GetGame(void){return m_nGame;}
	const int &GetRoomNumber(void){return m_nRoomNumber;}
	CBuddyGroup *GetGroup(void){return m_pGroup;}

	const bool IsOffline(void);

	void Clear(void);
};

class UPG_COMMON_EXPORT CBuddyGroup : public CBuddyBase
{
protected:
	bool						m_bExpand;
	std::string					m_strName;
	std::string					m_strEditName;
	LISTBUDDY					m_listFriend;
	bool						m_bEdit;
	bool						m_bSendChangeName;
public:
	CBuddyGroup();
	virtual ~CBuddyGroup();

	void Clear(void);
	void SetBuddyGroup(Camel::Dar2::SS::BuddyGroupEntry *pGroup);
	void AddBuddy(CBuddyFriend *pBuddy);
	CBuddyFriend *GetBuddy(std::string strID);

	void SetName(const std::string strName){if(m_strName == strName) return; m_strName = strName;};
	const std::string &GetName(void){return m_strName;}
	const LISTBUDDY &GetBuddyList(void){return m_listFriend;}

	const bool &IsExpand(void){return m_bExpand;}
	void SetExpand(const bool &bExpand);

	const bool &IsEdit(void){return m_bEdit;}
	void SetEdit(const bool &bEdit);
	const bool IsEditChangeName(void){return (m_strEditName == m_strName)?false:true;}
	const bool IsEmpty(void){return m_listFriend.size()?false:true;}

	int GetBuddyCount(void){return (int)m_listFriend.size();}
	int GetOnlineCount(void);

	void SetEditName(const std::string strName){if(m_strEditName == strName) return; m_strEditName = strName;};
	const std::string &GetEditName(void){return m_strEditName;}

	const bool &IsSendChangeName(void){return m_bSendChangeName;}
	void SetSendChangeName(const bool &bSend){if(m_bSendChangeName == bSend) return; m_bSendChangeName = bSend;}

	const bool IsSameName(const std::string &strName);

	void AllDeleteFriend(LSY::CList *pList);
	void AllAddFriend(LSY::CList *pList);
};

class UPG_COMMON_EXPORT CBlackBuddy
{
protected:
	std::string				m_strID;
	std::string				m_strNickName;

public:
	CBlackBuddy();
	virtual ~CBlackBuddy(){}

	void SetBuddy(const Camel::Dar2::SS::BlackBuddy *pBuddy);
	void SetBuddy(const std::string &strID, const std::string &strNickName);

	const std::string &GetID(void){return m_strID;}
	const std::string &GetNickName(void){return m_strNickName;}
};

class UPG_COMMON_EXPORT CReverseBuddy
{
protected:
	std::string				m_strID;
	std::string				m_strNickName;

public:
	CReverseBuddy();
	virtual ~CReverseBuddy(){}

	void SetBuddy(const Camel::Dar2::SS::BuddyInfo *pBuddy);

	const std::string &GetID(void){return m_strID;}
	const std::string &GetNickName(void){return m_strNickName;}
};