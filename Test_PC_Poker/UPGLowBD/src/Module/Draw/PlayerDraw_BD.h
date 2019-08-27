#pragma once

class CPlayer_BD;

class CPlayerDraw_BD : public CPlayerDraw
{
public:
	CPlayerDraw_BD( void );
	~CPlayerDraw_BD( void );

public:
	void Init( CPlayer *pPlayer, int nNum, NMBASE::GRAPHICGDI::CPage *pPage, CPlayerDrawManager *pManager );

	virtual void DrawBottom(CDC *pDC);
	virtual void DrawMiddle(CDC *pDC);
	virtual void DrawTop(CDC *pDC);
	
protected:
	
	//DrawBottom에서 그리는 함수들
	void Draw_HiddenOpenImage( CDC *pDC );
	void Draw_CutBackAndNumber( CDC* pDC );
	virtual void DrawJokboName(CDC *pDC);

	//DrawMiddle에서 그리는 함수들
	virtual void DrawBossMark(CDC *pDC);
	

	//DrawTop에서 그리는 함수들
	virtual void DrawResult(CDC *pDC);


	// 예외! 겹치는 문제로 별도로 그린다.
	void DrawCard( CDC* pDC );

protected:
	CPlayer_BD*		m_pPlayerBD;

	CPoint			m_ptDealer; // D 표시
	CPoint			m_ptCutNum;
	CPoint			m_ptTurn;
	CRect			m_rtstrJokbo;
	CRect			m_rtstrMoney;

	CPoint			m_ptHiddenKey;
	

private:

	friend class CPlayerDrawManager_BD;
};
