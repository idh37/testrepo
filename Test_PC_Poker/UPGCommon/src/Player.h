#pragma once

#include "UserStruct.h"

class UPG_COMMON_EXPORT CPlayer
{
protected:
	CHANGE_USERINFO_ROOM m_cRoomChangeInfo;
public:
	USERINFO			UI;
	int					ServPNum;				// ���� ������ �÷��̾� ��ȣ
	int					PNum;					// �÷��̾� ��ȣ
	NMBASE::AVATAR::CAvatar			Avatar;					// �ƹ�Ÿ Ŭ����

	int					JoinState;				// ���� ���� ����(0:����, 1:����)
	int					PlayState;				// �÷��� ����(0:��������, 1:������)

	int					nSndFxKind;
	int					MyIpCheck;

	INT64				nRoundPrevMoney;		//�ǽ��۸Ӵ�
	
	BOOL				bFold;
	INT64				nRealGold;				//���
	INT64				nPlayGold;				//���

	BOOL				bFoldOpen;				//��ǿ���(����ī�����)

public:
	virtual void Clear();
	virtual	void SetPlayerNum(int pnum);
	virtual void SetNewPlayer(USERINFO *pUI);
	virtual	void Reset();
	virtual	void SetGold(INT64 gold);

	virtual void SetChangeRoomInfo(CHANGE_USERINFO_ROOM &changeInfo);
	virtual CHANGE_USERINFO_ROOM *GetChangeRoomInfo(void){return &m_cRoomChangeInfo;}
	//virtual void InitGameJoinData(USERINFO *pUI = NULL);
public:
	CPlayer(void);
	virtual ~CPlayer(void);
};