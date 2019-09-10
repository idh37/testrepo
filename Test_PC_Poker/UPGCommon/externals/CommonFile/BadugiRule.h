// BadugiRule.h: interface for the CBadugiRule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BADUGIRULE_H__DD90F0E2_A1F2_4E4D_B309_F8366B450838__INCLUDED_)
#define AFX_BADUGIRULE_H__DD90F0E2_A1F2_4E4D_B309_F8366B450838__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum {
		BD_EMPTY_VALUE= -1,
		BD_EMPTY_CARD = -1,
		BD_TOTAL_CARD = 5,
		MAX_PATTERN = 4,		
		BD_TOTAL_USER = 5,
	};

enum {
	GOLF = 46,
	SECOND = 45,
	THIRD =	44,
};

#define SEED_NUMBERPOINT 14

typedef struct {
	int pattern;
	int cardnumber;	
}MyCard;

typedef struct {	
	int cardnumber[BD_TOTAL_CARD];	//���� ����
	int numberpoint[BD_TOTAL_CARD];	//���� ����
	int curcardnum;		//���� ���� ���Ͽ� ���ڰ� � �ִ�.
	int pattern;
}CardInfo;

typedef struct  {
	int Card[5];		// 1~13����
	int RealCard[5];	// 0~51����	
	int numberpoint;
	int m_RuleCompleteCnt;
}CompleteCard;


class CBadugiRule  
{
public:
	CBadugiRule();
	virtual ~CBadugiRule();

	enum BADUGI_CARD {
			B4C_TOP=0,
			B4C_GOLF,
			B4C_SECOND,
			B4C_THIRD,
			B4C_MADE,
			B4C_BASE,
			B4C_TWO_BASE,
	};

	int m_Card[BD_TOTAL_CARD];			//ó�� ���õ� ī�� ��
	int m_RealSortCard[BD_TOTAL_CARD];	//���ĸ� �� ī�嵦	
	CompleteCard m_RuleCompleteCard;	//������ �̷� ī�� 
	CardInfo m_sortCard[MAX_PATTERN];

	int PNum;							//�ѹ�
	int m_nTotalCard;					// �� ī���
	int	m_MyValue;						// ���� �� ���� 0 ž 1 ���� 2 ���� 3 ���� 4 ���̵� 5 ���̽� 6 �����̽� 
	int m_nGameMode;					//0 �̸� 4�� 1�̸� 5��	
	BOOL m_bFold;						//FALSE ������ TRUE ���
	CString m_strMyValueName;

	void Init(int mode=0);
	void Clear();
	
	void SetCard(int pnum, int pCard[],int CardNum);			//������ ���ϱ� ���� ī�� ����
	void sort_Card(int Start, int End);							// ī������	
	void GetMyRule();					//���� ���ϱ�
	
	CString GetNumString(int num = -1);	
	CString GetValueName();			//�� ���� �̸� ���ϱ�
	CString GetValueNumString();	//������ �̷� ���ڵ� �� ����
	CString GetFullValueName();		//GetValueNumString() + GetValueName() ����

	BOOL Made_4();						//���̵� ���ϱ� 
	BOOL Made_5();						//���̵� ���ϱ� 
	BOOL Base();						//���̽� ���ϱ�
	BOOL Base_5();						//���̽� ���ϱ�
	BOOL TwoBase();						//�����̽� ���ϱ�	
	void Top();							//ž ���ϱ�

	void Result_Sort();

	int m_MyRanking;								//���� ���
	void SetRanking(int rank){m_MyRanking =rank;}	//��� ����
	int  GetRanking(){return m_MyRanking;}			//��� ����	

	void SetFold(){m_bFold = TRUE;};
	void SetGameMode(int mode){m_nGameMode = mode;};
	int	 GetMyValueTopNum();
};

#endif // !defined(AFX_BADUGIRULE_H__DD90F0E2_A1F2_4E4D_B309_F8366B450838__INCLUDED_)
