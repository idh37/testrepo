// LobbyView.cpp : implementation file
//

#include "stdafx.h"
#include "LobbyView.h"

#include "WidgetID.h"
#include "./Object/TileButton/TitleButton.h"
#include "./Object/TabGame/TabGame.h"
#include "./Object/UserInfo/UserInfoGroup.h"
#include "./Object/UserInfo/UserInfoDetailDlg.h"
#include "./Object/Jackpot/JackpotGroup.h"
#include "./Object/Jackpot/JackpotMoney.h"
#include "./Object/Jackpot/JackpotCurrentInfoDlg.h"
#include "./Object/UserList/UserListGroup.h"
#include "./Object/UserList/UserListItem.h"
#include "./Object/RoomList/RoomListGroup.h"
#include "./Object/RoomList/RoomListItem.h"
#include "./Object/RoomList/RoomListItemLowBD.h"
#include "./Object/RoomList/RoomListItemHoola.h"
#include "./Object/RoomList/RoomListHeader.h"
#include "./Object/RoomList/RoomListHeaderLowBD.h"
#include "./Object/RoomList/RoomListHeaderHoola.h"
#include "./Object/RoomList/RoomListItemJoin.h"
#include "./Object/RoomList/RoomListItemLowBDJoin.h"
#include "./Object/RoomList/RoomListItemHoolaJoin.h"
#include "./Object/RoomList/RoomListHeaderJoin.h"
#include "./Object/RoomList/RoomListHeaderLowBDJoin.h"
#include "./Object/RoomList/RoomListHeaderHoolaJoin.h"
#include "./Object/RoomList/RadGoldRoomListVIP.h"
#include "./Object/RoomList/RadGoldRoomListNormal.h"
#include "./Object/RoomInfoDlg/RoomInfoDialog.h"
#include "./Object/RoomInfoDlg/RoomInfoUserListItem.h"
#include "./Object/ChatGroup/ChatGroup.h"
#include "./Object/ChatGroup/WhisperlistCombo.h"
#include "./Object/ShopTab/ShopTabControl.h"
#include "./Object/BuddyList/BuddyListGroup.h"
#include "./Object/BuddyList/BuddyListItem.h"
#include "./Object/BuddyList/CafeListGroup.h"
#include "./Object/BuddyList/CafeListItem.h"
#include "./Object/BuddyList/BlackBuddyListGroup.h"
#include "./Object/BuddyList/BlackBuddyListItem.h"
#include "./Object/BuddyList/ReverseBuddyListGroup.h"
#include "./Object/BuddyList/ReverseBuddyListItem.h"
#include "./popupmemuid.h"
#include "./resource.h"
#include "./Object/VipJackpot/VipJackpotMoney.h"
#include "./Object/VipJackpot/VipJackpotDlg.h"
#include "./Object/VipJackpot/VipJackpotGroup.h"
#include "./Object/TargetNotiGroup/TargetNotiGroup.h"
#include "./Object/TargetNotiGroup/CongNotiGroup.h"

#define RECT_ROOMLIST_AREA CRect(74, 240, 74 + 606, 240 + 453)

CLobbyView*	g_pLobbyView = NULL;

const int LOBBYVIEW_TIMER = 54343;
const int LOBBYVIEW_FPS = 60;

void CALLBACK LobbyUpdateProc( UINT uTimerID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2 ) 
{ 
	if( g_pLobbyView == NULL ) return;

	if( g_pLobbyView->GetSafeHwnd() ) 
	{
		static MSG wq_msg;
		if( ::PeekMessage( &wq_msg, NULL, WM_TIMER, WM_TIMER, PM_NOREMOVE ) != 0 )
		{ 
			// 이전에 포스트된 타이머 메시지가 아직도 처리되지 않은 경우라면 그냥 리턴 
			if( wq_msg.hwnd == g_pLobbyView->GetSafeHwnd() && wq_msg.wParam == LOBBYVIEW_TIMER )
			{
				return;
			}
		}
		g_pLobbyView->PostMessage( WM_TIMER, LOBBYVIEW_TIMER, 0 );
	}
}

//////////////////////////////////////////////////////////////////////////////
// CLobbyView

IMPLEMENT_DYNAMIC(CLobbyView, CWnd)

CLobbyView::CLobbyView()
{
	m_hEventTimer = NULL;
	m_rectMoveAble.SetRect(0,0,795,27);
	g_pLobbyView = this;
	m_pLobbyProcess=NULL;
	m_pTitleButtonGroup = NULL;
	m_pTabGame = NULL;
	m_pUserInfoGroup = NULL;
	m_pUserInfoDetailDlg = NULL;
	m_pJackpotGroup = NULL;
	m_pJackpotCurrentInfoDlg = NULL;
	m_pUserListGroup = NULL;
	m_pUserListItem = NULL;
	m_pRoomListGroup = NULL;
	m_pRoomListItem = NULL;
	m_pRoomListItemLowBD = NULL;
	m_pRoomListItemHoola = NULL;
	m_pRoomListItemJoin = NULL;
	m_pRoomListItemLowBDJoin = NULL;
	m_pRoomListItemHoolaJoin = NULL;

	m_pRoomInfoDlg = NULL; 
	m_pChatGroup = NULL;
	m_pShopTab = NULL;
	m_pPCRoomEventBack = NULL;
	m_pPCRoomWeb = NULL;
	m_pPopupMenu = NULL;
	m_pBuddyListGroup = NULL;
	m_pBuddyListItem = NULL;
	m_pBlackBuddyListGroup = NULL;
	m_pBlackBuddyListItem = NULL;
	m_pReverseBuddyListGroup = NULL;
	m_pReverseBuddyListItem = NULL;
	m_pEventInfoWnd = NULL; //신평호 테스트
	m_pVipJackpotDlg = NULL;
	m_pVipJackpotGroup = NULL;

	m_pBtnComebackPromotion = NULL;
	m_pComeBackDlg = NULL;

	m_pBtnChickenPromotion = NULL;

	m_pTargetNotiGroup = NULL;
	m_pCongNotiGroup = NULL;

	m_chuSeokTest = NULL;

	EnableAutomation();

	MM().AddMsgH(UPG_CMK_NOTIFY_ACTIVE_PCPROMOTION, Fnt(this, &CLobbyView::OnActivePCEvent));
	MM().AddMsgH(UPG_CMK_SET_URL_LOBBY_EVENT, Fnt(this, &CLobbyView::OnSetEventBannerURL));

	MM().AddMsgH(UPG_CMK_SEND_CREATE_CHATTING, Fnt(this, &CLobbyView::OnSendCreateChatting));

	MM().AddMsgH(UPG_CHECK_SHOW_COMMUNITY_WINDOW, Fnt(this, &CLobbyView::OnCheckShowCommunityWindow));
	MM().AddMsgH(UPG_ADD_CHATTING_WHISPER_LIST, Fnt(this, &CLobbyView::OnAddWhisper));

	MM().AppendMsgH(UPG_CMK_SETPOKERICON, Fnt(this, &CLobbyView::OnSetPokerIcon));	//작업 표시줄 아이콘 변경

	MM().AddMsgH(UPG_CMK_SET_USER_LIST_TAB_TYPE, Fnt(this, &CLobbyView::OnSetUserListTabType));
	MM().AddMsgH(UPG_CMK_GET_USER_LIST_TAB_TYPE, Fnt(this, &CLobbyView::OnGetUserListTabType));
	MM().AddMsgH(UPG_CMK_SET_USER_LIST_TAB_ACTIVE, Fnt(this, &CLobbyView::OnSetUserListTabActive));
	
	MM().AddMsgH(UPG_CMK_CHANGE_MYUSERINFO, Fnt(this, &CLobbyView::OnChangeMyUserInfo));

	//yoo
	MM().AddMsgH(UPG_CMK_SHOW_RESTRICTION_BUTTON, Fnt(this, &CLobbyView::OnShowRestrictionButton)); //규제안 : 로비에 버튼을 활성화한다.
	MM().AddMsgH(UPG_CMK_HIDE_RESTRICTION_BUTTON, Fnt(this, &CLobbyView::OnHideRestrictionButton)); //규제안 : 로비에 버튼을 비활성화한다.
	MM().AddMsgH(UPG_CMK_CHANGE_RESTRICTION_BUTTON, Fnt(this, &CLobbyView::OnChangeRestrictionButton)); //규제안 : 로비에 버튼을 비활성화한다.
	MM().AddMsgH(UPG_CMK_SHOW_RESTRICTION, Fnt(this, &CLobbyView::OnShowRestriction)); //규제안 : 로비에 버튼을 비활성화한다.
	MM().AddMsgH(UPG_CMK_SHOW_RESTRICTIONOVER, Fnt(this, &CLobbyView::OnShowRestrictionOver)); //규제안 : 로비에 버튼을 비활성화한다.
	MM().AddMsgH(UPG_CMK_SHOW_RESTRICTION_PLAYOVER, Fnt(this, &CLobbyView::OnShowRestrictionPlayOver));	//규제안 : 로비에 버튼을 활성화한다. 
	MM().AddMsgH(UPG_CMK_SHOW_PROMOTION_COMEBACK, Fnt(this, &CLobbyView::OnShowPromotionComeBack)); //프로모션 버튼을 활성화 한다
	MM().AddMsgH(UPG_CMK_SHOW_PROMOTION_COMEBACK_DLG, Fnt(this, &CLobbyView::OnShowPromotionComeBackDlg)); //프로모션 버튼을 활성화 한다
	MM().AddMsgH(UPG_CMK_SHOW_PROMOTION_COMEBACK_DLGUPDATE, Fnt(this, &CLobbyView::OnShowPromotionComeBackDlgUpdate)); //프로모션 버튼을 활성화 한다

	MM().AddMsgH(UPG_CMK_CHANGE_PROMOTION_LOBBY, Fnt(this, &CLobbyView::OnChangePromotionLobby)); //프로모션 버튼을 활성화 한다
	MM().AddMsgH(UPG_CMK_SHOW_CHICKEN_PROMOTION_BTN, Fnt(this, &CLobbyView::OnShowChickenPromotionBtn)); //프로모션 버튼을 활성화 한다
	MM().AddMsgH(UPG_CMK_SHOW_GOLDBACK_PROMOTION_BTN, Fnt(this, &CLobbyView::OnShowGoldBackPromotionBtn)); //프로모션 버튼을 활성화 한다

	MM().AddMsgH(UPG_CMK_ROOMLISTHEADER_CHANGE, Fnt(this, &CLobbyView::OnChangeGoldTabState)); //골드경기장은 무조건 참여가능 리스트로 노출한다.
	MM().AddMsgH(UPG_CMK_SET_FUNDEVENT_ENABLE, Fnt(this, &CLobbyView::OnSetFundEventEnable)); // 골드 적립금 이벤트 이미지 활성.
}

CLobbyView::~CLobbyView()
{
	StopTimer();
}

BEGIN_MESSAGE_MAP(CLobbyView, CWnd)
	//{{AFX_MSG(CLobbyView)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//}}AFX_MSG.
	ON_WM_ERASEBKGND()
	ON_WM_ENABLE()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CLobbyView, CWnd)
	/*
	외부 ActiveX 사용 할때
	DISP_FUNCTION(클래스명, 외부함수명, 맴버함수명, 리턴타입, 인자값1 인자값2) 
	참고 ) http://msdn.microsoft.com/ko-kr/library/cc451431(VS.71).aspx
	*/
	DISP_FUNCTION( CLobbyView, "PopupShopDialog", PopupShopDialog, VT_EMPTY, VTS_I4 VTS_BSTR )
	DISP_FUNCTION( CLobbyView, "PopupEventDialog", PopupEventDialog, VT_EMPTY, VTS_I4 )
	DISP_FUNCTION( CLobbyView, "PopupEventURLDialog", PopupEventURLDialog, VT_EMPTY, VTS_BSTR )
END_DISPATCH_MAP()


const int CLobbyView::GetWidth()
{
	return 1000;
}

const int CLobbyView::GetHeight()
{
	return 740;
}

BOOL CLobbyView::PreTranslateMessage( MSG* pMsg )
{
	return CWnd::PreTranslateMessage( pMsg );
}

LRESULT CLobbyView::WindowProc( UINT message, WPARAM wParam, LPARAM lParam )
{
	if( message >= WM_USER || message==WM_COMMAND )
	{
		if (m_pLobbyProcess->OnMessage(message, wParam, lParam))		
			return TRUE;		
	}

	switch(message)
	{
#ifdef _DEBUG
	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_F2:
			{
				static int stNodeCount = 0;

				++stNodeCount;
				CString strNode;

				Camel::Dar2::SS::Memo memo;
				strNode.Format("이승영%d", stNodeCount/3);
				memo.senderID	= strNode.GetString();
				strNode.Format("이건 테스트 메모입니다.[%ld]", stNodeCount);
				memo.content	= strNode.GetString();
				memo.recvTime	= "2011-01-20 19:00";
				memo.boxType	= Camel::Dar2::SS::IN_BOX;
				strNode.Format("seq%ld", stNodeCount);
				memo.node		= strNode.GetString();
				memo.read		= false;
				CCommunityManager::Instance()->OnReceiveMemo(memo);
			}
			break;
		case VK_F6:
			ShowMainMessageDlg("이것은 메시지 박스 테스트 입니다.\n테스트2??^^;;\n\n테스트가 잘되나요?\n\nOK?");
			break;
		}
		break;
#endif
	//커뮤니티 서버 연결 성공
	case CM_LOGIN_SUCCESS:
		{
			//자신의 위치 및 머니를 커뮤니티 친구서비스에 등록
			CPlayer *pPlayer = GM().GetMyInfo();
			/*CCommunityManager::Instance()->SendBuddyInfoChange(GM().GetCurrentGameType(), pPlayer->UI.nRoomNum, pPlayer->UI.GetMoney(IsTournament()));*/
			/*CCommunityManager::Instance()->SendBuddyInfoChange(GM().GetCurrentGameType(), pPlayer->UI.nRoomNum, pPlayer->UI.GetMoney());*/
			CCommunityManager::Instance()->SendBuddyInfoChange(GM().GetCurrentGameType(), pPlayer->UI.nRoomNum, pPlayer->UI.GetRealPMoney()); // 토너먼트 추가로 변경된 함수

			//쪽지 수신 여부 및 대화 수신 여부 옵션 정보 요청
			CCommunityManager::Instance()->SendGetOption();

			//읽지않는 쪽지 총 갯수 정보 요청
			CCommunityManager::Instance()->SendGetMemoInfo();
			//Buddy 리스트를 요청한다
			CCommunityManager::Instance()->SendGetBuddyGroupEntryList();
			CCommunityManager::Instance()->SendGetBlackBuddyList();
			CCommunityManager::Instance()->SendGetReverseBuddyList();

			//만약 쪽지함이 떠 있다면 쪽지함 리스트 및 보관쪽지함 리스트 요청을 한다.
			CMessageBox *pMessageBox = GetMessageBoxDlg();
			if(::IsWindow(pMessageBox->GetSafeHwnd()) && pMessageBox->IsWindowVisible())
			{
				CCommunityManager::Instance()->SendGetMemoList();
				CCommunityManager::Instance()->SendGetMemoStorageList();
			}

			//if(!wParam)
			//{
			//	ShowMainMessageDlg("커뮤니티 서버에 연결되었습니다.");
			//}

			m_pUserInfoGroup->SetComunityConnect(true);
			m_pBuddyListGroup->SetShowConnectBuddy();
			m_pReverseBuddyListGroup->SetShowIgnoreMyBuddy();
		}
		break;
	//커뮤니티 서버 연결 실패
	case CM_LOGIN_FAILE:
		m_pUserInfoGroup->SetComunityConnect(false);
		break;
	//쪽지 보내기 결과
	case CM_SEND_MEMO_RESULT:
		{
			LISTPAIRSTRING listResult;
			CCommunityManager::Instance()->ResultSendMemo(listResult);

			LISTPAIRSTRING::iterator start = listResult.begin();
			LISTPAIRSTRING::iterator end = listResult.end();

			std::list<std::string> listFaile;

			for(;start != end;++start)
			{
				if(!start->second)
					listFaile.push_back(start->first);
			}

			if(listFaile.size())
			{
				CString strFormat;
				strFormat.Format("다음 사용자에게 쪽지 보내기에 실패하였습니다.\n");
				std::list<std::string>::iterator start = listFaile.begin();
				std::list<std::string>::iterator end = listFaile.end();
				for(int i=0;start != end;++start,++i)
				{
					if(i != 0) strFormat += ",";
					strFormat += (*start).c_str();
				}
				ShowMainMessageDlg(strFormat);
			}
			else
			{
				ShowMainMessageDlg("쪽지가 정상적으로 발송되었습니다.");
			}
		}
		break;
	//쪽지함 리스트 받기 성공
	case CM_GET_MEMOLIST_SUCCESS:
		{
			CMessageBox *pMessageBox = GetMessageBoxDlg();
			if(::IsWindow(pMessageBox->GetSafeHwnd()) && pMessageBox->IsWindowVisible()) pMessageBox->ReceiveMemoList(true);
			else CCommunityManager::Instance()->MoveReciveMemoListToMomoList(NULL);
		}
		break;
	//쪽지함 리스트 받기 실패
	case CM_GET_MEMOLIST_FAILE:
		ShowMainMessageDlg("받은쪽지함 정보를 가져오지 못했습니다.");
		break;
	//보관 쪽지함 받기 성공
	case CM_GET_MEMOSTORAGE_SUCCESS:
		{
			CMessageBox *pMessageBox = GetMessageBoxDlg();
			if(::IsWindow(pMessageBox->GetSafeHwnd()) && pMessageBox->IsWindowVisible()) pMessageBox->ReceiveMemoList(false);
			else CCommunityManager::Instance()->MoveReciveStorageMemoListToStorageMomoList(NULL);
		}
		break;
	//보관쪽지함 받기 실패
	case CM_GET_MEMOSTORAGE_FAILE:
		ShowMainMessageDlg("저장쪽지함 정보를 읽어오는데 실패하였습니다.");
		break;
	//쪽지 정보 읽기 실패
	case CM_GET_MEMOINFO_SUCCESS:
		m_pUserInfoGroup->SetMemoCount(CCommunityManager::Instance()->GetUnReadMemoCount());
		break;
	case CM_BUDDYS_CONNECT_CHANGE:
		m_pUserInfoGroup->SetConnectBuddyCount(CCommunityManager::Instance()->GetConnectBuddyCount());
		break;
	//쪽지 정보 읽기 실패
	case CM_GET_MEMOINFO_FAILE:
		ShowMainMessageDlg("쪽지 정보를 읽어오는데 실패하였습니다.");
		break;
	//쪽지 읽기 성공
	case CM_READ_MEMO_SUCCESS:
		{
			CMessageBox *pMessageBox = GetMessageBoxDlg();
			if(::IsWindow(pMessageBox->GetSafeHwnd()) && pMessageBox->IsWindowVisible()) pMessageBox->OnReadMemoList();
			else
			{
				MEMO memo;
				if(CCommunityManager::Instance()->GetReadMemo(memo, NULL))
				{
					ShowReceiveMessageDialog(&memo, NULL);
				}
			}
		}
		break;
	//쪽지 읽기 실패
	case CM_READ_MEMO_FAILE:
		ShowMainMessageDlg("쪽지 읽기에 실패하였습니다.");
		break;
	//쪽지 수신 여부/대화 수신 여부 설정 성공
	case CM_SET_OPTIONINFO_SUCCESS:
		break;
	//쪽지 수신 여부/대화 수신 여부 설정 실패
	case CM_SET_OPTIONINFO_FAILE:
		{
			bool bMemo = lParam?true:false;
			if(bMemo) ShowMainMessageDlg("쪽지 수신 여부 옵션정보를 설정하는데 실패하였습니다.");
			else ShowMainMessageDlg("대화 수신 여부 옵션정보를 설정하는데 실패하였습니다.");
		}
		break;
	//쪽지 삭제 성공
	case CM_REMOVEMEMO_SUCCESS:
		{
			CMessageBox *pMessageBox = GetMessageBoxDlg();
			if(::IsWindow(pMessageBox->GetSafeHwnd()) && pMessageBox->IsWindowVisible()) pMessageBox->ResultRemoveMemo(true);
			else
			{
				bool bRemoveMemoBox = false;
				bool bRemoveStorageMemoBox = false;
				int nSendCount = 0;
				CCommunityManager::Instance()->ResultRemoveMemo(true, bRemoveMemoBox, bRemoveStorageMemoBox, nSendCount);
			}
		}
		break;
	//쪽지 삭제 실패
	case CM_REMOVEMEMO_FAILE:
		{
			CMessageBox *pMessageBox = GetMessageBoxDlg();
			if(::IsWindow(pMessageBox->GetSafeHwnd()) && pMessageBox->IsWindowVisible()) pMessageBox->ResultRemoveMemo(false);
			ShowMainMessageDlg("쪽지를 삭제하지 못했습니다.");
		}
		break;
	//보관함 이동 성공
	case CM_STORAGEMOVEMEMO_SUCCESS:
		{
			CMessageBox *pMessageBox = GetMessageBoxDlg();
			if(::IsWindow(pMessageBox->GetSafeHwnd()) && pMessageBox->IsWindowVisible()) pMessageBox->ResultMoveBoxToStrageMemo(true);
			else
			{
				int nSendCount = 0;
				CCommunityManager::Instance()->ResultMoveBoxToStrageMemo(true, nSendCount);

				CMemoNotifyDlg dlgNotify;
				dlgNotify.DoModal(GM().GetMainWnd(), CMemoNotifyDlg::EMNT_MEMO_MOVE_STORAGE);
			}
		}
		break;
	//보관함 이동 실패
	case CM_STORAGEMOVE_FAILE:
		{
			CMessageBox *pMessageBox = GetMessageBoxDlg();
			if(::IsWindow(pMessageBox->GetSafeHwnd()) && pMessageBox->IsWindowVisible()) pMessageBox->ResultMoveBoxToStrageMemo(false);
			ShowMainMessageDlg("쪽지보관함으로 이동하지 못했습니다.");
		}
		break;
	//커뮤니티 서버와의 접속 끊김
	case CM_DISCONNECT:
		CCommunityManager::Instance()->ReleaseConnectHandle();
		AllDisconnectDisableChattingDlg();
		m_pUserInfoGroup->SetComunityConnect(false);
		//ShowMainMessageDlg("커뮤니티 서버와의 접속이 끊어졌습니다. 잠시후 재접속을 시도하겠습니다.");
		break;
	//서버에서 중복 접속을 알려옴. 접속을 끊음
	case CM_DUPLICATE_DISCONNECT:
		CCommunityManager::Instance()->ReleaseConnectHandle();
		AllDisconnectDisableChattingDlg();
		m_pUserInfoGroup->SetComunityConnect(false);
		break;
	//쪽지 수신 여부/대화 수신 여부 옵션 정보 가져오기 성공
	case CM_GET_OPTIONINFO_SUCCESS:
		break;
	//쪽지 수신 여부/대화 수신 여부 옵션 정보 가져오기 성공
	case CM_GET_OPTIONINFO_FAILE:
		ShowMainMessageDlg("쪽지 수신 여부/대화 수신 여부 옵션정보를 가져오는데 실패하였습니다.");
		break;
	//쪽지 삭제 메시지를 보낼때
	case CM_SEND_REMOVE_MEMO:
		{
			CMessageBox *pMessageBox = GetMessageBoxDlg();
			if(::IsWindow(pMessageBox->GetSafeHwnd()) && pMessageBox->IsWindowVisible()) 
				pMessageBox->DisableList();
		}
		break;
	//보관함 쪽지로 이동 메시지를 보낼때
	case CM_SEND_MOVE_STORAGE_MEMO:
		{
			CMessageBox *pMessageBox = GetMessageBoxDlg();
			if(::IsWindow(pMessageBox->GetSafeHwnd()) && pMessageBox->IsWindowVisible())
				pMessageBox->DisableList();
		}
		break;
	//메모 수신
	case CM_RECEIVE_MEMO:
		{
			if(GM().GetCurrentWhere() != IDX_GAMEWHERE_LOBY) break;
			LISTMEMO listMemo;
			CCommunityManager::Instance()->GetReceiveMemoList(listMemo);
			Camel::Dar2::SS::Memo *pMemo = NULL;

			LISTMEMO::iterator start = listMemo.begin();
			LISTMEMO::iterator end = listMemo.end();

			// 쪽지알림으로 인한 로딩이 길어지는 이슈로 쪽지 수신 시 알림 제거.
			/*if(listMemo.size() != 0)
			{
				NMBASE::SOUND::g_Sound.PlayWav(SND_MEMORECEIVE);
			}*/

			//bool bShow = true;
			//if(bShow)
			//{
			//	for(;start != end;++start)
			//	{
			//		pMemo = *start;
			//		CreateMemoNotifyPopup(pMemo);
			//		delete pMemo;
			//	}
			//}
		}
		break;
	case CM_BUDDY_CONNECT:
		{
			if(GM().GetCurrentWhere() != IDX_GAMEWHERE_LOBY) break;
			LISTSTRING listConnectBuddyNickName;
			CCommunityManager::Instance()->GetConnectBuddyList(listConnectBuddyNickName);

			LISTSTRING::iterator start = listConnectBuddyNickName.begin();
			LISTSTRING::iterator end = listConnectBuddyNickName.end();

			if(listConnectBuddyNickName.size() != 0)
			{
				NMBASE::SOUND::g_Sound.PlayWav(SND_MEMORECEIVE);
			}

			for(;start != end;++start)
			{
				CreateMemoNotifyPopup(*start);
			}
		}
		break;
	case CM_SHOW_OPTION:
		GM().ShowOptionDlg(false);
		break;
	//채팅방 만들기 성공
	case CM_SEND_CREATE_CHATTING_ROOM_SUCCESS:
		{
			MAPCREATEROOM mapRoomID;
			CCommunityManager::Instance()->GetCreateChattingRoom(mapRoomID);
			if(mapRoomID.size() == 0) break;
			if(GM().GetCurrentWhere() != IDX_GAMEWHERE_LOBY) break;
			MAPCREATEROOM::iterator start = mapRoomID.begin();
			MAPCREATEROOM::iterator end = mapRoomID.end();
			for(;start != end;++start)
			{
				if(IsFullCreateRoom())
				{
					ShowMainMessageDlg("대화창은 최대 10개까지만 생성 가능합니다.");
					break;
				}

				ASSERT(GetChattingRoom(start->first) == NULL);

				CreateCattingRoom(start->first, start->second, true);
			}
		}
		break;
	//채팅방 만들기 실패
	case CM_SEND_CREATE_CHATTING_ROOM_FAILE:
		break;
	//방에 입장 성공
	case CM_SEND_JOIN_CHATTING_ROOM_SUCCESS:
		{
			LISTPAIRSTRING listJoinRoom;
			CCommunityManager::Instance()->GetJoinRoom(listJoinRoom);

			if(GM().GetCurrentWhere() != IDX_GAMEWHERE_LOBY) break;
			LISTPAIRSTRING::iterator start = listJoinRoom.begin();
			LISTPAIRSTRING::iterator end = listJoinRoom.end();

			CChattingRoomDlg *pChattingRoom = NULL;

			for(;start != end;++start)
			{
				pChattingRoom = GetChattingRoom(start->first);
				if(pChattingRoom) pChattingRoom->OnJoinSuccessRoom();
			}
		}
		break;
	//방에 입장 실패
	case CM_SEND_JOIN_CHATTING_ROOM_FAILE:
		{
			LISTSTRING listJoinFaileRoomID;
			CCommunityManager::Instance()->GetJoinFaileRoom(listJoinFaileRoomID);

			if(GM().GetCurrentWhere() != IDX_GAMEWHERE_LOBY) break;
			LISTSTRING::iterator start = listJoinFaileRoomID.begin();
			LISTSTRING::iterator end = listJoinFaileRoomID.end();
			CChattingRoomDlg *pChattingRoom = NULL;

			for(;start != end;++start)
			{
				pChattingRoom = GetChattingRoom(*start);
				if(pChattingRoom) pChattingRoom->OnJoinFaileRoom();
			}
		}
		break;
	case CM_RECEIVE_ENTER_CHATTING_ROOM:
		{
			LISTENTERCHATROOMUSER listEnterRoomUser;
			CCommunityManager::Instance()->GetEnterRoom(listEnterRoomUser);

			if(GM().GetCurrentWhere() != IDX_GAMEWHERE_LOBY) break;
			LISTENTERCHATROOMUSER::iterator start = listEnterRoomUser.begin();
			LISTENTERCHATROOMUSER::iterator end = listEnterRoomUser.end();
			CChattingRoomDlg *pChattingRoom = NULL;

			for(;start != end;++start)
			{
				pChattingRoom = GetChattingRoom(start->first);
				if(pChattingRoom) pChattingRoom->OnEnterRoom(start->second);
				else delete start->second;
			}
		}
		break;
	case CM_RECEIVE_LEAVE_CHATTING_ROOM:
		{
			LISTPAIRSTRING2 listLeaveRoom;
			CCommunityManager::Instance()->GetLeaveRoom(listLeaveRoom);

			if(GM().GetCurrentWhere() != IDX_GAMEWHERE_LOBY) break;
			LISTPAIRSTRING2::iterator start = listLeaveRoom.begin();
			LISTPAIRSTRING2::iterator end = listLeaveRoom.end();
			CChattingRoomDlg *pChattingRoom = NULL;

			for(;start != end;++start)
			{
				pChattingRoom = GetChattingRoom(start->first);
				if(pChattingRoom && ::IsWindow(pChattingRoom->GetSafeHwnd())) pChattingRoom->OnLeaveRoom(start->second);
			}
		}
		break;
	case CM_SEND_LEAVE_CHATTING_ROOM_FAILE:
		ShowMainMessageDlg("대화방에서 정상적으로 나가기 처리가 되지 않았습니다.");
		break;
	case CM_SEND_CATTING_MESSAGE_FAILE:
		{
			LISTPAIRSTRING2 listSendFaileChat;
			CCommunityManager::Instance()->GetSendFaileChat(listSendFaileChat);

			if(GM().GetCurrentWhere() != IDX_GAMEWHERE_LOBY) break;
			LISTPAIRSTRING2::iterator start = listSendFaileChat.begin();
			LISTPAIRSTRING2::iterator end = listSendFaileChat.end();
			CChattingRoomDlg *pChattingRoom = NULL;

			for(;start != end;++start)
			{
				pChattingRoom = GetChattingRoom(start->first);
				if(pChattingRoom) pChattingRoom->OnSendFaileChat(start->second);
			}
		}
		break;
	case CM_RECEIVE_CAHTTING_MESSAGE:
		{
			MAPPAIRSTRING mapReceiveChat;
			CCommunityManager::Instance()->GetReceiveChat(mapReceiveChat);

			if(GM().GetCurrentWhere() != IDX_GAMEWHERE_LOBY) break;
			MAPPAIRSTRING::iterator start = mapReceiveChat.begin();
			MAPPAIRSTRING::iterator end = mapReceiveChat.end();

			LISTPAIRSTRING2::iterator start_msg, end_msg;

			CChattingRoomDlg *pChattingRoom = NULL;

			for(;start != end;++start)
			{
				pChattingRoom = GetChattingRoom(start->first);
				if(pChattingRoom)
				{
					start_msg = start->second.begin();
					end_msg = start->second.end();

					for(;start_msg != end_msg;++start_msg)
					{
						pChattingRoom->OnReceiveChat(start_msg->first, start_msg->second);
					}
				}
			}
		}
		break;
	case CM_RECEIVE_ROOM_USER_LIST:
		{
			MAPROOMUSERLIST mapRoomUser;
			CCommunityManager::Instance()->GetRoomUserList(mapRoomUser);

			if(GM().GetCurrentWhere() != IDX_GAMEWHERE_LOBY) break;
			MAPROOMUSERLIST::iterator start = mapRoomUser.begin();
			MAPROOMUSERLIST::iterator end = mapRoomUser.end();

			LISTCHATUSER::iterator start_user, end_user;

			CChattingRoomDlg *pChattingRoom = NULL;

			for(;start != end;++start)
			{
				start_user = start->second.begin();
				end_user = start->second.end();

				pChattingRoom = GetChattingRoom(start->first);
				if(pChattingRoom)
				{
					for(;start_user != end_user;++start_user)
					{
						pChattingRoom->AddUser(*start_user);
					}
				}
				else
				{
					for(;start_user != end_user;++start_user)
					{
						delete *start_user;
					}
				}
			}
		}
		break;
	case CM_SEND_INVITE_CHATTING_ROOM_FAILE:
		{
			MAPLISTSTRING mapInvateFaileUser;
			CCommunityManager::Instance()->GetInviteFaileUser(mapInvateFaileUser);

			if(GM().GetCurrentWhere() != IDX_GAMEWHERE_LOBY) break;
			MAPLISTSTRING::iterator start = mapInvateFaileUser.begin();
			MAPLISTSTRING::iterator end = mapInvateFaileUser.end();

			LISTSTRING::iterator start_user, end_user;

			CChattingRoomDlg *pChattingRoom = NULL;

			for(;start != end;++start)
			{
				pChattingRoom = GetChattingRoom(start->first);
				if(pChattingRoom)
				{
					start_user = start->second.begin();
					end_user = start->second.end();

					for(;start_user != end_user;++start_user)
					{
						pChattingRoom->OnInvateFaileUser(*start_user);
					}
				}
			}
		}
		break;
	case CM_RECEIVE_USER_PRESENCE:
		CCommunityManager::Instance()->SetUserPresence();
		break;
	case CM_RECEIVE_INVITE_CATTING_ROOM:
		{
			LISTPAIRSTRING2 listInviteChatRoom;
			CCommunityManager::Instance()->GetInviteChattingRoom(listInviteChatRoom);

			if(GM().GetCurrentWhere() != IDX_GAMEWHERE_LOBY) break;
			LISTPAIRSTRING2::iterator start = listInviteChatRoom.begin();
			LISTPAIRSTRING2::iterator end = listInviteChatRoom.end();

			for(;start != end;++start)
			{
				if(!IsExistInviteChatting(start->first, start->second))
				{
					CreateInviteCatting(start->first, start->second);
				}
			}
		}
		break;
	case CM_CLOSE_CHATTING_ROOM:
		CloseChattingDlg((CChattingRoomDlg *)lParam);
		break;
	case CM_CLOSE_INVITE_CHATTING_ROOM:
		CloseInviteChatting((CChattingRoomInviteDlg *)lParam, (UINT)wParam);
		break;
	case CM_CLOSE_MEMO_NOTIFY_POPUP:
		CloseMemoNotifyPopup((CMessageNotificationPopup *)lParam);
		break;
	case UPGM_POPUPMEMU_CLOSE:
		if(m_pPopupMenu)
		{
			m_pPopupMenu->DestroyWindow();
			delete m_pPopupMenu;
			m_pPopupMenu = NULL;
		}
		OnClickPopupMenu(wParam);
		break;
	case CM_SEND_GET_COMMUNITY_USERINFO_SUCCESS:
		break;
	case CM_SEND_GET_COMMUNITY_USERINFO_FAILE:
		{
			CCommunityUserInfo *pCommunityUserInfo = (CCommunityUserInfo *)lParam;

			switch(pCommunityUserInfo->GetType())
			{
			case CCommunityUserInfo::ECUIT_SENDMEMO:
				ShowMainMessageDlg("쪽지를 발송하지 못했습니다.");
				break;
			case CCommunityUserInfo::ECUIT_ADDBLACKLIST:
				{
					CBlackBuddyAddDlg dlg;
					dlg.DoModal(CBlackBuddyAddDlg::BBADT_MESSAGE, "블랙리스트에 추가하지 못햇습니다.", GM().GetMainWnd());
				}
				break;
			case CCommunityUserInfo::ECUIT_ADDBUDDY:
				{
					CBuddyAddResultDlg dlg;
					dlg.DoModal("내친구 목록에 추가하지 못햇습니다.", GM().GetMainWnd());
				}
				break;
			case CCommunityUserInfo::ECUIT_SHOWPROFILE:
				ShowMainMessageDlg("유저정보를 가져오지 못했습니다.");
				break;
			case CCommunityUserInfo::ECUIT_INVITECHAT:
				{
					CChattingRoomDlg *pDlg = (CChattingRoomDlg *)pCommunityUserInfo->GetParam();
					if(IsShowChattingDlg(pDlg))
					{
						CMsgBoxDlg dlg(pDlg);
						dlg.DoModal(CMsgBoxDlg::EMBT_NOTICE, "초대 메시지를 보내지 못했습니다.");
					}
				}
				break;
			case CCommunityUserInfo::ECUIT_FINDCHATADDUSER:
				{
					CChattingRoomAddUserDlg *pDlg = GetIDToChattingRoomAddUserDlg((int)pCommunityUserInfo->GetParam());
					if(pDlg) pDlg->OnNotFind();
				}
				break;
			case CCommunityUserInfo::ECUIT_FINDMEMOADDUSER:
				{
					GetSendMessageDlg()->OnNotFind((int)pCommunityUserInfo->GetParam());
				}
				break;
			case CCommunityUserInfo::ECUIT_MEMOBOX:
				{
					CMessageBox *pDlg = GetMessageBoxDlg();
					if(pDlg == (CMessageBox *)pCommunityUserInfo->GetParam() && ::IsWindow(pDlg->GetSafeHwnd()) && pDlg->IsWindowVisible())
					{
						CMsgBoxDlg dlg(pDlg);
						dlg.DoModal(CMsgBoxDlg::EMBT_NOTICE, "유저 정보를 가져오지 못했습니다.");
					}
				}
				break;
			}

			delete pCommunityUserInfo;
		}
		break;
	case CM_RECEIVE_GET_COMMUNITY_USERINFO_FAILE:
		{
			CCommunityUserInfo *pCommunityUserInfo = (CCommunityUserInfo *)lParam;
			switch(pCommunityUserInfo->GetType())
			{
			case CCommunityUserInfo::ECUIT_SENDMEMO:
				{
					ShowMainMessageDlg("쪽지를 발송하지 못했습니다.");
				}
				break;
			case CCommunityUserInfo::ECUIT_ADDBLACKLIST:
				{
					CBlackBuddyAddDlg dlg;
					dlg.DoModal(CBlackBuddyAddDlg::BBADT_MESSAGE, "유저정보를 찾지 못하여 블랙리스트에 추가하지 못햇습니다.", GM().GetMainWnd());
				}
				break;
			case CCommunityUserInfo::ECUIT_ADDBUDDY:
				{
					CBuddyAddResultDlg dlg;
					dlg.DoModal("유저정보를 찾지 못하여 내친구 목록에 추가하지 못햇습니다.", GM().GetMainWnd());
				}
				break;
			case CCommunityUserInfo::ECUIT_SHOWPROFILE:
				ShowMainMessageDlg("유저정보를 가져오지 못했습니다.");
				break;
			case CCommunityUserInfo::ECUIT_INVITECHAT:
				{
					CChattingRoomDlg *pDlg = (CChattingRoomDlg *)pCommunityUserInfo->GetParam();
					if(IsShowChattingDlg(pDlg))
					{
						pDlg->SendInviteCahtRoom("","",false,false);
					}
				}
				break;
			case CCommunityUserInfo::ECUIT_FINDCHATADDUSER:
				{
					CChattingRoomAddUserDlg *pDlg = GetIDToChattingRoomAddUserDlg((int)pCommunityUserInfo->GetParam());
					if(pDlg) pDlg->OnNotFind();
				}
				break;
			case CCommunityUserInfo::ECUIT_FINDMEMOADDUSER:
				{
					GetSendMessageDlg()->OnNotFind((int)pCommunityUserInfo->GetParam());
				}
				break;
			case CCommunityUserInfo::ECUIT_MEMOBOX:
				{
					CMessageBox *pDlg = GetMessageBoxDlg();
					if(pDlg == (CMessageBox *)pCommunityUserInfo->GetParam() && ::IsWindow(pDlg->GetSafeHwnd()) && pDlg->IsWindowVisible())
					{
						CMsgBoxDlg dlg(pDlg);
						dlg.DoModal(CMsgBoxDlg::EMBT_NOTICE, "유저 정보를 가져오지 못했습니다.");
					}
				}
				break;
			}
			delete pCommunityUserInfo;
		}
		break;
	case CM_RECEIVE_GET_COMMUNITY_USERINFO_SUCCESS:
		{
			CCommunityUserInfo *pCommunityUserInfo = (CCommunityUserInfo *)lParam;
			switch(pCommunityUserInfo->GetType())
			{
			case CCommunityUserInfo::ECUIT_SENDMEMO:
				{
					const std::list<CChatUser *> &listUser = pCommunityUserInfo->GetUserList();
					std::list<std::string> listFailID;
					std::list<std::string> listSuccessID;

					std::list<CChatUser *>::const_iterator start = listUser.begin();
					std::list<CChatUser *>::const_iterator end = listUser.end();

					CChatUser *pUser = NULL;
					for(;start != end;++start)
					{
						pUser = *start;
						if(pUser->GetResult())
						{
							listFailID.push_back(pUser->GetID());
						}
						else
						{
							listSuccessID.push_back(pUser->GetID());
						}
					}

					if(listFailID.size() != 0)
					{
						std::list<std::string>::iterator start = listFailID.begin();
						std::list<std::string>::iterator end = listFailID.end();

						CString strFormat = "아래의 리스트는 존재하지 않는 아이디/닉네임이므로 쪽지를 발송하지 못했습니다.\n";
						bool bFirst = true;
						for(;start != end;++start)
						{
							if(bFirst)
							{
								bFirst = false;
							}
							else
							{
								strFormat += ", ";
							}

							strFormat += start->c_str();
						}

						ShowMainMessageDlg(strFormat);
					}

					if(listSuccessID.size() == 0)
					{
						ShowMainMessageDlg("쪽지를 받을 유저가 존재하지 않으므로 쪽지를 발송하지 않습니다.");
					}
					else
					{
						CCommunityManager::Instance()->SendMemo(listSuccessID, pCommunityUserInfo->GetMessage());
					}
				}
				break;
			case CCommunityUserInfo::ECUIT_INVITECHAT:
				{
					CChattingRoomDlg *pDlg = (CChattingRoomDlg *)pCommunityUserInfo->GetParam();
					if(IsShowChattingDlg(pDlg))
					{
						const std::list<CChatUser *> &listUser = pCommunityUserInfo->GetUserList();
						
						std::list<CChatUser *>::const_iterator start = listUser.begin();
						std::list<CChatUser *>::const_iterator end = listUser.end();
						CChatUser *pUserInfo = NULL;
						for(;start != end;++start)
						{
							pUserInfo = *start;
							if(pUserInfo->GetResult())
							{
								pDlg->SendInviteCahtRoom(pUserInfo->GetID(), pUserInfo->GetNickName(), true, false);
							}
							else
							{
								pDlg->SendInviteCahtRoom(pUserInfo->GetID(), pUserInfo->GetNickName(), true, true);
							}
						}
					}
				}
				break;
			case CCommunityUserInfo::ECUIT_FINDCHATADDUSER:
				{
					CChattingRoomAddUserDlg *pDlg = GetIDToChattingRoomAddUserDlg((int)pCommunityUserInfo->GetParam());
					if(pDlg)
					{
						const std::list<CChatUser *> &listUser = pCommunityUserInfo->GetUserList();

						std::list<CChatUser *>::const_iterator start = listUser.begin();
						std::list<CChatUser *>::const_iterator end = listUser.end();
						CChatUser *pUserInfo = NULL;
						for(;start != end;++start)
						{
							pUserInfo = *start;
							if(!pUserInfo->GetResult())
							{
								pDlg->OnFind(pUserInfo->GetID(), pUserInfo->GetNickName());
							}
						}
					}
				}
				break;
			case CCommunityUserInfo::ECUIT_FINDMEMOADDUSER:
				{
					const std::list<CChatUser *> &listUser = pCommunityUserInfo->GetUserList();

					std::list<CChatUser *>::const_iterator start = listUser.begin();
					std::list<CChatUser *>::const_iterator end = listUser.end();
					CChatUser *pUserInfo = NULL;
					for(;start != end;++start)
					{
						pUserInfo = *start;
						if(!pUserInfo->GetResult())
						{
							GetSendMessageDlg()->OnFind((int)pCommunityUserInfo->GetParam(), pUserInfo->GetID(), pUserInfo->GetNickName());
						}
					}
				}
				break;
			case CCommunityUserInfo::ECUIT_MEMOBOX:
				{
					CMessageBox *pDlg = GetMessageBoxDlg();
					if(pDlg == (CMessageBox *)pCommunityUserInfo->GetParam() && ::IsWindow(pDlg->GetSafeHwnd()) && pDlg->IsWindowVisible())
					{
						const std::list<CChatUser *> &listUser = pCommunityUserInfo->GetUserList();
						CChatUser *pUserInfo = NULL;
						if(listUser.size() > 0)
						{
							pUserInfo = listUser.front();
							if(pUserInfo->GetResult())
							{
								pUserInfo = NULL;
							}
						}

						if(pUserInfo)
						{
							AskProfileToServer(0, pUserInfo->GetID().c_str(), pUserInfo->GetCN().c_str());
						}
						else
						{
							CMsgBoxDlg dlg(pDlg);
							dlg.DoModal(CMsgBoxDlg::EMBT_NOTICE, "유저 정보를 가져오지 못했습니다. 아이디/닉네임이 존재하지 않을 수 있습니다.");
						}
					}
				}
				break;
			case CCommunityUserInfo::ECUIT_ADDBLACKLIST:
				{
					const std::list<CChatUser *> &listUser = pCommunityUserInfo->GetUserList();
					std::list<CChatUser *>::const_iterator start = listUser.begin();
					std::list<CChatUser *>::const_iterator end = listUser.end();

					ASSERT(start != end);

					CChatUser *pUser = *start;

					if(pUser->GetResult())
					{
						CBlackBuddyAddDlg dlg;
						dlg.DoModal(CBlackBuddyAddDlg::BBADT_MESSAGE, "유저정보를 찾지 못하여 블랙리스트에 추가하지 못햇습니다.", GM().GetMainWnd());
					}
					else
					{
						if(CCommunityManager::Instance()->IsExistBlackBuddy(pUser->GetNickName()))
						{
							CBlackBuddyAddDlg dlg;
							dlg.DoModal(CBlackBuddyAddDlg::BBADT_MESSAGE, "이미 블랙리스트에 추가되어 있는 사용자 입니다.", GM().GetMainWnd());
						}
						else if(CCommunityManager::Instance()->GetMyID() == pUser->GetID())
						{
							CBlackBuddyAddDlg dlg;
							dlg.DoModal(CBlackBuddyAddDlg::BBADT_MESSAGE, "자기 자신을 블랙리스트에 추가할 수 없습니다.", GM().GetMainWnd());
						}
						else
						{
							CCommunityManager::Instance()->SendAddBlackBuddy(pUser->GetID());
						}
					}
				}
				break;
			case CCommunityUserInfo::ECUIT_ADDBUDDY:
				{
					const std::list<CChatUser *> &listUser = pCommunityUserInfo->GetUserList();
					std::list<CChatUser *>::const_iterator start = listUser.begin();
					std::list<CChatUser *>::const_iterator end = listUser.end();

					ASSERT(start != end);

					CChatUser *pUser = *start;

					if(pUser->GetResult())
					{
						CBuddyAddResultDlg dlg;
						dlg.DoModal("유저정보를 찾지 못하여 내친구 목록에 추가하지 못햇습니다.", GM().GetMainWnd());
					}
					else
					{
						if(CCommunityManager::Instance()->IsExistBuddy(pUser->GetNickName()))
						{
							CBuddyAddResultDlg dlg;
							dlg.DoModal("이미 내친구 목록에 추가되어 있는 사용자 입니다.", GM().GetMainWnd());
						}
						else if(CCommunityManager::Instance()->GetMyID() == pUser->GetID())
						{
							CBuddyAddResultDlg dlg;
							dlg.DoModal("자기 자신을 친구로 추가할 수 없습니다.", GM().GetMainWnd());
						}
						else
						{
							CCommunityManager::Instance()->SendAddBuddy(pUser->GetID(), pCommunityUserInfo->GetMessage());
						}
					}
				}
				break;
			case CCommunityUserInfo::ECUIT_SHOWPROFILE:
				{
					const std::list<CChatUser *> &listUser = pCommunityUserInfo->GetUserList();
					std::list<CChatUser *>::const_iterator start = listUser.begin();
					std::list<CChatUser *>::const_iterator end = listUser.end();

					ASSERT(start != end);

					CChatUser *pUser = *start;

					if(pUser->GetResult())
					{
						CBlackBuddyAddDlg dlg;
						dlg.DoModal(CBlackBuddyAddDlg::BBADT_MESSAGE, "유저정보를 찾지 못하였습니다.", GM().GetMainWnd());
					}
					else
					{
						AskProfileToServer(0, pUser->GetID().c_str(), pUser->GetCN().c_str());
					}
				}
				break;
			}
			delete pCommunityUserInfo;
		}
		break;
	case UPGM_POPUPMEMU_OPEN:
		if(m_pPopupMenu)
		{
			m_pPopupMenu->DestroyWindow();
			delete m_pPopupMenu;
			m_pPopupMenu = NULL;
		}

		m_pPopupMenu = (CPopupMemu *)lParam;
		break;
	case CM_RECEIVE_GET_BUDDY_GROUP_ENTRY_LIST_SUCCESS:
		m_pBuddyListGroup->OnReceiveBuddyGroupEntryList();
		break;
	case CM_RECEIVE_GET_BUDDY_GROUP_ENTRY_LIST_FAILE:
		ShowMainMessageDlg("친구 리스트를 가져오지 못했습니다.");
		break;
	case CM_RECEIVE_GET_BLACK_BUDDY_LIST_SUCCESS:
		m_pBlackBuddyListGroup->OnReceiveBlackBuddyList();
		break;
	case CM_RECEIVE_GET_BLACK_BUDDY_LIST_FAILE:
		ShowMainMessageDlg("블랙 리스트를 가져오지 못했습니다.");
		break;
	case CM_RECEIVE_GET_REVERSE_BUDDY_LIST_SUCCESS:
		m_pReverseBuddyListGroup->OnReceiveReverseBuddyList();
		break;
	case CM_RECEIVE_GET_REVERSE_BUDDY_LIST_FAILE:
		ShowMainMessageDlg("나를 추가한 친구 리스트를 가져오지 못했습니다.");
		break;
	case CM_RECEIVE_ADD_BLACK_BUDDY_SUCCESS:
		{
			CCommunityManager::Instance()->SetAddBlackBuddyToListObject();
			CBlackBuddyAddDlg dlg;
			dlg.DoModal(CBlackBuddyAddDlg::BBADT_MESSAGE, "블랙리스트에 추가되었습니다.", GM().GetMainWnd());
		}
		break;
	case CM_RECEIVE_ADD_BLACK_BUDDY_FAILE:
		ShowMainMessageDlg("블랙리스트에 추가히지 못했습니다.");
		break;
	case CM_RECEIVE_GET_BUDDY_GROUP_NAME_CHANGE_SUCCESS:
		m_pBuddyListGroup->OnReceiveBuddyGroupNameChange();
		break;
	case CM_RECEIVE_GET_BUDDY_GROUP_NAME_CHANGE_FAILE:
		ShowMainMessageDlg("그룹명 변경에 실패하였습니다. 잠시 후 다시 시도해 주세요.");
		break;
	case CM_RECEIVE_REMOVE_BLACK_BUDDY_SUCCESS:
		CCommunityManager::Instance()->SetRemoveBlackBuddyFromListObject();
		break;
	case CM_RECEIVE_REMOVE_BLACK_BUDDY_FAILE:
		ShowMainMessageDlg("블랙리스트에서 제거하지 못했습니다.");
		break;
	case CM_RECEIVE_BUDDY_GROUP_ADD_SUCCESS:
		m_pBuddyListGroup->OnReceiveBuddyGroupAdd();
		break;
	case CM_RECEIVE_BUDDY_GROUP_ADD_FAILE:
		ShowMainMessageDlg("그룹을 추가하지 못했습니다.");
		break;
	case CM_RECEIVE_BUDDY_GROUP_REMOVE_SUCCESS:
		{
			m_pBuddyListGroup->OnReceiveBuddyGroupRemove();
			CBuddyGroupRemoveDlg dlg;
			dlg.DoModal(CBuddyGroupRemoveDlg::BGRDT_MESSAGE, "그룹을 삭제하였습니다.", GM().GetMainWnd());
		}
		break;
	case CM_RECEIVE_BUDDY_GROUP_REMOVE_FAILE:
		ShowMainMessageDlg("그룹을 삭제하지 못했습니다.");
		break;
	case CM_RECEIVE_BUDDY_ADD_SUCCESS:
		CCommunityManager::Instance()->SendAddBuddyToGroup();
		break;
	case CM_RECEIVE_BUDDY_ADD_FAILE:
		{
			CBuddyAddResultDlg dlg;
			dlg.DoModal("내친구 목록에 추가하지 못했습니다.", GM().GetMainWnd());
		}
		break;
	case CM_RECEIVE_BUDDY_ADD_TO_GROUP_SUCCESS:
		CCommunityManager::Instance()->SetAddBuddyToListObject();
		break;
	case CM_RECEIVE_BUDDY_ADD_TO_GROUP_FAILE:
		{
			CBuddyAddResultDlg dlg;
			dlg.DoModal("추가한 친구를 그룹에 추가 못했습니다.", GM().GetMainWnd());
		}
		break;
	case CM_RECEIVE_BUDDY_REMOVE_FROM_GROUP_SUCCESS:
		CCommunityManager::Instance()->SendRemoveBuddy();
		break;
	case CM_RECEIVE_BUDDY_REMOVE_FROM_GROUP_FAILE:
		{
			CBuddyRemoveDlg dlg; 
			dlg.DoModal(CBuddyRemoveDlg::BRDT_MESSAGE, "친구 목록에서 선택한 친구를 제거하지 못했습니다.", GM().GetMainWnd());
		}
		break;
	case CM_RECEIVE_MOVE_BUDDY_TO_GROUP_SUCCESS:
		CCommunityManager::Instance()->SetMoveBuddyToGroup();
		break;
	case CM_RECEIVE_MOVE_BUDDY_TO_GROUP_FAILE:
		{
			CBuddyMoveGroupDlg dlg;
			dlg.DoModal("선택한 친구의 그룹을 이동하지 못하였습니다.", GM().GetMainWnd());
		}
		break;
	case CM_RECEIVE_REVERSCE_BUDDY_NTF:
		CCommunityManager::Instance()->SetReverseBuddyNtfToListObject();
		break;
	}

	return CWnd::WindowProc( message, wParam, lParam );
}

LRESULT CLobbyView::OnSendCreateChatting(WPARAM &wParam, LPARAM &lParam)
{
	if(CCommunityManager::Instance()->IsConnect())
	{
		if(IsFullCreateRoom())
		{
			ShowMainMessageDlg("대화창은 최대 10개 까지만 만들수 있습니다.");
		}
		else
		{
			CCommunityManager::Instance()->SendCreateRoom(*((std::list<std::string> *)wParam), "");
		}
	}
	else
	{
		ShowMainMessageDlg((char *)lParam);
	}
	return TRUE;
}

LRESULT CLobbyView::OnCheckShowCommunityWindow(WPARAM &wParam, LPARAM &lParam)
{
	bool bShowCommunityWindow = false;
	if(m_mapChattingDlg.size() != 0) bShowCommunityWindow = true;
	CMessageBox *pMemoBoxDlg = GetMessageBoxDlg();
	if(pMemoBoxDlg->GetSafeHwnd() && pMemoBoxDlg->IsWindowVisible()) bShowCommunityWindow = true;
	CSendMessageDlg *pSendMemoDlg = GetSendMessageDlg();
	if(pSendMemoDlg->GetSafeHwnd() && pSendMemoDlg->IsWindowVisible()) bShowCommunityWindow = true;
	CReceiveMessageDlg *pReceiveMemoDlg = GetReceiveMessageDlg();
	if(pReceiveMemoDlg->GetSafeHwnd() && pReceiveMemoDlg->IsWindowVisible()) bShowCommunityWindow = true;
	CReceiveAdminMessageDlg *pReceiveAdminMemoDlg = GetReceiveAdminMessageDlg();
	if(pReceiveAdminMemoDlg->GetSafeHwnd() && pReceiveAdminMemoDlg->IsWindowVisible()) bShowCommunityWindow = true;

	if(bShowCommunityWindow)
	{
		CMsgBoxDlg dlg(GM().GetMainWnd());
		if(dlg.DoModal(CMsgBoxDlg::EMBT_ENTERROOM, "게임방에 입장하게 되면 대화/쪽지 관련 모든 창이 닫히게 됩니다. 그래도 입장하시겠습니까?") == IDCANCEL) return FALSE;

		AllCloseChattingDlg();
		AllCloseInviteChatting();
		AllCloseMemoNotifyPopup();

		if(pReceiveAdminMemoDlg->GetSafeHwnd() && pReceiveAdminMemoDlg->IsWindowVisible()) pReceiveAdminMemoDlg->PostMessage(WM_COMMAND, IDOK);
		if(pReceiveMemoDlg->GetSafeHwnd() && pReceiveMemoDlg->IsWindowVisible()) pReceiveMemoDlg->PostMessage(WM_COMMAND, IDOK);
		if(pSendMemoDlg->GetSafeHwnd() && pSendMemoDlg->IsWindowVisible()) pSendMemoDlg->PostMessage(WM_COMMAND, IDCANCEL);
		if(pMemoBoxDlg->GetSafeHwnd() && pMemoBoxDlg->IsWindowVisible()) pMemoBoxDlg->PostMessage(WM_COMMAND, IDOK);
	}
	return TRUE;
}

LRESULT CLobbyView::OnAddWhisper(WPARAM &wParam, LPARAM &lParam)
{
	bool bSetCurSelect = (wParam & 0x00000001)?true:false;
	bool bStartAnimation = (wParam & 0x00000002)?true:false;
	bool bAllUser = (wParam & 0x00000004)?true:false;

	m_pChatGroup->AddWhisper((LPCTSTR)lParam, bSetCurSelect, bStartAnimation, bAllUser);
	
	//if(wParam && GetCurrentWhere() == IDX_GAMEWHERE_LOBY)
	//	RaiseChatView();

	return TRUE;
}

LRESULT CLobbyView::OnSetPokerIcon(WPARAM &wParam, LPARAM &lParam)
{
	HICON hIcon = GM().GetCurrentGame()->GetChatIcon();

	LISTCHATTINGROOM::iterator start = m_mapChattingDlg.begin();
	LISTCHATTINGROOM::iterator end = m_mapChattingDlg.end();

	CChattingRoomDlg *pChatRoom = NULL;
	for(;start != end;++start)
	{
		pChatRoom = start->second;
		pChatRoom->SetIcon(hIcon, FALSE);
	}

	return TRUE;
}

LRESULT CLobbyView::OnSetUserListTabType(WPARAM &wParam, LPARAM &lParam)
{
	LSY::CObjectProject *pProject = GetObjectMan()->GetProjectFromID(ID_LOBBY);
	LSY::CTabControl *pUpTab = (LSY::CTabControl *)pProject->GetObject(ID_LOBBY_TAB_USERGROUP);
	LSY::CTabControl *pDownTab = (LSY::CTabControl *)pProject->GetObject(ID_LOBBY_TAB_FRIEND);

	switch(wParam)
	{
	case ESUT_CONNECT_USER:
		pUpTab->SetCurSelFromID(ID_LOBBY_RAD_USERLIST_CONNECT);
		break;
	case ESUT_MY_BUDDY:
		pUpTab->SetCurSelFromID(ID_LOBBY_RAD_USERLIST_FRIEND);
		pDownTab->SetCurSelFromID(ID_LOBBY_RAD_MY_FRIEND);
		break;
	case ESUT_CLUB:
		pUpTab->SetCurSelFromID(ID_LOBBY_RAD_USERLIST_FRIEND);
		pDownTab->SetCurSelFromID(ID_LOBBY_RAD_MY_CLUB);
		break;
	case ESUT_CAFE:
		pUpTab->SetCurSelFromID(ID_LOBBY_RAD_USERLIST_FRIEND);
		pDownTab->SetCurSelFromID(ID_LOBBY_RAD_MY_CAFE);
		break;
	case ESUT_BLACK_BUDDY:
		pUpTab->SetCurSelFromID(ID_LOBBY_RAD_USERLIST_FRIEND);
		pDownTab->SetCurSelFromID(ID_LOBBY_RAD_MY_BLACKLIST);
		break;
	}

	return TRUE;
}

LRESULT CLobbyView::OnGetUserListTabType(WPARAM &wParam, LPARAM &lParam)
{
	LSY::CObjectProject *pProject = GetObjectMan()->GetProjectFromID(ID_LOBBY);
	LSY::CTabControl *pUpTab = (LSY::CTabControl *)pProject->GetObject(ID_LOBBY_TAB_USERGROUP);
	LSY::CTabControl *pDownTab = (LSY::CTabControl *)pProject->GetObject(ID_LOBBY_TAB_FRIEND);
	
	if (pUpTab->GetCurSelID() == ID_LOBBY_RAD_USERLIST_CONNECT) return ESUT_CONNECT_USER;
	
	switch(pDownTab->GetCurSelID())
	{
	case ID_LOBBY_RAD_MY_FRIEND:
		return ESUT_MY_BUDDY;
	case ID_LOBBY_RAD_MY_CLUB:
		return ESUT_CLUB;
	case ID_LOBBY_RAD_MY_CAFE:
		return ESUT_CAFE;
	}
	
	return ESUT_BLACK_BUDDY;
}

LRESULT CLobbyView::OnSetUserListTabActive(WPARAM &wParam, LPARAM &lParam)
{
	LSY::CObjectProject *pProject = GetObjectMan()->GetProjectFromID(ID_LOBBY);
	LSY::CTabControl *pUpTab = (LSY::CTabControl *)pProject->GetObject(ID_LOBBY_TAB_USERGROUP);
	LSY::CTabControl *pDownTab = (LSY::CTabControl *)pProject->GetObject(ID_LOBBY_TAB_FRIEND);
	
	int nRadIdx = wParam;
	bool bEnable = lParam;
	
	pDownTab->GetRadioFromIndex(nRadIdx)->SetEnable(bEnable);
	//pDownTab->GetRadioFromIndex(nRadIdx)->SetOneStatusButton(2);
	
	return TRUE;
}

LRESULT CLobbyView::OnChangeMyUserInfo(WPARAM &wParam, LPARAM &lParam)
{
	SetMyUserInfo(&GM().GetMyInfo()->UI, TRUE);

	return TRUE;
}

LRESULT CLobbyView::OnChangePromotionLobby(WPARAM &wParam, LPARAM &lParam)
{
	if(NULL!=m_pRoomListGroup){
		m_pRoomListGroup->ChangeRoomListState();
	}
	return TRUE;
}

LRESULT CLobbyView::OnShowChickenPromotionBtn(WPARAM &wParam, LPARAM &lParam)
{
	if(NULL!=m_pBtnChickenPromotion){
		m_pBtnChickenPromotion->SetShow((bool)wParam);
		m_pBtnChickenPromotion->SetEnable((bool)wParam);
	}
	return TRUE;
}

LRESULT CLobbyView::OnShowGoldBackPromotionBtn(WPARAM &wParam, LPARAM &lParam)
{
	m_pTabGame->SetShowGoldBackBtn((bool)wParam);
	return TRUE;
}

LRESULT CLobbyView::OnChangeGoldTabState(WPARAM &wParam, LPARAM &lParam)
{
	if(NULL!=m_pRoomListGroup){
		m_pRoomListGroup->ChangeGoldTabState((bool)wParam, (int)lParam);
	}
	return TRUE;
}

LRESULT CLobbyView::OnSetFundEventEnable(WPARAM &wParam, LPARAM &lParam)
{
	SetFundEventEnable();
	return TRUE;
}

void CLobbyView::OnOK()
{
}

void CLobbyView::OnCancel()
{
}

int CLobbyView::OnCreate( LPCREATESTRUCT lpCreateStruct ) 
{
	if( CWnd::OnCreate( lpCreateStruct ) == -1 )
		return -1;

	NMBASE::SKINGDI::NMRESAM.Initialize(this);
	NMBASE::SKINGDI::NMRESAM.RegistAudio(".\\Lobby\\script\\audio.nml");
	NMBASE::SKINGDI::NMRESSM.RegistSprite(".\\Lobby\\script\\sprite.nml");

	LSY::CObjectMan *pObjectMan = CreateObjectMan(this, GM().GetMainWnd());
	pObjectMan->LoadXml("poker.lsyproj");
	pObjectMan->SetUseCustomCursor(true);

	LSY::CObjectProject *pProject = pObjectMan->GetProjectFromID(ID_LOBBY);
	pProject->SetCallCreateObject(LSY::Fnt(this, &CLobbyView::OnCallCreateObject));
	pProject->LoadXml();

#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
	if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
	{
		LSY::CWindows *pWindows = (LSY::CWindows *)pProject->GetMainObject();
		if(pWindows) pWindows->SetIndex(10);
	}
#endif //UPGRADE_20120308_TOCTOC_CHANNELING

	pObjectMan->SetProject(0, ID_LOBBY);

	pObjectMan->AddHotKeyMsgH(false, false, VK_SP_ENTER, Fnt(this, &CLobbyView::OnReturn));

	m_pPCRoomEventBack	= (LSY::CImage *)pProject->GetObject(ID_LOBBY_IMG_PCROOM_EVENT_BACK);
	m_pPCRoomWeb		= (LSY::CWeb *)pProject->GetObject(ID_LOBBY_WEB_PCROOM_EVENT);

	if(m_pPCRoomEventBack) m_pPCRoomEventBack->SetShow(false);
	if(m_pBtnComebackPromotion){
		m_pBtnComebackPromotion->SetShow(false);
		m_pBtnComebackPromotion->SetEnable(false);
	}
	if(m_pBtnChickenPromotion){
		m_pBtnChickenPromotion->SetShow(false);
		m_pBtnChickenPromotion->SetEnable(false);
	}
#if defined(_VIPJACKPOT)
	if(m_pJackpotGroup){
		m_pJackpotGroup->SetShow(false);
		m_pJackpotGroup->SetEnable(false);
	}
	if(m_pVipJackpotGroup){
		m_pVipJackpotGroup->SetShow(true);
		m_pVipJackpotGroup->SetEnable(true);
	}
#else
	if(m_pJackpotGroup){
		m_pJackpotGroup->SetShow(true);
		m_pJackpotGroup->SetEnable(true);
	}
	if(m_pVipJackpotGroup){
		m_pVipJackpotGroup->SetShow(false);
		m_pVipJackpotGroup->SetEnable(false);
	}
#endif

	m_pImgFundEventVIP = (LSY::CImage *)pProject->GetObject(ID_LOBBY_IMG_FUNDEVENT_VIP);
	m_pImgFundEventVIP->SetShow(false);
	m_pImgFundEventNormal = (LSY::CImage *)pProject->GetObject(ID_LOBBY_IMG_FUNDEVENT_NORMAL);
	m_pImgFundEventNormal->SetShow(false);
	m_pImgFundTooltipVIP = (LSY::CImage *)pProject->GetObject(ID_LOBBY_IMG_FUNDTOOLTIP_VIP);
	m_pImgFundTooltipVIP->SetShow(false);
	m_pImgFundTooltipNormal = (LSY::CImage *)pProject->GetObject(ID_LOBBY_IMG_FUNDTOOLTIP_NORMAL);
	m_pImgFundTooltipNormal->SetShow(false);
	m_pImgVIPGoldDisable = (LSY::CImage *)pProject->GetObject(ID_LOBBY_IMG_ROOMLIST_HEADER_VIP_GOLD_DISABLE);
	m_pImgVIPGoldDisable->SetShow(false);
	m_pImgNormalGoldDisable = (LSY::CImage *)pProject->GetObject(ID_LOBBY_IMG_ROOMLIST_HEADER_NORMAL_GOLD_DISABLE);
	m_pImgNormalGoldDisable->SetShow(false);

// 	//찬스아바타 정보 DLG
// 	m_pEventInfoWnd = (LSY::CSpriteWindows *)pProject->GetObject(ID_LOBBY_DLG_CHANCE_EVENT_NOTIFY);
// 	m_pEventInfoWnd->SetShow(false);
	return TRUE;
}

void CLobbyView::SetLobyProcess(CLobbyProcess *pLobbyProcess)
{
	m_pLobbyProcess = pLobbyProcess;
	// 포커칩 애니메이션 후 머니 정보 갱신하던 부분 2017년 로비 개편으로 쓰이지 않고 있다. 2019.05 shisky 삭제
	//m_pMoneyGaugeGroup->AddHandler(LSY::EM_O_ANILOGIC, LSY::Fnt(m_pLobbyProcess, &CLobbyProcess::OnLogic));
}

void CLobbyView::StartTimer(void)
{
	//if(m_hEventTimer) return;

	//TIMECAPS caps; memset( &caps, 0, sizeof(caps) );
	//timeGetDevCaps( &caps, sizeof(caps) ); 
	//m_hEventTimer = timeSetEvent( 10, max(1, caps.wPeriodMin), LobbyUpdateProc, 0, TIME_PERIODIC|TIME_CALLBACK_FUNCTION );
}

void CLobbyView::StopTimer(void)
{
	//if(m_hEventTimer == 0) return;
	//timeKillEvent(m_hEventTimer);
	//m_hEventTimer = 0;
}

void CLobbyView::OnPaint() 
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트	
	GetObjectMan()->BitBlt(&dc);
}

void CLobbyView::MessageLooping()
{
	// 누적된 메시지를 모두 처리함(윈도우 핸들을 NULL로 주어 쓰레드의 모든 메시지를 처리하게 함)
	static MSG wndq_msg;
	while( ::PeekMessage( &wndq_msg, NULL, 0U, 0U, PM_REMOVE ) ) 
	{
		::TranslateMessage( &wndq_msg );
		::DispatchMessage( &wndq_msg );
	}
}


void CLobbyView::OnDestroy() 
{
	if(m_pPopupMenu)
	{
		m_pPopupMenu->DestroyWindow();
		delete m_pPopupMenu;
		m_pPopupMenu = NULL;
	}

	AllCloseChattingDlg();
	AllCloseInviteChatting();
	AllCloseMemoNotifyPopup();

	ReleaseObjectMan();

	CWnd::OnDestroy();
}

void CLobbyView::AllCloseChattingDlg(void)
{
	LISTCHATTINGROOM::iterator start = m_mapChattingDlg.begin();
	LISTCHATTINGROOM::iterator end = m_mapChattingDlg.end();

	CChattingRoomDlg *pChattingRoom = NULL;
	for(;start != end;++start)
	{
		pChattingRoom = start->second;
		pChattingRoom->DestroyWindow();
		delete pChattingRoom;
	}

	m_mapChattingDlg.clear();
}

void CLobbyView::AllDisconnectDisableChattingDlg(void)
{
	LISTCHATTINGROOM::iterator start = m_mapChattingDlg.begin();
	LISTCHATTINGROOM::iterator end = m_mapChattingDlg.end();

	CChattingRoomDlg *pChattingRoom = NULL;
	for(;start != end;++start)
	{
		pChattingRoom = start->second;
		pChattingRoom->DisconnectDisable();
	}
}

void CLobbyView::AllChangeCaptionChattingDlg(void)
{
	LISTCHATTINGROOM::iterator start = m_mapChattingDlg.begin();
	LISTCHATTINGROOM::iterator end = m_mapChattingDlg.end();

	CChattingRoomDlg *pChattingRoom = NULL;
	for(;start != end;++start)
	{
		pChattingRoom = start->second;
		pChattingRoom->CheckChangeCaption();
	}
}

void CLobbyView::CloseChattingDlg(CChattingRoomDlg *pDlg)
{
	LISTCHATTINGROOM::iterator start = m_mapChattingDlg.begin();
	LISTCHATTINGROOM::iterator end = m_mapChattingDlg.end();

	CChattingRoomDlg *pChattingRoom = NULL;
	for(;start != end;++start)
	{
		pChattingRoom = start->second;
		if(pChattingRoom == pDlg)
		{
			pChattingRoom->DestroyWindow();
			delete pChattingRoom;
			m_mapChattingDlg.erase(start);
			break;
		}
	}
}

bool CLobbyView::IsShowChattingDlg(CChattingRoomDlg *pDlg)
{
	LISTCHATTINGROOM::iterator start = m_mapChattingDlg.begin();
	LISTCHATTINGROOM::iterator end = m_mapChattingDlg.end();

	CChattingRoomDlg *pChattingRoom = NULL;
	for(;start != end;++start)
	{
		pChattingRoom = start->second;
		if(pChattingRoom == pDlg)
		{
			return true;
		}
	}
	return false;
}

bool CLobbyView::IsFullCreateRoom(void)
{
	int nMaxRoom = CCommunityManager::Instance()->GetMaxCreateChattingRoom();
	if(nMaxRoom <= (int)m_mapChattingDlg.size())
		return true;
	return false;
}

CChattingRoomDlg *CLobbyView::GetChattingRoom(std::string strRoomID)
{
	std::map<std::string, CChattingRoomDlg *>::iterator findIt = m_mapChattingDlg.find(strRoomID);
	if(findIt == m_mapChattingDlg.end()) return NULL;
	return findIt->second;
}

bool CLobbyView::CreateCattingRoom(const std::string &strRoomID, const std::list<std::string> &listInviteID, const bool &bMaster)
{
	if(IsFullCreateRoom()) return false;

	CChattingRoomDlg *pChattingRoom = new CChattingRoomDlg;
	pChattingRoom->Create(GM().GetMainWnd(), strRoomID, listInviteID, bMaster);
	pChattingRoom->SetIcon(GM().GetCurrentGame()->GetChatIcon(), FALSE);

	m_mapChattingDlg[strRoomID] = pChattingRoom;
	return true;
}

void CLobbyView::AllCloseInviteChatting(void)
{
	LISTINVITECHATTING::iterator start = m_listInviteChatting.begin();
	LISTINVITECHATTING::iterator end = m_listInviteChatting.end();

	CChattingRoomInviteDlg *pDlg = NULL;
	for(;start != end;++start)
	{
		pDlg = *start;
		pDlg->DestroyWindow();
		delete pDlg;
	}

	m_listInviteChatting.clear();
}

void CLobbyView::CloseInviteChatting(CChattingRoomInviteDlg *pDlg, UINT nResult)
{
	LISTINVITECHATTING::iterator findIt = find(m_listInviteChatting.begin(), m_listInviteChatting.end(), pDlg);
	if(findIt != m_listInviteChatting.end())
	{
		bool bFullRoom = false;
		if(nResult == IDOK && CCommunityManager::Instance()->IsConnect())
		{
			if(IsFullCreateRoom())
			{
				bFullRoom = true;
			}
			else
			{
				CChattingRoomDlg *pChattingRoom = GetChattingRoom(pDlg->GetRoomID());
				std::list<std::string> listInviteID;
				listInviteID.push_back(pDlg->GetSender());
				if(pChattingRoom == NULL) CreateCattingRoom(pDlg->GetRoomID(), listInviteID, false);
			}
		}

		pDlg->DestroyWindow();
		delete pDlg;

		m_listInviteChatting.erase(findIt);

		if(bFullRoom) ShowMainMessageDlg("대화창은 10개 까지만 생성 가능합니다.");
	}
}

bool CLobbyView::CreateInviteCatting(std::string strRoomID, std::string strSenderID)
{
	CChattingRoomInviteDlg *pInviteChatting = new CChattingRoomInviteDlg;
	pInviteChatting->Create(GM().GetMainWnd(), strRoomID, strSenderID);

	m_listInviteChatting.push_back(pInviteChatting);
	return true;	
}

bool CLobbyView::IsExistInviteChatting(const std::string &strRoomID, const std::string &strSenderID)
{
	LISTINVITECHATTING::iterator start = m_listInviteChatting.begin();
	LISTINVITECHATTING::iterator end = m_listInviteChatting.end();

	CChattingRoomInviteDlg *pDlg = NULL;
	for(;start != end;++start)
	{
		pDlg = *start;
		if(pDlg->GetRoomID() == strRoomID && pDlg->GetSender() == strSenderID) return true;
	}

	return false;
}

void CLobbyView::AllCloseMemoNotifyPopup(void)
{
	LISTMEMONOTIFYPOPUP::iterator start = m_listMemoNotifyPopup.begin();
	LISTMEMONOTIFYPOPUP::iterator end = m_listMemoNotifyPopup.end();

	CMessageNotificationPopup *pWnd = NULL;
	for(;start != end;++start)
	{
		pWnd = *start;
		pWnd->DestroyWindow();
		delete pWnd;
	}

	m_listMemoNotifyPopup.clear();
}

void CLobbyView::CloseMemoNotifyPopup(CMessageNotificationPopup *pWnd)
{
	LISTMEMONOTIFYPOPUP::iterator findIt = find(m_listMemoNotifyPopup.begin(), m_listMemoNotifyPopup.end(), pWnd);
	if(findIt != m_listMemoNotifyPopup.end())
	{
		pWnd->DestroyWindow();
		delete pWnd;
		m_listMemoNotifyPopup.erase(findIt);
	}
}

bool CLobbyView::CreateMemoNotifyPopup(MEMO *pMemo)
{
	LSY::CObjectProject *pProject = GetObjectMan()->GetProjectFromID(CMessageNotificationPopup::GetProjectID());

	CSize szSize = pProject->GetSize();
	CPoint ptStart(798, 659);
	CPoint ptPos = ptStart;

	CMessageNotificationPopup *pWnd = new CMessageNotificationPopup;
	while(1)
	{
		if(!IsMemoNotifyUsePos(ptPos))
		{
			pWnd->Create(this, ptPos.x, ptPos.y);
			break;
		}

		ptPos.y -= szSize.cy;
		if(ptPos.y < 0)
		{
			ptPos.x -= szSize.cx;
			ptPos.y = ptStart.y;
		}

		if(ptPos.x < 0)
		{
			CPoint ptTemp = GetFirstHideMemoNotifyPopupPos(szSize, ptStart);
			pWnd->Create(this, ptTemp.x, ptTemp.y);
			break;
		}
	}
	
	pWnd->ShowNotification(pMemo, 1000, 2000);
	m_listMemoNotifyPopup.push_back(pWnd);
	return true;
}

bool CLobbyView::CreateMemoNotifyPopup(std::string strConnectBuddyNickName)
{
	if(!g_Config.bNotificationConnectBuddy) return true;
	LSY::CObjectProject *pProject = GetObjectMan()->GetProjectFromID(CMessageNotificationPopup::GetProjectID());

	CSize szSize = pProject->GetSize();
	CPoint ptStart(798, 659);
	CPoint ptPos = ptStart;

	CMessageNotificationPopup *pWnd = new CMessageNotificationPopup;
	while(1)
	{
		if(!IsMemoNotifyUsePos(ptPos))
		{
			pWnd->Create(this, ptPos.x, ptPos.y);
			break;
		}

		ptPos.y -= szSize.cy;
		if(ptPos.y < 0)
		{
			ptPos.x -= szSize.cx;
			ptPos.y = ptStart.y;
		}

		if(ptPos.x < 0)
		{
			CPoint ptTemp = GetFirstHideMemoNotifyPopupPos(szSize, ptStart);
			pWnd->Create(this, ptTemp.x, ptTemp.y);
			break;
		}
	}

	pWnd->ShowNotification(strConnectBuddyNickName, 1000, 2000);
	m_listMemoNotifyPopup.push_back(pWnd);
	return true;
}

bool CLobbyView::IsMemoNotifyUsePos(const CPoint &ptPos)
{
	LISTMEMONOTIFYPOPUP::iterator start = m_listMemoNotifyPopup.begin();
	LISTMEMONOTIFYPOPUP::iterator end = m_listMemoNotifyPopup.end();

	CMessageNotificationPopup *pWnd = NULL;
	for(;start != end;++start)
	{
		pWnd = *start;
		if(pWnd->IsWindowVisible() && (pWnd->GetPos() == ptPos)) return true;
	}

	return false;
}

CPoint CLobbyView::GetFirstHideMemoNotifyPopupPos(const CSize &szSize, const CPoint &ptStart)
{
	CPoint ptPos = ptStart;

	LISTMEMONOTIFYPOPUP::iterator start = m_listMemoNotifyPopup.begin();
	LISTMEMONOTIFYPOPUP::iterator end = m_listMemoNotifyPopup.end();

	CMessageNotificationPopup *pWnd = NULL;
	CMessageNotificationPopup *pRet = NULL;

	for(;start != end;++start)
	{
		pWnd = *start;

		if(pWnd->IsMouseOver())
		{
		}
		else if(pRet == NULL)
		{
			pRet = pWnd;
		}
		else if(pWnd->GetStatus() > pRet->GetStatus())
		{
			pRet = pWnd;
		}
		else if((pWnd->GetStatus() == pRet->GetStatus()) && (pWnd->GetEndTime() < pRet->GetEndTime()))
		{
			pRet = pWnd;
		}

		ptPos.y -= szSize.cy;
		if(ptPos.y < 0)
		{
			ptPos.x -= szSize.cx;
			ptPos.y = ptStart.y;
		}

		if(ptPos.x < 0)
		{
			break;
		}
	}

	CPoint ptRet = ptStart;
	if(pRet)
	{
		m_listMemoNotifyPopup.remove(pRet);
		ptRet = pRet->GetPos();
		pRet->DestroyWindow();
		delete pRet;
	}

	return pRet?pRet->GetPos():ptStart;
}

CMessageNotificationPopup *CLobbyView::GetPosToLastHideMemoNotifyPopup(const CPoint &ptPos)
{
	LISTMEMONOTIFYPOPUP::iterator start = m_listMemoNotifyPopup.begin();
	LISTMEMONOTIFYPOPUP::iterator end = m_listMemoNotifyPopup.end();

	CMessageNotificationPopup *pWnd = NULL;
	CMessageNotificationPopup *pRet = NULL;
	for(;start != end;++start)
	{
		pWnd = *start;
		if(pWnd->GetPos() != ptPos) continue;

		if(pWnd->IsMouseOver())
		{
			return pWnd;
		}
		else if(pRet == NULL)
		{
			pRet = pWnd;
		}
		else if(pWnd->GetStatus() < pRet->GetStatus())
		{
			pRet = pWnd;
		}
		else if((pWnd->GetStatus() == pRet->GetStatus()) && (pWnd->GetEndTime() > pRet->GetEndTime()))
		{
			pRet = pWnd;
		}
	}

	return pRet;
}

LRESULT CLobbyView::OnCallCreateObject(LSY::CMessage *msg)
{
	LSY::CMO_CreateObject *pMsg = (LSY::CMO_CreateObject *)msg;
	switch(*pMsg->GetID())
	{
	case ID_LOBBY_GRP_TITLE_BUTTON:
		m_pTitleButtonGroup = new CTitleButtonGroup;
		return (LRESULT)m_pTitleButtonGroup;
	case ID_LOBBY_TAB_GAME:
		m_pTabGame = new CTabGame;
		return (LRESULT)m_pTabGame;
	case ID_LOBBY_GRP_MYINFO:
		m_pUserInfoGroup = new CUserInfoGroup;
		return (LRESULT)m_pUserInfoGroup;
	case ID_LOBBY_WND_MYINFO_DETAIL:
		m_pUserInfoDetailDlg = new CUserInfoDetailDlg;
		return (LRESULT)m_pUserInfoDetailDlg;
	case ID_LOBBY_GROUP_JACKPOT:
		m_pJackpotGroup = new CJackpotGroup;
		return (LRESULT)m_pJackpotGroup;
	case ID_LOBBY_NUMBER_JACKPOT:
		return (LRESULT)(new CJackpotMoney);
	case ID_LOBBY_DLG_JACKPOT_DISPLAY:
		m_pJackpotCurrentInfoDlg = new CJackpotCurrentInfoDlg;
		return (LRESULT)(m_pJackpotCurrentInfoDlg);
	case ID_LOBBY_HALFJACKPOTIMAGENUM:
		return (LRESULT)(new CHalfJackpotMoney);
	case ID_LOBBY_GROUP_USER:
		m_pUserListGroup = new CUserListGroup;
		return (LRESULT)m_pUserListGroup;
	case ID_LOBBY_LISTITEM_USER:
		m_pUserListItem = new CUserListItem;
		return (LRESULT)m_pUserListItem;
	case ID_LOBBY_GROUP_ROOM:
		m_pRoomListGroup = new CRoomListGroup;
		return (LRESULT)m_pRoomListGroup;
	case ID_LOBBY_LISTITEM_ROOM:
		m_pRoomListItem = new CRoomListItem;
		return (LRESULT)m_pRoomListItem;
	case ID_LOBBY_LISTITEM_ROOM_LOWBD:
		m_pRoomListItemLowBD = new CRoomListItemLowBD;
		return (LRESULT)m_pRoomListItemLowBD;
	case ID_LOBBY_LISTITEM_ROOM_HOOLA:
		m_pRoomListItemHoola = new CRoomListItemHoola;
		return (LRESULT)m_pRoomListItemHoola;
	case ID_LOBBY_LISTITEM_ROOM_JOIN:
		m_pRoomListItemJoin = new CRoomListItemJoin;
		return (LRESULT)m_pRoomListItemJoin;
	case ID_LOBBY_LISTITEM_ROOM_LOWBD_JOIN:
		m_pRoomListItemLowBDJoin = new CRoomListItemLowBDJoin;
		return (LRESULT)m_pRoomListItemLowBDJoin;
	case ID_LOBBY_LISTITEM_ROOM_HOOLA_JOIN:
		m_pRoomListItemHoolaJoin = new CRoomListItemHoolaJoin;
		return (LRESULT)m_pRoomListItemHoolaJoin;
	case ID_LOBBY_GRP_ROOMLIST_HEADER:
		return (LRESULT)(new CRoomListHeader);
	case ID_LOBBY_GRP_ROOMLIST_HEADER_LOWBD:
		return (LRESULT)(new CRoomListHeaderLowBD);
	case ID_LOBBY_GRP_ROOMLIST_HEADER_HOOLA:
		return (LRESULT)(new CRoomListHeaderHoola);
	case ID_LOBBY_GRP_ROOMLIST_HEADER_JOIN:
		return (LRESULT)(new CRoomListHeaderJoin);
	case ID_LOBBY_GRP_ROOMLIST_HEADER_LOWBD_JOIN:
		return (LRESULT)(new CRoomListHeaderLowBDJoin);
	case ID_LOBBY_GRP_ROOMLIST_HEADER_HOOLA_JOIN:
		return (LRESULT)(new CRoomListHeaderHoolaJoin);
		/*
	case ID_LOBBY_RAD_VIP_GOLD_ROOM_LIST:
	case ID_LOBBY_RAD_VIP_GOLD_ROOM_LIST_JOIN:
	case ID_LOBBY_RAD_VIP_GOLD_ROOM_LIST_LOWBD:
	case ID_LOBBY_RAD_VIP_GOLD_ROOM_LIST_LOWBD_JOIN:
		return (LRESULT)(new CRadGoldRoomListVIP);
	case ID_LOBBY_RAD_NORMAL_GOLD_ROOM_LIST:
	case ID_LOBBY_RAD_NORMAL_GOLD_ROOM_LIST_JOIN:
	case ID_LOBBY_RAD_NORMAL_GOLD_ROOM_LIST_LOWBD:
	case ID_LOBBY_RAD_NORMAL_GOLD_ROOM_LIST_LOWBD_JOIN:
		return (LRESULT)(new CRadGoldRoomListNormal);
		*/
	case ID_LOBBY_DLG_GAMEROOM_INFO:
		m_pRoomInfoDlg = new CRoomInfoDialog;
		return (LRESULT)m_pRoomInfoDlg;
	case ID_LOBBY_LISTITEM_ROOMINFO_PLAYUSER:
		return (LRESULT)(new CRoomInfoUserListItem);
	case ID_LOBBY_GRP_CHAT:
		m_pChatGroup = new CChatGroup;
		return (LRESULT)m_pChatGroup;
	case ID_LOBBY_TAB_EVENT:
		m_pShopTab = new CShopTabControl;
		return (LRESULT)m_pShopTab;
	case ID_LOBBY_WND_MYFRIEND:
		m_pBuddyListGroup = new CBuddyListGroup;
		return (LRESULT)m_pBuddyListGroup;
	case ID_LOBBY_LISTITEM_MY_FRIEND:
		m_pBuddyListItem = new CBuddyListItem;
		return (LRESULT)m_pBuddyListItem;
	case ID_LOBBY_WND_MY_CAFE:
		m_pCafeListGroup = new CCafeListGroup;
		return (LRESULT)m_pCafeListGroup;
	case ID_LOBBY_LISTITEM_MY_CAFE:
		m_pCafeListItem = new CCafeListItem;
		return (LRESULT)m_pCafeListItem;
	case ID_LOBBY_WND_MY_BLACKLIST:
		m_pBlackBuddyListGroup = new CBlackBuddyListGroup;
		return (LRESULT)m_pBlackBuddyListGroup;
	case ID_LOBBY_LISTITEM_MY_BLACKLIST:
		m_pBlackBuddyListItem = new CBlackBuddyListItem;
		return (LRESULT)m_pBlackBuddyListItem;
	case ID_LOBBY_WND_MY_CLUB:
		m_pReverseBuddyListGroup = new CReverseBuddyListGroup;
		return (LRESULT)m_pReverseBuddyListGroup;
	case ID_LOBBY_LISTITEM_MY_CLUB:
		m_pReverseBuddyListItem = new CReverseBuddyListItem;
		return (LRESULT)m_pReverseBuddyListItem;
	case ID_LOBBY_COMBO_WHISPERLIST_CLOSE:
		return (LRESULT)(new CWhisperlistCombo(true, m_pChatGroup));
	case ID_LOBBY_COMBO_WHISPERLIST_OPEN:
		return (LRESULT)(new CWhisperlistCombo(false, m_pChatGroup));
	case ID_LOBBY_BUTTON_MADE:
	case ID_LOBBY_BUTTON_REGEN:
		return (LRESULT)(new CChanceButton());
	case ID_LOBBY_GRP_CHANCE_TOOLTIPS:
		return (LRESULT)(new CChanceTooltips());
	case ID_LOBBY_BTN_COMEBACK_PROMOTION:
		{
			m_pBtnComebackPromotion = new LSY::CButton();
			m_pBtnComebackPromotion->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CLobbyView::OnClickButton));
		}
		return (LRESULT)m_pBtnComebackPromotion;
	case ID_LOBBY_BTN_CHICKEN:
		{
			m_pBtnChickenPromotion = new LSY::CButton();
			m_pBtnChickenPromotion->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CLobbyView::OnClickButton));
		}
		return (LRESULT)m_pBtnChickenPromotion;
	case ID_LOBBY_GRP_VIPJACKPOT:
		m_pVipJackpotGroup = new CVipJackpotGroup;
		return (LRESULT)m_pVipJackpotGroup;
	case ID_LOBBY_NUM_VIPJACKPOTMONEY:
		return (LRESULT)(new CVipJackpotMoney);
	case ID_LOBBY_DLG_VIPJACKPOT:
		m_pVipJackpotDlg = new CVipJackpotDlg();
		return (LRESULT)m_pVipJackpotDlg;
	case ID_LOBBY_LISTITEM_VIP_MONTH:
		return (LRESULT)(new CVipJackpotListItem());
	case ID_LOBBY_LISTITEM_VIP_TODAY:
		return (LRESULT)(new CVipJackpotListItem());
	case ID_LOBBY_GRP_TARGETNOTI:
		m_pTargetNotiGroup = new CTargetNotiGroup();
		return (LRESULT)m_pTargetNotiGroup;
	case ID_LOBBY_GRP_CONGNOTI:
		m_pCongNotiGroup = new CCongNotiGroup();
		return (LRESULT)m_pCongNotiGroup;	
	}

	return NULL;
}

void CLobbyView::OnChangeDisplayMode(BOOL bFullScr)
{
	if(m_pTitleButtonGroup)
		m_pTitleButtonGroup->SetChangeDisplayMode(bFullScr?true:false);

	//풀스크린일때는 마우스 드래그로 로비창이 움직이지 않게 한다.
	LSY::CObjectMan *pObjectMan = GetObjectMan();
	if (pObjectMan)
	{
		if (bFullScr)
			pObjectMan->SetCaptionHeight(0);
		else
			pObjectMan->SetCaptionHeight(HEIGHT_CAPTION_LOBBY);
	}
}

void CLobbyView::SetMyUserInfo(USERINFO *pMyInfo, BOOL bLogin)
{
	//bLogin:TRUE  -> 로그인 성공시처음 한 번만 호출됨)
	//bLogin:FALSE -> 게임이동시 호출 (USERINFO의 포인터 변경시)

	//1. 유저 정보 세팅
	m_pUserInfoGroup->SetMyUserInfo(pMyInfo, bLogin);
	
	//3. 접속중인 유저리스트  게임 콤보박스 다시 세팅
	m_pUserListGroup->ResetGameComboBox((IDX_GAME)pMyInfo->nGameIndex, pMyInfo->GetRealPMoney()); // 토너먼트 추가로 변경된 함수

	//4. PC방 버튼 세팅
	m_pTitleButtonGroup->SetPCRoomBtn(pMyInfo->nPCRoomIPType);	//유저정보세팅

	//5. VIP상담 버튼 세팅
	bool bTitleButtonShow = IsAbleToVIPCounsel(UPGCheckPremiumState(pMyInfo))?true:false;
#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
	if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
		bTitleButtonShow = false;
	m_pTitleButtonGroup->SetVIPCounselShow(bTitleButtonShow);
#else //UPGRADE_20120308_TOCTOC_CHANNELING
	m_pTitleButtonGroup->SetVIPCounselShow(IsAbleToVIPCounsel(UPGCheckPremiumState(pMyInfo))?true:false);
#endif //UPGRADE_20120308_TOCTOC_CHANNELING
}

BOOL CLobbyView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	return TRUE;
}

void CLobbyView::AddChatText(const char *pMsg, COLORREF color)
{
	m_pChatGroup->AddText(pMsg, color);
}

void CLobbyView::ShowGameRoomInfo(GAMEINFO *pGI)
{
	m_pRoomInfoDlg->ShowGameRoomInfo(pGI);
}

//bool CLobbyView::GetEnableJoinable()
//{
//	CNMCheckButton *pCheckBtn = dynamic_cast<CNMCheckButton*>(FindWidget(LRI_BUTTON_ID_JOINABLE));
//	ASSERT(pCheckBtn);
//
//	return pCheckBtn->GetButtonState();
//}
//
//void CLobbyView::SetRoomListJoinable( bool bset )
//{
//	CLobbyGameTab *pGameTab=dynamic_cast<CLobbyGameTab*>(FindWidget(LOBBY_WIDGET_ID_GAMETAB));
//	ASSERT(pGameTab);
//
//	if (pGameTab) pGameTab->SetRoomListJoinable( bset );
//}

void CLobbyView::CloseGameRoomInfo()
{
	m_pRoomInfoDlg->SetShow(false);
}

MONEYRANGE *CLobbyView::GetCurrentUserListMoneyRange()
{
	// 유저리스트의 콤보박스에 선택된 머니범위 정보를 가져온다.
	return m_pUserListGroup->GetCurrentUserListMoneyRange();
}

IDX_GAME CLobbyView::GetCurrentUserListGame()
{
	return m_pUserListGroup->GetCurrentUserListGame();
}

void CLobbyView::OnEnterLobby(bool bRefresh)
{
	GetObjectMan()->SetProject(0, ID_LOBBY);
	RefreshLobbyWeb();

	//게임중 받은 쪽지들을 지운드 알려준다.
// 	LISTMEMO listMemo;
// 	CCommunityManager::Instance()->GetReceiveMemoList(listMemo);
// 	Camel::Dar2::SS::Memo *pMemo = NULL;
// 
// 	if(listMemo.size() != 0)
// 	{
// 		NMBASE::SOUND::g_Sound.PlayWav(SND_MEMORECEIVE);
// 	}
// 
// 	LISTMEMO::iterator start = listMemo.begin();
// 	LISTMEMO::iterator end = listMemo.end();
// 
// 	for(;start != end;++start)
// 	{
// 		pMemo = *start;
// 		delete pMemo;
// 	}

	LISTSTRING listConnectBuddyNickName;
	CCommunityManager::Instance()->GetConnectBuddyList(listConnectBuddyNickName);
	
// 	int nSize = (int)listMemo.size();
// 	if(nSize > 0)
// 	{
// 		bool bShow = true;// 
// 		if(bShow)
// 		{
// 			CString strTemp;
// 			strTemp.Format("게임방에 있을 때 %ld개의 쪽지를 수신하였습니다. 쪽지함에서 확인하시기 바랍니다.", nSize);
// 			ShowMainMessageDlg(strTemp);
// 		}
// 	}
	MM().Call(UPG_MEMO_CHECK_START);
	m_pUserInfoGroup->m_bRefresh = bRefresh;
}

void CLobbyView::RefreshLobbyWeb()										//웹영역 리프레쉬
{
	m_pShopTab->RefreshWeb();

	if(m_pPCRoomWeb)
		m_pPCRoomWeb->SetURL(getGameURL(IDX_GAMEURL_EVENT_PCPROMOTION_LOBBY_TOP).GetString());
}

void CLobbyView::OnLeaveLobby()
{
	GetObjectMan()->SetProject(0, _ID_PROJECT_NOTUSE);
}

void CLobbyView::SetGameTab(IDX_GAME idxGame)
{
	m_pTabGame->SetGameTab(idxGame);
	m_pRoomListGroup->SetGameType(idxGame);

	if (m_pUserInfoGroup != NULL)
		m_pUserInfoGroup->SetEnableBtnDetailOpen(true);

#if defined(_VIPJACKPOT)
	if(IDX_GAME_HA==idxGame){
		if(m_pJackpotGroup){
			m_pJackpotGroup->SetShow(true);
			m_pJackpotGroup->SetEnable(true);
		}
		if(m_pVipJackpotGroup){
			m_pVipJackpotGroup->SetShow(false);
			m_pVipJackpotGroup->SetEnable(false);
		}
	}
	else{
		if(m_pJackpotGroup){
			m_pJackpotGroup->SetShow(false);
			m_pJackpotGroup->SetEnable(false);
		}
		if(m_pVipJackpotGroup){
			m_pVipJackpotGroup->SetShow(true);
			m_pVipJackpotGroup->SetEnable(true);
		}
	}
#endif
	SetGoldDisable();
}

void CLobbyView::SetGoldDisable()
{
	if (GM().GetCurrentGameType() == IDX_GAME_SP || GM().GetCurrentGameType() == IDX_GAME_BD
		|| GM().GetCurrentGameType() == IDX_GAME_NP || GM().GetCurrentGameType() == IDX_GAME_SD)
	{
		m_pImgVIPGoldDisable->SetShow(false);
		m_pImgNormalGoldDisable->SetShow(false);
	}
	else
	{
		m_pImgVIPGoldDisable->SetShow(true);
		m_pImgNormalGoldDisable->SetShow(true);
	}
	MM().Call(UPG_CMK_SET_FUNDEVENT_ENABLE);
}

void CLobbyView::SetFundEventEnable()
{
	if (GM().GetCurrentGameType() == IDX_GAME_SP || GM().GetCurrentGameType() == IDX_GAME_BD
		|| GM().GetCurrentGameType() == IDX_GAME_NP || GM().GetCurrentGameType() == IDX_GAME_SD)
	{
		m_pImgFundEventVIP->SetShow(GM().GetPlaying());
		m_pImgFundEventNormal->SetShow(GM().GetPlaying());
	}
	else
	{
		m_pImgFundEventVIP->SetShow(false);
		m_pImgFundEventNormal->SetShow(false);
	}
}

void CLobbyView::ChangeRoomTab(ROOMLIST_TYPE eRoomListType)
{
	m_pRoomListGroup->ChangeRoomTab(eRoomListType);
}

void CLobbyView::ShowRoomList(bool bShow)
{

	LSY::CObjectProject *pProject = GetObjectMan()->GetProjectFromID(ID_LOBBY);
	
	m_pRoomListGroup->SetShow(bShow);	
	m_pUserListGroup->SetShow(bShow);
	m_pUserInfoGroup->ShowRoomList(bShow);
	/*m_pUserInfoDetailDlg->ShowRoomList(bShow);*/
	m_pChatGroup->ShowRoomList(bShow);
	/*m_pShopTab->SetShow(bShow);*/
	m_pShopTab->SetShow(true);

	int nBackIndex = 0;
//
//#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
//	if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
//		nBackIndex = 11;
//#endif //UPGRADE_20120308_TOCTOC_CHANNELING

#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
	if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
		nBackIndex = 10;
#endif //UPGRADE_20120308_TOCTOC_CHANNELING

	if(bShow)
	{
	}
	else
	{
		m_pRoomListGroup->RemoveAll();
		m_pUserListGroup->RemoveAll();
		
#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
		if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
			nBackIndex = 10;
		else
			nBackIndex = 0;
#else //UPGRADE_20120308_TOCTOC_CHANNELING
		nBackIndex = 1;
#endif //UPGRADE_20120308_TOCTOC_CHANNELING
 	}
}

BOOL CLobbyView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return __super::OnCommand(wParam, lParam);
}

void CLobbyView::SetChanceInfo(CRewardItemMan::REWARD_TYPE eRewardType, int nRewardItemCode, COleDateTime &timeExpire)
{
	if (nRewardItemCode==0)
	{
		m_pTabGame->ClearChance(eRewardType);
		return;
	}

	m_pTabGame->SetChanceInfo(eRewardType, nRewardItemCode, timeExpire);
}

void CLobbyView::RoomListSortDefault(void)
{
	m_pRoomListGroup->SortDefault();
}

void CLobbyView::ShowLoadingRoom(BOOL bLoading)
{
	// 룸리스트가 로딩중이라는 애니메이션을 ON/OFF한다.
	if(bLoading)
	{
		m_pRoomListGroup->RemoveAll();
	}

	m_pRoomListGroup->ShowLoadingRoom(bLoading?true:false);
}

void CLobbyView::ShowLoadingUser(BOOL bLoading)
{
	// 유저리스트가 로딩중이라는 애니메이션을 ON/OFF한다.
	if(bLoading) m_pUserListGroup->RemoveAll();
	m_pUserListGroup->ShowLoadingUser(bLoading?true:false);
}

void CLobbyView::OnEnable(BOOL bEnable)
{
	__super::OnEnable(bEnable);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (bEnable)
		SetFocus();
}

void CLobbyView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	__super::OnShowWindow(bShow, nStatus);

	if (bShow)
		SetFocus();
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


SMALLUSERINFO *CLobbyView::GetSelectedUserInfo()
{
	return m_pUserListGroup->GetSelectedUserInfo();
}

void CLobbyView::ShowQJButton(IDX_GAME idxGame, BOOL bShow)
{
	//CLobbyGameTab *pLobbyGameTab = dynamic_cast<CLobbyGameTab*>(FindWidget(LOBBY_WIDGET_ID_GAMETAB));
	//ASSERT(pLobbyGameTab);

	//if(pLobbyGameTab)
	//	pLobbyGameTab->ShowQJButton(idxGame, bShow);

}

void CLobbyView::EnableChat(BOOL bEnable)
{
	m_pChatGroup->EnableChat(bEnable?true:false);
}

void CLobbyView::SetFocusChat()
{
	m_pChatGroup->SetFocusChat();
}

void CLobbyView::SetFocusSearchUser()										
{
	//유저검색창에 포커스를 준다.
	m_pUserListGroup->SetFocusFindUser();
}

void CLobbyView::SetFocusSearchRoom()										
{
	//방검색창에 포커스를 준다.
	m_pUserListGroup->SetFocusFindRoomNumber();
}


void CLobbyView::RaiseChatView()
{
	m_pChatGroup->RaiseChatView(true);
}

void CLobbyView::ClearChatHistory()
{
	m_pChatGroup->ClearChatHistory();
}

BOOL CLobbyView::ShowJackpotDisplay(BOOL bShow)
{
	if (bShow)		
	{
		m_pJackpotCurrentInfoDlg->SetShowDlg();
	}
	else
	{
		m_pJackpotCurrentInfoDlg->SetShow(false);
	}

	return TRUE;
}

void CLobbyView::SetTargetNotiPos(int x, int y)
{
	m_pTargetNotiGroup->SetPos((float)x, (float)y);
}

//규제안 : 이용제한 안내 버튼
void CLobbyView::SetShowUseLimitBtn(bool bShow)
{
	m_pTabGame->SetShowUseLimitBtn(bShow);
}

//규제안 : 이용제한 안내 버튼(메시지 활성)
LRESULT CLobbyView::OnShowRestrictionButton(WPARAM &wParam, LPARAM &lParam)
{
	m_pTabGame->SetShowUseLimitBtn(true);
	return 1;
}

//규제안 : 이용제한 안내 버튼(메시지 비활성)
LRESULT CLobbyView::OnHideRestrictionButton(WPARAM &wParam, LPARAM &lParam)
{
	m_pTabGame->SetShowUseLimitBtn(false);
	return 1;
}


//규제안 : 이용제한 안내 버튼(메시지 비활성)
LRESULT CLobbyView::OnChangeRestrictionButton(WPARAM &wParam, LPARAM &lParam)
{
	m_pTabGame->ChangeLimitGameTab();
	return 1;
}


LRESULT CLobbyView::OnShowRestriction(WPARAM &wParam, LPARAM &lParam)
{
	if(true==m_pTabGame->ChangeLimitGameTab()){
		ShowUseLimitDlg();
	}
	return 1;
}


LRESULT CLobbyView::OnShowRestrictionOver(WPARAM &wParam, LPARAM &lParam)
{
	m_pTabGame->ShowUseLimitOverDlg();
	return 1;
}

LRESULT CLobbyView::OnShowRestrictionPlayOver(WPARAM &wParam, LPARAM &lParam)
{
	m_pTabGame->ChangeLimitGameBtns((BOOL)wParam);
	return 1;
}

LRESULT CLobbyView::OnShowPromotionComeBack(WPARAM &wParam, LPARAM &lParam)
{
	if(NULL!=m_pBtnComebackPromotion){
		m_pBtnComebackPromotion->SetShow(true);
		m_pBtnComebackPromotion->SetEnable(true);
	}
	return 1;
}


LRESULT CLobbyView::OnShowPromotionComeBackDlg(WPARAM &wParam, LPARAM &lParam)
{
	CPromotionComeBackDlg dlg(GM().GetMainWnd());
	m_pComeBackDlg=&dlg;
	dlg.DoModal();
	m_pComeBackDlg=NULL;
	return 1;
}

LRESULT CLobbyView::OnShowPromotionComeBackDlgUpdate(WPARAM &wParam, LPARAM &lParam)
{
	if(NULL!=m_pComeBackDlg){
		m_pComeBackDlg->Invalidate(TRUE);
	}
	return 1;
}

LRESULT CLobbyView::OnClickButton(LSY::CMessage *msg)
{
	LSY::CMO_MouseLClick *pMsg = (LSY::CMO_MouseLClick *)msg;
	LSY::CButton *pBtn = (LSY::CButton *)pMsg->GetObject();
	switch(pBtn->GetID())
	{
	case ID_LOBBY_BTN_COMEBACK_PROMOTION:
		{
			MM().Call(UPG_CMK_SHOW_PROMOTION_COMEBACK_DLG);
		}
		break;
	case ID_LOBBY_BTN_CHICKEN:
		{
			CLobbyEventDlg dlg(GM().GetMainWnd(), EVENT_CHICKEN);
			dlg.DoModal(getGameURL(IDX_GAMEURL_CHICKEN_URL));
		}
		break;	
	}
	return TRUE;

}

//규제안 : 안내 팝업
void CLobbyView::ShowUseLimitDlg()
{
	m_pTabGame->ShowUseLimitDlg();
}


void CLobbyView::SetJackpotDisplyInfo(jackpot::JP_MONEYINFO *pJackpotMoneyInfo)		//잭팟 현황판 정보 세팅
{
	if(m_pJackpotCurrentInfoDlg) m_pJackpotCurrentInfoDlg->SetJackpotDisplyInfo(pJackpotMoneyInfo);
}

//잭팟대방출
void CLobbyView::SetJackpotRelease()
{
	if (m_pJackpotGroup) m_pJackpotGroup->SetJackpotRelease();	
}

// 하프잭팟 관련 함수
void CLobbyView::SetHalfJackpotInit()
{
	m_pJackpotGroup->SetHalfJackpotInit();
}

void CLobbyView::SetHalfJackpotReady(UINT32 nTime)
{
	m_pJackpotGroup->SetHalfJackpotReady(nTime);
}

void CLobbyView::SetHalfJackpotTimeStart(UINT32 nTime)
{
	m_pJackpotGroup->SetHalfJackpotTimeStart(nTime);
}

void CLobbyView::SetHalfJackpotTimeBeforeEnd()
{
	m_pJackpotGroup->SetHalfJackpotTimeBeforeEnd();
}

void CLobbyView::SetHalfJackpotTimeEnd()
{
	m_pJackpotGroup->SetHalfJackpotTimeEnd();
}

void CLobbyView::SetHalfJackpotAlarm(promotion::_HALFJACKPOT_ALARM* pAlarm )
{
	m_pJackpotGroup->SetHalfJackpotAlarm(pAlarm) ;
}

void CLobbyView::SetHalfJackpotMoney(promotion::HALFJACKPOT_MONEY* pMoney)
{
	m_pJackpotGroup->SetHalfJackpotMoney(pMoney) ;
}

void CLobbyView::SetHalfJackpotRoom(promotion::HALFJACKPOT_ROOM* pRoom)
{
	m_pJackpotGroup->SetHalfJackpotRoom(pRoom);
}

void CLobbyView::SetHalfJackpotRoomEnd(promotion::HALFJACKPOT_PRIZE* pPrize)
{
	m_pJackpotGroup->SetHalfJackpotRoomEnd(pPrize);
}

void CLobbyView::PopupShopDialog(int nTab, LPCTSTR url)
{
	SendMessage(ID_LOBBY_PROECSS_AVATAR_SHOP, (WPARAM)nTab, (LPARAM)url);
}

void CLobbyView::PopupEventDialog(int nTab)
{
	SendMessage(ID_LOBBY_PROCESS_EVENTDLG_TABNO, (WPARAM)nTab);
}

void CLobbyView::SetMaintenanceGame(IDX_GAME idxGame, BOOL bMaintenance, BOOL bAdmin)
{
	m_pTabGame->SetMaintenance(idxGame, bMaintenance?true:false, bAdmin?true:false);
}


LRESULT CLobbyView::OnActivePCEvent(WPARAM &wParam, LPARAM &lParam)
{
	//PC방 상생 프로모션 이 활성화 또는 비활성화 될때 처리
	BOOL bEventActive = (BOOL) wParam;
	if(m_pPCRoomEventBack) m_pPCRoomEventBack->SetShow(bEventActive?true:false);
	if(m_pPCRoomWeb && bEventActive)
		m_pPCRoomWeb->SetURL(getGameURL(IDX_GAMEURL_EVENT_PCPROMOTION_LOBBY_TOP).GetString());
	return 0;
}

LRESULT CLobbyView::OnReturn(LSY::CMessage *msg)
{
	LSY::CMO_HotKey *pMsg = (LSY::CMO_HotKey *)msg;
	LSY::CObjectBase *pFocus = pMsg->GetObject();
	if(pFocus && pFocus->GetType() == LSY::EOT_EDIT)
	{
		return FALSE;
	}

	SetFocusChat();
	return TRUE;
}

void CLobbyView::SetEventBannerURL(const char *szURL)	//대기실 이벤트 배너 영역 URL 세팅
{
	if (szURL==NULL)
		return;

	m_pShopTab->SetEventURL(szURL);
}


LRESULT CLobbyView::OnSetEventBannerURL(WPARAM &wParam, LPARAM &lParam)
{
	SetEventBannerURL((const char *)wParam);
	return 1;
}

void CLobbyView::PopupEventURLDialog ( LPCSTR url)
{
	SendMessage(LGT_BUTTON_ID_EVENT, (WPARAM)url);
}

void CLobbyView::SetSelectID(const std::string &strID)
{
	m_pUserListGroup->SetSelectID(strID);
}

void CLobbyView::SetSelectNickName(const std::string &strNickName)
{
	m_pUserListGroup->SetSelectNickName(strNickName);
}

void CLobbyView::SetSelectRoomNum(int nRoomNum)
{
	m_pUserListGroup->SetSelectRoomNum(nRoomNum);
}

void CLobbyView::OnClickPopupMenu(UINT nID)
{
	if(nID == 0) return;

	SMALLUSERINFO *smallUserInfo = m_pUserListGroup->GetSelectedUserInfo();
	const std::string &strID = m_pUserListGroup->GetSelectID();
	const std::string &strNickName = m_pUserListGroup->GetSelectNickName();
	int nRoomNum = m_pUserListGroup->GetSelectRoomNum(); // 방
	
	switch(nID)
	{
	case ID_LOBBYPOPUPMENU_TXTBTN_PROFILE:
		SendUILog_Basic(CSSTATS_UINO_POKER_LOBBY_USERLIST_PROFILE);
		PostMessage(WM_COMMAND, IDC_BUTTON_VIEWPROFILE);
		break;
	case ID_LOBBYPOPUPMENU_TXTBTN_PROFILE_CLUB:
		{
			std::vector<char> writable(strNickName.begin(), strNickName.end());
			writable.push_back('\0');
			char* ptr = &writable[0];
			std::cout << ptr;

			SendUILog_Basic(CSSTATS_UINO_POKER_LOBBY_USERLIST_PROFILE); // 로그
			CSV_ASK_FINDID msg;
			msg.Set(ptr);
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
		}
		break;
	case ID_LOBBYPOPUPMENU_TXTBTN_SEND_MEMO:
		#ifdef UPGRADE_10210619_ADD_CLICKLOG
			SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_USERLIST_USERTAB_RCLICK_SEND_MEMO);
		#endif //UPGRADE_10210619_ADD_CLICKLOG

// 		if(CCommunityManager::Instance()->IsConnect())
 		{
			ShowSendMessageDialog( strNickName.c_str() );			
		}
// 		else
// 		{
// 			ShowMainMessageDlg("커뮤니티 서버와 연결되어 있지 않아 쪽지를 보낼 수 없습니다.");
// 		}
		break;
		/*
	case ID_LOBBYPOPUPMENU_TXTBTN_SEND_WHISPER:
		SendUILog_Basic(CSSTATS_UINO_POKER_LOBBY_USERLIST_WHISPER);
		PostMessage(WM_COMMAND, IDC_BUTTON_WHISPER);
		break;
		*/
	case ID_LOBBYPOPUPMENU_TXTBTN_INVITE_CLUB:
		{
			// 혹시 나중에 채팅하기 관련 작업이 있을 수 있어 주석을 남겨둠..
			//#ifdef UPGRADE_10210619_ADD_CLICKLOG
			//	SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_USERLIST_USERTAB_RCLICK_TALK);
			//#endif //UPGRADE_10210619_ADD_CLICKLOG

			//std::list<std::string> listInviteID;
			//listInviteID.push_back(strID);
			//SendCreateChatting(&listInviteID, "커뮤니티 서버와 연결되어 있지 않아 대화창을 열수 없습니다.");
			
			// 초대(내가 보스고 상대가 미가입 상태)
			int nMyGrade = GM().GetMyInfo()->UI.ClubMemberGrade;
			int nUserGrade = smallUserInfo->ClubMemberGrade;
			if (nMyGrade == CLUB_MEMBER_GRADE_BOSS &&
				nUserGrade == CLUB_MEMBER_GRADE_NONE)
			{
				CCL_CLUB_INVITE msg;
				
				msg.Set(strID.c_str());
				NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
			}
			// 가입요청(내가 클럽 미가입 상태이고 상대가 보스일때)
			else if (nMyGrade == CLUB_MEMBER_GRADE_NONE &&
				nUserGrade == CLUB_MEMBER_GRADE_BOSS)
			{
				CCL_CLUB_JOIN msg;
				
				msg.Set(smallUserInfo->ClubCode);
				NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
			}
		}
		break;
	case ID_LOBBYPOPUPMENU_TXTBTN_COPY_ID:
		PostMessage(WM_COMMAND, IDC_BUTTON_COPY_ID);
		break;
	case ID_LOBBYPOPUPMENU_TXTBTN_ADD_BLACK_LIST:
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_USERLIST_USERTAB_RCLICK_ADD_BLACKLIST);
			#endif //UPGRADE_10210619_ADD_CLICKLOG

			CBlackBuddyAddDlg dlg;

			if(dlg.DoModal(CBlackBuddyAddDlg::BBADT_QUESTION, "", GM().GetMainWnd()) == IDCANCEL)
			{
				break;
			}

			if(CCommunityManager::Instance()->IsConnect())
			{
				CCommunityManager::Instance()->SendAddBlackBuddy(strID);			
			}
			else
			{
				ShowMainMessageDlg("커뮤니티 서버와 연결되어 있지 않아 블랙리스트 목록에 추가할 수 없습니다.");
			}
		}
		break;
	case ID_LOBBYPOPUPMENU_TXTBTN_ADD_BUDDY:
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_USERLIST_USERTAB_RCLICK_ADD_FRIEND);
			#endif //UPGRADE_10210619_ADD_CLICKLOG
			
			CBuddyAddDlg dlg;
			
			if(dlg.DoModal(strNickName, GM().GetMainWnd()) == IDCANCEL)
			{
				break;
			}	

			if(!CCommunityManager::Instance()->IsConnect())
			{
				ShowMainMessageDlg("커뮤니티 서버와 연결되어 있지 않아 내친구 목록에 추가할 수 없습니다.");
			}
		}
		break;
	case ID_LOBBYPOPUPMENU_TXTBTN_BLACKBUDDY_PROFILE:
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_USERLIST_POKERFRIENDTAB_BLACKLIST_RCLICK_PROFILE);
			#endif //UPGRADE_10210619_ADD_CLICKLOG

			std::list<std::string> listNIckName;
			listNIckName.push_back(strID.c_str());
			CCommunityManager::Instance()->SendGetUserInfo(CCommunityUserInfo::ECUIT_SHOWPROFILE, NULL, listNIckName,"");
		}
		break;
	case ID_LOBBYPOPUPMENU_TXTBTN_BLACKBUDDY_REMOVE:
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_USERLIST_POKERFRIENDTAB_BLACKLIST_RCLICK_DELETE_BLACKLIST);
			#endif //UPGRADE_10210619_ADD_CLICKLOG

			if(CCommunityManager::Instance()->IsConnect())
			{
				CBlackBuddyRemoveDlg dlg;
				if(dlg.DoModal(CBlackBuddyRemoveDlg::BBRDT_QUESTION, "", GM().GetMainWnd()) == IDOK)
				{
					CCommunityManager::Instance()->SendRemoveBlackBuddyList(strID);
				}
			}
			else
			{
				ShowMainMessageDlg("커뮤니티 서버에 연결되어 있지 않아 블랙리스트에서 삭제할 수 없습니다.");
			}
		}
		break;
	case ID_LOBBYPOPUPMENU_TXTBTN_REVERSE_BUDDY_PROFILE:
	case ID_LOBBYPOPUPMENU_TXTBTN_BUDDY_PROFILE:
		{
			if (ID_LOBBYPOPUPMENU_TXTBTN_REVERSE_BUDDY_PROFILE == nID)
			{
				#ifdef UPGRADE_10210619_ADD_CLICKLOG
					SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_USERLIST_POKERFRIENDTAB_FRIEND_ADDED_ME_RCLICK_PROFILE);
				#endif //UPGRADE_10210619_ADD_CLICKLOG
			}
			else if (ID_LOBBYPOPUPMENU_TXTBTN_BUDDY_PROFILE == nID)
			{
				#ifdef UPGRADE_10210619_ADD_CLICKLOG
					SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_USERLIST_POKERFRIENDTAB_MYFRIEND_RCLICK_SHOW_PROFILE);
				#endif //UPGRADE_10210619_ADD_CLICKLOG
			}

			if (CCommunityManager::Instance()->IsConnect())
			{
				std::list<std::string> listNIckName;
				listNIckName.push_back(strID);
				CCommunityManager::Instance()->SendGetUserInfo(CCommunityUserInfo::ECUIT_SHOWPROFILE, NULL, listNIckName, "");
			}
			else
			{
				ShowMainMessageDlg("커뮤니티 서버에 연결되어 있지 않아 프로필을 조회 수 없습니다.");
			}
		}
		break;
		/*귓말 삭제
	case ID_LOBBYPOPUPMENU_TXTBTN_BUDDY_WHISPER:
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
					SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_USERLIST_POKERFRIENDTAB_MYFRIEND_RCLICK_WHISPER);
			#endif //UPGRADE_10210619_ADD_CLICKLOG	

			SMALLUSERINFO tempInfo;
			strcpy(tempInfo.NickName, strNickName.c_str());
			strcpy(tempInfo.ID, strID.c_str());
			tempInfo.llUserKey = -1;
			ShowWhisperDialog(&tempInfo, NULL);
		}
		break;
		*/
	case ID_LOBBYPOPUPMENU_TXTBTN_BUDDY_INVITE_CLUB:
		{
			// 혹시 나중에 채팅하기 관련 작업이 있을 수 있어 주석을 남겨둠..
			//#ifdef UPGRADE_10210619_ADD_CLICKLOG
			//		SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_USERLIST_POKERFRIENDTAB_MYFRIEND_RCLICK_TALK);
			//#endif //UPGRADE_10210619_ADD_CLICKLOG	

			//std::list<std::string> listInviteID;
			//listInviteID.push_back(strID);
			//SendCreateChatting(&listInviteID, "커뮤니티 서버와 연결되어 있지 않아 대화창을 열수 없습니다.");
			
			// 초대(내가 보스고 상대가 미가입 상태)
			int nMyGrade = GM().GetMyInfo()->UI.ClubMemberGrade;
			//int nUserGrade = m_cUserInfo.ClubMemberGrade;
			//if (nMyGrade == CLUB_MEMBER_GRADE_BOSS &&
			//	nUserGrade == CLUB_MEMBER_GRADE_NONE)
			//{
				//CCL_CLUB_INVITE msg;
				
				//msg.Set(m_cUserInfo.ID);
				//NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
			//}
			// 가입요청(내가 클럽 미가입 상태이고 상대가 보스일때)
			//else if (nMyGrade == CLUB_MEMBER_GRADE_NONE &&
			//	nUserGrade == CLUB_MEMBER_GRADE_BOSS)
			//{
				//CCL_CLUB_JOIN msg;
				
				//msg.Set(m_cUserInfo.ClubCode);
				//NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
			//}
		}
		break;
	case ID_LOBBYPOPUPMENU_TXTBTN_BUDDY_MEMO:
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_USERLIST_POKERFRIENDTAB_MYFRIEND_RCLICK_SEND_MEMO);
			#endif //UPGRADE_10210619_ADD_CLICKLOG	

			//if(CCommunityManager::Instance()->IsConnect())
			{
				ShowSendMessageDialog( strNickName.c_str() );
			}
// 			else
// 			{
// 				ShowMainMessageDlg("커뮤니티 서버와 연결되어 있지 않아 쪽지를 보낼 수 없습니다.");
// 			}
		}
		break;
	case ID_LOBBYPOPUPMENU_TXTBTN_BUDDY_DELETE:
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_USERLIST_POKERFRIENDTAB_MYFRIEND_RCLICK_DELETE);
			#endif //UPGRADE_10210619_ADD_CLICKLOG	

			if(!CCommunityManager::Instance()->IsConnect())
			{
				ShowMainMessageDlg("커뮤니티 서버와 연결되어 있지 않아 선택한 친구를 내친구 목록에서 제거할 수 없습니다.");
				break;
			}

			CBuddyRemoveDlg dlg;
			if(dlg.DoModal(CBuddyRemoveDlg::BRDT_QUESTION,"",GM().GetMainWnd()) == IDOK)
			{
				CBuddyFriend *pBuddy = CCommunityManager::Instance()->IsExistBuddy(strNickName);
				if(pBuddy)
				{
					std::map<std::string, std::string> mapIDAndGroup;
					CBuddyGroup *pGroup = pBuddy->GetGroup();
					mapIDAndGroup[pBuddy->GetID()] = pGroup->IsSendChangeName()?pGroup->GetEditName():pGroup->GetName();
					CCommunityManager::Instance()->SendRemoveBuddyFromGroup(mapIDAndGroup);
				}
			}
		}
		break;
	case ID_LOBBYPOPUPMENU_TXTBTN_BUDDY_MOVE:
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_USERLIST_POKERFRIENDTAB_MYFRIEND_RCLICK_MOVE);
			#endif //UPGRADE_10210619_ADD_CLICKLOG	

			if(!CCommunityManager::Instance()->IsConnect())
			{
				ShowMainMessageDlg("커뮤니티 서버와 연결되어 있지 않아 선택한 친구의 그룹을 이동할 수 없습니다.");
				break;
			}

			CBuddyFriend *pBuddy = CCommunityManager::Instance()->IsExistBuddy(strNickName);

			if(pBuddy)
			{
				CBuddyMoveGroupDlg dlg;
				CBuddyGroup *pGroup = pBuddy->GetGroup();
				dlg.DoModal(pBuddy->GetID(), pGroup->IsSendChangeName()?pGroup->GetEditName():pGroup->GetName(), GM().GetMainWnd());
			}
		}
		break;
	case ID_LOBBYPOPUPMENU_TXTBTN_REVERSE_BUDDY_ADD_BLACKLIST:
	case ID_LOBBYPOPUPMENU_TXTBTN_BUDDY_ADD_BLACKLIST:
		{
			if (ID_LOBBYPOPUPMENU_TXTBTN_REVERSE_BUDDY_ADD_BLACKLIST == nID)
			{
				#ifdef UPGRADE_10210619_ADD_CLICKLOG
					SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_USERLIST_POKERFRIENDTAB_FRIEND_ADDED_ME_RCLICK_ADD_BLACKLIST);
				#endif //UPGRADE_10210619_ADD_CLICKLOG
			}
			else if (ID_LOBBYPOPUPMENU_TXTBTN_BUDDY_ADD_BLACKLIST == nID)
			{
				#ifdef UPGRADE_10210619_ADD_CLICKLOG
					SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_USERLIST_POKERFRIENDTAB_MYFRIEND_RCLICK_ADD_BLACKLIST);
				#endif //UPGRADE_10210619_ADD_CLICKLOG
			}

			CBlackBuddyAddDlg dlg;

			if(dlg.DoModal(CBlackBuddyAddDlg::BBADT_QUESTION, "", GM().GetMainWnd()) == IDCANCEL)
			{
				break;
			}

			if(CCommunityManager::Instance()->IsConnect())
			{
				CCommunityManager::Instance()->SendAddBlackBuddy(strID);
			}
			else
			{
				ShowMainMessageDlg("커뮤니티 서버와 연결되어 있지 않아 블랙리스트 목록에 추가할 수 없습니다.");
			}
		}
		break;
	case ID_LOBBYPOPUPMENU_TXTBTN_BUDDY_REMOVE_BLACKLIST:
		{
			if(CCommunityManager::Instance()->IsConnect())
			{
				CBlackBuddyRemoveDlg dlg;
				if(dlg.DoModal(CBlackBuddyRemoveDlg::BBRDT_QUESTION, "", GM().GetMainWnd()) == IDOK)
				{
					CCommunityManager::Instance()->SendRemoveBlackBuddyList(strID);
				}
			}
			else
			{
				ShowMainMessageDlg("커뮤니티 서버에 연결되어 있지 않아 블랙리스트에서 삭제할 수 없습니다.");
			}
		}
		break;
	case ID_LOBBYPOPUPMENU_TXTBTN_BUDDY_SAMEROOM_ENTER:
		{
			CBuddyFriend *pBuddy = CCommunityManager::Instance()->IsExistBuddy(strNickName);
			if(pBuddy)
			{
				SendUILog_Basic(CLSTATS_UINO_POKER_PROFILE_JOIN_FRIENDROOM);
				MM().Call(UPG_CMK_LOBBY_PLAY_TOGETHER_COMMUNITY, (WPARAM)(pBuddy));
			}
		}
		break;
	case ID_LOBBYPOPUPMENU_TXTBTN_BUDDY_TOGETHER_GAME:
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_USERLIST_POKERFRIENDTAB_MYFRIEND_RCLICK_ENTER_SAME_ROOM);
			#endif //UPGRADE_10210619_ADD_CLICKLOG

			CBuddyFriend *pBuddy = CCommunityManager::Instance()->IsExistBuddy(strNickName);
			if(pBuddy)
			{
				MM().Call(UPG_CMK_LOBBY_PLAY_TOGETHER_COMMUNITY, (WPARAM)(pBuddy));
			}
		}
		break;
	case ID_LOBBYPOPUPMENU_TXTBTN_BUDDYGROUP_SENDMEMO:
		if(CCommunityManager::Instance()->IsConnect())
		{
			CBuddyGroup *pGroup = CCommunityManager::Instance()->IsExistBuddyGroup(strNickName);
			if(pGroup)
			{
				if(pGroup->IsEmpty())
				{
					ShowMainMessageDlg("그룹에 쪽지를 보낼 친구가 없습니다.");
				}
				else if(pGroup->GetBuddyCount() > 10)
				{
					ShowMainMessageDlg("쪽지는 10명 이내의 친구에게만 보낼 수 있습니다.");
				}
				else
				{
					std::list<CBuddyFriend *>::const_iterator it = pGroup->GetBuddyList().begin();
					std::string groupNick = (*it)->GetNickName();
					it++;
					for(; it != pGroup->GetBuddyList().end(); ++it)
					{
						groupNick.append("," + (*it)->GetNickName());
					}
					ShowSendMessageDialog(groupNick.c_str(), true);
				}
			}
		}
		else
		{
			ShowMainMessageDlg("커뮤니티 서버와 연결되어 있지 않아 쪽지를 보낼 수 없습니다.");
		}
		break;
	case ID_LOBBYPOPUPMENU_TXTBTN_BUDDYGROUP_SENDMULTICHAT:
		if(CCommunityManager::Instance()->IsConnect())
		{
			CBuddyGroup *pGroup = CCommunityManager::Instance()->IsExistBuddyGroup(strNickName);
			if(pGroup)
			{
				if(pGroup->IsEmpty())
				{
					ShowMainMessageDlg("그룹에 대화를 신청할 친구가 없습니다.");
				}
				else
				{
					std::list<std::string> listInviteID;

					LISTBUDDY::const_iterator start = pGroup->GetBuddyList().begin();
					LISTBUDDY::const_iterator end = pGroup->GetBuddyList().end();

					CBuddyFriend *pBuddy = NULL;
					bool bFirst = true;
					for(;start != end;++start)
					{
						pBuddy = *start;
						if(!pBuddy->IsOffline() && pBuddy->GetRoomNumber() == 0)
							listInviteID.push_back(pBuddy->GetID());
					}

					if(listInviteID.size())
					{
						SendCreateChatting(&listInviteID, "커뮤니티 서버와 연결되어 있지 않아 대화창을 열수 없습니다.");
					}
					else
					{
						ShowMainMessageDlg("그룹에 포함된 친구 중 대화를 신청할 수 있는 친구가 없습니다.");
					}
				}
			}
		}
		else
		{
			ShowMainMessageDlg("커뮤니티 서버와 연결되어 있지 않아 대화에 초대할 수 없습니다.");
		}
		break;
	case ID_LOBBYPOPUPMENU_TXTBTN_REVERSE_BUDDY_ADD_BUDDY:
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_USERLIST_POKERFRIENDTAB_FRIEND_ADDED_ME_RCLICK_ADD_FRIEND);
			#endif //UPGRADE_10210619_ADD_CLICKLOG

			CBuddyAddDlg dlg;

			if(dlg.DoModal(strNickName, GM().GetMainWnd()) == IDCANCEL)
			{
				break;
			}

			if(!CCommunityManager::Instance()->IsConnect())
			{
				ShowMainMessageDlg("커뮤니티 서버와 연결되어 있지 않아 내친구 목록에 추가할 수 없습니다.");
			}
		}
		break;
	case ID_LOBBYPOPUPMENU_TXTBTN_CAFE_TOGETHER_GAME:
		{
			if (nRoomNum > 0)
				MM().Call(UPG_CMK_LOBBY_PLAY_TOGETHER_CAFE, (WPARAM)(nRoomNum));
		}
		break;
	}	
}

void CLobbyView::RemoveSelectedRoom()
{	
	if (m_pRoomListGroup) m_pRoomListGroup->RemoveSelectedRoom();
}

void CLobbyView::OnTimer(UINT nIDEvent)
{
	if( nIDEvent == _DEF_MEMO_RESET_TIMER )
	{
		KillTimer( nIDEvent );
		if( GM().GetCurrentGameType() != IDX_GAMEWHERE_GAME )
		{
			MM().Call(UPG_MEMO_CHECK_START);
			SetTimer( nIDEvent, 60000*5, NULL );
			//SetTimer( nIDEvent, 5000, NULL );
		}
	}
}

/// <summary>
/// 서버에서 보내는 패킷(응답) 처리
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CLobbyView::HandleResponse(int Signal, int TotSize, char *lpData)
{
	switch(Signal)
	{
		//해당 유저의 기타재화 보험금 가입 정보를 보내준다.
	case SV_ETCCHIP_INSURANCE_USER_INFO:
		{
			m_pUserInfoDetailDlg->HandleResponse(Signal, TotSize, lpData);
		}
		break;
		//해당 유저의 기타재화 보험금 변동 내역을 보내준다.(가입, 지급등)
	case SV_ETCCHIP_INSURANCE_USER_CHANGE_INFO:
		{
			m_pUserInfoDetailDlg->HandleResponse(Signal, TotSize, lpData);
		}
		break;
	}
}
