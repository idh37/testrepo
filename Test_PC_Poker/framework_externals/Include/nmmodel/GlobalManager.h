#pragma once

#include "ModelDefine.h"

class NMMODEL_CLASS CGlobalManager
{

public:
	CGlobalManager();
	~CGlobalManager();

public:


private:

protected:

};


NMMODEL_API CGlobalManager& GM();



//
//#include <list>
//#include <vector>
//#include <string>
//
//#include "DataStruct/MemberData.h"
//#include "DataStruct/RoomDataList.h"
//#include "DataStruct/LevelData.h"
//#include "DataStruct/MsnBuddyList.h"
//#include "Common/DataStruct/UPPData.h"
//
//#include "base/SmilageDefine.h"
//#ifdef	CS_SMILAGE
//#include "DataStruct/SmilageData.h"
//#endif
//
//#include "base/AlarmDialogDefine.h"
//#ifdef NU_ALARM_SERVICE
//#include "DataStruct/AlarmData.h"
//#endif //NU_ALARM_SERVICE
//
//#include "base/GaeaDefine.h"
//#ifdef NU_GAEA_BASE_LAYER
//#include "DataStruct/GaeaData.h"
//#endif //NU_GAEA_BASE_LAYER
//
//class	CMetpParserRet		;
//class	CChannelstGroupData	;
//class	CChannelData		;
//class	CPlayerInfoList		;
//class	CItemManager		;
//class	CMembershipManager	;
//class	CGameOption			;
//class	CGameBanish			;
//class	CExitReason			;
//class	CGlobalGameEvent	;
//class	CGameWnd			;
//class	CDisplayModeChanger	;
//class	CCfgSetting			;
//class   CAppRunningState	;
//class	CWndProperty		;
//class	CGaugeManager		;
//
//#ifdef	CS_SMILAGE
//class	CSmilageData		;
//#endif
//
//
//enum NInfoTypes
//{
//	GI_GAMESTR		,	///< game string passed when lauched.
//	GI_GAMEINFO		,	///< game info
//	GI_MYINFO		,	///< my info
//	GI_GAMERECORD	,	///< backup of my game record	
//	GI_CHANNELINFO	,	///< Channel (currently connected) info
//	GI_GMRMINFO		,	///< GameRoom (currently connected) Info
//	GI_INVITE		,	///< Temporary information about invite scenario.
//	GI_ROOMRECORD	,
//	GI_MAX			,
//};
//
//enum NThread_Location
//{
//	TL_NONE_USE	= 0,	///< Disabled Multi-Thread
//	TL_BASE_USE	= 1,	///< Using Multi-Thread Base Code
//	TL_GAME_USE	= 2,    ///< Using Multi-Thread Game Code
//};
//
//enum NThread_State
//{
//	TS_NONE		= 0,    ///< Multi-Thread Loading
//	TS_LOADING	= 1,    ///< Multi-Thread Loading
//	TS_FINISH   = 2,	///< Multi_Thread Finish
//};
//
//typedef std::vector<CChannelstGroupData*>	TChannelstGroupDataList;
//typedef std::vector<CChannelData*>			TChannelDataList;
//
//// TODO : Adding Is
//class COMMON_CLASS SGlobalInfo
//{
//public:
//	SGlobalInfo();
//	~SGlobalInfo();
//
//	CMetpParserRet&			MetpInfo				( NInfoTypes nIndex );
//	int						SetGameString			( LPCTSTR szGameStr );
//	//TODO : 적당한 위치 찾아서 옮기자.
//	std::string				GetPacketData			( LPCTSTR lpStrFind, LPCTSTR lpSrc );			
//
//	// Helper functions
//public:
//	bool					IsMyGameId( LPCTSTR pszID);
//
//
//public:
//	void								Clear();
//	CMemberData*						MyData();
//	const TChannelstGroupDataList&		ChannelstGroupDataList();
//	const TChannelDataList&				ChannelDataList();
//	const CRoomDataList&				RoomDataList();
//	const CMemberDataList&				MemberDataList();
//	const CLevelDataList&				LevelDataList();
//	CMsnBuddyList&						MsnBuddyList();
//	CPlayerInfoList&					PlayerInfoList();
//	CItemManager&						ItemManager();
//	CMembershipManager&					MembershipManager();
//
//	LPTSTR								GetDefaultFontName();
//	int									GetDefaultFontSize();
//	CGameOption&						GameOption();
//	void								ReplaceGameOptionInstance( CGameOption* pNewOptionInst );
//	CGameBanish&						GameBanish();
//	CExitReason&						ExitReason();
//	CGlobalGameEvent&					GameEvent();
//	CDisplayModeChanger&				DisplayMode();
//	CCfgSetting&						CfgSetting();
//	CAppRunningState&					AppRunningState();
//	CWndProperty&						WndProperty();
//	CGaugeManager&						GaugeManager();
//	CUPPData&							UPPData();
//	void								SetGameWndPtr( void* pGameWnd );
//	void*								GetGameWndPtr( );
//	int&								GetGameEvent(int nIndex );
//	LPSTR								GetGameId( bool bGetRealId = false );
//	LPSTR								GetPwd( void );
//#ifdef	CS_SMILAGE
//	CSmilageData&						SmilageData();
//#endif
//
//#ifdef NU_ALARM_SERVICE
//	CAlarmData&							AlarmData();
//#endif //NU_ALARM_SERVICE
//
//#ifdef NU_GAEA_BASE_LAYER
//	CGaeaData&							GaeaData();
//#endif //NU_GAEA_BASE_LAYER
//	IParam&								GData();
//	
//
//	void								SetThreadLocation(NThread_Location eLocation);
//	NThread_Location					GetThreadLocation();
//
//	void								SetThreadState(NThread_State eState);
//	NThread_State						GetThreadState();
//
//public:
//	TChannelstGroupDataList				m_lstChannelstGroupData	;
//	TChannelDataList					m_lstChannel			;
//	CRoomDataList						m_lstRoomData			;
//	CMemberDataList						m_lstMemberData			;
//	CLevelDataList						m_lstLevelData			;
//	CMsnBuddyList						m_lstMsnBuddyData		;
//	CPlayerInfoList*					m_plstPlayerInfo		;
//	CItemManager*						m_pItemManager			;	
//	CMembershipManager*					m_pMembershipManager	;
//	CGameOption*						m_pGameOption			;
//	CGameBanish*						m_pGameBanish			;
//	CExitReason*						m_pExitReason			;
//	CGlobalGameEvent*					m_pGameEvent			;
//	void*								m_pGameWnd				; /**< CGameWnd* in GDI, dxBase::CGameWnd* in DX */
//	CDisplayModeChanger*				m_pDisplayMode			;
//	CCfgSetting*						m_pCfgSetting			;
//	CAppRunningState*					m_pAppRunningState		;
//	CWndProperty*						m_pWndProperty			;
//	CGaugeManager*						m_pGaugeManager			;
//	CUPPData*							m_pUPPData;
//#ifdef CS_SMILAGE
//	CSmilageData*						m_pSmilageData			;
//#endif
//
//
//#ifdef NU_ALARM_SERVICE
//	CAlarmData*							m_pAlarmData			;
//#endif //NU_ALARM_SERVICE
//
//#ifdef NU_GAEA_BASE_LAYER
//	CGaeaData*							m_pGaeaData			;
//#endif //NU_GAEA_BASE_LAYER
//	
//	IParam*								m_pGlobalData;
//
////	MiniDumper*							m_pMiniDumper			;
//
//	// -----------------------------------------------------
//	// CLASS MEMBERS
//	// -----------------------------------------------------
//
//	/**
//	 * For more information, See comment at end of file.
//	 * This vector contains,
//	 *		- [0] : gameString
//	 *		- [1] : gameInfo
//	 *		- [2] : MyInfo
//	 *		- [3] : GameRecord
//	 *		- [4] : ChannelInfo
//	 *		- [5] : GameRoomInfo
//	 */
//	std::vector<CMetpParserRet*> m_vecInfo	;
//	std::vector<int>			 m_vecEvents;
//
//	TCHAR	m_szDefaultFontName[ 100 ]		;
//	int		m_nDefaultFontSize				;
//
//	HANDLE	m_hHangameMutex;
//
//private:
//	NThread_Location				m_eThread_Location;
//	NThread_State					m_eThread_State;
//
//protected:
//	BOOL PropagateGameStrInfo();
//};
//
//COMMON_CLASS SGlobalInfo& GInfo();
//
//
//COMMON_CLASS BOOL IsMyDataChatAgreeFlag();
//COMMON_CLASS void SetMyDataChatAgreeFlag( BOOL );
