// NMovingCardMan.h: interface for the CNMovingCardMan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NMOVINGCARDMAN_H__55BC776D_B1A0_4ABD_A5B5_1110E79F7462__INCLUDED_)
#define AFX_NMOVINGCARDMAN_H__55BC776D_B1A0_4ABD_A5B5_1110E79F7462__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "NMovingCard.h"

class CNMovingCardMan  
{
public:
	CNMovingCardMan();
	virtual ~CNMovingCardMan();

	std::list< CNMovingCard* >	m_cMoveCardList;

	void Destroy();
	void ADDCard( CNMovingCard *pData, bool bLastCommandDelete = false );
	void ADDCardF4( CNMovingCard *pData ); // √≥¿Ω 4¿Â
	void Draw(CDC* pDC, NMBASE::GRAPHICGDI::CPage *pPage);
	void Process();
	bool IsCardMoving();
	bool IsCardMoving(int nPNum );
};


#endif // !defined(AFX_NMOVINGCARDMAN_H__55BC776D_B1A0_4ABD_A5B5_1110E79F7462__INCLUDED_)
