#pragma once

typedef struct  {
	int		nCard[5];
	int		nDiscardindex ;		//����ī�� �ε���
	int		nDiscardnum;		//����ī�� ��
	int		nMouseOverIndex ;	//���콺 ������ �ε���
	int	    nOPenCardIndex;
	int		nOPenCardNum;		//ī�� �ѹ�
	void Clear(){
		for(int i=0 ; i<5 ;i++ )
			nCard[i]=-1;

		nDiscardindex = -1;		//����ī�� �ε���
		nDiscardnum = -1;		//����ī�� ��
		nMouseOverIndex = -1;	//���콺 ������ �ε���
		nOPenCardIndex = -1;
		nOPenCardNum = -1;		//ī�� �ѹ�
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

	//���ӿ��� �����ؾ��� �κ�
	virtual void OnEndSelectCard(BOOL bautoselect) = 0;
};