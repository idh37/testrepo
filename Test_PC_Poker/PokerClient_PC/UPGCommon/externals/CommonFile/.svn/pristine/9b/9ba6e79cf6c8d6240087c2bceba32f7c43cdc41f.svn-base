// BadugiRule.h: interface for the CBadugiRule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BADUGIRULE_H__DD90F0E2_A1F2_4E4D_B309_F8366B450838__INCLUDED_)
#define AFX_BADUGIRULE_H__DD90F0E2_A1F2_4E4D_B309_F8366B450838__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum {
		BD_EMPTY_VALUE= -1,
		BD_EMPTY_CARD = -1,
		BD_TOTAL_CARD = 5,
		MAX_PATTERN = 4,		
		BD_TOTAL_USER = 5,
	};

enum {
	GOLF = 46,
	SECOND = 45,
	THIRD =	44,
};

#define SEED_NUMBERPOINT 14

typedef struct {
	int pattern;
	int cardnumber;	
}MyCard;

typedef struct {	
	int cardnumber[BD_TOTAL_CARD];	//숫자 정보
	int numberpoint[BD_TOTAL_CARD];	//숫자 정보
	int curcardnum;		//현재 같은 패턴에 숫자가 몇개 있다.
	int pattern;
}CardInfo;

typedef struct  {
	int Card[5];		// 1~13까지
	int RealCard[5];	// 0~51까지	
	int numberpoint;
	int m_RuleCompleteCnt;
}CompleteCard;


class CBadugiRule  
{
public:
	CBadugiRule();
	virtual ~CBadugiRule();

	enum BADUGI_CARD {
			B4C_TOP=0,
			B4C_GOLF,
			B4C_SECOND,
			B4C_THIRD,
			B4C_MADE,
			B4C_BASE,
			B4C_TWO_BASE,
	};

	int m_Card[BD_TOTAL_CARD];			//처음 셋팅된 카드 값
	int m_RealSortCard[BD_TOTAL_CARD];	//정렬만 한 카드덱	
	CompleteCard m_RuleCompleteCard;	//족보를 이룬 카드 
	CardInfo m_sortCard[MAX_PATTERN];

	int PNum;							//넘버
	int m_nTotalCard;					// 총 카드수
	int	m_MyValue;						// 현재 내 족보 0 탑 1 골프 2 세컨 3 서드 4 메이드 5 베이스 6 투베이스 
	int m_nGameMode;					//0 이면 4장 1이면 5장	
	BOOL m_bFold;						//FALSE 게임중 TRUE 기권
	CString m_strMyValueName;

	void Init(int mode=0);
	void Clear();
	
	void SetCard(int pnum, int pCard[],int CardNum);			//족보를 구하기 위해 카드 셋팅
	void sort_Card(int Start, int End);							// 카드정렬	
	void GetMyRule();					//족보 구하기
	
	CString GetNumString(int num = -1);	
	CString GetValueName();			//내 족보 이름 구하기
	CString GetValueNumString();	//족보를 이룬 숫자들 만 리턴
	CString GetFullValueName();		//GetValueNumString() + GetValueName() 족보

	BOOL Made_4();						//메이드 구하기 
	BOOL Made_5();						//메이드 구하기 
	BOOL Base();						//베이스 구하기
	BOOL Base_5();						//베이스 구하기
	BOOL TwoBase();						//투베이스 구하기	
	void Top();							//탑 구하기

	void Result_Sort();

	int m_MyRanking;								//나의 등수
	void SetRanking(int rank){m_MyRanking =rank;}	//등수 저장
	int  GetRanking(){return m_MyRanking;}			//등수 저장	

	void SetFold(){m_bFold = TRUE;};
	void SetGameMode(int mode){m_nGameMode = mode;};
	int	 GetMyValueTopNum();
};

#endif // !defined(AFX_BADUGIRULE_H__DD90F0E2_A1F2_4E4D_B309_F8366B450838__INCLUDED_)
