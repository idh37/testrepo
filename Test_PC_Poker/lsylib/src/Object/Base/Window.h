#pragma once

#include "Image.h"

namespace LSY
{
class UPG_LSYLIB_EXPORT CWindows : public CImage
{
protected:
	CObjectBase		*m_pPopupObject;
	bool			m_bModal;

public:
	CWindows(void);
	virtual ~CWindows();

	virtual const ENUM_OBJECTTYPE GetType(void){return EOT_WINDOWS;}
	virtual void SubChild(CObjectBase *pObject);
	virtual bool IsIn(IN CPoint &ptPos, OUT bool &bIn);

	virtual bool IsPossibilityReSize(void){return true;}

	virtual void SetModal(const bool &bModal);
	virtual const bool &IsModal(void){return m_bModal;}

	virtual void SetPopup(CObjectBase *pPopup);
public:
	//Object 복사 관련 함수들
	virtual CObjectBase *NewObject(void);
	virtual CObjectBase *Copy(CObjectBase *pObject);

	virtual size_t GetInheritanceClipSize(int &nVersion);
	virtual size_t SaveInheritanceClipSize(int &nVersion, BYTE *pDest);
	virtual size_t LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID);

	virtual bool SaveInheritanceXml(TiXmlNode *pXmlObject);
	virtual bool LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject);
};
} //namespace LSY