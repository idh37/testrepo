#pragma once




//게임종료시 음선 우선순위 리스트
//#define END_GONIEVENT	7	//타짜이벤트
#define	END_ROOMJACKPOT 6	//룸잭팟승 
#define	END_ALLIN		5	//올인승패
#define END_SORT		4	//한끗차이승패
#define END_DRAW		3	//다이승패 
#define	END_BIG			2	//대박승
#define END_SMALL		1	//쪽박승
#define END_NORMAL		0	//일반승패 

enum AV_VOICE
{
	AV_BANISHVOTE = 0,	//강제퇴장예약시 
	AV_SUPERBANISH,		//슈퍼방장이 강퇴 시킬때
	AV_FIRSTBET,		//초구 배팅시 
	AV_GETHIDDEN,		//히든카드받았을때
	AV_JOINROOM,		//게임에 입장했을때
	AV_WATCHSUBSCRIPT,	//관전예약
	AV_ACTIONJOKBO,		//족보를 까봤을때 

	AV_MAKE_HIDDEN,		//히든족보완성	
	AV_FAIL_HIDDEN,		//히든족보실패 //7

	AV_OTHER_ALLIN,		//상대방을 올인시켰을때 
	AV_MY_ALLIN,		//내가 올인당했을때	//10

	AV_BET,				//배팅

	AV_SORT_LOSS,		//한끗차이로 질때
	AV_DRAW_LOSS,		//승패를 보지 못하고 모둔 유저가 포기한경우
	AV_NORMAL_LOSS,		//일반패 위에 2개사항 이외일경우

	AV_SORT_WIN,		//한끗차이 승리 
	AV_DRAW_WIN,		//다이승
	AV_BIG_WIN,			//대박승 (5억이상 승)
	AV_SMALL_WIN,		//쪽박승 (4백이하 승)
	AV_NORMAL_WIN,		//일반승 (위에 승리사항에 없을경우)

	AV_49_REMATCH,		//49로 다시 경기 
	AV_JOKBO_REMATCH,	//족보로 비김 

	AV_ROOMJACPOT_START,	//룸잭팟 시작 
	AV_ROOMJACKPT_WIN,		//룸잭팟판에서 승리	//23
};


class CSutdaGameSound
{

	int m_nVoiceType;		//설정창에 설정된 보이스 타입 (1~4)

	int m_vWinandLossVoice;	//승패에 따른 음성효과

	int GetVoiceType(BOOL bMySex, int nSndFxKind);
public:
	CSutdaGameSound(void);
	~CSutdaGameSound(void);
	static CSutdaGameSound& GetInstance();



	void SetMyVoiceType(BOOL bMySex, int nSndFxKind);
	void SetEndActionVoice(int r_order);
	void OnResetGame();


	void WinJokboVoice(int nJokboPoint, BOOL bSex, int nSndFxKind);
	void NormalJokboVoice( int nJokboPoint, BOOL bSex, int nSndFxKind);
	void LoseJokboVoice(int nJokboPoint, BOOL bSex, int nSndFxKind);
	
	void OnActionVoice( int r_kind, int r_snum, int r_randnum=0, int r_option=-1  );
	void OnResultVoice(BOOL bMyWin, BOOL bMyFold);
	void OnBetVoice(BOOL bSex,int nSndFxKind, RAISE_LAST_CMD eBetKind, BOOL bHiddenBet);



	


};

CSutdaGameSound& SutdaGameSound();
