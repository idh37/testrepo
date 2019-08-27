#include "StdAfx.h"
#include "VIPJackpotWinListItem.h"
#include "../../WidgetID.h"

CVIPJackpotWinListItem::CVIPJackpotWinListItem()
: m_pNum(NULL)
, m_pMoney(NULL)
, m_pID(NULL)
, m_pGame(NULL)
, m_pRound(NULL)
{

}

CVIPJackpotWinListItem::~CVIPJackpotWinListItem()
{

}

void CVIPJackpotWinListItem::OnCreateChild( LSY::CObjectBase *pObject )
{
	if(pObject == NULL) 
		return;

	switch(pObject->GetID())
	{
	case ID_LOBBY_TXTNB_VIPJACKPOT_WIN_NUM:
		m_pNum = (LSY::CTextNumber*)pObject;
		break;
	case ID_LOBBY_TXTNB_VIPJACKPOT_WIN_MONEY:
		m_pMoney = (LSY::CTextNumber*)pObject;
		break;
	case ID_LOBBY_TXT_VIPJACKPOT_WIN_ID:
		m_pID = (LSY::CText*)pObject;
		break;
	case ID_LOBBY_TXT_VIPJACKPOT_WIN_GAME:
		m_pGame = (LSY::CText*)pObject;
		break;
	case ID_LOBBY_TXT_VIPJACKPOT_WIN_ROUND:
		m_pRound = (LSY::CText*)pObject;
		break;

	}

	__super::OnCreateChild(pObject);
}

void CVIPJackpotWinListItem::OnCreated( void )
{

}

LSY::CObjectBase * CVIPJackpotWinListItem::NewObject( void )
{
	return new CVIPJackpotWinListItem;
}

void CVIPJackpotWinListItem::UpdateData( const LPARAM &lParam )
{
	LSY::CListItemData *pListItemData = (LSY::CListItemData*)GetData();
	if (NULL == pListItemData)
		return;

	CVIPJackpotWinListItem *pInfo = reinterpret_cast<CVIPJackpotWinListItem*>(pListItemData->GetItemData());
	if(NULL == pInfo) 
		return;


}

void CVIPJackpotWinListItem::SetBarType( int nStartIndex )
{

}
