
// CardDefine.h

#ifndef CARDDEFINE_H_
#define CARDDEFINE_H_

#define CARD_GAB_Y		9

#define FLY_CARD_POS_X	457
#define FLY_CARD_POS_Y	15

enum EVENT_ENUM
{
	// [ 세븐 포커 ]
	EVENT_PREPARESTART_ONEEND = 0, // 카드 선택때 한장 받았따
	EVENT_THREEFLYEND = 1,		   // 첫 3장을 모두 날렸다
	EVENT_ONEFLYEND,			   // 한장씩
	EVENT_ONEFLYTOTEND,			   // 한장씩 모두

	EVENT_BD_PREPARESTART_ONEFLY_END,	// 처음 4장 중 한장 도착
	EVENT_BD_PREPARESTART_FOURFLY_END,  // 처음 4장 전부 도착
	EVENT_BD_CUTTING_WAIT,				// 커팅 카드 이동 명령 후 딜레이 타임이 0일때 호출
	EVENT_BD_CUTTING_MOVE_UP,			// 커팅 카드 위로 이동 한장 
	EVENT_BD_CUTTING_MOVE_DOWN,			// 커팅 카드 도착 한장
	EVENT_BD_CUTTING_ALL_DOWN,			// 커팅 카드 도착 모두
	EVENT_BD_CUTTING_PASS,				// 커팅 패스( 이동없이 이벤트처리 사운드 )
	EVENT_BD_CUTTING_ALL_PASS,			// 커팅 패스 도착 모두

	EVENT_BOMBFLYEND,
	EVENT_BOMBFLY_TO_END,
	EVENT_CHOICESELECT_AFTER_ONEEND,//초이스 카드 선택 후 한장씩 받을때


	//섯다
	EVENT_CARD_GATHEREND,			//결과 때 카드 모으기 끝
	EVENT_CARD_OPENEND_EFT,			//결과 오픈시 이펙트 처리
	EVENT_CARD_END_RESULT_CARD,		//결과 카드 오픈 종료
};

typedef struct
{
	int CardNo;			// 카드 번호
	int EventCode;		// 이벤트 코드
	int Option;			// 이벤트 옵션
	int Option2;		// 옵션 2
	int Option3;
} CARDEVENT;

typedef struct
{
	int Code;
	int Option;			// 이벤트 옵션
	int Option2;		// 옵션 2
	int Option3;
}RESERVATION;

enum CHIPKIND
{
	StartChip = 0,
	_5=StartChip,	//0
	_10,		//1
	_100,		//2
	_1K,		//3
	_10K,		//4
	_100K,		//5
	_1M,		//6
	_10M,		//7
	_100M,		//8
	_1Y,		//9
	_10Y,		//10
	_100Y,		//11
	_1000Y,		//12
	_1J,		//13
	_10J,		//14
	_100J,		//15
	_1000J,		//16
	_1KJ,		//17
	_10KJ,		//18
	_100KJ,		//19
	_1000KJ,	//20
	EndChip,
};

typedef struct 
{
	char chipnum;		// 칩 값	
	char sprnum;		// 스프라이트 넘버 
	char Twincnt[10];	// 투명값

	CPoint ptPos;	
}CHIPDATA;

typedef struct _tagMYCHIPDATA
{
	char	chipnum;			// 칩 값	
	char	sprnum;				// 스프라이트 넘버
	DWORD	dwStartTime;
	DWORD	dwMoveTime;
	int		nAlpha;
	bool	bMoveEnd;

	CPoint CurPos[10];			// 그려지는 좌표
	CPoint TarPos[10];			// 타겟 
	CPoint StartPos[10];		// 시작
}MYCHIPDATA;


#endif