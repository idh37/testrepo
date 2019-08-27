// UIInfoData.h: interface for the CSprMoneyDraw class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include <vector>

enum ENUM_POSTYPE
{
	EPT_UNKNOWN = -1,
	EPT_ME,
	EPT_LEFT,
	EPT_RIGHT,
};

enum ENUM_BUTTON_TYPE
{
	EBT_UNKNOWN = -1,
	EBT_PING_N,
	EBT_DADANG_N,
	EBT_CHECK_N,
	EBT_HALF_N,
	EBT_CALL_N,
	EBT_DIE_N,
	EBT_PING_H,
	EBT_DADANG_H,
	EBT_CHECK_H,
	EBT_HALF_H,
	EBT_CALL_H,
	EBT_DIE_H,
	EBT_PING_P,
	EBT_DADANG_P,
	EBT_CHECK_P,
	EBT_HALF_P,
	EBT_CALL_P,
	EBT_DIE_P,
	EBT_QUARTER_P,
	EBT_PING_V,
	EBT_DADANG_V,
	EBT_CHECK_V,
	EBT_HALF_V,
	EBT_CALL_V,
	EBT_DIE_V,
	EBT_GAME_TIP,
	EBT_CAPTURE,
	EBT_OPTION,
	EBT_MINIMIZE,
	EBT_CHANGEDISPLAY,
	EBT_TOP_EXIT,
	EBT_USER_GRADE,
	EBT_INVITE,
	EBT_OBSERVERVIEW,
	EBT_CARD_OPEN, // 본인 카드 오픈 버튼
	EBT_SINOUT,	// 관전 버튼
	EBT_BOTTOM_EXIT, // 나가기 버튼
	EBT_JOKBOHELP_ON,
	EBT_JOKBOHELP_OFF,
	EBT_JOKBOHELP_USE,
	EBT_JOKBOHELP_INFO,
	EBT_CHAT,
	EBT_JOKBO,
	EBT_EVENT,
	EBT_BADUSER,
	EBT_WHISPERLIST,
	EBT_SIDEPOT,
	EBT_SEATKEEPER,
	EBT_ONESHOTCHARGE,
	EBT_POKERSHOP,
	EBT_LOSSMONEYWARNING,
	EBT_LOSSMONEYWARNINGCLOSE,
	EBT_WAITUSER, // 대기버튼
	EBT_COMPLAIN,
	EBT_GIVEUP, // 기권하기 버튼
	EBT_MAX, // ???
};

enum ENUM_CHAT_COLOR
{
	ECC2_UNKNOWN = -1,
	ECC2_NOTIFY,			//입장,퇴장,112 신고등의 알림 메시지
	ECC2_USEITEM,			//아바타 구매, 충전, 보험금, 아이템 사용
	ECC2_CHAT,				//일반 채팅 내용
	ECC2_SERVMSG,			//서버에서 오는 메시지
	ECC2_WHISPER_SEND,		//귓말 보내기
	ECC2_WHISPER_RECEIVE,	//귓말 받기
	ECC2_RESULT,			//결과 일반
	ECC2_RESULT_WINNER,		//결과 승자 아이디
	ECC2_RESULT_WINMONEY,	//결과 승자 획득금액
	ECC2_RESULT_JOKBO,		//결과 승자 족보
	ECC2_RESULT_LOSER,		//결과 패자 아이디
	ECC2_RESULT_LOSEMONEY,	//결과 패자 잃은 금액
	ECC2_RESULT_JACKPOT,	//잭팟
	ECC2_MAX,
};

typedef struct _tagQUICKJOINAPPLYBUT
{
	CPoint ptButton[2];	//설정 / 취소
}SQUICKJOINAPPLYBUT;

class UPG_GAMEBASE_EXPORT CUIInfoData
{
public:
	bool					m_bObserverChat;				//관전자끼리만 채팅하도록 한다.

	CRect					m_rtJokoName;					//족보 이름을 표시할 영역
	COLORREF				m_clrJokboNameShadowColor;		//족보 이름을 표시할때의 그림자색
	COLORREF				m_clrJokboNameColor;			//족보 이름을 표시할때의 글자색

	CPoint					m_ptBonusCardBack;				//보너스 카드가 위치한 바닥의 배경 위치
	CPoint					m_ptBonusCardGab;				//보너스 카드가 위치할 보너스카드 바닥 과의 간격
	CPoint					m_ptBonusOpen;					//보너스 카드가 오픈될 위치
	CPoint					m_ptRuleKind;					//룰 종류 스프라이트 위치
	CPoint					m_ptDisCardGab;					//버린 카드의 위치, 자신의 카드 시작으로 부터의 상대 위치

	COLORREF				m_clrResultJokboNameWinColor;	//결과 출력시 각 플레이어가 승리 족보를 표시할 색
	COLORREF				m_clrResultJokboNameLoseColor;	//결과 출력시 각 플레이어가 패배 족보를 표시할 색
	COLORREF				m_clrResultMoneyWinColor;		//결과 출력시 각 플레이어가 딴 머니를 표시할 색
	COLORREF				m_clrResultMoneyLoseColor;		//결과 출력시 각 플레이어가 잃은 머니를 표시할 색

	CRect					m_rtResultJokboYouSize;			//결과 출력시 상대 플레이어의 족보
	CRect					m_rtResultMoneyYouSize;			//결과 출력시 상대 플레이어가 잃거나 딴 머니를 표시할 영역의 크기
	CPoint					m_ptResultBackYouGab;			//결과 출력시 상대 플레이어의 결과 바닥의 위치, 카드시작점으로 부터의 상대값

	CRect					m_rtResultJokboMySize;			//결과 출력시 내 족보
	CRect					m_rtResultMoneyMySize;			//결과 출력시 내가 잃거나 딴 머니를 표시할 영역의 크기
	CPoint					m_ptResultBackMyGab;			//결과 출력시 내 결과 바닥의 위치, 카드시작점으로 부터의 상대값

	CPoint					m_ptResultJokboNameGab;			//결과 출력시 각 플레이어의 족보 표시 위치, 결과바닥으로 부터의 상대값
	CPoint					m_ptResultMoneyGab;				//결과 출력시 각 플레이어의 머니 표시 위치, 결과바닥으로 부터의 상대값

	CPoint					m_ptTimer;						//다른 유저의 타이머의 위치
	
	std::vector<CPoint>		m_ptlistBossMarkGab;			//보스 마크표시 - (자신,왼쪽,오른쪽) (카드의 시작점을 기준으로한 상대 위치)
	std::vector<CPoint>		m_ptlistDealerMarkGab;			//딜러 마크표시 - (자신,왼쪽,오른쪽) (카드의 시작점을 기준으로한 상대 위치)
	std::vector<CPoint>		m_ptlistHostMarkGab;			//방장 마크표시 - (자신,왼쪽,오른쪽) (아바타 기준 상대 위치)

	std::vector<CPoint>		m_listCardStartPos;				//각 플레이어의 카드의 시작 위치를 가지고 있는 리스트
	std::vector<CPoint>		m_listAvatarPos;				//각 플레이어의 아바타의 시작 위치를 가지고 있는 리스트
	std::vector<CRect>		m_listUserMoneyRect;			//각 플레이어의 보유머니 표시 위치
	int						m_nCardGab;						//각 카드의 간격을 나타냄
	int						m_nMyCardGab;					//내가 가진 각 카드의 간격을 나타냄
	CPoint					m_ptOverHaveMoneyGab;			//플레이어가 실제로 가지고 있는 머니를 표시하는 툴팁 위치, 유저머니 표시 상단 중앙을 기준으로한 상대 위치

	CPoint					m_ptUserNameGab;				//각 플레이어의 이름을 표시할 아바타 시작점으로 부터의 상대좌표
	CRect					m_rtUserName;					//각 플레이어의 이름을 표시할 영역의 크기
	COLORREF				m_clrUserNameShadowColor;		//각 플레이어의 이름을 표시때 사용할 그림자 색
	COLORREF				m_clrUserNameColor;				//각 플레이어의 이름을 표시때 사용할 글자색

	std::vector<CPoint>		m_listLevelGab;					//각 플레이어의 등급을 표시할 유저 아바타 위치로 부터의 상대좌표
	CSize					m_szAvatar;						//아바타의 크기
	CPoint					m_ptReservation;				//나가기/관전예약 표시 위치, 아바타 시작위치로 부터의 상대좌표

	CRect					m_rtMyTotalBettingMoney;		//자신의 총 베팅 금액 표시 영역 크기
	CPoint					m_ptMyTotalBettingMoney;		//자신의 총 베팅 금액 표시 위치
	COLORREF				m_clrMyTotalBettingMoneyText;	//자신의 총 베팅 금액 표시 색
	COLORREF				m_clrMyTotalBettingMoneyOutline;//자신의 총 베팅 금액 표시 외곽선 색

	CRect					m_rtBettingMoney;				//이전 베팅 금액 표시 영역 크기
	COLORREF				m_clrBettingMoneyText;			//이전 베팅 금액 표시 색
	COLORREF				m_clrBettingMoneyOutline;		//이전 베팅 금액 표시 외곽선 색
	std::vector<CPoint>		m_listBettingMoneyPos;			//이전 베팅 금액 표시 위치, 각 사용자별 위치

	CRect					m_rtBackTotalMoney;				//게임중 바탕에 총 배팅 금액을 표시할 영역
	CRect					m_rtBackCallMoney;				//게임중 바탕에 콜 할때 필요한 금액을 표시할 영역

	CPoint					m_ptJacpotBack;					//잭팟 바닥을 그려줄 위치
	CPoint					m_ptJacpotNumberStart;			//잭팟 숫자 표시 시작 위치
	int						m_nJacpotNumberWidth;			//잭팟 숫자의 폭

	std::vector<CPoint>		m_listButtonPos;				//버튼의 위치를 가지고 있는 리스트
	CRect					m_rtBetBtnCombo;				//베팅 버튼 타입 선택 콤보박스의 렉트

	CRect					m_ptBtnMoneyDrawRect;			//버튼을 클릭했을때 지불되는 머니를 표시하는 렉트
	std::vector<CPoint>		m_ptBtnMonyDrawGab;				//버튼을 클릭했을때 지불되는 머니를 표시하는 위치, 버튼 시작점을 기준으로한 상대좌표(N, H, P형)
	
	COLORREF				m_clrBtnBetText;				//버튼의 베팅 금액 표시 글자색
	COLORREF				m_clrBtnBetTextShadow;			//버튼의 베팅 금액 표시 글자의 그림자색
	CRect					m_rtBtnBetText;					//버튼의 베팅 금액 표시 영역 크기
	CRect					m_rtObserverCount;				//관전자수 표시 영역의 크기
	COLORREF				m_clrObserverCount;				//관전자수 표시 글자색
	CPoint					m_ptObserverCountGab;			//관전자수 표시 영역 위치, 관전자보기 버튼에서의 상대위치

	int						m_nMaxJokbo;					//족보도우미 최대 표시 족보수
	std::vector<std::string> m_listJokboName;				//족보 이름
	CRect					m_rtJokboHelpPercent;			//족보도우미 각각의 족보 확률표시 크기
	CPoint					m_ptJokboHelpPercent;			//족보도우미 족보 확률표시 시작위치
	int						m_nJokboHelpHightGab;			//각각의 족보 확률표시의 높이 간격
	CPoint					m_ptJokboHelpCompleteGab;		//완성된 족보를 패시하기 위한 각각의 족보 확률표시 시작위치에서의 간격
	COLORREF				m_clrJokboHelpNoPossibilityName;//족보가 이미 완성된것 보다 작은 족보이거나 가능성이 없는 족보이름 표시 색
	COLORREF				m_clrJokboHelpPossibilityName;	//가능한 족보의 족보이름 표시 색
	COLORREF				m_clrJokboHelpMyJokboName;		//족보 도우미에서 자신의 족보의 색
	COLORREF				m_clrJokboHelpPercent;			//족보 확률 표시 색
	CPoint					m_ptJokboHelpPercentGab;		//각각의 족보의 확률표시 위치, m_ptJokboHelpPercent에서의 상대 좌표 이다
	CPoint					m_ptJokboHelpBack;				//도우미 배경 위치
	CPoint					m_ptJokboHelpUseDlg;			//도우미 사용 버튼을 클릭햇을때 나타는 다이알로그 위치
	CPoint					m_ptJokboHelpUseDlgUseBtnGab;	//도우미 사용창에서 사용하기 버튼의 위치, 창의 상대좌표
	CPoint					m_ptJokboHelpUseDlgCancelBtnGab;//도우미 사용창에서 취소 버튼의 위치, 창의 상대좌표
	CPoint					m_ptJokboHelpUseDlgOK;			//도우미 사용 결과창에서 확인 버튼의 위치, 창의 상대좌표
	CPoint					m_ptJokboArrowGab;				//각 사용자의 족보 표시에 나타나는 화살표의 좌표, 카드로 부터의 상대 좌표
	CPoint					m_ptJokboInfo;					//족보 설명이 표시될 위치

	std::vector<CPoint>		m_listUserChangeGameInfoBackPos;//ENUM_POSTYPE별 게임중 변동금액/진행판수 표시 배경 이미지 위치
	CRect					m_rtUserChangeGameInfoMoney;	//변경머니 표시 영역, 각 플레이어의 변동금액/진행판수 표시 배경 이미지에서의 상대 좌표
	CRect					m_rtUserChangeGameInfoInfo;		//진행판수 표시 영역, 각 플레이어의 변동금액/진행판수 표시 배경 이미지에서의 상대 좌표
	CRect					m_rtUserChangeGameInfoLimit;	//최대베팅 표시 영역, 각 플레이어의 변동금액/진행판수 표시 배경 이미지에서의 상대 좌표
	COLORREF				m_clrUserChangeGameInfoMoneyWin;//딴금액을 표시할 색
	COLORREF				m_clrUserChangeGameInfoMoneyLose;//잃은머니 표시할 색
	COLORREF				m_clrUserChangeGameInfoInfo;	//진행판수 표시할 색
	COLORREF				m_clrUserChangeGameInfoLimitMoney;//딴금액을 표시할 색

	CPoint					m_ptCardDeck;					//카드덱의 위치
	CRect					m_rtRoomTitle;					//방제 출력 크기
	CPoint					m_ptRoomTitle;					//방제 출력 위치
	COLORREF				m_clrRoomTitle;					//방제 출력 색
	COLORREF				m_clrRoomTitleOutLine;			//방제 출력 아웃라인색

	std::vector<COLORREF>	m_listChatColor;				//채팅창에 출력될 ENUM_CHAT_COLOR 상태별 색
	CPoint					m_ptChat;						//채팅창의 시작 위치
	CSize					m_szChat;						//채팅창의 크기
	CPoint					m_ptInfo;						//정보창의 시작 위치
	CSize					m_szInfo;						//정보창의 크기
	CPoint					m_ptEvent;						//이벤트창의 시작 위치
	CPoint					m_ptEdit;						//에디트창의 시작 위치
	CSize					m_szEdit;						//에디트창의 크기
	CPoint					m_ptInfoScroll;					//스크롤바의 시작 위치
	int						m_nInfoScrollHeight;			//스크롤바의 높이
	CPoint					m_ptChatScroll;					//스크롤바의 시작 위치
	int						m_nChatScrollHeight;			//스크롤바의 높이
	CPoint					m_ptEventPage;					//이벤트 페이지 단위 표시 마지막 페이지 시작위치, 이벤트창 시작 위치로부터의 상대값
	int						m_nEventPageGab;				//이벤트 각각의 페이지 단위 표시 X축 간격

	std::vector<CPoint>		m_listPlayerBackPos;			//각 플레이어의 바탕 위치

	CPoint					m_ptBackChipCenter;				//바닥 칩의 기준 위치
	std::vector<std::vector<CPoint> > m_listBackChipGabPos;	//바닥 칩의 위치 정보
	CPoint					m_ptChipEndGab;					//승자에게 칩이 들어갈때 각 유저의 목적지 계산을 위한 위치 차이, 각 유저의 카드의 시작점을 기준으로 한다. 칩은 다섯줄을 기본으로 해서 위치 차이를 지정
	DWORD					m_dwChipMoveDelayTime;			//결과 출력시 칩이 움직이기전 대기 시간

	CRect					m_rtDisplayBoard;				//전광판의 크기
	CPoint					m_ptDisplayBoard;				//전광판의 위치
	COLORREF				m_clrDisplayNormal;				//전광판의 일반 텍스트 색
	COLORREF				m_clrDisplayJokbo;				//전광판의 족보 색
	COLORREF				m_clrDisplayMoney;				//전광판의 머니 색

	CPoint					m_ptBettingCmd;					//유저의 마지막 베팅을 표시해주는 스프라이트의 위치

	CRect					m_rtSidePotMoneyName;			//사이드팟 이름의 표시 영역 크기를 지정
	CPoint					m_ptSidePotMoneyNameGab;		//사이드팟 이름의 표시 위치, 각 사이트팟을 출력할 이미지의 시작점으로 부터의 상대값
	COLORREF				m_clrSidePotMoneyName;			//사이드팟 이름의 색
	CRect					m_rtSidePotMoney;				//각 사이드팟의 머니의 표시 영역의 크기를 지정
	CPoint					m_ptSidePotMoneyGab;			//각 사이드팟의 머니의 표시 위치, 각 사이트팟을 출력할 이미지의 시작점으로 부터의 상대값
	COLORREF				m_clrSidePotMoney;				//사이드팟 머니의 색
	CPoint					m_ptSidePotResult;				//사이드팟 결과표시 위치
	CRect					m_rtSidePotResultVS;			//사이드팟 결과시 각 팟의 승/패 여부 표시 영역의 크기
	CPoint					m_ptSidePotResultVSGab;			//사이드팟 결과시 각 팟의 승/패 여부 표시 영역의 위치
	int						m_nSidePotResultGab;			//사이드팟 결과시 각 팟의 승/패 여부 표시 영역의 높이차
	CRect					m_rtSidePotResultMoney;			//사이드팟 결과시 각 팟의 머니 표시 영역의 크기
	CPoint					m_ptSidePotResultMoneyGab;		//사이드팟 결과시 각 팟의 머니 표시 영역의 위치
	CRect					m_rtSidePotResultTotalMoney;	//사이드팟 결과시 전체 획득 머니 표시 영역의 크기
	CPoint					m_ptSidePotResultTotalMoneyGab;	//사이드팟 결과시 전체 획득 머니 표시 영역의 위치

	CRect					m_rtHighJokboUserName;			//4카드 이상 족보를 달성했을때 나타나는 이펙트에 표시할 유저 이름 영역 크기
	CPoint					m_ptHighJokboUserName;			//4카드 이상 족보를 달성했을때 나타나는 이펙트에 표시할 유저 이름 위치
	COLORREF				m_clrHighJokboUserName;			//4카드 이상 족보를 달성했을때 나타나는 이펙트에 표시할 유저 이름 색
	CRect					m_rtHighJokboName;				//4카드 이상 족보를 달성했을때 나타나는 이펙트에 표시할 족보 이름 영역 크기
	CPoint					m_ptHighJokboName;				//4카드 이상 족보를 달성했을때 나타나는 이펙트에 표시할 족보 이름 위치
	COLORREF				m_clrHighJokboName;				//4카드 이상 족보를 달성했을때 나타나는 이펙트에 표시할 족보 이름 색
	CRect					m_rtHighJokboWinMoney;			//4카드 이상 족보를 달성했을때 나타나는 이펙트에 표시할 획득 머니 영역 크기
	CPoint					m_ptHighJokboWinMoney;			//4카드 이상 족보를 달성했을때 나타나는 이펙트에 표시할 획득 머니 위치
	COLORREF				m_clrHighJokboWinMoney;			//4카드 이상 족보를 달성했을때 나타나는 이펙트에 표시할 획득 머니 색
	CRect					m_rtHighJokboJacpotMoney;		//잭팟시 획득 머니 영역 크기
	CPoint					m_ptHighJokboJacpotMoney;		//잭팟시 획득 머니 위치
	COLORREF				m_clrHighJokboJacpotMoney;		//잭팟시 획득 머니 색

	CPoint					m_ptObserver;					//관전자 판넬 표시 위치
	CPoint					m_ptObserverBtnGab;				//관전자 참여하기/참여취소 버튼의 관전자 팔넬로 부터의 상대 위치
	CPoint					m_ptObserverReservePriorityGab;	//관전자 판넬에서 참여예약시 예약순위 표시 위치

	std::vector<CPoint>		m_listSeatKeeperWinPos;			//자리지킴이 윈도우 위치값(0, 1, 2, 3, 4) ->아바타를 기준으로한 상대 좌표
	std::vector<CPoint>		m_listTurnDisplayPos;			//자리지킴이 윈도우를 기준으로한 상대좌표(윈도우를 기준으로한 상대좌표)
	CPoint					m_TurnTextPos;					//자리지킴이 턴숫자 출력 위치(윈도우를 기준으로한 상대좌표)
	CPoint					m_SeatKeeperJoinButPos;			//자리지킴이 게임참여 버튼(윈도우를 기준으로한 상대좌표)
	CRect					m_rtSeatKeeperWeb;				//자리지킴이 웹(윈도우를 기준으로한 상대좌표)
	COLORREF				m_clrSeatKeeperTurn;			//자리지킴이 턴 색

	DWORD					m_dwBombCardOpenMoveTime;		//폭탄카드를 오픈 or 오픈후 제자리로 돌리기 위해서 하여 이동하는 시간 
	CPoint					m_ptBombEffectGab;				//폭탄이펙트 재생 위치, 폭탄 카드로 부터의 상대 좌표
	CPoint					m_ptJokerEffectGab;				//조커이펙트 재생 위치, 폭탄 카드로 부터의 상대 좌표
	DWORD					m_dwBombEffectGab;				//폭탄이펙트 재생시 각 프레임의 재생시간
	DWORD					m_dwStartGameDealingTimeGab;	//최초 카드(3,4장) 딜링시 딜링 시간차
	float					m_fStartGameDealingCardSpeed;	//최초 카드(3,4장) 딜링시 딜링 카드의 재생 배속
	DWORD					m_dwDealingTimeGab;				//카드 딜링시 각 카드별 딜링 시간차
	float					m_fDealingCardSpeed;			//카드 딜링시 각 카드별 딜링시 딜링 카드의 재생 배속

	std::vector<CPoint>		m_listSameIPGab;				//ENUM_POSTYPE별 동일 아이피 표시 위치, 아바타를 기준으로한 상대 좌표
	std::vector<bool>		m_listSameIPMarkRight;			//ENUM_POSTYPE별 동일 아이피 표시 위치, 아바타의 오른쪽에 표시되는지의 여부

	CPoint					m_ptAutoBettingPos;				//자동베팅 위치

	CPoint					m_ptOneShotDlg;						//원샷충전 다이알로그 출력 위치
	CPoint					m_ptOneShotDlg_btnOnceGab;			//원샷충전 다이알로그의 한번에 받기 버튼 위치(상대 위치)
	CPoint					m_ptOneShotDlg_btnTwiceGab;			//원샷충전 다이알로그의 나눠서 받기 버튼 위치(상대 위치)
	CPoint					m_ptOneShotDlg_btnCloseGab;			//원샷충전 다이알로그의 닫기 버튼 위치(상대 위치)
	CRect					m_rtOneShotPossibilityMoneySize;	//원샷충전 지급가능 금액 표시 크기
	CPoint					m_ptOneShotPossibilityMoneyGab;		//지급 가능 금액 표시 위치, 다이알로그 시작 위치로부터의 상대 좌표
	COLORREF				m_clrPossibilityMoneyOutline;		//지급 가능 금액 표시 외곽선색
	COLORREF				m_clrPossibilityMoney;				//지급 가능 금액 표시 색
	CRect					m_rtOneShotOnceTextSize;			//한번에 충전 금액 표시 크기
	CPoint					m_ptOneShotOnceTextGab;				//한번에 충전 금액 표시 위치, 다이알로그 시작 위치로부터의 상대 좌표
	CRect					m_rtOneShotTwiceTextSize;			//나눠서 충전 금액 표시 크기
	CPoint					m_ptOneShotTwiceTextGab;			//나눠서 충전 금액 표시 위치, 다이알로그 시작 위치로부터의 상대 좌표
	COLORREF				m_clrTextOutline;					//한번에/나눠서 충전 그액 표시 외곽선색
	COLORREF				m_clrText;							//한번에/나눠서 충전 그액 표시	색
	CRect					m_rtOneShotError;					//원샷 충전 실패시 에러문구 노출 영역 크기
	CPoint					m_ptOneShotErrorGab;				//원샷 충전 실패시 에러문구 노출 영역 위치
	COLORREF				m_clrResultOutline;					//결과 출력시 문자 외곽선 색
	COLORREF				m_clrResultEmphasis;				//결과 출력시 강조 문자 텍스트 색
	COLORREF				m_clrResultNormal;					//결과 출력시 일반 문자 텍스트 색
	CRect					m_rtMsgSize;						//지급/예약시 메시지 표시 영역의 크기
	CPoint					m_ptSuccessMoneyGab;				//지급시 원샷충전으로 지급되는 머니표시 표시 영역의 위치
	int						m_nLineGab;							//지급/예약 메시지 표시시 라인의 수직 위치 차이
	CPoint					m_ptReservationGab;					//예약시 메시지 출력 위치, 상대좌표

	CPoint					m_ptOwnLimitMarkGab;				//보유한도 초과 마크 표시 위치, 아바타로 부터의 상대 위치
	CPoint					m_ptOwnLimitTipGab;					//보유한도 초과 팁의 위치, 마크로부터의 상대좌표
	CRect					m_rtOwnLimitMoneySize;				//보유한도 초과 금액 표시영역 크기
	CPoint					m_ptOwnLimitMoneyGab;				//보유한도 초과 금액 표시위치, 팁으로부터의 상대좌표
	CPoint					m_ptOwnLimitGoldGab;				//보유한도 초과 골드 표시위치, 초과 머니로부터의 상대좌표	
	COLORREF				m_clrOwnLimitMoney;					//보유한도 촤과 금액 표시색

	CPoint					m_ptChipStartGab;					//자신이 던지는 칩의 위치, 카드의 시작 위치로 부터의 상대 좌표

	CPoint					m_ptSelectCardFirstCard_Choice;		//초이스 카드 선택 창의 위치
	CPoint					m_ptSelectCardFirstCard_Normal;		//일반 카드 선택 창의 위치
	CRect					m_rtSelectCardFirstCard_Choice;		//초이스 카드 선택 창에서 첫번째 카드의 영역, 창의 시작위치에서의 상대 좌표
	CRect					m_rtSelectCardFirstCard_Normal;		//일반 카드 선택 창에서 첫번째 카드의 영역, 창의 시작위치에서의 상대 좌표
	int						m_nSelectCardCardGab;				//카드 선택 창에서 각 카드의 위치 차
	CPoint					m_ptSlectCardSelectMarkGab;			//버릴카드/오픈카드 선택 마크 간격을 나타낸다. 각카드에서의 상대 좌표

	CPoint					m_ptQuickJoinDlgSearchON;			// 내맘대로 바로입장 사용/ 사용안함
	CPoint					m_ptQuickJoinDlgSearchOFF;			
	std::vector<SQUICKJOINAPPLYBUT>	m_listQuickJoinDlgApplayBut;// 설정버튼
																
	std::vector<CPoint>		m_listPokerTimeEffectPos;			//각 유저별 포커타임 이벤트 당청 이펙트 위치, 각각의 유저의 카드 시작 위치와의 간격
	CRect					m_rtPokerTimeEffect;				//이펙트 표시 영역의 크기

	std::map<IDX_GAME, CRect>	m_listRoomInfoMinimumMoney;		//방정보, 입장제한 머니 표시
	std::map<IDX_GAME, CRect>	m_listRoomInfoRule;				//방의 포커룰 표시 영역
	std::map<IDX_GAME, COLORREF>	m_listRoomInfoRuleColor;	//방의 포커룰 표시 텍스트 색

	COLORREF				m_clrRoomInfoMinimumMoney;			//입장 제한 머니 텍스트 색
	COLORREF				m_clrRoomInfoOutline;				//외곽선 색
	CRect					m_rtRoomInfoPokerKind;				//맞포커에서 게임종류 표시 영역

	CPoint					m_ptWaitAlertPos;					//규제안 : 대기하기 상태 이미지 위치

	CRect					m_ptGold;							//일반 경기장 골드 좌표
	COLORREF				m_clrGold;							//일반 경기장 골드 텍스트 색상
	COLORREF				m_clrGoldDiff;						//일반 경기장 골드 변경 색상	
	COLORREF				m_clrGoldReward;					//골드 리워드 색상	
	//yoo 2015.01 규제안
	std::vector<CPoint>		m_listLimitIcon[2];					//손실머니 경고 표시 아이콘
public:
	CUIInfoData();
	virtual ~CUIInfoData();

	virtual void Init(void);

	const char *GetJokboName(int nKind);
};