#pragma once


//������ ���� ���� ������
class UPG_COMMON_EXPORT CPrevRoomInfo
{	
	ROOMINFO	m_PrevRoomInfo[MAX_GAME];
public:
	CPrevRoomInfo(void);
	~CPrevRoomInfo(void);



	void SetPrevRoomInfo(IDX_GAME eGameIdx, ROOMINFO *pRI);
	ROOMINFO *GetPrevRoomInfo(IDX_GAME eGameIdx);

	BOOL IsPrevJoinedRoom(SMALLROOMINFO *pSRI);		//pSRI ���� ��� �� ���� �����߾��� ���ΰ�?

	

};
