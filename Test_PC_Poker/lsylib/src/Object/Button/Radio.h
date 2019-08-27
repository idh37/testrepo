#pragma once

#include "CheckBox.h"

namespace LSY
{
class UPG_LSYLIB_EXPORT CRadio : public CCheckBox
{
protected:
	bool	m_bTab;
public:
	CRadio(void);
	virtual ~CRadio();

	virtual const ENUM_OBJECTTYPE GetType(void){return EOT_RADIO;}
	virtual void OnMouseLClick(const CPoint &ptPos);
	virtual bool SetCheck(const bool &bCheck, const bool &bSendMsg = false);
public:
	//Object 복사 관련 함수들
	virtual CObjectBase *NewObject(void);
	virtual CObjectBase *Copy(CObjectBase *pObject);

	virtual void AddChild(CObjectBase *pObject, CObjectBase *pBeforeObject = NULL, bool bBefore = true);
	virtual bool LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject);

	virtual void DrawChild(NMBASE::GRAPHICGDI::CPage *pPage);
protected:
	virtual void OnSetParent(CObjectBase *pParent, bool bSet);
	virtual void SetChildShowRef(void);

	friend class CTabControl;
};
} //namespace LSY/