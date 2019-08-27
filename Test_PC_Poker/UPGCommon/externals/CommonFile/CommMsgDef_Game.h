#pragma once



//2004.06.29 게임시작버튼 활성화
class CSV_STARTBTNENABLE : public CCommMsg
{
public:
	char *ID;
	int  *Kind;
	void Set(char *id, int nKind) 
	{
		char strid[16]={0,};
		if(id != NULL) strncpy(strid, id, 15);
		SetHeader(SV_STARTBTNENABLE);
		PushData(_PVAR(ID), strid, 16);
		PushData(_PVAR(Kind), &nKind, sizeof(int));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(Kind), sizeof(int) );
	}
};

//2004.06.29 게임시작 요청
class CSV_ASKGAMESTART : public CCommMsg
{
public:
	int		*RoomNum;
	char	*ID;
	void Set(char *id, int rnum) 
	{
		char strid[16]={0,};
		if(id != NULL) strncpy(strid, id, 15);
		SetHeader(SV_ASKGAMESTART);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(ID), strid, 16);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(ID), 16);
	}
};

// [수호천사] 2004.07.08 머니 충전 메시지
//[슈퍼아바타] 2006.04.19
class CCL_CHARGE_MONEY : public CCommMsg
{
public:
	enum CHARGEFLAG{
		CHARGE_ANGEL=1,			// 수호 천사로 채워지는 금액
		CHARGE_SUPERAVATAR,		// 슈퍼 아바타로 채워지는 금액  //[슈퍼아바타]
		CHARGE_ALLINANGEL,		// 올인천사로 채워지는 금액 	
		CHARGE_LOTTERY,			//로또이벤트
		CHARGE_CHAMP,			// 챔피언쉽 상금
		CHARGE_ATTEVENT,		// 출석이벤트 상금(2008.12)
		CHARGE_PREMOPTION_JEKI,	// [재기충전 작업] [2009-02-11]
	};

public:

	int*   kind;
	INT64* money;
	char	*ID;
	int*	LeftCnt;		//남은 횟수 
	int*	TotalLeftCnt;	// 총 남은 횟수

	void Set(char *id, int Kind, INT64 Money , int nLeftCnt=0, int nTotalLeftCnt = 100) 
	{
		char strid[16]={0,};
		if(id != NULL) strncpy(strid, id, 15);
		SetHeader(CL_CHARGE_MONEY);
		PushData(_PVAR(kind), &Kind, sizeof(*kind) );
		PushData(_PVAR(money), &Money, sizeof(*money) );
		PushData(_PVAR(ID), strid, 16);
		PushData(_PVAR(LeftCnt), &nLeftCnt, sizeof(*LeftCnt));
		PushData(_PVAR(TotalLeftCnt), &nTotalLeftCnt, sizeof(*TotalLeftCnt));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(kind), sizeof(*kind) );
		TakeData(_PVAR(money), sizeof(*money) );
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(LeftCnt), sizeof(*LeftCnt) );
		TakeData(_PVAR(TotalLeftCnt), sizeof(*TotalLeftCnt) );
	}
};

// 판 종료 알림
class CSV_ENDGAME : public CCommMsg
{
public:

	int *Reserve;
	void Set(int reserve) 
	{
	
		SetHeader(SV_ENDGAME);
		PushData(_PVAR(Reserve), &reserve, sizeof(*Reserve));
	
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(Reserve), sizeof(*Reserve));
	}
};

/////////// 방장이 바뀜
class CSV_CHANGEROOMMASTER : public CCommMsg
{
public:

	int *RoomNum;
	int *UNum;
	char *ID;
	char *RoomMasterType;
	void Set(int rnum, int unum, char *id, char rmt) 
	{
		char strid[16]={0,};
		if(id != NULL) strncpy(strid, id, 15);
		SetHeader(SV_CHANGEROOMMASTER);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(UNum), &unum, sizeof(*UNum));
		PushData(_PVAR(ID), strid, 16);
		PushData(_PVAR(RoomMasterType), &rmt, sizeof(char));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(UNum), sizeof(*UNum));
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(RoomMasterType), sizeof(char));
	}
};

/////////// 나가기 예약상태 알림
class CCL_EXITSUBSCRIPT : public CCommMsg
{
public:

	int *RoomNum;
	int *UNum;
	char *ID;
	int *bExit;
	int	*bQuickStart;
	void Set(int rnum, int unum, char *id, int bexit, int bq) 
	{
		char strId[16]={0,};
		SetHeader(CL_EXITSUBSCRIPT);
		if(strlen(id) < 16) strcpy(strId, id);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(UNum), &unum, sizeof(*UNum));
		PushData(_PVAR(ID), strId, 16);
		PushData(_PVAR(bExit), &bexit, sizeof(*bExit));
		PushData(_PVAR(bQuickStart), &bq, sizeof(*bQuickStart));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(UNum), sizeof(*UNum));
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(bExit), sizeof(*bExit));
		TakeData(_PVAR(bQuickStart), sizeof(*bQuickStart));
	}
};


/////////// 게임방용 채팅 데이타
class CCL_CHATDATA : public CCommMsg
{
public:

	int *RoomNum;
	char *ID;			// 아이디 스트링
	char *Kind;			// 채팅 데이터의 종류
	short *l_StrChat;	// 채팅 스트링의 길이
	char *StrChat;		// 채팅 스트링
	void Set(int rnum, char *id, char kind, char *chat) 
	{
		char strId[16]={0,};
		if(strlen(id) < 16) strcpy(strId, id);
	
		CString szChat;
		szChat.Format("%s", chat);
		short l_strchat = (short) szChat.GetLength() + 1;	
	
		SetHeader(CL_CHATDATA);
	
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(ID), strId, 16);
		PushData(_PVAR(Kind), &kind, sizeof(*Kind));
		PushData(_PVAR(l_StrChat), &l_strchat, sizeof(*l_StrChat));
		PushData(_PVAR(StrChat), (void*)szChat.operator LPCTSTR(), l_strchat);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(Kind), sizeof(*Kind));
		TakeData(_PVAR(l_StrChat), sizeof(*l_StrChat));
		TakeData(_PVAR(StrChat), *l_StrChat);
	}
};


/////////// 스페샬 카드로 이겼을 경우 모든 접속자에게 알림
class CSV_SPECIALCARD : public CCommMsg
{
public:


	SPECIALCARD *pSC;
	void Set(SPECIALCARD *psc) 
	{
		SetHeader(SV_SPECIALCARD);
		PushData(_PVAR(pSC), psc, sizeof(SPECIALCARD));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(pSC), sizeof(SPECIALCARD) );
	}
};

/////////////자리 지킴이
class CSV_SEATKEEPER : public CCommMsg
{
public:

	enum SIGNAL
	{
		//서버 -> 클라이언트 시그널
		SEATKEEPER_SIG_RESERVE,
		SEATKEEPER_SIG_USING,
		SEATKEEPER_SIG_CLOSE, //지킴이 종료 
		SEATKEEPER_SIG_COUNTINFO, //지킴이 몇번 사용가능한지 알려준다. 

		//클라이언트 -> 서버 시그널
		SEATKEEPER_SIG_ASKRESERVE,	//예약 
		SEATKEEPER_SIG_BACKTOGAME, // 게임다시 참여하기
	};

	char *Sig;				// 시그널
	char *ReserveID;		// 대상 아이디
	char *ServPNum;			// 유저 번호 
	char *TurnCnt;			// 턴 카운트 
	char *LeftCnt;			// 오늘 남은 횟수
	char *MaxCnt;			// 하루 사용 가능 횟수

	void Set(char _Sig, char* _ReserveID, char _PNum, char _TurnCnt, char _LeftCnt, char _MaxCnt) 
	{
		char strReserveId[16]={0,};
		strncpy(strReserveId, _ReserveID, 15);
	
		SetHeader(SV_SEATKEEPER);
	
		PushData(_PVAR(Sig), &_Sig, sizeof(*Sig));
		PushData(_PVAR(ReserveID), strReserveId, 16);
		PushData(_PVAR(ServPNum), &_PNum, sizeof(*ServPNum));
		PushData(_PVAR(TurnCnt), &_TurnCnt, sizeof(*TurnCnt));
		PushData(_PVAR(LeftCnt), &_LeftCnt, sizeof(*LeftCnt));
		PushData(_PVAR(MaxCnt), &_MaxCnt, sizeof(*MaxCnt));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(Sig), sizeof(*Sig));
		TakeData(_PVAR(ReserveID),16);
		TakeData(_PVAR(ServPNum), sizeof(*ServPNum));
		TakeData(_PVAR(TurnCnt), sizeof(*TurnCnt));
		TakeData(_PVAR(LeftCnt), sizeof(*LeftCnt));
		TakeData(_PVAR(MaxCnt), sizeof(*MaxCnt));
	}
};


//////// 대기 요청 (다이아몬드 방장만 가능)
class CCL_ASK_WAIT_INROOM : public CCommMsg
{
public:
	bool	*m_bWait;	// 대기/해제 여부
	void Set(bool bWait)
	{
		SetHeader(CL_ASK_WAIT_INROOM);
		PushData(_PVAR(m_bWait), &bWait, sizeof(*m_bWait) );
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(m_bWait), sizeof(*m_bWait) );
	}
};
//////// 대기 요청 응답(다이아몬드 방장만 가능)
class CSV_WAIT_INROOM : public CCommMsg
{
public:
	bool	*m_bWait;	// 대기/해제 여부
	void Set(bool bWait)
	{
		SetHeader(SV_ASK_WAIT_INROOM);
		PushData(_PVAR(m_bWait), &bWait, sizeof(*m_bWait) );
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(m_bWait), sizeof(*m_bWait) );
	}
};

/**
 * 일 최대 손실액 규제 관련 정보
 */
class CCL_CHECK_DAILY_LOSS_MONEY : public CCommMsg
{
public:
	void Set()
	{
		SetHeader(CL_CHECK_DAILY_LOSS_MONEY);
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
	}
};

/**
 * 일 최대 손실액 규제 관련 정보
 */
class CSV_DAILY_LOSS_MONEY : public CCommMsg
{
public:
	IDX_GAME		*idxGame;
	INT64			*llLossMoney;		// 획득/손실 금액 
	time_t			*tBlockTime;		// 차단된 시간
	DAILYLOSS_STATE	*eState;			// 손실 상태

	void Set(IDX_GAME game, INT64 loss, time_t block_time, DAILYLOSS_STATE state)
	{
		SetHeader(SV_DAILY_LOSS_MONEY);
		PushData(_PVAR(idxGame), &game, sizeof(*idxGame) );
		PushData(_PVAR(llLossMoney), &loss, sizeof(*llLossMoney) );
		PushData(_PVAR(tBlockTime), &block_time, sizeof(*tBlockTime) );
		PushData(_PVAR(eState), &state, sizeof(*eState) );
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(idxGame), sizeof(*idxGame) );
		TakeData(_PVAR(llLossMoney), sizeof(*llLossMoney) );
		TakeData(_PVAR(tBlockTime), sizeof(*tBlockTime) );
		TakeData(_PVAR(eState), sizeof(*eState) );
	}
};



// [재기충전 작업] [2009-02-12]
class CSV_PREM_OPTION_INFO : public CCommMsg
{
public:
	
	PREMOPTION *OptionInfo;

	void Set( PREMOPTION r_OptionInfo )
	{
		SetHeader( SV_PREM_OPTION_INFO );
		PushData( _PVAR(OptionInfo), &r_OptionInfo, sizeof(*OptionInfo) );
	}

	void Get( char* lpdata, int size )
	{
		if( !GetHeader(lpdata, size) ) return;
		TakeData( _PVAR(OptionInfo), sizeof(*OptionInfo) );
	}
};

class CSV_STARTCARD : public CCommMsg
{
public:

	int *RoomNum;
	int *UNum;
	char *ID;
	STARTINFO *pSC;
	////////////////////////////블라인드배팅, 타겟알림 2017.10.12/////////////////////////////////////
	bool	*m_BlindBet; //블라인드 배팅여부.. 로우바둑이에서만 유효하다.
	////////////////////////////

	void Set(int rnum, int unum, char *id, STARTINFO *psc, bool BlindBet) 
	{
		char strId[16]={0,};
		SetHeader(SV_STARTCARD);
		if(strlen(id) < 16) strcpy(strId, id);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(UNum), &unum, sizeof(*UNum));
		PushData(_PVAR(ID), strId, 16);
		PushData(_PVAR(pSC), psc, sizeof(STARTINFO));
		PushData(_PVAR(m_BlindBet), &BlindBet, sizeof(*m_BlindBet));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(UNum), sizeof(*UNum));
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(pSC), sizeof(STARTINFO));
		TakeData(_PVAR(m_BlindBet), sizeof(*m_BlindBet));
	}
};

////////////////////////////블라인드배팅, 타겟알림 2017.10.12/////////////////////////////////////
class CSV_BLINDSHOW : public CCommMsg
{
	//바둑이 블라인드 배팅이후에, 실제 내패 정보를 동기화 하기 위한 패킷이다.
public:
	int *RoomNum;
	int *UNum;
	char *ID;

	BLINDSHOWINFO *pBlindShowInfo;

	void Set(int rnum, int unum, char *id, BLINDSHOWINFO *BlindShowInfo) 
	{
		char strId[16]={0,};
		SetHeader(SV_BLINDSHOW);
		if(strlen(id) < 16) strcpy(strId, id);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(UNum), &unum, sizeof(*UNum));
		PushData(_PVAR(ID), strId, 16);
		PushData(_PVAR(pBlindShowInfo), BlindShowInfo, sizeof(BLINDSHOWINFO));
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(UNum), sizeof(*UNum));
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(pBlindShowInfo), sizeof(BLINDSHOWINFO));
	}
};
//////////////////////

/////////// 게임종료 결과를 통보
class CSV_GAMEOVERRESULT : public CCommMsg
{
public:

	GAMEOVERRESULT *GOR;
	void Set(GAMEOVERRESULT *pGOR) 
	{
		SetHeader(SV_GAMEOVERRESULT);
		PushData(_PVAR(GOR), pGOR, sizeof(GAMEOVERRESULT));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;		
		TakeData(_PVAR(GOR), sizeof(GAMEOVERRESULT) );
	}
};

/////////// 서버 알림 레이즈
class CSV_RAISE : public CCommMsg
{
public:


	POKERGAME *pPG;
	void Set(POKERGAME *ppg) 
	{
		SetHeader(SV_RAISE);
		PushData(_PVAR(pPG), ppg, sizeof(POKERGAME));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(pPG), sizeof(POKERGAME) );
	}
};

//첫 3장을 보낸후 유저가 하나의 카드를 선택 => 모두 선택시 게임시작
class CSV_WINDCARDEND : public CCommMsg
{
public:

	int *Reserve;
	void Set(int reserve) 
	{
	
		SetHeader(SV_WINDCARDEND);
		PushData(_PVAR(Reserve), &reserve, sizeof(*Reserve));
	
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(Reserve), sizeof(*Reserve));
	}
};

// 유저 폴드
class CSV_FOLDUSER : public CCommMsg
{
public:

	int *RoomNum;
	int *UNum;
	int *PNum;
	char *ID;
	int  *Kind;
	INT64 *JackPotMoney;
	INT64 *PMoney;
	void Set(int rnum, int unum, int pnum, char* id, int kind, INT64 jackpot,INT64 money) 
	{
		char strId[16]={0,};
		SetHeader(SV_FOLDUSER);
		if(strlen(id) < 16) strcpy(strId, id);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(UNum), &unum, sizeof(*UNum));
		PushData(_PVAR(PNum), &pnum, sizeof(*PNum));
		PushData(_PVAR(ID), strId, 16);
		PushData(_PVAR(Kind), &kind, sizeof(*Kind));
		PushData(_PVAR(JackPotMoney), &jackpot, sizeof(*JackPotMoney));
		PushData(_PVAR(PMoney), &money, sizeof(*PMoney));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(UNum), sizeof(*UNum));
		TakeData(_PVAR(PNum), sizeof(*PNum));
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(Kind), sizeof(*Kind));
		TakeData(_PVAR(JackPotMoney), sizeof(*JackPotMoney));
		TakeData(_PVAR(PMoney), sizeof(*PMoney));
	}
};

// 폴드
class CCL_ASK_FOLD : public CCommMsg
{
public:

	int *RoomNum;
	int *UNum;
	int *PNum;
	char *ID;
	POKERCLNT *pFD;
	void Set(int rnum, int unum, int pnum, char* id, POKERCLNT *pfd) 
	{
		char strId[16]={0,};
		SetHeader(CL_ASK_FOLD);
		if(strlen(id) < 16) strcpy(strId, id);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(UNum), &unum, sizeof(*UNum));
		PushData(_PVAR(PNum), &pnum, sizeof(*PNum));
		PushData(_PVAR(ID), strId, 16);
		PushData(_PVAR(pFD), pfd, sizeof(POKERCLNT));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(UNum), sizeof(*UNum));
		TakeData(_PVAR(PNum), sizeof(*PNum));
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(pFD), sizeof(POKERCLNT));
	}
};

// Raise
class CCL_ASK_RAISE : public CCommMsg
{
public:

	int *RoomNum;
	int *UNum;
	int *PNum;
	char *ID;
	POKERCLNT *pRA;
	void Set(int rnum, int unum, int pnum, char* id, POKERCLNT *pra) 
	{
		char strId[16]={0,};
		SetHeader(CL_ASK_RAISE);
		if(strlen(id) < 16) strcpy(strId, id);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(UNum), &unum, sizeof(*UNum));
		PushData(_PVAR(PNum), &pnum, sizeof(*PNum));
		PushData(_PVAR(ID), strId, 16);
		PushData(_PVAR(pRA), pra, sizeof(POKERCLNT));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(UNum), sizeof(*UNum));
		TakeData(_PVAR(PNum), sizeof(*PNum));
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(pRA), sizeof(POKERCLNT));
	}
};

// 각 유저 카드날리기 종료
class CCL_WINDCARDEND : public CCommMsg
{
public:

	int *RoomNum;
	int *PNum;
	int *UNum;
	char *ID;

	void Set(int rnum, int pnum, int unum, char* id)
	{
		char strId[16]={0,};
		SetHeader(CL_WINDCARDEND);
		if(strlen(id) < 16) strcpy(strId, id);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(PNum), &pnum, sizeof(*PNum));
		PushData(_PVAR(UNum), &unum, sizeof(*UNum));
		PushData(_PVAR(ID), strId, 16);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(PNum), sizeof(*PNum));
		TakeData(_PVAR(UNum), sizeof(*UNum));
		TakeData(_PVAR(ID), 16);
		
	}
};

// 카드 선택 타이머 알림
class CSV_SELECTCARD : public CCommMsg
{
public:

	int *RoomNum;
	int *UNum;
	int *ServPNum;
	char *ID;
	void Set(int rnum, int unum, int servpnum, char *id) 
	{
		char strid[16]={0,};
		if(id != NULL) strncpy(strid, id, 15);
		SetHeader(SV_SELECTCARD);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(UNum), &unum, sizeof(*UNum));
		PushData(_PVAR(ServPNum), &servpnum, sizeof(*ServPNum));
		PushData(_PVAR(ID), strid, 16);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(UNum), sizeof(*UNum));
		TakeData(_PVAR(ServPNum), sizeof(*ServPNum));
		TakeData(_PVAR(ID), 16);
	}
};


// 마지막 히든 카드 한장 (각각 날림)
class CSV_HIDDENCARD : public CCommMsg
{
public:

	int *RoomNum;
	int *UNum;
	char *ID;
	int  *nCard;
	void Set(int rnum, int unum, char* id, int card) 
	{
		char strId[16]={0,};
		SetHeader(SV_HIDDENCARD);
		if(strlen(id) < 16) strcpy(strId, id);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(UNum), &unum, sizeof(*UNum));
		PushData(_PVAR(ID), strId, 16);
		PushData(_PVAR(nCard), &card, sizeof(*nCard));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(UNum), sizeof(*UNum));
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(nCard), sizeof(*nCard));
	}
};

// 카드 선택
class CCL_ASK_SELECTCARD : public CCommMsg
{
public:

	int *RoomNum;
	int *UNum;
	int *PNum;
	char *ID;
	POKERCLNT *pSD;
	int *DisCard;
	void Set(int rnum, int unum, int pnum, char* id, POKERCLNT *psd, int discard) 
	{
		char strId[16]={0,};
		SetHeader(CL_ASK_SELECTCARD);
		if(strlen(id) < 16) strcpy(strId, id);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(UNum), &unum, sizeof(*UNum));
		PushData(_PVAR(PNum), &pnum, sizeof(*PNum));
		PushData(_PVAR(ID), strId, 16);
		PushData(_PVAR(pSD), psd, sizeof(POKERCLNT));
		PushData(_PVAR(DisCard), &discard, sizeof(*DisCard));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(UNum), sizeof(*UNum));
		TakeData(_PVAR(PNum), sizeof(*PNum));
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(pSD), sizeof(POKERCLNT));
		TakeData(_PVAR(DisCard), sizeof(*DisCard));
	}
};

/////////// 서버 알림 레이즈 끝
class CSV_HL_ENDRAISE : public CCommMsg
{
public:
	int *Flag;	
	void Set(int flag) 
	{
		SetHeader(SV_HL_ENDRAISE);
		PushData(_PVAR(Flag), &flag, sizeof(*Flag));		
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(Flag), sizeof(*Flag) );		
	}
};


class CSV_HL_HLS_SEL : public CCommMsg		//
{
public :

	int* UserNum;

	void Set( int UNum ) 
	{
		SetHeader( SV_HL_HLS_SEL);
		PushData(_PVAR(UserNum), &UNum, sizeof(*UserNum) );
	
	}
	void Get( char* lpdata, int size ) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(UserNum), sizeof(*UserNum) );
	
	}
};



/////////// 백업카드 정보
class CSV_HL_CARDINFO : public CCommMsg
{
public:
	HL_CARDINFO *CI;

	void Set(HL_CARDINFO *pCI) 
	{
		SetHeader(SV_HL_CARDINFO);
		PushData(_PVAR(CI), pCI, sizeof(HL_CARDINFO));
	}
	
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(CI), sizeof(*CI));
	}
};



// 어떤걸로 승부를 겨루겠는가
class CCL_HL_SELECTWINCASE : public CCommMsg
{
public:

	int *RoomNum;
	int *ServPNum;
	int *UNum;
	char *ID;
	int  *nCase; // 1: 하이 2: 로우 3: 스윙
	void Set(int rnum, int servpnum, int unum, char* id, int ncase) 
	{
		char strId[16]={0,};
		SetHeader(CL_HL_SELECTWINCASE);
		if(strlen(id) < 16) strcpy(strId, id);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(ServPNum), &servpnum, sizeof(*ServPNum));
		PushData(_PVAR(UNum), &unum, sizeof(*UNum));
		PushData(_PVAR(ID), strId, 16);
		PushData(_PVAR(nCase), &ncase, sizeof(*nCase));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(ServPNum), sizeof(*ServPNum));
		TakeData(_PVAR(UNum), sizeof(*UNum));
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(nCase), sizeof(*nCase));
	}
};


///////////  카드 선택을 알려준다.
class CSV_BD_NOTIFY_CARDEXCHANGE : public CCommMsg
{
public:
	
	int *pCurrType;
	int *NextChangerPNum;		//컷팅 알리면서 처음 으로 바꿔야 하는 넘을 알려줌

	void Set( int nCurrType , int nextpnum ) 
	{
		SetHeader(SV_BD_NOTIFY_CARDEXCHANGE);		
		PushData(_PVAR(pCurrType), &nCurrType, sizeof(int) );
		PushData(_PVAR(NextChangerPNum), &nextpnum, sizeof(int) );
	}
	void Get( char* lpdata, int size ) 
	{
		if(!GetHeader(lpdata, size)) return;		
		TakeData(_PVAR(pCurrType), sizeof(int) );
		TakeData(_PVAR(NextChangerPNum), sizeof(int) );
	}
};

/////////   새로운카드를 요청한 유저에게 보내준다.
class CSV_BD_GIVETOUSER_NEWCHANGECARD : public CCommMsg
{
public:

	int *PNum;
	int *ChangedNum;
	int *PosIndex;
	int *CardIndex;
	int *nChangeKind;	// 0 : 일반 모드에서 바꾼 정보 , 1 : 바꾸기 완료 메세지 전달
	int *NextChangerPNum;	//바꿀 다음 사람을 알려줌
	BOOL *bLastChanger;			// TRUE 마지막 넘 , FALSE 아직 남아 있음 

	void Set( int nPNum, int nChangedNum, int *pPosIndex, int *pCardIndex , int ckind , int nextpnum ,BOOL lastuser ) 
	{
		SetHeader( SV_BD_GIVETOUSER_NEWCHANGECARD);
		PushData(_PVAR(PNum), &nPNum, sizeof(int) );
		PushData(_PVAR(ChangedNum), &nChangedNum, sizeof(int) );
		PushData(_PVAR(PosIndex), pPosIndex, sizeof(int)* 5 );
		PushData(_PVAR(CardIndex), pCardIndex, sizeof(int)* 5 );
		PushData(_PVAR(nChangeKind), &ckind, sizeof(int) );
		PushData(_PVAR(NextChangerPNum), &nextpnum, sizeof(int) );
		PushData(_PVAR(bLastChanger), &lastuser, sizeof(BOOL) );
		
	
	}
	void Get( char* lpdata, int size ) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(PNum), sizeof(int) );
		TakeData(_PVAR(ChangedNum), sizeof(int) );
		TakeData(_PVAR(PosIndex), sizeof(int)* 5 );
		TakeData(_PVAR(CardIndex), sizeof(int)* 5 );
		TakeData(_PVAR(nChangeKind), sizeof(int) );
		TakeData(_PVAR(NextChangerPNum), sizeof(int) );
		TakeData(_PVAR(bLastChanger), sizeof(BOOL) );
	}
};

/////////   카드 바꾸기를 요청한디.- client to server
class CCL_BD_CARD_EXCHANGE : public CCommMsg
{
public:

	int *RoomNum;
	int *PNum;
	int *UNum;
	char *id;
	int *ChangedNum;
	int *PosIndex;
	int *CardIndex;

	void Set( int nRoomNum, int nPNum, int nUNum, char *pID, int nChangedNum, int *pPosIndex, int *pCardIndex ) 
	{
		char strId[16]={0,};
		if(strlen(pID) < 16) strcpy(strId, pID);
		SetHeader( CL_BD_CARD_EXCHANGE);
		PushData(_PVAR(RoomNum), &nRoomNum, sizeof(int) );
		PushData(_PVAR(PNum), &nPNum, sizeof(int) );
		PushData(_PVAR(UNum), &nUNum, sizeof(int) );
		PushData(_PVAR(id), strId, 16);
		PushData(_PVAR(ChangedNum), &nChangedNum, sizeof(int) );
		PushData(_PVAR(PosIndex), pPosIndex, sizeof(int)* 5 );
		PushData(_PVAR(CardIndex), pCardIndex, sizeof(int)* 5 );
	
	}
	void Get( char* lpdata, int size ) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(int) );
		TakeData(_PVAR(PNum), sizeof(int) );
		TakeData(_PVAR(UNum), sizeof(int) );
		TakeData(_PVAR(id), 16);
		TakeData(_PVAR(ChangedNum), sizeof(int) );
		TakeData(_PVAR(PosIndex), sizeof(int)* 5 );
		TakeData(_PVAR(CardIndex), sizeof(int)* 5 );
	}
};

// 카드 교환 끝
class CCL_BD_CARD_EXCHANGEEND : public CCommMsg
{
public:

	int *RoomNum;
	int *PNum;
	int *UNum;
	char *ID;
	void Set(int rnum, int pnum, int unum, char* id) 
	{
	
		char strId[16]={0,};
		SetHeader(CL_BD_CARD_EXCHANGEEND);
		if(strlen(id) < 16) strcpy(strId, id);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(PNum), &pnum, sizeof(*PNum));
		PushData(_PVAR(UNum), &unum, sizeof(*UNum));
		PushData(_PVAR(ID), strId, 16);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(PNum), sizeof(*PNum));
		TakeData(_PVAR(UNum), sizeof(*UNum));
		TakeData(_PVAR(ID), 16);
	}
};

#if defined(__NEWPOKER__)
/////////// 게임 방식 바꾸기 요청
class CCL_ASK_CHANGEGAMEKIND : public CCommMsg
{
public:

	int *RoomNum;
	int *ServPNum;
	char *ID;
	int *GameKind;
	int *bWhis;
	int *nCard;
	void Set(int rnum, int servpnum, char *id, int gamekind, int bwhis, int ncard) 
	{
		char strId[16]={0,};
		SetHeader(CL_ASK_CHANGEGAMEKIND);
		if(strlen(id) < 16) strcpy(strId, id);

		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(ServPNum), &servpnum, sizeof(*ServPNum));
		PushData(_PVAR(ID), strId, 16);
		PushData(_PVAR(GameKind), &gamekind, sizeof(*GameKind));
		PushData(_PVAR(bWhis), &bwhis, sizeof(*bWhis));
		PushData(_PVAR(nCard), &ncard, sizeof(*nCard));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(ServPNum), sizeof(*ServPNum));
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(GameKind), sizeof(*GameKind));
		TakeData(_PVAR(bWhis), sizeof(*bWhis));
		TakeData(_PVAR(nCard), sizeof(*nCard));
	}
};

/////////// 게임 방식 바꾸기 결과
class CCL_CHANGEGAMEKIND : public CCommMsg
{
public:

	ROOMINFO *pRI;
	void Set(ROOMINFO *pri) 
	{
		SetHeader(CL_CHANGEGAMEKIND);
		PushData(_PVAR(pRI), pri, sizeof(ROOMINFO));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(pRI), sizeof(ROOMINFO));
	}
};



// add bomb card
class CSV_ADD_BOMBCARD: public CCommMsg
{
public:

	BOMBCARD_DATA *pBC;
	void Set(BOMBCARD_DATA *pbc) 
	{
		SetHeader(SV_ADD_BOMBCARD);
		PushData(_PVAR(pBC), pbc, sizeof(BOMBCARD_DATA));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(pBC), sizeof(BOMBCARD_DATA));
	}
};

//add bomb card result
class CCL_ADD_BOMBCARD_RESULT: public CCommMsg
{
public:

	int *RoomNum;
	int *ServPNum;
	char *ID;
	void Set(int rnum, int servpnum, char *id) 
	{
		char strId[16]={0,};
		SetHeader(CL_ADD_BOMBCARD_RESULT);
		if(strlen(id) < 16) strcpy(strId, id);

		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(ServPNum), &servpnum, sizeof(*ServPNum));
		PushData(_PVAR(ID), strId, 16);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(ServPNum), sizeof(*ServPNum));
		TakeData(_PVAR(ID), 16);
	}
};

// open bomb card
class CSV_OPEN_BOMBCARD: public CCommMsg
{
public:

	BOMBCARD_DATA *pBC;
	void Set(BOMBCARD_DATA *pbc) 
	{
		SetHeader(SV_OPEN_BOMBCARD);
		PushData(_PVAR(pBC), pbc, sizeof(BOMBCARD_DATA));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(pBC), sizeof(BOMBCARD_DATA));
	}
};

// open bomb card result
class CCL_OPEN_BOMBCARD_RESULT: public CCommMsg
{
public:

	int *RoomNum;
	int *ServPNum;
	char *ID;
	void Set(int rnum, int servpnum, char *id) 
	{
		char strId[16]={0,};
		SetHeader(CL_OPEN_BOMBCARD_RESULT);
		if(strlen(id) < 16) strcpy(strId, id);

		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(ServPNum), &servpnum, sizeof(*ServPNum));
		PushData(_PVAR(ID), strId, 16);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(ServPNum), sizeof(*ServPNum));
		TakeData(_PVAR(ID), 16);
	}
};




//수호천사 충전 머니값(노말,리더스,리더스플러스)
class CSV_SAFEANGELDATA: public CCommMsg
{
public:

	SAFEANGEL_DATA *pSA;
	void Set(SAFEANGEL_DATA *psa) 
	{
		SetHeader(SV_SAFEANGELDATA);
		PushData(_PVAR(pSA), psa, sizeof(SAFEANGEL_DATA));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(pSA), sizeof(SAFEANGEL_DATA));
	}
};


class CSV_GIVE_JOKERCARD : public CCommMsg
{
public:

	JOKERCARDDATA	*pJD;

	void Set(JOKERCARDDATA *data) 
	{
		SetHeader(SV_GIVE_JOKERCARD);
		PushData(_PVAR(pJD), data, sizeof(JOKERCARDDATA));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(pJD), sizeof(JOKERCARDDATA));
	}
};


//룰방 입장 물어보기


// 도신지수 아이디 칼라 바꾸기 요청
class CCL_CHANGE_IDCOLOR : public CCommMsg
{
public:

	int *nUNum;
	int *nIDColorKind;	//

	void Set(int unum,int idc) // 추가
	{
		SetHeader(CL_CHANGE_IDCOLOR);
		PushData(_PVAR(nUNum), &unum, sizeof(*nUNum));
		PushData(_PVAR(nIDColorKind), &idc, sizeof(*nIDColorKind));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(nUNum), sizeof(*nUNum));		
		TakeData(_PVAR(nIDColorKind), sizeof(*nIDColorKind));
	}
};

// 도신지수 아이디 칼라 바꾸기 요청(게임 중인넘들한테 알려준다
class CSV_CHANGE_GAMEUSERINFO : public CCommMsg
{
public:

	CHANGE_GAMEUSERINFO *pCGUI;

	void Set(CHANGE_GAMEUSERINFO *pdata) // 추가
	{
		SetHeader(SV_CHANGE_GAMEUSERINFO);
		PushData(_PVAR(pCGUI), pdata, sizeof(CHANGE_GAMEUSERINFO));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(pCGUI), sizeof(CHANGE_GAMEUSERINFO));
	}
};


// 도신지수 데이타 보내주기
class CSV_GIVE_TO_DSDATA : public CCommMsg
{
public:

	DS_DATA *pDSD;

	void Set(DS_DATA *pdata) // 추가
	{
		SetHeader(SV_GIVE_TO_DSDATA);
		PushData(_PVAR(pDSD),pdata, sizeof(DS_DATA));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(pDSD), sizeof(DS_DATA));
	}
};	






// Night 이벤트 시작 
class CSV_NIGHTEVENT_MSG : public CCommMsg
{
public:	
	int *nKind;				// 0 비활성 1 활성 2 대기

	short *l_strloby;
	char *strLobyNote;
	short *l_strgame;	
	char *strGameNote;	

	void Set(int kind ,char *strloby ,char *strgame) 
	{
		short l_strlobychat = (short)strlen(strloby);
		short l_strgamechat = (short)strlen(strgame);

		SetHeader(SV_NIGHTEVENT_MSG);
		PushData(_PVAR(nKind), &kind, sizeof(*nKind));
		PushData(_PVAR(l_strloby), &l_strlobychat, sizeof(*l_strloby));
		PushData(_PVAR(strLobyNote), strloby, l_strlobychat);		
		PushData(_PVAR(l_strgame), &l_strgamechat, sizeof(*l_strgame));		
		PushData(_PVAR(strGameNote), strgame, l_strgamechat);		
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;	
		TakeData(_PVAR(nKind), sizeof(*nKind));
		TakeData(_PVAR(l_strloby), sizeof(*l_strloby));		
		TakeData(_PVAR(strLobyNote), *l_strloby);	
		TakeData(_PVAR(l_strgame), sizeof(*l_strgame));
		TakeData(_PVAR(strGameNote), *l_strgame);				
	}
};

//골드이벤트
class CSV_ACTIVE_NIGHTEVENT_MSG : public CCommMsg
{
public:

	int *RoomNum;
	int *nActiveNightEventFlg;	// 1 이벤트를 서버에서 알려줌 100 클라이언트에서 서버로 게임시작 요청

	void Set(int rnum,int activeflg) // 추가
	{
		SetHeader(SV_ACTIVE_NIGHTEVENT_MSG);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(nActiveNightEventFlg), &activeflg, sizeof(*nActiveNightEventFlg));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(nActiveNightEventFlg), sizeof(*nActiveNightEventFlg));
	}
};




/*
통계로그 추가 2008.05.19
*/
class CMS_NOTIFY_LOG_CHANNEL_USER : public CCommMsg
{
public:
	CHANNEL_USERNUMBER *pUserNumber;
	void Set( CHANNEL_USERNUMBER* puserNumber ) 
	{		
		SetHeader( MS_NOTIFY_LOG_CHANNEL_USER );
		PushData( _PVAR(pUserNumber), puserNumber, sizeof(CHANNEL_USERNUMBER) );
	}
	void Get( char* lpdata, int size ) 
	{
		if( !GetHeader( lpdata, size ) ) return;
		TakeData( _PVAR(pUserNumber), sizeof( CHANNEL_USERNUMBER ) );
	}

};



















// 관리자 게임 환경설정 세팅 
class CMS_ADMIN_COMMAND : public CCommMsg
{
public:		
	short *l_StrCommand;	// 채팅 스트링의 길이
	char *StrCommand;		// 채팅 스트링
	void Set(char *command) 
	{		
		short len_command = (short)strlen(command);	

		SetHeader(MS_ADMIN_COMMAND);		
		PushData(_PVAR(l_StrCommand), &len_command, sizeof(*l_StrCommand));
		PushData(_PVAR(StrCommand), command, len_command);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(l_StrCommand), sizeof(*l_StrCommand));
		TakeData(_PVAR(StrCommand), *l_StrCommand);
	}

};

#endif	// defined(__NEWPOKER__)

#if defined(__SUTDA__)
// 섯다 게임 시작
class CSV_SD_STARTCARD : public CCommMsg
{
public:
	int *nRoomNum;
	int *nUNum;
	char *szID;
	SD_STARTINFO *stSDStartInfo;
	bool *bRematch;			// 재경기 일 경우 true, 아니면 false
	void Set(int RoomNum, int UNum, char *szId, SD_STARTINFO *StartInfo, bool rematch)
	{
		char strId[16]={0,};
		SetHeader(SV_STARTCARD);
		if(strlen(szId) < 16) strcpy(strId, szId);

		PushData(_PVAR(nRoomNum), &RoomNum, sizeof(int));
		PushData(_PVAR(nUNum), &UNum, sizeof(int));
		PushData(_PVAR(szID), strId, 16);
		PushData(_PVAR(stSDStartInfo), StartInfo, sizeof(SD_STARTINFO));
		PushData(_PVAR(bRematch), &rematch, sizeof(bool));
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(nRoomNum), sizeof(*nRoomNum));
		TakeData(_PVAR(nUNum), sizeof(*nUNum));
		TakeData(_PVAR(szID), 16);
		TakeData(_PVAR(stSDStartInfo), sizeof(SD_STARTINFO));
		TakeData(_PVAR(bRematch), sizeof(bool));
	}
};

class CCL_SD_JJOGI : public CCommMsg
{
public:
	int *nRoomNum;		// 방 번호
	int *nUNum;			// 유저 번호
	char *szID;			// 아이디
	void Set(int RoomNum, int ServPNum, char *szId)
	{
		char strId[16] = {0,};
		SetHeader(CL_SD_JJOGI);
		if(strlen(szId) < 16) strcpy(strId, szId);

		PushData(_PVAR(nRoomNum), &RoomNum, sizeof(*nRoomNum));
		PushData(_PVAR(nUNum), &ServPNum, sizeof(*nUNum));
		PushData(_PVAR(szID), strId, 16);
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(nRoomNum), sizeof(*nRoomNum));
		TakeData(_PVAR(nUNum), sizeof(*nUNum));
		TakeData(_PVAR(szID), 16);
	}
};

class CSV_SD_NOTIFY_JJOGI : public CCommMsg
{
public:
	int *nRoomNum;		// 방 번호
	int *nUNum;			// 유저 번호
	char *szID;			// 아이디
	void Set(int RoomNum, int ServPNum, char *szId)
	{
		char strId[16] = {0,};
		SetHeader(SV_SD_NOTIFY_JJOGI);
		if(strlen(szId) < 16) strcpy(strId, szId);

		PushData(_PVAR(nRoomNum), &RoomNum, sizeof(*nRoomNum));
		PushData(_PVAR(nUNum), &ServPNum, sizeof(*nUNum));
		PushData(_PVAR(szID), strId, 16);
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(nRoomNum), sizeof(*nRoomNum));
		TakeData(_PVAR(nUNum), sizeof(*nUNum));
		TakeData(_PVAR(szID), 16);
	}
};


// 족보 선택 공지: 
class CSV_SD_CHOICE : public CCommMsg
{
public:
	int *nRoomNum;		// 방 번호
	int *nUNum;			// 유저 번호
	char *szID;			// 아이디
	void Set(int RoomNum, int ServPNum, char *szId)
	{
		char strId[16] = {0,};
		SetHeader(SV_SD_CHOICE);
		if(strlen(szId) < 16) strcpy(strId, szId);

		PushData(_PVAR(nRoomNum), &RoomNum, sizeof(*nRoomNum));
		PushData(_PVAR(nUNum), &ServPNum, sizeof(*nUNum));
		PushData(_PVAR(szID), strId, 16);
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(nRoomNum), sizeof(*nRoomNum));
		TakeData(_PVAR(nUNum), sizeof(*nUNum));
		TakeData(_PVAR(szID), 16);
	}
};

// 에서 족보 선택
class CCL_SD_ASK_CHOICE : public CCommMsg
{
public:
	int *nRoomNum;		// 방 번호
	int *nUNum;			// 유저 번호
	char *szID;			// 아이디
	char *cIndex;		// 버릴 카드 위치(0, 1, 2)
	char *cJokbo;		// 버릴 카드 인덱스
	void Set(int RoomNum, int UNum, char *szId, char Index, char Jokbo)
	{
		char strId[16] = {0,};
		SetHeader(CL_SD_ASK_CHOICE);
		if(strlen(szId) < 16) strcpy(strId, szId);

		PushData(_PVAR(nRoomNum), &RoomNum, sizeof(*nRoomNum));
		PushData(_PVAR(nUNum), &UNum, sizeof(*nUNum));
		PushData(_PVAR(szID), szId, 16);
		PushData(_PVAR(cIndex), &Index, sizeof(*cIndex));
		PushData(_PVAR(cJokbo), &Jokbo, sizeof(*cJokbo));
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(nRoomNum), sizeof(*nRoomNum));
		TakeData(_PVAR(nUNum), sizeof(*nUNum));
		TakeData(_PVAR(szID), 16);
		TakeData(_PVAR(cIndex), sizeof(*cIndex));
		TakeData(_PVAR(cJokbo), sizeof(*cJokbo));
	}
};

// 족보 선택 수락, 방 전채에 전달
class CSV_SD_ACCEPT_CHOICE : public CCommMsg
{
public:
	int *nRoomNum;		// 방 번호
	int *nUNum;			// 유저 번호
	char *szID;			// 아이디
	void Set(int RoomNum, int UNum, int PNum, char *szId)
	{
		char strId[16] = {0,};
		SetHeader(SV_SD_ACCEPT_CHOICE);
		if(strlen(szId) < 16) strcpy(strId, szId);

		PushData(_PVAR(nRoomNum), &RoomNum, sizeof(*nRoomNum));
		PushData(_PVAR(nUNum), &UNum, sizeof(*nUNum));
		PushData(_PVAR(szID), szId, 16);
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(nRoomNum), sizeof(*nRoomNum));
		TakeData(_PVAR(nUNum), sizeof(*nUNum));
		TakeData(_PVAR(szID), 16);
	}
};

class CSV_SD_RESULTCARD : public CCommMsg
{
public:
	int *nRoomNum;		// 방 번호
	SD_RESULTCARD *ResultInfo;
	void Set(int RoomNum, SD_RESULTCARD *Info)
	{
		SetHeader(SV_SD_RESULTCARD);

		PushData(_PVAR(nRoomNum), &RoomNum, sizeof(int));
		PushData(_PVAR(ResultInfo), Info, sizeof(SD_RESULTCARD));
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(nRoomNum), sizeof(int));
		TakeData(_PVAR(ResultInfo), sizeof(SD_RESULTCARD));
	}
};

class CCL_SD_RESULTCARD : public CCommMsg
{
public:
	int *nRoomNum;		// 방 번호
	int *nUNum;			// 유저 번호
	char *szID;			// 아이디
	void Set(int RoomNum, int UNum, int PNum, char *szId)
	{
		char strId[16] = {0,};
		SetHeader(CL_SD_RESULTCARD);
		if(strlen(szId) < 16) strcpy(strId, szId);

		PushData(_PVAR(nRoomNum), &RoomNum, sizeof(*nRoomNum));
		PushData(_PVAR(nUNum), &UNum, sizeof(*nUNum));
		PushData(_PVAR(szID), szId, 16);
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(nRoomNum), sizeof(*nRoomNum));
		TakeData(_PVAR(nUNum), sizeof(*nUNum));
		TakeData(_PVAR(szID), 16);
	}
};


// 49 재경기 공지
class CSV_SD_NOTIFY_REMATCH : public CCommMsg
{
public:
	int *nState;
	bool *bWinner;
	bool *bReEntry;		// 재경기 참가 선택자가 있는지 유무, true 면 선택가능자가 있고, false 면 없음
	void Set(int state, bool winner, bool reEntry)
	{
		SetHeader(SV_SD_NOTIFY_REMATCH);
		PushData(_PVAR(nState), &state, sizeof(int));
		PushData(_PVAR(bWinner), &winner, sizeof(bool));
		PushData(_PVAR(bReEntry), &reEntry, sizeof(bool));
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(nState), sizeof(int));
		TakeData(_PVAR(bWinner), sizeof(bool));
		TakeData(_PVAR(bReEntry), sizeof(bool));
	}
};

// 49 재경기 공지
class CSV_SD_49REMATCH : public CCommMsg
{
public:
	int *nRoomNum;			// 방 번호
	int *nUNum;				// 유저 번호
	int *szID;				// 아이디
	INT64 *nEntryFee;		// 재경기 참가 비용
	void Set(int RoomNum, int UNum, char *szId, INT64 EntryFee)
	{
		char strId[16] = {0,};
		SetHeader(SV_SD_49REMATCH);
		if(strlen(szId) < 16) strcpy(strId, szId);
		PushData(_PVAR(nRoomNum), &RoomNum, sizeof(*nRoomNum));
		PushData(_PVAR(nUNum), &UNum, sizeof(*nUNum));
		PushData(_PVAR(szID), strId, 16);
		PushData(_PVAR(nEntryFee), &EntryFee, sizeof(*nEntryFee));
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(nRoomNum), sizeof(*nRoomNum));
		TakeData(_PVAR(nUNum), sizeof(*nUNum));
		TakeData(_PVAR(szID), 16);
		TakeData(_PVAR(nEntryFee), sizeof(*nEntryFee));
	}
};

// 재경기 참가 요청
class CCL_SD_ASK_49REMATCH : public CCommMsg
{
public:
	int *nRoomNum;				// 방 번호
	int *nUNum;					// 유저 번호
	char *szID;					// 유저 아이디
	bool *bReEntry;				// 재경기 참가 수락 유무(true 면 참가, false 면 참가 안함)
	INT64 *nEntryFee;			// 재경기 참가비
	void Set(int RoomNum, int UNum, char *szId, bool reEntry, INT64 EntryFee)
	{
		char strId[16] = {0,};
		SetHeader(CL_SD_ASK_49REMATCH);
		if(strlen(szId) < 16) strcpy(strId, szId);

		PushData(_PVAR(nRoomNum), &RoomNum, sizeof(*nRoomNum));
		PushData(_PVAR(nUNum), &UNum, sizeof(*nUNum));
		PushData(_PVAR(szID), strId, 16);
		PushData(_PVAR(bReEntry), &reEntry, sizeof(bool));
		PushData(_PVAR(nEntryFee), &EntryFee, sizeof(*nEntryFee));
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(nRoomNum), sizeof(*nRoomNum));
		TakeData(_PVAR(nUNum), sizeof(*nUNum));
		TakeData(_PVAR(szID), 16);
		TakeData(_PVAR(bReEntry), sizeof(bool));
		TakeData(_PVAR(nEntryFee), sizeof(*nEntryFee));
	}
};

// 재경기 재경기 참가 공지 (방 내 공지)
class CSV_SD_NOTIFY_ASK : public CCommMsg
{
public:
	int *nRoomNum;				// 방 번호
	int *nUNum;					// 유저 번호
	char *szID;					// 유저 아이디
	INT64 *nEntryFee;			// 재경기 참가비
	INT64 *nBaseMoney;			// 판돈(바닥에 깔린 돈)
	void Set(int RoomNum, int UNum, char *szId, INT64 EntryFee, INT64 BaseMoney)
	{
		char strId[16] = {0,};
		SetHeader(SV_SD_NOTIFY_ASK);
		if(strlen(szId) < 16) strcpy(strId, szId);

		PushData(_PVAR(nRoomNum), &RoomNum, sizeof(*nRoomNum));
		PushData(_PVAR(nUNum), &UNum, sizeof(*nUNum));
		PushData(_PVAR(szID), strId, 16);
		PushData(_PVAR(nEntryFee), &EntryFee, sizeof(*nEntryFee));
		PushData(_PVAR(nBaseMoney), &BaseMoney, sizeof(*nBaseMoney));
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(nRoomNum), sizeof(*nRoomNum));
		TakeData(_PVAR(nUNum), sizeof(*nUNum));
		TakeData(_PVAR(szID), 16);
		TakeData(_PVAR(nEntryFee), sizeof(*nEntryFee));
		TakeData(_PVAR(nBaseMoney), sizeof(*nBaseMoney));
	}
};


// 재경기 패 전달
//class CSV_SD_REMATCH_CARDINFO : public CCommMsg
//{
//public:
//	int *nRoomNum;				// 방 번호
//	int *nUNum;					// 유저 번호
//	char *szID;					// 유저 아이디
//	char *cCardFirst;			// 재경기 카드
//	char *cCardSecond;			// 재경기 카드
//	int   *nDistUser;			// 카드 날리기
//	void Set(int RoomNum, int UNum, int PNum, char *szId, char CardFirst, char CardSecond, int distUser)
//	{
//		char strId[16] = {0,};
//		SetHeader(SV_SD_REMATCH_CARDINFO);
//		if(strlen(szId) < 16) strcpy(strId, szId);
//
//		PushData(_PVAR(nRoomNum), &RoomNum, sizeof(*nRoomNum));
//		PushData(_PVAR(nUNum), &UNum, sizeof(*nUNum));
//		PushData(_PVAR(szID), strId, 16);
//		PushData(_PVAR(cCardFirst), &CardFirst, sizeof(cCardFirst));
//		PushData(_PVAR(cCardSecond), &CardSecond, sizeof(cCardSecond));
//		PushData(_PVAR(nDistUser), &distUser, sizeof(int));
//	}
//	void Get(char* lpdata, int size)
//	{
//		if(!GetHeader(lpdata, size)) return;
//
//		TakeData(_PVAR(nRoomNum), sizeof(*nRoomNum));
//		TakeData(_PVAR(nUNum), sizeof(*nUNum));
//		TakeData(_PVAR(szID), 16);
//		TakeData(_PVAR(cCardFirst), sizeof(*cCardFirst));
//		TakeData(_PVAR(cCardSecond), sizeof(*cCardSecond));
//		TakeData(_PVAR(nDistUser), sizeof(int));
//	}
//};
#endif	// defined(__SUTDA__)


#if defined(__HOOLA__)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 훌라 Begin
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 유저가 탱크 카드 선택
class CCL_ASK_THANK : public CCommMsg
{
public:

	int *RoomNum;
	int *PNum;
	int *UNum;
	char *ID;
	CLTHANKCARD *pCT; // 사용 X
	void Set(int rnum, int pnum, int unum, char* id, CLTHANKCARD *pct) 
	{
		char strId[16]={0,};
		SetHeader(CL_ASK_THANK);
		if(strlen(id) < 16) strcpy(strId, id);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(PNum), &pnum, sizeof(*PNum));
		PushData(_PVAR(UNum), &unum, sizeof(*UNum));
		PushData(_PVAR(ID), strId, 16);
		PushData(_PVAR(pCT), pct, sizeof(CLTHANKCARD));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(PNum), sizeof(*PNum));
		TakeData(_PVAR(UNum), sizeof(*UNum));
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(pCT), sizeof(CLTHANKCARD));
	}
};

// 카드 한장을 요청한다.
class CCL_ASK_ONECARD : public CCommMsg
{
public:

	int *RoomNum;
	int *PNum;
	int *UNum;
	char *ID;
	void Set(int rnum, int pnum, int unum, char* id) //, POKERCLNT *pwd) 
	{
		char strId[16]={0,};
		SetHeader(CL_ASK_ONECARD);
		if(strlen(id) < 16) strcpy(strId, id);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(PNum), &pnum, sizeof(*PNum));
		PushData(_PVAR(UNum), &unum, sizeof(*UNum));
		PushData(_PVAR(ID), strId, 16);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(PNum), sizeof(*PNum));
		TakeData(_PVAR(UNum), sizeof(*UNum));
		TakeData(_PVAR(ID), 16);
	}
};

// 카드 한장을 보내준다
class CCL_BRINGINCARD : public CCommMsg
{
public:

	int *RoomNum;
	int *PNum;
	int *UNum;
	char *ID;
	int *CardNum; // -1 에러
	INT64 *JackpotMoney;
	void Set(int rnum, int pnum, int unum, char* id, int cardnum , INT64& nJackpotMoney) 
	{
		char strId[16]={0,};
		SetHeader(CL_BRINGINCARD);
		if(strlen(id) < 16) strcpy(strId, id);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(PNum), &pnum, sizeof(*PNum));
		PushData(_PVAR(UNum), &unum, sizeof(*UNum));
		PushData(_PVAR(ID), strId, 16);
		PushData(_PVAR(CardNum), &cardnum, sizeof(*CardNum));
		PushData(_PVAR(JackpotMoney), &nJackpotMoney, sizeof(INT64));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(PNum), sizeof(*PNum));
		TakeData(_PVAR(UNum), sizeof(*UNum));
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(CardNum), sizeof(*CardNum));
		TakeData(_PVAR(JackpotMoney), sizeof(INT64));
	}
};


// 카드 버림 서버 알림
class CCL_DISCARD : public CCommMsg
{
public:

	int *RoomNum;
	int *PNum;
	int *UNum;
	char *ID;
	int *CardNum; // -1 에러  0 : 없다
	void Set(int rnum, int pnum, int unum, char* id, int cardnum) 
	{
		char strId[16]={0,};
		SetHeader(CL_DISCARD);
		if(strlen(id) < 16) strcpy(strId, id);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(PNum), &pnum, sizeof(*PNum));
		PushData(_PVAR(UNum), &unum, sizeof(*UNum));
		PushData(_PVAR(ID), strId, 16);
		PushData(_PVAR(CardNum), &cardnum, sizeof(*CardNum));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(PNum), sizeof(*PNum));
		TakeData(_PVAR(UNum), sizeof(*UNum));
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(CardNum), sizeof(*CardNum));
	}
};

// 카드 한장을 버린다
class CSV_DISCARD : public CCommMsg
{
public:

	int *RoomNum;
	int *PNum;
	int *UNum;
	char *ID;
	THANKYOUCARD *pTC;
	void Set(int rnum, int pnum, int unum, char* id,  THANKYOUCARD *ptc) 
	{
		char strId[16]={0,};
		SetHeader(SV_DISCARD);
		if(strlen(id) < 16) strcpy(strId, id);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(PNum), &pnum, sizeof(*PNum));
		PushData(_PVAR(UNum), &unum, sizeof(*UNum));
		PushData(_PVAR(ID), strId, 16);
		PushData(_PVAR(pTC), ptc, sizeof(THANKYOUCARD));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(PNum), sizeof(*PNum));
		TakeData(_PVAR(UNum), sizeof(*UNum));
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(pTC), sizeof(THANKYOUCARD));
	}
};

// 유저가 탱크 카드 선택

// 등록한 카드에 대한 응답
class CSV_REGRESULT : public CCommMsg
{
public:

	int *RoomNum;
	int *PNum;
	int *UNum;
	char *ID;
	REGISTERCARD *RC; // 사용 X
	void Set(int rnum, int pnum, int unum, char* id, REGISTERCARD *pRC) 
	{
		char strId[16]={0,};
		SetHeader(SV_REGRESULT);
		if(strlen(id) < 16) strcpy(strId, id);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(PNum), &pnum, sizeof(*PNum));
		PushData(_PVAR(UNum), &unum, sizeof(*UNum));
		PushData(_PVAR(ID), strId, 16);
		PushData(_PVAR(RC), pRC, sizeof(REGISTERCARD));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(PNum), sizeof(*PNum));
		TakeData(_PVAR(UNum), sizeof(*UNum));
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(RC), sizeof(REGISTERCARD));
	}
};

// 유저가 카드 등록 요청
class CCL_REGISTER : public CCommMsg
{
public:

	int *RoomNum;
	int *PNum;
	int *UNum;
	char *ID;
	USERREGISTERCARD *pUR; // 사용 X
	void Set(int rnum, int pnum, int unum, char* id, USERREGISTERCARD *pur) 
	{
		char strId[16]={0,};
		SetHeader(CL_REGISTER);
		if(strlen(id) < 16) strcpy(strId, id);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(PNum), &pnum, sizeof(*PNum));
		PushData(_PVAR(UNum), &unum, sizeof(*UNum));
		PushData(_PVAR(ID), strId, 16);
		PushData(_PVAR(pUR), pur, sizeof(USERREGISTERCARD));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(PNum), sizeof(*PNum));
		TakeData(_PVAR(UNum), sizeof(*UNum));
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(pUR), sizeof(USERREGISTERCARD));
	}
};


// 훌라 개선 작업
// 낼 수 있는 카드 털어내기
class CCL_REGIST : public CCommMsg
{
public:

	int *RoomNum;
	int *PNum;
	int *UNum;
	char *ID;
	REGISTERCARD *RC; // 사용 X
	void Set(int rnum, int pnum, int unum, char* id, REGISTERCARD* pRC) 
	{
		char strId[16]={0,};
		SetHeader(CL_REGIST);
		if(strlen(id) < 16) strcpy(strId, id);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(PNum), &pnum, sizeof(*PNum));
		PushData(_PVAR(UNum), &unum, sizeof(*UNum));
		PushData(_PVAR(ID), strId, 16);
		PushData(_PVAR(RC), pRC, sizeof(REGISTERCARD));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(PNum), sizeof(*PNum));
		TakeData(_PVAR(UNum), sizeof(*UNum));
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(RC), sizeof(REGISTERCARD));
	}
};


// 유저가 카드 붙이기 요청
class CCL_ATTACH : public CCommMsg
{
public:

	int *RoomNum;
	int *PNum;
	int *UNum;
	char *ID;
	USERATTACHCARD *pUA;
	void Set(int rnum, int pnum, int unum, char* id, USERATTACHCARD *pua) 
	{
		char strId[16]={0,};
		SetHeader(CL_ATTACH);
		if(strlen(id) < 16) strcpy(strId, id);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(PNum), &pnum, sizeof(*PNum));
		PushData(_PVAR(UNum), &unum, sizeof(*UNum));
		PushData(_PVAR(ID), strId, 16);
		PushData(_PVAR(pUA), pua, sizeof(USERATTACHCARD));

	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(PNum), sizeof(*PNum));
		TakeData(_PVAR(UNum), sizeof(*UNum));
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(pUA), sizeof(USERATTACHCARD));

	}
};

// 스톱 게임오버
class CCL_STOPGAME : public CCommMsg
{
public:
	enum
	{
		eSTOP_TYPE_STOP		= 0,
		eSTOP_TYPE_DABANG	= 1,
		eSTOP_TYPE_SOBANG	= 2,
		eSTOP_TYPE_SEVEN4	= 3,
	};

	int *RoomNum;
	int *PNum;
	int *UNum;
	char *ID;
	int  *nType;
	void Set(int rnum, int pnum, int unum, char* id, int ntype) 
	{
		char strId[16]={0,};
		SetHeader(CL_STOPGAME);
		if(strlen(id) < 16) strcpy(strId, id);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(PNum), &pnum, sizeof(*PNum));
		PushData(_PVAR(UNum), &unum, sizeof(*UNum));
		PushData(_PVAR(ID), strId, 16);
		PushData(_PVAR(nType), &ntype, sizeof(*nType));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(PNum), sizeof(*PNum));
		TakeData(_PVAR(UNum), sizeof(*UNum));
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(nType), sizeof(*nType));
	}
};

// 스톱 게임오버 아림
class CSV_STOPGAMERESULT : public CCommMsg
{
public:

	int *RoomNum;
	int *PNum;
	int *UNum;
	char *ID;
	int *Kind; // [ 훌라 ] (1 훌라(첫턴), 2 훌라, 3 카드모두버림 , 4 대빵, 5 소빵, 6 stop , 7 엎어놓은 카드 없음)
	void Set(int rnum, int pnum, int unum, char* id, int kind) 
	{
		char strId[16]={0,};
		SetHeader(SV_STOPGAMERESULT);
		if(strlen(id) < 16) strcpy(strId, id);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(PNum), &pnum, sizeof(*PNum));
		PushData(_PVAR(UNum), &unum, sizeof(*UNum));
		PushData(_PVAR(ID), strId, 16);
		PushData(_PVAR(Kind), &kind, sizeof(*Kind));

	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(PNum), sizeof(*PNum));
		TakeData(_PVAR(UNum), sizeof(*UNum));
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(Kind), sizeof(*Kind));
	}
};


class CSV_SPECIALRULERESULT : public CCommMsg
{
public:

	int *RoomNum;
	int *PNum;
	int *UNum;
	char *ID;
	int *Kind; // [ 훌라 ] (1 훌라(첫턴), 2 훌라, 3 카드모두버림 , 4 대빵, 5 소빵, 6 stop , 7 엎어놓은 카드 없음)
	char *Card;
	void Set(int rnum, int pnum, int unum, char* id, int kind, char *pCard) 
	{
		char strId[16]={0,};
		SetHeader(SV_SPECIALRULERESULT);
		if(strlen(id) < 16) strcpy(strId, id);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(PNum), &pnum, sizeof(*PNum));
		PushData(_PVAR(UNum), &unum, sizeof(*UNum));
		PushData(_PVAR(ID), strId, 16);
		PushData(_PVAR(Kind), &kind, sizeof(*Kind));
		PushData(_PVAR(Card), pCard, 8);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(PNum), sizeof(*PNum));
		TakeData(_PVAR(UNum), sizeof(*UNum));
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(Kind), sizeof(*Kind));
		TakeData(_PVAR(Card), 8);
	}
};

////////////////////////////훌라 기권방 2018.03.29/////////////////////////////////////
class CSV_GIVEUP_RULE_RESULT : public CCommMsg
{
	//누군가 기권하기를 통해 결과가 나왔을때 알람이다.
public:
	int *m_WinnerPNum; //승자 좌석번호
	int *m_GiveUpPNum; //기권한 좌석번호
	INT64 *m_RealWinMoney; //실제 승자가 획득한 머니(게임비제외한 금액)
	INT64 *m_RealLoseMoney; //실제 패자가 잃은 금액
	INT64 *m_GameFee; //게임딜비

	void Set(int WinnerPNum, int GiveUpPNum, INT64 RealWinMoney, INT64 RealLoseMoney, INT64 GameFee) 
	{
		SetHeader(SV_GIVEUP_RULE_RESULT);

		PushData(_PVAR(m_WinnerPNum), &WinnerPNum, sizeof(*m_WinnerPNum));
		PushData(_PVAR(m_GiveUpPNum), &GiveUpPNum, sizeof(*m_GiveUpPNum));
		PushData(_PVAR(m_RealWinMoney), &RealWinMoney, sizeof(*m_RealWinMoney));
		PushData(_PVAR(m_RealLoseMoney), &RealLoseMoney, sizeof(*m_RealLoseMoney));
		PushData(_PVAR(m_GameFee), &GameFee, sizeof(*m_GameFee));
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_WinnerPNum), sizeof(*m_WinnerPNum));
		TakeData(_PVAR(m_GiveUpPNum), sizeof(*m_GiveUpPNum));
		TakeData(_PVAR(m_RealWinMoney), sizeof(*m_RealWinMoney));
		TakeData(_PVAR(m_RealLoseMoney), sizeof(*m_RealLoseMoney));
		TakeData(_PVAR(m_GameFee), sizeof(*m_GameFee));
	}
};
///////////////////////////////////////////////////////////

// 계속 게임 진행
class CCL_ASK_CONTINUE : public CCommMsg
{
public:

	int *RoomNum;
	int *PNum;
	int *UNum;
	char *ID;
	void Set(int rnum, int pnum, int unum, char* id) 
	{
		char strId[16]={0,};
		SetHeader(CL_ASK_CONTINUE);
		if(strlen(id) < 16) strcpy(strId, id);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(PNum), &pnum, sizeof(*PNum));
		PushData(_PVAR(UNum), &unum, sizeof(*UNum));
		PushData(_PVAR(ID), strId, 16);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(PNum), sizeof(*PNum));
		TakeData(_PVAR(UNum), sizeof(*UNum));
		TakeData(_PVAR(ID), 16);
	}
};

// 탱크에 대한 응답
class CSV_THANKRESULT : public CCommMsg
{
public:

	int *RoomNum;
	int *PNum;
	int *UNum;
	char *ID;
	THANKYOURESULT *pRT; // 사용 X
	void Set(int rnum, int pnum, int unum, char* id, THANKYOURESULT *prt) 
	{
		char strId[16]={0,};
		SetHeader(SV_THANKRESULT);
		if(strlen(id) < 16) strcpy(strId, id);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(PNum), &pnum, sizeof(*PNum));
		PushData(_PVAR(UNum), &unum, sizeof(*UNum));
		PushData(_PVAR(ID), strId, 16);
		PushData(_PVAR(pRT), prt, sizeof(THANKYOURESULT));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(PNum), sizeof(*PNum));
		TakeData(_PVAR(UNum), sizeof(*UNum));
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(pRT), sizeof(THANKYOURESULT));
	}
};

// 땡큐 할 수 있음
class CSV_CANTHANK : public CCommMsg
{
public:

	int *RoomNum;
	int *PNum;
	int *UNum;
	char *ID;
	void Set(int rnum, int pnum, int unum, char* id) 
	{
		char strId[16]={0,};
		SetHeader(SV_CANTHANK);
		if(strlen(id) < 16) strcpy(strId, id);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(PNum), &pnum, sizeof(*PNum));
		PushData(_PVAR(UNum), &unum, sizeof(*UNum));
		PushData(_PVAR(ID), strId, 16);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(PNum), sizeof(*PNum));
		TakeData(_PVAR(UNum), sizeof(*UNum));
		TakeData(_PVAR(ID), 16);
	}
};

class CSV_ADDEDCARDDATA : public CCommMsg
{
public:

	char *StCards;
	char *ToCards;
	char *ID;
	int  *DisCard;
	char *PlayerCard;

	void Set( char pStCards[4][13], char pToCards[13][4], char *pID, int nDisCard, char pPlayerCard[8] ) 
	{
		SetHeader(SV_ADDEDCARDDATA);
		PushData(_PVAR(StCards), pStCards, sizeof(char)*13*4);
		PushData(_PVAR(ToCards), pToCards, sizeof(char)*13*4);
		PushData(_PVAR(ID), pID, 15 );
		PushData(_PVAR(DisCard), &nDisCard, sizeof(int) );
		PushData(_PVAR(PlayerCard), pPlayerCard, sizeof(char)*8);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(StCards), sizeof(char)*13*4);
		TakeData(_PVAR(ToCards), sizeof(char)*13*4);
		TakeData(_PVAR(ID), 15 );
		TakeData(_PVAR(DisCard), sizeof(int) );
		TakeData(_PVAR(PlayerCard), sizeof(char)*8);
	}
};


class CSV_FIXREGIST : public CCommMsg
{
public:
	int* PNum;
	char *StCards;
	char *ToCards;
	char *ID;
	int  *DisCard;
	char *PlayerCard;

	void Set( int nPN, char pStCards[4][13], char pToCards[13][4], char *pID, int nDisCard, char pPlayerCard[8] ) 
	{
		SetHeader(SV_FIXREGIST);
		PushData(_PVAR(PNum), &nPN, sizeof(int) );
		PushData(_PVAR(StCards), pStCards, sizeof(char)*13*4);
		PushData(_PVAR(ToCards), pToCards, sizeof(char)*13*4);
		PushData(_PVAR(ID), pID, 15 );
		PushData(_PVAR(DisCard), &nDisCard, sizeof(int) );
		PushData(_PVAR(PlayerCard), pPlayerCard, sizeof(char)*8);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(PNum), sizeof( int ) );
		TakeData(_PVAR(StCards), sizeof(char)*13*4);
		TakeData(_PVAR(ToCards), sizeof(char)*13*4);
		TakeData(_PVAR(ID), 15 );
		TakeData(_PVAR(DisCard), sizeof(int) );
		TakeData(_PVAR(PlayerCard), sizeof(char)*8);
	}
};

/////////// 선 고르기 시작을 알림 : GameServer -> Client
class CCL_SUN_PICKUP_START : public CCommMsg
{
public:
	int * CardNum;				// 몇장의 카드로 선 뽑기를 할 건지

	void Set( int TotalCardNum )
	{
		SetHeader(CL_SUN_PICKUP_START);
		PushData(_PVAR(CardNum), &TotalCardNum, sizeof(*CardNum));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(CardNum), sizeof(*CardNum));
	}
};

/////////// 선 고르기
class CCL_SUN_PICKUP : public CCommMsg
{
public:

	int *CardIndex;			// 유저가 뒤집힌 카드의 인덱스중 몇번째를 뽑았는지 서버에 보내기 위해
	int *CardNo;			// 서버에서 유저로 다시 보내줄때, Spade 2~ Spade 10등의 실제 카드 번호(CCL_SUN_PICKUP을 다시 보내줄 때 선택한 카드의 실제 무늬를 알려주기 위해)
	int *rnum;
	int *pnum;


	void Set( int recv_cardIndex, int recv_rnum, int recv_pnum, int send_cardNo ) 
	{
		SetHeader( CL_SUN_PICKUP);
		PushData(_PVAR(CardIndex), &recv_cardIndex, sizeof(*CardIndex) );
		PushData(_PVAR(CardNo), &send_cardNo, sizeof(*CardNo) );
		PushData(_PVAR(rnum), &recv_rnum, sizeof(*rnum) );
		PushData(_PVAR(pnum), &recv_pnum, sizeof(*pnum) );
	}
	void Get(char* lpdata, int size ) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(CardIndex), sizeof(*CardIndex) );
		TakeData(_PVAR(CardNo), sizeof(*CardNo) );
		TakeData(_PVAR(rnum), sizeof(*rnum) );
		TakeData(_PVAR(pnum), sizeof(*pnum) );

	}
};

/////////// 선 고르기 완료
class CCL_SUN_PICKUP_COMPLETE : public CCommMsg
{
public:

	int *pnum;				// 선으로 선택된 유저 번호

	void Set( int iWinnerPnum ) 
	{
		SetHeader( CL_SUN_PICKUP_COMPLETE);
		PushData(_PVAR(pnum), &iWinnerPnum, sizeof(*pnum) );
	}
	void Get(char* lpdata, int size ) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(pnum), sizeof(*pnum) );
	}
};

///////////////////////////////////////////////////////////////////////////////
// 매직 카드
class CSV_MAGICCARD : public CCommMsg
{
public:
	int *CardNum;
	int *CardList;
	void Set( int nCardNum, int* pCardList )
	{
		SetHeader(SV_MAGICCARD);
		PushData(_PVAR(CardNum), &nCardNum, sizeof(int) );
		PushData(_PVAR(CardList), pCardList, sizeof(int)*nCardNum);
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(CardNum), sizeof(int));
		TakeData(_PVAR(CardList), sizeof(int)*(*CardNum));
	}
};


// 턴 매니저
class CSV_TURNTHANK : public CCommMsg
{
public:	
	char *ID;
	BOOL *CanThank;
	int  *CardNum;
	BOOL *FirstTurn;

	////////////////////////////훌라 기권방 2018.03.29/////////////////////////////////////
	BOOL *m_GiveUpEnable; //기권이 가능한지 여부

	void Set(char *pID, BOOL bCanThank, int nCardNum, BOOL bFirst, BOOL GiveUpEnable) 
	{
		char strid[16]={0,};
		if(pID != NULL) strncpy(strid, pID, 15);
		SetHeader(SV_TURNTHANK);
		PushData(_PVAR(ID), strid, 16);
		PushData(_PVAR(CanThank), &bCanThank, sizeof(BOOL));
		PushData(_PVAR(CardNum), &nCardNum, sizeof(int));
		PushData(_PVAR(FirstTurn), &bFirst, sizeof(BOOL));

		////////////////////////////훌라 기권방 2018.03.29/////////////////////////////////////
		PushData(_PVAR(m_GiveUpEnable), &GiveUpEnable, sizeof(*m_GiveUpEnable));

	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(CanThank), sizeof(BOOL));
		TakeData(_PVAR(CardNum), sizeof(int));
		TakeData(_PVAR(FirstTurn), sizeof(BOOL));

		////////////////////////////훌라 기권방 2018.03.29/////////////////////////////////////
		TakeData(_PVAR(m_GiveUpEnable), sizeof(*m_GiveUpEnable));
	}
};

class CSV_TURNGET : public CCommMsg
{
public:	
	char *TurnID;
	int  *CardNum;
	int  *Magic;

	////////////////////////////훌라 기권방 2018.03.29/////////////////////////////////////
	BOOL *m_GiveUpEnable; //기권이 가능한지 여부

	void Set(char *id, int nCardNum, int nMagic, char nTurnCount, BOOL GiveUpEnable) 
	{
		char strid[16]={0,};
		if(id != NULL) strncpy(strid, id, 15);
		SetHeader(SV_TURNGET);
		PushData(_PVAR(TurnID), strid, 16);
		PushData(_PVAR(CardNum), &nCardNum, sizeof(int));
		PushData(_PVAR(Magic), &nMagic, sizeof(int));

		////////////////////////////훌라 기권방 2018.03.29/////////////////////////////////////
		PushData(_PVAR(m_GiveUpEnable), &GiveUpEnable, sizeof(*m_GiveUpEnable));
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(TurnID), 16);
		TakeData(_PVAR(CardNum), sizeof(int));
		TakeData(_PVAR(Magic), sizeof(int));

		////////////////////////////훌라 기권방 2018.03.29/////////////////////////////////////
		TakeData(_PVAR(m_GiveUpEnable), sizeof(*m_GiveUpEnable));
	}
};


class CSV_TURNREG : public CCommMsg
{
public:	
	char *TurnID;

	////////////////////////////훌라 기권방 2018.03.29/////////////////////////////////////
	BOOL *m_GiveUpEnable; //기권이 가능한지 여부

	void Set(char *id, BOOL GiveUpEnable) 
	{
		char strid[16]={0,};
		if(id != NULL) strncpy(strid, id, 15);
		SetHeader(SV_TURNREG);
		PushData(_PVAR(TurnID), strid, 16);

		////////////////////////////훌라 기권방 2018.03.29/////////////////////////////////////
		PushData(_PVAR(m_GiveUpEnable), &GiveUpEnable, sizeof(*m_GiveUpEnable));
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(TurnID), 16);

		////////////////////////////훌라 기권방 2018.03.29/////////////////////////////////////
		TakeData(_PVAR(m_GiveUpEnable), sizeof(*m_GiveUpEnable));
	}
};


class CSV_THANK_FAIL : public CCommMsg
{
public:
	char *id;

	void Set( char *pID )
	{
		char buf[16] = {0, };
		strncpy( buf, pID, 15 );
		SetHeader( SV_THANK_FAIL );
		PushData( _PVAR(id), buf, 16 );
	}
	void Get( char* lpdata, int size ) 
	{
		if( !GetHeader( lpdata, size ) ) return;		
		TakeData( _PVAR(id), 16 );		
	}
};
/////////// 미션
class CSV_MISSION : public CCommMsg
{
public:

	int* MissionNum;
	int* MissionBonus;
	int* MaxMission;
	void Set( int nMNum, int nMBonus, int nMaxM ) 
	{

		SetHeader(SV_MISSION);

		PushData(_PVAR(MissionNum), &nMNum, sizeof(int) );
		PushData(_PVAR(MissionBonus), &nMBonus, sizeof(int) );
		PushData(_PVAR(MaxMission), &nMaxM, sizeof(int) );
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(MissionNum), sizeof(int));
		TakeData(_PVAR(MissionBonus), sizeof(int));
		TakeData(_PVAR(MaxMission), sizeof(int));
	}
};

class CSV_MISSIONCOMPLETE : public CCommMsg
{
public:

	char* ID;
	int* GetMoney;
	void Set( char* pID, int& Money ) 
	{

		SetHeader(SV_MISSIONCOMPLETE);

		PushData(_PVAR(ID), pID, sizeof(char)*16 );
		PushData(_PVAR(GetMoney), &Money, sizeof(int) );
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(ID), sizeof(char)*16);
		TakeData(_PVAR(GetMoney), sizeof(int));
	}
};

////////////////////////////훌라 기권방 2018.03.29/////////////////////////////////////
class CCL_PRESS_GIVEUP : public CCommMsg
{
	//게임 중도 포기
public:

	void Set() 
	{
		SetHeader(CL_PRESS_GIVEUP);
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
	}
};

class CSV_GIVEUP_RULE_NOTI : public CCommMsg
{
	//기권방룰로 운영되고 있는 방임을 알려준다.
	//방개설, 유저 입장시 기권방일 경우 이 패킷을 보내줌
public:

	void Set() 
	{
		SetHeader(SV_GIVEUP_RULE_NOTI);
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
	}
};

class CSV_GIVEUP_LOSE_MONEY_NOTI : public CCommMsg
{
	//이번판 기권시 잃게되는 머니정보를 보내준다.
	//매판 시작시마다 클라에 알림(셋팅값등이 계속 변경될 수 있음)
public:

	INT64* m_LoseMoney;

	void Set(INT64 LoseMoney) 
	{
		SetHeader(SV_GIVEUP_LOSE_MONEY_NOTI);

		PushData(_PVAR(m_LoseMoney), &LoseMoney, sizeof(*m_LoseMoney));
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_LoseMoney), sizeof(*m_LoseMoney));
	}
};
///////////////////////////////////////////////

//CL_PRESS_THANK
////////// 땡큐 가능?
class CCL_PRESS_THANK : public CCommMsg
{
public:

	int *RoomNum;
	int *PNum;
	int *UNum;
	char *ID;
	char *MyCard;
	void Set(int rnum, int pnum, int unum, char* id, char *pMyCard) 
	{
		char strId[16]={0,};
		SetHeader(CL_PRESS_THANK);
		if(strlen(id) < 16) strcpy(strId, id);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(PNum), &pnum, sizeof(*PNum));
		PushData(_PVAR(UNum), &unum, sizeof(*UNum));
		PushData(_PVAR(ID), strId, 16);
		PushData(_PVAR(MyCard), pMyCard, sizeof(char)*8);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(PNum), sizeof(*PNum));
		TakeData(_PVAR(UNum), sizeof(*UNum));
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(MyCard), sizeof(char)*8);
	}
};

class CCL_ENDCARDMOVE : public CCommMsg
{
public:

	int *RoomNum;
	int *PNum;
	int *UNum;
	char *ID;
	char *EndFlag;
	void Set(int rnum, int pnum, int unum, char* id, char cEndFlag ) 
	{
		char strId[16]={0,};
		SetHeader(CL_ENDCARDMOVE);
		if(strlen(id) < 16) strcpy(strId, id);
		PushData(_PVAR(RoomNum), &rnum, sizeof(*RoomNum));
		PushData(_PVAR(PNum), &pnum, sizeof(*PNum));
		PushData(_PVAR(UNum), &unum, sizeof(*UNum));
		PushData(_PVAR(ID), strId, 16);
		PushData(_PVAR(EndFlag), &cEndFlag, sizeof(char));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(*RoomNum));
		TakeData(_PVAR(PNum), sizeof(*PNum));
		TakeData(_PVAR(UNum), sizeof(*UNum));
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(EndFlag), sizeof(char));
	}
};

class CCL_REQGAMESTART : public CCommMsg
{
public:

	int  *RoomNum;
	char *ID;
	void Set( int nRN, char *pID ) 
	{
		SetHeader(CL_REQGAMESTART);
		PushData(_PVAR(RoomNum), &nRN, sizeof(int));
		PushData(_PVAR(ID), pID, 16);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(int));
		TakeData(_PVAR(ID), 16);
	}
};

class CCL_REQGAMEPRACTICS : public CCommMsg
{
public:

	int  *RoomNum;
	char *ID;
	void Set( int nRN, char *pID ) 
	{
		SetHeader(CL_REQGAMEPRACTICS);
		PushData(_PVAR(RoomNum), &nRN, sizeof(int));
		PushData(_PVAR(ID), pID, 16);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(RoomNum), sizeof(int));
		TakeData(_PVAR(ID), 16);
	}
};

// 연속 3번이상 스톱승일경우 잭팟 지급
class CSV_JACKPOTCOUNT : public CCommMsg
{
public:	
	char *ID;
	int  *Count;

	void Set(char* pID, int nCount ) 
	{
		char strid[ ID_LEN ] = { 0,};
		if ( NULL != pID ) strncpy(strid, pID,  ID_LEN - 1 );
		SetHeader(SV_JACKPOTCOUNT);
		PushData(_PVAR(ID), pID, ID_LEN );
		PushData(_PVAR(Count), &nCount, sizeof(int));
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(ID), ID_LEN );
		TakeData(_PVAR(Count), sizeof(int));
	}
};

// 훌라 골드스테이지 정보
struct GOLD_STAGE_INFO
{
	bool bGoldHoola;
	int  nGoldStage;
};

/*
class CMS_ADD_AUTOUSER : public CCommMsg
{
public:
	char *ID;

	void Set(char *id) 
	{
		char strid[16]={0,};
		if(id != NULL) strncpy(strid, id, 15);
		SetHeader(MS_ADD_AUTOUSER);
		PushData(_PVAR(ID), strid, 16);
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(ID), 16);
	}
};

class CMS_DEL_AUTOUSER : public CCommMsg
{
public:
	char *ID;

	void Set(char *id) 
	{
		char strid[16]={0,};
		if(id != NULL) strncpy(strid, id, 15);
		SetHeader(MS_DEL_AUTOUSER);
		PushData(_PVAR(ID), strid, 16);
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(ID), 16);
	}
};
*/





#endif



class CCV_REQVIP_RANK_INFO: public CCommMsg
{
public:	
	void Set( ) 
	{
		SetHeader(CL_VIP_RANK_INFO);		
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;		
	}
};

#define MAX_VIP_JACKPOT_COUNT 4			// 1일 최대 회차
#define MAX_RANK_LIST_SU 200			// 오늘의 랭킹 정보
#define MAX_MONTH_RANK_LIST_SU 10		// 한달간의 랭킹

typedef struct _VIPJACKPOT_RANK_INFO
{		
	BYTE	nGameCode;				// 게임코드
	BYTE	nVipJPCount;			// 회차
	char	szID[ID_LEN];			// ID
	INT64	nJackpotMoney;//[MAX_GAME_SU][VIPJACKPOT_CHANNEL_MAX];
}VIPJACKPOT_RANK_INFO;

typedef struct _VIPJACKPOT_MONTH_RANK_INFO
{		
	BYTE	nGameCode;				// 게임코드
	//BYTE	nVipJPCount;			// 회차
	SYSTEMTIME	RegTime;			// 날짜
	char	szID[ID_LEN];			// ID
	INT64	nJackpotMoney;//[MAX_GAME_SU][VIPJACKPOT_CHANNEL_MAX];
}VIPJACKPOT_MONTH_RANK_INFO;

typedef struct _TODAY_VIPJACKPOT_RANK_INFO
{		
	VIPJACKPOT_RANK_INFO TodayRank[MAX_RANK_LIST_SU];
}TODAY_VIPJACKPOT_RANK_INFO;

typedef struct _MONTH_VIPJACKPOT_RANK_INFO
{		
	VIPJACKPOT_MONTH_RANK_INFO MonthRank[MAX_MONTH_RANK_LIST_SU];
}MONTH_VIPJACKPOT_RANK_INFO;

typedef struct _VIPJACKPOT_EVENT_ROOM_INFO
{		
	BYTE	nGameCode;				// 게임코드
	BYTE	nVipJPCount;			// 회차	
	int		nRoomNum;
	INT64	nJackpotMoney;//[MAX_GAME_SU][VIPJACKPOT_CHANNEL_MAX];
}VIPJACKPOT_EVENT_ROOM_INFO;

typedef struct _VIPJACKPOT_EVENT_ROOM_LIST
{		
	_VIPJACKPOT_EVENT_ROOM_INFO RoomList[MAX_MONTH_RANK_LIST_SU];
}VIPJACKPOT_EVENT_ROOM_LIST;

class CSV_REQVIP_RANK_INFO: public CCommMsg
{
public:	

	int* nListSize;
	int* nEventCount;						// 회차
	int* nEventState;						// 현제 상태값
	MONTH_VIPJACKPOT_RANK_INFO* stMonthRank;		// 10개로 고정
	VIPJACKPOT_RANK_INFO* stTodayRank;		// 최대 200개	
	void Set( int ListSize ,const MONTH_VIPJACKPOT_RANK_INFO* pMonthRank ,const VIPJACKPOT_RANK_INFO* pTodayRank ,int EventCount , int EventState) 
	{
		SetHeader(SV_VIP_RANK_INFO);		

		PushData(_PVAR(nListSize), &ListSize, sizeof(int));
		PushData(_PVAR(stMonthRank), pMonthRank, sizeof(MONTH_VIPJACKPOT_RANK_INFO));
		PushData(_PVAR(stTodayRank), pTodayRank, sizeof(VIPJACKPOT_RANK_INFO) * (ListSize) );		
		PushData(_PVAR(nEventCount), &EventCount, sizeof(int));
		PushData(_PVAR(nEventState), &EventState, sizeof(int));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;		
		TakeData(_PVAR(nListSize), sizeof(*nListSize));
		TakeData(_PVAR(stMonthRank), sizeof(*stMonthRank));
		TakeData(_PVAR(stTodayRank), sizeof(VIPJACKPOT_RANK_INFO) * (*nListSize));
		TakeData(_PVAR(nEventCount), sizeof(int));
		TakeData(_PVAR(nEventState), sizeof(int));
	}
};


#if defined (__LEAGUE2017__)
typedef struct _GET_LEAGUE_REWARD
{	
	BYTE	nLeagueCount;			// 회차	
}GET_LEAGUE_REWARD;

class CSV_GET_LEAGUE_REWARD : public CCommMsg
{
public:

	int *nLeagueCount;
	void Set(int leagueCount) 
	{
		SetHeader(CL_GET_LEAGUE_REWARD_MONEY);
		PushData(_PVAR(nLeagueCount), &leagueCount, sizeof(*nLeagueCount));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(nLeagueCount), sizeof(*nLeagueCount));
	}
};


class CSV_LEAGUE_REWARD_MONEY : public CCommMsg
{
public:

	INT64 *nRewardMoney;
	void Set(INT64 rewardMoney) 
	{
		SetHeader(SV_LEAGUE_REWARD_MONEY);
		PushData(_PVAR(nRewardMoney), &rewardMoney, sizeof(*nRewardMoney));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(nRewardMoney), sizeof(*nRewardMoney));
	}
};

typedef struct _LEAGUE_LOGIN_INFO
{
	bool bPromotion;					// 프로모션 시작여부
	BYTE nLeagueGroup;	//0~9  (0: 참여 안됨)

	int  nBetPoint;
	SYSTEMTIME nBeginDay;		// 리그 시작 날짜
	SYSTEMTIME nEndDay;		// 리그 종료 날짜

	bool bReward;		// ture면 보상 받을게 있다.
	BYTE nOldLeagueNum;	// 지난 리그 번호////////////////////////////리그전 추가작업 2017.04.14/////////////////////////////////////
	int nOldLeagueGroupNum;	// 지난 리그의 그룹번호////////////////////////////리그전 추가작업 2017.04.14/////////////////////////////////////
	BYTE nOldLeagueRank;	// 지난 리그 랭킹
	SYSTEMTIME nOldBeginDay;		// 지난 리그 시작 날짜
	SYSTEMTIME nOldEndDay;		// 지난 리그 종료 날짜
	INT64 nRewardMoney;		// 지난 리그 보상금
	int nGroupNum;		// 포함된 티어의 그룹 번호
	BYTE nOldLeagueView;	// 지난 리그 승급
	int nOldBetPoint;		// 지난 리그 포인트
	int nWeek;					// 현재 주차

} LEAGUE_LOGIN_INFO;

class CSV_LEAGUE_LOGIN_INFO : public CCommMsg
{
public:
	LEAGUE_LOGIN_INFO *	stLeagueInfo;
	void Set(LEAGUE_LOGIN_INFO* pLeagueInfo) 
	{
		SetHeader(SV_LEAGUE_LOGIN_INFO);
		PushData(_PVAR(stLeagueInfo), pLeagueInfo, sizeof(LEAGUE_LOGIN_INFO));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(stLeagueInfo), sizeof(LEAGUE_LOGIN_INFO));
	}
};

class CSV_GET_LEAGUE_ICON : public CCommMsg
{
public:

	char *ID;
	bool *bLeague;	
	void Set(char* pID,bool League) 
	{
		char strid[ ID_LEN ] = { 0,};
		if ( NULL != pID ) strncpy(strid, pID,  ID_LEN - 1 );		

		SetHeader(SV_LEAGUE_NO_ICON);
		PushData(_PVAR(ID), pID, ID_LEN );		
		PushData(_PVAR(bLeague), &League, sizeof(*bLeague));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		
		TakeData(_PVAR(ID), ID_LEN );
		TakeData(_PVAR(bLeague), sizeof(*bLeague));
	}
};

class CSV_SET_LEAGUE_ICON : public CCommMsg
{
public:

	char *ID;
	bool *bLeague;	

	void Set(char* pID,bool League) 
	{
		char strid[ ID_LEN ] = { 0,};
		if ( NULL != pID ) strncpy(strid, pID,  ID_LEN - 1 );		

		SetHeader(SV_LEAGUE_NO_ICON);
		PushData(_PVAR(ID), pID, ID_LEN );		
		PushData(_PVAR(bLeague), &League, sizeof(*bLeague));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		
		TakeData(_PVAR(ID), ID_LEN );
		TakeData(_PVAR(bLeague), sizeof(*bLeague));
	}
};


class CSV_ADD_LEAGUE_BET_POINT : public CCommMsg
{
public:

	int *nLeaguePoint;
	void Set(int leaguePoint) 
	{
		SetHeader(SV_LEAGUE_BET_POINT);
		PushData(_PVAR(nLeaguePoint), &leaguePoint, sizeof(*nLeaguePoint));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(nLeaguePoint), sizeof(*nLeaguePoint));
	}
};

class CSV_SET_IS_LEAGUE : public CCommMsg
{
public:
	
	bool *bLeague;	

	void Set(bool League) 
	{
		SetHeader(SV_LEAGUE_IS_LEAGUE);		
		PushData(_PVAR(bLeague), &League, sizeof(*bLeague));
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(bLeague), sizeof(*bLeague));
	}
};

class CSV_GET_MY_LEAGUE_INFO : public CCommMsg
{
public:

	void Set() 
	{
		SetHeader(CL_GET_MY_LEAGUE_INFO);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;		
	}
};

class CSV_SET_OLDLEAGUE_VIEW : public CCommMsg
{
public:

	void Set() 
	{
		SetHeader(CL_SET_OLDLEAGUE_VIEW);
	}
	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;		
	}
};
#endif