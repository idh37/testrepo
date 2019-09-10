#pragma once

#define SIZE_UMCODE (9)	//���� �Ӵ� �ڵ� ���� ������ 
#define CN_LEN (16)		//CN ���ۻ�����
#define ID_LEN (16)		//���̵� ���ۻ�����
#define IP_LEN (24)		//IP ���ۻ�����
#define NICK_LEN (20)
#define DI_LEN (64)
#define MAC_LEN (18)	//MAC Address

////////////////////////////Ŭ���ý��� 2017.12.21/////////////////////////////////////
//������ �ڵ�� �迭�� ���ڸ� ����ְ� �־�, �ش� �����Ͱ� �������� ���� ���ǵɶ�, ���������� ������ �߻��� ������ ����.
//�Ͽ� �̹����� �迭�� ���� ����Ʈ���� �̰��� �����Ͽ� �������� ���� �ϰ��� ��.
#define CHAR_BUFFER_CLUB_NAME 50
#define CHAR_BUFFER_CLUB_BOSS_ID 50
/////////////////////////////////////////////////

////////////////////////////2018�����̺�Ʈ 2018.01.22/////////////////////////////////////
#define CHAR_BUFFER_PROMOTION_COMMON_GAMECOUNT_ADDINFO 100
#define CHAR_BUFFER_PROMOTION_COMMON_GAMECOUNT_REWARDTYPE 5
#define CHAR_BUFFER_PROMOTION_COMMON_PM_NAME 30
#define CHAR_BUFFER_PROMOTION_COMMON_TICKET_GIFT_NAME 50
#define CHAR_BUFFER_PROMOTION_COMMON_ATTENDANCE_REWARDTYPE 5
#define CHAR_BUFFER_PROMOTION_COMMON_ETC_DATA 30 //��Ÿ ������ ���� ����
#define CHAR_BUFFER_GAME_NAME 20 //�Ƕ�, ������Ŀ�� ���� �̸��� ���
#define CHAR_BUFFER_CHANNEL_NAME 20 //1���, 3����� ä�� �̸��� ���
#define CHAR_BUFFER_REWARD_NAME 100 //ġŲŸ��, ������ ����Ǵ� ��ǰ �̸��� ���
/////////////////////////////////////////////////

#define CHAR_BUFFER_DAYCODE_LEN 11 //yyyy-mm-dd ������ ��¥ �ڵ� �����̴�.
#define CHAR_BUFFER_ETCDATA_LEN 255
#define CHAR_BUFFER_DATE_LEN 21 //yyyy-mm-dd hh:mm:ss ������ ��¥ �ڵ� �����̴�.

#define CHAR_BUFFER_COMMAND_CODE 30
#define CHAR_BUFFER_COMMAND_DATA 150
#define CHAR_BUFFER_COMMON_ADD_INFO 100

#define CHAR_BUFFER_MESSAGE_SENDER_ID 20
#define CHAR_BUFFER_MESSAGE_SENDER_CN 20
#define CHAR_BUFFER_MESSAGE_SENDER_NICK 20
#define CHAR_BUFFER_MESSAGE_SENDER_MESSAGE 1000

#define CHAR_BUFFER_ETC_CHIP_CODE 5
#define CHAR_BUFFER_ETC_CHANGE_UMCODE 30

////////////////////////////2018��06�� ���� 2018.06.01/////////////////////////////////////
#define CHAR_BUFFER_GOLD_INSURANCE_NAME 50 //��庸�� ������
////////////////////////////////

////////////////////////////����⼮�� CAPTCHA 2018.07.31/////////////////////////////////////
#define CHAR_BUFFER_CAPTCHA_KEY_LEN 512 //���Ȱ�ȭ�� ���� ĸ�������� ���޹޴� Ű�� ����
/////////////////////////////////////

#define ONE_HUNDRED_MILLION	((INT64)100000000)				// 1��
#define ONE_BILLION					((INT64)1000000000)			// 10��
#define TEN_BILLION					((INT64)10000000000)			// 100��
#define ONE_HUNDRED_BILLION	((INT64)100000000000)			// 1000��
#define ONE_TRILLION				((INT64)1000000000000)		// 1��
//#define TEN_TRILLION_SUB_TENBILLION					((INT64)9900000000000)		// 9.9��
#define TEN_TRILLION_SUB_TENBILLION					    ((INT64)16500000000000)		// 16.5��
#define TEN_TRILLION					((INT64)10000000000000)		// 10��
#define ONE_HUNDRED_TRILLION	((INT64)100000000000000)		// 100��

/////////////////////////////UPP4�ܰ�, ������� ���� �߰� 2017.05.19/////////////////////////////////////////////
enum NEW_UPP_REASON
{
	//�ݸ������� DI �������� UPP�� ����Ǿ�����, ����� ���� �ڵ��̴�.
	None = 0,
	Netmarble = 1, //�ݸ�����å
	OneSelf, //�ڱ� �ڽ��� �ɾ���.
	Family, //������ �������� �ɾ���.
	Organization, //���� ������� �ɾ���.
	Etc, //��Ÿ �ٸ� �����̴�.
};
/////////////////////////////////////////////////////////

////////////////////////////�Ѱ���, ���뷮Ȯ�� 2017.08.25/////////////////////////////////////
enum RANDOM_SECTION_TYPE
{
	//�������� ���ϱ� ���� �׷��� �ϴ� ���� Ÿ��
	BD_CARD_EVENT,
	NP_CARD_EVENT,
	
};
////////////////////////////////////////////////////////////////

enum PROMOTION_ATTENDANCE_TYPE
{
	//�⼮ ���θ���� Ÿ�� �ڵ�
	ATTENDANCE_NORMAL = 1, //�Ϲ� ����
	ATTENDANCE_CONTINUE, //����
};

enum IDX_GAME
{
	IDX_GAME_NULL=-1,
	IDX_GAME_SP=0,
	IDX_GAME_BD,
	IDX_GAME_NP,
	IDX_GAME_DP,
	IDX_GAME_HL,
	IDX_GAME_SD,
	IDX_GAME_HA,

// ��ʸ�Ʈ
	IDX_GAME_NPT,
	IDX_GAME_BDT,
	IDX_GAME_SDT,

// Ŭ�󿡼��� ���� ����
	IDX_GAME_BT,
//#if defined (__ROUND_MODE__)
	IDX_GAME_RMP,	// ���� ���� ��Ŀ
//#endif

	MAX_GAME,	
};

enum IDX_GAMEGROUP
{
	IDX_GAMEGROUP_NULL=-1,
	IDX_GAMEGROUP_SP=110,
	IDX_GAMEGROUP_BD=120,
	IDX_GAMEGROUP_NP=130,
	IDX_GAMEGROUP_DP=140,
	IDX_GAMEGROUP_HL=150,
	IDX_GAMEGROUP_SD = 160,
	IDX_GAMEGROUP_HA = 170,

	IDX_GAMEGROUP_NPT = 180,
	IDX_GAMEGROUP_BDT = 190,
	IDX_GAMEGROUP_SDT = 200,

};

// R_Key, P_Key ������ �����ڵ� -> �α��ڵ�� ����
enum IDX_GAME_LOGCODE
{
	IDX_LOGCODE_SP = 1,
	IDX_LOGCODE_BD = 2,
	IDX_LOGCODE_NP = 3,
	IDX_LOGCODE_DP = 4,
	IDX_LOGCODE_HL = 5,
	IDX_LOGCODE_SD = 6,
	IDX_LOGCODE_HA = 7,
	IDX_LOGCODE_NPT = 8,
	IDX_LOGCODE_BDT = 9,
	IDX_LOGCODE_SDT = 10,
};


static char *g_GameAbb[]={"SP","BD","NP", "DP", "HL", "SD", "HA", "NPT", "BDT", "SDT"};

#define GetGameABB(x) g_GameAbb[x]

inline IDX_GAME g_GetGameIdxByGameGroupCode(IDX_GAMEGROUP eGameGroupCode)
{
	switch (eGameGroupCode)
	{
	case IDX_GAMEGROUP_SP:
		return IDX_GAME_SP;

	case IDX_GAMEGROUP_BD:
		return IDX_GAME_BD;

	case IDX_GAMEGROUP_NP:
		return IDX_GAME_NP;

	case IDX_GAMEGROUP_DP:
		return IDX_GAME_DP;

	case IDX_GAMEGROUP_HL:
		return IDX_GAME_HL;

	case IDX_GAMEGROUP_SD:
		return IDX_GAME_SD;

		//#ifdef _NEXTUP_HOOLA_20110812
	case IDX_GAMEGROUP_HA:
		{
			return IDX_GAME_HA;
		}
//#endif

	case IDX_GAMEGROUP_NPT:
		return IDX_GAME_NPT;

	case IDX_GAMEGROUP_BDT:
		return IDX_GAME_BDT;

	case IDX_GAMEGROUP_SDT:
		return IDX_GAME_SDT;

	default:
		return IDX_GAME_NULL;
	}
}

inline IDX_GAMEGROUP g_GetGameGroupCodeByGameIdx(IDX_GAME eGameIdx)
{
	switch (eGameIdx)
	{
	case IDX_GAME_SP:
		return IDX_GAMEGROUP_SP;

	case IDX_GAME_BD:
		return IDX_GAMEGROUP_BD;

	case IDX_GAME_NP:
		return IDX_GAMEGROUP_NP;

	case IDX_GAME_DP:
		return IDX_GAMEGROUP_DP;

	case IDX_GAME_HL:
		return IDX_GAMEGROUP_HL;

	case IDX_GAME_SD:
		return IDX_GAMEGROUP_SD;

//#ifdef _NEXTUP_HOOLA_20110812
	case IDX_GAME_HA:
		{
			return IDX_GAMEGROUP_HA;
		}
//#endif

	case IDX_GAME_NPT:
		return IDX_GAMEGROUP_NPT;

	case IDX_GAME_BDT:
		return IDX_GAMEGROUP_BDT;

	case IDX_GAME_SDT:
		return IDX_GAMEGROUP_SDT;

	default:
		return IDX_GAMEGROUP_NULL;
	}
}

inline bool g_IsTournamentGameIndex(IDX_GAME eGameIdx)
{
	switch (eGameIdx)
	{
	case IDX_GAME_NPT:
	case IDX_GAME_BDT:
	case IDX_GAME_SDT:
	case IDX_GAME_RMP:
		return true;

	default:
		return false;
	}
}



// ### [ ������� ] ###
const int  MAX_PLAYER	=   5;

const int  MAX_OBSERVER =   11;//  - (1 ### [������ ��� �۾�] ###)  


//const int MAX_OBSERVER = 100;			// ������ 99�� + ������ 1��
const int  MAX_NEWPLAYER = (MAX_PLAYER+MAX_OBSERVER);

// ������ ���� ���� üũ�� ���
// ��ʸ�Ʈ ���������� MAX_OBSERVER_TMS �� ����ϰ� �ٸ� ���ӿ����� OTHER �� ����Ѵ�.
// const int MAX_OBSERVER_TMS = MAX_OBSERVER;
// const int MAX_OBSERVER_OTHER = 11;	// ������ 10�� + ������ 1��

const int  MAX_DP_PLAYER =  2;	//����Ŀ��
//const int  MAX_DP_OBSERVER =  11; 
const int  MAX_DP_NEWPLAYER = (MAX_DP_PLAYER+MAX_OBSERVER);

const int MIN_PLAYERS_REQUIRED = 2;
#define MAX_AVATARLAYER		  26


#define UPDATE_GAMENUM        20  //    20ȸ ########### �߰��� ��Ͻ� ������ �� ����Ƚ�� ###########

#define MAX_BET_TURN			10
#define MAX_ROUND_LOG			10

enum {
	GRADE_PYOUNGMIN = 0,//���
	GRADE_GOSU,
	GRADE_YOUNGWOONG,
	GRADE_JIZON,
	GRADE_DOSIN,
	GRADE_MAX,
};

// �׷��ȣ
// ��2�ڸ� : ��� 
// ��2�ڸ� : �������� 
enum GROUP_GRADE			//�׷��ڵ��� ���ڸ� 
{
	GROUP_GRADE_NULL = 0,

	GROUP_GRADE_START =1,
	GROUP_GRADE_FRIENDS=GROUP_GRADE_START,	// 1 - ģ�� �׷���
	GROUP_GRADE_ROOKEY,					// 2 - �Ƹ��߾� �׷���
	GROUP_GRADE_DUMMY_1,
	GROUP_GRADE_AMATEUR,					// 4 - �������� �׷��� 
	GROUP_GRADE_PRO,						// 5 - ���� �׷���
	GROUP_GRADE_PLATINUM_1,				// 6 - ���ʽ� �׷���
	GROUP_GRADE_PLATINUM_2,					// 7 - ��������1 �׷���	
	GROUP_GRADE_DUMMY_2,
	GROUP_GRADE_PLATINUM_3,					// 9 - ��������2 �׷���
	MAX_GROUP_GRADE_INDEX,					//�迭���� �ε���  ��ƾ��� �ִ밪
};

enum GROUP_NO
{
	GROUP_NULL=0,

	GROUP_FREE_SP  = 101,	   // ģ�� ( ���� )
	GROUP_FREE_BD  = 103,
	GROUP_FRIENDS_DP   = 105,	//���� �߰� (2009.05.14)

	GROUP_ROOKIE_SP  = 201,    // ��Ű ( ��Ű / �Ƹ��߾� )
	GROUP_ROOKIE_BD  = 203,
	GROUP_PRO_DP      = 205,

	GROUP_PLATINUM_DP = 305,

	GROUP_AMATEUR_SP = 401,    // �Ƹ��߾�  ( ���� )
	GROUP_AMATEUR_BD = 403,

	GROUP_PRO_SP	= 501,     // ���� ( ���̽� )
	GROUP_PRO_BD	= 503,

	GROUP_PLATINUM1_SP	= 601, // �÷�Ƽ��1 ( ���ʽ�, �������� )
	GROUP_PLATINUM1_BD	= 603,

	GROUP_PLATINUM2_SP	= 801, // �÷�Ƽ��2 ( �׶����� )
	GROUP_PLATINUM2_BD	= 803,
};




//���������� �������� �з�
//(����� 3������ ������)
enum IDX_GAMERECORD
{
	IDX_GAMERECORD_NULL=-1,	
	IDX_GAMERECORD_BASIC=0,		//����~���ʽ� ä�� ����	
	IDX_GAMERECORD_MASTER,		//�������� ä�� ����	
	IDX_GAMERECORD_REAL,		//���� ä�� ����	(4��, 5�� ����)
	//---------------
	MAX_GAMERECORD,		
};


enum CHOICE_GAME_MODE{
	CHOICE_GAME_MODE_NULL=-1,
	CHOICE_GAME_MODE_SP = 0,	//������Ŀ ������ ����
	CHOICE_GAME_MODE_BD,		//�ٵ��� ������ ����
//	CHOICE_GAME_MODE_LIMIT,
};



// �׷��� ����(CLASS)�� �������� �з� 
enum IDX_GROUPCLASS
{
	IDX_GROUPCLASS_BASIC=0,		// ��,��,��, ���� 
	IDX_GROUPCLASS_MASTER,		// ���� S
	IDX_GROUPCLASS_REAL,		// 
	//IDX_GROUPCLASS_LIMIT,		// ���� L
};



enum BETRULE_KIND
{
	BETRULE_KIND_SEMIREAL=0,	//�ظ��� ����
	BETRULE_KIND_REAL,			//���� ���� (�Ŀ� ����) �⺻
	BETRULE_KIND_LIMIT,		    //����Ʈ ���� //< CYS 110506 >
};


/////////////////////////////�ս��ѵ� �ʰ� ��� �߰� 2017.05.15/////////////////////////////////////////////
enum DAILYLOSS_USER_SETTING
{
	STOP_GAME = 0,
	OVER_CHALLANGE,
	COMEBACK,
	NO_DATA_INDB,
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////

//�׷��ڵ带 �׷������� �ٲ۴�
inline GROUP_GRADE g_GetGroupGrade(GROUP_NO nGroup = GROUP_NULL)
{
	return GROUP_GRADE_NULL;
}


//�׷� ��ȣ�� GAMERECORD �ε��� �� �����´�. 
//(�׷쿡 ���� ��� �������� ī��Ʈ �� �� ����  ����)
inline IDX_GAMERECORD g_GetGameRecordIndex(GROUP_NO nGroup)
{
	return IDX_GAMERECORD_BASIC;
}

// �׷��ȣ�� ���ñ�Ģ�� �����´�.
inline BETRULE_KIND g_GetBetRuleKind(GROUP_NO nGroup)
{
	return BETRULE_KIND_REAL;
}

#if defined(_BETAVER)
#define GAME_CODE_SP (10042)
#define GAME_CODE_BD (10054)
#define GAME_CODE_DP (10218)
#define GAME_CODE_NP (10061)
#define GAME_CODE_HL (10036)
#define GAME_CODE_SD (10006)
#define GAME_CODE_HA (10033)
#define GAME_CODE_NPT (10074)
#else // defined(_BETAVER)
#define GAME_CODE_SP (42)
#define GAME_CODE_BD (54)
#define GAME_CODE_DP (218)
#define GAME_CODE_NP (61)
#define GAME_CODE_HL (36)
#define GAME_CODE_SD (6)
#define GAME_CODE_HA (33)
#define GAME_CODE_NPT (74)
#define GAME_CODE_BDT (75)
#define GAME_CODE_SDT (76)
#define GAME_CODE_BT (9999)
#endif  // defined(_BETAVER)


//�����ε����� �����ڵ带 �����´�. 
inline int g_GetGameCode(IDX_GAME game)
{
	if (game==IDX_GAME_SP) return GAME_CODE_SP;
	else if (game==IDX_GAME_BD) return GAME_CODE_BD;
	else if (game==IDX_GAME_DP) return GAME_CODE_DP;
	else if (game==IDX_GAME_NP) return GAME_CODE_NP;
	else if (game==IDX_GAME_HL) return GAME_CODE_HL;
	else if (game == IDX_GAME_SD) return GAME_CODE_SD;
//#ifdef _NEXTUP_HOOLA_20110812
	else if ( game == IDX_GAME_HA ) return GAME_CODE_HA;
//#endif

	else if (game == IDX_GAME_NPT) return GAME_CODE_NPT;
	else if (game == IDX_GAME_BDT) return GAME_CODE_BDT;
	else if (game == IDX_GAME_SDT) return GAME_CODE_SDT;
	return 0;
}

//�����ڵ�� �����ε����� �����´�.
inline IDX_GAME g_GetGameIndex(int nGameCode)
{
	if (nGameCode==GAME_CODE_SP) return IDX_GAME_SP;
	else if (nGameCode==GAME_CODE_BD) return IDX_GAME_BD;
	else if (nGameCode==GAME_CODE_DP) return IDX_GAME_DP;
	else if (nGameCode==GAME_CODE_NP) return IDX_GAME_NP;
	else if ( nGameCode == GAME_CODE_HL ) return IDX_GAME_HL;
	else if ( nGameCode == GAME_CODE_SD ) return IDX_GAME_SD;
	else if ( nGameCode == GAME_CODE_HA ) return IDX_GAME_HA;
	else if ( nGameCode == GAME_CODE_NPT) return IDX_GAME_NPT;
	else if ( nGameCode == GAME_CODE_BDT) return IDX_GAME_BDT;
	else if ( nGameCode == GAME_CODE_SDT) return IDX_GAME_SDT;
	return IDX_GAME_NULL;
}

inline IDX_GAME g_GetGameIndexDailyLoss(IDX_GAME idx_game)
{
	switch (idx_game)
	{
	case IDX_GAME_SP:
	case IDX_GAME_BD:
	case IDX_GAME_NP:
	case IDX_GAME_DP:
	case IDX_GAME_HL:
	case IDX_GAME_SD:
	case IDX_GAME_HA:
		return idx_game;

	case IDX_GAME_NPT:	return IDX_GAME_NP;
	case IDX_GAME_BDT:	return IDX_GAME_BT;
	case IDX_GAME_SDT:	return IDX_GAME_SD;
	}

	return IDX_GAME_NULL;
}

inline bool g_IsTournamentGameCode(int gameCode)
{
	switch (gameCode)
	{
	case GAME_CODE_BDT:
	case GAME_CODE_SDT:
	case GAME_CODE_NPT:
		return true;

	default:
		return false;
	}
}


//���Ӻ� ���� ũ�� �迭 ���� 
template<typename T>
struct ARRAY_VALUE
{
	T data[MAX_GAME];	

	void operator +=(const ARRAY_VALUE &av)
	{		
		for(int i=0; i<MAX_GAME; i++) 
		{	
			data[i] += av.data[i];
		}		
	}
	void operator -=(const ARRAY_VALUE &av)
	{		
		for(int i=0; i<MAX_GAME; i++) 
		{	
			data[i] -= av.data[i];
		}		
	}

	ARRAY_VALUE operator +(const ARRAY_VALUE &av)
	{
		ARRAY_VALUE ret;
		for(int i=0; i<MAX_GAME; i++) 
		{	
			ret.data[i] = data[i] + av.data[i];
		}
		return ret;
	}
	ARRAY_VALUE operator -(const ARRAY_VALUE &av)
	{
		ARRAY_VALUE ret;
		for(int i=0; i<MAX_GAME; i++) 
		{	
			ret.data[i] = data[i] - av.data[i];
		}
		return ret;
	}
	bool operator ==(const ARRAY_VALUE &av)
	{		
		for(int i=0; i<MAX_GAME; i++) 
		{	
			if (data[i] != av.data[i]) return false;
		}
		return true;
	}
	bool operator !=(const ARRAY_VALUE &av)
	{		
		for(int i=0; i<MAX_GAME; i++) 
		{	
			if (data[i] != av.data[i]) return true;
		}
		return false;
	}
	T& operator[] (int iGame)
	{
		return data[iGame];
	}
	ARRAY_VALUE()
	{
		for(int i=0; i<MAX_GAME; i++) 
		{
			data[i]=0;
		}
	}
	
};

namespace hoola
{

enum EHOOLA_RESULTCASE
{
	eHOOLA_UNKNOWN    = 0,
	eHOOLA_FIRSTHOOLA,
	eHOOLA_HOOLA,
	eHOOLA_ALLDISCARD,
	eHOOLA_DABANG,
	eHOOLA_SOBANG,
	eHOOLA_STOP,
	eHOOLA_EMPTYCARD,
	eHOOLA_NAGARI,
	eHOOLA_SEVEN4,
	eHOOLA_DABANG_BAK,
	eHOOLA_SOBANG_BAK,
	eHOOLA_STOP_BAK,

	////////////////////////////�Ƕ� ��ǹ� 2018.03.29/////////////////////////////////////
	eHOOLA_GIVEUP,
	////////////////////////////////
};

enum EGAME_RULE_TYPE
{
	eRULE_TYPE_NORMAL = 0, // �Ϲ� �Ƕ�
	eRULE_TYPE_WAR,		   // ���� �Ƕ�
	eRULE_TYPE_STOP		   // ���� �Ƕ�
};

enum EFORM_KIND_TYPE
{
	eFORM_KIND_NORMAL = 1, // �Ϲ�
	eFORM_KIND_JOKER,	   // ��Ŀ	
	eFORM_KIND_MAGIC,	   // ����
};

}

#ifdef _GAMETEST
#define ROUND_MODE_MAX_PLAY_COUNT 	500	// �ִ� ���� �÷��� �Ǽ�
#else
#define ROUND_MODE_MAX_PLAY_COUNT   10		// �ִ� ���� �÷��� �Ǽ�
#endif

#define FOBIS_GAMEABUSE_LEN		4000
