#pragma once

#include "../Base/ObjectBase.h"

namespace LSY
{
class CDumyImage;
class UPG_LSYLIB_EXPORT CObjectRect :public CObjectBase
{
protected:
	CDumyImage			*m_pLeft;			//좌측선
	CDumyImage			*m_pRight;			//우측선
	CDumyImage			*m_pTop;			//위쪽선
	CDumyImage			*m_pBottom;			//아래쪽선
	CDumyImage			*m_pLeftTop;		//좌측위
	CDumyImage			*m_pRightTop;		//우측위
	CDumyImage			*m_pLeftBottom;		//좌측아래
	CDumyImage			*m_pRightBottom;	//우측아래

	CObjectBase			*m_pObserver;		//감시할 Object
	bool				m_bObserverChange;
public:
	CObjectRect(void);
	virtual ~CObjectRect();

	virtual const ENUM_OBJECTTYPE GetType(void){return EOT_OBJECTRECT;}

	bool Create(CObjectBase *pObject);
	virtual void Draw(NMBASE::GRAPHICGDI::CPage *pPage);

	void SetOserverChange(const bool &bChange);

	CObjectBase	*GetObserver(void){return m_pObserver;}
public:
	//Object 복사 관련 함수들
	virtual CObjectBase *NewObject(void);
	virtual CObjectBase *Copy(CObjectBase *pObject);

protected:
	void CheckObserver(void);
	void Release(void);
};
} //namespace LSY