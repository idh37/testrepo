#pragma once

#include "../Base/Group.h"

namespace LSY
{
class CImage;
class CDumyImage;
class UPG_LSYLIB_EXPORT CTabControl : public CGroup
{
protected:
	CImage		*m_pImage;
	int			m_nIndex;
	int			m_nIndexGap;
	CDumyImage	*m_pRectImage;
	
public:
	CTabControl(void);
	virtual ~CTabControl();

	virtual bool Create(LPCTSTR lpPath);
	virtual const ENUM_OBJECTTYPE GetType(void){return EOT_TAB;}

	virtual void SetIndex(const int &nIndex);
	virtual const int GetIndex(void);

	virtual void SetIndexGap(const int &nIndex);
	virtual const int GetIndexGap(void);

	virtual const std::string GetFileName(void);
	virtual const int GetTotalNum(void);

	virtual void SetCurSelFromObject(CRadio *pRadio);
public:
	//Object 복사 관련 함수들
	virtual CObjectBase *NewObject(void);
	virtual CObjectBase *Copy(CObjectBase *pObject);

	virtual size_t GetInheritanceClipSize(int &nVersion);
	virtual size_t SaveInheritanceClipSize(int &nVersion, BYTE *pDest);
	virtual size_t LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID);

	virtual bool SaveInheritanceXml(TiXmlNode *pXmlObject);
	virtual bool LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject);
protected:
	virtual void ClearRadioCheck(CRadio *pException, const bool &bSendMsg = false);
};
} //namespace LSY