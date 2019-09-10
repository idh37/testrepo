// ObjectList.h: interface for the CObjectList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJECTLIST_H__E283FFB3_5789_4AB3_8837_538D70C36AEE__INCLUDED_)
#define AFX_OBJECTLIST_H__E283FFB3_5789_4AB3_8837_538D70C36AEE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum ENUM_ANIFILE_COMMON
{
	//�����Ÿ�� �̹��� ȿ��
	BK_SEASON_NORMAL = 300,	//�ð��� ���� ���
	BK_SEASON_JOINROOM,		//�������	
	BK_TIMEZONE_ACTIVE,		//���Ÿ�� ���϶�
	BK_TIMEZONE_STAGEATCIVE,//���Ÿ�� �������� �˸�
	TP_EVENT_POPUP,			//�˾� �˸�
	TP_EVENT_WIN,			//�¸��� ����
};

#include "BaseObject.h"
class UPG_COMMON_EXPORT CObjectList  
{
public:
	int TotNum;
	CBaseObject* pFirst;
	CBaseObject* pLast;

	CObjectList();
	virtual ~CObjectList();
	
	void Destroy();

	BOOL AddObject(CBaseObject* pObj);
	BOOL DelObject(CBaseObject* pObj);
	BOOL DelObject(int skind , int ekind);
	BOOL DelObject(int kind);	

	CBaseObject* GetObject(int kind);
	BOOL CheckObject(int kind);

	void ProcessAll();
	void DrawAll(CDC *pDC);
	void TopDrawAll(CDC *pDC);
	void MiddleDrawAll(CDC *pDC);
	void BottomDrawAll(CDC *pDC);	

	void OnLButtonDown(int x, int y);
	void OnLButtonUp(int x, int y);
	void OnMouseMove(int x, int y);
};

#endif // !defined(AFX_OBJECTLIST_H__E283FFB3_5789_4AB3_8837_538D70C36AEE__INCLUDED_)
