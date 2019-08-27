#include "stdafx.h"
#include "ListItem.h"
#include "List.h"
#include "../../Functor/Message.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace LSY
{
CListItemData::CListItemData()
:	m_lParam(NULL),
	m_nIndex(-1),
	m_strText(""),
	m_bSelect(false),
	m_bEnable(true),
	m_pItem(NULL),
	m_pList(NULL)
{
}

CListItemData::~CListItemData()
{
}

void CListItemData::SetText(const std::string &strText)
{
	if(m_strText == strText) return;
	m_strText = strText;
}

CListItem *CListItemData::GetItem(void)
{
	if(m_pList) m_pList->CheckReposition();
	return m_pItem;
}

void CListItemData::SetUserData(int nIndex, LPARAM lParam)
{
	m_mapUserData[nIndex] = lParam;
}

LPARAM CListItemData::GetUserData(int nIndex)
{
	if(m_mapUserData.find(nIndex) != m_mapUserData.end())
	{
		return m_mapUserData[nIndex];
	}

	return 0;
}

CListItem::CListItem()
:	m_pData(NULL),
	m_bAttach(false),
	m_pList(NULL)
{
}

CListItem::~CListItem()
{
}

CObjectBase *CListItem::NewObject(void)
{
	return new CListItem;
}

CObjectBase *CListItem::Copy(CObjectBase *pObject)
{
	CCheckButton::Copy(pObject);

	CListItem *pList		= (CListItem *)pObject;
	return pObject;
}

//Attach가 되었을 때 클릭에 의해서 자동으로 체크 상태가 변경되지 않도록 수정한다.
void CListItem::OnMouseLClick(const CPoint &ptPos)
{
	if(m_bAttach) CButton::OnMouseLClick(ptPos);
	else CCheckButton::OnMouseLClick(ptPos);
}

LRESULT CListItem::OnObserverNotify(CMessage *msg)
{
	//하위 오브젝트에서 전달되어 지는 메시지를 감시한다.
	//하위 오브젝트에 Mouse Over/Leave가 온다면 리스트아이템에 Mouse Over/Leave가 일어난걸로 처리한다.
	switch(msg->GetID())
	{
	case EM_O_MOUSEOVER:
		OnMouseOver();
		break;
	case EM_O_MOUSELEAVE:
		OnMouseLeave();
		break;
	}

	return CCheckButton::OnObserverNotify(msg);
}

CListItemData *CListItem::GetData(void)
{
	if(m_pList) m_pList->CheckReposition();
	return m_pData;
}
} //namespace LSY