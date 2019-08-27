#pragma once

class CRoomData;

typedef std::vector<CRoomData*>	ROOM_LIST	;
typedef ROOM_LIST::iterator		ROOM_LIST_IT;

class COMMON_CLASS CRoomDataList
{
public:

	ROOM_LIST	m_list								;

	CRoomDataList();
	virtual ~CRoomDataList()						;

	void		Add  ( CRoomData* pNewData )		;
	VOID		Clear( bool bInit = true )			;

	CRoomData*	FindRoomData ( int nRoomId	 ) const;
};
