// CardRule.h: interface for the CCardRule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CARDRULE_H__286FF389_2D26_4A3D_AFFB_592B77B5DE7D__INCLUDED_)
#define AFX_CARDRULE_H__286FF389_2D26_4A3D_AFFB_592B77B5DE7D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCardRule  
{
public:
	enum SEVEN_CARD {
			N7C_DUMMY=0,
			// 망통.
			N7C_NOPAIR,
			// 원페어.
			N7C_ONEPAIR,
			// 투페어.
			N7C_TWOPAIR,
			// 쓰리카드.
			N7C_TRIPLE,
			// 스트레이트
			N7C_STRAIGHT,
			N7C_BACKS,  //백스트레이트
			N7C_MOUNT,  // 마운틴
			// 플러쉬.
			N7C_FLUSH,
			// 풀하우스
			N7C_TITLE,
			// 포카드
			N7C_QUADS,
			// 스트레이트 플러쉬.
			N7C_SFULSH,
			N7C_BSFLUSH, //백스트레이트 플러시.
			// 로얄스트레이트 플러시.
			N7C_CROYAL,
			N7C_HROYAL,
			N7C_DROYAL,
			N7C_SROYAL   
	};

	// 결과
	typedef struct 
	{
		int Royal;
		int BackStrFsh;
		int StrFsh;
		int Poker[2];
		int FullH[2];
		int Fsh;
		int MStraight;
		int BStraight;
		int Straight;
		int Trip[2];
		int Onep[4];
		int mang;
	
	} ST_RULE_DATA;

	enum {
		EMPTY_VALUE= -1,
		EMPTY_CARD = -1,
		TOTAL_CARD = 10,
	};

//private:
public:	
	ST_RULE_DATA m_stRuleData;			//  룰

	int m_sortCard[ TOTAL_CARD ];		// 카드정렬 결과
	int m_RemainCard[TOTAL_CARD];	    // 족보를 제외한 카드    
	int m_RuleComplteCard[TOTAL_CARD];  // 족보를 이루는 카드	
	int m_RealCard[TOTAL_CARD];			// 실제 카드값

	//로우 족보를 위한 배열 
	int m_sortCard_low[ TOTAL_CARD ];		// 카드정렬 결과
	int m_RuleComplteCard_Low[TOTAL_CARD];  // 족보를 이루는 카드
	int m_RemainCard_Low[TOTAL_CARD];	    // 족보를 제외한 카드    
	int m_Extra_WinCard_Low[3][TOTAL_CARD]; // 0 : 하이 1 : extra 2: equal

	/* 클라이언트용 */
	int m_Extra_WinCard[3][TOTAL_CARD]; // 0 : 하이 1 : extra 2: equal

	/* 이벤트용  모든 족보카드 */
	int m_AllRuleCard[14][TOTAL_CARD];

	/*동일 족보일때 5장을 담고 있는다. 함수 호출시에 카드덱에 넣는다.*/
	int m_EqualCard[TOTAL_CARD]; 

	/*동일 족보일때 5장을 담고 있는다. 함수 호출시에 카드덱에 넣는다.*/
	int m_EqualCard_low[TOTAL_CARD]; 

	int m_nTotalCard;					// 총 카드수
	int m_SevenValue;					// 족보
	int m_SecondSevenValue;				// 로티플이상에서만 사용
	int m_SevenValue_low;				// 로우 족보
	BOOL bValid;

public:
	CCardRule();
	virtual ~CCardRule();

public:

	void Init();
	inline BOOL isValid() const { return bValid; }				// 유효한 사용자
	void   set_Init_Valid() { bValid = FALSE; }					// 사용자 무효화
	inline int is_Card_pattern(int &cnum) { return cnum/13;}	// 카드무뉘
	
	void cal_Card();											// 족보검사
	void cal_pair_class();
	void cal_flush_class();
	void cal_straight_class();
	

	void sort_Card(int Start, int End);							// 카드정렬	
	void set_Card(int pCard[], int cardNum);					// 카드셋
	int get_TotalCard() const { return m_nTotalCard; }			
	int get_sevenValue( bool bUseLog = false ) const 
	{
		if ( bUseLog )
		{
			return __max( m_SevenValue, m_SecondSevenValue ); // 로티플 이상에서 정확한 값을 리턴한다( 기존은 클로버로티플 값으로 리턴 )
		}

		return m_SevenValue; 
	}
	
	void get_cal_sevenRule();
	void get_cal_AllsevenRule();                                // 이벤트용 모든룰 족보 구하기
	
	CString get_RuleValue_to_Name() ;
	CString get_RuleValue_to_Name2();
	CString get_RuleValue_to_Name3();							//win 결과 창 족보
	CString get_RuleValue_to_Name_HaveNumber();							//[1,2,3,4,5]식의 족보 

	int	get_ramain_card_number(); //족보를 이루지 못한 카드 장수	
	int get_rule_complete_card_number(); //족보를 이루는 카드 장수 


	//로우 족보 
	void sort_Card_low(int Start, int End);							// 카드정렬	
	void get_cal_sevenLowRule();
	CString get_LowRuleValue_to_Name();							//로우 족보 이름 가지고 오기
	int get_sevenLowValue() const { return m_SevenValue_low; }
	int get_rule_complete_Low_card_number(); //족보를 이루는 카드 장수 
	CString get_Low_RuleValue_to_Name_HaveNumber();							//[1,2,3,4,5]식의 족보 	

	//종료시 로그 남기기
	CString get_RuleValue_to_Name_for_LogDB(int nOneCard);

	//동일 족보 
	int get_Second_Value();
	BOOL is_value_equal(int wincase); //동일패가 있는가?
	void set_EqualCard(); //동일 족보 5장을 담아 둔다. 원페어 투페어 때문

	//동일 족보 로우
	void set_Low_EqualCard(); //동일 족보 5장을 담아 둔다.
	BOOL is_Low_value_equal(int wincase); //동일패가 있는가?
};

#endif // !defined(AFX_CARDRULE_H__286FF389_2D26_4A3D_AFFB_592B77B5DE7D__INCLUDED_)
