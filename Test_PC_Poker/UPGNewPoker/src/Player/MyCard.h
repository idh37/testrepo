// MyCard.h: interface for the CMyCard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYCARD_H__D047206A_6B00_4D98_A0DA_D6F76BFAD6FA__INCLUDED_)
#define AFX_MYCARD_H__D047206A_6B00_4D98_A0DA_D6F76BFAD6FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../SevenPoker/MovementEx.h"

class CMyCard  
{
protected:
	CARDEVENT	m_cEvent;
	CARDEVENT	m_cEvent1;
	// ī�� �Ӽ�
	int			m_nCardNo;	// ī���ȣ(0 ~ 51, 0 ~ 12: Ŭ�ι�  13 ~ 25:���̾Ƹ�� 26~38 :��Ʈ  39 ~ 51:�����̵�)
	int			m_nPNum;	// ī���� ������(0-7:�÷��̾�)		
	int			m_nIndex;	// ī�� �ε��� 	

	// ī�� ����
	BOOL		m_bShow;	// ȭ�鿡 �������°�?
	BOOL		m_bFront;	// �ո��� ���̴� �����ΰ�?
	BOOL		m_bMove;	// �̵����ΰ�?	
	// �̺�Ʈ�� ���õ� ����.
	BOOL		m_bEvent;	// ī�� �̺�Ʈ�� �ִ°�?
	BOOL		m_bEvent1;	// ī�� �̺�Ʈ�� �ִ°�?

	// ī�� �̵� ���� ����
	CPoint		m_ptPos;			// ī�尡 �׷��� ��ǥ

	// ��źī�� ����(���� ��ź ī���̴�.
	BOOL		m_bBombCard;	
	BOOL		m_bBombShow;

	BOOL		m_bJokerCard; // ���� ��Ŀ ī���
	int			m_nJokerCard; // ��Ŀ ī�� ��

	// ī�带 ��Ӱ� ĥ�Ѵ� (�¸��ڸ� ����)
	BOOL		m_bDark;	
	BOOL		m_bIsBomb;
	BOOL		m_bMoveStart;
	BOOL		m_bOPen;		// FALSE �޸� ���� , TRUE �ո� 
	BOOL		m_bDirect;	//      ������    ,		      ����
	int			m_nMovelen;
	
	int			m_nDelay;	// �̵��� ������ Ÿ��
	CMovementEx	m_cMove;
public:	
	CMyCard();
	virtual ~CMyCard();

	int  GetCardNum();

	BOOL PtInCard(int x, int y);
	void Init(int cardnum, int pnum,BOOL bbomb = FALSE, int index = 0);	// ī�� ����
	void Clear();			// ��� Ŭ����
	void Reset();			// ���� ������ 
	void OnTimer();
	
	void SetEvent(int eventcode, int option=0, int option1 = 0, int option2 = 0);
	void SetEvent1(int eventcode, int option=0, int option1 = 0, int option2 = 0);
	
	void ClearEvent();
	void ClearEvent1();	

	void			SetPos(CPoint ptPos){m_ptPos = ptPos;}
	const CPoint	&GetPos(void){return m_ptPos;}
	CMovementEx		&GetMove(void){return m_cMove;}

	void			SetCardNo(int nCardNo){m_nCardNo = nCardNo;}
	int				GetCardNo(void){return m_nCardNo;}

	void			SetFront(BOOL bFront){m_bFront = bFront;}
	BOOL			IsFront(void){return m_bFront;}

	void			SetShow(BOOL bShow){m_bShow = bShow;}
	BOOL			IsShow(void){return m_bShow;}

	void			SetDark(BOOL bDark){m_bDark = bDark;}
	BOOL			IsDark(void){return m_bDark;}

	void			SetBombCard(BOOL bBombCard){m_bBombCard = bBombCard;}
	BOOL			IsBombCard(void){return m_bBombCard;}

	void			SetBombShow(BOOL bShow){m_bBombShow = bShow;}
	BOOL			IsBombShow(void){return m_bBombShow;}

	void			SetIsBombCard(BOOL bIsBombCard){m_bIsBomb = bIsBombCard;}
	BOOL			GetIsBombCard(void){return m_bIsBomb;}

	void			SetJokerCard(BOOL bJoker){m_bJokerCard = bJoker;}
	BOOL			IsJokerCard(void){return m_bJokerCard;}

	void			SetJokerCardNo(int nJokerCardNo){m_nJokerCard = nJokerCardNo;}
	BOOL			GetJokerCardNo(void){return m_nJokerCard;}

	BOOL			IsMoveStart(void){return m_bMoveStart;}
	BOOL			IsOpen(void){return m_bOPen;}
	int				GetDelay(void){return m_nDelay;}
	BOOL			IsDirect(void){return m_bDirect;}

	// ��ǥ �� 	
	void Draw( NMBASE::GRAPHICGDI::CPage *pPage, BOOL bFold = FALSE, BOOL bFoldOpen = FALSE );	

	void SetCardFace(BOOL bopen, int delay,BOOL bdirect);		
	void CardMove();

// 
// 	BOOL IsHiddenCard(){return m_bHiddenCard;}						// ���� ī���.
// 	void SetHiddenCard(BOOL bFlag){m_bHiddenCard = bFlag;}
// 
// 	BOOL IsShowHiddenCard(){return m_bShowHiddenCard;}				// ���� ī�� �����ش�.
// 	void SetShowHiddenCard(BOOL bFlag){m_bShowHiddenCard = bFlag;}

};

#endif // !defined(AFX_MYCARD_H__D047206A_6B00_4D98_A0DA_D6F76BFAD6FA__INCLUDED_)
