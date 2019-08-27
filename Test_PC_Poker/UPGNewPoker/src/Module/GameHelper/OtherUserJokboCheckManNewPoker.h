#pragma once

#define MAX_TOTAL_CARDNUM 10
#define MAX_POSSIBLE_RULE 9
#define MAX_POSSIBLE_PLAYER 5

#define ACE_CARD 14				// Ace
#define ONEPATTERN_TOTAL 13		//한 문늬 총 13장
#define C_NUMBER_A 12			//크로바 Ace
#define H_NUMBER_A 25
#define D_NUMBER_A 38
#define S_NUMBER_A 51			//스페이드 Ace

#define C_PATTERN 0				//크로바 
#define H_PATTERN 1
#define D_PATTERN 2
#define S_PATTERN 3				//스페이드 

#define MAXPLAYER 5

#define CARD_GAP 7	//카드 간 

enum POSSIBLE_VALUE{
	en_Value_None = -1,
	en_Value_One,
	en_Value_Two,
	en_Value_Three,
	en_Value_Straight,
	en_Value_Flush,
	en_Value_House,
	en_Value_Four,
	en_Value_St_Flush,
	en_Value_Rst_Flush,
	en_Value_Max,
};

enum POSSIBLE_CARD_COLOR{
	en_Value_Color_None = -1,
	en_Value_Color_Red = 0,	//족보 완성
	en_Value_Color_Green,   //스트레이트 경우
	en_Value_Color_Blue,	//플러쉬 경우 ( 스트레이트 플러쉬도 블루에 포함)
};

struct COMPARE_CARD 
{
	int m_nTotal;
	int m_listCardNo[MAX_TOTAL_CARDNUM];
	int m_listCardPa[MAX_TOTAL_CARDNUM];

	COMPARE_CARD& operator = (COMPARE_CARD p ){
		Clear();
		memcpy( m_listCardNo , p.m_listCardNo , sizeof(m_listCardNo) );
		memcpy( m_listCardPa , p.m_listCardPa , sizeof(m_listCardPa) );
		m_nTotal = p.m_nTotal;

		return (*this);
	}

	void Clear(){
		m_nTotal = 0;
		memset(m_listCardNo,-1,sizeof(m_listCardNo));	
		memset(m_listCardPa,-1,sizeof(m_listCardPa));	
	}

	bool set_BaseCardNo( int basecardno){
		if( m_nTotal < 0 )return false;

		for ( int i = 0 ; i < m_nTotal ; i ++ ){
			if( m_listCardNo[i] == basecardno ){
				return false;
			}
		}

		m_listCardNo[m_nTotal] = basecardno;
		m_nTotal++;
		return true;
	}

	bool set_StraightCardNumber( int number, int pattern , bool bCompare = true){

		for ( int i = 0 ; i < m_nTotal ; i ++ ){
			if( m_listCardNo[i] == number ){
				return false;
			}
		}

		m_listCardNo[m_nTotal] = number;
		m_listCardPa[m_nTotal] = pattern;
		m_nTotal++;
		return true;
	}

	void set_UniCardNo( int number, int pattern ){
		//
		const int None = -1;
		int nSameIndex = None;
		for ( int i = 0 ; i < m_nTotal ; i ++ ){
			if( m_listCardNo[i] == number ){
				nSameIndex = i;
				break;
			}
		}

		if( nSameIndex == None ){
			m_listCardNo[m_nTotal] = number;
			m_listCardPa[m_nTotal] = pattern;
			m_nTotal++;
		}
		else{
			//같은 숫자일때 검사 해서 높은 숫자를 넣어준다.
			int No_1 = ( m_listCardPa[nSameIndex] * ONEPATTERN_TOTAL ) + m_listCardNo[nSameIndex] - 2;
			int No_2 = ( pattern * ONEPATTERN_TOTAL ) + number - 2;

			if( No_2 > No_1 ){
				m_listCardNo[nSameIndex] = number;
				m_listCardPa[nSameIndex] = pattern;
			}
		}
	}

	int get_CardNumber( int index ){
		if( m_nTotal <= 0 )return false;

		int number = m_listCardNo[index] == 1 ? 14 : m_listCardNo[index] ;		
		number = (ONEPATTERN_TOTAL* m_listCardPa[index] ) + number - 2;
		return number;
	}

	int get_LastCardNo(){
		if( m_nTotal <= 0 )return -1;
		return m_listCardNo[m_nTotal-1];
	}

	bool ADD_Ace_Number(){
		if( m_nTotal <= 0 ){
			return false;
		}

		int i = 0 ;
		if( m_listCardNo[ m_nTotal-1 ] == ACE_CARD ){

			for( i = m_nTotal - 1  ; i >= 0; i -- ){
				m_listCardNo[i+1] = m_listCardNo[i];
				m_listCardPa[i+1] = m_listCardPa[i];
			}

			m_listCardNo[0] = 1;
			m_listCardPa[0] = m_listCardPa[m_nTotal];
			m_nTotal += 1;

			return true;
		}

		return false;
	}
};

typedef struct  
{
	int m_nTotalCardNum;
	int m_listPossibleCard[MAX_TOTAL_CARDNUM];
	int m_listNumBer[MAX_TOTAL_CARDNUM];
	int m_listPattern[MAX_TOTAL_CARDNUM];

	void Clear(){
		m_nTotalCardNum = 0;

		for( int i = 0 ; i < MAX_TOTAL_CARDNUM ; i ++ ){
			m_listPossibleCard[i] = -1;
			m_listNumBer[i] = -1;
			m_listPattern[i] = -1;
		}
	}	

	void set_CardInfo( int total , int card[] ){

		Clear();
		if( total <= 0 ){			
			return;
		}
		m_nTotalCardNum = total;

		for( int i = 0 ; i < total ; i ++ ){
			m_listPossibleCard[i] = card[i];

			int nCard = m_listPossibleCard[i];

			if( nCard == -1 )continue;

			if(nCard== C_NUMBER_A || nCard == H_NUMBER_A || nCard == D_NUMBER_A || nCard == S_NUMBER_A )  
				m_listNumBer[i] = ACE_CARD;
			else
				m_listNumBer[i] = nCard -  (ONEPATTERN_TOTAL*(nCard/ONEPATTERN_TOTAL))+2 ;

			if(      nCard <= C_NUMBER_A ) 
				m_listPattern[i] = C_PATTERN;//클로바.
			else if( nCard <= H_NUMBER_A ) 
				m_listPattern[i] = H_PATTERN;//하트.
			else if( nCard <= D_NUMBER_A ) 
				m_listPattern[i] = D_PATTERN;//다이아.
			else if( nCard <= S_NUMBER_A ) 
				m_listPattern[i] = S_PATTERN;//스페이드.	
		}

		BubbleSort();
	}

	bool BubbleSort(){
		if( m_nTotalCardNum <= 0 )return false;

		int i=0, j=0, temp=-1,pic=-1;

		for(i = 0; i < m_nTotalCardNum-1; i++){
			if( m_listPossibleCard[i] == -1 )continue;
			for(j = i+1; j < m_nTotalCardNum; j++){

				if( m_listNumBer[j] == -1 )continue;
				if(m_listNumBer[i] > m_listNumBer[j]){
					temp = m_listNumBer[i];//카드 정렬하구...
					m_listNumBer[i] = m_listNumBer[j];
					m_listNumBer[j] = temp;

					pic  = m_listPattern[i];//무늬는 따라간다.
					m_listPattern[i] = m_listPattern[j];
					m_listPattern[j] = pic;
				}
				else if(m_listNumBer[i] == m_listNumBer[j]){ //숫자가 같다면,
					if(m_listPattern[i] > m_listPattern[j]){
						int t=0;
						t    = m_listPattern[i];//무늬 정렬한다....
						m_listPattern[i] = m_listPattern[j];
						m_listPattern[j] = t;
					}
				}
			}
		}

		return true;
	}

	//같은 카드가 잇는지 
	bool IsSameCard( int CardNo ){
		if( CardNo < 0 || CardNo > 51 )return false;

		for( int i = 0 ; i < m_nTotalCardNum ; i ++ ){
			if( m_listPossibleCard[i] == CardNo ){
				return true;
			}
		}
		return false;
	}

	int get_Number( int index ){
		if( index < 0 || index >= m_nTotalCardNum )return -1;

		return m_listNumBer[index];
	}

	int get_Pattern( int index ){
		if( index < 0 || index >= m_nTotalCardNum )return -1;

		return m_listPattern[index];
	}

	int get_BaseCardNo(int index ){
		if( index < 0 || index >= m_nTotalCardNum )return -1;		

		return ( m_listPattern[index] * ONEPATTERN_TOTAL ) + m_listNumBer[index] - 2;
	}

}POSSIBLE_CARD_INFO;

typedef struct  
{
	POSSIBLE_CARD_INFO m_CardInfo;

	void Clear(){
		m_CardInfo.Clear();
	}	

	void set_Possible_CardInfo( int total , int card[] ){
		if( total <= 0 )return;

		m_CardInfo.set_CardInfo( total, card );		
	}

	int get_TotalCardNum(){
		return m_CardInfo.m_nTotalCardNum;
	}

	int* get_PossibleCardNum(){
		return m_CardInfo.m_listPossibleCard;
	}

	int get_PossibleCardNum( int index ){
		if( index < 0 || index >= m_CardInfo.m_nTotalCardNum )return -1;
		return m_CardInfo.m_listPossibleCard[index];
	}

	//같은 카드가 잇는지 
	bool IsSameCard( int CardNo ){
		if( CardNo < 0 || CardNo > 51 )return false;
		return m_CardInfo.IsSameCard( CardNo );		
	}

}POSSIBLE_RULE;

typedef struct
{
	POSSIBLE_CARD_INFO	m_cBaseCardInfo;	
	POSSIBLE_RULE		m_listPossibleCard[MAX_POSSIBLE_RULE];

	void Clear(){
		m_cBaseCardInfo.Clear();			
		int i = 0 ;		
		for( i = 0 ; i < MAX_POSSIBLE_RULE ; i ++ ){
			m_listPossibleCard[i].Clear();
		}
	}

	void Clear_SelectValue( POSSIBLE_VALUE ps_value ){
		if( ps_value <= en_Value_None || ps_value > en_Value_Rst_Flush )return;
		m_listPossibleCard[ps_value].Clear();
	}

	POSSIBLE_CARD_COLOR IsMade( POSSIBLE_VALUE ps_value );
	bool get_MadeCardInfo( POSSIBLE_VALUE &ps_value , POSSIBLE_CARD_COLOR &value_color);

	void set_Possible_CardInfo(POSSIBLE_VALUE ps_value , int total , int card[] );
	int get_TotalCardNum(POSSIBLE_VALUE ps_value);

	int* get_PossibleCardNum(POSSIBLE_VALUE ps_value);
	int get_PossibleCardNum(POSSIBLE_VALUE ps_value , int index );
	bool IsSameCard( POSSIBLE_VALUE ps_value , int CardNo );
}POSSIBLE_RULE_USER;

class COtherUserJokboCheckManNewPoker : public COtherUserJokboCheckMan
{
protected:
	bool				m_bUse;
	bool				m_bShow;

	POSSIBLE_RULE_USER	m_Other_User[MAX_POSSIBLE_PLAYER];
public:
	COtherUserJokboCheckManNewPoker();
	virtual ~COtherUserJokboCheckManNewPoker();

	bool set_Use(bool use){m_bUse = use;}
	bool set_Show(bool show ){m_bShow = show;}
	bool IsUse(){return m_bUse;}
	bool IsValid(){return m_bShow && m_bUse;}

	bool get_Pair( int pnum );
	bool get_One( int pnum);
	bool get_Two( int pnum );
	bool get_Three( int pnum );
	bool get_Straight( int pnum );
	bool get_Flush( int pnum );
	bool get_House( int pnum );
	bool get_Four( int pnum );
	bool get_St_Flush( int pnum );
	bool get_Rst_Flush( int pnum );	

	virtual void Init(NMBASE::GRAPHICGDI::xSprite *pArrowSpr);
	virtual void Clear(void);
	virtual void Draw(CDC *pDC , NMBASE::GRAPHICGDI::CPage *pPage);
	virtual void CheckJokbo(void);
};