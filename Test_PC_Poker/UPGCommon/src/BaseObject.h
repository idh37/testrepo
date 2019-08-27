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
	DRAW_BOTTOM = 0,	//제일 아래
	DRAW_MIDDLE,		//중간
	DRAW_TOP			// 이펙트류		젤 위
};

typedef struct  {
	int pnum;			//플레이어 넘버
	int wincase;		//승패
	int valuekind;		//족보 종류
	char str_value[40];	//족보 이름	
	
	//높은 족보들에서 쓰임
	int nTotalCardNum;	//카드 장수
	int nCard[5];		//카드정보
	char ID[20];		//아이디	
	INT64 nResultMoney;	//획득 금액

	//하이로우 용 하이,로우,스윙 승패 여부
	int nResultWinCase;	//1:하이 2:로우 3:스윙
	
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

	// 그리기 순서 종류	
	ENUM_DRAW DrawFaceKind;

	//ENUM_ANIFILEID nAniFileKind;
	int	m_nObjID;		//UPG작업 ENUM_ANIFILEID nAniFileKin이 int m_nObjID으로 바뀜 에러나는곳 다 바꿀껏

	BOOL	bMove;	//이미지가 움직이는지
	// 기본적인 위치 정보들	
	int nResult;		//결과 진행 해도 된다. 안된다. 종료다.
	int	DXp;		//그리는 시작점
	int DYp;
	
	int MXp;		//마우스 좌표
	int MYp;		//마우스 좌표	

	BOOL	m_bIsDestroy;
	int		spr_index;

	CBaseObject();
	virtual ~CBaseObject();

	///////////// 가상 함수

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
