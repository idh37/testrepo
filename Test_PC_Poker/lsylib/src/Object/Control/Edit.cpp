#include "stdafx.h"
#include "Edit.h"
#include "../Base/DumyImage.h"
#include <imm.h>
#include <Dimm.h>
#include "../Control/ScrollBar.h"
#include "../ObjectMan.h"
#include "../../tinyxml/tinyxml.h"
#include "../../Utility/MyMime64.h"
#include "../../Utility/ScanCode.h"
#include "../../Functor/Message.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace LSY
{
#define LID_TRADITIONAL_CHINESE	0x0404
#define LID_JAPANESE			0x0411
#define LID_KOREAN				0x0412
#define LID_SIMPLIFIED_CHINESE	0x0804

CEditItem::CEditItem()
:	m_rtRect(CRectZero)
{
	ZeroMemory(m_str, 3);
}

const size_t CEditItem::GetLength(void)
{
	return strlen(m_str);
}

bool CEditItem::IsIn(const CPoint &ptLocal, const CPoint &ptStartPos, const float &fSpaceBetweenLetters)
{
	if((ptStartPos.x + m_rtRect.left <= ptLocal.x) && (ptStartPos.x + m_rtRect.right + fSpaceBetweenLetters > ptLocal.x)) return true;
	return false;
}

bool CEditItem::IsPrevIn(const CPoint &ptLocal, const CPoint &ptStartPos)
{
	if(ptLocal.x < ptStartPos.x + m_rtRect.left) return true;
	return false;
}

CEditLine::CEditLine()
:	m_ptPos(CPointZero),
	m_fTotalWidth(0)
{
}

CEditLine::~CEditLine()
{
	EDITITEMLISTITER start = m_listItems.begin();
	EDITITEMLISTITER end = m_listItems.end();
	for(;start != end;++start)
	{
		delete *start;
	}

	m_listItems.clear();
}

EDITITEMLISTITER CEditLine::begin()
{
	return m_listItems.begin();
}

EDITITEMLISTITER CEditLine::end()
{
	return m_listItems.end();
}

const size_t CEditLine::size(void)
{
	return m_listItems.size();
}

bool CEditLine::IsLineIn(const CPoint &ptLocal, const CPoint &ptStartPos, const float &fLineSpace, const float &fLineHeight)
{
	CPoint ptPos = ptStartPos + m_ptPos;
	if(ptPos.y <= ptLocal.y && (ptPos.y + fLineHeight + fLineSpace) > ptLocal.y) return true;
	return false;
}

bool CEditLine::IsPrevLineIn(const CPoint &ptLocal, const CPoint &ptStartPos)
{
	CPoint ptPos = ptStartPos + m_ptPos;
	if(ptPos.y > ptLocal.y) return true;
	return false;
}

void CEditLine::push_back(CEditItem *pItem)
{
	m_listItems.push_back(pItem);
}

void CEditLine::push_front(CEditItem *pItem)
{
	m_listItems.push_front(pItem);
}

EDITITEMLISTITER CEditLine::erase(const EDITITEMLISTITER &start, const EDITITEMLISTITER &end)
{
	return m_listItems.erase(start, end);
}

EDITITEMLISTITER CEditLine::erase(const EDITITEMLISTITER &start)
{
	return m_listItems.erase(start);
}

EDITITEMLISTITER CEditLine::insert(const EDITITEMLISTITER &start, CEditItem *pItem)
{
	return m_listItems.insert(start, pItem);
}

const CEditLine &CEditLine::operator=(const CEditLine &w)
{
	m_listItems = w.m_listItems;
	m_fTotalWidth = w.m_fTotalWidth;
	m_ptPos = w.m_ptPos;
	return *this;
}

const CEditLine &CEditLine::operator=(const CPoint &w)
{
	m_ptPos = w;
	return *this;
}

const CEditLine &CEditLine::operator=(const float &w)
{
	m_fTotalWidth = w;
	return *this;
}

const CEditLine &CEditLine::operator=(const EDITITEMLIST &w)
{
	m_listItems = w;
	return *this;
}

const CEditLine &CEditLine::operator+=(const float &w)
{
	m_fTotalWidth += w;
	return *this;
}

const CEditLine &CEditLine::operator-=(const float &w)
{
	m_fTotalWidth -= w;
	return *this;
}

bool operator==(const EDITPOS &z, const EDITPOS &w)
{
	if(z.m_itLine == w.m_itLine && z.m_itItem == w.m_itItem) return true;
	return false;
}

bool operator!=(const EDITPOS &z, const EDITPOS &w)
{
	if(z.m_itLine != w.m_itLine || z.m_itItem != w.m_itItem) return true;
	return false;
}

bool operator==(const EDITPOS &z, const EDITLINELISTITER &w)
{
	if(z.m_itLine == w) return true;
	return false;
}

bool operator!=(const EDITPOS &z, const EDITLINELISTITER &w)
{
	if(z.m_itLine != w) return true;
	return false;
}

bool operator==(const EDITPOS &z, const EDITITEMLISTITER &w)
{
	if(z.m_itItem == w) return true;
	return false;
}

bool operator!=(const EDITPOS &z, const EDITITEMLISTITER &w)
{
	if(z.m_itItem != w) return true;
	return false;
}

const EDITPOS &EDITPOS::operator=(const EDITPOS &w)
{
	m_itLine = w.m_itLine;
	m_itItem = w.m_itItem;
	return *this;
}

const EDITPOS &EDITPOS::operator=(const EDITLINELISTITER &w)
{
	m_itLine = w;
	return *this;
}

const EDITPOS &EDITPOS::operator=(const EDITITEMLISTITER &w)
{
	m_itItem = w;
	return *this;
}

EDITLINELISTITER &EDITPOS::NextLine(void)
{
	return ++m_itLine;
}

EDITITEMLISTITER &EDITPOS::NextItem(void)
{
	return ++m_itItem;
}

EDITLINELISTITER &EDITPOS::PrevLine(void)
{
	return --m_itLine;
}

EDITITEMLISTITER &EDITPOS::PrevItem(void)
{
	return --m_itItem;
}

EDITPOS::operator CEditLine*()
{
	return *m_itLine;
}

EDITPOS::operator CEditItem*()
{
	if((*m_itLine)->end() == m_itItem) return NULL;
	return *m_itItem;
}

EDITPOS::operator CEditLine*() const
{
	return *m_itLine;
}

EDITPOS::operator CEditItem*() const
{
	if((*m_itLine)->end() == m_itItem) return NULL;
	return *m_itItem;
}

EDITPOS::operator EDITLINELISTITER()
{
	return m_itLine;
}

EDITPOS::operator EDITITEMLISTITER()
{
	return m_itItem;
}

CEdit::CEdit(void)
:	m_bKoreanMode(false),
	m_nLimitText(128),
	m_bPassword(false),
	m_bStat(false),
	m_bIgnoreWMChar(false),
	m_bIgnoreWMKeyDown(false),
	m_bRedraw(false),
	m_pCursor(NULL),
	m_ptStartPos(0,0),
	m_bCaculateCursorPos(true),
	m_bReCaculateItemWidth(false),
	m_pBack(NULL),
	m_bNumber(false),
	m_bUseClipboard(true),
	m_clrSelectBack(RGB(0,0,230)),
	m_clrSelectFont(RGB(255,255,255)),
	m_ptDragPos(CPointZero),
	m_dwDragStartTime(0),
	m_dwDragMoveTime(0),
	m_fLineSpace(1),
	m_fLineHeight(0.0f),
	m_nScrollPos(0),
	m_bAddScroll(false),
	m_pScrollBar(NULL),
	m_bLineScroll(true)
{
	m_itLineEnd = m_listLines.end();

	SetMessage(true);

	m_pBack = new CDumyImage;
	AddLocalChild(m_pBack);

	m_pCursor = new CEditCursor;
	m_pCursor->SetShow(false);
	AddLocalChild(m_pCursor);

	m_pCursor->SetLocalClipUseParentPos(true);

	Clear();
}

CEdit::~CEdit()
{
	Clear();
}

bool CEdit::Create(int nWidth, int nHeight)
{
	if(!m_pBack->Create(nWidth, nHeight)) return false;

	SetLocalRect(m_pBack->GetLocalRect());
	SetReCaculateItemWidth();
	SetCaculateCursorPos();
	SetRedraw();

	CRect rtRect = m_pBack->GetLocalRect();
	--rtRect.left;
	--rtRect.top;
	++rtRect.right;
	++rtRect.bottom;

	m_pCursor->SetLocalClipRect(rtRect);

	CheckScrollData();

	return true;
}

void CEdit::CheckKoreanMode(bool bFocus)
{
	DWORD dwConversion;
	DWORD dwSentence;

	HWND  hWnd = NULL;
	if(m_pProject)
	{
		CObjectMan *pManager = m_pProject->GetObjectMan();
		if(pManager)
		{
			const CWnd *pWnd = pManager->GetWnd();
			if(pWnd) hWnd = pWnd->GetSafeHwnd();
		}
	}
	HIMC  hIMC = ::ImmGetContext( hWnd );
	ImmGetConversionStatus( hIMC, &dwConversion, &dwSentence );
	if(bFocus)ImmNotifyIME(hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
	ImmReleaseContext( hWnd,hIMC );
	m_bKoreanMode = ((dwConversion & IME_CMODE_NATIVE) != 0);
}

void CEdit::SetText( const std::string& str )
{
	Clear();

	int nLen   = (int)str.length();
	const char* szBuf = str.c_str();

	for( int i = 0; i < nLen; ++i )
	{
		char szCurStr[3];

		if( ::IsDBCSLeadByte( szBuf[i] ) )
		{
			szCurStr[0] = szBuf[i];
			szCurStr[1] = szBuf[i+1];
			szCurStr[2] = '\0';
			++i;
		}
		else
		{
			szCurStr[0] = szBuf[i];
			szCurStr[1] = '\0';
		}

		OnAddChar( szCurStr, true );
	}

	SetRedraw();
}

const std::string CEdit::GetText(void)
{
	std::string str = "";
	bool bAdd = (GetCursorText() == "")?true:false;

	EDITLINELISTITER start_line = m_listLines.begin();
	EDITLINELISTITER end_line = m_itLineEnd;

	EDITITEMLISTITER start_item, end_item;

	CEditLine *pLine = NULL;
	CEditItem *pItem = NULL;

	for(;start_line != end_line;++start_line)
	{
		pLine = *start_line;
		start_item = pLine->begin();
		end_item = pLine->end();

		for(;start_item != end_item;++start_item)
		{
			pItem = *start_item;
			if((m_cCurrentPos != start_line) || (m_cCurrentPos != start_item) || bAdd) str += pItem->m_str;
		}
	}

	return str;
}

CObjectBase *CEdit::OnFocusIn(CObjectBase *pObject)
{
	CheckKoreanMode(true);
	m_pCursor->SetShow(true);
	return CObjectBase::OnFocusIn(pObject);
}

void CEdit::OnFocusOut(CObjectBase *pObject)
{
	std::string strTemp = GetCursorText();
	if(strTemp != "")
	{
		SetCursorText("");
		m_bStat = false;
		OnAddChar(strTemp, true);
	}

	if(m_cSelectStart != m_itLineEnd)
	{
		m_cSelectStart = m_itLineEnd;
		m_cSelectEnd = m_itLineEnd;
		SetRedraw();
	}

	CheckKoreanMode(false);
	m_pCursor->SetShow(false);
	CObjectBase::OnFocusOut(pObject);
}

CObjectBase *CEdit::NewObject(void)
{
	return new CEdit;
}

CObjectBase *CEdit::Copy(CObjectBase *pObject)
{
	CObjectBase::Copy(pObject);

	CEdit *pEdit				= (CEdit *)pObject;
	std::string strText = GetText();
	pEdit->SetText(strText);

	pEdit->m_pBack->Create((int)m_rtLocalRect.GetWidth(), (int)m_rtLocalRect.GetHeight());

	pEdit->m_ptStartPos		= m_ptStartPos;
	pEdit->m_cFontInfo		= m_cFontInfo;
	pEdit->m_nLimitText		= m_nLimitText;
	pEdit->m_bPassword		= m_bPassword;
	
	pEdit->m_bRedraw = true;
	pEdit->m_bCaculateCursorPos = true;
	pEdit->m_bReCaculateItemWidth = true;

	pEdit->m_bNumber			= m_bNumber;

	pEdit->m_bUseClipboard		= m_bUseClipboard;
	pEdit->m_clrSelectBack		= m_clrSelectBack;
	pEdit->m_clrSelectFont		= m_clrSelectFont;

	pEdit->m_pCursor->SetBlinkTime(m_pCursor->GetBlinkTime());
	pEdit->m_pCursor->SetCursorColor(m_pCursor->GetCursorColor());
	pEdit->m_pCursor->SetTextColor(m_pCursor->GetTextColor());

	return pObject;
}

size_t CEdit::GetInheritanceClipSize(int &nVersion)
{
	size_t nRet = __super::GetInheritanceClipSize(nVersion);
	std::string strText = GetText();
	nRet += sizeof(size_t);		//텍스트 크기
	nRet += strText.size();		//텍스트
	nRet += sizeof(CPoint);		//m_ptStartPos
	nRet += m_cFontInfo.GetInheritanceClipSize(nVersion);	//m_cFontInfo
	nRet += sizeof(int);		//m_nLimitText
	nRet += sizeof(bool);		//m_bPassword
	nRet += sizeof(DWORD);		//m_pCursor->GetBlinkTime()
	nRet += sizeof(COLORREF);	//m_pCursor->GetCursorColor()
	nRet += sizeof(COLORREF);	//m_pCursor->GetTextColor()
	nRet += sizeof(bool);		//m_bNumber
	nRet += sizeof(bool);		//m_bUseClipboard
	nRet += sizeof(COLORREF);	//m_clrSelectBack
	nRet += sizeof(COLORREF);	//m_clrSelectFont
	nRet += sizeof(float);		//m_fLineSpace
	nRet += sizeof(bool);		//m_bLineScroll
	nRet += sizeof(bool);		//스크롤바유무
	if(m_pScrollBar) nRet += m_pScrollBar->GetClipSize(nVersion);	//스크롤바가 있을 경우
	return nRet;
}

size_t CEdit::SaveInheritanceClipSize(int &nVersion, BYTE *pDest)
{
	size_t nRet = __super::SaveInheritanceClipSize(nVersion, pDest);

	std::string strText = GetText();
	size_t nSize = strText.size();
	memcpy(pDest+nRet, &nSize, sizeof(size_t));
	nRet += sizeof(size_t);		//텍스트 크기
	if(nSize)
	{
		memcpy(pDest+nRet, strText.c_str(), nSize);
		nRet += nSize;				//텍스트
	}
	memcpy(pDest+nRet, &m_ptStartPos, sizeof(CPoint));
	nRet += sizeof(CPoint);		//m_ptStartPos
	nRet += m_cFontInfo.SaveInheritanceClipSize(nVersion, pDest+nRet);	//m_cFontInfo
	memcpy(pDest+nRet, &m_nLimitText, sizeof(int));
	nRet += sizeof(int);		//m_nLimitText
	memcpy(pDest+nRet, &m_bPassword, sizeof(bool));
	nRet += sizeof(bool);		//m_bPassword

	DWORD dwTime = m_pCursor->GetBlinkTime();
	memcpy(pDest+nRet, &dwTime, sizeof(DWORD));
	nRet += sizeof(DWORD);		//m_pCursor->GetBlinkTime()

	COLORREF clrColor = m_pCursor->GetCursorColor();
	memcpy(pDest+nRet, &clrColor, sizeof(COLORREF));
	nRet += sizeof(COLORREF);	//m_pCursor->GetCursorColor()

	clrColor = m_pCursor->GetTextColor();
	memcpy(pDest+nRet, &clrColor, sizeof(COLORREF));
	nRet += sizeof(COLORREF);	//m_pCursor->GetTextColor()

	memcpy(pDest+nRet, &m_bNumber, sizeof(bool));
	nRet += sizeof(bool);		//m_bNumber

	memcpy(pDest+nRet, &m_bUseClipboard, sizeof(bool));
	nRet += sizeof(bool);		//m_bUseClipboard
	memcpy(pDest+nRet, &m_clrSelectBack, sizeof(COLORREF));
	nRet += sizeof(COLORREF);	//m_clrSelectBack
	memcpy(pDest+nRet, &m_clrSelectFont, sizeof(COLORREF));
	nRet += sizeof(COLORREF);	//m_clrSelectFont
	memcpy(pDest+nRet, &m_fLineSpace, sizeof(float));
	nRet += sizeof(float);	//m_fLineSpace
	memcpy(pDest+nRet, &m_bLineScroll, sizeof(bool));
	nRet += sizeof(bool);	//m_bLineScroll

	bool bUse = m_pScrollBar?true:false;
	memcpy(pDest+nRet, &bUse, sizeof(bool));
	nRet += sizeof(bool);		//스크롤바 유무

	if(m_pScrollBar)
	{
		nRet += m_pScrollBar->SaveClipSize(nVersion, pDest+nRet);
	}

	return nRet;
}

size_t CEdit::LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID)
{
	size_t nRet = __super::LoadInheritanceClipSize(nVersion, pDest, pProject, bSetID);

	std::string strText;
	size_t nSize = 0;
	memcpy(&nSize, pDest+nRet, sizeof(size_t));
	nRet += sizeof(size_t);		//텍스트 크기
	if(nSize)
	{
		GetPointerToString(pDest+nRet, nSize, strText);
		nRet += nSize;				//텍스트
	}
	memcpy(&m_ptStartPos, pDest+nRet, sizeof(CPoint));
	nRet += sizeof(CPoint);		//m_ptStartPos
	nRet += m_cFontInfo.LoadInheritanceClipSize(nVersion, pDest+nRet, pProject, bSetID);	//m_cFontInfo
	memcpy(&m_nLimitText, pDest+nRet, sizeof(int));
	nRet += sizeof(int);		//m_nLimitText
	memcpy(&m_bPassword, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);		//m_bPassword

	DWORD dwTime = 0;
	memcpy(&dwTime, pDest+nRet, sizeof(DWORD));
	nRet += sizeof(DWORD);		//m_pCursor->GetBlinkTime()

	m_pCursor->SetBlinkTime(dwTime);

	COLORREF clrColor = 0;
	memcpy(&clrColor, pDest+nRet, sizeof(COLORREF));
	nRet += sizeof(COLORREF);	//m_pCursor->GetCursorColor()

	m_pCursor->SetCursorColor(clrColor);

	memcpy(&clrColor, pDest+nRet, sizeof(COLORREF));
	nRet += sizeof(COLORREF);	//m_pCursor->GetTextColor()

	memcpy(&m_bNumber, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);		//m_bNumber

	memcpy(&m_bUseClipboard, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);		//m_bUseClipboard
	memcpy(&m_clrSelectBack, pDest+nRet, sizeof(COLORREF));
	nRet += sizeof(COLORREF);	//m_clrSelectBack
	memcpy(&m_clrSelectFont, pDest+nRet, sizeof(COLORREF));
	nRet += sizeof(COLORREF);	//m_clrSelectFont

	memcpy(&m_fLineSpace, pDest+nRet, sizeof(float));
	nRet += sizeof(float);	//m_fLineSpace
	memcpy(&m_bLineScroll, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);	//m_bLineScroll

	bool bUse = false;
	memcpy(&bUse, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);		//스크롤바 유무

	if(bUse)
	{
		CScrollBar *pScroll = new CScrollBar;
		nRet += sizeof(ENUM_OBJECTTYPE);
		nRet += pScroll->LoadClipSize(nVersion, pDest+nRet, pProject, bSetID);
		AttachScroll(pScroll);
	}

	m_pCursor->SetTextColor(clrColor);

	Create((int)m_rtLocalRect.GetWidth(), (int)m_rtLocalRect.GetHeight());
	SetText(strText);

	SetUseMouseWheel((m_cFontInfo.m_uFormat & DT_SINGLELINE)?false:true);

	return nRet;
}

bool CEdit::SaveInheritanceXml(TiXmlNode *pXmlObject)
{
	__super::SaveInheritanceXml(pXmlObject);

	std::string strText = GetText();
	CString strTemp;

	CMyMime64 myMime;
	myMime.SetString(strText,FALSE,TRUE);

	TiXmlElement xmlInheritance("Inheritance");
	xmlInheritance.SetAttribute("text",  myMime.GetString());
	xmlInheritance.SetAttribute("limit",  m_nLimitText);
	xmlInheritance.SetAttribute("pass",  m_bPassword?1:0);
	xmlInheritance.SetAttribute("blicktime",  (int)m_pCursor->GetBlinkTime());
	strTemp.Format("%06X", m_pCursor->GetCursorColor());
	xmlInheritance.SetAttribute("color",  strTemp.GetString());
	strTemp.Format("%06X", m_pCursor->GetTextColor());
	xmlInheritance.SetAttribute("textcolor",  strTemp.GetString());
	xmlInheritance.SetAttribute("number",  m_bNumber?1:0);
	xmlInheritance.SetAttribute("linespace",  (int)m_fLineSpace);
	xmlInheritance.SetAttribute("linescroll",  m_bLineScroll?1:0);

	xmlInheritance.SetAttribute("clipboard",  m_bUseClipboard?1:0);
	strTemp.Format("%06X", m_clrSelectBack);
	xmlInheritance.SetAttribute("selectback",  strTemp.GetString());
	strTemp.Format("%06X", m_clrSelectFont);
	xmlInheritance.SetAttribute("selectfont",  strTemp.GetString());

	m_cFontInfo.SaveInheritanceXml(pXmlObject);

	TiXmlElement xmlAttach("attach");
	if(m_pScrollBar) m_pScrollBar->SaveXml(&xmlAttach);
	pXmlObject->InsertEndChild(xmlAttach);

	pXmlObject->InsertEndChild(xmlInheritance);

	return true;
}

bool CEdit::LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject)
{
	__super::LoadInheritanceXml(nVersion, pXmlObject, pProject);

	std::string strTemp;
	int nTemp;
	COLORREF clrColor;
	TiXmlElement *pXmlInheritance = pXmlObject->FirstChildElement("Inheritance");
	GetAttributeMime64(pXmlInheritance, "text", strTemp);
	GetAttribute(pXmlInheritance, "limit", m_nLimitText);
	GetAttribute(pXmlInheritance, "pass", m_bPassword);
	GetAttribute(pXmlInheritance, "blicktime", nTemp);
	m_pCursor->SetBlinkTime((DWORD)nTemp);
	GetAttributeColor(pXmlInheritance, "color", clrColor);
	m_pCursor->SetCursorColor(clrColor);
	GetAttributeColor(pXmlInheritance, "textcolor", clrColor);
	m_pCursor->SetTextColor(clrColor);
	
	if(pXmlInheritance->Attribute("number"))
	{
		GetAttribute(pXmlInheritance, "number", m_bNumber);
	}
	if(pXmlInheritance->Attribute("linespace"))
	{
		int nLineSpace = 0;
		GetAttribute(pXmlInheritance, "linespace", nLineSpace);
		m_fLineSpace = (float)nLineSpace;
		GetAttribute(pXmlInheritance, "linescroll", m_bLineScroll);
	}

	if(pXmlInheritance->Attribute("clipboard"))
	{
		GetAttribute(pXmlInheritance, "clipboard", m_bUseClipboard);
		GetAttributeColor(pXmlInheritance, "selectback", m_clrSelectBack);
		GetAttributeColor(pXmlInheritance, "selectfont", m_clrSelectFont);
	}

	TiXmlElement *pXmlFont = pXmlObject->FirstChildElement("font");
	m_cFontInfo.LoadInheritanceXml(nVersion, pXmlFont, pProject);

	if(nVersion < 2)
	{
		m_cFontInfo.m_uFormat |= DT_SINGLELINE;
	}

	SetUseMouseWheel((m_cFontInfo.m_uFormat & DT_SINGLELINE)?false:true);

	TiXmlElement *pXmlAttach = pXmlObject->FirstChildElement("attach");
	if(pXmlAttach)
	{
		CObjectBase *pChildObject = NULL;
		TiXmlElement *pXmlChildObject = pXmlAttach->FirstChildElement("Object");
		while(pXmlChildObject)
		{
			pChildObject = pProject->LoadObject(nVersion, pXmlChildObject);
			switch(pChildObject->GetType())
			{
			case EOT_SCROLLBAR:
				AttachScroll((CScrollBar *)pChildObject);
				break;
			default:
				ASSERT(FALSE);
				break;
			}
			OnAttachChild(pChildObject);
			pChildObject->OnCreated();
			pXmlChildObject = pXmlChildObject->NextSiblingElement("Object");
		}
	}

	Create((int)m_rtLocalRect.GetWidth(), (int)m_rtLocalRect.GetHeight());
	SetText(strTemp);

	return true;
}

const int CEdit::GetLength(void)
{
	EDITLINELISTITER start_line = m_listLines.begin();
	EDITLINELISTITER end_line = m_itLineEnd;

	EDITITEMLISTITER start_item, end_item;
	size_t len = 0;

	CEditLine *pLine = NULL;
	CEditItem *pItem = NULL;

	for(;start_line != end_line;++start_line)
	{
		pLine = *start_line;
		start_item = pLine->begin();
		end_item = pLine->end();

		for(;start_item != end_item;++start_item)
		{
			pItem = *start_item;
			len += pItem->GetLength();
		}
	}

	return int(len);
}

void CEdit::Clear(void)
{
	EDITLINELISTITER start = m_listLines.begin();
	EDITLINELISTITER end = m_itLineEnd;

	for(;start != end;++start)
	{
		delete *start;
	}

	m_listLines.clear();

	m_cCurrentPos = m_itLineEnd;
	m_cSelectStart = m_itLineEnd;
	m_cSelectEnd = m_itLineEnd;
	m_cViewFirstItem = m_itLineEnd;
	m_cViewEndItem = m_itLineEnd;
	m_cDragFirst = m_itLineEnd;
	m_cDragEnd = m_itLineEnd;

	m_nScrollPos		= 0;
	m_ptStartPos		= CPointZero;

	m_bReCaculateItemWidth = false;

	SetCursorText("");

	SetCaculateCursorPos();
	SetRedraw();
}

void CEdit::SetObjectProject(CObjectProject *pProject)
{
	__super::SetObjectProject(pProject);
	m_pBack->SetTempEventDraw(m_bEventDraw);
}

void CEdit::SetRedraw(void)
{
	if(m_bRedraw) return;
	m_bRedraw = true;
	m_pBack->CheckEventObjectDraw();
}

bool CEdit::IsLineFeedChar(const char *pChar)
{
	if(pChar[0] == '\n') return true;
	return false;
}

void CEdit::InsertEditItem( CEditItem *pItem )
{
	//아직 아무것도 존재하지 않는다면
	//새라인을 만들고 그 라인에 아이템을 추가한다.
	float fCharWidth = pItem->GetWidth();
	float fWidth = m_rtLocalRect.GetWidth();

	bool bLineFeed = IsLineFeedChar(pItem->m_str);

	//싱글 라인일때 개행 문자는 무시한다.
	if((m_cFontInfo.m_uFormat & DT_SINGLELINE) && bLineFeed)
	{
		delete pItem;
		return;
	}

	//처음 문자를 입력할 경우 줄바꿈에 대한 고민을 하지 않는다.
	if(m_listLines.size() == 0)
	{
		//첫문자가 개행 문자일 경우 라인을 두개 추가한다.
		if(bLineFeed)
		{
			CEditLine *pLine = new CEditLine;
			m_listLines.push_back(pLine);
			m_cCurrentPos = m_listLines.begin();

			pLine->push_back(pItem);

			//개행 문자이므로 폭은 없다.
			*pLine = 0.0f;

			//신규 라인을 추가해준다.
			pLine = new CEditLine;
			m_listLines.push_back(pLine);
			m_cCurrentPos.NextLine();

			//라인만 추가 했으므로 폭은 존재하지 않는다.
			*pLine = 0;

			m_cCurrentPos = pLine->end();
		}
		else
		{
			CEditLine *pLine = new CEditLine;
			m_listLines.push_back(pLine);
			m_cCurrentPos = m_listLines.begin();

			pLine->push_back(pItem);
			m_cCurrentPos = pLine->end();

			//라인 추가시 그리기 모드가 그림자/아웃라인 일경우 전체폭의 앞쪽에 1을 더해준다.
			switch(m_cFontInfo.m_nDrawType)
			{
			case ETDT_SHADOW:
			case ETDT_OUTLINE:
				*pLine += 1.0f;
				break;
			}

			*pLine += fCharWidth;

			//라인 추가시 그리기 모드가 아웃라인 일 경우 전체폭의 뒤쪽에 1을 더해준다.
			switch(m_cFontInfo.m_nDrawType)
			{
			case ETDT_OUTLINE:
				*pLine += 1.0f;
				break;
			}
		}
	}
	//현재 커서 위치가 있는 라인이 존재한다면
	else if(m_cCurrentPos != m_itLineEnd)
	{
		CEditLine *pCurrentLine = m_cCurrentPos;
		//개행 문자일 경우에도 줄 길이에 따른 줄바꿈에 대한 고민을 하지 않는다.
		if(bLineFeed)
		{
			//현재 라인의 마직막에 커서가 있다면 신규로 라인을 추가해 준다.
			if(m_cCurrentPos == pCurrentLine->end())
			{
				//그 라인에 처음으로 문자가 입력 될때
				if(pCurrentLine->size() == 0)
				{
					//현재 위치에 개행문자을 입력해주고 다음 라인으로 이동한다.
					pCurrentLine->push_back(pItem);

					//신규 라인만을 생성한다.
					CEditLine *pLine = new CEditLine;
					m_cCurrentPos.NextLine();
					
					m_cCurrentPos = m_listLines.insert(m_cCurrentPos, pLine);
					m_cCurrentPos = pLine->end();
				}
				//그 라인의 마지막에 커서가 있다면 현재 위치에 개행 문자를 입력해주고
				else
				{
					pCurrentLine->push_back(pItem);

					//신규 라인만을 생성하고
					//마지막 라인이 아닐경우 개행 문자를 추가해준다.
					CEditLine *pLine = new CEditLine;
					m_cCurrentPos.NextLine();
					m_cCurrentPos = m_listLines.insert(m_cCurrentPos, pLine);			

					if(!IsLastLine(m_cCurrentPos))
					{
						CEditItem *pNewItem = new CEditItem;
						memcpy(pNewItem, pItem, sizeof(CEditItem));
						pLine->push_back(pNewItem);
						m_cCurrentPos = pLine->end();
						m_cCurrentPos.PrevItem();
					}
					else
					{
						m_cCurrentPos = pLine->end();
					}
				}
			}
			//현재 라인의 중간에 커서가 존재한다면 중간에서 신규로 라인을 추가해주고 커서의 위치를 뒤로 이동시킨다.
			else
			{
				//가장 앞에 커서가 있다면 현재 위치에 한줄을 추가해주고
				//개행 문자를 추가해준다.
				if(m_cCurrentPos == pCurrentLine->begin())
				{
					CEditLine *pLine = new CEditLine;
					pLine->push_back(pItem);

					m_cCurrentPos = m_listLines.insert(m_cCurrentPos, pLine);
					m_cCurrentPos.NextLine();
				}
				else
				{
					//현재 커서 뒤쪽의 아이템들을 구해서 임시 저장하고
					//새로운 라인을 생성해 둔다.
					EDITITEMLIST listTemp;
					listTemp.insert(listTemp.begin(), (EDITITEMLISTITER)m_cCurrentPos, pCurrentLine->end());

					CEditLine *pLine = new CEditLine;
					*pLine = listTemp;

					//라인 추가시 그리기 모드가 그림자/아웃라인 일경우 전체폭의 앞쪽에 1을 더해준다.
					switch(m_cFontInfo.m_nDrawType)
					{
					case ETDT_SHADOW:
					case ETDT_OUTLINE:
						*pLine += 1.0f;
						break;
					}

					EDITITEMLISTITER start_item = pLine->begin();
					EDITITEMLISTITER end_item = pLine->end();
					float fCharTotalWidth = 0;

					for(;start_item != end_item;++start_item)
					{
						fCharTotalWidth += (float)m_cFontInfo.m_nSpaceBetweenLetters + (*start_item)->m_rtRect.GetWidth();
					}
					fCharTotalWidth -= (float)m_cFontInfo.m_nSpaceBetweenLetters;

					switch(m_cFontInfo.m_nDrawType)
					{
					case ETDT_OUTLINE:
						*pLine += 1.0f;
						break;
					}
					*pLine += fCharTotalWidth;

					//다음 라인으로 가는 문자들을 제거하고 개행문자를 추가한다.
					pCurrentLine->erase(m_cCurrentPos, pCurrentLine->end());
					pCurrentLine->push_back(pItem);
					*pCurrentLine -= fCharTotalWidth;

					//새로운 라인을 추가하고, 새 라인의 가장 처음에 커서를 위치시킨다.
					m_cCurrentPos.NextLine();
					m_cCurrentPos = m_listLines.insert(m_cCurrentPos, pLine);
					m_cCurrentPos = pLine->begin();

					CheckDeleteMultiLine((EDITLINELISTITER)m_cCurrentPos, fWidth);
				}
			}
		}
		//개행 문자가 아닐 경우
		//멀티라인 일때 바이트일때 줄 바꿈에 대한 고민을 반드시 하여야 한다.
		else
		{
			//현재 라인의 마지막에 커서가 있다면 마지막에 추가해준다.
			if(m_cCurrentPos == pCurrentLine->end())
			{
				if(pCurrentLine->size() == 0)
				{
					switch(m_cFontInfo.m_nDrawType)
					{
					case ETDT_SHADOW:
					case ETDT_OUTLINE:
						*pCurrentLine += 1.0f;
						break;
					}

					switch(m_cFontInfo.m_nDrawType)
					{
					case ETDT_OUTLINE:
						*pCurrentLine += 1.0f;
						break;
					}

					*pCurrentLine += fCharWidth;
				}
				else
				{
					*pCurrentLine += (float)m_cFontInfo.m_nSpaceBetweenLetters + fCharWidth;
				}

				pCurrentLine->push_back(pItem);
				m_cCurrentPos = pCurrentLine->end();
			}
			//현재 라인의 중간에 커서가 존재한다면 중간에 추가해주고 커서의 위치를 뒤로 이동시킨다.
			else
			{
				m_cCurrentPos = pCurrentLine->insert(m_cCurrentPos, pItem);
				m_cCurrentPos.NextItem();

				//라인의 전체 폭에 자간 + 글자의 폭을 더해준다.
				*pCurrentLine += (float)m_cFontInfo.m_nSpaceBetweenLetters + fCharWidth;
			}

			//멀티 라인이라면 줄 바꿈에 대한 고민을 하여야 한다.
			CheckInsertMultiLine((EDITLINELISTITER)m_cCurrentPos, fWidth);
		}
	}
	//현재 커서의 위치가 없다면 추가해주지 않는다.
	else
	{
		ASSERT(FALSE);
	}

	m_bAddScroll = true;
	CheckItemTotalWidth();
}

//멀티 라인일 경우 만약 라인폭이 전체 폭보다 크다이면 라인 변경을 해준다.
void CEdit::CheckInsertMultiLine(EDITLINELISTITER &itLine, const float &fWidth)
{
	if(m_cFontInfo.m_uFormat & DT_SINGLELINE) return;
	//만약 표시 영역보다 라인폭이 크다면 마지막 문자를 다음 라인으로 옮겨 줘야 한다.
	CEditLine *pCurrentLine = *itLine;
	if(pCurrentLine->GetTotalWidth() > fWidth)
	{
		size_t nSize = pCurrentLine->size();
		if(nSize > 0)
		{
			bool bMoveCharCursor1 = false;
			bool bMoveCharCursor2 = false;

			//마지막 아이템을 구해온다.
			EDITITEMLISTITER end_item = pCurrentLine->end();
			--end_item;

			//라인의 가장 마지막 또는 커서가 있는 위치가 이동하였을 때 커서를
			//다음 라인 처음으롱 이동 시킨다.
			if((m_cCurrentPos == itLine) && (m_cCurrentPos == end_item))
				bMoveCharCursor1 = true;

			if((m_cCurrentPos == itLine) && (m_cCurrentPos == pCurrentLine->end()))
				bMoveCharCursor2 = true;

			//아이템 만큼의 폭은 전체에서 제거해준다.
			CEditItem *pItem = *end_item;
			bool bLineFeedChar = IsLineFeedChar(pItem->m_str);
			//마지막 문자가 개행 문자이면 같은 작업을 두번해준다.
			if(bLineFeedChar)
			{
				//개행문자는 문자의 폭이 없으므로 폭에 변화는 없다.
				//해당 라인에서 마지막 문자를 제거한다.
				pCurrentLine->erase(end_item);
			}
			else
			{
				//문자의 폭을 라인폭에서 제거해준다.
				*pCurrentLine -= pItem->GetWidth();
				if(nSize >= 2) *pCurrentLine -= (float)m_cFontInfo.m_nSpaceBetweenLetters;

				//해당 라인에서 마지막 문자를 제거한다.
				pCurrentLine->erase(end_item);
			}

			//마지막 문자를 추가할 라인을 구한다.
			EDITLINELISTITER itNewLine = itLine;
			++itNewLine;
			//다음 라인이 없다면 라인을 추가해준다.
			if(itNewLine == m_itLineEnd)
			{
				CEditLine *pLine = new CEditLine;
				itNewLine = m_listLines.insert(itNewLine, pLine);
				pLine->push_back(pItem);

				if(bLineFeedChar)
				{
					if(bMoveCharCursor1)
					{
						m_cCurrentPos = ((CEditLine *)m_cCurrentPos)->end();
					}

					if(bMoveCharCursor2)
					{
						m_cCurrentPos = itNewLine;
						m_cCurrentPos = (*itNewLine)->begin();
						if((*itNewLine)->size()) m_cCurrentPos.NextItem();
					}

					CheckInsertMultiLine(itLine, fWidth);
					return;
				}
				else
				{
					//라인 추가시 그리기 모드가 그림자/아웃라인 일경우 전체폭의 앞쪽에 1을 더해준다.
					switch(m_cFontInfo.m_nDrawType)
					{
					case ETDT_SHADOW:
					case ETDT_OUTLINE:
						*pLine += 1.0f;
						break;
					}

					*pLine += pItem->GetWidth();

					//라인 추가시 그리기 모드가 아웃라인 일 경우 전체폭의 뒤쪽에 1을 더해준다.
					switch(m_cFontInfo.m_nDrawType)
					{
					case ETDT_OUTLINE:
						*pLine += 1.0f;
						break;
					}
				}
			}
			//다음 라인이 있다면 그 라인의 처음에 아이템을 추가한다.
			else
			{
				if(bLineFeedChar)
				{
					CEditLine *pLine = new CEditLine;
					itNewLine = m_listLines.insert(itNewLine, pLine);
					pLine->push_back(pItem);

					if(bMoveCharCursor1)
					{
						m_cCurrentPos = ((CEditLine *)m_cCurrentPos)->end();
					}

					if(bMoveCharCursor2)
					{
						m_cCurrentPos = itNewLine;
						m_cCurrentPos = (*itNewLine)->begin();
						if((*itNewLine)->size()) m_cCurrentPos.NextItem();
					}

					CheckInsertMultiLine(itLine, fWidth);
					return;
				}
				else
				{
					CEditLine *pNewLine = *itNewLine;
					size_t nSize = pNewLine->size();
					if(nSize == 0)
					{
						switch(m_cFontInfo.m_nDrawType)
						{
						case ETDT_SHADOW:
						case ETDT_OUTLINE:
							*pNewLine += 1.0f;
							break;
						}

						switch(m_cFontInfo.m_nDrawType)
						{
						case ETDT_OUTLINE:
							*pNewLine += 1.0f;
							break;
						}
					}

					*pNewLine += pItem->GetWidth();
					if(nSize >= 2) *pNewLine += (float)m_cFontInfo.m_nSpaceBetweenLetters;

					pNewLine->push_front(pItem);

					CheckInsertMultiLine(itNewLine, fWidth);
				}
			}

			if(bMoveCharCursor1)
			{
				m_cCurrentPos = ((CEditLine *)m_cCurrentPos)->end();
			}

			if(bMoveCharCursor2)
			{
				m_cCurrentPos = itNewLine;
				m_cCurrentPos = (*itNewLine)->begin();
				if((*itNewLine)->size()) m_cCurrentPos.NextItem();
			}

			CheckInsertMultiLine(itLine, fWidth);
		}
	}
}

void CEdit::DeleteEditItem( bool bFront )
{
	//문자가 없다면 삭제를 하지 않는다.
	if(m_listLines.size() == 0) return;
	//현재 커서의 현재 위치가 없다면 무시한다.
	if(m_cCurrentPos == m_itLineEnd) return;

	CEditLine *pCurrentLine = m_cCurrentPos;

	//커서의 위치가 현재 라인의 끝일 경우
	if(m_cCurrentPos == pCurrentLine->end())
	{
		//앞문자를 지울경우
		if(bFront)
		{
			//현재 빈 라인일 경우, 가장 마지막 라인을 의미한다.
			//따라서 삭제후 줄 바꿈에 대한 고민은 하지 않는다.
			if(pCurrentLine->size() == 0)
			{
				//현재 라인을 삭제 한다.
				delete pCurrentLine;
				m_cCurrentPos = m_listLines.erase(m_cCurrentPos);

				//모든 라인이 삭제 되어을 때
				if(m_listLines.size() == 0)
				{
					m_cCurrentPos = m_itLineEnd;
					return;
				}

				//이전 라인으로 이동한다
				m_cCurrentPos.PrevLine();
				pCurrentLine = m_cCurrentPos;

				size_t nSize = pCurrentLine->size();
				ASSERT(nSize != 0);

				m_cCurrentPos = pCurrentLine->end();
				m_cCurrentPos.PrevItem();

				CEditItem *pItem = m_cCurrentPos;
				//마지막 문자가 개행 문자일 때 폭의 변화는 없다.
				if(IsLineFeedChar(pItem->m_str))
				{
				}
				else
				{
					*pCurrentLine -= pItem->GetWidth();
					if(nSize >= 2) *pCurrentLine -= (float)m_cFontInfo.m_nSpaceBetweenLetters;
				}

				//해당 마지막 아이템을 지우고 이전라인 가장 뒷부분으로 커서를 이동 시킨다.
				delete pItem;

				pCurrentLine->erase(m_cCurrentPos);
				m_cCurrentPos = pCurrentLine->end();
			}
			else
			{
				//현재 라인의 마지막을 제거해준다.
				//현재 라인 뒷부분에 대한 줄 바꿈에 대해서 고민하여야 한다.
				size_t nSize = pCurrentLine->size();
				ASSERT(nSize != 0);

				m_cCurrentPos.PrevItem();

				CEditItem *pItem = m_cCurrentPos;
				*pCurrentLine -= pItem->GetWidth();
				if(nSize >= 2) *pCurrentLine -= (float)m_cFontInfo.m_nSpaceBetweenLetters;

				delete pItem;

				pCurrentLine->erase(m_cCurrentPos);
				m_cCurrentPos = pCurrentLine->end();

				CheckDeleteMultiLine((EDITLINELISTITER)m_cCurrentPos, m_rtLocalRect.GetWidth());
			}
		}
		//뒷문자를 지울경우
		else
		{
			//현재 빈 라인일 경우, 가장 마지막 라인을 의미한다.
			//따라서 아무 해동도 하지 않는다.
			if(pCurrentLine->size() == 0)
			{
				return;
			}
			//다음 라인의 가장 처음 문자를 제거해준다. 커서는 이동하지 않는다.
			//현재 라인 뒷부분에 대한 줄 바꿈에 대해서 고민하여야 한다.
			else
			{
				EDITLINELISTITER itLineTemp = m_cCurrentPos;
				++itLineTemp;
				//현재 커서의 위치가 마지막 라인이면 아무것도 하지 않는다.
				if(itLineTemp == m_itLineEnd)
				{
					return;
				}

				pCurrentLine = *itLineTemp;
				size_t nSize = pCurrentLine->size();
				ASSERT(nSize != 0);

				EDITITEMLISTITER itItemTemp = pCurrentLine->begin();
				CEditItem *pItem = *itItemTemp;

				*pCurrentLine -= pItem->GetWidth();
				if(nSize >= 2) *pCurrentLine -= (float)m_cFontInfo.m_nSpaceBetweenLetters;

				delete pItem;

				CheckDeleteMultiLine((EDITLINELISTITER)m_cCurrentPos, m_rtLocalRect.GetWidth());
			}
		}
	}
	//커서의 위치가 현재 라인의 가장 앞일 경우
	else if(m_cCurrentPos == pCurrentLine->begin())
	{
		//앞문자를 제거해준다.
		if(bFront)
		{
			//가장 첫 라인이면 아무동작도 하지 않는다.
			if(IsFirstLine(m_cCurrentPos)) return;

			//이전 라인의 가장 마지막을 제거해주고, 커서를 이전 라인의 가장 뒤로 이동시킨다.
			m_cCurrentPos.PrevLine();
			pCurrentLine = m_cCurrentPos;
			m_cCurrentPos = pCurrentLine->end();
			m_cCurrentPos.PrevItem();
			CEditItem *pItem = m_cCurrentPos;
			size_t nSize = pCurrentLine->size();
			//마지막 문자가 개행 문자일 때 폭의 변화는 없다.
			if(IsLineFeedChar(pItem->m_str))
			{
				int i=0;
			}
			else
			{
				*pCurrentLine -= pItem->GetWidth();
				if(nSize >= 2) *pCurrentLine -= (float)m_cFontInfo.m_nSpaceBetweenLetters;
			}

			pCurrentLine->erase(m_cCurrentPos);
			m_cCurrentPos = pCurrentLine->end();

			delete pItem;

			if(pCurrentLine->size() == 0)
			{
				delete pCurrentLine;
				m_cCurrentPos = m_listLines.erase(m_cCurrentPos);
				m_cCurrentPos = ((CEditLine *)m_cCurrentPos)->begin();
			}
		}
		//뒷분자를 제거해준다.
		else
		{
			CEditItem *pItem = m_cCurrentPos;
			//처음 문자가 개행문자일 경우
			//현재 라인을 제거해주고 커서위치를 다음라인으로 이동한다.
			if(IsLineFeedChar(pItem->m_str))
			{
				delete pCurrentLine;
				m_cCurrentPos = m_listLines.erase(m_cCurrentPos);
				pCurrentLine = m_cCurrentPos;
				m_cCurrentPos = pCurrentLine->begin();
			}
			else
			{
				size_t nSize = pCurrentLine->size();
				*pCurrentLine -= pItem->GetWidth();
				if(nSize >= 2) *pCurrentLine -= (float)m_cFontInfo.m_nSpaceBetweenLetters;

				m_cCurrentPos = pCurrentLine->erase(m_cCurrentPos);
				delete pItem;
			}
		}

		CheckDeleteMultiLine((EDITLINELISTITER)m_cCurrentPos, m_rtLocalRect.GetWidth());

		pCurrentLine = m_cCurrentPos;
		if(m_cCurrentPos != pCurrentLine->begin())
		{
			EDITPOS cPos= m_cCurrentPos;
			CEditItem *pItem  = *cPos.PrevItem();
			if(IsLineFeedChar(pItem->m_str))
			{
				m_cCurrentPos = cPos;
			}
		}
	}
	//커서의 위치가 현재 라인의 중간일 경우
	else
	{
		//앞문자를 지울경우
		if(bFront)
		{
			m_cCurrentPos.PrevItem();
			CEditItem *pItem = m_cCurrentPos;

			size_t nSize = pCurrentLine->size();
			*pCurrentLine -= pItem->GetWidth();
			if(nSize >= 2) *pCurrentLine -= (float)m_cFontInfo.m_nSpaceBetweenLetters;

			m_cCurrentPos = pCurrentLine->erase(m_cCurrentPos);
			delete pItem;
		}
		//뒷문자를 지울경우
		else
		{
			CEditItem *pItem = m_cCurrentPos;

			if(IsLineFeedChar(pItem->m_str))
			{
			}
			else
			{
				size_t nSize = pCurrentLine->size();
				*pCurrentLine -= pItem->GetWidth();
				if(nSize >= 2) *pCurrentLine -= (float)m_cFontInfo.m_nSpaceBetweenLetters;
			}

			m_cCurrentPos = pCurrentLine->erase(m_cCurrentPos);
			delete pItem;
		}

		CheckDeleteMultiLine((EDITLINELISTITER)m_cCurrentPos, m_rtLocalRect.GetWidth());

		pCurrentLine = m_cCurrentPos;
		if(m_cCurrentPos != pCurrentLine->begin())
		{
			EDITPOS cPos= m_cCurrentPos;
			CEditItem *pItem  = *cPos.PrevItem();
			if(IsLineFeedChar(pItem->m_str))
			{
				m_cCurrentPos = cPos;
			}
		}
	}

	CheckItemTotalWidth();
}

//현재 라인의 값이 아이템이 삭제되었기 때문에 현재 라인 뒤로 위치 변경에 대해서 검토 하여야 한다.
void CEdit::CheckDeleteMultiLine(EDITLINELISTITER &itLine, const float &fWidth)
{
	if(m_cFontInfo.m_uFormat & DT_SINGLELINE) return;
	if(itLine == m_itLineEnd) return;

	CEditLine *pCurrentLine = *itLine;
	//가장 마지막 라인일 경우 아무것도 하지 않는다.
	if(pCurrentLine->size() == 0) return;
	EDITLINELISTITER itTempLine = itLine;
	++itTempLine;
	if(itTempLine == m_itLineEnd) return;

	//현재 라인의 마지막이 개행 문자일 경우 무시한다.
	EDITITEMLISTITER itTempItem = pCurrentLine->end();
	--itTempItem;
	CEditItem *pItem = *itTempItem;
	if(IsLineFeedChar(pItem->m_str)) return;

	//다음 라인의 첫번째 문자를 구해온다.
	CEditLine *pNextLine = *itTempLine;
	itTempItem = pNextLine->begin();
	//다음라인에 첫번째 문자가 없다면 해당 라인을 삭제한다.
	if(itTempItem == pNextLine->end())
	{
		delete pNextLine;
		itTempLine = m_listLines.erase(itTempLine);
		CheckDeleteMultiLine(itTempLine, fWidth);
		return;
	}
	pItem = *itTempItem;

	bool bLineFeed = IsLineFeedChar(pItem->m_str);
	//다음 라인의 첫번째 문자가 개행 문자일경우 현재라인의 가장 마지막에 추가해주고
	//다음 라인을 삭제한다.
	if(bLineFeed)
	{
		//다음 라인의 첫번째 문자를 지운다.
		pNextLine->erase(itTempItem);
		delete pNextLine;
		m_listLines.erase(itTempLine);

		//현재 라인의 끝에 문자를 추가한다.
		pCurrentLine->push_back(pItem);
		return;
	}

	size_t nSize = pCurrentLine->size();
	float fTotalWidth = pCurrentLine->GetTotalWidth();
	if(nSize == 0)
	{
		//다음 라인의 첫번째 문자가 개행 문자일 경우 무시
		switch(m_cFontInfo.m_nDrawType)
		{
		case ETDT_SHADOW:
		case ETDT_OUTLINE:
			++fTotalWidth;
			break;
		}

		fTotalWidth += pItem->m_rtRect.GetWidth();

		switch(m_cFontInfo.m_nDrawType)
		{
		case ETDT_OUTLINE:
			++fTotalWidth;
			break;
		}
	}
	else
	{
		fTotalWidth += pItem->m_rtRect.GetWidth();
		fTotalWidth += m_cFontInfo.m_nSpaceBetweenLetters;
	}

	//추가된 폭이 전체 폭이라 같거나 작다면 마지막에 추가한다.
	if(fTotalWidth <= fWidth)
	{
		//다음 라인의 첫번째 문자를 지운다.
		pNextLine->erase(itTempItem);
		nSize = pNextLine->size();

		if(nSize == 0)
		{
			*pNextLine = 0.0f;
		}
		else
		{
			*pNextLine -= pItem->GetWidth();
			if(nSize >= 2)
			{
				*pNextLine -= (float)m_cFontInfo.m_nSpaceBetweenLetters;
			}
		}

		//현재 라인의 끝에 문자를 추가한다.
		pCurrentLine->push_back(pItem);
		*pCurrentLine += pItem->GetWidth();
		*pCurrentLine += (float)m_cFontInfo.m_nSpaceBetweenLetters;

		//현재 커서의 위치가 가장 뒤일때 이면서
		//라인이 추가될때 커서의 끝에서 첫번째 문자로 이동 시킨다.
		if(m_cCurrentPos == itLine && m_cCurrentPos == pCurrentLine->end())
		{
			m_cCurrentPos.PrevItem();
		}

		CheckDeleteMultiLine(itLine, fWidth);
	}
	else
	{
		++itLine;
		if(itLine != m_itLineEnd)
		{
			CheckDeleteMultiLine(itLine, fWidth);
		}
	}
}

bool CEdit::DeleteSelectItem( void )
{
	//선택된 라인이 없을경우 또는 시작과 끝이 같을 경우 아무동작도 하지 않는다.
	if(m_cSelectStart == m_itLineEnd || m_cSelectStart == m_cSelectEnd) return false;
	//시작라인과 끝라인을 구해온다.
	CEditLine *pLineStart = m_cSelectStart;
	CEditLine *pLineEnd = m_cSelectEnd;
	CEditItem *pItem = NULL;

	EDITLINELISTITER firstline = m_cSelectStart;

	//현재 선택의 이전 아이템을 구한다.
	EDITPOS cPrevItem;
	bool bFirstItem = IsFirstItem(m_cSelectStart);
	if(!bFirstItem)	GetPrevItem(m_cSelectStart, cPrevItem);
	
	//선택된 시작 라인과 끝나인이 같을 경우
	if(pLineStart == pLineEnd)
	{
		EDITITEMLISTITER start = m_cSelectStart;
		EDITITEMLISTITER end = m_cSelectEnd;
		while(start != end)
		{
			pItem = *start;
			if(IsLineFeedChar(pItem->m_str))
			{

			}
			else
			{
				*pLineStart -= pItem->GetWidth();
				*pLineStart -= (float)m_cFontInfo.m_nSpaceBetweenLetters;
			}
			delete pItem;
			start = pLineStart->erase(start);
		}

		//해당 라인이 모두 삭제되었다면 해당 라인을 삭제한다.
		if(pLineStart->size() == 0 && !IsLastLine(m_cSelectStart))
		{
			delete pLineStart;
			m_cSelectStart = m_listLines.erase(m_cSelectStart);
			//모든 라인이 삭제 되었다면
			if(m_listLines.size() == 0)
			{
				m_cSelectStart = m_itLineEnd;
			}
			//첫번째 라인이 아니라면 이전 라인으로 이동한다.
			else if(!IsFirstLine(m_cSelectStart))
			{
				//이전 라인의 마지막으로 이동한다.
				m_cSelectStart.PrevLine();
				m_cSelectStart = ((CEditLine *)m_cSelectStart)->end();
			}
		}
		//해당 라인이 모두 삭제되지 않았다면 지원진 위치에 커서를 위치시킨다.
		else
		{
			m_cSelectStart = start;
		}

		firstline = m_cSelectStart;
	}
	//선택된 시작 라인과 끝나인이 다를경우
	else
	{
		EDITLINELISTITER start_line = m_cSelectStart;
		EDITLINELISTITER end_line = m_cSelectEnd;

		//시작라인의 처음부터 끝까지를 제거 해준다.
		EDITITEMLISTITER start_item = m_cSelectStart;
		EDITITEMLISTITER end_item = pLineStart->end();
		while(start_item != end_item)
		{
			pItem = *start_item;
			if(IsLineFeedChar(pItem->m_str))
			{

			}
			else
			{
				*pLineStart -= pItem->GetWidth();
				*pLineStart -= (float)m_cFontInfo.m_nSpaceBetweenLetters;
			}
			delete pItem;
			start_item = pLineStart->erase(start_item);
		}

		//해당 라인이 모두 삭제되었다면 해당 라인을 삭제한다.
		if(pLineStart->size() == 0)
		{
			delete pLineStart;
			start_line = m_listLines.erase(start_line);
			//해당 라인이 모두 제거 되었기 때문에
			//줄바꿈 체크를 다음 라인부토 하도록 수정
			firstline = start_line;
		}
		//해당 라인이 모두 삭제되지 않았다면 다음 라인으로 이동한다.
		else
		{
			++start_line;
		}

		bool bChangeLine = false;
		//다음 라인에서 마지막 직전 라인까지는 모든 라인을 제거한다.
		while(start_line != end_line)
		{
			if(firstline == start_line) bChangeLine = true;
			else bChangeLine = false;

			delete *start_line;
			start_line = m_listLines.erase(start_line);

			if(bChangeLine) firstline = start_line;
		}

		//마지막 라인은 처음 부터 현재까지를 제거해준다.
		start_item = pLineEnd->begin();
		end_item = m_cSelectEnd;
		while(start_item != end_item)
		{
			pItem = *start_item;
			if(IsLineFeedChar(pItem->m_str))
			{

			}
			else
			{
				*pLineEnd -= pItem->GetWidth();
				*pLineEnd -= (float)m_cFontInfo.m_nSpaceBetweenLetters;
			}
			delete pItem;
			start_item = pLineEnd->erase(start_item);
		}

		//마지막 라인이 모두 삭제되었다면
		if(pLineEnd->size() == 0)
		{
			//첫라인 부터 마지막 라인까지 모든 아이템이 삭제되었다면
			if(m_listLines.size() > 1 && firstline == m_cSelectEnd)
			{
				//CEditItem *pItem = new CEditItem;
				//pItem->m_str[0] = '\n';
				//pLineEnd->push_back(pItem);
			}
			else
			{
				delete pLineEnd;
				m_cSelectEnd = m_listLines.erase(m_cSelectEnd);
				//모든 라인이 삭제 되었다면
				if(m_listLines.size() == 0)
				{
					m_cSelectEnd = m_itLineEnd;
					firstline = m_itLineEnd;
					m_ptStartPos = CPointZero;
				}
				//첫번째 라인이 아니라면 이전 라인으로 이동한다.
				else if(!IsFirstLine(m_cSelectEnd))
				{
					//이전 라인의 마지막으로 이동한다.
					m_cSelectEnd.PrevLine();
					m_cSelectEnd = ((CEditLine *)m_cSelectEnd)->end();
				}
			}
		}
		//해당 라인이 모두 삭제되지 않았다면 지원진 위치에 커서를 위치시킨다.
		else
		{
			m_cSelectEnd = start_item;
		}

		m_cSelectStart = m_cSelectEnd;
	}

	m_cSelectStart = m_itLineEnd;
	m_cSelectEnd = m_itLineEnd;

	//다음 아이템으로 이동한다.
	if(!bFirstItem) m_cCurrentPos = cPrevItem;
	else m_cCurrentPos = GetFirstPos();

	CheckDeleteMultiLine(firstline, m_rtLocalRect.GetWidth());

	if(!bFirstItem) GetNextItem(cPrevItem, m_cCurrentPos);
	else m_cCurrentPos = GetFirstPos();

	CheckItemTotalWidth();
	m_pCursor->SetShow(true);
	return true;
}

void CEdit::CheckItemTotalWidth(void)
{
	SetReCaculateItemWidth();
	SetCaculateCursorPos();
	SetRedraw();
}

void CEdit::OnMakeAddChars(char const * ach)
{
	size_t len = strlen(ach);
	for(size_t i=0; i<len;)
	{
		char one_char[3];

		if(IsDBCSLeadByte(ach[i]) && (i+1)<len)
		{
			one_char[0] = ach[i];
			one_char[1] = ach[i+1];
			one_char[2] = '\0';
			i += 2;
		}
		else
		{
			one_char[0] = ach[i];
			one_char[1] = '\0';
			++i;
		}
		OnAddChar(one_char);
		m_bAddScroll = true;
	}
}

void CEdit::OnAddChar(const std::string &strChar, bool bForced /*= false*/ )
{
	if((bForced == false && m_bEnable == false))
		return;

	if((m_cFontInfo.m_uFormat & DT_SINGLELINE) && IsLineFeedChar(strChar.c_str())) return;

	bool bSelectDelete = DeleteSelectItem();

	//추가되는 글자의 갯수를 구한다.
	int len = int(strChar.length());
	if( len <= 0 )
		return;

	if(m_bNumber)
	{
		if(len != 1) return;
		char ch = strChar[0];
		if(ch < '0' || ch > '9') return;
	}

	//추가되는 글자를 넣었을때 최대 글자수가 넘는지 구한다.
	if( m_nLimitText < (GetLength() + len) )
		return;

	//멀티 바이트가 개별 바이트로 입력되었는지를 검사하는 변수
	bool		bMerged = false;
	CEditItem	*pItem = new CEditItem;
	CEditItem	*pPrevItem = NULL;

	// 멀티바이트 캐릭터가 하나씩 들어올 때 처리하는 부분, 싱글 바이트가 들어왔을때
	if(len == 1 && m_cCurrentPos != m_itLineEnd)
	{
		//현재 커서의 전 문자를 찾는다.
		CEditLine *pCurrentLine = m_cCurrentPos;
		if(pCurrentLine->size() && (m_cCurrentPos != pCurrentLine->begin()))
		{
			EDITITEMLISTITER itTempItem = m_cCurrentPos;
			--itTempItem;

			pPrevItem = *itTempItem;
		}
		else if(IsFirstLine(m_cCurrentPos))
		{
			pPrevItem = NULL;
		}
		else
		{
			EDITLINELISTITER itTempLine = m_cCurrentPos;
			--itTempLine;

			EDITITEMLISTITER itTempItem = (*itTempLine)->end();
			--itTempItem;

			pPrevItem = *itTempItem;
		}

		if(pPrevItem != 0)
		{
			//넣으려는 곳의 글자가 싱글바이트 일경우
			if( strlen(pPrevItem->m_str) == 1 )
			{
				char chSingleByte = pPrevItem->m_str[0];
				//멀티 바이트 문자의 첫번째 바이트 인지를 검사
				if(::IsDBCSLeadByte(chSingleByte))
				{
					//새 문자 아이템에 뒷 부분을 채워준다.
					pItem->m_str[0] = chSingleByte;
					strcpy_s(pItem->m_str + 1, 2, strChar.c_str());

					//이전 문자 item을 삭제한다.
					DeleteEditItem(true);
					bMerged = true;
				}
			}
		}
	}

	//멀티바이트가 개발 바이트로 들어오지 않았을 경우
	if(!bMerged)
	{
		//새로운 문자 아이템에 값을 채워준다.
		strcpy_s( pItem->m_str, 3, strChar.c_str() );
	}

	SIZE size;

	HDC dc = GetDC(NULL);
	HGDIOBJ hOldFont = ::SelectObject( dc, m_cFontInfo.GetFont()->m_hObject );
	int nOldSpaceBetweenLetters = ::SetTextCharacterExtra( dc, m_cFontInfo.m_nSpaceBetweenLetters);

	bool bLineFeed = IsLineFeedChar(pItem->m_str);
	//문자열을 길이를 구해서 아이템에 채워준다.
	if(bLineFeed)
	{
		::GetTextExtentPoint32( dc, "A", 1, &size );
		size.cx = 0;
	}
	else if( m_bPassword == true )
		::GetTextExtentPoint32( dc, PASSWORD_CHAR, (int)strlen(PASSWORD_CHAR), &size );
	else
		::GetTextExtentPoint32( dc, pItem->m_str, (int)strlen(pItem->m_str), &size );


	::SelectObject( dc, hOldFont );
	::SetTextCharacterExtra( dc, nOldSpaceBetweenLetters);

	::ReleaseDC(NULL, dc);

	//아이템의 폭을 구한다.
	pItem->m_rtRect.SetRect(0,0,(float)size.cx,(float)size.cy);

	//아이템을 추가한다.
	InsertEditItem( pItem );
	m_pCursor->ResetTimer();
}

bool CEdit::IsByteToMerge(char const* pcchInput) const
{
	if(m_bNumber) return false;
	if(m_cCurrentPos == m_itLineEnd) return false;

	CEditLine *pLine = m_cCurrentPos;
	if(strlen(pcchInput) == 1) // 이 글자가 싱글바이트이고, 
	{
		// 앞에 글자가 있고,
		if(pLine->size() && (m_cCurrentPos != pLine->begin()))
		{
			EDITPOS cPrevPos = m_cCurrentPos;
			cPrevPos.PrevItem();

			CEditItem *pPrevItem = cPrevPos;
			if( strlen(pPrevItem->m_str)==1 ) // 그 글자가 싱글바이트이고, 
			{
				char single_byte = pPrevItem->m_str[0];
				if(::IsDBCSLeadByte(single_byte)) // LeadByte일 때
				{
					return true;
				}
			}
		}
	}
	return false;
}

CEditItem *CEdit::GetItem(EDITPOS &cPos)
{
	if(cPos == m_itLineEnd) return NULL;
	CEditLine *pLine = cPos;
	if(cPos == pLine->end()) return NULL;
	return cPos;
}

CEditItem *CEdit::GetPrevItem(EDITPOS &cPos, OUT EDITPOS &cPrevPos)
{
	cPrevPos = cPos;
	//선택된 라인이 없으면 구하지 않는다.
	if(cPrevPos == m_itLineEnd)
	{
		return NULL;
	}

	//현재 라인에 아이템이 존재하지 않는다면
	//이전 라인의 가장 마지막 아이템을 구한다.
	CEditLine *pLine = cPrevPos; 
	if(pLine->size() == 0)
	{
		//현재 라인이 가장 첫 라인이면 이전 아이템을 존재하지 않는다.
		if(IsFirstLine(cPrevPos))
		{
			cPrevPos = m_itLineEnd;
			return NULL;
		}

		//이전 라인을 가장 마지막 아이템을 구한다.
		cPrevPos.PrevLine();
		pLine = cPrevPos;
		cPrevPos = pLine->end();
		cPrevPos.PrevItem();
		return cPrevPos;
	}

	pLine = cPrevPos;
	//현재 라인에 아이템이 존재할때
	//현재 라인의 가장 첫 아이템일 경우
	if(cPrevPos == pLine->begin())
	{
		//현재 라인이 가장 첫 라인이면 이전 아이템을 존재하지 않는다.
		if(IsFirstLine(cPrevPos))
		{
			cPrevPos = m_itLineEnd;
			return NULL;
		}
		//이전 라인을 가장 마지막 아이템을 구한다.
		
		cPrevPos.PrevLine();
		pLine = cPrevPos;
		cPrevPos = pLine->end();
		cPrevPos.PrevItem();
		return cPrevPos;
	}

	cPrevPos.PrevItem();
	return cPrevPos;
}

CEditItem *CEdit::GetNextItem(EDITPOS &cPos, OUT EDITPOS &cNextPos)
{
	cNextPos = cPos;
	//선택된 라인이 없으면구하지 않는다.
	if(cNextPos == m_itLineEnd)
	{
		return NULL;
	}

	CEditLine *pLine = cNextPos;
	//현재 라인에 아이템이 없다면 가장 마지막 이므로 다음 위치는 존재하지 않는다.
	if(pLine->size() == 0)
	{
		cNextPos = m_itLineEnd;
		return NULL;
	}

	//현재 라인에 아이템이 존재할때
	//현재 라인의 가장 뒷 아이템일 경우
	if(cNextPos == pLine->end())
	{
		EDITITEMLISTITER end_item = pLine->end();
		--end_item;
		CEditItem *pItem = *end_item;

		//그 라인의 마지막이 개행 문자인지 조사
		bool bLineFeed = IsLineFeedChar(pItem->m_str);

		//다음 라인으로 이동
		cNextPos.NextLine();
		//다음 라인이 존재하지 않는다면 아이템을 구하지 않고 이전 위치로 되돌린다.
		if(cNextPos == m_itLineEnd)
		{
			cNextPos.PrevLine();
			return NULL;
		}

		pLine = cNextPos;
		//다음 라인에 아이템이 없다면 라인만 변경하고 아이템을 넘겨주지 않는다.
		if(pLine->size() == 0)
		{
			cNextPos = pLine->end();
			return NULL;
		}

		//다음 라인의 가장 처음 아이템을 구해서 넘겨준다.
		cNextPos = pLine->begin();
		//개행문자가 아니면 다음 위치로 이동시킨다.
		if(!bLineFeed) cNextPos.NextItem();
		return cNextPos;
	}

	//다음 아이템으로 이동
	cNextPos.NextItem();
	//그 라인의 가장 마지막 일경우 다음 라인으로 이동
	if(cNextPos == pLine->end())
	{
		cNextPos.NextLine();
		//가장 마지막 라인이었다면
		//이전 라인의 가장 뒷부분으로 이동
		if(cNextPos == m_listLines.end())
		{
			cNextPos.PrevLine();
			cNextPos = pLine->end();
			return NULL;
		}

		//가장 마지막 라인이 아이였다면
		//다음 라인의 가장 앞으로 이동
		pLine = cNextPos;
		//만약 빈 라인이라면
		if(pLine->size() == 0)
		{
			cNextPos = pLine->end();
			return NULL;
		}

		cNextPos = pLine->begin();
	}
	else
	{
		//마지막은 아니지만 개행 문자일 경우 다음 라인의 처음으로 이동한다.
		CEditItem *pItem = cNextPos;
		bool bLineFeed = IsLineFeedChar(pItem->m_str);
		if(bLineFeed)
		{
			cNextPos.NextLine();
			//가장 마지막 라인이었다면
			//이전 라인의 가장 뒷부분으로 이동
			if(cNextPos == m_listLines.end())
			{
				cNextPos.PrevLine();
				cNextPos = pLine->end();
				return NULL;
			}

			//가장 마지막 라인이 아이였다면
			//다음 라인의 가장 앞으로 이동
			pLine = cNextPos;
			//만약 빈 라인이라면
			if(pLine->size() == 0)
			{
				cNextPos = pLine->end();
				return NULL;
			}

			cNextPos = pLine->begin();
		}
	}
	return cNextPos;
}

bool CEdit::IsFirstItem(EDITPOS &cPos)
{
	//입력이 없으면 처음이다.
	if(m_listLines.size() == 0) return true;
	//첫번째 라인이 아니면 첫문자가 아니다.
	if(!IsFirstLine(cPos)) return false;
	//라인에 아이템이 없으면 첫문자이다.
	CEditLine *pLine = cPos;
	if(pLine->size() == 0) return true;
	//라인의 첫번째 문자이면 첫문자이다.
	if(cPos == pLine->begin()) return true;
	return false;
}

bool CEdit::IsLastItem(EDITPOS &cPos)
{
	//입력이 없으면 처음이다.
	if(m_listLines.size() == 0) return true;
	//마지막 라인이 아니면 마지막 문자가 아니다.
	if(!IsLastLine(cPos)) return false;
	//라인에 아이템이 없으면 마지막 아이템이다.
	CEditLine *pLine = cPos;
	if(pLine->size() == 0) return true;
	//라인의 마지막 문자인지 검사
	if(cPos == pLine->end()) return true;
	return false;
}

EDITPOS	CEdit::GetFirstPos(void)
{
	EDITPOS itFirst;
	itFirst = m_itLineEnd;
	if(m_listLines.size() == 0) return itFirst;
	itFirst = m_listLines.begin();
	itFirst = ((CEditLine *)itFirst)->begin();
	return itFirst;
}

EDITPOS	CEdit::GetLastPos(void)
{
	EDITPOS itFirst;
	itFirst = m_itLineEnd;
	if(m_listLines.size() == 0) return itFirst;
	itFirst = m_listLines.end();
	itFirst.PrevLine();
	itFirst = ((CEditLine *)itFirst)->end();
	return itFirst;
}

bool CEdit::IsFirstLine(EDITPOS &cPos)
{
	//입력이 없으면 처음이다. 무조건 처음 라인인다.
	if(m_listLines.size() == 0) return true;
	//첫번째 라인인지를 검사한하다.
	if(cPos == m_listLines.begin()) return true;
	return false;
}

bool CEdit::IsLastLine(EDITPOS &cPos)
{
	//입력이 없으면 마지막 라인이다.
	if(m_listLines.size() == 0) return true;
	//첫번째 라인인지를 검사한하다.
	if(cPos == m_listLines.end()) return true;
	EDITPOS cTempPos = cPos;
	cTempPos.NextLine();
	if(cTempPos == m_listLines.end()) return true;
	return false;
}

bool CEdit::OnSpecialKeyDown(const WPARAM &wParam, const LPARAM &lParam)
{
	int nScanCode = CObjectMan::GetScanCode(wParam, lParam);

	bool bPushCtrl = false;
	bool bPushShift = false;
	if(GetAsyncKeyState(VK_LCONTROL) & 0x8000) bPushCtrl = true;
	if(GetAsyncKeyState(VK_LSHIFT) & 0x8000) bPushShift = true;

	switch( nScanCode )
	{
	case VK_S_LEFT:
	case VK_S_RIGHT:
	case VK_S_UP:
	case VK_S_DOWN:
	case VK_S_HOME:
	case VK_S_END:
	case VK_S_PAGEDOWN:
	case VK_S_PAGEUP:
	case VK_S_BS:
	case VK_S_DEL:
		m_pCursor->ResetTimer();
		break;
	}

	switch( nScanCode )
	{
	case VK_S_V:		
		if(m_bUseClipboard && bPushCtrl && OnPasteClipboard()) 
			break;
		return false;
	case VK_S_C:		
		if(m_bUseClipboard && bPushCtrl && OnCopyClipboard()) 
			break;
		return false;
	case VK_S_X:		
		if(m_bUseClipboard && bPushCtrl && OnCutClipboard()) 
			break;
		return false;
	case VK_S_LEFT:		OnCursorLeft(bPushCtrl, bPushShift);	break;
	case VK_S_RIGHT:	OnCursorRight(bPushCtrl, bPushShift);	break;
	case VK_S_UP:		OnCursorUp(bPushCtrl, bPushShift);		break;
	case VK_S_DOWN:		OnCursorDown(bPushCtrl, bPushShift);	break;
	case VK_S_HOME:		OnCursorHome(bPushCtrl, bPushShift);	break;
	case VK_S_END:		OnCursorEnd(bPushCtrl, bPushShift);		break;
	case VK_S_PAGEDOWN:	OnCursorPageDown(bPushCtrl, bPushShift);break;
	case VK_S_PAGEUP:	OnCursorPageUp(bPushCtrl, bPushShift);	break;
	case VK_S_BS:		OnBackSpace();							break;
	case VK_S_DEL:		OnDelete();								break;
	default:			return false;
	}
	return true;
}

bool CEdit::MoreThanItem(EDITPOS &w, EDITPOS &z)
{
	CEditLine *pLine1 = w;
	CEditLine *pLine2 = z;

	//w 위치의 y좌표기 z위치 보다 작다면 z가 큰 위치이다.
	if(pLine1->GetPosY() < pLine2->GetPosY()) return false;
	//w 위치의 y좌표기 z위치 보다 크다면 w가 큰 위치이다.
	if(pLine1->GetPosY() > pLine2->GetPosY()) return true;
	
	//같은 y좌표에서 모두 끝을 가르키고 있으면 같은 값이다.
	if(w == pLine1->end() && z == pLine2->end()) return true;
	//z만 끝을 가르키도 있으면 z가 큰 값이다.
	if(z == pLine2->end()) return false;
	//w만 끝을 가르키도 있다면 w가 큰 값이다.
	if(w == pLine1->end()) return true;

	CEditItem *pItem1 = w;
	CEditItem *pItem2 = z;

	//모두 끝이 아닐경우
	//w 위치의 x좌표가 z위치 보다 작다면 z가 큰 위치이다.
	if(pItem1->left() < pItem2->left()) return false;
	//w 위치의 y좌표기 z위치 보다 크다면 w가 큰 위치이다.
	if(pItem1->left() > pItem2->left()) return true;
	return true;
}

bool CEdit::BigItem(EDITPOS &w, EDITPOS &z)
{
	CEditLine *pLine1 = w;
	CEditLine *pLine2 = z;

	//w 위치의 y좌표기 z위치 보다 작다면 z가 큰 위치이다.
	if(pLine1->GetPosY() < pLine2->GetPosY()) return false;
	//w 위치의 y좌표기 z위치 보다 크다면 w가 큰 위치이다.
	if(pLine1->GetPosY() > pLine2->GetPosY()) return true;

	//같은 y좌표에서 모두 끝을 가르키고 있으면 같은 값이다.
	if(w == pLine1->end() && z == pLine2->end()) return false;
	//z만 끝을 가르키도 있으면 z가 큰 값이다.
	if(z == pLine2->end()) return false;
	//w만 끝을 가르키도 있다면 w가 큰 값이다.
	if(w == pLine1->end()) return true;

	CEditItem *pItem1 = w;
	CEditItem *pItem2 = z;

	//모두 끝이 아닐경우
	//w 위치의 x좌표가 z위치 보다 작다면 z가 큰 위치이다.
	if(pItem1->left() < pItem2->left()) return false;
	//w 위치의 y좌표기 z위치 보다 크다면 w가 큰 위치이다.
	if(pItem1->left() > pItem2->left()) return true;
	return false;
}

void CEdit::OnReturn(void)
{
	if(m_cFontInfo.m_uFormat & DT_SINGLELINE)
	{
		__super::OnReturn();
		return;
	}

	bool bPushCtrl = false;
	if(GetAsyncKeyState(VK_LCONTROL) & 0x8000) bPushCtrl = true;
	if(bPushCtrl)
	{
		SetCursorText("");
		OnAddChar("\n");
		m_bAddScroll = true;
		CheckItemTotalWidth();
		return;
	}

	__super::OnReturn();
}

void CEdit::OnCursorLeft(const bool &bPushCtrl, const bool &bPushShift)
{
	if(m_listLines.size() == 0) return;
	//현재 커서의 위치가 없다면 무시
	if(m_cCurrentPos == m_itLineEnd) return;

	EDITPOS cOldCurrentPos = m_cCurrentPos;
	
	EDITPOS cCurPos = m_cCurrentPos;
	EDITPOS cPrevPos;

	if(m_bUseClipboard && bPushCtrl)
	{
		bool bFirst = true;
		CEditItem *pItem = GetPrevItem(cCurPos, cPrevPos);
		while(pItem)
		{
			if(pItem->m_str[0] == ' ')
			{
				bFirst = false;
			}
			else if(!bFirst)
			{
				break;
			}

			cCurPos = cPrevPos;
			pItem = GetPrevItem(cCurPos, cPrevPos);
		}

		m_cCurrentPos = cCurPos;
	}
	else
	{
		GetPrevItem(cCurPos, cPrevPos);
		m_cCurrentPos = cPrevPos;
	}

	if(m_cCurrentPos == m_itLineEnd)
	{
		m_cCurrentPos = m_listLines.begin();
		m_cCurrentPos = ((CEditLine *)m_cCurrentPos)->begin();
	}

	if(m_bUseClipboard && bPushShift)
	{
		if(cOldCurrentPos == m_cCurrentPos) return;

		if(m_cSelectStart == m_itLineEnd && m_cSelectEnd == m_itLineEnd)
		{
			m_cSelectStart = m_cCurrentPos;
			m_cSelectEnd = cOldCurrentPos;
		}
		else if(MoreThanItem(m_cCurrentPos, m_cSelectStart))
		{
			m_cSelectEnd = m_cCurrentPos;
		}
		else
		{
			m_cSelectStart = m_cCurrentPos;
		}

		if(m_cSelectEnd == m_cSelectStart)
		{
			m_cSelectStart = m_itLineEnd;
			m_cSelectEnd = m_itLineEnd;
			m_pCursor->SetShow(true);
		}
		else
		{
			m_pCursor->SetShow(false);
		}
		SetRedraw();
	}
	else if(m_cSelectStart != m_itLineEnd)
	{
		m_cCurrentPos = m_cSelectStart;
		m_cSelectStart = m_itLineEnd;
		m_cSelectEnd = m_itLineEnd;
		m_pCursor->SetShow(true);
		SetRedraw();
	}
	else
	{
		m_pCursor->SetShow(true);
	}

	m_bAddScroll = true;

	SetCaculateCursorPos();
}

void CEdit::OnCursorHome(const bool &bPushCtrl, const bool &bPushShift)
{
	//입력이 없었을 경우 무시
	if(m_listLines.size() == 0) return;
	//현재 커서의 위치가 없다면 무시
	if(m_cCurrentPos == m_itLineEnd) return;

	EDITPOS cOldCurrentPos = m_cCurrentPos;

	//라인의 가장 처음으로 이동시킨다.
	m_cCurrentPos = ((CEditLine *)m_cCurrentPos)->begin();
	//해당라인의 가장 처음으로 이동한다.

	if(m_bUseClipboard && bPushShift)
	{
		if(m_cSelectStart == m_itLineEnd)
		{
			m_cSelectEnd = cOldCurrentPos;
		}
		else if(m_cSelectStart != cOldCurrentPos)
		{
			m_cSelectEnd = m_cSelectStart;
		}
		m_cSelectStart = m_cCurrentPos;
		if(m_cSelectEnd == m_cSelectStart)
		{
			m_cSelectStart = m_itLineEnd;
			m_cSelectEnd = m_itLineEnd;
			m_pCursor->SetShow(true);
		}
		else
		{
			m_pCursor->SetShow(false);
		}
		SetRedraw();
	}
	else if(m_cSelectStart != m_itLineEnd)
	{
		m_cSelectStart = m_itLineEnd;
		m_cSelectEnd = m_itLineEnd;
		m_pCursor->SetShow(true);
		SetRedraw();
	}
	else
	{
		m_pCursor->SetShow(true);
	}

	m_bAddScroll = true;
	SetCaculateCursorPos();
}

void CEdit::OnCursorRight(const bool &bPushCtrl, const bool &bPushShift)
{
	//입력이 없었을 경우 무시
	if(m_listLines.size() == 0) return;
	//현재 커서의 위치가 없다면 무시
	if(m_cCurrentPos == m_itLineEnd) return;

	EDITPOS cOldCurrentPos = m_cCurrentPos;

	EDITPOS cCurPos = m_cCurrentPos;
	EDITPOS cNextPos;

	if(m_bUseClipboard && bPushCtrl)
	{
		CEditItem *pItem = GetNextItem(cCurPos, cNextPos);
		bool bFirst = true;
		while(pItem)
		{
			if(pItem->m_str[0] == ' ')
			{
				bFirst = false;
			}
			else if(!bFirst)
			{
				break;
			}

			pItem = GetNextItem(cCurPos, cNextPos);
			cCurPos = cNextPos;
		}

		m_cCurrentPos = cCurPos;
	}
	else if(!m_bIgnoreWMKeyDown || (m_bIgnoreWMKeyDown && bPushShift))
	{
		GetNextItem(cCurPos, cNextPos);
		if(cNextPos == m_itLineEnd) m_cCurrentPos = cOldCurrentPos;
		else m_cCurrentPos = cNextPos;
	}

	if(m_bUseClipboard && bPushShift)
	{
		if(m_cCurrentPos == cOldCurrentPos) return;

		if(m_cSelectStart == m_itLineEnd && m_cSelectEnd == m_itLineEnd)
		{
			m_cSelectStart = cOldCurrentPos;
			m_cSelectEnd = m_cCurrentPos;
		}
		else if(MoreThanItem(m_cSelectEnd, m_cCurrentPos))
		{
			m_cSelectStart = m_cCurrentPos;
		}
		else
		{
			m_cSelectEnd = m_cCurrentPos;
		}

		if(m_cSelectEnd == m_cSelectStart)
		{
			m_cSelectStart = m_itLineEnd;
			m_cSelectEnd = m_itLineEnd;
			m_pCursor->SetShow(true);
		}
		else
		{
			m_pCursor->SetShow(false);
		}

		SetRedraw();
	}
	else if(m_cSelectStart != m_itLineEnd)
	{
		m_cCurrentPos = m_cSelectEnd;
		m_cSelectStart = m_itLineEnd;
		m_cSelectEnd = m_itLineEnd;
		m_pCursor->SetShow(true);
		SetRedraw();
	}
	else
	{
		m_pCursor->SetShow(true);
	}

	m_bAddScroll = true;
	SetCaculateCursorPos();
}

void CEdit::OnCursorEnd(const bool &bPushCtrl, const bool &bPushShift)
{
	//입력이 없었을 경우 무시
	if(m_listLines.size() == 0) return;
	//현재 커서의 위치가 없다면 무시
	if(m_cCurrentPos == m_itLineEnd) return;

	EDITPOS cOldCurrentPos = m_cCurrentPos;

	//현재 라인의 가장 뒤로 이동시킨다.
	m_cCurrentPos = ((CEditLine *)m_cCurrentPos)->end();
	CEditLine *pLine = m_cCurrentPos;
	if(pLine->size())
	{
		EDITPOS cPos = m_cCurrentPos;
		CEditItem *pItem = *cPos.PrevItem();
		if(IsLineFeedChar(pItem->m_str)) m_cCurrentPos = cPos;
	}

	if(m_bUseClipboard && bPushShift)
	{
		if(m_cSelectStart == m_itLineEnd)
		{
			m_cSelectStart = cOldCurrentPos;
		}
		else if(BigItem(m_cSelectEnd, cOldCurrentPos))
		{
			m_cSelectStart = m_cSelectEnd;
		}
		m_cSelectEnd = m_cCurrentPos;
		
		if(m_cSelectEnd == m_cSelectStart)
		{
			m_cSelectStart = m_itLineEnd;
			m_cSelectEnd = m_itLineEnd;
			m_pCursor->SetShow(true);
		}
		else
		{
			m_pCursor->SetShow(false);
		}

		SetRedraw();
	}
	else if(m_cSelectStart != m_itLineEnd)
	{
		m_cSelectStart = m_itLineEnd;
		m_cSelectEnd = m_itLineEnd;
		m_pCursor->SetShow(true);
		SetRedraw();
	}
	else
	{
		m_pCursor->SetShow(true);
	}

	m_bAddScroll = true;
	SetCaculateCursorPos();
}

void CEdit::OnCursorUp(const bool &bPushCtrl, const bool &bPushShift)
{
	if(m_listLines.size() == 0) return;
	//현재 커서의 위치가 없다면 무시
	if(m_cCurrentPos == m_itLineEnd) return;
	//싱글라인 모드이면 무시
	if(m_cFontInfo.m_uFormat & DT_SINGLELINE) return;
	//첫번째 라인이면 처리하지 않는다.
	if(IsFirstLine(m_cCurrentPos)) return;

	EDITPOS cOldCurrentPos = m_cCurrentPos;

	//Up/Down시에는 컨트롤키에 대한 처리는 무시한다.
	CPoint ptLocalCursor = m_pCursor->GetLocalPos();
	ptLocalCursor -= CPoint(0, m_fLineHeight + m_fLineSpace);

	EDITPOS cTempPos;
	cTempPos = m_cCurrentPos;
	cTempPos.PrevLine();
	cTempPos = ((CEditLine *)cTempPos)->begin();

	m_cCurrentPos = HitTest(cTempPos, m_cViewEndItem, ptLocalCursor);

	EDITPOS cCurPos = m_cCurrentPos;
	EDITPOS cPrevPos;

	if(m_bUseClipboard && bPushShift)
	{
		if(cOldCurrentPos == m_cCurrentPos) return;

		if(m_cSelectStart == m_itLineEnd && m_cSelectEnd == m_itLineEnd)
		{
			m_cSelectStart = m_cCurrentPos;
			m_cSelectEnd = cOldCurrentPos;
		}
		else if(MoreThanItem(m_cCurrentPos, m_cSelectStart))
		{
			m_cSelectEnd = m_cCurrentPos;
		}
		else
		{
			m_cSelectStart = m_cCurrentPos;
		}

		if(m_cSelectEnd == m_cSelectStart)
		{
			m_cSelectStart = m_itLineEnd;
			m_cSelectEnd = m_itLineEnd;
			m_pCursor->SetShow(true);
		}
		else
		{
			m_pCursor->SetShow(false);
		}
		SetRedraw();
	}
	else if(m_cSelectStart != m_itLineEnd)
	{
		m_cCurrentPos = m_cSelectStart;
		m_cSelectStart = m_itLineEnd;
		m_cSelectEnd = m_itLineEnd;
		m_pCursor->SetShow(true);
		SetRedraw();
	}
	else
	{
		m_pCursor->SetShow(true);
	}

	m_bAddScroll = true;

	SetCaculateCursorPos();
}

void CEdit::OnCursorDown(const bool &bPushCtrl, const bool &bPushShift)
{
	if(m_listLines.size() == 0) return;
	//현재 커서의 위치가 없다면 무시
	if(m_cCurrentPos == m_itLineEnd) return;
	//싱글라인 모드이면 무시
	if(m_cFontInfo.m_uFormat & DT_SINGLELINE) return;
	//첫번째 라인이면 처리하지 않는다.
	if(IsLastLine(m_cCurrentPos)) return;

	EDITPOS cOldCurrentPos = m_cCurrentPos;

	//Up/Down시에는 컨트롤키에 대한 처리는 무시한다.
	CPoint ptLocalCursor = m_pCursor->GetLocalPos();
	ptLocalCursor += CPoint(0, m_fLineHeight + m_fLineSpace);

	if(IsLastLine(m_cViewEndItem))
	{
		m_cCurrentPos = HitTest(m_cCurrentPos, m_cViewEndItem, ptLocalCursor);
	}
	else
	{
		EDITPOS cTempPos;
		cTempPos = m_cCurrentPos;
		cTempPos.NextLine();
		cTempPos = ((CEditLine *)cTempPos)->end();

		m_cCurrentPos = HitTest(m_cCurrentPos, cTempPos, ptLocalCursor);
	}

	if(cOldCurrentPos != m_cCurrentPos)
	{
		CEditItem *pItem1 = cOldCurrentPos;
		CEditItem *pItem2 = m_cCurrentPos;

		if(pItem1 && pItem2 && !IsLineFeedChar(pItem2->m_str) && (pItem1->right() != pItem2->right()))
		{
			EDITPOS cNextPos;
			GetNextItem(m_cCurrentPos, cNextPos);
			if(cNextPos != m_itLineEnd) m_cCurrentPos = cNextPos;
		}
	}

	if(m_bUseClipboard && bPushShift)
	{
		if(m_cCurrentPos == cOldCurrentPos) return;

		if(m_cSelectStart == m_itLineEnd && m_cSelectEnd == m_itLineEnd)
		{
			m_cSelectStart = cOldCurrentPos;
			m_cSelectEnd = m_cCurrentPos;
		}
		else if(MoreThanItem(m_cSelectEnd, m_cCurrentPos))
		{
			m_cSelectStart = m_cCurrentPos;
		}
		else
		{
			m_cSelectEnd = m_cCurrentPos;
		}

		if(m_cSelectEnd == m_cSelectStart)
		{
			m_cSelectStart = m_itLineEnd;
			m_cSelectEnd = m_itLineEnd;
			m_pCursor->SetShow(true);
		}
		else
		{
			m_pCursor->SetShow(false);
		}

		SetRedraw();
	}
	else if(m_cSelectStart != m_itLineEnd)
	{
		m_cCurrentPos = m_cSelectEnd;
		m_cSelectStart = m_itLineEnd;
		m_cSelectEnd = m_itLineEnd;
		m_pCursor->SetShow(true);
		SetRedraw();
	}
	else
	{
		m_pCursor->SetShow(true);
	}

	m_bAddScroll = true;
	SetCaculateCursorPos();
}

void CEdit::OnCursorPageUp(const bool &bPushCtrl, const bool &bPushShift)
{
	if(m_listLines.size() == 0) return;
	//현재 커서의 위치가 없다면 무시
	if(m_cCurrentPos == m_itLineEnd) return;
	//싱글라인 모드이면 무시
	if(m_cFontInfo.m_uFormat & DT_SINGLELINE) return;
	//첫번째 라인이면 처리하지 않는다.
	if(IsFirstLine(m_cCurrentPos)) return;

	EDITPOS cOldCurrentPos = m_cCurrentPos;

	//Up/Down시에는 컨트롤키에 대한 처리는 무시한다.
	CPoint ptLocalCursor = m_pCursor->GetLocalPos();
	ptLocalCursor -= CPoint(0, m_rtLocalRect.GetHeight() + m_fLineSpace);

	m_cCurrentPos = HitTest(GetFirstPos(), m_cCurrentPos, ptLocalCursor);
	m_ptStartPos.y += m_rtLocalRect.GetHeight();
	if(m_ptStartPos.y > 0.0f)
	{
		m_ptStartPos.y = 0.0f;
	}

	if(m_cCurrentPos != cOldCurrentPos)	SetRedraw();

	EDITPOS cCurPos = m_cCurrentPos;
	EDITPOS cPrevPos;

	if(m_bUseClipboard && bPushShift)
	{
		if(cOldCurrentPos == m_cCurrentPos) return;

		if(m_cSelectStart == m_itLineEnd && m_cSelectEnd == m_itLineEnd)
		{
			m_cSelectStart = m_cCurrentPos;
			m_cSelectEnd = cOldCurrentPos;
		}
		else if(MoreThanItem(m_cCurrentPos, m_cSelectStart))
		{
			m_cSelectEnd = m_cCurrentPos;
		}
		else
		{
			m_cSelectStart = m_cCurrentPos;
		}

		if(m_cSelectEnd == m_cSelectStart)
		{
			m_cSelectStart = m_itLineEnd;
			m_cSelectEnd = m_itLineEnd;
			m_pCursor->SetShow(true);
		}
		else
		{
			m_pCursor->SetShow(false);
		}
		SetRedraw();
	}
	else if(m_cSelectStart != m_itLineEnd)
	{
		m_cCurrentPos = m_cSelectStart;
		m_cSelectStart = m_itLineEnd;
		m_cSelectEnd = m_itLineEnd;
		m_pCursor->SetShow(true);
		SetRedraw();
	}
	else
	{
		m_pCursor->SetShow(true);
	}

	SetCaculateCursorPos();
}

void CEdit::OnCursorPageDown(const bool &bPushCtrl, const bool &bPushShift)
{
	if(m_listLines.size() == 0) return;
	//현재 커서의 위치가 없다면 무시
	if(m_cCurrentPos == m_itLineEnd) return;
	//싱글라인 모드이면 무시
	if(m_cFontInfo.m_uFormat & DT_SINGLELINE) return;
	//첫번째 라인이면 처리하지 않는다.
	if(IsLastLine(m_cCurrentPos)) return;

	EDITPOS cOldCurrentPos = m_cCurrentPos;

	//Up/Down시에는 컨트롤키에 대한 처리는 무시한다.
	CPoint ptLocalCursor = m_pCursor->GetLocalPos();
	ptLocalCursor += CPoint(0, m_rtLocalRect.GetHeight() + m_fLineSpace);

	m_cCurrentPos = HitTest(m_cCurrentPos, GetLastPos(), ptLocalCursor);
	m_ptStartPos.y -= m_rtLocalRect.GetHeight();

	float fMaxHeight = m_rtLocalRect.GetHeight() - m_fTotalHeight;
	if(m_ptStartPos.y < fMaxHeight)
	{
		m_ptStartPos.y = fMaxHeight;
	}

	if(m_cCurrentPos != cOldCurrentPos)	SetRedraw();

	if(cOldCurrentPos != m_cCurrentPos)
	{
		CEditItem *pItem1 = cOldCurrentPos;
		CEditItem *pItem2 = m_cCurrentPos;

		if(pItem1 && pItem2 && (pItem1->right() != pItem2->right()))
		{
			EDITPOS cNextPos;
			GetNextItem(m_cCurrentPos, cNextPos);
			if(cNextPos != m_itLineEnd) m_cCurrentPos = cNextPos;
		}
	}

	if(m_bUseClipboard && bPushShift)
	{
		if(m_cCurrentPos == cOldCurrentPos) return;

		if(m_cSelectStart == m_itLineEnd && m_cSelectEnd == m_itLineEnd)
		{
			m_cSelectStart = cOldCurrentPos;
			m_cSelectEnd = m_cCurrentPos;
		}
		else if(MoreThanItem(m_cSelectEnd, m_cCurrentPos))
		{
			m_cSelectStart = m_cCurrentPos;
		}
		else
		{
			m_cSelectEnd = m_cCurrentPos;
		}

		if(m_cSelectEnd == m_cSelectStart)
		{
			m_cSelectStart = m_itLineEnd;
			m_cSelectEnd = m_itLineEnd;
			m_pCursor->SetShow(true);
		}
		else
		{
			m_pCursor->SetShow(false);
		}

		SetRedraw();
	}
	else if(m_cSelectStart != m_itLineEnd)
	{
		m_cCurrentPos = m_cSelectEnd;
		m_cSelectStart = m_itLineEnd;
		m_cSelectEnd = m_itLineEnd;
		m_pCursor->SetShow(true);
		SetRedraw();
	}
	else
	{
		m_pCursor->SetShow(true);
	}

	SetCaculateCursorPos();
}

void CEdit::OnBackSpace(void)
{
	//입력이 없었을 경우 무시
	if(m_listLines.size() == 0) return;
	//현재 커서의 위치가 없다면 무시
	if(m_cCurrentPos == m_itLineEnd) return;

	if(m_bEnable == false)
		return;

	if(DeleteSelectItem()) return;

	if(IsFirstItem(m_cCurrentPos))
		return;

	if(GetCursorText() == "")
		DeleteEditItem( true );
}

void CEdit::OnDelete(void)
{
	if(m_bEnable == false)
		return;

	if(DeleteSelectItem()) return;

	DeleteEditItem( false );
}

void CEdit::SetCursorText( const std::string& str )
{
	if(m_bNumber)
		return;

	std::string strOld = m_pCursor->GetText();
	if(strOld == str) return;
	if(m_nLimitText < (GetLength() + (int)str.length()))
	{
		HWND  hWnd = NULL;
		if(m_pProject)
		{
			CObjectMan *pManager = m_pProject->GetObjectMan();
			if(pManager)
			{
				const CWnd *pWnd = pManager->GetWnd();
				if(pWnd) hWnd = pWnd->GetSafeHwnd();
			}
		}
		HIMC  hIMC = ::ImmGetContext( hWnd );
		ImmNotifyIME(hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
		ImmReleaseContext( hWnd,hIMC );
		return;
	}
	m_pCursor->SetText(str);
	if(strOld != "") DeleteEditItem(true);
	if(str != "") OnMakeAddChars(str.c_str());
}

const std::string &CEdit::GetCursorText(void)
{
	return m_pCursor->GetText();
}

CDC *CEdit::CalculateCursorPos(CDC *pDC)
{
	if(pDC == NULL)
	{
		pDC = m_pBack->GetDC();
		if(pDC)
		{
			pDC->SelectObject((CFont *)m_cFontInfo.GetFont());
			pDC->SetTextColor( m_cFontInfo.m_clrColor );
			pDC->SetTextCharacterExtra(m_cFontInfo.m_nSpaceBetweenLetters);
		}
	}
	if(pDC == NULL) return NULL;

	m_bCaculateCursorPos = false;

	float fWidth = m_rtLocalRect.GetWidth();
	float fHeight = m_rtLocalRect.GetHeight();

	CSize szSize;
	if(m_fLineHeight == 0.0f)
	{
		::GetTextExtentPoint32( pDC->m_hDC, "A", 1, &szSize );
		m_fLineHeight = (float)szSize.cy;
	}

	EDITPOS cPrevPos;
	//각 라인의 위치를 재 정리해준다.
	if(m_cFontInfo.m_uFormat & DT_SINGLELINE)
	{
		if(m_cFontInfo.m_uFormat & DT_VCENTER)
		{
			m_ptStartPos.y = (fHeight - m_fLineHeight)/2.0f;
		}
		else if(m_cFontInfo.m_uFormat & DT_BOTTOM)
		{
			m_ptStartPos.y = fHeight - m_fLineHeight;
		}
		else
		{
			m_ptStartPos.y = 0.0f;
		}

		//아직 입력된 내용이 없다면
		if(m_listLines.size() == 0)
		{
			if(m_cFontInfo.m_uFormat & DT_RIGHT) m_ptStartPos.x = fWidth;
			else if(m_cFontInfo.m_uFormat & DT_CENTER) m_ptStartPos.x = fWidth/2;
			else m_ptStartPos.x = 0;

			m_pCursor->SetLocalPos(m_ptStartPos);
		}
		else
		{
			//첫번째 라인을 가져온다.
			CEditLine *pLine = *m_listLines.begin();
			const float &fTotalLineWidth = pLine->GetTotalWidth();
			//라인에 추가된 문자가 없을 경우 해당 라인을 삭제해 버린다.
			if(pLine->size() == 0)
			{
				m_listLines.clear();
				delete pLine;

				m_cCurrentPos = m_itLineEnd;

				if(m_cFontInfo.m_uFormat & DT_RIGHT) m_ptStartPos.x = fWidth;
				else if(m_cFontInfo.m_uFormat & DT_CENTER) m_ptStartPos.x = fWidth/2;
				else m_ptStartPos.x = 0;

				m_pCursor->SetLocalPos(m_ptStartPos);
			}
			//표시 폭보다 작다면
			else if(fTotalLineWidth < fWidth)
			{			
				if(m_cFontInfo.m_uFormat & DT_RIGHT) m_ptStartPos.x = fWidth - fTotalLineWidth;
				else if(m_cFontInfo.m_uFormat & DT_CENTER) m_ptStartPos.x = (fWidth - fTotalLineWidth)/2;
				else m_ptStartPos.x = 0;

				if(m_cCurrentPos == pLine->begin())
				{
					m_pCursor->SetLocalPos(m_ptStartPos);
				}
				else
				{
					//현재 커서의 이전 문자를 구한다.
					CEditItem *pPrevItem = GetPrevItem(m_cCurrentPos, cPrevPos);
					if(GetCursorText() == "") m_pCursor->SetLocalPos(m_ptStartPos + pPrevItem->m_rtRect.GetRightTop());
					else m_pCursor->SetLocalPos(m_ptStartPos + pPrevItem->m_rtRect.GetLeftTop());
				}
			}
			//표시 폭보다 크거나 같다면
			else
			{
				if(m_cCurrentPos == pLine->begin())
				{
					m_ptStartPos.x = 0;
					SetRedraw();
					m_pCursor->SetLocalPos(m_ptStartPos);
				}
				else
				{
					//현재 커서의 이전 문자를 구한다.
					CEditItem *pPrevItem = GetPrevItem(m_cCurrentPos, cPrevPos);
					if(m_ptStartPos.x + fTotalLineWidth < fWidth)
					{
						m_ptStartPos.x = fWidth - fTotalLineWidth;
						SetRedraw();
					}
					else if(m_ptStartPos.x + pPrevItem->right() > fWidth)
					{
						m_ptStartPos.x = fWidth - pPrevItem->right();
						SetRedraw();
					}
					else if(m_ptStartPos.x + pPrevItem->right() < 0)
					{
						m_ptStartPos.x = -pPrevItem->right();
						SetRedraw();
					}

					if(GetCursorText() == "") m_pCursor->SetLocalPos(m_ptStartPos + pPrevItem->m_rtRect.GetRightTop());
					else m_pCursor->SetLocalPos(m_ptStartPos + pPrevItem->m_rtRect.GetLeftTop());
				}
			}
		}
	}
	else
	{
		//전체 높이가 표시 높이 보다 작다면 상하 정렬을 수행한다.
		if(m_fTotalHeight < fHeight)
		{
			if(m_cFontInfo.m_uFormat & DT_VCENTER)
			{
				m_ptStartPos.y = (fHeight - m_fTotalHeight)/2.0f;
			}
			else if(m_cFontInfo.m_uFormat & DT_BOTTOM)
			{
				m_ptStartPos.y = fHeight - m_fTotalHeight;
			}
			else
			{
				m_ptStartPos.y = 0.0f;
			}
		}

		CPoint ptCursor = CPointZero;
		//아직 입력된 내용이 없다면
		if(m_listLines.size() == 0)
		{
			if(m_cFontInfo.m_uFormat & DT_RIGHT) ptCursor.x = fWidth;
			else if(m_cFontInfo.m_uFormat & DT_CENTER) ptCursor.x = fWidth/2;
			else ptCursor.x = 0;

			ptCursor.y = m_ptStartPos.y;
		}
		else
		{
			//커서의 라인을 가져온다.
			CEditLine *pLine = m_cCurrentPos;
			const float &fTotalLineWidth = pLine->GetTotalWidth();

			if(m_cFontInfo.m_uFormat & DT_RIGHT) pLine->SetPosX(fWidth - fTotalLineWidth);
			else if(m_cFontInfo.m_uFormat & DT_CENTER) pLine->SetPosX((fWidth - fTotalLineWidth)/2.0f);
			else pLine->SetPosX(0.0f);

			if(m_cCurrentPos == pLine->begin())
			{
				ptCursor = m_ptStartPos + pLine->GetPoint();
			}
			else
			{
				//현재 커서의 이전 문자를 구한다.
				CEditItem *pPrevItem = GetPrevItem(m_cCurrentPos, cPrevPos);
				if(GetCursorText() == "") ptCursor = m_ptStartPos + pLine->GetPoint() + pPrevItem->m_rtRect.GetRightTop();
				else ptCursor = m_ptStartPos + pLine->GetPoint() + pPrevItem->m_rtRect.GetLeftTop();
			}

			//입력 또는 커서의 이동으로 인한 위치 조정이 필요할때
			if(m_bAddScroll)
			{
				//커서의 y 위치가 0보다 작다이면 y위치를 0으로 조종해준다.
				if(ptCursor.y < 0)
				{
					m_ptStartPos.y -= ptCursor.y;
					ptCursor.y = 0;
					SetRedraw();
				}
				//커서의 보이는 위치가 보이는 높이보다 크다이면 가장 아래로 조정해준다.
				else if(ptCursor.y + m_fLineHeight > fHeight)
				{
					m_ptStartPos.y += fHeight - (ptCursor.y + m_fLineHeight);
					ptCursor.y = fHeight - m_fLineHeight;
					SetRedraw();
				}
			}
		}

		m_pCursor->SetLocalPos(ptCursor);
	}

	if(m_pScrollBar)
	{
		m_pScrollBar->SetCurValue(-m_ptStartPos.y);
	}

	m_bAddScroll = false;

	return pDC;
}

void CEdit::Draw(NMBASE::GRAPHICGDI::CPage *pPage)
{
	CDC *pDC = NULL;
	if(m_bReCaculateItemWidth)	pDC = ReCaculateItemWidth();
	if(m_bCaculateCursorPos)	pDC = CalculateCursorPos(pDC);
	if(m_bRedraw)				CheckRedraw(pDC);

	CObjectBase::Draw(pPage);
}

void CEdit::CheckRedraw(CDC *pDC)
{
	if(m_cFontInfo.GetFont() == NULL) return;
	if(pDC == NULL)
	{
		pDC = m_pBack->GetDC();

		if(pDC)
		{
			pDC->SelectObject((CFont *)m_cFontInfo.GetFont());
			pDC->SetTextColor( m_cFontInfo.m_clrColor );
			pDC->SetTextCharacterExtra(m_cFontInfo.m_nSpaceBetweenLetters);
		}
	}
	if(pDC == NULL) return;

	m_pBack->ClearColor(RGB(0,0,255));

	EDITLINELISTITER start = m_listLines.begin();
	EDITLINELISTITER end = m_itLineEnd;
	if(start == end) return;

	CRect rtTemp;
	CRect rtChar;
	EDITITEMLISTITER start_item, end_item;

	m_cViewFirstItem = m_itLineEnd;
	m_cViewEndItem = m_itLineEnd;

	bool bFirst = true;
	std::string strOutput;
	COLORREF clrOldBkColor = RGB(0,0,0);
	COLORREF clrFontColor = m_cFontInfo.m_clrColor;

	pDC->SetBkMode(TRANSPARENT);

	bool bDraw = false;
	float fHeight = m_rtLocalRect.GetHeight();
	if(m_cFontInfo.m_uFormat & DT_SINGLELINE)
	{
		CEditLine *pLine = *start;
		CEditItem *pItem = NULL;
		start_item = pLine->begin();
		end_item = pLine->end();
		if(start_item == end_item) return;

		bool bCheckSelect = (m_cSelectStart != m_itLineEnd)?true:false;
		
		for(;start_item != end_item;++start_item)
		{
			pItem = *start_item;
			if(bCheckSelect)
			{
				if(m_cSelectStart == start_item)
				{
					pDC->SetBkMode(OPAQUE);
					clrOldBkColor = pDC->SetBkColor(m_clrSelectBack);
					clrFontColor = m_clrSelectFont;
				}
				else if(m_cSelectEnd == start_item)
				{
					pDC->SetBkMode(TRANSPARENT);
					pDC->SetBkColor(clrOldBkColor);
					clrFontColor = m_cFontInfo.m_clrColor;
				}
			}

			rtChar = pItem->m_rtRect;
			rtChar.OffsetRect(m_ptStartPos);
			
			if(rtTemp.IntersectRect(&m_rtLocalRect, &rtChar))
			{
				bDraw = true;
				if(m_bPassword) strOutput = PASSWORD_CHAR;
				else strOutput = pItem->m_str;

				switch(m_cFontInfo.m_nDrawType)
				{
				case ETDT_OUTLINE:
					pDC->SetTextColor(m_cFontInfo.m_clrOutline);
					pDC->TextOut((int)rtChar.left+1, (int)rtChar.top, strOutput.c_str());
					pDC->TextOut((int)rtChar.left+1, (int)rtChar.top+2, strOutput.c_str());
					pDC->TextOut((int)rtChar.left, (int)rtChar.top+1, strOutput.c_str());
					pDC->TextOut((int)rtChar.left+2, (int)rtChar.top+1, strOutput.c_str());
					pDC->SetTextColor(clrFontColor);
					pDC->TextOut((int)rtChar.left+1, (int)rtChar.top+1, strOutput.c_str());
					break;
				case ETDT_SHADOW:
					pDC->SetTextColor(m_cFontInfo.m_clrOutline);
					pDC->TextOut((int)rtChar.left+1, (int)rtChar.top+1, strOutput.c_str());
					pDC->SetTextColor(clrFontColor);
					pDC->TextOut((int)rtChar.left, (int)rtChar.top, strOutput.c_str());
					break;
				default:
					pDC->SetTextColor(clrFontColor);
					pDC->TextOut((int)rtChar.left, (int)rtChar.top, strOutput.c_str());
					break;
				}

				if(bFirst)
				{
					m_cViewFirstItem = start;
					m_cViewFirstItem = start_item;
					bFirst = false;
				}

				m_cViewEndItem = start;
				m_cViewEndItem = start_item;
			}
			else if(bDraw)
			{
				break;
			}
		}
	}
	else
	{
		CEditLine *pLine = *start;
		CEditItem *pItem = NULL;

		bool bCheckSelect = (m_cSelectStart != m_itLineEnd)?true:false;
		
		//라인스크롤을 체크할지 여부를 지정한다.
		bool bMoveCheckLineScroll = true;
		float fMaxHeight = fHeight - m_fTotalHeight;
		
		//마지막으로 스크롤 되어 있으면 변경하지 않는다.
		if(fMaxHeight == m_ptStartPos.y)
		{
			bMoveCheckLineScroll = false;
		}
		//스크롤바가 있고 라인스크롤이 아니면 변경하지 않는다.
		else if(m_pScrollBar && !m_bLineScroll)
		{
			bMoveCheckLineScroll = false;
		}

		//모든 라인을 그린다.
		EDITPOS cTempPos;
		size_t nSize = 0;
		for(;start != end;++start)
		{
			pLine = *start;
			//화면에 보이는지 체크한다.
			//화면에 보일때만 그린다.
			if(m_ptStartPos.y + pLine->GetPosY() + m_fLineHeight > 0)
			{
				if(bFirst)
				{
					m_cViewFirstItem = start;
					m_cViewFirstItem = pLine->begin();
					bFirst = false;

					//첫번째 라인의 위치가 0보다 작다 이면 0으로 보정해준다.
					if(bMoveCheckLineScroll)
					{
						float fStartY = m_ptStartPos.y + pLine->GetPosY();
						if(fStartY < m_fLineSpace)
						{
							m_ptStartPos.y -= fStartY;
							CPoint ptCursor = m_pCursor->GetLocalPos();
							ptCursor.y -= fStartY;
							m_pCursor->SetLocalPos(ptCursor);
						}
					}
				}

				m_cViewEndItem = start;
				m_cViewEndItem = pLine->end();

				start_item = pLine->begin();
				end_item = pLine->end();
				nSize = pLine->size();
				//그라인의 모든 문자를 그린다.
				for(;start_item != end_item;++start_item)
				{
					pItem = *start_item;
					if(bCheckSelect)
					{
						cTempPos = start;
						cTempPos = start_item;
						if(MoreThanItem(cTempPos, m_cSelectStart) && BigItem(m_cSelectEnd, cTempPos))
						{
							pDC->SetBkMode(OPAQUE);
							clrOldBkColor = pDC->SetBkColor(m_clrSelectBack);
							clrFontColor = m_clrSelectFont;
						}
						else
						{
							pDC->SetBkMode(TRANSPARENT);
							pDC->SetBkColor(clrOldBkColor);
							clrFontColor = m_cFontInfo.m_clrColor;
						}
					}

					if(m_bPassword) strOutput = PASSWORD_CHAR;
					else strOutput = pItem->m_str;

					rtChar = pItem->m_rtRect;
					rtChar.OffsetRect(m_ptStartPos + pLine->GetPoint());

					if(!IsLineFeedChar(pItem->m_str))
					{
						switch(m_cFontInfo.m_nDrawType)
						{
						case ETDT_OUTLINE:
							pDC->SetTextColor(m_cFontInfo.m_clrOutline);
							pDC->TextOut((int)rtChar.left+1, (int)rtChar.top, strOutput.c_str());
							pDC->TextOut((int)rtChar.left+1, (int)rtChar.top+2, strOutput.c_str());
							pDC->TextOut((int)rtChar.left, (int)rtChar.top+1, strOutput.c_str());
							pDC->TextOut((int)rtChar.left+2, (int)rtChar.top+1, strOutput.c_str());
							pDC->SetTextColor(clrFontColor);
							pDC->TextOut((int)rtChar.left+1, (int)rtChar.top+1, strOutput.c_str());
							break;
						case ETDT_SHADOW:
							pDC->SetTextColor(m_cFontInfo.m_clrOutline);
							pDC->TextOut((int)rtChar.left+1, (int)rtChar.top+1, strOutput.c_str());
							pDC->SetTextColor(clrFontColor);
							pDC->TextOut((int)rtChar.left, (int)rtChar.top, strOutput.c_str());
							break;
						default:
							pDC->SetTextColor(clrFontColor);
							pDC->TextOut((int)rtChar.left, (int)rtChar.top, strOutput.c_str());
							break;
						}
					}
					else if(nSize == 1)
					{
						pDC->SetTextColor(clrFontColor);
						pDC->TextOut((int)rtChar.left, (int)rtChar.top, " ");
					}
				}
			}
			//화면에 보이는 부분을 넘어갈경우 그리기를 중단한다.
			else if(m_ptStartPos.y + pLine->GetPosY() > fHeight)
			{
				break;
			}
		}
	}

	m_pBack->ReleaseDC();
	m_bRedraw = false;
}

BOOL CEdit::OnKeyProc(const int &nMessage, const WPARAM &wParam, const LPARAM &lParam)
{
	if(m_bEnable == false || !m_bShow || !m_bShowRef || !m_bFocus) return FALSE;

	BOOL bRet = FALSE;
	switch( nMessage )
	{
	case WM_CHAR:
		if(m_bStat || m_bIgnoreWMChar)
		{
			bRet = TRUE;
			break;
		}
		if( GetAsyncKeyState( VK_CONTROL ) )
		{
			//컨트롤 키를 누르고 있을때 입력은 출력하지 않는다.
			bRet = TRUE;
			break;
		}
		else
		{
			if((char)wParam == '.')
			{
				int i=0;
			}
			char ach_wparam[2];
			ach_wparam[0] = wParam;
			ach_wparam[1] = '\0';

			//멀티 바이트를 합성 중일때 아래로 진행시킨다.
			if(IsByteToMerge(ach_wparam)) 
			{
			}
			else
			{
				//ALT + 숫자로 입력하는 캐릭터 입력처리
				if( wParam < 0x20 )
				{
					bRet = TRUE;
					break;
				}
			}

			OnAddChar(ach_wparam);
		}
		bRet = TRUE;
		break;
	case WM_KEYDOWN:
		if(!m_bStat)
		{
			m_bIgnoreWMChar = false;
			if(OnSpecialKeyDown( wParam, lParam ))
			{
				m_bIgnoreWMKeyDown = false;
				bRet = TRUE;
				break;
			}
			m_bIgnoreWMKeyDown = false;
		}
		bRet = TRUE;
		break;
	case WM_IME_KEYDOWN:
		break;
	case WM_IME_NOTIFY:
		{
			if( wParam == IMN_SETCONVERSIONMODE ) // 비스타는 이게 쓸 때 없이 3번 온다. (false-true-true 식으로)
			{
				CheckKoreanMode(true);
			}
		}
		bRet = TRUE;
		break;
	case WM_IME_STARTCOMPOSITION:
		m_bStat = true;
		bRet = TRUE;
		break;
	case WM_IME_COMPOSITION: // Windows 2000 이상
		{
			// 한글이 조합중일 때는 GCS_COMPSTR | GCS_COMPATTR 결과는 GCS_RESULTSTR로 들어옴 (다른 플래그 셋 없음)
			// 한글 모에서 숫자 입력시, GCS_RESULTSTR로 들어오는 경우 있음 (WM_IME_STARTCOMPOSITION도 없이)
			if(lParam & GCS_RESULTSTR)
			{
				SetCursorText("");
				HWND  hWnd = NULL;
				if(m_pProject && m_pProject->GetObjectMan())
				{
					const CWnd *pWnd = m_pProject->GetObjectMan()->GetWnd();
					if(pWnd) hWnd = pWnd->GetSafeHwnd();
				}
				HIMC hIMC = ::ImmGetContext( hWnd );

				LONG	size_buffer = ::ImmGetCompositionStringA( hIMC, GCS_RESULTSTR, 0, 0 );
				if(size_buffer)
				{
					char*	szCompStr = new char[size_buffer+1];
					{
						LONG lRet = ::ImmGetCompositionStringA( hIMC, GCS_RESULTSTR, szCompStr, size_buffer );
						szCompStr[lRet] = '\0';

						OnMakeAddChars(szCompStr);
					}
					delete[] szCompStr;
				}
				::ImmReleaseContext(hWnd, hIMC);

				SetRedraw();

				m_bIgnoreWMChar = true;
				bRet = TRUE;
				break;
			}

			int nLanguage = 0;
			{
				HKL hKeyboardLayout = ::GetKeyboardLayout(0);
				switch(LOWORD(hKeyboardLayout))
				{
				case LID_TRADITIONAL_CHINESE:	nLanguage = LID_TRADITIONAL_CHINESE;	break;
				case LID_JAPANESE:				nLanguage = LID_JAPANESE;				break;
				case LID_KOREAN:				nLanguage = LID_KOREAN;					break;
				case LID_SIMPLIFIED_CHINESE:	nLanguage = LID_SIMPLIFIED_CHINESE;		break;
				}
			}

			std::string compStringPri(GetCursorText());
			SetCursorText("");
			if( wParam == 0 || wParam == 0x1b ) 
			// 조합중에 백스페이스로 지우는 경우
			// XP는 0, Vista에서는 0x1b(27)이 온다. 
			{
				// 일, 중국어는 계속 이리 온다. 조합 중에
			}
			else
			{
				if(nLanguage == LID_KOREAN)
				{
					std::string strComp = GetCursorText();
					strComp += wParam >> 8;
					strComp += wParam;
					SetCursorText(strComp);
				}
			}

			if(compStringPri != GetCursorText())
			{
				SetRedraw();
			}
		}
		bRet = TRUE;
		break;
	case WM_IME_ENDCOMPOSITION:
		{
			m_bStat = false;
			m_bIgnoreWMChar = true;
			m_bIgnoreWMKeyDown = true;

			if(GetCursorText().length())
			{
				SetCursorText("");
				SetRedraw();
			}
		}
		bRet = TRUE;
		break;
	}

	CObjectBase::OnKeyProc(nMessage, wParam, lParam);

	return bRet;
}

bool CEdit::OnPasteClipboard(void)
{
	if(m_pProject == NULL || m_pProject->GetObjectMan() == NULL) return false;
	CWnd *pWnd = m_pProject->GetObjectMan()->GetWnd();
	if(pWnd == NULL) return false;
	if(!IsClipboardFormatAvailable(CF_TEXT)) return false;
	if(!OpenClipboard(pWnd->GetSafeHwnd())) return false;

	HGLOBAL hmem = GetClipboardData(CF_TEXT);
	size_t nTotalSize = GlobalSize(hmem);
	if(nTotalSize == 0)
	{
		CloseClipboard();
		return false;
	}

	char *pDes = new char[nTotalSize];

	LPVOID pTemp = GlobalLock(hmem);
	memcpy((void *)pDes, pTemp, nTotalSize);
	GlobalUnlock(hmem);
	CloseClipboard();

	int nLen   = strlen(pDes);
	const char* szBuf = pDes;

	for( int i = 0; i < nLen; ++i )
	{
		char szCurStr[3];

		if( ::IsDBCSLeadByte( szBuf[i] ) )
		{
			szCurStr[0] = szBuf[i];
			szCurStr[1] = szBuf[i+1];
			szCurStr[2] = '\0';
			++i;
		}
		else
		{
			szCurStr[0] = szBuf[i];
			szCurStr[1] = '\0';
		}

		OnAddChar( szCurStr, true );
	}
	delete [] pDes;

	SetRedraw();
	return true;
}

bool CEdit::OnCopyClipboard(void)
{
	if(m_pProject == NULL || m_pProject->GetObjectMan() == NULL) return false;
	CWnd *pWnd = m_pProject->GetObjectMan()->GetWnd();
	if(pWnd == NULL) return false;
	const std::string strSelect = GetSelectText();
	if(strSelect == "")
	{
		OpenClipboard(pWnd->GetSafeHwnd());
		EmptyClipboard();
		CloseClipboard();
		return false;
	}

	//클립보드에 저장한다.
	size_t nSize = strSelect.size();
	HGLOBAL hmem = GlobalAlloc(GHND, nSize + 1);
	BYTE *pDes = (BYTE *)GlobalLock(hmem);
	ZeroMemory(pDes, nSize + 1);

	memcpy(pDes, strSelect.c_str(), nSize);

	GlobalUnlock(hmem);

	if(OpenClipboard(pWnd->GetSafeHwnd()))
	{
		EmptyClipboard();
		SetClipboardData(CF_TEXT, hmem);
		CloseClipboard();
		return true;
	}

	return false;
}

const std::string CEdit::GetSelectText(void)
{
	std::string strOutput;
	if(m_cSelectStart == m_itLineEnd || m_cSelectStart == m_cSelectEnd) return strOutput;

	CEditLine *pLineStart = m_cSelectStart;
	CEditLine *pLineEnd = m_cSelectEnd;
	CEditItem *pItem = NULL;

	EDITLINELISTITER start_line = m_cSelectStart;
	EDITLINELISTITER end_line = m_cSelectEnd;

	//한줄말 선택되어 있다면
	if(pLineStart == pLineEnd)
	{
		//선택된 첫번째 라인의 문자들을 구해온다.
		EDITITEMLISTITER start_item = m_cSelectStart;
		EDITITEMLISTITER end_item  = m_cSelectEnd;

		for(;start_item != end_item;++start_item)
		{
			pItem = *start_item;
			strOutput += pItem->m_str;
		}
	}
	//여러줄이 선택되어 있다면
	else
	{
		//첫번째 라인의 문자들을 구해온다.
		EDITITEMLISTITER start_item = m_cSelectStart;
		EDITITEMLISTITER end_item  = pLineStart->end();

		for(;start_item != end_item;++start_item)
		{
			pItem = *start_item;
			strOutput += pItem->m_str;
		}

		//다음라인으로 이동한다.
		++start_line;
		//마지막 라인이 아니라면 모든 문자를 더해준다.
		for(;start_line != end_line;++start_line)
		{
			pLineStart = *start_line;
			start_item = pLineStart->begin();
			end_item = pLineStart->end();
			for(;start_item != end_item;++start_item)
			{
				pItem = *start_item;
				strOutput += pItem->m_str;
			}
		}

		//마지막 라인의 처음에서 마지막 까지를 구한다.
		start_item = pLineEnd->begin();
		end_item = m_cSelectEnd;
		for(;start_item != end_item;++start_item)
		{
			pItem = *start_item;
			strOutput += pItem->m_str;
		}
	}

	return strOutput;
}

bool CEdit::OnCutClipboard(void)
{
	if(!OnCopyClipboard()) return false;

	DeleteSelectItem();

	return true;
}

void CEdit::SetCursorColor(const COLORREF &clrColor)
{
	if(m_pCursor) m_pCursor->SetCursorColor(clrColor);
}

const COLORREF CEdit::GetCursorColor(void)
{
	COLORREF clrColor = RGB(255,255,255);
	if(m_pCursor) clrColor = m_pCursor->GetCursorColor();
	return clrColor;
}

void CEdit::SetCursorFontColor(const COLORREF &clrColor)
{
	if(m_pCursor) m_pCursor->SetTextColor(clrColor);
}

const COLORREF CEdit::GetCursorFontColor(void)
{
	COLORREF clrColor = RGB(0, 0, 0);
	if(m_pCursor) clrColor = m_pCursor->GetTextColor();
	return clrColor;
}

void CEdit::SetBlinkTime(const DWORD &dwBlinkTime)
{
	if(m_pCursor) m_pCursor->SetBlinkTime(dwBlinkTime);
}

const DWORD CEdit::GetBlinkTime(void)
{
	DWORD dwBlinkTime = 500;
	if(m_pCursor) dwBlinkTime = m_pCursor->GetBlinkTime();
	return dwBlinkTime;
}

void CEdit::SetFormat(const UINT &uFormat)
{
	if(m_cFontInfo.m_uFormat == uFormat) return;

	bool bSetText = false;
	std::string strTemp = "";
	if((uFormat & DT_SINGLELINE) != (m_cFontInfo.m_uFormat & DT_SINGLELINE))
	{
		strTemp = GetText();
		bSetText = true;
	}

	m_cFontInfo.m_uFormat = uFormat;

	SetUseMouseWheel((uFormat & DT_SINGLELINE)?false:true);

	if(bSetText)
	{
		SetText(strTemp);
	}
	else
	{
		SetCaculateCursorPos();
		SetRedraw();
	}
}

void CEdit::SetCaculateCursorPos(void)
{
	if(m_bCaculateCursorPos) return;
	m_bCaculateCursorPos = true;
	m_pCursor->CheckEventDraw();
}

CEditLine *CEdit::GetViewFirstLine(EDITPOS &cStart, EDITPOS &cEnd)
{
	//입력된 문자가 하나도 없을 경우
	if(m_listLines.size() == 0) return NULL;
	m_cViewFind = cStart;
	//화면에 보이는 문자가 없을 경우
	if(m_cViewFind == m_itLineEnd) return NULL;
	//화면에 첫번째로 보이는 라인을 리턴한다.
	return m_cViewFind;
}

CEditLine *CEdit::GetViewNextLine(EDITPOS &cStart, EDITPOS &cEnd)
{
	//입력된 문자가 하나도 없을 경우
	if(m_listLines.size() == 0) return NULL;
	//화면에 보이는 문자가 없을 경우
	if(m_cViewFind == m_itLineEnd) return NULL;
	CEditLine *pLine = m_cViewFind;
	CEditLine *pEndLine = cEnd;
	//이미 마지막 라인까지 돌았으면 검색이 종료된 것임
	if(pLine == pEndLine) return NULL;
	m_cViewFind.NextLine();
	m_cViewFind = ((CEditLine *)m_cViewFind)->begin();
	return m_cViewFind;
}

CEditItem *CEdit::GetViewFirstItem(EDITPOS &cPos)
{
	//입력된 문자가 하나도 없을 경우
	if(m_listLines.size() == 0) return NULL;
	//검색할 라인이 없을 경우
	if(cPos == m_itLineEnd) return NULL;

	m_cViewFind = cPos;
	CEditLine *pLine = m_cViewFind;
	//라인이 비어 있을 경우
	if(pLine->size() == 0) return NULL;
	//라인의 마지막일 경우
	if(m_cViewFind == pLine->end()) return NULL;
	return m_cViewFind;
}

CEditItem *CEdit::GetViewNextItem(void)
{
	CEditLine *pLine = m_cViewFind;
	m_cViewFind.NextItem();
	if(m_cViewFind == pLine->end()) return NULL;
	return m_cViewFind;
}

EDITPOS	CEdit::HitTest(EDITPOS &cStart, EDITPOS &cEnd, const CPoint &ptLocal)
{
	CRect rtChar;

	EDITPOS cCurrent;
	cCurrent = m_itLineEnd;

	//어떤 라인을 클릭했는지를 조사한다.
	CEditLine *pLine = GetViewFirstLine(cStart, cEnd);
	if(pLine == NULL) return cCurrent;
	//첫번째 라인보다 위쪽이면 첫번째 라인을 선택한다.
	if(pLine->IsPrevLineIn(ptLocal, m_ptStartPos))
	{
		cCurrent = m_cViewFirstItem;
	}
	else
	{
		while(pLine)
		{
			//해당하는 라인을 찾는다.
			if(pLine->IsLineIn(ptLocal, m_ptStartPos, m_fLineSpace, m_fLineHeight))
			{
				cCurrent = m_cViewFind;
				break;
			}
			pLine = GetViewNextLine(cStart, cEnd);
		}

		//해당하는 라인이 없으면 가장 마지막 라인이다.
		if(pLine == NULL)
		{
			cCurrent = m_cViewEndItem;
			cCurrent = ((CEditLine *)cCurrent)->begin();
		}
	}

	CEditLine *pCurrentLine = cCurrent;
	if(pCurrentLine->size() == 0)
	{
	}
	else
	{
		CEditItem *pItem = GetViewFirstItem(cCurrent);
		CPoint ptStartPos = m_ptStartPos + pCurrentLine->GetPoint();
		float fSpaceBetweenLetters = (float)m_cFontInfo.m_nSpaceBetweenLetters;
		if(pItem->IsPrevIn(ptLocal, ptStartPos))
		{
		}
		else
		{
			while(pItem)
			{
				if(pItem->IsIn(ptLocal, ptStartPos, fSpaceBetweenLetters))
				{
					cCurrent = m_cViewFind;
					break;
				}

				pItem = GetViewNextItem();
			}

			if(pItem == NULL)
			{
				cCurrent = pCurrentLine->end();
				
				//가장 마지막 문자가 만약 개행 문자이면
				//개행문자로 커서를 이동시켜야 한다.
				EDITPOS ptTempPos = cCurrent;
				ptTempPos.PrevItem();
				pItem = ptTempPos;
				if(IsLineFeedChar(pItem->m_str))
				{
					cCurrent.PrevItem();
				}
			}
		}
	}

	return cCurrent;
}

bool CEdit::OnDragBegin(const CPoint &ptPos)
{
	const CPoint &ptLocal = GetGlobalToLocalPos(ptPos);

	if((int)m_listLines.size()== 0)
	{
		m_cDragFirst = m_itLineEnd;
		m_cDragEnd = m_itLineEnd;
	}
	else if(m_cViewFirstItem != m_itLineEnd)
	{
		m_cDragFirst = HitTest(m_cViewFirstItem, m_cViewEndItem, ptLocal);
	}

	if(m_cSelectStart != m_itLineEnd)
	{
		m_cSelectStart = m_itLineEnd;
		m_cSelectEnd = m_itLineEnd;
		SetRedraw();
	}

	m_cCurrentPos = m_cDragFirst;
	if(m_bUseClipboard)
	{
		m_cDragEnd = m_cDragFirst;
		m_pCursor->SetShow(false);
	}
	else
	{
		m_pCursor->SetShow(true);
	}
	m_bAddScroll = true;
	SetCaculateCursorPos();

	return __super::OnDragBegin(ptPos);
}

bool CEdit::OnDraging(const CPoint &ptPos)
{
	const CPoint &ptLocal = GetGlobalToLocalPos(ptPos);
	m_ptDragPos = ptLocal;

	if(m_cFontInfo.m_uFormat & DT_SINGLELINE)
	{
		if(((m_rtLocalRect.left > m_ptDragPos.x) || (m_rtLocalRect.right <= m_ptDragPos.x)))
		{
			if(m_rtLocalRect.left > m_ptDragPos.x)
			{
				m_dwDragMoveTime = max(500 - int(m_rtLocalRect.left - m_ptDragPos.x) * 10, 10);
			}
			else if(m_rtLocalRect.right <= m_ptDragPos.x)
			{
				m_dwDragMoveTime = max(500 - int(m_ptDragPos.x - m_rtLocalRect.right) * 10, 10);
			}

			if(!IsUseLogic())
			{
				m_dwDragStartTime = 0;
				SetUseLogic(true);
			}
		}
		else if(IsUseLogic())
		{
			SetUseLogic(false);
		}
	}
	else
	{
		if(((m_rtLocalRect.top > m_ptDragPos.y) || (m_rtLocalRect.bottom <= m_ptDragPos.y)))
		{
			if(m_rtLocalRect.top > m_ptDragPos.y)
			{
				m_dwDragMoveTime = max(500 - int(m_rtLocalRect.top - m_ptDragPos.y) * 10, 10);
			}
			else if(m_rtLocalRect.bottom <= m_ptDragPos.y)
			{
				m_dwDragMoveTime = max(500 - int(m_ptDragPos.y - m_rtLocalRect.bottom) * 10, 10);
			}

			if(!IsUseLogic())
			{
				m_dwDragStartTime = 0;
				SetUseLogic(true);
			}
		}
		else if(IsUseLogic())
		{
			SetUseLogic(false);
		}
	}

	if((int)m_listLines.size() == 0)
	{
		m_cDragFirst = m_itLineEnd;
		m_cDragEnd = m_itLineEnd;
	}
	else
	{
		m_cDragEnd = HitTest(m_cViewFirstItem, m_cViewEndItem, ptLocal);
	}

	m_cCurrentPos = m_cDragEnd;
	if(m_bUseClipboard && m_cDragFirst != m_itLineEnd)
	{
		if(m_cDragFirst != m_cDragEnd)
		{
			if(BigItem(m_cDragFirst, m_cDragEnd))
			{
				m_cSelectStart = m_cDragEnd;
				m_cSelectEnd = m_cDragFirst;
			}
			else
			{
				m_cSelectStart = m_cDragFirst;
				m_cSelectEnd = m_cDragEnd;
			}

			SetRedraw();
		}
	}
	else
	{
		m_pCursor->SetShow(true);
	}
	m_bAddScroll = true;
	SetCaculateCursorPos();

	return __super::OnDraging(ptPos);
}

bool CEdit::OnDragEnd(const CPoint &ptPos)
{
	const CPoint &ptLocal = GetGlobalToLocalPos(ptPos);

	if(IsUseLogic())
	{
		SetUseLogic(false);
	}

	if((int)m_listLines.size() == 0)
	{
		m_cDragFirst = m_itLineEnd;
		m_cDragEnd = m_itLineEnd;
	}
	else
	{
		m_cDragEnd = HitTest(m_cViewFirstItem, m_cViewEndItem, ptLocal);
	}

	m_cCurrentPos = m_cDragEnd;
	if(m_bUseClipboard && m_cDragFirst != m_itLineEnd)
	{
		if(m_cDragFirst == m_cDragEnd)
		{
			if(m_cSelectStart != m_itLineEnd)
			{
				m_cSelectStart = m_itLineEnd;
				m_cSelectEnd = m_itLineEnd;
				SetRedraw();
			}

			m_pCursor->SetShow(true);
		}
		else
		{
			if(BigItem(m_cDragFirst, m_cDragEnd))
			{
				m_cSelectStart = m_cDragEnd;
				m_cSelectEnd = m_cDragFirst;
			}
			else
			{
				m_cSelectStart = m_cDragFirst;
				m_cSelectEnd = m_cDragEnd;
			}

			SetRedraw();
		}
	}
	else
	{
		m_pCursor->SetShow(true);
	}
	m_bAddScroll = true;
	SetCaculateCursorPos();

	return __super::OnDragEnd(ptPos);
}

void CEdit::Logic(const DWORD &dwTime)
{
	if(m_cFontInfo.m_uFormat & DT_SINGLELINE)
	{
		if(dwTime - m_dwDragStartTime >= m_dwDragMoveTime)
		{
			m_dwDragStartTime += m_dwDragMoveTime;
			EDITPOS cTempPos;

			if(m_rtLocalRect.left > m_ptDragPos.x)
			{
				if(IsFirstItem(m_cViewFirstItem)) return;

				GetPrevItem(m_cDragEnd, cTempPos);
				if(cTempPos != m_itLineEnd)
				{
					m_cDragEnd = cTempPos;
				}
			}
			else if(m_rtLocalRect.right <= m_ptDragPos.x)
			{
				if(IsLastItem(m_cViewEndItem)) return;

				GetNextItem(m_cDragEnd, cTempPos);
				if(cTempPos != m_itLineEnd)
				{
					m_cDragEnd = cTempPos;
				}
			}

			m_cCurrentPos = m_cDragEnd;

			if(m_bUseClipboard)
			{
				if(BigItem(m_cDragFirst, m_cDragEnd))
				{
					m_cSelectStart = m_cDragEnd;
					m_cSelectEnd = m_cDragFirst;
				}
				else
				{
					m_cSelectStart = m_cDragFirst;
					m_cSelectEnd = m_cDragEnd;
				}
			}

			SetRedraw();
			SetCaculateCursorPos();
		}
	}
	else
	{
		if(dwTime - m_dwDragStartTime >= m_dwDragMoveTime)
		{
			m_dwDragStartTime += m_dwDragMoveTime;
			EDITPOS cTempPos;

			CPoint ptLocalCursor = m_pCursor->GetLocalPos();
			if(m_rtLocalRect.top > m_ptDragPos.y)
			{
				if(IsFirstLine(m_cViewFirstItem)) return;

				EDITPOS cTempPos;
				cTempPos = m_cViewFirstItem;
				cTempPos.PrevLine();
				cTempPos = ((CEditLine *)cTempPos)->begin();

				ptLocalCursor -= CPoint(0, m_fLineHeight + m_fLineSpace);
				ptLocalCursor.x = m_ptDragPos.x;

				m_cDragEnd = HitTest(cTempPos, m_cViewEndItem, ptLocalCursor);
				if(m_cDragEnd == m_itLineEnd)
				{
					m_cDragEnd = GetFirstPos();
				}
			}
			else if(m_rtLocalRect.bottom <= m_ptDragPos.y)
			{
				if(IsLastLine(m_cViewEndItem)) return;

				EDITPOS cTempPos;
				cTempPos = m_cViewEndItem;
				cTempPos.NextLine();
				cTempPos = ((CEditLine *)cTempPos)->end();

				ptLocalCursor += CPoint(0, m_fLineHeight + m_fLineSpace);
				ptLocalCursor.x = m_ptDragPos.x;

				m_cDragEnd = HitTest(m_cViewFirstItem, cTempPos, ptLocalCursor);
				if(m_cDragEnd == m_itLineEnd)
				{
					m_cDragEnd = GetLastPos();
				}
			}

			m_bAddScroll = true;
			m_cCurrentPos = m_cDragEnd;

			if(m_bUseClipboard)
			{
				if(BigItem(m_cDragFirst, m_cDragEnd))
				{
					m_cSelectStart = m_cDragEnd;
					m_cSelectEnd = m_cDragFirst;
				}
				else
				{
					m_cSelectStart = m_cDragFirst;
					m_cSelectEnd = m_cDragEnd;
				}
			}

			SetCaculateCursorPos();
			SetRedraw();
		}
	}
}

void CEdit::SetPassword(const bool &bPassword)
{
	if(m_bPassword == bPassword) return;
	m_bPassword = bPassword;
	SetReCaculateItemWidth();
	SetCaculateCursorPos();
	SetRedraw();
}

void CEdit::SetFont(const std::string &strFontName, const int &nFontSize, const bool &bBold)
{
	if(m_cFontInfo.GetName() == strFontName && m_cFontInfo.GetSize() == nFontSize && m_cFontInfo.IsBold() == bBold) return;

	m_cFontInfo.SetFont(strFontName, nFontSize, bBold);
	m_fLineHeight = 0.0f;

	std::string strTemp = GetText();
	SetText(strTemp);

	CheckScrollData();
}

void CEdit::SetReCaculateItemWidth(void)
{
	m_bReCaculateItemWidth = true;
}

CDC *CEdit::ReCaculateItemWidth(void)
{
	CDC *pDC = m_pBack->GetDC();
	if(pDC == NULL) return NULL;

	pDC->SelectObject((CFont *)m_cFontInfo.GetFont());
	pDC->SetTextColor( m_cFontInfo.m_clrColor );
	pDC->SetTextCharacterExtra(m_cFontInfo.m_nSpaceBetweenLetters);

	m_bReCaculateItemWidth = false;

	//한줄의 높이를 구한다.
	CSize szSize;
	if(m_fLineHeight == 0.0f)
	{
		::GetTextExtentPoint32( pDC->m_hDC, "A", 1, &szSize );
		m_fLineHeight = (float)szSize.cy;
	}

	m_fTotalHeight = 0;

	//각 라인의 시작 위치를 구한다.
	EDITLINELISTITER start = m_listLines.begin();
	EDITLINELISTITER end = m_itLineEnd;

	EDITITEMLISTITER start_item, end_item;

	CEditLine *pLine = NULL;
	CPoint ptStartPos = CPointZero;

	for(;start != end;++start)
	{
		pLine = *start;
		*pLine = ptStartPos;
		ptStartPos.y += m_fLineHeight;		  //한줄의 높이를 더해준다.
		ptStartPos.y += (float)m_fLineSpace; //줄간격을 더해준다.
	}

	if(pLine) m_fTotalHeight = pLine->GetPosY() + m_fLineHeight;
	else m_fTotalHeight = m_fLineHeight;

	float fWidth = m_rtLocalRect.GetWidth();
	float fHeight = m_rtLocalRect.GetHeight();

	//싱글 라인일때는 처리하는 내용이 없음
	if(m_cFontInfo.m_uFormat & DT_SINGLELINE)
	{
		start = m_listLines.begin();
		if(start != end)
		{
			start_item = (*start)->begin();
			end_item = (*start)->end();

			CPoint ptPos = CPointZero;
			CEditItem *pItem = NULL;
			for(;start_item != end_item;++start_item)
			{
				pItem = *start_item;
				pItem->SetPos(ptPos);
				ptPos.x += pItem->GetWidth();
				ptPos.x += m_cFontInfo.m_nSpaceBetweenLetters;
			}
		}
	}
	else
	{
		//멀티 라인일때 각 라인의 시작 위치를 계산해준다.
		start = m_listLines.begin();
		CPoint ptPos = CPointZero;
		CEditItem *pItem = NULL;

		CPoint ptTempPos;

		for(;start != end;++start)
		{
			pLine = *start;
			ptTempPos = pLine->GetPoint();
			if(m_cFontInfo.m_uFormat & DT_CENTER)
			{
				ptTempPos.x = ((fWidth -  pLine->GetTotalWidth())/2.0f);
			}
			else if(m_cFontInfo.m_uFormat & DT_RIGHT)
			{
				ptTempPos.x = fWidth - pLine->GetTotalWidth();;
			}
			else
			{
				ptTempPos.x = 0.0f;
			}

			*pLine = ptTempPos;

			start_item = (*start)->begin();
			end_item = (*start)->end();
			ptPos = CPointZero;
			
			for(;start_item != end_item;++start_item)
			{
				pItem = *start_item;
				pItem->SetPos(ptPos);
				ptPos.x += pItem->GetWidth();
				ptPos.x += (int)m_cFontInfo.m_nSpaceBetweenLetters;
			}
		}
	}

	if(m_pScrollBar)
	{
		if(fHeight >= m_fTotalHeight)
		{
			m_pScrollBar->SetRangeValue(0, 0);
			m_pScrollBar->SetEnable(false);
		}
		else
		{
			m_pScrollBar->SetRangeValue(0, m_fTotalHeight - fHeight);
			m_pScrollBar->SetEnable(true);
		}

		if(fHeight - m_fTotalHeight > m_ptStartPos.y)
		{
			m_ptStartPos.y = fHeight - m_fTotalHeight;
		}

		m_pScrollBar->SetCurValue(-m_ptStartPos.y);
	}
	
	m_pCursor->SetRedraw();
	SetCaculateCursorPos();
	return pDC;
}

void CEdit::SetTextColor(const COLORREF &clrColor)
{
	if(m_cFontInfo.m_clrColor == clrColor) return;
	m_cFontInfo.m_clrColor = clrColor;
	SetRedraw();
}

void CEdit::SetTextDrawType(const ENUM_TEXTDRAW_TYPE &nType)
{
	if(m_cFontInfo.m_nDrawType == nType) return;
	m_cFontInfo.m_nDrawType = nType;
	SetReCaculateItemWidth();
	SetRedraw();
}

void CEdit::SetSpaceBetweenLetters(const int &nSpaceBetweenLetters)
{
	if(m_cFontInfo.m_nSpaceBetweenLetters == nSpaceBetweenLetters) return;
	m_cFontInfo.m_nSpaceBetweenLetters = nSpaceBetweenLetters;
	SetReCaculateItemWidth();
	SetRedraw();
}

void CEdit::SetShadowColor(const COLORREF &clrColor)
{
	if(m_cFontInfo.m_clrOutline == clrColor) return;
	m_cFontInfo.m_clrOutline = clrColor;
	SetRedraw();
	m_pCursor->SetRedraw();
}

void CEdit::SetLimitText(const int &nLimit)
{
	if(m_nLimitText == nLimit) return;
	m_nLimitText = nLimit;

	if(GetLength() > m_nLimitText)
	{
		EDITLINELISTITER start_line = m_listLines.begin();
		EDITLINELISTITER end_line = m_itLineEnd;

		EDITITEMLISTITER start_item, end_item;
		size_t len = 0;

		CEditLine *pLine = NULL;
		CEditItem *pItem = NULL;
		bool bDelete = false;

		while(start_line != end_line)
		{
			pLine = *start_line;
			start_item = pLine->begin();
			end_item = pLine->end();

			if(bDelete)
			{
				delete pLine;
				start_line = m_listLines.erase(start_line);
			}
			else
			{
				while(start_item != end_item)
				{
					pItem = *start_item;
					len += pItem->GetLength();

					//뒷 부분을 모두 삭제한다.
					if((int)len > m_nLimitText)
					{
						bDelete = true;
					}

					if(bDelete)
					{
						delete pItem;
						start_item = pLine->erase(start_item);
					}
					else
					{
						++start_item;
					}
				}

				if(bDelete)
				{
					//아이템이 전부 지워졌을 해당 라인을 삭제한다.
					if(pLine->size() == 0)
					{
						delete pLine;
						start_line = m_listLines.erase(start_line);
					}
				}
				else
				{
					++start_line;
				}
			}
		}
	}

	if(m_listLines.size())
	{
		m_cCurrentPos = m_listLines.end();
		m_cCurrentPos.PrevLine();
		m_cCurrentPos = ((CEditLine *)m_cCurrentPos)->end();

		CheckDeleteMultiLine((EDITLINELISTITER)m_cCurrentPos, m_rtLocalRect.GetWidth());
	}
	else
	{
		m_cCurrentPos = m_itLineEnd;
	}

	m_cSelectStart = m_itLineEnd;
	m_cSelectEnd = m_itLineEnd;

	SetReCaculateItemWidth();
	SetCaculateCursorPos();
	SetRedraw();
}

void CEdit::SetNumber(const bool &bNumber)
{
	if(m_bNumber == bNumber) return;
	std::string strText;
	if(bNumber)
	{
		SetCursorText("");
		strText = GetText();
		SetText("");
	}
	m_bNumber = bNumber;
	if(bNumber) SetText(strText);
}

void CEdit::SetUseClipboard(const bool &bUseClipboard)
{
	if(m_bUseClipboard == bUseClipboard) return;
	m_bUseClipboard = bUseClipboard;
	if(!bUseClipboard && m_cSelectStart != m_itLineEnd)
	{
		m_cSelectStart = m_itLineEnd;
		m_cSelectEnd = m_itLineEnd;
		SetRedraw();
	}
}

void CEdit::SetSelectBackColor(const COLORREF &clrColor)
{
	if(m_clrSelectBack == clrColor) return;
	m_clrSelectBack = clrColor;
	if(m_bUseClipboard && m_cSelectStart != m_itLineEnd)
		SetRedraw();
}

void CEdit::SetSelectFontColor(const COLORREF &clrColor)
{
	if(m_clrSelectFont == clrColor) return;
	m_clrSelectFont = clrColor;
	if(m_bUseClipboard && m_cSelectStart != m_itLineEnd)
		SetRedraw();
}

void CEdit::SetSelect(int nSelectStart, int nSelectEnd)
{
	//if(m_bUseClipboard == false) return;
	//if(nSelectStart == -1)
	//{
	//	m_nSelectFirst = -1;
	//	m_nSelectEnd = -1;
	//	SetRedraw();
	//	return;
	//}

	//m_nSelectFirst = nSelectStart;
	//if(nSelectEnd == -1)
	//{
	//	m_nSelectEnd = (int)m_listItems.size();
	//}
	//else
	//{
	//	m_nSelectEnd = min(nSelectEnd + 1, (int)m_listItems.size());
	//}

	//SetRedraw();
}

void CEdit::SetCursorPos(int nCursorPos)
{
	//if(nCursorPos < 0) m_nCurrentPos = 0;
	//else if(nCursorPos >= (int)m_listItems.size()) nCursorPos = (int)m_listItems.size();
	//else m_nCurrentPos = nCursorPos;

	//SetCaculateCursorPos();
}

void CEdit::AttachScroll(CScrollBar *pScroll)
{
	ASSERT(m_pScrollBar == NULL);
	m_pScrollBar = pScroll;
	AddLocalChild(m_pScrollBar);

	CheckScrollData();
}

void CEdit::DettachScroll(void)
{
	ASSERT(m_pScrollBar != NULL);
	SubChild(m_pScrollBar);
	m_pScrollBar = NULL;
}

void CEdit::CheckScrollData(void)
{
	if(m_pScrollBar == NULL) return;

	if(m_fTotalHeight <= GetHeight())
	{
		m_pScrollBar->SetCurValue(0);
		m_pScrollBar->SetEnable(false);
	}
	else
	{
		m_pScrollBar->SetRangeValue(0, m_fTotalHeight - GetHeight());
		m_pScrollBar->SetCurValue(-m_ptStartPos.y);
		m_pScrollBar->SetEnable(true);
	}

	m_pScrollBar->SetPageValue(m_rtLocalRect.GetHeight());
}

LRESULT CEdit::OnLocalChildNotify(CMessage *msg)
{
	CObjectBase *pObject = ((CObjectMessage *)msg)->GetObject();
	if((pObject == m_pScrollBar) && ((m_cFontInfo.m_uFormat & DT_SINGLELINE) == 0))
	{
		switch(msg->GetID())
		{
		case EM_O_CLICKUP:
			return OnClickUp(msg);
		case EM_O_CLICKDOWN:
			return OnClickDown(msg);
		case EM_O_CLICKPAGEUP:
			return OnClickPageUp(msg);
		case EM_O_CLICKPAGEDOWN:
			return OnClickPageDown(msg);
		case EM_O_CURVALUECHANGE:
			return OnClickValueChange(msg);
		case EM_O_FOCUSIN:
			return OnFocusInScroll(msg);
		}
	}
	return CObjectBase::OnLocalChildNotify(msg);
}

void CEdit::CheckUp(void)
{
	if(m_ptStartPos.y >= 0)
		return;

	float fScroll = m_ptStartPos.y + m_fLineHeight + m_fLineSpace;
	if(fScroll > 0)
	{
		fScroll = 0.0f;
	}

	if(m_ptStartPos.y == fScroll) return;

	m_ptStartPos.y = fScroll;

	if(m_pScrollBar)
		m_pScrollBar->SetCurValue(-fScroll);

	SetCaculateCursorPos();
	SetRedraw();
}

void CEdit::CheckDown(void)
{
	float fMaxScroll = GetHeight() - m_fTotalHeight;
	if(fMaxScroll >= m_ptStartPos.y)
		return;

	float fScroll = m_ptStartPos.y - m_fLineHeight - m_fLineSpace;
	if(fScroll < fMaxScroll)
	{
		fScroll = fMaxScroll;
	}

	if(m_ptStartPos.y == fScroll) return;
	m_ptStartPos.y = fScroll;
	
	if(m_pScrollBar)
		m_pScrollBar->SetCurValue(-fScroll);

	SetCaculateCursorPos();
	SetRedraw();
}

LRESULT CEdit::OnClickUp(CMessage *pMsg)
{
	if(!m_bLineScroll) return FALSE;
	if(m_pScrollBar->IsIncrease()) CheckDown();
	else CheckUp();
	return TRUE;
}

LRESULT CEdit::OnClickDown(CMessage *pMsg)
{
	if(!m_bLineScroll) return FALSE;
	if(m_pScrollBar->IsIncrease()) CheckUp();
	else CheckDown();
	return TRUE;
}

void CEdit::CheckPageUp(void)
{
	if(m_ptStartPos.y >= 0)
		return;

	float fScroll = m_ptStartPos.y + m_rtLocalRect.GetHeight();
	if(fScroll > 0)
	{
		fScroll = 0.0f;
	}

	if(m_ptStartPos.y == fScroll) return;
	m_ptStartPos.y = fScroll;

	if(m_pScrollBar)
		m_pScrollBar->SetCurValue(-fScroll);

	SetCaculateCursorPos();
	SetRedraw();
}

void CEdit::CheckPageDown(void)
{
	float fMaxScroll = GetHeight() - m_fTotalHeight;
	if(fMaxScroll >= m_ptStartPos.y)
		return;

	float fScroll = m_ptStartPos.y - m_rtLocalRect.GetHeight();
	if(fScroll < fMaxScroll)
	{
		fScroll = fMaxScroll;
	}

	if(m_ptStartPos.y == fScroll) return;
	m_ptStartPos.y = fScroll;

	if(m_pScrollBar)
		m_pScrollBar->SetCurValue(-fScroll);

	SetCaculateCursorPos();
	SetRedraw();
}

LRESULT CEdit::OnClickPageUp(CMessage *pMsg)
{
	if(!m_bLineScroll) return FALSE;
	if(m_pScrollBar->IsIncrease()) CheckPageDown();
	else CheckPageUp();
	return TRUE;
}

LRESULT CEdit::OnClickPageDown(CMessage *pMsg)
{
	if(!m_bLineScroll) return FALSE;
	if(m_pScrollBar->IsIncrease()) CheckPageUp();
	else CheckPageDown();
	return TRUE;
}

LRESULT CEdit::OnClickValueChange(CMessage *pMsg)
{
	CMO_CurValueChange *msg = (CMO_CurValueChange *)pMsg;
	float fCurValue = msg->GetValue();
	if(m_ptStartPos.y != -fCurValue)
	{
		m_ptStartPos.y = -fCurValue;
		SetCaculateCursorPos();
		SetRedraw();
	}
	return TRUE;
}

LRESULT CEdit::OnFocusInScroll(CMessage *pMsg)
{
	return (LRESULT)this;
}

bool CEdit::OnMouseWheel(const int &nKey, const short &zDelta)
{
	if(m_pScrollBar) m_pScrollBar->OnMouseWheel(nKey, zDelta);
	return CObjectBase::OnMouseWheel(nKey, zDelta);
}

void CEdit::SetLineSpace(const int &nLineSpace)
{
	if(m_fLineSpace == (float)nLineSpace) return;
	m_fLineSpace = (float)nLineSpace;

	SetReCaculateItemWidth();
	SetCaculateCursorPos();
	SetRedraw();
}

void CEdit::SetLineScroll(const bool &bLineScroll)
{
	if(m_bLineScroll == bLineScroll) return;
	m_bLineScroll = bLineScroll;
	SetCaculateCursorPos();
}
} //namespace LSY