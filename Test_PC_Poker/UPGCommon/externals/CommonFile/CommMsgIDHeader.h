#pragma once


// ������ ������ �޼��� #########################################################

enum
{
	////////////////// ���� ��ġ�� �������� ���ƾ� �� �κ� ////////////////

	MS_CHECKVERSION = 2000,		// ���� üũ

	/////////////////// ���� �������� ��ſ� �޼��� ///////////////////

	MS_ASK_SERVERLOGIN = 2100,	// ���Ӽ��� �α��� ��û
	MS_ACCEPT_SERVERLOGIN,		// ���Ӽ��� �α��� ���
	MS_REFUSE_SERVERLOGIN,		// ���Ӽ��� �α��� �Ұ�
	MS_SERVERSTATUS,			// ���� ���� ���� ���� ����
	MS_MASTERSTATUS,			// ������ ���� ���� ����(�α��� ��� ���� ���� ���� ������)
	MS_CONTROLCMD,				// ���Ӽ��� ���� ���
	MS_STATUSLOG,				// ���� ����͸� �α�(from ���Ӽ���)
	MS_USERINFOLIST,			// ����� ���� ����Ʈ(���� �α�� ������Ʈ��)
	MS_CHANUPDATE,				// ä�� ������Ʈ ����
	MS_USERCHANGECHAN,			// ����� ä�� ���� ����(����, ����, ����)
	MS_DISCONNECTUSER,			// ����� ���� ���� ���
	MS_CHECKUSER,				// ����� ���� üũ ���
	MS_ASK_MOVECHAN,			// ����ڰ� �ٸ� ä�η� �̵� ��û
	MS_RESULT_MOVECHAN,			// ����ڰ� �ٸ� ä�η� �̵� ��û ���
	MS_ASK_FINDUSER,			// ����� ã�� ��û
	MS_RESULT_FINDUSER,			// ����� ã�� ���

	MS_ASK_MOVEGAME,			// �����̵� ���� ��û
	MS_RESULT_MOVEGAME,		// �����̵� ���� ����

	/////////////////// ���� Ŭ���̾�Ʈ���� ��ſ� �޼��� ///////////////////

	MS_ASK_DESTSERVERINFO = 2500,	// ������ ���Ӽ��� ���� ��û
	MS_REFUSE_DESTSERVERINFO,		// ���� �ź�
	MS_DESTSERVERINFO,				// ������ ���Ӽ��� ����

	//����õ�� 2006.3.2/////////////////////////////////////////////////////////////////////////////////////////////
	MS_SEND_GS_CHARGEINOF,	// �����Ͱ� �� �����鿡�� ����õ�� ������ �����ش�
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 	MS_ADD_AUTOUSER,   // �Ƕ� �������� ������ ������ �� ��!!!
// 	MS_DEL_AUTOUSER,

	//[���ƹ�Ÿ] 2006.07.24
	MS_SEND_SUPPLY_LUKCAVATAINFO=3000,	// �����Ͱ� �� �����鿡�� ���ƹ�Ÿ �Ⱦ� ����� ����Ѵ�.
	MS_RESULT_LUCKAVATAIFO,			// ���Ӽ����� �ȾҴٸ� �����Ϳ��� �����ش� � �ȾҴٰ�

	//������ ���ø� 
	MS_ADMIN_COMMAND,		//������ ��ɾ� 

// 	//���̽ý��� 
// 	MS_JACKPOT_BROADCASTSTATE, 		//������ ���¸� �˸���.
// 	MS_JACKPOT_COUNTHALF,			//����->������   ����ī��Ʈ�� �˷��ش�.
// 	MS_JACKPOT_NOMINATION,			//������ -> ���� ���� �ĺ����� �˷��ش�.
// 	MS_JACKPOT_WINROOM,				//������ -> ���� ���� ��÷���� �˷��ش�.
// 	MS_JACKPOT_NOTIFYHALF,			//������ -> ���� -> Ŭ�� �ּ� �ĺ� ���� ī��Ʈ�� �˷��ش�. 	
// 	MS_JACKPOT_ROOM_STATE,			//���� -> Ŭ�� ���̷� State�� �˷��ش�.
// 	MS_JACKPOT_WINJACKPOT,			//���� -> Ŭ�� ���� ��÷
// 	MS_JACKPOT_REPLYJACKPOT,		//���� -> ������  �ش���� ���� �ݾ��� �����Ϳ� �˷��ش�.
// 	MS_JACKPOT_DISPLAY,				//���� -> Ŭ�� ���̵��÷��̿�  �ݾ�
// 	MS_JACKPOT_WINROOM_FOR_LOSER,   //���� -> Ŭ�� ���� ��÷���� �˷��ش�.
// 	MS_JACKPOT_DESTORYROOM,			//���� -> ������ ������ �˷��ش�. 

	/*
	���α� �߰� 2008.05.19
	*/	
	MS_NOTIFY_LOG_CHANNEL_USER,			// �����Ϳ��� ������ ä�κ� ������ ������ �˸�

	// ������Ŀ ���̽ý���
// 	MS_SEND_GAMEFEE_TO_MASTER,
// 	MS_SEND_JACKPOTMONEY_TO_GAMESERVER,
// 	MS_OFFER_JACKPOTMONEY,
// 	MS_SEND_OVERJACKPOTMONEY,

	// 2011�� 1�� ���θ��
	MS_OFFER_RESTING_PROMOTION_MONEY,
	MS_GROUP_INSPECTION_INFO,			// �׷� ���˻��� ����
	MS_GROUP_INSPECTION_ADMININFO,	// �׷����˽� ���尡���� ���� ����
	MS_OPENTOURNAMENT_NOTI,				// ��ʸ�Ʈ ���� ����
};
// ������ �޼��� #########################################################
enum
{
	////////////////// ���� ��ġ�� �������� ���ƾ� �� �κ� ////////////////

	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	SV_CHECKVERSION=10,		// ���� üũ
	SV_RECONNECT,		// �����ؼ� ����
	SV_ASK_LOGIN,		// ���� ��û �α��� ����	// [SSO �۾�] - SV_ASK_LOGIN�� �̸��� �ٲ�
	SV_SERVERINFO,		// ���� ����	[���� ������ �۾�]


	/////////////////// ������ ������ �޼��� ///////////////////
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	SV_ACCEPT_LOGIN,	// �α����� �����
	SV_REFUSE_LOGIN,		// �α����� �ź���(����: ���̵� ����, ��й�ȣ Ʋ��, ������ Ʋ��)
	SV_ASK_LOGOUT,			// �α׾ƿ��� ��û��
	SV_ACCEPT_LOGOUT,		// �α׾ƿ��� �����
	SV_HEARTBEAT,			// ��Ʈ��Ʈ
	SV_DESTCHANNELINFO,		// ������ ä�� ����(�ٸ� ������ ä�� �̵���)
	SV_CONNECT_MOVECHAN,	// �ٸ��������� ä�� �̵����� ���� ��û
	SV_DESTGAMEINFO,		// ������ ä�� ����(�ٸ� ������ ���� �̵���)
	SV_CONNECT_MOVEGAME,	// �ٸ��������� ���� �̵����� ���� ��û
	SV_ASK_DETAILUSERINFO,	// ������� �� ������ �䱸(�����ڿ�)
	SV_DETAILUSERINFO,		// ������� �� ����
	SV_ASK_ENTERCHAN,		// ä�� ������ �䱸
	SV_ACCEPT_ENTERCHAN,	// ä�� ������ �㰡
	SV_ASK_FINDID,			// ���̵� ã�� ��û
	SV_RESULT_FINDID,		// ���̵� ã�� ���
	SV_USERINFO,			// ����� ������ ������(��� : Ư�� ����� ���� �䱸��)
	SV_CHANGEMYINFO,		// ����� ����� ������ �˷���
	SV_ALLUSERINFO,			// ��� ������ ����� �����ش�(��� : �䱸�ϴ� �����)
	SV_NEWUSER,				// ���ο� ����ڰ� ���ӵǾ���(��� : ��ü �����)
	SV_OUTUSER,				// ����ڰ� ���������Ͽ���(��� : ��ü �����)
	SV_CHANGEUSER,			// ������ �ٸ� ������� ������ ����Ǿ���(���: ��ü�����)
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	SV_ACCEPT_CHANGECHAR,	// ����� ĳ���͸� �ٲٱ⸦ ���
	SV_REFUSE_CREATEROOM,	// �� ����⸦ �ź���(���: �� ������ �õ��� �����)
	// ������ ���� ���� �ڵ�(0:�� ���� ���� �� �ʰ�, 1:������ ���̵�)
	SV_ACCEPT_CREATEROOM,	// �� ����� �䱸�� ���� ���� ���������(���: �� ������ �õ��� �����)
	// ����ID, �� ����, �� ����(����, ����)
	SV_ALLROOMINFO,			// ������ ���� ��ü ������ �����ش�(��� : �䱸�ϴ� �����)
	SV_ROOMINFO,			// Ư�� ���� ������ �����ش�(��� : �䱸�ϴ� �����)
	SV_ROOMMASTER,			// Ư���� ���� ���� ������ �����ش�(��� : �䱸�ϴ� �����)
	SV_NEWROOM,				// ���ο� ���� �����Ǿ����� �˸�
	SV_OUTROOM,				// ���� ��������� �˸�
	SV_CHANGEROOM,			// �� ������ �����Ǿ����� �˸�
	SV_USEROUTROOM,			// ����ڰ� �濡�� ���Դ� - ���� �� �÷��̾�鿡�� �뺸
	SV_ACCEPT_OUTROOM,		// �濡�� �����⸦ �㰡
	SV_CHANGEROOMMASTER,	// ������ �ٲ��� �˸���
	SV_ABNORMALOUTROOM,		// ����ڿ��� ������������ �����ϱ⸦ ��û(������� �����ӿ� �������� ���)

	// ### [���� �߰� �۾�] ###
	SV_ASK_CHANGESNDFX,     // ��û
	SV_ACCEPT_CHANGESNDFX,  // ���

	/////////////////// ������ �޴� �޼��� ///////////////////

	SV_ASK_BADUSER,			// ä��â ��ȭ���� �Ű� ��û
	SV_ASK_CHANGEMYINFO,	// ���� ����� ���� ������ �䱸
	SV_ASK_ALLCHANINFO,		// ��ü ä�� ������ �䱸
	SV_ALLCHANINFO,			// ��ü ä�� ����
	SV_ASK_ALLROOMINFO,		// ��ü �� ����� �䱸
	SV_ASK_ROOMINFO,		// Ư�� �������� �䱸
	SV_ASK_ROOMMASTER,		// Ư�� ���� ���� ������ �䱸
	SV_ASK_ALLUSERINFO,		// ��� ������ ����� �䱸
	SV_ASK_USERINFO,		// Ư�� ����� ������ �䱸
	SV_ASK_CHANGECHAR,		// ����� ĳ���͸� �ٲٱ⸦ �䱸
	SV_ASK_CREATEROOM,		// �� ����⸦ �䱸
	SV_ASK_ENTERROOM=555,		// �濡 �����ϱ⸦ �䱸
	SV_ACCEPT_ENTERROOM,	// �濡 �����ϱ⸦ �㰡��(���: ������ �䱸�� �����)
	SV_REFUSE_ENTERROOM,	// �� �����û�� �ź���(���: ������ �䱸�� �����)
	SV_USERENTERROOM,		// ����ڰ� �濡 �����Ͽ���
	SV_LOBYOPEN,			// ���� ���� �ʴ�â ���� ���� �˸�
	SV_ASK_OUTROOM,			// �濡�� �����⸦ �䱸
	SV_ASK_INVITE,			// ��û �޼����� ����
	SV_REFUSE_INVITE,		// ��û �޼����� ����
	SV_ASK_BANISHVOTE,		// �߹� ��û
	SV_MYBANISHVOTE,		// ���� �߹� ��ǥ ���
	SV_NOWHISPER,			// �Ӹ� �ź�,�㰡 ���� ����
	SV_ASK_FORCEBANISH,		// �����߹� ��û	[���� ������ �۾�]
	SV_NOTIFY_BANISHCOUNT,	// �����߹� ī��Ʈ Ŭ���̾�Ʈ�� �˷��ֱ�
	


	/////////////////// ���� ������ �޼��� [���� ������ �۾�] ///////////////////

	SV_ASK_GAMEITEMLIST,	// ���� ������ ��� ������ ��û
	SV_GAMEITEMLIST,		// ���� ������ ����� ������
	SV_USEGAMEITEM,			// ���� ������ ��� ��û
	SV_BUYGAMEITEM,			// ���� �������� �����Ұ����� ���

	/////////////////// ���� �޼��� ///////////////////

	SV_SERVERMSG,			// ���� �޼���
	SV_CHATDATA,			// ���ǿ� ä�� ����Ÿ(����: ������ ������ ��� ���� �޼��� ���)
	SV_WHISPERCHAT,			// �ӼӸ�

	SV_ASK_QUICKSTART,		// ���� �ٷ� ����
	SV_REFUSE_QUICKSTART,	// �ٷν��� �ȵ� (�游���)

	SV_ASK_QUICKSTART2,		// ������ �ٷ� ����
	SV_REFUSE_QUICKSTART2,	//

	SV_ORDER_QUICKSTART_IN_ROOM,	// �ٸ��� �ٷΰ��� Order
	CL_ASK_QUICKSTART_IN_ROOM,		// �ٸ��� �ٷΰ��� ��û
	SV_REFUSE_QUICKSTART_IN_ROOM,	// �ٸ��� �ٷΰ��� ����

	/////////////////// ### [ ������� ] ###  ///////////////////

	SV_ASK_PARTICIPATION,   // ������ ��û
	SV_ACCEPT_PARTICIPATION, // ����
	SV_ENTER_PARTICIPATION,  // �ٸ� ����ڿ��� �˸���
	SV_ASK_EMERAGE,         // ���������� ��������
	SV_ACCEPT_EMERAGE,      // ����
	SV_ENTER_EMERAGE,		// �ٸ� ����ڿ��� �˸���
	SV_RESERVATIONINFO,     // ������ ����
	SV_OBSERVER_MESSAGE,	// ���� ���� �޼���

	/////////////////// ### [������ ��� �۾�] ###  ///////////////////

	NM_ADMIN_WHISPERCHAT,	// �Ӹ��ϱ�(����)
	NM_ADMIN_WARNUSER,		// ����� ����(���)
	NM_ADMIN_OUTUSER,		// ����� �����ϱ�(���)
	NM_ADMIN_CHANGEROOM,	// �׹� �ɼ� �����û(���)
	NM_ADMIN_DELROOM,		// �������ϱ�(���)
	SV_CHANGESTYLEROOMINFO, // ���̸� ���� �˸�(���)
	NM_SPECIALMESSAGE,      // Ư�� ���� �޼���(���)
	NM_ADMIN_SETTINGMAN,	// ������ ���� ȯ�漳�� ���� 


	SV_SERVERNOTIC,			// ���� ���� �޼���
	CL_ASK_CHSTYLEROOMINFO, // ����������	
	CL_CHARGE_MONEY,	//  �Ӵ� ���� �޽���
	

	SV_SPEEDHACK_DEFENSE,	// [���ǵ���] 2004.09.01
	SV_ASK_GAME_CONTINUE,	// [DB��� Process] 2004.11.29
	SV_RES_GAME_CONTINUE,	// [DB��� Process] 2004.11.29
	
	SV_ASK_ENTERRULEROOM,	// [�ڵ� ���÷�] 2005.05.25 - ��� ���� �����

	SV_ASK_BUYITEM,				// ������ �Ķ���� ��û
	SV_ALLOW_BUYITEM,			// 
	SV_BUYITEMPARAM,			// ������ �Ķ���͸� ����
	SV_BUYITEM_RESULT,			// ������ �Ķ���� ��û�� ���� ���
	SV_ASK_CHECK_REWARD,		// ������ ���� ���� üũ
	SV_REWARD_CHECK_RESULT,		// ������ ���� ���� üũ ���
	// reward item db 08.02.27	
	SV_REWARDITEM_LIST,			// DB����� ���������� ������


	/////////////////// ### [���κ���] ###  ///////////////////

	SV_AIN_INSURANCE_JOIN_REQUEST,		// ���κ��� ���� ��û
	SV_AIN_INSURANCE_SUPPLY_REQUEST,	// ���κ��� ���� ��û
	SV_AIN_INSURANCE_JOIN_RESULT,		// ���κ��� ���� ��û ��� �޼���
	SV_AIN_INSURANCE_SUPPLY_RESULT,		// ���κ��� ��� �޼���
	
		
// Ŭ���̾�Ʈ�� �޼��� #########################################################

	/////////////////// ���� �޼��� ///////////////////

	CL_EXITSUBSCRIPT,	// �÷��̾��� ������ ���� ����
	CL_CHATDATA,			// Ŭ���̾�Ʈ���� ä�� �޼���

	
	// [ ���� ������ ]
	CL_ITEMRESULT,			// ������ �����..( �������� ���� ��쿡��.. )
	CL_ITEMLIST,			// ������ ����Ʈ..
	CL_ITEMLOGIN,			// �������� ����� �α���..
	CL_ASK_ITEMLOGIN,		// ������ ����� ���ΰ� ���.
	CL_ACCEPT_ITEMLOGIN,	// ������ ���� �α��� �㰡.

	// [���� ������ �۾�]
	CL_ASK_JUMPITEM,		// ä�ο��� ���������� ��� ���.
	CL_JUMPITEM_LOGIN,		// ä�ο��� ���������� ��� �α�
	CL_ACCEPT_JUMPITEM_CH,  // ���� -> ���� ��û DB OK => �������(������ ���̾�α� �ڽ��� �����ش�)
	CL_ACCEPT_JUMPITEM,		// ä�ο��� ���������� �ѹ��̶� ����� ������ ������(�ٷ� ����)

	SV_LEADERSJOIN,			//2004.05.07 �������Ѿ� ����
	SV_SPECIALCARD,			// ��� ���� �����ڿ��� �˸�, ����

	
	//���ӹ泻 ����޼��� 
	SV_ENDGAME,				// ������ �˸�
	SV_ASKGAMESTART,		// ���ӽ��� ��û
	SV_STARTBTNENABLE,		// ��ưȰ��ȭ(�޼���)

	//
	SV_PREM_OPTION_INFO,		// [������� �۾�] [2009-02-12]
	CL_ASK_RECOVERY_CHARGE,			// [������� �۾�] [2009-02-10]
	CL_RECOVERY_CHARGE_INFO,		// [������� �۾�] [2009-02-11]


	//���ȸ�� 
	CL_REPLY_KISACHARGE,		//���ȸ�� �ɼǻ�ǰ
	SV_NOTIFY_ALLIN,			//����â ����ֱ� 


	//������� �ٷ�����
	CL_ASK_PREMQUICKJOIN,	//������� �ٷ����� ��û
	SV_REFUSE_PREMQUICKJOIN, //������� �ٷ�����  ���ǿ� �ش��ϴ� ���� ���� ��

	SV_SEATKEEPER,			//�ڸ� ��Ŵ�� 

	CL_ASK_WAIT_INROOM,		// ��� ���
	SV_ASK_WAIT_INROOM,

	SV_ROOMOPTIONINFO,		/// �� �ɼ� ���� 

	CL_NOTIFY_MACADDRESS,   //DAT�α� (�������� �ƾ�巹�� �뺸)

	SV_PCROOMEVENT_INFO,		//pc���̺�Ʈ
	SV_YEARENDEVENT_INFO,		//�����̺�Ʈ
	SV_GC_ASK_JOKBO_STEP2,	//���� 2�ܰ� ����Ʈ ��û

	//G_TFT
	SV_CHANGE_MONEY,		//PMoney ��ȭ�� ������ �˷��ִ� �޼���

	SV_NOTIFY_CHANGEOVERMONEY,	// �ʰ���, �����Ӵ� �߻��� �˷���(ä�� �����, ���ӿ�����)
	SV_NOTIFY_CHANGEPREMIUM,	// ȸ���� ���� ���� �˸�

// ���� Ŭ���̾�Ʈ ���Ӻ��� ��û
	CL_ASK_MOVEGAME,				// ���� �� ���� ��û
	SV_ERROR_MOVEGAME,			// ���� �� ������� ����
	SV_ACCEPT_MOVEGAME,

	SV_ADD_BOMBCARD,		// ��ź ī�� �߰�
	SV_OPEN_BOMBCARD,		// ��Ź ī�� ����	

	SV_SAFEANGELDATA,		// ��ȣõ�� ���� �Ӵϰ�(�븻,������,�������÷���)

	SV_GIVE_JOKERCARD,		// ��Ŀ ī�尪���� ������.(��Ŀ �涡�� �߰�


	SV_CHANGE_GAMEUSERINFO, // ���ӵ��� ���� ���� ����

	SV_GIVE_TO_DSDATA,		// �������� ����Ÿ �����ֱ�	


	//���Ĩ �̺�Ʈ 2006.2.28	
	SV_GC_GOLDENDCHIP,			// ���� ��� �̺�Ʈ ������
	SV_ACTIVE_GOLDEVENT_MSG,				// ��级 �̺�Ʈ ���̶�� �˸�

	SV_NIGHTEVENT_MSG,					// ����Ʈ �̺�Ʈ ���� �˸�	
	SV_ACTIVE_NIGHTEVENT_MSG,			// ����Ʈ �̺�Ʈ ���̶�� �˸�(����Ʈ �̺�Ʈ ���̴ٶ�� �޼���



	//[���ƹ�Ÿ] 2006.07.24
	SV_RESULT_BUYLUCKAVATAITEM,	// �������� ���ƹ�Ÿ�� ���� ������ ��� ��(������������ ������ ����)

	//[��Ŀ �߼� �̺�Ʈ] 2006.08.24
	SV_SEND_GAMEBETEVENTINFO,	//���� �α� ������ �̺�Ʈ ������ ������.
	SV_RESULT_GAMEBETEVENTINFO,	//���� ���� ���� �� ����Ʈ ������ �����ش�.
	SV_RUN_GAMEBETEVENTINFO,    //�̺�Ʈ ���� //��Ŀ è�Ǿ� 2007

	// �����ڸ���۾�
	SV_RESULT_GAMEBETEVENTINFO_ADMIN,	//��Ŀ è�Ǿ�(������)

	/////////////////// ���� ���� ���� �޼��� /////////

	CL_ASK_CHANGEGAMEKIND,	// ���� ��� �ٲ� ��û(0:������ 1:����)
	CL_CHANGEGAMEKIND,		// ���� ��� �ٲ� ���(0:������ 1:����)


	CL_ADD_BOMBCARD_RESULT,		// ��ź ī�� �߰�	���
	CL_OPEN_BOMBCARD_RESULT,	// ��Ź ī�� ���� ���

	CL_CHANGE_IDCOLOR,		// ���̵� �� ���� ��û
	CL_REQ_LOCKROOM,
	CL_ASK_INVITEUSERLIST,
	SV_INVITEUSERLIST,
	SV_HEARTBEATGHOSTCHECK,			// ��Ʈ��Ʈ ��Ʈüũ��
	SV_EMERGENCYCHECK,					// �ӽ����� ���ξ˸� ��Ŷ
	////////////UPP ���� �޽��� ////////////
	CL_REQUEST_PLAYTIMEINFO,
	SV_RESPONSE_PLAYTIMEINFO,
	SV_NOTICE_TOURNAMENT_PLAYTIME,
	SV_UPPTIMEINFO , 
	SV_UPPEVENT , 
	SV_UPPINFO ,  
	CL_ASK_UPPMSG , 

	CL_CHECK_DAILY_LOSS_MONEY,
	SV_DAILY_LOSS_MONEY,
	CL_RETENTION_REWARD,		// �������� �̺�Ʈ ��÷ ���

#if defined(_BETAVER)
	CL_REQUEST_BETA_ALLIN_MONEY,
#endif  // defined(_BETAVER)

//#ifdef __ROUND_MODE__	// ���� ����
	CL_ROUND_MODE_RESERVE,		// ������ ����
	CL_ROUND_MODE_CANCEL,		// ������ ���
	SV_ROUND_MODE_RESERVE,
	SV_ROUND_MODE_CANCEL,		// ������ ���
	CL_ROUND_MODE_SEND_SELECT_ENTER_ROOM,	// ������ �� ���� / ��� �˸� �޽���
	SV_ROUND_MODE_SEND_SELECT_ENTER_ROOM,
	CL_ROUND_MODE_ENTER_ROOM,	// ������ �� ���� 
	CL_ROUND_MODE_EXIT_ROOM,
	SV_ROUND_MODE_GAME_START_WAIT_TIME, // ���� ������ 3�� ���
	SV_ROUND_MODE_GAME_RESULT,  // �Ϲ� ��� ����
	SV_ROUND_MODE_GAME_OVER,	// ���� ��� ���� ����� ������ �ݾ� ����
	SV_ROUND_MODE_ROUND_COUNT,	// ���� ��� ��ȿ����� �÷��� Ƚ��
	SV_ROUND_MODE_BCAST_CHIPS,	// ���� ��忡�� ����ϴ� Ĩ�� �濡 �ִ� �����鿡�� �����ش�.
	SV_ROUND_MODE_FOCUS_TIME,   //���� ��� ���� �÷��� �ð�
//#endif
	

	SV_VIP_JACKPOT_BET_POINT,		// ���� �̺�Ʈ�� ���� ���� ����
	CL_VIP_RANK_INFO,				// Ŭ���û ���� ��ŷ ������û	
	SV_VIP_RANK_INFO,				// ���巩ŷ �� ���� ��ŷ ����
	CL_GET_LEAGUE_REWARD_MONEY,		// ���� ������ �Ӵ� ��û
	SV_LEAGUE_REWARD_MONEY,			// ���� ������ ���� ����
	SV_LEAGUE_LOGIN_INFO,			// �α��ν� �����ִ� ������ ����
	SV_LEAGUE_NO_ICON,				// ������ ������ ��Ȱ��ȭ
	CL_LEAGUE_NO_ICON,				// ������ ������ Ȱ��/��Ȱ��
	SV_LEAGUE_BET_POINT,			// �߰��Ǵ� ���� ����Ʈ�� �����Ѵ�.
	SV_LEAGUE_IS_LEAGUE,			// ���� ������

	CL_GET_MY_LEAGUE_INFO,
	CL_SET_OLDLEAGUE_VIEW,		// ���� ���� �°� �� Ȯ��

	SV_NEW_UPPINFO, ////////////////////////////2017.04.08 �ű�UPP �۾�/////////////////////////////
	SV_DAILYLOSSLIMIT_USER_SETTING_INFO, /////////////////////////////�ս��ѵ� �ʰ� ��� �߰� 2017.05.15/////////////////////////////////////////////
	CL_SET_DAILYLOSSLIMIT_INFO, /////////////////////////////�ս��ѵ� �ʰ� ��� �߰� 2017.05.15/////////////////////////////////////////////
	SV_REWARD_DAILYLOSS_OVER, /////////////////////////////�ս��ѵ� �ʰ� ��� �߰� 2017.05.15/////////////////////////////////////////////


	SV_REWARD_ALLIN_ANGEL, ////////////////////////////ä�ι뷱��, ����õ�� 2017.08.10/////////////////////////////////////

	////////////////////////////�Ѱ���, ���뷮Ȯ�� 2017.08.25/////////////////////////////////////
	SV_PLAYINFO_SUPERMOON,
	SV_REWARD_SUPERMOON,
	/////////////////////////////////////////////////

	////////////////////////////���ý���,���ä�� 2017.11.07/////////////////////////////////////
	SV_REWARD_TOTALPOKER_SUPERSCORE,
	SV_REWARD_TOTALPOKER_START,
	////////////////////////////////////////////////

	////////////////////////////����ε����, Ÿ�پ˸� 2017.10.12/////////////////////////////////////
	SV_NOTICE_BIGWIN,
	CL_SEND_CONG_MSG,
	SV_RECEIVE_CONG_MSG,
	///////////////////////

	////////////////////////////���ý���,���ä�� 2017.11.07/////////////////////////////////////
	SV_ETCCHIP_INFO,
	CL_CHANGE_ETCCHIP,
	SV_CHANGE_ETCCHIP_RESULT,
	SV_ETCCHIP_PLAYER_INFO,
	SV_ETCCHIP_EVENT_INFO,
	SV_REWARD_ALLIN_ANGEL_GOLD,
	SV_ETCCHIP_OPEN_REWARD,
	SV_ETCCHIP_CHANGE_INFO,
	SV_ETCCHIP_RAKEBACK_INFO,
	///////////////////////

	////////////////////////////Ŭ���ý��� 2017.12.21/////////////////////////////////////
	SV_CLUB_MYINFO,
	CL_CLUB_CREATE,
	SV_CLUB_CREATE_RESULT,
	CL_CLUB_INVITE,
	SV_CLUB_INVITE_RESULT,
	CL_CLUB_ACCEPT_INVITE,
	SV_CLUB_ACCEPT_INVITE_RESULT,
	CL_CLUB_REQUEST_INVITED_LIST,
	SV_CLUB_RESPONSE_INVITED_LIST,
	CL_CLUB_WITHDRAW,
	SV_CLUB_WITHDRAW_RESULT,
	CL_CLUB_MEMBER_FIRE,
	SV_CLUB_MEMBER_FIRE_RESULT,
	SV_CLUB_MEMBER_FIRE_NOTIFY,
	CL_CLUB_REQUEST_DETAIL_INFO,
	SV_CLUB_RESPONSE_DETAIL_INFO,
	CL_CLUB_JOIN,
	SV_CLUB_JOIN_RESULT,
	CL_CLUB_ACCEPT_JOIN,
	SV_CLUB_ACCEPT_JOIN_RESULT,
	CL_CLUB_REQUEST_JOIN_LIST,
	SV_CLUB_RESPONSE_JOIN_LIST,
	CL_CLUB_REQUEST_ALL_LIST,
	SV_CLUB_RESPONSE_ALL_LIST,
	CL_CLUB_REQUEST_MYSELF_INVITED_LIST,
	SV_CLUB_RESPONSE_MYSELF_INVITED_LIST,
	CL_CLUB_REQUEST_MYSELF_JOIN_LIST,
	SV_CLUB_RESPONSE_MYSELF_JOIN_LIST,
	SV_TOTAL_REQUEST_FAIL_RESULT,
	//////////////////////

	////////////////////////////2018�����̺�Ʈ 2018.01.22/////////////////////////////////////
	SV_PROMOTION_COMMON_GAMECOUNT_USERINFO,
	SV_PROMOTION_COMMON_GAMECOUNT_GET_REWARD,
	SV_PROMOTION_COMMON_ATTENDANCE_USERINFO,
	CL_PROMOTION_COMMON_REQUEST_TICKET_GIFTLIST,
	SV_PROMOTION_COMMON_RESPONSE_TICKET_GIFTLIST,
	CL_PROMOTION_COMMON_TICKET_REQUEST_SUBSCRIPTION_LIST,
	SV_PROMOTION_COMMON_TICKET_RESPONSE_SUBSCRIPTION_LIST,
	CL_PROMOTION_COMMON_TICKET_REQUEST_SUBSCRIPTION,
	SV_PROMOTION_COMMON_TICKET_RESPONSE_SUBSCRIPTION,
	SV_PROMOTION_COMMON_ATTENDANCE_GET_REWARD,
	SV_PROMOTION_COMMON_GAMECOUNT_RUNNING_INFO,
	SV_PROMOTION_COMMON_REWARD_ALL_NOTICE,
	/////////////////////////

	////////////////////////////������� ������� 2018.02.20/////////////////////////////////////
	SV_GOLD_TEMPMONEY_INFO,
	SV_GOLD_TEMPMONEY_REAL_UPDATE,
	/////////////////////////

	////////////////////////////���� �������, ��������� 2018.02.27/////////////////////////////////////
	SV_GOLD_HELP_REWARD,
	SV_GOLD_HELP_REWARD_INFO,
	////////////////////////////

	////////////////////////////Ȧ¦�̴ϰ��� �̺�Ʈ 2018.03.12/////////////////////////////////////
	SV_PM_ODDEVEN_START_INFO,
	CL_PM_ODDEVEN_REQUEST_START,
	SV_PM_ODDEVEN_RESPONSE_START,
	SV_PM_ODDEVEN_START_ROUND,
	CL_PM_ODDEVEN_USER_BET,
	SV_PM_ODDEVEN_USER_BET_RESULT,
	SV_PM_ODDEVEN_FIX_REWARD,
	CL_PM_ODDEVEN_GAME_GIVEUP,
	SV_PM_ODDEVEN_WINNER_NOTI,
	////////////////////////////

	////////////////////////////�� �̴ϰ��� �̺�Ʈ 2019.07.30/////////////////////////////////////
	SV_PM_WHEEL_START_INFO,
	CL_PM_WHEEL_REQUEST_START,
	SV_PM_WHEEL_RESPONSE_START,
	SV_PM_WHEEL_START_ROUND,
	CL_PM_WHEEL_USER_BET,
	SV_PM_WHEEL_USER_BET_RESULT,
	SV_PM_WHEEL_FIX_REWARD,
	// CL_PM_WHEEL_GAME_GIVEUP,
	SV_PM_WHEEL_WINNER_NOTI,
	////////////////////////////

	////////////////////////////�̴ϰ��� ���� 2018.06.04/////////////////////////////////////
	CL_PM_BIGWHEEL_GET_REMAIN_GAMECOUNT,		// ���� ���� Ƚ�� ��û
	SV_PM_BIGWHEEL_GET_REMAIN_GAMECOUNT,		// ���� ���� Ƚ�� ����
	CL_PM_BIGWHEEL_PLAY_GAME,					// ���� �÷��� ��û
	SV_PM_BIGWHEEL_PLAY_GAME,					// ���� �÷��� ����/���
	////////////////////////////


	////////////////////////////2018��06�� ���� 2018.06.01/////////////////////////////////////
	CL_GOLDBANK_WITHDRAW,
	SV_GOLDBANK_WITHDRAW_RESULT,
	SV_ETCCHIP_INSURANCE_USER_INFO,
	SV_ETCCHIP_INSURANCE_USER_CHANGE_INFO,
	////////////////////////////

	////////////////////////////2018��07�� 2��° ������Ʈ/////////////////////////////////////
	CL_ASK_CARDOPEN,			// ī�� ���� ��û
	SV_ASK_CARDOPEN_RESULT,		// ī�� ���� ��û ���
	////////////////////////////

	// �Ű��ϱ� ��� �޽���. 2018.07. 2��° ������Ʈ
	SV_REPORT_RIGUSER_RESULT,

	//////////////////////////////////////////////////////////////////////////
	// ī�� �ý���. 2018.12.20
	CL_CAFE_MEMBERLIST,		// ��� ����Ʈ ��û
	SV_CAFE_MEMBERLIST,
	CL_CAFE_ASK_JOIN,		// ����
	SV_CAFE_ASK_JOIN,
	CL_CAFE_INVITE,			// �ʴ�
	SV_CAFE_INVITE,
	SV_CAFE_RESERVED_POINT,	// ���� ����Ʈ
	SV_CAFE_NOTICE,			// ����. VIP ��� ��� �� �϶��� ���� ��Ƽ.
	//////////////////////////////////////////////////////////////////////////
	
	////////////////////////////2019 �Ѱ��� �̺�Ʈ 2019.09.05/////////////////////////////////////
	SV_PM_2019HAVEST_INFO,
	CL_PM_2019HAVEST_REQUEST_REWARD,
	SV_PM_2019HAVEST_REQUEST_RESULT,
	////////////////////////////

	CL_DEBUG_SETMONEY=9999,					// ����׿� �޼���
};

///////////////////  ���� ���� ���� �޼��� ////////////////////
//////////////////////////////////////////////////////////////
enum SIGNAL_COMMONGAME{SIG_HEAD_COMMONGAME=1000,
//////////////////////////////////////////////////////////////
//{
	SV_STARTCARD,		// ó�� 3���� ī�带 ����
	SV_STARTGAME,			// ������ ���۵Ǿ���
	SV_GAMEOVERRESULT,	// ���� ����Ǿ����� ����� �뺸(��� : ���� �� �÷��̾� ���)
	SV_GAMEOVER,			// �������� ������ ������� �뺸(������ ����)
	SV_RAISE,			// ���� �˸� ������
	SV_WINDCARDEND,		// ù 3���� ������ ������ �ϳ��� ī�带 ���� => ��� ���ý� ���ӽ���
	SV_FOLDUSER,			// ������ ������

	CL_ASK_FOLD,			// �÷��̾� ����
	CL_ASK_RAISE,		// ������� ������
	CL_WINDCARDEND,		// ������� ī�峯���� ����

	SV_SELECTCARD,		// ī�� Ÿ�̸� ����			
	SV_HIDDENCARD,		// ������ ���� ī�� ����
	CL_ASK_SELECTCARD,	// ������� ī�弱��

	SV_BLINDSHOW, //�ο�ٵ��� �����Ʈ ������ ������, �� �и� �����Ѵ�.

//}
//////////////////////////////////////////////////////////////
SIG_TAIL_COMMONGAME};				//������Ŀ 2 ���� �޼��� ��
//////////////////////////////////////////////////////////////	
//////////////////////////////////////////////////////////////


/////////////////// ������Ŀ2 ���� �޼��� ////////////////////
//////////////////////////////////////////////////////////////
enum SIGNAL_SPOKER{SIG_HEAD_SPOKER=1100, // ����
//////////////////////////////////////////////////////////////
//{

	
//}
//////////////////////////////////////////////////////////////
SIG_TAIL_SPOKER};				//������Ŀ 2 ���� �޼��� ��
//////////////////////////////////////////////////////////////	
//////////////////////////////////////////////////////////////


/////////////////// ���̷ο� ���� �޼��� ////////////////////
//////////////////////////////////////////////////////////////
enum SIGNAL_HIGHLOW{SIG_HEAD_HIGHLOW=1200, // ����
//////////////////////////////////////////////////////////////
//{

	SV_HL_ENDRAISE,			// ��� ������ �� => ���� �ο� ���� ���� ��û	
	SV_HL_HLS_SEL,		// ����, �ο�, ���� ��ư Ŭ�������� �˸�	
	SV_HL_CARDINFO,			//ī�� ������ �����ش�.	
	
	CL_HL_SELECTWINCASE,		// ��ɷ� �ºθ� �ܷ�ڴ°� ���� �ο� ����
//	CL_HL_ENDWORK,	//�����ٰ� �޼����� �����ش�.(�������� ��Ÿ)

//}
//////////////////////////////////////////////////////////////
SIG_TAIL_HIGHLOW};				//��
//////////////////////////////////////////////////////////////	
//////////////////////////////////////////////////////////////


/////////////////// �ٵ��� ���� �޼��� ////////////////////
//////////////////////////////////////////////////////////////
enum SIGNAL_BADUGI{SIG_HEAD_BADUGI=1300, // ����
//////////////////////////////////////////////////////////////
//{

	SV_BD_NOTIFY_CARDEXCHANGE,		//ī�� ���ýð��̴ٶ�°� Ŭ�󿡰� �˷��ش�.
	SV_BD_GIVETOUSER_NEWCHANGECARD,//�ٲٱ� ��û�� ������� ���ο� ī�带 �ش�.		
	
	CL_BD_CARD_EXCHANGE,       // ī�带 �ٲ۰� �������� �˷��ش�.
	CL_BD_CARD_EXCHANGEEND,	//���ο�ī�带 ���������� �޾Ҵ� ��� ������ �˷��ش�.

	SV_REPORT_BANISH_MORNINGBET,	// ��׹� �� ��� ����

//}
//////////////////////////////////////////////////////////////
SIG_TAIL_BADUGI};				//��
//////////////////////////////////////////////////////////////	
//////////////////////////////////////////////////////////////


//#if defined(__SUTDA__)
/////////////////// ���� ���� �޼��� /////////////////////////
//////////////////////////////////////////////////////////////
enum SIGNAL_SUTDA{SIG_HEAD_SUTDA=1400, // ����
//////////////////////////////////////////////////////////////
//{
CL_SD_JJOGI,					// ī�� �ɱ�
SV_SD_NOTIFY_JJOGI,				// ī�� �ɱ� ����

SV_SD_CHOICE,					// �������� ���� ���� ��û(3�� ���ٿ����� ����)
CL_SD_ASK_CHOICE,				// Ŭ���̾�Ʈ���� ���� ����
SV_SD_ACCEPT_CHOICE,			// �������� ���� ���� ����

SV_SD_RESULTCARD,				// ���� ��� �� ���� ���� ��û
CL_SD_RESULTCARD,				// ���� ��� �� ���� ���� �Ϸ�

SV_SD_NOTIFY_REMATCH,			// 49 ���� ����
SV_SD_49REMATCH,				// �������� 49 ���� ���� (��Ŷ ������ Ŭ���̾�Ʈ�� ���� ���� �����)
CL_SD_ASK_49REMATCH,			// Ŭ���̾�Ʈ���� 49 ���� ���� ��û (��Ŷ ������ Ŭ���̾�Ʈ�� ���� ��û)
SV_SD_NOTIFY_ASK,				// ���� ���� ����


SV_SD_REMATCH_CARDINFO,			// ���� ī�� ����(���̺� �������׸� ���� ����)
//}
//////////////////////////////////////////////////////////////
SIG_TAIL_SUTDA};				//��
//////////////////////////////////////////////////////////////	
//////////////////////////////////////////////////////////////
//#endif	// defined(__SUTDA__)


//#if defined(__HOOLA__)
/////////////////// ���� ���� �޼��� /////////////////////////
enum SIGNAL_HOOLA {SIG_HEAD_HOOLA=1500, // ����

// �Ƕ� ���� �۾�
// �� �Ŵ���
SV_DISCARD,				// �������� �˸� ī�� ���� ��ũ�� ����
SV_REGRESULT,			// ����� ī�� ����
SV_STOPGAMERESULT,		// ������ ��� ��ο���
SV_THANKRESULT,			// ��ũī�� ���
SV_SPECIALRULERESULT,   // 7��ī�� ����� ��Ÿ Ư����� �̰��� ��� ����

SV_TURNTHANK,			// �Ƕ�
SV_TURNGET,
SV_TURNREG,
SV_THANK_FAIL,

SV_CANTHANK,            // ��ť �� �� ����
SV_ADDEDCARDDATA,       // �ǿ� �� ī�� ������
SV_FIXREGIST,
SV_FIXCARDDATA,
SV_ENABLEGAMESTARTBTN,

SV_MISSION,             // ���� ���۽� �̼� �˸�
SV_MISSIONCOMPLETE,     // �̼� ���� �˸�

SV_MAGICCARD,


CL_REQGAMESTART,
CL_REQGAMEPRACTICS,
CL_ASK_ONECARD,			// ī�� ��û
CL_BRINGINCARD,			// ī�������� ������

CL_DISCARD,				// ���� ������ ī�� ������
CL_ASK_THANK,			// ������ ��ũ ����
CL_ASK_CONTINUE,		// ��ũ ���� �ʰ� ��� ����
CL_REGISTER,			// ������ ��� ��û
CL_ATTACH,				// ī�� ���̱�
CL_STOPGAME,			// ����

CL_PRESS_THANK,         // thank ��ư ����
CL_REGIST,
CL_ENDCARDMOVE,
SV_JACKPOTCOUNT,

// �� ����
CL_SUN_PICKUP_START,
CL_SUN_PICKUP,
CL_SUN_PICKUP_COMPLETE,

CL_GOLDSTAGE_INFO,	  // �̼� ����

////////////////////////////�Ƕ� ��ǹ� 2018.03.29/////////////////////////////////////
CL_PRESS_GIVEUP,
SV_GIVEUP_RULE_RESULT,
SV_GIVEUP_RULE_NOTI,
SV_GIVEUP_LOSE_MONEY_NOTI,
/////////////////////////////////////////////

SIG_TAIL_HOOLA};				//��
//////////////////////////////////////////////////////////////
//#endif


/////////////////// ��ʸ�Ʈ �޼��� /////////////////////////
//////////////////////////////////////////////////////////////
enum SIGNAL_TOURNAMENT{SIG_HEAD_TOURNAMENT=1600, // ����, 1600 ����� �Ƕ� ���
//////////////////////////////////////////////////////////////
//{

// ��ʸ�Ʈ ����Ʈ ��û/����
CL_REQUEST_TOURNAMENT_LIST,
SV_RESPONSE_TOURNAMENT_LIST,

// ��ʸ�Ʈ ���� ��û/����
CL_REQUEST_TOURNAMENT_INFO,
SV_RESPONSE_TOURNAMENT_INFO,
SV_NOTIFY_TOURNAMENT_INFO,

// Ƽ�� �Ǹ� ����/���� ����
SV_OPEN_DESK,
SV_CLOSE_DESK,

// ��ʸ�Ʈ ����/���� ����
SV_START_TOURNAMENT,
SV_END_TOURNAMENT,

// ������ ����
SV_LEVEL_UP_TOURNAMENT,

// Ƽ�� ����
CL_BUY_TOURNAMENT_TICKET,			// Ƽ�� ���� ��û
SV_RESULT_BUY_TOURNAMENT_TICKET,	// Ƽ�� ���� ���
SV_NOTIFY_TOURNAMENT_TICKET,		// ���� �ο� ������ ���� ����
SV_MY_TOURNAMENT_LIST,				// ������ Ƽ���� ������ ��ʸ�Ʈ ���

// Ƽ�� ȯ��
CL_REFUND_TOURNAMENT_TICKET,		// Ƽ�� ȯ�� ��û
SV_RESULT_REFUND_TOURNAMENT_TICKET,	// Ƽ�� ȯ�� ��û ���

// ��ʸ�Ʈ ����/����
SV_NOTIFY_REGIST_TOURNAMENT,
CL_REQUEST_REGIST_TOURNAMENT,
SV_RESPONSE_REGIST_TOURNAMENT,
CL_REQUEST_DROP_TOURNAMENT,
SV_RESPONSE_DROP_TOURNAMENT,

// Ŭ�󿡰� �� ����/����/���� ����
SV_CREATE_TOURNAMENT_ROOM,
SV_ENTER_TOURNAMENT_ROOM,
SV_LEAVE_TOURNAMENT_ROOM,

// ���ٺ���
SV_CHECK_REBUYIN,		// �������� ���� ���� ��û
CL_ASK_REBUYIN,		// �������� ����
SV_RESULT_REBUYIN,		// �������� ��û ���
SV_ELIMINATED,		// Ż��
SV_NOTIFY_ELIMINATED,	// Ż�� ����

// ���� ���
SV_TOURNAMENT_GAME_RESULT,

// ��ŷ ��û/����
CL_REQUEST_RANKING,
SV_RESPONSE_RANKING,
CL_REQUEST_MY_RANKING,
SV_RESPONSE_MY_RANKING,

// ��ʸ�Ʈ ���� ��Ƽ
SV_NOTIFY_TOURNAMENT,

// �޽� �ð�
SV_NOTIFY_START_BREAK,
SV_NOTIFY_STOP_BREAK,

// ���� ����/�簳
SV_STOP_GAME,
SV_START_GAME,

// ���� ��ʸ�Ʈ ���� ����
CL_REQUEST_TOURNAMENT_USER_INFO,
SV_RESPONSE_TOURNAMENT_USER_INFO,

// Ư�� ������ ������� ������ ���� ����
CL_ASK_WATCH_TOURNAMENT_ROOM,
SV_WATCH_TOURNAMENT_ROOM,

// ���� ���� ��ũ �� ��Ÿ ����
CL_REQUEST_CURRENT_USER_RANK,
SV_RESPONSE_CURRENT_USER_RANK,

// �����ޱ� ȯ��
SV_TOURNAMENT_RETURN_UNPAID,



GS2CL_STARTTOURNAMENT_NOTI,			// ��ʸ�Ʈ ���� ���� (GS->CL)
GS2CL_ENDTOURNAMENT_NOTI,			// ��ʸ�Ʈ ���� ���� (GS->CL)
CL2GS_REGISTERUSER_REQ,				// ��ʸ�Ʈ ���� ��û (CL->GS)
CL2GS_REGISTERUSER_ANS,				// ��ʸ�Ʈ ���� ��û ��� (GS->CL)
GS2CL_REGISTERUSER_NOTI,			// ��ʸ�Ʈ ���� ���� (GS->CL)
CL2GS_GETUSERRAT_REQ,				// ���� ��ʸ�Ʈ ���� ��û (CL->GS), ���� ������ �α��� ������ �������� �ڵ����� ��û ó��, Ÿ�� ��ʸ�Ʈ ���� �ʿ�� ��û �ϸ� ��.
CL2GS_GETUSERRAT_ANS,				// ���� ��ʸ�Ʈ ���� ��� (GS->CL)
GS2CL_UNREGISTERUSER_NOTI,			// ��ʸ�Ʈ ���� ��� ���� (GS->CL)
GS2CL_CREATEROOM_REQ,				// �� ���� ��û (GS->CL)
GS2CL_JOINROOM_REQ,					// �� ���� ��û (GS->CL)
GS2CL_LEAVEROOM_REQ,				// �� ���� ��û (GS->CL)
GS2CL_SEEDUP_NOTI,					// �õ�� ���� (GS->CL)
GS2CL_GAMEOVER_NOTI,				// ���� ��� ���� (GS->CL)
CL2GS_USERRANKING_REQ,				// ���� ��ŷ ��û (CL->GS)
CL2GS_USERRANKING_ANS,				// ���� ��ŷ (GS->CL)
CL2GS_TOURNAMENTINFO_REQ,			// ��ʸ�Ʈ ���� ��û (CL->GS)
CL2GS_TOURNAMENTINFO_ANS,			// ��ʸ�Ʈ ���� ��û ��� (GS->CL)
CL2GS_TOURNAMENTRESULT_REQ,			// ���� ��ʸ�Ʈ ���� ��û (CL->GS)
CL2GS_TOURNAMENTRESULT_ANS,			// ���� ��ʸ�Ʈ ���� ��� (GS->CL)
CL2GS_CHATMSG_REQ,					// TMS ���� ä��
CL2GS_CHATMSG_NOTI,					// TMS ���� ä�� ����
GS2CL_UNREGISTER_NOTI,				// ��ʸ�Ʈ ���� ��� ���� (GS->CL)
GS2CL_SETMONEYINFO_NOTI,			// �Ӵ� ���� ���� ��û (GS->CL)
GS2CL_SHOWMOVIE_NOTI,				// ��ʸ�Ʈ ������ ��� ���� (GS->CL)
GS2CL_LEAVEROOM_NOTI,				// ���� ���� ���� (GS->CL), ������ UserExitRoom ���� ������ ��� ���۵�.
GS2CL_OPENTOURNAMENT_NOTI,			// ��ʸ�Ʈ ���� ���� (GS->CL) TS->GS->MS->GS->CL �� ����
GS2CL_UNEARNEDWIN_NOTI,				// ������ ���� (GS->CL)
CL2GS_UNEARNEDWIN_REQ,				// ������ �Ӵ� ���� ��û (CL->GS)
CL2GS_UNEARNEDWIN_ANS,				// ������ �Ӵ� ���� ��� (GS->CL)
GS2CL_RESERVEDUSERPRIZE_NOTI,		// ���� ������ �Ӵ� ���� ����(GS->CL)
CL2GS_RESERVEDUSERPRIZE_REQ,		// ���� ������ �Ӵ� ���� ��û (CL->GS)
CL2GS_RESERVEDUSERPRIZE_ANS,		// ���� ������ �Ӵ� ���� ��� (GS->CL)
GS2CL_CHANGETOURNAMENTSTATE_NOTI,	// ��ʸ�Ʈ ���º��� ��Ƽ
GS2CL_ELIMINATETOURNAMENT_NOTI,		// ��ʸ�Ʈ Ż�� ��Ƽ
GS2CL_MATCHWAITING_NOTI,			// ��ʸ�Ʈ ��ġ ��� ��Ƽ
GS2CL_FINALLASTROOMMATCH_NOTI,		// ��� ���� �ϳ��� �������� ��Ƽ
GS2CL_TOURNAMENTPROGRESS_NOTI,		// ����/���� ���� 5����
//}
//////////////////////////////////////////////////////////////
SIG_TAIL_TOURNAMENT};				//��
//////////////////////////////////////////////////////////////	
//////////////////////////////////////////////////////////////

