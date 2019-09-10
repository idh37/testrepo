// BaseObject.cpp: implementation of the CBaseObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BaseObject.h"

#include "ObjectList.h"


#ifndef RADIAN
#define RADIAN 0.01745f
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


double AniGetRadian(int x1, int y1, int x2, int y2)
{
	// �ȼ� �Ÿ�
	int w = x2-x1;
	int h = y1-y2;

	if(w==0 && h==0) return 0;
	else if(w==0) {
		if(h>0) return 90.0f * RADIAN;
		else return 270.0f * RADIAN;
	}
	else if(h==0) {
		if(w>0) return 0.0f;
		else return 180.0f * RADIAN;
	}

	// ���� ���ϱ�
	double al = (double)abs(h)/(double)abs(w);

	double rad = atan(al);

	if(w==0 && h==0) return 0;
	else if(w>=0 && h>=0) {		// 1��и�
	}
	else if(w<0 && h>=0) {	// 2��и�
		rad = (180.0f * RADIAN) - rad;
	}
	else if(w<0 && h<0) {	// 3��и�
		rad += 180.0f * RADIAN;
	}	
	else if(w>=0 && h<0) {	// 4��и�
		rad = (360.0f * RADIAN) - rad;
	}

	return rad;
}

float AniGetAngle(int x1, int y1, int x2, int y2)
{
	// �ȼ� �Ÿ�
	int w = x2-x1;
	int h = y1-y2;

	if(w==0 && h==0) return 0;
	else if(w==0) {
		if(h>0) return 90.0f;
		else return 270.0f;
	}
	else if(h==0) {
		if(w>0) return 0.0f;
		else return 180.0f;
	}

	// ���� ���ϱ�
	double al = (double)abs(h)/(double)abs(w);
	float angle = (float)(atan(al) / RADIAN);

	if(w==0 && h==0) return 0;
	else if(w>=0 && h>=0) {		// 1��и�
	}
	else if(w<0 && h>=0) {	// 2��и�
		angle = 180.0f - angle;
	}
	else if(w<0 && h<0) {	// 3��и�
		angle += 180.0f;
	}	
	else if(w>=0 && h<0) {	// 4��и�
		angle = 360.0f - angle;
	}

	return angle;
}

float AniGetDistance(int x1, int y1, int x2, int y2)
{
	float len = 0;

	// �ȼ� �Ÿ�
	int w = abs(x2-x1);
	int h = abs(y2-y1);
	
	if(w==0 && h==0) return 0;
	else if(w==0) len = (float)h;
	else if(h==0) len = (float)w;
	else len = (float)sqrt((float)(w*w + h*h));

	return len;
}

POINT AniGetMovePoint(int sx, int sy, float angle, float distance)
{
	POINT pnt;
	pnt.x = sx;
	pnt.y = sy;
	double rad = angle * RADIAN;
	int x = (int)(cos(rad) * distance);
	int y = -(int)(sin(rad) * distance);
	pnt.x += x;
	pnt.y += y;
	return pnt;
}

// ���ذ��� ���� ��밢�� �������ϱ�
float AniGetCrossAngle(float sangle, float rangle)
{
	if(sangle == rangle) return 0;

	if(sangle < rangle) {
		float ang = rangle - sangle;
		if(ang > 180.0f) {
			ang = 360.0f - ang;
			return -ang;
		}
		else return ang;
	}
	else {
		float ang = sangle - rangle;
		if(ang > 180.0f) {
			ang = 360.0f - ang;
			return ang;
		}
		else return -ang;
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseObject::CBaseObject()
{
	pPreNode = pNextNode = NULL;

	DrawFaceKind = DRAW_TOP;
	m_nObjID = 0;	
	DXp = 0;		//�׸��� ������
	DYp = 0;
	
	MXp = 0;		//���콺 ��ǥ
	MYp = 0;		//���콺 ��ǥ
	nResult = 0;
	spr_index = -1;
	bMove =FALSE;
	m_bIsDestroy = FALSE;
}

CBaseObject::~CBaseObject()
{

}