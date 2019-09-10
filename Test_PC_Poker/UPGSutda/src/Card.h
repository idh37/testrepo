// Card.h: interface for the CCard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CARD_H__34924AA1_21CA_11D4_97A5_0050BF0FBE67__INCLUDED_)
#define AFX_CARD_H__34924AA1_21CA_11D4_97A5_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graphic.h"

enum
{
	EVENT_IAMREADY =1,			// ������ ������ �غ� �Ǿ���
	EVENT_FIRSTBET=2,			// ù��° ī�忡�� ����ī�尡 ����
	EVENT_FLOORTOGETCARD=4,		// ���� ī�带 ������
	EVENT_DUMMYTOMYCARD=8,		// ī�� ���̿��� ������ ������ �÷��̾�� �ѱ��
	EVENT_ENDMYTURN=16,			// �����ʰ� ����
	EVENT_JOKBO=32,				// ���� ���� �̺�Ʈ  
	EVENT_AUTOHIDDEN=64,		// �ڵ��ɱ⸦�Ѵ�.
	EVENT_CARDOPEN=128,			// ī�带 �����ϰ� ������ Ȯ���Ѵ�.
	EVENT_OPENEFT=256,			// ���������� ����Ʈ ȿ���� �ش�.
	EVENT_JOKBOCHECK=512,		// ����Ȯ�� �ߴٴ� �޼����� �ش�.
	EVENT_WINDCARDEND=1024,		// ������ī�带 ������.
};

typedef struct
{
	int CardNo;			// ī�� ��ȣ
	int EventCode;		// �̺�Ʈ �ڵ�
	int Option;			// �̺�Ʈ �ɼ�
	int Option2;		// �̺�Ʈ �ɼ�2
} CARDEVENT_SUTDA;

class CCard  
{
public:
	void CardValue(int cardnum);
	void SetClickPos(int x,int y);
	NMBASE::GRAPHICGDI::CPage *pPage;
	NMBASE::GRAPHICGDI::xSprite *m_pHwatuNSpr;

	CARDEVENT_SUTDA Event;
	CARDEVENT_SUTDA Event2;

	// ī�� �Ӽ�
	int CardNo;			// ī���ȣ(0: ������ ������ ī�� �̰ų� �Ⱦ���, 1-40:�⺻�� )
	int Who;			// ī���� ������(10:�⸮ 9:�� 0-2:�÷��̾�)
	int Kind;			// ���� �з� (1-12��... (13=������))

	int ShowMode;		// �����ֱ� ���(0:���� ���� 1:������ �� �̵��� ������)

	// ����
//	int MoveStartSnd;	// �̵��� ���۵ɶ��� ����
	//int MoveEndSnd;		// �̵��� ���������� ����
	char *MoveStartSnd;	// �̵��� ���۵ɶ��� ����
	char *MoveEndSnd;	// �̵��� ���������� ����

	// �˸� �̺�Ʈ
	BOOL bNotifyEvent;	// �˸� �̺�Ʈ�� �ִ°�?
	COLORREF NotifyStringColor;	// �˸� �̺�Ʈ ��Ʈ�� ����
	CString NotifyString;// �˸� �̺�Ʈ ��Ʈ��

	// ī�� ����
	BOOL bShow;			// ȭ�鿡 �������°�?
	BOOL bFront;		// �ո��� ���̴� �����ΰ�?
	BOOL bMove;			// �̵����ΰ�?
	BOOL bEvent;		// ī�� �̺�Ʈ�� �ִ°�?
	BOOL bEvent2;
	BOOL bEndMove;		// �̵��� ����ī��
//--------------------

	// ī�� �̵� ���� ����
	int Xp, Yp;			// ī�尡 �׷��� ��ǥ
	int MoveLen;		// �̵��� �Ÿ�
	int SXp, SYp;		// ī�� �̵��� ��� ��ǥ
	int TXp, TYp;		// ī�� �̵��� ��ǥ ��ǥ
	int MoveDelay;		// �̵� ���� ����ġ
	int EndMoveDelay;	// �̵� �Ϸ� ����ġ
	int MoveSpeed;		// �̵� �ӵ�

	int CXp,CYp;        // Ŭ������� ���콺 ����Ʈ

	// ��¦�� ���� ����
	BOOL bTwincle;
	int TwincleCnt;		// ��¦�� ī��Ʈ

	// ������ ����
	BOOL bRedTwincle;
	int RedTwincleCnt;		// ��¦�� ī��Ʈ

	BOOL m_bActive;			//�ִϸ��̼� �������ΰ�...
	DWORD m_dwActiveTime;	//ī�忡�� �׼��� �����ٶ�...
	
	//## �� Ȯ�� ##//
	int m_State;	// �� ���� 0 -> �������� �ʴ�. 1 -> ���� ���� ����Ǿ���. 2-> ���� �ִ�. 3-> Ŀ���� �ִ�. 

	BOOL m_bResvGrowth;
	int	m_growth_delay;

	int	m_xl;
	int	m_subval_x;
	int	m_addval_x;

	int	m_yl;
	int	m_subval_y;
	int	m_addval_y;

	int	m_Max_Growth_X;	//ī�尡 Ŀ���� �ִ���� 
	int	m_Max_Growth_Y;

	void ReserveGrowth( int growth_delay = 0 );
	//////////////////////////////////////////////////////////////////////////
	

	CCard();
	virtual ~CCard();

	void Init(NMBASE::GRAPHICGDI::CPage *ppage, NMBASE::GRAPHICGDI::xSprite *pHwatuNSpr);
	void Clear();
	void SetCard(int cardnum);
	void Reset();
	void OnTimer();
	void SetEvent(int eventcode, int option=0, int option2=0);
	void SetEvent2(int eventcode, int option=0, int option2=0);
	void ClearEvent();
	void ClearEvent2();
	void SetNotifyEvent(char *pstr, COLORREF color=RGB(0,0,0));
	void ClearNotifyEvent();
	void SetMoveStartSnd(char* sndfilename);
	void SetMoveEndSnd(char* sndfilename);
	void SetPos(int tx, int ty);
	void SetMovePos( int mx, int my);	//�����̸鼭 �̵��Ѵ�.
	void SetMove(int tx, int ty, int sdelay=0, int edelay=0, int speed=-1, int front=0, char* startsound = NULL, char* endsound = NULL);
	void CardDraw(BOOL bFold=FALSE, BOOL r_beft=FALSE);

	void SetShow( BOOL r_show );	//ī�� �׷��ֱ� ���� 
	void SetFront( BOOL r_front );	//�޸� ���̱�
	void SetAnimation( int r_kind); //�ִϸ��̼��� �����Ѵ�.
//-------------------------------------------------------------
//-------------------------�߰� �κ�---------------------------
	BOOL bFrontCall;	//ī�� �ո麸�̱� ����
	int m_PNum; //���� ������ ī�� ������ �Ǿ� �ֳ�...
	
	//���̵� ����
	BOOL bMadeDraw;	//���̵� ǥ�ø� ����� �ϳ�...
	int nMadeCnt;	//���̵� ǥ�ø��� �ִ� ���� �����ش�.
	void SetMadeDraw(int cnt = 0);
};

#endif // !defined(AFX_CARD_H__34924AA1_21CA_11D4_97A5_0050BF0FBE67__INCLUDED_)
