// BaseObject.h: interface for the CBaseObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASEOBJECT_H__8E7830E1_F92A_4B5F_A4A1_48077BD48980__INCLUDED_)
#define AFX_BASEOBJECT_H__8E7830E1_F92A_4B5F_A4A1_48077BD48980__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <math.h>


#define PROCESS_KILLME	(-1)
#define PROCESS_OK		(1)
#define PROCESS_ADD     (2)


enum ENUM_DRAW
{	
	DRAW_BOTTOM = 0,	//���� �Ʒ�
	DRAW_MIDDLE,		//�߰�
	DRAW_TOP			// ����Ʈ��		�� ��
};

typedef struct  {
	int pnum;			//�÷��̾� �ѹ�
	int wincase;		//����
	int valuekind;		//���� ����
	char str_value[40];	//���� �̸�	
	
	//���� �����鿡�� ����
	int nTotalCardNum;	//ī�� ���
	int nCard[5];		//ī������
	char ID[20];		//���̵�	
	INT64 nResultMoney;	//ȹ�� �ݾ�

	//���̷ο� �� ����,�ο�,���� ���� ����
	int nResultWinCase;	//1:���� 2:�ο� 3:����
	
	void Clear(){
		nTotalCardNum = 0;
		pnum = -1;
		wincase = -1;
		valuekind = -1;
		
		memset(nCard,-1,sizeof(nCard));		
		ZeroMemory(ID,sizeof(ID));
		ZeroMemory(str_value,sizeof(str_value));		
		nResultMoney = 0;
		nResultWinCase = 0;
	}

} GAMERESULTDATA ;


double AniGetRadian(int x1, int y1, int x2, int y2);
float AniGetAngle(int x1, int y1, int x2, int y2);
float AniGetDistance(int x1, int y1, int x2, int y2);
POINT AniGetMovePoint(int sx, int sy, float angle, float distance);
float AniGetCrossAngle(float sangle, float rangle);

//////////////////////////////////////////////////////////

class CBaseObject;


class UPG_COMMON_EXPORT CBaseObject  
{
public:

	CBaseObject* pPreNode;
	CBaseObject* pNextNode;

	// �׸��� ���� ����	
	ENUM_DRAW DrawFaceKind;

	//ENUM_ANIFILEID nAniFileKind;
	int	m_nObjID;		//UPG�۾� ENUM_ANIFILEID nAniFileKin�� int m_nObjID���� �ٲ� �������°� �� �ٲܲ�

	BOOL	bMove;	//�̹����� �����̴���
	// �⺻���� ��ġ ������	
	int nResult;		//��� ���� �ص� �ȴ�. �ȵȴ�. �����.
	int	DXp;		//�׸��� ������
	int DYp;
	
	int MXp;		//���콺 ��ǥ
	int MYp;		//���콺 ��ǥ	

	BOOL	m_bIsDestroy;
	int		spr_index;

	CBaseObject();
	virtual ~CBaseObject();

	///////////// ���� �Լ�

	virtual void Destroy(){}
	virtual int  ProcessObject(){return 0;}
	virtual void DrawObject(CDC *pDC){}
	virtual void SetPosition(int x, int y){}	
	virtual	void OnLButtonUp(int x, int y){}
	virtual	void OnLButtonDown(int x, int y){}
	virtual	void OnMouseMove(int x, int y){}			
	virtual BOOL ClickCheck(int x, int y){return 0;}
};

#endif // !defined(AFX_BASEOBJECT_H__8E7830E1_F92A_4B5F_A4A1_48077BD48980__INCLUDED_)
