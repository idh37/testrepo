#pragma once

typedef struct  {
	int		nCard[5];
	int		nDiscardindex ;		//버린카드 인덱스
	int		nDiscardnum;		//버린카드 값
	int		nMouseOverIndex ;	//마우스 오버시 인덱스
	int	    nOPenCardIndex;
	int		nOPenCardNum;		//카드 넘버
	void Clear(){
		for(int i=0 ; i<5 ;i++ )
			nCard[i]=-1;

		nDiscardindex = -1;		//버린카드 인덱스
		nDiscardnum = -1;		//버린카드 값
		nMouseOverIndex = -1;	//마우스 오버시 인덱스
		nOPenCardIndex = -1;
		nOPenCardNum = -1;		//카드 넘버
	}
} SELECTCARDDATA;

class CUIInfoData;

class UPG_GAMEBASE_EXPORT CSelectCard
{
protected:
	CUIInfoData		*m_pUIInfoData;
	CGameViewBase	*m_pGameView;
	CGameProcess	*m_pGameProcess;
	int				m_nGameType;
	NMBASE::GRAPHICGDI::xSprite			m_sprSelectCard;
	NMBASE::GRAPHICGDI::xSprite			*m_pBigCardSpr;
	NMBASE::GRAPHICGDI::xSprite			*m_pBigDarkCardSpr;
	bool			m_bChoiceCard;

	SELECTCARDDATA	m_cChoiceCardDeck;

	NMBASE::GRAPHICGDI::CPage			*m_pPage;
	bool			m_bShow;
	int				m_nMaxNum;

	CRect			m_listCardRect[4];
	CPoint			m_ptStart;
public:
	CSelectCard();
	virtual ~CSelectCard();

	virtual void LoadImage(void);
	virtual void Init(CUIInfoData *pUIInfoData, CGame *pGame, NMBASE::GRAPHICGDI::xSprite *pBigCardSpr, NMBASE::GRAPHICGDI::xSprite *pBigDarkCardSpr);

	virtual void SetChoiceCard(bool bChoiceCard);
	virtual bool IsChoiceCard(void){return m_bChoiceCard;}
	virtual bool IsShow(void){return m_bShow;}
	virtual int GetDisCard(void){return m_cChoiceCardDeck.nDiscardnum;}

	virtual void Clear();
	virtual void Reset();
	virtual void ShowSelectCard(){m_bShow = TRUE;};
	virtual void DisableSelectCard();
	virtual void SetCard(int nGameType, char ncardnum[]);
	virtual void OnDraw( CDC *pDC  );
	virtual void OnGameKey( int nVKey, int nextended, int repeat = 0);
	virtual void OnMouseMove( int x, int y );
	virtual void OnLButtonDown( int x, int y );
	virtual void OnLButtonUp(int x, int y);
	virtual void OnSelectCard( int index );
	virtual void EndSelectCard(BOOL bautoselect = FALSE);

	//게임에서 구현해야할 부분
	virtual void OnEndSelectCard(BOOL bautoselect) = 0;
};