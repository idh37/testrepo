#pragma once

#include "Sprite.h"

namespace LSY
{
class UPG_LSYLIB_EXPORT CSpriteWindows : public CSprite
{
protected:
	CObjectBase		*m_pPopupObject;
	bool			m_bModal;

public:
	CSpriteWindows(void);
	virtual ~CSpriteWindows();

	virtual bool Create(LPCTSTR strPath);

	virtual void SetShow(const bool &bShow);
	virtual void SetShowRef(const bool &bShowRef);
	virtual void OnAddSubRootObject(bool bAdd);

	virtual const ENUM_OBJECTTYPE GetType(void){return EOT_SPRITEWINDOWS;}
	virtual void SubChild(CObjectBase *pObject);
	virtual bool IsIn(IN CPoint &ptPos, OUT bool &bIn);

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