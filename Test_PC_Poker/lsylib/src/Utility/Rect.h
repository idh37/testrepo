#pragma once
#include "Point.h"

namespace LSY
{
class UPG_LSYLIB_EXPORT CRect
{
public:
	float left;
	float top;
	float right;
	float bottom;
public:
	CRect(void);
	CRect(const CRect &rtRect);
	CRect(const RECT &rtRect);
	CRect(const float &left, const float &top, const float &right, const float &bottom);
	virtual ~CRect();

	const float GetWidth(void);
	const float GetHeight(void);

	RECT GetRect(void);

	void SetRect(const float &left, const float &top, const float &right, const float &bottom);
	void SetRectEmpty(void);
	void OffsetRect(const float &fx, const float &fy);
	void OffsetRect(const CPoint &ptOffset);
	void OffSetZeroPoint(void);
	void SetPos(const CPoint &ptPos);
	void SetPos(const float &fX, const float &fY);
	void SetSize(const float &fX, const float &fY);

	void SetLeftTop(const CPoint &ptPos);
	void SetRightBottom(const CPoint &ptPos);

	const CPoint GetLeftTop(void);
	const CPoint GetLeftBottom(void);
	const CPoint GetRightTop(void);
	const CPoint GetRightBottom(void);

	const CSize	 GetSize(void);
	void  SetSize(CSize szSize);

	const CRect GetBound(const float &fRotation);
	void AddBound(const CPoint &ptPos);

	//����/���� ũ�Ⱑ 0�� ������� ����
	bool IsRectEmpty(void);
	//����/���� ũ�Ⱑ 0�̰� ��ǥ��(0,0)���� ����
	bool IsRectNull(void);
	//ptPos�� �簢�� �ȿ� �ִ��� ����
	bool PtInRect(IN const CPoint &ptPos);

	const bool IntersectRect(const CRect *lpRect1, const CRect *lpRect2);

	const CRect &operator=(const float &fValue);
	const CRect &operator+=(const CPoint &ptPos);
	const CRect &operator-=(const CPoint &ptPos);

	friend bool operator==(const CRect &z, const CRect &w);
	friend bool operator!=(const CRect &z, const CRect &w);

	friend CRect operator+(const CRect &z, const CPoint &w);
	friend CRect operator-(const CRect &z, const CPoint &w);
	friend CRect operator*(const CRect &z, const CPoint &w);
	friend CRect operator/(const CRect &z, const CPoint &w);

	friend CRect operator+(const CRect &z, const float &w);
	friend CRect operator-(const CRect &z, const float &w);
	friend CRect operator*(const CRect &z, const float &w);
	friend CRect operator/(const CRect &z, const float &w);
};

UPG_LSYLIB_EXPORT extern const CRect CRectZero;
} //namespace LSY