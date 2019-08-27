#include "StdAfx.h"
#include "BuddyListItem.h"
#include "adduserselectdlgid.h"
#include "Buddy.h"
#include "AddUserSelectDlgBuddyList.h"

CBuddyListItem::CBuddyListItem()
:	m_pGrpBuddyGroup(NULL),
	m_pCheckGroupSelect(NULL),
	m_pTxtGroupName(NULL),
	m_pGrpBuddy(NULL),
	m_pCheckBuddySelect(NULL),
	m_pTxtBuddyNickName(NULL),
	m_pTxtnbBuddyHaveMoney(NULL),
	m_pImgBuddyLocation(NULL),
	m_pTxtBuddyLocationGame(NULL),
	m_pTxtnbBuddyLocationRoomNumber(NULL),
	m_pTxtBuddyOffline(NULL),
	m_pImgBuddyOnOffline(NULL)
{
}

CBuddyListItem::~CBuddyListItem()
{
}

void CBuddyListItem::OnCreateChild(LSY::CObjectBase *pObject)
{
	if(pObject == NULL) return;
	switch(pObject->GetID())
	{
	case ID_ADDUSERSELECTDLG_MY_FRIEND_LISTITEM_GRP_GROUP:				m_pGrpBuddyGroup				= (LSY::CGroup *)pObject;			break;
	case ID_ADDUSERSELECTDLG_MY_FRIEND_LISTITEM_CHECKBOX_GROUP_SELECT:	m_pCheckGroupSelect				= (LSY::CCheckButton *)pObject;		break;
	case ID_ADDUSERSELECTDLG_MY_FRIEND_LISTITEM_TXT_GROUP_NAME:			m_pTxtGroupName					= (LSY::CText *)pObject;			break;
	case ID_ADDUSERSELECTDLG_MY_FRIEND_LISTITEM_GRP_FRIEND:				m_pGrpBuddy						= (LSY::CGroup *)pObject;			break;
	case ID_ADDUSERSELECTDLG_MY_FRIEND_LISTITEM_CHECKBOX_BUDDY_SELECT:	m_pCheckBuddySelect				= (LSY::CCheckButton *)pObject;		break;
	case ID_ADDUSERSELECTDLG_MY_FRIEND_LISTITEM_TXT_NICKNAME:			m_pTxtBuddyNickName				= (LSY::CText *)pObject;			break;
	case ID_ADDUSERSELECTDLG_MY_FRIEND_LISTITEM_TXTNB_HAVE_MONEY:		m_pTxtnbBuddyHaveMoney			= (LSY::CTextNumber *)pObject;		break;
	case ID_ADDUSERSELECTDLG_MY_FRIEND_LISTITEM_IMG_LOCATION_BACK:		m_pImgBuddyLocation				= (LSY::CImage *)pObject;			break;
	case ID_ADDUSERSELECTDLG_MY_FRIEND_LISTITEM_TXT_LOCATION:			m_pTxtBuddyLocationGame			= (LSY::CText *)pObject;			break;
	case ID_ADDUSERSELECTDLG_MY_FRIEND_LISTITEM_TXTNB_ROOM_NUMBER:		m_pTxtnbBuddyLocationRoomNumber	= (LSY::CText *)pObject;			break;
	case ID_ADDUSERSELECTDLG_MY_FRIEND_LISTITEM_TXT_OFFLINE:			m_pTxtBuddyOffline				= (LSY::CText *)pObject;			break;
	case ID_ADDUSERSELECTDLG_MY_FRIEND_LISTITEM_IMG_ONOFFLINE:			m_pImgBuddyOnOffline			= (LSY::CImage *)pObject;			break;
	}
	__super::OnCreateChild(pObject);
}

void CBuddyListItem::OnCreated(void)
{
	OnCreateChild(m_pGrpBuddyGroup->GetFirstChildFromID(ID_ADDUSERSELECTDLG_MY_FRIEND_LISTITEM_CHECKBOX_GROUP_SELECT));
	OnCreateChild(m_pGrpBuddyGroup->GetFirstChildFromID(ID_ADDUSERSELECTDLG_MY_FRIEND_LISTITEM_TXT_GROUP_NAME));
	OnCreateChild(m_pGrpBuddy->GetFirstChildFromID(ID_ADDUSERSELECTDLG_MY_FRIEND_LISTITEM_TXT_NICKNAME));
	OnCreateChild(m_pGrpBuddy->GetFirstChildFromID(ID_ADDUSERSELECTDLG_MY_FRIEND_LISTITEM_CHECKBOX_BUDDY_SELECT));
	OnCreateChild(m_pGrpBuddy->GetFirstChildFromID(ID_ADDUSERSELECTDLG_MY_FRIEND_LISTITEM_TXTNB_HAVE_MONEY));
	OnCreateChild(m_pGrpBuddy->GetFirstChildFromID(ID_ADDUSERSELECTDLG_MY_FRIEND_LISTITEM_IMG_LOCATION_BACK));
	OnCreateChild(m_pImgBuddyLocation->GetFirstChildFromID(ID_ADDUSERSELECTDLG_MY_FRIEND_LISTITEM_TXT_LOCATION));
	OnCreateChild(m_pImgBuddyLocation->GetFirstChildFromID(ID_ADDUSERSELECTDLG_MY_FRIEND_LISTITEM_TXTNB_ROOM_NUMBER));
	OnCreateChild(m_pGrpBuddy->GetFirstChildFromID(ID_ADDUSERSELECTDLG_MY_FRIEND_LISTITEM_TXT_OFFLINE));
	OnCreateChild(m_pGrpBuddy->GetFirstChildFromID(ID_ADDUSERSELECTDLG_MY_FRIEND_LISTITEM_IMG_ONOFFLINE));

	__super::OnCreated();
}

LSY::CObjectBase *CBuddyListItem::NewObject(void)
{
	return new CBuddyListItem;
}

void CBuddyListItem::UpdateData(const LPARAM &lParam)
{
	CBuddyBase *pBuddy = (CBuddyBase *)m_pData->GetItemData();
	if(pBuddy == NULL) return;

	if(pBuddy->IsGroup())
	{
		UpdateBuddyGroup((CBuddyGroup *)pBuddy, m_pData);
	}
	else
	{
		UpdateBuddy((CBuddyFriend *)pBuddy, m_pData);
	}
}

void CBuddyListItem::UpdateBuddyGroup(CBuddyGroup *pBuddyGroup, LSY::CListItemData *pItemData)
{
	m_pGrpBuddy->SetShow(false);
	m_pGrpBuddyGroup->SetShow(true);

	bool bExpand = pItemData->GetUserData(_DEF_USER_DATA_BUDDYITEM_TYPE_EXPAND)?false:true;
	bool bCheck = pItemData->GetUserData(_DEF_USER_DATA_BUDDYITEM_TYPE_CHECK)?true:false;

	m_pCheckGroupSelect->SetCheck(bCheck);

	if(pBuddyGroup->IsSendChangeName())
	{
		CString strName;
		strName.Format("(%d/%d)", pBuddyGroup->GetOnlineCount(), pBuddyGroup->GetBuddyCount());
		m_pTxtGroupName->SetText(pBuddyGroup->GetEditName());
		m_pTxtGroupName->SetAddText(strName.GetString());
	}
	else
	{
		CString strName;
		strName.Format("(%d/%d)", pBuddyGroup->GetOnlineCount(), pBuddyGroup->GetBuddyCount());
		m_pTxtGroupName->SetText(pBuddyGroup->GetName());
		m_pTxtGroupName->SetAddText(strName.GetString());
	}
}

void CBuddyListItem::UpdateBuddy(CBuddyFriend *pBuddy, LSY::CListItemData *pItemData)
{
	m_pGrpBuddy->SetShow(true);
	m_pGrpBuddyGroup->SetShow(false);

	bool bCheck = pItemData->GetUserData(_DEF_USER_DATA_BUDDYITEM_TYPE_CHECK)?true:false;
	m_pCheckBuddySelect->SetCheck(bCheck);

	m_pTxtBuddyNickName->SetText(pBuddy->GetNickName());
	IDX_GAME idxGame = pBuddy->GetGame();

	bool bBlackList = CCommunityManager::Instance()->IsExistBlackBuddy(pBuddy->GetNickName())?true:false;

	if(pBuddy->IsOffline())
	{
		m_pTxtBuddyOffline->SetShow(true);
		m_pTxtnbBuddyHaveMoney->SetShow(false);
		m_pImgBuddyLocation->SetShow(false);

		m_pImgBuddyOnOffline->SetIndex(bBlackList?48:46);
	}
	else
	{
		m_pTxtBuddyOffline->SetShow(false);
		m_pTxtnbBuddyHaveMoney->SetShow(true);
		m_pImgBuddyLocation->SetShow(true);

		CGame *pGame = GM().GetGame(idxGame);
		if(pGame)
		{
			m_pTxtBuddyLocationGame->SetTextColor(pGame->GetGameColor());
			m_pTxtBuddyLocationGame->SetText(pGame->GetGameName().GetString());
		}

		if(pBuddy->GetRoomNumber() == 0)
		{
			m_pTxtnbBuddyLocationRoomNumber->SetText("대기실");
		}
		else
		{
			CString strText;
			strText.Format("%d번", pBuddy->GetRoomNumber());
			m_pTxtnbBuddyLocationRoomNumber->SetText(strText.GetString());
		}

		m_pTxtnbBuddyHaveMoney->SetNumber(pBuddy->GetMoney());
		m_pImgBuddyOnOffline->SetIndex(bBlackList?47:45);
	}
}

LRESULT CBuddyListItem::OnCheckChangeBuddyGroupExpand(LSY::CMessage *msg)
{
	LSY::CMO_CheckChange *pMsg = (LSY::CMO_CheckChange *)msg;
	LSY::CListItem *pItem = LSY::GetListItem(pMsg);
	if(pItem == NULL) return FALSE;
	LSY::CListItemData *pItemData = pItem->GetData();
	if(pItemData == NULL) return FALSE;

	CBuddyBase *pBuddyBase = (CBuddyBase *)pItemData->GetItemData();
	if(!pBuddyBase->IsGroup()) return FALSE;

	CBuddyGroup *pBuddyGroup = (CBuddyGroup *)pBuddyBase;

	//0이면 expand임
	bool bExpand = pItemData->GetUserData(_DEF_USER_DATA_BUDDYITEM_TYPE_EXPAND)?true:false;
	pItemData->SetUserData(_DEF_USER_DATA_BUDDYITEM_TYPE_EXPAND, (LPARAM)bExpand?0:1);
	
	if(bExpand)
	{
		pBuddyGroup->AllAddFriend(pItem->GetList());
	}
	else
	{
		pBuddyGroup->AllDeleteFriend(pItem->GetList());
	}

	return TRUE;
}

LRESULT CBuddyListItem::OnCheckChange(LSY::CMessage *msg)
{
	LSY::CMO_CheckChange *pMsg = (LSY::CMO_CheckChange *)msg;
	LSY::CListItem *pItem = LSY::GetListItem(pMsg);
	if(pItem == NULL) return FALSE;
	LSY::CListItemData *pItemData = pItem->GetData();
	if(pItemData == NULL) return FALSE;

	CAddUserSelectDlgBuddyList *pList = (CAddUserSelectDlgBuddyList *)pItem->GetList();

	CBuddyBase *pBuddyBase = (CBuddyBase *)pItemData->GetItemData();

	if( pBuddyBase->IsGroup())
	{
		pItemData->SetUserData(_DEF_USER_DATA_BUDDYITEM_TYPE_CHECK, (LPARAM)0);
		pList->UpdateItemFromParam(pItemData->GetItemData()) ;
	}else
	{
		pList->ClearAllItemCheck() ;

		bool bCheck = pItemData->GetUserData(_DEF_USER_DATA_BUDDYITEM_TYPE_CHECK)?false:true;
		pItemData->SetUserData(_DEF_USER_DATA_BUDDYITEM_TYPE_CHECK, (LPARAM)bCheck?1:0);
		pList->UpdateItemFromParam(pItemData->GetItemData()) ;
	}

	// 2013.5.1
	// 쪽지 발송을 1명만 가능하도록 기능 수정.
	//0이면 expand임
	/*bool bCheck = pItemData->GetUserData(_DEF_USER_DATA_BUDDYITEM_TYPE_CHECK)?false:true;
	pItemData->SetUserData(_DEF_USER_DATA_BUDDYITEM_TYPE_CHECK, (LPARAM)bCheck?1:0);

	CBuddyBase *pBuddyBase = (CBuddyBase *)pItemData->GetItemData();

	CBuddyGroup *pBuddyGroup = NULL;
	CBuddyFriend *pBuddy = NULL;
	LSY::CListItemData *pTempItemData = NULL;
	std::list<CBuddyFriend *>::const_iterator start, end;

	CAddUserSelectDlgBuddyList *pList = (CAddUserSelectDlgBuddyList *)pItem->GetList();

	if(pBuddyBase->IsGroup())
	{
		pBuddyGroup = (CBuddyGroup *)pBuddyBase;
		start = pBuddyGroup->GetBuddyList().begin();
		end = pBuddyGroup->GetBuddyList().end();

		for(;start != end;++start)
		{
			pBuddy = *start;
			pTempItemData = pList->GetItemFromParam((LPARAM)pBuddy);

			if(pTempItemData->GetUserData(_DEF_USER_DATA_BUDDYITEM_TYPE_CHECK)?1:0 != bCheck)
			{
				pTempItemData->SetUserData(_DEF_USER_DATA_BUDDYITEM_TYPE_CHECK, (LPARAM)bCheck?1:0);
				pList->UpdateItemFromParam((LPARAM)pBuddy);
			}
		}
	}
	else
	{
		pBuddy = (CBuddyFriend *)pBuddyBase;
		pBuddyGroup = pBuddy->GetGroup();

		pTempItemData = pList->GetItemFromParam((LPARAM)pBuddyGroup);
		bool bGroupCheck = pTempItemData->GetUserData(_DEF_USER_DATA_BUDDYITEM_TYPE_CHECK)?true:false;

		start = pBuddyGroup->GetBuddyList().begin();
		end = pBuddyGroup->GetBuddyList().end();

		bool bAllCheck = true;

		for(;start != end;++start)
		{
			pBuddy = *start;
			pTempItemData = pList->GetItemFromParam((LPARAM)pBuddy);
			if(!pTempItemData->GetUserData(_DEF_USER_DATA_BUDDYITEM_TYPE_CHECK))
			{
				bAllCheck = false;
				break;
			}
		}

		pTempItemData = pList->GetItemFromParam((LPARAM)pBuddyGroup);
		if(!bGroupCheck && bAllCheck)
		{
			pTempItemData->SetUserData(_DEF_USER_DATA_BUDDYITEM_TYPE_CHECK, (LPARAM)1);
			pList->UpdateItemFromParam((LPARAM)pBuddyGroup);
		}
		else if(bGroupCheck && !bAllCheck)
		{
			pTempItemData->SetUserData(_DEF_USER_DATA_BUDDYITEM_TYPE_CHECK, (LPARAM)0);
			pList->UpdateItemFromParam((LPARAM)pBuddyGroup);
		}
	}

	pList->CheckAllItemCheck();*/

	return TRUE;
}