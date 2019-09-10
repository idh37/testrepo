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
		//	/// ��ȯ����
		//	// ��ü �� ����� �䱸��
		//	CSV_ASK_ALLROOMINFO cRoomInfo;
		//	///	cRoomInfo.Set(GM().GetCurrentGameType());
		//	GNET().SendData(GAMESOCK, cRoomInfo.pData, cRoomInfo.GetTotalSize());
		//
		//	// ��ȯ ����
		//	// �ε� �̹����� �����ִ°� ����.
		//	//m_pLobbyView->ShowLoadingRoom(TRUE);
		//
		//	return TRUE;
		//}
		//
		//
		//LRESULT CLobbyModel::SendUserListRequest(WPARAM& wParam, LPARAM& lParam)
		//{
		//	/// ��ȯ����
		//	//MONEYRANGE* pMR  = m_pLobbyView->GetCurrentUserListMoneyRange();
		//	//IDX_GAME idxGame = m_pLobbyView->GetCurrentUserListGame();
		//
		//	//if(pMR)
		//	//{
		//	//	CSV_ASK_ALLUSERINFO cUserInfo;
		//	//	cUserInfo.Set(idxGame, *pMR);	//����
		//	//	GNET().SendData(GAMESOCK, cUserInfo.pData, cUserInfo.GetTotalSize());
		//
		//	//	// ��ȯ ����
		//	//	// �ε� �̹����� �����ִ°� ����.
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
		//	///< 500 ������ ����� ����� ��� �����ֱ�
		//	int nTotalSendCnt = *msg.m_nTotalSendCnt;				///< �� ���� ��Ŷ����
		//	int nCurSendCnt   = *msg.m_nCurSendCnt;					///< ���� ������ �ִ� ��Ŷ�ε���(1���ͽ���)	
		//	ASSERT( nCurSendCnt <= nTotalSendCnt);
		//
		//	BOOL bFirstPacket = (nCurSendCnt == 1);					///< ó�� ��Ŷ���� üũ(500���� ���� ������.)
		//	BOOL bLastPacket  = (nCurSendCnt == nTotalSendCnt);		///< ������ ��Ŷ���� üũ(500���� ���� ������.)
		//
		//	if(bFirstPacket)
		//	{
		//		m_pLobbyProcess->DelAllData_User();
		//	}
		//
		//	for(int i = 0; i < *msg.TotNum; ++i)					
		//	{					
		//		//### [������ ��� �۾�] ###
		//		if(msg.ArraySUI[i].AdminGrade == 0)
		//		{
		//			m_pLobbyProcess->AddData_User(&msg.ArraySUI[i]);
		//		}				
		//	}	
		//
		//	if(bLastPacket)
		//	{
		//		//����Ʈ ����
		//		GM().GetDataUserList()->Sort_Default();
		//
		//		//�� ��������
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
		//	// 500 ������ ����� ����� ��� �����ֱ�
		//	int nTotalSendCnt = *msg.m_nTotalSendCnt;			///< �� ���� ��Ŷ����
		//	int nCurSendCnt   = *msg.m_nCurSendCnt;				///< ���� ������ �ִ� ��Ŷ�ε���(1���ͽ���)	
		//	ASSERT( nCurSendCnt <= nTotalSendCnt);
		//
		//	BOOL bFirstPacket = (nCurSendCnt==1);				///< ó�� ��Ŷ���� üũ(500���� ���� ������.)
		//	BOOL bLastPacket  = (nCurSendCnt == nTotalSendCnt);	///< ������ ��Ŷ���� üũ(500���� ���� ������.)
		//
		//
		//	if(bFirstPacket)
		//	{
		//		//������ Ŭ����			
		//		m_pLobbyProcess->DelAllData_Room();
		//	}
		//
		//	//������ �߰�
		//	for(int i = 0; i < *msg.TotNum; ++i)
		//	{					
		//		m_pLobbyProcess->AddData_Room(&msg.ArrayRI[i]);			
		//	}
		//
		//	if(bLastPacket)
		//	{
		//		//���̹� �߰�
		//		m_pLobbyProcess->MakeDummyRooms();	
		//
		//		//����Ʈ ����// ����Ʈ ���� ���� �����ʰ��ӽ���� ����ӹ濡�����Ƿ��̵��� �����VIP ���ӹ�, �Ϲݰ��ӹ����̵���
		//		GM().GetDataRoomList()->Sort_Default();
		//
		//		//�� ��������
		//		m_pLobbyView->RefreshRoomList();
		//
		//		//�ε��� �ִϸ��̼� ����
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
		//	// ��ȯ����	
		//	// �����˾��� �� �� �ֵ��� �Ѵ�.
		//	//GM().SetWaitResponse(false);
		//	//GM().SetNowLogout(true);
		//	//CloseClient();
		//
		//	return TRUE;
		//}

	}//namespace MODEL

}//namespace NMBASE
