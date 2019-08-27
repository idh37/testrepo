// AniSprListMan.h: interface for the CAniSprListMan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANISPRLISTMAN_H__8E1751AB_EAD1_4BB7_BDAC_2100CD18B00E__INCLUDED_)
#define AFX_ANISPRLISTMAN_H__8E1751AB_EAD1_4BB7_BDAC_2100CD18B00E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AniSprClassList.h"

class CAniSprListMan  
{
public:
	CAniSprListMan();
	virtual ~CAniSprListMan();

	CObjectList m_AniSprMan;	

	void ADDObject(CBaseObject *pObj);

	void DrawObject(CDC *pDC);
	void DrawTop(CDC *pDC);
	void DrawMiddle(CDC *pDC);
	void DrawBottom(CDC *pDC);	

	void AniSprProcess();

	void OnLButtonDown(int x, int y);
	void OnLButtonUp(int x, int y);
	void OnMouseMove(int x, int y);

	CBaseObject* GetAniListOBj(ENUM_ANIFILEID skind);

	void Del_AniListOBj(ENUM_ANIFILEID skind , ENUM_ANIFILEID ekind);
	void Del_AniListOBj(ENUM_ANIFILEID skind);
	void Del_AniListOBj(CBaseObject* pObj);
	void Destroy(int kind = -1);



};

#endif // !defined(AFX_ANISPRLISTMAN_H__8E1751AB_EAD1_4BB7_BDAC_2100CD18B00E__INCLUDED_)
