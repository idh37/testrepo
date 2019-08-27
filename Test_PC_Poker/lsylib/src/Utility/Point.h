#pragma once

namespace LSY
{
class UPG_LSYLIB_EXPORT CPoint
{
public:
	float	x;
	float	y;
public:
	CPoint(void);
	CPoint(float x, float y);
	virtual ~CPoint();

	POINT GetPoint(void);
	inline void SetPoint(float fx, float fy){x = fx, y = fy;}
	inline void Offset(float fx, float fy){x += fx, y += fy;}

	const CPoint &operator=(const POINT &ptPos);
	const CPoint &operator=(const float &fValue);
	const CPoint &operator+=(const float &fValue);
	const CPoint &operator-=(const float &fValue);
	const CPoint &operator*=(const float &fValue);
	const CPoint &operator/=(const float &fValue);

	const CPoint &operator+=(const CPoint &ptPos);
	const CPoint &operator-=(const CPoint &ptPos);
	const CPoint &operator*=(const CPoint &ptPos);
	const CPoint &operator/=(const CPoint &ptPos);

	friend UPG_LSYLIB_EXPORT CPoint operator+(const CPoint &z, const CPoint &w);
	friend UPG_LSYLIB_EXPORT CPoint operator-(const CPoint &z, const CPoint &w);
	friend UPG_LSYLIB_EXPORT CPoint operator*(const CPoint &z, const CPoint &w);
	friend UPG_LSYLIB_EXPORT CPoint operator/(const CPoint &z, const CPoint &w);

	friend UPG_LSYLIB_EXPORT CPoint operator+(const CPoint &z, const float &w);
	friend UPG_LSYLIB_EXPORT CPoint operator-(const CPoint &z, const float &w);
	friend UPG_LSYLIB_EXPORT CPoint operator*(const CPoint &z, const float &w);
	friend UPG_LSYLIB_EXPORT CPoint operator/(const CPoint &z, const float &w);

	friend UPG_LSYLIB_EXPORT bool operator==(const CPoint &z, const CPoint &w);
	friend UPG_LSYLIB_EXPORT bool operator!=(const CPoint &z, const CPoint &w);
};

UPG_LSYLIB_EXPORT extern const CPoint CPointZero;
} //namespace LSY