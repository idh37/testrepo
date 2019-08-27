#pragma once

#include "ObjectBase.h"

namespace LSY
{
class CRadio;
class CDumyImage;

class UPG_LSYLIB_EXPORT CGroup : public CObjectBase
{
protected:
	CDumyImage			*m_pRectImage;
	std::list<CRadio *>	m_listRadio;

public:
	CGroup(void);
	virtual ~CGroup();

	virtual const ENUM_OBJECTTYPE GetType(void){return EOT_GROUP;}

	virtual void AttachRadio(CRadio *pRadio, int nIndex = -1);
	virtual void DettachRadio(CRadio *pRadio);
	virtual void DettachAllRadio(void);

	virtual bool IsPossibilityReSize(void){return true;}
	virtual void SetLocalRect(const CRect &rtRect);

	virtual CRadio *GetCurSel(void);
	virtual const int GetCurSelIndex(void);
	virtual const int GetCurSelID(void);
	virtual void SetCurSelFromID(const int &nID);
	virtual void SetCurSelFromIndex(const int &nIndex);
	virtual void SetCurSelFromObject(CRadio *pRadio);

	virtual CRadio *GetRadioFromIndex(const int &nIndex);
	virtual const int GetRadioToIndex(CRadio *pRadio);

	virtual LRESULT OnLocalChildNotify(CMessage *msg);
	const std::list<CRadio *> &GetAttachRadioList(void){return m_listRadio;}
public:
	//Object 복사 관련 함수들
	virtual CObjectBase *NewObject(void);
	virtual CObjectBase *Copy(CObjectBase *pObject);

	virtual size_t GetInheritanceClipSize(int &nVersion);
	virtual size_t SaveInheritanceClipSize(int &nVersion, BYTE *pDest);
	virtual size_t LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID);

	virtual bool SaveInheritanceXml(TiXmlNode *pXmlObject);
	virtual bool LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject);

	LRESULT OnCheckChange(CMessage *pMsg);
protected:
	virtual void ClearRadioCheck(CRadio *pException, const bool &bSendMsg = false);
	friend class CRadio;
};
} //namespace LSY