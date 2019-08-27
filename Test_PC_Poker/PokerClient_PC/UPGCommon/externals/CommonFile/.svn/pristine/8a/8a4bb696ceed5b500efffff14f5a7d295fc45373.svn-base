// SevenRuleMan.h: interface for the CSevenRuleMan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEVENRULEMAN_H__D2F5D281_61ED_4C31_98B5_1C53556A4D52__INCLUDED_)
#define AFX_SEVENRULEMAN_H__D2F5D281_61ED_4C31_98B5_1C53556A4D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CardRule.h"


enum RULE_KIND
{
	// 정통 넷마블 룰 (동일족보비교 -> 모든 나머지카드 들의 숫자를 비교 -> 패를 이루는 카드들의 무늬 비교)
	RULE_KIND_ORIGINAL_NETMARBLE,
	// 한게임 룰 (동일족보비교 -> 나머지카드 중 낮은 숫자 2장 버리고 난 카드들의 숫자비교 -> 패를 이루는 카드들의 무늬 비교)
	RULE_KIND_HANGAME,
};

class CSevenRuleMan  
{
	enum {
		WIN		= 0,
		LOSE	= 1,
		EQUAL	= 2,
		TOTAL_USER = 7,
	};

public:

	CCardRule m_cRule[TOTAL_USER];
	int		m_Ranking[TOTAL_USER]; // ranking
	int		m_Ranking_low[TOTAL_USER]; // ranking
	int		m_BoardCard;			// 폭탄 카드로 쓰인거다 넣을려면 넣어라..ㅡㅡㅋ

	RULE_KIND m_RuleKind;

public:

	CSevenRuleMan();
	virtual ~CSevenRuleMan();
	
public:

	void Init();

	void get_cal_rule_user(int pnum);
	int IsSpecial_Rule(int pnum);
	int get_Winner(int rank=1);
	int get_MyRule(int pnum, bool bUseLog = false );
	int get_Rank(int pnum);

	bool is_BoardCard(int cnum) { 
		if(m_BoardCard==cnum) return true;
		else return false; 
	}

	void set_boardCard(int cnum) { m_BoardCard = cnum; }
	int  get_boardCard() const { return m_BoardCard; }
	
	BOOL IsExtraCardWin(int pnum, int ncard);
	BOOL IsExtraCardLose(int pnum, int ncard);
	BOOL IsExtraCardEqual(int pnum, int ncard);
	BOOL IsRuleComplate(int pnum, int ncard);
	BOOL IsHaveEventCard(int pnum, int ncard, int range_GTE = CCardRule::N7C_DUMMY /* all rule*/);
	BOOL IsHaveEventCardEx(int pnum,int pkind,BOOL *blucky, char pcard[],char ppcard[],int cardnum,int jokercard = -1, int range_GTE = CCardRule::N7C_DUMMY /* all rule*/);
	
	BOOL IsHaveCurrentEvent(int pnum,int ncard, int range_GTE);	// 해당 족보에 이벤트가 있는지 검사
	BOOL IsHaveCurrentEventEx(int pnum,char pcard[],int cardnum,int jokercard, int range_GTE); // 해당 족보에 이벤트가 있는지 검사

	void ClearAll();
	void set_total_Card(int PNum, int pCard[], int cardNum);
	void get_cal_rule();
	void get_cal_Winner_Client();
	bool Remain_card_Winner_Client(int i, int j, int &cnt, int &ncntlose, int &ncntwin, int &ncntequal);

	void get_cal_Winner_Server();
	bool Remain_card_Winner_Server(int i, int j, int &cnt);

	BOOL IsRemainCard(int pnum,int ncard);
	BOOL is_value_equal(); //동일패가 있는가?


	//로우 족보
	int get_MyRule_Low(int pnum);
	int get_Low_Winner(int rank=1);
	BOOL is_Low_value_equal(); //동일패가 있는가?	
	BOOL IsExtraCardWin_low(int pnum, int ncard);
	BOOL IsExtraCardLose_low(int pnum, int ncard);
	BOOL IsExtraCardEqual_low(int pnum, int ncard);
	BOOL IsLowRuleComplate(int pnum, int ncard);
	void get_cal_Low_Winner_Client();
	void get_cal_Low_Winner_Server();
	bool Remain_Low_card_Winner_Client(int i, int j, int &cnt, int &ncntlose, int &ncntwin, int &ncntequal);
	bool Remain_Low_card_Winner_Server(int i, int j, int &cnt);
};

#endif // !defined(AFX_SEVENRULEMAN_H__D2F5D281_61ED_4C31_98B5_1C53556A4D52__INCLUDED_)
