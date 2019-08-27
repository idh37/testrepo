/**    
    @file    ChannelData.h
    @date    2011/2/11
    @author  PJH(gamking@cj.com)
    @brief   채널 정보 ( 게임마다 DataType이 다르기 때문에 일일이 세팅을 한다.
*/

#pragma once
#include "DataDefine.h"

class CMetpParser;
class CPlayerInfo;



/////////////////////////////////////////////////////////////////////////////
// CLASS CMemberData
/////////////////////////////////////////////////////////////////////////////
class NMDATA_CLASS CMemberData
{
public:
	__int64				m_nUserNo									;	///< MemberNo or CharacterNo
	TCHAR				m_szMemberId        [MEMBERIDLEN    + 1]    ;	///< MemberID or CharacterID
	char				m_szAvatarId        [AVATARIDLEN    + 1]    ;	///< MemberAvatarId

	TCHAR				m_szName            [NAMELEN        + 1]    ;	///< Real name
	char				m_szSex             [SEXLEN         + 1]    ;	///< sex
	int					m_nAge										;	///< age
	TCHAR				m_szLocation        [LOCNAMELEN     + 1]    ;	///< cityName, now living

	int					m_nMatchCnt									;	///< total playing count == m_nWinCnt+m_nDefeatCnt+m_nDrawCnt
	int					m_nWinCnt									;	///< winning count
	int					m_nDefeatCnt								;	///< defeat count
	int					m_nDrawCnt									;	///< draw count
	int					m_nDisconnCnt								;
	
	int					m_nRating									;	///< your numeric level
	int					m_nGamRanking								;
	int					m_nLevelIdx									;	///< Index of LevelString 
	TCHAR				m_szSlevel          [SLEVELLEN      + 1]    ;	///< your string level
	int					m_nNlevel									;

// 	EncodeLongLong		m_llTotMoney								;	///< total normal money
// 	EncodeLongLong		m_llGamMoney								;	///< total plusGame money

	char				m_szLastGameDate[LASTDATELEN    + 1]		;
	char				m_szLastDisconn	[LASTDISCONNLEN + 1]		;

	int					m_nExScore[20]								;
	char				m_szReservedString[ STRBUFLEN +1 ]			;	// nouse, 
	
	CMetpParser*		m_pUserData									;	/**< Game specific data */

	int					m_nDayCnt									;
	
	// PCPLUS_NONE = 0, PCPLUS_STANDARD = 1, PCPLUS_GOLD = 2, PCPLUS_PLATINUM = 3
	int					m_nPlusPcStatus								;

	int					m_nRoomId									; 
	char				m_szGuildId[ GUILDIDLEN + 1 ]				;	/**< Guild Id */
	

	int					m_nReferenceCnt								;

	BOOL				m_SaveChatConsent							;					




protected:
	CPlayerInfo*		m_pPlayerInfo								;	/**< NULL in wating room */
	CAvtr2004*			m_pAvata;

	int					m_nCouponCount;								;	///< Coupon Count

public:
	CMemberData			(    );
	virtual ~CMemberData(    );
	
	void			SetPlayerInfo	( CPlayerInfo* pPlayerInfo );
	CPlayerInfo*	GetPlayerInfo	( void );
	void			DeletePlayerInfo( void );

	bool			IsValidGuildId() const;
	
	void			ReferenceIncrement( void ){++m_nReferenceCnt;}
	void			ReferenceDecrement( void ){--m_nReferenceCnt;}

	virtual void	DrawAvata(  HDC hDC , int x, int y, int dummy = 0 );


	void			SetCouponCount(int nCount){m_nCouponCount = nCount;}
	int				GetCouponCount(void){return m_nCouponCount;}
};


/////////////////////////////////////////////////////////////////////////////
// CLASS CMemberDataList
/////////////////////////////////////////////////////////////////////////////
class COMMON_CLASS CMemberDataList
{
public:
	typedef std::map<std::string, CMemberData*>		TMemberDataList;

	CMemberDataList					(    );
	virtual ~CMemberDataList		(    );

	void			Add				( CMemberData* pNewData, bool bRemovePrev = true );
	void			Remove			( LPCTSTR szMemberId );

	int				GetCount		(    ) const;
	
	VOID			Clear			(    );
	CMemberData*	FindMemberData	( LPCTSTR sId ) const;

	TMemberDataList& GetMap			(    );

	template<int funcSel, typename T>
	CMemberData*	FindMemberData	( T val )
	{
		if ( m_list.empty() ) return NULL;

		MEMBERDATALIST_IT fit = biFindIf( m_list.begin(), m_list.end(), CMemberFinder<funcSel,T,CMemberData*>(val) );
		if ( fit != m_list.end() )
			return *fit;

		return NULL;
	}

protected:
	TMemberDataList	m_map		 ;
};