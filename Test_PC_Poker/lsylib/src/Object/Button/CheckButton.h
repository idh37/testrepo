#pragma once

#include "Button.h"

namespace LSY
{
class UPG_LSYLIB_EXPORT CCheckButton : public CButton
{
protected:
	bool		m_bCheck;
public:
	CCheckButton(void);
	virtual ~CCheckButton();

	virtual const ENUM_OBJECTTYPE GetType(void){return EOT_CHECKBUTTON;}
public:
	//Object 복사 관련 함수들
	virtual CObjectBase *NewObject(void);
	virtual CObjectBase *Copy(CObjectBase *pObject);

	virtual size_t GetInheritanceClipSize(int &nVersion);
	virtual size_t SaveInheritanceClipSize(int &nVersion, BYTE *pDest);
	virtual size_t LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID);

	virtual bool SaveInheritanceXml(TiXmlNode *pXmlObject);
	virtual bool LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject);

	virtual void SetFourStatusButton(int nStartIndex);
	virtual void SetThreeStatusButton(int nStartIndex);
	virtual void SetTwoStatusButton(int nStartIndex);
	virtual void SetOneStatusButton(int nStartIndex);
	virtual void SetFreeStatusButton(int *plistStatusIndex);

	virtual void SetEnable(const bool &bEnable);
	virtual void SetEnableRef(const bool &bEnableRef);
	virtual void OnMouseOver(void);
	virtual void OnMouseLeave(void);
	virtual bool OnLButtonDown(const CPoint &ptPos);
	virtual bool OnLButtonDbClick(const CPoint &ptPos);
	virtual bool OnLButtonUP(const CPoint &ptPos);
	virtual void OnMouseLClick(const CPoint &ptPos);

	virtual bool SetCheck(const bool &bCheck, const bool &bSendMsg = false);
	const bool &IsCheck(void){return m_bCheck;}
protected:
	void CheckStatus(void);
	void CopyStatus(int nGab);
};
} //namespace LSY