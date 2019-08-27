#pragma once

#define CM_MESSAGE_FIRST						(WM_USER + 20000)
#define CM_LOGIN_SUCCESS						(CM_MESSAGE_FIRST + 0)
#define CM_LOGIN_FAILE							(CM_MESSAGE_FIRST + 1)
#define CM_SEND_MEMO_RESULT						(CM_MESSAGE_FIRST + 2)
#define CM_GET_MEMOLIST_SUCCESS					(CM_MESSAGE_FIRST + 3)
#define CM_GET_MEMOLIST_FAILE					(CM_MESSAGE_FIRST + 4)
#define CM_GET_MEMOSTORAGE_SUCCESS				(CM_MESSAGE_FIRST + 5)
#define CM_GET_MEMOSTORAGE_FAILE				(CM_MESSAGE_FIRST + 6)
#define CM_GET_MEMOINFO_SUCCESS					(CM_MESSAGE_FIRST + 7)
#define CM_GET_MEMOINFO_FAILE					(CM_MESSAGE_FIRST + 8)
#define CM_READ_MEMO_SUCCESS					(CM_MESSAGE_FIRST + 9)
#define CM_READ_MEMO_FAILE						(CM_MESSAGE_FIRST + 10)
#define CM_SET_OPTIONINFO_SUCCESS				(CM_MESSAGE_FIRST + 11)
#define CM_SET_OPTIONINFO_FAILE					(CM_MESSAGE_FIRST + 12)
#define CM_REMOVEMEMO_SUCCESS					(CM_MESSAGE_FIRST + 13)
#define CM_REMOVEMEMO_FAILE						(CM_MESSAGE_FIRST + 14)
#define CM_STORAGEMOVEMEMO_SUCCESS				(CM_MESSAGE_FIRST + 15)
#define CM_STORAGEMOVE_FAILE					(CM_MESSAGE_FIRST + 16)
#define CM_DISCONNECT							(CM_MESSAGE_FIRST + 17)
#define CM_GET_OPTIONINFO_SUCCESS				(CM_MESSAGE_FIRST + 18)
#define CM_GET_OPTIONINFO_FAILE					(CM_MESSAGE_FIRST + 19)
#define CM_SEND_REMOVE_MEMO						(CM_MESSAGE_FIRST + 20)
#define CM_SEND_MOVE_STORAGE_MEMO				(CM_MESSAGE_FIRST + 21)
#define CM_RECEIVE_MEMO							(CM_MESSAGE_FIRST + 22)
#define CM_DUPLICATE_DISCONNECT					(CM_MESSAGE_FIRST + 23)

#define CM_SEND_CREATE_CHATTING_ROOM_SUCCESS	(CM_MESSAGE_FIRST + 50)
#define CM_SEND_CREATE_CHATTING_ROOM_FAILE		(CM_MESSAGE_FIRST + 51)
#define CM_SEND_INVITE_CHATTING_ROOM_SUCCESS	(CM_MESSAGE_FIRST + 52)
#define CM_SEND_INVITE_CHATTING_ROOM_FAILE		(CM_MESSAGE_FIRST + 53)
#define CM_SEND_JOIN_CHATTING_ROOM_SUCCESS		(CM_MESSAGE_FIRST + 54)
#define CM_SEND_JOIN_CHATTING_ROOM_FAILE		(CM_MESSAGE_FIRST + 55)
#define CM_SEND_LEAVE_CHATTING_ROOM_SUCCESS		(CM_MESSAGE_FIRST + 56)
#define CM_SEND_LEAVE_CHATTING_ROOM_FAILE		(CM_MESSAGE_FIRST + 57)
#define CM_SEND_CATTING_MESSAGE_SUCCESS			(CM_MESSAGE_FIRST + 58)
#define CM_SEND_CATTING_MESSAGE_FAILE			(CM_MESSAGE_FIRST + 59)
#define CM_RECEIVE_ENTER_CHATTING_ROOM			(CM_MESSAGE_FIRST + 60)
#define CM_RECEIVE_LEAVE_CHATTING_ROOM			(CM_MESSAGE_FIRST + 61)
#define CM_RECEIVE_CAHTTING_MESSAGE				(CM_MESSAGE_FIRST + 62)
#define CM_RECEIVE_ROOM_USER_LIST				(CM_MESSAGE_FIRST + 63)
#define CM_RECEIVE_INVITE_CATTING_ROOM			(CM_MESSAGE_FIRST + 64)
#define CM_SEND_GET_COMMUNITY_USERINFO_SUCCESS	(CM_MESSAGE_FIRST + 65)
#define CM_SEND_GET_COMMUNITY_USERINFO_FAILE	(CM_MESSAGE_FIRST + 66)
#define CM_RECEIVE_GET_COMMUNITY_USERINFO_SUCCESS (CM_MESSAGE_FIRST + 67)
#define CM_RECEIVE_GET_COMMUNITY_USERINFO_FAILE	(CM_MESSAGE_FIRST + 68)
#define CM_RECEIVE_USER_PRESENCE				(CM_MESSAGE_FIRST + 69)

#define CM_CLOSE_CHATTING_ROOM					(CM_MESSAGE_FIRST + 100)
#define CM_CLOSE_INVITE_CHATTING_ROOM			(CM_MESSAGE_FIRST + 101)
#define CM_CLOSE_MEMO_NOTIFY_POPUP				(CM_MESSAGE_FIRST + 102)

#define CM_RECEIVE_GET_BUDDY_GROUP_ENTRY_LIST_SUCCESS	(CM_MESSAGE_FIRST + 200)
#define CM_RECEIVE_GET_BUDDY_GROUP_ENTRY_LIST_FAILE		(CM_MESSAGE_FIRST + 201)
#define CM_RECEIVE_GET_BUDDY_GROUP_NAME_CHANGE_SUCCESS	(CM_MESSAGE_FIRST + 202)
#define CM_RECEIVE_GET_BUDDY_GROUP_NAME_CHANGE_FAILE	(CM_MESSAGE_FIRST + 203)
#define CM_RECEIVE_GET_BLACK_BUDDY_LIST_SUCCESS			(CM_MESSAGE_FIRST + 204)
#define CM_RECEIVE_GET_BLACK_BUDDY_LIST_FAILE			(CM_MESSAGE_FIRST + 205)
#define CM_RECEIVE_GET_REVERSE_BUDDY_LIST_SUCCESS		(CM_MESSAGE_FIRST + 206)
#define CM_RECEIVE_GET_REVERSE_BUDDY_LIST_FAILE			(CM_MESSAGE_FIRST + 207)
#define CM_RECEIVE_ADD_BLACK_BUDDY_SUCCESS				(CM_MESSAGE_FIRST + 208)
#define CM_RECEIVE_ADD_BLACK_BUDDY_FAILE				(CM_MESSAGE_FIRST + 209)
#define CM_RECEIVE_REMOVE_BLACK_BUDDY_SUCCESS			(CM_MESSAGE_FIRST + 210)
#define CM_RECEIVE_REMOVE_BLACK_BUDDY_FAILE				(CM_MESSAGE_FIRST + 211)
#define CM_RECEIVE_BUDDY_GROUP_ADD_SUCCESS				(CM_MESSAGE_FIRST + 212)
#define CM_RECEIVE_BUDDY_GROUP_ADD_FAILE				(CM_MESSAGE_FIRST + 213)
#define CM_RECEIVE_BUDDY_ADD_SUCCESS					(CM_MESSAGE_FIRST + 214)
#define CM_RECEIVE_BUDDY_ADD_FAILE						(CM_MESSAGE_FIRST + 215)
#define CM_RECEIVE_BUDDY_ADD_TO_GROUP_SUCCESS			(CM_MESSAGE_FIRST + 216)
#define CM_RECEIVE_BUDDY_ADD_TO_GROUP_FAILE				(CM_MESSAGE_FIRST + 217)
#define CM_RECEIVE_BUDDY_REMOVE_FROM_GROUP_SUCCESS		(CM_MESSAGE_FIRST + 218)
#define CM_RECEIVE_BUDDY_REMOVE_FROM_GROUP_FAILE		(CM_MESSAGE_FIRST + 219)
#define CM_RECEIVE_BUDDY_REMOVE_SUCCESS					(CM_MESSAGE_FIRST + 220)
#define CM_RECEIVE_BUDDY_REMOVE_FAILE					(CM_MESSAGE_FIRST + 221)
#define CM_RECEIVE_MOVE_BUDDY_TO_GROUP_SUCCESS			(CM_MESSAGE_FIRST + 222)
#define CM_RECEIVE_MOVE_BUDDY_TO_GROUP_FAILE			(CM_MESSAGE_FIRST + 223)
#define CM_RECEIVE_BUDDY_GROUP_REMOVE_SUCCESS			(CM_MESSAGE_FIRST + 224)
#define CM_RECEIVE_BUDDY_GROUP_REMOVE_FAILE				(CM_MESSAGE_FIRST + 225)
#define CM_RECEIVE_REVERSCE_BUDDY_NTF					(CM_MESSAGE_FIRST + 226)
#define CM_BUDDYS_CONNECT_CHANGE						(CM_MESSAGE_FIRST + 227)
#define CM_BUDDY_CONNECT								(CM_MESSAGE_FIRST + 228)
#define CM_SHOW_OPTION									(CM_MESSAGE_FIRST + 229)

#define	CM_MESSAGE_LAST									(CM_MESSAGE_FIRST + 1000)

#define _DEF_DEFAULT_BUDDY_GROUP				"기본 그룹"

#define _DEF_USER_DATA_BUDDYITEM_TYPE_CHECK		0
#define _DEF_USER_DATA_BUDDYITEM_TYPE_EXPAND	1

enum ENUM_COMMUNITY_OPTION_TYPE
{
	COM_ECOT_MEMO = 0,			//메모 옵션
	COM_ECOT_CHAT,				//채팅 옵션
	COM_ECOT_WHISPER,			//귓말 옵션
	COM_ECOT_INVITE,			//초대 옵션
	COM_ECOT_MAX
};

enum ENUM_COMMUNITY_OPTION_PERMISION_TYPE
{
	COM_ECOPT_ALLUSER = 0,		//모든 사용자에게 허용함
	COM_ECOPT_OFF,				//모든 사용자에게 허용안함
	COM_ECOPT_MYBUDDY,			//내친구에게만 허용함
	COM_ECOPT_MAX
};

class UPG_COMMON_EXPORT CChatUser
{
protected:
	std::string		m_strID;
	std::string		m_strNickName;
	int			m_pAvatarInfo[MAX_AVATARLAYER];
	std::string		m_strCN;
	int				m_nResult;
public:
	CChatUser(const std::string &strID, const std::string &strNickName, const int *pAvatarInfo);

	const std::string &GetID(void){return m_strID;}
	const std::string &GetNickName(void){return m_strNickName;}
	int *GetAvatarInfo(void){return m_pAvatarInfo;}
	void SetCN(const std::string &strCN){m_strCN = strCN;}
	const std::string &GetCN(void){return m_strCN;}
	void SetResult(const int &nResult){m_nResult = nResult;}
	const int &GetResult(void){return m_nResult;}
};

class UPG_COMMON_EXPORT CCommunityUserInfo
{
public:
	enum ENUM_COMMUNITY_USERINFO_TYPE
	{
		ECUIT_SENDMEMO = 0,
		ECUIT_INVITECHAT,
		ECUIT_MEMOBOX,
		ECUIT_ADDBLACKLIST,
		ECUIT_SHOWPROFILE,
		ECUIT_ADDBUDDY,
		ECUIT_FINDCHATADDUSER,	//대화방 초대하기 유저추가시 유저검색
		ECUIT_FINDMEMOADDUSER,	//쪽지보내기에 유저추가시 유저검색
	};

protected:
	ENUM_COMMUNITY_USERINFO_TYPE	m_nType;
	LPARAM							m_lParam;
	std::string						m_strMessage;
	std::list<CChatUser *>			m_listResultUserInfo;

public:
	CCommunityUserInfo(ENUM_COMMUNITY_USERINFO_TYPE nType, LPARAM lParam, std::string strMessage):m_nType(nType),m_lParam(lParam),m_strMessage(strMessage){}
	virtual ~CCommunityUserInfo();

	ENUM_COMMUNITY_USERINFO_TYPE	GetType(void){return m_nType;}
	LPARAM							GetParam(void){return m_lParam;}
	std::string						GetMessage(void){return m_strMessage;}

	void AddUser(CChatUser *pUser){m_listResultUserInfo.push_back(pUser);}
	const std::list<CChatUser *> &GetUserList(void){return m_listResultUserInfo;}
};

class							CSendMessageItemBase;
class							CBuddyGroup;
class							CBlackBuddy;
class							CBuddyFriend;
class							CReverseBuddy;

#ifndef _DEF_LISTBUDDY
#define _DEF_LISTBUDDY
typedef std::list<CBuddyFriend *>	LISTBUDDY;
#endif

typedef Camel::Dar2::SS::Memo	MEMO;

typedef std::list<MEMO *>											LISTMEMO;
typedef std::list<CSendMessageItemBase *>							LISTSENDQUEUE;
typedef std::list<std::string>										LISTSTRING;
typedef std::map<std::string, std::string>							MAPSTRING;
typedef std::list<pair<std::string, bool> >							LISTPAIRSTRING;
typedef std::list<std::pair<std::string, std::string> >				LISTPAIRSTRING2;
typedef std::map<std::string, LISTSTRING>							MAPLISTSTRING;
typedef std::map<std::string, LISTPAIRSTRING2 >						MAPPAIRSTRING;

typedef std::list<CBuddyGroup *>									LISTBUDDYGROUP;
typedef std::list<CBlackBuddy *>									LISTBLACKBUDDY;
typedef std::list<CReverseBuddy *>									LISTREVERSEBUDDY;

typedef	std::list<CChatUser *>										LISTCHATUSER;
typedef std::list<pair<std::string, CChatUser *> >					LISTENTERCHATROOMUSER;
typedef std::map<std::string, LISTCHATUSER>							MAPROOMUSERLIST;
typedef std::map<CBuddyFriend *, std::string>						MAPBUDDYTOGROUP;
typedef std::map<std::string, std::list<std::string> >				MAPCREATEROOM;

typedef std::map<ENUM_COMMUNITY_OPTION_TYPE, ENUM_COMMUNITY_OPTION_PERMISION_TYPE> MAPOPTIONPERMITION;

class UPG_COMMON_EXPORT CCommunityManager
{
public:
	struct CMemoCompare
	{
	public:
		enum ENUM_MEMO_COMPARE_TYPE
		{
			EMCT_READMEMO = 0,
			EMCT_SENDER,
			EMCT_MEMO,
			EMCT_DATE,
		};

		ENUM_MEMO_COMPARE_TYPE	m_nType;		//Compare시 사용할 변수
		bool					m_bIncrement;

		CMemoCompare() : m_nType(EMCT_DATE), m_bIncrement(false) {}
		bool operator() (MEMO *pItem1, MEMO *pItem2);
	};

	struct CBuddyMoveToGroup
	{
	public:
		std::string m_strTargetGroup;
		MAPSTRING	m_mapIDAndGroup;
	};

protected:
	static CCommunityManager							*m_pInst;

	Camel::Base::System::ThreadPool::Handle				m_hThreadPool;
	Pigeon::SS::Environment::Handle						m_hEnvironment;
	Pigeon::SS::Connector::Handle						m_hConnector;
	int													m_nThreadPoolCount;
	DWORD												m_dwTimeOut;

	std::string											m_strSubDomain;
	std::string											m_strServiceDomain;

	std::string											m_strMyID;
	std::string											m_strMyNickName;

	CRITICAL_SECTION									m_cLock;
	CRITICAL_SECTION									m_cConnectLock;
	CRITICAL_SECTION									m_cDisconnectLock;

	//받은쪽지
	LISTMEMO											m_listMemo;
	//쪽지함 리스트
	LISTMEMO											m_listMemoBox;
	//쪽지보관함 리스트
	LISTMEMO											m_listMemoStorage;

	LISTMEMO											m_listReceiveMemoList;
	LISTMEMO											m_listReceiveStorageMemoList;
	LISTMEMO											m_listReadMemo;
	LISTPAIRSTRING										m_listSendMemoResult;
	LISTSTRING											m_listRemoveResult;
	LISTSTRING											m_listStorageMoveResult;

	int													m_nUnReadCount;

	//쪽지 관련 메시지 발신 여부
	bool												m_bSendUnReadCount;
	bool												m_bSendMemoBox;
	bool												m_bSendStorageMemoBox;
	int													m_nSendRemoveMemo;
	int													m_nSendBoxToStorageMemo;

	//쪽지 관련 메시지 수신 여부
	bool												m_bReceiveUnReadCount;
	bool												m_bReceiveMemoBox;
	bool												m_bReceiveStorageMemoBox;

	//쪽지 관련 메시지 성공 여부
	bool												m_bSuccessUnReadCount;
	bool												m_bSuccessMemoBox;
	bool												m_bSuccessStorageMemoBox;

	//대화창 관련 리스트
	MAPCREATEROOM										m_mapCreateRoom;
	LISTPAIRSTRING										m_listJoinRoomID;
	LISTSTRING											m_listJoinFaileRoomID;
	LISTENTERCHATROOMUSER								m_listEnterChatRoomUser;
	LISTPAIRSTRING2										m_listLeaveRoomID;
	LISTPAIRSTRING2										m_listSendChatResult;
	MAPPAIRSTRING										m_mapReceiveChat;
	MAPROOMUSERLIST										m_mapRoomUser;
	MAPLISTSTRING										m_mapInviteSendFaile;
	LISTPAIRSTRING2										m_listInviteChatRoom;

	//Send Thread 관련 변수들
	HANDLE												m_hCreate;
	HANDLE												m_hDelete;
	HANDLE												m_hThread;
	DWORD												m_dwThread;
	bool												m_bContinue;
	CRITICAL_SECTION									m_cSendQueueLock;

	//Send Message Queue
	LISTSENDQUEUE										m_listSendQueue;
	bool												m_bConnect;

	MAPOPTIONPERMITION									m_mapOptionPermition;

	bool												m_bFirstTryConnect;
	bool												m_bFirst;

	DWORD												m_dwNextTryConnectTime;
	bool												m_bRetryConnect;

	CMemoCompare										m_cCompare;
	std::string											m_strGSIP;
	int													m_nGSPort;

	//친구관련
	std::map<LSY::CList *, bool>						m_mapObjectBuddyGroupEntry;
	LSY::CList											*m_pListObjectBlackBuddy;
	LSY::CList											*m_pListObjectReverseBuddy;

	LISTBUDDYGROUP										m_listBuddyTempGroup;
	LISTBUDDYGROUP										m_listBuddyGroup;

	LISTSTRING											m_listAddBuddyGroupName;
	LISTSTRING											m_listRemoveBuddyGroupName;
	LISTBUDDY											m_listAddBuddy;
	
	MAPBUDDYTOGROUP										m_mapReceiveAddBuddy;
	MAPBUDDYTOGROUP										m_mapAddBuddySendMoveGroup;
	MAPBUDDYTOGROUP										m_mapAddBuddyRecevieMoveGroup;

	MAPSTRING											m_mapBuddyNameChange;
	MAPSTRING											m_mapAddBuddyIDAndGroup;
	MAPSTRING											m_mapRemoveBuddyIDAndGroup;

	LISTBLACKBUDDY										m_listBlackBuddy;
	LISTBLACKBUDDY										m_listTempBlackBuddy;
	MAPSTRING											m_mapAddBlackBuddy;
	LISTSTRING											m_listRemoveBlackBuddy;
	std::list<CBuddyMoveToGroup *>						m_listMoveBuddyToGroup;

	LISTREVERSEBUDDY									m_listReverseBuddy;
	LISTREVERSEBUDDY									m_listTempReverseBuddy;

	std::list<Camel::Dar2::SS::PresenceInfo *>			m_listTempPresenceInfo;
	std::list<Camel::Dar2::SS::BuddyInfo *>				m_listTempBuddyNtfInfo;

	std::map<std::string, bool>							m_mapGroupExpand;
	bool												m_bShowConnectBuddy;
	bool												m_bShowIgnoreMyBuddy;
	LISTSTRING											m_listConnectBuddy;
public:
	CCommunityManager(void);
	virtual ~CCommunityManager(void);

	static CCommunityManager *Instance(void);
	static bool Create(void);
	static void Release(void);

	static DWORD WINAPI	stSendThreadProc( LPVOID pArg );
protected:
	bool OnCreate(void);
	bool OnDestroy(void);

	bool FindMemo(IN const std::string &strNode, OUT LISTMEMO::iterator &itFind, OUT bool &bMemoBox);
	void CheckMaxMemo(void);
	//Lock이 된곳에서만 사용하세요
	void ClearMemoList(LISTMEMO &listMemo);
	void ClearBuddy(void);
public:
	const Pigeon::SS::Connector::Handle &GetHandle(void){return m_hConnector;}
	const std::string &GetMyID(void){return m_strMyID;}
	const std::string &GetMyNickName(void){return m_strMyNickName;}
	const CRITICAL_SECTION &GetDisconnectLock(void){return m_cDisconnectLock;}

	const int GetMaxMemo(void);
	const int GetMaxStorageMemo(void);
	const int GetMaxCreateChattingRoom(void);

	const int GetMemoCount(void);
	const int GetStorageMemoCount(void);
	const int GetTotalStorageMemoCount(void);

	//쪽지 관련 패킷 발신 여부
	const bool	IsSendUnReadCount(void);
	const bool	IsSendMemoBox(void);
	const bool	IsSendStorageMemoBox(void);
	const int	GetSendRemoveMemoCount(void);
	const int	GetSendMoveMemoBoxToStorageMemoCount(void);

	//쪽지 관련 패킷 수신 여부
	const bool IsReceiveUnReadCount(void);
	const bool IsReceiveMemoBox(void);
	const bool IsReceiveStorageMemoBox(void);

	//쪽지 관련 패킷 성공 여부
	const bool IsSuccessUnReadCount(void);
	const bool IsSuccessMemoBox(void);
	const bool IsSuccessStorageMemoBox(void);

	const bool SetOption(IN const ENUM_COMMUNITY_OPTION_TYPE &nOptionType, IN const ENUM_COMMUNITY_OPTION_PERMISION_TYPE &nPermition, IN const bool &bSendOptionMessage = true);
	const ENUM_COMMUNITY_OPTION_PERMISION_TYPE &GetOption(IN const ENUM_COMMUNITY_OPTION_TYPE &nOptionType);

	static bool IsAdminMemo(MEMO *pMemo);

	void SetConnect(const bool &bConnect, const bool &bFirstTryConnect, const bool &bRetryConnect);
	const bool &IsConnect(void);
	const bool &IsFirstTryConnect(void);
	const bool IsRetryConnect(void);

	void GetMomoBoxReceiveInfo(OUT bool &bSendMemoBox, OUT bool &bReceiveMemobox, OUT bool &bSuccessMemoBox, OUT bool &bSendStorageMemoBox, OUT bool &bReceiveStorageMemobox, OUT bool &bSuccessStorageMemobox, OUT int &nSendRemoveCount, OUT int &nSendMoveStorage);

	int GetPage(IN bool bMemoBox, IN LSY::CList *pList, IN CMemoCompare::ENUM_MEMO_COMPARE_TYPE nCompaireType, IN bool bIncrement, IN OUT int &nPage, OUT int &nMemoCount, OUT int &nStorageMemoCount);
	int GetSearchPage(IN bool bMemoBox, IN LSY::CList *pList, IN CMemoCompare::ENUM_MEMO_COMPARE_TYPE nCompaireType, IN bool bIncrement, IN bool bNickName, std::string strFind, IN OUT int &nPage, OUT int &nMemoCount, OUT int &nStorageMemoCount);

	//친구관련 함수들
	void AddListObjectBuddyGroupEntry(IN LSY::CList *pList, IN bool bIsUseOption);
	void RemoveListObjectBuddyGroupEntry(IN LSY::CList *pList);

	void SetBuddyListObjectFromCommunityData(IN LSY::CList *pList, IN bool bIsUseOption);

	void SetBuddyListDeleteItem(LPARAM lParam);
	void SetBuddyListUpdateItem(LPARAM lParam);
	void SetBuddyListSort(void);
	void SetBuddyListAddItem(LPARAM lParam);
	void SetBuddyLIstUserPresenceChange(CBuddyFriend *pBuddy);
	void SetBuddyLIstAddBuddy(CBuddyFriend *pBuddy);
	LSY::CList *GetBuddyListUseOption(void);

	void SetListObjectBlackBuddy(IN LSY::CList *pList){m_pListObjectBlackBuddy = pList;}
	void SetListObjectResersseBuddy(IN LSY::CList *pList){m_pListObjectReverseBuddy = pList;}

	void SetBuddyGroupListEntryToListObject(void);
	void SetBlackBuddyListToListObject(void);
	void SetReverseBuddyListToListObject(void);
	void SetReverseBuddyNtfToListObject(void);

	void SetConnectBuddyListToTextList(LSY::CTextList *pList);

	void SetBuddyGroupNameChange(void);
	void SetAddBuddyGroupToListObject(void);
	void SetRemoveBuddyGroupFromListObject(void);
	void SetAddBuddyToListObject(void);
	void SetAddBlackBuddyToListObject(void);
	void SetRemoveBlackBuddyFromListObject(void);
	void SetMoveBuddyToGroup(void);
	void SetUserPresence(void);

	const bool &IsShowConnectBuddy(void){return m_bShowConnectBuddy;}
	void SetShowConnectBuddy(const bool &bShowConnectBuddy);
	const bool &IsShowIgnoreMyBuddy(void){return m_bShowIgnoreMyBuddy;}
	void SetShowIgnoreMyBuddy(const bool &bShowIgnoreMyBuddy);

	CBuddyFriend *IsExistBuddy(const std::string strNickName);
	CBuddyFriend *IsExistBuddyByID(const std::string strID);
	CBlackBuddy *IsExistBlackBuddy(const std::string strNickName);
	CBlackBuddy *IsExistBlackBuddyByID(const std::string strID);
	CReverseBuddy *IsExistReverseBuddy(const std::string strNickName);
	CBuddyGroup *IsExistBuddyGroup(const std::string strName);

	//그룹안에 접속해 있는 유저의 아이디와 닉네임 맵을 가져온다.
	void GetGroupInConnectIDAndNickName(IN const std::string &strGroupName, OUT MAPSTRING &mapIDAndNickName);
	void GetGroupInIDAndNickName(IN const std::string &strGroupName, OUT MAPSTRING &mapIDAndNickName);

	int GetConnectBuddyCount(void);

	void SetComboGroupList(LSY::CCombo *pCombo, const std::string &strDefault);
//메시지 수신 관련 함수들
public:
	void OnReceiveUserPresence(IN const Camel::Dar2::SS::PresenceInfo &presenceInfo);

//쪽지 관련
	void OnReceiveMemo(IN const MEMO &memo);
	void OnReceiveReadMemo(IN const MEMO &memo);
	void OnReceiveSendMemoResult(IN const std::list<Camel::Dar2::SS::SendMemoResult* > &listResult);
	void OnReceiveMemoList(IN const LISTMEMO &listMemo, IN const bool &bSuccess);
	void OnReceiveMemoStorageList(IN const LISTMEMO &listMemo, IN const bool &bSuccess);
	void OnReceiveMemoInfo(IN const short &nUnReadCount, IN const bool &bSuccess);
	void OnReceiveRemoveMemo(IN const std::list<std::string> &listRemoveNode);
	void OnReceiveStorageMoveMemo(IN const std::list<std::string> &listMoveNode);
//시스템관련
	void OnReceiveDisconnect(void);
	void OnReceiveDuplicateDisconnect(void);
	void OnReceiveGetOptionInfo(IN const ENUM_COMMUNITY_OPTION_TYPE &nOptionType, IN const ENUM_COMMUNITY_OPTION_PERMISION_TYPE &nPermition, IN const bool &bSuccess);
	void OnReceiveSetOptionInfo(IN const ENUM_COMMUNITY_OPTION_TYPE &nOptionType, IN const ENUM_COMMUNITY_OPTION_PERMISION_TYPE &nPermition, IN const bool &bSuccess);
//채팅 관련
	void OnReceiveCreateChattingRoom(IN const std::string &strRoomID, IN std::list<std::string > *pListInviteID, IN const bool &bSuccess);
	void OnReceiveInviteChattingRoom(IN const std::string &strRoomID, IN const std::string &strSender);
	void OnReceiveJoinChattingRoom(IN const std::string &strRoomID, IN const bool &bMaster, IN const bool &bSuccess);
	void OnReceiveEnterChattingRoom(IN const std::string &strRoomID, IN const std::string strID, IN const std::string &strNickName, IN const int *pAvatarInfo);
	void OnReceiveLeaveChattingRoom(IN const std::string &strRoomID, IN const std::string strSenderID);
	void OnReceiveSendChatFaileResult(IN const std::string &strRoomID, IN const std::string strMessage);
	void OnReceiveChatMessage(IN const std::string &strRoomID, IN const std::string strSender, IN const std::string strMessage);
	void OnReceiveChattingUserList(IN const std::string &strRoomID, IN const std::list<CChatUser *> &listUser);
	void OnReceiveSendInviteFaile(IN const std::string &strRoomID, IN const std::string &strID);
//친구 관련
	void OnReceiveBuddyGroupEntryList(IN const Camel::Dar2::SS::BuddyGroupEntryList& entryList, IN const bool &bSuccess);
	void OnReceiveBuddyGroupNameChange(IN const std::string strOldName, IN const std::string strNewName, IN const bool &bSuccess);
	void OnReceiveBuddyGroupAdd(IN const std::string strName, IN const bool &bSuccess);
	void OnReceiveBuddyGroupRemove(IN const std::string strName, IN const bool &bSuccess);
	void OnReceiveAddBuddy(IN const Camel::Dar2::SS::BuddyInfo &buddyInfo, IN const bool &bSuccess);
	void OnReceiveAddBuddyToGroup(IN const std::string &strID, IN const std::string &strGroup, IN const bool &bSuccess);
	void OnReceiveRemoveBuddyFromGroup(IN const Camel::Dar2::SS::BuddyResultList &resultList, IN const bool &bSuccess);
	void OnReceiveRemoveBuddy(IN const std::string &strID, IN const bool &bSuccess);
	void OnReceiveMoveBuddyToGroup(IN const std::string &strTargetGroup, IN const Camel::Dar2::SS::BuddyResultList &resultList, IN const bool &bSuccess);

	void OnReceiveBlackBuddyList(IN const Camel::Dar2::SS::BlackList& entryList, IN const bool &bSuccess);
	void OnReceiveAddBlackBuddy(IN const std::string &strBlackBuddyID, IN const std::string &strBlackBuddyNickName, IN const bool &bSuccess);
	void OnReceiveRemoveBlackBuddy(IN const std::string &strBlackBuddyID, IN const bool &bSuccess);

	void OnReceiveReverseBuddyList(IN const Camel::Dar2::SS::BuddyInfoList& entryList, IN const bool &bSuccess);
	void OnReceiveUpdateBuddyNtf(IN const Camel::Dar2::SS::BuddyInfo& buddyInfo);


//메시지 처리 함수들
	bool MoveReciveMemoListToMomoList(OUT int *pMemoCount);
	bool MoveReciveStorageMemoListToStorageMomoList(OUT int *pStorageMemoCount);
	bool ResultSendMemo(OUT LISTPAIRSTRING &listResult);
	bool GetReadMemo(OUT MEMO &memo, LSY::CList *pList);
	bool ResultRemoveMemo(IN const bool &bSuccess, OUT bool &bRemoveMemoBox, OUT bool &bRemoveStorageMemoBox, OUT int &nSendCount);
	bool ResultMoveBoxToStrageMemo(IN const bool &bSuccess, OUT int &nSendCount);
	bool GetCreateChattingRoom(OUT MAPCREATEROOM &mapCreate);
	bool GetJoinRoom(OUT LISTPAIRSTRING &listJoinRoomID);
	bool GetJoinFaileRoom(OUT LISTSTRING &listJoinFaileRoom);
	bool GetEnterRoom(OUT LISTENTERCHATROOMUSER &listEnterRoomUser);
	bool GetLeaveRoom(OUT LISTPAIRSTRING2 &listLeaveRoom);
	bool GetSendFaileChat(OUT LISTPAIRSTRING2 &listSendFaileChat);
	bool GetReceiveChat(OUT MAPPAIRSTRING &mapReceiveChat);
	bool GetRoomUserList(OUT MAPROOMUSERLIST &mapRoomUser);
	bool GetInviteFaileUser(OUT MAPLISTSTRING &mapInviteUser);
	bool GetInviteChattingRoom(OUT LISTPAIRSTRING2 &listInviteChatRoom);
	void ReleaseConnectHandle(void);

	void ClearEnterChatRoomUserList(void);
	void ClearRoomUserList(void);

//메지지 가져오기 관련 함수들
public:
	void GetReceiveMemoList(LISTMEMO &listMemo);
	int GetUnReadMemoCount(void);

	void GetConnectBuddyList(LISTSTRING &listBuddyNickName);

	bool ConnectLogin(const int &nThreadPoolCount, const DWORD &dwTimeOut);
protected:
	bool GetServerList(IN Pigeon::GS::Connector::Handle hHandle, OUT std::string &strIP, OUT int &nPort, OUT CString &strError);

//메시지 보내기 관련 함수들
public:
	bool AddSendQueue(CSendMessageItemBase *pMessage, bool bCheckConnect = true);
	CSendMessageItemBase *GetSendQueue(void);

public:
	bool Disconnect();
	bool Login(std::string strMyID, std::string strMyNickName, const int &nThreadPoolCount = 2, const DWORD &dwTimeOut = 3000);
	bool SendMemo(IN const list<std::string> &listID, IN const std::string &strMemo);
	bool SendGetMemoList(void);
	bool SendGetMemoStorageList(void);
	bool SendGetMemoInfo(void);
	bool SendRemoveMemo(IN const list<std::string> &listMemoNode);
	bool SendStorageMoveMemo(IN const list<std::string> &listMemoNode);
	bool SendGetOption(void);
	bool SendSetOption(const ENUM_COMMUNITY_OPTION_TYPE &nOptionType, const ENUM_COMMUNITY_OPTION_PERMISION_TYPE &nPermition);
	bool SendReadMemo(MEMO &memo);
	bool SendGetUserInfo(CCommunityUserInfo::ENUM_COMMUNITY_USERINFO_TYPE nType, LPARAM lParam, std::list<std::string> &listIDorNickName, std::string strMesage);

	bool SendCreateRoom(const std::list<std::string> &listInviteID, const std::string &strPassword);
	bool SendInviteChatRoom(const std::string &strRoomID, const std::string &strRecvID);
	bool SendJoinChatRoom(const std::string &strRoomID, const bool &bMaster);
	bool SendLeaveChatRoom(const std::string &strRoomID);
	bool SendChat(const std::string &strRoomID, const std::string &strMessage);

	bool SendGetBuddyGroupEntryList(void);
	bool SendBuddyGroupNameChanage(CBuddyGroup *pBuddyGroup);
	bool SendBuddyGroupAdd(const std::string &strName);
	bool SendBuddyGroupRemove(const std::string &strName);
	bool SendBuddyInfoChange(const IDX_GAME &nGame, const int &nRoomNumber, const INT64 &i64Money);

	bool SendGetBlackBuddyList(void);
	bool SendAddBlackBuddy(const std::string &strBlackBuddyID);
	bool SendAddBuddy(const std::string &strBuddyID, const std::string &strGroupName);
	bool SendRemoveBuddy(void);
	bool SendAddBuddyToGroup(void);
	bool SendRemoveBuddyFromGroup(const std::map<std::string, std::string> &mapIDAndGroup);
	bool SendMoveBuddyToGroup(const std::map<std::string, std::string> &mapIDAndGroup, const std::string &strTargetGroup);
	bool SendRemoveBlackBuddyList(const std::string &strBlackBuddyID);

	bool SendGetReverseBuddyList(void);


//메시지 비우기 관련 함수들
	void ClearSendMessageQueue(void);

	void LoadBuddyINI(void);
	void SaveBuddyINI(void);
};

UPG_COMMON_EXPORT extern bool IsExistNickName(LISTSTRING &listNickName, std::string &strNickName, LPCTSTR lpMyNickName);