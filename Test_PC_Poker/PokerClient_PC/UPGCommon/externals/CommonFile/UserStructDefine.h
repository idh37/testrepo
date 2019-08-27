#ifndef __USERSTRUCTDEFINE_H__
#define __USERSTRUCTDEFINE_H__


//////////////////////////////////////////////////////////////////////////
//골든칩 보유 한도 
#define MAX_GOLDENCHIP_LIMIT 5000
//////////////////////////////////////////////////////////////////////////

#define NORMAL_MODE 0
#define CHOICE_MODE 1

#define MAX_LEVEL	20


//////////////////////////////////////////////////////////////////////////
//[프리미엄 개편 무료충전 추가]
const int CODE_CONTENTS_NORMAL =100001;			// 리더스 일반
const int CODE_CONTENTS_CERTIFICATE =100002;	// 리더스 상품권
//////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////
////////////		[대화 저장]		   //////////////
/////////////////////////////////////////////////////
#define MAX_CAPTURE_NUM           (300) // 최대 관리유저




//>>>>>>>>>> 넷마블프로젝트 1570 으로 세팅
#define CGV_EVENT_SITE_KIND (1570)
//<<<<<<<<<<<<< 여기까지



// [프리미엄 개편 무료충전 추가2] [2007-05]
enum STATUS_PREM
{
	//프리미엄 가입자
	STATUS_PREM_NOTJOINED=0,		//미가입자
		STATUS_PREM_NEWLYSIGNEDUP,  //신규가입자
		STATUS_PREM_EXTENDED,		//기존가입자(연장)
		STATUS_PREM_LONGTERM,		//장기상품가입자
		STATUS_PREM_CERTIFICATE,	//상품권가입자
};



//////////////////////////////////////////////////////////////////////////
//성별 사운드 정의
enum {
	
	SNDM0 =0,	
	SNDM1,
	SNDM2,	
	SNDM3,
	SNDF0,
	SNDF1,
	SNDF2,
	SNDF3,
};
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//서버 상태 
enum{
	WORKSTATE_NORMAL=0,		//정상
		WORKSTATE_CHECK,		//채널 종료중
		WORKSTATE_SHUTDOWN,		//채널 점중
};
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

//////// TODO: 정리필요 뉴포커만 사용 ////////
typedef struct
{
	BOOL bUse; // 사용유무
	int  nTotal;                // 관리자 수
	char ID[MAX_CAPTURE_NUM][ID_LEN]; // 관리자 아이디
} CAPTURE_INFO;


//특정아이디검색
typedef struct
{
	BOOL bUse; // 사용유무
	int  nTotal;                // 관리자 수
	char ID[MAX_CAPTURE_NUM][ID_LEN]; // 관리자 아이디
} SERACH_INFO;



typedef struct{
	char	ID[ID_LEN];			// 방장 아디

	short   nRoomNum;		// 방번호
	char 	nBombCard;		// 폭탄 카드 
	char	nState;			// add = 0 or open = 1

	char	nReMainTotalCardNum;	// 남은 카드 수
	char	nBossNum;		// 선 을 줌
	char    nTemp[2];
}BOMBCARD_DATA; 


// 수호천사 충전 값 
typedef struct{
	// 일반 
	INT64   MinMoneyLimit;
	INT64   MaxMoneyLimit;

	// 리더스 
	INT64   MinMoneyLimitLD;
	INT64   MaxMoneyLimitLD;

	// 리더스 플러스
	INT64   MinMoneyLimitLDPlus;
	INT64   MaxMoneyLimitLDPlus;

	//미니
	INT64   MinMoneyLimitLDMini;
	INT64   MaxMoneyLimitLDMini;

	//VIP
	INT64   MinMoneyLimitLDVIP;
	INT64   MaxMoneyLimitLDVIP;
}SAFEANGEL_DATA;

typedef struct{
	char nJokerCardDeck[5];
	char SPNum;					// 받는사람의 서버 인덱스 값 
	char temp[2];
}JOKERCARDDATA;







typedef struct{
	short nRoomNum;
	short nPNum;

	char  nIDColor;
	char  temp[3];
}CHANGE_GAMEUSERINFO;


//뉴도신지수 2006.3.2
typedef struct{
	char  ID[ID_LEN];

	short UNum;
	char  DS_IDColor;			// 도신 아디 칼라
	char  DS_Level;				// 도신 레벨 

	INT64 DS_GetMaxMoney;		// 최고 보유 머니
	INT64 DS_GetBetMaxMoney;	// 최고 획득 머니	

	short DS_SupplyBenefit;		// 지급 받은 혜택
	short nSupplyGoldenChip;	// 골든칩 수

	double DS_Exp;				// 경험치
}DS_DATA;


typedef struct{

	int   GroupNo;
	INT64 MinMoney;
	INT64 MaxMoney;

}GROUP_LIMITMONEY;




typedef struct {	
	////////////////////////////////////////////////////////////////////////////////	
	// tvN드라마 이벤트
	int			nPlayCnt;
	int			nEventTicket;	
	int			nBefor_EventTicket;
	int			GetTotalTicketNum(){
		return nBefor_EventTicket+nEventTicket;
	}
	////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////
	// 첫경험 이벤트 (이벤트 1,2,3)
#define FULL_MISSION_COUNT (10)	
	enum FIRSTEVENT_USERTYPE {	//이벤트 유저 타입
		FIRSTEVENT_USERTYPE_NULL=0,		//이벤트 대상자가 아니다. 
		FIRSTEVENT_USERTYPE_EVENT1=1,	//이벤트1 대상자 (이벤트1대상자는 한판을 하게되면 EVENT2대상자가된다)
		FIRSTEVENT_USERTYPE_EVENT2=2,	//이벤트2 대상자 (이벤트기간에 새로 가입한 유저)
		FIRSTEVENT_USERTYPE_EVENT3=3,	//이벤트3 대상자 (기존유저)

	};	
	char UserType;	// 신규유저인가?
	char nMissionCnt;		// 신규유저 승수 누적	(신규유저 FULL_MISSION_COUNT 승까지만 카운트한다.)
	char nMissionCntAdd;	// 이번 슨규유저 승수
	int nWinCnt;		// 승수	누적				(신규유저든 아니든 무조건 이길때 카운트한다)
	int nWinCntAdd;		// 이번 로그인시 승수	

} EVENT_PRIZE;


//[스행운]2007.10.17
typedef struct
{	
	int			nSpecialLuckItemCnt;	//월 구매 기회 노출 횟수 
	int			nTodayBuyItemCnt;		//일일 구매한 횟수
}SPECIALLUCKITEM;

typedef struct  {

	//블럭 유저 찬스아이템 제한 리스트
	char	BlockUserChanceItemList[48];
	//블럭 아이피 찬스아이템 제한 리스트
	char	BlockIPChanceItemList[48];

} BLOCKDATA;



typedef struct {

	int nState;		//진행상태	0 = pc방 아님, 1 = 참여가능, 2 = 응모완료
	int nPlayCnt;	//게임진행횟수 

	//추가
	int nBeforCoin;	//이전획득 코인	 
	int nAfterCoin;	//추가획득 코인

} PCROOM_EVENT_DATA;

typedef struct
{
	BOOL bPlayed;			//현재 게임 플레이 완료	
	char debug_code[16];		//테스트 코드
}UNIONEVENT_DATA;			//[머니통합이벤트]



//연말이벤트
typedef struct {

	int nState;		//진행상태	0 = 참여불가, 1 = 참여가능, 2 = 응모완료
	int nPlayCnt;	//게임진행횟수

} YEAREND_EVENT_DATA;

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////




//뉴도신지수 2006.3.2 올인천사
typedef struct  {
	char		ID[ID_LEN];				//아이디

	short		UNum;				//유저 넘버
	char		nChargeSaveCnt;		//남은 충전 횟수
	char		temp;

	INT64		nChargeMoney;		//중전된 머니 ( 1억으로 나눈값이 들어간다.)
}MYCHARGE_DATA;







typedef struct{
	char *cSection;
	char Len;
}SECTION ;

typedef struct{
	SECTION cSection;
	SECTION cKey;
	SECTION cValue;
}KEY_VALUE;


//도신 지수 레벨
int DS_GetLevel(INT64 Money,int winnum);
INT64 DS_GetLevel(int lv);

/////////////////////////////////////////////////////
////////////////     포카 정보     //////////////////
/////////////////////////////////////////////////////





typedef struct
{
	char nCard; // 대표카드
	int  nRSF;
	int  nSF;
	int  nFCARD;
} POKERINFO;





#endif