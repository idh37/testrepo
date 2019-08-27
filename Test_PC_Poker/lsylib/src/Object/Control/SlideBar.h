#pragma once

#include "../Base/ObjectBase.h"

namespace LSY
{
class CImage;
class CButton;

class UPG_LSYLIB_EXPORT CSlideBar : public CObjectBase
{
protected:
	CImage		*m_pBar1;
	CImage		*m_pBar2;
	CButton		*m_pThumb;

	CPoint		m_ptThumb;
	CPoint		m_ptThumbPivot;

	float		m_fThumbLeftTopWidth;
	float		m_fThumbRightBottomWidth;

	float		m_fCurValue;
	float		m_fMaxValue;
	float		m_fMinValue;
	CRect		m_rcBound;

	bool		m_bHorizontality;
	bool		m_bIncrease;

	int			m_nStartIndex;
public:
	CSlideBar(void);
	virtual ~CSlideBar();

	virtual const ENUM_OBJECTTYPE GetType(void){return EOT_SLIDEBAR;}

	virtual bool Create(LPCTSTR lpPath);
	virtual const std::string GetFileName(void);
	virtual const int GetTotalNum(void);

	virtual void SetStartIndex(const int &nStartIndex);
	virtual const int &GetStartIndex(void);

	void SetRangeValue(const float &fMin, const float &fMax);
	const float &GetMinValue(void){return m_fMinValue;}
	const float &GetMaxValue(void){return m_fMaxValue;}

	void SetCurValue(const float &fCur, const bool &bSetButtonPos = true);
	const float &GetCurValue(void){return m_fCurValue;}

	void SetThumbPivot(const CPoint &ptPivot);
	const CPoint &GetThumbPivot(void){return m_ptThumbPivot;}

	const int GetThumbWidth(void);
	const int GetThumbHeight(void);

	virtual void SetEnable(const bool &bEnable);

	void SetThumbLeftTopWidth(const float &fThumbWidth);
	const float &GetThumbLeftTopWidth(void){return m_fThumbLeftTopWidth;}
	void SetThumbRightBottomWidth(const float &fThumbWidth);
	const float &GetThumbRightBottomWidth(void){return m_fThumbRightBottomWidth;}
	void SetHorizontality(const bool &bHorizontality);
	const bool &IsHorizontality(void){return m_bHorizontality;}
	void SetIncrease(const bool &bIncrease);
	const bool &IsIncrease(void){return m_bIncrease;}
public:
	//Object 복사 관련 함수들
	virtual CObjectBase *NewObject(void);
	virtual CObjectBase *Copy(CObjectBase *pObject);

	virtual size_t GetInheritanceClipSize(int &nVersion);
	virtual size_t SaveInheritanceClipSize(int &nVersion, BYTE *pDest);
	virtual size_t LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID);

	virtual bool SaveInheritanceXml(TiXmlNode *pXmlObject);
	virtual bool LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject);

	virtual LRESULT OnLocalChildNotify(CMessage *msg);

	LRESULT OnThumbBtnProc(CMessage *pMsg);

	virtual bool OnDragBegin(const CPoint &ptPos);
	virtual bool OnDraging(const CPoint &ptPos);
	virtual bool OnDragEnd(const CPoint &ptPos);

	virtual bool EventDraw(CObjectMan *pObjectMan, NMBASE::GRAPHICGDI::CPage *pPage);

	virtual void SetObjectProject(CObjectProject *pProject);
protected:
	void CheckBound(void);
	const float &OnBarDrag(const CPoint &ptPos);
	void CheckThumb(const CPoint &ptPos);
};
} //namespace LSY