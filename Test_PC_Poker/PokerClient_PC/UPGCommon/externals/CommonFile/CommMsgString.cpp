#include "StdAfx.h"
#include "CommMsgString.h"

#include "CommMsgIDHeader.h"
#include "EventCommon.h"


std::string ToStringCommMsgIDHeader(int nSignal)
{
	////////////////// ���� ��ġ�� �������� ���ƾ� �� �κ� ////////////////
	std::string sStr;
	switch(nSignal){
	CASEENUMSTRING(MS_CHECKVERSION) //)// 2000,		// ���� üũ
	/////////////////// ���� �������� ��ſ� �޼��� ///////////////////
	CASEENUMSTRING(MS_ASK_SERVERLOGIN) // )// 2100,	// ���Ӽ��� �α��� ��û
	CASEENUMSTRING(MS_ACCEPT_SERVERLOGIN) //,		// ���Ӽ��� �α��� ���
	CASEENUMSTRING(MS_REFUSE_SERVERLOGIN) //,		// ���Ӽ��� �α��� �Ұ�
	CASEENUMSTRING(MS_SERVERSTATUS) //,			// ���� ���� ���� ���� ����
	CASEENUMSTRING(MS_MASTERSTATUS) //,			// ������ ���� ���� ����(�α��� ��� ���� ���� ���� ������)
	CASEENUMSTRING(MS_CONTROLCMD) //,				// ���Ӽ��� ���� ���
	CASEENUMSTRING(MS_STATUSLOG) //,				// ���� ����͸� �α�(from ���Ӽ���)
	CASEENUMSTRING(MS_USERINFOLIST) //,			// ����� ���� ����Ʈ(���� �α�� ������Ʈ��)
	CASEENUMSTRING(MS_CHANUPDATE) //,				// ä�� ������Ʈ ����
	CASEENUMSTRING(MS_USERCHANGECHAN) //,			// ����� ä�� ���� ����(����, ����, ����)
	CASEENUMSTRING(MS_DISCONNECTUSER) //,			// ����� ���� ���� ���
	CASEENUMSTRING(MS_CHECKUSER) //,				// ����� ���� üũ ���
	CASEENUMSTRING(MS_ASK_MOVECHAN) //,			// ����ڰ� �ٸ� ä�η� �̵� ��û
	CASEENUMSTRING(MS_RESULT_MOVECHAN) //,			// ����ڰ� �ٸ� ä�η� �̵� ��û ���
	CASEENUMSTRING(MS_ASK_FINDUSER) //,			// ����� ã�� ��û
	CASEENUMSTRING(MS_RESULT_FINDUSER) //,			// ����� ã�� ���
	CASEENUMSTRING(MS_ASK_MOVEGAME) //,			// �����̵� ���� ��û
	CASEENUMSTRING(MS_RESULT_MOVEGAME) //,		// �����̵� ���� ����
	/////////////////// ���� Ŭ���̾�Ʈ���� ��ſ� �޼��� ///////////////////
	CASEENUMSTRING(MS_ASK_DESTSERVERINFO) // )// 2500,	// ������ ���Ӽ��� ���� ��û
	CASEENUMSTRING(MS_REFUSE_DESTSERVERINFO) //,		// ���� �ź�
	CASEENUMSTRING(MS_DESTSERVERINFO) //)//				// ������ ���Ӽ��� ����
	//����õ�� 2006.3.2/////////////////////////////////////////////////////////////////////////////////////////////
	CASEENUMSTRING(MS_SEND_GS_CHARGEINOF)//	// �����Ͱ� �� �����鿡�� ����õ�� ������ �����ش�
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 	MS_ADD_AUTOUSER)//   // �Ƕ� �������� ������ ������ �� ��!!!
// 	MS_DEL_AUTOUSER)//
	//[���ƹ�Ÿ] 2006.07.24
	CASEENUMSTRING(MS_SEND_SUPPLY_LUKCAVATAINFO)//3000)//	// �����Ͱ� �� �����鿡�� ���ƹ�Ÿ �Ⱦ� ����� ����Ѵ�.
	CASEENUMSTRING(MS_RESULT_LUCKAVATAIFO)//			// ���Ӽ����� �ȾҴٸ� �����Ϳ��� �����ش� � �ȾҴٰ�
	//������ ���ø� 
	CASEENUMSTRING(MS_ADMIN_COMMAND)//		//������ ��ɾ� 
// 	//���̽ý��� 
// 	MS_JACKPOT_BROADCASTSTATE)// 		//������ ���¸� �˸���.
// 	MS_JACKPOT_COUNTHALF)//			//����->������   ����ī��Ʈ�� �˷��ش�.
// 	MS_JACKPOT_NOMINATION)//			//������ -> ���� ���� �ĺ����� �˷��ش�.
// 	MS_JACKPOT_WINROOM)//				//������ -> ���� ���� ��÷���� �˷��ش�.
// 	MS_JACKPOT_NOTIFYHALF)//			//������ -> ���� -> Ŭ�� �ּ� �ĺ� ���� ī��Ʈ�� �˷��ش�. 	
// 	MS_JACKPOT_ROOM_STATE)//			//���� -> Ŭ�� ���̷� State�� �˷��ش�.
// 	MS_JACKPOT_WINJACKPOT)//			//���� -> Ŭ�� ���� ��÷
// 	MS_JACKPOT_REPLYJACKPOT)//		//���� -> ������  �ش���� ���� �ݾ��� �����Ϳ� �˷��ش�.
// 	MS_JACKPOT_DISPLAY)//				//���� -> Ŭ�� ���̵��÷��̿�  �ݾ�
// 	MS_JACKPOT_WINROOM_FOR_LOSER)//   //���� -> Ŭ�� ���� ��÷���� �˷��ش�.
// 	MS_JACKPOT_DESTORYROOM)//			//���� -> ������ ������ �˷��ش�. 
	/*
	���α� �߰� 2008.05.19
	*/	
	CASEENUMSTRING(MS_NOTIFY_LOG_CHANNEL_USER)//			// �����Ϳ��� ������ ä�κ� ������ ������ �˸�
	// ������Ŀ ���̽ý���
// 	MS_SEND_GAMEFEE_TO_MASTER)//
// 	MS_SEND_JACKPOTMONEY_TO_GAMESERVER)//
// 	MS_OFFER_JACKPOTMONEY)//
// 	MS_SEND_OVERJACKPOTMONEY)//
	// 2011�� 1�� ���θ��
	CASEENUMSTRING(MS_OFFER_RESTING_PROMOTION_MONEY)//
	CASEENUMSTRING(MS_GROUP_INSPECTION_INFO)//			// �׷� ���˻��� ����
	CASEENUMSTRING(MS_GROUP_INSPECTION_ADMININFO)//	// �׷����˽� ���尡���� ���� ����
	CASEENUMSTRING(MS_OPENTOURNAMENT_NOTI)//				// ��ʸ�Ʈ ���� ����
	////////////////// ���� ��ġ�� �������� ���ƾ� �� �κ� ////////////////
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	CASEENUMSTRING(SV_CHECKVERSION)//10)//		// ���� üũ
	CASEENUMSTRING(SV_RECONNECT)//		// �����ؼ� ����
	CASEENUMSTRING(SV_ASK_LOGIN)//		// ���� ��û �α��� ����	// [SSO �۾�] - SV_ASK_LOGIN�� �̸��� �ٲ�
	CASEENUMSTRING(SV_SERVERINFO)//		// ���� ����	[���� ������ �۾�]
	/////////////////// ������ ������ �޼��� ///////////////////
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	CASEENUMSTRING(SV_ACCEPT_LOGIN)//	// �α����� �����
	CASEENUMSTRING(SV_REFUSE_LOGIN)//		// �α����� �ź���(����) ���̵� ����)// ��й�ȣ Ʋ��)// ������ Ʋ��)
	CASEENUMSTRING(SV_ASK_LOGOUT)//			// �α׾ƿ��� ��û��
	CASEENUMSTRING(SV_ACCEPT_LOGOUT)//		// �α׾ƿ��� �����
	CASEENUMSTRING(SV_HEARTBEAT)//			// ��Ʈ��Ʈ
	CASEENUMSTRING(SV_DESTCHANNELINFO)//		// ������ ä�� ����(�ٸ� ������ ä�� �̵���)
	CASEENUMSTRING(SV_CONNECT_MOVECHAN)//	// �ٸ��������� ä�� �̵����� ���� ��û
	CASEENUMSTRING(SV_DESTGAMEINFO)//		// ������ ä�� ����(�ٸ� ������ ���� �̵���)
	CASEENUMSTRING(SV_CONNECT_MOVEGAME)//	// �ٸ��������� ���� �̵����� ���� ��û
	CASEENUMSTRING(SV_ASK_DETAILUSERINFO)//	// ������� �� ������ �䱸(�����ڿ�)
	CASEENUMSTRING(SV_DETAILUSERINFO)//		// ������� �� ����
	CASEENUMSTRING(SV_ASK_ENTERCHAN)//		// ä�� ������ �䱸
	CASEENUMSTRING(SV_ACCEPT_ENTERCHAN)//	// ä�� ������ �㰡
	CASEENUMSTRING(SV_ASK_FINDID)//			// ���̵� ã�� ��û
	CASEENUMSTRING(SV_RESULT_FINDID)//		// ���̵� ã�� ���
	CASEENUMSTRING(SV_USERINFO)//			// ����� ������ ������(��� ) Ư�� ����� ���� �䱸��)
	CASEENUMSTRING(SV_CHANGEMYINFO)//		// ����� ����� ������ �˷���
	CASEENUMSTRING(SV_ALLUSERINFO)//			// ��� ������ ����� �����ش�(��� ) �䱸�ϴ� �����)
	CASEENUMSTRING(SV_NEWUSER)//				// ���ο� ����ڰ� ���ӵǾ���(��� ) ��ü �����)
	CASEENUMSTRING(SV_OUTUSER)//				// ����ڰ� ���������Ͽ���(��� ) ��ü �����)
	CASEENUMSTRING(SV_CHANGEUSER)//			// ������ �ٸ� ������� ������ ����Ǿ���(���) ��ü�����)
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	CASEENUMSTRING(SV_ACCEPT_CHANGECHAR)//	// ����� ĳ���͸� �ٲٱ⸦ ���
	CASEENUMSTRING(SV_REFUSE_CREATEROOM)//	// �� ����⸦ �ź���(���) �� ������ �õ��� �����)
	// ������ ���� ���� �ڵ�(0)�� ���� ���� �� �ʰ�)// 1)������ ���̵�)
	CASEENUMSTRING(SV_ACCEPT_CREATEROOM)//	// �� ����� �䱸�� ���� ���� ���������(���) �� ������ �õ��� �����)
	// ����ID)// �� ����)// �� ����(����)// ����)
	CASEENUMSTRING(SV_ALLROOMINFO)//			// ������ ���� ��ü ������ �����ش�(��� ) �䱸�ϴ� �����)
	CASEENUMSTRING(SV_ROOMINFO)//			// Ư�� ���� ������ �����ش�(��� ) �䱸�ϴ� �����)
	CASEENUMSTRING(SV_ROOMMASTER)//			// Ư���� ���� ���� ������ �����ش�(��� ) �䱸�ϴ� �����)
	CASEENUMSTRING(SV_NEWROOM)//				// ���ο� ���� �����Ǿ����� �˸�
	CASEENUMSTRING(SV_OUTROOM)//				// ���� ��������� �˸�
	CASEENUMSTRING(SV_CHANGEROOM)//			// �� ������ �����Ǿ����� �˸�
	CASEENUMSTRING(SV_USEROUTROOM)//			// ����ڰ� �濡�� ���Դ� - ���� �� �÷��̾�鿡�� �뺸
	CASEENUMSTRING(SV_ACCEPT_OUTROOM)//		// �濡�� �����⸦ �㰡
	CASEENUMSTRING(SV_CHANGEROOMMASTER)//	// ������ �ٲ��� �˸���
	CASEENUMSTRING(SV_ABNORMALOUTROOM)//		// ����ڿ��� ������������ �����ϱ⸦ ��û(������� �����ӿ� �������� ���)
	// ### [���� �߰� �۾�] ###
	CASEENUMSTRING(SV_ASK_CHANGESNDFX)//     // ��û
	CASEENUMSTRING(SV_ACCEPT_CHANGESNDFX)//  // ���
	/////////////////// ������ �޴� �޼��� ///////////////////
	CASEENUMSTRING(SV_ASK_BADUSER)//			// ä��â ��ȭ���� �Ű� ��û
	CASEENUMSTRING(SV_ASK_CHANGEMYINFO)//	// ���� ����� ���� ������ �䱸
	CASEENUMSTRING(SV_ASK_ALLCHANINFO)//		// ��ü ä�� ������ �䱸
	CASEENUMSTRING(SV_ALLCHANINFO)//			// ��ü ä�� ����
	CASEENUMSTRING(SV_ASK_ALLROOMINFO)//		// ��ü �� ����� �䱸
	CASEENUMSTRING(SV_ASK_ROOMINFO)//		// Ư�� �������� �䱸
	CASEENUMSTRING(SV_ASK_ROOMMASTER)//		// Ư�� ���� ���� ������ �䱸
	CASEENUMSTRING(SV_ASK_ALLUSERINFO)//		// ��� ������ ����� �䱸
	CASEENUMSTRING(SV_ASK_USERINFO)//		// Ư�� ����� ������ �䱸
	CASEENUMSTRING(SV_ASK_CHANGECHAR)//		// ����� ĳ���͸� �ٲٱ⸦ �䱸
	CASEENUMSTRING(SV_ASK_CREATEROOM)//		// �� ����⸦ �䱸
	CASEENUMSTRING(SV_ASK_ENTERROOM)//555)//		// �濡 �����ϱ⸦ �䱸
	CASEENUMSTRING(SV_ACCEPT_ENTERROOM)//	// �濡 �����ϱ⸦ �㰡��(���) ������ �䱸�� �����)
	CASEENUMSTRING(SV_REFUSE_ENTERROOM)//	// �� �����û�� �ź���(���) ������ �䱸�� �����)
	CASEENUMSTRING(SV_USERENTERROOM)//		// ����ڰ� �濡 �����Ͽ���
	CASEENUMSTRING(SV_LOBYOPEN)//			// ���� ���� �ʴ�â ���� ���� �˸�
	CASEENUMSTRING(SV_ASK_OUTROOM)//			// �濡�� �����⸦ �䱸
	CASEENUMSTRING(SV_ASK_INVITE)//			// ��û �޼����� ����
	CASEENUMSTRING(SV_REFUSE_INVITE)//		// ��û �޼����� ����
	CASEENUMSTRING(SV_ASK_BANISHVOTE)//		// �߹� ��û
	CASEENUMSTRING(SV_MYBANISHVOTE)//		// ���� �߹� ��ǥ ���
	CASEENUMSTRING(SV_NOWHISPER)//			// �Ӹ� �ź�)//�㰡 ���� ����
	CASEENUMSTRING(SV_ASK_FORCEBANISH)//		// �����߹� ��û	[���� ������ �۾�]
	CASEENUMSTRING(SV_NOTIFY_BANISHCOUNT)//	// �����߹� ī��Ʈ Ŭ���̾�Ʈ�� �˷��ֱ�
	/////////////////// ���� ������ �޼��� [���� ������ �۾�] ///////////////////
	CASEENUMSTRING(SV_ASK_GAMEITEMLIST)//	// ���� ������ ��� ������ ��û
	CASEENUMSTRING(SV_GAMEITEMLIST)//		// ���� ������ ����� ������
	CASEENUMSTRING(SV_USEGAMEITEM)//			// ���� ������ ��� ��û
	CASEENUMSTRING(SV_BUYGAMEITEM)//			// ���� �������� �����Ұ����� ���
	/////////////////// ���� �޼��� ///////////////////
	CASEENUMSTRING(SV_SERVERMSG)//			// ���� �޼���
	CASEENUMSTRING(SV_CHATDATA)//			// ���ǿ� ä�� ����Ÿ(����) ������ ������ ��� ���� �޼��� ���)
	CASEENUMSTRING(SV_WHISPERCHAT)//			// �ӼӸ�
	CASEENUMSTRING(SV_ASK_QUICKSTART)//		// ���� �ٷ� ����
	CASEENUMSTRING(SV_REFUSE_QUICKSTART)//	// �ٷν��� �ȵ� (�游���)
	CASEENUMSTRING(SV_ASK_QUICKSTART2)//		// ������ �ٷ� ����
	CASEENUMSTRING(SV_REFUSE_QUICKSTART2)//	//
	CASEENUMSTRING(SV_ORDER_QUICKSTART_IN_ROOM)//	// �ٸ��� �ٷΰ��� Order
	CASEENUMSTRING(CL_ASK_QUICKSTART_IN_ROOM)//		// �ٸ��� �ٷΰ��� ��û
	CASEENUMSTRING(SV_REFUSE_QUICKSTART_IN_ROOM)//	// �ٸ��� �ٷΰ��� ����
	/////////////////// ### [ ������� ] ###  ///////////////////
	CASEENUMSTRING(SV_ASK_PARTICIPATION)//   // ������ ��û
	CASEENUMSTRING(SV_ACCEPT_PARTICIPATION)// // ����
	CASEENUMSTRING(SV_ENTER_PARTICIPATION)//  // �ٸ� ����ڿ��� �˸���
	CASEENUMSTRING(SV_ASK_EMERAGE)//         // ���������� ��������
	CASEENUMSTRING(SV_ACCEPT_EMERAGE)//      // ����
	CASEENUMSTRING(SV_ENTER_EMERAGE)//		// �ٸ� ����ڿ��� �˸���
	CASEENUMSTRING(SV_RESERVATIONINFO)//     // ������ ����
	CASEENUMSTRING(SV_OBSERVER_MESSAGE)//	// ���� ���� �޼���
	/////////////////// ### [������ ��� �۾�] ###  ///////////////////
	CASEENUMSTRING(NM_ADMIN_WHISPERCHAT)//	// �Ӹ��ϱ�(����)
	CASEENUMSTRING(NM_ADMIN_WARNUSER)//		// ����� ����(���)
	CASEENUMSTRING(NM_ADMIN_OUTUSER)//		// ����� �����ϱ�(���)
	CASEENUMSTRING(NM_ADMIN_CHANGEROOM)//	// �׹� �ɼ� �����û(���)
	CASEENUMSTRING(NM_ADMIN_DELROOM)//		// �������ϱ�(���)
	CASEENUMSTRING(SV_CHANGESTYLEROOMINFO)// // ���̸� ���� �˸�(���)
	CASEENUMSTRING(NM_SPECIALMESSAGE)//      // Ư�� ���� �޼���(���)
	CASEENUMSTRING(NM_ADMIN_SETTINGMAN)//	// ������ ���� ȯ�漳�� ���� 
	CASEENUMSTRING(SV_SERVERNOTIC)//			// ���� ���� �޼���
	CASEENUMSTRING(CL_ASK_CHSTYLEROOMINFO)// // ����������	
	CASEENUMSTRING(CL_CHARGE_MONEY)//	//  �Ӵ� ���� �޽���
	CASEENUMSTRING(SV_SPEEDHACK_DEFENSE)//	// [���ǵ���] 2004.09.01
	CASEENUMSTRING(SV_ASK_GAME_CONTINUE)//	// [DB��� Process] 2004.11.29
	CASEENUMSTRING(SV_RES_GAME_CONTINUE)//	// [DB��� Process] 2004.11.29
	CASEENUMSTRING(SV_ASK_ENTERRULEROOM)//	// [�ڵ� ���÷�] 2005.05.25 - ��� ���� �����
	CASEENUMSTRING(SV_ASK_BUYITEM)//				// ������ �Ķ���� ��û
	CASEENUMSTRING(SV_ALLOW_BUYITEM)//			// 
	CASEENUMSTRING(SV_BUYITEMPARAM)//			// ������ �Ķ���͸� ����
	CASEENUMSTRING(SV_BUYITEM_RESULT)//			// ������ �Ķ���� ��û�� ���� ���
	CASEENUMSTRING(SV_ASK_CHECK_REWARD)//		// ������ ���� ���� üũ
	CASEENUMSTRING(SV_REWARD_CHECK_RESULT)//		// ������ ���� ���� üũ ���
	// reward item db 08.02.27	
	CASEENUMSTRING(SV_REWARDITEM_LIST)//			// DB����� ���������� ������
	/////////////////// ### [���κ���] ###  ///////////////////
	CASEENUMSTRING(SV_AIN_INSURANCE_JOIN_REQUEST)//		// ���κ��� ���� ��û
	CASEENUMSTRING(SV_AIN_INSURANCE_SUPPLY_REQUEST)//	// ���κ��� ���� ��û
	CASEENUMSTRING(SV_AIN_INSURANCE_JOIN_RESULT)//		// ���κ��� ���� ��û ��� �޼���
	CASEENUMSTRING(SV_AIN_INSURANCE_SUPPLY_RESULT)//		// ���κ��� ��� �޼���
	// Ŭ���̾�Ʈ�� �޼��� #########################################################
	/////////////////// ���� �޼��� ///////////////////
	CASEENUMSTRING(CL_EXITSUBSCRIPT)//	// �÷��̾��� ������ ���� ����
	CASEENUMSTRING(CL_CHATDATA)//			// Ŭ���̾�Ʈ���� ä�� �޼���
	// [ ���� ������ ]
	CASEENUMSTRING(CL_ITEMRESULT)//			// ������ �����..( �������� ���� ��쿡��.. )
	CASEENUMSTRING(CL_ITEMLIST)//			// ������ ����Ʈ..
	CASEENUMSTRING(CL_ITEMLOGIN)//			// �������� ����� �α���..
	CASEENUMSTRING(CL_ASK_ITEMLOGIN)//		// ������ ����� ���ΰ� ���.
	CASEENUMSTRING(CL_ACCEPT_ITEMLOGIN)//	// ������ ���� �α��� �㰡.
	// [���� ������ �۾�]
	CASEENUMSTRING(CL_ASK_JUMPITEM)//		// ä�ο��� ���������� ��� ���.
	CASEENUMSTRING(CL_JUMPITEM_LOGIN)//		// ä�ο��� ���������� ��� �α�
	CASEENUMSTRING(CL_ACCEPT_JUMPITEM_CH)//  // ���� -> ���� ��û DB OK )//> �������(������ ���̾�α� �ڽ��� �����ش�)
	CASEENUMSTRING(CL_ACCEPT_JUMPITEM)//		// ä�ο��� ���������� �ѹ��̶� ����� ������ ������(�ٷ� ����)
	CASEENUMSTRING(SV_LEADERSJOIN)//			//2004.05.07 �������Ѿ� ����
	CASEENUMSTRING(SV_SPECIALCARD)//			// ��� ���� �����ڿ��� �˸�)// ����
	//���ӹ泻 ����޼��� 
	CASEENUMSTRING(SV_ENDGAME)//				// ������ �˸�
	CASEENUMSTRING(SV_ASKGAMESTART)//		// ���ӽ��� ��û
	CASEENUMSTRING(SV_STARTBTNENABLE)//		// ��ưȰ��ȭ(�޼���)	
	CASEENUMSTRING(SV_PREM_OPTION_INFO)//		// [������� �۾�] [2009-02-12]
	CASEENUMSTRING(CL_ASK_RECOVERY_CHARGE)//			// [������� �۾�] [2009-02-10]
	CASEENUMSTRING(CL_RECOVERY_CHARGE_INFO)//		// [������� �۾�] [2009-02-11]
	//���ȸ�� 
	CASEENUMSTRING(CL_REPLY_KISACHARGE)//		//���ȸ�� �ɼǻ�ǰ
	CASEENUMSTRING(SV_NOTIFY_ALLIN)//			//����â ����ֱ� 
	//������� �ٷ�����
	CASEENUMSTRING(CL_ASK_PREMQUICKJOIN)//	//������� �ٷ����� ��û
	CASEENUMSTRING(SV_REFUSE_PREMQUICKJOIN)// //������� �ٷ�����  ���ǿ� �ش��ϴ� ���� ���� ��
	CASEENUMSTRING(SV_SEATKEEPER)//			//�ڸ� ��Ŵ�� 
	CASEENUMSTRING(CL_ASK_WAIT_INROOM)//		// ��� ���
	CASEENUMSTRING(SV_ASK_WAIT_INROOM)//
	CASEENUMSTRING(SV_ROOMOPTIONINFO)//		/// �� �ɼ� ���� 
	CASEENUMSTRING(CL_NOTIFY_MACADDRESS)//   //DAT�α� (�������� �ƾ�巹�� �뺸)
	CASEENUMSTRING(SV_PCROOMEVENT_INFO)//		//pc���̺�Ʈ
	CASEENUMSTRING(SV_YEARENDEVENT_INFO)//		//�����̺�Ʈ
	CASEENUMSTRING(SV_GC_ASK_JOKBO_STEP2)//	//���� 2�ܰ� ����Ʈ ��û
	//G_TFT
	CASEENUMSTRING(SV_CHANGE_MONEY)//		//PMoney ��ȭ�� ������ �˷��ִ� �޼���
	CASEENUMSTRING(SV_NOTIFY_CHANGEOVERMONEY)//	// �ʰ���)// �����Ӵ� �߻��� �˷���(ä�� �����)// ���ӿ�����)
	CASEENUMSTRING(SV_NOTIFY_CHANGEPREMIUM)//	// ȸ���� ���� ���� �˸�
	// ���� Ŭ���̾�Ʈ ���Ӻ��� ��û
	CASEENUMSTRING(CL_ASK_MOVEGAME)//				// ���� �� ���� ��û
	CASEENUMSTRING(SV_ERROR_MOVEGAME)//			// ���� �� ������� ����
	CASEENUMSTRING(SV_ACCEPT_MOVEGAME)//
	CASEENUMSTRING(SV_ADD_BOMBCARD)//		// ��ź ī�� �߰�
	CASEENUMSTRING(SV_OPEN_BOMBCARD)//		// ��Ź ī�� ����	
	CASEENUMSTRING(SV_SAFEANGELDATA)//		// ��ȣõ�� ���� �Ӵϰ�(�븻)//������)//�������÷���)
	CASEENUMSTRING(SV_GIVE_JOKERCARD)//		// ��Ŀ ī�尪���� ������.(��Ŀ �涡�� �߰�
	CASEENUMSTRING(SV_CHANGE_GAMEUSERINFO)// // ���ӵ��� ���� ���� ����
	CASEENUMSTRING(SV_GIVE_TO_DSDATA)//		// �������� ����Ÿ �����ֱ�	
	//���Ĩ �̺�Ʈ 2006.2.28	
	CASEENUMSTRING(SV_GC_GOLDENDCHIP)//			// ���� ��� �̺�Ʈ ������
	CASEENUMSTRING(SV_ACTIVE_GOLDEVENT_MSG)//				// ��级 �̺�Ʈ ���̶�� �˸�
	CASEENUMSTRING(SV_NIGHTEVENT_MSG)//					// ����Ʈ �̺�Ʈ ���� �˸�	
	CASEENUMSTRING(SV_ACTIVE_NIGHTEVENT_MSG)//			// ����Ʈ �̺�Ʈ ���̶�� �˸�(����Ʈ �̺�Ʈ ���̴ٶ�� �޼���
	//[���ƹ�Ÿ] 2006.07.24
	CASEENUMSTRING(SV_RESULT_BUYLUCKAVATAITEM)//	// �������� ���ƹ�Ÿ�� ���� ������ ��� ��(������������ ������ ����)
	//[��Ŀ �߼� �̺�Ʈ] 2006.08.24
	CASEENUMSTRING(SV_SEND_GAMEBETEVENTINFO)//	//���� �α� ������ �̺�Ʈ ������ ������.
	CASEENUMSTRING(SV_RESULT_GAMEBETEVENTINFO)//	//���� ���� ���� �� ����Ʈ ������ �����ش�.
	CASEENUMSTRING(SV_RUN_GAMEBETEVENTINFO)//    //�̺�Ʈ ���� //��Ŀ è�Ǿ� 2007	
	// �����ڸ���۾�
	CASEENUMSTRING(SV_RESULT_GAMEBETEVENTINFO_ADMIN)//	//��Ŀ è�Ǿ�(������)
	/////////////////// ���� ���� ���� �޼��� /////////
	CASEENUMSTRING(CL_ASK_CHANGEGAMEKIND)//	// ���� ��� �ٲ� ��û(0)������ 1)����)
	CASEENUMSTRING(CL_CHANGEGAMEKIND)//		// ���� ��� �ٲ� ���(0)������ 1)����)
	CASEENUMSTRING(CL_ADD_BOMBCARD_RESULT)//		// ��ź ī�� �߰�	���
	CASEENUMSTRING(CL_OPEN_BOMBCARD_RESULT)//	// ��Ź ī�� ���� ���
	CASEENUMSTRING(CL_CHANGE_IDCOLOR)//		// ���̵� �� ���� ��û
	CASEENUMSTRING(CL_REQ_LOCKROOM)//
	CASEENUMSTRING(CL_ASK_INVITEUSERLIST)//
	CASEENUMSTRING(SV_INVITEUSERLIST)//
	CASEENUMSTRING(SV_HEARTBEATGHOSTCHECK)//			// ��Ʈ��Ʈ ��Ʈüũ��
	CASEENUMSTRING(SV_EMERGENCYCHECK)//					// �ӽ����� ���ξ˸� ��Ŷ
	////////////UPP ���� �޽��� ////////////
	CASEENUMSTRING(CL_REQUEST_PLAYTIMEINFO)//
	CASEENUMSTRING(SV_RESPONSE_PLAYTIMEINFO)//
	CASEENUMSTRING(SV_NOTICE_TOURNAMENT_PLAYTIME)//
	CASEENUMSTRING(SV_UPPTIMEINFO )// 
	CASEENUMSTRING(SV_UPPEVENT )// 
	CASEENUMSTRING(SV_UPPINFO )//  
	CASEENUMSTRING(CL_ASK_UPPMSG )// 
	CASEENUMSTRING(CL_CHECK_DAILY_LOSS_MONEY)//
	CASEENUMSTRING(SV_DAILY_LOSS_MONEY)//
	CASEENUMSTRING(CL_RETENTION_REWARD)//
#if defined(_BETAVER)
	CASEENUMSTRING(CL_REQUEST_BETA_ALLIN_MONEY)//
#endif
	CASEENUMSTRING(CL_DEBUG_SETMONEY)//9999)//					// ����׿� �޼���
	CASEENUMSTRING(SIG_HEAD_COMMONGAME)//1000)//
	//////////////////////////////////////////////////////////////
	//{
	CASEENUMSTRING(SV_STARTCARD)//		// ó�� 3���� ī�带 ����
	CASEENUMSTRING(SV_STARTGAME)//			// ������ ���۵Ǿ���
	CASEENUMSTRING(SV_GAMEOVERRESULT)//	// ���� ����Ǿ����� ����� �뺸(��� ) ���� �� �÷��̾� ���)
	CASEENUMSTRING(SV_GAMEOVER)//			// �������� ������ ������� �뺸(������ ����)
	CASEENUMSTRING(SV_RAISE)//			// ���� �˸� ������
	CASEENUMSTRING(SV_WINDCARDEND)//		// ù 3���� ������ ������ �ϳ��� ī�带 ���� )//> ��� ���ý� ���ӽ���
	CASEENUMSTRING(SV_FOLDUSER)//			// ������ ������
	CASEENUMSTRING(CL_ASK_FOLD)//			// �÷��̾� ����
	CASEENUMSTRING(CL_ASK_RAISE)//		// ������� ������
	CASEENUMSTRING(CL_WINDCARDEND)//		// ������� ī�峯���� ����
	CASEENUMSTRING(SV_SELECTCARD)//		// ī�� Ÿ�̸� ����			
	CASEENUMSTRING(SV_HIDDENCARD)//		// ������ ���� ī�� ����
	CASEENUMSTRING(CL_ASK_SELECTCARD)//	// ������� ī�弱��
	//}
	//////////////////////////////////////////////////////////////
	CASEENUMSTRING(SIG_TAIL_COMMONGAME)
	CASEENUMSTRING(SIG_HEAD_SPOKER)//1100)// // ����
	CASEENUMSTRING(SIG_TAIL_SPOKER)
	CASEENUMSTRING(SIG_HEAD_HIGHLOW)//1200)// // ����
	CASEENUMSTRING(SV_HL_ENDRAISE)//			// ��� ������ �� )//> ���� �ο� ���� ���� ��û	
	CASEENUMSTRING(SV_HL_HLS_SEL)//		// ����)// �ο�)// ���� ��ư Ŭ�������� �˸�	
	CASEENUMSTRING(SV_HL_CARDINFO)//			//ī�� ������ �����ش�.	
	CASEENUMSTRING(CL_HL_SELECTWINCASE)//		// ��ɷ� �ºθ� �ܷ�ڴ°� ���� �ο� ����
	//	CL_HL_ENDWORK)//	//�����ٰ� �޼����� �����ش�.(�������� ��Ÿ)
	CASEENUMSTRING(SIG_TAIL_HIGHLOW)
	CASEENUMSTRING(SIG_HEAD_BADUGI)//1300)// // ����
	CASEENUMSTRING(SV_BD_NOTIFY_CARDEXCHANGE)//		//ī�� ���ýð��̴ٶ�°� Ŭ�󿡰� �˷��ش�.
	CASEENUMSTRING(SV_BD_GIVETOUSER_NEWCHANGECARD)////�ٲٱ� ��û�� ������� ���ο� ī�带 �ش�.		
	CASEENUMSTRING(CL_BD_CARD_EXCHANGE)//       // ī�带 �ٲ۰� �������� �˷��ش�.
	CASEENUMSTRING(CL_BD_CARD_EXCHANGEEND)//	//���ο�ī�带 ���������� �޾Ҵ� ��� ������ �˷��ش�.
	CASEENUMSTRING(SIG_TAIL_BADUGI)
//#if defined(__SUTDA__)
	CASEENUMSTRING(SIG_HEAD_SUTDA)//1400)// // ����
	CASEENUMSTRING(CL_SD_JJOGI)//					// ī�� �ɱ�
	CASEENUMSTRING(SV_SD_NOTIFY_JJOGI)//				// ī�� �ɱ� ����
	CASEENUMSTRING(SV_SD_CHOICE)//					// �������� ���� ���� ��û(3�� ���ٿ����� ����)
	CASEENUMSTRING(CL_SD_ASK_CHOICE)//				// Ŭ���̾�Ʈ���� ���� ����
	CASEENUMSTRING(SV_SD_ACCEPT_CHOICE)//			// �������� ���� ���� ����
	CASEENUMSTRING(SV_SD_RESULTCARD)//				// ���� ��� �� ���� ���� ��û
	CASEENUMSTRING(CL_SD_RESULTCARD)//				// ���� ��� �� ���� ���� �Ϸ�
	CASEENUMSTRING(SV_SD_NOTIFY_REMATCH)//			// 49 ���� ����
	CASEENUMSTRING(SV_SD_49REMATCH)//				// �������� 49 ���� ���� (��Ŷ ������ Ŭ���̾�Ʈ�� ���� ���� �����)
	CASEENUMSTRING(CL_SD_ASK_49REMATCH)//			// Ŭ���̾�Ʈ���� 49 ���� ���� ��û (��Ŷ ������ Ŭ���̾�Ʈ�� ���� ��û)
	CASEENUMSTRING(SV_SD_NOTIFY_ASK)//				// ���� ���� ����
	CASEENUMSTRING(SV_SD_REMATCH_CARDINFO)//			// ���� ī�� ����(���̺� �������׸� ���� ����)
	CASEENUMSTRING(SIG_TAIL_SUTDA)
//#endif	// defined(__SUTDA__)
//#if defined(__HOOLA__)
	CASEENUMSTRING(SIG_HEAD_HOOLA)//1500)// // ����
	// �Ƕ� ���� �۾�
	// �� �Ŵ���
	CASEENUMSTRING(SV_DISCARD)//				// �������� �˸� ī�� ���� ��ũ�� ����
	CASEENUMSTRING(SV_REGRESULT)//			// ����� ī�� ����
	CASEENUMSTRING(SV_STOPGAMERESULT)//		// ������ ��� ��ο���
	CASEENUMSTRING(SV_THANKRESULT)//			// ��ũī�� ���
	CASEENUMSTRING(SV_SPECIALRULERESULT)//   // 7��ī�� ����� ��Ÿ Ư����� �̰��� ��� ����
	CASEENUMSTRING(SV_TURNTHANK)//			// �Ƕ�
	CASEENUMSTRING(SV_TURNGET)//
	CASEENUMSTRING(SV_TURNREG)//
	CASEENUMSTRING(SV_THANK_FAIL)//
	CASEENUMSTRING(SV_CANTHANK)//            // ��ť �� �� ����
	CASEENUMSTRING(SV_ADDEDCARDDATA)//       // �ǿ� �� ī�� ������
	CASEENUMSTRING(SV_FIXREGIST)//
	CASEENUMSTRING(SV_FIXCARDDATA)//
	CASEENUMSTRING(SV_ENABLEGAMESTARTBTN)//
	CASEENUMSTRING(SV_MISSION)//             // ���� ���۽� �̼� �˸�
	CASEENUMSTRING(SV_MISSIONCOMPLETE)//     // �̼� ���� �˸�
	CASEENUMSTRING(SV_MAGICCARD)//
	CASEENUMSTRING(CL_REQGAMESTART)//
	CASEENUMSTRING(CL_REQGAMEPRACTICS)//
	CASEENUMSTRING(CL_ASK_ONECARD)//			// ī�� ��û
	CASEENUMSTRING(CL_BRINGINCARD)//			// ī�������� ������
	CASEENUMSTRING(CL_DISCARD)//				// ���� ������ ī�� ������
	CASEENUMSTRING(CL_ASK_THANK)//			// ������ ��ũ ����
	CASEENUMSTRING(CL_ASK_CONTINUE)//		// ��ũ ���� �ʰ� ��� ����
	CASEENUMSTRING(CL_REGISTER)//			// ������ ��� ��û
	CASEENUMSTRING(CL_ATTACH)//				// ī�� ���̱�
	CASEENUMSTRING(CL_STOPGAME)//			// ����
	CASEENUMSTRING(CL_PRESS_THANK)//         // thank ��ư ����
	CASEENUMSTRING(CL_REGIST)//
	CASEENUMSTRING(CL_ENDCARDMOVE)//
	CASEENUMSTRING(SV_JACKPOTCOUNT)//
	// �� ����
	CASEENUMSTRING(CL_SUN_PICKUP_START)//
	CASEENUMSTRING(CL_SUN_PICKUP)//
	CASEENUMSTRING(CL_SUN_PICKUP_COMPLETE)//
	CASEENUMSTRING(CL_GOLDSTAGE_INFO)//	  // �̼� ����
	CASEENUMSTRING(SIG_TAIL_HOOLA)
//#endif
	CASEENUMSTRING(SIG_HEAD_TOURNAMENT)//1600)// // ����)// 1600 ����� �Ƕ� ���
	// ��ʸ�Ʈ ����Ʈ ��û/����
	CASEENUMSTRING(CL_REQUEST_TOURNAMENT_LIST)//
	CASEENUMSTRING(SV_RESPONSE_TOURNAMENT_LIST)//
	// ��ʸ�Ʈ ���� ��û/����
	CASEENUMSTRING(CL_REQUEST_TOURNAMENT_INFO)//
	CASEENUMSTRING(SV_RESPONSE_TOURNAMENT_INFO)//
	CASEENUMSTRING(SV_NOTIFY_TOURNAMENT_INFO)//
	// Ƽ�� �Ǹ� ����/���� ����
	CASEENUMSTRING(SV_OPEN_DESK)//
	CASEENUMSTRING(SV_CLOSE_DESK)//
	// ��ʸ�Ʈ ����/���� ����
	CASEENUMSTRING(SV_START_TOURNAMENT)//
	CASEENUMSTRING(SV_END_TOURNAMENT)//
	// ������ ����
	CASEENUMSTRING(SV_LEVEL_UP_TOURNAMENT)//
	// Ƽ�� ����
	CASEENUMSTRING(CL_BUY_TOURNAMENT_TICKET)//			// Ƽ�� ���� ��û
	CASEENUMSTRING(SV_RESULT_BUY_TOURNAMENT_TICKET)//	// Ƽ�� ���� ���
	CASEENUMSTRING(SV_NOTIFY_TOURNAMENT_TICKET)//		// ���� �ο� ������ ���� ����
	CASEENUMSTRING(SV_MY_TOURNAMENT_LIST)//				// ������ Ƽ���� ������ ��ʸ�Ʈ ���
	// Ƽ�� ȯ��
	CASEENUMSTRING(CL_REFUND_TOURNAMENT_TICKET)//		// Ƽ�� ȯ�� ��û
	CASEENUMSTRING(SV_RESULT_REFUND_TOURNAMENT_TICKET)//	// Ƽ�� ȯ�� ��û ���
	// ��ʸ�Ʈ ����/����
	CASEENUMSTRING(SV_NOTIFY_REGIST_TOURNAMENT)//
	CASEENUMSTRING(CL_REQUEST_REGIST_TOURNAMENT)//
	CASEENUMSTRING(SV_RESPONSE_REGIST_TOURNAMENT)//
	CASEENUMSTRING(CL_REQUEST_DROP_TOURNAMENT)//
	CASEENUMSTRING(SV_RESPONSE_DROP_TOURNAMENT)//
	// Ŭ�󿡰� �� ����/����/���� ����
	CASEENUMSTRING(SV_CREATE_TOURNAMENT_ROOM)//
	CASEENUMSTRING(SV_ENTER_TOURNAMENT_ROOM)//
	CASEENUMSTRING(SV_LEAVE_TOURNAMENT_ROOM)//
	// ���ٺ���
	CASEENUMSTRING(SV_CHECK_REBUYIN)//		// �������� ���� ���� ��û
	CASEENUMSTRING(CL_ASK_REBUYIN)//		// �������� ����
	CASEENUMSTRING(SV_RESULT_REBUYIN)//		// �������� ��û ���
	CASEENUMSTRING(SV_ELIMINATED)//		// Ż��
	CASEENUMSTRING(SV_NOTIFY_ELIMINATED)//	// Ż�� ����
	// ���� ���
	CASEENUMSTRING(SV_TOURNAMENT_GAME_RESULT)//
	// ��ŷ ��û/����
	CASEENUMSTRING(CL_REQUEST_RANKING)//
	CASEENUMSTRING(SV_RESPONSE_RANKING)//
	CASEENUMSTRING(CL_REQUEST_MY_RANKING)//
	CASEENUMSTRING(SV_RESPONSE_MY_RANKING)//
	// ��ʸ�Ʈ ���� ��Ƽ
	CASEENUMSTRING(SV_NOTIFY_TOURNAMENT)//
	// �޽� �ð�
	CASEENUMSTRING(SV_NOTIFY_START_BREAK)//
	CASEENUMSTRING(SV_NOTIFY_STOP_BREAK)//
	// ���� ����/�簳
	CASEENUMSTRING(SV_STOP_GAME)//
	CASEENUMSTRING(SV_START_GAME)//
	// ���� ��ʸ�Ʈ ���� ����
	CASEENUMSTRING(CL_REQUEST_TOURNAMENT_USER_INFO)//
	CASEENUMSTRING(SV_RESPONSE_TOURNAMENT_USER_INFO)//
	// Ư�� ������ ������� ������ ���� ����
	CASEENUMSTRING(CL_ASK_WATCH_TOURNAMENT_ROOM)//
	CASEENUMSTRING(SV_WATCH_TOURNAMENT_ROOM)//
	// ���� ���� ��ũ �� ��Ÿ ����
	CASEENUMSTRING(CL_REQUEST_CURRENT_USER_RANK)//
	CASEENUMSTRING(SV_RESPONSE_CURRENT_USER_RANK)//
	// �����ޱ� ȯ��
	CASEENUMSTRING(SV_TOURNAMENT_RETURN_UNPAID)//
	CASEENUMSTRING(GS2CL_STARTTOURNAMENT_NOTI)//			// ��ʸ�Ʈ ���� ���� (GS->CL)
	CASEENUMSTRING(GS2CL_ENDTOURNAMENT_NOTI)//			// ��ʸ�Ʈ ���� ���� (GS->CL)
	CASEENUMSTRING(CL2GS_REGISTERUSER_REQ)//				// ��ʸ�Ʈ ���� ��û (CL->GS)
	CASEENUMSTRING(CL2GS_REGISTERUSER_ANS)//				// ��ʸ�Ʈ ���� ��û ��� (GS->CL)
	CASEENUMSTRING(GS2CL_REGISTERUSER_NOTI)//			// ��ʸ�Ʈ ���� ���� (GS->CL)
	CASEENUMSTRING(CL2GS_GETUSERRAT_REQ)//				// ���� ��ʸ�Ʈ ���� ��û (CL->GS))// ���� ������ �α��� ������ �������� �ڵ����� ��û ó��)// Ÿ�� ��ʸ�Ʈ ���� �ʿ�� ��û �ϸ� ��.
	CASEENUMSTRING(CL2GS_GETUSERRAT_ANS)//				// ���� ��ʸ�Ʈ ���� ��� (GS->CL)
	CASEENUMSTRING(GS2CL_UNREGISTERUSER_NOTI)//			// ��ʸ�Ʈ ���� ��� ���� (GS->CL)
	CASEENUMSTRING(GS2CL_CREATEROOM_REQ)//				// �� ���� ��û (GS->CL)
	CASEENUMSTRING(GS2CL_JOINROOM_REQ)//					// �� ���� ��û (GS->CL)
	CASEENUMSTRING(GS2CL_LEAVEROOM_REQ)//				// �� ���� ��û (GS->CL)
	CASEENUMSTRING(GS2CL_SEEDUP_NOTI)//					// �õ�� ���� (GS->CL)
	CASEENUMSTRING(GS2CL_GAMEOVER_NOTI)//				// ���� ��� ���� (GS->CL)
	CASEENUMSTRING(CL2GS_USERRANKING_REQ)//				// ���� ��ŷ ��û (CL->GS)
	CASEENUMSTRING(CL2GS_USERRANKING_ANS)//				// ���� ��ŷ (GS->CL)
	CASEENUMSTRING(CL2GS_TOURNAMENTINFO_REQ)//			// ��ʸ�Ʈ ���� ��û (CL->GS)
	CASEENUMSTRING(CL2GS_TOURNAMENTINFO_ANS)//			// ��ʸ�Ʈ ���� ��û ��� (GS->CL)
	CASEENUMSTRING(CL2GS_TOURNAMENTRESULT_REQ)//			// ���� ��ʸ�Ʈ ���� ��û (CL->GS)
	CASEENUMSTRING(CL2GS_TOURNAMENTRESULT_ANS)//			// ���� ��ʸ�Ʈ ���� ��� (GS->CL)
	CASEENUMSTRING(CL2GS_CHATMSG_REQ)//					// TMS ���� ä��
	CASEENUMSTRING(CL2GS_CHATMSG_NOTI)//					// TMS ���� ä�� ����
	CASEENUMSTRING(GS2CL_UNREGISTER_NOTI)//				// ��ʸ�Ʈ ���� ��� ���� (GS->CL)
	CASEENUMSTRING(GS2CL_SETMONEYINFO_NOTI)//			// �Ӵ� ���� ���� ��û (GS->CL)
	CASEENUMSTRING(GS2CL_SHOWMOVIE_NOTI)//				// ��ʸ�Ʈ ������ ��� ���� (GS->CL)
	CASEENUMSTRING(GS2CL_LEAVEROOM_NOTI)//				// ���� ���� ���� (GS->CL))// ������ UserExitRoom ���� ������ ��� ���۵�.
	CASEENUMSTRING(GS2CL_OPENTOURNAMENT_NOTI)//			// ��ʸ�Ʈ ���� ���� (GS->CL) TS->GS->MS->GS->CL �� ����
	CASEENUMSTRING(GS2CL_UNEARNEDWIN_NOTI)//				// ������ ���� (GS->CL)
	CASEENUMSTRING(CL2GS_UNEARNEDWIN_REQ)//				// ������ �Ӵ� ���� ��û (CL->GS)
	CASEENUMSTRING(CL2GS_UNEARNEDWIN_ANS)//				// ������ �Ӵ� ���� ��� (GS->CL)
	CASEENUMSTRING(GS2CL_RESERVEDUSERPRIZE_NOTI)//		// ���� ������ �Ӵ� ���� ����(GS->CL)
	CASEENUMSTRING(CL2GS_RESERVEDUSERPRIZE_REQ)//		// ���� ������ �Ӵ� ���� ��û (CL->GS)
	CASEENUMSTRING(CL2GS_RESERVEDUSERPRIZE_ANS)//		// ���� ������ �Ӵ� ���� ��� (GS->CL)
	CASEENUMSTRING(GS2CL_CHANGETOURNAMENTSTATE_NOTI)//	// ��ʸ�Ʈ ���º��� ��Ƽ
	CASEENUMSTRING(GS2CL_ELIMINATETOURNAMENT_NOTI)//		// ��ʸ�Ʈ Ż�� ��Ƽ
	CASEENUMSTRING(GS2CL_MATCHWAITING_NOTI)//			// ��ʸ�Ʈ ��ġ ��� ��Ƽ
	CASEENUMSTRING(GS2CL_FINALLASTROOMMATCH_NOTI)//		// ��� ���� �ϳ��� �������� ��Ƽ
	CASEENUMSTRING(GS2CL_TOURNAMENTPROGRESS_NOTI)//		// ����/���� ���� 5����
	//}
	//////////////////////////////////////////////////////////////
	CASEENUMSTRING(SIG_TAIL_TOURNAMENT)
	CASEENUMSTRING(OPCODE_MASTER_SERVER) //= 20000,		// �����ͼ��� ���� �̺�Ʈ
	CASEENUMSTRING(OPCODE_G_TO_M_JP_GAMEFEE)    // ���Ӽ���   -> �����ͼ��� ���Ӻ� ����
	CASEENUMSTRING(OPCODE_M_TO_G_JP_ALLMONEY)			// �����ͼ��� -> ���Ӽ���   �����ݾ׹� ����
	CASEENUMSTRING(OPCODE_M_TO_G_JP_OFFER_MONEY)		// �����ͼ��� -> ���Ӽ���   ��÷�ݾ�
	CASEENUMSTRING(OPCODE_M_TO_G_JP_EVENT_ALRAM)		// �����ͼ��� -> ���Ӽ���   �̺�Ʈ���� �˸�
	CASEENUMSTRING(OPCODE_SV_REGISTER_ALL)// = 25000)     //< CYS 100901 >
	CASEENUMSTRING(OPCODE_SV_JP_INFO)					// ���� ����
	CASEENUMSTRING(OPCODE_SV_JP_WINNER_ALRAM)			// ��÷�� �˸�
	CASEENUMSTRING(OPCODE_SV_JP_EVENTTIME_ROOM)		// �̺�Ʈ ����Ÿ�� Ư�� �� �˸�
	CASEENUMSTRING(OPCODE_SV_JP_EVENTTIME_ALLUSER)		// �̺�Ʈ ����Ÿ�� ��� ���� �˸�
	CASEENUMSTRING(OPCODE_SV_JP_EVENT_GIFTTIME)		// �̺�Ʈ 3�� �̻� ���� ���������� ���� ����
	CASEENUMSTRING(OPCODE_SV_GTIME_START)          // ��ĿŸ��
	CASEENUMSTRING(OPCODE_SV_GTIME_ALRAM)
	CASEENUMSTRING(OPCODE_SV_GTIME_END)
	CASEENUMSTRING(OPCODE_SV_CHAMP_ALRAM)          // �ٵ��� ����ȸ
	CASEENUMSTRING(OPCODE_SV_PCROOMACT_START)		// PC�� Ȱ��ȭ ����
	CASEENUMSTRING(OPCODE_SV_PCROOMACT_INITIALIZE) // PC�� Ȱ��ȭ �ʱ�ȭ
	CASEENUMSTRING(OPCODE_SV_PCROOMACT_ALRAM)		// PC�� Ȱ��ȭ ����
	CASEENUMSTRING(OPCODE_SV_PCROOMACT_END)		// PC�� Ȱ��ȭ ��
	CASEENUMSTRING(OPCODE_SV_PCPZONE_START)		// PC�� ��Ŀ��
	CASEENUMSTRING(OPCODE_SV_PCPZONE_INITIALIZE)	// PC�� ��Ŀ�� �ʱ�ȭ
	CASEENUMSTRING(OPCODE_SV_PCPZONE_ALRAM)		// PC�� ��Ŀ�� ����
	CASEENUMSTRING(OPCODE_SV_PCPZONE_END)			// PC�� ��Ŀ�� ��
	CASEENUMSTRING(OPCODE_SV_PCPZONE_TIMEOUT)		// PC�� 10�а� ����
	CASEENUMSTRING(OPCODE_SV_BETTING_START)		// ��Ŀ�Ӵ� ���� ����
	CASEENUMSTRING(OPCODE_SV_BETTING_INITIALIZE)	// ��Ŀ�Ӵ� ���� �ʱ�ȭ
	CASEENUMSTRING(OPCODE_SV_BETTING_ALRAM)		// ��Ŀ�Ӵ� ���� ����
	CASEENUMSTRING(OPCODE_SV_BETTING_END)			// ��Ŀ�Ӵ� ���� ��
	CASEENUMSTRING(OPCODE_SV_RETENTION_START)		// ���ټ� ���θ�� ����
	CASEENUMSTRING(OPCODE_SV_RETENTION_INITIALIZE)	// ���ټ� ���θ�� �ʱ�ȭ
	CASEENUMSTRING(OPCODE_SV_RETENTION_ALRAM)		// ���ټ� ���θ�� ����
	CASEENUMSTRING(OPCODE_SV_RETENTION_END)		// ���ټ� ���θ�� ��
	CASEENUMSTRING(OPCODE_SV_NEWYEARRETENTION_START)		// ��Ŀ �ų� ���ټ� ���θ�� ����
	CASEENUMSTRING(OPCODE_SV_NEWYEARRETENTION_INITIALIZE)	// ��Ŀ �ų� ���ټ� ���θ�� �ʱ�ȭ
	CASEENUMSTRING(OPCODE_SV_NEWYEARRETENTION_ALRAM)		// ��Ŀ �ų� ���ټ� ���θ�� ����
	CASEENUMSTRING(OPCODE_SV_NEWYEARRETENTION_END)			// ��Ŀ �ų� ���ټ� ���θ�� ��
	CASEENUMSTRING(OPCODE_SV_USERCARE_START)
	CASEENUMSTRING(OPCODE_SV_USERCARE_INITIALIZE)
	CASEENUMSTRING(OPCODE_SV_USERCARE_ALRAM)
	CASEENUMSTRING(OPCODE_SV_USERCARE_END)
	CASEENUMSTRING(OPCODE_SV_GOLDCARD_START)
	CASEENUMSTRING(OPCODE_SV_GOLDCARD_INITIALIZE)
	CASEENUMSTRING(OPCODE_SV_GOLDCARD_ALARM)			// ���θ�� ����
	CASEENUMSTRING(OPCODE_SV_GOLDCARD_APPEAR)			// �����濡�� ���ī�� ����
	CASEENUMSTRING(OPCODE_SV_GOLDCARD_REWARD)			// �̼� �޼����� ���� �Ӵ� ȹ��
	CASEENUMSTRING(OPCODE_SV_GOLDCARD_END)
	CASEENUMSTRING(OPCODE_SV_ADULTCARD_START)
	CASEENUMSTRING(OPCODE_SV_ADULTCARD_INITIALIZE)
	CASEENUMSTRING(OPCODE_SV_ADULTCARD_ALARM)			// ���θ�� ����
	CASEENUMSTRING(OPCODE_SV_ADULTCARD_REWARD)			// �̼� �޼����� ���� �Ӵ� ȹ��
	CASEENUMSTRING(OPCODE_SV_ADULTCARD_END)
	CASEENUMSTRING(OPCODE_SV_MISSION003_START)
	CASEENUMSTRING(OPCODE_SV_MISSION003_INITIALIZE)
	CASEENUMSTRING(OPCODE_SV_MISSION003_ALARM)			// ���θ�� ����
	CASEENUMSTRING(OPCODE_SV_MISSION003_ALLIN)			// �̼� �޼����� ���� �Ӵ� ȹ��
	CASEENUMSTRING(OPCODE_SV_MISSION003_END)
	CASEENUMSTRING(OPCODE_SV_POKERADVENTURE_START)
	CASEENUMSTRING(OPCODE_SV_POKERADVENTURE_INITIALIZE)
	CASEENUMSTRING(OPCODE_SV_POKERADVENTURE_ALARM)
	CASEENUMSTRING(OPCODE_SV_POKERADVENTURE_REWARD)
	CASEENUMSTRING(OPCODE_SV_POKERADVENTURE_ALLIN)	
	CASEENUMSTRING(OPCODE_SV_POKERADVENTURE_END)
	CASEENUMSTRING(OPCODE_SV_HALFJACKPOT_START)
	CASEENUMSTRING(OPCODE_SV_HALFJACKPOT_INITIALIZE)
	CASEENUMSTRING(OPCODE_SV_HALFJACKPOT_TIME_BEFORE_START)	// ���� Ÿ�� ������
	CASEENUMSTRING(OPCODE_SV_HALFJACKPOT_TIME_START)			// ���� Ÿ�� ����
	CASEENUMSTRING(OPCODE_SV_HALFJACKPOT_TIME_BEFORE_END)		// ���� Ÿ�� ������
	CASEENUMSTRING(OPCODE_SV_HALFJACKPOT_TIME_END)				// ���� Ÿ�� ����
	CASEENUMSTRING(OPCODE_SV_HALFJACKPOT_MONEY)				// ���� ���� �Ӵ� �˸�
	CASEENUMSTRING(OPCODE_SV_HALFJACKPOT_ALARM)				// ���� Ÿ�� ���� �� ���� �������� �˸�
	CASEENUMSTRING(OPCODE_SV_HALFJACKPOT_ROOM)					// ���̹� ����
	CASEENUMSTRING(OPCODE_SV_HALFJACKPOT_ENTER_JACKPOT_ROOM)	// ���̹� ����
	CASEENUMSTRING(OPCODE_SV_HALFJACKPOT_PRIZE)				// ������ ����&��÷�� ����
	CASEENUMSTRING(OPCODE_SV_HALFJACKPOT_TOP_HALF_COUNT_MS)	// ������ �ĺ��� ���� MS�� ����
	CASEENUMSTRING(OPCODE_SV_HALFJACKPOT_DELETE_ROOM_MS)		// �ĺ� ���� ������
	CASEENUMSTRING(OPCODE_SV_HALFJACKPOT_END_HALF_MS)			// �ش� ä�� ���� ī���� ����
	CASEENUMSTRING(OPCODE_SV_HALFJACKPOT_JACKPOT_ROUND_START)	// ���̹� ����
	CASEENUMSTRING(OPCODE_SV_HALFJACKPOT_END)
	CASEENUMSTRING(OPCODE_SV_MISSION201304_START)
	CASEENUMSTRING(OPCODE_SV_MISSION201304_INITIALIZE)
	CASEENUMSTRING(OPCODE_SV_MISSION201304_ALARM)			// ���θ�� ����
	CASEENUMSTRING(OPCODE_SV_MISSION201304_REWARD)			// �̼� �޼����� ���� �Ӵ� ȹ��
	CASEENUMSTRING(OPCODE_SV_MISSION201304_END)
	CASEENUMSTRING(OPCODE_SV_BORNIN1996_START)
	CASEENUMSTRING(OPCODE_SV_BORNIN1996_INITIALIZE)
	CASEENUMSTRING(OPCODE_SV_BORNIN1996_ALARM)
	CASEENUMSTRING(OPCODE_SV_BORNIN1996_REWARD)
	CASEENUMSTRING(OPCODE_SV_BORNIN1996_END)
	CASEENUMSTRING(OPCODE_SV_LOTTERY201406_START)
	CASEENUMSTRING(OPCODE_SV_LOTTERY201406_INITIALIZE)
	CASEENUMSTRING(OPCODE_SV_LOTTERY201406_ALARM)			// ���θ�� ����
	CASEENUMSTRING(OPCODE_SV_LOTTERY201406_REWARD)			// �Ǽ� �޼����� ���� ����� ȹ��
	CASEENUMSTRING(OPCODE_SV_LOTTERY201406_END)
	// #if defined(__PROMOTION_CHANCEAVATA__) (2011.07.28 �����ƹ�Ÿ���θ��)
	CASEENUMSTRING(OPCODE_SV_CHANCE_START)
	CASEENUMSTRING(OPCODE_SV_CHANCE_INITIALIZE)
	CASEENUMSTRING(OPCODE_SV_CHANCE_END)
	//--�������θ��
	CASEENUMSTRING(OPCODE_SV_HALF_START)
	CASEENUMSTRING(OPCODE_SV_HALF_ALARM)			// ���θ�� ����
	CASEENUMSTRING(OPCODE_SV_HALF_REWARD)			// �Ǽ� �޼����� ���� ����� ȹ��
	CASEENUMSTRING(OPCODE_SV_HALF_END)
	//--�⼮üũ�̺�Ʈ
	CASEENUMSTRING(OPCODE_SV_VISITE_START)
	CASEENUMSTRING(OPCODE_SV_VISITE_ALARM)			// ���θ�� ����
	CASEENUMSTRING(OPCODE_SV_VISITE_END)
	CASEENUMSTRING(OPCODE_SV_PLAYCOUNT_START)
	CASEENUMSTRING(OPCODE_SV_PLAYCOUNT_ALARM)
	CASEENUMSTRING(OPCODE_SV_PLAYCOUNT_REWARD)
	CASEENUMSTRING(OPCODE_SV_PLAYCOUNT_END)
	CASEENUMSTRING(OPCODE_SV_RETENTION_REWARD)
	CASEENUMSTRING(OPCODE_SV_PLAYCOUPON_ALRAM)
	CASEENUMSTRING(CL_ROUND_MODE_RESERVE)		// ������ ����
	CASEENUMSTRING(CL_ROUND_MODE_CANCEL)		// ������ ���
	CASEENUMSTRING(SV_ROUND_MODE_RESERVE)
	CASEENUMSTRING(SV_ROUND_MODE_CANCEL)		// ������ ���
	CASEENUMSTRING(CL_ROUND_MODE_SEND_SELECT_ENTER_ROOM)	// ������ �� ���� / ��� �˸� �޽���
	CASEENUMSTRING(SV_ROUND_MODE_SEND_SELECT_ENTER_ROOM)
	CASEENUMSTRING(CL_ROUND_MODE_ENTER_ROOM)	// ������ �� ���� 
	CASEENUMSTRING(CL_ROUND_MODE_EXIT_ROOM)
	CASEENUMSTRING(SV_ROUND_MODE_GAME_START_WAIT_TIME) // ���� ������ 3�� ���
	CASEENUMSTRING(SV_ROUND_MODE_GAME_RESULT)  // �Ϲ� ��� ����
	CASEENUMSTRING(SV_ROUND_MODE_GAME_OVER)	// ���� ��� ���� ����� ������ �ݾ� ����
	CASEENUMSTRING(SV_ROUND_MODE_ROUND_COUNT)	// ���� ��� ��ȿ����� �÷��� Ƚ��
	CASEENUMSTRING(SV_ROUND_MODE_BCAST_CHIPS)	// ���� ��忡�� ����ϴ� Ĩ�� �濡 �ִ� �����鿡�� �����ش�.
	CASEENUMSTRING(SV_ROUND_MODE_FOCUS_TIME)

	CASEENUMSTRING(SV_VIP_JACKPOT_BET_POINT)  // ���� �̺�Ʈ�� ���� ���� ����
	CASEENUMSTRING(OPCODE_SV_VIPJACKPOT_START)				// �α��� �� �������� ���θ�� ���� ����
	CASEENUMSTRING(OPCODE_SV_VIPJACKPOT_INITIALIZE)		// �α��� �� �������� ���θ�� ���� ����
	CASEENUMSTRING(OPCODE_SV_VIPJACKPOT_TIME_BEFORE_START)	// ���� Ÿ�� ������(�����ǿ� ���� Ÿ�� ����)
	CASEENUMSTRING(OPCODE_SV_VIPJACKPOT_TIME_START)			// ���� Ÿ�� ����
	CASEENUMSTRING(OPCODE_SV_VIPJACKPOT_TIME_BEFORE_END)	// ���� Ÿ�� ������(�����ǿ� ���� Ÿ�� ����)
	CASEENUMSTRING(OPCODE_SV_VIPJACKPOT_TIME_END)			// ���� Ÿ�� ����
	CASEENUMSTRING(OPCODE_SV_VIPJACKPOT_WAIT_PULL_JACKPOT)	// ��÷�� �̴� ��// ��� 3��
	CASEENUMSTRING(OPCODE_SV_VIPJACKPOT_ALARM)				// ���� Ÿ�� ���� �� ���� �������� �˸�
	CASEENUMSTRING(OPCODE_SV_VIPJACKPOT_TOTAL_EVENT_SUCESS_ROOM_INFO)				// ���̹� ����
	CASEENUMSTRING(OPCODE_SV_VIPJACKPOT_TOTAL_JACKPOT_EVENT_INFO)// ������ ����&��÷�� ����
	CASEENUMSTRING(OPCODE_SV_VIPJACKPOT_BET_POINT)			// ���� ���� ( �� ���� ������ ���� ���� ���� ��� ���۵�)
	CASEENUMSTRING(OPCODE_SV_VIPJACKPOT_PRIZE)              // �ش� ���� ���� ���� ����
	CASEENUMSTRING(OPCODE_SV_VIPJACKPOT_TOP_HALF_COUNT_MS)	// ������ �ĺ��� ���� MS�� ����
	CASEENUMSTRING(OPCODE_SV_VIPJACKPOT_DELETE_ROOM_MS)		// �ĺ� ���� ������
	CASEENUMSTRING(OPCODE_SV_VIPJACKPOT_END_HALF_MS)		// �ش� ä�� ����)Ǯ ī���� ����
	CASEENUMSTRING(OPCODE_SV_VIPJACKPOT_JACKPOT_ROUND_START)// ���̹� ����
	CASEENUMSTRING(OPCODE_SV_VIPJACKPOT_END)
	CASEENUMSTRING(OPCODE_SV_VIPJACKPOT_TOTAL_WAIT_ROOM_INFO)
	CASEENUMSTRING(OPCODE_SV_VIPJACKPOT_BETPOINT_ALARM);
	CASEENUMSTRING(CL_VIP_RANK_INFO)				// Ŭ���û ���� ��ŷ ������û	
	CASEENUMSTRING(SV_VIP_RANK_INFO)				// ���巩ŷ �� ���� ��ŷ ����

	default:
		sStr=NMBASE::UTIL::format("%d", nSignal);
	}
	return sStr;
}