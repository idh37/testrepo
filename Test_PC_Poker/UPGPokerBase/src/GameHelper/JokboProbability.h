#pragma once

#define  MAX_CARDNUM 10
#define  MAX_DUMMY 52

class UPG_GAMEBASE_EXPORT PRO_VALUE
{
protected:
	double	*m_plistProValue;
	int		m_nTotal;
	int		m_nMaxValue;
public:
	PRO_VALUE();
	virtual ~PRO_VALUE();

	void Init(int nMaxValue);

	inline	int GetTotal(void){return m_nTotal;}

	void set_Data( int index );
	void set_Data( int index , double pro );
	double get_Data( int index );
	void Clear();
};


class UPG_GAMEBASE_EXPORT PRO_CARD  
{
protected:
	int m_listCardNo[MAX_DUMMY];
	int m_nTotal;

public:
	PRO_CARD();
	virtual ~PRO_CARD();

	inline	int GetTotal(void){return m_nTotal;}

	void	Clear();
	void	set_CardNo( int no );
	BOOL	IsSame_CardNo( int no );
	int		get_CardNo( int index );
};

enum enum_ROUND
{
	en_No_None = 3,	
	en_5_Round = 5,
	en_6_Round = 6,
	en_7_Round = 7,
	en_8_Round = 8,
	en_9_Round = 9,
	en_10_Round = 10,
};

class UPG_GAMEBASE_EXPORT CJokboProbability
{
protected:
	PRO_VALUE	m_cProValue;
	PRO_VALUE	m_cProValue_CntDeck;

	PRO_CARD	m_cMyCardInfo;
	PRO_CARD	m_cOtherCardInfo;
	PRO_CARD	m_cCardDeck;

	int			m_nCurRound;
	int			m_nCompareCnt;
	int			m_nMaxPlayer;
public:
	CJokboProbability(void);
	virtual ~CJokboProbability(void);

	virtual void Init(int nMaxPlayer, int nMaxValue);
	virtual void Clear();
	virtual void Reset();
	virtual void SetAllUserCardInfo() = 0;
	virtual void CalculationProbability() = 0;
	virtual int GetMyCurJokboIndex() = 0;
	virtual float GetProbability(int nJokbo) = 0;
};