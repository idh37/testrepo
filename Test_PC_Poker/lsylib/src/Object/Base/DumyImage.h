#pragma once

#include "ObjectBase.h"

namespace LSY
{
class UPG_LSYLIB_EXPORT CDumyImage :public CObjectBase
{
protected:
	NMBASE::GRAPHICGDI::CPage		*m_pPage;
	CDC			*m_pDC;

	CRect		m_rtPageRect;
public:
	CDumyImage(void);
	virtual ~CDumyImage();

	virtual const ENUM_OBJECTTYPE GetType(void){return EOT_DUMYIMAGE;}
	virtual void DrawLocal(NMBASE::GRAPHICGDI::CPage *pPage);
	virtual void DrawMy(NMBASE::GRAPHICGDI::CPage *pPage);

	virtual bool Create(int nWidth, int nHeight);
	virtual void ClearColor(const COLORREF &clrColor);

	virtual void DrawHLine(const CPoint &ptPos, const float &fLen, const COLORREF &clrColor);
	virtual void DrawVLine(const CPoint &ptPos, const float &fLen, const COLORREF &clrColor);

	NMBASE::GRAPHICGDI::CPage	*GetPage(void){return m_pPage;}
	virtual CDC		*GetDC(void){return m_pDC;}
	virtual void	ReleaseDC(void){}
	CRect &GetPageRect(void){return m_rtPageRect;}
public:
	//Object 복사 관련 함수들
	virtual CObjectBase *NewObject(void);
	virtual CObjectBase *Copy(CObjectBase *pObject);
};
} //namespace LSY