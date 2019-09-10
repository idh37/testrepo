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
	
	//DrawBottom���� �׸��� �Լ���
	void Draw_HiddenOpenImage( CDC *pDC );
	void Draw_CutBackAndNumber( CDC* pDC );
	virtual void DrawJokboName(CDC *pDC);

	//DrawMiddle���� �׸��� �Լ���
	virtual void DrawBossMark(CDC *pDC);
	

	//DrawTop���� �׸��� �Լ���
	virtual void DrawResult(CDC *pDC);


	// ����! ��ġ�� ������ ������ �׸���.
	void DrawCard( CDC* pDC );

protected:
	CPlayer_BD*		m_pPlayerBD;

	CPoint			m_ptDealer; // D ǥ��
	CPoint			m_ptCutNum;
	CPoint			m_ptTurn;
	CRect			m_rtstrJokbo;
	CRect			m_rtstrMoney;

	CPoint			m_ptHiddenKey;
	

private:

	friend class CPlayerDrawManager_BD;
};
