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

	// 서버 응답(SV_CAFE_MEMBERLIST) 데이터
	m_cafeKind = CafeSystem::CafeKind::NONE;
	m_cafeID = 0;
	m_memInfoCnt = 0;
	m_memInfoList = NULL;

	// 사용자 정의
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
				ShowMainMessageDlg("선택된 유저가 없습니다.");

				break;
			}

			CAFEMEMBERINFO *pMemInfo = (CAFEMEMBERINFO *)pSelectItemData->GetItemData();

			if (pMemInfo == NULL)
			{
				ShowMainMessageDlg("선택된 유저가 없습니다.");

				break;
			}

			if (!CCommunityManager::Instance()->IsConnect())
			{
				ShowMainMessageDlg("커뮤니티 서버와 연결되어 있지 않아 선택한 유저를 친구추가 할 수 없습니다.");

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
		
		pPopupMenu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_BUDDY_PROFILE, "프로필보기", true);
		pPopupMenu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_BUDDY_MEMO, "쪽지보내기", true);

		if (bBlackList)
			pPopupMenu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_BUDDY_REMOVE_BLACKLIST, "블랙리스트삭제", true);
		else
			pPopupMenu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_BUDDY_ADD_BLACKLIST, "블랙리스트추가", true);
		
		CafeSystem::MemberGrade::Type myGrade = GM().GetMyInfo()->UI.cafeMemberGrade_;
		
		// 연합방에 들어가있고 연합원이고 내가 정회원 이상일 경우
		if (pMemInfo->nRoomNum > 0 && pMemInfo->typeGroup == CafeSystem::CafeKind::UNITED_CAFE && myGrade >= 2)
			pPopupMenu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_CAFE_TOGETHER_GAME, "카페원따라가기", true);
		
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
/// 서버에서 보내는 패킷(응답) 처리
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CCafeListGroup::HandleResponse(int Signal, int TotSize, char *lpData)
{
	switch(Signal)
	{
		//해당 유저의 기타재화 보험금 가입 정보를 보내준다.
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
	// 데이터가 없으면 반환
	if (m_cafeKind == NULL || m_memInfoList == NULL)
	{
		return;
	}

	char *memberID; // 멤버 ID
	CafeSystem::MemberGrade::Type memberGrade; // 0: 비회원 1: 준회원 2: 정회원 3: 카페장, 4: 연합원(카페장 포함) 5: 연합장 
	int globalRoomNum; // 게임중인 경우 방 번호. 게임중이 아니면 0
	int nCafeID = GM().GetMyInfo()->UI.cafeInfo_.cafeID_;
	int nUniteID = GM().GetMyInfo()->UI.unitedCafeInfo_.cafeID_;
	char* cMyID = GM().GetMyInfo()->UI.ID;
	CAFEMEMBERINFO* pMemInfo;
	std::list<CAFEMEMBERINFO *> m_listMemInfo_temp;
	
	// 카페 탭 활성화
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

			// 연합 리스트
			int nListCnt = 0;

			for (int i = 0; i < m_memInfoCnt; i++)
			{
				memberID = m_memInfoList[i].memberID_; // 멤버 ID
				memberGrade = m_memInfoList[i].memberGrade_; // 0: 비회원 1: 준회원 2: 정회원 3: 카페장, 4: 연합원(카페장 포함) 5: 연합장
				globalRoomNum = m_memInfoList[i].globalRoomNum_; // 게임중인 경우 방 번호. 게임중이 아니면 0

				// 본인 이거나 정회원 밑으로는 목록에서 제외
				if ((strcmp(memberID, cMyID) == 0) || memberGrade < 2)
					continue;

				nListCnt++;

				// int -> std::string 형변환
				//ostringstream ssCnt;
				//ssCnt << (nListCnt);
				//std::string sCnt =  ssCnt.str();
				std::string id = memberID;

				// 연합 맴버 생성
				pMemInfo = new CAFEMEMBERINFO();
				pMemInfo->sName = id;// + " " + sCnt;
				pMemInfo->typeGroup = CafeSystem::CafeKind::UNITED_CAFE;
				pMemInfo->memGrade = memberGrade; // 0: 비회원 1: 준회원 2: 정회원 3: 카페장, 4: 연합원(카페장 포함) 5: 연합장 
				pMemInfo->nRoomNum = globalRoomNum; // 게임중인 경우 방 번호. 게임중이 아니면 0
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
			
			// 연합 그룹
			pMemInfo = new CAFEMEMBERINFO();
			
			// int -> std::string 형변환
			ostringstream ss;
			ss << (nListCnt);
			std::string s =  ss.str();
			
			pMemInfo->sName = "연합 (" + s + ")";
			pMemInfo->typeGroup = CafeSystem::CafeKind::UNITED_CAFE;
			pMemInfo->bBuddy = false;
			pMemInfo->bGroup = true;
			pMemInfo->bExpend = false;
			//m_pListCafe->AddItem((LPARAM)pMemInfo);
			m_pListItem->m_listMemInfo.push_front(pMemInfo);
			
			if (nCafeID > 0)
			{
				// 카페 멤버 요청
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
			
			// 카페 리스트
			int nListCnt = 0;
			int nUniSize = m_pListItem->m_listMemInfo.size();
			std::list<CAFEMEMBERINFO *>::iterator iter = m_pListItem->m_listMemInfo.end();
			
			if (nUniSize > 0)
				std::advance(iter, -1);
			
			for (int i = 0; i < m_memInfoCnt; i++)
			{
				memberID = m_memInfoList[i].memberID_; // 멤버 ID
				memberGrade = m_memInfoList[i].memberGrade_; // 0: 비회원 1: 준회원 2: 정회원 3: 카페장, 4: 연합원(카페장 포함) 5: 연합장 
				globalRoomNum = m_memInfoList[i].globalRoomNum_; // 게임중인 경우 방 번호. 게임중이 아니면 0

				// 본인 이거나 정회원 밑으로는 목록에서 제외
				if (strcmp(memberID, cMyID) == 0 || memberGrade < 2)
				{
					continue;
				}
				
				nListCnt++;

				// int -> std::string 형변환
				//ostringstream ssCnt;
				//ssCnt << (nListCnt);
				//std::string sCnt =  ssCnt.str();
				std::string id = memberID;
				
				// 카페 맴버 생성
				pMemInfo = new CAFEMEMBERINFO();
				pMemInfo->sName = id;// + " " + sCnt;
				pMemInfo->typeGroup = CafeSystem::CafeKind::CAFE;
				pMemInfo->memGrade = memberGrade; // 0: 비회원 1: 준회원 2: 정회원 3: 카페장, 4: 연합원(카페장 포함) 5: 연합장 
				pMemInfo->nRoomNum = globalRoomNum; // 게임중인 경우 방 번호. 게임중이 아니면 0
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

			// 카페 그룹
			pMemInfo = new CAFEMEMBERINFO();

			// int -> std::string 형변환
			ostringstream ss;
			ss << (nListCnt);
			std::string s =  ss.str();

			pMemInfo->sName = "카페 (" + s + ")";
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