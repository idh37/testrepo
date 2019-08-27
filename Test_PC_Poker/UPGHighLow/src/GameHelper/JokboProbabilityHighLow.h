#pragma once

enum enum_CARD_VALUE
{
	en_None = 0,
	en_One,
	en_Two,
	en_Three,
	en_Straight,
	en_BackStraight,
	en_Mount,
	en_Flush,
	en_FullHouse,
	en_FourCard,
	en_SF,	
	en_RSF,
	en_Max,
};

enum enum_CARD_VALUE_LOW
{
	en_Low_None = 0,	
	en_K_Top,
	en_Q_Top,
	en_J_Top,
	en_10_Top,
	en_9_Top,
	en_8_Top,
	en_7_Top,
	en_6_Top,
	en_5_Top,	
	en_Low_Max,
};

class CJokboProbabilityHighLow : public CJokboProbability
{

protected:
	PRO_VALUE	m_ProValue_low;
	PRO_VALUE	m_ProValue_CntDeck_low;

public:
	CJokboProbabilityHighLow();
	virtual ~CJokboProbabilityHighLow();

	void set_MyCard_Info( int no );
	void set_OtherCard_Info( int card[] , int total );
	void set_OtherCard_Info( int card );
	void set_RoundInfo( int round );
	int GetValueToIndex( int value );
	int get_value_Index_Low( int value );
	BOOL OnWhile( int start, int end ,int end_depth ,int cur_depth ,int cardno[]);

	int GetHighJokboIndex(int nJokbo);	//하이족보 인덱스로 변환?
	int GetLowJokboIndex(int nJokbo);	//로우족보 인덱스로 변환?


	virtual void Init(int nMaxPlayer, int nMaxValue);
	virtual void Clear();
	virtual void Reset();
	virtual void SetAllUserCardInfo();
	virtual void CalculationProbability();
	virtual int GetMyCurJokboIndex();
	virtual int GetMyCurJokboIndex_low();

	virtual float GetProbability(int nJokbo);
};