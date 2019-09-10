#pragma once




//��������� ���� �켱���� ����Ʈ
//#define END_GONIEVENT	7	//Ÿ¥�̺�Ʈ
#define	END_ROOMJACKPOT 6	//�����̽� 
#define	END_ALLIN		5	//���ν���
#define END_SORT		4	//�Ѳ����̽���
#define END_DRAW		3	//���̽��� 
#define	END_BIG			2	//��ڽ�
#define END_SMALL		1	//�ʹڽ�
#define END_NORMAL		0	//�Ϲݽ��� 

enum AV_VOICE
{
	AV_BANISHVOTE = 0,	//�������忹��� 
	AV_SUPERBANISH,		//���۹����� ���� ��ų��
	AV_FIRSTBET,		//�ʱ� ���ý� 
	AV_GETHIDDEN,		//����ī��޾�����
	AV_JOINROOM,		//���ӿ� ����������
	AV_WATCHSUBSCRIPT,	//��������
	AV_ACTIONJOKBO,		//������ ������� 

	AV_MAKE_HIDDEN,		//���������ϼ�	
	AV_FAIL_HIDDEN,		//������������ //7

	AV_OTHER_ALLIN,		//������ ���ν������� 
	AV_MY_ALLIN,		//���� ���δ�������	//10

	AV_BET,				//����

	AV_SORT_LOSS,		//�Ѳ����̷� ����
	AV_DRAW_LOSS,		//���и� ���� ���ϰ� ��� ������ �����Ѱ��
	AV_NORMAL_LOSS,		//�Ϲ��� ���� 2������ �̿��ϰ��

	AV_SORT_WIN,		//�Ѳ����� �¸� 
	AV_DRAW_WIN,		//���̽�
	AV_BIG_WIN,			//��ڽ� (5���̻� ��)
	AV_SMALL_WIN,		//�ʹڽ� (4������ ��)
	AV_NORMAL_WIN,		//�Ϲݽ� (���� �¸����׿� �������)

	AV_49_REMATCH,		//49�� �ٽ� ��� 
	AV_JOKBO_REMATCH,	//������ ��� 

	AV_ROOMJACPOT_START,	//������ ���� 
	AV_ROOMJACKPT_WIN,		//�������ǿ��� �¸�	//23
};


class CSutdaGameSound
{

	int m_nVoiceType;		//����â�� ������ ���̽� Ÿ�� (1~4)

	int m_vWinandLossVoice;	//���п� ���� ����ȿ��

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
