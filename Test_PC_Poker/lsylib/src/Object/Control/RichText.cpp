#include "stdafx.h"
#include "RichText.h"
#include "ScrollBar.h"
#include "../../Functor/Message.h"
#include <algorithm>
#include "../Base/DumyImage.h"
#include "../ObjectMan.h"
#include "../../tinyxml/tinyxml.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace LSY
{
CRichItem::CRichItem()
:	m_strOutput(""),
	m_szSize(0,0),
	m_pFontInfo(NULL),
	m_lParam(NULL),
	m_pSprite(NULL),
	m_nIndex(-1)
{
}

CRichItem::~CRichItem()
{
	if(m_pSprite)
	{
		RM().RemoveSprite(m_pSprite);
		m_pSprite = NULL;

		m_nIndex = -1;
	}
}

CRichItem *CRichItem::Clone(CRichText *pSrcRichText, CRichText *pDestRichText)
{
	CRichItem *pItem = new CRichItem;
	pItem->m_pFontInfo  = (FONTINFO *)pDestRichText->GetFontInfo(pSrcRichText->GetFontInfoIndex(m_pFontInfo));
	pItem->m_strOutput  = m_strOutput;
	pItem->m_szSize		= m_szSize;
	pItem->m_lParam		= m_lParam;
	if(m_pSprite) pItem->m_pSprite = RM().Clone(m_pSprite);
	pItem->m_nIndex		= m_nIndex;
	return pItem;
}

CRichLine::CRichLine()
:	m_szSize(0,0),
	m_bEnd(false),
	m_uFormat(DT_LEFT|DT_TOP|DT_SINGLELINE),
	m_fStartY(0.0f)
{
}

CRichLine::~CRichLine()
{
	Clear();
}

void CRichLine::Clear(void)
{
	RICHITEMLISTITER start = m_listItems.begin();
	RICHITEMLISTITER end = m_listItems.end();

	for(;start != end;++start)
	{
		delete *start;
	}

	m_listItems.clear();
	m_bEnd = false;
	m_szSize = CSize(0,0);
}

CRichLine *CRichLine::Clone(CRichText *pSrcRichText, CRichText *pDestRichText)
{
	CRichLine *pLine = new CRichLine;
	pLine->m_bEnd		= m_bEnd;
	pLine->m_fStartY	= m_fStartY;
	pLine->m_szSize		= m_szSize;
	pLine->m_uFormat	= m_uFormat;

	RICHITEMLISTITER start = m_listItems.begin();
	RICHITEMLISTITER end = m_listItems.end();

	for(;start != end;++start)
	{
		pLine->m_listItems.push_back((*start)->Clone(pSrcRichText, pDestRichText));
	}
	return pLine;
}

const int CRichLine::GetLineHeight(const int &nMinimumHeight)
{
	m_szSize.cy = nMinimumHeight;

	RICHITEMLISTITER start = m_listItems.begin();
	RICHITEMLISTITER end = m_listItems.end();

	for(;start != end;++start)
	{
		if(m_szSize.cy < (*start)->m_szSize.cy)
			m_szSize.cy = (*start)->m_szSize.cy;
	}

	return (int)m_szSize.cy;
}

CRichText::CRichText(void)
:	m_bRedraw(false),
	m_pFontInfo(NULL),
	m_nFontIndex(-1),
	m_pLastLine(NULL),
	m_pLastItem(NULL),
	m_fStartY(0.0f),
	m_nLineSpace(0),
	m_pScrollBar(NULL),
	m_fTotalHeight(0.0f),
	m_nLimitLine(0),
	m_bLineScroll(true),
	m_pEmoticonSpr(NULL),
	m_nMinimumLineHeight(-1)
{
	FONTINFO font;
	m_listFonts.push_back(font);

	m_itFirstVisibleLine = m_listLines.end();

	SetCurrentFont(0);

	m_pBack = new CDumyImage;
	AddLocalChild(m_pBack);

	SetUseMouseWheel(true);
}

CRichText::~CRichText()
{
	m_listFonts.clear();
	Clear();

	if(m_pEmoticonSpr)
	{
		RM().RemoveSprite(m_pEmoticonSpr);
		m_pEmoticonSpr = NULL;
	}
}

bool CRichText::CreateEmoticon(std::string strPath)
{
	if(m_pEmoticonSpr)
	{
		if(m_pEmoticonSpr->GetFilePath() == strPath) return true;
		else
		{
			RM().RemoveSprite(m_pEmoticonSpr);
			m_pEmoticonSpr = NULL;
		}
	}

	m_pEmoticonSpr = RM().GetSprite(strPath.c_str());
	if(m_pEmoticonSpr == NULL) return false;

	return true;
}

bool CRichText::SetEmoticonText(std::string strText, int nIndex)
{
	if(nIndex < 0) return false;
	m_mapEmoticon[strText] = nIndex;
	return true;
}

void CRichText::SetRedraw(void)
{
	if(m_bRedraw) return;
	m_bRedraw = true;
	m_pBack->CheckEventObjectDraw();
}

bool CRichText::Create(int nWidth, int nHeight)
{
	if(!m_pBack->Create(nWidth, nHeight)) return false;

	SetLocalRect(m_pBack->GetLocalRect());
	if(IsTool()) MakeSample();
	SetRedraw();

	return true;
}

CObjectBase *CRichText::NewObject(void)
{
	return new CRichText;
}

CObjectBase *CRichText::Copy(CObjectBase *pObject)
{
	CObjectBase::Copy(pObject);

	CRichText *pRichText = (CRichText *)pObject;
	pRichText->m_listFonts.clear();

	FONTINFOLISTITER start_font = m_listFonts.begin();
	FONTINFOLISTITER end_font = m_listFonts.end();
	for(;start_font != end_font;++start_font)
	{
		pRichText->m_listFonts.push_back(*start_font);
	}

	RICHLINELISTITER start_line = m_listLines.begin();
	RICHLINELISTITER end_line = m_listLines.end();
	for(;start_line != end_line;++start_line)
	{
		pRichText->m_listLines.push_back((*start_line)->Clone(this, pRichText));
	}

	pRichText->m_pBack->Create((int)m_rtLocalRect.GetWidth(), (int)m_rtLocalRect.GetHeight());

	pRichText->m_nLineSpace = m_nLineSpace;
	pRichText->m_nFontIndex = m_nFontIndex;
	pRichText->m_pFontInfo = (FONTINFO *)pRichText->GetFontInfo(m_nFontIndex);

	if(m_pLastLine) pRichText->m_pLastLine = *(pRichText->m_listLines.rbegin());
	if(m_pLastItem) pRichText->m_pLastItem = *(pRichText->m_pLastLine->m_listItems.rbegin());

	if(m_pScrollBar) pRichText->AttachScroll((CScrollBar *)m_pScrollBar->Clone(true));
	pRichText->m_fTotalHeight	= m_fTotalHeight;
	pRichText->m_fStartY		= m_fStartY;
	pRichText->m_bRedraw		= true;
	pRichText->m_nLimitLine		= m_nLimitLine;
	pRichText->m_bLineScroll	= m_bLineScroll;

	if(m_pEmoticonSpr) pRichText->m_pEmoticonSpr = RM().Clone(m_pEmoticonSpr);
	pRichText->m_mapEmoticon	= m_mapEmoticon;
	pRichText->m_nMinimumLineHeight = m_nMinimumLineHeight;

	return pObject;
}

size_t CRichText::GetInheritanceClipSize(int &nVersion)
{
	size_t nRet = __super::GetInheritanceClipSize(nVersion);
	nRet += sizeof(size_t);	//폰크 갯수
	
	FONTINFOLISTITER start_font = m_listFonts.begin();
	FONTINFOLISTITER end_font = m_listFonts.end();
	for(;start_font != end_font;++start_font)
	{
		nRet += start_font->GetInheritanceClipSize(nVersion);	//이모티콘명 크기
	}

	nRet += sizeof(int);		//m_nLineSpace
	nRet += sizeof(int);		//m_nFontIndex
	nRet += sizeof(bool);		//스크롤바 유무
	if(m_pScrollBar) nRet += m_pScrollBar->GetClipSize(nVersion);
	nRet += sizeof(int);		//m_nLimitLine
	nRet += sizeof(bool);		//m_bLineScroll
	nRet += sizeof(size_t);		//이모티콘이미지 크기
	if(m_pEmoticonSpr) nRet += m_pEmoticonSpr->GetFilePath().size();
	nRet += sizeof(size_t);		//이모티콘 갯수

	EMOTICONMAPITER start = m_mapEmoticon.begin();
	EMOTICONMAPITER end = m_mapEmoticon.end();

	for(;start != end;++start)
	{
		nRet += sizeof(size_t);	//이모티콘명 크기
		nRet += start->first.size(); //이모티콘명
		nRet += sizeof(int);	//인덱스
	}

	nRet += sizeof(int);		//m_nMinimumLineHeight
	return nRet;
}

size_t CRichText::SaveInheritanceClipSize(int &nVersion, BYTE *pDest)
{
	size_t nRet = __super::SaveInheritanceClipSize(nVersion, pDest);

	size_t nSize = m_listFonts.size();
	memcpy(pDest+nRet, &nSize, sizeof(size_t));
	nRet += sizeof(size_t);	//폰크 갯수

	FONTINFOLISTITER start_font = m_listFonts.begin();
	FONTINFOLISTITER end_font = m_listFonts.end();
	for(;start_font != end_font;++start_font)
	{
		nRet += start_font->SaveInheritanceClipSize(nVersion, pDest+nRet);	//이모티콘명 크기
	}

	memcpy(pDest+nRet, &m_nLineSpace, sizeof(int));
	nRet += sizeof(int);		//m_nLineSpace
	memcpy(pDest+nRet, &m_nFontIndex, sizeof(int));
	nRet += sizeof(int);		//m_nFontIndex

	bool bUse = m_pScrollBar?true:false;
	memcpy(pDest+nRet, &bUse, sizeof(bool));
	nRet += sizeof(bool);		//스크롤바 유무

	if(m_pScrollBar)
	{
		nRet += m_pScrollBar->SaveClipSize(nVersion, pDest+nRet);
	}

	memcpy(pDest+nRet, &m_nLimitLine, sizeof(int));
	nRet += sizeof(int);		//m_nLimitLine
	memcpy(pDest+nRet, &m_bLineScroll, sizeof(bool));
	nRet += sizeof(bool);		//m_bLineScroll

	nSize = 0;
	if(m_pEmoticonSpr) nSize = m_pEmoticonSpr->GetFilePath().size();
	memcpy(pDest+nRet, &nSize, sizeof(size_t));
	nRet += sizeof(size_t);		//이모티콘이미지 크기

	if(m_pEmoticonSpr)
	{
		memcpy(pDest+nRet, m_pEmoticonSpr->GetFilePath().c_str(), nSize);
		nRet += nSize;
	}

	nSize = m_mapEmoticon.size();
	memcpy(pDest+nRet, &nSize, sizeof(size_t));
	nRet += sizeof(size_t);		//이모티콘 갯수

	EMOTICONMAPITER start = m_mapEmoticon.begin();
	EMOTICONMAPITER end = m_mapEmoticon.end();

	for(;start != end;++start)
	{
		nSize = start->first.size();
		memcpy(pDest+nRet, &nSize, sizeof(size_t));
		nRet += sizeof(size_t);	//이모티콘명 크기
		if(nSize)
		{
			memcpy(pDest+nRet, start->first.c_str(), nSize);
			nRet += nSize; //이모티콘명
		}
		memcpy(pDest+nRet, &start->second, sizeof(int));
		nRet += sizeof(int);	//인덱스
	}

	memcpy(pDest+nRet, &m_nMinimumLineHeight, sizeof(int));
	nRet += sizeof(int);		//m_nMinimumLineHeight
	return nRet;
}

size_t CRichText::LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID)
{
	size_t nRet = __super::LoadInheritanceClipSize(nVersion, pDest, pProject, bSetID);

	size_t nSize = 0;
	memcpy(&nSize, pDest+nRet, sizeof(size_t));
	nRet += sizeof(size_t);	//폰크 갯수

	FONTINFO fontInfo;
	m_listFonts.clear();

	int i=0;
	for(i=0;i<(int)nSize;++i)
	{
		nRet += fontInfo.LoadInheritanceClipSize(nVersion, pDest+nRet, pProject, bSetID);	//이모티콘명 크기
		m_listFonts.push_back(fontInfo);
	}

	memcpy(&m_nLineSpace, pDest+nRet, sizeof(int));
	nRet += sizeof(int);		//m_nLineSpace
	memcpy(&m_nFontIndex, pDest+nRet, sizeof(int));
	nRet += sizeof(int);		//m_nFontIndex

	m_pFontInfo = (FONTINFO *)GetFontInfo(m_nFontIndex);

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

	memcpy(&m_nLimitLine, pDest+nRet, sizeof(int));
	nRet += sizeof(int);		//m_nLimitLine
	memcpy(&m_bLineScroll, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);		//m_bLineScroll

	memcpy(&nSize, pDest+nRet, sizeof(size_t));
	nRet += sizeof(size_t);		//이모티콘이미지 크기

	std::string strEmoticon;

	if(nSize)
	{
		GetPointerToString(pDest+nRet, nSize, strEmoticon);
		nRet += nSize;

		CreateEmoticon(strEmoticon);
	}

	memcpy(&nSize, pDest+nRet, sizeof(size_t));
	nRet += sizeof(size_t);		//이모티콘 갯수

	size_t nSize2 = 0;
	int nIndex = 0;
	m_mapEmoticon.clear();

	for(i=0;i<(int)nSize;++i)
	{
		memcpy(&nSize2, pDest+nRet, sizeof(size_t));
		nRet += sizeof(size_t);	//이모티콘명 크기
		if(nSize)
		{
			GetPointerToString(pDest+nRet, nSize, strEmoticon);
			nRet += nSize; //이모티콘명
		}
		memcpy(&nIndex, pDest+nRet, sizeof(int));
		nRet += sizeof(int);	//인덱스

		m_mapEmoticon[strEmoticon] = nIndex;
	}

	memcpy(&m_nMinimumLineHeight, pDest+nRet, sizeof(int));
	nRet += sizeof(int);		//m_nMinimumLineHeight

	Create((int)m_rtLocalRect.GetWidth(), (int)m_rtLocalRect.GetHeight());

	if(IsTool())
	{
		MakeSample();
		SetMessage(true);
	}

	return nRet;
}

bool CRichText::SaveInheritanceXml(TiXmlNode *pXmlObject)
{
	__super::SaveInheritanceXml(pXmlObject);

	TiXmlElement xmlInheritance("Inheritance");
	xmlInheritance.SetAttribute("linespace",  m_nLineSpace);
	xmlInheritance.SetAttribute("index",  m_nFontIndex);
	xmlInheritance.SetAttribute("limit",  m_nLimitLine);
	xmlInheritance.SetAttribute("linescroll",  m_bLineScroll?1:0);
	xmlInheritance.SetAttribute("minimum",  m_nMinimumLineHeight);
	pXmlObject->InsertEndChild(xmlInheritance);

	TiXmlElement xmlFontList("fontlist");
	FONTINFOLISTITER start_font = m_listFonts.begin();
	FONTINFOLISTITER end_font = m_listFonts.end();
	for(;start_font != end_font;++start_font)
	{
		start_font->SaveInheritanceXml(&xmlFontList);
	}
	pXmlObject->InsertEndChild(xmlFontList);

	TiXmlElement xmlAttach("attach");
	if(m_pScrollBar) m_pScrollBar->SaveXml(&xmlAttach);
	pXmlObject->InsertEndChild(xmlAttach);

	return true;
}

bool CRichText::LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject)
{
	__super::LoadInheritanceXml(nVersion, pXmlObject, pProject);

	std::string strTemp;

	TiXmlElement *pXmlInheritance = pXmlObject->FirstChildElement("Inheritance");
	GetAttribute(pXmlInheritance, "linespace", m_nLineSpace);
	GetAttribute(pXmlInheritance, "index", m_nFontIndex);
	GetAttribute(pXmlInheritance, "limit", m_nLimitLine);
	GetAttribute(pXmlInheritance, "linescroll", m_bLineScroll);
	GetAttribute(pXmlInheritance, "minimum", m_nMinimumLineHeight);

	m_listFonts.clear();

	FONTINFO fontInfo;
	TiXmlElement *pXmlFontList = pXmlObject->FirstChildElement("fontlist");
	TiXmlElement *pXmlFont = pXmlFontList->FirstChildElement("font");
	while(pXmlFont)
	{
		fontInfo.LoadInheritanceXml(nVersion, pXmlFont, pProject);
		m_listFonts.push_back(fontInfo);
		pXmlFont = pXmlFont->NextSiblingElement("font");
	}

	m_pFontInfo = (FONTINFO *)GetFontInfo(m_nFontIndex);

	TiXmlElement *pXmlAttach = pXmlObject->FirstChildElement("attach");
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

	Create((int)m_rtLocalRect.GetWidth(), (int)m_rtLocalRect.GetHeight());

	if(IsTool())
	{
		MakeSample();
		SetMessage(true);
	}

	return true;
}

void CRichText::SetFontList(const FONTINFOLIST &listFont)
{
	if(m_listFonts == listFont) return;
	m_listFonts = listFont;

	FONTINFOLISTITER start = m_listFonts.begin();
	FONTINFOLISTITER end = m_listFonts.end();

	m_pFontInfo = NULL;

	if(m_listFonts.size())
	{
		if((m_nFontIndex < 0) || (m_nFontIndex >= (int)m_listFonts.size()))
		{
			m_nFontIndex = 0;
		}

		for(int nIndex = 0;start != end;++start, ++nIndex)
		{
			if(m_nFontIndex == nIndex) m_pFontInfo = &(*start);
		}
	}
}

void CRichText::AddFont(const FONTINFO &fontInfo)
{
	m_listFonts.push_back(fontInfo);
}

void CRichText::SetCurrentFont(int nIndex)
{
	if(m_nFontIndex == nIndex) return;
	if(nIndex < 0 || nIndex >= (int)m_listFonts.size()) return;
	m_nFontIndex = nIndex;

	FONTINFOLISTITER start = m_listFonts.begin();
	FONTINFOLISTITER end = m_listFonts.end();

	for(int nIndex = 0;start != end;++start, ++nIndex)
	{
		if(m_nFontIndex == nIndex) m_pFontInfo = &(*start);
	}
}

void CRichText::SetMinimumHight(int nMinimumHight)
{
	ASSERT(nMinimumHight >= -1);
	if(m_nMinimumLineHeight == nMinimumHight) return;
	m_nMinimumLineHeight = nMinimumHight;
	RepositionLine();
}

const int &CRichText::GetMiminumHight(void)
{
	return m_nMinimumLineHeight;
}

CRichItem *CRichText::AddItem(const std::string &strOutput, int nEmoticonIndex, const CSize &szSize, bool bEnd, LPARAM lParam)
{
	//첫번째 라인의 삭제 여부
	bool bRemoveItem = false;
	bool bNewLine = false;
	if(m_pLastLine == NULL || m_pLastLine->m_bEnd)
	{
		//최대 라인이 설정 되어 있고 최대 라인까지 채워져 있을때
		//새로운 라인을 추가하려고 하면 가장 앞 라인을 제거한다.
		if(m_nLimitLine > 0 && m_nLimitLine == (int)m_listLines.size())
		{
			bRemoveItem = true;
			RemoveLine(1);
		}

		//추가되는 라인의 시작 위치를 계산한다.
		float fStartY = 0.0f;
		if(m_pLastLine)
		{
			m_pLastLine->m_bEnd = true;
			fStartY = m_pLastLine->m_fStartY + m_pLastLine->m_szSize.cy + m_nLineSpace;
			bNewLine = true;
		}

		//새로운 라인을 추가한다.
		m_pLastLine = new CRichLine;
		m_pLastLine->m_fStartY = fStartY;
		m_pLastItem = NULL;
		m_listLines.push_back(m_pLastLine);
	}

	//새로운 아이템을 추가한다.
	CRichItem *pItem = new CRichItem;
	pItem->m_strOutput = strOutput;
	pItem->m_lParam = lParam;

	//공백문자("\n")가 들어왔을 경우, 높이를 기본 높이로 셋팅한다.
	pItem->m_szSize = szSize;
	pItem->m_pFontInfo = m_pFontInfo;
	
	if(nEmoticonIndex != -1)
	{
		pItem->m_pSprite = RM().Clone(m_pEmoticonSpr);
	}
	else
	{
		pItem->m_pSprite = NULL;
	}

	pItem->m_nIndex = nEmoticonIndex;

	m_pLastLine->m_listItems.push_back(pItem);
	m_pLastLine->m_bEnd = bEnd;
	m_pLastLine->m_uFormat = m_pFontInfo->m_uFormat;

	//라인의 폭을 추가된 아이템 만큼 더해준다.
	m_pLastLine->m_szSize.cx += pItem->m_szSize.cx;
	//이전 아이템의 자간만큼 라인의 폭을 보정해준다.
	if(m_pLastItem)
	{
		m_pLastLine->m_szSize.cx -= m_pLastItem->m_pFontInfo->m_nSpaceBetweenLetters;
		switch(m_pLastItem->m_pFontInfo->m_nDrawType)
		{
		case ETDT_SHADOW:
		case ETDT_OUTLINE:
			--m_pLastLine->m_szSize.cx;
			break;
		}

		if(pItem->m_pFontInfo->m_nDrawType == ETDT_OUTLINE) --m_pLastLine->m_szSize.cx;
	}

	//스크롤이 현재값이 마지막에 있으면 자동 개행해준다.
	bool bAutoScroll = false;
	if(m_pScrollBar && ((GetHeight() <= m_fTotalHeight) && ((GetHeight() - m_fTotalHeight) == m_fStartY)))
	{
		bAutoScroll = true;
	}

	if(bNewLine)
		m_fTotalHeight += m_nLineSpace;

	//라인의 높이를 보정해준다. 추가된 아이템이의 높이가 크면 추가된 아이템 값으로 셋팅
	//라인의 높이는 그 라인에 추가되는 아이템중 가장 높이가 높은 아이템을 기준으로 셋팅한다.
	if(m_pLastLine->m_szSize.cy < pItem->m_szSize.cy)
	{
		m_fTotalHeight -= m_pLastLine->m_szSize.cy;
		m_pLastLine->GetLineHeight(m_nMinimumLineHeight);
		m_fTotalHeight += m_pLastLine->m_szSize.cy;
	}

	//스크롤 바가 있으면 최대/최소 현재값을 다시 셋팅해준다.
	if(m_pScrollBar)
	{
		m_pScrollBar->SetRangeValue(0.0f, m_fTotalHeight - GetHeight());

		//스크롤 바를 보이거나 보이지 않게 설정한다.
		if(m_fTotalHeight > GetHeight())
		{
			if(m_pScrollBar->IsEnable() == false)
			{
				//스크롤바가 안보이다 보일때는 무조건 자동 개행해준다.
				m_pScrollBar->SetEnable(true);
				bAutoScroll = true;
			}
		}
		else
		{
			if(m_pScrollBar->IsEnable() == true)
			{
				m_pScrollBar->SetCurValue(0);
				m_pScrollBar->SetEnable(false);
			}
		}

		//자동 개행일 경우 가장 끝으로 이동 시켜준다.
		if(bAutoScroll)
		{
			m_fStartY = GetHeight() - m_fTotalHeight;
			m_pScrollBar->SetCurValue(-m_fStartY);
		}
	}

	//화면에 보이는 영역에 아이템이 추가되었을 경우 다시 그리기 해주도록 임시로 자동개행 변수를 셋팅한다.
	if(m_fStartY + m_pLastLine->m_fStartY >= 0 && m_fStartY + m_pLastLine->m_fStartY < GetHeight())
		bAutoScroll = true;

	//자동 개행이거나 첫 라인이 삭제되었을 때, 화면에 보이는 영역에 아이템이 추가되었을 경우 새로 그려준다.
	if(bAutoScroll || bRemoveItem) 
		SetRedraw();

	m_pLastItem = pItem;
	return pItem;
}

void CRichText::RemoveLine(int nLineCount)
{
	int nCount = min(nLineCount, (int)m_listLines.size());
	for(int i=0;i<nCount;++i)
	{
		RICHLINELISTITER start = m_listLines.begin();
		//첫번째 라인을 삭제 하였을때 제거되는 라인의 높이를 구한다.
		float fLineHeight = (*start)->m_szSize.cy + (float)m_nLineSpace;
		//전체 높이에서 삭제되는 라인의 높이를 빼준다.
		m_fTotalHeight -= fLineHeight;
		//첫번째 라인을 리스트에서 제거해준다.
		delete *start;
		m_listLines.erase(start);

		//전체 라인의 시작 Y값을 첫번째 라인의 높이 만큼씩을 빼준다.
		start = m_listLines.begin();
		RICHLINELISTITER end = m_listLines.end();
		for(;start != end;++start)
		{
			(*start)->m_fStartY -= fLineHeight;
		}

		//그리기 시작 위치를 제거되는 높이 만큼 더해준다.(이유는 m_fStartY값을 -값으로 증가한다)
		m_fStartY += fLineHeight;

		//m_fStartY값을 보정한다. 값의 범위를 벋어났는지를 체크한다.
		if(m_fStartY > 0.0f) m_fStartY = 0.0f;
		else if(m_fStartY < GetHeight()-m_fTotalHeight) m_fStartY = GetHeight()-m_fTotalHeight;
	}

	//스크롤이 Attatch되어 잇을경우 최대/최소값 현재값을 다시 셋팅해준다.
	if(m_pScrollBar)
	{
		//스크롤 바를 보이거나 보이지 않게 설정한다.
		if(m_fTotalHeight > GetHeight())
		{
			if(m_pScrollBar->IsEnable() == false)
			{
				//스크롤바가 안보이다 보일때는 무조건 자동 개행해준다.
				m_pScrollBar->SetEnable(true);
			}
		}
		else
		{
			if(m_pScrollBar->IsEnable() == true)
			{
				m_pScrollBar->SetCurValue(0);
				m_pScrollBar->SetEnable(false);
			}
		}


		m_pScrollBar->SetRangeValue(0.0f, m_fTotalHeight-GetHeight());
		m_pScrollBar->SetCurValue(-m_fStartY);
	}
}

void CRichText::SetLimitLine(const unsigned int &nLimitLine)
{
	if(m_nLimitLine == nLimitLine) return;
	m_nLimitLine = nLimitLine;
	if(m_nLimitLine > 0 && m_nLimitLine <= (int)m_listLines.size())
	{
		RemoveLine((int)m_listLines.size() - m_nLimitLine);
		SetRedraw();
	}
}

void CRichText::AddText(std::string strText, LPARAM lParam/* = NULL*/)
{
	if(m_pFontInfo == NULL) return;
	if(strText == "") return;
	ASSERT(m_pFontInfo != NULL);

	std::list<CTempRichText> listText;
	SplitEmoticon(strText, listText);

	std::list<CTempRichText>::iterator start = listText.begin();
	std::list<CTempRichText>::iterator end = listText.end();

	for(;start != end;++start)
	{
		if(start->m_nIndex == -1)
		{
			if(start->m_strOutput != "") AddForceText(start->m_strOutput, lParam);
		}
		else
		{
			AddEmoticon(start->m_strOutput, start->m_nIndex, lParam);
		}
	}
}

CRichItem *CRichText::AddEmoticon(const std::string &strOutput, int nIndex, LPARAM lParam)
{
	ASSERT(m_pEmoticonSpr != NULL);
	ASSERT(m_pEmoticonSpr->GetTotalSpr() > nIndex);

	int nSub = (m_pLastLine && !m_pLastLine->m_bEnd)?(m_pLastLine->m_szSize.cx):0;
	int nWidth = (int)m_pBack->GetLocalRect().GetWidth() - nSub;

	if(m_pLastItem)
	{
		switch(m_pLastItem->m_pFontInfo->m_nDrawType)
		{
		case ETDT_SHADOW:
		case ETDT_OUTLINE:
			++nWidth;
			break;
		}

		if(m_pFontInfo->m_nDrawType == ETDT_OUTLINE) ++nWidth;
		nWidth -= m_pLastItem->m_pFontInfo->m_nSpaceBetweenLetters;
	}

	CSize szSize(m_pEmoticonSpr->spr[nIndex].xl, m_pEmoticonSpr->spr[nIndex].yl);
	CRichItem *pItem = NULL;
	if(nWidth == szSize.cx)
	{
		pItem = AddItem(strOutput, nIndex, szSize, true, lParam);
	}
	else if(nWidth < szSize.cx)
	{
		m_pLastLine->m_bEnd = true;
		pItem = AddItem(strOutput, nIndex, szSize, false, lParam);
	}
	else
	{
		pItem = AddItem(strOutput, nIndex, szSize, false, lParam);
	}
	return pItem;
}

void CRichText::AddForceText(std::string strText, LPARAM lParam)
{
	int nLength = (int)strText.length();
	LPCTSTR lpszText = strText.c_str();

	HDC dc = GetDC(NULL);
	HGDIOBJ hOldFont = ::SelectObject( dc, m_pFontInfo->GetFont()->m_hObject );
	int nOldSpaceBetweenLetters = ::SetTextCharacterExtra( dc, m_pFontInfo->m_nSpaceBetweenLetters);

	int nAddPos = 0;
	int nEndPos = 0;
	int nStartPos = 0;
	int nSub = (m_pLastLine && !m_pLastLine->m_bEnd)?(m_pLastLine->m_szSize.cx):0;
	int nWidth = (int)m_pBack->GetLocalRect().GetWidth() - nSub;

	if(m_pLastItem)
	{
		switch(m_pLastItem->m_pFontInfo->m_nDrawType)
		{
		case ETDT_SHADOW:
		case ETDT_OUTLINE:
			++nWidth;
			break;
		}

		if(m_pFontInfo->m_nDrawType == ETDT_OUTLINE) ++nWidth;
		nWidth -= m_pLastItem->m_pFontInfo->m_nSpaceBetweenLetters;
	}

	CSize szSize;
	std::string strOutput;
	bool bNewLine = false;

	while(TRUE)
	{
		bNewLine = false;
		if(strText.at(nEndPos) == '\r')
		{
			//마지막 전 문자일 경우 \n을 비교한다.
			if((nEndPos < (nLength-1)) && strText.at(nEndPos+1) == '\n')
			{
				nAddPos = 2;
			}
			else
			{
				nAddPos = 1;
			}
			bNewLine = true;
		}
		else if(strText.at(nEndPos) == '\n')
		{
			nAddPos = 1;
			bNewLine = true;
		}
		else if(IsDBCSLeadByte(strText.at(nEndPos))) nAddPos = 2;
		else nAddPos = 1;

		nEndPos += nAddPos;

		if(bNewLine)
		{
			szSize = GetTextSize(dc, lpszText + nStartPos, nEndPos - nStartPos - nAddPos);
			strOutput = strText.substr(nStartPos, nEndPos - nStartPos - nAddPos);
			if(strOutput == "")
			{
				szSize = GetTextSize(dc, "A", 1);
				szSize.cx = 0;
			}
			AddItem(strOutput, -1, szSize, true, lParam);
			nWidth = (int)m_pBack->GetLocalRect().GetWidth();
			nStartPos = nEndPos;
		}
		else
		{	
			szSize = GetTextSize(dc, lpszText + nStartPos, nEndPos - nStartPos);
			if(nWidth <= szSize.cx)
			{
				if(nWidth != szSize.cx)
				{
					nEndPos -= nAddPos;
					szSize = GetTextSize(dc, lpszText + nStartPos, nEndPos - nStartPos);
				}

				strOutput = strText.substr(nStartPos, nEndPos - nStartPos);
				if(strOutput == "")
				{
					szSize = GetTextSize(dc, "A", 1);
				}
				AddItem(strOutput, -1, szSize, true, lParam);
				nWidth = (int)m_pBack->GetLocalRect().GetWidth();

				nStartPos = nEndPos;
			}
		}

		if(nLength <= nEndPos)
		{
			strOutput = strText.substr(nStartPos, nEndPos - nStartPos);
			if(strOutput == "")
			{
				szSize = GetTextSize(dc, "A", 1);
				szSize.cx = 0;
			}
			AddItem(strOutput, -1, szSize, false, lParam);
			break;
		}
	}

	::SelectObject( dc, hOldFont );
	::SetTextCharacterExtra( dc, nOldSpaceBetweenLetters);

	ReleaseDC(NULL, dc);
}

CSize CRichText::GetTextSize(HDC &dc, LPCTSTR lpText, const int &nLength)
{
	if(m_pFontInfo == NULL) return CSize(0,0);
	CSize szSize;
	::GetTextExtentPoint( dc, lpText, nLength, &szSize );

	switch(m_pFontInfo->m_nDrawType)
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

void CRichText::Draw(NMBASE::GRAPHICGDI::CPage *pPage)
{
	if(m_bRedraw) CheckRedraw();
	CObjectBase::Draw(pPage);
}

bool CRichText::CheckRedraw(void)
{
	CDC *pDC = m_pBack->GetDC();
	if(pDC == NULL) return false;
	if(m_pFontInfo == NULL) return false;

	m_pBack->ClearColor(RGB(0,0,255));

	RICHLINELISTITER start_line = m_listLines.begin();
	RICHLINELISTITER end_line = m_listLines.end();
	RICHITEMLISTITER start_item, end_item;

	CRect rtTextRect = m_pBack->GetLocalRect();
	CRect rtRect = CRect(0,m_fStartY,rtTextRect.GetWidth(),m_fStartY);
	CRect rtTemp;

	CRichLine *pLine = NULL;
	CRichItem *pItem, *pPrevItem;
	CPoint ptDraw;
	m_itFirstVisibleLine = m_listLines.end();

	NMBASE::GRAPHICGDI::CPage *pPage = m_pBack->GetPage();

	for(;start_line != end_line;++start_line)
	{
		pLine = *start_line;
		rtRect.bottom = rtRect.top + pLine->m_szSize.cy;

		if(pLine->m_uFormat & DT_CENTER)
		{
			ptDraw.x = (rtTextRect.GetWidth() - pLine->m_szSize.cx)/2.0f;
		}
		else if(pLine->m_uFormat & DT_LEFT)
		{
			ptDraw.x = rtTextRect.GetWidth() - pLine->m_szSize.cx;
		}
		else
		{
			ptDraw.x = 0.0f;
		}

		if(rtTemp.IntersectRect(&rtRect, &rtTextRect))
		{
			if(m_itFirstVisibleLine == m_listLines.end())
			{
				m_itFirstVisibleLine = start_line;
				if(m_bLineScroll && (m_fStartY != (GetHeight() - m_fTotalHeight)))
				{
					m_fStartY = -pLine->m_fStartY;
					rtRect.top = 0;
					rtRect.bottom = rtRect.top + pLine->m_szSize.cy;
				}
			}
			start_item = pLine->m_listItems.begin();
			end_item = pLine->m_listItems.end();

			pPrevItem = NULL;

			for(;start_item != end_item;++start_item)
			{
				pItem = *start_item;
				ptDraw.y = rtRect.top;
				
				if(pItem->m_pFontInfo->m_uFormat & DT_VCENTER)
				{
					ptDraw.y += (pLine->m_szSize.cy - pItem->m_szSize.cy)/2.0f;
				}
				else if(pItem->m_pFontInfo->m_uFormat & DT_BOTTOM)
				{
					ptDraw.y += (pLine->m_szSize.cy - pItem->m_szSize.cy);
				}

				if(pPrevItem)
				{
					switch(pPrevItem->m_pFontInfo->m_nDrawType)
					{
					case ETDT_SHADOW:
					case ETDT_OUTLINE:
						--ptDraw.x;
						break;
					}

					if(pItem->m_pFontInfo->m_nDrawType == ETDT_OUTLINE) --ptDraw.x;

					ptDraw.x -= pPrevItem->m_pFontInfo->m_nSpaceBetweenLetters;
				}

				if(pItem->m_pSprite && pItem->m_nIndex > -1 && pItem->m_nIndex < pItem->m_pSprite->GetTotalSpr())
				{
					int nOption = 0;
					if(pItem->m_pSprite->spr[pItem->m_nIndex].PixelFormat == 5558)
					{
						nOption = AB;
					}

					pPage->PutSprAuto((int)ptDraw.x, (int)ptDraw.y, pItem->m_pSprite, pItem->m_nIndex,nOption);
				}
				else
				{
					pDC->SelectObject((CFont *)pItem->m_pFontInfo->GetFont());
					pDC->SetTextCharacterExtra(pItem->m_pFontInfo->m_nSpaceBetweenLetters);

					switch(pItem->m_pFontInfo->m_nDrawType)
					{
					case ETDT_NORMAL:
						pDC->SetTextColor(pItem->m_pFontInfo->m_clrColor);
						pDC->TextOut((int)ptDraw.x, (int)ptDraw.y, pItem->m_strOutput.c_str());
						break;
					case ETDT_SHADOW:
						pDC->SetTextColor(pItem->m_pFontInfo->m_clrOutline);
						pDC->TextOut((int)ptDraw.x+1, (int)ptDraw.y+1, pItem->m_strOutput.c_str());
						pDC->SetTextColor(pItem->m_pFontInfo->m_clrColor);
						pDC->TextOut((int)ptDraw.x, (int)ptDraw.y, pItem->m_strOutput.c_str());
						break;
					case ETDT_OUTLINE:
						pDC->SetTextColor(pItem->m_pFontInfo->m_clrOutline);
						pDC->TextOut((int)ptDraw.x+1, (int)ptDraw.y, pItem->m_strOutput.c_str());
						pDC->TextOut((int)ptDraw.x+1, (int)ptDraw.y+2, pItem->m_strOutput.c_str());
						pDC->TextOut((int)ptDraw.x, (int)ptDraw.y+1, pItem->m_strOutput.c_str());
						pDC->TextOut((int)ptDraw.x+2, (int)ptDraw.y+1, pItem->m_strOutput.c_str());
						pDC->SetTextColor(pItem->m_pFontInfo->m_clrColor);
						pDC->TextOut((int)ptDraw.x+1, (int)ptDraw.y+1, pItem->m_strOutput.c_str());
						break;
					}
				}

				ptDraw.x += pItem->m_szSize.cx;
				pPrevItem = pItem;
			}
		}

		rtRect.bottom += m_nLineSpace;
		rtRect.top = rtRect.bottom;
	}

	m_bRedraw = false;
	return true;
}

void CRichText::SetLineSpace(const int &nLineSpace)
{
	if(m_nLineSpace == nLineSpace) return;
	m_nLineSpace = nLineSpace;
	RepositionLine();
}

void CRichText::SetLineScroll(const bool &bLine)
{
	if(m_bLineScroll == bLine) return;
	m_bLineScroll = bLine;
	SetRedraw();
}

void CRichText::CheckScrollData(void)
{
	if(m_pScrollBar == NULL) return;

	if(m_fTotalHeight <= GetHeight())
	{
		m_pScrollBar->SetCurValue(0);
		m_pScrollBar->SetEnable(false);
	}
	else
	{
		m_pScrollBar->SetRangeValue(0, m_fTotalHeight-GetHeight());
		if(m_fTotalHeight-GetHeight() < (m_fStartY * -1.0f))
		{
			m_fStartY = m_fTotalHeight-GetHeight();
		}	
		m_pScrollBar->SetCurValue(m_fStartY);
		m_pScrollBar->SetEnable(true);
	}
}

void CRichText::CheckUp(void)
{
	if(m_fStartY == 0) return;

	if(m_itFirstVisibleLine != m_listLines.end())
	{
		CRichLine *pFirstVisibleLine = *m_itFirstVisibleLine;
		if(pFirstVisibleLine->m_fStartY != -m_fStartY)
		{
			m_fStartY = -pFirstVisibleLine->m_fStartY;
			m_pScrollBar->SetCurValue(-m_fStartY);
		}
		else
		{
			RICHLINELISTITER findIt = find(m_listLines.begin(), m_listLines.end(), pFirstVisibleLine);
			if(findIt != m_listLines.end())
			{
				if(findIt == m_listLines.begin())
				{
					m_fStartY = 0.0f;
					m_pScrollBar->SetCurValue(0);
				}
				else
				{
					--findIt;
					m_fStartY = -(*findIt)->m_fStartY;
					m_pScrollBar->SetCurValue(-m_fStartY);
				}
			}
			else return;
		}
		SetRedraw();
	}
}

void CRichText::CheckDown(void)
{
	if((m_fTotalHeight + m_fStartY) <= GetHeight()) return;

	if(m_itFirstVisibleLine != m_listLines.end())
	{
		RICHLINELISTITER findIt = m_itFirstVisibleLine;
		++findIt;
		if(findIt == m_listLines.end())
		{
			m_fStartY = -(m_fTotalHeight - GetHeight());
			m_pScrollBar->SetCurValue(-m_fStartY);
		}
		else
		{
			CRichLine *pFirstVisibleLine = *m_itFirstVisibleLine;
			if((m_fTotalHeight - pFirstVisibleLine->m_fStartY) <= GetHeight())
			{
				m_fStartY = m_fTotalHeight - GetHeight();
			}
			else
			{
				m_fStartY = -(*findIt)->m_fStartY;
				if(m_fTotalHeight + m_fStartY <= GetHeight()) m_fStartY = GetHeight() - m_fTotalHeight;
				m_pScrollBar->SetCurValue(-m_fStartY);
			}
		}

		SetRedraw();
	}
}

LRESULT CRichText::OnClickUp(CMessage *pMsg)
{
	if(!m_bLineScroll) return FALSE;
	if(m_pScrollBar->IsIncrease()) CheckDown();
	else CheckUp();
	return TRUE;
}

LRESULT CRichText::OnClickDown(CMessage *pMsg)
{
	if(!m_bLineScroll) return FALSE;
	if(m_pScrollBar->IsIncrease()) CheckUp();
	else CheckDown();
	return TRUE;
}

void CRichText::CheckPageUp(void)
{
	if(m_fStartY == 0) return;

	if(m_itFirstVisibleLine != m_listLines.end())
	{
		m_fStartY += GetHeight();
		if(m_fStartY > 0) m_fStartY = 0;
		m_pScrollBar->SetCurValue(-m_fStartY);
		SetRedraw();
	}
}

void CRichText::CheckPageDown(void)
{
	if((m_fTotalHeight + m_fStartY) <= GetHeight()) return;

	if(m_itFirstVisibleLine != m_listLines.end())
	{
		m_fStartY -= GetHeight();
		if(GetHeight() - m_fTotalHeight > m_fStartY) m_fStartY = GetHeight() - m_fTotalHeight;
		m_pScrollBar->SetCurValue(-m_fStartY);
		SetRedraw();
	}
}

LRESULT CRichText::OnClickPageUp(CMessage *pMsg)
{
	if(!m_bLineScroll) return FALSE;
	if(m_pScrollBar->IsIncrease()) CheckPageDown();
	else CheckPageUp();
	return TRUE;
}

LRESULT CRichText::OnClickPageDown(CMessage *pMsg)
{
	if(!m_bLineScroll) return FALSE;
	if(m_pScrollBar->IsIncrease()) CheckPageUp();
	else CheckPageDown();
	return TRUE;
}

LRESULT CRichText::OnClickValueChange(CMessage *pMsg)
{
	CMO_CurValueChange *msg = (CMO_CurValueChange *)pMsg;
	float fCurValue = msg->GetValue();
	m_fStartY = -fCurValue;
	SetRedraw();
	return TRUE;
}

LRESULT CRichText::OnFocusInScroll(CMessage *pMsg)
{
	return (LRESULT)this;
}

LRESULT CRichText::OnLocalChildNotify(CMessage *msg)
{
	CObjectBase *pObject = ((CObjectMessage *)msg)->GetObject();
	if(pObject == m_pScrollBar)
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

void CRichText::AttachScroll(CScrollBar *pScall)
{
	ASSERT(m_pScrollBar == NULL && pScall != NULL);
	m_pScrollBar = pScall;
	AddLocalChild(m_pScrollBar);

	SetMessage(true);

	CheckScrollData();
}

void CRichText::DettachScroll(void)
{
	ASSERT(m_pScrollBar != NULL);
	SubChild(m_pScrollBar);

	SetMessage(false);

	m_pScrollBar = NULL;
}

bool CRichText::OnMouseWheel(const int &nKey, const short &zDelta)
{
	if(m_pScrollBar) m_pScrollBar->OnMouseWheel(nKey, zDelta);
	return CObjectBase::OnMouseWheel(nKey, zDelta);
}

void CRichText::Clear(void)
{
	RICHLINELISTITER start_line = m_listLines.begin();
	RICHLINELISTITER end_line = m_listLines.end();
	for(;start_line != end_line;++start_line)
	{
		delete *start_line;
	}

	m_listLines.clear();

	m_pLastLine = NULL;
	m_pLastItem = NULL;
	m_itFirstVisibleLine = m_listLines.end();
	m_fTotalHeight = 0;
	m_fStartY = 0.0f;

	if(m_pScrollBar)
	{
		m_pScrollBar->SetRangeValue(0,0);
		m_pScrollBar->SetCurValue(0);
		m_pScrollBar->SetEnable(false);
	}

	SetRedraw();
}

const int CRichText::GetFontInfoIndex(const FONTINFO *pFontInfo)
{
	FONTINFOLISTITER start = m_listFonts.begin();
	FONTINFOLISTITER end = m_listFonts.end();

	for(int i=0;start != end;++start,++i)
	{
		if(&(*start) == pFontInfo) return i;
	}

	return -1;
}

const FONTINFO *CRichText::GetFontInfo(const int &nIndex)
{
	FONTINFOLISTITER start = m_listFonts.begin();
	FONTINFOLISTITER end = m_listFonts.end();

	for(int i=0;start != end;++start,++i)
	{
		if(i == nIndex) return &(*start);
	}

	return NULL;
}

const int CRichText::GetLineIndex(CRichLine *pLine)
{
	RICHLINELISTITER start = m_listLines.begin();
	RICHLINELISTITER end = m_listLines.end();

	for(int i=0;start != end;++start,++i)
	{
		if(*start == pLine) return i;
	}
	return -1;

}
const CRichLine *CRichText::GetLineFromIndex(const int &nIndex)
{
	RICHLINELISTITER start = m_listLines.begin();
	RICHLINELISTITER end = m_listLines.end();

	for(int i=0;start != end;++start,++i)
	{
		if(i == nIndex) return *start;
	}
	return NULL;

}

CRichItem *CRichText::HitTest(const CPoint &ptPos, OUT CRect &rtItem)
{
	if(m_itFirstVisibleLine == m_listLines.end()) return NULL;

	RICHLINELISTITER start = m_itFirstVisibleLine;
	RICHLINELISTITER end = m_listLines.end();

	CRect rtTextRect = m_pBack->GetLocalRect();
	CRect rtRect = CRect(0,m_fStartY+(*start)->m_fStartY,rtTextRect.GetWidth(),m_fStartY+(*start)->m_fStartY);
	CRect rtTemp, rtRectItem;

	RICHITEMLISTITER start_item, end_item;

	CRichLine *pLine = NULL;
	CRichItem *pItem, *pPrevItem;
	CPoint ptDraw;
	int nAdd;

	for(;start != end;++start)
	{
		pLine = *start;
		rtRect.bottom = rtRect.top + pLine->m_szSize.cy;

		if(rtTemp.IntersectRect(&rtRect, &rtTextRect))
		{
			start_item = pLine->m_listItems.begin();
			end_item = pLine->m_listItems.end();

			pPrevItem = NULL;

			if(pLine->m_uFormat & DT_CENTER)
			{
				ptDraw.x = (rtTextRect.GetWidth() - pLine->m_szSize.cx)/2.0f;
			}
			else if(pLine->m_uFormat & DT_LEFT)
			{
				ptDraw.x = rtTextRect.GetWidth() - pLine->m_szSize.cx;
			}
			else
			{
				ptDraw.x = 0.0f;
			}

			for(;start_item != end_item;++start_item)
			{
				pItem = *start_item;
				ptDraw.y = rtRect.top;

				if(pItem->m_pFontInfo->m_uFormat & DT_VCENTER)
				{
					ptDraw.y += (pLine->m_szSize.cy - pItem->m_szSize.cy)/2.0f;
				}
				else if(pItem->m_pFontInfo->m_uFormat & DT_BOTTOM)
				{
					ptDraw.y += (pLine->m_szSize.cy - pItem->m_szSize.cy);
				}

				nAdd = 0;

				if(pPrevItem)
				{
					switch(pPrevItem->m_pFontInfo->m_nDrawType)
					{
					case ETDT_SHADOW:
					case ETDT_OUTLINE:
						--ptDraw.x;
						++nAdd;
						break;
					}

					if(pItem->m_pFontInfo->m_nDrawType == ETDT_OUTLINE)
					{
						--ptDraw.x;
						++nAdd;
					}

					ptDraw.x -= pPrevItem->m_pFontInfo->m_nSpaceBetweenLetters;
				}

				rtRectItem.left = ptDraw.x;
				rtRectItem.top = ptDraw.y;
				rtRectItem.right = rtRectItem.left + (*start_item)->m_szSize.cx + nAdd;
				rtRectItem.bottom = rtRectItem.top + (*start_item)->m_szSize.cy + nAdd;

				if(rtRectItem.PtInRect(ptPos))
				{
					rtItem = rtRectItem;
					return *start_item;
				}

				ptDraw.x += pItem->m_szSize.cx;
				pPrevItem = pItem;
			}
		}
		else
		{
			break;
		}

		rtRect.bottom += m_nLineSpace;
		rtRect.top = rtRect.bottom;
	}

	return NULL;
}

bool CRichText::OnLButtonDown(const CPoint &ptPos)
{
	CPoint ptLocal = GetGlobalToLocalPos(ptPos);
	CRect rtItem;
	CRichItem *pItem = HitTest(ptLocal, rtItem);
	if(pItem && pItem->m_lParam)
	{
		CMO_RichItemLButtonDown msg(this, pItem, ptLocal, ptPos, rtItem);
		Dispatch(&msg);
	}
	return CObjectBase::OnLButtonDown(ptPos);
}

bool CRichText::OnRButtonDown(const CPoint &ptPos)
{
	CPoint ptLocal = GetGlobalToLocalPos(ptPos);
	CRect rtItem;
	CRichItem *pItem = HitTest(ptLocal, rtItem);
	if(pItem && pItem->m_lParam)
	{
		CMO_RichItemRButtonDown msg(this, pItem, ptLocal, ptPos, rtItem);
		Dispatch(&msg);
	}
	return CObjectBase::OnRButtonDown(ptPos);
}

void CRichText::SplitEmoticon(std::string strOutput, std::list<CTempRichText> &listText)
{
	if(strOutput == "")
	{
		listText.push_back(CTempRichText(strOutput, -1));
		return;
	}

	std::string::size_type nPos1, nPos2, nStart, nPrevStart;
	std::string strTemp1, strEmoticon;

	nStart = 0;
	nPrevStart = 0;

	int nIndex = 0;
	while(1)
	{
		nPos1 = strOutput.find('/',nStart);
		if(nPos1 == std::string::npos)
		{
			strTemp1 = strOutput.substr(nPrevStart, strOutput.size() - nPrevStart);
			listText.push_back(CTempRichText(strTemp1, -1));
			break;
		}

		nPos2 = strOutput.find('/', nPos1+1);
		if(nPos2 == std::string::npos)
		{
			strTemp1 = strOutput.substr(nPrevStart, strOutput.size() - nPrevStart);
			listText.push_back(CTempRichText(strTemp1, -1));
			break;
		}

		strTemp1 = strOutput.substr(nPrevStart, nPos1 - nPrevStart);
		strEmoticon = strOutput.substr(nPos1+1, nPos2 - nPos1 - 1);

		nStart = nPos2 + 1;

		nIndex = GetEmoticonIndex(strEmoticon);

		if(nIndex != -1)
		{
			listText.push_back(CTempRichText(strTemp1, -1));
			listText.push_back(CTempRichText(strEmoticon, nIndex));
		
			nPrevStart = nStart;
		}
	}
}

int CRichText::GetEmoticonIndex(std::string &strTemp)
{
	EMOTICONMAPITER findIt = m_mapEmoticon.find(strTemp);
	if(findIt == m_mapEmoticon.end()) return -1;
	return findIt->second;
}

const std::string &CRichText::GetString(int nMaxLength/* = -1*/)
{
	RICHLINELISTITER start = m_listLines.begin();
	RICHLINELISTITER end = m_listLines.end();

	RICHITEMLISTITER start_item, end_item;

	CRichLine *pLine = NULL;
	CRichItem *pItem = NULL;

	m_strTotalString.clear();

	for(;start != end;++start)
	{
		pLine = *start;
		start_item = pLine->m_listItems.begin();
		end_item = pLine->m_listItems.end();

		for(;start_item != end_item;++start_item)
		{
			pItem = *start_item;
			//이모티콘 일때
			if(pItem->m_pSprite)
			{
				m_strTotalString += "/";
				m_strTotalString += pItem->m_strOutput;
				m_strTotalString += "/";
			}
			//일반 텍스트
			else
			{
				m_strTotalString += pItem->m_strOutput;
			}
		}

		m_strTotalString += "\n";
	}

	if(nMaxLength != -1)
	{
		int nLength = (int)m_strTotalString.size();
		if(nMaxLength < nLength)
		{
			int i=0;
			while(i<nLength)
			{
				if(IsDBCSLeadByte(m_strTotalString.at(i)))
				{
					i += 2;
				}
				else
				{
					++i;
				}

				if((nLength - i) < nMaxLength)
				{
					break;
				}
			}

			m_strTotalString = m_strTotalString.substr(i, nLength-i);
		}
	}

	return m_strTotalString;
}

void CRichText::MakeSample(void)
{
	Clear();

	FONTINFOLISTITER start = m_listFonts.begin();
	FONTINFOLISTITER end = m_listFonts.end();

	CString strTemp;
	CString strDrawMode;
	CString strArrange;
	for(int i=0;start != end;++start,++i)
	{
		switch(start->m_nDrawType)
		{
		case ETDT_NORMAL:
			strDrawMode = "일반";
			break;
		case ETDT_OUTLINE:
			strDrawMode = "외곽선";
			break;
		case ETDT_SHADOW:
			strDrawMode = "그림자";
			break;
		}

		switch(start->m_uFormat & (DT_VCENTER | DT_BOTTOM | DT_TOP))
		{
		case DT_TOP:
			strArrange = "상단";
			break;
		case DT_VCENTER:
			strArrange = "중단";
			break;
		case DT_BOTTOM:
			strArrange = "하단";
			break;
		}

		switch(start->m_uFormat & (DT_CENTER | DT_LEFT | DT_RIGHT))
		{
		case DT_LEFT:
			strArrange += "좌측";
			break;
		case DT_CENTER:
			strArrange += "중앙";
			break;
		case DT_RIGHT:
			strArrange += "우측";
			break;
		}

		strTemp.Format("폰트명 : %s, 크기 : %d, 굵게 : %s, 색 : %06X, 외곽색 : %06X, 그리기모드 : %s, 자간 : %d, 정렬 : %s\n",
			start->GetName().c_str(), start->GetSize(), start->IsBold()?"true":"false", start->m_clrColor, start->m_clrOutline,
			strDrawMode, start->m_nSpaceBetweenLetters, strArrange);

		SetCurrentFont(i);
		AddText(strTemp.GetString());
	}
}

void CRichText::RepositionLine(void)
{
	m_fTotalHeight = 0.0f;

	RICHLINELISTITER start = m_listLines.begin();
	RICHLINELISTITER end = m_listLines.end();

	bool bFirst = true;
	float fHeight = 0.0f;
	for(;start != end;++start)
	{
		if(bFirst) bFirst = false;
		else m_fTotalHeight = fHeight + m_nLineSpace;

		(*start)->m_fStartY = m_fTotalHeight;

		fHeight = (float)(*start)->GetLineHeight(m_nMinimumLineHeight);
	}

	m_fTotalHeight += fHeight;

	if(m_pScrollBar)
		m_pScrollBar->SetRangeValue(0.0f, m_fTotalHeight - GetHeight());

	SetRedraw();
}

void CRichText::SetObjectProject(CObjectProject *pProject)
{
	__super::SetObjectProject(pProject);
	m_pBack->SetTempEventDraw(m_bEventDraw);
}
} //namespace LSY