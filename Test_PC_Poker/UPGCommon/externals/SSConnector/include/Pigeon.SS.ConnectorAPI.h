#ifndef _PIGEON_SS_CONNECTORAPI_H_
#define _PIGEON_SS_CONNECTORAPI_H_

#if defined(_WIN32) || defined(_WIN64)
#define PIGEON_SS_CONNECTOR_API __declspec(dllexport)

#if !defined(PIGEONSSCONNECTOR_EXPORTS)
#if defined(_DEBUG)
#pragma comment(lib, "Pigeon.SS.ConnectorD.lib")
#else
#pragma comment(lib, "Pigeon.SS.Connector.lib")
#endif
#endif
#else
#define PIGEON_SS_CONNECTOR_API
#endif


#include <Camel/Base/System/ThreadPool.h>
#include "Pigeon.SS.ConnectorAPIError.h"
#include <string>
#include <list>
#if !defined(PIGEONSSCONNECTOR_EXPORTS)
#include "Pigeon.SS.Share.h"
#else
#include "../Protocol/Pigeon.SS.Protocol.h"
#endif
#include "Pigeon.SS.Allocator.h"

#define IN 
#define OUT

#define _DECLARE_NAMESPACE_PIGEON_SS_	namespace Pigeon \
										{ \
											namespace SS \
											{


#define _UNDECLARE_NAMESPACE_PIGEON_SS_	} }

using namespace std;

_DECLARE_NAMESPACE_PIGEON_SS_
namespace Environment
{
	typedef struct _Handle {} *THandle, *Handle;

	PIGEON_SS_CONNECTOR_API Handle	__stdcall	CreateHandle  ( Camel::Base::System::ThreadPool::Handle hRecvThreadPool );
	PIGEON_SS_CONNECTOR_API void	__stdcall	DestroyHandle ( Handle hEnv );

	PIGEON_SS_CONNECTOR_API bool	__stdcall	SetTimeout	( Handle hEnv, int nTimeout );
	PIGEON_SS_CONNECTOR_API int		__stdcall	GetTimeout	( Handle hEnv );

	PIGEON_SS_CONNECTOR_API bool    __stdcall	SetDomain( Handle hEnv, string& subDomain, string& serviceCode );

	PIGEON_SS_CONNECTOR_API Camel::Base::System::ThreadPool::Handle __stdcall	GetNetworkThreadPool ( Handle hEnv );
}	// namespace Environment


namespace Connector
{
	typedef struct _Handle {} *THandle, *Handle;

	PIGEON_SS_CONNECTOR_API Handle	__stdcall CreateHandle (Environment::Handle env);
	PIGEON_SS_CONNECTOR_API void	__stdcall DestroyHandle( Handle conn );

	PIGEON_SS_CONNECTOR_API bool	__stdcall Connect( Handle conn, const char* szAddr, unsigned short nPort );
	PIGEON_SS_CONNECTOR_API bool	__stdcall Reconnect( Handle conn );
	PIGEON_SS_CONNECTOR_API void	__stdcall Disconnect( Handle conn );

	typedef void (__stdcall	*OnDisconnected )	( void* /*pContext*/);
	PIGEON_SS_CONNECTOR_API bool __stdcall SetOnDisconnected(Handle conn, OnDisconnected pCallback, void* pData);

	// Login
	PIGEON_SS_CONNECTOR_API bool	__stdcall Auth( Handle conn, IN string& id, IN char authData[], IN int siteNo, OUT int& result );
	PIGEON_SS_CONNECTOR_API bool	__stdcall Auth( Handle conn, IN string& id, IN Camel::Dar2::SS::LoginInfo& loginInfo, OUT int& result );
	PIGEON_SS_CONNECTOR_API bool	__stdcall MobileAuth( Handle conn, IN string& id, IN Camel::Dar2::SS::LoginInfo& loginInfo, OUT int& result );
	PIGEON_SS_CONNECTOR_API void	__stdcall AuthRelease( Handle conn, IN string& id, OUT int& result );

	typedef void (__stdcall	*OnRecvDuplicateLoginCallback )	( const string& reason, void* pContext  );
	PIGEON_SS_CONNECTOR_API bool	__stdcall SetOnRecvDuplicateLogin( Handle conn, OnRecvDuplicateLoginCallback pCallback, void* pData, Camel::Base::System::ThreadPool::Handle hThreadPool = NULL );

	//typedef bool (__stdcall	*OnRecvLogin )	( void* pContext, USERINFO& myInfo );
	//PIGEON_SS_CONNECTOR_API bool		__stdcall SetOnRecvLogin( Handle hCon, OnRecvLogin pCallback, void* pData, Camel::Base::System::ThreadPool::Handle hThreadPool = NULL );
	// Login


	// Memo
	typedef void (__stdcall	*OnSendMemoCallback )				( void* pContext, int errorCode, bool saveToSendBox, const Camel::Dar2::SS::SendMemoResultList& resultList );
	typedef void (__stdcall	*OnReadMemoCallback )				( void* pContext, int errorCode, const Camel::Dar2::SS::Memo& memo );
	typedef void (__stdcall	*OnGetMemoListInBoxCallback )		( void* pContext, int errorCode, const Camel::Dar2::SS::MemoList& memoList );
	typedef void (__stdcall	*OnGetMemoListStorageCallback )		( void* pContext, int errorCode, const Camel::Dar2::SS::MemoList& memoList );
	typedef void (__stdcall	*OnRemoveMemoListCallback )			( void* pContext, int errorCode, const Camel::Dar2::SS::RemoveMemoNodeList& nodeList );
	typedef void (__stdcall	*OnStoreMemoListCallback )			( void* pContext, int errorCode, const Camel::Dar2::SS::StoredMemoNodeList& nodeList );
	typedef void (__stdcall	*OnSearchMemoCallback )				( void* pContext, int errorCode, const Camel::Dar2::SS::SearchCategory category, const string& keyword, const Camel::Dar2::SS::MemoNodeList& nodeList);

	typedef void (__stdcall	*OnReceiveMemoNtfCallback )			( void* pContext, const Camel::Dar2::SS::Memo& memo );

	PIGEON_SS_CONNECTOR_API bool    __stdcall GetMemoBoxInfo( Handle conn, IN string& myID, IN Camel::Dar2::SS::BoxPosition boxType, OUT Camel::Dar2::SS::MemoInfo** ppMemoInfo );
	PIGEON_SS_CONNECTOR_API bool	__stdcall SendMemo( Handle conn, IN string& senderID, IN vector<string>& recverID, IN string& content, IN bool saveToSendBox, OnSendMemoCallback callback, void *context );
	PIGEON_SS_CONNECTOR_API bool	__stdcall ReadMemo( Handle conn, IN string& id, IN string& node, OnReadMemoCallback callback, void* context );
	PIGEON_SS_CONNECTOR_API bool	__stdcall GetMemoListInBox( Handle conn, IN string& id, OnGetMemoListInBoxCallback callback, void* context); //Async
	PIGEON_SS_CONNECTOR_API bool    __stdcall GetMemoListStorage( Handle conn, IN string& id, OnGetMemoListStorageCallback callback, void* context); //Async
	PIGEON_SS_CONNECTOR_API bool    __stdcall RemoveMemoList( Handle conn, IN string& id, IN list<string>& nodeList, OnRemoveMemoListCallback callback, void* context); //Async
	PIGEON_SS_CONNECTOR_API bool    __stdcall StoreMemoList( Handle conn, IN string& id, IN list<string>& nodeList, OnStoreMemoListCallback callback, void* context); //Async
	PIGEON_SS_CONNECTOR_API bool	__stdcall SearchMemoInBox( Handle conn, IN string& id, IN Camel::Dar2::SS::SearchCategory category, string& keyword, OnSearchMemoCallback callback, void* context );
	PIGEON_SS_CONNECTOR_API bool	__stdcall SearchMemoStorage( Handle conn, IN string& id, IN Camel::Dar2::SS::SearchCategory category, string& keyword, OnSearchMemoCallback callback, void* context );

	PIGEON_SS_CONNECTOR_API bool	__stdcall SetOnRecvMemo( Handle conn, OnReceiveMemoNtfCallback pCallback, void* pData, Camel::Base::System::ThreadPool::Handle hThreadPool = NULL );
	// Memo

	// Whisper
	typedef void (__stdcall	*OnRecvWhisperNtfCallback )	( void* pContext, string& senderID, string& content );

	PIGEON_SS_CONNECTOR_API bool	__stdcall SendWhisper( Handle conn, IN string& senderID, IN string& recverID, IN string& content );
	PIGEON_SS_CONNECTOR_API bool	__stdcall SetOnRecvWhisper( Handle conn, OnRecvWhisperNtfCallback pCallback, void* pData, Camel::Base::System::ThreadPool::Handle hThreadPool = NULL );
	// Whisper

	// 1:1 Chat
	typedef void (__stdcall	*OnRecvChatMessageNtfCallback )	( void* pContext, string& senderID, string& content );

	PIGEON_SS_CONNECTOR_API bool	__stdcall SendChatMessage( Handle conn, IN string& senderID, IN string& recverID, IN string& content );
	PIGEON_SS_CONNECTOR_API bool	__stdcall SetOnRecvChatMessage( Handle conn, OnRecvChatMessageNtfCallback pCallback, void* pData, Camel::Base::System::ThreadPool::Handle hThreadPool = NULL );
	// 1:1 Chat

	// Option
	PIGEON_SS_CONNECTOR_API bool	__stdcall SetRecvWhisperOption( Handle conn, IN string& id, IN Camel::Dar2::SS::RecvState state, OUT int& result );
	PIGEON_SS_CONNECTOR_API bool	__stdcall SetRecvChatOption( Handle conn, IN string& id, IN Camel::Dar2::SS::RecvState state, OUT int& result );
	PIGEON_SS_CONNECTOR_API bool	__stdcall SetRecvMemoOption( Handle conn, IN string& id, IN Camel::Dar2::SS::RecvState state, OUT int& result );
	PIGEON_SS_CONNECTOR_API bool	__stdcall SetRecvGameInvitationOption( Handle conn, IN string& id, IN Camel::Dar2::SS::RecvState state, OUT int& result );
	//PIGEON_SS_CONNECTOR_API bool    __stdcall SetRecvOption( Handle conn, IN string& id, IN string& optionName, IN int value, OUT int& result );
	PIGEON_SS_CONNECTOR_API bool    __stdcall GetRecvOption( Handle conn, IN string& id, OUT Camel::Dar2::SS::ReceiveOptionList** ppOptionList );
	// Option

	// User Profile
	typedef void (__stdcall	*OnGetUserProfilesCallback )  ( const Camel::Dar2::SS::UserProfileList& profileList, void* context, int errorCode );

	PIGEON_SS_CONNECTOR_API bool __stdcall GetUserProfiles( Handle conn, const string& id, const Camel::Dar2::SS::UserIDList& idList, OnGetUserProfilesCallback callback, void* context );
	// User Profile
	
	// MU Chat
	// CallBack
	typedef void (__stdcall	*OnCreateChatRoomCallback)				(const string& id, const string& nickName, const string& password, const string& roomID, void* pContext, int errorCode); // Async Call
	typedef void (__stdcall	*OnSetChatRoomConfigCallback)			(const string& roomID, const Camel::Dar2::SS::MucRoomConfig& roomConfig, void* pContext, int errorCode); // Async Call
	typedef void (__stdcall	*OnChangeChatRoomConfigCallback)		(const string& roomID, const Camel::Dar2::SS::MucRoomConfig& roomConfig, void* pContext, int errorCode); // Async Call
	typedef void (__stdcall	*OnBanUserCallback)						(const string& roomID, const string& bannedID, void* context, int errorCode); // Async Call
	typedef void (__stdcall	*OnChangeRoomOwnerCallback)				(const string& roomID, const string& OwnerID, void* context, int errorCode);
	typedef void (__stdcall	*OnChangeMucAffiliationCallback)		(const string& roomID, const string& targetID, const Camel::Dar2::SS::MUCRoomAffiliation& Affiliation, const Camel::Dar2::SS::MUCRoomRole& role, void* context, int errorCode);

	typedef void (__stdcall	*OnGetRoomListCallback)					(const Camel::Dar2::SS::MucRoomFilter& filter, const Camel::Dar2::SS::MUCRoomList& roomList, void* context, int errorCode ); // async
	typedef void (__stdcall	*OnGetRoomServiceListCallback)			(const Camel::Dar2::SS::MUCRoomServiceList& roomServiceList, void* context, int errorCode ); // async
	typedef void (__stdcall	*OnGetSearchRoomListCallback)			(const Camel::Dar2::SS::MucRoomFilter& filter, const Camel::Dar2::SS::MUCRoomList& roomList, void* context, int errorCode ); // async
	typedef void (__stdcall	*OnGetRoomInfoCallback)					(const string& roomID, const Camel::Dar2::SS::MUCRoomInfo& roominfo, void* context, int errorCode ); 

	typedef void (__stdcall	*OnGetSelfRoomListCallback)				(const Camel::Dar2::SS::MUCRoomList& roomList, void* context, int errorCode );
	typedef void (__stdcall	*OnGetFavoriteRoomListCallback)			(const Camel::Dar2::SS::MUCRoomList& roomList, void* context, int errorCode );
	typedef void (__stdcall	*OnAddFavoriteRoomCallback)				(const string& roomID, void* context, int errorCode);
	typedef void (__stdcall	*OnRemoveFavoriteRoomCallback)			(const string& roomID, void* context, int errorCode);
	typedef void (__stdcall	*OnRemovePersistentRoomCallback)		(const string& roomID, void* context, int errorCode);

	typedef void (__stdcall	*OnChangeChatRoomConfigNtfCallback)		(const string& roomID, const Camel::Dar2::SS::MucRoomConfig& roomConfig, void* context );	
	typedef void (__stdcall	*OnInvitedFromChatRoomNtfCallback)		(const string& roomID, const string& senderID, const Camel::Dar2::SS::MUCRoomInfo& roomInfo, void* pContext); // Notification
	typedef void (__stdcall	*OnUserJoinChatRoomNtfCallback)			(const string& roomID, const Camel::Dar2::SS::MUCUserInfo& userInfo, void* pContext); // Notification
	typedef void (__stdcall	*OnUserLeaveChatRoomNtfCallback)		(const string& roomID, const Camel::Dar2::SS::MUCUserInfo& userInfo, void* pContext); // Notification
	typedef void (__stdcall	*OnBanUserNtfCallback)					(const string& roomID, const Camel::Dar2::SS::MUCUserInfo& bannedUser, void* context);
	typedef void (__stdcall	*OnChangeAffiliationRoomNtfCallback)    (const string& roomID, const Camel::Dar2::SS::MUCUserInfo& changeAffiliationUser, void* context);
	typedef void (__stdcall	*OnRecvChatRoomEntryUserListNtfCallback)(const string& roomID, const list<Camel::Dar2::SS::MUCUserInfo*>& entryUserList, void* pContext);  // Notification
	typedef void (__stdcall	*OnRecvMUChatNtfCallback)				(const string& roomID, const string& senderID, const string& content, const Camel::Dar2::SS::MUCContentType contentType, void* pContext); // Notification

	PIGEON_SS_CONNECTOR_API bool __stdcall CreateChatRoom(Handle conn, IN string& id, IN string& nickName, IN string& password, OnCreateChatRoomCallback callback, void* context);
	PIGEON_SS_CONNECTOR_API bool __stdcall CreateChatRoomEx(Handle conn, IN string& id, IN Camel::Dar2::SS::MUCJoinRoomInfo& joinRoomInfo, OnCreateChatRoomCallback callback, void* context);
	PIGEON_SS_CONNECTOR_API bool __stdcall SetChatRoomConfig(Handle conn, IN string& id, IN string& roomID, IN string& nickName, Camel::Dar2::SS::MucRoomConfig& roomConfig, OnSetChatRoomConfigCallback callback, void* context);
	PIGEON_SS_CONNECTOR_API bool __stdcall ChangeChatRoomConfig(Handle conn, IN string& id, IN string& roomID, IN Camel::Dar2::SS::MucRoomConfig& roomConfig, OnChangeChatRoomConfigCallback callback, void* context);
	PIGEON_SS_CONNECTOR_API bool __stdcall JoinChatRoom(Handle conn, IN string& id, IN string& nickname, IN string& roomID, IN string& password, OUT int& errorCode );
	PIGEON_SS_CONNECTOR_API bool __stdcall LeaveChatRoom(Handle conn, IN string& id, IN string& roomID);
	PIGEON_SS_CONNECTOR_API bool __stdcall InviteToChatRoom(Handle conn, IN string& id, IN string& roomID, IN string& contactID);
	PIGEON_SS_CONNECTOR_API bool __stdcall BanUser(Handle conn, IN string& id, IN string& roomID, IN string& banUserID, OnBanUserCallback callback, void* context);
	PIGEON_SS_CONNECTOR_API bool __stdcall ChangeChatRoomOwner(Handle conn, IN string& id, IN string& roomID, IN string& newOwnerID, OnChangeRoomOwnerCallback callback, void* context);
	PIGEON_SS_CONNECTOR_API bool __stdcall ChangeMUCAffiliation( Handle conn, IN const string& id, IN const string& roomID, IN const string& targetID, IN Camel::Dar2::SS::MUCRoomAffiliation affiliation, IN Camel::Dar2::SS::MUCRoomRole role, OnChangeMucAffiliationCallback callback, void* context);
	PIGEON_SS_CONNECTOR_API bool __stdcall SendMUChat(Handle conn, IN string& id, IN string& roomID, IN string& content, IN Camel::Dar2::SS::MUCContentType contentType = Camel::Dar2::SS::MCT_TEXT );

	PIGEON_SS_CONNECTOR_API bool __stdcall GetRoomServiceList( Handle conn, const string& id, OnGetRoomServiceListCallback callback, void* context);
	PIGEON_SS_CONNECTOR_API bool __stdcall GetRoomInfo( Handle conn, string& id, string& roomID, OnGetRoomInfoCallback callback, void* context);
	PIGEON_SS_CONNECTOR_API bool __stdcall GetRoomList( Handle conn, const string& id, const Camel::Dar2::SS::MucRoomFilter& filter, OnGetRoomListCallback callback, void* context);
	PIGEON_SS_CONNECTOR_API bool __stdcall GetSearchRoomList( Handle conn, const string& id, const Camel::Dar2::SS::MucRoomFilter& filter, OnGetSearchRoomListCallback callback, void* context);

	// M.Voice
	PIGEON_SS_CONNECTOR_API bool __stdcall GetSelfRoomList( Handle conn, const string& id, OnGetSelfRoomListCallback callback, void* context);
	PIGEON_SS_CONNECTOR_API bool __stdcall AddFavoriteRoom( Handle conn, const string& id, const string&roomID, const string& roomOwnerID, OnAddFavoriteRoomCallback callback, void* context);
	PIGEON_SS_CONNECTOR_API bool __stdcall GetFavoriteRoomList( Handle conn, const string& id, OnGetFavoriteRoomListCallback callback, void* context);
	PIGEON_SS_CONNECTOR_API bool __stdcall RemoveFavoriteRoom( Handle conn, const string& id, const string& roomID, const string& roomOwnerID, OnRemoveFavoriteRoomCallback callback, void* context);
	PIGEON_SS_CONNECTOR_API bool __stdcall RemovePersistentRoom( Handle conn, const string& id, const string& roomID, OnRemovePersistentRoomCallback callback, void* context);

	PIGEON_SS_CONNECTOR_API bool __stdcall SetOnChangeChatRoomConfig( Handle conn, OnChangeChatRoomConfigNtfCallback pCallback, void* pData, Camel::Base::System::ThreadPool::Handle hThreadPool = NULL);
	PIGEON_SS_CONNECTOR_API bool __stdcall SetOnInvitedFromChatRoom(Handle conn, OnInvitedFromChatRoomNtfCallback pCallback, void* pData, Camel::Base::System::ThreadPool::Handle hThreadPool = NULL);
	PIGEON_SS_CONNECTOR_API bool __stdcall SetOnUserJoinChatRoom(Handle conn, OnUserJoinChatRoomNtfCallback pCallback, void* pData, Camel::Base::System::ThreadPool::Handle hThreadPool = NULL);
	PIGEON_SS_CONNECTOR_API bool __stdcall SetOnUserLeaveChatRoom(Handle conn, OnUserLeaveChatRoomNtfCallback pCallback, void* pData, Camel::Base::System::ThreadPool::Handle hThreadPool = NULL);
	PIGEON_SS_CONNECTOR_API bool __stdcall SetOnBanUser(Handle conn, OnBanUserNtfCallback pCallback, void* pData, Camel::Base::System::ThreadPool::Handle hThreadPool = NULL);
	PIGEON_SS_CONNECTOR_API bool __stdcall SetOnChangeAffiliationRoom(Handle conn, OnChangeAffiliationRoomNtfCallback pCallback, void* pData, Camel::Base::System::ThreadPool::Handle hThreadPool = NULL);
	PIGEON_SS_CONNECTOR_API bool __stdcall SetOnRecvChatRoomEntryUserList(Handle conn, OnRecvChatRoomEntryUserListNtfCallback pCallback, void* pData, Camel::Base::System::ThreadPool::Handle hThreadPool = NULL);
	PIGEON_SS_CONNECTOR_API bool __stdcall SetOnRecvMUChat(Handle conn, OnRecvMUChatNtfCallback pCallback, void* pData, Camel::Base::System::ThreadPool::Handle hThreadPool = NULL);
	// MU Chat end

	// Presence
	typedef void (__stdcall	*OnGetPresenceInfoCallback )	( const string& serviceCode, const Camel::Dar2::SS::PresenceInfoResultList& resultList, void* pContext, int errorCode );

	PIGEON_SS_CONNECTOR_API bool	__stdcall UpdatePresence( Handle conn, IN string& id, IN Camel::Dar2::SS::PresenceInfo& presenceInfo );
	PIGEON_SS_CONNECTOR_API bool	__stdcall GetPresenceInfo( Handle conn, IN string& id, IN string& serviceCode, IN Camel::Dar2::SS::UserIDList& idList, OnGetPresenceInfoCallback callback, void* context );

	typedef void (__stdcall	*OnRecvUserPresenceNtfCallback )	( const Camel::Dar2::SS::PresenceInfo& presenceInfo, void* pContext  );
	PIGEON_SS_CONNECTOR_API bool	__stdcall SetOnRecvUserPresence( Handle conn, OnRecvUserPresenceNtfCallback pCallback, void* pData, Camel::Base::System::ThreadPool::Handle hThreadPool = NULL );

	// Presence

	// 112
	// 112
	
// roster
	// Callback
	typedef void (__stdcall	*OnGetBuddyGroupEntryListCallback)	( const string& serviceCode, const Camel::Dar2::SS::BuddyGroupEntryList& entryList, void* context, int errorCode );
	typedef void (__stdcall	*OnAddBuddyGroupCallback)			( const string& groupName, void* context, int errorCode );
	typedef void (__stdcall	*OnUpdateBuddyGroupNameCallback)	( const string& oldGroupName, const string& newGroupName, void* context, int errorCode );
	typedef void (__stdcall	*OnRemoveBuddyGroupCallback)		( const string& groupName, void* context, int errorCode );
	typedef void (__stdcall	*OnUpdateBuddyGroupListCallback)	( const Camel::Dar2::SS::BuddyGroupList& groupList, void* context, int errorCode );

	typedef void (__stdcall	*OnGetBuddyListCallback)		( const string& serviceCode,  const Camel::Dar2::SS::BuddyInfoList& buddyList, void* context, int errorCode );
	typedef void (__stdcall	*OnAddBuddyCallback)			( const Camel::Dar2::SS::BuddyInfo& buddyInfo, void* context, int errorCode );
	typedef void (__stdcall	*OnUpdateBuddyMemoCallback)		( const string& buddyID, const string& newNickname, void* context, int errorCode );
	typedef void (__stdcall	*OnRemoveBuddyCallback)			( const string& buddyID, void* context, int errorCode );
	typedef void (__stdcall	*OnMoveBuddyCallback)			( const Camel::Dar2::SS::BuddyResultList& resultList, const string& targetGroup, void* context, int errorCode );

	typedef void (__stdcall	*OnGetBlackBuddyListCallback)	( const Camel::Dar2::SS::BlackList& blackList, void* context, int errorCode );
	typedef void (__stdcall	*OnAddBlackBuddyCallback)		( const Camel::Dar2::SS::BlackBuddy& buddyInfo, void* context, int errorCode );
	typedef void (__stdcall	*OnRemoveBlackBuddyCallback)	( const string& blackBuddyID, void* context, int errorCode );

	typedef void (__stdcall	*OnGetReverseBuddyListCallback) ( const Camel::Dar2::SS::BuddyInfoList& buddyList, void* context, int errorCode );

	typedef void (__stdcall	*OnAddBuddyToGroupCallback)		( const string& buddyID, const string& groupName, void* context, int errorCode );
	typedef void (__stdcall	*OnRemoveBuddyFromGroupCallback)( const Camel::Dar2::SS::BuddyResultList& resultList, void* context, int errorCode);
	typedef void (__stdcall	*OnMoveBuddyToGroupCallback)	( const string& targetGroupName, const Camel::Dar2::SS::BuddyResultList& resultList, void* context, int errorCode);
	// Callback

	// Group
	PIGEON_SS_CONNECTOR_API bool __stdcall GetBuddyGroupEntryList( Handle conn, IN string& id, OnGetBuddyGroupEntryListCallback callback, void* context);
	PIGEON_SS_CONNECTOR_API bool __stdcall GetBuddyGroupEntryList( Handle conn, IN string& id, IN string& serviceCode, OnGetBuddyGroupEntryListCallback callback, void* context);
	PIGEON_SS_CONNECTOR_API bool __stdcall AddBuddyGroup( Handle conn, IN string& id, IN string& groupName, OnAddBuddyGroupCallback callback, void* context);
	PIGEON_SS_CONNECTOR_API bool __stdcall UpdateBuddyGroupName( Handle conn, IN string& id, IN string& oldGroupName, IN string& newGroupName, OnUpdateBuddyGroupNameCallback callback, void* context);
	PIGEON_SS_CONNECTOR_API bool __stdcall RemoveBuddyGroup( Handle conn, IN string& id, IN string& groupName, OnRemoveBuddyGroupCallback callback, void* context);
	PIGEON_SS_CONNECTOR_API bool __stdcall UpdateBuddyGroupList( Handle conn, IN string& id, IN Camel::Dar2::SS::BuddyGroupList& groupList, OnUpdateBuddyGroupListCallback callback, void* context);
	// Group
	
	// buddy
 	PIGEON_SS_CONNECTOR_API bool	__stdcall GetBuddyList( Handle conn, IN string& id, OnGetBuddyListCallback callback, void* context );
	PIGEON_SS_CONNECTOR_API bool	__stdcall GetBuddyList( Handle conn, IN string& id, IN string& serviceCode, OnGetBuddyListCallback callback, void* context );
 	PIGEON_SS_CONNECTOR_API bool	__stdcall AddBuddy( Handle conn, IN string& id, IN string& buddyID, IN string& memo, OnAddBuddyCallback callback, void* context );
 	PIGEON_SS_CONNECTOR_API bool	__stdcall UpdateBuddyMemo( Handle conn, IN string& id, IN string& buddyID, IN string& memo, OnUpdateBuddyMemoCallback callback, void* context );
 	PIGEON_SS_CONNECTOR_API bool	__stdcall RemoveBuddy( Handle conn, IN string& id, IN string& buddyID, OnRemoveBuddyCallback callback, void* context );
// 	PIGEON_SS_CONNECTOR_API bool	__stdcall MoveBuddy( Handle conn, IN string& id, IN Camel::Dar2::SS::SelectedBuddyList& moveBuddyList, IN string& targetGroup, OnMoveBuddyCallback callback, void* context );
// 
 	PIGEON_SS_CONNECTOR_API bool	__stdcall GetBlackBuddyList( Handle conn, IN string& id, OnGetBlackBuddyListCallback callback, void* context );
 	PIGEON_SS_CONNECTOR_API bool	__stdcall AddBlackBuddy( Handle conn, IN string& id, IN string& blackBuddyID, OnAddBlackBuddyCallback callback, void* context );
 	PIGEON_SS_CONNECTOR_API bool	__stdcall RemoveBlackBuddy( Handle conn, IN string& id, IN string& blackBuddyID, OnRemoveBlackBuddyCallback callback, void* context );

	PIGEON_SS_CONNECTOR_API bool	__stdcall GetReverseBuddyList( Handle conn, IN string& id, OnGetReverseBuddyListCallback callback, void* context);

	PIGEON_SS_CONNECTOR_API bool	__stdcall AddBuddyToGroup( Handle conn, IN string& id, IN string& buddyID, IN string& groupName, OnAddBuddyToGroupCallback callback, void* context);
	PIGEON_SS_CONNECTOR_API bool	__stdcall RemoveBuddyFromGroup( Handle conn, IN string& id, IN Camel::Dar2::SS::SelectedBuddyList& buddyList, OnRemoveBuddyFromGroupCallback callback, void* context);
	PIGEON_SS_CONNECTOR_API bool	__stdcall MoveBuddyToGroup( Handle conn, IN string& id, IN Camel::Dar2::SS::SelectedBuddyList& buddyList, IN string& targetGroupName, OnMoveBuddyToGroupCallback callback, void* context);

	// buddy

	// noti
 	typedef void (__stdcall	*OnUpdateBuddyNtfCallback)	( const Camel::Dar2::SS::BuddyInfo& buddyInfo, void* context );
 	PIGEON_SS_CONNECTOR_API bool	__stdcall SetOnUpdateBuddy( Handle conn, OnUpdateBuddyNtfCallback callback, void* pData, Camel::Base::System::ThreadPool::Handle hThreadPool = NULL );

// 	typedef bool (__stdcall	*OnAskToAddBuddyNtfCallback) ( const string& askID, void* context );
// 	PIGEON_SS_CONNECTOR_API bool __stdcall SetOnAskToAddBuddy( Handle conn, OnAskToAddBuddyNtfCallback callback, void* pData, Camel::Base::System::ThreadPool::Handle hThreadPool = NULL );
	// noti

//	PIGEON_SS_CONNECTOR_API bool	__stdcall AnsForAddBuddy( Handle conn, IN string& myID, IN string& askID, IN bool bAccept );


	// Privacy  
	// Privacy

	// Action List
	// Action List

	// Common
	typedef void (__stdcall	*OnGetServiceCodeInfoListCallback)		(const Camel::Dar2::SS::ServiceCodeInfoList& serviceCodeInfoList, void* context, int errorCode);
	typedef void (__stdcall	*OnGetCategoryMasterInfoListCallback)	(const Camel::Dar2::SS::CategoryMasterInfoList& serviceCodeInfoList, void* context, int errorCode);

	PIGEON_SS_CONNECTOR_API bool __stdcall GetServiceCodeInfoList( Handle conn, IN string& id, OnGetServiceCodeInfoListCallback callback, void* context);
	PIGEON_SS_CONNECTOR_API bool __stdcall GetCategoryMasterInfoList( Handle conn, IN string& id, OnGetCategoryMasterInfoListCallback callback, void* context);
	// Common

	// VCard
	typedef void (__stdcall	*OnGetVCardCallback)		(const string& vcardKey, const string& vcardName, const Camel::Dar2::SS::VCard& userConfigInfoList, void* context, int errorCode);
	typedef void (__stdcall	*OnSetVCardCallback)		(const string& vcardKey, const string& vcardName, const Camel::Dar2::SS::VCard& userConfigInfoList, void* context, int errorCode);
	typedef void (__stdcall	*OnGetVCardListCallback)	(const string& vcardKey, const Camel::Dar2::SS::VCardList& userConfigInfoList, void* context, int errorCode);

	PIGEON_SS_CONNECTOR_API bool __stdcall GetVCard( Handle conn, IN string& id, IN string& vcardKey, IN string& vcardName, OnGetVCardCallback callback, void* context);
	PIGEON_SS_CONNECTOR_API bool __stdcall SetVCard( Handle conn, IN string& id, IN string& vcardKey, IN string& vcardName, Camel::Dar2::SS::VCard& vcard, OnSetVCardCallback callback, void* context);
	PIGEON_SS_CONNECTOR_API bool __stdcall GetVCardList( Handle conn, IN string& id, IN string& vcardKey, OnGetVCardListCallback callback, void* context);
	// VCard

	// VCard2
	typedef void (__stdcall	*OnGetVCard2Callback)					( const string& targetID, const string& vcardKey, const Camel::Dar2::SS::VCard2List& vcardList, void* context, int errorCode );
	typedef void (__stdcall	*OnSetVCard2Callback)					( const Camel::Dar2::SS::VCard2& vcard, void* context, int errorCode );
	typedef void (__stdcall	*OnGetVCard2ByServiceCodeCallback)		( const string& targetID, const string& serviceCode, const string& vcardKey, const Camel::Dar2::SS::VCard2List& vcardList, void* context, int errorCode );

	PIGEON_SS_CONNECTOR_API bool __stdcall GetVCard2( Handle conn, IN const string& id, IN const string& targetID, IN const string& vcardKey, OnGetVCard2Callback callback, void* context);
	PIGEON_SS_CONNECTOR_API bool __stdcall SetVCard2( Handle conn, IN const string& id, IN const Camel::Dar2::SS::VCard2& vCard, OnSetVCard2Callback callback, void* context);
	PIGEON_SS_CONNECTOR_API bool __stdcall GetVCard2ByServiceCode( Handle conn, IN const string& id, IN const string& targetID, IN const string& serviceCode, IN const string& vcardKey, OnGetVCard2ByServiceCodeCallback callback, void* context);
	// VCard2

	// Clan, Guild and ETC...
	typedef void (__stdcall	*OnGetClanInfoCallback)			( const Camel::Dar2::SS::ClanInfo& clanInfo, void* context, int errorCode );
	typedef void (__stdcall	*OnGetClanInfoListCallback)		( const Camel::Dar2::SS::ClanInfoList& clanInfoList, void* context, int errorCode );
	typedef void (__stdcall	*OnGetClanMemberListCallback)	( const string& serviceCode, const string& clanCode, const Camel::Dar2::SS::ClanMemberInfoList& memberList, void* context, int errorCode );

	PIGEON_SS_CONNECTOR_API bool __stdcall GetClanInfo(Handle conn, IN string& id, IN string& serviceCode, OnGetClanInfoCallback callback, void* context);
	PIGEON_SS_CONNECTOR_API bool __stdcall GetClanInfoList(Handle conn, IN string& id, OnGetClanInfoListCallback callback, void* context);
	PIGEON_SS_CONNECTOR_API bool __stdcall GetClanMemberList(Handle conn, IN  string& id, IN string& serviceCode, IN string& clanCode, OnGetClanMemberListCallback callback, void* context);
	// Clan, Guild and ETC...

	// Revision
 	typedef void (__stdcall	*OnGetRevisionListCallback)			( const Camel::Dar2::SS::RevisionDataList& revisionList, void* context, int errorCode );
 	typedef void (__stdcall	*OnRequestAllExternalSyncCallback)	( const string& id, const string& cn, void* context, int errorCode );
 	typedef void (__stdcall	*OnRequestExternalSyncCallback)		( enum Camel::Dar2::SS::RevisionDataType revisionDataType, const string& masterRevision, void* context, int errorCode );
 	typedef void (__stdcall	*OnExternalSyncNtfCallback)			( enum Camel::Dar2::SS::RevisionDataType revisionDataType, const string& masterRevision, void* context, int errorCode );
 	PIGEON_SS_CONNECTOR_API bool __stdcall GetRevisionList(Handle conn, IN const string& id, OnGetRevisionListCallback callback, void* context);
 	PIGEON_SS_CONNECTOR_API bool __stdcall RequestAllExternalSync(Handle conn, IN const string& id, IN const string& cn, OnRequestAllExternalSyncCallback callback, void* context); // for web
 	PIGEON_SS_CONNECTOR_API bool __stdcall RequestExternalSync(Handle conn, IN const string& id, IN enum Camel::Dar2::SS::RevisionDataType revisionDataType, IN const string& masterRevision, OnRequestExternalSyncCallback callback, void* context);
 	PIGEON_SS_CONNECTOR_API bool __stdcall SetOnExternalSyncCallback(Handle conn, OnExternalSyncNtfCallback callback, void* pData, Camel::Base::System::ThreadPool::Handle hThreadPool = NULL );
	// Revision

	// Mobile
	typedef void (__stdcall	*OnRegistNewDeviceCallback)				( const string& udid, const Camel::Dar2::SS::MobileDeviceInfo& deviceInfo, const string& pushRegistrationID, int pushAllowFlag, int pushServiceType, __int64 uuid, void* context, int errorCode );
	typedef void (__stdcall	*OnUpdateMobileDeviceInfoCallback)		( __int64 uuid, const Camel::Dar2::SS::MobileDeviceInfo& deviceInfo, void* context, int errorCode );
	typedef void (__stdcall	*OnUpdatePushRegistrationIDCallback)	( __int64 uuid, const string& pushRegistrationID, int pushAllowFlag, int pushServiceType, void* context, int errorCode );
	typedef void (__stdcall	*OnUnRegistDeviceCallback)				( __int64 uuid, void* context, int errorCode );
	typedef void (__stdcall *OnRequestAuthTokenCallback)            ( const string& newToken, const string& currentToken, const __int64& uuid, void* context, int errorCode );

	PIGEON_SS_CONNECTOR_API bool __stdcall RegistNewDevice( Handle conn, IN const string& id, IN const string& udid, IN const Camel::Dar2::SS::MobileDeviceInfo& deviceInfo, IN const string& pushRegistrationID, IN const int& pushAllowFlag, IN const int& pushServiceType, OnRegistNewDeviceCallback callback, void* context );
	PIGEON_SS_CONNECTOR_API bool __stdcall UpdateMobileDeviceInfo( Handle conn, IN const string& id, IN const __int64 uuid, IN const Camel::Dar2::SS::MobileDeviceInfo& deviceInfo, OnUpdateMobileDeviceInfoCallback callback, void* context );
	PIGEON_SS_CONNECTOR_API bool __stdcall UpdatePushRegistrationID( Handle conn, IN const string& id, IN const __int64 uuid, IN const string& pushRegistrationID, IN const int& pushAllowFlag, OnUpdatePushRegistrationIDCallback callback, void* context );
	PIGEON_SS_CONNECTOR_API bool __stdcall UnRegistDevice( Handle conn, IN const string& id, IN const __int64 uuid, OnUnRegistDeviceCallback callback, void* context );
	PIGEON_SS_CONNECTOR_API bool __stdcall RequestAuthToken( Handle conn, IN const string& id, IN const string& currentToken, IN const __int64& uuid, OnRequestAuthTokenCallback callback, void* context );
	PIGEON_SS_CONNECTOR_API bool __stdcall DestroyOldToken( Handle conn, IN const string& id, IN const string& oldToken, IN const __int64& uuid );
	// Mobile
}

namespace System
{
	PIGEON_SS_CONNECTOR_API void __stdcall GetVersion(unsigned long *majorVersion, unsigned long *minorVersion, unsigned long *buildVersion1, unsigned long *buildVersion2);
	PIGEON_SS_CONNECTOR_API void __stdcall GetProductVersion(unsigned long *majorVersion, unsigned long *minorVersion, unsigned long *buildVersion1, unsigned long *buildVersion2);
}

_UNDECLARE_NAMESPACE_PIGEON_SS_

#endif //_PIGEON_SS_CONNECTORAPI_H_