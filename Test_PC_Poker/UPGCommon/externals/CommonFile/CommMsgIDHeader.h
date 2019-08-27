#pragma once


// 마스터 서버용 메세지 #########################################################

enum
{
	////////////////// 값과 위치를 변경하지 말아야 할 부분 ////////////////

	MS_CHECKVERSION = 2000,		// 버전 체크

	/////////////////// 게임 서버와의 통신용 메세지 ///////////////////

	MS_ASK_SERVERLOGIN = 2100,	// 게임서버 로그인 요청
	MS_ACCEPT_SERVERLOGIN,		// 게임서버 로그인 허용
	MS_REFUSE_SERVERLOGIN,		// 게임서버 로그인 불가
	MS_SERVERSTATUS,			// 게임 서버 상태 변경 정보
	MS_MASTERSTATUS,			// 마스터 서버 상태 정보(로그인 허용 정보 보다 먼저 보내짐)
	MS_CONTROLCMD,				// 게임서버 제어 명령
	MS_STATUSLOG,				// 상태 모니터링 로그(from 게임서버)
	MS_USERINFOLIST,			// 사용자 정보 리스트(최초 로긴시 업데이트용)
	MS_CHANUPDATE,				// 채널 업데이트 정보
	MS_USERCHANGECHAN,			// 사용자 채널 변경 정보(입장, 변경, 퇴장)
	MS_DISCONNECTUSER,			// 사용자 강제 종료 명령
	MS_CHECKUSER,				// 사용자 접속 체크 명령
	MS_ASK_MOVECHAN,			// 사용자가 다른 채널로 이동 요청
	MS_RESULT_MOVECHAN,			// 사용자가 다른 채널로 이동 요청 결과
	MS_ASK_FINDUSER,			// 사용자 찾기 요청
	MS_RESULT_FINDUSER,			// 사용자 찾기 결과

	MS_ASK_MOVEGAME,			// 게임이동 서버 요청
	MS_RESULT_MOVEGAME,		// 게임이동 서버 응답

	/////////////////// 게임 클라이언트와의 통신용 메세지 ///////////////////

	MS_ASK_DESTSERVERINFO = 2500,	// 접속할 게임서버 정보 요청
	MS_REFUSE_DESTSERVERINFO,		// 접속 거부
	MS_DESTSERVERINFO,				// 접속할 게임서버 정보

	//올인천사 2006.3.2/////////////////////////////////////////////////////////////////////////////////////////////
	MS_SEND_GS_CHARGEINOF,	// 마스터가 각 서버들에게 올인천사 정보를 보내준다
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 	MS_ADD_AUTOUSER,   // 훌라 오토유저 마스터 서버에 왜 또!!!
// 	MS_DEL_AUTOUSER,

	//[행운아바타] 2006.07.24
	MS_SEND_SUPPLY_LUKCAVATAINFO=3000,	// 마스터가 각 서버들에게 행운아바타 팔아 오라고 명령한다.
	MS_RESULT_LUCKAVATAIFO,			// 게임서버가 팔았다면 마스터에게 보내준다 몇개 팔았다고

	//관리자 세팅맨 
	MS_ADMIN_COMMAND,		//마스터 명령어 

// 	//잭팟시스템 
// 	MS_JACKPOT_BROADCASTSTATE, 		//현재의 상태를 알린다.
// 	MS_JACKPOT_COUNTHALF,			//서버->마스터   하프카운트를 알려준다.
// 	MS_JACKPOT_NOMINATION,			//마스터 -> 서버 잭팟 후보방을 알려준다.
// 	MS_JACKPOT_WINROOM,				//마스터 -> 서버 잭팟 당첨방을 알려준다.
// 	MS_JACKPOT_NOTIFYHALF,			//마스터 -> 서버 -> 클라 최소 후보 하프 카운트를 알려준다. 	
// 	MS_JACKPOT_ROOM_STATE,			//서버 -> 클라 잭팟룸 State를 알려준다.
// 	MS_JACKPOT_WINJACKPOT,			//서버 -> 클라 잭팟 당첨
// 	MS_JACKPOT_REPLYJACKPOT,		//서버 -> 마스터  해당방의 잭팟 금액을 마스터에 알려준다.
// 	MS_JACKPOT_DISPLAY,				//서버 -> 클라 잭팟디스플레이용  금액
// 	MS_JACKPOT_WINROOM_FOR_LOSER,   //서버 -> 클라 잭팟 당첨방을 알려준다.
// 	MS_JACKPOT_DESTORYROOM,			//서버 -> 마스터 방폭을 알려준다. 

	/*
	통계로그 추가 2008.05.19
	*/	
	MS_NOTIFY_LOG_CHANNEL_USER,			// 마스터에서 서버로 채널별 유저를 남기라고 알림

	// 통합포커 잭팟시스템
// 	MS_SEND_GAMEFEE_TO_MASTER,
// 	MS_SEND_JACKPOTMONEY_TO_GAMESERVER,
// 	MS_OFFER_JACKPOTMONEY,
// 	MS_SEND_OVERJACKPOTMONEY,

	// 2011년 1월 프로모션
	MS_OFFER_RESTING_PROMOTION_MONEY,
	MS_GROUP_INSPECTION_INFO,			// 그룹 점검상태 정보
	MS_GROUP_INSPECTION_ADMININFO,	// 그룹점검시 입장가능한 어드민 정보
	MS_OPENTOURNAMENT_NOTI,				// 토너먼트 오픈 공지
};
// 서버용 메세지 #########################################################
enum
{
	////////////////// 값과 위치를 변경하지 말아야 할 부분 ////////////////

	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	SV_CHECKVERSION=10,		// 버전 체크
	SV_RECONNECT,		// 리컨넥션 정보
	SV_ASK_LOGIN,		// 접속 요청 로그인 정보	// [SSO 작업] - SV_ASK_LOGIN로 이름을 바꿈
	SV_SERVERINFO,		// 서버 정보	[게임 아이템 작업]


	/////////////////// 서버가 보내는 메세지 ///////////////////
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	SV_ACCEPT_LOGIN,	// 로그인을 허용함
	SV_REFUSE_LOGIN,		// 로그인을 거부함(원인: 아이디 없음, 비밀번호 틀림, 버전이 틀림)
	SV_ASK_LOGOUT,			// 로그아웃을 요청함
	SV_ACCEPT_LOGOUT,		// 로그아웃을 허용함
	SV_HEARTBEAT,			// 허트비트
	SV_DESTCHANNELINFO,		// 접속할 채널 정보(다른 서버로 채널 이동시)
	SV_CONNECT_MOVECHAN,	// 다른서버에서 채널 이동으로 접속 요청
	SV_DESTGAMEINFO,		// 접속할 채널 정보(다른 서버로 게임 이동시)
	SV_CONNECT_MOVEGAME,	// 다른서버에서 게임 이동으로 접속 요청
	SV_ASK_DETAILUSERINFO,	// 사용자의 상세 정보를 요구(개발자용)
	SV_DETAILUSERINFO,		// 사용자의 상세 정보
	SV_ASK_ENTERCHAN,		// 채널 입장을 요구
	SV_ACCEPT_ENTERCHAN,	// 채널 입장을 허가
	SV_ASK_FINDID,			// 아이디 찾기 요청
	SV_RESULT_FINDID,		// 아이디 찾기 결과
	SV_USERINFO,			// 사용자 정보를 보내줌(대상 : 특정 사용자 정보 요구시)
	SV_CHANGEMYINFO,		// 변경된 사용자 정보를 알려줌
	SV_ALLUSERINFO,			// 모든 접속자 목록을 보내준다(대상 : 요구하는 사용자)
	SV_NEWUSER,				// 새로운 사용자가 접속되었음(대상 : 전체 사용자)
	SV_OUTUSER,				// 사용자가 접속종료하였음(대상 : 전체 사용자)
	SV_CHANGEUSER,			// 접속한 다른 사용자의 정보가 변경되었음(대상: 전체사용자)
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	SV_ACCEPT_CHANGECHAR,	// 사용자 캐릭터를 바꾸기를 허락
	SV_REFUSE_CREATEROOM,	// 방 만들기를 거부함(대상: 방 개설을 시도한 사용자)
	// 실패한 원인 종류 코드(0:방 개설 제한 수 초과, 1:시험판 아이디)
	SV_ACCEPT_CREATEROOM,	// 방 만들기 요구에 의해 방이 만들어졌음(대상: 방 개설을 시도한 사용자)
	// 방장ID, 방 제목, 방 종류(협동, 대전)
	SV_ALLROOMINFO,			// 개설된 방의 전체 정보를 보내준다(대상 : 요구하는 사용자)
	SV_ROOMINFO,			// 특정 방의 정보를 보내준다(대상 : 요구하는 사용자)
	SV_ROOMMASTER,			// 특정한 방의 방장 정보를 보내준다(대상 : 요구하는 사용자)
	SV_NEWROOM,				// 새로운 방이 개설되었음을 알림
	SV_OUTROOM,				// 방이 사라졌음을 알림
	SV_CHANGEROOM,			// 방 정보가 수정되었음을 알림
	SV_USEROUTROOM,			// 사용자가 방에서 나왔다 - 같은 방 플레이어들에게 통보
	SV_ACCEPT_OUTROOM,		// 방에서 나가기를 허가
	SV_CHANGEROOMMASTER,	// 방장이 바뀜을 알린다
	SV_ABNORMALOUTROOM,		// 사용자에게 비정상적으로 퇴장하기를 요청(방장과의 재접속에 실패했을 경우)

	// ### [사운드 추가 작업] ###
	SV_ASK_CHANGESNDFX,     // 요청
	SV_ACCEPT_CHANGESNDFX,  // 허락

	/////////////////// 서버가 받는 메세지 ///////////////////

	SV_ASK_BADUSER,			// 채팅창 대화내용 신고 요청
	SV_ASK_CHANGEMYINFO,	// 나의 사용자 정보 수정을 요구
	SV_ASK_ALLCHANINFO,		// 전체 채널 정보를 요구
	SV_ALLCHANINFO,			// 전체 채널 정보
	SV_ASK_ALLROOMINFO,		// 전체 방 목록을 요구
	SV_ASK_ROOMINFO,		// 특정 방정보를 요구
	SV_ASK_ROOMMASTER,		// 특정 방의 방장 정보를 요구
	SV_ASK_ALLUSERINFO,		// 모든 접속자 목록을 요구
	SV_ASK_USERINFO,		// 특정 사용자 정보를 요구
	SV_ASK_CHANGECHAR,		// 사용자 캐릭터를 바꾸기를 요구
	SV_ASK_CREATEROOM,		// 방 만들기를 요구
	SV_ASK_ENTERROOM=555,		// 방에 입장하기를 요구
	SV_ACCEPT_ENTERROOM,	// 방에 입장하기를 허가함(대상: 입장을 요구한 사용자)
	SV_REFUSE_ENTERROOM,	// 방 입장요청을 거부함(대상: 입장을 요구한 사용자)
	SV_USERENTERROOM,		// 사용자가 방에 입장하였음
	SV_LOBYOPEN,			// 대기실 보기 초대창 오픈 상태 알림
	SV_ASK_OUTROOM,			// 방에서 나가기를 요구
	SV_ASK_INVITE,			// 초청 메세지를 보냄
	SV_REFUSE_INVITE,		// 초청 메세지를 받음
	SV_ASK_BANISHVOTE,		// 추방 요청
	SV_MYBANISHVOTE,		// 나의 추방 투표 결과
	SV_NOWHISPER,			// 귓말 거부,허가 상태 변경
	SV_ASK_FORCEBANISH,		// 강제추방 요청	[게임 아이템 작업]
	SV_NOTIFY_BANISHCOUNT,	// 강제추방 카운트 클라이언트에 알려주기
	


	/////////////////// 게임 아이템 메세지 [게임 아이템 작업] ///////////////////

	SV_ASK_GAMEITEMLIST,	// 게임 아이템 목록 갱신을 요청
	SV_GAMEITEMLIST,		// 게임 아이템 목록을 보내줌
	SV_USEGAMEITEM,			// 게임 아이템 사용 요청
	SV_BUYGAMEITEM,			// 게임 아이템을 구입할것인지 물어봄

	/////////////////// 공통 메세지 ///////////////////

	SV_SERVERMSG,			// 서버 메세지
	SV_CHATDATA,			// 대기실용 채팅 데이타(주의: 보내고 받을때 모두 같은 메세지 사용)
	SV_WHISPERCHAT,			// 귓속말

	SV_ASK_QUICKSTART,		// 게임 바로 시작
	SV_REFUSE_QUICKSTART,	// 바로시작 안됨 (방만들기)

	SV_ASK_QUICKSTART2,		// 규제안 바로 시작
	SV_REFUSE_QUICKSTART2,	//

	SV_ORDER_QUICKSTART_IN_ROOM,	// 다른방 바로가기 Order
	CL_ASK_QUICKSTART_IN_ROOM,		// 다른방 바로가기 요청
	SV_REFUSE_QUICKSTART_IN_ROOM,	// 다른방 바로가기 실패

	/////////////////// ### [ 관전기능 ] ###  ///////////////////

	SV_ASK_PARTICIPATION,   // 겜참여 요청
	SV_ACCEPT_PARTICIPATION, // 참여
	SV_ENTER_PARTICIPATION,  // 다른 사용자에게 알린다
	SV_ASK_EMERAGE,         // 겜참여에서 관전으로
	SV_ACCEPT_EMERAGE,      // 관전
	SV_ENTER_EMERAGE,		// 다른 사용자에게 알린다
	SV_RESERVATIONINFO,     // 예약자 정보
	SV_OBSERVER_MESSAGE,	// 관전 관련 메세지

	/////////////////// ### [관리자 모드 작업] ###  ///////////////////

	NM_ADMIN_WHISPERCHAT,	// 귓말하기(강제)
	NM_ADMIN_WARNUSER,		// 사용자 관리(운영자)
	NM_ADMIN_OUTUSER,		// 사용자 제거하기(운영자)
	NM_ADMIN_CHANGEROOM,	// 겜방 옵션 변경요청(운영자)
	NM_ADMIN_DELROOM,		// 방제거하기(운영자)
	SV_CHANGESTYLEROOMINFO, // 방이름 변경 알림(운영자)
	NM_SPECIALMESSAGE,      // 특별 관리 메세지(운영자)
	NM_ADMIN_SETTINGMAN,	// 관리자 게임 환경설정 세팅 


	SV_SERVERNOTIC,			// 서버 공지 메세지
	CL_ASK_CHSTYLEROOMINFO, // 방정보변경	
	CL_CHARGE_MONEY,	//  머니 충전 메시지
	

	SV_SPEEDHACK_DEFENSE,	// [스피드헥] 2004.09.01
	SV_ASK_GAME_CONTINUE,	// [DB기록 Process] 2004.11.29
	SV_RES_GAME_CONTINUE,	// [DB기록 Process] 2004.11.29
	
	SV_ASK_ENTERRULEROOM,	// [자동 베팅룰] 2005.05.25 - 룰방 입장 물어보기

	SV_ASK_BUYITEM,				// 아이템 파라미터 요청
	SV_ALLOW_BUYITEM,			// 
	SV_BUYITEMPARAM,			// 아이템 파라미터를 보냄
	SV_BUYITEM_RESULT,			// 아이템 파라미터 요청시 에러 결과
	SV_ASK_CHECK_REWARD,		// 아이템 구매 여부 체크
	SV_REWARD_CHECK_RESULT,		// 아이템 구매 여부 체크 결과
	// reward item db 08.02.27	
	SV_REWARDITEM_LIST,			// DB저장된 찬스아이템 보내줌


	/////////////////// ### [올인보험] ###  ///////////////////

	SV_AIN_INSURANCE_JOIN_REQUEST,		// 올인보험 가입 요청
	SV_AIN_INSURANCE_SUPPLY_REQUEST,	// 올인보험 지급 요청
	SV_AIN_INSURANCE_JOIN_RESULT,		// 올인보험 가입 요청 결과 메세지
	SV_AIN_INSURANCE_SUPPLY_RESULT,		// 올인보험 결과 메세지
	
		
// 클라이언트용 메세지 #########################################################

	/////////////////// 공통 메세지 ///////////////////

	CL_EXITSUBSCRIPT,	// 플레이어의 나가기 예약 상태
	CL_CHATDATA,			// 클라이언트간의 채팅 메세지

	
	// [ 게임 아이템 ]
	CL_ITEMRESULT,			// 아이템 결과값..( 에러값이 있을 경우에만.. )
	CL_ITEMLIST,			// 아이템 리스트..
	CL_ITEMLOGIN,			// 아이템을 사용한 로그인..
	CL_ASK_ITEMLOGIN,		// 아이템 사용할 것인가 물어봄.
	CL_ACCEPT_ITEMLOGIN,	// 아이템 서버 로그인 허가.

	// [점프 아이템 작업]
	CL_ASK_JUMPITEM,		// 채널에서 점프아이템 사용 물어봄.
	CL_JUMPITEM_LOGIN,		// 채널에서 점프아이템 사용 로긴
	CL_ACCEPT_JUMPITEM_CH,  // 유저 -> 서버 요청 DB OK => 유저결과(아이템 다이얼로그 박스만 보여준다)
	CL_ACCEPT_JUMPITEM,		// 채널에서 점프아이템 한번이라도 사용한 아템이 있을때(바로 입장)

	SV_LEADERSJOIN,			//2004.05.07 리더스총액 제한
	SV_SPECIALCARD,			// 모든 서버 접속자에게 알림, 잭팟

	
	//게임방내 공통메세지 
	SV_ENDGAME,				// 판종료 알림
	SV_ASKGAMESTART,		// 게임시작 요청
	SV_STARTBTNENABLE,		// 버튼활성화(메세지)

	//
	SV_PREM_OPTION_INFO,		// [재기충전 작업] [2009-02-12]
	CL_ASK_RECOVERY_CHARGE,			// [재기충전 작업] [2009-02-10]
	CL_RECOVERY_CHARGE_INFO,		// [재기충전 작업] [2009-02-11]


	//기사회생 
	CL_REPLY_KISACHARGE,		//기사회생 옵션상품
	SV_NOTIFY_ALLIN,			//올인창 띄워주기 


	//내맘대로 바로입장
	CL_ASK_PREMQUICKJOIN,	//내맘대로 바로입장 요청
	SV_REFUSE_PREMQUICKJOIN, //내맘대로 바로입장  조건에 해당하는 방이 없을 때

	SV_SEATKEEPER,			//자리 지킴이 

	CL_ASK_WAIT_INROOM,		// 대기 기능
	SV_ASK_WAIT_INROOM,

	SV_ROOMOPTIONINFO,		/// 방 옵션 정보 

	CL_NOTIFY_MACADDRESS,   //DAT로그 (서버에게 맥어드레스 통보)

	SV_PCROOMEVENT_INFO,		//pc방이벤트
	SV_YEARENDEVENT_INFO,		//연말이벤트
	SV_GC_ASK_JOKBO_STEP2,	//족보 2단계 퀘스트 요청

	//G_TFT
	SV_CHANGE_MONEY,		//PMoney 변화가 있을때 알려주는 메세지

	SV_NOTIFY_CHANGEOVERMONEY,	// 초과금, 보정머니 발생시 알려줌(채널 입장시, 게임오버시)
	SV_NOTIFY_CHANGEPREMIUM,	// 회원제 상태 변경 알림

// 통합 클라이언트 게임변경 요청
	CL_ASK_MOVEGAME,				// 게임 탭 변경 요청
	SV_ERROR_MOVEGAME,			// 게임 탭 변경실패 응답
	SV_ACCEPT_MOVEGAME,

	SV_ADD_BOMBCARD,		// 폭탄 카드 추가
	SV_OPEN_BOMBCARD,		// 폭탁 카드 오픈	

	SV_SAFEANGELDATA,		// 수호천사 충전 머니값(노말,리더스,리더스플러스)

	SV_GIVE_JOKERCARD,		// 조커 카드값을를 보낸다.(조커 방땀시 추가


	SV_CHANGE_GAMEUSERINFO, // 게임도중 유저 정보 변경

	SV_GIVE_TO_DSDATA,		// 도신지수 데이타 보내주기	


	//골든칩 이벤트 2006.2.28	
	SV_GC_GOLDENDCHIP,			// 유저 골드 이벤트 데이터
	SV_ACTIVE_GOLDEVENT_MSG,				// 골든벨 이벤트 판이라고 알림

	SV_NIGHTEVENT_MSG,					// 나이트 이벤트 시작 알림	
	SV_ACTIVE_NIGHTEVENT_MSG,			// 나이트 이벤트 판이라고 알림(나이트 이벤트 판이다라는 메세지



	//[행운아바타] 2006.07.24
	SV_RESULT_BUYLUCKAVATAITEM,	// 유저에게 행운아바타를 구매 여부의 결과 값(구매했을때만 서버로 날림)

	//[포커 추석 이벤트] 2006.08.24
	SV_SEND_GAMEBETEVENTINFO,	//유저 로긴 성공시 이벤트 정보를 보낸다.
	SV_RESULT_GAMEBETEVENTINFO,	//게임 한판 종료 후 포인트 정보를 보내준다.
	SV_RUN_GAMEBETEVENTINFO,    //이벤트 종료 //포커 챔피언쉽 2007

	// 관리자모드작업
	SV_RESULT_GAMEBETEVENTINFO_ADMIN,	//포커 챔피언(관리자)

	/////////////////// 게임 진행 관련 메세지 /////////

	CL_ASK_CHANGEGAMEKIND,	// 게임 방식 바꿈 요청(0:아이템 1:노템)
	CL_CHANGEGAMEKIND,		// 게임 방식 바꿈 결과(0:아이템 1:노템)


	CL_ADD_BOMBCARD_RESULT,		// 폭탄 카드 추가	결과
	CL_OPEN_BOMBCARD_RESULT,	// 폭탁 카드 오픈 결과

	CL_CHANGE_IDCOLOR,		// 아이디 색 변경 요청
	CL_REQ_LOCKROOM,
	CL_ASK_INVITEUSERLIST,
	SV_INVITEUSERLIST,
	SV_HEARTBEATGHOSTCHECK,			// 허트비트 고스트체크용
	SV_EMERGENCYCHECK,					// 임시점검 여부알림 패킷
	////////////UPP 관련 메시지 ////////////
	CL_REQUEST_PLAYTIMEINFO,
	SV_RESPONSE_PLAYTIMEINFO,
	SV_NOTICE_TOURNAMENT_PLAYTIME,
	SV_UPPTIMEINFO , 
	SV_UPPEVENT , 
	SV_UPPINFO ,  
	CL_ASK_UPPMSG , 

	CL_CHECK_DAILY_LOSS_MONEY,
	SV_DAILY_LOSS_MONEY,
	CL_RETENTION_REWARD,		// 복귀유저 이벤트 당첨 결과

#if defined(_BETAVER)
	CL_REQUEST_BETA_ALLIN_MONEY,
#endif  // defined(_BETAVER)

//#ifdef __ROUND_MODE__	// 라운드 모드용
	CL_ROUND_MODE_RESERVE,		// 라운드모드 예약
	CL_ROUND_MODE_CANCEL,		// 라운드모드 취소
	SV_ROUND_MODE_RESERVE,
	SV_ROUND_MODE_CANCEL,		// 라운드모드 취소
	CL_ROUND_MODE_SEND_SELECT_ENTER_ROOM,	// 라운드모드 방 입장 / 취소 알림 메시지
	SV_ROUND_MODE_SEND_SELECT_ENTER_ROOM,
	CL_ROUND_MODE_ENTER_ROOM,	// 라운드모드 방 입장 
	CL_ROUND_MODE_EXIT_ROOM,
	SV_ROUND_MODE_GAME_START_WAIT_TIME, // 게임 시작전 3초 대기
	SV_ROUND_MODE_GAME_RESULT,  // 일반 경기 종료
	SV_ROUND_MODE_GAME_OVER,	// 라운드 모드 게임 종료로 순위별 금액 지급
	SV_ROUND_MODE_ROUND_COUNT,	// 라운드 모드 방안에서의 플레이 횟수
	SV_ROUND_MODE_BCAST_CHIPS,	// 라운드 모드에서 사용하는 칩을 방에 있는 유저들에게 보내준다.
	SV_ROUND_MODE_FOCUS_TIME,   //라운드 모드 집중 플레이 시간
//#endif
	

	SV_VIP_JACKPOT_BET_POINT,		// 잭팟 이벤트용 베팅 점수 정보
	CL_VIP_RANK_INFO,				// 클라요청 잭팟 랭킹 정보요청	
	SV_VIP_RANK_INFO,				// 월드랭킹 및 요일 랭킹 정보
	CL_GET_LEAGUE_REWARD_MONEY,		// 지난 리그전 머니 요청
	SV_LEAGUE_REWARD_MONEY,			// 지난 리그전 보상 지급
	SV_LEAGUE_LOGIN_INFO,			// 로그인시 보내주는 리그전 정보
	SV_LEAGUE_NO_ICON,				// 리그전 아이콘 비활성화
	CL_LEAGUE_NO_ICON,				// 리그전 아이콘 활성/비활성
	SV_LEAGUE_BET_POINT,			// 추가되는 베팅 포인트만 전송한다.
	SV_LEAGUE_IS_LEAGUE,			// 리그 진행중

	CL_GET_MY_LEAGUE_INFO,
	CL_SET_OLDLEAGUE_VIEW,		// 지난 리그 승강 뷰 확인

	SV_NEW_UPPINFO, ////////////////////////////2017.04.08 신규UPP 작업/////////////////////////////
	SV_DAILYLOSSLIMIT_USER_SETTING_INFO, /////////////////////////////손실한도 초과 기능 추가 2017.05.15/////////////////////////////////////////////
	CL_SET_DAILYLOSSLIMIT_INFO, /////////////////////////////손실한도 초과 기능 추가 2017.05.15/////////////////////////////////////////////
	SV_REWARD_DAILYLOSS_OVER, /////////////////////////////손실한도 초과 기능 추가 2017.05.15/////////////////////////////////////////////


	SV_REWARD_ALLIN_ANGEL, ////////////////////////////채널밸런싱, 올인천사 2017.08.10/////////////////////////////////////

	////////////////////////////한가위, 유통량확대 2017.08.25/////////////////////////////////////
	SV_PLAYINFO_SUPERMOON,
	SV_REWARD_SUPERMOON,
	/////////////////////////////////////////////////

	////////////////////////////골드시스템,골드채널 2017.11.07/////////////////////////////////////
	SV_REWARD_TOTALPOKER_SUPERSCORE,
	SV_REWARD_TOTALPOKER_START,
	////////////////////////////////////////////////

	////////////////////////////블라인드배팅, 타겟알림 2017.10.12/////////////////////////////////////
	SV_NOTICE_BIGWIN,
	CL_SEND_CONG_MSG,
	SV_RECEIVE_CONG_MSG,
	///////////////////////

	////////////////////////////골드시스템,골드채널 2017.11.07/////////////////////////////////////
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

	////////////////////////////클럽시스템 2017.12.21/////////////////////////////////////
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

	////////////////////////////2018설날이벤트 2018.01.22/////////////////////////////////////
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

	////////////////////////////골드경기장 골드적립 2018.02.20/////////////////////////////////////
	SV_GOLD_TEMPMONEY_INFO,
	SV_GOLD_TEMPMONEY_REAL_UPDATE,
	/////////////////////////

	////////////////////////////섯다 골드경기장, 골드지원금 2018.02.27/////////////////////////////////////
	SV_GOLD_HELP_REWARD,
	SV_GOLD_HELP_REWARD_INFO,
	////////////////////////////

	////////////////////////////홀짝미니게임 이벤트 2018.03.12/////////////////////////////////////
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

	////////////////////////////휠 미니게임 이벤트 2019.07.30/////////////////////////////////////
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

	////////////////////////////미니게임 빅휠 2018.06.04/////////////////////////////////////
	CL_PM_BIGWHEEL_GET_REMAIN_GAMECOUNT,		// 남은 게임 횟수 요청
	SV_PM_BIGWHEEL_GET_REMAIN_GAMECOUNT,		// 남은 게임 횟수 응답
	CL_PM_BIGWHEEL_PLAY_GAME,					// 게임 플레이 요청
	SV_PM_BIGWHEEL_PLAY_GAME,					// 게임 플레이 응답/결과
	////////////////////////////


	////////////////////////////2018년06월 오픈 2018.06.01/////////////////////////////////////
	CL_GOLDBANK_WITHDRAW,
	SV_GOLDBANK_WITHDRAW_RESULT,
	SV_ETCCHIP_INSURANCE_USER_INFO,
	SV_ETCCHIP_INSURANCE_USER_CHANGE_INFO,
	////////////////////////////

	////////////////////////////2018년07월 2번째 업데이트/////////////////////////////////////
	CL_ASK_CARDOPEN,			// 카드 오픈 요청
	SV_ASK_CARDOPEN_RESULT,		// 카드 오픈 요청 결과
	////////////////////////////

	// 신고하기 결과 메시지. 2018.07. 2번째 업데이트
	SV_REPORT_RIGUSER_RESULT,

	//////////////////////////////////////////////////////////////////////////
	// 카페 시스템. 2018.12.20
	CL_CAFE_MEMBERLIST,		// 멤버 리스트 요청
	SV_CAFE_MEMBERLIST,
	CL_CAFE_ASK_JOIN,		// 가입
	SV_CAFE_ASK_JOIN,
	CL_CAFE_INVITE,			// 초대
	SV_CAFE_INVITE,
	SV_CAFE_RESERVED_POINT,	// 예비 포인트
	SV_CAFE_NOTICE,			// 공지. VIP 등급 상승 및 하락에 따른 노티.
	//////////////////////////////////////////////////////////////////////////
	
	////////////////////////////2019 한가위 이벤트 2019.09.05/////////////////////////////////////
	SV_PM_2019HAVEST_INFO,
	CL_PM_2019HAVEST_REQUEST_REWARD,
	SV_PM_2019HAVEST_REQUEST_RESULT,
	////////////////////////////

	CL_DEBUG_SETMONEY=9999,					// 디버그용 메세지
};

///////////////////  게임 진행 공통 메세지 ////////////////////
//////////////////////////////////////////////////////////////
enum SIGNAL_COMMONGAME{SIG_HEAD_COMMONGAME=1000,
//////////////////////////////////////////////////////////////
//{
	SV_STARTCARD,		// 처음 3개의 카드를 날림
	SV_STARTGAME,			// 게임이 시작되었음
	SV_GAMEOVERRESULT,	// 게임 종료되었을때 결과를 통보(대상 : 같은 방 플레이어 모두)
	SV_GAMEOVER,			// 서버에게 게임이 종료됨을 통보(방장이 보냄)
	SV_RAISE,			// 서버 알림 레이즈
	SV_WINDCARDEND,		// 첫 3장을 보낸후 유저가 하나의 카드를 선택 => 모두 선택시 게임시작
	SV_FOLDUSER,			// 유저가 폴드함

	CL_ASK_FOLD,			// 플레이어 폴드
	CL_ASK_RAISE,		// 사용자의 레이즈
	CL_WINDCARDEND,		// 사용자의 카드날리기 종료

	SV_SELECTCARD,		// 카드 타이머 제거			
	SV_HIDDENCARD,		// 마지막 히든 카드 날림
	CL_ASK_SELECTCARD,	// 사용자의 카드선택

	SV_BLINDSHOW, //로우바둑이 블라인트 배팅이 끝나고, 내 패를 오픈한다.

//}
//////////////////////////////////////////////////////////////
SIG_TAIL_COMMONGAME};				//세븐포커 2 게임 메세지 끝
//////////////////////////////////////////////////////////////	
//////////////////////////////////////////////////////////////


/////////////////// 세븐포커2 게임 메세지 ////////////////////
//////////////////////////////////////////////////////////////
enum SIGNAL_SPOKER{SIG_HEAD_SPOKER=1100, // 시작
//////////////////////////////////////////////////////////////
//{

	
//}
//////////////////////////////////////////////////////////////
SIG_TAIL_SPOKER};				//세븐포커 2 게임 메세지 끝
//////////////////////////////////////////////////////////////	
//////////////////////////////////////////////////////////////


/////////////////// 하이로우 게임 메세지 ////////////////////
//////////////////////////////////////////////////////////////
enum SIGNAL_HIGHLOW{SIG_HEAD_HIGHLOW=1200, // 시작
//////////////////////////////////////////////////////////////
//{

	SV_HL_ENDRAISE,			// 모든 레이즈 끝 => 하이 로우 스윙 결정 요청	
	SV_HL_HLS_SEL,		// 하이, 로우, 스윙 버튼 클릭했음을 알림	
	SV_HL_CARDINFO,			//카드 정보를 보내준다.	
	
	CL_HL_SELECTWINCASE,		// 어떤걸로 승부를 겨루겠는가 하이 로우 스윙
//	CL_HL_ENDWORK,	//끝났다고 메세지를 보내준다.(결과보기및 기타)

//}
//////////////////////////////////////////////////////////////
SIG_TAIL_HIGHLOW};				//끝
//////////////////////////////////////////////////////////////	
//////////////////////////////////////////////////////////////


/////////////////// 바둑이 게임 메세지 ////////////////////
//////////////////////////////////////////////////////////////
enum SIGNAL_BADUGI{SIG_HEAD_BADUGI=1300, // 시작
//////////////////////////////////////////////////////////////
//{

	SV_BD_NOTIFY_CARDEXCHANGE,		//카드 선택시간이다라는걸 클라에게 알려준다.
	SV_BD_GIVETOUSER_NEWCHANGECARD,//바꾸기 요청한 사람에게 새로운 카드를 준다.		
	
	CL_BD_CARD_EXCHANGE,       // 카드를 바꾼걸 서버에게 알려준다.
	CL_BD_CARD_EXCHANGEEND,	//새로운카드를 정상적으로 받았다 라고 서버에 알려준다.

	SV_REPORT_BANISH_MORNINGBET,	// 모닝뱃 밴 결과 전송

//}
//////////////////////////////////////////////////////////////
SIG_TAIL_BADUGI};				//끝
//////////////////////////////////////////////////////////////	
//////////////////////////////////////////////////////////////


//#if defined(__SUTDA__)
/////////////////// 섯다 게임 메세지 /////////////////////////
//////////////////////////////////////////////////////////////
enum SIGNAL_SUTDA{SIG_HEAD_SUTDA=1400, // 시작
//////////////////////////////////////////////////////////////
//{
CL_SD_JJOGI,					// 카드 쪼기
SV_SD_NOTIFY_JJOGI,				// 카드 쪼기 공지

SV_SD_CHOICE,					// 서버에서 족보 선택 요청(3장 섯다에서만 전송)
CL_SD_ASK_CHOICE,				// 클라이언트에서 족보 선택
SV_SD_ACCEPT_CHOICE,			// 서버에서 족보 선택 수락

SV_SD_RESULTCARD,				// 게임 결과 패 오픈 연출 요청
CL_SD_RESULTCARD,				// 게임 결과 패 오픈 연출 완료

SV_SD_NOTIFY_REMATCH,			// 49 재경기 공지
SV_SD_49REMATCH,				// 서버에서 49 재경기 공지 (패킷 수신한 클라이언트는 참가 가능 대상자)
CL_SD_ASK_49REMATCH,			// 클라이언트에서 49 재경기 참가 신청 (패킷 전송한 클라이언트는 참가 신청)
SV_SD_NOTIFY_ASK,				// 재경기 참가 공지


SV_SD_REMATCH_CARDINFO,			// 재경기 카드 정보(라이브 유저한테만 각각 전달)
//}
//////////////////////////////////////////////////////////////
SIG_TAIL_SUTDA};				//끝
//////////////////////////////////////////////////////////////	
//////////////////////////////////////////////////////////////
//#endif	// defined(__SUTDA__)


//#if defined(__HOOLA__)
/////////////////// 섯다 게임 메세지 /////////////////////////
enum SIGNAL_HOOLA {SIG_HEAD_HOOLA=1500, // 시작

// 훌라 개선 작업
// 턴 매니저
SV_DISCARD,				// 서버에서 알림 카드 버림 탱크도 같이
SV_REGRESULT,			// 등록한 카드 응답
SV_STOPGAMERESULT,		// 스톱한 결과 모두에게
SV_THANKRESULT,			// 탱크카드 결과
SV_SPECIALRULERESULT,   // 7포카드 등등의 기타 특수룰로 이겼을 경우 전송

SV_TURNTHANK,			// 훌라
SV_TURNGET,
SV_TURNREG,
SV_THANK_FAIL,

SV_CANTHANK,            // 땡큐 할 수 있음
SV_ADDEDCARDDATA,       // 판에 깔린 카드 데이터
SV_FIXREGIST,
SV_FIXCARDDATA,
SV_ENABLEGAMESTARTBTN,

SV_MISSION,             // 게임 시작시 미션 알림
SV_MISSIONCOMPLETE,     // 미션 성공 알림

SV_MAGICCARD,


CL_REQGAMESTART,
CL_REQGAMEPRACTICS,
CL_ASK_ONECARD,			// 카드 요청
CL_BRINGINCARD,			// 카드한장을 보낸다

CL_DISCARD,				// 현재 유저의 카드 버리기
CL_ASK_THANK,			// 유저가 탱크 선택
CL_ASK_CONTINUE,		// 탱크 하지 않고 계속 진행
CL_REGISTER,			// 유저가 등록 요청
CL_ATTACH,				// 카드 붙이기
CL_STOPGAME,			// 스톱

CL_PRESS_THANK,         // thank 버튼 누름
CL_REGIST,
CL_ENDCARDMOVE,
SV_JACKPOTCOUNT,

// 선 고르기
CL_SUN_PICKUP_START,
CL_SUN_PICKUP,
CL_SUN_PICKUP_COMPLETE,

CL_GOLDSTAGE_INFO,	  // 미션 리셋

////////////////////////////훌라 기권방 2018.03.29/////////////////////////////////////
CL_PRESS_GIVEUP,
SV_GIVEUP_RULE_RESULT,
SV_GIVEUP_RULE_NOTI,
SV_GIVEUP_LOSE_MONEY_NOTI,
/////////////////////////////////////////////

SIG_TAIL_HOOLA};				//끝
//////////////////////////////////////////////////////////////
//#endif


/////////////////// 토너먼트 메세지 /////////////////////////
//////////////////////////////////////////////////////////////
enum SIGNAL_TOURNAMENT{SIG_HEAD_TOURNAMENT=1600, // 시작, 1600 번대는 훌라 사용
//////////////////////////////////////////////////////////////
//{

// 토너먼트 리스트 요청/응답
CL_REQUEST_TOURNAMENT_LIST,
SV_RESPONSE_TOURNAMENT_LIST,

// 토너먼트 정보 요청/응답
CL_REQUEST_TOURNAMENT_INFO,
SV_RESPONSE_TOURNAMENT_INFO,
SV_NOTIFY_TOURNAMENT_INFO,

// 티켓 판매 시작/종료 공지
SV_OPEN_DESK,
SV_CLOSE_DESK,

// 토너먼트 시작/종료 공지
SV_START_TOURNAMENT,
SV_END_TOURNAMENT,

// 레벨업 공지
SV_LEVEL_UP_TOURNAMENT,

// 티켓 구매
CL_BUY_TOURNAMENT_TICKET,			// 티켓 구매 요청
SV_RESULT_BUY_TOURNAMENT_TICKET,	// 티켓 구매 결과
SV_NOTIFY_TOURNAMENT_TICKET,		// 접수 인원 변동에 따른 공지
SV_MY_TOURNAMENT_LIST,				// 유저가 티켓을 구매한 토너먼트 목록

// 티켓 환불
CL_REFUND_TOURNAMENT_TICKET,		// 티켓 환불 요청
SV_RESULT_REFUND_TOURNAMENT_TICKET,	// 티켓 환불 요청 결과

// 토너먼트 참가/포기
SV_NOTIFY_REGIST_TOURNAMENT,
CL_REQUEST_REGIST_TOURNAMENT,
SV_RESPONSE_REGIST_TOURNAMENT,
CL_REQUEST_DROP_TOURNAMENT,
SV_RESPONSE_DROP_TOURNAMENT,

// 클라에게 방 생성/입장/퇴장 전달
SV_CREATE_TOURNAMENT_ROOM,
SV_ENTER_TOURNAMENT_ROOM,
SV_LEAVE_TOURNAMENT_ROOM,

// 리바비인
SV_CHECK_REBUYIN,		// 리바이인 여부 결정 요청
CL_ASK_REBUYIN,		// 리바이인 결정
SV_RESULT_REBUYIN,		// 리바이인 요청 결과
SV_ELIMINATED,		// 탈락
SV_NOTIFY_ELIMINATED,	// 탈락 공지

// 게임 결과
SV_TOURNAMENT_GAME_RESULT,

// 랭킹 요청/응답
CL_REQUEST_RANKING,
SV_RESPONSE_RANKING,
CL_REQUEST_MY_RANKING,
SV_RESPONSE_MY_RANKING,

// 토너먼트 참가 노티
SV_NOTIFY_TOURNAMENT,

// 휴식 시간
SV_NOTIFY_START_BREAK,
SV_NOTIFY_STOP_BREAK,

// 게임 중지/재개
SV_STOP_GAME,
SV_START_GAME,

// 유저 토너먼트 참가 정보
CL_REQUEST_TOURNAMENT_USER_INFO,
SV_RESPONSE_TOURNAMENT_USER_INFO,

// 특정 유저가 경기중인 방으로 관전 입장
CL_ASK_WATCH_TOURNAMENT_ROOM,
SV_WATCH_TOURNAMENT_ROOM,

// 현재 유저 랭크 및 기타 정보
CL_REQUEST_CURRENT_USER_RANK,
SV_RESPONSE_CURRENT_USER_RANK,

// 미지급금 환급
SV_TOURNAMENT_RETURN_UNPAID,



GS2CL_STARTTOURNAMENT_NOTI,			// 토너먼트 시작 공지 (GS->CL)
GS2CL_ENDTOURNAMENT_NOTI,			// 토너먼트 종료 공지 (GS->CL)
CL2GS_REGISTERUSER_REQ,				// 토너먼트 참가 신청 (CL->GS)
CL2GS_REGISTERUSER_ANS,				// 토너먼트 참가 신청 결과 (GS->CL)
GS2CL_REGISTERUSER_NOTI,			// 토너먼트 참가 공지 (GS->CL)
CL2GS_GETUSERRAT_REQ,				// 유저 토너먼트 정보 요청 (CL->GS), 본인 정보는 로그인 성공시 서버에서 자동으로 요청 처리, 타인 토너먼트 정보 필요시 요청 하면 됨.
CL2GS_GETUSERRAT_ANS,				// 유저 토너먼트 정보 결과 (GS->CL)
GS2CL_UNREGISTERUSER_NOTI,			// 토너먼트 참가 취소 공지 (GS->CL)
GS2CL_CREATEROOM_REQ,				// 방 생성 요청 (GS->CL)
GS2CL_JOINROOM_REQ,					// 방 입장 요청 (GS->CL)
GS2CL_LEAVEROOM_REQ,				// 방 퇴장 요청 (GS->CL)
GS2CL_SEEDUP_NOTI,					// 시드업 공지 (GS->CL)
GS2CL_GAMEOVER_NOTI,				// 게임 결과 공지 (GS->CL)
CL2GS_USERRANKING_REQ,				// 유저 랭킹 요청 (CL->GS)
CL2GS_USERRANKING_ANS,				// 유저 랭킹 (GS->CL)
CL2GS_TOURNAMENTINFO_REQ,			// 토너먼트 정보 요청 (CL->GS)
CL2GS_TOURNAMENTINFO_ANS,			// 토너먼트 정보 요청 결과 (GS->CL)
CL2GS_TOURNAMENTRESULT_REQ,			// 이전 토너먼트 정보 요청 (CL->GS)
CL2GS_TOURNAMENTRESULT_ANS,			// 이전 토너먼트 정보 결과 (GS->CL)
CL2GS_CHATMSG_REQ,					// TMS 전용 채팅
CL2GS_CHATMSG_NOTI,					// TMS 전용 채팅 공지
GS2CL_UNREGISTER_NOTI,				// 토너먼트 참가 취소 공지 (GS->CL)
GS2CL_SETMONEYINFO_NOTI,			// 머니 정보 갱신 요청 (GS->CL)
GS2CL_SHOWMOVIE_NOTI,				// 토너먼트 동영상 재생 공지 (GS->CL)
GS2CL_LEAVEROOM_NOTI,				// 유저 퇴장 공지 (GS->CL), 유저가 UserExitRoom 으로 퇴장한 경우 전송됨.
GS2CL_OPENTOURNAMENT_NOTI,			// 토너먼트 오픈 공지 (GS->CL) TS->GS->MS->GS->CL 로 공지
GS2CL_UNEARNEDWIN_NOTI,				// 부전승 공지 (GS->CL)
CL2GS_UNEARNEDWIN_REQ,				// 부전승 머니 지급 요청 (CL->GS)
CL2GS_UNEARNEDWIN_ANS,				// 부전승 머니 지급 결과 (GS->CL)
GS2CL_RESERVEDUSERPRIZE_NOTI,		// 사전 예약자 머니 지급 공지(GS->CL)
CL2GS_RESERVEDUSERPRIZE_REQ,		// 사전 예약자 머니 지급 요청 (CL->GS)
CL2GS_RESERVEDUSERPRIZE_ANS,		// 사전 예약자 머니 지급 결과 (GS->CL)
GS2CL_CHANGETOURNAMENTSTATE_NOTI,	// 토너먼트 상태변경 노티
GS2CL_ELIMINATETOURNAMENT_NOTI,		// 토너먼트 탈락 노티
GS2CL_MATCHWAITING_NOTI,			// 토너먼트 매치 대기 노티
GS2CL_FINALLASTROOMMATCH_NOTI,		// 결승 방이 하나만 남았을때 노티
GS2CL_TOURNAMENTPROGRESS_NOTI,		// 예선/본선 종료 5분전
//}
//////////////////////////////////////////////////////////////
SIG_TAIL_TOURNAMENT};				//끝
//////////////////////////////////////////////////////////////	
//////////////////////////////////////////////////////////////

