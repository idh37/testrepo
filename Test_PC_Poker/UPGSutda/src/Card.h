// Card.h: interface for the CCard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CARD_H__34924AA1_21CA_11D4_97A5_0050BF0FBE67__INCLUDED_)
#define AFX_CARD_H__34924AA1_21CA_11D4_97A5_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graphic.h"

enum
{
	EVENT_IAMREADY =1,			// 게임을 시작할 준비가 되었음
	EVENT_FIRSTBET=2,			// 첫번째 카드에서 좋은카드가 나옴
	EVENT_FLOORTOGETCARD=4,		// 먹은 카드를 가져옴
	EVENT_DUMMYTOMYCARD=8,		// 카드 더미에서 한장을 가져와 플레이어에게 넘긴다
	EVENT_ENDMYTURN=16,			// 내차례가 끝남
	EVENT_JOKBO=32,				// 족보 보기 이벤트  
	EVENT_AUTOHIDDEN=64,		// 자동쪼기를한다.
	EVENT_CARDOPEN=128,			// 카드를 오픈하고 족보를 확인한다.
	EVENT_OPENEFT=256,			// 족보에따른 이펙트 효과를 준다.
	EVENT_JOKBOCHECK=512,		// 족보확인 했다는 메세지를 준다.
	EVENT_WINDCARDEND=1024,		// 마지막카드를 보낸다.
};

typedef struct
{
	int CardNo;			// 카드 번호
	int EventCode;		// 이벤트 코드
	int Option;			// 이벤트 옵션
	int Option2;		// 이벤트 옵션2
} CARDEVENT_SUTDA;

class CCard  
{
public:
	void CardValue(int cardnum);
	void SetClickPos(int x,int y);
	NMBASE::GRAPHICGDI::CPage *pPage;
	NMBASE::GRAPHICGDI::xSprite *m_pHwatuNSpr;

	CARDEVENT_SUTDA Event;
	CARDEVENT_SUTDA Event2;

	// 카드 속성
	int CardNo;			// 카드번호(0: 상대방의 뒤집은 카드 이거나 안쓰임, 1-40:기본패 )
	int Who;			// 카드의 소유자(10:기리 9:판 0-2:플레이어)
	int Kind;			// 패의 분류 (1-12월... (13=서비스패))

	int ShowMode;		// 보여주기 방법(0:세팅 없음 1:딜레이 후 이동시 보여줌)

	// 사운드
//	int MoveStartSnd;	// 이동이 시작될때의 사운드
	//int MoveEndSnd;		// 이동이 끝났을때의 사운드
	char *MoveStartSnd;	// 이동이 시작될때의 사운드
	char *MoveEndSnd;	// 이동이 끝났을때의 사운드

	// 알림 이벤트
	BOOL bNotifyEvent;	// 알림 이벤트가 있는가?
	COLORREF NotifyStringColor;	// 알림 이벤트 스트링 색상
	CString NotifyString;// 알림 이벤트 스트링

	// 카드 상태
	BOOL bShow;			// 화면에 보여지는가?
	BOOL bFront;		// 앞면이 보이는 상태인가?
	BOOL bMove;			// 이동중인가?
	BOOL bEvent;		// 카드 이벤트가 있는가?
	BOOL bEvent2;
	BOOL bEndMove;		// 이동이 끝난카드
//--------------------

	// 카드 이동 관련 변수
	int Xp, Yp;			// 카드가 그려질 좌표
	int MoveLen;		// 이동한 거리
	int SXp, SYp;		// 카드 이동시 출발 좌표
	int TXp, TYp;		// 카드 이동시 목표 좌표
	int MoveDelay;		// 이동 시작 지연치
	int EndMoveDelay;	// 이동 완료 지연치
	int MoveSpeed;		// 이동 속도

	int CXp,CYp;        // 클릭당시의 마우스 포인트

	// 반짝임 관련 변수
	BOOL bTwincle;
	int TwincleCnt;		// 반짝임 카운트

	// 붉은색 점멸
	BOOL bRedTwincle;
	int RedTwincleCnt;		// 반짝임 카운트

	BOOL m_bActive;			//애니메이션 가동중인가...
	DWORD m_dwActiveTime;	//카드에서 액션을 보여줄때...
	
	//## 패 확대 ##//
	int m_State;	// 패 상태 0 -> 돌고있지 않다. 1 -> 도는 것이 예약되었다. 2-> 돌고 있다. 3-> 커지고 있다. 

	BOOL m_bResvGrowth;
	int	m_growth_delay;

	int	m_xl;
	int	m_subval_x;
	int	m_addval_x;

	int	m_yl;
	int	m_subval_y;
	int	m_addval_y;

	int	m_Max_Growth_X;	//카드가 커지는 최대비율 
	int	m_Max_Growth_Y;

	void ReserveGrowth( int growth_delay = 0 );
	//////////////////////////////////////////////////////////////////////////
	

	CCard();
	virtual ~CCard();

	void Init(NMBASE::GRAPHICGDI::CPage *ppage, NMBASE::GRAPHICGDI::xSprite *pHwatuNSpr);
	void Clear();
	void SetCard(int cardnum);
	void Reset();
	void OnTimer();
	void SetEvent(int eventcode, int option=0, int option2=0);
	void SetEvent2(int eventcode, int option=0, int option2=0);
	void ClearEvent();
	void ClearEvent2();
	void SetNotifyEvent(char *pstr, COLORREF color=RGB(0,0,0));
	void ClearNotifyEvent();
	void SetMoveStartSnd(char* sndfilename);
	void SetMoveEndSnd(char* sndfilename);
	void SetPos(int tx, int ty);
	void SetMovePos( int mx, int my);	//움직이면서 이동한다.
	void SetMove(int tx, int ty, int sdelay=0, int edelay=0, int speed=-1, int front=0, char* startsound = NULL, char* endsound = NULL);
	void CardDraw(BOOL bFold=FALSE, BOOL r_beft=FALSE);

	void SetShow( BOOL r_show );	//카드 그려주기 설정 
	void SetFront( BOOL r_front );	//뒷면 보이기
	void SetAnimation( int r_kind); //애니메이션을 설정한다.
//-------------------------------------------------------------
//-------------------------추가 부분---------------------------
	BOOL bFrontCall;	//카드 앞면보이기 예약
	int m_PNum; //현재 누구의 카드 소유로 되어 있나...
	
	//메이드 관련
	BOOL bMadeDraw;	//메이드 표시를 해줘야 하나...
	int nMadeCnt;	//메이드 표시를해 주는 것을 늦춰준다.
	void SetMadeDraw(int cnt = 0);
};

#endif // !defined(AFX_CARD_H__34924AA1_21CA_11D4_97A5_0050BF0FBE67__INCLUDED_)
