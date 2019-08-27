/**    
    @file    ChannelData.h
    @date    2011/2/11
    @author  PJH(gamking@cj.com)
    @brief   채널 그룹 ( 게임마다 DataType이 다르기 때문에 일일이 세팅을 한다.
*/

#pragma once
#include "DataDefine.h"

class NMDATA_CLASS CChannelstGroupData
{
public:
	TCHAR		m_szGroupId[GROUPIDLEN+1];
	TCHAR		m_szGroupName[GROUPNAMELEN+1];
	int			m_nMinRating;
	int			m_nMaxRating;
	int			m_nMinAge;
	int			m_nMaxAge;
	TCHAR		m_szSex[SEXLEN+1];
	BOOL		m_bEnterable;

public:
	CChannelstGroupData()
	{
		::memset( this, 0, sizeof(*this) );
	}
};
