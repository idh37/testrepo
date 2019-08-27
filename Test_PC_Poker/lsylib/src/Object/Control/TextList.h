#pragma once

#include "../Base/ObjectBase.h"

namespace LSY
{
class UPG_LSYLIB_EXPORT CTextList : public CObjectBase
{
protected:
	class TEXTITEM
	{
	public:
		std::string m_strOutput;
		CSize		m_szSize;
		CPoint		m_ptPos;
		FONTINFO	*m_pFontInfo;
	public:
		TEXTITEM();
		virtual ~TEXTITEM();
	};

	typedef list<TEXTITEM *>				TEXTITEMLIST;
	typedef list<TEXTITEM *>::iterator		TEXTITEMLISTITER;

	typedef vector<FONTINFO>				FONTINFOLIST;
	typedef vector<FONTINFO>::iterator		FONTINFOLISTITER;

	FONTINFOLIST	m_listFonts;
	TEXTITEMLIST	m_listItems;

	CPage			*m_pPage;
	CDC				*m_pDC;
	CRect			m_rtTextRect;

	bool			m_bRedraw;
public:
	CTextList(void);
	virtual ~CTextList();

	virtual bool Create(int nWidth, int nHeight);

	virtual const ENUM_OBJECTTYPE GetType(void){return EOT_TEXTLIST;}

	//넣어주는 폰트 리스트에는 폰트가 생성되어 있으면 안된다.
	void SetFontList(const FONTINFOLIST &listFont);
	bool IsSameFontList(const FONTINFOLIST &listFont);

	//Object 복사 관련 함수들
	virtual CObjectBase *NewObject(void);
	virtual CObjectBase *Copy(CObjectBase *pObject);
protected:
	virtual void SetRedraw(void);
};
} //namespace LSY