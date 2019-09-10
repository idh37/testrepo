// NMyCard.h: interface for the CNMyCard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NMYCARD_H__DBBCF71C_D638_4ECA_BDE3_A16992451925__INCLUDED_)
#define AFX_NMYCARD_H__DBBCF71C_D638_4ECA_BDE3_A16992451925__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NMovement.h"

class CNMyCard  
{
	
#define  CARD_GAB_Y_SELECT 12
#define  DUMY_CARD_ 52
#define  TOTAL_CARD_BD 4

public:
	CNMyCard();
	virtual ~CNMyCard();

	POINT m_size;		// ��������Ʈ ũ�� (ī�� ũ��)	
	// ī�� �Ӽ�
	
	int  m_nPNum;			// ī���� ������(0-7:�÷��̾�)		
	

	

	// ī�� ����
	bool m_bShow;			// ȭ�鿡 �������°�?
	bool m_bFront;			// �ո��� ���̴� �����ΰ�?	
	bool m_bStay;			// ī�� ������ �̵��� ��� ���� �϶��� �ȱ׸���.

	// ī�� �̵� ���� ����
	int m_Xp, m_Yp;			// ī�尡 �׷��� ��ǥ	

	
	BOOL bFoldDark;	//����� ���� ǥ��
	BOOL bMoveStart;
	BOOL bOPen;		// FALSE �޸� ���� , TRUE �ո� 
	BOOL bDirect;	//      ������    ,		      ����
	int	 nMovelen;	
	int  nDelay;	// �̵��� ������ Ÿ�� 
	
	
	//���� ī�� ����
	int	m_nHinddenArrow;
	
	void Init(int cardnum, int pnum, int index );	// ī�� ����
	bool PtInCard(int x, int y);
	void Clear();			// ��� Ŭ����
	void Reset();			// ���� ������ 
	void OnTimer();
	
	void SetPos(int tx, int ty);
	// ��ǥ �� 	
	void Draw( NMBASE::GRAPHICGDI::CPage *pPage, BOOL bFold = FALSE, BOOL bFoldOpen = FALSE );
	
	void SetCardFace(BOOL bopen, int delay,BOOL bdirect);
	void CardMove();
	void StraightMoveTo( POINT start ,POINT dest, float spd, int delay = 0 ,BOOL bChange = FALSE);

	void SetStayState( bool Stay )
	{
		m_bStay = Stay;
	}

	bool IsCardStay()
	{
		return m_bStay;
	}

	int  GetCardNo() const	
	{ 
		return m_nCardNo; 
	}

	void SetCardNo( int nCardNum )
	{
		m_nCardNo = nCardNum;
	}

	// ��ȿ�� ī��ѹ�
	bool IsVaildCardNo()
	{
		if ( m_nCardNo != DUMY_CARD_ && m_nCardNo != -1 )
		{
			return true;
		}
		
		return false;
	}

	POINT GetMovingCurPos()
	{
		return m_pMovement->GetCurPos();
	}

	void SetMovingCurPos( POINT& ptPos )
	{
		m_pMovement->SetCurPos( ptPos );
	}

	bool IsCardMoving()
	{
		return m_pMovement->IsMoving();
	}

	//����ī��
	void SetHiddenCard( bool bHidden )
	{
		m_bHiddenCard = bHidden;
	}

	bool IsHiddenCard() const
	{
		return m_bHiddenCard;
	}

	//����ī�庸�̱�
	void SetHiddenMovingComplete( bool bShowCard )
	{
		m_bHiddenMovingComplete = bShowCard;
	}

	bool IsHiddenMovingComplete() const 
	{
		return m_bHiddenMovingComplete;
	}

	// ��Ӱ� �׸���
	void SetDarkCard( bool bDark )
	{
		m_bDark = bDark;
	}

	bool IsDarkCard() const
	{
		return m_bDark;
	}

	bool IsSelected() const
	{
		return m_bSelected;
	}

	void SetSelected( bool bSelect )
	{
		m_bSelected = bSelect;
	}

	int GetCardIndex() const
	{
		return m_nIndex;
	}

protected:
	NMBASE::GRAPHICGDI::xSprite *m_pSpr;
	NMBASE::GRAPHICGDI::xSprite *m_pSprDark;

	// �̵��� ���õ� ����.
	CNMovement* m_pMovement;
	int  m_nCardNo;					// ī���ȣ(0 ~ 51, 0 ~ 12: Ŭ�ι�  13 ~ 25:���̾Ƹ�� 26~38 :��Ʈ  39 ~ 51:�����̵�)

	bool m_bHiddenCard;				// ���� ī�� ��.
	bool m_bHiddenMovingComplete;	// ���� ī�� �����ش�.
	bool m_bDark;					// ī�带 ��Ӱ� �׸���. (�¸��ڸ� ����)
	bool m_bSelected;				// �ٲٱ� ���� �÷���
	int  m_nIndex;					// ī�� ī��Ʈ �ε���


};

#endif // !defined(AFX_NMYCARD_H__DBBCF71C_D638_4ECA_BDE3_A16992451925__INCLUDED_)
