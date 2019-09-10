#pragma once

class CPlayerDraw_HL :
	public CPlayerDraw
{


	bool							 m_bSelectedHLS;					// ����/�ο�/���� ���� �Ϸ� �ߴ���
	NMBASE::SKINGDI::CNText			*m_pLowJokboName;					// �ο������̸� (������ ǥ��)

public:
	CPlayerDraw_HL(void);
	~CPlayerDraw_HL(void);


	virtual void Reset(void);
	virtual void Init(CPlayer *pPlayer, int nNum, NMBASE::GRAPHICGDI::CPage *pPage, CPlayerDrawManager *pManager);
	virtual void InitUIData(void);

	virtual void DrawJokboName(CDC *pDC);
	virtual void UpdateJokboName();

	virtual void DrawResult(CDC *pDC);
	virtual void DrawTimer(CDC *pDC);

	virtual void SetSelHLS(bool bSel)  { m_bSelectedHLS = bSel; }
	


};
