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


class CJokboProbability7Poker : public CJokboProbability
{
public:
	CJokboProbability7Poker();
	virtual ~CJokboProbability7Poker();

	void set_MyCard_Info( int no );
	void set_OtherCard_Info( int card[] , int total );
	void set_OtherCard_Info( int card );
	void set_RoundInfo( int round );
	int GetValueToIndex( int value );
	BOOL OnWhile( int start, int end ,int end_depth ,int cur_depth ,int cardno[]);

	virtual void SetAllUserCardInfo();
	virtual void CalculationProbability();
	virtual int GetMyCurJokboIndex();
	virtual float GetProbability(int nJokbo);
};