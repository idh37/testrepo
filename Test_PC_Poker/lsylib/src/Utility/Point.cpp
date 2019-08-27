// WhisperDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Point.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace LSY
{
const CPoint CPointZero(0.0f, 0.0f);

CPoint::CPoint()
:	x(0.0f),
	y(0.0f)
{
}

CPoint::CPoint(float x, float y)
{
	this->x = x;
	this->y = y;
}

CPoint::~CPoint()
{
}

POINT CPoint::GetPoint(void)
{
	POINT ptRet;
	ptRet.x = (int)x;
	ptRet.y = (int)y;
	return ptRet;
}

const CPoint &CPoint::operator=(const POINT &ptPos)
{
	x = (float)ptPos.x;
	y = (float)ptPos.y;
	return *this;
}

const CPoint &CPoint::operator=(const float &fValue)
{
	x = y = fValue;
	return *this;
}

const CPoint &CPoint::operator+=(const float &fValue)
{
	x += fValue;
	y += fValue;
	return *this;
}

const CPoint &CPoint::operator-=(const float &fValue)
{
	x -= fValue;
	y -= fValue;
	return *this;
}

const CPoint &CPoint::operator*=(const float &fValue)
{
	x *= fValue;
	y *= fValue;
	return *this;
}

const CPoint &CPoint::operator/=(const float &fValue)
{
	x /= fValue;
	y /= fValue;
	return *this;
}

const CPoint &CPoint::operator+=(const CPoint &ptPos)
{
	x += ptPos.x;
	y += ptPos.y;
	return *this;
}

const CPoint &CPoint::operator-=(const CPoint &ptPos)
{
	x -= ptPos.x;
	y -= ptPos.y;
	return *this;
}

const CPoint &CPoint::operator*=(const CPoint &ptPos)
{
	x *= ptPos.x;
	y *= ptPos.y;
	return *this;
}

const CPoint &CPoint::operator/=(const CPoint &ptPos)
{
	ASSERT(ptPos.x != 0.0f && ptPos.y != 0.0f);
	x /= ptPos.x;
	y /= ptPos.y;
	return *this;
}

CPoint operator+(const CPoint &z, const CPoint &w)
{
	return CPoint(z.x+w.x, z.y+w.y);
}

CPoint operator-(const CPoint &z, const CPoint &w)
{
	return CPoint(z.x-w.x, z.y-w.y);
}

CPoint operator*(const CPoint &z, const CPoint &w)
{
	return CPoint(z.x*w.x, z.y*w.y);
}

CPoint operator/(const CPoint &z, const CPoint &w)
{
	return CPoint(z.x/w.x, z.y/w.y);
}

CPoint operator+(const CPoint &z, const float &w)
{
	return CPoint(z.x+w, z.y+w);
}

CPoint operator-(const CPoint &z, const float &w)
{
	return CPoint(z.x-w, z.y-w);
}

CPoint operator*(const CPoint &z, const float &w)
{
	return CPoint(z.x*w, z.y*w);
}

CPoint operator/(const CPoint &z, const float &w)
{
	return CPoint(z.x/w, z.y/w);
}

bool operator==(const CPoint &z, const CPoint &w)
{
	if(z.x == w.x && z.y == w.y) return true;
	return false;
}

bool operator!=(const CPoint &z, const CPoint &w)
{
	if(z.x != w.x || z.y != w.y) return true;
	return false;
}
} //namespace LSY