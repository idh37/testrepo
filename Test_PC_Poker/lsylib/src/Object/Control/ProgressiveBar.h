#pragma once

#include "../Base/ObjectBase.h"

namespace LSY
{
class CImage;

class UPG_LSYLIB_EXPORT CProgressiveBar : public CObjectBase
{
protected:
	CImage		*m_pBar;
	int			m_nIndex;

	float		m_fCurValue;
	float		m_fMaxValue;
	float		m_fMinValue;

	bool		m_bHorizontality;
	bool		m_bIncrease;
public:
	CProgressiveBar(void);
	virtual ~CProgressiveBar();

	virtual const ENUM_OBJECTTYPE GetType(void){return EOT_PROGRESSIVEBAR;}

	virtual bool Create(LPCTSTR lpPath);
	virtual const std::string GetFileName(void);
	
	virtual void SetIndex(int nIndex);
	virtual const int GetIndex(void);

	virtual const int GetTotalNum(void);

	void SetRangeValue(const float &fMin, const float &fMax);
	const float &GetMinValue(void){return m_fMinValue;}
	const float &GetMaxValue(void){return m_fMaxValue;}

	void SetCurValue(const float &fCur);
	const float &GetCurValue(void){return m_fCurValue;}

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

	virtual void SetObjectProject(CObjectProject *pProject);
};
} //namespace LSY