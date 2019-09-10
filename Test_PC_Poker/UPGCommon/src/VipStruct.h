#pragma once

//////////////////////////////////////////////////////////////////////////
//////////////////////      ���� �α� ����      //////////////////////////
//////////////////////////////////////////////////////////////////////////


const int PREM_SERVICE_NOT = 0;
const int PREM_LEADERSNOT = 0; // ������ �������� �ʾ���
const int PREM_LEADERSMINI = 7; // ������ �̴�
const int PREM_LEADERS = 1; // ������
const int PREM_LEADERSPLUS = 4; // ������ �÷���
const int PREM_LEADERSVIP   = 15; // ������ 

// �α׼��� �α�������
typedef struct 
{
	BYTE	nType;					// 0 : ����, 1 : ����
	int		nGameCode;
	int		nPremCode;				// PREM_LEADERSVIP �ΰ�츸 �α��� ���
	int		nGroupNum;
	int		nChannelNum;
	int		nRoomNum;
	char	szID[20];
	char	szPass[20];
} ST_VIPSERVER_LOGIN_INFO;

typedef struct 
{
	int				nType;			// 0 : ������ ���񽺰� �ƴҶ� 1:���ð� �ƴ� / 2:�Ͻ��ߴ� / 3:��Ÿ.
	BOOL			bService;		// 0 : ���� ���� / 1:������
	int				nStartTime;
	int				nEndTime;
} ST_SERVICE_ON_OFF;

typedef struct 
{

	char	szID[20];
	int		nGameCode;
	int		nPremCode;
	int		nGroupNum;
	int		nChannelNum;
	int		nRoomNum;
	int		nWaitingNum;
	BOOL    bCounseling;
	int		nCounselRoomNum;

} ST_WAITING_USERINFO;

////////////////////////////////////////////////
// ���� Ŭ���̾�Ʈ���� ���� ����ü