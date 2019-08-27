#pragma once

#include "DataLenDef.h"


///< 방입장하는 종류
// enum NEnterRoomType 
// {
// 	CREATE_ROOM = 0, 
// 	JOIN_ROOM, 
// 	VIEW_ROOM, 
// 	CREATE_ROOM_FOR_INVITE, 
// 	AUTO_ENTERROOM_CREATE, 
// 	AUTO_ENTERROOM_JOIN, 
// 	AUTO_CREATE_ROOM,
// 	AUTO_ENTERROOM_BYSERVER,
// 	ERT_MAX
// };


class COMMON_CLASS CRoomData
{
public:
	enum 
	{
		//ROOM_CURRENT	= -1,	// not used.. maybe no need...
		ROOM_EMPTY		= 0	,	// Empty
		ROOM_JOIN			,
		ROOM_JOINREADY		,
		ROOM_READY			,
		ROOM_GAMEJOIN		,	// Ready
		ROOM_GAMEVIEW		,
		ROOM_GAMEFULL		,	// Full
		ROOM_WAITING		,
		ROOM_DISABLE		,
		ROOM_JOINVIEW			// ROOM_GAMEJOIN | ROOM_GAMEVIEW
	};

public:
	typedef std::map<int,std::string>	MEMBER_MAP		;
	typedef MEMBER_MAP::iterator		MEMBER_MAP_IT	;
	typedef MEMBER_MAP::const_iterator	MEMBER_MAP_CIT	;
	typedef std::pair<int,std::string>	MEMBER_PAIR		;

public:

	// room attribute

	int                 m_nRoomNo                       ;
	int                 m_nRoomState                    ;
	int                 m_nRoomLimitCnt                 ;
	bool                m_bPrivateRoom                  ;

	TCHAR               m_szRoomTitle  [ROOMTITLELEN+1 ];
	TCHAR               m_szRoomOption [ROOMOPTIONLEN+1];
	TCHAR               m_szRoomHost   [MEMBERIDLEN+1  ];
	TCHAR               m_szRoomCreator [MEMBERIDLEN+1 ];
	TCHAR				m_szRoomType   [ROOMTYPELEN+1  ];	

	MEMBER_MAP			m_mapMemberId					;
	MEMBER_MAP			m_mapViewerId					;

//	bool				m_bSuperRoom					;
	int					m_nRoomPlaying					;
	bool				m_bRandomRoomEnter				;

public:

	CRoomData				(						)		;
	CRoomData				( int nRoomNo			)		;
	virtual ~CRoomData		(						)		;

	bool	AddMember		( int nPos, LPCTSTR szMemberId );
	bool	AddViewer		( int nPos, LPCTSTR szViewerId );

	void	RemoveMember	( int nPos				)		;
	bool	RemoveMember	( LPCTSTR szId			)		;
	void	RemoveViewer	( int nPos				)		;
	bool	RemoveViewer	( LPCTSTR szId			)		;
	void	RemoveAllMember	(						)		;
	void	RemoveAllViewer	(						)		;

	bool	IsMember		( LPCTSTR szMemberId	) const	;
	bool	IsViewer		( LPCTSTR szViewerId	) const	;

	int		GetRoomNo		(						) const ;
	LPCTSTR	GetMemberId		( int nPos				) const ;
	LPCTSTR	GetViewerId		( int nPos				) const ;
	int		GetMemberPos	( LPCTSTR szMemberId	) const ;
	int		GetViewerPos	( LPCTSTR szMemberId	) const ;
	int		GetMemberCount	(						) const ;
	int		GetViewerCount	(						) const ;

	bool	GetSuperYN		(						) const ;
	int		GetState		(						) const ;
	int		GetPlayingYN	(						) const ;

	void	SetState		( int nState			)		;
	void	SetCreator		( LPCTSTR szRoomCreator )		;
	void	SetHost			( LPCTSTR szRoomHost	)		;
	void	SetTitle		( LPCTSTR szRoomTitle	)		;
	void	SetLimitCnt		( int nLimitCnt			)		;
	void	SetPrivateYN	( LPCSTR szPrivateYN	)		;
	void	SetOption		( LPCTSTR szRoomOption	)		;
	void	SetRoomType		( LPCTSTR szRoomType	)		;
//	void	SetSuperYN		( bool bSuperRoom		)		;
	void	SetPlayingYN	( int nRoomPlaying		)		;

};