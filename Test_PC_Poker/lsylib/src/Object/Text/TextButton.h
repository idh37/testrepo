#pragma once

#include "Text.h"
#include <vector>

namespace LSY
{
class UPG_LSYLIB_EXPORT CTextButton : public CText
{
protected:
	std::vector<COLORREF>	m_listStatusColor;

	CPoint					m_ptDownGap;
	ENUM_BUTTON_STATUS		m_nStatus;
public:
	CTextButton(void);
	virtual ~CTextButton();

	virtual const ENUM_OBJECTTYPE GetType(void){return EOT_TEXTBUTTON;}

	virtual void SetStatusColor(const ENUM_BUTTON_STATUS &nStatus, const COLORREF &clrColor);
	virtual const COLORREF GetStatusColor(const ENUM_BUTTON_STATUS &nStatus);

	virtual ENUM_BUTTON_STATUS GetButtonStatus(void){return m_nStatus;}

	virtual void SetDownGap(const CPoint &ptPos);
	virtual const CPoint &GetDownGap(void){return m_ptDownGap;}

	//Object 복사 관련 함수들
	virtual CObjectBase *NewObject(void);
	virtual CObjectBase *Copy(CObjectBase *pObject);

	virtual size_t GetInheritanceClipSize(int &nVersion);
	virtual size_t SaveInheritanceClipSize(int &nVersion, BYTE *pDest);
	virtual size_t LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID);

	virtual bool SaveInheritanceXml(TiXmlNode *pXmlObject);
	virtual bool LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject);

	virtual bool Create(int nWidth, int nHeight);

	virtual void SetEnable(const bool &bEnable);
	virtual void OnMouseOver(void);
	virtual void OnMouseLeave(void);
	virtual bool OnLButtonDown(const CPoint &ptPos);
	virtual bool OnLButtonDbClick(const CPoint &ptPos);
	virtual bool OnLButtonUP(const CPoint &ptPos);
protected:
	virtual void SetStatusColor(void);
	virtual void SetButtonStatus(const ENUM_BUTTON_STATUS &nStatus);

	friend class CButton;
	friend class CSpriteButton;
};
} //namespace LSY