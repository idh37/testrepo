/**    
    @file    ChannelData.h
    @date    2011/2/11
    @author  PJH(gamking@cj.com)
    @brief   채널 정보 ( 게임마다 DataType이 다르기 때문에 일일이 세팅을 한다.
*/

#pragma once
#include "DataDefine.h"

class NMDATA_CLASS CChannelData
{
public:
	TCHAR		m_szChannelId[CHANNELIDLEN+1];
	TCHAR		m_szSubId[CHANNELIDLEN+1];
	TCHAR		m_szChannelName[CHANNELNAMELEN+1];

	TCHAR		m_szChannelIP[IPADDRLEN+1];
	int			m_nChannelPort;

	int			m_nChannelActive;						///< 0: dead 1: alive

	int			m_nMemberCnt;							///< the current number of user 
	int			m_nMaxMember;							///< the max number of user

	int			m_nMaxRoom;
	int			m_nActiveRoom;
	int			m_nMaxRoomMembers;
	int			m_nMaxRoomViewers;
	BOOL		m_bEnterFailed;

public:

	CChannelData()
	{
		m_nChannelActive = 1; ///< activate
	}

	~CChannelData()
	{

	}
};
