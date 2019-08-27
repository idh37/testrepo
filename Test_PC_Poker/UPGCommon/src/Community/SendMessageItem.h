#pragma once

class CSendMessageItemBase
{
public:
	CSendMessageItemBase(void);
	virtual ~CSendMessageItemBase(void);

	virtual bool IsCheckConnect(void){return true;}
	virtual bool SendMessage(void) = 0;
};

class CSendMessageLogin : public CSendMessageItemBase
{
protected:
	int		m_nThreadPoolCount;
	DWORD	m_dwTimeOut;
public:	
	CSendMessageLogin(const int &nThreadPoolCount, const DWORD &dwTimeOut):m_nThreadPoolCount(nThreadPoolCount),m_dwTimeOut(dwTimeOut){}
	virtual ~CSendMessageLogin(void){}

	virtual bool IsCheckConnect(void){return false;}
	virtual bool SendMessage(void);
};

class CSendMessageMemo : public CSendMessageItemBase
{
protected:
	std::list<std::string>	m_listID;
	std::string				m_strMemo;
public:	
	CSendMessageMemo(const std::list<std::string> &listID, const std::string &strMemeo):m_listID(listID), m_strMemo(strMemeo){}
	virtual ~CSendMessageMemo(void){}

	virtual bool SendMessage(void);
};

class CSendMessageMemoList : public CSendMessageItemBase
{
public:	
	CSendMessageMemoList(){}
	virtual ~CSendMessageMemoList(void){}

	virtual bool SendMessage(void);
};

class CSendMessageMemoStorageList : public CSendMessageItemBase
{
public:	
	CSendMessageMemoStorageList(){}
	virtual ~CSendMessageMemoStorageList(void){}

	virtual bool SendMessage(void);
};

class CSendMessageMemoInfo : public CSendMessageItemBase
{
public:	
	CSendMessageMemoInfo(){}
	virtual ~CSendMessageMemoInfo(void){}

	virtual bool SendMessage(void);
};

class CSendMessageRemoveMemoList : public CSendMessageItemBase
{
public:
	std::list<std::string>	m_listRemoveMemoSeq;
public:	
	CSendMessageRemoveMemoList(const std::list<std::string> &listRemove):m_listRemoveMemoSeq(listRemove){}
	virtual ~CSendMessageRemoveMemoList(void){}

	virtual bool SendMessage(void);
};

class CSendMessageStorageMoveMemoList : public CSendMessageItemBase
{
public:
	std::list<std::string>	m_listStorageMoveMemoSeq;
public:	
	CSendMessageStorageMoveMemoList(const std::list<std::string> &listMove):m_listStorageMoveMemoSeq(listMove){}
	virtual ~CSendMessageStorageMoveMemoList(void){}

	virtual bool SendMessage(void);
};

class CSendReadMemo : public CSendMessageItemBase
{
public:
	std::string m_strNode;
public:	
	CSendReadMemo(const std::string &strNode):m_strNode(strNode){}
	virtual ~CSendReadMemo(void){}

	virtual bool SendMessage(void);
};

class CSendGetOption : public CSendMessageItemBase
{
public:	
	CSendGetOption(void){}
	virtual ~CSendGetOption(void){}

	virtual bool SendMessage(void);
};

class CSendSetOption : public CSendMessageItemBase
{
public:
	ENUM_COMMUNITY_OPTION_TYPE				m_nOptionType;
	ENUM_COMMUNITY_OPTION_PERMISION_TYPE	m_nPermition;
public:	
	CSendSetOption(const ENUM_COMMUNITY_OPTION_TYPE &nOptionType, const ENUM_COMMUNITY_OPTION_PERMISION_TYPE &nPermition):m_nOptionType(nOptionType),m_nPermition(nPermition){}
	virtual ~CSendSetOption(void){}

	virtual bool SendMessage(void);
};

class CSendCreateChatRoom : public CSendMessageItemBase
{
protected:
	std::string				m_strPassword;
	std::list<std::string>	m_listInviteID;
public:	
	CSendCreateChatRoom(const std::list<std::string> &listInviteID, const std::string &strPassword):m_listInviteID(listInviteID), m_strPassword(strPassword){}
	virtual ~CSendCreateChatRoom(){}

	virtual bool SendMessage(void);
};

class CSendInviteChatRoom : public CSendMessageItemBase
{
protected:
	std::string		m_strRoomID;
	std::string		m_strRecvID;
public:	
	CSendInviteChatRoom(const std::string &strRoomID, const std::string &strRecvID):m_strRoomID(strRoomID),m_strRecvID(strRecvID){}
	virtual ~CSendInviteChatRoom(){}

	virtual bool SendMessage(void);
};

class CSendJoinChatRoom : public CSendMessageItemBase
{
public:
	std::string m_strRoomID;
	bool		m_bMaster;
public:	
	CSendJoinChatRoom(const std::string &strRoomID, const bool &bMaster):m_strRoomID(strRoomID), m_bMaster(bMaster){}
	virtual ~CSendJoinChatRoom(void){}

	virtual bool SendMessage(void);
};

class CSendLeaveChatRoom : public CSendMessageItemBase
{
public:
	std::string m_strRoomID;
public:	
	CSendLeaveChatRoom(const std::string &strRoomID):m_strRoomID(strRoomID){}
	virtual ~CSendLeaveChatRoom(void){}

	virtual bool SendMessage(void);
};

class CSendChat : public CSendMessageItemBase
{
public:
	std::string m_strRoomID;
	std::string m_strMessage;
public:	
	CSendChat(const std::string &strRoomID, const std::string &strMessage):m_strRoomID(strRoomID), m_strMessage(strMessage){}
	virtual ~CSendChat(void){}

	virtual bool SendMessage(void);
};

class CSendGetUserInfo : public CSendMessageItemBase
{
public:
	CCommunityUserInfo::ENUM_COMMUNITY_USERINFO_TYPE	m_nType;
	LPARAM												m_lParam;
	std::list<std::string>								m_listIDorNickName;
	std::string											m_strMessage;
public:	
	CSendGetUserInfo(CCommunityUserInfo::ENUM_COMMUNITY_USERINFO_TYPE nType, LPARAM lParam, std::list<std::string> &listIDorNickName, std::string strMessage):m_nType(nType), m_lParam(lParam), m_listIDorNickName(listIDorNickName), m_strMessage(strMessage){}
	virtual ~CSendGetUserInfo(void){}

	virtual bool SendMessage(void);
};

class CSendBuddyGroupEntryList : public CSendMessageItemBase
{
public:	
	CSendBuddyGroupEntryList(){}
	virtual ~CSendBuddyGroupEntryList(void){}

	virtual bool SendMessage(void);
};

class CSendBuddyGroupNameChange : public CSendMessageItemBase
{
public:
	std::string m_strOldName;
	std::string m_strNewName;
public:	
	CSendBuddyGroupNameChange(const std::string &strOldName, const std::string &strNewName):m_strOldName(strOldName),m_strNewName(strNewName){}
	virtual ~CSendBuddyGroupNameChange(void){}

	virtual bool SendMessage(void);
};

class CSendBuddyGroupAdd : public CSendMessageItemBase
{
protected:
	std::string m_strName;
public:	
	CSendBuddyGroupAdd(const std::string &strName):m_strName(strName){}
	virtual ~CSendBuddyGroupAdd(void){}

	virtual bool SendMessage(void);
};

class CSendBuddyGroupRemove : public CSendMessageItemBase
{
protected:
	std::string m_strName;

public:	
	CSendBuddyGroupRemove(const std::string &strName):m_strName(strName){}
	virtual ~CSendBuddyGroupRemove(void){}

	virtual bool SendMessage(void);
};

class CSendBuddyAdd : public CSendMessageItemBase
{
protected:
	std::string m_strID;
public:	
	CSendBuddyAdd(const std::string &strID):m_strID(strID){}
	virtual ~CSendBuddyAdd(void){}

	virtual bool SendMessage(void);
};

class CSendBuddyAddBuddyToGroup : public CSendMessageItemBase
{
protected:
	std::string m_strID;
	std::string m_strGroup;

public:	
	CSendBuddyAddBuddyToGroup(const std::string &strID, const std::string &strGroup):m_strID(strID),m_strGroup(strGroup){}
	virtual ~CSendBuddyAddBuddyToGroup(void){}

	virtual bool SendMessage(void);
};

class CSendBuddyRemove : public CSendMessageItemBase
{
protected:
	std::string m_strID;
public:	
	CSendBuddyRemove(const std::string &strID):m_strID(strID){}
	virtual ~CSendBuddyRemove(void){}

	virtual bool SendMessage(void);
};

class CSendBuddyRemoveBuddyFromGroup : public CSendMessageItemBase
{
protected:
	std::map<std::string, std::string>	m_mapIDAndGroup;

public:	
	CSendBuddyRemoveBuddyFromGroup(const std::map<std::string, std::string> &mapIDAndGroup):m_mapIDAndGroup(mapIDAndGroup){}
	virtual ~CSendBuddyRemoveBuddyFromGroup(void){}

	virtual bool SendMessage(void);
};

class CSendMoveBuddyToGroup : public CSendMessageItemBase
{
protected:
	std::map<std::string, std::string>	m_mapIDAndGroup;
	std::string							m_strTargetGroup;
public:	
	CSendMoveBuddyToGroup(const std::map<std::string, std::string> &mapIDAndGroup, const std::string &strTargetGroup):m_mapIDAndGroup(mapIDAndGroup),m_strTargetGroup(strTargetGroup){}
	virtual ~CSendMoveBuddyToGroup(void){}

	virtual bool SendMessage(void);
};

class CSendBuddyInfoChange : public CSendMessageItemBase
{
public:
	int		m_nGameType;
	int		m_nRoomNumber;
	INT64	m_i64Money;
public:	
	CSendBuddyInfoChange(const int &nGameType, const int &nRoomNumber, const INT64 &i64Money):m_nGameType(nGameType),m_nRoomNumber(nRoomNumber),m_i64Money(i64Money){}
	virtual ~CSendBuddyInfoChange(void){}

	virtual bool SendMessage(void);
};

class CSendBlackBuddyList : public CSendMessageItemBase
{
public:	
	CSendBlackBuddyList(){}
	virtual ~CSendBlackBuddyList(void){}

	virtual bool SendMessage(void);
};

class CSendAddBlackBuddy : public CSendMessageItemBase
{
protected:
	std::string m_strBlackBuddyID;
public:	
	CSendAddBlackBuddy(const std::string &strBlackBuddyID):m_strBlackBuddyID(strBlackBuddyID){}
	virtual ~CSendAddBlackBuddy(void){}

	virtual bool SendMessage(void);
};

class CSendRemoveBlackBuddy : public CSendMessageItemBase
{
protected:
	std::string m_strBlackBuddyID;
public:	
	CSendRemoveBlackBuddy(const std::string &strBlackBuddyID):m_strBlackBuddyID(strBlackBuddyID){}
	virtual ~CSendRemoveBlackBuddy(void){}

	virtual bool SendMessage(void);
};

class CSendReverseBuddyList : public CSendMessageItemBase
{
public:	
	CSendReverseBuddyList(){}
	virtual ~CSendReverseBuddyList(void){}

	virtual bool SendMessage(void);
};