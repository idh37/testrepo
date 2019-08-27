#include "stdafx.h"
#include "LobbyModel.h"

namespace NMBASE
{
	namespace MODEL
	{

		CLobbyModel::CLobbyModel()
		{

		}


		CLobbyModel::~CLobbyModel()
		{

		}


		BOOL CLobbyModel::OnAttached()
		{
			//UTIL::MM().AddNetMsgH(NET_GAME_SND_ALLROOMINFO,	Fnt(this, &CLobbyModel::SendRoomListRequest));
			//UTIL::MM().AddNetMsgH(NET_GAME_SND_ALLUSERINFO,	Fnt(this, &CLobbyModel::SendUserListRequest));
			//UTIL::MM().AddNetMsgH(NET_GAME_SND_LOGOUT,		Fnt(this, &CLobbyModel::SendLogOutRequest));


			//UTIL::MM().AddNetMsgH(MSG_GAME_PINGCHECK,			Fnt(this, &CLobbyModel::GameServerPingCheck));	
			//UTIL::MM().AddNetMsgH(MSG_GAME_ALLUSERINFO,		Fnt(this, &CLobbyModel::AllUserInfo));	
			//UTIL::MM().AddNetMsgH(MSG_GAME_ALLROOMINFO,		Fnt(this, &CLobbyModel::AllRoomInfo));	

			//UTIL::MM().AddNetMsgH(MSG_GAME_LOGOUT,			Fnt(this, &CLobbyModel::Logout));	



			return TRUE;
		}

		BOOL CLobbyModel::OnDetached()
		{
			///< Delete
			delete this;

			return TRUE;
		}

		//
		//LRESULT CLobbyModel::GameServerPingCheck(WPARAM& wParam, LPARAM& lParam)
		//{
		//	NMSOCK::SGAMENOTIFY *pNotifyData = (NMSOCK::SGAMENOTIFY*)lParam;
		//
		//	CSV_HEARTBEATGHOSTCHECK msg;
		//	msg.Get(pNotifyData->lpData, pNotifyData->TotSize);
		//	
		//	BOOL bResult = GNET().SendData(GAMESOCK, msg.pData, msg.GetTotalSize());
		//
		//	return bResult;
		//}
		//
		//LRESULT CLobbyModel::SendRoomListRequest(WPARAM& wParam, LPARAM& lParam)
		//{
		//	/// 종환복구
		//	// 전체 방 목록을 요구함
		//	CSV_ASK_ALLROOMINFO cRoomInfo;
		//	///	cRoomInfo.Set(GM().GetCurrentGameType());
		//	GNET().SendData(GAMESOCK, cRoomInfo.pData, cRoomInfo.GetTotalSize());
		//
		//	// 종환 복구
		//	// 로딩 이미지를 보여주는것 같다.
		//	//m_pLobbyView->ShowLoadingRoom(TRUE);
		//
		//	return TRUE;
		//}
		//
		//
		//LRESULT CLobbyModel::SendUserListRequest(WPARAM& wParam, LPARAM& lParam)
		//{
		//	/// 종환복구
		//	//MONEYRANGE* pMR  = m_pLobbyView->GetCurrentUserListMoneyRange();
		//	//IDX_GAME idxGame = m_pLobbyView->GetCurrentUserListGame();
		//
		//	//if(pMR)
		//	//{
		//	//	CSV_ASK_ALLUSERINFO cUserInfo;
		//	//	cUserInfo.Set(idxGame, *pMR);	//수정
		//	//	GNET().SendData(GAMESOCK, cUserInfo.pData, cUserInfo.GetTotalSize());
		//
		//	//	// 종환 복구
		//	//	// 로딩 이미지를 보여주는것 같다.
		//	//	///m_pLobbyView->ShowLoadingUser(TRUE);
		//	//}
		//
		//	return TRUE;
		//}
		//
		//
		//LRESULT CLobbyModel::SendLogOutRequest(WPARAM& wParam, LPARAM& lParam)
		//{
		//	CSV_ASK_LOGOUT cLogout;
		//	cLogout.Set(GM().GetMyInfo()->UI.UNum, GM().GetMyInfo()->UI.ID);
		//	GNET().SendData(GAMESOCK, cLogout.pData, cLogout.GetTotalSize());
		//
		//	return TRUE;
		//}
		//
		//
		//
		//LRESULT CLobbyModel::AllUserInfo(WPARAM& wParam, LPARAM& lParam)
		//{
		//	NMSOCK::SGAMENOTIFY *pNotifyData = (NMSOCK::SGAMENOTIFY*)lParam;
		//
		//	CSV_ALLUSERINFO msg;
		//	msg.Get(pNotifyData->lpData, pNotifyData->TotSize);
		//	
		//	///< 500 단위로 사용자 목록을 끊어서 보내주기
		//	int nTotalSendCnt = *msg.m_nTotalSendCnt;				///< 총 보낼 패킷개수
		//	int nCurSendCnt   = *msg.m_nCurSendCnt;					///< 현재 보내고 있는 패킷인덱스(1부터시작)	
		//	ASSERT( nCurSendCnt <= nTotalSendCnt);
		//
		//	BOOL bFirstPacket = (nCurSendCnt == 1);					///< 처음 패킷인지 체크(500개씩 끊어 보낸다.)
		//	BOOL bLastPacket  = (nCurSendCnt == nTotalSendCnt);		///< 마지막 패킷인지 체크(500개씩 끊어 보낸다.)
		//
		//	if(bFirstPacket)
		//	{
		//		m_pLobbyProcess->DelAllData_User();
		//	}
		//
		//	for(int i = 0; i < *msg.TotNum; ++i)					
		//	{					
		//		//### [관리자 모드 작업] ###
		//		if(msg.ArraySUI[i].AdminGrade == 0)
		//		{
		//			m_pLobbyProcess->AddData_User(&msg.ArraySUI[i]);
		//		}				
		//	}	
		//
		//	if(bLastPacket)
		//	{
		//		//디폴트 정렬
		//		GM().GetDataUserList()->Sort_Default();
		//
		//		//뷰 리프레쉬
		//		m_pLobbyView->RefreshUserList();
		//		m_pLobbyView->ShowLoadingUser(FALSE);	
		//	}
		//
		//	return TRUE;
		//}
		//
		//
		//LRESULT CLobbyModel::AllRoomInfo(WPARAM& wParam, LPARAM& lParam)
		//{
		//	NMSOCK::SGAMENOTIFY *pNotifyData = (NMSOCK::SGAMENOTIFY*)lParam;
		//
		//	CSV_ALLROOMINFO msg;
		//	msg.Get(pNotifyData->lpData, pNotifyData->TotSize);
		//
		//	// 500 단위로 사용자 목록을 끊어서 보내주기
		//	int nTotalSendCnt = *msg.m_nTotalSendCnt;			///< 총 보낼 패킷개수
		//	int nCurSendCnt   = *msg.m_nCurSendCnt;				///< 현재 보내고 있는 패킷인덱스(1부터시작)	
		//	ASSERT( nCurSendCnt <= nTotalSendCnt);
		//
		//	BOOL bFirstPacket = (nCurSendCnt==1);				///< 처음 패킷인지 체크(500개씩 끊어 보낸다.)
		//	BOOL bLastPacket  = (nCurSendCnt == nTotalSendCnt);	///< 마지막 패킷인지 체크(500개씩 끊어 보낸다.)
		//
		//
		//	if(bFirstPacket)
		//	{
		//		//데이터 클리어			
		//		m_pLobbyProcess->DelAllData_Room();
		//	}
		//
		//	//데이터 추가
		//	for(int i = 0; i < *msg.TotNum; ++i)
		//	{					
		//		m_pLobbyProcess->AddData_Room(&msg.ArrayRI[i]);			
		//	}
		//
		//	if(bLastPacket)
		//	{
		//		//더미방 추가
		//		m_pLobbyProcess->MakeDummyRooms();	
		//
		//		//디폴트 정렬// 디폴트 정렬 시점 ①최초게임실행시 ②게임방에서대기실로이동시 ③대기실VIP 게임방, 일반게임방탭이동시
		//		GM().GetDataRoomList()->Sort_Default();
		//
		//		//뷰 리프레쉬
		//		m_pLobbyView->RefreshRoomList();
		//
		//		//로딩중 애니메이션 종료
		//		m_pLobbyView->ShowLoadingRoom(FALSE);	
		//	}
		//
		//	return TRUE;
		//}
		//
		//
		//LRESULT CLobbyModel::Logout(WPARAM& wParam, LPARAM& lParam)
		//{
		//	NMSOCK::SGAMENOTIFY *pNotifyData = (NMSOCK::SGAMENOTIFY*)lParam;
		//
		//	CSV_ACCEPT_LOGOUT msg;
		//	msg.Get(pNotifyData->lpData, pNotifyData->TotSize);
		//
		//
		//	// 종환복구	
		//	// 종료팝없이 뜰 수 있도록 한다.
		//	//GM().SetWaitResponse(false);
		//	//GM().SetNowLogout(true);
		//	//CloseClient();
		//
		//	return TRUE;
		//}

	}//namespace MODEL

}//namespace NMBASE
