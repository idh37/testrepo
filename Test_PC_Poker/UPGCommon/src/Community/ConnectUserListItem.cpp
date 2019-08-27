#include "StdAfx.h"
#include "ConnectUserListItem.h"
#include "AddUserSelectDlgID.h"
#include "AddUserSelectDlgConnectUserList.h"

CConnectUserListItem::CConnectUserListItem()
:	m_pCheckSelect(NULL),
	m_pTxtNickName(NULL),
	m_pTxtnbHaveMoney(NULL),
	m_pImgLocation(NULL),
	m_pTxtLocationGame(NULL),
	m_pTxtnbLocationRoomNumber(NULL)
{
}

CConnectUserListItem::~CConnectUserListItem()
{
}

void CConnectUserListItem::OnCreateChild(LSY::CObjectBase *pObject)
{
	if(pObject == NULL) return;
	switch(pObject->GetID())
	{
	case ID_ADDUSERSELECTDLG_MY_CONNECT_LISTITEM_CHECKBOX_SELECT:		m_pCheckSelect					= (LSY::CCheckButton *)pObject;		break;
	case ID_ADDUSERSELECTDLG_MY_CONNECT_LISTITEM_TXT_NICKNAME:			m_pTxtNickName					= (LSY::CText *)pObject;			break;
	case ID_ADDUSERSELECTDLG_MY_CONNECT_LISTITEM_TXTNB_HAVE_MONEY:		m_pTxtnbHaveMoney				= (LSY::CTextNumber *)pObject;		break;
	case ID_ADDUSERSELECTDLG_MY_CONNECT_LISTITEM_IMG_LOCATION_BACK:		m_pImgLocation					= (LSY::CImage *)pObject;			break;
	case ID_ADDUSERSELECTDLG_MY_CONNECT_LISTITEM_TXT_LOCATION:			m_pTxtLocationGame				= (LSY::CText *)pObject;			break;
	case ID_ADDUSERSELECTDLG_MY_CONNECT_LISTITEM_TXTNB_ROOM_NUMBER:		m_pTxtnbLocationRoomNumber		= (LSY::CText *)pObject;			break;
	}
	__super::OnCreateChild(pObject);
}

void CConnectUserListItem::OnCreated(void)
{
	OnCreateChild(m_pImgLocation->GetFirstChildFromID(ID_ADDUSERSELECTDLG_MY_CONNECT_LISTITEM_TXT_LOCATION));
	OnCreateChild(m_pImgLocation->GetFirstChildFromID(ID_ADDUSERSELECTDLG_MY_CONNECT_LISTITEM_TXTNB_ROOM_NUMBER));
	m_pCheckSelect->SetCheck(false);
	__super::OnCreated();
}

LSY::CObjectBase *CConnectUserListItem::NewObject(void)
{
	return new CConnectUserListItem;
}

void CConnectUserListItem::UpdateData(const LPARAM &lParam)
{
	SMALLUSERINFO *pUserInfo = (SMALLUSERINFO *)m_pData->GetItemData();
	if(pUserInfo == NULL) return;

	bool bCheck = m_pData->GetUserData(_DEF_USER_DATA_BUDDYITEM_TYPE_CHECK)?true:false;
	m_pCheckSelect->SetCheck(bCheck);

	m_pTxtNickName->SetText(pUserInfo->NickName);
	IDX_GAME idxGame = (IDX_GAME)pUserInfo->nGameIndex;

	CGame *pGame = GM().GetGame(idxGame);
	if(pGame)
	{
		m_pTxtLocationGame->SetTextColor(pGame->GetGameColor());
		m_pTxtLocationGame->SetText(pGame->GetGameName().GetString());
	}

	if(pUserInfo->RoomNum == 0)
	{
		m_pTxtnbLocationRoomNumber->SetText("대기실");
	}
	else
	{
		CString strText;
		strText.Format("%d번", pUserInfo->RoomNum);
		m_pTxtnbLocationRoomNumber->SetText(strText.GetString());
	}

	m_pTxtnbHaveMoney->SetNumber(pUserInfo->PMoney);
}

LRESULT CConnectUserListItem::OnCheckChange(LSY::CMessage *msg)
{
	LSY::CMO_CheckChange *pMsg = (LSY::CMO_CheckChange *)msg;
	LSY::CListItem *pItem = LSY::GetListItem(pMsg);
	if(pItem == NULL) return FALSE;
	LSY::CListItemData *pItemData = pItem->GetData();
	if(pItemData == NULL) return FALSE;

	////0이면 expand임
	//bool bCheck = pItemData->GetUserData(_DEF_USER_DATA_BUDDYITEM_TYPE_CHECK)?false:true;
	//pItemData->SetUserData(_DEF_USER_DATA_BUDDYITEM_TYPE_CHECK, (LPARAM)bCheck?1:0);

	CAddUserSelectDlgConnectUserList *pList = (CAddUserSelectDlgConnectUserList *)pItem->GetList();
	pList->ClearAllItemCheck();

	bool bCheck = pItemData->GetUserData(_DEF_USER_DATA_BUDDYITEM_TYPE_CHECK)?false:true;
	pItemData->SetUserData(_DEF_USER_DATA_BUDDYITEM_TYPE_CHECK, (LPARAM)bCheck?1:0);
	pList->UpdateItemFromParam(pItemData->GetItemData()) ;

	return TRUE;
}