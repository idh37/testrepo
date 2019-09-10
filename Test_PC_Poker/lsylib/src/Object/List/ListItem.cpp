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

//Attach�� �Ǿ��� �� Ŭ���� ���ؼ� �ڵ����� üũ ���°� ������� �ʵ��� �����Ѵ�.
void CListItem::OnMouseLClick(const CPoint &ptPos)
{
	if(m_bAttach) CButton::OnMouseLClick(ptPos);
	else CCheckButton::OnMouseLClick(ptPos);
}

LRESULT CListItem::OnObserverNotify(CMessage *msg)
{
	//���� ������Ʈ���� ���޵Ǿ� ���� �޽����� �����Ѵ�.
	//���� ������Ʈ�� Mouse Over/Leave�� �´ٸ� ����Ʈ�����ۿ� Mouse Over/Leave�� �Ͼ�ɷ� ó���Ѵ�.
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