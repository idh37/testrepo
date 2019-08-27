#include "stdafx.h"
#include "GlobalManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//#include "GlobalInfo.h"
//#include "MetpParserRet.h"
//#include "FwExceptions.h"
//#include "DataStruct/ChannelstGroupData.h"
//#include "DataStruct/ChannelData.h"
//#include "DataStruct/PlayerInfo.h"
//#include "ProtocolKey.h"
//#include "GlobalApi.h"
//#include "ItemManager.h"
//#include "MembershipManager.h"
//#include "GameOption.h"
//#include "GameBanish.h"
//#include "ExitReason.h"
//#include "Functor.h"
//#include "CfgSetting.h"
//#include "GlobalGameEvent.h"
//#include "DisplayModeChanger.h"
//#include "common/Tool/EncodeToWeb.h"
//#include "AppRunningState.h"
//#include "base/WndProperty.h"
//#include "HgObjectFactory.h"
//#include "HgObjectId.h"
//#include "GaugeManager.h"
//#include "HgResDef.h"
//
//#include "EventTypes.h"
//
//
//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif
//
//
////This value is used for game event (for example, lucky cube event)
////You should sync this value with NLocalEventType::Num_Of_Reserved_LocalEvents in GameWnd.cpp
//#define RESERVED_GM_EVENT	10
//
//SGlobalInfo& GInfo()
//{
//	static SGlobalInfo info;
//	return info;
//}
//
//SGlobalInfo::SGlobalInfo()
//{
//	for (int i = 0; i < GI_MAX; ++i)
//		m_vecInfo.push_back( new CMetpParserRet() );
//
//	for (int i = 0; i < Num_Of_Reserved_LocalEvents; ++i)
//		m_vecEvents.push_back( 0 );
//
//	m_plstPlayerInfo			= new CPlayerInfoList		;
//	m_pItemManager				= reinterpret_cast<CItemManager*>(ObjFactory().CreateObject( OBJ_ITEMMANAGER ));
//	//m_pItemManager			= new CItemManager			;
//	m_pMembershipManager		= new CMembershipManager	;
//	m_pGameOption				= new CGameOption			;
//	m_pGameBanish				= new CGameBanish			;
//	m_pExitReason				= new CExitReason			;
//	m_pGameEvent				= new CGlobalGameEvent		;
//	m_pDisplayMode				= new CDisplayModeChanger	;
//	m_pCfgSetting				= reinterpret_cast<CCfgSetting*>(ObjFactory().CreateObject( OBJ_CFGSETTING ));
//	m_pAppRunningState			= new CAppRunningState		;
//	m_pWndProperty				= new CWndProperty			;
//	m_pGameWnd					= NULL						;
//	m_pGaugeManager				= new CGaugeManager			;
//	m_pUPPData					= new CUPPData();
//
//#ifdef CS_SMILAGE
//	m_pSmilageData				= new CSmilageData			;
//#endif
//
//#ifdef NU_ALARM_SERVICE
//	m_pAlarmData				= new CAlarmData;
//#endif //NU_ALARM_SERVICE
//
//#ifdef NU_GAEA_BASE_LAYER
//	m_pGaeaData					= new CGaeaData;
//#endif //NU_GAEA_BASE_LAYER
//
//	m_pGlobalData				= new CParam();
//
//	m_nDefaultFontSize = 9;
//	CString strDefFntName = LoadStr( IDS_N_HANCHAT_DEFFACENAME );
////	_tcscpy( m_szDefaultFontName, strDefFntName );
//	HG::strcpy( m_szDefaultFontName, 100, strDefFntName );
//
//	m_hHangameMutex	= NULL;
//
//	m_eThread_Location	= TL_NONE_USE;
//	m_eThread_State		= TS_NONE;
//}
//
//SGlobalInfo::~SGlobalInfo()
//{
//	Clear();
//}
//
//CMetpParserRet& SGlobalInfo::MetpInfo( NInfoTypes nIndex )
//{
//	_ASSERTE( nIndex >= 0 && nIndex < GI_MAX );
//	if ( ! m_vecInfo[nIndex] )
//		throw Ex( "SGlobalInfo.MetpInfo() :: no metpParser");
//
//    return *m_vecInfo[nIndex];
//}
//
//int& SGlobalInfo::GetGameEvent( int nIndex )
//{
//	_ASSERTE( nIndex >= 0 && nIndex < Num_Of_Reserved_LocalEvents );
//
//	return m_vecEvents[nIndex];
//}
//
//LPSTR SGlobalInfo::GetGameId( bool bGetRealId )
//{
//	if( bGetRealId )
//	{
//		if( MetpInfo(GI_GAMEINFO).ParserPtr()->IsExistKey(keyRealGameId) )
//			return MetpInfo(GI_GAMEINFO).GetString(keyRealGameId);
//
//		else if( MetpInfo(GI_GAMESTR).ParserPtr()->IsExistKey(keyRealGameId) )
//			return MetpInfo(GI_GAMESTR).GetString(keyRealGameId);
//	}
//
//	if( MetpInfo(GI_GAMEINFO).ParserPtr()->IsExistKey(keyGameId) )
//		return MetpInfo(GI_GAMEINFO).GetString(keyGameId);
//
//	else if ( MetpInfo(GI_GAMESTR).ParserPtr()->IsExistKey(keyGameId) )
//		return MetpInfo(GI_GAMESTR).GetString(keyGameId);
//
//	return NULL;
//}
//
//LPSTR SGlobalInfo::GetPwd( void )
//{
//	if		( MetpInfo(GI_MYINFO).ParserPtr()->IsExistKey(keyPwd) )
//		return MetpInfo(GI_MYINFO).GetString(keyPwd);
//
//	else if ( MetpInfo(GI_GAMESTR).ParserPtr()->IsExistKey(keyPwd) )
//		return MetpInfo(GI_GAMESTR).GetString(keyPwd);
//
//	return NULL;
//}
//int	SGlobalInfo::SetGameString( LPCTSTR szGameStr)
//{
//	_ASSERTE( m_vecInfo[GI_GAMESTR] );
//
//	delete m_vecInfo[GI_GAMESTR];
//	m_vecInfo[GI_GAMESTR] = new CMetpParserRet( szGameStr);
//
//	if( !PropagateGameStrInfo() ) return -1;
//
//	return 0;
//}
//
//CMemberData* SGlobalInfo::MyData()
//{
//	try
//	{
//		TCHAR* szMemId = NULL; 
//		GInfo().GData().GetValue( keyUserId, szMemId );
//		// szMemId = GInfo().MetpInfo(GI_GAMESTR).GetString(keyMemberId);
//		CMemberData* pData = MemberDataList().FindMemberData( szMemId );
//		return pData;
//	}
//	catch( CFrameworkException* e )
//	{
//		e->Delete();
//	}
//	
//	return	NULL;
//}
//
//const TChannelstGroupDataList& SGlobalInfo::ChannelstGroupDataList()
//{
//	return m_lstChannelstGroupData;
//}
//
//const TChannelDataList& SGlobalInfo::ChannelDataList()
//{
//	return m_lstChannel;
//}
//
//const CRoomDataList& SGlobalInfo::RoomDataList()
//{
//	return	m_lstRoomData;
//}
//
//const CMemberDataList& SGlobalInfo::MemberDataList()
//{
//	return m_lstMemberData;
//}
//
//const CLevelDataList& SGlobalInfo::LevelDataList()
//{
//	return m_lstLevelData;
//}
//#ifdef CS_SMILAGE
//CSmilageData& SGlobalInfo::SmilageData()
//{
//	ASSERT( m_pSmilageData );
//	return *m_pSmilageData;
//}
//#endif
//
//#ifdef NU_ALARM_SERVICE
//CAlarmData& SGlobalInfo::AlarmData()
//{
//	ASSERT( m_pAlarmData );
//	return *m_pAlarmData;
//}
//#endif// NU_ALARM_SERVICE
//
//#ifdef NU_GAEA_BASE_LAYER
//CGaeaData& SGlobalInfo::GaeaData()
//{
//	ASSERT( m_pGaeaData );
//	return *m_pGaeaData;
//}
//#endif //NU_GAEA_BASE_LAYER
//
//CMsnBuddyList& SGlobalInfo::MsnBuddyList()
//{
//	return m_lstMsnBuddyData;
//}
//
//CPlayerInfoList& SGlobalInfo::PlayerInfoList()
//{
//	_ASSERTE( m_plstPlayerInfo );
//	return *m_plstPlayerInfo;
//}
//
//CDisplayModeChanger& SGlobalInfo::DisplayMode()
//{
//	_ASSERTE( m_pDisplayMode );
//	return *m_pDisplayMode;
//}
//
//CCfgSetting& SGlobalInfo::CfgSetting()
//{
//	_ASSERTE( m_pCfgSetting );
//	return *m_pCfgSetting;
//}
//
//
//BOOL SGlobalInfo::PropagateGameStrInfo()
//{
//	// Alias GI_GAMESTR metpParser
//	CMetpParser& psGameStr = *GInfo().MetpInfo( GI_GAMESTR).ParserPtr();
//
//	//
//	// set GameInfo
//	//
//	CopyValueToGInfo( GI_GAMEINFO, psGameStr, keyGameId );
//	CopyValueToGInfo( GI_GAMEINFO, psGameStr, keyRealGameId );
//	CopyValueToGInfo( GI_GAMEINFO, psGameStr, keyGameName );
//
//	MetpInfo(GI_GAMEINFO).SetString( keyGameFolder, GetGameId() );
//
//	//
//	// set channelinfo
//	//
//	CopyValueToGInfo( GI_CHANNELINFO, psGameStr, keyGroupKey );
//
//	//
//	// set myinfo
//	//
//	CopyValueToGInfo( GI_MYINFO, psGameStr, keyMemberId );
//	CopyValueToGInfo( GI_MYINFO, psGameStr, keyPwd );
//	CopyValueToGInfo( GI_MYINFO, psGameStr, keyAvatarId );
//	CopyValueToGInfo( GI_MYINFO, psGameStr, keyName );
//	CopyValueToGInfo( GI_MYINFO, psGameStr, keySex );
//	CopyValueToGInfo( GI_MYINFO, psGameStr, keyAge );
//	CopyValueToGInfo( GI_MYINFO, psGameStr, keyTotMoney );
//	CopyValueToGInfo( GI_MYINFO, psGameStr, keyRating );
//#include "base/TicketDefine.h"
//#ifdef CS_TICKET
//	CopyValueToGInfo( GI_MYINFO, psGameStr, keyTicket );
//	CopyValueToGInfo( GI_MYINFO, psGameStr, keyCookie );
//#endif
//
//	//GameRecord Rating Set
//	CopyValueToGInfo( GI_GAMERECORD, psGameStr, keyRating );
//	GInfo().MetpInfo( GI_GAMERECORD ).Set<int> ( keyNormalRating, GInfo().MetpInfo( GI_GAMERECORD ).Get<int>( keyRating ) );
//
//	// set Global Data
//	GInfo().GData().AddValue( keyMemberId, GInfo().MetpInfo(GI_MYINFO).GetString(keyMemberId) );
//	GInfo().GData().AddValue( keyUserId, GInfo().MetpInfo(GI_MYINFO).GetString(keyMemberId) );
//
//
////#ifdef NU_GPBASE
////	TCHAR szHomeDir [ MAX_PATH + 1 ];
////	if ( GetCurrentDirectory ( MAX_PATH, szHomeDir ) != 0 )
////	{
////		CString strFilePath;
////		strFilePath.Format("%s\\%s.dmp", szHomeDir, GInfo().MetpInfo( GI_GAMEINFO ).GetString( keyGameId ) );
////
////#ifndef _DEBUG
////		m_pMiniDumper = new MiniDumper( strFilePath );		
////#endif // _DEBUG
////	}
////#endif
//
//	LPSTR szFcName		= NULL;
//	LPSTR szUrlFileName = NULL;
//
//	if ( !psGameStr.IsExistKey( keyfc ) ) return FALSE;
//
//	psGameStr.GetValue( keyfc, szFcName );
//	if ( !szFcName ) return FALSE;
//
////	strcpy( m_pCfgSetting->m_szFCName, szFcName );
//	HG::strcpy( m_pCfgSetting->m_szFCName, 4, szFcName );
//
//	//// URL of refill item
//	//// ** FIXME ** Make me alive please.
//	////CEncodeToWeb().packString( std::string(GInfo->GetStringW(GI_MYINFO, keyMemberId)) );
//
//	if ( !psGameStr.IsExistKey( keyUrlCfgFile ) ) return FALSE;
//
//	psGameStr.GetValue( keyUrlCfgFile, szUrlFileName );
//	if ( !szUrlFileName ) return FALSE;
//
//	// after setting m_szFCName
//	m_pCfgSetting->CheckFCName( szUrlFileName, TRUE );
//
//	//  check available
//	if( !m_pCfgSetting->isAvailable() ) return FALSE;
//
//	//set advertise cfg
//	CEncodeToWeb enCodeToWeb;
//	std::string  szId_( GInfo().MetpInfo( GI_MYINFO ).GetString( keyMemberId ) );
//	std::string	 strMemberId = enCodeToWeb.packString( szId_ );
//
//	m_pCfgSetting->SetAdvertiseCfg( GInfo().GetGameId(), strMemberId.c_str() );
//
//	return TRUE;
//}
//
//bool SGlobalInfo::IsMyGameId( LPCTSTR pszID)
//{
//	_ASSERTE( pszID);
//	if ( ! pszID)
//		return false;
//
//	LPTSTR pszGameID = MetpInfo( GI_GAMESTR ).GetString( keyGameId );
//	if ( !  pszGameID)
//	{
//		_ASSERTE( ! "GameID NOT Found");
//		return false;
//	}
//
//	return (0 == strcmp( pszGameID, pszID ));
//}
//
//std::string	 SGlobalInfo::GetPacketData ( LPCTSTR lpStrFind, LPCTSTR lpSrc )
//{
//	std::string	 s2( lpSrc ), s;
//
//	int n = s2.find( lpStrFind );
//	if ( n != -1 ) {
//		s = s2.substr( n, s2.size() - n );
//	}
//	else {
//		return std::string("");
//	}
//
//	int spos = s.find_first_of( '=' );			/* 데이터 시작 위치 찾콅E*/
//	spos += (spos != -1) ? 1 : 0;				/* 찾아낸 다음 글자로 이동 */
//	int epos = s.find_first_of( '&', spos );	/* 마지막 위치 찾콅E*/
//
//	if ( spos == epos )							/* 데이터가 없는 경퓖E- 2*/
//		return std::string("");
//
//	if ( epos == -1 )							/* '&' 기호가 없는 경퓖E마지막 데이터로 인식 */
//		epos = s.size();
//
//	return s.substr( spos, epos-spos );			/* 찾은 스트링 픸E?*/
//}
//
//
//CItemManager& SGlobalInfo::ItemManager()
//{
//	_ASSERTE( m_pItemManager);
//	return *m_pItemManager;
//}
//
//CMembershipManager& SGlobalInfo::MembershipManager()
//{
//	_ASSERTE( m_pMembershipManager);
//	return *m_pMembershipManager;
//}
//
//LPTSTR SGlobalInfo::GetDefaultFontName()
//{
//	return	m_szDefaultFontName;
//}
//
//int SGlobalInfo::GetDefaultFontSize()
//{
//	return m_nDefaultFontSize;
//}
//
//CGameOption& SGlobalInfo::GameOption()
//{
//	_ASSERTE( m_pGameOption );
//	return (*m_pGameOption);
//}
//
//void SGlobalInfo::ReplaceGameOptionInstance( CGameOption* pNewOptionInst )
//{
//	_ASSERTE( pNewOptionInst );
//	CGameOption* pOldOpt = m_pGameOption;
//	m_pGameOption = pNewOptionInst;
//	delete pOldOpt;
//}
//
//CGameBanish& SGlobalInfo::GameBanish()
//{
//	_ASSERTE( m_pGameBanish );
//	return (*m_pGameBanish);
//}
//
//CExitReason& SGlobalInfo::ExitReason()
//{
//	_ASSERTE( m_pExitReason );
//	return *m_pExitReason;
//}
//
//CGlobalGameEvent& SGlobalInfo::GameEvent()
//{
//	_ASSERTE( m_pGameEvent );
//	return *m_pGameEvent;
//}
//
//CAppRunningState& SGlobalInfo::AppRunningState()
//{
//	_ASSERTE( m_pAppRunningState );
//	return *m_pAppRunningState;
//}
//
//CWndProperty& SGlobalInfo::WndProperty()
//{
//	_ASSERTE( m_pWndProperty );
//	return *m_pWndProperty;
//}
//
//CGaugeManager&	SGlobalInfo::GaugeManager()
//{
//	_ASSERTE( m_pGaugeManager );
//	return *m_pGaugeManager;
//}
//CUPPData&	SGlobalInfo::UPPData()
//{
//	_ASSERTE( m_pUPPData );
//	return (*m_pUPPData);
//}
//
//IParam& SGlobalInfo::GData()
//{
//	_ASSERTE( m_pGlobalData );
//	return (*m_pGlobalData);
//}
//
///**
// * Clear all data.
// * Even if you called Clear(), you cannot reuse the SGlobalInfo object.
// */
//void SGlobalInfo::Clear()
//{
//	// delete metpParser information
//	for (int i = 0; i < (int)m_vecInfo.size(); ++i)
//		delete m_vecInfo[i];
//	m_vecInfo.clear();
//
//	m_vecEvents.clear();
//
//	delete m_plstPlayerInfo	 ;  m_plstPlayerInfo	= NULL;
//	delete m_pItemManager	 ;  m_pItemManager		= NULL;
//	delete m_pMembershipManager;m_pMembershipManager= NULL;
//	delete m_pGameOption	 ;  m_pGameOption		= NULL;
//	delete m_pGameBanish	 ;  m_pGameBanish		= NULL;
//	delete m_pExitReason	 ;  m_pExitReason		= NULL;
//	delete m_pGameEvent		 ;  m_pGameEvent		= NULL;
//	delete m_pDisplayMode	 ;  m_pDisplayMode		= NULL;
//	delete m_pCfgSetting	 ;  m_pCfgSetting		= NULL;
//	delete m_pAppRunningState;  m_pAppRunningState  = NULL;
//	delete m_pWndProperty	 ;	m_pWndProperty		= NULL;
//
//	delete m_pGaugeManager   ;	m_pGaugeManager		= NULL;
//	delete m_pUPPData;			m_pUPPData = NULL;
//#ifdef CS_SMILAGE
//	delete m_pSmilageData	;	m_pSmilageData		= NULL;
//#endif
//
//#ifdef NU_ALARM_SERVICE
//	delete m_pAlarmData;		m_pAlarmData		= NULL;
//#endif// NU_ALARM_SERVICE
//
//#ifdef NU_GAEA_BASE_LAYER
//	delete m_pGaeaData;			m_pGaeaData			= NULL;
//#endif //NU_GAEA_BASE_LAYER
//
//	delete static_cast<CParam*>(m_pGlobalData);		m_pGlobalData		= NULL;
//
//	// delete group data
//	{
//		TChannelstGroupDataList::iterator it;
//		for (it = m_lstChannelstGroupData.begin(); it != m_lstChannelstGroupData.end(); ++it)
//			delete (*it);
//		m_lstChannelstGroupData.clear();
//	}
//
//	// delete channel data
//	{
//		TChannelDataList::iterator it;
//		for (it = m_lstChannel.begin(); it != m_lstChannel.end(); ++it)
//			delete (*it);
//		m_lstChannel.clear();
//	}
//
//	// delete room data
//	m_lstRoomData.Clear();
//
//	// delete member data
//	m_lstMemberData.Clear();
//}
//
//void SGlobalInfo::SetGameWndPtr( void* pGameWnd )
//{
//	m_pGameWnd = pGameWnd;
//}
//
//void* SGlobalInfo::GetGameWndPtr( )
//{
//	return m_pGameWnd;
//}
//
//
//
//void SGlobalInfo::SetThreadLocation(NThread_Location eLocation)
//{
//	m_eThread_Location = eLocation;
//
//	if(m_eThread_Location > TL_NONE_USE)
//	{
//		m_eThread_State = TS_LOADING;
//	}
//}
//
//NThread_Location SGlobalInfo::GetThreadLocation()
//{
//	return m_eThread_Location;
//}
//
//void SGlobalInfo::SetThreadState(NThread_State eState)
//{
//	m_eThread_State = eState;
//}
//
//NThread_State SGlobalInfo::GetThreadState()
//{
//	return m_eThread_State;
//}
//
//
//BOOL IsMyDataChatAgreeFlag()
//{
//	CMemberData* pData = GInfo().MyData(); 
//
//	if( pData )
//	{
//		return pData->m_SaveChatConsent;
//	}
//
//	return TRUE;
//}
//
//void SetMyDataChatAgreeFlag( BOOL flag )
//{
//	CMemberData* pData = GInfo().MyData(); 
//
//	if( pData )
//	{
//		pData->m_SaveChatConsent = flag;
//	}
//}