#if !defined(AFX_QUICKJOINERDLG_H__88E12F72_2FFE_4BDD_938C_D365D994A84E__INCLUDED_)
#define AFX_QUICKJOINERDLG_H__88E12F72_2FFE_4BDD_938C_D365D994A84E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../resource.h"

typedef struct _tagGroupInfo
{
	bool	m_bUsed;			// ����üũ
	int		m_nRadioButPos;		// ������ư ��ġ
	int		m_nComboValue;		// �޺���
}SGroupInfo;

typedef struct 
{
	bool	m_bUseGameMode;			// ���Ӹ�� ��� ����
	int		m_nGameMode;			// ���Ӹ��

	bool	m_bUseBetRule;			// ���÷� ��뿩��
	bool	m_bAutoBet;				// �ڵ�����

	bool	m_bUseSeedMoney;		// ���ӱԸ� ��� ����
	int		m_nSeedType;			// ���ӱԸ�(0: ū��, 1: �Ϲݹ�, 2:������)

	bool	m_bAutoEnterRoom;		// �ڵ����� ����

	short	m_sBetRuleKind;			//����Ʈ�Ӵ�

	int		m_nCardSeting;			//ī�弼�� 4~6��

	float	m_nBetMulti;			//���� ������

} SCONDITION;

// ���� ���� ���� ����
#define QJ_VER		(3)

// Dialog Type  (IDX_GAME �� ������ ����)
enum QUICKJOINDLGTYPE
{
	EQJ_7POKER		= 0,	 
	EQJ_LOWBD		= 1,
	EQJ_NEWPOKER	= 2,
	EQJ_SUTDA		= 3,
	EQJ_HOOLA		= 4,
	//--------------------
	EQJ_MAXDLG	
};

enum PID_QUICKJOIN_ITEM
{
	//-----------------------
	PID_QUICKJOIN_ITEM_GAMEMODE,		// ���Ӹ��
	PID_QUICKJOIN_ITEM_BETRULE,			// ���÷�
	PID_QUICKJOIN_ITEM_SEEDMONEY,		// �õ�Ӵ�	
	PID_QUICKJOIN_ITEM_CONDITION,		// ���ǿ� �´� ���� ���°��
//	PID_QUICKJOIN_ITEM_STOPOPTION,		// ����ɼ� ����(�Ƕ� ����)
	//-----------------------
	PMAX_QUICKJOIN_ITEM,
};

// �⺻���� �����Ǵ� ���� ��ư �ִ� ����
#define MAX_RADIO	3

// �ư��� ���ؼ� ��Ʈ���� X��ǥ�� �ذ��� �ȴ�.

// �����ۺ�  ������ư�� X��ǥ
// { ���̾�α� ����(5�� - 7Poker, LowBD, NewPoker, Sutda, Hoola),
//   �ִ� ������ ����(4�� - ���Ӹ��, ���÷�, �õ�Ӵ�, ���ǿ� �´� ���� ���°��),
//   ������ư ����(3�� - ���׸� X ��ǥ(�ִ� 3��))
// }
//const CPoint ptRadioButton[EQJ_MAXDLG][PMAX_QUICKJOIN_ITEM][MAX_RADIO]=
//{
//	// 7Poker
//	{{CPoint(36, 124),	CPoint(126, 124),	CPoint(216, 124)},	// ���Ӹ��
//	 {CPoint(36, 179),	CPoint(126, 179),	CPoint(216, 179)},	// ���÷� 
//	 {CPoint(36, 234),	CPoint(126, 234),	CPoint(0, 0)},		// �õ�Ӵ�
//	{CPoint(36, 331),	CPoint(36, 351),	CPoint(0, 0)}},		// ���ǿ� �´� ���� ���°�� 
////	{CPoint(0, 0),		CPoint(0, 0),		CPoint(0, 0)}},		// �Ƕ� �۾����� �߰�����(����ɼ�)
//
//    // LowBD
//	{{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},				// ���Ӹ��
//	{CPoint(36, 124),	CPoint(126, 124),	CPoint(216, 124)},	// ���÷� 
//	{CPoint(36, 179),	CPoint(126, 179),	CPoint(0, 0)},		// �õ�Ӵ�
//	{CPoint(36, 276),	CPoint(36, 296),	CPoint(0, 0)}},		// ���ǿ� �´� ���� ���°�� 
////	{CPoint(0, 0),		CPoint(0, 0),		CPoint(0, 0)}},		// �Ƕ� �۾����� �߰�����(����ɼ�)
//
//	// NewPoker
//	{{CPoint(36, 124),	CPoint(126, 124),	CPoint(216, 124)},	// ���Ӹ��
//	 {CPoint(36, 179),	CPoint(126, 179),	CPoint(216, 179)},	// ���÷� 
//	 {CPoint(36, 234),	CPoint(126, 234),	CPoint(0, 0)},		// �õ�Ӵ�
//	 {CPoint(36, 331),	CPoint(36, 351),	CPoint(0, 0)}},		// ���ǿ� �´� ���� ���°��
////	{CPoint(0, 0),		CPoint(0, 0),		CPoint(0, 0)}},		// �Ƕ� �۾����� �߰�����(����ɼ�)
//
//	// Sutda
//	{{CPoint(36, 124),	CPoint(126, 124),	CPoint(216, 124)},	// ���Ӹ��
//	{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},	// ���÷� 
//	{CPoint(36, 179),	CPoint(126, 179),	CPoint(0, 0)},		// �õ�Ӵ�
//	{CPoint(36, 276),	CPoint(36, 296),	CPoint(0, 0)}},		// ���ǿ� �´� ���� ���°�� 
////	{CPoint(0, 0),		CPoint(0, 0),		CPoint(0, 0)}},		// �Ƕ� �۾����� �߰�����(����ɼ�)
//
//	// Hoola
//	{{CPoint(36, 124),	CPoint(126, 124),	CPoint(216, 124)},	// ���Ӹ�� //�� ���� (��������, 2��, 5��)
//	{CPoint(36, 179),	CPoint(126, 179),	CPoint(216, 179)},	// ���÷�	//���Ӹ��(��������, ��Ŀ, �Ϲ�)
//	{CPoint(36, 234),	CPoint(126, 234),	CPoint(0, 0)},		// �õ�Ӵ�	//�õ�Ӵ�(��������, ������)
//	{CPoint(36, 331),	CPoint(36, 351),	CPoint(0, 0)}},		// ���ǿ� �´� ���� ���°�� 
////	{CPoint(125, 204),	CPoint(216, 204),	CPoint(0, 0)}}		// ����ɼ�	//����ɼ�(����on, ����off)
//};

//// �����ۺ�  ������ư�� X��ǥ
//// { ���̾�α� ����(4�� - 7Poker, LowBD, NewPoker, Sutda),
////   �ִ� ������ ����(4�� - ���Ӹ��, ���÷�, �õ�Ӵ�, ���ǿ� �´� ���� ���°��),
////   ������ư ����(3�� - ���׸� X ��ǥ(�ִ� 3��))
//// }
//const CPoint ptComboBox[EQJ_MAXDLG][PMAX_QUICKJOIN_ITEM][MAX_RADIO]=
//{
//	// 7Poker
//	{{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},		// ���Ӹ��
//	{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},		// ���÷� 
//	{CPoint(0, 0),	CPoint(185, 233),	CPoint(0, 0)},	// �õ�Ӵ�
//	{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)}},		// ���ǿ� �´� ���� ���°�� 
//
//	// LowBD
//	{{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},		// ���Ӹ��
//	{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},		// ���÷� 
//	{CPoint(0, 0),	CPoint(185, 177),	CPoint(0, 0)},	// �õ�Ӵ�
//	{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)}},		// ���ǿ� �´� ���� ���°�� 
//
//	// NewPoker
//	{{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},		// ���Ӹ��
//	{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},		// ���÷� 
//	{CPoint(0, 0),	CPoint(185, 233),	CPoint(0, 0)},	// �õ�Ӵ�
//	{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)}},		// ���ǿ� �´� ���� ���°��
//
//	// Sutda
//	{{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},		// ���Ӹ��
//	{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},		// ���÷� 
//	{CPoint(0, 0),	CPoint(185, 177),	CPoint(0, 0)},	// �õ�Ӵ�
//	{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)}},		// ���ǿ� �´� ���� ���°�� 
//
//	// Hoola
//	{{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},		// ���Ӹ��
//	{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},		// ���÷� 
//	{CPoint(0, 0),	CPoint(185, 233),	CPoint(0, 0)},	// �õ�Ӵ�
//	{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)}},		// ���ǿ� �´� ���� ���°�� 
//};

// ���������� ���� �۾�.. enum ���� �׸��� �ȸ¾Ƶ� ������ ��������.
const CPoint ptRadioButton[EQJ_MAXDLG][PMAX_QUICKJOIN_ITEM][MAX_RADIO]=
{
	// 7Poker
	{{CPoint(36, 124),	CPoint(126, 124),	CPoint(216, 124)},	// ���Ӹ��
	{CPoint(-10, 0),		CPoint(-10, 0),	CPoint(-10, 0)},			// ���÷� 
	{CPoint(36, 179),	CPoint(126, 179),	CPoint(216, 179)},	// �õ�Ӵ� // ���ӱԸ� 
	{CPoint(0, 0),		CPoint(0, 0),	CPoint(0, 0)}},			// ���ǿ� �´� ���� ���°�� // ������
	//	{CPoint(0, 0),		CPoint(0, 0),		CPoint(0, 0)}},	// �Ƕ� �۾����� �߰�����(����ɼ�)

	// LowBD
	{{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},				// ���Ӹ��
	{CPoint(36, 124),	CPoint(126, 124),	CPoint(216, 124)},	// ���÷� 
	{CPoint(36, 179),	CPoint(126, 179),	CPoint(0, 0)},		// �õ�Ӵ�
	{CPoint(36, 276),	CPoint(36, 296),	CPoint(0, 0)}},		// ���ǿ� �´� ���� ���°�� 
	//	{CPoint(0, 0),		CPoint(0, 0),		CPoint(0, 0)}},		// �Ƕ� �۾����� �߰�����(����ɼ�)

	// NewPoker
	{{CPoint(36, 124),	CPoint(126, 124),	CPoint(216, 124)},	// ���Ӹ��
	{CPoint(36, 179),	CPoint(126, 179),	CPoint(216, 179)},	// ���÷� 
	{CPoint(36, 234),	CPoint(126, 234),	CPoint(0, 0)},		// �õ�Ӵ�
	{CPoint(36, 331),	CPoint(36, 351),	CPoint(0, 0)}},		// ���ǿ� �´� ���� ���°��
	//	{CPoint(0, 0),		CPoint(0, 0),		CPoint(0, 0)}},		// �Ƕ� �۾����� �߰�����(����ɼ�)

	// Sutda
	{{CPoint(36, 124),	CPoint(126, 124),	CPoint(216, 124)},	// ���Ӹ��
	{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},	// ���÷� 
	{CPoint(36, 179),	CPoint(126, 179),	CPoint(0, 0)},		// �õ�Ӵ�
	{CPoint(36, 276),	CPoint(36, 296),	CPoint(0, 0)}},		// ���ǿ� �´� ���� ���°�� 
	//	{CPoint(0, 0),		CPoint(0, 0),		CPoint(0, 0)}},		// �Ƕ� �۾����� �߰�����(����ɼ�)

	// Hoola
	{{CPoint(36, 124),	CPoint(126, 124),	CPoint(216, 124)},	// ���Ӹ�� //�� ���� (��������, 2��, 5��)
	{CPoint(36, 179),	CPoint(126, 179),	CPoint(216, 179)},	// ���÷�	//���Ӹ��(��������, ��Ŀ, �Ϲ�)
	{CPoint(36, 234),	CPoint(126, 234),	CPoint(0, 0)},		// �õ�Ӵ�	//�õ�Ӵ�(��������, ������)
	{CPoint(36, 331),	CPoint(36, 351),	CPoint(0, 0)}},		// ���ǿ� �´� ���� ���°�� 
	//	{CPoint(125, 204),	CPoint(216, 204),	CPoint(0, 0)}}		// ����ɼ�	//����ɼ�(����on, ����off)
};

// �����ۺ�  ������ư�� X��ǥ
// { ���̾�α� ����(4�� - 7Poker, LowBD, NewPoker, Sutda),
//   �ִ� ������ ����(4�� - ���Ӹ��, ���÷�, �õ�Ӵ�, ���ǿ� �´� ���� ���°��),
//   ������ư ����(3�� - ���׸� X ��ǥ(�ִ� 3��))
// }
const CPoint ptComboBox[EQJ_MAXDLG][PMAX_QUICKJOIN_ITEM][MAX_RADIO]=
{
	//// 7Poker
	//{{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},		// ���Ӹ��
	//{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},	// ���ӱԸ� 
	//{CPoint(0, 0),	CPoint(0, 0),	CPoint(185, 177)},		// �õ�Ӵ�
	//{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)}},		// ���ǿ� �´� ���� ���°�� 

	{{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},		// ���Ӹ��
	{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},		// ���÷� 
	{CPoint(0, 0),	CPoint(185, 233),	CPoint(0, 0)},	// �õ�Ӵ�
	{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)}},		// ���ǿ� �´� ���� ���°�� 

	// LowBD
	{{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},		// ���Ӹ��
	{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},		// ���÷� 
	{CPoint(0, 0),	CPoint(185, 177),	CPoint(0, 0)},	// �õ�Ӵ�
	{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)}},		// ���ǿ� �´� ���� ���°�� 

	// NewPoker
	{{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},		// ���Ӹ��
	{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},		// ���÷� 
	{CPoint(0, 0),	CPoint(185, 233),	CPoint(0, 0)},	// �õ�Ӵ�
	{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)}},		// ���ǿ� �´� ���� ���°��

	// Sutda
	{{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},		// ���Ӹ��
	{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},		// ���÷� 
	{CPoint(0, 0),	CPoint(185, 177),	CPoint(0, 0)},	// �õ�Ӵ�
	{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)}},		// ���ǿ� �´� ���� ���°�� 

	// Hoola
	{{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},		// ���Ӹ��
	{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},		// ���÷� 
	{CPoint(0, 0),	CPoint(185, 233),	CPoint(0, 0)},	// �õ�Ӵ�
	{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)}},		// ���ǿ� �´� ���� ���°�� 
};

const CPoint ptGameScale[EQJ_MAXDLG]=
{
	CPoint(25, 198),	// 7Poker
	CPoint(25, 143),	// LowBD
	CPoint(25, 198),	// NewPoker
	CPoint(25, 143),	// Studa
	CPoint(25, 198)		// Hoola
};

const CRect	rtGameScaleText = CRect(11, 59, 11+253, 59+31);


// ���� ��ư�� ũ��
#define WIDTH_RADIOBUTTON			(77)	// ���� ��ư�� �ʺ�(���� ���̴� 11������ Ŭ���� ���ϵ��� ���� ���̸� ��ư�� ��ġ�� �ʴ� ��ŭ�� ����)
#define HEIGHT_RADIOBUTTON			(11)	// ���� ��ư�� ����

// �޺� �ڽ��� ũ��
#define WIDTH_COMBOBOX				(106)	// �޺� �ڽ� �ʺ�
#define HEIGHT_COMBOBOX				(100)	// �޺� �ڽ� ����


// �ϳ��� ������ �׷쿡 �ִ� �Ʒ��� ���� 3������ Type�� �ִ�.
enum TYPE_CONTROL
{
	TYPE_CONTROL_RADIO			 = 0,	// ���� ��ư
	TYPE_CONTROL_COMBO			 = 1,	// �޺� �ڽ�
};


#define IDC_QUICKJOIN_PRIORITYBUTTON (1000000)
#define IDC_QUICKJOIN_RADIO			 (1001000)
 
// �⺻���� ���� Base
class CQJConditionBase
{
public:
	typedef struct _tagCONTROL
	{
		_tagCONTROL()
		{
			pContol = pChildControl = NULL;				
		}
		CWnd* pContol;		
		CWnd* pChildControl;
		TYPE_CONTROL TypeControl;
	}SCONTROL;

	CWnd* m_pParentWnd;						// �θ� ������
	QUICKJOINDLGTYPE	m_eDialogType;		// ���̾�α� Ÿ��	
	PID_QUICKJOIN_ITEM	m_eQuickJoinItemID;	// �ٷ����� ���� �׸� ID
	int m_nItemNumber;						// ���° �׸��ΰ�?(�����Ӵ�, ��������, ���Ӹ��, ���÷�, �õ�Ӵ�, ����ɼ����뿩��)

	vector<SCONTROL> m_vecControls;			// ��Ʈ�� ����
	
	QJData m_sQuickJoinData;				// Data

public:	
	CQJConditionBase();
	virtual ~CQJConditionBase();

	virtual BOOL Create(QUICKJOINDLGTYPE eQJDlgType, PID_QUICKJOIN_ITEM eQJItemID, CWnd *pParentWnd, int nItemNumber);
	virtual void OnDestory();
	virtual void EnableControl(BOOL bEnable);
	virtual void ShowControl(BOOL bEnable);
	virtual int GetCurSelRadioButton(void){return 0;}
};


class CQJCondition : public CQJConditionBase
{
public:

	NMBASE::SKINGDI::CBmpRadioBtn*   m_pFirstRadioBtn;
	BOOL			m_bPriorityCheck;

	RULETYPE		m_eRuleType;			// ����Ŀ���� ��� 5/7���� �ִ�.	

public:
	CQJCondition();
	virtual ~CQJCondition();

	BOOL DoesExist();

	// ���� ��Ʈ���������ϴ� �Լ�( �ϳ��� ������ �׷쿡 �ش��ϴ� ��Ʈ�Ѹ� ������ �Ѵ�. )
	virtual BOOL Create(QUICKJOINDLGTYPE eQJDlgType,  PID_QUICKJOIN_ITEM eQJItemID, CWnd *pParentWnd, int nItemNumber);

	int GetRadioComboIndex(int &idxCombo);
	virtual int GetCurSelRadioButton(void);

	CComboBox *GetComboBox(int idxCombo);

	void RefreshConditonButton();

	virtual BOOL SaveData(const char *pStrSection);

	virtual BOOL LoadData(const char *pSection, BOOL bUseFlag);

	virtual BOOL _LoadData(const char *pSection);
};

// �ϳ��� �����ۿ� �ش��ϴ� ��Ʈ�� �׷�
class CMyCQJCondition : public CQJCondition
{
public:

	virtual BOOL Create(QUICKJOINDLGTYPE eQJDlgType, PID_QUICKJOIN_ITEM eQJItemID, CWnd *pParentWnd, int nItemNumber, INT64&	i64LImitMoney);	

	// ��Ʈ�ѿ� �̺�Ʈ�� �߻��� �ϸ� ȣ��Ǵ� �Լ�
	BOOL ReloadCondition(IDX_GAME idxGame);	

	void SetCheckOff();	
	
	void SetCombo_AutobetSeedMoney(IDX_GAME idxGame, CComboBox* pComboBox, INT64 i64LImitMoney);
	BOOL SetAutobetSeedMoneyValue( void );	

	PID_QUICKJOIN_ITEM GetQuickjoinItemType(){return m_eItemType;}

	virtual BOOL SaveData(const char *pStrSection);

	virtual BOOL LoadData(const char *pSection, BOOL bUseFlag);

	virtual BOOL _LoadData(const char *pSection);
	
protected:
	CComboBox* m_pComboAutobetSeedMoney;		// ���÷�/�õ�Ӵ� �޺��ڽ�

	IDX_GAME m_eGameType;

	PID_QUICKJOIN_ITEM m_eItemType;				// ������ Ÿ��
};

// ���ӿ� �ش��ϴ� ��� ��Ʈ�� �׷� ����
class CQuickJoinControls 
{	

public:

	QUICKJOINDLGTYPE m_eDialogType;								// ���̾�α� Ÿ��

	CMyCQJCondition m_QuickJoinItemGroup[PMAX_QUICKJOIN_ITEM];	// ��Ʈ�� �׷��� �����Ѵ�.(�����Ӵ�/���Ӹ��/���ӷ�/���÷�/�õ�Ӵ�)

	int				m_nQuickJoinItemCreateCount;				// Item�� ������ ����
	IDX_GAME		m_eSelectGameID;							// ���õ� ���� ID


	INT64			m_i64SelectLImitMoney;						// ���ѸӴ�

public:

	BOOL Create(CWnd *pParentWnd, QUICKJOINDLGTYPE eQJDlgType);

	void SetCheckOff();

	BOOL OnCommand(UINT CmdID);

	void ResetComboBox(IDX_GAME idxGame);

	BOOL SaveData();

	BOOL LoadData(BOOL bUseFlag);

	BOOL EnableControl(BOOL bEnable);

	int GetQjDataSize();

	int GetQjDataBinary(BYTE *pBuffer);

	static CString BinaryToString(const BYTE *pBuffer, int nSize);

	static BOOL SendQJData();

	CMyCQJCondition *GetQJCondition(PID_QUICKJOIN_ITEM eIDQJ);
private:
	void RefreshCondition();
};

/////////////////////////////////////////////////////////////////////////////
// CQuickJoinDlg dialog

class CUIInfoData;

class UPG_GAMEBASE_EXPORT CQuickJoinDlg : public CUPGDlgBase
{
	// Construction
public:
	CQuickJoinDlg(QUICKJOINDLGTYPE eDlgType, CUIInfoData *pUIInfoData, CWnd* pParent = NULL);

	NMBASE::SKINGDI::CMyBitmap m_cBackImg;
	NMBASE::SKINGDI::CMyBitmap m_cGameScaleImage;

	NMBASE::SKINGDI::CHTransRegion m_cRgn;

	IDX_GAME GetSelectedGame();

	static BOOL StringToBinary(CString &str, BYTE *pBuffer, int nSize);
	static bool IsEnableQJ(QUICKJOINDLGTYPE eType);
	static BOOL GetSearchCondition(QUICKJOINDLGTYPE eType, CString StrSection, SCONDITION* pInfo);

	static void SetInfo(QUICKJOINDLGTYPE eType, bool bUseGameMode, int nGameMode, int nRoomType, int nMaxUser, int nComboSel) ;

	//static IDX_GAME GetSelectGame();
	
	// Dialog Data
	//{{AFX_DATA(CQuickJoinDlg)
	enum { IDD = IDD_DIALOG_QUICKJOIN };
	//}}AFX_DATA

	/*--------- ���� ��ư ---------*/

	// �˻���� ���(ON/OFF)(����)
	NMBASE::SKINGDI::CBmpRadioBtn	m_RadioSearchOn;
	NMBASE::SKINGDI::CBmpRadioBtn	m_RadioSearchOff;

	NMBASE::SKINGDI::CBmpRadioBtn	m_rdMode1;	
	NMBASE::SKINGDI::CBmpRadioBtn	m_rdMode2;
	NMBASE::SKINGDI::CBmpRadioBtn	m_rdMode3;
	NMBASE::SKINGDI::CBmpRadioBtn	m_rdMode4;
	NMBASE::SKINGDI::CBmpRadioBtn	m_rdMode5;	
	NMBASE::SKINGDI::CBmpRadioBtn	m_rdMode6;
	NMBASE::SKINGDI::CBmpRadioBtn	m_rdMode7;
	NMBASE::SKINGDI::CBmpRadioBtn	m_rdMode8;

	//�����ȿ� �߰��� ī�弼�� ���� ��ư
	NMBASE::SKINGDI::CBmpRadioBtn	m_rdMode9;
	NMBASE::SKINGDI::CBmpRadioBtn	m_rdMode10;

	CComboBox		m_cbRoomType;	//���ӱԸ� �޺��ڽ�
	CComboBox		m_cbCardSeting;	//������ ī�弼�� �߰� 2014.01

	int				m_nMaxUser ;
	int				m_nRoomType ;
	int				m_nGameMode ;
	int				m_nComboSel ;	//���ӱԸ��� ���ð�

	float			m_nBetMoneyMulti ; //������ �����ؼ� ���� �������� ����� ���ð�
	int				m_nCardSet ; //������ �����ؼ� ī�弼���� ����� ���ð�
	bool			m_bUseCardSeting ;		//������ �����ؼ� ī�弼�� ��뿩��

	bool			m_bUseGameMode;			// ���Ӹ�� ��� ����

	/*--------- ��ư(����) ---------*/

	NMBASE::SKINGDI::CMyButton		m_BtnApply;					// ����
	NMBASE::SKINGDI::CMyButton		m_BtnCancel;				// ���	

	QUICKJOINDLGTYPE m_eDialogType;				// ���̾�α� ����

	CQuickJoinControls m_cQuickJoinControls;	// Quick Join�� �ʿ��� ��Ʈ�� �׷�

	CUIInfoData* m_pUIInfoData;					// UI��ǥ ����

	CPoint m_ptRoomType ;
	
	IDX_GAME m_nGame ;

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQuickJoinDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	static BOOL _LoadData(const char *pSection, int nItemID, SGroupInfo& sGroupInfo);

	static void LoadData(const char *pSection, bool* bUsed, int* nMode, int* nRoomType, int* nMaxUser, int* nCursel, bool* bUseCardOrBetMulti, int* nCardSet ,  float *nBetMoneyMulti );
	BOOL SaveData(const char *pStrSection) ;

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CQuickJoinDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();	
	afx_msg void OnButtonOn();
	afx_msg void OnButtonOff();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	afx_msg void OnButtonApply();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	afx_msg void OnCbnSelchangeComboCardseting();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QUICKJOINERDLG_H__88E12F72_2FFE_4BDD_938C_D365D994A84E__INCLUDED_)
