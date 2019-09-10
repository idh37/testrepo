// ChatWnd_PlayResultInfo.h: interface for the CChatWnd_PlayResultInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHATWND_PLAYRESULTINFO_H__ECF7DBF8_9780_4CDC_94BF_5EDC9E296235__INCLUDED_)
#define AFX_CHATWND_PLAYRESULTINFO_H__ECF7DBF8_9780_4CDC_94BF_5EDC9E296235__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PLAYRESULTINFO_UP_BTN 420
#define PLAYRESULTINFO_DOWN_BTN 421

typedef struct
{		
	int 			nPNum;		    //�� �ε���	
	int				nServPNum;		//�����ε���
	int				nWinType;		//���� ���п���
	std::string		strID;	
	std::string		strValueName;   //���� ����
	int				nValueKind;		//���� ����
	INT64			llResultMoney;	//���� ��� �Ӵ�	
	int				arrCard[ TOTAL_CARD_BD ];	//ī������

	int				nWinCase;	    //���� �ο�
	float			fGoryRatio;		//������
	int				PremMenuNo;		//ȸ�� ���
	INT64			nGoryMoney;
	bool			bAllIn;			//��������
	
	void Clear()
	{
		nPNum = -1;		    // �� �ε���	
		nServPNum = -1;
		nWinType = -1;		// ���� ���п���
		strID.clear();
		strValueName.clear();	
		nValueKind = 0;
		memset( arrCard, 0, sizeof( arrCard ));

		llResultMoney = 0;	//���� ��� �Ӵ�	
		nWinCase = 0;
		fGoryRatio = 0.000f;
		PremMenuNo = 0;
		nGoryMoney = 0;
		bAllIn = false;
	}

}PLAYRESULTINFO;

typedef struct  
{
	int		nPageIndex;		//�� �ε���
	PLAYRESULTINFO Data[5];

	int     nDrawIndex[5];	//��ο� ����

	void Clear()
	{
		for ( int i = 0 ; i < 5 ; i ++ )
		{
			Data[ i ].Clear();
			nDrawIndex[ i ] = -1;
		}
	}
}PLAYRESULTINFO_DATA;

class CChatWnd_PlayResultInfo  
{
public:
	CChatWnd_PlayResultInfo();
	virtual ~CChatWnd_PlayResultInfo();

	deque<PLAYRESULTINFO_DATA> m_deque_PlayResultInfo;

	BOOL	m_bShow;

	CPoint			m_OffSetpt;				//���� �����丮�� ��ġ�ϴ� ��ǥ��		
	NMBASE::SKINGDI::CGraphButtonEx	m_ScrollUpBtn;			//��ũ�� �� ��ư
	NMBASE::SKINGDI::CGraphButtonEx	m_ScrollDownBtn;		//��ũ�� �ٿ� ��ư
	
	int nCurViewPageNum;		// ���� ������ �ε���
	int nCurTotalPageNum;

	void Init(int x, int y);
	void Clear();
	void InitButton();
	void InPut(PLAYRESULTINFO_DATA data);	
	void Draw(CDC *pdc ,NMBASE::GRAPHICGDI::CPage *page);	
	PLAYRESULTINFO_DATA * GetHistoryData(int index);
	
	void OnLButtonDown(int x, int y);	
	void OnLButtonUp(int x, int y);
	void OnMouseMove(int x, int y);
	void OnScrollButtonUpDown(BOOL bUpDown ); //bUpDown == 0 up bUpDown == 1 down	
	void ReSetWindowPos(int x, int y);

	void GetDrawIndex(int drawindex[] );

	BOOL IsShow(){
		return m_bShow;
	}
	void SetShow(BOOL bshow){
		m_bShow = bshow == TRUE ? 1 : 0;		
	}	

	void EnterRoom();
};

#endif // !defined(AFX_CHATWND_PLAYRESULTINFO_H__ECF7DBF8_9780_4CDC_94BF_5EDC9E296235__INCLUDED_)
