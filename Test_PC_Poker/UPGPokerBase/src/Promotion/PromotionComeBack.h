#pragma once

#include "PromotionBase.h"
#include <EventCommon.h>
#include "PromotionComeBackDlg.h"


class UPG_GAMEBASE_EXPORT CPromotionComeback : public CPromotionBase
{
public:
	CPromotionComeback( CPromotionManager *pManager, int nType );
	virtual ~CPromotionComeback(void);

protected:
	NMBASE::GRAPHICGDI::xSprite	m_sprBack;					//¹è°æ

public:

	virtual void DrawBottom(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage);
	virtual void DrawMiddle(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage);
	virtual void DrawTop(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage);
	

	void OpenComeBackDlg(void);
	virtual void Accept_OutRoom();
	virtual void Accept_CreateRoom();
	virtual void Accept_EnterRoom();

	virtual BOOL OnLButtonDownMiddle(int &x , int &y);
	virtual BOOL OnLButtonUpMiddle(int &x , int &y);
	virtual BOOL OnMouseMoveMiddle(int &x , int &y);

	virtual void DrawEventTab(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage, const CPoint &ptStart);
	virtual BOOL OnPacketNotify(char* pName, int &Signal, int &TotSize, char *lpData);

};
