#include "stdafx.h"
#include "List.h"
#include "ListItem.h"
#include "../Control/ScrollBar.h"
#include "../../Functor/Message.h"
#include "../ObjectMan.h"
#include "../Base/DumyImage.h"
#include "../../tinyxml/tinyxml.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DEF_CHECK_REPOSITION if(m_bReposition) RepositionItem()

namespace LSY
{
//기본 비교 함수
bool CList_DefaultCompareFunction(CListItemData *pItem1, CListItemData *pItem2, bool bIncrement, LPARAM lParam)
{
	if (bIncrement)
		return strcmp(pItem1->GetText().c_str(), pItem2->GetText().c_str()) < 0;
	else
		return strcmp(pItem1->GetText().c_str(), pItem2->GetText().c_str()) > 0;
}

//기본 찾기 함수, lUserData에 문자열의 포인터를 넘겨주면 된다.
bool CList_DefaultFindFunction(CListItemData *pItem, LPARAM lUserData, LPARAM lParam)
{
	return (pItem->GetText() == (char *)lUserData);
}

bool CList_FindFromlParamFunction(CListItemData *pItem, LPARAM lUserData, LPARAM lParam)
{
	return (pItem->GetItemData() == lUserData);
}

CList::CList(void)
:	m_pSourceItem(NULL),
	m_nXNum(1),
	m_nYNum(1),
	m_nSortType(-1),
	m_nFindType(-1),
	m_bIncrement(true),
	m_bAutoSort(false),
	m_ptItemGab(0.0f, 0.0f),
	m_bSingleSelect(true),
	m_bVirtical(true),
	m_pScroll(NULL),
	m_nItemWidth(0),
	m_nItemHeight(0),
	m_nClipWidth(0),
	m_nClipHeight(0),
	m_nScrollPos(0),
	m_bLineScroll(false),
	m_pUpdateFunction(NULL),
	m_lUpdateParam(NULL),
	m_pClonedFunction(NULL),
	m_lClonedParam(NULL),
	m_bReposition(false),
	m_pRectImage(NULL)
{
	if(IsTool())
	{
		m_pRectImage = new CDumyImage;
		m_pRectImage->SetLocalAlpha(0.5f);
		m_pRectImage->Create(25, 25);
		m_pRectImage->ClearColor(RGB(128, 128, 20));
		SetLocalRect(m_pRectImage->GetLocalRect());
		AddLocalChild(m_pRectImage);
	}

	m_DefaultCompare.m_pFunction = CList_DefaultCompareFunction;
	m_DefaultFind.m_pFindFunction = CList_DefaultFindFunction;
	m_itFind = m_listData.end();
	m_pItemMain = new CObjectBase;
	m_pItemMain->SetMessage(true);
	m_pItemMain->AddHandler(EM_O_LOCALCHILDNOTIFY, Fnt(this, &CList::OnListItemNotify));
	AddLocalChild(m_pItemMain);
	SetMessage(true);
}

CList::~CList()
{
	LISTITEMDATAITER start = m_listData.begin();
	LISTITEMDATAITER end = m_listData.end();

	for(;start != end;++start)
	{
		delete (*start);
	}

	m_listData.clear();

	LISTITEMITER start_item = m_listNotUseItems.begin();
	LISTITEMITER end_item = m_listNotUseItems.end();

	for(;start_item != end_item;++start_item)
	{
		delete (*start_item);
	}

	m_listNotUseItems.clear();

	if(m_pSourceItem)
	{
		delete m_pSourceItem;
		m_pSourceItem = NULL;
	}

	if(m_pScroll)
	{
		delete m_pScroll;
		m_pScroll = NULL;
	}
}

bool CList::CCompareFuntion::operator() (CListItemData *pItem1, CListItemData*pItem2)
{
	return m_pFunction(pItem1, pItem2, m_bIncrement, m_lParam);
}

CObjectBase *CList::NewObject(void)
{
	return new CList;
}

CObjectBase *CList::Copy(CObjectBase *pObject)
{
	CObjectBase::Copy(pObject);

	CList *pList				= (CList *)pObject;
	pList->m_listCompare		= m_listCompare;
	pList->m_listFind			= m_listFind;
	pList->m_pUpdateFunction	= m_pUpdateFunction;
	pList->m_lUpdateParam		= m_lUpdateParam;
	pList->m_pClonedFunction	= m_pClonedFunction;
	pList->m_lClonedParam		= m_lClonedParam;
	if(m_pSourceItem) pList->AttachItem((CListItem *)m_pSourceItem->Clone(true));
	pList->m_bIncrement			= m_bIncrement;
	pList->m_nSortType			= m_nSortType;
	pList->m_bAutoSort			= m_bAutoSort;
	pList->m_nFindType			= m_nFindType;
	pList->m_nXNum				= m_nXNum;
	pList->m_nYNum				= m_nYNum;
	pList->m_ptItemGab			= m_ptItemGab;
	pList->m_bSingleSelect		= m_bSingleSelect;
	pList->m_bVirtical			= m_bVirtical;
	if(m_pScroll)	pList->AttachScroll((CScrollBar *)m_pScroll->Clone(true));
	pList->m_nItemWidth			= m_nItemWidth;
	pList->m_nItemHeight		= m_nItemHeight;
	pList->m_nClipWidth			= m_nClipWidth;
	pList->m_nClipHeight		= m_nClipHeight;
	pList->m_nScrollPos			= m_nScrollPos;
	pList->m_rtListRect			= m_rtListRect;
	pList->m_bLineScroll		= m_bLineScroll;

	return pObject;
}

size_t CList::GetInheritanceClipSize(int &nVersion)
{
	size_t nRet = __super::GetInheritanceClipSize(nVersion);
	nRet += sizeof(bool);	//Item 사용유무
	if(m_pSourceItem) nRet += m_pSourceItem->GetClipSize(nVersion);
	nRet += sizeof(bool);	//m_bIncrement
	nRet += sizeof(int);	//m_nSortType
	nRet += sizeof(bool);	//m_bAutoSort
	nRet += sizeof(int);	//m_nFindType
	nRet += sizeof(int);	//m_nXNum
	nRet += sizeof(int);	//m_nYNum
	nRet += sizeof(CPoint);	//m_ptItemGab
	nRet += sizeof(bool);	//m_bSingleSelect
	nRet += sizeof(bool);	//m_bVirtical
	nRet += sizeof(bool);	//스클롤 사용유무
	if(m_pScroll) nRet += m_pScroll->GetClipSize(nVersion);
	nRet += sizeof(int);	//m_nItemWidth
	nRet += sizeof(int);	//m_nItemHeight
	nRet += sizeof(int);	//m_nClipWidth
	nRet += sizeof(int);	//m_nClipHeight
	nRet += sizeof(int);	//m_nScrollPos
	nRet += sizeof(CRect);	//m_rtListRect
	nRet += sizeof(bool);	//m_bLineScroll
	return nRet;
}

size_t CList::SaveInheritanceClipSize(int &nVersion, BYTE *pDest)
{
	size_t nRet = __super::SaveInheritanceClipSize(nVersion, pDest);
	bool bUse = m_pSourceItem?true:false;
	memcpy(pDest+nRet, &bUse, sizeof(bool));
	nRet += sizeof(bool);	//Item 사용유무
	if(m_pSourceItem)
	{
		nRet += m_pSourceItem->SaveClipSize(nVersion, pDest+nRet);
	}
	memcpy(pDest+nRet, &m_bIncrement, sizeof(bool));
	nRet += sizeof(bool);	//m_bIncrement
	memcpy(pDest+nRet, &m_nSortType, sizeof(int));
	nRet += sizeof(int);	//m_nSortType
	memcpy(pDest+nRet, &m_bAutoSort, sizeof(bool));
	nRet += sizeof(bool);	//m_bAutoSort
	memcpy(pDest+nRet, &m_nFindType, sizeof(int));
	nRet += sizeof(int);	//m_nFindType
	memcpy(pDest+nRet, &m_nXNum, sizeof(int));
	nRet += sizeof(int);	//m_nXNum
	memcpy(pDest+nRet, &m_nYNum, sizeof(int));
	nRet += sizeof(int);	//m_nYNum
	memcpy(pDest+nRet, &m_ptItemGab, sizeof(CPoint));
	nRet += sizeof(CPoint);	//m_ptItemGab
	memcpy(pDest+nRet, &m_bSingleSelect, sizeof(bool));
	nRet += sizeof(bool);	//m_bSingleSelect
	memcpy(pDest+nRet, &m_bVirtical, sizeof(bool));
	nRet += sizeof(bool);	//m_bVirtical
	bUse = m_pScroll?true:false;
	memcpy(pDest+nRet, &bUse, sizeof(bool));
	nRet += sizeof(bool);	//스클롤 사용유무
	if(m_pScroll)
	{
		nRet += m_pScroll->SaveClipSize(nVersion, pDest+nRet);
	}
	memcpy(pDest+nRet, &m_nItemWidth, sizeof(int));
	nRet += sizeof(int);	//m_nItemWidth
	memcpy(pDest+nRet, &m_nItemHeight, sizeof(int));
	nRet += sizeof(int);	//m_nItemHeight
	memcpy(pDest+nRet, &m_nClipWidth, sizeof(int));
	nRet += sizeof(int);	//m_nClipWidth
	memcpy(pDest+nRet, &m_nClipHeight, sizeof(int));
	nRet += sizeof(int);	//m_nClipHeight
	memcpy(pDest+nRet, &m_nScrollPos, sizeof(int));
	nRet += sizeof(int);	//m_nScrollPos
	memcpy(pDest+nRet, &m_rtListRect, sizeof(CRect));
	nRet += sizeof(CRect);	//m_rtListRect
	memcpy(pDest+nRet, &m_bLineScroll, sizeof(bool));
	nRet += sizeof(bool);	//m_bLineScroll
	return nRet;
}

size_t CList::LoadInheritanceClipSize(int &nVersion, BYTE *pDest, CObjectProject *pProject, bool bSetID)
{
	size_t nRet = __super::LoadInheritanceClipSize(nVersion, pDest, pProject, bSetID);
	bool bUse = false;
	memcpy(&bUse, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);	//Item 사용유무
	if(bUse)
	{
		CListItem *pListItem = new CListItem;
		nRet += sizeof(ENUM_OBJECTTYPE);
		nRet += pListItem->LoadClipSize(nVersion, pDest+nRet, pProject, bSetID);
		AttachItem(pListItem);
	}
	memcpy(&m_bIncrement, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);	//m_bIncrement
	memcpy(&m_nSortType, pDest+nRet, sizeof(int));
	nRet += sizeof(int);	//m_nSortType
	memcpy(&m_bAutoSort, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);	//m_bAutoSort
	memcpy(&m_nFindType, pDest+nRet, sizeof(int));
	nRet += sizeof(int);	//m_nFindType
	memcpy(&m_nXNum, pDest+nRet, sizeof(int));
	nRet += sizeof(int);	//m_nXNum
	memcpy(&m_nYNum, pDest+nRet, sizeof(int));
	nRet += sizeof(int);	//m_nYNum
	memcpy(&m_ptItemGab, pDest+nRet, sizeof(CPoint));
	nRet += sizeof(CPoint);	//m_ptItemGab
	memcpy(&m_bSingleSelect, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);	//m_bSingleSelect
	memcpy(&m_bVirtical, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);	//m_bVirtical
	memcpy(&bUse, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);	//스클롤 사용유무
	if(bUse)
	{
		CScrollBar *pScroll = new CScrollBar;
		nRet += sizeof(ENUM_OBJECTTYPE);
		nRet += pScroll->LoadClipSize(nVersion, pDest+nRet, pProject, bSetID);
		AttachScroll(pScroll);
	}
	memcpy(&m_nItemWidth, pDest+nRet, sizeof(int));
	nRet += sizeof(int);	//m_nItemWidth
	memcpy(&m_nItemHeight, pDest+nRet, sizeof(int));
	nRet += sizeof(int);	//m_nItemHeight
	memcpy(&m_nClipWidth, pDest+nRet, sizeof(int));
	nRet += sizeof(int);	//m_nClipWidth
	memcpy(&m_nClipHeight, pDest+nRet, sizeof(int));
	nRet += sizeof(int);	//m_nClipHeight
	memcpy(&m_nScrollPos, pDest+nRet, sizeof(int));
	nRet += sizeof(int);	//m_nScrollPos
	memcpy(&m_rtListRect, pDest+nRet, sizeof(CRect));
	nRet += sizeof(CRect);	//m_rtListRect
	memcpy(&m_bLineScroll, pDest+nRet, sizeof(bool));
	nRet += sizeof(bool);	//m_bLineScroll

	CalculteClipRect();
	if(IsTool()) MakeSample();

	return nRet;
}

bool CList::SaveInheritanceXml(TiXmlNode *pXmlObject)
{
	__super::SaveInheritanceXml(pXmlObject);

	TiXmlElement xmlInheritance("Inheritance");
	xmlInheritance.SetAttribute("increment",  m_bIncrement?1:0);
	xmlInheritance.SetAttribute("autosort",  m_bAutoSort?1:0);
	xmlInheritance.SetAttribute("x",  m_nXNum);
	xmlInheritance.SetAttribute("y",  m_nYNum);
	xmlInheritance.SetDoubleAttribute("item_x",  m_ptItemGab.x);
	xmlInheritance.SetDoubleAttribute("item_y",  m_ptItemGab.y);
	xmlInheritance.SetAttribute("singleselect",  m_bSingleSelect?1:0);
	xmlInheritance.SetAttribute("virtical",  m_bVirtical?1:0);
	xmlInheritance.SetAttribute("clip_x",  m_nClipWidth);
	xmlInheritance.SetAttribute("clip_y",  m_nClipHeight);
	xmlInheritance.SetAttribute("linescroll",  m_bLineScroll?1:0);
	pXmlObject->InsertEndChild(xmlInheritance);

	TiXmlElement xmlAttach("attach");
	if(m_pSourceItem) m_pSourceItem->SaveXml(&xmlAttach);
	if(m_pScroll) m_pScroll->SaveXml(&xmlAttach);
	pXmlObject->InsertEndChild(xmlAttach);

	return true;
}

bool CList::LoadInheritanceXml(const int &nVersion, TiXmlElement *pXmlObject, CObjectProject *pProject)
{
	__super::LoadInheritanceXml(nVersion, pXmlObject, pProject);

	TiXmlElement *pXmlInheritance = pXmlObject->FirstChildElement("Inheritance");
	GetAttribute(pXmlInheritance, "increment", m_bIncrement);
	GetAttribute(pXmlInheritance, "autosort", m_bAutoSort);
	GetAttribute(pXmlInheritance, "x", m_nXNum);
	GetAttribute(pXmlInheritance, "y", m_nYNum);
	GetAttribute(pXmlInheritance, "item_x", m_ptItemGab.x);
	GetAttribute(pXmlInheritance, "item_y", m_ptItemGab.y);
	GetAttribute(pXmlInheritance, "singleselect", m_bSingleSelect);
	GetAttribute(pXmlInheritance, "virtical", m_bVirtical);
	GetAttribute(pXmlInheritance, "clip_x", m_nClipWidth);
	GetAttribute(pXmlInheritance, "clip_y", m_nClipHeight);
	GetAttribute(pXmlInheritance, "linescroll", m_bLineScroll);

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
		case EOT_LISTITEM:
			AttachItem((CListItem *)pChildObject);
			break;
		default:
			ASSERT(FALSE);
			break;
		}
		OnAttachChild(pChildObject);
		pChildObject->OnCreated();
		pXmlChildObject = pXmlChildObject->NextSiblingElement("Object");
	}

	CalculteClipRect();
	if(IsTool()) MakeSample();

	return true;
}

bool CList::OnMouseWheel(const int &nKey, const short &zDelta)
{
	if(m_pScroll) m_pScroll->OnMouseWheel(nKey, zDelta);

	if(m_pProject)
	{
		CObjectMan *pManager = m_pProject->GetObjectMan();
		if(pManager)
		{
			::CPoint ptPos;
			GetCursorPos(&ptPos);
			pManager->GetWnd()->ScreenToClient(&ptPos);

			LPARAM lParam = MAKELPARAM(ptPos.x, ptPos.y);
			pManager->GetWnd()->PostMessage(WM_MOUSEMOVE, 0, lParam);
		}
	}

	return CObjectBase::OnMouseWheel(nKey, zDelta);
}

bool CList::Create(int nXNum, int nYNum)
{
	if(m_nXNum == nXNum && m_nYNum == nYNum) return false;
	m_nXNum = nXNum;
	m_nYNum = nYNum;

	CalculteClipRect();

	return true;
}

void CList::ReIndexItem(void)
{
	LISTITEMDATAITER start = m_listData.begin();
	LISTITEMDATAITER end = m_listData.end();

	for(int i=0;start != end;++start,++i)
	{
		(*start)->SetItemIndex(i);
	}
}

CList::CCompareFuntion *CList::GetCompareFunction(const int &nType)
{
	std::map<int, CCompareFuntion>::iterator findIt = m_listCompare.find(nType);
	if (findIt != m_listCompare.end())
		return &findIt->second;

	return &m_DefaultCompare;
}

void CList::SetCompareFunction(const int &nType, const LISTCOMPAREFUNCTION pCompare)
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

void CList::SetCompareFunctionParam(const int &nType, const LPARAM &lParam)
{
	std::map<int, CCompareFuntion>::iterator findIt = m_listCompare.find(nType);
	if (findIt != m_listCompare.end())
	{
		findIt->second.m_lParam = lParam;
		return;
	}

	ASSERT(NULL);
}

void CList::SetFindFunction(const int &nType, const LISTITEMFINDFUNCTION &pFindFunction)
{
	std::map<int, CFindFuntion>::iterator findIt = m_listFind.find(nType);

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

void CList::SetFindFunctionParam(const int &nType, const LPARAM &lParam)
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

CList::CFindFuntion *CList::GetFindFunction(const int &nType)
{
	std::map<int, CFindFuntion>::iterator findIt = m_listFind.find(nType);
	if(findIt == m_listFind.end()) return &m_DefaultFind;
	return &findIt->second;
}

void CList::SetIncrement(const bool &bIncrement)
{
	if (m_bIncrement == bIncrement) return;
	m_bIncrement = bIncrement;
	if(m_bAutoSort) Sort();
}

void CList::SetSortType(const int &nType)
{
	if(m_nSortType == nType) return;
	m_nSortType = nType;
	if(m_bAutoSort) Sort();
}

void CList::SetAutoSort(const bool &bAuto)
{
	if(m_bAutoSort == bAuto) return;
	m_bAutoSort = bAuto;
	if(m_bAutoSort) Sort();
}

const int CList::Sort(CListItemData *pData/* = NULL*/)
{
	if (m_listData.size() < 2)
		return 1;

	CCompareFuntion* pCompare = GetCompareFunction(m_nSortType);
	if (pCompare == NULL)
	{
		assert(!"소트 펑션을 찾지 못했습니다.");
		return -1;
	}

	pCompare->m_bIncrement = m_bIncrement;
	m_listData.sort(*pCompare);

	LISTITEMDATAITER start = m_listData.begin();
	LISTITEMDATAITER end = m_listData.end();
	int nIndex = -1;

	for(int i=0;start != end;++start,++i)
	{
		(*start)->SetItemIndex(i);
		if(*start == pData) nIndex = i;
	}

	SetReposition(true);

	return nIndex;
}

//접속중인 유저 리스트 셔플기능 추가:2012.10.22 YS.Jeong
const int CList::SortShuffle(CListItemData *pData/* = NULL*/)
{
	if (m_listData.size() < 2)
		return 1;


	m_listData.sort();

	LISTITEMDATAITER start = m_listData.begin();
	LISTITEMDATAITER end = m_listData.end();
	int nIndex = -1;

	//아이템을 배치한다.
	int len = 0;
	for(int i=0;start != end;++start,++i)
	{
		(*start)->SetItemIndex(i);
		if(*start == pData) nIndex = i;
		len++;
	}


	//배치된 아이템을 섞는다.
	int randPosition = 0;
	int tempNum = 0;
	for(int i=0;start != end;++start,++i)
	{
		randPosition = rand()%len;
		tempNum = (*start)->GetItemIndex();
		(*start)->SetItemIndex( randPosition );

		for(int j=0;start != end;++start,++j)
		{
			if( (*start)->GetItemIndex() == randPosition ){
				(*start)->SetItemIndex( tempNum );
			}
		}
	}

	SetReposition(true);

	return nIndex;
}

int	CList::GetItemIndex(CListItem *pItem)
{
	if(pItem == NULL) return -1;
	DEF_CHECK_REPOSITION;
	CListItemData *pData = pItem->GetData();
	return pData->GetItemIndex();
}

CListItemData *CList::GetItemFromIndex(int nIndex)
{
	LISTITEMDATAITER start = m_listData.begin();
	LISTITEMDATAITER end = m_listData.end();
	CListItemData *pData;
	for(;start != end;++start)
	{
		pData = *start;
		if(pData->GetItemIndex() == nIndex) return pData;
	}
	return NULL;
}

CListItemData *CList::GetItemFromParam(LPARAM lParam)
{
	LISTITEMDATAITER start = m_listData.begin();
	LISTITEMDATAITER end = m_listData.end();
	CListItemData *pData;
	for(;start != end;++start)
	{
		pData = *start;
		if(pData->GetItemData() == lParam) return pData;
	}
	return NULL;
}

CListItemData *CList::GetItemFromString(string strText)
{
	LISTITEMDATAITER start = m_listData.begin();
	LISTITEMDATAITER end = m_listData.end();
	CListItemData *pData;
	for(;start != end;++start)
	{
		pData = *start;
		if(pData->GetText() == strText) return pData;
	}
	return NULL;
}

CListItemData *CList::GetFirstFindItem(int nType, LPARAM lUserData)
{
	CFindFuntion *pFind = GetFindFunction(nType);
	if(pFind == NULL) return NULL;

	m_itFind = m_listData.begin();
	LISTITEMDATAITER end = m_listData.end();

	CListItemData *pData = NULL;

	for(;m_itFind != end;++m_itFind)
	{
		pData = *m_itFind;
		if(pFind->m_pFindFunction(pData, lUserData, pFind->m_lParam))
			return pData;
	}

	return NULL;
}

CListItemData *CList::GetNextFindItem(int nType, LPARAM lUserData)
{
	CFindFuntion *pFind = GetFindFunction(nType);
	if(pFind == NULL) return NULL;
	LISTITEMDATAITER end = m_listData.end();
	if(m_itFind == end) return NULL;

	++m_itFind;
	CListItemData *pData = NULL;

	for(;m_itFind != end;++m_itFind)
	{
		pData = *m_itFind;
		if(pFind->m_pFindFunction(pData, lUserData, pFind->m_lParam))
			return pData;
	}

	return NULL;
}

void CList::SetItemGab(float fGapX, float fGapY)
{
	m_ptItemGab.x = fGapX;
	m_ptItemGab.y = fGapY;

	CalculteClipRect();
	SetScrollData();
}

void CList::SetItemGab(CPoint ptGap)
{
	m_ptItemGab = ptGap;

	CalculteClipRect();
	SetScrollData();
}

void CList::AttachItem(CListItem *pItem)
{
	ASSERT(m_pSourceItem == NULL && pItem != NULL);

	m_pSourceItem = pItem;
	m_pSourceItem->SetAttach(true);

	m_pItemMain->SetLocalPos(pItem->GetLocalPos());

	if(pItem->GetParent()) pItem->GetParent()->SubChild(pItem);

	m_nItemWidth = (int)m_pSourceItem->GetWidth();
	m_nItemHeight = (int)m_pSourceItem->GetHeight();

	if(m_pScroll)
	{
		if(m_bVirtical) m_pScroll->SetChangeValue((float)m_nItemHeight);
		else m_pScroll->SetChangeValue((float)m_nItemWidth);
	}

	CalculteClipRect();

	if(IsTool()) MakeSample();
}

void CList::DettachItem(void)
{
	ASSERT(m_pSourceItem != NULL);
	DeleteAllItem();
	m_pSourceItem->SetAttach(false);
	m_pSourceItem = NULL;
}

void CList::AttachScroll(CScrollBar *pScroll)
{
	ASSERT(m_pScroll == NULL && pScroll != NULL);

	m_pScroll = pScroll;
	AddLocalChild(m_pScroll);
	SetUseMouseWheel(true);

	if(m_bVirtical) m_pScroll->SetChangeValue((float)m_nItemHeight);
	else m_pScroll->SetChangeValue((float)m_nItemWidth);

	SetScrollData();
}

void CList::DettachScroll(void)
{
	ASSERT(m_pScroll != NULL);
	SubChild(m_pScroll);

	SetUseMouseWheel(false);

	m_pScroll = NULL;
}

void CList::SetSplit(int nXNum, int nYNum)
{
	if((m_nXNum == nXNum) && (m_nYNum == nYNum)) return;
	ASSERT((nXNum > 0) && (nYNum > 0));

	m_nXNum = nXNum;
	m_nYNum = nYNum;

	CalculteClipRect();
	SetScrollData();
}

void CList::UpdateItem(CListItem *pItem)
{
	if(pItem == NULL) return;
	CListItemData *pData = pItem->GetData();
	pItem->SetEnable(pData->IsEnable());
	pItem->SetCheck(pData->IsSelect());
	if(m_pUpdateFunction) m_pUpdateFunction(pItem, m_lUpdateParam);
	else pItem->UpdateData(m_lUpdateParam);
}

void CList::UpdateItem(CListItemData *pData)
{
	UpdateItem(pData->GetItem());
}

void CList::UpdateItemFromFind(int nType, LPARAM lUserData)
{
	if(m_pUpdateFunction == NULL) return;
	CListItemData *pData = GetFirstFindItem(nType, lUserData);
	if(pData == NULL) return;

	UpdateItem(pData);	
}

void CList::UpdateItemFromIndex(int nIndex)
{
	CListItemData *pData = GetItemFromIndex(nIndex);
	if(pData == NULL) return;

	UpdateItem(pData);
}

void CList::UpdateItemFromParam(LPARAM lParam)
{
	CListItemData *pData = GetItemFromParam(lParam);
	if(pData == NULL) return;

	UpdateItem(pData);
}

void CList::UpdateItemFromString(string strText)
{
	CListItemData *pData = GetItemFromString(strText);
	if(pData == NULL) return;

	UpdateItem(pData);
}

void CList::SetClonedFunction(LISTITEMCLONEDFUNCTION pClonedFunction)
{
	if(m_pClonedFunction == pClonedFunction) return;
	m_pClonedFunction = pClonedFunction;
}

void CList::SetUpdateFunction(LISTITEMUPDATAFUNCTION pUpdateFunction)
{
	if(m_pUpdateFunction == pUpdateFunction) return;
	m_pUpdateFunction = pUpdateFunction;
}

void CList::SetItemSelect(CListItem *pItem)
{
	if(pItem == NULL) return;
	pItem->SetCheck(pItem->GetData()->IsSelect());
}

bool CList::SetItemSelect(CListItemData *pData, bool bSelect)
{
	bool bRet = false;
	if(m_bSingleSelect && bSelect)
	{
		LISTITEMDATAITER start = m_listData.begin();
		LISTITEMDATAITER end = m_listData.end();

		CListItemData *pTemp = NULL;
		for(;start != end;++start)
		{
			pTemp = *start;
			if(pTemp == pData)
			{
				pTemp->SetSelect(bSelect);
				bRet = true;
			}
			else pTemp->SetSelect(false);
		}
	}
	else
	{
		pData->SetSelect(bSelect);
		bRet = true;
	}

	LISTITEMITER start = m_listItems.begin();
	LISTITEMITER end = m_listItems.end();

	for(;start != end;++start)
	{
		SetItemSelect(*start);
	}

	return bRet;
}

bool CList::SetItemSelectFromFind(int nType, LPARAM lUserData, bool bSelect)
{
	CListItemData *pData = GetFirstFindItem(nType, lUserData);
	if(pData == NULL) return false;

	return SetItemSelect(pData, bSelect);	
}

bool CList::SetItemSelectFromIndex(int nIndex, bool bSelect)
{
	CListItemData *pData = GetItemFromIndex(nIndex);
	if(pData == NULL) return false;

	return SetItemSelect(pData, bSelect);
}

bool CList::SetItemSelectFromParam(LPARAM lParam, bool bSelect)
{
	CListItemData *pData = GetItemFromParam(lParam);
	if(pData == NULL) return false;

	return SetItemSelect(pData, bSelect);
}

bool CList::SetItemSelectFromString(string strText, bool bSelect)
{
	CListItemData *pData = GetItemFromString(strText);
	if(pData == NULL) return false;

	return SetItemSelect(pData, bSelect);
}

void CList::SetEnableItem(CListItem *pItem)
{
	if(pItem == NULL) return;
	pItem->SetEnable(pItem->GetData()->IsEnable());
}

bool CList::SetEnableItem(CListItemData *pData, bool bEnable)
{
	ASSERT(pData != NULL);
	pData->SetEnable(bEnable);
	SetEnableItem(pData->GetItem());
	return true;
}

bool CList::SetEnableItemFromFind(int nType, LPARAM lUserData, bool bEnable)
{
	CListItemData *pData = GetFirstFindItem(nType, lUserData);
	if(pData == NULL) return false;

	return SetEnableItem(pData, bEnable);	
}

bool CList::SetEnableItemFromIndex(int nIndex, bool bEnable)
{
	CListItemData *pData = GetItemFromIndex(nIndex);
	if(pData == NULL) return false;

	return SetEnableItem(pData, bEnable);
}

bool CList::SetEnableItemFromParam(LPARAM lParam, bool bEnable)
{
	CListItemData *pData = GetItemFromParam(lParam);
	if(pData == NULL) return false;

	return SetEnableItem(pData, bEnable);
}

bool CList::SetEnableItemFromString(string strText, bool bEnable)
{
	CListItemData *pData = GetItemFromString(strText);
	if(pData == NULL) return false;

	return SetEnableItem(pData, bEnable);

}

bool CList::IsEnableItem(CListItem *pItem)
{
	return IsEnableItem(pItem->GetData());
}

bool CList::IsEnableItem(CListItemData *pData)
{
	return pData->IsEnable();
}

bool CList::IsEnableItemFromFind(int nType, LPARAM lUserData)
{
	CListItemData *pData = GetFirstFindItem(nType, lUserData);
	if(pData == NULL) return false;

	return IsEnableItem(pData);	
}

bool CList::IsEnableItemFromIndex(int nIndex)
{
	CListItemData *pData = GetItemFromIndex(nIndex);
	if(pData == NULL) return false;

	return IsEnableItem(pData);
}

bool CList::IsEnableItemFromParam(LPARAM lParam)
{
	CListItemData *pData = GetItemFromParam(lParam);
	if(pData == NULL) return false;

	return IsEnableItem(pData);
}

bool CList::IsEnableItemFromString(string strText)
{
	CListItemData *pData = GetItemFromString(strText);
	if(pData == NULL) return false;

	return IsEnableItem(pData);
}

bool CList::SetString(CListItemData *pData, std::string strText)
{
	if(pData == NULL) return false;
	pData->SetText(strText);
	return true;
}

void CList::SetString(CListItem *pItem)
{
	if(pItem == NULL) return;
}

bool CList::SetStringFromIndex(int nIndex, std::string strText)
{
	CListItemData *pData = GetItemFromIndex(nIndex);
	if(pData == NULL) return false;

	return SetString(pData, strText);
}

bool CList::SetStringFromParam(LPARAM lParam, std::string strText)
{
	CListItemData *pData = GetItemFromParam(lParam);
	if(pData == NULL) return false;

	return SetString(pData, strText);
}

bool CList::SetStringFromFind(int nType, LPARAM lUserData, std::string strText)
{
	CListItemData *pData = GetFirstFindItem(nType, lUserData);
	if(pData == NULL) return false;

	return SetString(pData, strText);
}

bool CList::SetStringFromString(std::string strOldText, std::string strText)
{
	CListItemData *pData = GetItemFromString(strOldText);
	if(pData == NULL) return false;

	return SetString(pData, strText);
}

void CList::DeleteAllItem(void)
{
	LISTITEMITER start_item = m_listItems.begin();
	LISTITEMITER end_item = m_listItems.end();

	for(;start_item != end_item;++start_item)
	{
		(*start_item)->SetAttach(false);
		m_pItemMain->SubChild(*start_item);
		(*start_item)->SetData(NULL);
		m_listNotUseItems.push_back(*start_item);
	}

	m_listItems.clear();

	LISTITEMDATAITER start = m_listData.begin();
	LISTITEMDATAITER end = m_listData.end();

	for(;start != end;++start)
	{
		delete *start;
	}

	m_listData.clear();

	SetScrollData();
}

bool CList::DeleteItem(CListItemData *pData)
{
	LISTITEMDATAITER findIt = find(m_listData.begin(), m_listData.end(), pData);
	if(findIt == m_listData.end()) return false;

	if(pData->GetItemIndex() < GetFirstVisibleItem())
	{
		if(m_bVirtical)
		{
			m_nScrollPos -= (int)(m_nItemHeight+m_ptItemGab.y);
		}
		else
		{
			m_nScrollPos -= (int)(m_nItemWidth+m_ptItemGab.x);
		}
	}

	if(pData->GetItem())
		pData->GetItem()->SetData(NULL);

	delete *findIt;
	m_listData.erase(findIt);

	ReIndexItem();
	SetScrollData();

	return true;
}

bool CList::DeleteItem(int nIndex)
{
	CListItemData *pData = GetItemFromIndex(nIndex);
	if(pData == NULL) return false;

	return DeleteItem(pData);
}

bool CList::DeleteItem(LPARAM lParam)
{
	CListItemData *pData = GetItemFromParam(lParam);
	if(pData == NULL) return false;

	return DeleteItem(pData);
}

bool CList::DeleteItem(int nType, LPARAM lUserData)
{
	CListItemData *pData = GetFirstFindItem(nType, lUserData);
	if(pData == NULL) return false;

	return DeleteItem(pData);
}

int CList::GetFirstVisibleItem(void)
{
	int nIndex = -1;
	if(m_listData.size() != 0)
	{
		if(m_bVirtical)
		{
			int nY = m_nScrollPos / (int)(m_nItemHeight+m_ptItemGab.y);
			nIndex = nY * m_nXNum;
		}
		else
		{
			int nX = m_nScrollPos / (int)(m_nItemWidth+m_ptItemGab.x);
			nIndex = nX * m_nYNum;
		}
	}
	return nIndex;
}

int CList::GetLastVisibleItem(void)
{
	int nFirstVisibleItem = GetFirstVisibleItem();
	int nLastVisibleItem = nFirstVisibleItem;

	if(nFirstVisibleItem != -1)
	{
		if(m_bVirtical)
		{
			int nViewCount = (int)m_rtListRect.GetHeight() / (int)(m_nItemHeight+m_ptItemGab.y);
			nLastVisibleItem = nFirstVisibleItem + nViewCount * m_nXNum;
		}
		else
		{
			int nViewCount = (int)m_rtListRect.GetWidth() / (int)(m_nItemWidth+m_ptItemGab.x);
			nLastVisibleItem = nFirstVisibleItem + nViewCount * m_nYNum;
		}

		if(nLastVisibleItem >= GetItemCount()) nLastVisibleItem = GetItemCount() - 1;
	}

	return nLastVisibleItem;
}

CListItemData *CList::HitTest(const CPoint &ptPos)
{
	CPoint ptLocal = GetGlobalToLocalPos(ptPos);
	if(!m_rtListRect.PtInRect(ptLocal)) return NULL;

	DEF_CHECK_REPOSITION;
	CListItemData *pItem = NULL;

	int nFirstVisibleItem = GetFirstVisibleItem();
	if(nFirstVisibleItem != -1) return NULL;

	int nLastVisualItem = GetLastVisibleItem();
	CRect rtTemp(CRectZero);
	if(m_bVirtical)
	{
		for(int i = nFirstVisibleItem ; i <= nLastVisualItem ; ++i)
		{
			rtTemp.top	= float((nFirstVisibleItem / m_nXNum) * (m_nItemHeight+m_ptItemGab.y) - m_nScrollPos);
			rtTemp.left	= float((nFirstVisibleItem % m_nXNum) * (m_nItemWidth+m_ptItemGab.x));
			rtTemp.bottom += (float)m_nItemHeight;
			rtTemp.right += (float)m_nItemWidth;

			if(rtTemp.PtInRect(ptLocal))
			{
				pItem = GetItemFromIndex(i);
				break;
			}
		}
	}
	else
	{
		for(int i = nFirstVisibleItem ; i <= nLastVisualItem ; ++i)
		{
			rtTemp.left	= float((nFirstVisibleItem / m_nYNum) * (m_nItemWidth+m_ptItemGab.x) - m_nScrollPos);
			rtTemp.top	= float((nFirstVisibleItem % m_nYNum) * (m_nItemHeight+m_ptItemGab.y));
			rtTemp.right += (float)m_nItemWidth;
			rtTemp.bottom += (float)m_nItemHeight;

			if(rtTemp.PtInRect(ptLocal))
			{
				pItem = GetItemFromIndex(i);
				break;
			}
		}
	}
	return pItem;
}

void CList::SetVisibleItem(CListItem *pItem)
{
	if(pItem == NULL) return;
	SetVisibleItem(pItem->GetData());
}

void CList::SetVisibleItem(CListItemData *pData)
{
	if(pData == NULL) return;
	SetVisibleItemFromIndex(pData->GetItemIndex());
}

void CList::SetVisibleItemFromIndex(int nIndex)
{
	if(nIndex < 0) return;
	if(nIndex >= (int)m_listData.size()) return;

	if(m_bVirtical)
	{
		int nY = nIndex/m_nXNum;
		m_nScrollPos = (int)(nY * (m_nItemHeight+m_ptItemGab.y));
	}
	else
	{
		int nX = nIndex/m_nYNum;
		m_nScrollPos = (int)(nX * (m_nItemWidth+m_ptItemGab.x));
	}
	SetScrollData();
}

void CList::SetVisibleItemFromParam(LPARAM lParam)
{
	CListItemData *pData = GetItemFromParam(lParam);
	if(pData == NULL) return;
	SetVisibleItem(pData);
}

void CList::SetVisibleItemFromFind(int nType, LPARAM lUserData)
{
	CListItemData *pData = GetFirstFindItem(nType, lUserData);
	if(pData == NULL) return;
	SetVisibleItem(pData);
}

void CList::SetVisibleItemFromString(string strText)
{
	CListItemData *pData = GetItemFromString(strText);
	if(pData == NULL) return;
	SetVisibleItem(pData);
}

bool CList::DeleteItem(string strText)
{
	CListItemData *pData = GetItemFromString(strText);
	if(pData == NULL) return false;

	return DeleteItem(pData);
}

void CList::ClearAllSelect(void)
{
	LISTITEMDATAITER start = m_listData.begin();
	LISTITEMDATAITER end = m_listData.end();

	CListItemData *pData = NULL;
	for(;start != end;++start)
	{
		pData = *start;
		pData->SetSelect(false);
	}

	LISTITEMITER start_item = m_listItems.begin();
	LISTITEMITER end_item = m_listItems.end();

	for(;start_item != end_item;++start_item)
	{
		SetItemSelect(*start_item);
	}
}

void CList::SetSingleSelect(bool bSingleSelect)
{
	if(m_bSingleSelect == bSingleSelect) return;
	ClearAllSelect();
	m_bSingleSelect = bSingleSelect;
}

void CList::SetVirtical(const bool &bVirtical)
{
	if(m_bVirtical == bVirtical) return;
	m_bVirtical = bVirtical;
	SetScrollData();
}

void CList::SetClipSize(int nWidth, int nHeight)
{
	ASSERT((nWidth >= 0) && (nHeight >= 0));
	m_nClipWidth = nWidth;
	m_nClipHeight = nHeight;

	CalculteClipRect();
	SetScrollData();
}

//현재 선택된 아이템을 구해오는 함수들
//GetFirstSelectItem을 먼저 호출하여 첫번째 선택된 아이템을 구해오고, 그 다음 부터는 GetNextSelectItem 함수로 
//차례로 선택된 아이템을 구해오면 된다.
CListItemData *CList::GetFirstSelectItem(void)
{
	DEF_CHECK_REPOSITION;
	m_itFind = m_listData.begin();
	LISTITEMDATAITER end = m_listData.end();
	
	for(;m_itFind != end;++m_itFind)
	{
		if((*m_itFind)->IsSelect())
			return *m_itFind;
	}

	return NULL;
}

CListItemData *CList::GetNextSelectItem(void)
{
	DEF_CHECK_REPOSITION;
	LISTITEMDATAITER end = m_listData.end();
	if(m_itFind == end) return NULL;
	++m_itFind;
	for(;m_itFind != end;++m_itFind)
	{
		if((*m_itFind)->IsSelect())
			return *m_itFind;
	}
	return NULL;
}

//아이템을 처음부터 검색하는 함수
//GetFirstItem을 먼저 호출하여 첫번째 아이템을 구해오고, 그 다음 부터는 GetNextItem 함수로 
//차례로 아이템을 구해오면 된다.
CListItemData *CList::GetFirstItem(void)
{
	DEF_CHECK_REPOSITION;
	m_itFind = m_listData.begin();
	LISTITEMDATAITER end = m_listData.end();

	for(;m_itFind != end;++m_itFind)
	{
		return *m_itFind;
	}

	return NULL;
}

CListItemData *CList::GetNextItem(void)
{
	DEF_CHECK_REPOSITION;
	LISTITEMDATAITER end = m_listData.end();
	if(m_itFind == end) return NULL;
	++m_itFind;
	if(m_itFind != end)	return *m_itFind;
	return NULL;
}

void CList::SetScrollData(void)
{
	SetReposition(true);
	int nCount = (int)m_listData.size();
	if(nCount == 0)
	{
		if(m_pScroll)
		{
			m_pScroll->SetRangeValue(0.0f, 0.0f);
			m_pScroll->SetPageValue(1.0f);
			m_pScroll->SetCurValue(0.0f);
			m_pScroll->SetEnable(false);
		}
		m_nScrollPos = 0;
		return;
	}

	if(m_nScrollPos < 0) m_nScrollPos = 0;

	if(m_bVirtical)
	{
		int nTotalHeight = 0;
		int nTemp = nCount/m_nXNum;

		if(nCount%m_nXNum == 0)
		{
			nTotalHeight = (int)(nTemp * m_nItemHeight + (nTemp - 1) * m_ptItemGab.y);
		}
		else
		{
			nTotalHeight = (int)((nTemp + 1) * m_nItemHeight + nTemp * m_ptItemGab.y);
		}

		int nTotalViewHeight = m_nClipHeight;
		if(m_nClipHeight == 0)
			nTotalViewHeight = (int)(m_nYNum * m_nItemHeight + (m_nYNum - 1) * m_ptItemGab.y);

		if(nTotalHeight - nTotalViewHeight <= 0)
		{
			if(m_pScroll)
			{
				m_pScroll->SetRangeValue(0.0f, 0.0f);
				m_pScroll->SetPageValue(1.0f);
				m_pScroll->SetCurValue(0.0f);
				m_pScroll->SetEnable(false);
			}

			if(m_nScrollPos != 0)
			{
				m_nScrollPos = 0;
			}
		}
		else
		{
			if(m_pScroll) m_pScroll->SetEnable(true);
			if(nTotalHeight - nTotalViewHeight < m_nScrollPos)
			{
				m_nScrollPos = nTotalHeight - nTotalViewHeight;

				if(m_pScroll)
				{
					m_pScroll->SetRangeValue(0.0f, float(nTotalHeight - nTotalViewHeight));
					m_pScroll->SetPageValue(float(nTotalViewHeight));
					m_pScroll->SetCurValue(float(m_nScrollPos));
				}
			}
			else
			{
				if(m_pScroll)
				{
					m_pScroll->SetRangeValue(0.0f, float(nTotalHeight - nTotalViewHeight));
					m_pScroll->SetPageValue(float(nTotalViewHeight));
					m_pScroll->SetCurValue(float(m_nScrollPos));
				}
			}
		}
	}
	else
	{
		int nTotalWidth = 0;
		int nTemp = nCount/m_nYNum;

		if(nCount%m_nYNum == 0)
		{
			nTotalWidth = (int)(nTemp * m_nItemWidth + (nTemp - 1) * m_ptItemGab.x);
		}
		else
		{
			nTotalWidth = (int)((nTemp + 1) * m_nItemWidth + nTemp * m_ptItemGab.x);
		}

		int nTotalViewWidth = m_nClipWidth;
		if(m_nClipWidth == 0)
			nTotalViewWidth = (int)(m_nXNum * m_nItemWidth + (m_nXNum - 1) * m_ptItemGab.x);

		if(nTotalWidth - nTotalViewWidth <= 0)
		{
			if(m_pScroll)
			{
				m_pScroll->SetRangeValue(0.0f, 0.0f);
				m_pScroll->SetPageValue(1.0f);
				m_pScroll->SetCurValue(0.0f);
				m_pScroll->SetEnable(false);
			}

			if(m_nScrollPos != 0)
			{
				m_nScrollPos = 0;
			}
		}
		else
		{
			if(m_pScroll) m_pScroll->SetEnable(true);
			if(nTotalWidth - nTotalViewWidth < m_nScrollPos)
			{
				m_nScrollPos = nTotalWidth - nTotalViewWidth;

				if(m_pScroll)
				{
					m_pScroll->SetRangeValue(0.0f, float(nTotalWidth - nTotalViewWidth));
					m_pScroll->SetPageValue(float(nTotalViewWidth));
					m_pScroll->SetCurValue(float(m_nScrollPos));
				}
			}
			else
			{
				if(m_pScroll)
				{
					m_pScroll->SetRangeValue(0.0f, float(nTotalWidth - nTotalViewWidth));
					m_pScroll->SetPageValue(float(nTotalViewWidth));
					m_pScroll->SetCurValue(float(m_nScrollPos));
				}
			}
		}
	}
}

void CList::CalculteClipRect(void)
{
	m_rtListRect.left = 0.0f;
	m_rtListRect.top = 0.0f;

	if(m_pSourceItem)
	{
		if(m_nClipWidth > 0) m_rtListRect.right = m_rtListRect.left + m_nClipWidth;
		else m_rtListRect.right = m_rtListRect.left + m_nItemWidth * m_nXNum + m_ptItemGab.x * (m_nXNum - 1);

		if(m_nClipHeight > 0) m_rtListRect.bottom = m_rtListRect.top + m_nClipHeight;
		else m_rtListRect.bottom = m_rtListRect.top + m_nItemHeight * m_nYNum + m_ptItemGab.y * (m_nYNum - 1);
	}
	else
	{
		m_rtListRect.right = m_rtListRect.left + m_nClipWidth;
		m_rtListRect.bottom = m_rtListRect.top + m_nClipHeight;
	}

	m_pItemMain->SetLocalRect(m_rtListRect);
	m_pItemMain->SetLocalClipRect(m_rtListRect);
}

CListItem *CList::GetNewItem(void)
{
	CListItem *pItem = NULL;
	if(m_listNotUseItems.size())
	{
		LISTITEMITER findIt = m_listNotUseItems.begin();
		pItem = *findIt;
		m_listNotUseItems.erase(findIt);
	}
	else
	{
		pItem = (CListItem *)m_pSourceItem->Clone(true, true, true, NULL);
		pItem->SetList(this);
	}

	return pItem;
}

void CList::Draw(NMBASE::GRAPHICGDI::CPage *pPage)
{
	DEF_CHECK_REPOSITION;
	CObjectBase::Draw(pPage);
}

void CList::RepositionItem(void)
{
	SetReposition(false);
	if(m_pSourceItem == NULL) return;
	if(m_listData.empty())
	{
		LISTITEMITER start_item = m_listItems.begin();
		LISTITEMITER end_item = m_listItems.end();

		for(;start_item != end_item;++start_item)
		{
			(*start_item)->SetAttach(false);
			m_pItemMain->SubChild(*start_item);
			(*start_item)->SetData(NULL);
			m_listNotUseItems.push_back(*start_item);
		}

		m_listItems.clear(); 
		return;
	}

	LISTITEMDATAITER start = m_listData.begin();
	LISTITEMDATAITER end = m_listData.end();

	CListItemData *pData = NULL;
	int nXNum = 0;
	int nYNum = 0;

	CPoint ptGab, ptGab2;
	bool bShow = false;
	CListItem *pItem = NULL;

	bool bFirst = true;

	LISTITEMITER start_item = m_listItems.begin();
	LISTITEMITER end_item = m_listItems.end();

	int nShowNum = 0;

	if(m_bVirtical)
	{
		for(;start != end;++start,++nXNum)
		{
			if(nXNum == m_nXNum)
			{
				++nYNum;
				nXNum = 0;
			}

			ptGab.x = nXNum * (m_nItemWidth + m_ptItemGab.x);
			ptGab.y = nYNum * (m_nItemHeight + m_ptItemGab.y) - m_nScrollPos;

			pData = *start;

			ptGab2.x = ptGab.x;
			ptGab2.y = ptGab.y;

			bShow = false;

			if((ptGab2.y >= m_rtListRect.top) && (ptGab2.y < m_rtListRect.bottom))
			{
				bShow = true;
			}
			else
			{
				ptGab2.y += (m_nItemHeight - 1);
				if((ptGab2.y >= m_rtListRect.top) && (ptGab2.y < m_rtListRect.bottom))
				{
					bShow = true;
				}
			}

			if(bShow)
			{
				if(bFirst && m_bLineScroll)
				{
					m_nScrollPos = (int)(nYNum * (m_nItemHeight + m_ptItemGab.y));

					ptGab.x = nXNum * (m_nItemWidth + m_ptItemGab.x);
					ptGab.y = nYNum * (m_nItemHeight + m_ptItemGab.y) - m_nScrollPos;
				}

				bFirst = false;

				if(start_item != end_item)
				{
					pItem = *start_item;
					++start_item;
				}
				else
				{
					pItem = GetNewItem();
					pItem->SetAttach(true);
					m_pItemMain->AddLocalChild(pItem);
					m_listItems.push_back(pItem);
				}

				if(pItem->GetData() != pData)
				{
					pData->SetItem(pItem);
					pItem->SetData(pData);
					UpdateItem(pItem);
				}

				pData->GetItem()->SetLocalPos(ptGab.x, ptGab.y);
			}
			else
			{
				if(pData->GetItem())
				{
					pData->SetItem(NULL);
				}
			}
		}
	}
	else
	{
		for(;start != end;++start,++nYNum)
		{
			if(nYNum == m_nYNum)
			{
				++nXNum;
				nYNum = 0;
			}

			ptGab.x = nXNum * (m_nItemWidth + m_ptItemGab.x) - m_nScrollPos;
			ptGab.y = nYNum * (m_nItemHeight + m_ptItemGab.y);

			pData = *start;

			ptGab2.x = ptGab.x;
			ptGab2.y = ptGab.y;

			bShow = false;

			if((ptGab2.x >= m_rtListRect.left) && (ptGab2.x < m_rtListRect.right))
			{
				bShow = true;
			}
			else
			{
				ptGab2.x += (m_nItemWidth - 1);
				if((ptGab2.x >= m_rtListRect.left) && (ptGab2.x < m_rtListRect.right))
				{
					bShow = true;
				}
			}

			if(bShow)
			{
				if(bFirst && m_bLineScroll)
				{
					m_nScrollPos = (int)(nXNum * (m_nItemWidth + m_ptItemGab.x));

					ptGab.x = nXNum * (m_nItemWidth + m_ptItemGab.x) - m_nScrollPos;
					ptGab.y = nYNum * (m_nItemHeight + m_ptItemGab.y);
				}

				bFirst = false;

				if(start_item != end_item)
				{
					pItem = *start_item;
					++start_item;
				}
				else
				{
					pItem = GetNewItem();
					pItem->SetAttach(true);
					m_pItemMain->AddLocalChild(pItem);
					m_listItems.push_back(pItem);
				}

				if(pItem->GetData() != pData)
				{
					pData->SetItem(pItem);
					pItem->SetData(pData);
					UpdateItem(pItem);
				}

				pData->GetItem()->SetLocalPos(ptGab.x, ptGab.y);
			}
			else
			{
				//보이지 않지만 아이템이 있으면 제거한다.
				if(pData->GetItem())
				{
					pData->SetItem(NULL);
				}
			}
		}
	}

	while(start_item != end_item)
	{
		CListItem *pItem = *start_item;
		pItem->SetAttach(false);
		m_pItemMain->SubChild(pItem);
		m_listNotUseItems.push_back(pItem);
		pItem->SetData(NULL);
		start_item = m_listItems.erase(start_item);
	}
}

LRESULT CList::OnLocalChildNotify(CMessage *msg)
{
	CObjectBase *pObject = ((CObjectMessage *)msg)->GetObject();
	if(pObject == m_pScroll)
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

LRESULT CList::OnListItemNotify(CMessage *pMsg)
{
	CMO_LocalChildNotify *msg = (CMO_LocalChildNotify *)pMsg;
	CObjectMessage *msg2 = (CObjectMessage *)msg->GetMessage();

	CObjectBase *pObject = msg2->GetObject();

	if(pObject->GetType() == EOT_LISTITEM)
	{
		CListItemData *pData = ((CListItem *)pObject)->GetData();
		if(pData == NULL) return FALSE;

		switch(msg2->GetID())
		{
		case EM_O_MOUSELCLICK:
			{
				if(m_bSingleSelect)
				{
					if(pData->IsSelect() == false)
					{
						CListItemData *pSelData = GetFirstSelectItem();
						if(pSelData != pData)
						{
							SetItemSelect(pData, true);
						}
					}
				}
				else
				{
					bool bPushCtrl = (GetKeyState(VK_LCONTROL) & 0xff00)?true:false;
					if(bPushCtrl)
					{
						SetItemSelect(pData, pData->IsSelect()?false:true);
					}
					else
					{
						ClearAllSelect();
						SetItemSelect(pData, true);
					}
				}
				CMO_ListItemClick msgclick(this, pData);
				Dispatch(&msgclick);
			}
			break;
		case EM_O_MOUSERCLICK:
			{
				CMO_ListItemRClick msgclick(this, pData);
				Dispatch(&msgclick);
			}
			break;
		case EM_O_LBUTTONDBCLICK:
			{
				CMO_ListItemDbClick msgclick(this, pData);
				Dispatch(&msgclick);
			}
			break;
		case EM_O_RBUTTONDBCLICK:
			{
				CMO_ListItemRDbClick msgclick(this, pData);
				Dispatch(&msgclick);
			}
			break;
		// MouseOver 추가 [12/14/2011 guesty]
		case EM_O_MOUSEOVER:
			{
				CMO_ListItemMouseOver msgclick(this, pData);
				Dispatch(&msgclick);
			}
			break;
		// MouseLeave 추가 [12/14/2011 guesty]
		case EM_O_MOUSELEAVE:
			{
				CMO_ListItemMouseLeave msgclick(this, pData);
				Dispatch(&msgclick);
			}
			break;
		}
	}

	return FALSE;
}

void CList::CheckUp(void)
{
	if(m_nScrollPos == 0) return;
	if(m_listItems.size() == 0) return;

	CListItem *pFirstItem = *(m_listItems.begin());
	int nIndex = pFirstItem->GetData()->GetItemIndex();
	CPoint ptStart;
	int nXNum, nYNum;

	if(m_bVirtical)
	{
		nXNum = nIndex%m_nXNum;
		nYNum = nIndex/m_nXNum;
	}
	else
	{
		nYNum = nIndex%m_nYNum;
		nXNum = nIndex/m_nYNum;
	}

	ptStart.x = nXNum * (m_nItemWidth + m_ptItemGab.x);
	ptStart.y = nYNum * (m_nItemHeight + m_ptItemGab.y);
	int nScroll = 0;

	if(m_bVirtical)
	{
		nScroll = (int)ptStart.y;
	}
	else
	{
		nScroll = (int)ptStart.x;
	}

	
	if(m_bVirtical)
	{
		if(nYNum != 0)
		{
			nScroll -= m_nItemHeight + (int)m_ptItemGab.y;
		}
	}
	else
	{
		if(nXNum != 0)
		{
			nScroll -= m_nItemWidth + (int)m_ptItemGab.x;
		}
	}

	if(nScroll != m_nScrollPos)
	{
		m_nScrollPos = nScroll;
		m_pScroll->SetCurValue((float)nScroll);
	}
	else return;

	SetReposition(true);
}

void CList::GetNextItemIter(LISTITEMITER &it, int nNext)
{
	ASSERT(nNext > 0);
	if(it == m_listItems.end()) return;

	for(int i=0;i<nNext;++i)
	{
		++it;
		if(it == m_listItems.end()) return;
	}
}

void CList::CheckDown(void)
{
	if(m_nScrollPos >= m_pScroll->GetMaxValue()) return;
	if(m_listItems.size() == 0) return;

	CListItem *pFirstItem = *(m_listItems.begin());
	int nIndex = pFirstItem->GetData()->GetItemIndex();
	CPoint ptStart;
	int nXNum, nYNum;

	if(m_bVirtical)
	{
		nXNum = nIndex%m_nXNum;
		nYNum = nIndex/m_nXNum;
	}
	else
	{
		nYNum = nIndex%m_nYNum;
		nXNum = nIndex/m_nYNum;
	}

	ptStart.x = nXNum * (m_nItemWidth + m_ptItemGab.x);
	ptStart.y = nYNum * (m_nItemHeight + m_ptItemGab.y);
	int nScroll = 0;

	if(m_bVirtical)
	{
		nScroll = (int)ptStart.y;
	}
	else
	{
		nScroll = (int)ptStart.x;
	}

	LISTITEMITER findIt = find(m_listItems.begin(), m_listItems.end(), pFirstItem);
	if(findIt != m_listItems.end())
	{
		if(m_bVirtical)
		{
			GetNextItemIter(findIt, m_nXNum);
		}
		else
		{
			GetNextItemIter(findIt, m_nYNum);
		}

		if(findIt == m_listItems.end())
		{
			m_nScrollPos = nScroll;
			m_pScroll->SetCurValue((float)nScroll);
		}
		else
		{
			if(m_pScroll->GetMaxValue() <= (float)nScroll)
			{
				m_nScrollPos = (int)m_pScroll->GetMaxValue();
				m_pScroll->SetCurValue((float)m_nScrollPos);
			}
			else
			{
				if(m_bVirtical)
				{
					nScroll += m_nItemHeight + (int)m_ptItemGab.y;
				}
				else
				{
					nScroll += m_nItemWidth + (int)m_ptItemGab.x;
				}

				m_nScrollPos = nScroll;
				if(m_pScroll->GetMaxValue() <= (float)nScroll) m_nScrollPos = (int)m_pScroll->GetMaxValue();
				m_pScroll->SetCurValue((float)m_nScrollPos);
			}
		}

		SetReposition(true);
	}
}

LRESULT CList::OnClickUp(CMessage *pMsg)
{
	if(!m_bLineScroll) return FALSE;
	if(m_pScroll->IsIncrease()) CheckDown();
	else CheckUp();
	return TRUE;
}

LRESULT CList::OnClickDown(CMessage *pMsg)
{
	if(!m_bLineScroll) return FALSE;
	if(m_pScroll->IsIncrease()) CheckUp();
	else CheckDown();
	return TRUE;
}

void CList::CheckPageUp(void)
{
	if(m_nScrollPos == 0) return;

	if(m_bVirtical) m_nScrollPos -= (int)m_rtListRect.GetHeight();
	else m_nScrollPos -= (int)m_rtListRect.GetWidth();

	if(m_nScrollPos < 0) m_nScrollPos = 0;
	m_pScroll->SetCurValue((float)m_nScrollPos);
	SetReposition(true);
}

void CList::CheckPageDown(void)
{
	if(m_nScrollPos >= m_pScroll->GetMaxValue()) return;

	if(m_bVirtical) m_nScrollPos += (int)m_rtListRect.GetHeight();
	else m_nScrollPos += (int)m_rtListRect.GetWidth();

	if(m_nScrollPos > m_pScroll->GetMaxValue()) m_nScrollPos = (int)m_pScroll->GetMaxValue();
	m_pScroll->SetCurValue((float)m_nScrollPos);
	SetReposition(true);
}

LRESULT CList::OnClickPageUp(CMessage *pMsg)
{
	if(!m_bLineScroll) return FALSE;
	if(m_pScroll->IsIncrease()) CheckPageDown();
	else CheckPageUp();
	SetReposition(true);
	return TRUE;
}

LRESULT CList::OnClickPageDown(CMessage *pMsg)
{
	if(!m_bLineScroll) return FALSE;
	if(m_pScroll->IsIncrease()) CheckPageUp();
	else CheckPageDown();
	SetReposition(true);
	return TRUE;
}

LRESULT CList::OnClickValueChange(CMessage *pMsg)
{
	CMO_CurValueChange *msg = (CMO_CurValueChange *)pMsg;
	float fCurValue = msg->GetValue();
	m_nScrollPos = (int)fCurValue;

	if(m_pProject)
	{
		CObjectMan *pManager = m_pProject->GetObjectMan();
		if(pManager)
		{
			::CPoint ptPos;
			GetCursorPos(&ptPos);
			pManager->GetWnd()->ScreenToClient(&ptPos);

			LPARAM lParam = MAKELPARAM(ptPos.x, ptPos.y);
			pManager->GetWnd()->PostMessage(WM_MOUSEMOVE, 0, lParam);
		}
	}

	SetReposition(true);
	return TRUE;
}

void CList::CheckReposition(void)
{
	DEF_CHECK_REPOSITION;
}

LRESULT CList::OnFocusInLocalChild(CMessage *pMsg)
{
	return (LRESULT)this;
}

void CList::MakeSample(void)
{
	DeleteAllItem();

	LISTITEMITER start = m_listNotUseItems.begin();
	LISTITEMITER end = m_listNotUseItems.end();

	for(;start != end;++start)
	{
		delete (*start);
	}

	m_listNotUseItems.clear();

	for(int i=0;i<100;++i)
	{
		AddItem(NULL);
	}
}

int CList::AddItem(LPARAM lParam)
{
	if(m_pSourceItem == NULL) return -1;

	CListItemData *pData = new CListItemData;
	pData->SetList(this);
	pData->SetItemData(lParam);

	m_listData.push_back(pData);
	int nRet = (int)m_listData.size() - 1;
	pData->SetItemIndex(nRet);

	if(IsAutoSort()) nRet = Sort(pData);

	SetScrollData();

	return nRet;
}

int CList::AddString(string strText)
{
	if(m_pSourceItem == NULL) return -1;

	CListItemData *pData = new CListItemData;
	pData->SetText(strText);

	m_listData.push_back(pData);
	int nRet = (int)m_listData.size() - 1;
	pData->SetItemIndex(nRet);

	if(IsAutoSort()) nRet = Sort(pData);

	SetScrollData();

	return nRet;
}

CListItem *GetListItem(CMessage *pMsg)
{
	CObjectMessage *msg = dynamic_cast<CObjectMessage *>(pMsg);
	if(msg == NULL) return NULL;
	CObjectBase *pObject = msg->GetObject();
	if(pObject == NULL) return NULL;
	CObjectBase *pItem = pObject->GetObserverObject();
	if(pItem == NULL) return NULL;
	if(pItem->GetType() != EOT_LISTITEM) return NULL;
	return (CListItem *)pItem;
}

CListItem *GetListItem(CObjectBase *pObject)
{
	if(pObject == NULL) return NULL;
	CObjectBase *pItem = pObject->GetObserverObject();
	if(pItem == NULL) return NULL;
	if(pItem->GetType() != EOT_LISTITEM) return NULL;
	return (CListItem *)pItem;
}

CListItemData *GetListItemData(CMessage *pMsg)
{
	CListItem *pItem = GetListItem(pMsg);
	if(pItem == NULL) return NULL;
	return pItem->GetData();
}

CListItemData *GetListItemData(CObjectBase *pObject)
{
	CListItem *pItem = GetListItem(pObject);
	if(pItem == NULL) return NULL;
	return pItem->GetData();
}

int GetListItemIndex(CMessage *pMsg)
{
	CListItemData *pData = GetListItemData(pMsg);
	if(pData == NULL) return -1;
	return pData->GetItemIndex();
}
} //namespace LSY