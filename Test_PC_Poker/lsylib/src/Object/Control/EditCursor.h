#pragma once

#include "../Base/ObjectBase.h"

namespace LSY
{
class UPG_LSYLIB_EXPORT CEditCursor : public CObjectBase
{
protected:
	NMBASE::GRAPHICGDI::CPage			*m_pPage;
	CDC				*m_pDC;
	bool			m_bShowCursor;
	COLORREF		m_clrCursorColor;
	DWORD			m_dwBlinkTime;

	DWORD			m_dwStartTime;
	DWORD			m_dwCurTime;

	std::string		m_strText;
	bool			m_bRedraw;
	CSize			m_szCursor;
	COLORREF		m_clrTextColor;
	bool			m_bCursorSize;
protected:
	virtual bool Create(int nWidth, int nHeight);
	virtual void CheckRedraw(void);
public:
	CEditCursor(void);
	virtual ~CEditCursor();

	virtual const ENUM_OBJECTTYPE GetType(void){return EOT_EDITCURSOR;}

	virtual void SetCursorColor(const COLORREF &clrColor);
	virtual const COLORREF &GetCursorColor(void){return m_clrCursorColor;}

	virtual void SetBlinkTime(const DWORD &dwBlinkTime){m_dwBlinkTime = dwBlinkTime;}
	virtual const DWORD &GetBlinkTime(void){return m_dwBlinkTime;}

	virtual void SetTextColor(const COLORREF &clrColor);
	virtual const COLORREF &GetTextColor(void){return m_clrTextColor;}

	NMBASE::GRAPHICGDI::CPage *GetPage(void){return m_pPage;}
	const CSize &GetCursorSize(void);

	virtual void SetText(const std::string &strText);
	virtual const std::string &GetText(void){return m_strText;}

	virtual void SetShow(const bool &bShow);
	virtual void Logic(const DWORD &dwTime);

	//Object 복사 관련 함수들
	virtual CObjectBase *NewObject(void);
	virtual CObjectBase *Copy(CObjectBase *pObject);

	virtual void Draw(NMBASE::GRAPHICGDI::CPage *pPage);
	virtual void DrawLocal(NMBASE::GRAPHICGDI::CPage *pPage);
	virtual void DrawMy(NMBASE::GRAPHICGDI::CPage *pPage);

	virtual void SetRedraw(void);
	virtual void ResetTimer(void);

	virtual void CheckEventDraw(void);
	virtual const bool &IsRedraw(void){return m_bRedraw;}

	virtual CDC *GetDC(void){return m_pDC;}

	virtual void SetLogicStartTime(const DWORD &dwStartTime);
};
} //namespace LSY