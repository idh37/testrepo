#pragma once



class CAskRematch
{
	//뉴재대결참여
	BOOL m_bAsk_Rematch;	
	int  m_nAsk_RematchCnt;
	INT64 m_llPayment;	


	NMBASE::GRAPHICGDI::xSprite m_YesBtnSpr, m_NoBtnSpr, m_RematchGameDlgSpr; 

	NMBASE::GRAPHICGDI::CPage *m_pPage;
	NMBASE::SKINGDI::CGraphButton m_BtnAccept_Rematch;	//재대결참가
	NMBASE::SKINGDI::CGraphButton m_BtnRefusal_Rematch;	//재대결거부


public:
	CAskRematch(void);
	~CAskRematch(void);


	void Init(CWnd *pParent, NMBASE::GRAPHICGDI::CPage *pPage);
	void Show(INT64 llPayment);
	void Hide();
	void DoAskRematch(bool bAcceptToJoin);
	
	void OnTimer();
	void Draw(CDC *pDC);

	void LoadImage();

	BOOL OnMouseMove(int x, int y);
	BOOL OnLButtonUp(int x, int y);
	BOOL OnLButtonDown(int x, int y);
	BOOL OnGameKey( int nVKey, int nextended, int repeat );
};
