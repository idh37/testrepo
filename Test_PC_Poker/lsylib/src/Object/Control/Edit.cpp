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
	nRet += sizeof(size_t);		//�ؽ�Ʈ ũ��
	nRet += strText.size();		//�ؽ�Ʈ
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
	nRet += sizeof(bool);		//��ũ�ѹ�����
	if(m_pScrollBar) nRet += m_pScrollBar->GetClipSize(nVersion);	//��ũ�ѹٰ� ���� ���
	return nRet;
}

size_t CEdit::SaveInheritanceClipSize(int &nVersion, BYTE *pDest)
{
	size_t nRet = __super::SaveInheritanceClipSize(nVersion, pDest);

	std::string strText = GetText();
	size_t nSize = strText.size();
	memcpy(pDest+nRet, &nSize, sizeof(size_t));
	nRet += sizeof(size_t);		//�ؽ�Ʈ ũ��
	if(nSize)
	{
		memcpy(pDest+nRet, strText.c_str(), nSize);
		nRet += nSize;				//�ؽ�Ʈ
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
	nRet += sizeof(bool);		//��ũ�ѹ� ����

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
	nRet += sizeof(size_t);		//�ؽ�Ʈ ũ��
	if(nSize)
	{
		GetPointerToString(pDest+nRet, nSize, strText);
		nRet += nSize;				//�ؽ�Ʈ
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
	nRet += sizeof(bool);		//��ũ�ѹ� ����

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
	//���� �ƹ��͵� �������� �ʴ´ٸ�
	//�������� ����� �� ���ο� �������� �߰��Ѵ�.
	float fCharWidth = pItem->GetWidth();
	float fWidth = m_rtLocalRect.GetWidth();

	bool bLineFeed = IsLineFeedChar(pItem->m_str);

	//�̱� �����϶� ���� ���ڴ� �����Ѵ�.
	if((m_cFontInfo.m_uFormat & DT_SINGLELINE) && bLineFeed)
	{
		delete pItem;
		return;
	}

	//ó�� ���ڸ� �Է��� ��� �ٹٲ޿� ���� ����� ���� �ʴ´�.
	if(m_listLines.size() == 0)
	{
		//ù���ڰ� ���� ������ ��� ������ �ΰ� �߰��Ѵ�.
		if(bLineFeed)
		{
			CEditLine *pLine = new CEditLine;
			m_listLines.push_back(pLine);
			m_cCurrentPos = m_listLines.begin();

			pLine->push_back(pItem);

			//���� �����̹Ƿ� ���� ����.
			*pLine = 0.0f;

			//�ű� ������ �߰����ش�.
			pLine = new CEditLine;
			m_listLines.push_back(pLine);
			m_cCurrentPos.NextLine();

			//���θ� �߰� �����Ƿ� ���� �������� �ʴ´�.
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

			//���� �߰��� �׸��� ��尡 �׸���/�ƿ����� �ϰ�� ��ü���� ���ʿ� 1�� �����ش�.
			switch(m_cFontInfo.m_nDrawType)
			{
			case ETDT_SHADOW:
			case ETDT_OUTLINE:
				*pLine += 1.0f;
				break;
			}

			*pLine += fCharWidth;

			//���� �߰��� �׸��� ��尡 �ƿ����� �� ��� ��ü���� ���ʿ� 1�� �����ش�.
			switch(m_cFontInfo.m_nDrawType)
			{
			case ETDT_OUTLINE:
				*pLine += 1.0f;
				break;
			}
		}
	}
	//���� Ŀ�� ��ġ�� �ִ� ������ �����Ѵٸ�
	else if(m_cCurrentPos != m_itLineEnd)
	{
		CEditLine *pCurrentLine = m_cCurrentPos;
		//���� ������ ��쿡�� �� ���̿� ���� �ٹٲ޿� ���� ����� ���� �ʴ´�.
		if(bLineFeed)
		{
			//���� ������ �������� Ŀ���� �ִٸ� �űԷ� ������ �߰��� �ش�.
			if(m_cCurrentPos == pCurrentLine->end())
			{
				//�� ���ο� ó������ ���ڰ� �Է� �ɶ�
				if(pCurrentLine->size() == 0)
				{
					//���� ��ġ�� ���๮���� �Է����ְ� ���� �������� �̵��Ѵ�.
					pCurrentLine->push_back(pItem);

					//�ű� ���θ��� �����Ѵ�.
					CEditLine *pLine = new CEditLine;
					m_cCurrentPos.NextLine();
					
					m_cCurrentPos = m_listLines.insert(m_cCurrentPos, pLine);
					m_cCurrentPos = pLine->end();
				}
				//�� ������ �������� Ŀ���� �ִٸ� ���� ��ġ�� ���� ���ڸ� �Է����ְ�
				else
				{
					pCurrentLine->push_back(pItem);

					//�ű� ���θ��� �����ϰ�
					//������ ������ �ƴҰ�� ���� ���ڸ� �߰����ش�.
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
			//���� ������ �߰��� Ŀ���� �����Ѵٸ� �߰����� �űԷ� ������ �߰����ְ� Ŀ���� ��ġ�� �ڷ� �̵���Ų��.
			else
			{
				//���� �տ� Ŀ���� �ִٸ� ���� ��ġ�� ������ �߰����ְ�
				//���� ���ڸ� �߰����ش�.
				if(m_cCurrentPos == pCurrentLine->begin())
				{
					CEditLine *pLine = new CEditLine;
					pLine->push_back(pItem);

					m_cCurrentPos = m_listLines.insert(m_cCurrentPos, pLine);
					m_cCurrentPos.NextLine();
				}
				else
				{
					//���� Ŀ�� ������ �����۵��� ���ؼ� �ӽ� �����ϰ�
					//���ο� ������ ������ �д�.
					EDITITEMLIST listTemp;
					listTemp.insert(listTemp.begin(), (EDITITEMLISTITER)m_cCurrentPos, pCurrentLine->end());

					CEditLine *pLine = new CEditLine;
					*pLine = listTemp;

					//���� �߰��� �׸��� ��尡 �׸���/�ƿ����� �ϰ�� ��ü���� ���ʿ� 1�� �����ش�.
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

					//���� �������� ���� ���ڵ��� �����ϰ� ���๮�ڸ� �߰��Ѵ�.
					pCurrentLine->erase(m_cCurrentPos, pCurrentLine->end());
					pCurrentLine->push_back(pItem);
					*pCurrentLine -= fCharTotalWidth;

					//���ο� ������ �߰��ϰ�, �� ������ ���� ó���� Ŀ���� ��ġ��Ų��.
					m_cCurrentPos.NextLine();
					m_cCurrentPos = m_listLines.insert(m_cCurrentPos, pLine);
					m_cCurrentPos = pLine->begin();

					CheckDeleteMultiLine((EDITLINELISTITER)m_cCurrentPos, fWidth);
				}
			}
		}
		//���� ���ڰ� �ƴ� ���
		//��Ƽ���� �϶� ����Ʈ�϶� �� �ٲ޿� ���� ����� �ݵ�� �Ͽ��� �Ѵ�.
		else
		{
			//���� ������ �������� Ŀ���� �ִٸ� �������� �߰����ش�.
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
			//���� ������ �߰��� Ŀ���� �����Ѵٸ� �߰��� �߰����ְ� Ŀ���� ��ġ�� �ڷ� �̵���Ų��.
			else
			{
				m_cCurrentPos = pCurrentLine->insert(m_cCurrentPos, pItem);
				m_cCurrentPos.NextItem();

				//������ ��ü ���� �ڰ� + ������ ���� �����ش�.
				*pCurrentLine += (float)m_cFontInfo.m_nSpaceBetweenLetters + fCharWidth;
			}

			//��Ƽ �����̶�� �� �ٲ޿� ���� ����� �Ͽ��� �Ѵ�.
			CheckInsertMultiLine((EDITLINELISTITER)m_cCurrentPos, fWidth);
		}
	}
	//���� Ŀ���� ��ġ�� ���ٸ� �߰������� �ʴ´�.
	else
	{
		ASSERT(FALSE);
	}

	m_bAddScroll = true;
	CheckItemTotalWidth();
}

//��Ƽ ������ ��� ���� �������� ��ü ������ ũ���̸� ���� ������ ���ش�.
void CEdit::CheckInsertMultiLine(EDITLINELISTITER &itLine, const float &fWidth)
{
	if(m_cFontInfo.m_uFormat & DT_SINGLELINE) return;
	//���� ǥ�� �������� �������� ũ�ٸ� ������ ���ڸ� ���� �������� �Ű� ��� �Ѵ�.
	CEditLine *pCurrentLine = *itLine;
	if(pCurrentLine->GetTotalWidth() > fWidth)
	{
		size_t nSize = pCurrentLine->size();
		if(nSize > 0)
		{
			bool bMoveCharCursor1 = false;
			bool bMoveCharCursor2 = false;

			//������ �������� ���ؿ´�.
			EDITITEMLISTITER end_item = pCurrentLine->end();
			--end_item;

			//������ ���� ������ �Ǵ� Ŀ���� �ִ� ��ġ�� �̵��Ͽ��� �� Ŀ����
			//���� ���� ó������ �̵� ��Ų��.
			if((m_cCurrentPos == itLine) && (m_cCurrentPos == end_item))
				bMoveCharCursor1 = true;

			if((m_cCurrentPos == itLine) && (m_cCurrentPos == pCurrentLine->end()))
				bMoveCharCursor2 = true;

			//������ ��ŭ�� ���� ��ü���� �������ش�.
			CEditItem *pItem = *end_item;
			bool bLineFeedChar = IsLineFeedChar(pItem->m_str);
			//������ ���ڰ� ���� �����̸� ���� �۾��� �ι����ش�.
			if(bLineFeedChar)
			{
				//���๮�ڴ� ������ ���� �����Ƿ� ���� ��ȭ�� ����.
				//�ش� ���ο��� ������ ���ڸ� �����Ѵ�.
				pCurrentLine->erase(end_item);
			}
			else
			{
				//������ ���� ���������� �������ش�.
				*pCurrentLine -= pItem->GetWidth();
				if(nSize >= 2) *pCurrentLine -= (float)m_cFontInfo.m_nSpaceBetweenLetters;

				//�ش� ���ο��� ������ ���ڸ� �����Ѵ�.
				pCurrentLine->erase(end_item);
			}

			//������ ���ڸ� �߰��� ������ ���Ѵ�.
			EDITLINELISTITER itNewLine = itLine;
			++itNewLine;
			//���� ������ ���ٸ� ������ �߰����ش�.
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
					//���� �߰��� �׸��� ��尡 �׸���/�ƿ����� �ϰ�� ��ü���� ���ʿ� 1�� �����ش�.
					switch(m_cFontInfo.m_nDrawType)
					{
					case ETDT_SHADOW:
					case ETDT_OUTLINE:
						*pLine += 1.0f;
						break;
					}

					*pLine += pItem->GetWidth();

					//���� �߰��� �׸��� ��尡 �ƿ����� �� ��� ��ü���� ���ʿ� 1�� �����ش�.
					switch(m_cFontInfo.m_nDrawType)
					{
					case ETDT_OUTLINE:
						*pLine += 1.0f;
						break;
					}
				}
			}
			//���� ������ �ִٸ� �� ������ ó���� �������� �߰��Ѵ�.
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
	//���ڰ� ���ٸ� ������ ���� �ʴ´�.
	if(m_listLines.size() == 0) return;
	//���� Ŀ���� ���� ��ġ�� ���ٸ� �����Ѵ�.
	if(m_cCurrentPos == m_itLineEnd) return;

	CEditLine *pCurrentLine = m_cCurrentPos;

	//Ŀ���� ��ġ�� ���� ������ ���� ���
	if(m_cCurrentPos == pCurrentLine->end())
	{
		//�չ��ڸ� ������
		if(bFront)
		{
			//���� �� ������ ���, ���� ������ ������ �ǹ��Ѵ�.
			//���� ������ �� �ٲ޿� ���� ����� ���� �ʴ´�.
			if(pCurrentLine->size() == 0)
			{
				//���� ������ ���� �Ѵ�.
				delete pCurrentLine;
				m_cCurrentPos = m_listLines.erase(m_cCurrentPos);

				//��� ������ ���� �Ǿ��� ��
				if(m_listLines.size() == 0)
				{
					m_cCurrentPos = m_itLineEnd;
					return;
				}

				//���� �������� �̵��Ѵ�
				m_cCurrentPos.PrevLine();
				pCurrentLine = m_cCurrentPos;

				size_t nSize = pCurrentLine->size();
				ASSERT(nSize != 0);

				m_cCurrentPos = pCurrentLine->end();
				m_cCurrentPos.PrevItem();

				CEditItem *pItem = m_cCurrentPos;
				//������ ���ڰ� ���� ������ �� ���� ��ȭ�� ����.
				if(IsLineFeedChar(pItem->m_str))
				{
				}
				else
				{
					*pCurrentLine -= pItem->GetWidth();
					if(nSize >= 2) *pCurrentLine -= (float)m_cFontInfo.m_nSpaceBetweenLetters;
				}

				//�ش� ������ �������� ����� �������� ���� �޺κ����� Ŀ���� �̵� ��Ų��.
				delete pItem;

				pCurrentLine->erase(m_cCurrentPos);
				m_cCurrentPos = pCurrentLine->end();
			}
			else
			{
				//���� ������ �������� �������ش�.
				//���� ���� �޺κп� ���� �� �ٲ޿� ���ؼ� ����Ͽ��� �Ѵ�.
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
		//�޹��ڸ� ������
		else
		{
			//���� �� ������ ���, ���� ������ ������ �ǹ��Ѵ�.
			//���� �ƹ� �ص��� ���� �ʴ´�.
			if(pCurrentLine->size() == 0)
			{
				return;
			}
			//���� ������ ���� ó�� ���ڸ� �������ش�. Ŀ���� �̵����� �ʴ´�.
			//���� ���� �޺κп� ���� �� �ٲ޿� ���ؼ� ����Ͽ��� �Ѵ�.
			else
			{
				EDITLINELISTITER itLineTemp = m_cCurrentPos;
				++itLineTemp;
				//���� Ŀ���� ��ġ�� ������ �����̸� �ƹ��͵� ���� �ʴ´�.
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
	//Ŀ���� ��ġ�� ���� ������ ���� ���� ���
	else if(m_cCurrentPos == pCurrentLine->begin())
	{
		//�չ��ڸ� �������ش�.
		if(bFront)
		{
			//���� ù �����̸� �ƹ����۵� ���� �ʴ´�.
			if(IsFirstLine(m_cCurrentPos)) return;

			//���� ������ ���� �������� �������ְ�, Ŀ���� ���� ������ ���� �ڷ� �̵���Ų��.
			m_cCurrentPos.PrevLine();
			pCurrentLine = m_cCurrentPos;
			m_cCurrentPos = pCurrentLine->end();
			m_cCurrentPos.PrevItem();
			CEditItem *pItem = m_cCurrentPos;
			size_t nSize = pCurrentLine->size();
			//������ ���ڰ� ���� ������ �� ���� ��ȭ�� ����.
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
		//�޺��ڸ� �������ش�.
		else
		{
			CEditItem *pItem = m_cCurrentPos;
			//ó�� ���ڰ� ���๮���� ���
			//���� ������ �������ְ� Ŀ����ġ�� ������������ �̵��Ѵ�.
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
	//Ŀ���� ��ġ�� ���� ������ �߰��� ���
	else
	{
		//�չ��ڸ� ������
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
		//�޹��ڸ� ������
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

//���� ������ ���� �������� �����Ǿ��� ������ ���� ���� �ڷ� ��ġ ���濡 ���ؼ� ���� �Ͽ��� �Ѵ�.
void CEdit::CheckDeleteMultiLine(EDITLINELISTITER &itLine, const float &fWidth)
{
	if(m_cFontInfo.m_uFormat & DT_SINGLELINE) return;
	if(itLine == m_itLineEnd) return;

	CEditLine *pCurrentLine = *itLine;
	//���� ������ ������ ��� �ƹ��͵� ���� �ʴ´�.
	if(pCurrentLine->size() == 0) return;
	EDITLINELISTITER itTempLine = itLine;
	++itTempLine;
	if(itTempLine == m_itLineEnd) return;

	//���� ������ �������� ���� ������ ��� �����Ѵ�.
	EDITITEMLISTITER itTempItem = pCurrentLine->end();
	--itTempItem;
	CEditItem *pItem = *itTempItem;
	if(IsLineFeedChar(pItem->m_str)) return;

	//���� ������ ù��° ���ڸ� ���ؿ´�.
	CEditLine *pNextLine = *itTempLine;
	itTempItem = pNextLine->begin();
	//�������ο� ù��° ���ڰ� ���ٸ� �ش� ������ �����Ѵ�.
	if(itTempItem == pNextLine->end())
	{
		delete pNextLine;
		itTempLine = m_listLines.erase(itTempLine);
		CheckDeleteMultiLine(itTempLine, fWidth);
		return;
	}
	pItem = *itTempItem;

	bool bLineFeed = IsLineFeedChar(pItem->m_str);
	//���� ������ ù��° ���ڰ� ���� �����ϰ�� ��������� ���� �������� �߰����ְ�
	//���� ������ �����Ѵ�.
	if(bLineFeed)
	{
		//���� ������ ù��° ���ڸ� �����.
		pNextLine->erase(itTempItem);
		delete pNextLine;
		m_listLines.erase(itTempLine);

		//���� ������ ���� ���ڸ� �߰��Ѵ�.
		pCurrentLine->push_back(pItem);
		return;
	}

	size_t nSize = pCurrentLine->size();
	float fTotalWidth = pCurrentLine->GetTotalWidth();
	if(nSize == 0)
	{
		//���� ������ ù��° ���ڰ� ���� ������ ��� ����
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

	//�߰��� ���� ��ü ���̶� ���ų� �۴ٸ� �������� �߰��Ѵ�.
	if(fTotalWidth <= fWidth)
	{
		//���� ������ ù��° ���ڸ� �����.
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

		//���� ������ ���� ���ڸ� �߰��Ѵ�.
		pCurrentLine->push_back(pItem);
		*pCurrentLine += pItem->GetWidth();
		*pCurrentLine += (float)m_cFontInfo.m_nSpaceBetweenLetters;

		//���� Ŀ���� ��ġ�� ���� ���϶� �̸鼭
		//������ �߰��ɶ� Ŀ���� ������ ù��° ���ڷ� �̵� ��Ų��.
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
	//���õ� ������ ������� �Ǵ� ���۰� ���� ���� ��� �ƹ����۵� ���� �ʴ´�.
	if(m_cSelectStart == m_itLineEnd || m_cSelectStart == m_cSelectEnd) return false;
	//���۶��ΰ� �������� ���ؿ´�.
	CEditLine *pLineStart = m_cSelectStart;
	CEditLine *pLineEnd = m_cSelectEnd;
	CEditItem *pItem = NULL;

	EDITLINELISTITER firstline = m_cSelectStart;

	//���� ������ ���� �������� ���Ѵ�.
	EDITPOS cPrevItem;
	bool bFirstItem = IsFirstItem(m_cSelectStart);
	if(!bFirstItem)	GetPrevItem(m_cSelectStart, cPrevItem);
	
	//���õ� ���� ���ΰ� �������� ���� ���
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

		//�ش� ������ ��� �����Ǿ��ٸ� �ش� ������ �����Ѵ�.
		if(pLineStart->size() == 0 && !IsLastLine(m_cSelectStart))
		{
			delete pLineStart;
			m_cSelectStart = m_listLines.erase(m_cSelectStart);
			//��� ������ ���� �Ǿ��ٸ�
			if(m_listLines.size() == 0)
			{
				m_cSelectStart = m_itLineEnd;
			}
			//ù��° ������ �ƴ϶�� ���� �������� �̵��Ѵ�.
			else if(!IsFirstLine(m_cSelectStart))
			{
				//���� ������ ���������� �̵��Ѵ�.
				m_cSelectStart.PrevLine();
				m_cSelectStart = ((CEditLine *)m_cSelectStart)->end();
			}
		}
		//�ش� ������ ��� �������� �ʾҴٸ� ������ ��ġ�� Ŀ���� ��ġ��Ų��.
		else
		{
			m_cSelectStart = start;
		}

		firstline = m_cSelectStart;
	}
	//���õ� ���� ���ΰ� �������� �ٸ����
	else
	{
		EDITLINELISTITER start_line = m_cSelectStart;
		EDITLINELISTITER end_line = m_cSelectEnd;

		//���۶����� ó������ �������� ���� ���ش�.
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

		//�ش� ������ ��� �����Ǿ��ٸ� �ش� ������ �����Ѵ�.
		if(pLineStart->size() == 0)
		{
			delete pLineStart;
			start_line = m_listLines.erase(start_line);
			//�ش� ������ ��� ���� �Ǿ��� ������
			//�ٹٲ� üũ�� ���� ���κ��� �ϵ��� ����
			firstline = start_line;
		}
		//�ش� ������ ��� �������� �ʾҴٸ� ���� �������� �̵��Ѵ�.
		else
		{
			++start_line;
		}

		bool bChangeLine = false;
		//���� ���ο��� ������ ���� ���α����� ��� ������ �����Ѵ�.
		while(start_line != end_line)
		{
			if(firstline == start_line) bChangeLine = true;
			else bChangeLine = false;

			delete *start_line;
			start_line = m_listLines.erase(start_line);

			if(bChangeLine) firstline = start_line;
		}

		//������ ������ ó�� ���� ��������� �������ش�.
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

		//������ ������ ��� �����Ǿ��ٸ�
		if(pLineEnd->size() == 0)
		{
			//ù���� ���� ������ ���α��� ��� �������� �����Ǿ��ٸ�
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
				//��� ������ ���� �Ǿ��ٸ�
				if(m_listLines.size() == 0)
				{
					m_cSelectEnd = m_itLineEnd;
					firstline = m_itLineEnd;
					m_ptStartPos = CPointZero;
				}
				//ù��° ������ �ƴ϶�� ���� �������� �̵��Ѵ�.
				else if(!IsFirstLine(m_cSelectEnd))
				{
					//���� ������ ���������� �̵��Ѵ�.
					m_cSelectEnd.PrevLine();
					m_cSelectEnd = ((CEditLine *)m_cSelectEnd)->end();
				}
			}
		}
		//�ش� ������ ��� �������� �ʾҴٸ� ������ ��ġ�� Ŀ���� ��ġ��Ų��.
		else
		{
			m_cSelectEnd = start_item;
		}

		m_cSelectStart = m_cSelectEnd;
	}

	m_cSelectStart = m_itLineEnd;
	m_cSelectEnd = m_itLineEnd;

	//���� ���������� �̵��Ѵ�.
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

	//�߰��Ǵ� ������ ������ ���Ѵ�.
	int len = int(strChar.length());
	if( len <= 0 )
		return;

	if(m_bNumber)
	{
		if(len != 1) return;
		char ch = strChar[0];
		if(ch < '0' || ch > '9') return;
	}

	//�߰��Ǵ� ���ڸ� �־����� �ִ� ���ڼ��� �Ѵ��� ���Ѵ�.
	if( m_nLimitText < (GetLength() + len) )
		return;

	//��Ƽ ����Ʈ�� ���� ����Ʈ�� �ԷµǾ������� �˻��ϴ� ����
	bool		bMerged = false;
	CEditItem	*pItem = new CEditItem;
	CEditItem	*pPrevItem = NULL;

	// ��Ƽ����Ʈ ĳ���Ͱ� �ϳ��� ���� �� ó���ϴ� �κ�, �̱� ����Ʈ�� ��������
	if(len == 1 && m_cCurrentPos != m_itLineEnd)
	{
		//���� Ŀ���� �� ���ڸ� ã�´�.
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
			//�������� ���� ���ڰ� �̱۹���Ʈ �ϰ��
			if( strlen(pPrevItem->m_str) == 1 )
			{
				char chSingleByte = pPrevItem->m_str[0];
				//��Ƽ ����Ʈ ������ ù��° ����Ʈ ������ �˻�
				if(::IsDBCSLeadByte(chSingleByte))
				{
					//�� ���� �����ۿ� �� �κ��� ä���ش�.
					pItem->m_str[0] = chSingleByte;
					strcpy_s(pItem->m_str + 1, 2, strChar.c_str());

					//���� ���� item�� �����Ѵ�.
					DeleteEditItem(true);
					bMerged = true;
				}
			}
		}
	}

	//��Ƽ����Ʈ�� ���� ����Ʈ�� ������ �ʾ��� ���
	if(!bMerged)
	{
		//���ο� ���� �����ۿ� ���� ä���ش�.
		strcpy_s( pItem->m_str, 3, strChar.c_str() );
	}

	SIZE size;

	HDC dc = GetDC(NULL);
	HGDIOBJ hOldFont = ::SelectObject( dc, m_cFontInfo.GetFont()->m_hObject );
	int nOldSpaceBetweenLetters = ::SetTextCharacterExtra( dc, m_cFontInfo.m_nSpaceBetweenLetters);

	bool bLineFeed = IsLineFeedChar(pItem->m_str);
	//���ڿ��� ���̸� ���ؼ� �����ۿ� ä���ش�.
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

	//�������� ���� ���Ѵ�.
	pItem->m_rtRect.SetRect(0,0,(float)size.cx,(float)size.cy);

	//�������� �߰��Ѵ�.
	InsertEditItem( pItem );
	m_pCursor->ResetTimer();
}

bool CEdit::IsByteToMerge(char const* pcchInput) const
{
	if(m_bNumber) return false;
	if(m_cCurrentPos == m_itLineEnd) return false;

	CEditLine *pLine = m_cCurrentPos;
	if(strlen(pcchInput) == 1) // �� ���ڰ� �̱۹���Ʈ�̰�, 
	{
		// �տ� ���ڰ� �ְ�,
		if(pLine->size() && (m_cCurrentPos != pLine->begin()))
		{
			EDITPOS cPrevPos = m_cCurrentPos;
			cPrevPos.PrevItem();

			CEditItem *pPrevItem = cPrevPos;
			if( strlen(pPrevItem->m_str)==1 ) // �� ���ڰ� �̱۹���Ʈ�̰�, 
			{
				char single_byte = pPrevItem->m_str[0];
				if(::IsDBCSLeadByte(single_byte)) // LeadByte�� ��
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
	//���õ� ������ ������ ������ �ʴ´�.
	if(cPrevPos == m_itLineEnd)
	{
		return NULL;
	}

	//���� ���ο� �������� �������� �ʴ´ٸ�
	//���� ������ ���� ������ �������� ���Ѵ�.
	CEditLine *pLine = cPrevPos; 
	if(pLine->size() == 0)
	{
		//���� ������ ���� ù �����̸� ���� �������� �������� �ʴ´�.
		if(IsFirstLine(cPrevPos))
		{
			cPrevPos = m_itLineEnd;
			return NULL;
		}

		//���� ������ ���� ������ �������� ���Ѵ�.
		cPrevPos.PrevLine();
		pLine = cPrevPos;
		cPrevPos = pLine->end();
		cPrevPos.PrevItem();
		return cPrevPos;
	}

	pLine = cPrevPos;
	//���� ���ο� �������� �����Ҷ�
	//���� ������ ���� ù �������� ���
	if(cPrevPos == pLine->begin())
	{
		//���� ������ ���� ù �����̸� ���� �������� �������� �ʴ´�.
		if(IsFirstLine(cPrevPos))
		{
			cPrevPos = m_itLineEnd;
			return NULL;
		}
		//���� ������ ���� ������ �������� ���Ѵ�.
		
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
	//���õ� ������ �����鱸���� �ʴ´�.
	if(cNextPos == m_itLineEnd)
	{
		return NULL;
	}

	CEditLine *pLine = cNextPos;
	//���� ���ο� �������� ���ٸ� ���� ������ �̹Ƿ� ���� ��ġ�� �������� �ʴ´�.
	if(pLine->size() == 0)
	{
		cNextPos = m_itLineEnd;
		return NULL;
	}

	//���� ���ο� �������� �����Ҷ�
	//���� ������ ���� �� �������� ���
	if(cNextPos == pLine->end())
	{
		EDITITEMLISTITER end_item = pLine->end();
		--end_item;
		CEditItem *pItem = *end_item;

		//�� ������ �������� ���� �������� ����
		bool bLineFeed = IsLineFeedChar(pItem->m_str);

		//���� �������� �̵�
		cNextPos.NextLine();
		//���� ������ �������� �ʴ´ٸ� �������� ������ �ʰ� ���� ��ġ�� �ǵ�����.
		if(cNextPos == m_itLineEnd)
		{
			cNextPos.PrevLine();
			return NULL;
		}

		pLine = cNextPos;
		//���� ���ο� �������� ���ٸ� ���θ� �����ϰ� �������� �Ѱ����� �ʴ´�.
		if(pLine->size() == 0)
		{
			cNextPos = pLine->end();
			return NULL;
		}

		//���� ������ ���� ó�� �������� ���ؼ� �Ѱ��ش�.
		cNextPos = pLine->begin();
		//���๮�ڰ� �ƴϸ� ���� ��ġ�� �̵���Ų��.
		if(!bLineFeed) cNextPos.NextItem();
		return cNextPos;
	}

	//���� ���������� �̵�
	cNextPos.NextItem();
	//�� ������ ���� ������ �ϰ�� ���� �������� �̵�
	if(cNextPos == pLine->end())
	{
		cNextPos.NextLine();
		//���� ������ �����̾��ٸ�
		//���� ������ ���� �޺κ����� �̵�
		if(cNextPos == m_listLines.end())
		{
			cNextPos.PrevLine();
			cNextPos = pLine->end();
			return NULL;
		}

		//���� ������ ������ ���̿��ٸ�
		//���� ������ ���� ������ �̵�
		pLine = cNextPos;
		//���� �� �����̶��
		if(pLine->size() == 0)
		{
			cNextPos = pLine->end();
			return NULL;
		}

		cNextPos = pLine->begin();
	}
	else
	{
		//�������� �ƴ����� ���� ������ ��� ���� ������ ó������ �̵��Ѵ�.
		CEditItem *pItem = cNextPos;
		bool bLineFeed = IsLineFeedChar(pItem->m_str);
		if(bLineFeed)
		{
			cNextPos.NextLine();
			//���� ������ �����̾��ٸ�
			//���� ������ ���� �޺κ����� �̵�
			if(cNextPos == m_listLines.end())
			{
				cNextPos.PrevLine();
				cNextPos = pLine->end();
				return NULL;
			}

			//���� ������ ������ ���̿��ٸ�
			//���� ������ ���� ������ �̵�
			pLine = cNextPos;
			//���� �� �����̶��
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
	//�Է��� ������ ó���̴�.
	if(m_listLines.size() == 0) return true;
	//ù��° ������ �ƴϸ� ù���ڰ� �ƴϴ�.
	if(!IsFirstLine(cPos)) return false;
	//���ο� �������� ������ ù�����̴�.
	CEditLine *pLine = cPos;
	if(pLine->size() == 0) return true;
	//������ ù��° �����̸� ù�����̴�.
	if(cPos == pLine->begin()) return true;
	return false;
}

bool CEdit::IsLastItem(EDITPOS &cPos)
{
	//�Է��� ������ ó���̴�.
	if(m_listLines.size() == 0) return true;
	//������ ������ �ƴϸ� ������ ���ڰ� �ƴϴ�.
	if(!IsLastLine(cPos)) return false;
	//���ο� �������� ������ ������ �������̴�.
	CEditLine *pLine = cPos;
	if(pLine->size() == 0) return true;
	//������ ������ �������� �˻�
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
	//�Է��� ������ ó���̴�. ������ ó�� �����δ�.
	if(m_listLines.size() == 0) return true;
	//ù��° ���������� �˻����ϴ�.
	if(cPos == m_listLines.begin()) return true;
	return false;
}

bool CEdit::IsLastLine(EDITPOS &cPos)
{
	//�Է��� ������ ������ �����̴�.
	if(m_listLines.size() == 0) return true;
	//ù��° ���������� �˻����ϴ�.
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

	//w ��ġ�� y��ǥ�� z��ġ ���� �۴ٸ� z�� ū ��ġ�̴�.
	if(pLine1->GetPosY() < pLine2->GetPosY()) return false;
	//w ��ġ�� y��ǥ�� z��ġ ���� ũ�ٸ� w�� ū ��ġ�̴�.
	if(pLine1->GetPosY() > pLine2->GetPosY()) return true;
	
	//���� y��ǥ���� ��� ���� ����Ű�� ������ ���� ���̴�.
	if(w == pLine1->end() && z == pLine2->end()) return true;
	//z�� ���� ����Ű�� ������ z�� ū ���̴�.
	if(z == pLine2->end()) return false;
	//w�� ���� ����Ű�� �ִٸ� w�� ū ���̴�.
	if(w == pLine1->end()) return true;

	CEditItem *pItem1 = w;
	CEditItem *pItem2 = z;

	//��� ���� �ƴҰ��
	//w ��ġ�� x��ǥ�� z��ġ ���� �۴ٸ� z�� ū ��ġ�̴�.
	if(pItem1->left() < pItem2->left()) return false;
	//w ��ġ�� y��ǥ�� z��ġ ���� ũ�ٸ� w�� ū ��ġ�̴�.
	if(pItem1->left() > pItem2->left()) return true;
	return true;
}

bool CEdit::BigItem(EDITPOS &w, EDITPOS &z)
{
	CEditLine *pLine1 = w;
	CEditLine *pLine2 = z;

	//w ��ġ�� y��ǥ�� z��ġ ���� �۴ٸ� z�� ū ��ġ�̴�.
	if(pLine1->GetPosY() < pLine2->GetPosY()) return false;
	//w ��ġ�� y��ǥ�� z��ġ ���� ũ�ٸ� w�� ū ��ġ�̴�.
	if(pLine1->GetPosY() > pLine2->GetPosY()) return true;

	//���� y��ǥ���� ��� ���� ����Ű�� ������ ���� ���̴�.
	if(w == pLine1->end() && z == pLine2->end()) return false;
	//z�� ���� ����Ű�� ������ z�� ū ���̴�.
	if(z == pLine2->end()) return false;
	//w�� ���� ����Ű�� �ִٸ� w�� ū ���̴�.
	if(w == pLine1->end()) return true;

	CEditItem *pItem1 = w;
	CEditItem *pItem2 = z;

	//��� ���� �ƴҰ��
	//w ��ġ�� x��ǥ�� z��ġ ���� �۴ٸ� z�� ū ��ġ�̴�.
	if(pItem1->left() < pItem2->left()) return false;
	//w ��ġ�� y��ǥ�� z��ġ ���� ũ�ٸ� w�� ū ��ġ�̴�.
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
	//���� Ŀ���� ��ġ�� ���ٸ� ����
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
	//�Է��� ������ ��� ����
	if(m_listLines.size() == 0) return;
	//���� Ŀ���� ��ġ�� ���ٸ� ����
	if(m_cCurrentPos == m_itLineEnd) return;

	EDITPOS cOldCurrentPos = m_cCurrentPos;

	//������ ���� ó������ �̵���Ų��.
	m_cCurrentPos = ((CEditLine *)m_cCurrentPos)->begin();
	//�ش������ ���� ó������ �̵��Ѵ�.

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
	//�Է��� ������ ��� ����
	if(m_listLines.size() == 0) return;
	//���� Ŀ���� ��ġ�� ���ٸ� ����
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
	//�Է��� ������ ��� ����
	if(m_listLines.size() == 0) return;
	//���� Ŀ���� ��ġ�� ���ٸ� ����
	if(m_cCurrentPos == m_itLineEnd) return;

	EDITPOS cOldCurrentPos = m_cCurrentPos;

	//���� ������ ���� �ڷ� �̵���Ų��.
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
	//���� Ŀ���� ��ġ�� ���ٸ� ����
	if(m_cCurrentPos == m_itLineEnd) return;
	//�̱۶��� ����̸� ����
	if(m_cFontInfo.m_uFormat & DT_SINGLELINE) return;
	//ù��° �����̸� ó������ �ʴ´�.
	if(IsFirstLine(m_cCurrentPos)) return;

	EDITPOS cOldCurrentPos = m_cCurrentPos;

	//Up/Down�ÿ��� ��Ʈ��Ű�� ���� ó���� �����Ѵ�.
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
	//���� Ŀ���� ��ġ�� ���ٸ� ����
	if(m_cCurrentPos == m_itLineEnd) return;
	//�̱۶��� ����̸� ����
	if(m_cFontInfo.m_uFormat & DT_SINGLELINE) return;
	//ù��° �����̸� ó������ �ʴ´�.
	if(IsLastLine(m_cCurrentPos)) return;

	EDITPOS cOldCurrentPos = m_cCurrentPos;

	//Up/Down�ÿ��� ��Ʈ��Ű�� ���� ó���� �����Ѵ�.
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
	//���� Ŀ���� ��ġ�� ���ٸ� ����
	if(m_cCurrentPos == m_itLineEnd) return;
	//�̱۶��� ����̸� ����
	if(m_cFontInfo.m_uFormat & DT_SINGLELINE) return;
	//ù��° �����̸� ó������ �ʴ´�.
	if(IsFirstLine(m_cCurrentPos)) return;

	EDITPOS cOldCurrentPos = m_cCurrentPos;

	//Up/Down�ÿ��� ��Ʈ��Ű�� ���� ó���� �����Ѵ�.
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
	//���� Ŀ���� ��ġ�� ���ٸ� ����
	if(m_cCurrentPos == m_itLineEnd) return;
	//�̱۶��� ����̸� ����
	if(m_cFontInfo.m_uFormat & DT_SINGLELINE) return;
	//ù��° �����̸� ó������ �ʴ´�.
	if(IsLastLine(m_cCurrentPos)) return;

	EDITPOS cOldCurrentPos = m_cCurrentPos;

	//Up/Down�ÿ��� ��Ʈ��Ű�� ���� ó���� �����Ѵ�.
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
	//�Է��� ������ ��� ����
	if(m_listLines.size() == 0) return;
	//���� Ŀ���� ��ġ�� ���ٸ� ����
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
	//�� ������ ��ġ�� �� �������ش�.
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

		//���� �Էµ� ������ ���ٸ�
		if(m_listLines.size() == 0)
		{
			if(m_cFontInfo.m_uFormat & DT_RIGHT) m_ptStartPos.x = fWidth;
			else if(m_cFontInfo.m_uFormat & DT_CENTER) m_ptStartPos.x = fWidth/2;
			else m_ptStartPos.x = 0;

			m_pCursor->SetLocalPos(m_ptStartPos);
		}
		else
		{
			//ù��° ������ �����´�.
			CEditLine *pLine = *m_listLines.begin();
			const float &fTotalLineWidth = pLine->GetTotalWidth();
			//���ο� �߰��� ���ڰ� ���� ��� �ش� ������ ������ ������.
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
			//ǥ�� ������ �۴ٸ�
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
					//���� Ŀ���� ���� ���ڸ� ���Ѵ�.
					CEditItem *pPrevItem = GetPrevItem(m_cCurrentPos, cPrevPos);
					if(GetCursorText() == "") m_pCursor->SetLocalPos(m_ptStartPos + pPrevItem->m_rtRect.GetRightTop());
					else m_pCursor->SetLocalPos(m_ptStartPos + pPrevItem->m_rtRect.GetLeftTop());
				}
			}
			//ǥ�� ������ ũ�ų� ���ٸ�
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
					//���� Ŀ���� ���� ���ڸ� ���Ѵ�.
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
		//��ü ���̰� ǥ�� ���� ���� �۴ٸ� ���� ������ �����Ѵ�.
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
		//���� �Էµ� ������ ���ٸ�
		if(m_listLines.size() == 0)
		{
			if(m_cFontInfo.m_uFormat & DT_RIGHT) ptCursor.x = fWidth;
			else if(m_cFontInfo.m_uFormat & DT_CENTER) ptCursor.x = fWidth/2;
			else ptCursor.x = 0;

			ptCursor.y = m_ptStartPos.y;
		}
		else
		{
			//Ŀ���� ������ �����´�.
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
				//���� Ŀ���� ���� ���ڸ� ���Ѵ�.
				CEditItem *pPrevItem = GetPrevItem(m_cCurrentPos, cPrevPos);
				if(GetCursorText() == "") ptCursor = m_ptStartPos + pLine->GetPoint() + pPrevItem->m_rtRect.GetRightTop();
				else ptCursor = m_ptStartPos + pLine->GetPoint() + pPrevItem->m_rtRect.GetLeftTop();
			}

			//�Է� �Ǵ� Ŀ���� �̵����� ���� ��ġ ������ �ʿ��Ҷ�
			if(m_bAddScroll)
			{
				//Ŀ���� y ��ġ�� 0���� �۴��̸� y��ġ�� 0���� �������ش�.
				if(ptCursor.y < 0)
				{
					m_ptStartPos.y -= ptCursor.y;
					ptCursor.y = 0;
					SetRedraw();
				}
				//Ŀ���� ���̴� ��ġ�� ���̴� ���̺��� ũ���̸� ���� �Ʒ��� �������ش�.
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
		
		//���ν�ũ���� üũ���� ���θ� �����Ѵ�.
		bool bMoveCheckLineScroll = true;
		float fMaxHeight = fHeight - m_fTotalHeight;
		
		//���������� ��ũ�� �Ǿ� ������ �������� �ʴ´�.
		if(fMaxHeight == m_ptStartPos.y)
		{
			bMoveCheckLineScroll = false;
		}
		//��ũ�ѹٰ� �ְ� ���ν�ũ���� �ƴϸ� �������� �ʴ´�.
		else if(m_pScrollBar && !m_bLineScroll)
		{
			bMoveCheckLineScroll = false;
		}

		//��� ������ �׸���.
		EDITPOS cTempPos;
		size_t nSize = 0;
		for(;start != end;++start)
		{
			pLine = *start;
			//ȭ�鿡 ���̴��� üũ�Ѵ�.
			//ȭ�鿡 ���϶��� �׸���.
			if(m_ptStartPos.y + pLine->GetPosY() + m_fLineHeight > 0)
			{
				if(bFirst)
				{
					m_cViewFirstItem = start;
					m_cViewFirstItem = pLine->begin();
					bFirst = false;

					//ù��° ������ ��ġ�� 0���� �۴� �̸� 0���� �������ش�.
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
				//�׶����� ��� ���ڸ� �׸���.
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
			//ȭ�鿡 ���̴� �κ��� �Ѿ��� �׸��⸦ �ߴ��Ѵ�.
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
			//��Ʈ�� Ű�� ������ ������ �Է��� ������� �ʴ´�.
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

			//��Ƽ ����Ʈ�� �ռ� ���϶� �Ʒ��� �����Ų��.
			if(IsByteToMerge(ach_wparam)) 
			{
			}
			else
			{
				//ALT + ���ڷ� �Է��ϴ� ĳ���� �Է�ó��
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
			if( wParam == IMN_SETCONVERSIONMODE ) // ��Ÿ�� �̰� �� �� ���� 3�� �´�. (false-true-true ������)
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
	case WM_IME_COMPOSITION: // Windows 2000 �̻�
		{
			// �ѱ��� �������� ���� GCS_COMPSTR | GCS_COMPATTR ����� GCS_RESULTSTR�� ���� (�ٸ� �÷��� �� ����)
			// �ѱ� �𿡼� ���� �Է½�, GCS_RESULTSTR�� ������ ��� ���� (WM_IME_STARTCOMPOSITION�� ����)
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
			// �����߿� �齺���̽��� ����� ���
			// XP�� 0, Vista������ 0x1b(27)�� �´�. 
			{
				// ��, �߱���� ��� �̸� �´�. ���� �߿�
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

	//Ŭ�����忡 �����Ѵ�.
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

	//���ٸ� ���õǾ� �ִٸ�
	if(pLineStart == pLineEnd)
	{
		//���õ� ù��° ������ ���ڵ��� ���ؿ´�.
		EDITITEMLISTITER start_item = m_cSelectStart;
		EDITITEMLISTITER end_item  = m_cSelectEnd;

		for(;start_item != end_item;++start_item)
		{
			pItem = *start_item;
			strOutput += pItem->m_str;
		}
	}
	//�������� ���õǾ� �ִٸ�
	else
	{
		//ù��° ������ ���ڵ��� ���ؿ´�.
		EDITITEMLISTITER start_item = m_cSelectStart;
		EDITITEMLISTITER end_item  = pLineStart->end();

		for(;start_item != end_item;++start_item)
		{
			pItem = *start_item;
			strOutput += pItem->m_str;
		}

		//������������ �̵��Ѵ�.
		++start_line;
		//������ ������ �ƴ϶�� ��� ���ڸ� �����ش�.
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

		//������ ������ ó������ ������ ������ ���Ѵ�.
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
	//�Էµ� ���ڰ� �ϳ��� ���� ���
	if(m_listLines.size() == 0) return NULL;
	m_cViewFind = cStart;
	//ȭ�鿡 ���̴� ���ڰ� ���� ���
	if(m_cViewFind == m_itLineEnd) return NULL;
	//ȭ�鿡 ù��°�� ���̴� ������ �����Ѵ�.
	return m_cViewFind;
}

CEditLine *CEdit::GetViewNextLine(EDITPOS &cStart, EDITPOS &cEnd)
{
	//�Էµ� ���ڰ� �ϳ��� ���� ���
	if(m_listLines.size() == 0) return NULL;
	//ȭ�鿡 ���̴� ���ڰ� ���� ���
	if(m_cViewFind == m_itLineEnd) return NULL;
	CEditLine *pLine = m_cViewFind;
	CEditLine *pEndLine = cEnd;
	//�̹� ������ ���α��� �������� �˻��� ����� ����
	if(pLine == pEndLine) return NULL;
	m_cViewFind.NextLine();
	m_cViewFind = ((CEditLine *)m_cViewFind)->begin();
	return m_cViewFind;
}

CEditItem *CEdit::GetViewFirstItem(EDITPOS &cPos)
{
	//�Էµ� ���ڰ� �ϳ��� ���� ���
	if(m_listLines.size() == 0) return NULL;
	//�˻��� ������ ���� ���
	if(cPos == m_itLineEnd) return NULL;

	m_cViewFind = cPos;
	CEditLine *pLine = m_cViewFind;
	//������ ��� ���� ���
	if(pLine->size() == 0) return NULL;
	//������ �������� ���
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

	//� ������ Ŭ���ߴ����� �����Ѵ�.
	CEditLine *pLine = GetViewFirstLine(cStart, cEnd);
	if(pLine == NULL) return cCurrent;
	//ù��° ���κ��� �����̸� ù��° ������ �����Ѵ�.
	if(pLine->IsPrevLineIn(ptLocal, m_ptStartPos))
	{
		cCurrent = m_cViewFirstItem;
	}
	else
	{
		while(pLine)
		{
			//�ش��ϴ� ������ ã�´�.
			if(pLine->IsLineIn(ptLocal, m_ptStartPos, m_fLineSpace, m_fLineHeight))
			{
				cCurrent = m_cViewFind;
				break;
			}
			pLine = GetViewNextLine(cStart, cEnd);
		}

		//�ش��ϴ� ������ ������ ���� ������ �����̴�.
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
				
				//���� ������ ���ڰ� ���� ���� �����̸�
				//���๮�ڷ� Ŀ���� �̵����Ѿ� �Ѵ�.
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

	//������ ���̸� ���Ѵ�.
	CSize szSize;
	if(m_fLineHeight == 0.0f)
	{
		::GetTextExtentPoint32( pDC->m_hDC, "A", 1, &szSize );
		m_fLineHeight = (float)szSize.cy;
	}

	m_fTotalHeight = 0;

	//�� ������ ���� ��ġ�� ���Ѵ�.
	EDITLINELISTITER start = m_listLines.begin();
	EDITLINELISTITER end = m_itLineEnd;

	EDITITEMLISTITER start_item, end_item;

	CEditLine *pLine = NULL;
	CPoint ptStartPos = CPointZero;

	for(;start != end;++start)
	{
		pLine = *start;
		*pLine = ptStartPos;
		ptStartPos.y += m_fLineHeight;		  //������ ���̸� �����ش�.
		ptStartPos.y += (float)m_fLineSpace; //�ٰ����� �����ش�.
	}

	if(pLine) m_fTotalHeight = pLine->GetPosY() + m_fLineHeight;
	else m_fTotalHeight = m_fLineHeight;

	float fWidth = m_rtLocalRect.GetWidth();
	float fHeight = m_rtLocalRect.GetHeight();

	//�̱� �����϶��� ó���ϴ� ������ ����
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
		//��Ƽ �����϶� �� ������ ���� ��ġ�� ������ش�.
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

					//�� �κ��� ��� �����Ѵ�.
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
					//�������� ���� �������� �ش� ������ �����Ѵ�.
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