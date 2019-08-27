#include "stdafx.h"
#include "TextList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace LSY
{
CTextList::TEXTITEM::TEXTITEM()
:	m_strOutput(""),
	m_szSize(0,0),
	m_ptPos(0.0f,0.0f),
	m_pFontInfo(NULL)
{
}

CTextList::TEXTITEM::~TEXTITEM()
{
}

CTextList::CTextList(void)
:	m_pPage(NULL),
	m_pDC(NULL),
	m_bRedraw(false),
	m_rtTextRect(CRectZero)
{
}

CTextList::~CTextList()
{
	if(m_pPage) delete m_pPage;
	m_pPage = NULL;

	if(m_pDC)
	{
		m_pDC->DeleteDC();
		delete m_pDC;
	}
	m_pDC = NULL;

	m_listFonts.clear();

	TEXTITEMLISTITER start_item = m_listItems.begin();
	TEXTITEMLISTITER end_item = m_listItems.end();

	for(;start_item != end_item;start_item++)
	{
		delete *start_item;
	}

	m_listItems.clear();
}

void CTextList::SetRedraw(void)
{
	m_bRedraw = true;
}

bool CTextList::Create(int nWidth, int nHeight)
{
	if(m_rtTextRect.GetWidth() == (float)nWidth && m_rtTextRect.GetHeight() == (float)nHeight) return false;

	if(m_pPage) delete m_pPage;
	m_pPage = NULL;

	if(m_pDC)
	{
		m_pDC->DeleteDC();
		delete m_pDC;
	}
	m_pDC = NULL;

	if(nWidth == 0 || nHeight == 0)
	{
		m_rtTextRect = CRectZero;
	}
	else
	{
		m_pPage = new CPage;
		m_pPage->Init(nWidth + nWidth%2, nHeight, 16);
		m_rtTextRect = CRect(0.0f,0.0f, (float)nWidth, (float)nHeight);

		m_pDC = new CDC;
		CDC tmpDC;
		tmpDC.Attach(GetDC(NULL));

		m_pDC->CreateCompatibleDC(&tmpDC);
		m_pDC->SelectObject(m_pPage->hBmp);
		m_pDC->SetBkMode(TRANSPARENT);

		tmpDC.DeleteDC();
	}

	SetLocalRect(m_rtTextRect);
	SetRedraw();

	return true;
}

CObjectBase *CTextList::NewObject(void)
{
	return new CTextList;
}

CObjectBase *CTextList::Copy(CObjectBase *pObject)
{
	CObjectBase::Copy(pObject);

	CTextList *pList		= (CTextList *)pObject;
	return pObject;
}

bool CTextList::IsSameFontList(const FONTINFOLIST &listFont)
{
	if(m_listFonts.size() != listFont.size()) return false;

	FONTINFOLISTITER start_scr = m_listFonts.begin();
	FONTINFOLISTITER end_scr = m_listFonts.end();

	vector<FONTINFO>::const_iterator start_dst = listFont.begin();
	vector<FONTINFO>::const_iterator end_dst = listFont.end();

	for(;start_scr != end_scr;start_scr++,start_dst++)
	{
		if(*start_scr != *start_dst) return false;
	}

	return true;
}

void CTextList::SetFontList(const FONTINFOLIST &listFont)
{
	if(IsSameFontList(listFont)) return;
	m_listFonts = listFont;

	FONTINFOLISTITER start = m_listFonts.begin();
	FONTINFOLISTITER end = m_listFonts.end();

	for(;start != end;start++)
	{
		start->Create();
	}
}
} //namespace LSY