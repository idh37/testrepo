#include "stdafx.h"
#include "TextList.h"
#include "../Control/ScrollBar.h"
#include "../../Functor/Message.h"
#include <algorithm>
#include "../ObjectMan.h"
#include "../Base/Image.h"
#include "../Base/DumyImage.h"
#include "../../tinyxml/tinyxml.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace LSY
{
//기본 비교 함수
bool CTextList_DefaultCompareFunction(CTextList::TEXTITEM *pItem1, CTextList::TEXTITEM *pItem2, bool bIncrement, LPARAM lParam)
{
	if (bIncrement)
		return strcmp(pItem1->m_strOutput.c_str(), pItem2->m_strOutput.c_str()) < 0;
	else
		return strcmp(pItem1->m_strOutput.c_str(), pItem2->m_strOutput.c_str()) > 0;
}

//기본 찾기 함수, lUserData에 문자열의 포인터를 넘겨주면 된다.
bool CTextList_DefaultFindFunction(CTextList::TEXTITEM *pItem, LPARAM lUserData, LPARAM lParam)
{
	return (pItem->m_strOutput == (char *)lUserData);
}

bool CTextList_FindFromlParamFunction(CTextList::TEXTITEM *pItem, LPARAM lUserData, LPARAM lParam)
{
	return (pItem->m_lParam == lUserData);
}

CTextList::TEXTITEM::TEXTITEM()
:	m_strOutput(""),
	m_szSize(0,0),
	m_lParam(NULL),
	m_fStartY(0.0f),
	m_clrColor(RGB(0,0,0))
{
}

CTextList::TEXTITEM::~TEXTITEM()
{

}

CTextList::TEXTITEM *CTextList::TEXTITEM::Clone(void)
{
	TEXTITEM *pItem = new TEXTITEM;
	pItem->m_strOutput	= m_strOutput;
	pItem->m_szSize		= m_szSize;
	pItem->m_lParam		= m_lParam;
	pItem->m_fStartY	= m_fStartY;
	pItem->m_clrColor	= m_clrColor;
	return pItem;
}

size_t CTextList::TEXTITEM::GetInheritanceClipSize(int &nVersion)
{
	size_t nRet = sizeof(size_t);		//문자열 크기
	nRet += m_strOutput.size();			//문자열
	nRet += sizeof(CSize);				//m_szSize
	nRet += sizeof(float);				//m_fStartY
	nRet += sizeof(COLORREF);			//m_clrColor
	return nRet;
}

size_t CTextList::TEXTITEM::SaveInheritanceClipSize(int &nVersion, BYTE *pDest)
{
	size_t nRet = 0;
	size_t nSize = m_strOutput.size();
	memcpy(pDest+nRet, &nSize, sizeof(size_t));
	nRet += sizeof(size_t);				//문자열 크기
	if(nSize)
	{
		memcpy(pDest+nRet, m_strOutput.c_str(), nSize);
		nRet += nSize;			//문자열
	}
	memcpy(pDest+nRet, &m_szSize, sizeof(CSize));
	nRet += sizeof(CSize);				//m_szSize
	memcpy(pDest+nRet, &m_fStartY, sizeof(float));
	nRet += sizeof(float);				//m_fStartY
	memcpy(pDest+nRet, &m_clrColor, sizeof(COLORREF));
	nRet += sizeof(COLORREF);			//m_clrColor
	return nRet;
}

size_t CTextList::TEXTITEM::LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID)
{
	size_t nRet = 0;
	size_t nSize = 0;
	memcpy(&nSize, pDest+nRet, sizeof(size_t));
	nRet += sizeof(size_t);				//문자열 크기
	if(nSize)
	{
		GetPointerToString(pDest+nRet, nSize, m_strOutput);
		nRet += nSize;			//문자열
	}
	memcpy(&m_szSize, pDest+nRet, sizeof(CSize));
	nRet += sizeof(CSize);				//m_szSize
	memcpy(&m_fStartY, pDest+nRet, sizeof(float));
	nRet += sizeof(float);				//m_fStartY
	memcpy(&m_clrColor, pDest+nRet, sizeof(COLORREF));
	nRet += sizeof(COLORREF);			//m_clrColor
	return nRet;
}

bool CTextList::CCompareFuntion::operator() (CTextList::TEXTITEM *pItem1, CTextList::TEXTITEM *pItem2)
{
	return m_pFunction(pItem1, pItem2, m_bIncrement, m_lParam);
}

CTextList::CTextList(void)
:	m_bRedraw(false),
	m_clrFocusText(RGB(255,255,255)),
	m_clrSelectText(RGB(255,255,255)),
	m_clrSelectBack(RGB(198,198,198)),
	m_clrFocusBack(RGB(128,128,128)),
	m_bUseSprite(false),
	m_nLineHeight(16),
	m_fStartY(0.0f),
	m_pFirstVisibleItem(NULL),
	m_bLineScroll(true),
	m_pScrollBar(NULL),
	m_pSelectItem(NULL),
	m_pFocusItem(NULL),
	m_pBar(NULL),
	m_nBarIndex(0),
	m_fTotalHeight(0.0f),
	m_nSortType(-1),
	m_nFindType(-1),
	m_bIncrement(true),
	m_bAutoSort(false),
	m_pImage(NULL),
	m_pBack(NULL),
	m_nIndent(0),
	m_bUseFocus(true)
{
	m_cFontInfo.m_clrColor = RGB(198,198,198);
	m_cFontInfo.m_uFormat = DT_LEFT | DT_VCENTER | DT_SINGLELINE;
	SetMessage(true);

	m_DefaultCompare.m_pFunction = CTextList_DefaultCompareFunction;
	m_DefaultFind.m_pFindFunction = CTextList_DefaultFindFunction;

	m_itFind = m_listItems.end();
	
	m_itFirstVisible = m_itFind;
	m_nFirstVisible = -1;

	m_itLastVisible = m_itFind;
	m_nLastVisible = -1;

	m_nSelectItem = -1;
	m_nFocusItem = -1;

	m_pBack = new CDumyImage;
	AddLocalChild(m_pBack);
	m_pBack->SetMessage(true);
	m_pBack->SetInterceptSelect(this);
}

CTextList::~CTextList()
{
	if(m_pBar)
	{
		RM().RemoveSprite(m_pBar);
		m_pBar = NULL;
	}

	RemoveAll();
}

void CTextList::SetDefaultFindType(bool bUserData)
{
	if(bUserData) m_DefaultFind.m_pFindFunction = CTextList_FindFromlParamFunction;
	else m_DefaultFind.m_pFindFunction = CTextList_DefaultFindFunction;
}

void CTextList::RemoveAll(void)
{
	TEXTITEMLISTITER start = m_listItems.begin();
	TEXTITEMLISTITER end = m_listItems.end();
	for(;start != end;++start)
	{
		delete *start;
	}
	m_listItems.clear();

	m_pFirstVisibleItem = NULL;
	m_itFirstVisible = m_listItems.end();
	m_nFirstVisible = -1;

	m_pLastVisibleItem = NULL;
	m_itLastVisible = m_listItems.end();
	m_nLastVisible = -1;

	m_pSelectItem = NULL;
	m_nSelectItem = -1;

	m_pFocusItem = NULL;
	m_nFocusItem = -1;

	m_fStartY = 0.0f;
	m_fTotalHeight = 0.0f;

	SetRedraw();
}

CTextList::CCompareFuntion *CTextList::GetCompareFunction(const int &nType)
{
	std::map<int, CCompareFuntion>::iterator findIt = m_listCompare.find(nType);
	if (findIt != m_listCompare.end())
		return &findIt->second;

	return &m_DefaultCompare;
}

void CTextList::SetCompareFunction(const int &nType, const LISTCOMPAREFUNCTION pCompare)
{
	std::map<int, CCompareFuntion>::iterator findIt = m_listCompare.find(nType);
	if (findIt == m_listCompare.end())
	{
		m_listCompare[nType].m_pFunction = pCompare;
	}
	else
	{
		if (findIt->second.m_pFunction != pCompare)
			findIt->second.m_pFunction = pCompare;
	}
}

void CTextList::SetCompareFunctionParam(const int &nType, const LPARAM &lParam)
{
	std::map<int, CCompareFuntion>::iterator findIt = m_listCompare.find(nType);
	if (findIt != m_listCompare.end())
	{
		findIt->second.m_lParam = lParam;
		return;
	}

	ASSERT(NULL);
}

void CTextList::SetFindFunction(const int &nType, const LISTITEMFINDFUNCTION &pFindFunction)
{
	std::map<int, CFindFuntion>::iterator findIt = m_listFind.find(nType);
	if(pFindFunction == NULL)
	{
		if(findIt != m_listFind.end()) m_listFind.erase(findIt);
		return;
	}

	if(findIt == m_listFind.end())
	{
		m_listFind[nType].m_pFindFunction = pFindFunction;
	}
	else
	{
		if(findIt->second.m_pFindFunction == pFindFunction) return;
		findIt->second.m_pFindFunction = pFindFunction;
	}
}

void CTextList::SetFindFunctionParam(const int &nType, const LPARAM &lParam)
{
	std::map<int, CFindFuntion>::iterator findIt = m_listFind.find(nType);

	if(findIt == m_listFind.end())
	{
		ASSERT(NULL);
	}
	else
	{
		findIt->second.m_lParam = lParam;
	}
}

CTextList::CFindFuntion *CTextList::GetFindFunction(const int &nType)
{
	std::map<int, CFindFuntion>::iterator findIt = m_listFind.find(nType);
	if(findIt == m_listFind.end()) return &m_DefaultFind;
	return &findIt->second;
}

void CTextList::SetSortType(const int &nType)
{
	if(m_nSortType == nType) return;
	m_nSortType = nType;
}

void CTextList::SetIncrement(const bool &bIncrement)
{
	if (m_bIncrement == bIncrement) return;
	m_bIncrement = bIncrement;
}

void CTextList::SetAutoSort(const bool &bAuto)
{
	if(m_bAutoSort == bAuto) return;
	m_bAutoSort = bAuto;
	if(m_bAutoSort) Sort();
}

const int CTextList::Sort(TEXTITEM *pItem/* = NULL*/)
{
	if (m_listItems.size() < 2)
		return 1;

	CCompareFuntion* pCompare = GetCompareFunction(m_nSortType);
	if (pCompare == NULL)
	{
		assert(!"소트 펑션을 찾지 못했습니다.");
		return -1;
	}

	pCompare->m_bIncrement = m_bIncrement;
	m_listItems.sort(*pCompare);

	TEXTITEMLISTITER start = m_listItems.begin();
	TEXTITEMLISTITER end = m_listItems.end();
	int nIndex = -1;

	float fTotalHeight = 0.0f;

	for(int i=0;start != end;++start,++i)
	{
		if(*start == m_pSelectItem) m_nSelectItem = i;
		if(*start == m_pFocusItem) m_nFocusItem = i;
		if(*start == pItem) nIndex = i;

		(*start)->m_fStartY = fTotalHeight;
		fTotalHeight += (float)m_nLineHeight;
	}

	CheckVisibleItem();
	SetRedraw();
	return nIndex;
}

void CTextList::SetRedraw(void)
{
	if(m_bRedraw) return;
	m_bRedraw = true;
	CheckEventDraw();
}

bool CTextList::Create(int nWidth, int nHeight)
{
	if(!m_pBack->Create(nWidth, nHeight)) return false;

	SetLocalRect(m_pBack->GetLocalRect());
	CheckScrollData();
	CheckVisibleItem();
	SetRedraw();

	if(IsTool() && GetItemCount() == 0) MakeSample();

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
	
	if(m_pBar) pList->m_pBar	= RM().Clone(m_pBar);
	pList->m_nBarIndex			= m_nBarIndex;

	TEXTITEMLISTITER start = m_listItems.begin();
	TEXTITEMLISTITER end = m_listItems.end();

	for(;start != end;++start)
	{
		pList->m_listItems.push_back((*start)->Clone());
	}

	pList->m_listCompare = m_listCompare;
	pList->m_listFind = m_listFind;

	pList->m_bRedraw = true;

	pList->m_cFontInfo = m_cFontInfo;
	pList->m_clrFocusText = m_clrFocusText;
	pList->m_clrSelectText = m_clrSelectText;
	pList->m_clrSelectBack = m_clrSelectBack;
	pList->m_clrFocusBack = m_clrFocusBack;

	pList->m_pFirstVisibleItem = pList->GetItem(GetItemIndex(m_pFirstVisibleItem));
	pList->m_pLastVisibleItem = pList->GetItem(GetItemIndex(m_pLastVisibleItem));
	pList->m_pSelectItem = pList->GetItem(GetItemIndex(m_pSelectItem));
	pList->m_pFocusItem = pList->GetItem(GetItemIndex(m_pFocusItem));

	pList->m_nSortType = m_nSortType;
	pList->m_nFindType = m_nFindType;
	pList->m_bIncrement = m_bIncrement;
	pList->m_bAutoSort = m_bAutoSort;
	pList->m_bUseSprite = m_bUseSprite;

	pList->m_bLineScroll = m_bLineScroll;
	pList->m_nLineHeight = m_nLineHeight;

	pList->m_fStartY = m_fStartY;
	pList->m_fTotalHeight = m_fTotalHeight;
	pList->m_nIndent = m_nIndent;
	pList->m_bUseFocus = m_bUseFocus;

	pList->m_pBack->Create((int)m_rtLocalRect.GetWidth(), (int)m_rtLocalRect.GetHeight());

	if(m_pImage)
	{
		pList->AttachBackImage((CImage *)m_pImage->Clone(true));
	}

	if(m_pScrollBar) pList->AttachScroll((CScrollBar *)m_pScrollBar->Clone(true));

	return pObject;
}

size_t CTextList::GetInheritanceClipSize(int &nVersion)
{
	size_t nRet = __super::GetInheritanceClipSize(nVersion);
	nRet += sizeof(size_t);		//이미지바 파일명 크기
	if(m_pBar) nRet += m_pBar->GetFilePath().size();	//이미지바 파일명
	nRet += sizeof(int);		//m_nBarIndex
	nRet += sizeof(size_t);		//아이템 갯수

	TEXTITEMLISTITER start = m_listItems.begin();
	TEXTITEMLISTITER end = m_listItems.end();
	
	for(;start != end;++start)
	{
		nRet += (*start)->GetInheritanceClipSize(nVersion);
	}

	nRet += m_cFontInfo.GetInheritanceClipSize(nVersion);	//m_cFontInfo
	nRet += sizeof(COLORREF);						//m_clrFocusText
	nRet += sizeof(COLORREF);						//m_clrSelectText
	nRet += sizeof(COLORREF);						//m_clrSelectBack
	nRet += sizeof(COLORREF);						//m_clrFocusBack
	nRet += sizeof(int);							//GetItemIndex(m_pFirstVisibleItem)
	nRet += sizeof(int);							//GetItemIndex(m_pLastVisibleItem)
	nRet += sizeof(int);							//GetItemIndex(m_pSelectItem)
	nRet += sizeof(int);							//GetItemIndex(m_pFocusItem)
	nRet += sizeof(int);							//m_nSortType
	nRet += sizeof(int);							//m_nFindType
	nRet += sizeof(bool);							//m_bIncrement
	nRet += sizeof(bool);							//m_bAutoSort
	nRet += sizeof(bool);							//m_bUseSprite
	nRet += sizeof(bool);							//m_bLineScroll
	nRet += sizeof(int);							//m_nLineHeight
	nRet += sizeof(float);							//m_fStartY
	nRet += sizeof(float);							//m_fTotalHeight
	nRet += sizeof(bool);							//배경이미지 사용유무
	if(m_pImage) nRet += m_pImage->GetClipSize(nVersion);
	nRet += sizeof(bool);							//스크롤바 사용유무
	if(m_pScrollBar) nRet += m_pScrollBar->GetClipSize(nVersion);
	nRet += sizeof(int);							//m_nIndent
	nRet += sizeof(bool);							//m_bUseFocus
	return nRet;
}

size_t CTextList::SaveInheritanceClipSize(int &nVersion, BYTE *pDest)
{
	size_t nRet = __super::SaveInheritanceClipSize(nVersion, pDest);
	size_t nSize = 0;
	if(m_pBar) nSize = m_pBar->GetFilePath().size();
	memcpy(pDest+nRet, &nSize, sizeof(size_t));
	nRet += sizeof(size_t);		//이미지바 파일명 크기
	if(m_pBar)
	{
		memcpy(pDest+nRet, m_pBar->GetFilePath().c_str(), nSize);
		nRet += nSize;	//이미지바 파일명
	}
	memcpy(pDest+nRet, &m_nBarIndex, sizeof(int));
	nRet += sizeof(int);		//m_nBarIndex

	nSize = m_listItems.size();
	memcpy(pDest+nRet, &nSize, sizeof(size_t));
	nRet += sizeof(size_t);		//아이템 갯수

	TEXTITEMLISTITER start = m_listItems.begin();
	TEXTITEMLISTITER end = m_listItems.end();

	for(;start != end;++start)
	{
		nRet += (*start)->SaveInheritanceClipSize(nVersion, pDest+nRet);
	}

	nRet += m_cFontInfo.SaveInheritanceClipSize(nVersion, pDest+nRet);	//m_cFontInfo

	memcpy(pDest+nRet, &m_clrFocusText, sizeof(COLORREF));
	nRet += sizeof(COLORREF);						//m_clrFocusText
	memcpy(pDest+nRet, &m_clrSelectText, sizeof(COLORREF));
	nRet += sizeof(COLORREF);						//m_clrSelectText
	memcpy(pDest+nRet, &m_clrSelectBack, sizeof(COLORREF));
	nRet += sizeof(COLORREF);						//m_clrSelectBack
	memcpy(pDest+nRet, &m_clrFocusBack, sizeof(COLORREF));
	nRet += sizeof(COLORREF);						//m_clrFocusBack

	int nIndex = GetItemIndex(m_pFirstVisibleItem);
	memcpy(pDest+nRet, &nIndex, sizeof(int));
	nRet += sizeof(int);							//GetItemIndex(m_pFirstVisibleItem)

	nIndex = GetItemIndex(m_pLastVisibleItem);
	memcpy(pDest+nRet, &nIndex, sizeof(int));
	nRet += sizeof(int);							//GetItemIndex(m_pLastVisibleItem)

	nIndex = GetItemIndex(m_pSelectItem);
	memcpy(pDest+nRet, &nIndex, sizeof(int));
	nRet += sizeof(int);							//GetItemIndex(m_pSelectItem)

	nIndex = GetItemIndex(m_pFocusItem);
	memcpy(pDest+nRet, &nIndex, sizeof(int));
	nRet += sizeof(int);							//GetItemIndex(m_pFocusItem)

	memcpy(pDest+nRet, &m_nSortType, sizeof(int));
	nRet += sizeof(int);							//m_nSortType
	memcpy(pDest+nRet, &m_nFindType, sizeof(int));
	nRet += sizeof(int);							//m_nFindType
	memcpy(pDest+nRet, &m_bIncrement, sizeof(bool));
	nRet += sizeof(bool);							//m_bIncrement
	memcpy(pDest+nRet, &m_bAutoSort, sizeof(bool));
	nRet += sizeof(bool);							//m_bAutoSort
	memcpy(pDest+nRet, &m_bUseSprite, sizeof(bool));
	nRet += sizeof(bool);							//m_bUseSprite
	memcpy(pDest+nRet, &m_bLineScroll, sizeof(bool));
	nRet += sizeof(bool);							//m_bLineScroll
	memcpy(pDest+nRet, &m_nLineHeight, sizeof(int));
	nRet += sizeof(int);							//m_nLineHeight
	memcpy(pDest+nRet, &m_fStartY, sizeof(float));
	nRet += sizeof(float);							//m_fStartY
	memcpy(pDest+nRet, &m_fTotalHeight, sizeof(float));
	nRet += sizeof(float);							//m_fTotalHeight
	
	bool bUse = m_pImage?true:false;
	memcpy(pDest+nRet, &bUse, sizeof(bool));
	nRet += sizeof(bool);							//배경이미지 사용유무
	if(m_pImage) nRet += m_pImage->SaveClipSize(nVersion, pDest+nRet);

	bUse = m_pScrollBar?true:false;
	memcpy(pDest+nRet, &bUse, sizeof(bool));
	nRet += sizeof(bool);							//스크롤바 사용유무
	if(m_pScrollBar) nRet += m_pScrollBar->SaveClipSize(nVersion, pDest+nRet);
	memcpy(pDest+nRet, &m_nIndent, sizeof(int));
	nRet += sizeof(int);
	memcpy(pDest+nRet, &m_bUseFocus, sizeof(bool));
	nRet += sizeof(bool);
	return nRet;
}

size_t CTextList::LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID)
{
	size_t nRet = __super::LoadInheritanceClipSize(nVersion, pDest, pProject, bSetID);
	size_t nSize = 0;
	std::string strFileName;

	memcpy(&nSize, pDest+nRet, sizeof(size_t));
	nRet += sizeof(size_t);		//이미지바 파일명 크기
	if(nSize)
	{
		GetPointerToString(pDest+nRet, nSize, strFileName);
		nRet += nSize;	//이미지바 파일명

		SetBarSprite(strFileName);
	}
	memcpy(&m_nBarIndex, pDest+nRet, sizeof(int));
	nRet += sizeof(int);		//m_nBarIndex

	memcpy(&nSize, pDest+nRet, sizeof(size_t));
	nRet += sizeof(size_t);		//아이템 갯수

	RemoveAll();

	TEXTITEM *pItem = NULL;
	for(int i=0;i<(int)nSize;++i)
	{
		pItem = new TEXTITEM;
		nRet += pItem->LoadInheritanceClipSize(nVersion, pDest+nRet, pProject, bSetID);
		m_listItems.push_back(pItem);
	}

	nRet += m_cFontInfo.LoadInheritanceClipSize(nVersion, pDest+nRet, pProject, bSetID);	//m_cFontInfo

	memcpy(&m_clrFocusText, pDest+nRet, sizeof(COLORREF));
	nRet += sizeof(COLORREF);						//m_clrFocusText
	memcpy(&m_clrSelectText, pDest+nRet, sizeof(COLORREF));
	nRet += sizeof(COLORREF);						//m_clrSelectText
	memcpy(&m_clrSelectBack, pDest+nRet, sizeof(COLORREF));
	nRet += sizeof(COLORREF);						//m_clrSelectBack
	memcpy(&m_clrFocusBack, pDest+nRet, sizeof(COLORREF));
	nRet += sizeof(COLORREF);						//m_clrFocusBack

	int nIndex = 0;
	memcpy(&nIndex, pDest+nRet, sizeof(int));
	nRet += sizeof(int);							//GetItemIndex(m_pFirstVisibleItem)

	m_pFirstVisibleItem = GetItem(nIndex);

	memcpy(&nIndex, pDest+nRet, sizeof(int));
	nRet += sizeof(int);							//GetItemIndex(m_pLastVisibleItem)

	m_pLastVisibleItem = GetItem(nIndex);

	memcpy(&nIndex, pDest+nRet, sizeof(int));
	nRet += sizeof(int);							//GetItemIndex(m_pSelectItem)

	m_pSelectItem = GetItem(nIndex);

	memcpy(&nIndex, pDest+nRet, sizeof(int));
	nRet += sizeof(int);							//GetItemIndex(m_pFocusItem)

	m_pFocusItem = GetItem(nIndex);

	memcpy(&m_nSortType, pDest+nRet, sizeof(int));
	nRet += sizeof(int);							//m_nSortType
	memcpy(&m_nFindType, pDest+nRet, sizeof(int));
	nRet += sizeof(int);							//m_nFindType
	memcpy(&m_bIncrement, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);							//m_bIncrement
	memcpy(&m_bAutoSort, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);							//m_bAutoSort
	memcpy(&m_bUseSprite, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);							//m_bUseSprite
	memcpy(&m_bLineScroll, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);							//m_bLineScroll
	memcpy(&m_nLineHeight, pDest+nRet, sizeof(int));
	nRet += sizeof(int);							//m_nLineHeight
	memcpy(&m_fStartY, pDest+nRet, sizeof(float));
	nRet += sizeof(float);							//m_fStartY
	memcpy(&m_fTotalHeight, pDest+nRet, sizeof(float));
	nRet += sizeof(float);							//m_fTotalHeight

	bool bUse = false;
	memcpy(&bUse, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);							//배경이미지 사용유무
	if(bUse)
	{
		CImage *pImage = new CImage;
		nRet += sizeof(ENUM_OBJECTTYPE);
		nRet += pImage->LoadClipSize(nVersion, pDest+nRet, pProject, bSetID);
		AttachBackImage(pImage);
	}

	memcpy(&bUse, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);							//스크롤바 사용유무
	if(bUse)
	{
		CScrollBar *pScroll = new CScrollBar;
		nRet += sizeof(ENUM_OBJECTTYPE);
		nRet += pScroll->LoadClipSize(nVersion, pDest+nRet, pProject, bSetID);
		AttachScroll(pScroll);
	}

	memcpy(&m_nIndent, pDest+nRet, sizeof(int));
	nRet += sizeof(int);
	memcpy(&m_bUseFocus, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);

	Create((int)m_rtLocalRect.GetWidth(), (int)m_rtLocalRect.GetHeight());
	CheckScrollData();
	CheckVisibleItem();

	if(IsTool()) MakeSample();

	return nRet;
}

bool CTextList::SaveInheritanceXml(TiXmlNode *pXmlObject)
{
	__super::SaveInheritanceXml(pXmlObject);

	CString strTemp;

	TiXmlElement xmlInheritance("Inheritance");
	xmlInheritance.SetAttribute("path",  m_pBar?m_pBar->GetFilePath().c_str():"");
	xmlInheritance.SetAttribute("index",  m_nBarIndex);
	strTemp.Format("%06X", m_clrFocusText);
	xmlInheritance.SetAttribute("focustext",  strTemp.GetString());
	strTemp.Format("%06X", m_clrSelectText);
	xmlInheritance.SetAttribute("selecttext",  strTemp.GetString());
	strTemp.Format("%06X", m_clrFocusBack);
	xmlInheritance.SetAttribute("focusback",  strTemp.GetString());
	strTemp.Format("%06X", m_clrSelectBack);
	xmlInheritance.SetAttribute("selectback",  strTemp.GetString());
	xmlInheritance.SetAttribute("increment",  m_bIncrement?1:0);
	xmlInheritance.SetAttribute("autosort",  m_bAutoSort?1:0);
	xmlInheritance.SetAttribute("usesprite",  m_bUseSprite?1:0);
	xmlInheritance.SetAttribute("linescroll",  m_bLineScroll?1:0);
	xmlInheritance.SetDoubleAttribute("lineheight",  m_nLineHeight);
	xmlInheritance.SetAttribute("indent", m_nIndent);
	xmlInheritance.SetAttribute("usefocus", m_bUseFocus?1:0);
	pXmlObject->InsertEndChild(xmlInheritance);

	m_cFontInfo.SaveInheritanceXml(pXmlObject);

	TiXmlElement xmlAttach("attach");
	if(m_pImage) m_pImage->SaveXml(&xmlAttach);
	if(m_pScrollBar) m_pScrollBar->SaveXml(&xmlAttach);
	pXmlObject->InsertEndChild(xmlAttach);

	return true;
}

bool CTextList::LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject)
{
	__super::LoadInheritanceXml(nVersion, pXmlObject, pProject);

	std::string strTemp;

	TiXmlElement *pXmlInheritance = pXmlObject->FirstChildElement("Inheritance");
	GetAttribute(pXmlInheritance, "path", strTemp);
	GetAttribute(pXmlInheritance, "index", m_nBarIndex);
	GetAttributeColor(pXmlInheritance, "focustext", m_clrFocusText);
	GetAttributeColor(pXmlInheritance, "selecttext", m_clrSelectText);
	GetAttributeColor(pXmlInheritance, "focusback", m_clrFocusBack);
	GetAttributeColor(pXmlInheritance, "selectback", m_clrSelectBack);
	GetAttribute(pXmlInheritance, "increment", m_bIncrement);
	GetAttribute(pXmlInheritance, "autosort", m_bAutoSort);
	GetAttribute(pXmlInheritance, "usesprite", m_bUseSprite);
	GetAttribute(pXmlInheritance, "linescroll", m_bLineScroll);
	GetAttribute(pXmlInheritance, "lineheight", m_nLineHeight);
	if(pXmlInheritance->Attribute("indent")) GetAttribute(pXmlInheritance, "indent", m_nIndent);
	if(pXmlInheritance->Attribute("usefocus")) GetAttribute(pXmlInheritance, "usefocus", m_bUseFocus);

	TiXmlElement *pXmlFont = pXmlObject->FirstChildElement("font");
	m_cFontInfo.LoadInheritanceXml(nVersion, pXmlFont, pProject);

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
		case EOT_IMAGE:
			AttachBackImage((CImage *)pChildObject);
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
	if(strTemp != "") SetBarSprite(strTemp);
	CheckScrollData();
	CheckVisibleItem();

	if(IsTool()) MakeSample();

	return true;
}

CSize CTextList::GetTextSize(HDC &dc, LPCTSTR lpText, const int &nLength)
{
	CSize szSize;
	::GetTextExtentPoint( dc, lpText, nLength, &szSize );

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

void CTextList::Draw(NMBASE::GRAPHICGDI::CPage *pPage)
{
	if(m_bRedraw) CheckRedraw();
	CObjectBase::Draw(pPage);
}

bool CTextList::CheckRedraw(void)
{
	CDC *pDC = m_pBack->GetDC();
	m_bRedraw = false;
	if(pDC == NULL) return false;

	pDC->SelectObject((CFont *)m_cFontInfo.GetFont());
	pDC->SetTextCharacterExtra(m_cFontInfo.m_nSpaceBetweenLetters);
	m_pBack->ClearColor(RGB(0,0,255));

	if(m_nFirstVisible == -1) return false;

	TEXTITEMLISTITER start = m_itFirstVisible;
	TEXTITEMLISTITER end = m_listItems.end();

	CRect rtTextRect = m_pBack->GetLocalRect();
	CRect rtRect = CRect(0,m_fStartY + m_nFirstVisible * m_nLineHeight, rtTextRect.GetWidth(),m_fStartY + m_nFirstVisible * m_nLineHeight);
	CRect rtTemp;

	TEXTITEM *pItem;
	CPoint ptDraw;
	COLORREF clrText;

	for(;start != end;++start)
	{
		pItem = *start;
		rtRect.bottom = rtRect.top + (int)m_nLineHeight;

		if(m_cFontInfo.m_uFormat & DT_CENTER)
		{
			ptDraw.x = (rtTextRect.GetWidth() - pItem->m_szSize.cx)/2.0f;
		}
		else if(m_cFontInfo.m_uFormat & DT_RIGHT)
		{
			ptDraw.x = rtTextRect.GetWidth() - pItem->m_szSize.cx - (float)m_nIndent;
		}
		else
		{
			ptDraw.x = (float)m_nIndent;
		}

		ptDraw.y = rtRect.top;

		if(m_cFontInfo.m_uFormat & DT_VCENTER)
		{
			ptDraw.y += ((float)m_nLineHeight - pItem->m_szSize.cy)/2.0f;
		}
		else if(m_cFontInfo.m_uFormat & DT_BOTTOM)
		{
			ptDraw.y += ((float)m_nLineHeight - pItem->m_szSize.cy);
		}

		if(m_bUseFocus && (pItem == m_pFocusItem))
		{
			clrText = m_clrFocusText;
		}
		else if(pItem == m_pSelectItem)
		{
			clrText = m_clrSelectText;
		}
		else
		{
			clrText = pItem->m_clrColor;
		}

		if(m_bUseSprite)
		{
			if(m_pBar)
			{
				if(pItem == m_pFocusItem)
				{
					m_pBack->GetPage()->PutSprAuto((int)rtRect.left, (int)rtRect.top, m_pBar, m_nBarIndex);
				}
				else if(pItem == m_pSelectItem)
				{
					m_pBack->GetPage()->PutSprAuto((int)rtRect.left, (int)rtRect.top, m_pBar, m_nBarIndex+1);
				}
			}
		}
		else
		{
			CBrush brush;
			float fOldRight = rtRect.right;
			if(m_bUseFocus && (pItem == m_pFocusItem))
			{
				brush.CreateSolidBrush(m_clrFocusBack);
				pDC->FillRect(&rtRect.GetRect(), &brush);
				brush.DeleteObject();
			}
			else if(pItem == m_pSelectItem)
			{
				brush.CreateSolidBrush(m_clrSelectBack);
				pDC->FillRect(&rtRect.GetRect(), &brush);
				brush.DeleteObject();
			}
			rtRect.right = fOldRight;
		}

		switch(m_cFontInfo.m_nDrawType)
		{
		case ETDT_NORMAL:
			pDC->SetTextColor(clrText);
			pDC->TextOut((int)ptDraw.x, (int)ptDraw.y, pItem->m_strOutput.c_str());
			break;
		case ETDT_SHADOW:
			pDC->SetTextColor(m_cFontInfo.m_clrOutline);
			pDC->TextOut((int)ptDraw.x+1, (int)ptDraw.y+1, pItem->m_strOutput.c_str());
			pDC->SetTextColor(clrText);
			pDC->TextOut((int)ptDraw.x, (int)ptDraw.y, pItem->m_strOutput.c_str());
			break;
		case ETDT_OUTLINE:
			pDC->SetTextColor(m_cFontInfo.m_clrOutline);
			pDC->TextOut((int)ptDraw.x+1, (int)ptDraw.y, pItem->m_strOutput.c_str());
			pDC->TextOut((int)ptDraw.x+1, (int)ptDraw.y+2, pItem->m_strOutput.c_str());
			pDC->TextOut((int)ptDraw.x, (int)ptDraw.y+1, pItem->m_strOutput.c_str());
			pDC->TextOut((int)ptDraw.x+2, (int)ptDraw.y+1, pItem->m_strOutput.c_str());
			pDC->SetTextColor(clrText);
			pDC->TextOut((int)ptDraw.x+1, (int)ptDraw.y+1, pItem->m_strOutput.c_str());
			break;
		}

		rtRect.top = rtRect.bottom;
		if(start == m_itLastVisible) break;
	}

	return true;
}

void CTextList::CheckVisibleItem(void)
{
	CRect rtTextRect = m_pBack->GetLocalRect();
	CRect rtRect = CRect(0,m_fStartY,rtTextRect.GetWidth(),m_fStartY);
	CRect rtTemp;

	TEXTITEM *pItem;
	CPoint ptDraw;
	m_pFirstVisibleItem = NULL;
	m_pLastVisibleItem = NULL;
	m_nFirstVisible = -1;
	m_nLastVisible = -1;
	
	TEXTITEMLISTITER start = m_listItems.begin();
	TEXTITEMLISTITER end = m_listItems.end();

	m_itFirstVisible = end;
	m_itLastVisible = end;

	for(int i=0;start != end;++start,++i)
	{
		pItem = *start;
		rtRect.bottom = rtRect.top + (int)m_nLineHeight;

		if(rtTemp.IntersectRect(&rtRect, &rtTextRect))
		{
			if(m_pFirstVisibleItem == NULL)
			{
				m_pFirstVisibleItem = pItem;
				m_nFirstVisible = i;
				m_itFirstVisible = start;
				if(m_bLineScroll && (m_fStartY != (GetHeight() - m_fTotalHeight)))
				{
					m_fStartY = -pItem->m_fStartY;
					rtRect.top = 0;
					rtRect.bottom = rtRect.top + (float)m_nLineHeight;
				}
			}

			m_itLastVisible = start;
			m_nLastVisible = i;
			m_pLastVisibleItem = pItem;
		}

		rtRect.top = rtRect.bottom;
	}
}

int CTextList::AddText(const std::string &strText)
{
	TEXTITEM *pItemLast = NULL;
	if(m_listItems.size())
	{
		TEXTITEMLIST::reverse_iterator start = m_listItems.rbegin();
		pItemLast = *start;
	}

	TEXTITEM *pItem = new TEXTITEM;
	pItem->m_strOutput = strText;
	pItem->m_clrColor = m_cFontInfo.m_clrColor;

	HDC dc = GetDC(NULL);
	HGDIOBJ hOldFont = ::SelectObject( dc, m_cFontInfo.GetFont()->m_hObject );
	int nOldSpaceBetweenLetters = ::SetTextCharacterExtra( dc, m_cFontInfo.m_nSpaceBetweenLetters);

	pItem->m_szSize = GetTextSize(dc, strText.c_str(), strText.length());

	if(pItemLast) pItem->m_fStartY = pItemLast->m_fStartY + m_nLineHeight;
	else pItem->m_fStartY = 0.0f;

	m_fTotalHeight += (float)m_nLineHeight;

	m_listItems.push_back(pItem);
	int nIndex = (int)m_listItems.size() - 1;
	if(m_bAutoSort)
		nIndex = Sort(pItem);

	//스크롤 바가 있으면 최대/최소 현재값을 다시 셋팅해준다.
	if(m_pScrollBar)
	{
		m_pScrollBar->SetRangeValue(0.0f, m_fTotalHeight - GetHeight());

		//스크롤 바를 보이거나 보이지 않게 설정한다.
		if(m_fTotalHeight > GetHeight())
		{
			if(m_pScrollBar->IsShow() == false)
			{
				m_pScrollBar->SetShow(true);
			}
		}
		else
		{
			if(m_pScrollBar->IsShow() == true)
			{
				m_pScrollBar->SetShow(false);
			}
		}
	}

	//화면에 보이는 영역에 아이템이 추가되었을 경우 다시 그리기 해준다.
	if(IsVisualItem(pItem))
	{
		CheckVisibleItem();
		SetRedraw();
	}

	::SelectObject( dc, hOldFont );
	::SetTextCharacterExtra( dc, nOldSpaceBetweenLetters);

	ReleaseDC(NULL, dc);

	CheckScrollData();

	return nIndex;
}

int CTextList::InsertText(const int &nIndex, const std::string &strText)
{
	int nSize = (int)m_listItems.size();
	if(nIndex == -1 || nSize == 0 || nSize < nIndex)
	{
		return AddText(strText);
	}

	TEXTITEMLIST::iterator findIt = m_listItems.begin();
	TEXTITEMLIST::iterator end = m_listItems.end();

	TEXTITEM *pItemLast = NULL;
	for(int i=0;findIt != end;++i,++findIt)
	{
		if(i == nIndex) break;
		pItemLast = *findIt;
	}

	TEXTITEM *pNewItem = new TEXTITEM;
	pNewItem->m_strOutput = strText;
	pNewItem->m_clrColor = m_cFontInfo.m_clrColor;

	HDC dc = GetDC(NULL);
	HGDIOBJ hOldFont = ::SelectObject( dc, m_cFontInfo.GetFont()->m_hObject );
	int nOldSpaceBetweenLetters = ::SetTextCharacterExtra( dc, m_cFontInfo.m_nSpaceBetweenLetters);

	pNewItem->m_szSize = GetTextSize(dc, strText.c_str(), strText.length());

	if(pItemLast) pNewItem->m_fStartY = pItemLast->m_fStartY + m_nLineHeight;
	else pNewItem->m_fStartY = 0.0f;

	m_fTotalHeight += (float)m_nLineHeight;

	m_listItems.insert(findIt, pNewItem);
	pItemLast = pNewItem;

	for(;findIt != end;++findIt)
	{
		(*findIt)->m_fStartY = pItemLast->m_fStartY + m_nLineHeight;
		pItemLast = *findIt;
	}

	int nRetIndex = nIndex;

	if(m_bAutoSort)
		nRetIndex = Sort(pNewItem);

	//스크롤 바가 있으면 최대/최소 현재값을 다시 셋팅해준다.
	if(m_pScrollBar)
	{
		m_pScrollBar->SetRangeValue(0.0f, m_fTotalHeight - GetHeight());

		//스크롤 바를 보이거나 보이지 않게 설정한다.
		if(m_fTotalHeight > GetHeight())
		{
			if(m_pScrollBar->IsShow() == false)
			{
				m_pScrollBar->SetShow(true);
			}
		}
		else
		{
			if(m_pScrollBar->IsShow() == true)
			{
				m_pScrollBar->SetShow(false);
			}
		}
	}

	//화면에 보이는 영역에 아이템이 추가되었을 경우 다시 그리기 해준다.
	CheckScrollData();
	CheckVisibleItem();
	SetRedraw();

	::SelectObject( dc, hOldFont );
	::SetTextCharacterExtra( dc, nOldSpaceBetweenLetters);

	ReleaseDC(NULL, dc);

	return nIndex;
}

const int &CTextList::GetCurSelectItem(void)
{
	return m_nSelectItem;
}

void CTextList::SetCurSelectItem(const int &nSel)
{
	TEXTITEM *pOldItem = m_pSelectItem;
	bool bRedraw = IsVisualItem(pOldItem);

	if(nSel == -1)
	{
		m_pSelectItem = NULL;
		m_nSelectItem = -1;
	}
	else
	{
		TEXTITEMLISTITER start = m_listItems.begin();
		TEXTITEMLISTITER end = m_listItems.end();
		for(int i=0;start != end;++start,++i)
		{
			if(i == nSel)
			{
				m_pSelectItem = *start;
				m_nSelectItem = i;
				bRedraw = bRedraw || IsVisualItem(m_pSelectItem);
				break;
			}
		}
	}

	//선택색이 투명색이 아니어야 함
	bRedraw = bRedraw && ((m_clrSelectBack != RGB(0,0,255)) || (m_cFontInfo.m_clrColor != m_clrSelectText));

	if(bRedraw)
	{
		SetRedraw();
	}

	return;
}

int CTextList::GetItemIndex(TEXTITEM *pItem)
{
	if(pItem == NULL) return -1;
	TEXTITEMLISTITER start = m_listItems.begin();
	TEXTITEMLISTITER end = m_listItems.end();
	for(int i=0;start != end;++start,++i)
	{
		if(pItem == *start)
		{
			return i;
		}
	}

	return -1;
}

CTextList::TEXTITEM *CTextList::GetItem(const int &nIndex)
{
	if(nIndex == -1) return NULL;
	TEXTITEMLISTITER start = m_listItems.begin();
	TEXTITEMLISTITER end = m_listItems.end();
	for(int i=0;start != end;++start,++i)
	{
		if(i == nIndex)
		{
			return *start;
		}
	}

	return NULL;
}

const int CTextList::HitTest(const CPoint &ptPos)
{
	CPoint ptLocal = GetGlobalToLocalPos(ptPos);

	TEXTITEMLISTITER start = m_itFirstVisible;
	TEXTITEMLISTITER end = m_listItems.end();
	for(int i=m_nFirstVisible;start != end;++start,++i)
	{
		if(IsInItem(*start, ptLocal))
		{
			return i;
		}

		if(start == m_itLastVisible) break;
	}
	return -1;
}

const bool CTextList::IsVisualItem(TEXTITEM *pItem)
{
	if(pItem == NULL) return false;
	
	CRect rtRect(0, m_fStartY + pItem->m_fStartY, GetWidth(), m_fStartY + pItem->m_fStartY + (float)m_nLineHeight);
	CRect rtTemp;
	if(rtTemp.IntersectRect(&rtRect, &m_pBack->GetLocalRect()))
	{
		return true;
	}
	return false;
}

const bool CTextList::IsVisualItem(const int &nIndex)
{
	TEXTITEM *pItem = GetItem(nIndex);
	return IsVisualItem(pItem);
}

bool CTextList::IsInItem(TEXTITEM *pItem, const CPoint &ptPos)
{
	if(pItem == NULL) return false;
	CRect rtRect(0, m_fStartY + pItem->m_fStartY, GetWidth(), m_fStartY + pItem->m_fStartY + (float)m_nLineHeight);
	return rtRect.PtInRect(ptPos);
}

void CTextList::SetTotalHeightUseLineHeight(int nTotalLine)
{
	ASSERT(m_nLineHeight > 0);
	int nTotalHeight = nTotalLine * m_nLineHeight;
	Create((int)m_rtLocalRect.GetWidth(), nTotalHeight);
	if(m_pImage)
		m_pImage->SetMiddleCount(nTotalLine);
}

void CTextList::OnMouseLeave(void)
{
	if(m_pFocusItem)
	{
		m_pFocusItem = NULL;
		m_nFocusItem = -1;

		if(m_bUseFocus)
		{
			if((m_clrFocusBack != RGB(0,0,255)) || (m_clrFocusBack != m_clrSelectBack) || (m_clrFocusText != m_cFontInfo.m_clrColor) || (m_clrSelectText != m_clrFocusText))
				SetRedraw();
		}
	}
	CObjectBase::OnMouseLeave();
}

bool CTextList::OnMouseMove(const CPoint &ptPos)
{
	if(m_pFirstVisibleItem && m_pLastVisibleItem)
	{
		TEXTITEM *pOldItem = m_pFocusItem;

		const int nHitIndex = HitTest(ptPos);
		if(nHitIndex != -1)
		{
			m_pFocusItem = GetItem(nHitIndex);
			m_nFocusItem = nHitIndex;
		}
		else
		{
			m_pFocusItem = NULL;
			m_nFocusItem = -1;
		}

		if(m_bUseFocus)
		{
			if((pOldItem != m_pFocusItem) && ((m_clrFocusBack != RGB(0,0,255)) || (m_clrFocusText != m_cFontInfo.m_clrColor)))
			{
				SetRedraw();
			}
		}
	}
	return CObjectBase::OnMouseMove(ptPos);
}

void CTextList::OnMouseLClick(const CPoint &ptPos)
{
	if(m_pFirstVisibleItem && m_pLastVisibleItem)
	{
		TEXTITEM *pOldItem = m_pSelectItem;

		const int &nHitIndex = HitTest(ptPos);

		if(nHitIndex != -1)
		{
			m_pSelectItem = GetItem(nHitIndex);
			m_nSelectItem = nHitIndex;
		}

		if(pOldItem != m_pSelectItem)
		{
			CMO_SelChange msg(this, nHitIndex);
			Dispatch(&msg);

			if(m_clrSelectBack != RGB(0,0,255) || (m_clrSelectText != m_cFontInfo.m_clrColor)) 
				SetRedraw();
		}
	}
	CObjectBase::OnMouseLClick(ptPos);
}

void CTextList::SetFont(const std::string &strName, const int &nSize, const bool &bBold)
{
	if(m_cFontInfo.GetName() == strName && m_cFontInfo.GetSize() == nSize && m_cFontInfo.IsBold() == bBold) return;
	m_cFontInfo.SetFont(strName, nSize, bBold);
	CheckItemResize();
}

const CFont *CTextList::GetFont(void)
{
	return m_cFontInfo.GetFont();
}

const std::string &CTextList::GetFontName(void)
{
	return m_cFontInfo.GetName();
}

const int &CTextList::GetFontSize(void)
{
	return m_cFontInfo.GetSize();
}

const bool &CTextList::IsBold(void)
{
	return m_cFontInfo.IsBold();
}

void CTextList::SetTextNormalColor(const COLORREF &clrNormal)
{
	if(m_cFontInfo.m_clrColor == clrNormal) return;
	m_cFontInfo.m_clrColor = clrNormal;
	if(IsTool())
	{
		int nSelect = GetCurSelectItem();
		MakeSample();
		SetCurSelectItem(nSelect);
	}
	SetRedraw();
}

void CTextList::SetTextFocusColor(const COLORREF &clrFocus)
{
	if(m_clrFocusText == clrFocus) return;
	m_clrFocusText = clrFocus;
	if(IsVisualItem(m_pFocusItem)) SetRedraw();
}

void CTextList::SetTextSelectColor(const COLORREF &clrSelect)
{
	if(m_clrSelectText == clrSelect) return;
	m_clrSelectText = clrSelect;
	if(IsVisualItem(m_pSelectItem)) SetRedraw();
}

void CTextList::SetTextOutlineColor(const COLORREF &clrOutline)
{
	if(m_cFontInfo.m_clrOutline == clrOutline) return;
	m_cFontInfo.m_clrOutline = clrOutline;
	switch(m_cFontInfo.m_nDrawType)
	{
	case ETDT_OUTLINE:
	case ETDT_SHADOW:
		SetRedraw();
		break;
	}
}

const COLORREF &CTextList::GetTextNormalColor(void)
{
	return m_cFontInfo.m_clrColor;
}

const COLORREF &CTextList::GetTextFocusColor(void)
{
	return m_clrFocusText;
}

const COLORREF &CTextList::GetTextSelectColor(void)
{
	return m_clrSelectText;
}

const COLORREF &CTextList::GetTextOutlineColor(void)
{
	return m_cFontInfo.m_clrOutline;
}

void CTextList::SetDrawType(const ENUM_TEXTDRAW_TYPE &nType)
{
	if(m_cFontInfo.m_nDrawType == nType) return;
	m_cFontInfo.m_nDrawType = nType;
	SetRedraw();
}

void CTextList::SetFormat(const UINT &uFormat)
{
	if(m_cFontInfo.m_uFormat == uFormat) return;
	m_cFontInfo.m_uFormat = uFormat;
	SetRedraw();
}

void CTextList::SetSpaceBetweenLetters(const int &nSpaceBetweenLetters)
{
	if(m_cFontInfo.m_nSpaceBetweenLetters == nSpaceBetweenLetters) return;
	m_cFontInfo.m_nSpaceBetweenLetters = nSpaceBetweenLetters;
	SetRedraw();
}

void CTextList::SetIndent(const int &nIndent)
{
	if(m_nIndent == nIndent) return;
	m_nIndent = nIndent;
	SetRedraw();
}

const ENUM_TEXTDRAW_TYPE &CTextList::GetDrawType(void)
{
	return m_cFontInfo.m_nDrawType;
}

const UINT &CTextList::GetFormat(void)
{
	return m_cFontInfo.m_uFormat;
}

const int &CTextList::GetSpaceBetweenLetters(void)
{
	return m_cFontInfo.m_nSpaceBetweenLetters;
}

void CTextList::CheckItemResize(void)
{
	HDC dc = GetDC(NULL);
	HGDIOBJ hOldFont = ::SelectObject( dc, m_cFontInfo.GetFont()->m_hObject );
	int nOldSpaceBetweenLetters = ::SetTextCharacterExtra( dc, m_cFontInfo.m_nSpaceBetweenLetters);

	TEXTITEMLISTITER start = m_listItems.begin();
	TEXTITEMLISTITER end = m_listItems.end();

	for(;start != end;++start)
	{
		(*start)->m_szSize = GetTextSize(dc, (*start)->m_strOutput.c_str(), (int)(*start)->m_strOutput.length());
	}

	::SelectObject( dc, hOldFont );
	::SetTextCharacterExtra( dc, nOldSpaceBetweenLetters);

	ReleaseDC(NULL, dc);

	SetRedraw();
}

void CTextList::SetBackFocusColor(const COLORREF &clrFocus)
{
	if(m_clrFocusBack == clrFocus) return;
	m_clrFocusBack = clrFocus;
	if(!m_bUseSprite && IsVisualItem(m_pFocusItem)) SetRedraw();
}

void CTextList::SetBackSelectColor(const COLORREF &clrSelect)
{
	if(m_clrSelectBack == clrSelect) return;
	m_clrSelectBack = clrSelect;
	if(!m_bUseSprite && IsVisualItem(m_pSelectItem)) SetRedraw();
}

const COLORREF &CTextList::GetBackFocusColor(void)
{
	return m_clrFocusBack;
}

const COLORREF &CTextList::GetBackSelectColor(void)
{
	return m_clrSelectBack;
}

void CTextList::SetLineHeight(const int &nLineHeight)
{
	if(m_nLineHeight == nLineHeight) return;
	m_nLineHeight = nLineHeight;

	m_fTotalHeight = 0.0f;
	TEXTITEMLISTITER start = m_listItems.begin();
	TEXTITEMLISTITER end = m_listItems.end();

	for(;start != end;++start)
	{
		(*start)->m_fStartY = m_fTotalHeight;
		m_fTotalHeight += (float)m_nLineHeight;
	}

	m_fStartY = 0.0f;
	if(m_pScrollBar)
	{
		m_pScrollBar->SetChangeValue((float)m_nLineHeight);
		m_pScrollBar->SetRangeValue(0, m_fTotalHeight - GetHeight());
		m_pScrollBar->SetCurValue(0);
	}

	CheckVisibleItem();
	SetRedraw();
}

const int &CTextList::GetLineHeight(void)
{
	return m_nLineHeight;
}

bool CTextList::SetBarSprite(const std::string &strPath)
{
	if(m_pBar && m_pBar->GetFilePath() == strPath) return true;
	if(m_pBar)
	{
		RM().RemoveSprite(m_pBar);
		m_pBar = NULL;
	}

	m_pBar = RM().GetSprite(strPath.c_str());
	if(m_pBar == NULL)
		return false;

	if(m_bUseSprite && (IsVisualItem(m_pFocusItem) || IsVisualItem(m_pSelectItem))) SetRedraw();
	return true;
}

const std::string CTextList::GetBarSprite(void)
{
	if(m_pBar == NULL) return "";
	return m_pBar->GetFilePath();
}

const int CTextList::GetTotalBarNum(void)
{
	return m_pBar?m_pBar->GetTotalSpr():0;
}

void CTextList::SetBarIndex(const int &nIndex)
{
	if(m_nBarIndex == nIndex) return;

	m_nBarIndex = nIndex;
	if(m_bUseSprite && (IsVisualItem(m_pFocusItem) || IsVisualItem(m_pSelectItem))) SetRedraw();
}

void CTextList::SetUseBarSprite(const bool &bUse)
{
	if(m_bUseSprite = bUse) return;
	m_bUseSprite = bUse;
	if(IsVisualItem(m_pFocusItem) || IsVisualItem(m_pSelectItem)) SetRedraw();
}

const bool &CTextList::IsUseBarSprite(void)
{
	return m_bUseSprite;
}

LRESULT CTextList::OnLocalChildNotify(CMessage *msg)
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
			return OnFocusInLocalChild(msg);
		}
	}
	return CObjectBase::OnLocalChildNotify(msg);
}

void CTextList::AttachScroll(CScrollBar *pScall)
{
	ASSERT(m_pScrollBar == NULL && pScall != NULL);
	m_pScrollBar = pScall;
	AddLocalChild(m_pScrollBar);

	if(m_pScrollBar) m_pScrollBar->SetChangeValue((float)m_nLineHeight);

	SetUseMouseWheel(true);

	CheckScrollData();
}

void CTextList::DettachScroll(void)
{
	ASSERT(m_pScrollBar != NULL);
	SubChild(m_pScrollBar);

	SetUseMouseWheel(false);

	m_fStartY = 0.0f;
	m_pScrollBar = NULL;
	SetRedraw();
}

void CTextList::AttachBackImage(CImage *pImage)
{
	ASSERT(m_pImage == NULL && pImage != NULL);
	m_pImage = pImage;
	AddLocalChild(m_pImage);
	AddLocalChild(m_pBack);
	if(m_pScrollBar) AddLocalChild(m_pScrollBar);

	m_pImage->SetMessage(true);
}

void CTextList::DettachBackImage(void)
{
	ASSERT(m_pImage != NULL);
	SubChild(m_pImage);

	m_pImage->SetMessage(false);
	m_pImage = NULL;
}

void CTextList::CheckScrollData(void)
{
	if(m_fTotalHeight <= GetHeight())
	{
		m_fStartY = 0.0f;
		if(m_pScrollBar)
		{
			m_pScrollBar->SetCurValue(0);
			m_pScrollBar->SetEnable(false);
		}
	}
	else
	{
		if(m_fTotalHeight - GetHeight() < (m_fStartY * -1.0f))
		{
			m_fStartY = m_fTotalHeight - GetHeight();
		}	

		if(m_pScrollBar)
		{
			m_pScrollBar->SetRangeValue(0, m_fTotalHeight-GetHeight());
			m_pScrollBar->SetCurValue(-m_fStartY);
			m_pScrollBar->SetEnable(true);
		}
	}
}

void CTextList::CheckUp(void)
{
	if(m_fStartY == 0) return;

	if(m_pFirstVisibleItem)
	{
		if(m_pFirstVisibleItem->m_fStartY != -m_fStartY)
		{
			m_fStartY = -m_pFirstVisibleItem->m_fStartY;
			m_pScrollBar->SetCurValue(-m_fStartY);
		}
		else
		{
			TEXTITEMLISTITER findIt = find(m_listItems.begin(), m_listItems.end(), m_pFirstVisibleItem);
			if(findIt != m_listItems.end())
			{
				if(findIt == m_listItems.begin())
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

void CTextList::CheckDown(void)
{
	if((m_fTotalHeight + m_fStartY) <= GetHeight()) return;

	if(m_pFirstVisibleItem)
	{
		TEXTITEMLISTITER findIt = find(m_listItems.begin(), m_listItems.end(), m_pFirstVisibleItem);
		if(findIt != m_listItems.end())
		{
			++findIt;
			if(findIt == m_listItems.end())
			{
				m_fStartY = -(m_fTotalHeight - GetHeight());
				m_pScrollBar->SetCurValue(-m_fStartY);
			}
			else
			{
				if((m_fTotalHeight - m_pFirstVisibleItem->m_fStartY) <= GetHeight())
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
}

LRESULT CTextList::OnClickUp(CMessage *pMsg)
{
	if(!m_bLineScroll) return FALSE;
	if(m_pScrollBar->IsIncrease()) CheckDown();
	else CheckUp();
	CheckVisibleItem();
	CheckFocusItem();
	return TRUE;
}

LRESULT CTextList::OnClickDown(CMessage *pMsg)
{
	if(!m_bLineScroll) return FALSE;
	if(m_pScrollBar->IsIncrease()) CheckUp();
	else CheckDown();
	CheckVisibleItem();
	CheckFocusItem();
	return TRUE;
}

void CTextList::CheckFocusItem(void)
{
	if(m_pFocusItem && m_pProject && m_pProject->GetObjectMan())
	{
		CObjectMan *pManager = m_pProject->GetObjectMan();
		if(pManager->GetWnd())
		{
			POINT ptPos;
			GetCursorPos(&ptPos);

			pManager->GetWnd()->ScreenToClient(&ptPos);
			CPoint ptTemp((float)ptPos.x, (float)ptPos.y);
			const int &nHitIndex = HitTest(ptTemp);
			m_pFocusItem = GetItem(nHitIndex);
			m_nFocusItem = nHitIndex;
		}
	}
}

void CTextList::CheckPageUp(void)
{
	if(m_fStartY == 0) return;

	if(m_pFirstVisibleItem)
	{
		m_fStartY += GetHeight();
		if(m_fStartY > 0) m_fStartY = 0;
		m_pScrollBar->SetCurValue(-m_fStartY);
		SetRedraw();
	}
}

void CTextList::CheckPageDown(void)
{
	if((m_fTotalHeight + m_fStartY) <= GetHeight()) return;

	if(m_pFirstVisibleItem)
	{
		m_fStartY -= GetHeight();
		if(GetHeight() - m_fTotalHeight > m_fStartY) m_fStartY = GetHeight() - m_fTotalHeight;
		m_pScrollBar->SetCurValue(-m_fStartY);
		SetRedraw();
	}
}

LRESULT CTextList::OnClickPageUp(CMessage *pMsg)
{
	if(!m_bLineScroll) return FALSE;
	if(m_pScrollBar->IsIncrease()) CheckPageDown();
	else CheckPageUp();
	CheckVisibleItem();
	return TRUE;
}

LRESULT CTextList::OnClickPageDown(CMessage *pMsg)
{
	if(!m_bLineScroll) return FALSE;
	if(m_pScrollBar->IsIncrease()) CheckPageUp();
	else CheckPageDown();
	CheckVisibleItem();
	return TRUE;
}

LRESULT CTextList::OnClickValueChange(CMessage *pMsg)
{
	CMO_CurValueChange *msg = (CMO_CurValueChange *)pMsg;
	float fCurValue = msg->GetValue();
	m_fStartY = -fCurValue;
	CheckVisibleItem();
	SetRedraw();
	return TRUE;
}

LRESULT CTextList::OnFocusInLocalChild(CMessage *pMsg)
{
	return (LRESULT)this;
}

bool CTextList::OnMouseWheel(const int &nKey, const short &zDelta)
{
	if(m_pScrollBar) m_pScrollBar->OnMouseWheel(nKey, zDelta);
	return CObjectBase::OnMouseWheel(nKey, zDelta);
}

void CTextList::SetItemData(const int &nIndex, const LPARAM &lParam)
{
	TEXTITEM *pItem = GetItem(nIndex);
	if(pItem) pItem->m_lParam = lParam;
}

const LPARAM CTextList::GetItemData(const int nIndex)
{
	TEXTITEM *pItem = GetItem(nIndex);
	return pItem?pItem->m_lParam:NULL;
}

int CTextList::FirstFind(const LPARAM &lUserData)
{
	CFindFuntion *pFindFuntion = GetFindFunction(m_nFindType);
	if(pFindFuntion == NULL)
	{
		assert(!"찾기 관련 함수를 찾지 못했습니다.");
	}

	TEXTITEMLISTITER start = m_listItems.begin();
	TEXTITEMLISTITER end = m_listItems.end();
	m_nFindItem = -1;

	for(int i=0;start != end;++start,++i)
	{
		if(pFindFuntion->m_pFindFunction(*start, lUserData, pFindFuntion->m_lParam))
		{
			m_nFindItem = i;
			break;
		}
	}

	return m_nFindItem;
}

int CTextList::NextFind(const LPARAM &lUserData)
{
	CFindFuntion *pFindFuntion = GetFindFunction(m_nFindType);
	if(pFindFuntion == NULL)
	{
		assert(!"찾기 관련 함수를 찾지 못했습니다.");
	}

	TEXTITEMLISTITER start = m_itFind;
	TEXTITEMLISTITER end = m_listItems.end();

	if(start == m_itFind)
	{
		m_nFindItem = -1;
		return m_nFindItem;
	}
	else ++start;

	if(start == m_itFind)
	{
		m_nFindItem = -1;
		return m_nFindItem;
	}

	for(int i=m_nFindItem+1;start != end;++start,++i)
	{
		if(m_nFindItem < i && pFindFuntion->m_pFindFunction(*start, lUserData, pFindFuntion->m_lParam))
		{
			m_nFindItem = i;
			return m_nFindItem;
		}
	}

	m_nFindItem = -1;
	return m_nFindItem;
}

bool CTextList::RemoveText(const std::string &strText)
{
	TEXTITEMLISTITER start = m_listItems.begin();
	TEXTITEMLISTITER end = m_listItems.end();

	bool bScrollData = false;
	TEXTITEM *pTextItem = NULL;
	TEXTITEM *pLastTextItem = NULL;

	bool bRedraw = false;

	for(int i=0;start != end;++i)
	{
		pTextItem = *start;
		if((*start)->m_strOutput == strText)
		{
			delete *start;
			start = m_listItems.erase(start);
			pTextItem = NULL;
			
			m_fTotalHeight -= (float)m_nLineHeight;
			if(m_nFirstVisible < i)
			{
				m_fStartY += m_nLineHeight;
			}

			if(m_nLastVisible >= i)
			{
				bRedraw = true;
			}

			if(m_nSelectItem > i)
			{
				--m_nSelectItem;
			}
			else if(m_nSelectItem == i)
			{
				m_nSelectItem = -1;
				m_pSelectItem = NULL;
			}

			if(m_nFocusItem >= i)
			{
				CheckFocusItem();
			}

			bScrollData = true;
		}
		else
		{
			if(bScrollData)
			{
				if(pLastTextItem) pTextItem->m_fStartY = pLastTextItem->m_fStartY + (float)m_nLineHeight;
				else pTextItem->m_fStartY = 0.0f;
			}
			++start;
		}

		if(pTextItem) pLastTextItem = pTextItem;
	}

	if(bRedraw)
	{
		CheckVisibleItem();
		SetRedraw();
	}

	if(bScrollData)
		CheckScrollData();

	return bScrollData;
}

bool CTextList::RemoveText(const int &nIndex)
{
	TEXTITEMLISTITER start = m_listItems.begin();
	TEXTITEMLISTITER end = m_listItems.end();

	bool bScrollData = false;
	TEXTITEM *pTextItem = NULL;
	TEXTITEM *pLastTextItem = NULL;

	bool bRedraw = false;

	for(int i=0;start != end;++i)
	{
		pTextItem = *start;
		if(i == nIndex)
		{
			m_fTotalHeight -= (float)m_nLineHeight;
			if(m_itFirstVisible == start)
			{
				++m_itFirstVisible;
			}

			if(m_itLastVisible == start)
			{
				++m_itLastVisible;
			}

			if(m_nFirstVisible < i)
			{
				m_fStartY += m_nLineHeight;
			}

			if(m_nLastVisible >= i)
			{
				bRedraw = true;
			}

			if(m_nSelectItem > i)
			{
				--m_nSelectItem;
			}
			else if(m_nSelectItem == i)
			{
				m_nSelectItem = -1;
				m_pSelectItem = NULL;
			}

			if(m_nFocusItem >= i)
			{
				CheckFocusItem();
			}

			delete *start;
			pTextItem = NULL;
			start = m_listItems.erase(start);

			bScrollData = true;
		}
		else
		{
			if(bScrollData)
			{
				if(pLastTextItem) pTextItem->m_fStartY = pLastTextItem->m_fStartY + (float)m_nLineHeight;
				else pTextItem->m_fStartY = 0.0f;
			}
			++start;
		}

		if(pTextItem) pLastTextItem = pTextItem;
	}

	if(bRedraw)
	{
		CheckVisibleItem();
		SetRedraw();
	}

	if(bScrollData)
		CheckScrollData();

	return bScrollData;
}

bool CTextList::RemoveText(const LPARAM &lParam)
{
	TEXTITEMLISTITER start = m_listItems.begin();
	TEXTITEMLISTITER end = m_listItems.end();

	bool bScrollData = false;
	TEXTITEM *pTextItem = NULL;
	TEXTITEM *pLastTextItem = NULL;
	bool bRedraw = false;

	for(int i=0;start != end;++i)
	{
		pTextItem = *start;
		if((*start)->m_lParam == lParam)
		{
			delete *start;
			pTextItem = NULL;

			start = m_listItems.erase(start);

			m_fTotalHeight -= (float)m_nLineHeight;
			if(m_nFirstVisible < i)
			{
				m_fStartY += m_nLineHeight;
			}

			if(m_nLastVisible >= i)
			{
				bRedraw = true;
			}

			if(m_nSelectItem > i)
			{
				--m_nSelectItem;
			}
			else if(m_nSelectItem == i)
			{
				m_nSelectItem = -1;
				m_pSelectItem = NULL;
			}

			if(m_nFocusItem >= i)
			{
				CheckFocusItem();
			}
			bScrollData = true;
		}
		else
		{
			if(bScrollData)
			{
				if(pLastTextItem) pTextItem->m_fStartY = pLastTextItem->m_fStartY + (float)m_nLineHeight;
				else pTextItem->m_fStartY = 0.0f;
			}
			++start;
		}

		if(pTextItem) pLastTextItem = pTextItem;
	}

	if(bRedraw)
	{
		CheckVisibleItem();
		SetRedraw();
	}

	if(bScrollData)
		CheckScrollData();

	return bScrollData;
}

const int CTextList::GetItemCount(void)
{
	return (int)m_listItems.size();
}

const std::string CTextList::GetItemString(const int &nIndex)
{
	TEXTITEM *pItem = GetItem(nIndex);
	if(pItem == NULL) return "";
	return pItem->m_strOutput;
}

void CTextList::SetItemString(const int &nIndex, const std::string &strText)
{
	TEXTITEM *pItem = GetItem(nIndex);
	if(pItem == NULL) return;
	if(pItem->m_strOutput == strText) return;
	pItem->m_strOutput = strText;
	if(IsVisualItem(pItem)) SetRedraw();
}

void CTextList::SetItemColor(const int &nIndex, const COLORREF &clrColor)
{
	TEXTITEM *pItem = GetItem(nIndex);
	if(pItem == NULL) return;
	if(pItem->m_clrColor == clrColor) return;
	pItem->m_clrColor = clrColor;
	if(IsVisualItem(pItem)) SetRedraw();
}

void CTextList::SetVisibleItem(int nIndex, UINT uFormat)
{
	if(nIndex < 0 || nIndex >= (int)m_listItems.size()) return;
	if(GetHeight() >= m_fTotalHeight) return;

	TEXTITEM *pItem = GetItem(nIndex);
	float fYPos = 0.0f;
	if(uFormat & DT_VCENTER)
	{
		fYPos = (GetHeight() - (float)m_nLineHeight)/2.0f;
		//m_fStartY + pItem->m_fStartY;
	}
	else if(uFormat & DT_BOTTOM)
	{
		fYPos = GetHeight() - (float)m_nLineHeight;
	}

	m_fStartY = fYPos - pItem->m_fStartY;
	float fMaxScrollValue = GetHeight() - m_fTotalHeight;
	if(fMaxScrollValue > m_fStartY)
	{
		m_fStartY = fMaxScrollValue;
	}
	else if(m_fStartY > 0) m_fStartY = 0;

	if(m_pScrollBar) m_pScrollBar->SetCurValue(-m_fStartY);
	CheckVisibleItem();
	SetRedraw();
}

void CTextList::SetVisibleItem(int nIndex)
{
	if(nIndex < 0 || nIndex >= (int)m_listItems.size()) return;
	if(IsVisualItem(nIndex)) return;
	if(m_nFirstVisible > nIndex) SetVisibleItem(nIndex, DT_TOP);
	else SetVisibleItem(nIndex, DT_BOTTOM);
}

void CTextList::MakeSample(void)
{
	RemoveAll();

	CString strTemp;
	for(int i=0;i<100;++i)
	{
		strTemp.Format("테스트%02d",i);
		AddText(strTemp.GetString());
	}
}

void CTextList::SetUseFocus(const bool &bUseFocus)
{
	if(m_bUseFocus == bUseFocus) return;
	m_bUseFocus = bUseFocus;
	if(m_pFocusItem) SetRedraw();
}
} //namespace LSY