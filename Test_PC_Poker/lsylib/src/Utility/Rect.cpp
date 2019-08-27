// WhisperDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Rect.h"
#include <math.h>
#include "../Define.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace LSY
{
const CRect CRectZero(0.0f,0.0f,0.0f,0.0f);

CRect::CRect()
:	left(0.0f),
	top(0.0f),
	right(0.0f),
	bottom(0.0f)
{
}

CRect::CRect(const CRect &rtRect)
:	left(rtRect.left), right(rtRect.right), top(rtRect.top), bottom(rtRect.bottom)
{
}

CRect::CRect(const RECT &rtRect)
:	left((float)rtRect.left), right((float)rtRect.right), top((float)rtRect.top), bottom((float)rtRect.bottom)
{

}

CRect::CRect(const float &left, const float &top, const float &right, const float &bottom)
{
	ASSERT(right >= left && bottom >= top);
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
}

CRect::~CRect()
{
}

const float CRect::GetWidth(void)
{
	return fabs(right - left);
}

const float CRect::GetHeight(void)
{
	return fabs(bottom - top);
}

void CRect::SetRect(const float &left, const float &top, const float &right, const float &bottom)
{
	ASSERT(right >= left && bottom >= top);
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;	
}

void CRect::SetRectEmpty(void)
{
	SetRect(0.0f, 0.0f, 0.0f, 0.0f);
}

void CRect::OffsetRect(const float &fx, const float &fy)
{
	this->left += fx;
	this->top += fy;
	this->right += fx;
	this->bottom += fy;
}

const CPoint CRect::GetLeftTop(void)
{
	return CPoint(left, top);
}

const CPoint CRect::GetLeftBottom(void)
{
	return CPoint(left, bottom);
}

const CPoint CRect::GetRightTop(void)
{
	return CPoint(right, top);
}

const CPoint CRect::GetRightBottom(void)
{
	return CPoint(right, bottom);
}

const CRect CRect::GetBound(const float &fRotation)
{
	CRect rtBound = *this;

	CPoint ptLeftpTop = GetLeftTop();
	
	const float fCos = _cos(fRotation);
	const float fSin = _sin(fRotation);

	CPoint ptPos;

	const float fWidth = GetWidth();
	const float fHeight = GetHeight();

	ptPos.x = fWidth * fCos;
	ptPos.y = fWidth * fSin;

	rtBound.AddBound(ptPos + ptLeftpTop);

	ptPos.x = -fHeight * fSin;
	ptPos.y = fHeight * fCos;

	rtBound.AddBound(ptPos + ptLeftpTop);

	ptPos.x = fWidth * fCos - fHeight * fSin;
	ptPos.y = fWidth * fSin + fHeight * fCos;

	rtBound.AddBound(ptPos + ptLeftpTop);

	return rtBound;
}

void CRect::AddBound(const CPoint &ptPos)
{
	if(ptPos.x < left) left = ptPos.x;
	else if(ptPos.x > right) right = ptPos.x;

	if(ptPos.y < top) top = ptPos.y;
	else if(ptPos.y > bottom) bottom = ptPos.y;
}

void CRect::OffsetRect(const CPoint &ptOffset)
{
	OffsetRect(ptOffset.x, ptOffset.y);
}

void CRect::SetPos(const CPoint &ptPos)
{
	OffsetRect(ptPos.x-left, ptPos.y-top);
}

void CRect::SetPos(const float &fX, const float &fY)
{
	OffsetRect(fX-left, fY-top);
}

void CRect::SetSize(const float &fX, const float &fY)
{
	ASSERT(fX >= 0 && fY >= 0);
	right = left + fX;
	bottom = top + fY;
}

void CRect::SetLeftTop(const CPoint &ptPos)
{
	left = ptPos.x;
	top = ptPos.y;
}

void CRect::SetRightBottom(const CPoint &ptPos)
{
	right = ptPos.x;
	bottom = ptPos.y;
}

void CRect::OffSetZeroPoint(void)
{
	OffsetRect(-left, -top);
}

const bool CRect::IntersectRect(const CRect *lpRect1, const CRect *lpRect2)
{
	ASSERT(lpRect1 && lpRect2);
	if(lpRect1->left > lpRect2->left) left = lpRect1->left;
	else left = lpRect2->left;

	if(lpRect1->right < lpRect2->right) right = lpRect1->right;
	else right = lpRect2->right;

	if(lpRect1->top > lpRect2->top) top = lpRect1->top;
	else top = lpRect2->top;

	if(lpRect1->bottom < lpRect2->bottom) bottom = lpRect1->bottom;
	else bottom = lpRect2->bottom;

	if((right <= left) || (bottom <= top))
	{
		*this = CRectZero;
		return false;
	}

	return true;
}

RECT CRect::GetRect(void)
{
	RECT rt;
	rt.left = (int)left;
	rt.right = (int)right;
	rt.top = (int)top;
	rt.bottom = (int)bottom;
	return rt;
}

const CSize CRect::GetSize(void)
{
	CSize szSize;
	szSize.cx = (int)GetWidth();
	szSize.cy = (int)GetHeight();
	return szSize;
}

void CRect::SetSize(CSize szSize)
{
	right = left + (float)szSize.cx;
	bottom = right + (float)szSize.cy;
}

const CRect &CRect::operator=(const float &fValue)
{
	left = top = right = bottom = fValue;
	return *this;
}

const CRect &CRect::operator+=(const CPoint &ptPos)
{
	left += ptPos.x;
	top += ptPos.y;
	right += ptPos.x;
	bottom += ptPos.y;
	return *this;
}

bool CRect::IsRectNull(void)
{
	if(left == 0.0f && top == 0.0f && bottom == 0.0f && right == 0.0f) return true;
	return false;
}

bool CRect::IsRectEmpty(void)
{
	if(left == right && bottom == top) return true;
	return false;
}

bool CRect::PtInRect(IN const CPoint &ptPos)
{
	if(left <= ptPos.x && right > ptPos.x && top <= ptPos.y && bottom > ptPos.y) return true;
	return false;
}

const CRect &CRect::operator-=(const CPoint &ptPos)
{
	left -= ptPos.x;
	top -= ptPos.y;
	right -= ptPos.x;
	bottom -= ptPos.y;
	return *this;
}

bool operator==(const CRect &z, const CRect &w)
{
	if(z.left == w.left && z.top == w.top && z.right == w.right && z.bottom == w.bottom) return true;
	return false;
}

bool operator!=(const CRect &z, const CRect &w)
{
	if(z.left != w.left || z.top != w.top || z.right != w.right || z.bottom != w.bottom) return true;
	return false;
}

CRect operator+(const CRect &z, const CPoint &w)
{
	return CRect(z.left+w.x, z.top+w.y, z.right+w.x, z.bottom+w.y);
}

CRect operator-(const CRect &z, const CPoint &w)
{
	return CRect(z.left-w.x, z.top-w.y, z.right-w.x, z.bottom-w.y);
}

CRect operator*(const CRect &z, const CPoint &w)
{
	return CRect(z.left*w.x, z.top*w.y, z.right*w.x, z.bottom*w.y);
}

CRect operator/(const CRect &z, const CPoint &w)
{
	return CRect(z.left/w.x, z.top/w.y, z.right/w.x, z.bottom/w.y);
}

CRect operator+(const CRect &z, const float &w)
{
	return CRect(z.left+w, z.top+w, z.right+w, z.bottom+w);
}

CRect operator-(const CRect &z, const float &w)
{
	return CRect(z.left-w, z.top-w, z.right-w, z.bottom-w);
}

CRect operator*(const CRect &z, const float &w)
{
	return CRect(z.left*w, z.top*w, z.right*w, z.bottom*w);
}

CRect operator/(const CRect &z, const float &w)
{
	return CRect(z.left/w, z.top/w, z.right/w, z.bottom/w);
}

} //namespace LSY