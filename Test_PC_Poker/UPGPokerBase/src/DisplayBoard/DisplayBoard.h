// DisplayBoard.h: interface for the CDisplayBoard class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

enum ENUM_DISPLAY_BOARD_TEXT
{
	EDBT_UNKNOWN = 0,
	EDBT_MINUSER,				//1명만 있을때
	EDBT_OPENCARD_CHOICE,		//7포커 일반모드, 오픈카드선택
	EDBT_SELECTCARD,			//카드를 선택중
	EDBT_THROWAWAY_CARD,		//카드를 버리다
	EDBT_AUTOBETTING,			//자동베팅 진행 중일때
	EDBT_2BETTING,				//2구베팅
	EDBT_3BETTING,				//3구베팅
	EDBT_4BETTING,				//4구베팅
	EDBT_5BETTING,				//5구베팅
	EDBT_6BETTING,				//6구베팅
	EDBT_7BETTING,				//7구베팅
	EDBT_HIDDENBETTING,			//마지막 히든 베팅
	EDBT_BETTING_COUNT,			//베팅 횟수
	EDBT_GAMESTART,				//첫번째 카드 딜링할때
	EDBT_2CARD,					//2번째 카드 딜링
	EDBT_3CARD,					//3번째 카드 딜링
	EDBT_4CARD,					//4번째 카드 딜링
	EDBT_5CARD,					//5번째 카드 딜링
	EDBT_6CARD,					//6번째 카드 딜링
	EDBT_BOMBCARD,				//폭탄 카드 분배
	EDBT_JOKERCARD,				//조커 카드 분배
	EDBT_HIDDENCARD,			//마지막 히든카드 분배
	EDBT_RESULT,				//결과 출력시
	EDBT_RESULT2,				//공동우승
	EDBT_LIMITTIME,				//시간 제한 초과 2회 이상
	EDBT_BEFORE_MORNING_BETTING,	//아침 전 베팅
	EDBT_MORNING_BETTING,		//아침 베팅
	EDBT_AFTERNOON_BETTING,		//점심 베팅
	EDBT_NIGHT_BETTING,			//저녁 베팅
	EDBT_MORNING_CUTTING,		//아침 컷팅
	EDBT_AFTERNOON_CUTTING,		//점심 컷팅
	EDBT_NIGHT_CUTTING,			//저녁 컷팅
	EDBT_SELECT_HLS,			//하이/로우/스윙 선택
	EDBT_RESULT_HILOW_1WIN,			//하이로우 결과 (승자1명)
	EDBT_RESULT_HILOW_2WIN,			//하이로우 결과 (승자2명)
	EDBT_FIRST_BETTING,			//첫 번째 베팅 (섯다)
	EDBT_FINAL_BETTING,			//마지막 베팅 (섯다)
	EDBT_FINAL_DEAL,			//"마지막 패입니다."
	EDBT_NOTIFY_REMATCH_BY_DRAW,	// 동일패 재경기
	EDBT_NOTIFY_REMATCH_BY_49,	// 49 재경기
	EDBT_RESULT_SUTDA,			//섯다 결과 (OO님 OOO 획득하셨습니다.)
	EDBT_SELECTCARD_SUTDA,		//섯다 패 선택창
	EDBT_NORMAL0_HOOLA,					//훌라: 평상시 전광판 메시지 랜덤노출1
	EDBT_NORMAL1_HOOLA,					//훌라: 평상시 전광판 메시지 랜덤노출2
	EDBT_NORMAL2_HOOLA,					//훌라: 평상시 전광판 메시지 랜덤노출3
	EDBT_MISSION_COMPLETE_HOOLA,		//훌라: 미션 성공, 해당 상황이 되면 평상 시 문구를 대체함.
	EDBT_STOPJACKPOT_BELOW_1JO_HOOLA,	//훌라: 스톱 잭팟 성공시(1조방 이하) 발생 게임방에만 노출
	EDBT_STOPJACKPOT_UPPER_3JO_HOOLA,	//훌라: 스톱 잭팟 성공시(3조방 이상) 발생 게임방에만 노출
	EDBT_MAX,
};

enum ENUM_RASE_STYLE
{
	ERS_START = 1,
	ERS_1122 = ERS_START,
	ERS_1222,
	ERS_1123,
	ERS_1223,
	ERS_11223,
	ERS_MAX,
};

class CUIInfoData;

#define MAX_ROUND 5

class UPG_GAMEBASE_EXPORT CDisplayBoard
{
protected:
	struct DrawTextInfo
	{
	COLORREF clrColor;
	CString strOutput;
	CString strFormat;
	};
protected:
	map<int, map<int, DrawTextInfo> >	m_mapText;

	CPoint						m_ptPos;
	CRect						m_rtSize;
	ENUM_DISPLAY_BOARD_TEXT		m_nTextIndex;

	CGameProcess				*m_pGameProcess;
public:
	CDisplayBoard();
	virtual ~CDisplayBoard();

	int GetTextWidth(CDC *pDC);

	virtual void Init(CUIInfoData *pUIInfoData, CGame *pGame);
	virtual void SetText(ENUM_DISPLAY_BOARD_TEXT nTextIndex, LPARAM lParam = 0);
	virtual void Draw(CDC *pDC);
	virtual int GetRaseCount(int nRase);
};