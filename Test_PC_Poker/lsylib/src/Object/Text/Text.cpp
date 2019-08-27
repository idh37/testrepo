#include "stdafx.h"
#include "Text.h"
#include "../Base/DumyImage.h"
#include "../../tinyxml/tinyxml.h"
#include "../../Utility/MyMime64.h"
#include "../ObjectMan.h"
#include "../../Functor/Message.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace LSY
{
CText::SLIDE_ITEM::SLIDE_ITEM()
:	m_nRepeat(0),
	m_uFormat(0),
	m_nDrawMode(0),
	m_dwSlideType(0),
	m_dwStartTime(0),
	m_dwDelayTime(0),
	m_nCurStatus(SS_START)
{
	m_szSize.cx = 0;
	m_szSize.cy = 0;
}

CText::SLIDE_ITEM::~SLIDE_ITEM()
{
}

CText::CText(void)
:	m_strText(""),
	m_bRedraw(false),
	m_bDot(false),
	m_nLineSpace(1),
	m_nTotalHight(0),
	m_pBack(NULL),
	m_bSlide(false),
	m_dwSlideType(TEXT_STS_RIGHT|TEXT_STE_LEFT),
	m_dwSlideDelayTime(0),
	m_nSlideRepeatCount(-1),
	m_fSlideGap(50.0f),
	m_strAddString(""),
	m_nLineCount(0)
{
	m_pBack = new CDumyImage;
	AddLocalChild(m_pBack);
}

CText::~CText()
{
	ClearSlideString(true);
}

bool CText::Create(int nWidth, int nHeight)
{
	if(!m_pBack->Create(nWidth, nHeight)) return false;

	SetLocalRect(m_pBack->GetLocalRect());
	SetRedraw();

	return true;
}

void CText::SetText(const std::string &strText)
{
	CString strTemp = strText.c_str();
	strTemp.Replace( "\\t", "\t" );
	strTemp.Replace( "\\r", "\r" );
	strTemp.Replace( "\\n", "\n" );

	m_strText = strTemp.GetString();

	if(m_bSlide)
	{
		if(m_strText != "") AddSlideString_Back(m_strText);
	}
	else 
	{
		SetRedraw();
	}
}

void CText::SetAddText(const std::string &strAddText)
{
	CString strTemp = strAddText.c_str();
	strTemp.Replace( "\\t", "\t" );
	strTemp.Replace( "\\r", "\r" );
	strTemp.Replace( "\\n", "\n" );

	m_strAddString = strTemp.GetString();

	if(m_bSlide)
	{
	}
	else 
	{
		SetRedraw();
	}
}

void CText::SetRedraw(bool bRedraw)
{
	if(m_bRedraw == bRedraw) return;
	m_bRedraw = bRedraw;
	CheckEventDraw();
}

void CText::Draw(NMBASE::GRAPHICGDI::CPage *pPage)
{
	if(m_bRedraw) CheckRedraw();
	__super::Draw(pPage);
}

bool CText::OnSetLineCount(const int &nLine)
{
	if(m_nLineCount == nLine) return false;
	m_nLineCount = nLine;

	CMO_SetTextLineCount msg(this, nLine);
	return Dispatch(&msg)?true:false;
}

int CText::CheckRedraw(void)
{
	SetRedraw(false);
	if(m_cFontInfo.GetFont() == NULL || m_strText == "")
	{
		m_pBack->ClearColor(RGB(0,0,255));
		return 0;
	}

	CDC *pDC = m_pBack->GetDC();
	if(pDC == NULL) return 0;

	pDC->SelectObject((CFont *)m_cFontInfo.GetFont());
	pDC->SetTextCharacterExtra(m_cFontInfo.m_nSpaceBetweenLetters);

	//실제 출력 문자열을 만든다.
	LISTSTRINGINFO listText;

	MakeDotText(pDC);
	MakeMultiLine(pDC, listText);

	if(OnSetLineCount((int)listText.size()))
	{
		m_pBack->ReleaseDC();
		pDC = m_pBack->GetDC();

		m_pBack->ClearColor(RGB(0,0,255));

		pDC->SelectObject((CFont *)m_cFontInfo.GetFont());
		pDC->SetTextCharacterExtra(m_cFontInfo.m_nSpaceBetweenLetters);
	}
	else
	{
		m_pBack->ClearColor(RGB(0,0,255));
	}

	switch(m_cFontInfo.m_nDrawType)
	{
	case ETDT_NORMAL:
		pDC->SetTextColor(m_cFontInfo.m_clrColor);
		DrawText(pDC, listText);
		break;
	case ETDT_SHADOW:
		pDC->SetTextColor(m_cFontInfo.m_clrOutline);
		DrawText(pDC, listText, CPoint(1.0f, 1.0f));
		pDC->SetTextColor(m_cFontInfo.m_clrColor);
		DrawText(pDC, listText);
		break;
	case ETDT_OUTLINE:
		pDC->SetTextColor(m_cFontInfo.m_clrOutline);
		DrawText(pDC, listText, CPoint(0.0f, 1.0f));
		DrawText(pDC, listText, CPoint(2.0f, 1.0f));
		DrawText(pDC, listText, CPoint(1.0f, 0.0f));
		DrawText(pDC, listText, CPoint(1.0f, 2.0f));
		pDC->SetTextColor(m_cFontInfo.m_clrColor);
		DrawText(pDC, listText, CPoint(1.0f, 1.0f));
		break;
	}

	m_pBack->ReleaseDC();

	return (int)listText.size();
}

int CText::GetTextWidth(const CDC *pDC, LPCTSTR lpText, const int &nLength)
{
	CSize szSize;
	::GetTextExtentPoint( pDC->m_hDC, lpText, nLength, &szSize );

	switch(m_cFontInfo.m_nDrawType)
	{
	case ETDT_SHADOW:
		szSize.cx += 1;
		break;
	case ETDT_OUTLINE:
		szSize.cx += 2;
		break;
	}

	return szSize.cx;
}

int CText::GetTextHeight(const CDC *pDC, LPCTSTR lpText, const int &nLength)
{
	CSize szSize;
	::GetTextExtentPoint( pDC->m_hDC, lpText, nLength, &szSize );

	switch(m_cFontInfo.m_nDrawType)
	{
	case ETDT_SHADOW:
		szSize.cy += 1;
		break;
	case ETDT_OUTLINE:
		szSize.cy += 2;
		break;
	}

	return szSize.cy;
}

CSize CText::GetTextSize(const CDC *pDC, LPCTSTR lpText, const int &nLength)
{
	CSize szSize;
	::GetTextExtentPoint( pDC->m_hDC, lpText, nLength, &szSize );

	switch(m_cFontInfo.m_nDrawType)
	{
	case ETDT_SHADOW:
		szSize.cx += 1;
		szSize.cy += 1;
		break;
	case ETDT_OUTLINE:
		szSize.cx += 2;
		szSize.cy += 2;
		break;
	}

	return szSize;
}

void CText::MakeDotText(CDC *pDC)
{
	if(!m_bDot)
	{
		m_strOutput = m_strText;
		m_strOutput += m_strAddString;
		return;
	}

	m_strOutput = "";

	std::string strText = m_strText + m_strAddString;

	if(strText.size() == 0)
		return;

	int nWidth = (int)m_pBack->GetLocalRect().GetWidth();
	int cx = GetTextWidth(pDC, strText.c_str(), strText.length());
	if(cx <= nWidth)
	{
		m_strOutput = strText;
		return;
	}

	cx = GetTextWidth(pDC, "...", 3);
	int nAddWidth = GetTextWidth(pDC, m_strAddString.c_str(), m_strAddString.size());
	int nTempWidth = nWidth - cx - nAddWidth;

	int nLength = (int)m_strText.length();
	LPCTSTR lpszText = m_strText.c_str();

	int nAddPos = 0;
	int nEndPos = 0;

	while(TRUE)
	{
		if(IsDBCSLeadByte(m_strText.at(nEndPos))) nAddPos = 2;
		else nAddPos = 1;

		nEndPos += nAddPos;

		cx = GetTextWidth(pDC, lpszText, nEndPos);
		if(nTempWidth <= cx)
		{
			if(nTempWidth != cx)
			{
				nEndPos -= nAddPos;
			}

			m_strOutput = m_strText.substr(0, nEndPos);
			m_strOutput += "...";
			m_strOutput += m_strAddString;
			return;
		}
		if(nLength <= nEndPos) break;
	}

	m_strOutput = m_strText;
	m_strOutput += m_strAddString;
}

void CText::MakeMultiLine(CDC *pDC, LISTSTRINGINFO &listText)
{
	if(m_strOutput == "") return;

	m_nTotalHight = GetTextHeight(pDC, m_strOutput.c_str(), m_strOutput.length());
	if(m_bDot || (m_cFontInfo.m_uFormat & DT_SINGLELINE))
	{
		int nWidth = GetTextWidth(pDC, m_strOutput.c_str(), m_strOutput.length());
		listText.push_back(make_pair(m_strOutput, nWidth));
	}
	else
	{
		int nLength = (int)m_strOutput.length();
		LPCTSTR lpszText = m_strOutput.c_str();

		int nAddPos = 0;
		int nEndPos = 0;
		int nStartPos = 0;
		int nWidth = (int)m_pBack->GetLocalRect().GetWidth();
		int cx = 0;
		std::string strOutput;
		bool bNewLine = false;

		while(TRUE)
		{
			bNewLine = false;
			if(m_strOutput.at(nEndPos) == '\r')
			{
				//마지막 전 문자일 경우 \n을 비교한다.
				if((nEndPos < (nLength-1)) && m_strOutput.at(nEndPos+1) == '\n')
				{
					nAddPos = 2;
				}
				else
				{
					nAddPos = 1;
				}
				bNewLine = true;
			}
			else if(m_strOutput.at(nEndPos) == '\n')
			{
				nAddPos = 1;
				bNewLine = true;
			}
			else if(IsDBCSLeadByte(m_strOutput.at(nEndPos))) nAddPos = 2;
			else nAddPos = 1;

			nEndPos += nAddPos;

			if(bNewLine)
			{
				cx = GetTextWidth(pDC, lpszText + nStartPos, nEndPos - nStartPos - nAddPos);
				strOutput = m_strOutput.substr(nStartPos, nEndPos - nStartPos - nAddPos);
				listText.push_back(make_pair(strOutput, cx));
				nStartPos = nEndPos;
			}
			else
			{
				cx = GetTextWidth(pDC, lpszText + nStartPos, nEndPos - nStartPos);
				if(nWidth <= cx)
				{
					if(nWidth != cx)
					{
						nEndPos -= nAddPos;
						cx = GetTextWidth(pDC, lpszText + nStartPos, nEndPos - nStartPos);
					}

					strOutput = m_strOutput.substr(nStartPos, nEndPos - nStartPos);
					listText.push_back(make_pair(strOutput, cx));
					nStartPos = nEndPos;
				}
			}

			if(nLength <= nEndPos)
			{
				if(nStartPos <= (nEndPos-nAddPos))
				{
					strOutput = m_strOutput.substr(nStartPos, nEndPos - nStartPos);
					listText.push_back(make_pair(strOutput, cx));
				}
				break;
			}
		}

		int nLine = (int)listText.size();
		m_nTotalHight *= nLine;
		if(nLine > 2)
		{
			m_nTotalHight += (nLine - 1) * m_nLineSpace;
		}
	}
}

void CText::DrawText(const CDC *pDC, const LISTSTRINGINFO &listText, const CPoint &ptGab)
{
	if((int)listText.size() == 0) return;
	LISTSTRINGINFO::const_iterator start = listText.begin();
	LISTSTRINGINFO::const_iterator end = listText.end();

	CRect rtTextRect = m_pBack->GetLocalRect();
	float fStartY = rtTextRect.top;
	int nWidth = (int)rtTextRect.GetWidth();
	int nHeight = (int)rtTextRect.GetHeight();

	if(m_cFontInfo.m_uFormat & DT_VCENTER)
	{
		fStartY -= (m_nTotalHight - nHeight)/2;
	}
	else if(m_cFontInfo.m_uFormat & DT_BOTTOM)
	{
		fStartY = rtTextRect.bottom - m_nTotalHight;
	}

	fStartY += ptGab.y;

	float fStartX = 0;
	int nCharHeight = GetTextHeight(pDC, m_strOutput.c_str(), m_strOutput.length());

	for(;start != end;++start)
	{
		fStartX = rtTextRect.left;

		if(m_cFontInfo.m_uFormat & DT_RIGHT)
		{
			fStartX = nWidth - (float)start->second;
		}
		else if(m_cFontInfo.m_uFormat & DT_CENTER)
		{
			fStartX -= float((start->second - nWidth)/2);
		}

		fStartX += ptGab.x;

		::TextOut(pDC->m_hDC, (int)fStartX, (int)fStartY, start->first.c_str(), (int)start->first.size());
		fStartY += (nCharHeight + m_nLineSpace);
	}
}

const UINT CText::SetFormat(const UINT &uFormat)
{
	if(m_cFontInfo.m_uFormat == uFormat) return m_cFontInfo.m_uFormat;
	UINT uOldFormat = m_cFontInfo.m_uFormat;
	m_cFontInfo.m_uFormat = uFormat;
	SetRedraw(true);
	return uOldFormat;
}

const int CText::SetLineSpace(int nLineSpace)
{
	if(m_nLineSpace == nLineSpace) return m_nLineSpace;
	int nOldLineSpace = m_nLineSpace;
	m_nLineSpace = nLineSpace;
	SetRedraw(true);
	return nOldLineSpace;
}

const ENUM_TEXTDRAW_TYPE CText::SetTextDrawType(ENUM_TEXTDRAW_TYPE nType)
{
	if(m_cFontInfo.m_nDrawType == nType) return m_cFontInfo.m_nDrawType;
	ENUM_TEXTDRAW_TYPE nOldDrawType = m_cFontInfo.m_nDrawType;
	m_cFontInfo.m_nDrawType = nType;
	SetRedraw(true);
	return nOldDrawType;
}

const bool CText::SetUseDot(const bool &bUseDot)
{
	if(m_bDot == bUseDot) return m_bDot;
	bool bOldUseDot = m_bDot;
	m_bDot = bUseDot;
	SetRedraw(true);
	return bOldUseDot;
}

const COLORREF CText::SetTextColor(COLORREF clrColor)
{
	if(m_cFontInfo.m_clrColor == clrColor) return m_cFontInfo.m_clrColor;
	COLORREF bOldColor = m_cFontInfo.m_clrColor;
	m_cFontInfo.m_clrColor = clrColor;
	SetRedraw(true);
	return bOldColor;
}

const COLORREF CText::SetShadowColor(COLORREF clrColor)
{
	if(m_cFontInfo.m_clrOutline == clrColor) return m_cFontInfo.m_clrOutline;
	COLORREF bOldColor = m_cFontInfo.m_clrOutline;
	m_cFontInfo.m_clrOutline = clrColor;
	SetRedraw(true);
	return bOldColor;
}

void CText::SetFont(const std::string &strFontName, const int &nFontSize, const bool &bBold)
{
	if(m_cFontInfo.GetName() == strFontName && m_cFontInfo.GetSize() == nFontSize && m_cFontInfo.IsBold() == bBold) return;
	m_cFontInfo.SetFont(strFontName, nFontSize, bBold);
	SetRedraw(true);
}

void CText::SetSpaceBetweenLetters(const int &nSpaceBetweenLetters)
{
	if(m_cFontInfo.m_nSpaceBetweenLetters == nSpaceBetweenLetters) return;
	m_cFontInfo.m_nSpaceBetweenLetters = nSpaceBetweenLetters;
	SetRedraw(true);
}

CObjectBase *CText::NewObject(void)
{
	return new CText;
}

CObjectBase *CText::Copy(CObjectBase *pObject)
{
	__super::Copy(pObject);

	CText *pText			= (CText *)pObject;
	pText->m_bDot			= m_bDot;

	pText->m_pBack->Create((int)m_rtLocalRect.GetWidth(), (int)m_rtLocalRect.GetHeight());
	pText->m_cFontInfo		= m_cFontInfo;

	pText->m_strText		= m_strText;
	pText->m_nLineSpace		= m_nLineSpace;
	pText->m_bSlide			= m_bSlide;
	pText->m_dwSlideType	= m_dwSlideType;
	pText->m_nSlideRepeatCount	= m_nSlideRepeatCount;
	pText->m_fSlideGap		= m_fSlideGap;
	pText->SetRedraw(true);

	return pObject;
}

size_t CText::GetInheritanceClipSize(int &nVersion)
{
	size_t nRet = __super::GetInheritanceClipSize(nVersion);
	nRet += sizeof(bool);					//m_bDot
	nRet += m_cFontInfo.GetInheritanceClipSize(nVersion); //m_cFontInfo
	nRet += sizeof(size_t);					//텍스트의 크기
	nRet += m_strText.size();				//텍스트
	nRet += sizeof(int);					//m_nLineSpace
	nRet += sizeof(bool);					//m_bSlide
	nRet += sizeof(DWORD);					//m_dwSlideType
	nRet += sizeof(int);					//m_nSlideRepeatCount
	nRet += sizeof(float);					//m_fSlideGap
	return nRet;
}

size_t CText::SaveInheritanceClipSize(int &nVersion, BYTE *pDest)
{
	size_t nRet = __super::SaveInheritanceClipSize(nVersion, pDest);
	memcpy(pDest+nRet, &m_bDot, sizeof(bool));
	nRet += sizeof(bool);					//m_bDot
	nRet += m_cFontInfo.SaveInheritanceClipSize(nVersion, pDest+nRet); //m_cFontInfo
	size_t nSize = m_strText.size();
	memcpy(pDest+nRet, &nSize, sizeof(size_t));
	nRet += sizeof(size_t);					//텍스트의 크기
	if(nSize)
	{
		memcpy(pDest+nRet, m_strText.c_str(), m_strText.size());
		nRet += m_strText.size();				//텍스트
	}
	memcpy(pDest+nRet, &m_nLineSpace, sizeof(int));
	nRet += sizeof(int);					//m_nLineSpace
	memcpy(pDest+nRet, &m_bSlide, sizeof(bool));
	nRet += sizeof(bool);					//m_bSlide
	memcpy(pDest+nRet, &m_dwSlideType, sizeof(DWORD));
	nRet += sizeof(DWORD);					//m_dwSlideType
	memcpy(pDest+nRet, &m_nSlideRepeatCount, sizeof(int));
	nRet += sizeof(int);					//m_nSlideRepeatCount
	memcpy(pDest+nRet, &m_fSlideGap, sizeof(float));
	nRet += sizeof(float);					//m_fSlideGap
	return nRet;
}

size_t CText::LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID)
{
	size_t nRet = __super::LoadInheritanceClipSize(nVersion, pDest, pProject, bSetID);
	memcpy(&m_bDot, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);					//m_bDot
	nRet += m_cFontInfo.LoadInheritanceClipSize(nVersion, pDest+nRet, pProject, bSetID); //m_cFontInfo
	size_t nSize = 0;
	memcpy(&nSize, pDest+nRet, sizeof(size_t));
	nRet += sizeof(size_t);					//텍스트의 크기
	if(nSize)
	{
		GetPointerToString(pDest+nRet, nSize, m_strText);
		nRet += nSize;				//텍스트
	}
	memcpy(&m_nLineSpace, pDest+nRet, sizeof(int));
	nRet += sizeof(int);					//m_nLineSpace

	bool bUse = false;
	memcpy(&bUse, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);					//m_bSlide
	memcpy(&m_dwSlideType, pDest+nRet, sizeof(DWORD));
	nRet += sizeof(DWORD);					//m_dwSlideType
	memcpy(&m_nSlideRepeatCount, pDest+nRet, sizeof(int));
	nRet += sizeof(int);					//m_nSlideRepeatCount
	memcpy(&m_fSlideGap, pDest+nRet, sizeof(float));
	nRet += sizeof(float);					//m_fSlideGap

	Create((int)m_rtLocalRect.GetWidth(), (int)m_rtLocalRect.GetHeight());
	if(bUse) SetSlide(bUse);
	else SetRedraw();

	if(GetID() != _ID_OBJECT_NOTUSE && IsTool()) SetMessage(true);
	return nRet;
}

bool CText::SaveInheritanceXml(TiXmlNode *pXmlObject)
{
	__super::SaveInheritanceXml(pXmlObject);

	CMyMime64 myMime;
	myMime.SetString(m_strText,FALSE,TRUE);

	TiXmlElement xmlText("text");
	xmlText.SetAttribute("dot", m_bDot?1:0);
	xmlText.SetAttribute("linespace", m_nLineSpace);
	xmlText.SetAttribute("text", myMime.GetString());
	pXmlObject->InsertEndChild(xmlText);

	CString strTemp;
	TiXmlElement xmlSlide("slide");
	xmlSlide.SetAttribute("use", m_bSlide?1:0);
	strTemp.Format("%08X", m_dwSlideType);
	xmlSlide.SetAttribute("type", strTemp.GetString());
	xmlSlide.SetAttribute("repeat", m_nSlideRepeatCount);
	xmlSlide.SetDoubleAttribute("move", m_fSlideGap);
	pXmlObject->InsertEndChild(xmlSlide);

	m_cFontInfo.SaveInheritanceXml(pXmlObject);
	return true;
}

bool CText::LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject)
{
	__super::LoadInheritanceXml(nVersion, pXmlObject, pProject);

	TiXmlElement *pXmlInheritance = pXmlObject->FirstChildElement("text");
	GetAttribute(pXmlInheritance, "dot", m_bDot);
	GetAttribute(pXmlInheritance, "linespace", m_nLineSpace);
	GetAttributeMime64(pXmlInheritance, "text", m_strText);

	bool bUse = false;

	TiXmlElement *pXmlSlide = pXmlObject->FirstChildElement("slide");
	GetAttribute(pXmlSlide, "use", bUse);
	GetAttribute(pXmlSlide, "type", m_dwSlideType);
	GetAttribute(pXmlSlide, "repeat", m_nSlideRepeatCount);
	GetAttribute(pXmlSlide, "move", m_fSlideGap);

	TiXmlElement *pXmlFont = pXmlObject->FirstChildElement("font");
	m_cFontInfo.LoadInheritanceXml(nVersion, pXmlFont, pProject);

	Create((int)m_rtLocalRect.GetWidth(), (int)m_rtLocalRect.GetHeight());

	if(bUse) SetSlide(bUse);
	else SetRedraw();

	if(GetID() != _ID_OBJECT_NOTUSE && IsTool()) SetMessage(true);
	return true;
}

void CText::AddSlideString_Back(const std::string &strSlide)
{
	if(!m_bSlide) return;

	SLIDE_ITEM temp;
	temp.m_strOutput	= strSlide;
	temp.m_nRepeat		= m_nSlideRepeatCount;
	temp.m_uFormat		= m_cFontInfo.m_uFormat;
	temp.m_nDrawMode	= m_cFontInfo.m_nDrawType;
	temp.m_nCurStatus	= SLIDE_ITEM::SS_START;
	temp.m_dwSlideType	= m_dwSlideType;
	temp.m_dwDelayTime	= m_dwSlideDelayTime;
	m_listSlide.push_back(temp);
}

void CText::AddSlideString_Front(const std::string &strSlide)
{
	if(!m_bSlide) return;

	SLIDE_ITEM temp;
	temp.m_strOutput	= strSlide;
	temp.m_nRepeat		= m_nSlideRepeatCount;
	temp.m_uFormat		= m_cFontInfo.m_uFormat;
	temp.m_nDrawMode	= m_cFontInfo.m_nDrawType;
	temp.m_nCurStatus	= SLIDE_ITEM::SS_START;
	temp.m_dwSlideType	= m_dwSlideType;
	temp.m_dwDelayTime	= m_dwSlideDelayTime;
	m_listSlide.push_front(temp);
}

void CText::ClearSlideString(const bool &bForce)
{
	if(m_bSlide == false) return;
	m_listSlide.clear();

	std::list<SLIDE_ITEM>::iterator start = m_listSlideShow.begin();
	std::list<SLIDE_ITEM>::iterator end = m_listSlideShow.end();

	if(bForce)
	{
		m_listSlideShow.clear();
	}
	else
	{
		for(;start != end;++start)
		{
			start->m_nRepeat = 1;
		}
	}
}

void CText::SetSlide(const bool &bSlide)
{
	if(m_bSlide == bSlide) return;
	if(m_bSlide) ClearSlideString(true);

	m_bSlide = bSlide;
	SetUseLogic(m_bSlide);

	if(m_bSlide)
	{
		if(m_strText != "") AddSlideString_Back(m_strText);
	}
	else
	{
		SetRedraw();
	}
}

void CText::Logic(const DWORD &dwTime)
{
	std::list<SLIDE_ITEM>::iterator start = m_listSlideShow.begin();
	std::list<SLIDE_ITEM>::iterator end = m_listSlideShow.end();
	int i=0;

	m_pBack->ClearColor(RGB(0,0,255));

	CDC *pDC = m_pBack->GetDC();
	pDC->SelectObject((CFont *)m_cFontInfo.GetFont());
	pDC->SetTextCharacterExtra(m_cFontInfo.m_nSpaceBetweenLetters);

	if(start == end)
	{
		std::list<SLIDE_ITEM>::iterator first = m_listSlide.begin();
		if(first == m_listSlide.end())
		{
			m_pBack->ReleaseDC();
			return;
		}
		else
		{
			m_listSlideShow.push_back(*first);
			start = m_listSlideShow.begin();

			start->m_szSize = GetTextSize(pDC, start->m_strOutput.c_str(), start->m_strOutput.length());
			start->m_dwStartTime = m_pProject->GetTickCount();
			start->m_nCurStatus	= SLIDE_ITEM::SS_START;

			m_listSlide.erase(first);
		}
	}

	DWORD dwCurTime = m_pProject->GetTickCount();
	int nWidth = (int)m_rtLocalRect.GetWidth();
	int nHeight = (int)m_rtLocalRect.GetHeight();

	CPoint ptPos;
	CPoint ptEnd, ptStart, ptCur;

	DWORD dwTempTime = 0;
	DWORD dwTempTime2 = 0;
	bool bEnd = false;

	while(start != end)
	{
		bEnd = false;

		dwTempTime2 = dwCurTime - start->m_dwStartTime;
		switch(start->m_nCurStatus)
		{
		case SLIDE_ITEM::SS_START:
			{
				dwTempTime = _RedrawSlideStart(*start, dwCurTime, ptStart, ptEnd, ptCur, nWidth, nHeight, start->m_szSize);
				if(dwTempTime != dwTempTime2)
				{
					dwTempTime2 -= dwTempTime;
					if(dwTempTime2 > start->m_dwDelayTime)
					{
						start->m_nCurStatus = SLIDE_ITEM::SS_END;
						start->m_dwStartTime += (dwTempTime + start->m_dwDelayTime);

						bEnd = _RedrawSlideEnd(*start, dwCurTime, ptStart, ptEnd, ptCur, nWidth, nHeight, start->m_szSize);
					}
					else
					{
						start->m_nCurStatus = SLIDE_ITEM::SS_WAIT;
						start->m_dwStartTime += dwTempTime;
					}
				}
			}
			break;
		case SLIDE_ITEM::SS_WAIT:
			{
				if(dwTempTime2 > start->m_dwDelayTime)
				{
					start->m_nCurStatus = SLIDE_ITEM::SS_END;
					start->m_dwStartTime += start->m_dwDelayTime;

					bEnd = _RedrawSlideEnd(*start, dwCurTime, ptStart, ptEnd, ptCur, nWidth, nHeight, start->m_szSize);
				}
				else
				{
					_RedrawSlideWait(*start, dwCurTime, ptStart, ptEnd, ptCur, nWidth, nHeight, start->m_szSize);
				}
			}
			break;
		case SLIDE_ITEM::SS_END:
			bEnd = _RedrawSlideEnd(*start, dwCurTime, ptStart, ptEnd, ptCur, nWidth, nHeight, start->m_szSize);
			break;
		}

		if(bEnd)
		{
			if(start->m_nRepeat == -1)
			{
				m_listSlide.push_back(*start);
			}
			else
			{
				--start->m_nRepeat;
				if(start->m_nRepeat != 0)
				{
					m_listSlide.push_back(*start);
				}
			}

			start = m_listSlideShow.erase(start);
		}
		else
		{
			switch(m_cFontInfo.m_nDrawType)
			{
			case ETDT_NORMAL:
				pDC->SetTextColor(m_cFontInfo.m_clrColor);
				pDC->TextOut((int)ptCur.x, (int)ptCur.y, start->m_strOutput.c_str(), start->m_strOutput.length());
				break;
			case ETDT_SHADOW:
				pDC->SetTextColor(m_cFontInfo.m_clrOutline);
				pDC->TextOut((int)(ptCur.x+1.0f), (int)(ptCur.y+1.0f), start->m_strOutput.c_str(), start->m_strOutput.length());
				pDC->SetTextColor(m_cFontInfo.m_clrColor);
				pDC->TextOut((int)ptCur.x, (int)ptCur.y, start->m_strOutput.c_str(), start->m_strOutput.length());
				break;
			case ETDT_OUTLINE:
				pDC->SetTextColor(m_cFontInfo.m_clrOutline);
				pDC->TextOut((int)(ptCur.x+0.0f), (int)(ptCur.y+1.0f), start->m_strOutput.c_str(), start->m_strOutput.length());
				pDC->TextOut((int)(ptCur.x+2.0f), (int)(ptCur.y+1.0f), start->m_strOutput.c_str(), start->m_strOutput.length());
				pDC->TextOut((int)(ptCur.x+1.0f), (int)(ptCur.y+0.0f), start->m_strOutput.c_str(), start->m_strOutput.length());
				pDC->TextOut((int)(ptCur.x+1.0f), (int)(ptCur.y+2.0f), start->m_strOutput.c_str(), start->m_strOutput.length());
				pDC->SetTextColor(m_cFontInfo.m_clrColor);
				pDC->TextOut((int)(ptCur.x+1.0f), (int)(ptCur.y+1.0f), start->m_strOutput.c_str(), start->m_strOutput.length());
				break;
			}

			++start;
		}
	}

	m_pBack->ReleaseDC();
}

DWORD CText::_RedrawSlideStart(SLIDE_ITEM &item, DWORD &dwDeltaTime, CPoint &ptStart, CPoint &ptEnd, CPoint &ptCur, int &nWidth, int &nHeight, CSize &szSize)
{
	CPoint ptPos;

	DWORD dwRet = dwDeltaTime - item.m_dwStartTime;

	if(item.m_dwSlideType & TEXT_STS_LEFT)
	{
		if(item.m_uFormat & DT_BOTTOM) ptPos.y = (float)(nHeight - szSize.cy);
		else if(item.m_uFormat & DT_VCENTER)	ptPos.y = (float)((nHeight - szSize.cy)/2);
		else ptPos.y = 0.0f;

		ptCur.y = ptPos.y;

		ptStart.x = (float)(-szSize.cx);

		if(item.m_uFormat & DT_RIGHT)	ptEnd.x = (float)(nWidth - szSize.cx);
		else if(item.m_uFormat & DT_CENTER) ptEnd.x = (float)((nWidth - szSize.cx)/2);
		else ptEnd.x = 0.0f;

		ptCur.x = ptStart.x + float(dwDeltaTime - item.m_dwStartTime)/1000.0f * m_fSlideGap;
		if(ptCur.x > ptEnd.x)
		{
			dwRet = (DWORD)((ptEnd.x - ptStart.x)*1000.0f/m_fSlideGap);
			ptCur.x = ptEnd.x;		
		}
	}
	else if(item.m_dwSlideType & TEXT_STS_BOTTOM)
	{
		if(item.m_uFormat & DT_RIGHT) ptPos.x = (float)nWidth - szSize.cx;
		else if(item.m_uFormat & DT_CENTER)	ptPos.x = (float)((nWidth - szSize.cx)/2);
		else ptPos.x = 0.0f;

		ptCur.x = ptPos.x;
		ptStart.y = (float)nHeight;

		if(item.m_uFormat & DT_BOTTOM)	ptEnd.y = (float)(nHeight - szSize.cy);
		else if(item.m_uFormat & DT_VCENTER) ptEnd.y = (float)((nHeight - szSize.cy)/2);
		else ptEnd.y = 0.0f;

		ptCur.y = ptStart.y - float(dwDeltaTime - item.m_dwStartTime)/1000.0f * m_fSlideGap;

		if(ptCur.y < ptEnd.y)
		{
			dwRet = (DWORD)((ptStart.y - ptEnd.y)*1000.0f/m_fSlideGap);
			ptCur.y = ptEnd.y;		
		}
	}
	else if(item.m_dwSlideType & TEXT_STS_TOP)
	{
		if(item.m_uFormat & DT_RIGHT) ptPos.x = (float)(nWidth - szSize.cx);
		else if(item.m_uFormat & DT_CENTER)	ptPos.x = (float)((nWidth - szSize.cx)/2);
		else ptPos.x = 0.0f;

		ptCur.x = ptPos.x;
		ptStart.y = (float)(-szSize.cy);

		if(item.m_uFormat & DT_BOTTOM)	ptEnd.y = (float)(nHeight - szSize.cy);
		else if(item.m_uFormat & DT_VCENTER) ptEnd.y = (float)((nHeight - szSize.cy)/2);
		else ptEnd.y = 0;

		ptCur.y = ptStart.y + float(dwDeltaTime - item.m_dwStartTime)/1000.0f * m_fSlideGap;
		if(ptCur.y > ptEnd.y)
		{
			dwRet = (DWORD)((ptEnd.y - ptStart.y)*1000.0f/m_fSlideGap);
			ptCur.y = ptEnd.y;		
		}
	}
	else //TEXT_STS_RIGHT
	{
		if(item.m_uFormat & DT_BOTTOM) ptPos.y = (float)(nHeight - szSize.cy);
		else if(item.m_uFormat & DT_VCENTER) ptPos.y = (float)((nHeight - szSize.cy)/2);
		else ptPos.y = 0;

		ptCur.y = ptPos.y;
		ptStart.x = (float)nWidth;

		if(item.m_uFormat & DT_RIGHT)	ptEnd.x = (float)(nWidth - szSize.cx);
		else if(item.m_uFormat & DT_CENTER) ptEnd.x = (float)((nWidth - szSize.cx)/2);
		else ptEnd.x = 0;

		ptCur.x = ptStart.x - float(dwDeltaTime - item.m_dwStartTime)/1000.0f * m_fSlideGap;

		if(ptCur.x < ptEnd.x)
		{
			dwRet = (DWORD)((ptStart.x - ptEnd.x)*1000.0f/m_fSlideGap);
			ptCur.x = ptEnd.x;		
		}
	}

	return dwRet;
}

bool CText::_RedrawSlideEnd(SLIDE_ITEM &item, DWORD &dwDeltaTime, CPoint &ptStart, CPoint &ptEnd, CPoint &ptCur, int &nWidth, int &nHeight, CSize &szSize)
{
	CPoint ptPos;
	bool bEnd = false;

	if(item.m_dwSlideType & TEXT_STE_RIGHT)
	{
		if(item.m_uFormat & DT_BOTTOM) ptPos.y = (float)(nHeight - szSize.cy);
		else if(item.m_uFormat & DT_VCENTER) ptPos.y = (float)((nHeight - szSize.cy)/2);
		else ptPos.y = 0;

		ptCur.y = ptPos.y;
		ptEnd.x = (float)nWidth;

		if(item.m_uFormat & DT_RIGHT) ptStart.x = (float)(nWidth - szSize.cx);
		else if(item.m_uFormat & DT_CENTER) ptStart.x = (float)((nWidth - szSize.cx)/2);
		else ptStart.x = 0.0f;

		ptCur.x = ptStart.x + float(dwDeltaTime - item.m_dwStartTime)/1000.0f * m_fSlideGap;
		if(ptCur.x > ptEnd.x) bEnd = true;
	}
	else if(item.m_dwSlideType & TEXT_STE_BOTTOM)
	{
		if(item.m_uFormat & DT_RIGHT) ptPos.x = (float)(nWidth - szSize.cx);
		else if(item.m_uFormat & DT_CENTER)	ptPos.x = (float)((nWidth - szSize.cx)/2);
		else ptPos.x = 0;

		ptCur.x = ptPos.x;
		ptEnd.y = (float)nHeight;

		if(item.m_uFormat & DT_BOTTOM)	ptStart.y = (float)(nHeight - szSize.cy);
		else if(item.m_uFormat & DT_VCENTER) ptStart.y = (float)((nHeight - szSize.cy)/2);
		else ptStart.y = 0.0f;

		ptCur.y = ptStart.y + float(dwDeltaTime - item.m_dwStartTime)/1000.0f * m_fSlideGap;
		if(ptCur.y > ptEnd.y) bEnd = true;
	}
	else if(item.m_dwSlideType & TEXT_STE_TOP)
	{
		if(item.m_uFormat & DT_RIGHT) ptPos.x = (float)(nWidth - szSize.cx);
		else if(item.m_uFormat & DT_CENTER)	ptPos.x = (float)((nWidth - szSize.cx)/2);
		else ptPos.x = 0;

		ptCur.x = ptPos.x;
		ptEnd.y = (float)(-szSize.cy);

		if(item.m_uFormat & DT_BOTTOM)	ptStart.y = (float)(nHeight - szSize.cy);
		else if(item.m_uFormat & DT_VCENTER) ptStart.y = (float)((nHeight - szSize.cy)/2);
		else ptStart.y = 0.0f;

		ptCur.y = ptStart.y - float(dwDeltaTime - item.m_dwStartTime)/1000.0f * m_fSlideGap;
		if(ptCur.y < ptEnd.y) bEnd = true;
	}
	else //TEXT_STE_LEFT
	{
		if(item.m_uFormat & DT_BOTTOM) ptPos.y = (float)(nHeight - szSize.cy);
		else if(item.m_uFormat & DT_VCENTER) ptPos.y = (float)((nHeight - szSize.cy)/2);
		else ptPos.y = 0.0f;

		ptCur.y = ptPos.y;
		ptEnd.x = (float)(-szSize.cx);

		if(item.m_uFormat & DT_RIGHT)	ptStart.x = (float)(nWidth - szSize.cx);
		else if(item.m_uFormat & DT_CENTER) ptStart.x = (float)((nWidth - szSize.cx)/2);
		else ptStart.x = 0.0f;

		ptCur.x = ptStart.x - float(dwDeltaTime - item.m_dwStartTime)/1000.0f * m_fSlideGap;
		if(ptCur.x < ptEnd.x) bEnd = true;
	}

	return bEnd;
}

void CText::_RedrawSlideWait(SLIDE_ITEM &item, DWORD &dwDeltaTime, CPoint &ptStart, CPoint &ptEnd, CPoint &ptCur, int &nWidth, int &nHeight, CSize &szSize)
{
	if(item.m_uFormat & DT_BOTTOM) ptCur.y = (float)(nHeight - szSize.cy);
	else if(item.m_uFormat & DT_VCENTER) ptCur.y = (float)((nHeight - szSize.cy)/2);
	else ptCur.y = 0.0f;

	if(item.m_uFormat & DT_RIGHT) ptCur.x = (float)(nWidth - szSize.cx);
	else if(item.m_uFormat & DT_CENTER) ptCur.x = (float)((nWidth - szSize.cx)/2);
	else ptCur.x = 0.0f;
}
} //namespace LSY