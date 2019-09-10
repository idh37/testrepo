// MyCard.h: interface for the CMyCard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYCARD_H__DBBCF71C_D638_4ECA_BDE3_A16992451925__INCLUDED_)
#define AFX_MYCARD_H__DBBCF71C_D638_4ECA_BDE3_A16992451925__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MovementEx.h"

const int SHADOW_SIZE	= 1;
const int CARD_WIDTH	= 80;
const int CARD_HEIGHT	= 117;

#define DEALINGCARDGAP	7			// ������ ����ī�� �ִϰ� �� 7���� �ִ�.

class CMyCard  
{
public:
	CMyCard();
	virtual ~CMyCard();

private:
	int  m_nCardNum;				// ī���ȣ(0 ~ 51, 0 ~ 12: Ŭ�ι�  13 ~ 25:���̾Ƹ�� 26~38 :��Ʈ  39 ~ 51:�����̵�)

	CPoint m_ptCardDrawPos;			// ī�尡 �׷��� ��ǥ

	BOOL m_bCardShow;				// ȭ�鿡 �������°�?     (0:�Ⱥ��� 1:����)
	BOOL m_bCardFront;				// �ո��� ���̴� �����ΰ�?(0:�޸�   1:�ո�)	

	BOOL m_bDark;					// ī�带 ��Ӱ� ĥ�Ѵ� (�¸��ڸ� ����)

	BOOL m_bHiddenCard;				// ���� ī���.
	BOOL m_bShowHiddenCard;			// ���� ī�� �����ش�.

	BOOL m_bEvent;					// ī�� �̺�Ʈ�� �ִ°�?
	BOOL m_bEvent1;					// ī�� �̺�Ʈ�� �ִ°�?
	
	CMovementEx	m_cMove;			// ī���̵�ó��(��Ƽ�� ���)
	CARDEVENT	m_cEvent;
	CARDEVENT	m_cEvent1;

public:

	BOOL IsPtInCard(POINT ptPoint);
	void Init(int nCardnum);			// ī�� ����
	void Clear();						// ��� Ŭ����
	void Reset();						// ���� ������ 
	void OnTimer();

	void SetCardPos(int tx, int ty);	// ī�� ��ġ

	// ��ǥ �� 	
	void Draw(NMBASE::GRAPHICGDI::CPage *pPage,BOOL bDie = FALSE, BOOL bFoldOpen = FALSE);

	int  GetCardNum(){return m_nCardNum;}
	void SetCardNum(int nNum){m_nCardNum = nNum;}

	// �߰� �Ǿ�� �� �Լ�
	CMovementEx& GetMove(void){return m_cMove;}
	const CPoint& GetPos(void){return m_ptCardDrawPos;}

	void SetEvent(int eventcode, int option=0, int option1 = 0, int option2 = 0);
	void SetEvent1(int eventcode, int option=0, int option1 = 0, int option2 = 0);

	void ClearEvent();
	void ClearEvent1();	

	BOOL IsCardShow(){return m_bCardShow;}						
	void SetCardShow(BOOL bFlag){m_bCardShow = bFlag;}

	BOOL IsCardFront(){return m_bCardFront;}					
	void SetCardFront(BOOL bFlag)
	{
		m_bCardFront = bFlag;

	/*	CString ssss;
		if(m_bCardFront)
			ssss = "�ո�";
		else
			ssss = "�޸�";

		char szTemp1[256] = {0, };
		sprintf(szTemp1, "------ CMyCard::SetCardFront => ī���ȣ:%d %s\n", m_nCardNum, ssss.GetBuffer());
		OutputDebugString(szTemp1);*/
	}

	void SetMoveEnd(bool bFlag){m_cMove.SetMoveEnd(bFlag);}

	BOOL IsResultDarkDraw(){return m_bDark;}						// ī�带 ��Ӱ� ĥ�Ѵ� (�¸��ڸ� ����)
	void SetResultDarkDraw(BOOL bFlag){m_bDark = bFlag;}				

	BOOL IsHiddenCard(){return m_bHiddenCard;}						// ���� ī���.
	void SetHiddenCard(BOOL bFlag){m_bHiddenCard = bFlag;}

	BOOL IsShowHiddenCard(){return m_bShowHiddenCard;}				// ���� ī�� �����ش�.
	void SetShowHiddenCard(BOOL bFlag){m_bShowHiddenCard = bFlag;}

};

#endif // !defined(AFX_MYCARD_H__DBBCF71C_D638_4ECA_BDE3_A16992451925__INCLUDED_)
