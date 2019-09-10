#pragma once
#include "datalist.h"

class UPG_COMMON_EXPORT CDataRoomOptionList : public CDataList<int, ROOM_CREATEOPTION>
{
protected:
	//�������� �ݾ׺� ���ӹ� ����
	map<INT64, ROOMOPTION>		m_mapRoomOptions;		//map<�������ݾ�, ��ɼ�> //�� ������ �����Ʈ�ɼ�����; //���̹��� ���鶧 �����ϱ����� ����
	ROOMOPTION					m_QJRoom;
public:
	CDataRoomOptionList(void);
	virtual ~CDataRoomOptionList(void);
	virtual DATAENTITY* AddData(int key, ROOM_CREATEOPTION *data);
	virtual void Clear();

	ROOMOPTION *GetDefaultRoomOption(INT64 i64SeedMoney = 0, ROOM_CHIPKIND chipkind = ROOM_CHIPKIND_NORMAL );			//�ڽſ� �´� �⺻ �õ� �Ӵϸ� ����� �ش�.
	ROOMOPTION *GetDefaultFreeRoomOption(INT64 i64SeedMoney = 0);		//������ �߰��� ����Ʈ ������ �ɼ��� �ʿ�
	ROOM_CREATEOPTION *GetRoomCreationOption( INT64 llPMoney, ROOM_CHIPKIND chipkind = ROOM_CHIPKIND_NORMAL );
	ROOM_CREATEOPTION *GetFreeRoomCreationOption();	
	ROOM_CREATEOPTION *GetRoomCreationOption( ROOM_CHIPKIND chipkind = ROOM_CHIPKIND_NORMAL );
	BOOL IsAbleToJoinRoom(SMALLROOMINFO *pSRI);		//�ڽ��� �����Ҽ��ִ� ������ �ƴ��� �˷��ش�.
	BOOL IsTopClass(INT64 llPmoney);	//�Ӵϰ� �ֻ��� ������� ���θ� �����´�.	
	
	map<INT64, ROOMOPTION> *GetDefalutRoomOption()//����Ʈ �ɼǹ����� 
	{
		return &m_mapRoomOptions;
	}	


	BOOL GetEnterLimitMoneyKind(INT64 llPMoney, vector<INT64> &vecEnterLimitMoneyKind, BOOL bExceptFreeRoom);	//PMoney ������ ���ϴ� ������(10����, 100����) �� �����´�
	BOOL GetEnterLimitMoneyKind(vector<INT64> &vecEnterLimitMoneyKind, BOOL bExceptFreeRoom);// ��� ������(10����, 100����) �� �����´�
	BOOL GetWholeRoomOption(vector<ROOMOPTION> &vecEnterLimitMoneyKind, BOOL bExceptFreeRoom);	// ��� ��ɼ� ������ �����´�.

	ROOMOPTION *GetQJRoomOption(bool bUseGameMode, int nGameMode, bool bUseSeedMoney, int nSeedType) ;
	ROOMOPTION *GetQJRoomOption(bool bUseBetRule, bool bAutoBet, bool bUseSeedMoney, int nSeedType, short sBetRuleKind);

	//yoo
	INT64 FindMaxSeedMoney(INT64 llMoney, INT64 llUserMoney, char &cCardSetting);
};







