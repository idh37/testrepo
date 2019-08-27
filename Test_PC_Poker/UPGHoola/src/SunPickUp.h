// SunPickUp.h: interface for the CSunPickUp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUNPICKUP_H__DA1125B9_6102_481C_9C54_969A39BBFF8E__INCLUDED_)
#define AFX_SUNPICKUP_H__DA1125B9_6102_481C_9C54_969A39BBFF8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graphic.h"
#include "UIInfoDataHoola.h"

#define MAX_FIRSTSELECTCARD_NUM 9

typedef struct
{
	int   m_SelectPnum;
	int   m_CardNo;
	POINT m_ptSelectCard;	//카드그릴위치
} tSUNPICKUPINFO;

class CSunPickUp  
{
public:
	CSunPickUp();
	virtual ~CSunPickUp();

	bool m_bPickUp;
	bool m_bCardSelected;

	int m_nFirstPlayer;

	tSUNPICKUPINFO m_SunPickup_Info[ MAX_FIRSTSELECTCARD_NUM ];

	CRect m_rtSelectCard[ MAX_FIRSTSELECTCARD_NUM ];

	void Reset();
	void Draw( NMBASE::GRAPHICGDI::CPage *pPage );
	void InitSelectCardPos();
	void StartInfo( int cardnum );
	void PickupUser( int cardindex, int cardno, int pnum );
	void PickUpComplete( int winpnum );

	BOOL OnLButtonUp( int xp, int yp );
	
	POINT GetSelectCardPos( int nindex );	
};

#endif // !defined(AFX_SUNPICKUP_H__DA1125B9_6102_481C_9C54_969A39BBFF8E__INCLUDED_)
