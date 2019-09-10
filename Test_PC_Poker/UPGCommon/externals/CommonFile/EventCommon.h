#pragma once

#include "UserStruct.h"
#include "CommMsgDef.h"
#include "CommMsgDef_Game.h"

enum
{
	OPCODE_MASTER_SERVER = 20000,		// �����ͼ��� ���� �̺�Ʈ

	// BEGIN ����
	OPCODE_G_TO_M_JP_GAMEFEE,			// ���Ӽ���   -> �����ͼ��� ���Ӻ� ����
	OPCODE_M_TO_G_JP_ALLMONEY,			// �����ͼ��� -> ���Ӽ���   �����ݾ׹� ����
	OPCODE_M_TO_G_JP_OFFER_MONEY,		// �����ͼ��� -> ���Ӽ���   ��÷�ݾ�
	OPCODE_M_TO_G_JP_EVENT_ALRAM,		// �����ͼ��� -> ���Ӽ���   �̺�Ʈ���� �˸�


	//< Ŭ���̾�Ʈ ��Ŷ >
	OPCODE_SV_REGISTER_ALL = 25000,     //< CYS 100901 >

	OPCODE_SV_JP_INFO,					// ���� ����
	OPCODE_SV_JP_WINNER_ALRAM,			// ��÷�� �˸�
	OPCODE_SV_JP_EVENTTIME_ROOM,		// �̺�Ʈ ����Ÿ�� Ư�� �� �˸�
	OPCODE_SV_JP_EVENTTIME_ALLUSER,		// �̺�Ʈ ����Ÿ�� ��� ���� �˸�
	OPCODE_SV_JP_EVENT_GIFTTIME,		// �̺�Ʈ 3�� �̻� ���� ���������� ���� ����
	// END ����

	OPCODE_SV_GTIME_START,          // ��ĿŸ��
	OPCODE_SV_GTIME_ALRAM,
	OPCODE_SV_GTIME_END,


	OPCODE_SV_CHAMP_ALRAM,          // �ٵ��� ����ȸ

	OPCODE_SV_PCROOMACT_START,		// PC�� Ȱ��ȭ ����
	OPCODE_SV_PCROOMACT_INITIALIZE, // PC�� Ȱ��ȭ �ʱ�ȭ
	OPCODE_SV_PCROOMACT_ALRAM,		// PC�� Ȱ��ȭ ����
	OPCODE_SV_PCROOMACT_END,		// PC�� Ȱ��ȭ ��

	OPCODE_SV_PCPZONE_START,		// PC�� ��Ŀ��
	OPCODE_SV_PCPZONE_INITIALIZE,	// PC�� ��Ŀ�� �ʱ�ȭ
	OPCODE_SV_PCPZONE_ALRAM,		// PC�� ��Ŀ�� ����
	OPCODE_SV_PCPZONE_END,			// PC�� ��Ŀ�� ��
	OPCODE_SV_PCPZONE_TIMEOUT,		// PC�� 10�а� ����


	OPCODE_SV_BETTING_START,		// ��Ŀ�Ӵ� ���� ����
	OPCODE_SV_BETTING_INITIALIZE,	// ��Ŀ�Ӵ� ���� �ʱ�ȭ
	OPCODE_SV_BETTING_ALRAM,		// ��Ŀ�Ӵ� ���� ����
	OPCODE_SV_BETTING_END,			// ��Ŀ�Ӵ� ���� ��

	OPCODE_SV_RETENTION_START,		// ���ټ� ���θ�� ����
	OPCODE_SV_RETENTION_INITIALIZE,	// ���ټ� ���θ�� �ʱ�ȭ
	OPCODE_SV_RETENTION_ALRAM,		// ���ټ� ���θ�� ����
	OPCODE_SV_RETENTION_END,		// ���ټ� ���θ�� ��

	OPCODE_SV_NEWYEARRETENTION_START,		// ��Ŀ �ų� ���ټ� ���θ�� ����
	OPCODE_SV_NEWYEARRETENTION_INITIALIZE,	// ��Ŀ �ų� ���ټ� ���θ�� �ʱ�ȭ
	OPCODE_SV_NEWYEARRETENTION_ALRAM,		// ��Ŀ �ų� ���ټ� ���θ�� ����
	OPCODE_SV_NEWYEARRETENTION_END,			// ��Ŀ �ų� ���ټ� ���θ�� ��

	OPCODE_SV_USERCARE_START,
	OPCODE_SV_USERCARE_INITIALIZE,
	OPCODE_SV_USERCARE_ALRAM,
	OPCODE_SV_USERCARE_END,

	OPCODE_SV_GOLDCARD_START,
	OPCODE_SV_GOLDCARD_INITIALIZE,
	OPCODE_SV_GOLDCARD_ALARM,			// ���θ�� ����
	OPCODE_SV_GOLDCARD_APPEAR,			// �����濡�� ���ī�� ����
	OPCODE_SV_GOLDCARD_REWARD,			// �̼� �޼����� ���� �Ӵ� ȹ��
	OPCODE_SV_GOLDCARD_END,

	OPCODE_SV_ADULTCARD_START,
	OPCODE_SV_ADULTCARD_INITIALIZE,
	OPCODE_SV_ADULTCARD_ALARM,			// ���θ�� ����
	OPCODE_SV_ADULTCARD_REWARD,			// �̼� �޼����� ���� �Ӵ� ȹ��
	OPCODE_SV_ADULTCARD_END,

	OPCODE_SV_MISSION003_START,
	OPCODE_SV_MISSION003_INITIALIZE,
	OPCODE_SV_MISSION003_ALARM,			// ���θ�� ����
	OPCODE_SV_MISSION003_ALLIN,			// �̼� �޼����� ���� �Ӵ� ȹ��
	OPCODE_SV_MISSION003_END,

	OPCODE_SV_POKERADVENTURE_START,
	OPCODE_SV_POKERADVENTURE_INITIALIZE,
	OPCODE_SV_POKERADVENTURE_ALARM,
	OPCODE_SV_POKERADVENTURE_REWARD,
	OPCODE_SV_POKERADVENTURE_ALLIN,	
	OPCODE_SV_POKERADVENTURE_END,

	OPCODE_SV_HALFJACKPOT_START,
	OPCODE_SV_HALFJACKPOT_INITIALIZE,
	OPCODE_SV_HALFJACKPOT_TIME_BEFORE_START,	// ���� Ÿ�� ������
	OPCODE_SV_HALFJACKPOT_TIME_START,			// ���� Ÿ�� ����
	OPCODE_SV_HALFJACKPOT_TIME_BEFORE_END,		// ���� Ÿ�� ������
	OPCODE_SV_HALFJACKPOT_TIME_END,				// ���� Ÿ�� ����
	OPCODE_SV_HALFJACKPOT_MONEY,				// ���� ���� �Ӵ� �˸�
	OPCODE_SV_HALFJACKPOT_ALARM,				// ���� Ÿ�� ���� �� ���� �������� �˸�
	OPCODE_SV_HALFJACKPOT_ROOM,					// ���̹� ����
	OPCODE_SV_HALFJACKPOT_ENTER_JACKPOT_ROOM,	// ���̹� ����
	OPCODE_SV_HALFJACKPOT_PRIZE,				// ������ ����&��÷�� ����
	OPCODE_SV_HALFJACKPOT_TOP_HALF_COUNT_MS,	// ������ �ĺ��� ���� MS�� ����
	OPCODE_SV_HALFJACKPOT_DELETE_ROOM_MS,		// �ĺ� ���� ������
	OPCODE_SV_HALFJACKPOT_END_HALF_MS,			// �ش� ä�� ���� ī���� ����
	OPCODE_SV_HALFJACKPOT_JACKPOT_ROUND_START,	// ���̹� ����
	OPCODE_SV_HALFJACKPOT_END,

	OPCODE_SV_MISSION201304_START,
	OPCODE_SV_MISSION201304_INITIALIZE,
	OPCODE_SV_MISSION201304_ALARM,			// ���θ�� ����
	OPCODE_SV_MISSION201304_REWARD,			// �̼� �޼����� ���� �Ӵ� ȹ��
	OPCODE_SV_MISSION201304_END,

	OPCODE_SV_BORNIN1996_START,
	OPCODE_SV_BORNIN1996_INITIALIZE,
	OPCODE_SV_BORNIN1996_ALARM,
	OPCODE_SV_BORNIN1996_REWARD,
	OPCODE_SV_BORNIN1996_END,

	OPCODE_SV_LOTTERY201406_START,
	OPCODE_SV_LOTTERY201406_INITIALIZE,
	OPCODE_SV_LOTTERY201406_ALARM,			// ���θ�� ����
	OPCODE_SV_LOTTERY201406_REWARD,			// �Ǽ� �޼����� ���� ����� ȹ��
	OPCODE_SV_LOTTERY201406_END,


// #if defined(__PROMOTION_CHANCEAVATA__) (2011.07.28 �����ƹ�Ÿ���θ��)
	OPCODE_SV_CHANCE_START,
	OPCODE_SV_CHANCE_INITIALIZE,
	OPCODE_SV_CHANCE_END,

	//--�������θ��
	OPCODE_SV_HALF_START,
	OPCODE_SV_HALF_ALARM,			// ���θ�� ����
	OPCODE_SV_HALF_REWARD,			// �Ǽ� �޼����� ���� ����� ȹ��
	OPCODE_SV_HALF_END,

	//--�⼮üũ�̺�Ʈ
	OPCODE_SV_VISITE_START,
	OPCODE_SV_VISITE_ALARM,			// ���θ�� ����
	OPCODE_SV_VISITE_END,
// #endif // (__PROMOTION_CHANCEAVATA__)

	OPCODE_SV_PLAYCOUNT_START,	//�÷��� �Ǽ��� ���� ���θ��
	OPCODE_SV_PLAYCOUNT_ALARM,
	OPCODE_SV_PLAYCOUNT_REWARD,
	OPCODE_SV_PLAYCOUNT_END,
	// �������� �̺�Ʈ
	OPCODE_SV_RETENTION_REWARD,		// �������� �̺�Ʈ ��÷ ( �Ǽ� �̴޽� ��÷ ���� )

	//OPCODE_SV_PLAYCOUPON_START,		// ���θ�� ����
	OPCODE_SV_PLAYCOUPON_ALRAM,	

	OPCODE_SV_VIPJACKPOT_START,				// �α��� �� �������� ���θ�� ���� ����
	OPCODE_SV_VIPJACKPOT_INITIALIZE,		// �α��� �� �������� ���θ�� ���� ����
	OPCODE_SV_VIPJACKPOT_TIME_BEFORE_START,	// ���� Ÿ�� ������
	OPCODE_SV_VIPJACKPOT_TIME_START,			// ���� Ÿ�� ����
	OPCODE_SV_VIPJACKPOT_TIME_BEFORE_END,		// ���� Ÿ�� ������
	OPCODE_SV_VIPJACKPOT_TIME_END,				// ���� Ÿ�� ����
	OPCODE_SV_VIPJACKPOT_WAIT_PULL_JACKPOT,	// ��� 3��
	//OPCODE_SV_VIPJACKPOT_MONEY,				// ���� ���� �Ӵ� �˸�
	OPCODE_SV_VIPJACKPOT_ALARM,				// ���� Ÿ�� ���� �� ���� �������� �˸�
	//OPCODE_SV_VIPJACKPOT_WIAT_JACKPOT_ROOM,	// ���̹� �������ð�
	OPCODE_SV_VIPJACKPOT_TOTAL_EVENT_SUCESS_ROOM_INFO,	// ���� ������ ���� ���� // OPCODE_SV_VIPJACKPOT_ROOM �̰ſ��� �����	
	//OPCODE_SV_VIPJACKPOT_ENTER_JACKPOT_ROOM,	// ���̹� ���� 	
	OPCODE_SV_VIPJACKPOT_TOP_HALF_COUNT_MS,	// ������ �ĺ��� ���� MS�� ����
	OPCODE_SV_VIPJACKPOT_DELETE_ROOM_MS,		// �ĺ� ���� ������
	OPCODE_SV_VIPJACKPOT_END_HALF_MS,			// �ش� ä�� ����,Ǯ ī���� ����
	OPCODE_SV_VIPJACKPOT_JACKPOT_ROUND_START,	// ���̹� ����
	OPCODE_SV_VIPJACKPOT_END,
	OPCODE_SV_VIPJACKPOT_TOTAL_JACKPOT_EVENT_INFO,	// ��ü ������ ���� ��÷ ���� OPCODE_SV_VIPJACKPOT_PRIZE �̰ſ��� �����
	OPCODE_SV_VIPJACKPOT_BET_POINT,			// ���� �� ���� �����ش�.	
	OPCODE_SV_VIPJACKPOT_PRIZE, // �ش� ���� ���� ���� ����
	OPCODE_SV_VIPJACKPOT_MONEY_INFO,//  MS-> GS �� ���۵� (Ŭ���� ����)
	OPCODE_SV_VIPJACKPOT_TOTAL_WAIT_ROOM_INFO,	// �ش� ������ ��ü ���� ���� ����
	OPCODE_SV_VIPJACKPOT_MS_BETPOINT_INFO,			// ���Ӽ��� �������� Ÿ�Ժ� �����Ѵ�. (A ~ D)
	OPCODE_SV_VIPJACKPOT_BETPOINT_ALARM,		// �ּ� �濡 ��� ���� ���� ����Ʈ( GS->CL )
	OPCODE_SV_VISITE_SUB_MISSION_ALARM,			// �ǹ���ǰ ���θ�� ��÷ �˶�
	OPCODE_SV_VISITE_SUB_MISSION_REWARD_GEG_INFO_ALARM,			// �ǹ���ǰ ���θ�� ��÷ �ּ� �Է� �˸�
	OPCODE_SV_LEAGUE_REWARD_MONEY,		//������ ����

	////////////////////////////�Ѱ���, ���뷮Ȯ�� 2017.08.25/////////////////////////////////////
	OPCODE_SV_SUPERMOON_ALARM,
	OPCODE_SV_SUPERMOON_START,
	//////////////////////////////////////////////////////////

	////////////////////////////2018 �߼� �̺�Ʈ. ���� �⼮ & �Ǽ� ���� 2018.09.20/////////////////////////////////////
	OPCODE_CL_2018THANKSGIVING_ATTENDANCE_INFO,		// �⼮ �̺�Ʈ. �⼮�� ��û.		// ����: PROMOTION_2018THANKSGIVING_ATTEDANCE_TOTAL_INFO
	OPCODE_SV_2018THANKSGIVING_ATTENDANCE_INFO,		// �⼮ �̺�Ʈ. �α��ν� ��Ƽ Ȥ�� ��û(OPCODE_CL_2018THANKSGIVING_ATTENDANCE_INFO)�� ����.	// ����: PROMOTION_2018THANKSGIVING_ATTEDANCE_TOTAL_INFO
	OPCODE_CL_2018THANKSGIVING_GAMECOUNT,			// �Ǽ� �̺�Ʈ. ���� �Ǽ� ��û			// ����: PROMOTION_2018THANKSGIVING_GAMECOUNT
	OPCODE_SV_2018THANKSGIVING_GAMECOUNT,			// �Ǽ� �̺�Ʈ. ���� ���� ����� ��Ƽ.	// ����: PROMOTION_2018THANKSGIVING_GAMECOUNT
	OPCODE_SV_2018THANKSGIVING_GAMECOUNT_INFO,		// �Ǽ� �̺�Ʈ�� ����Ǵ� ä���� ���� ��Ƽ. (�α��ν� 1��)	����: PROMOTION_2018THANKSGIVING_GAMECOUNT_INFO
	//////////////////////////////////////////////////////////////////////////

};

enum
{
	REAL_REWARD_ITEM_NOTE_BOOK = 1,		//��Ʈ��
	REAL_REWARD_ITEM_SHAVER,			//�鵵��
	REAL_REWARD_ITEM_VACCUM,			//û�ұ�
	REAL_REWARD_ITEM_GIFT_CARD_10,		//��ǰ�� 10��
	REAL_REWARD_ITEM_GIFT_CARD_100,		//��ǰ�� 100��
	REAL_REWARD_ITEM_50TV,				// 50��ġ TV
	REAL_REWARD_ITEM_WINE_REFRIGERATOR,	// ���� �����
	REAL_REWARD_ITEM_MOBILE,			// ��S7��
	REAL_REWARD_ITEM_GODL_BAR,			//���� 10��
};


namespace promotion
{
	struct PROMOTION_2018THANKSGIVING_ATTEDANCE_TOTAL_INFO;
}

//< CYS 100604
namespace packet
{

typedef WORD OPCODE_ID;

template
	< 
		class OPERAND_TYPE 
	>
class TSendPacket : public CCommMsg
{
public:
	TSendPacket()
	{
	}
	TSendPacket( OPCODE_ID idOpcode )
	{
		SetHeader( idOpcode );
	}

	void operator<<( const OPERAND_TYPE& rOperand )
	{
		PushData( _PVAR(  TypeVarious ), ( void *)&rOperand, sizeof( OPERAND_TYPE ) );
	}

	void operator>>( OPERAND_TYPE*& rpOperand )
	{
		size_t size = sizeof( OPERAND_TYPE );

		TotalSize += size;

		// ���� ���� Ȯ��(���� �Ǵ� ���� �����ʹ� �Ź� �޶��� �� ����)
		pData = pBufUnit->Prepare(TotalSize);

		PACKETHEADER* pHead = (PACKETHEADER*)pData;

		void* ptr = pData + NowOffset;
		//memcpy(ptr, src, size);

		rpOperand = reinterpret_cast< OPERAND_TYPE* >( ptr );

		NowOffset += size;
		pHead->MsgLen += size;

	}

	bool GetOperand( char* lpdata, int size, OPERAND_TYPE*& rpOperand ) 
	{
		if ( !GetHeader( lpdata, size )) 
		{
			return false;
		}

		TakeData( _PVAR( rpOperand ), sizeof( OPERAND_TYPE ));		
		return true;
	}

public:
	void Set( OPERAND_TYPE& rOperand )
	{
		PushData( _PVAR( TypeVarious ), &rOperand, sizeof( OPERAND_TYPE ) );
	}

	void SetArgument( std::vector< OPERAND_TYPE >& rpArgument )
	{
		size_t size = rpArgument.size();
		PushData( _PVAR(nTotalArgu), &size, sizeof(*nTotalArgu));

		for ( size_t i = 0; i < size; ++i )
		{
			OPERAND_TYPE argument = rpArgument[ i ];
			PushData( _PVAR( TypeVarious ), &argument, sizeof( OPERAND_TYPE ) );
		}
	}

	void  GetArgument( char* lpdata, int size  )
	{
		if ( !GetHeader(lpdata, size)) return;
		TakeData( _PVAR( nTotalArgu ), sizeof( *nTotalArgu ));	
		TakeData( _PVAR( TypeVarious ), sizeof( OPERAND_TYPE ) * ( *nTotalArgu ) );				
	}

	void Get( char* lpdata, int size ) 
	{
		if ( !GetHeader(lpdata, size)) return;
		TakeData( _PVAR( TypeVarious ), sizeof( OPERAND_TYPE ));				
	}

	OPERAND_TYPE* Various()
	{
		return TypeVarious;
	}

	int* TotalArg()
	{
		return nTotalArgu;
	}

protected:
	OPERAND_TYPE* TypeVarious;
	int*          nTotalArgu;
};

}
//> CYS 100604



namespace jackpot
{
	
///////////////////////////////////////////////////////////////////////////////////
// BEGIN
///////////////////////////////////////////////////////////////////////////////////

enum eGameType
{
	GAME_UNKNOWN = -1,
	GAME_7POKER,
	GAME_LOWBD,
	GAME_NEWPOKER,
	GAME_HIGHLOW,
//#ifdef _NEXTUP_HOOLA_20110812 // �����Ƕ�
	GAME_HOOLA,
//#endi

	GAME_ALL	
};

enum EJACTPOT_TYPE
{
	EJACKPOT_UNKNOWN = 0,
	EJACKPOT_RT_S	 = 101,
	EJACKPOT_RT_D	 = 102,
	EJACKPOT_RT_H	 = 103,
	EJACKPOT_RT_C	 = 104,

	EJACKPOT_BSFLUSH = 105, // �齺Ʈ����Ʈ �÷���.
	EJACKPOT_SFULSH  = 106, // ��Ʈ����Ʈ �÷���.
	EJACKPOT_QUADS	 = 107, // ��ī��

	EJACKPOT_BPG	 = 108,	// ���� ����
	EJACKPOT_BG		 = 109, // �ٵ��� ����
	EJACKPOT_BS		 = 110,	// �ٵ��� ������

	EJACKPOT_HOOLA   = 111,
};

enum
{
//#ifdef _NEXTUP_HOOLA_20110812 // �����Ƕ�
	DEF_POKERGAME_NUM  = 5,   // ����Ŀ ������Ŀ �ο�ٵ��� ���̷ο� �Ƕ� 
//#else
//		DEF_POKERGAME_NUM  = 4,   // ����Ŀ ������Ŀ �ο�ٵ��� ���̷ο�
//#endif
	DEF_MONEYRANGE_NUM = 13,

	OFFER_JACKPOTMONEY = 1000,
	NOTICE_JACKPOTMONEY
};

/*
���Ӽ��� -> �����ͼ���
OPCODE_G_TO_M_JP_GAMEFEE
���Ӻ� ����
*/
typedef struct
{
	INT64   llMoney;
	short   sGameCode;
	INT64	arllGamefee[ DEF_MONEYRANGE_NUM ]; // �������ѸӴϺ� ���Ӻ�

} JP_GAMEFEE_ALRAM;


/*
�����ͼ��� -> ���Ӽ���
OPCODE_M_TO_G_JP_ALLMONEY,  
OPCODE_SV_JP_INFO
���� ����
*/

#define  LIMITMONEY_INT64_INT( x )	( ( int )  ( x / 1000000 ) )
#define  LIMITMONEY_INT_INT64( x )	( ( INT64 )x * 1000000 ) 

typedef struct
{
	short   sMoneyRangeCount;									// ���� ���� ī��Ʈ
	bool	bQuadrillionTime[ DEF_MONEYRANGE_NUM ];				// 1000�� && 22:00 ~ 02:00 �̺�Ʈ ����Ÿ��
	int		nLimitMoney[ DEF_MONEYRANGE_NUM ];					// ���� ���ѸӴ� * 1,000,000
	INT64	llJackPotMoney[ DEF_MONEYRANGE_NUM ];				// ���� �����ݾ�
} JP_BASE;

typedef struct  
{
	JP_BASE stJPBase[ DEF_POKERGAME_NUM ];

} JP_MONEYINFO;

/*
�����ͼ��� -> ���Ӽ���
OPCODE_M_TO_G_JP_OFFER_MONEY   
��÷�ݾ�
*/
typedef struct 
{
	char	szID[ ID_LEN ];
	char	szNickName[ NICK_LEN ];	// 2011.01.27 Nickname �߰�
	short   sGameCode;
	short   sServerCode;
	INT64	llLimitMoney;
	INT64	llBeforePMoney;
	INT64	llBonusPMoney;
	INT64	llAfterPMoney;
	short	sRoomNum;
	short	sJokbo;				   // 101:��Ƽ��(�����̵�) 102:��Ƽ��(���̾�) 103:��Ƽ��(��Ʈ) 104:��Ƽ��(Ŭ�ι�) 105:���ϰ���
	char	szJokbo[ 24 ];
	short	sUseType;

} JP_DATA;

/*
�����ͼ��� -> ���Ӽ���
OPCODE_M_TO_G_EVENT_ALRAM  
���� Ÿ�� �˸�
*/
typedef struct
{
	bool	bJackPotEventTime;           // true : �������� ���� , false : ���� ����
	bool	bBroadcast;					 // ture : ��� ��
	short	sGameCode;		  	
	INT64	llLimitMoney;	

} JP_EVENT_ALRAM;

/*
���Ӽ��� -> Ŭ���̾�Ʈ
OPCODE_SV_JP_WINNER_ALRAM
��÷ ����
*/
typedef struct
{
	short		  sToRoom;
	JP_DATA  JData;

} JP_WIN;

/*
OPCODE_SV_JP_EVENTTIME_ALLUSER		
�̺�Ʈ ����Ÿ�� ��
*/
typedef struct
{
	bool	bJackPotEventTime;           // true : �������� ���� , false : ���� ����
	bool	bBroadcast;					 // ture : ��� ��
	short	sGameCode;		 
	INT64	llLimitMoney;	   

} JP_EVENTTIME_ALLUSER;

/*
OPCODE_SV_JP_EVENTTIME_ROOM
�̺�Ʈ ����Ÿ�� ��
*/
typedef struct
{
	bool bJackPotEventTime; // ���� ��

} JP_EVENTTIME_ROOM;

/*
OPCODE_SV_JP_EVENT_GIFTTIME		
�̺�Ʈ 3�� �̻� ���� ���������� ���� ����
*/
typedef struct
{
	bool bJackPotEventGiftTime;  // true : ������ ����, false : ���
	bool bCurrentRound;			 // ����Ÿ�� �ٷ� ���˸�
	int  nRoundCount;            // ���� �����

} JP_EVENT_GIFTTIME;

///////////////////////////////////////////////////////////////////////////////////
// END
///////////////////////////////////////////////////////////////////////////////////

}

namespace promotion
{

///////////////////////////////////////////////////////////////////////////////////
// BEGIN
///////////////////////////////////////////////////////////////////////////////////
/*
OPCODE_SV_GTIME_START
OPCODE_SV_GTIME_ALRAM
OPCODE_SV_GTIME_END
*/
typedef struct _GTIME_ALRAM
{
	int   nTimeType;    // 0 : �Ⱓ, 1: ������
	int   nGrade;       // ����
	int   nValue;      
	char  szNickName[ NICK_LEN ];   // ���� ID 
	INT64 llGiftMoney;  

} GTIME_ALRAM;
///////////////////////////////////////////////////////////////////////////////////
// END
///////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////
// BEGIN  �����
///////////////////////////////////////////////////////////////////////////////////
typedef struct _CHAMP_ALRAM
{
	bool bComplete; // �Ϸ�

	bool bExistID;  // �����
} CHAMP_ALRAM;
///////////////////////////////////////////////////////////////////////////////////
// END
///////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////
// BEGIN  PC�� Ȱ��ȭ
///////////////////////////////////////////////////////////////////////////////////
/*
OPCODE_SV_PCROOMACT_START  PC�� Ȱ��ȭ ����
OPCODE_SV_PCROOMACT_ALRAM  PC�� Ȱ��ȭ ����
*/
typedef struct _PCROOMACT_ALRAM
{
	bool bComplete;			// ����� ȹ�� true : false

	bool  bDormantUser;		// �޸�����             ó�� �ѹ� 30�� ����
	int	  nPlayRoundCnt;	// �÷���ī��Ʈ
	int	  nCurApplyCnt;		// �̹��� ȹ���� �����	�Ϲ� 1, VIP�� 2
	int	  nTodayApplyCnt;	// ���� �� �����
	int	  nTotalApplyCnt;	// �� �����
	bool  bApplyLimit;		// ȹ������
	
} PCROOMACT_ALRAM;

/* 
OPCODE_SV_PCROOMACT_INITIALIZE  PC�� Ȱ��ȭ �ʱ�ȭ
*/
typedef struct _PCROOMACT_INITIALIZE
{
	int	  nTodayApplyCnt;	// ���� �� �����

} PCROOMACT_INITIALIZE;

/* 
OPCODE_SV_PCROOMACT_END  PC�� Ȱ��ȭ ��
*/
typedef struct _PCROOMACT_END
{
	bool bEndPromotion;	

} PCROOMACT_END;

///////////////////////////////////////////////////////////////////////////////////
// END  PC�� Ȱ��ȭ
///////////////////////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////////////////////
// BEGIN  PC�� ��Ŀ ��
///////////////////////////////////////////////////////////////////////////////////
/*
OPCODE_SV_PCPZONE_START
OPCODE_SV_PCPZONE_ALRAM
*/
typedef struct _PCPZONE_ALRAM
{
	bool   bComplete;			// ����� ȹ�� true : false

	int    nlimitTodaySS;       // ���� �ð� �� 3�ð���
	bool   bLogOutProcess;		// �α׾ƿ� ó��

} PCPZONE_ALRAM;


/* 
OPCODE_SV_PCROOMZONE_INITIALIZE  PC�� ��Ŀ�� �ʱ�ȭ
*/
typedef struct _PCPZONE_INITIALIZE
{
	int		nlimitTodaySS;	// ���� �ð�( �α׾ƿ� ����ǵ� �ʱ�ȭ �޽��� ���� ���� )

} PCPZONE_INITIALIZE;

/* 
OPCODE_SV_PCROOMZONE_END  PC�� Ȱ��ȭ ��
*/
typedef struct _PCPZONE_END
{
	bool bEndPromotion;	

} PCPZONE_END;

///////////////////////////////////////////////////////////////////////////////////
// END   PC�� ��Ŀ ��
///////////////////////////////////////////////////////////////////////////////////





///////////////////////////////////////////////////////////////////////////////////
// BEGIN  ��Ŀ�Ӵ� ����
///////////////////////////////////////////////////////////////////////////////////
/*
OPCODE_SV_BETTING_START  PC�� Ȱ��ȭ ����
OPCODE_SV_BETTING_ALRAM  PC�� Ȱ��ȭ ����
*/
typedef struct _BETTING_ALRAM
{
	bool bComplete;			// ����� ȹ�� true : false

	bool  bDormantUser;		// �޸�����             ó�� �ѹ� 10�� ����
	int	  nPlayRoundCnt;	// �÷���ī��Ʈ
	int	  nCurApplyCnt;		// �̹��� ȹ���� �����	�Ϲ� 1
	int	  nTodayApplyCnt;	// ���� �� �����
	int	  nTotalApplyCnt;	// �� �����
	bool  bApplyLimit;		// ȹ������
	int   nTodayLoseCnt;	// ���� �Ҹ꿹�� ����� �ż�
	// �Ϸ� �ִ� 5��
	// �ش��� �ű�/�޸� ������ ��� �ִ� 15��
	bool  bDormantLimit;

} BETTING_ALRAM;

/* 
OPCODE_SV_BETTING_INITIALIZE  PC�� Ȱ��ȭ �ʱ�ȭ
*/
typedef struct _BETTING_INITIALIZE
{
	int	  nTodayApplyCnt;	// ���� �� �����
	int	  nTotalApplyCnt;   // �� �����
	int   nTodayLoseCnt;	// ���� �Ҹ꿹�� ����� �ż�
	// �Ϸ� �ִ� 5��
	// �ش��� �ű�/�޸� ������ ��� �ִ� 15��
	bool  bDormantLimit;

} BETTING_INITIALIZE;

/* 
OPCODE_SV_BETTING_END  PC�� Ȱ��ȭ ��
*/
typedef struct _BETTING_END
{
	bool bEndPromotion;	

} BETTING_END;

///////////////////////////////////////////////////////////////////////////////////
// END  ��Ŀ�Ӵ� ����
///////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////
// BEGIN �����ƹ�Ÿ ���θ��
///////////////////////////////////////////////////////////////////////////////////


typedef struct _CHANCEAVATA_ALRAM
{
	int nChanceAvataCount;
} CHANCEAVATA_ALRAM;
///////////////////////////////////////////////////////////////////////////////////
// END �����ƹ�Ÿ ���θ��
///////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////
// ���ټ� ���θ��
///////////////////////////////////////////////////////////////////////////////////
/*
OPCODE_SV_RETENTION_INITIALIZE
OPCODE_SV_RETENTION_ALRAM
OPCODE_SV_RETENTION_END
*/

typedef struct _RETENTION_ALRAM
{
	int nUserState;					// 0:�ű�, 1:�޸�, -1:����� �ƴ�
	bool bTrial;					// �̼� �޼� ( ���� �̼� �޼� ���� ���)
	bool bToday;					// ���� 10�� �̼� 10ȸ �޼�
	bool bTotal;					// ���� 100�� �̼� �޼�
	int nTodayWinCnt;				// ���� 10�� �¼�
	int nTotalWinCnt;				// ���� 100�� �¼�
	int nTrialDayCnt;				// ���� 10��, ���� 100�� �̼� ���� ��¥
	int nTrialPrizeState;			// 1�� �̼� �޼� (1:100��, 2:200��, 3:500��, 4:1000��, 5:10000��)
	int nTodayPrizeState;			// ���� 10�� �̼� �޼� (1:100��, 2:200��, 3:500��, 4:700��, 5:1000��)
	int nTotalPrizeState;			// ���� 100�� �̼� �޼� (1:����99, 2:�ֻ���1000, 3:�ֻ���3000, 4:ĳ�����10000)
	////////////////////////////////////////////////////////////////////////////////
	BYTE nMissionComplet[MAX_RETENTION_MISSION_DAY];	// �̼� �޼�  0:���� , 1:�̼� ������ , 2: �̼� �Ϸ�.

} RETENTION_ALRAM;

///////////////////////////////////////////////////////////////////////////////////
// ���ټ� ���θ��
///////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////
// ��Ŀ �ų� ���ټ� ���θ��
///////////////////////////////////////////////////////////////////////////////////
/*
OPCODE_SV_NEWYEARRETENTION_INITIALIZE
OPCODE_SV_NEWYEARRETENTION_ALRAM
OPCODE_SV_NEWYEARRETENTION_END
*/
typedef struct _NEWYEARRETENTION_ALRAM
{
	bool bComplete;						// ����� ȹ�� ����
	int nTodayCnt;						// ���� ȹ�� �����
	int nMaxTodayCnt;					// ���� ȹ�� ���� �����
	int nTotalCnt;						// ���� ȹ�� �����
	int nTodayWinCntLowRoom;			// 1��~1000��� �Ǽ�
	int nTodayWinCntHiRoom;				// 1���̻�� �Ǽ�
	int nMaxTodayWinCntLowRoom;			// 1��~1000��� ����� ȹ���� ���� �Ǽ�(20)	// ���� ��ý
	int nMaxTodayWinCntHiRoom;			// 1���̻�� ����� ȹ���� ���� �Ǽ�(10)		// �� ��ý�ϼ�
} NEWYEARRETENTION_ALRAM;
///////////////////////////////////////////////////////////////////////////////////
// ��Ŀ �ų� ���ټ� ���θ��
///////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////
// 9�� �����ɾ� ���θ��
///////////////////////////////////////////////////////////////////////////////////
/*
OPCODE_SV_USERCARE_INITIALIZE
OPCODE_SV_USERCARE_ALRAM
OPCODE_SV_USERCARE_END
*/
typedef struct _USERCARE_ALRAM
{
	bool bComplete;						// ����� ȹ�� ����

	int nSilverPlayCnt;		// �ǹ� �÷��� �Ǽ�
	int nSilverTodayPoint;	// �ǹ� ���� ȹ�� ����
	int nSilverTotalPoint;	// �ǹ� ���� ȹ�� ����

	int nGoldPlayCnt;			// ��� �÷��� �Ǽ�
	int nGoldTodayPoint;		// ��� ���� ȹ�� ����
	int nGoldTotalPoint;		// ��� ���� ȹ�� ����

	int nFirstPlayCnt;		// ù��° ���� ȹ�濡 �ʿ��� �Ǽ�
	int nSecondPlayCnt;		// �ι�°���� ���� ȹ�濡 �ʿ��� �Ǽ�

	//int nNormalPlayCnt;		// 
	//int nNormalTodayPoint;	// 
	//int nNormalTotalPoint;	//

	int nPlayCnt;			// �̰� �⺻���� ����Ѵ�.
} USERCARE_ALRAM;

///////////////////////////////////////////////////////////////////////////////////
// 10�� �Ҿ���� Ȳ���� ã�Ƽ� ���θ��
///////////////////////////////////////////////////////////////////////////////////
/*
OPCODE_SV_GOLDCARD_INITIALIZE
OPCODE_SV_GOLDCARD_ALRAM
OPCODE_SV_GOLDCARD_APPEAR
OPCODE_SV_GOLDCARD_END
*/
typedef struct _GOLDCARD_ALARM
{
	int nLowPlayCount;		// ���� ä�� �Ϸ� �Ǽ�
	int nGoldWinCount;		// ���ī�� ȹ���ǿ��� �¸��� Ƚ��
	int nMaxGoldWinCount;	// �ִ� ��� ī�� ���� Ƚ��

	int nHighPlayCount;		// ���� ä�� �Ϸ� �Ǽ�
	int nHighWinCount;		// ���� ä�� �¸� �Ǽ�
} GOLDCARD_ALARM;

// OPCODE_SV_GOLDCARD_REWARD
typedef struct _GOLDCARD_REWARD
{
	bool	bLow;				// ������ �̼� ����
	INT64	nRewardMoney;		// ���� �ݾ�
} GOLDCARD_REWARD;


///////////////////////////////////////////////////////////////////////////////////
// ����ȭ�� ī�� ���θ�� (only 7��Ŀ)
///////////////////////////////////////////////////////////////////////////////////
/*
OPCODE_SV_ADULTCARD_START
OPCODE_SV_ADULTCARD_INITIALIZE
OPCODE_SV_ADULTCARD_ALRAM
OPCODE_SV_ADULTCARD_REWARD
OPCODE_SV_ADULTCARD_END
*/
typedef struct _ADULTCARD_ALARM
{
	int iStep;				// �̼� �ܰ� (1 ~ 4)
	int iCard;				// ���� �ܰ迡�� ȹ��(����) ī��

	int nPlayCount;			// �Ϸ� �Ǽ�
	int nWinCount;			// �¼�
	int nContWinCount;		// ����

	int nPlayCountMission;		// 2�ܰ� ���� �Ǽ�
	int nWinCountMission;		// 3�ܰ� �¸� �Ǽ�
	int nJokboMission;			// 4�ܰ� ���� (�ǵ��� ���� ���θ�ǿ��� ���� ������)
	int nContWinCountMission;	// 5�ܰ� ���� Ƚ��
} ADULTCARD_ALARM;


///////////////////////////////////////////////////////////////////////////////////
// 12�� 003�̼��� �����϶� ���θ��
///////////////////////////////////////////////////////////////////////////////////
/*
OPCODE_SV_MISSION003_START
OPCODE_SV_MISSION003_INITIALIZE
OPCODE_SV_MISSION003_ALRAM
OPCODE_SV_MISSION003_END
*/
typedef struct _MISSION003_ALARM
{
	int nTotalPlayTimeSec;		// �� ���� �÷��� Ÿ��(��)
	int nRoundPlayTImeSec;		// �̹��� �÷��� Ÿ��(��)
} MISSION003_ALARM;

// OPCODE_SV_MISSION003_ALLIN
typedef struct _MISSION003_ALLIN
{
	INT64 nMoney;			// ���� ������
} MISSION003_ALLIN;


///////////////////////////////////////////////////////////////////////////////////
// 2013.02�� ��ĿŽ�� ���θ��
///////////////////////////////////////////////////////////////////////////////////
/*
OPCODE_SV_POKERADVENTURE_START
OPCODE_SV_POKERADVENTURE_INITIALIZE
OPCODE_SV_POKERADVENTURE_ALRAM
OPCODE_SV_POKERADVENTURE_END
*/
typedef struct _POKERADVENTURE_ALARM
{
	int nWinCount;		// �̹��� �÷��� Ÿ��(��)
} POKERADVENTURE_ALARM;

// OPCODE_SV_POKERADVENTURE_REWARD
typedef struct _POKERADVENTURE_REWARD
{
	INT64	nRewardMoney;		// ���� �ݾ�
} POKERADVENTURE_REWARD;

// OPCODE_SV_POKERADVENTURE_ALLIN
typedef struct _POKERADVENTURE_ALLIN
{
	INT64 nMoney;			// ���� ������
} POKERADVENTURE_ALLIN;


///////////////////////////////////////////////////////////////////////////////////
// 2013.04�� �������� ���θ��
///////////////////////////////////////////////////////////////////////////////////

/**
 * OPCODE_SV_HALFJACKPOT_TIME_BEFORE_START,
 * OPCODE_SV_HALFJACKPOT_TIME_START,
 * OPCODE_SV_HALFJACKPOT_TIME_BEFORE_END,
 * OPCODE_SV_HALFJACKPOT_TIME_END
 * MS->GS->CL
 */
typedef struct _HALFJACKPOT_TIME
{
	UINT32	nRemainTimeSec;	// ����/���� ���� ���� �ð�(��)
} HALFJACKPOT_TIME;

enum
{
	CHANNEL_100000000 = 0,		// 1���
	CHANNEL_1000000000,
	CHANNEL_10000000000,
	CHANNEL_100000000000,
	CHANNEL_1000000000000,
	CHANNEL_3000000000000,		// 3�� ��
	CHANNEL_10000000000000,		// 10�� ~ 100�� ��
	HALFJACKPOT_CHANNEL_MAX,
};

/**
 * OPCODE_SV_HALFJACKPOT_MONEY
 * MS->GS->CL
 */
typedef struct _HALFJACKPOT_MONEY
{
	INT64	llJackpotMoney[ HALFJACKPOT_CHANNEL_MAX ];	// ä�κ� ���� ���� �Ӵ�(����)
} HALFJACKPOT_MONEY;

/**
 * OPCODE_SV_HALFJACKPOT_ALARM
 * MS->GS->CL
 */
typedef struct _HALFJACKPOT_ALARM
{
	int iChannel;					// ä�� ��ȣ
	UINT32	nTopHalfCount;			// ä�κ� �ĺ��濡 ��� ���� ���� ī��Ʈ
	UINT32	nRemainTimeSec;			// ����Ÿ�� ������� ���� �ð�(��)
	UINT32  nHalfCount;				// �ش� ���� ���� ī��Ʈ
} HALFJACKPOT_ALARM;


#define MAX_TOP_HALF_ROOM	(16)

/**
 * OPCODE_SV_HALFJACKPOT_ROOM
 * MS->GS->CL
 */
typedef struct _HALFJACKPOT_ROOM
{
	int		iChannel;								// ä��
	int		nCount;									// �� ����
	INT64	llEnterLimitMoney[MAX_TOP_HALF_ROOM];	// �ش� ä���� ���̹� ��ȣ
	int		iRoomNum[MAX_TOP_HALF_ROOM];			// �ش� ä���� ���̹� ��ȣ
	int		iTenProRoomNum;							// 0�̸� ��� ���̹��� ��÷�� ȹ��, �ƴϸ� �� ���ȣ�� ������ ������ ���̹��� ��÷���� 10%�� ����
	INT64	llJackpotMoney;							// ���� �ݾ�(��÷�� ������ ���޵�)
} HALFJACKPOT_ROOM;

/**
 * OPCODE_SV_HALFJACKPOT_ENTER_JACKPOT_ROOM
 * OPCODE_SV_HALFJACKPOT_JACKPOT_ROUND_START
 * GS->CL
 */
typedef struct _HALFJACKPOT_ENTER_JACKPOT_ROOM
{
	INT64	llPrizeMoney;							// ��÷�� ���޵� �ݾ�
} HALFJACKPOT_ENTER_JACKPOT_ROOM;

/**
 * OPCODE_SV_HALFJACKPOT_PRIZE
 * GS->MS->GS->CL
 */
typedef struct _HALFJACKPOT_PRIZE
{
	int		iChannel;				// ä��
	int		iRoomNum;				// ��÷��
	INT64	llEnterLimitMoney;		// ��÷���� ä�� �ݾ�
	char	szID[ID_LEN];			// ID
	char	szNick[NICK_LEN];		// �г���
	INT64	llPrizeMoney;			// ���� �ݾ�
} HALFJACKPOT_PRIZE;


/**
 * OPCODE_SV_HALFJACKPOT_TOP_HALF_COUNT_MS
 * GS->MS
 */
typedef struct _HALFJACKPOT_TOP_HALF_MS
{
	int iChannel;
	int iRoomNum[MAX_TOP_HALF_ROOM];
	int nHalfCount[MAX_TOP_HALF_ROOM];
	INT64 llSeedMoney[MAX_TOP_HALF_ROOM];
	INT64 llEnterLimitMoney[MAX_TOP_HALF_ROOM];
} HALFJACKPOT_TOP_HALF_MS;

/**
 * OPCODE_SV_HALFJACKPOT_DELETE_ROOM_MS
 * GS->MS
 */
typedef struct _HALFJACKPOT_DELETE_ROOM_MS
{
	int iChannel;
	int iRoomNum;

} HALFJACKPOT_DELETE_ROOM_MS;

/**
 * OPCODE_SV_HALFJACKPOT_END_HALF_MS
 * GS->MS
 */
typedef struct _HALFJACKPOT_END_HALF_MS
{
	int iChannel;
} HALFJACKPOT_END_HALF_MS;


///////////////////////////////////////////////////////////////////////////////////
// 2013�� ���? ���θ��
///////////////////////////////////////////////////////////////////////////////////

/*
OPCODE_SV_MISSION201304_START
OPCODE_SV_MISSION201304_INITIALIZE
OPCODE_SV_MISSION201304_ALRAM
OPCODE_SV_MISSION201304_REWARD
OPCODE_SV_MISSION201304_END
*/
typedef struct _MISSION201304_ALARM
{
	int iStep;				// �̼� �ܰ� (1 ~ 4)
	int iCard;				// ���� �ܰ迡�� ȹ��(����) ī��

	// �̼� ���� ����
	int nPlayCount;			// ������ �Ǽ�
	int nWinCount;			// �޼��� �¼�
	int nJokbo;				// �޼� ���� (���� ���� �̼��� ��� ������� ����)
	int nAllinCount;		// ��븦 ���ν�Ų Ƚ��
	int nContWinCount;		// ���� Ƚ�� (not used yet)
	int nPlayTimeSec;		// �÷��� �ð�  (not used yet)

	// ���� �̼� ����
	int nPlayCountMission;		// ���� �Ǽ� ����
	int nWinCountMission;		// �¸� �Ǽ� ����
	int nJokboMission;			// ���� �̼� (���� ���� �̼��� ��� ������� ����)
	int nAllinCountMission;		// ��� ���� Ƚ�� ����
	int nContWinCountMission;	// ���� Ƚ�� ����
	int nPlayTimeSecMission;	// �÷��� �ð� ����

	int nAttandanceCount;		// �⼮ �ϼ�

} MISSION201304_ALARM;


///////////////////////////////////////////////////////////////////////////////////
// �����϶� 1996
///////////////////////////////////////////////////////////////////////////////////
/*
OPCODE_SV_BORNIN1996_START
OPCODE_SV_BORNIN1996_INITIALIZE
OPCODE_SV_BORNIN1996_ALRAM
OPCODE_SV_BORNIN1996_END
*/
typedef struct _BORNIN1996_ALARM
{
	bool bIsEventTarget;			// �̺�Ʈ ��� ���� (1996�������)
	bool bTodayComplete;		// ���� �޼� ����
	int nAttandanceCount;		// �⼮ �ϼ�

} BORNIN1996_ALARM;


///////////////////////////////////////////////////////////////////////////////////
// �����϶� 1996
///////////////////////////////////////////////////////////////////////////////////
/*
OPCODE_SV_LOTTERY201406_ALARM_START
OPCODE_SV_LOTTERY201406_ALARM_INITIALIZE
OPCODE_SV_LOTTERY201406_ALARM_ALRAM
OPCODE_SV_LOTTERY201406_ALARM_END
*/
typedef struct _LOTTERY201406_ALARM
{
	int iStep;
	int nGoal[LOTTERY201406_MISSION_STEP_MAX];
	int nTicket[LOTTERY201406_MISSION_STEP_MAX];
	int nPlayCount;
	bool bDoubleReward;

} LOTTERY201406_ALARM;

//Harf���θ��
typedef struct _HARF_PROMOTION_ALARM
{
	int nType;

	int nGoldCount;
	int nSilverCount;
} HALF_PROMOTION_ALARM;


//�⼮üũ���θ��
typedef struct _VISITE_PROMOTION_ALARM
{
	int nVisiteCount;			// �湮 ��
	int nWinCount;				// ���� �¼�
	int nTodayRewardCount;		// ���� ȹ�� ��
	int nTotalRewardCount;		// ��ü ȹ�� ��
	INT64 nRewardMoney;			// ��÷ ��
	int nRewardIndex;	//�̹���ó���� ��÷�� �ε��� ��ȣ�̴�.///////////////////////////2017.04.27 ��Ű�� ���θ�� �۾�///////////////////////////////

	INT64 nRunLowerLimit; //�ش� ���θ���� �۵��ϱ� ���� ä������ ���Ѽ�
	INT64 nRunUpperLimit; //�ش� ���θ���� �۵��ϱ� ���� ä������ ���Ѽ�
	//�� �ΰ��� ���̿� �־��(�� ���� ����) �̺�Ʈ�� �۵���

} VISITE_PROMOTION_ALARM;


typedef struct _VISITE_SUB_PROMOTION_ALARM
{
	bool bReward;
	USHORT nRewardType;	// ��ǰ��ȣ 1 ~ 	
} VISITE_SUB_PROMOTION_ALARM;

typedef struct _VISITE_SUB_PROMOTION_REWARD_REG_INFO_ALARM
{
	bool bReward;	
} VISITE_SUB_PROMOTION_REWARD_REG_INFO_ALARM;


/*
OPCODE_SV_PLAYCOUNT_START,	//�÷��� �Ǽ��� ���� ���θ��
OPCODE_SV_PLAYCOUNT_ALARM,
OPCODE_SV_PLAYCOUNT_REWARD,
OPCODE_SV_PLAYCOUNT_END,
*/
typedef struct _MISSION201502_ALARM
{
	int iStep;				// �̼� �ܰ� (1 ~ 6)	
	int nPlayCount;			// ������ �Ǽ�
	bool bIsComplete;		// ���� �̼��� ��� �Ϸ��ߴ��� ? true(���̻� �������)/false(������..)
} MISSION201502_ALARM;

typedef struct _MISSION201502_REWARD
{
	int iStep;				// �̼� �ܰ� (1 ~ 6)	
	INT64 nReWardMoney;		// ��÷ �ݾ�
	bool bIsComplete;		// �������� ���θ���ΰ�? true/false
} MISSION201502_REWARD;




typedef struct _RETENTION_PROMOTION_REWARD
{
	BYTE	nPlayCount;			// ���� Ƚ��
	BYTE	nDay;				// ���� ��¥�� �迭��
	bool	bIsComplete;		// �̼� ��������
	INT64	nReWardMoney;		// ��÷ �ݾ�
} RETENTION_PROMOTION_REWARD;


typedef struct _PLAY_COUPON_ALRAM
{
	bool bPromotion;					// ���θ�� ���ۿ���
	int  nPlayCount;					// ���� �Ǽ�
	int  nCoupon;						// ����� ��
	
} PLAY_COUPON_ALRAM;




typedef struct _LEAGUE_REWARD_MONEY
{	
	INT64 nRewardMoney;		// ���� ���� �����
} LEAGUE_REWARD_MONEY;

///////////////////////////////////////////////////////////////////////////////////
// 2016.01 VIP Jackpot �̺�Ʈ
///////////////////////////////////////////////////////////////////////////////////

/**
 * OPCODE_SV_HALFJACKPOT_TIME_BEFORE_START,
 * OPCODE_SV_HALFJACKPOT_TIME_START,
 * OPCODE_SV_HALFJACKPOT_TIME_BEFORE_END,
 * OPCODE_SV_HALFJACKPOT_TIME_END
 * MS->GS->CL
 */


enum VIPJACKPOT_STATE
{
	VIPJACKPOT_BEFORE_START,
	VIPJACKPOT_PARTY,
	VIPJACKPOT_BEFORE_END,
	VIPJACKPOT_END,
	VIPJACKPOT_WAIT_PULL_JACKPOT_ROOM,
	VIPJACKPOT_JACKPOT_GAME_PLAYING,	// ���� ������
	VIPJACKPOT_NONE,
};

enum
{	
	VIP_CHANNEL_500000000000,		// 5õ�� ��  VIP �̻���� ���۵�
	VIP_CHANNEL_1000000000000,
	VIP_CHANNEL_3000000000000,		// 3�� ��
	VIP_CHANNEL_10000000000000,		// 10�� ~ 100�� ��
	VIP_CHANNEL_30000000000000,		// 30�� ~ 50�� ��
	VIP_CHANNEL_50000000000000,		// 50�� ~ 
	VIPJACKPOT_CHANNEL_MAX,
};



/**
 * OPCODE_SV_HALFJACKPOT_MONEY
 * MS->GS->CL
 */

typedef struct _VIPJACKPOT_TIME
{
	UINT32	nRemainTimeSec;			// ����/���� ���� ���� �ð�(��)
	int		nTodayEventCount;		// �̺�Ʈ Ƚ��
	int		nEventState;			// �̺�Ʈ ���°�
} VIPJACKPOT_TIME;


typedef struct _VIPJACKPOT_MONEY
{
	INT64	llJackpotMoney[ VIPJACKPOT_CHANNEL_MAX ];	// ä�κ� ���� ���� �Ӵ�(����)
} VIPJACKPOT_MONEY;

/**
 * OPCODE_SV_VIPJACKPOT_ALARM
 * MS->GS->CL
 */
typedef struct _VIPJACKPOT_ALARM
{
	int iChannel;					// ä�� ��ȣ
	UINT32	nTopHalfCount;			// ä�κ� �ĺ��濡 ��� ���� ���� ī��Ʈ
	UINT32	nRemainTimeSec;			// ����Ÿ�� ������� ���� �ð�(��)
	UINT32  nHalfCount;				// �ش� ���� ���� ī��Ʈ
	int		nTodayEventCount;		// �̺�Ʈ Ƚ��
	int		nEventState;			// �̺�Ʈ ���°�
	bool	bJackpotRoom;			// ���� ���̷� ���� 	
} VIPJACKPOT_ALARM;

typedef struct _VIPJACKPOT_BET_POINT_ALARM
{
	int iChannel;					// ä�� ��ȣ
	UINT32	nTopHalfCount;			// ä�κ� �ĺ��濡 ��� ���� ���� ī��Ʈ
	int		nTodayEventCount;		// �̺�Ʈ Ƚ��		
} VIPJACKPOT_BET_POINT_ALARM;



#define MAX_TOP_VIP_JACKPOT_ROOM	(11)
#define MAX_GAME_SU		(jackpot::GAME_ALL)

/**
 * VIPJACKPOT_TOTAL_EVENT_SUCESS_ROOM_INFO
 * MS->GS->CL
 */
typedef struct _VIPJACKPOT_ROOM
{
	int		iChannel;								// ä��
	int		nCount;									// �� ����
	INT64	llEnterLimitMoney[MAX_TOP_VIP_JACKPOT_ROOM];	// �ش� ä���� ���̹� ��ȣ
	int		iRoomNum[MAX_TOP_VIP_JACKPOT_ROOM];			// �ش� ä���� ���̹� ��ȣ
	//int		iSucessRoomNum[MAX_TOP_VIP_JACKPOT_ROOM];	// �� ���ȣ�� 
	int		iTenProRoomNum;							// 0�̸� ��� ���̹��� ��÷�� ȹ��, �ƴϸ� �� ���ȣ�� ������ ������ ���̹��� ��÷���� 10%�� ����
	INT64	llJackpotMoney;							// ���� �ݾ�(��÷�� ������ ���޵�)
	int		nGameCode;								// ���� ��ȣ	
} VIPJACKPOT_ROOM;

typedef struct _VIPJACKPOT_EVENT_SUCESS_ROOM_INFO
{   
	int      nGameCode[MAX_TOP_VIP_JACKPOT_ROOM];        // ���� ��ȣ
	INT64    llEnterLimitMoney;
	int      iRoomNum[MAX_TOP_VIP_JACKPOT_ROOM];         // �ش� ä���� ���̹� ��ȣ   
} VIPJACKPOT_EVENT_SUCESS_ROOM_INFO;

typedef struct _VIPJACKPOT_TOTAL_EVENT_SUCESS_ROOM_INFO
{   
	VIPJACKPOT_EVENT_SUCESS_ROOM_INFO stEventSucessRoomInfo[MAX_GAME_SU][VIPJACKPOT_CHANNEL_MAX];
} VIPJACKPOT_TOTAL_EVENT_SUCESS_ROOM_INFO;   // ��ü ���̹� ���� ����

/**
 * OPCODE_SV_VIPJACKPOT_ENTER_JACKPOT_ROOM
 * OPCODE_SV_VIPJACKPOT_JACKPOT_ROUND_START
 * GS->CL
 */
typedef struct _VIPJACKPOT_ENTER_JACKPOT_ROOM
{
	INT64	llPrizeMoney;							// ��÷�� ���޵� �ݾ�	
} VIPJACKPOT_ENTER_JACKPOT_ROOM;

/**
 * OPCODE_SV_VIPJACKPOT_PRIZE
 * GS->MS->GS->CL
 */
typedef struct _VIPJACKPOT_PRIZE
{
	int		iChannel;				// ä��
	int		iRoomNum;				// ��÷��
	INT64	llEnterLimitMoney;		// ��÷���� ä�� �ݾ�
	char	szID[MAX_PLAYER][ID_LEN];			// ID
	char	szNick[MAX_PLAYER][NICK_LEN];		// �г���
	INT64	llPrizeMoney[MAX_PLAYER];			// ���� �ݾ�
	int		nGameCode;				// ���Ӻ� �ڵ��ȣ
} VIPJACKPOT_PRIZE;		// ��� ���� ����


typedef struct _VIPJACKPOT_PRIZE_LOG_INFO
{	
	int		iRoomNum;				// ��÷��
	INT64	llEnterLimitMoney;		// ��÷���� ä�� �ݾ�
	char	szID[ID_LEN];			// �г���
	INT64	llPrizeMoney;			// ���� �ݾ�
	int		nGameCode;				// ���Ӻ� �ڵ��ȣ
} VIPJACKPOT_PRIZE_LOG_INFO;	// ���� ��÷�� ����

typedef struct _VIPJACKPOT_TOTAL_JACKPOT_EVENT_INFO
{
	VIPJACKPOT_PRIZE_LOG_INFO stPrizeInfo[MAX_GAME_SU][VIPJACKPOT_CHANNEL_MAX][MAX_TOP_VIP_JACKPOT_ROOM];
} VIPJACKPOT_TOTAL_JACKPOT_EVENT_INFO;		// ��ü ��÷�� ���� ����

typedef struct _VIPJACKPOT_TOTAL_WAIT_ROOM_INFO
{
	int stWaitRoomInfo[VIPJACKPOT_CHANNEL_MAX][MAX_TOP_VIP_JACKPOT_ROOM];
	UINT32	nRemainTimeSec;			// ����Ÿ�� ������� ���� �ð�(��)
} VIPJACKPOT_TOTAL_WAIT_ROOM_INFO;		// ��ü ��÷�� ���� ����


/**
 * OPCODE_SV_VIPJACKPOT_TOP_HALF_COUNT_MS
 * GS->MS
 */
typedef struct _VIPJACKPOT_TOP_HALF_MS
{
	int nGameCode;
	int iChannel;
	int iRoomNum[MAX_TOP_VIP_JACKPOT_ROOM];
	int nHalfCount[MAX_TOP_VIP_JACKPOT_ROOM];
	INT64 llSeedMoney[MAX_TOP_VIP_JACKPOT_ROOM];
	INT64 llEnterLimitMoney[MAX_TOP_VIP_JACKPOT_ROOM];
} VIPJACKPOT_TOP_HALF_MS;

/**
 * OPCODE_SV_VIPJACKPOT_DELETE_ROOM_MS
 * GS->MS
 */
typedef struct _VIPJACKPOT_DELETE_ROOM_MS
{
	int iChannel;
	int iRoomNum;

} VIPJACKPOT_DELETE_ROOM_MS;

/**
 * OPCODE_SV_VIPJACKPOT_END_HALF_MS
 * GS->MS
 */
typedef struct _VIPJACKPOT_END_HALF_MS
{
	int iChannel;
} VIPJACKPOT_END_HALF_MS;

typedef struct _VIPJACKPOT_BET_POINT
{
	int nRoomJackpotPoint;	// ���� �� ���� ����Ʈ
	int nNowBetPoint;		// ���� ���ÿ��� ȹ���� ����
	int nMinRoomListBetPoint;	// ���濡 ���� �ִ� ���� ����Ʈ
}VIPJACKPOT_BET_POINT;

typedef struct _VIPJACKPOT_MONEY_INFO
{	
	INT64 nJackpotMoney[MAX_GAME_SU][VIPJACKPOT_CHANNEL_MAX];
	INT64 nMaxJackpotMoney[MAX_GAME_SU][VIPJACKPOT_CHANNEL_MAX];
}VIPJACKPOT_MONEY_INFO;



/**
 * OPCODE_SV_VIPJACKPOT_MS_BETPOINT_INFO
 * GS->MS
 */
typedef struct _VIPJACKPOT_MS_BETPOINT_INFO
{
	int iChannelType;			// ä�� Type A ~ D (1-4)
	int iRoomNum;				// ���ȣ
	int nHalfCount;				// ���� ����Ʈ	
	INT64 llEnterLimitMoney;
	INT64 llSeedMoney;
	int nGameCode;
} VIPJACKPOT_MS_BETPOINT_INFO;


////////////////////////////�Ѱ���, ���뷮Ȯ�� 2017.08.25/////////////////////////////////////
typedef struct _SUPERMOON_PROMOTION_ALARM
{
	int TodayGameCount; //���� ������ ��Ƚ��
	int CurrentLevel; //���� ����.. -1�̸� ������ �����̴�.
	bool GiftOpen[4]; //�� ������ ���� ���� �����̴�.
	int ThisLevel_GoalCount; //��ǥ ī��Ʈ(�ش� ���� �Ǽ��� �Ǹ� ���� ����)
	int ThisLevel_CurrentCount; //���� �������� ����� �����Ǽ�

} SUPERMOON_PROMOTION_ALARM;
/////////////////////////////////////////////////////////////////


////////////////////////////2018 �߼� �̺�Ʈ. ���� �⼮ & �Ǽ� ���� 2018.09.20/////////////////////////////////////
// �⼮ ���� ���� ����.
struct PROMOTION_2018THANKSGIVING_ATTEDANCE_DAY_INFO
{
	int nAttendanceMonth;		// �⼮ �� (9�� 9)
	int nAttendanceDay;			// �⼮ �� (20�� 20)
	INT64 llReward;				// �⼮ ���� (100��)
	INT64 llAdditionalReward;	// �߰� ���� (7�� ���ٽ�...) (500��)
	bool bIsAttendance;			// �⼮ ����
	//	int nAdditionalRewardCount;	// �߰� ���� Ƚ��(7�� ���� -> 7, 3�� ���� -> 3)
};

// �⼮ ���� ��ü ����
struct PROMOTION_2018THANKSGIVING_ATTEDANCE_TOTAL_INFO
{
	std::vector<PROMOTION_2018THANKSGIVING_ATTEDANCE_DAY_INFO> vecAttendanceTotalInfo;		// �����ϱ����� ��Ƽ�...
	INT64 llAdditionalFinalReward;		// ���� ���ٽ� �߰� ���� (28�� ���ٽ�...)	3000��
	//int nAdditionalRewardFinalCount;	// ���� ���ٽ� �߰� ���� Ƚ��(28�� ���� -> 28, 14�� ���� -> 14)

	PROMOTION_2018THANKSGIVING_ATTEDANCE_TOTAL_INFO& operator=(const PROMOTION_2018THANKSGIVING_ATTEDANCE_TOTAL_INFO& src)
	{
		llAdditionalFinalReward = src.llAdditionalFinalReward;		
		vecAttendanceTotalInfo.assign(src.vecAttendanceTotalInfo.begin(), src.vecAttendanceTotalInfo.end());
		return *this;
	}

	void Clear()
	{
		vecAttendanceTotalInfo.clear();
		llAdditionalFinalReward = 0;
	}
};

// Ƚ�� �̺�Ʈ�� ����Ǵ� ���� ��Ƽ.
struct PROMOTION_2018THANKSGIVING_GAMECOUNT_INFO
{
	INT64 llRunLowerLimit; //�ش� ���θ���� �۵��ϱ� ���� ä������ ���Ѽ�		1
	INT64 llRunUpperLimit; //�ش� ���θ���� �۵��ϱ� ���� ä������ ���Ѽ�		1000��
	//�� �ΰ��� ���̿� �־��(�� ���� ����) �̺�Ʈ�� �۵���
};

// ���� ����� Ƚ��, ���� ��Ƽ.
struct PROMOTION_2018THANKSGIVING_GAMECOUNT
{
	int nTodayGameCount;		// ���� �� ���� Ƚ��		(1~150)
	INT64 llReward;				// ����(5�� ����, ������ 0) (10, 100, 1,000, 10,000)
	INT64 llAdditionalReward;	// �߰� ����(30�� ����, ������ 0) (500, 5,000, 10,000, 1,000,000)
};

// �⼮ ��û
class CSV_PROMOTION_2018THANKSGIVING_ATTEDANCE_TOTAL_INFO : public CCommMsg
{
public:
	INT64* m_llAdditionalFinalReward;			// ���� ���ٽ� �߰� ���� (28�� ���ٽ�...)	3000��
	size_t* m_nInfoSize;
	PROMOTION_2018THANKSGIVING_ATTEDANCE_DAY_INFO* m_pAttDayInfos;

	void Set(const PROMOTION_2018THANKSGIVING_ATTEDANCE_TOTAL_INFO& totalInfo) 
	{
		SetHeader(OPCODE_SV_2018THANKSGIVING_ATTENDANCE_INFO);

		size_t nInfoListSize = totalInfo.vecAttendanceTotalInfo.size();

		PushData(_PVAR(m_nInfoSize), &nInfoListSize, sizeof(*m_nInfoSize));

		for (size_t i = 0; i < nInfoListSize; ++i)
		{
			PushData(_PVAR(m_pAttDayInfos), &totalInfo.vecAttendanceTotalInfo[i], sizeof(PROMOTION_2018THANKSGIVING_ATTEDANCE_DAY_INFO));
		}

		PushData(_PVAR(m_llAdditionalFinalReward), &totalInfo.llAdditionalFinalReward, sizeof(*m_llAdditionalFinalReward));
	}

	void Get(char* lpdata, int size) 
	{
		if(!GetHeader(lpdata, size)) return;

		TakeData(_PVAR(m_nInfoSize), sizeof(*m_nInfoSize));
		TakeData(_PVAR(m_pAttDayInfos), sizeof(PROMOTION_2018THANKSGIVING_ATTEDANCE_DAY_INFO) * (*m_nInfoSize));
		TakeData(_PVAR(m_llAdditionalFinalReward), sizeof(*m_llAdditionalFinalReward));
	}
};
//////////////////////////////////////////////////////////////////////////

}/* namespace promotion */



