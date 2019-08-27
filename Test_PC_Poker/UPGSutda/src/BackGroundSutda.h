#pragma once


class CBackGroundSutda :
	public CBackGround
{
public:
	CBackGroundSutda(void);
	~CBackGroundSutda(void);


	virtual void OnEnterRoom(void);
	virtual void DrawBottom(NMBASE::GRAPHICGDI::CPage *pPage, CDC *pDC);
	virtual void DrawMiddle(NMBASE::GRAPHICGDI::CPage *pPage, CDC *pDC);


	virtual NMBASE::GRAPHICGDI::xSprite *GetBGSpr()
	{
		return &m_sprBack;
	}
};
