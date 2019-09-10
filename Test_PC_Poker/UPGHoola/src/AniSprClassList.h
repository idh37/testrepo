// AniSprClassList.h: interface for the AniSprClassList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANISPRCLASSLIST_H__4211E76B_F527_4845_A76F_6BEB394C1C82__INCLUDED_)
#define AFX_ANISPRCLASSLIST_H__4211E76B_F527_4845_A76F_6BEB394C1C82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UserStruct.h"
#include "PlayerHoola.h"
#include "AnimationDefine.h"	//UPG�۾� �߰�




//-----------------------(���� ������ �̹��� etc.spr mc = 0) -----------------------//	

//ANF_SDLG_GRADE
class CGradeSpr : public CBaseObject
{
public:

	NMBASE::SKINGDI::CNMultiScene *m_pMultiScene;	//����� ���� ȿ��
	DWORD m_dwDelay;	

	NMBASE::GRAPHICGDI::CPage		*m_pPage;

	CGradeSpr();
	virtual ~CGradeSpr();

	BOOL Init(int index , NMBASE::GRAPHICGDI::CPage *pPage);

	virtual void Destroy();
	virtual int  ProcessObject();
	virtual void DrawObject(CDC *pDC);
};



//ä��â ���� �޼��� ���� ������ ���� �����ش�.
class CChance_Msg_EffectSpr :public CBaseObject
{
public:
	int nItemCode;

	char	m_ChanceItemText[MAX_PATH];
	int		nDelTime;
	NMBASE::SKINGDI::CNMultiScene *m_pMultiScene;

	CChance_Msg_EffectSpr();
	virtual ~CChance_Msg_EffectSpr();

	void Init(int itemcode);	

	void get_ChanceText(int itemCode);
	virtual void Destroy();
	virtual int  ProcessObject();
	virtual void DrawObject(CDC *pDC);	
};


#endif // !defined(AFX_ANISPRCLASSLIST_H__4211E76B_F527_4845_A76F_6BEB394C1C82__INCLUDED_)
