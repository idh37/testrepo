#pragma once

#include "CheckButton.h"

namespace LSY
{
class CText;

class UPG_LSYLIB_EXPORT CCheckBox : public CCheckButton
{
protected:
	CText			*m_pText;
	CSize			m_szText;

	std::string		m_strFontName;
	int				m_nFontSize;
	bool			m_bFontBold;
	COLORREF		m_clrTextColor;

	std::string		m_strCheckFontName;
	int				m_nCheckFontSize;
	bool			m_bCheckFontBold;
	COLORREF		m_clrCheckTextColor;

	std::string		m_strText;
public:
	CCheckBox(void);
	virtual ~CCheckBox();

	virtual const ENUM_OBJECTTYPE GetType(void){return EOT_CHECKBOX;}

	virtual bool Create(LPCTSTR lpPath);
	virtual bool CreateTextArea(int nWidth, int nHeight);

	virtual void SetText(const std::string &strText);
	virtual const std::string &GetText(void){return m_strText;}

	virtual bool SetCheck(const bool &bCheck, const bool &bSendMsg = false);

	virtual void SetFont(const std::string &strFontName, const int &nFontSize, const bool &bBold);
	virtual void SetTextColor(COLORREF clrColor);

	const std::string &GetFontName(void){return m_strFontName;}
	const int &GetFontSize(void){return m_nFontSize;}
	const bool &IsFontBold(void){return m_bFontBold;}
	const COLORREF &GetTextColor(void){return m_clrTextColor;}

	virtual void SetCheckFont(const std::string &strFontName, const int &nFontSize, const bool &bBold);
	virtual void SetCheckTextColor(COLORREF clrColor);

	const std::string &GetCheckFontName(void){return m_strCheckFontName;}
	const int &GetCheckFontSize(void){return m_nCheckFontSize;}
	const bool &IsCheckFontBold(void){return m_bCheckFontBold;}
	const COLORREF &GetCheckTextColor(void){return m_clrCheckTextColor;}

	const CSize &GetTextSize(void){return m_szText;}
	virtual bool IsPossibilityReSize(void){return true;}

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
	void RepositionTextArea(void);
	void CheckTextStatus(void);
	virtual void SetStatusIndex(void);
};
} //namespace LSY