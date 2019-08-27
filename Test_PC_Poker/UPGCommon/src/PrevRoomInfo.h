#pragma once


//직전방 정보 저장 데이터
class UPG_COMMON_EXPORT CPrevRoomInfo
{	
	ROOMINFO	m_PrevRoomInfo[MAX_GAME];
public:
	CPrevRoomInfo(void);
	~CPrevRoomInfo(void);



	void SetPrevRoomInfo(IDX_GAME eGameIdx, ROOMINFO *pRI);
	ROOMINFO *GetPrevRoomInfo(IDX_GAME eGameIdx);

	BOOL IsPrevJoinedRoom(SMALLROOMINFO *pSRI);		//pSRI 방이 방금 전 내가 접속했었던 방인가?

	

};
