#pragma once

class CUIInfoData;

class CBackGround_BD : public CBackGround
{
public:
	CBackGround_BD();
	virtual ~CBackGround_BD();

	virtual void DrawBottom(NMBASE::GRAPHICGDI::CPage *pPage, CDC *pDC);
	virtual void DrawMiddle(NMBASE::GRAPHICGDI::CPage *pPage, CDC *pDC);
};


