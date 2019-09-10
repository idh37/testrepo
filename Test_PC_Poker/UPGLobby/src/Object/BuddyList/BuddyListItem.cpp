#include "StdAfx.h"
#include "BuddyListItem.h"

CBuddyListItem::CBuddyListItem()
:	m_pGrpBuddyGroup(NULL),
	m_pExpandGroup(NULL),
	m_pGrpBuddyGroupShow(NULL),
	m_pTxtGroupName(NULL),
	m_pBtnGroupNameChange(NULL),
	m_pBtnGroupDelete(NULL),
	m_pGrpBuddyGroupEdit(NULL),
	m_pImgBuddyGroupEditBack(NULL),
	m_pEditBuddyGroupName(NULL),
	m_pBtnGroupNameEditOK(NULL),
	m_pBtnGroupNameEditCancel(NULL),
	m_pGrpBuddy(NULL),
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
	case ID_LOBBY_MY_FRIEND_LISTITEM_GRP_GROUP:				m_pGrpBuddyGroup				= (LSY::CGroup *)pObject;			break;
	case ID_LOBBY_MY_FRIEND_LISTITEM__CHBTN_EXPAND_GROUP:	m_pExpandGroup					= (LSY::CCheckButton *)pObject;		break;

	case ID_LOBBY_GRP_FRIEND_LISTITEM_GROUP_SHOW:			m_pGrpBuddyGroupShow			= (LSY::CGroup *)pObject;			break;
	case ID_LOBBY_MY_FRIEND_LISTITEM_TXT_GROUP_NAME:		m_pTxtGroupName					= (LSY::CText *)pObject;			break;
	case ID_LOBBY_BTN_FRIEND_LISTITEM_GROUP_NAME_CHANGE:	m_pBtnGroupNameChange			= (LSY::CButton *)pObject;			break;
	case ID_LOBBY_BTN_FRIEND_LISTITEM_GROUP_DELETE:			m_pBtnGroupDelete				= (LSY::CButton *)pObject;			break;

	case ID_LOBBY_GRP_FRIEND_LISTITEM_GROUP_EDIT:			m_pGrpBuddyGroupEdit			= (LSY::CGroup *)pObject;			break;
	case ID_LOBBY_IMG_FRIEND_LISTITEM_GROUP_EDIT_BACK:		m_pImgBuddyGroupEditBack		= (LSY::CImage *)pObject;			break;
	case ID_LOBBY_EDIT_FRIEND_LISTITEM_GROUP_NAME:			m_pEditBuddyGroupName			= (LSY::CEdit *)pObject;			break;
	case ID_LOBBY_BTN_FRIEND_LISTITEM_GROUP_EDIT_OK:		m_pBtnGroupNameEditOK			= (LSY::CButton *)pObject;			break;
	case ID_LOBBY_BTN_FRIEND_LISTITEM_GROUP_EDIT_CANCEL:	m_pBtnGroupNameEditCancel		= (LSY::CButton *)pObject;			break;

	case ID_LOBBY_MY_FRIEND_LISTITEM_GRP_FRIEND:			m_pGrpBuddy						= (LSY::CGroup *)pObject;			break;
	case ID_LOBBY_MY_FRIEND_LISTITEM_TXT_NICKNAME:			m_pTxtBuddyNickName				= (LSY::CText *)pObject;			break;
	case ID_LOBBY_MY_FRIEND_LISTITEM_TXTNB_HAVE_MONEY:		m_pTxtnbBuddyHaveMoney			= (LSY::CTextNumber *)pObject;		break;
	case ID_LOBBY_MY_FRIEND_LISTITEM_IMG_LOCATION_BACK:		m_pImgBuddyLocation				= (LSY::CImage *)pObject;			break;
	case ID_LOBBY_MY_FRIEND_LISTITEM_TXT_LOCATION:			m_pTxtBuddyLocationGame			= (LSY::CText *)pObject;			break;
	case ID_LOBBY_MY_FRIEND_LISTITEM_TXTNB_ROOM_NUMBER:		m_pTxtnbBuddyLocationRoomNumber	= (LSY::CText *)pObject;			break;
	case ID_LOBBY_MY_FRIEND_LISTITEM_TXT_OFFLINE:			m_pTxtBuddyOffline				= (LSY::CText *)pObject;			break;
	case ID_LOBBY_MY_FRIEND_LISTITEM_IMG_ONOFFLINE:			m_pImgBuddyOnOffline			= (LSY::CImage *)pObject;			break;
	}
	__super::OnCreateChild(pObject);
}

void CBuddyListItem::OnCreated(void)
{
	LSY::CObjectProject *pProject = GetObjectMan()->GetProjectFromID(ID_LOBBY);

	OnCreateChild(m_pGrpBuddyGroup->GetFirstChildFromID(ID_LOBBY_MY_FRIEND_LISTITEM__CHBTN_EXPAND_GROUP));

	OnCreateChild(m_pGrpBuddyGroup->GetFirstChildFromID(ID_LOBBY_GRP_FRIEND_LISTITEM_GROUP_SHOW));
	OnCreateChild(m_pGrpBuddyGroupShow->GetFirstChildFromID(ID_LOBBY_MY_FRIEND_LISTITEM_TXT_GROUP_NAME));
	OnCreateChild(m_pGrpBuddyGroupShow->GetFirstChildFromID(ID_LOBBY_BTN_FRIEND_LISTITEM_GROUP_NAME_CHANGE));
	OnCreateChild(m_pGrpBuddyGroupShow->GetFirstChildFromID(ID_LOBBY_BTN_FRIEND_LISTITEM_GROUP_DELETE));

	OnCreateChild(m_pGrpBuddyGroup->GetFirstChildFromID(ID_LOBBY_GRP_FRIEND_LISTITEM_GROUP_EDIT));
	OnCreateChild(m_pGrpBuddyGroupEdit->GetFirstChildFromID(ID_LOBBY_IMG_FRIEND_LISTITEM_GROUP_EDIT_BACK));
	OnCreateChild(m_pGrpBuddyGroupEdit->GetFirstChildFromID(ID_LOBBY_EDIT_FRIEND_LISTITEM_GROUP_NAME));
	OnCreateChild(m_pGrpBuddyGroupEdit->GetFirstChildFromID(ID_LOBBY_BTN_FRIEND_LISTITEM_GROUP_EDIT_OK));
	OnCreateChild(m_pGrpBuddyGroupEdit->GetFirstChildFromID(ID_LOBBY_BTN_FRIEND_LISTITEM_GROUP_EDIT_CANCEL));

	OnCreateChild(m_pGrpBuddy->GetFirstChildFromID(ID_LOBBY_MY_FRIEND_LISTITEM_TXT_NICKNAME));
	OnCreateChild(m_pGrpBuddy->GetFirstChildFromID(ID_LOBBY_MY_FRIEND_LISTITEM_TXTNB_HAVE_MONEY));
	OnCreateChild(m_pGrpBuddy->GetFirstChildFromID(ID_LOBBY_MY_FRIEND_LISTITEM_IMG_LOCATION_BACK));
	OnCreateChild(m_pImgBuddyLocation->GetFirstChildFromID(ID_LOBBY_MY_FRIEND_LISTITEM_TXT_LOCATION));
	OnCreateChild(m_pImgBuddyLocation->GetFirstChildFromID(ID_LOBBY_MY_FRIEND_LISTITEM_TXTNB_ROOM_NUMBER));
	OnCreateChild(m_pGrpBuddy->GetFirstChildFromID(ID_LOBBY_MY_FRIEND_LISTITEM_TXT_OFFLINE));
	OnCreateChild(m_pGrpBuddy->GetFirstChildFromID(ID_LOBBY_MY_FRIEND_LISTITEM_IMG_ONOFFLINE));

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
		UpdateBuddyGroup((CBuddyGroup *)pBuddy);
	}
	else
	{
		UpdateBuddy((CBuddyFriend *)pBuddy);
	}
}

void CBuddyListItem::UpdateBuddyGroup(CBuddyGroup *pBuddyGroup)
{
	m_pGrpBuddy->SetShow(false);
	m_pGrpBuddyGroup->SetShow(true);

	m_pExpandGroup->SetCheck(pBuddyGroup->IsExpand());
	m_pGrpBuddyGroupEdit->SetShow(pBuddyGroup->IsEdit());
	m_pGrpBuddyGroupShow->SetShow(pBuddyGroup->IsEdit()?false:true);

	if(pBuddyGroup->IsSameName(_DEF_DEFAULT_BUDDY_GROUP))
	{
		m_pBtnGroupNameChange->SetShow(false);
		m_pBtnGroupDelete->SetShow(false);
	}
	else
	{
		m_pBtnGroupNameChange->SetShow(true);
		m_pBtnGroupDelete->SetShow(true);
	}

	if(pBuddyGroup->IsEdit())
	{
		m_pEditBuddyGroupName->SetText(pBuddyGroup->GetEditName());
	}
	else if(pBuddyGroup->IsSendChangeName())
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

void CBuddyListItem::UpdateBuddy(CBuddyFriend *pBuddy)
{
	m_pGrpBuddy->SetShow(true);
	m_pGrpBuddyGroup->SetShow(false);

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
			m_pTxtnbBuddyLocationRoomNumber->SetText("����");
		}
		else
		{
			CString strText;
			strText.Format("%d��", pBuddy->GetRoomNumber());
			m_pTxtnbBuddyLocationRoomNumber->SetText(strText.GetString());
		}

		m_pTxtnbBuddyHaveMoney->SetNumber(pBuddy->GetMoney());
		m_pImgBuddyOnOffline->SetIndex(bBlackList?47:45);
	}
}

LRESULT CBuddyListItem::OnCheckChangeBuddyGroupExpand(LSY::CMessage *msg)
{
	LSY::CMO_CheckChange *pMsg = (LSY::CMO_CheckChange *)msg;
	CBuddyListItem *pItem = (CBuddyListItem *)LSY::GetListItem(pMsg);
	if(pItem == NULL) return FALSE;
	CBuddyGroup *pBuddyGroup = (CBuddyGroup *)pItem->GetData()->GetItemData();
	if(pBuddyGroup == NULL) return FALSE;
	bool bExpand = pItem->m_pExpandGroup->IsCheck();
	pBuddyGroup->SetExpand(bExpand);
	
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

LRESULT CBuddyListItem::OnClickButton(LSY::CMessage *msg)
{
	LSY::CMO_MouseLClick *pMsg = (LSY::CMO_MouseLClick *)msg;
	CBuddyListItem *pItem = (CBuddyListItem *)LSY::GetListItem(pMsg);
	if(pItem == NULL) return FALSE;
	CBuddyGroup *pBuddyGroup = (CBuddyGroup *)pItem->GetData()->GetItemData();
	if(pBuddyGroup == NULL) return FALSE;

	switch(pMsg->GetObject()->GetID())
	{
	case ID_LOBBY_BTN_FRIEND_LISTITEM_GROUP_NAME_CHANGE:
		if(pBuddyGroup->IsSendChangeName())
		{
			ShowMainMessageDlg("�׷�� ���� ��û���� �׷����� �������� ó���� �̷������ �ʾ� �׷���� ������ �� �����ϴ�. ��� �� �ٽ� �õ����ּ���.");
		}
		else if(CCommunityManager::Instance()->IsConnect())
		{
			pBuddyGroup->SetEdit(true);
			pItem->UpdateBuddyGroup(pBuddyGroup);
		}
		else
		{
			ShowMainMessageDlg("Ŀ�´�Ƽ �������� ������ �Ǿ����� �ʾ� �׷�� ������ �� �����ϴ�.");
		}
		break;
	case ID_LOBBY_BTN_FRIEND_LISTITEM_GROUP_DELETE:
		{
			CBuddyGroupRemoveDlg dlg;
			if(!CCommunityManager::Instance()->IsConnect())
			{
				dlg.DoModal(CBuddyGroupRemoveDlg::BGRDT_MESSAGE, "Ŀ�´�Ƽ �������� ������ �Ǿ����� �ʾ� �׷��� ������ �� �����ϴ�.", GM().GetMainWnd());
			}
			else if(pBuddyGroup->IsEmpty())
			{
				if(pBuddyGroup->IsSameName(_DEF_DEFAULT_BUDDY_GROUP))
				{
					CString strTemp;
					strTemp.Format("[%s]�� ������ �� �����ϴ�.", _DEF_DEFAULT_BUDDY_GROUP);
					dlg.DoModal(CBuddyGroupRemoveDlg::BGRDT_MESSAGE, strTemp.GetString(), GM().GetMainWnd());
				}
				else if(dlg.DoModal(CBuddyGroupRemoveDlg::BGRDT_QUESTION, "", GM().GetMainWnd()) == IDOK)
				{
					CCommunityManager::Instance()->SendBuddyGroupRemove(pBuddyGroup->IsSendChangeName()?pBuddyGroup->GetEditName():pBuddyGroup->GetName());
				}
			}
			else
			{
				dlg.DoModal(CBuddyGroupRemoveDlg::BGRDT_MESSAGE, "�׷� ���� ģ���� �־� ������ �� �����ϴ�.\n�׷��� �����Ϸ��� ģ���� �ٸ� �׷����� �̵��ؾ� �մϴ�.", GM().GetMainWnd());
			}
		}
		break;
	case ID_LOBBY_BTN_FRIEND_LISTITEM_GROUP_EDIT_OK:
		if(pBuddyGroup->IsEditChangeName() && !CCommunityManager::Instance()->SendBuddyGroupNameChanage(pBuddyGroup))
		{
			ShowMainMessageDlg("�ߺ��� �׷���� �����Ͽ� �׷���� �������� ���Ͽ����ϴ�.");
			return TRUE;
		}
		pBuddyGroup->SetEdit(false);
		pItem->UpdateBuddyGroup(pBuddyGroup);
		pItem->GetList()->Sort();
		break;
	case ID_LOBBY_BTN_FRIEND_LISTITEM_GROUP_EDIT_CANCEL:
		pBuddyGroup->SetEdit(false);
		pBuddyGroup->SetEditName("");
		pItem->UpdateBuddyGroup(pBuddyGroup);
		break;
	}

	return TRUE;
}

LRESULT CBuddyListItem::OnEditFocusOut(LSY::CMessage *msg)
{
	LSY::CMO_MouseLClick *pMsg = (LSY::CMO_MouseLClick *)msg;
	CBuddyListItem *pItem = (CBuddyListItem *)LSY::GetListItem(pMsg);
	if(pItem == NULL) return FALSE;
	CBuddyGroup *pBuddyGroup = (CBuddyGroup *)pItem->GetData()->GetItemData();
	if(pBuddyGroup == NULL) return FALSE;

	pBuddyGroup->SetEditName(pItem->m_pEditBuddyGroupName->GetText());

	return TRUE;
}