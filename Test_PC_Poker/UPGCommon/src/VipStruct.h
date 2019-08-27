#pragma once

//////////////////////////////////////////////////////////////////////////
//////////////////////      베팅 로그 관련      //////////////////////////
//////////////////////////////////////////////////////////////////////////


const int PREM_SERVICE_NOT = 0;
const int PREM_LEADERSNOT = 0; // 리더스 가입하지 않았음
const int PREM_LEADERSMINI = 7; // 리더스 미니
const int PREM_LEADERS = 1; // 리더스
const int PREM_LEADERSPLUS = 4; // 리더스 플러스
const int PREM_LEADERSVIP   = 15; // 리더스 

// 로그서버 로그인정보
typedef struct 
{
	BYTE	nType;					// 0 : 서버, 1 : 어드민
	int		nGameCode;
	int		nPremCode;				// PREM_LEADERSVIP 인경우만 로그인 허용
	int		nGroupNum;
	int		nChannelNum;
	int		nRoomNum;
	char	szID[20];
	char	szPass[20];
} ST_VIPSERVER_LOGIN_INFO;

typedef struct 
{
	int				nType;			// 0 : 서비스중 서비스가 아닐때 1:상담시간 아님 / 2:일시중단 / 3:기타.
	BOOL			bService;		// 0 : 서비스 중지 / 1:서비스중
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
// 어드민 클라이언트에서 쓰는 구조체