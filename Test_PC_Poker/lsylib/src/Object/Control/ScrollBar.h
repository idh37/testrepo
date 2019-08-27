#pragma once

#include "../Base/ObjectBase.h"

namespace LSY
{
class CImage;
class CButton;

class UPG_LSYLIB_EXPORT CScrollBar : public CObjectBase
{
protected:
	CButton		*m_pUp;
	CButton		*m_pDown;
	CButton		*m_pThumb;

	CPoint		m_ptThumb;

	float		m_fCurValue;
	float		m_fMaxValue;
	float		m_fMinValue;
	CRect		m_rcBound;

	bool		m_bHorizontality;
	bool		m_bIncrease;

	int			m_nStartIndex;
	float		m_fLen;
	
	float		m_fChangeValue;
	float		m_fPageValue;
public:
	CScrollBar(void);
	virtual ~CScrollBar();

	virtual const ENUM_OBJECTTYPE GetType(void){return EOT_SCROLLBAR;}

	virtual bool Create(LPCTSTR lpPath);

	virtual const std::string GetFileName(void);
	virtual const int GetTotalNum(void);

	virtual bool SetLength(const float &fLen);
	virtual const float &GetLength(void){return m_fLen;}

	virtual bool SetStartIndex(const int &nStartIndex);
	virtual const int &GetStartIndex(void);

	virtual void SetEnable(const bool &bEnable);

	void SetRangeValue(const float &fMin, const float &fMax);
	const float &GetMinValue(void){return m_fMinValue;}
	const float &GetMaxValue(void){return m_fMaxValue;}

	//Up, Down 버튼 클릭시 변경되는 값
	void SetChangeValue(const float &fChangeValue);
	const float &GetChangeValue(void){return m_fChangeValue;}

	//바를 클릭했을때 이동하는 Page 값
	void SetPageValue(const float &fPageValue);
	const float &GetPageValue(void){return m_fPageValue;}

	void SetCurValue(const float &fCur, const bool &bSetButtonPos = true);
	const float &GetCurValue(void){return m_fCurValue;}

	virtual LRESULT OnLocalChildNotify(CMessage *msg);

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

	virtual void OnMouseLClick(const CPoint &ptPos);
	virtual bool OnMouseWheel(const int &nKey, const short &zDelta);

	LRESULT OnThumbBtnProc(CMessage *pMsg);
	LRESULT OnClickUpBtn(CMessage *pMsg);
	LRESULT OnClickDownBtn(CMessage *pMsg);
	LRESULT OnFocusInScroll(CMessage *pMsg);

	virtual void SetObjectProject(CObjectProject *pProject);
protected:
	void CheckBound(void);
	void CheckThumb(const CPoint &ptPos);
};
} //namespace LSY