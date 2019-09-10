// AniSprListMan.h: interface for the CAniSprListMan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANISPRLISTMAN_H__8E1751AB_EAD1_4BB7_BDAC_2100CD18B00E__INCLUDED_)
#define AFX_ANISPRLISTMAN_H__8E1751AB_EAD1_4BB7_BDAC_2100CD18B00E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "AnimationDefine.h"	//UPG�۾� �߰�

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

	CBaseObject* GetAniListOBj(OBJECT_TYPE skind);

	void Del_AniListOBj(OBJECT_TYPE skind , OBJECT_TYPE ekind);
	void Del_AniListOBj(OBJECT_TYPE skind);
	void Del_AniListOBj(CBaseObject* pObj);
	void Destroy(int kind = -1);
	/*
	void Del_MomentAniListOBj(ENUM_ANIFILE skind , ENUM_ANIFILE ekind , BOOL bmulti = FALSE);
	void Del_MomentAniListOBj(CBaseObject* pObj);
	
	void Del_StayAniListOBj(ENUM_ANIFILE skind , ENUM_ANIFILE ekind , BOOL bmulti = FALSE);
	void Del_StayAniListOBj(CBaseObject* pObj);
	*/

	//�� anilist �� Ưȭ�� �Լ�
	//BET_CMD
	//���� �̹����� �� ������� �����ɷ� �ٲ� �ְ� �� �����.
	void BetCmd_ChangeCmd(int pnum);
	//���� Ŀ��带 ������ �� �����.
	void BetCmd_DelDelay(int delaytime);
	//PNum �� Ŀ��带 �����.
	void BetCmd_ClearCmd(int pnum, int nDelayTime = 0 );
};

#endif // !defined(AFX_ANISPRLISTMAN_H__8E1751AB_EAD1_4BB7_BDAC_2100CD18B00E__INCLUDED_)
