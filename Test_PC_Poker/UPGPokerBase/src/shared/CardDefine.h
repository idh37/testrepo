
// CardDefine.h

#ifndef CARDDEFINE_H_
#define CARDDEFINE_H_

#define CARD_GAB_Y		9

#define FLY_CARD_POS_X	457
#define FLY_CARD_POS_Y	15

enum EVENT_ENUM
{
	// [ ���� ��Ŀ ]
	EVENT_PREPARESTART_ONEEND = 0, // ī�� ���ö� ���� �޾ҵ�
	EVENT_THREEFLYEND = 1,		   // ù 3���� ��� ���ȴ�
	EVENT_ONEFLYEND,			   // ���徿
	EVENT_ONEFLYTOTEND,			   // ���徿 ���

	EVENT_BD_PREPARESTART_ONEFLY_END,	// ó�� 4�� �� ���� ����
	EVENT_BD_PREPARESTART_FOURFLY_END,  // ó�� 4�� ���� ����
	EVENT_BD_CUTTING_WAIT,				// Ŀ�� ī�� �̵� ��� �� ������ Ÿ���� 0�϶� ȣ��
	EVENT_BD_CUTTING_MOVE_UP,			// Ŀ�� ī�� ���� �̵� ���� 
	EVENT_BD_CUTTING_MOVE_DOWN,			// Ŀ�� ī�� ���� ����
	EVENT_BD_CUTTING_ALL_DOWN,			// Ŀ�� ī�� ���� ���
	EVENT_BD_CUTTING_PASS,				// Ŀ�� �н�( �̵����� �̺�Ʈó�� ���� )
	EVENT_BD_CUTTING_ALL_PASS,			// Ŀ�� �н� ���� ���

	EVENT_BOMBFLYEND,
	EVENT_BOMBFLY_TO_END,
	EVENT_CHOICESELECT_AFTER_ONEEND,//���̽� ī�� ���� �� ���徿 ������


	//����
	EVENT_CARD_GATHEREND,			//��� �� ī�� ������ ��
	EVENT_CARD_OPENEND_EFT,			//��� ���½� ����Ʈ ó��
	EVENT_CARD_END_RESULT_CARD,		//��� ī�� ���� ����
};

typedef struct
{
	int CardNo;			// ī�� ��ȣ
	int EventCode;		// �̺�Ʈ �ڵ�
	int Option;			// �̺�Ʈ �ɼ�
	int Option2;		// �ɼ� 2
	int Option3;
} CARDEVENT;

typedef struct
{
	int Code;
	int Option;			// �̺�Ʈ �ɼ�
	int Option2;		// �ɼ� 2
	int Option3;
}RESERVATION;

enum CHIPKIND
{
	StartChip = 0,
	_5=StartChip,	//0
	_10,		//1
	_100,		//2
	_1K,		//3
	_10K,		//4
	_100K,		//5
	_1M,		//6
	_10M,		//7
	_100M,		//8
	_1Y,		//9
	_10Y,		//10
	_100Y,		//11
	_1000Y,		//12
	_1J,		//13
	_10J,		//14
	_100J,		//15
	_1000J,		//16
	_1KJ,		//17
	_10KJ,		//18
	_100KJ,		//19
	_1000KJ,	//20
	EndChip,
};

typedef struct 
{
	char chipnum;		// Ĩ ��	
	char sprnum;		// ��������Ʈ �ѹ� 
	char Twincnt[10];	// ����

	CPoint ptPos;	
}CHIPDATA;

typedef struct _tagMYCHIPDATA
{
	char	chipnum;			// Ĩ ��	
	char	sprnum;				// ��������Ʈ �ѹ�
	DWORD	dwStartTime;
	DWORD	dwMoveTime;
	int		nAlpha;
	bool	bMoveEnd;

	CPoint CurPos[10];			// �׷����� ��ǥ
	CPoint TarPos[10];			// Ÿ�� 
	CPoint StartPos[10];		// ����
}MYCHIPDATA;


#endif