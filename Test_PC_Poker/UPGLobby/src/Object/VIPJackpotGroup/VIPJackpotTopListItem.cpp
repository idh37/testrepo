#include "StdAfx.h"
#include "VIPJackpotTopListItem.h"
#include "../../WidgetID.h"

CVIPJackpotTopListItem::CVIPJackpotTopListItem()
: m_pNum(NULL)
, m_pMoney(NULL)
, m_pID(NULL)
, m_pGame(NULL)
, m_pDate(NULL)
{

}

CVIPJackpotTopListItem::~CVIPJackpotTopListItem()
{

}

void CVIPJackpotTopListItem::OnCreateChild( LSY::CObjectBase *pObject )
{
	if(pObject == NULL) 
		return;

	switch(pObject->GetID())
	{
	case ID_LOBBY_TXTNB_VIPJACKPOT_TOP_NUM:
		m_pNum = (LSY::CTextNumber*)pObject;
		break;
	case ID_LOBBY_TXTNB_VIPJACKPOT_TOP_MONEY:
		m_pMoney = (LSY::CTextNumber*)pObject;
		break;
	case ID_LOBBY_TXT_VIPJACKPOT_TOP_ID:
		m_pID = (LSY::CText*)pObject;
		break;
	case ID_LOBBY_TXT_VIPJACKPOT_TOP_GAME:
		m_pGame = (LSY::CText*)pObject;
		break;
	case ID_LOBBY_TXT_VIPJACKPOT_TOP_DATE:
		m_pDate = (LSY::CText*)pObject;
		break;
	}

	__super::OnCreateChild(pObject);
}

void CVIPJackpotTopListItem::OnCreated( void )
{

}

LSY::CObjectBase * CVIPJackpotTopListItem::NewObject( void )
{
	return new CVIPJackpotTopListItem;
}

void CVIPJackpotTopListItem::UpdateData( const LPARAM &lParam )
{
	LSY::CListItemData *pListItemData = (LSY::CListItemData*)GetData();
	if (NULL == pListItemData)
		return;

	CVIPJackpotTopListItem *pInfo = reinterpret_cast<CVIPJackpotTopListItem*>(pListItemData->GetItemData());
	if(NULL == pInfo) 
		return;
}

void CVIPJackpotTopListItem::SetBarType( int nStartIndex )
{

}

