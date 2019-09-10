#include "StdAfx.h"
#include "CafeListGroup.h"
#include "CafeListItem.h"
#include "../popupmemuid.h"

CCafeListGroup::CCafeListGroup(void)
{
	// LSY Control
	m_pListCafe = NULL;
	m_pBtnAddFnd = NULL;
	m_pChkExpFnd = NULL;

	// ���� ����(SV_CAFE_MEMBERLIST) ������
	m_cafeKind = CafeSystem::CafeKind::NONE;
	m_cafeID = 0;
	m_memInfoCnt = 0;
	m_memInfoList = NULL;

	// ����� ����
	m_pListItem = NULL;
}

CCafeListGroup::~CCafeListGroup(void)
{
}

void CCafeListGroup::OnCreateChild(LSY::CObjectBase *pObject)
{
	if (pObject == NULL)
		return;

	bool bAddChild = false;

	switch (pObject->GetID())
	{
	case ID_LOBBY_LIST_MY_CAFE:
		m_pListCafe = (LSY::CList *)pObject;
		break;
	case ID_LOBBY_BTN_MY_CAFE_ADD_FRIEND:
		m_pBtnAddFnd = (LSY::CButton *)pObject;
		bAddChild = true;
		break;
	case ID_LOBBY_CHECKBOX_MY_CAFE_EXCEPT_FRIEND:
		m_pChkExpFnd = (LSY::CCheckBox *)pObject;
		break;
	}

	if (bAddChild)
	{
		pObject->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CCafeListGroup::OnClickButton));
	}

	__super::OnCreateChild(pObject);
}

void CCafeListGroup::OnCreated(void)
{
	LSY::CObjectProject *pProject = GetObjectMan()->GetProjectFromID(ID_LOBBY);

	m_pListCafe->AddHandler(LSY::EM_O_LISTITEM_RCLICK, LSY::Fnt(this, &CCafeListGroup::OnListItemRClick));
	
	m_pListCafe->SetSortType(0);

	m_pListItem = (CCafeListItem *)m_pListCafe->GetItem();
	m_pListItem->m_pChkBtnCafeExpandGroup->AddHandler(LSY::EM_O_CHECKCHANGE, LSY::Fnt(m_pListItem, &CCafeListItem::OnCheckChangeCafeGroupExpand));

	m_pChkExpFnd->AddHandler(LSY::EM_O_CHECKCHANGE, LSY::Fnt(this, &CCafeListGroup::OnCheckChange));
	
	__super::OnCreated();
}

LRESULT CCafeListGroup::OnClickButton(LSY::CMessage *msg)
{
	LSY::CObjectMessage *pMsg = (LSY::CObjectMessage *)msg;

	switch (pMsg->GetObject()->GetID())
	{
	case ID_LOBBY_BTN_MY_CAFE_ADD_FRIEND:
		{
			LSY::CListItemData *pSelectItemData = m_pListCafe->GetFirstSelectItem();

			if (pSelectItemData == NULL)
			{
				ShowMainMessageDlg("���õ� ������ �����ϴ�.");

				break;
			}

			CAFEMEMBERINFO *pMemInfo = (CAFEMEMBERINFO *)pSelectItemData->GetItemData();

			if (pMemInfo == NULL)
			{
				ShowMainMessageDlg("���õ� ������ �����ϴ�.");

				break;
			}

			if (!CCommunityManager::Instance()->IsConnect())
			{
				ShowMainMessageDlg("Ŀ�´�Ƽ ������ ����Ǿ� ���� �ʾ� ������ ������ ģ���߰� �� �� �����ϴ�.");

				break;
			}

			CBuddyAddDlg dlg;
			dlg.DoModal(pMemInfo->sName, GM().GetMainWnd());
		}
		break;
	}

	return TRUE;
}

LRESULT CCafeListGroup::OnListItemRClick(LSY::CMessage *pMsg)
{
	LSY::CMO_ListItemRClick *pRClick = (LSY::CMO_ListItemRClick *)pMsg;
	LSY::CListItemData *pData = pRClick->GetItemData();

	CAFEMEMBERINFO *pMemInfo = (CAFEMEMBERINFO *)pData->GetItemData();
	m_pListCafe->SetItemSelectFromIndex(pData->GetItemIndex(), true);

	CWnd *pWnd = GetObjectMan()->GetWnd();
	LSY::CPoint ptPos = GetObjectMan()->GetCurMousePos();
	POINT ptTempPos = ptPos.GetPoint();
	pWnd->ClientToScreen(&ptTempPos);

	CPopupMemu *pPopupMenu = new CPopupMemu;
	CLobbyView *pLobbyWnd = (CLobbyView *)GetLobbyWindow();
	
	if(pMemInfo->bGroup == false)
	{
		//CBuddyFriend *pBuddy = (CBuddyFriend *)pMemInfo;
		
		//bool bOffline = pBuddy->IsOffline();
		bool bBlackList = CCommunityManager::Instance()->IsExistBlackBuddyByID(pMemInfo->sName) ? true : false;
		//bool bInGameRoom = bOffline?false:(pBuddy->GetRoomNumber()==0)?false:true;
		
		pPopupMenu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_BUDDY_PROFILE, "�����ʺ���", true);
		pPopupMenu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_BUDDY_MEMO, "����������", true);

		if (bBlackList)
			pPopupMenu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_BUDDY_REMOVE_BLACKLIST, "������Ʈ����", true);
		else
			pPopupMenu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_BUDDY_ADD_BLACKLIST, "������Ʈ�߰�", true);
		
		CafeSystem::MemberGrade::Type myGrade = GM().GetMyInfo()->UI.cafeMemberGrade_;
		
		// ���չ濡 ���ְ� ���տ��̰� ���� ��ȸ�� �̻��� ���
		if (pMemInfo->nRoomNum > 0 && pMemInfo->typeGroup == CafeSystem::CafeKind::UNITED_CAFE && myGrade >= 2)
			pPopupMenu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_CAFE_TOGETHER_GAME, "ī������󰡱�", true);
		
		pLobbyWnd->SetSelectID(pMemInfo->sName);
		pLobbyWnd->SetSelectNickName(pMemInfo->sName);
		pLobbyWnd->SetSelectRoomNum(pMemInfo->nRoomNum);
		
		pPopupMenu->Create(ptTempPos.x, ptTempPos.y);
	}

	GM().GetMainWnd()->SendMessage(UPGM_POPUPMEMU_OPEN, 0, (LPARAM)pPopupMenu);

	return TRUE;
}

LRESULT CCafeListGroup::OnCheckChange(LSY::CMessage *pMsg)
{
	LSY::CMO_CheckChange *pChange = (LSY::CMO_CheckChange *)pMsg;
	//CCommunityManager::Instance()->SetShowConnectBuddy(pChange->GetValue());
	
	m_pListItem->m_bExpFnd = pChange->GetValue();
	m_pListItem->AllAddCafe(m_pListCafe);
	
	return TRUE;
}

/// <summary>
/// �������� ������ ��Ŷ(����) ó��
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CCafeListGroup::HandleResponse(int Signal, int TotSize, char *lpData)
{
	switch(Signal)
	{
		//�ش� ������ ��Ÿ��ȭ ����� ���� ������ �����ش�.
	case SV_CAFE_MEMBERLIST:
		{
			CSV_CAFE_MEMBERLIST msg;
			msg.Get(lpData, TotSize);

			m_cafeKind = *msg.cafeKind_;
			m_cafeID = *msg.cafeID_;
			m_memInfoCnt = *msg.memberInfoCount_;

			m_memInfoList = new CafeSystem::MemberInfo[m_memInfoCnt];
			
			for (int i = 0; i < m_memInfoCnt; i++)
			{
				strncpy(m_memInfoList[i].memberID_, msg.memberInfoList_[i].memberID_, ID_LEN);
				m_memInfoList[i].memberGrade_ = msg.memberInfoList_[i].memberGrade_;
				m_memInfoList[i].globalRoomNum_ = msg.memberInfoList_[i].globalRoomNum_;
			}
			
			SetListCafeMember();
		}
		break;
	}
}

void CCafeListGroup::SetListCafeMember()
{
	// �����Ͱ� ������ ��ȯ
	if (m_cafeKind == NULL || m_memInfoList == NULL)
	{
		return;
	}

	char *memberID; // ��� ID
	CafeSystem::MemberGrade::Type memberGrade; // 0: ��ȸ�� 1: ��ȸ�� 2: ��ȸ�� 3: ī����, 4: ���տ�(ī���� ����) 5: ������ 
	int globalRoomNum; // �������� ��� �� ��ȣ. �������� �ƴϸ� 0
	int nCafeID = GM().GetMyInfo()->UI.cafeInfo_.cafeID_;
	int nUniteID = GM().GetMyInfo()->UI.unitedCafeInfo_.cafeID_;
	char* cMyID = GM().GetMyInfo()->UI.ID;
	CAFEMEMBERINFO* pMemInfo;
	std::list<CAFEMEMBERINFO *> m_listMemInfo_temp;
	
	// ī�� �� Ȱ��ȭ
	if (nCafeID > 0)
	{
		MM().Call(UPG_CMK_SET_USER_LIST_TAB_ACTIVE, (WPARAM)2, (LPARAM)TRUE);
	}
	
	switch (m_cafeKind)
	{
	case CafeSystem::CafeKind::UNITED_CAFE:
		{
			m_pListItem->m_listMemInfo.clear();
			m_listMemInfo_temp.clear();

			// ���� ����Ʈ
			int nListCnt = 0;

			for (int i = 0; i < m_memInfoCnt; i++)
			{
				memberID = m_memInfoList[i].memberID_; // ��� ID
				memberGrade = m_memInfoList[i].memberGrade_; // 0: ��ȸ�� 1: ��ȸ�� 2: ��ȸ�� 3: ī����, 4: ���տ�(ī���� ����) 5: ������
				globalRoomNum = m_memInfoList[i].globalRoomNum_; // �������� ��� �� ��ȣ. �������� �ƴϸ� 0

				// ���� �̰ų� ��ȸ�� �����δ� ��Ͽ��� ����
				if ((strcmp(memberID, cMyID) == 0) || memberGrade < 2)
					continue;

				nListCnt++;

				// int -> std::string ����ȯ
				//ostringstream ssCnt;
				//ssCnt << (nListCnt);
				//std::string sCnt =  ssCnt.str();
				std::string id = memberID;

				// ���� �ɹ� ����
				pMemInfo = new CAFEMEMBERINFO();
				pMemInfo->sName = id;// + " " + sCnt;
				pMemInfo->typeGroup = CafeSystem::CafeKind::UNITED_CAFE;
				pMemInfo->memGrade = memberGrade; // 0: ��ȸ�� 1: ��ȸ�� 2: ��ȸ�� 3: ī����, 4: ���տ�(ī���� ����) 5: ������ 
				pMemInfo->nRoomNum = globalRoomNum; // �������� ��� �� ��ȣ. �������� �ƴϸ� 0
				pMemInfo->bBuddy = CCommunityManager::Instance()->IsExistBuddyByID(memberID);
				pMemInfo->bGroup = false;
				pMemInfo->bExpend = false;

				bool bOnline = (pMemInfo->nRoomNum >= 0);

				if (bOnline)
					m_pListItem->m_listMemInfo.push_back(pMemInfo);
				else
					m_listMemInfo_temp.push_back(pMemInfo);
			}
			
			std::list<CAFEMEMBERINFO *>::iterator iterTemp = m_listMemInfo_temp.begin();
			std::list<CAFEMEMBERINFO *>::iterator iterTempEnd = m_listMemInfo_temp.end();
			CAFEMEMBERINFO *pMemInfo = NULL;
			
			for (; iterTemp != iterTempEnd; iterTemp++)
			{
				pMemInfo = *iterTemp;

				m_pListItem->m_listMemInfo.push_back(pMemInfo);
			}
			
			// ���� �׷�
			pMemInfo = new CAFEMEMBERINFO();
			
			// int -> std::string ����ȯ
			ostringstream ss;
			ss << (nListCnt);
			std::string s =  ss.str();
			
			pMemInfo->sName = "���� (" + s + ")";
			pMemInfo->typeGroup = CafeSystem::CafeKind::UNITED_CAFE;
			pMemInfo->bBuddy = false;
			pMemInfo->bGroup = true;
			pMemInfo->bExpend = false;
			//m_pListCafe->AddItem((LPARAM)pMemInfo);
			m_pListItem->m_listMemInfo.push_front(pMemInfo);
			
			if (nCafeID > 0)
			{
				// ī�� ��� ��û
				CCL_CAFE_MEMBERLIST msg;
				msg.Set(CafeSystem::CafeKind::CAFE, nCafeID);
				NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
			}
		}
		break;
	case CafeSystem::CafeKind::CAFE:
		{
			if (nUniteID <= 0)
			{
				m_pListItem->m_listMemInfo.clear();
			}
			
			m_listMemInfo_temp.clear();
			
			// ī�� ����Ʈ
			int nListCnt = 0;
			int nUniSize = m_pListItem->m_listMemInfo.size();
			std::list<CAFEMEMBERINFO *>::iterator iter = m_pListItem->m_listMemInfo.end();
			
			if (nUniSize > 0)
				std::advance(iter, -1);
			
			for (int i = 0; i < m_memInfoCnt; i++)
			{
				memberID = m_memInfoList[i].memberID_; // ��� ID
				memberGrade = m_memInfoList[i].memberGrade_; // 0: ��ȸ�� 1: ��ȸ�� 2: ��ȸ�� 3: ī����, 4: ���տ�(ī���� ����) 5: ������ 
				globalRoomNum = m_memInfoList[i].globalRoomNum_; // �������� ��� �� ��ȣ. �������� �ƴϸ� 0

				// ���� �̰ų� ��ȸ�� �����δ� ��Ͽ��� ����
				if (strcmp(memberID, cMyID) == 0 || memberGrade < 2)
				{
					continue;
				}
				
				nListCnt++;

				// int -> std::string ����ȯ
				//ostringstream ssCnt;
				//ssCnt << (nListCnt);
				//std::string sCnt =  ssCnt.str();
				std::string id = memberID;
				
				// ī�� �ɹ� ����
				pMemInfo = new CAFEMEMBERINFO();
				pMemInfo->sName = id;// + " " + sCnt;
				pMemInfo->typeGroup = CafeSystem::CafeKind::CAFE;
				pMemInfo->memGrade = memberGrade; // 0: ��ȸ�� 1: ��ȸ�� 2: ��ȸ�� 3: ī����, 4: ���տ�(ī���� ����) 5: ������ 
				pMemInfo->nRoomNum = globalRoomNum; // �������� ��� �� ��ȣ. �������� �ƴϸ� 0
				pMemInfo->bBuddy = CCommunityManager::Instance()->IsExistBuddyByID(memberID);
				pMemInfo->bGroup = false;
				pMemInfo->bExpend = false;
				
				bool bOnline = (pMemInfo->nRoomNum >= 0);
				
				if (bOnline)
					m_pListItem->m_listMemInfo.push_back(pMemInfo);
				else
					m_listMemInfo_temp.push_back(pMemInfo);
			}
			
			std::list<CAFEMEMBERINFO *>::iterator iterTemp = m_listMemInfo_temp.begin();
			std::list<CAFEMEMBERINFO *>::iterator iterTempEnd = m_listMemInfo_temp.end();
			CAFEMEMBERINFO *pMemInfo = NULL;

			for (; iterTemp != iterTempEnd; iterTemp++)
			{
				pMemInfo = *iterTemp;

				m_pListItem->m_listMemInfo.push_back(pMemInfo);
			}

			// ī�� �׷�
			pMemInfo = new CAFEMEMBERINFO();

			// int -> std::string ����ȯ
			ostringstream ss;
			ss << (nListCnt);
			std::string s =  ss.str();

			pMemInfo->sName = "ī�� (" + s + ")";
			pMemInfo->typeGroup = CafeSystem::CafeKind::CAFE;
			pMemInfo->bBuddy = false;
			pMemInfo->bGroup = true;
			pMemInfo->bExpend = false;
			
			if (nUniSize > 0)
				std::advance(iter, 1);
			else
				iter = m_pListItem->m_listMemInfo.begin();
			
			m_pListItem->m_listMemInfo.insert(iter, pMemInfo);
		}
		break;
	}
	
	m_pListItem->AllAddCafe(m_pListCafe);
}