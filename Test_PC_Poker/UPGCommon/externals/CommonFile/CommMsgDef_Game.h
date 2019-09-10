#pragma once



//2004.06.29 ���ӽ��۹�ư Ȱ��ȭ
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

//2004.06.29 ���ӽ��� ��û
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

// [��ȣõ��] 2004.07.08 �Ӵ� ���� �޽���
//[���۾ƹ�Ÿ] 2006.04.19
class CCL_CHARGE_MONEY : public CCommMsg
{
public:
	enum CHARGEFLAG{
		CHARGE_ANGEL=1,			// ��ȣ õ��� ä������ �ݾ�
		CHARGE_SUPERAVATAR,		// ���� �ƹ�Ÿ�� ä������ �ݾ�  //[���۾ƹ�Ÿ]
		CHARGE_ALLINANGEL,		// ����õ��� ä������ �ݾ� 	
		CHARGE_LOTTERY,			//�ζ��̺�Ʈ
		CHARGE_CHAMP,			// è�Ǿ� ���
		CHARGE_ATTEVENT,		// �⼮�̺�Ʈ ���(2008.12)
		CHARGE_PREMOPTION_JEKI,	// [������� �۾�] [2009-02-11]
	};

public:

	int*   kind;
	INT64* money;
	char	*ID;
	int*	LeftCnt;		//���� Ƚ�� 
	int*	TotalLeftCnt;	// �� ���� Ƚ��

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

// �� ���� �˸�
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

/////////// ������ �ٲ�
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

/////////// ������ ������� �˸�
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


/////////// ���ӹ�� ä�� ����Ÿ
class CCL_CHATDATA : public CCommMsg
{
public:

	int *RoomNum;
	char *ID;			// ���̵� ��Ʈ��
	char *Kind;			// ä�� �������� ����
	short *l_StrChat;	// ä�� ��Ʈ���� ����
	char *StrChat;		// ä�� ��Ʈ��
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


/////////// ���伣 ī��� �̰��� ��� ��� �����ڿ��� �˸�
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

/////////////�ڸ� ��Ŵ��
class CSV_SEATKEEPER : public CCommMsg
{
public:

	enum SIGNAL
	{
		//���� -> Ŭ���̾�Ʈ �ñ׳�
		SEATKEEPER_SIG_RESERVE,
		SEATKEEPER_SIG_USING,
		SEATKEEPER_SIG_CLOSE, //��Ŵ�� ���� 
		SEATKEEPER_SIG_COUNTINFO, //��Ŵ�� ��� ��밡������ �˷��ش�. 

		//Ŭ���̾�Ʈ -> ���� �ñ׳�
		SEATKEEPER_SIG_ASKRESERVE,	//���� 
		SEATKEEPER_SIG_BACKTOGAME, // ���Ӵٽ� �����ϱ�
	};

	char *Sig;				// �ñ׳�
	char *ReserveID;		// ��� ���̵�
	char *ServPNum;			// ���� ��ȣ 
	char *TurnCnt;			// �� ī��Ʈ 
	char *LeftCnt;			// ���� ���� Ƚ��
	char *MaxCnt;			// �Ϸ� ��� ���� Ƚ��

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


//////// ��� ��û (���̾Ƹ�� ���常 ����)
class CCL_ASK_WAIT_INROOM : public CCommMsg
{
public:
	bool	*m_bWait;	// ���/���� ����
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
//////// ��� ��û ����(���̾Ƹ�� ���常 ����)
class CSV_WAIT_INROOM : public CCommMsg
{
public:
	bool	*m_bWait;	// ���/���� ����
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
 * �� �ִ� �սǾ� ���� ���� ����
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
 * �� �ִ� �սǾ� ���� ���� ����
 */
class CSV_DAILY_LOSS_MONEY : public CCommMsg
{
public:
	IDX_GAME		*idxGame;
	INT64			*llLossMoney;		// ȹ��/�ս� �ݾ� 
	time_t			*tBlockTime;		// ���ܵ� �ð�
	DAILYLOSS_STATE	*eState;			// �ս� ����

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



// [������� �۾�] [2009-02-12]
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
	////////////////////////////����ε����, Ÿ�پ˸� 2017.10.12/////////////////////////////////////
	bool	*m_BlindBet; //����ε� ���ÿ���.. �ο�ٵ��̿����� ��ȿ�ϴ�.
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

////////////////////////////����ε����, Ÿ�پ˸� 2017.10.12/////////////////////////////////////
class CSV_BLINDSHOW : public CCommMsg
{
	//�ٵ��� ����ε� �������Ŀ�, ���� ���� ������ ����ȭ �ϱ� ���� ��Ŷ�̴�.
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

/////////// �������� ����� �뺸
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

/////////// ���� �˸� ������
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

//ù 3���� ������ ������ �ϳ��� ī�带 ���� => ��� ���ý� ���ӽ���
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

// ���� ����
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

// ����
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

// �� ���� ī�峯���� ����
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

// ī�� ���� Ÿ�̸� �˸�
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


// ������ ���� ī�� ���� (���� ����)
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

// ī�� ����
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

/////////// ���� �˸� ������ ��
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



/////////// ���ī�� ����
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



// ��ɷ� �ºθ� �ܷ�ڴ°�
class CCL_HL_SELECTWINCASE : public CCommMsg
{
public:

	int *RoomNum;
	int *ServPNum;
	int *UNum;
	char *ID;
	int  *nCase; // 1: ���� 2: �ο� 3: ����
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


///////////  ī�� ������ �˷��ش�.
class CSV_BD_NOTIFY_CARDEXCHANGE : public CCommMsg
{
public:
	
	int *pCurrType;
	int *NextChangerPNum;		//���� �˸��鼭 ó�� ���� �ٲ�� �ϴ� ���� �˷���

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

/////////   ���ο�ī�带 ��û�� �������� �����ش�.
class CSV_BD_GIVETOUSER_NEWCHANGECARD : public CCommMsg
{
public:

	int *PNum;
	int *ChangedNum;
	int *PosIndex;
	int *CardIndex;
	int *nChangeKind;	// 0 : �Ϲ� ��忡�� �ٲ� ���� , 1 : �ٲٱ� �Ϸ� �޼��� ����
	int *NextChangerPNum;	//�ٲ� ���� ����� �˷���
	BOOL *bLastChanger;			// TRUE ������ �� , FALSE ���� ���� ���� 

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

/////////   ī�� �ٲٱ⸦ ��û�ѵ�.- client to server
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

// ī�� ��ȯ ��
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
/////////// ���� ��� �ٲٱ� ��û
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

/////////// ���� ��� �ٲٱ� ���
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




//��ȣõ�� ���� �Ӵϰ�(�븻,������,�������÷���)
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


//��� ���� �����


// �������� ���̵� Į�� �ٲٱ� ��û
class CCL_CHANGE_IDCOLOR : public CCommMsg
{
public:

	int *nUNum;
	int *nIDColorKind;	//

	void Set(int unum,int idc) // �߰�
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

// �������� ���̵� Į�� �ٲٱ� ��û(���� ���γѵ����� �˷��ش�
class CSV_CHANGE_GAMEUSERINFO : public CCommMsg
{
public:

	CHANGE_GAMEUSERINFO *pCGUI;

	void Set(CHANGE_GAMEUSERINFO *pdata) // �߰�
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


// �������� ����Ÿ �����ֱ�
class CSV_GIVE_TO_DSDATA : public CCommMsg
{
public:

	DS_DATA *pDSD;

	void Set(DS_DATA *pdata) // �߰�
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






// Night �̺�Ʈ ���� 
class CSV_NIGHTEVENT_MSG : public CCommMsg
{
public:	
	int *nKind;				// 0 ��Ȱ�� 1 Ȱ�� 2 ���

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

//����̺�Ʈ
class CSV_ACTIVE_NIGHTEVENT_MSG : public CCommMsg
{
public:

	int *RoomNum;
	int *nActiveNightEventFlg;	// 1 �̺�Ʈ�� �������� �˷��� 100 Ŭ���̾�Ʈ���� ������ ���ӽ��� ��û

	void Set(int rnum,int activeflg) // �߰�
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
���α� �߰� 2008.05.19
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



















// ������ ���� ȯ�漳�� ���� 
class CMS_ADMIN_COMMAND : public CCommMsg
{
public:		
	short *l_StrCommand;	// ä�� ��Ʈ���� ����
	char *StrCommand;		// ä�� ��Ʈ��
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
// ���� ���� ����
class CSV_SD_STARTCARD : public CCommMsg
{
public:
	int *nRoomNum;
	int *nUNum;
	char *szID;
	SD_STARTINFO *stSDStartInfo;
	bool *bRematch;			// ���� �� ��� true, �ƴϸ� false
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
	int *nRoomNum;		// �� ��ȣ
	int *nUNum;			// ���� ��ȣ
	char *szID;			// ���̵�
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
	int *nRoomNum;		// �� ��ȣ
	int *nUNum;			// ���� ��ȣ
	char *szID;			// ���̵�
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


// ���� ���� ����: 
class CSV_SD_CHOICE : public CCommMsg
{
public:
	int *nRoomNum;		// �� ��ȣ
	int *nUNum;			// ���� ��ȣ
	char *szID;			// ���̵�
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

// ���� ���� ����
class CCL_SD_ASK_CHOICE : public CCommMsg
{
public:
	int *nRoomNum;		// �� ��ȣ
	int *nUNum;			// ���� ��ȣ
	char *szID;			// ���̵�
	char *cIndex;		// ���� ī�� ��ġ(0, 1, 2)
	char *cJokbo;		// ���� ī�� �ε���
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

// ���� ���� ����, �� ��ä�� ����
class CSV_SD_ACCEPT_CHOICE : public CCommMsg
{
public:
	int *nRoomNum;		// �� ��ȣ
	int *nUNum;			// ���� ��ȣ
	char *szID;			// ���̵�
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
	int *nRoomNum;		// �� ��ȣ
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
	int *nRoomNum;		// �� ��ȣ
	int *nUNum;			// ���� ��ȣ
	char *szID;			// ���̵�
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


// 49 ���� ����
class CSV_SD_NOTIFY_REMATCH : public CCommMsg
{
public:
	int *nState;
	bool *bWinner;
	bool *bReEntry;		// ���� ���� �����ڰ� �ִ��� ����, true �� ���ð����ڰ� �ְ�, false �� ����
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

// 49 ���� ����
class CSV_SD_49REMATCH : public CCommMsg
{
public:
	int *nRoomNum;			// �� ��ȣ
	int *nUNum;				// ���� ��ȣ
	int *szID;				// ���̵�
	INT64 *nEntryFee;		// ���� ���� ���
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

// ���� ���� ��û
class CCL_SD_ASK_49REMATCH : public CCommMsg
{
public:
	int *nRoomNum;				// �� ��ȣ
	int *nUNum;					// ���� ��ȣ
	char *szID;					// ���� ���̵�
	bool *bReEntry;				// ���� ���� ���� ����(true �� ����, false �� ���� ����)
	INT64 *nEntryFee;			// ���� ������
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

// ���� ���� ���� ���� (�� �� ����)
class CSV_SD_NOTIFY_ASK : public CCommMsg
{
public:
	int *nRoomNum;				// �� ��ȣ
	int *nUNum;					// ���� ��ȣ
	char *szID;					// ���� ���̵�
	INT64 *nEntryFee;			// ���� ������
	INT64 *nBaseMoney;			// �ǵ�(�ٴڿ� �� ��)
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


// ���� �� ����
//class CSV_SD_REMATCH_CARDINFO : public CCommMsg
//{
//public:
//	int *nRoomNum;				// �� ��ȣ
//	int *nUNum;					// ���� ��ȣ
//	char *szID;					// ���� ���̵�
//	char *cCardFirst;			// ���� ī��
//	char *cCardSecond;			// ���� ī��
//	int   *nDistUser;			// ī�� ������
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
// �Ƕ� Begin
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ������ ��ũ ī�� ����
class CCL_ASK_THANK : public CCommMsg
{
public:

	int *RoomNum;
	int *PNum;
	int *UNum;
	char *ID;
	CLTHANKCARD *pCT; // ��� X
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

// ī�� ������ ��û�Ѵ�.
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

// ī�� ������ �����ش�
class CCL_BRINGINCARD : public CCommMsg
{
public:

	int *RoomNum;
	int *PNum;
	int *UNum;
	char *ID;
	int *CardNum; // -1 ����
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


// ī�� ���� ���� �˸�
class CCL_DISCARD : public CCommMsg
{
public:

	int *RoomNum;
	int *PNum;
	int *UNum;
	char *ID;
	int *CardNum; // -1 ����  0 : ����
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

// ī�� ������ ������
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

// ������ ��ũ ī�� ����

// ����� ī�忡 ���� ����
class CSV_REGRESULT : public CCommMsg
{
public:

	int *RoomNum;
	int *PNum;
	int *UNum;
	char *ID;
	REGISTERCARD *RC; // ��� X
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

// ������ ī�� ��� ��û
class CCL_REGISTER : public CCommMsg
{
public:

	int *RoomNum;
	int *PNum;
	int *UNum;
	char *ID;
	USERREGISTERCARD *pUR; // ��� X
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


// �Ƕ� ���� �۾�
// �� �� �ִ� ī�� �о��
class CCL_REGIST : public CCommMsg
{
public:

	int *RoomNum;
	int *PNum;
	int *UNum;
	char *ID;
	REGISTERCARD *RC; // ��� X
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


// ������ ī�� ���̱� ��û
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

// ���� ���ӿ���
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

// ���� ���ӿ��� �Ƹ�
class CSV_STOPGAMERESULT : public CCommMsg
{
public:

	int *RoomNum;
	int *PNum;
	int *UNum;
	char *ID;
	int *Kind; // [ �Ƕ� ] (1 �Ƕ�(ù��), 2 �Ƕ�, 3 ī���ι��� , 4 �뻧, 5 �һ�, 6 stop , 7 ������� ī�� ����)
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
	int *Kind; // [ �Ƕ� ] (1 �Ƕ�(ù��), 2 �Ƕ�, 3 ī���ι��� , 4 �뻧, 5 �һ�, 6 stop , 7 ������� ī�� ����)
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

////////////////////////////�Ƕ� ��ǹ� 2018.03.29/////////////////////////////////////
class CSV_GIVEUP_RULE_RESULT : public CCommMsg
{
	//������ ����ϱ⸦ ���� ����� �������� �˶��̴�.
public:
	int *m_WinnerPNum; //���� �¼���ȣ
	int *m_GiveUpPNum; //����� �¼���ȣ
	INT64 *m_RealWinMoney; //���� ���ڰ� ȹ���� �Ӵ�(���Ӻ������� �ݾ�)
	INT64 *m_RealLoseMoney; //���� ���ڰ� ���� �ݾ�
	INT64 *m_GameFee; //���ӵ���

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

// ��� ���� ����
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

// ��ũ�� ���� ����
class CSV_THANKRESULT : public CCommMsg
{
public:

	int *RoomNum;
	int *PNum;
	int *UNum;
	char *ID;
	THANKYOURESULT *pRT; // ��� X
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

// ��ť �� �� ����
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

/////////// �� ���� ������ �˸� : GameServer -> Client
class CCL_SUN_PICKUP_START : public CCommMsg
{
public:
	int * CardNum;				// ������ ī��� �� �̱⸦ �� ����

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

/////////// �� ����
class CCL_SUN_PICKUP : public CCommMsg
{
public:

	int *CardIndex;			// ������ ������ ī���� �ε����� ���°�� �̾Ҵ��� ������ ������ ����
	int *CardNo;			// �������� ������ �ٽ� �����ٶ�, Spade 2~ Spade 10���� ���� ī�� ��ȣ(CCL_SUN_PICKUP�� �ٽ� ������ �� ������ ī���� ���� ���̸� �˷��ֱ� ����)
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

/////////// �� ���� �Ϸ�
class CCL_SUN_PICKUP_COMPLETE : public CCommMsg
{
public:

	int *pnum;				// ������ ���õ� ���� ��ȣ

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
// ���� ī��
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


// �� �Ŵ���
class CSV_TURNTHANK : public CCommMsg
{
public:	
	char *ID;
	BOOL *CanThank;
	int  *CardNum;
	BOOL *FirstTurn;

	////////////////////////////�Ƕ� ��ǹ� 2018.03.29/////////////////////////////////////
	BOOL *m_GiveUpEnable; //����� �������� ����

	void Set(char *pID, BOOL bCanThank, int nCardNum, BOOL bFirst, BOOL GiveUpEnable) 
	{
		char strid[16]={0,};
		if(pID != NULL) strncpy(strid, pID, 15);
		SetHeader(SV_TURNTHANK);
		PushData(_PVAR(ID), strid, 16);
		PushData(_PVAR(CanThank), &bCanThank, sizeof(BOOL));
		PushData(_PVAR(CardNum), &nCardNum, sizeof(int));
		PushData(_PVAR(FirstTurn), &bFirst, sizeof(BOOL));

		////////////////////////////�Ƕ� ��ǹ� 2018.03.29/////////////////////////////////////
		PushData(_PVAR(m_GiveUpEnable), &GiveUpEnable, sizeof(*m_GiveUpEnable));

	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(ID), 16);
		TakeData(_PVAR(CanThank), sizeof(BOOL));
		TakeData(_PVAR(CardNum), sizeof(int));
		TakeData(_PVAR(FirstTurn), sizeof(BOOL));

		////////////////////////////�Ƕ� ��ǹ� 2018.03.29/////////////////////////////////////
		TakeData(_PVAR(m_GiveUpEnable), sizeof(*m_GiveUpEnable));
	}
};

class CSV_TURNGET : public CCommMsg
{
public:	
	char *TurnID;
	int  *CardNum;
	int  *Magic;

	////////////////////////////�Ƕ� ��ǹ� 2018.03.29/////////////////////////////////////
	BOOL *m_GiveUpEnable; //����� �������� ����

	void Set(char *id, int nCardNum, int nMagic, char nTurnCount, BOOL GiveUpEnable) 
	{
		char strid[16]={0,};
		if(id != NULL) strncpy(strid, id, 15);
		SetHeader(SV_TURNGET);
		PushData(_PVAR(TurnID), strid, 16);
		PushData(_PVAR(CardNum), &nCardNum, sizeof(int));
		PushData(_PVAR(Magic), &nMagic, sizeof(int));

		////////////////////////////�Ƕ� ��ǹ� 2018.03.29/////////////////////////////////////
		PushData(_PVAR(m_GiveUpEnable), &GiveUpEnable, sizeof(*m_GiveUpEnable));
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(TurnID), 16);
		TakeData(_PVAR(CardNum), sizeof(int));
		TakeData(_PVAR(Magic), sizeof(int));

		////////////////////////////�Ƕ� ��ǹ� 2018.03.29/////////////////////////////////////
		TakeData(_PVAR(m_GiveUpEnable), sizeof(*m_GiveUpEnable));
	}
};


class CSV_TURNREG : public CCommMsg
{
public:	
	char *TurnID;

	////////////////////////////�Ƕ� ��ǹ� 2018.03.29/////////////////////////////////////
	BOOL *m_GiveUpEnable; //����� �������� ����

	void Set(char *id, BOOL GiveUpEnable) 
	{
		char strid[16]={0,};
		if(id != NULL) strncpy(strid, id, 15);
		SetHeader(SV_TURNREG);
		PushData(_PVAR(TurnID), strid, 16);

		////////////////////////////�Ƕ� ��ǹ� 2018.03.29/////////////////////////////////////
		PushData(_PVAR(m_GiveUpEnable), &GiveUpEnable, sizeof(*m_GiveUpEnable));
	}
	void Get(char* lpdata, int size)
	{
		if(!GetHeader(lpdata, size)) return;
		TakeData(_PVAR(TurnID), 16);

		////////////////////////////�Ƕ� ��ǹ� 2018.03.29/////////////////////////////////////
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
/////////// �̼�
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

////////////////////////////�Ƕ� ��ǹ� 2018.03.29/////////////////////////////////////
class CCL_PRESS_GIVEUP : public CCommMsg
{
	//���� �ߵ� ����
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
	//��ǹ��� ��ǰ� �ִ� ������ �˷��ش�.
	//�氳��, ���� ����� ��ǹ��� ��� �� ��Ŷ�� ������
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
	//�̹��� ��ǽ� �ҰԵǴ� �Ӵ������� �����ش�.
	//���� ���۽ø��� Ŭ�� �˸�(���ð����� ��� ����� �� ����)
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
////////// ��ť ����?
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

// ���� 3���̻� ������ϰ�� ���� ����
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

// �Ƕ� ��彺������ ����
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

#define MAX_VIP_JACKPOT_COUNT 4			// 1�� �ִ� ȸ��
#define MAX_RANK_LIST_SU 200			// ������ ��ŷ ����
#define MAX_MONTH_RANK_LIST_SU 10		// �Ѵް��� ��ŷ

typedef struct _VIPJACKPOT_RANK_INFO
{		
	BYTE	nGameCode;				// �����ڵ�
	BYTE	nVipJPCount;			// ȸ��
	char	szID[ID_LEN];			// ID
	INT64	nJackpotMoney;//[MAX_GAME_SU][VIPJACKPOT_CHANNEL_MAX];
}VIPJACKPOT_RANK_INFO;

typedef struct _VIPJACKPOT_MONTH_RANK_INFO
{		
	BYTE	nGameCode;				// �����ڵ�
	//BYTE	nVipJPCount;			// ȸ��
	SYSTEMTIME	RegTime;			// ��¥
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
	BYTE	nGameCode;				// �����ڵ�
	BYTE	nVipJPCount;			// ȸ��	
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
	int* nEventCount;						// ȸ��
	int* nEventState;						// ���� ���°�
	MONTH_VIPJACKPOT_RANK_INFO* stMonthRank;		// 10���� ����
	VIPJACKPOT_RANK_INFO* stTodayRank;		// �ִ� 200��	
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
	BYTE	nLeagueCount;			// ȸ��	
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
	bool bPromotion;					// ���θ�� ���ۿ���
	BYTE nLeagueGroup;	//0~9  (0: ���� �ȵ�)

	int  nBetPoint;
	SYSTEMTIME nBeginDay;		// ���� ���� ��¥
	SYSTEMTIME nEndDay;		// ���� ���� ��¥

	bool bReward;		// ture�� ���� ������ �ִ�.
	BYTE nOldLeagueNum;	// ���� ���� ��ȣ////////////////////////////������ �߰��۾� 2017.04.14/////////////////////////////////////
	int nOldLeagueGroupNum;	// ���� ������ �׷��ȣ////////////////////////////������ �߰��۾� 2017.04.14/////////////////////////////////////
	BYTE nOldLeagueRank;	// ���� ���� ��ŷ
	SYSTEMTIME nOldBeginDay;		// ���� ���� ���� ��¥
	SYSTEMTIME nOldEndDay;		// ���� ���� ���� ��¥
	INT64 nRewardMoney;		// ���� ���� �����
	int nGroupNum;		// ���Ե� Ƽ���� �׷� ��ȣ
	BYTE nOldLeagueView;	// ���� ���� �±�
	int nOldBetPoint;		// ���� ���� ����Ʈ
	int nWeek;					// ���� ����

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