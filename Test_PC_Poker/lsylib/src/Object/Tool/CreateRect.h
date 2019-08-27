#pragma once

#include "../Base/ObjectBase.h"

namespace LSY
{
class CDumyImage;
class UPG_LSYLIB_EXPORT CCreateRect :public CObjectBase
{
protected:
	CDumyImage			*m_pLeft;			//좌측선
	CDumyImage			*m_pRight;			//우측선
	CDumyImage			*m_pTop;			//위쪽선
	CDumyImage			*m_pBottom;			//아래쪽선
	CDumyImage			*m_pBack;			//좌측위

	CPoint				m_ptStartPos;
	CPoint				m_ptEndPos;
	CRect				m_rtCreateRect;
public:
	CCreateRect(void);
	virtual ~CCreateRect();

	virtual void Create(const CPoint &ptPos);
	virtual void SetEndPos(const CPoint &ptPos);

	const CRect &GetCreateRect(void){return m_rtCreateRect;}

	virtual const ENUM_OBJECTTYPE GetType(void){return EOT_CREATERECT;}
public:
	//Object 복사 관련 함수들
	virtual CObjectBase *NewObject(void);
	virtual CObjectBase *Copy(CObjectBase *pObject);
};
} //namespace LSY