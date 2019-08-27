#pragma once

class CGameProcessSutda;
class CGameViewSutda;
class CSelectCardSutda 
{
protected:

	CUIInfoData		*m_pUIInfoData;
	CGameViewSutda	*m_pGameViewSutda;
	CGameProcessSutda	*m_pGameProcessSutda;

	NMBASE::GRAPHICGDI::CPage			*m_pPage;
	bool			m_bShow;
	

	NMBASE::GRAPHICGDI::xSprite			m_sprSelectCard;
	NMBASE::GRAPHICGDI::xSprite			*m_pBigCardSpr;
	NMBASE::GRAPHICGDI::xSprite			*m_pBigDarkCardSpr;


	//뉴족보선택
	BOOL m_bChoiceCard;
	int  m_nChoiceCardCnt;
	int  m_nCurSelectCard;	
	BOOL m_bChoiceBtnFlag;

	//뉴족보 선택시 렉트
	CRect m_ChoiceCardRect1, m_ChoiceCardRect2, m_ChoiceCardRect3;			//mousemove용 랙터 
	CRect m_ChoiceJokboRect;												//족보 텍스트로 찍어줌
	CString m_strChoiceJokbo1, m_strChoiceJokbo2, m_strChoiceJokbo3;			//족보 텍스트 1, 2, 3 

	int m_HighSelCardIndex;		//높은 족보의 위치  (카드 선택 다이얼로그에서 쓰인다) 값: (0,1,2)

	int m_nCard1, m_nCard2, m_nCard3;
public:
	CSelectCardSutda();
	virtual ~CSelectCardSutda();

	//virtual void OnEndSelectCard(BOOL bautoselect);
	static CSelectCardSutda &GetInstance();


	virtual void LoadImage(void);
	virtual void Init(CUIInfoData *pUIInfoData, CGame *pGame, NMBASE::GRAPHICGDI::xSprite *pBigCardSpr, NMBASE::GRAPHICGDI::xSprite *pBigDarkCardSpr);

	virtual void SetChoiceCard(bool bChoiceCard);
	virtual BOOL IsChoiceCard(void){return m_bChoiceCard;}
	
	virtual bool IsShow(void){return m_bShow;}	

	virtual void Clear();
	virtual void Reset();
	virtual void ShowSelectCard();
	virtual void DisableSelectCard();
	virtual void SetCard(int nCard1, int nCard2, int nCard3);
	virtual void OnDraw( CDC *pDC  );
	virtual void OnGameKey( int nVKey, int nextended, int repeat = 0);
	virtual BOOL OnMouseMove( int x, int y );
	virtual BOOL OnLButtonDown( int x, int y );
	virtual BOOL OnLButtonUp(int x, int y);
	virtual void OnSelectCard( int index );
	virtual void EndSelectCard(BOOL bautoselect = FALSE);	
	virtual void OnTimer();




	//카드 선택(3장섯다)
	void ChoiceJokboInit();
	void ChoiceJokboEnd();	
	void DoSelectCard( int index );
	void SetHighSelCardIndex(int nIndex) { m_HighSelCardIndex = nIndex; }
	int GetHighSelCardIndex() { return m_HighSelCardIndex; }


};

CSelectCardSutda &g_SelectCard();