#pragma once

class CPlayerDraw_HL :
	public CPlayerDraw
{


	bool							 m_bSelectedHLS;					// 하이/로우/스윙 선택 완료 했는지
	NMBASE::SKINGDI::CNText			*m_pLowJokboName;					// 로우족보이름 (내족보 표시)

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
