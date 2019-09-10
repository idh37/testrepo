#pragma once
#include "Cards.h"

class CHoolaHand
{	
//Functions
public:
	CHoolaHand(void);
	~CHoolaHand(void);

	// ī�� ������ �ڽ��� �տ� ���
	void PushCard(int nCardIndex);
	// Ư�� ī����� �̾ƿ´�.
	CCards PopCards(CCards cCards);
	// ��� ������ ī����� �����´�.
	CCards GetAvailableCards();
	// Hand���� ��� �����͸� �ʱ�ȭ �Ѵ�.
	void Reset();


private:
	// ī�� ������ ������Ʈ �Ѵ�.
	void Update(void);
	// ���������� ã�´�.
	DUALBYTEDQ FindStraightCards(BYTEDQ dqCards);
	// ���������� ã�´�,
	DUALBYTEDQ FindTogtherCards(BYTEDQ dqCards);
	// ���� ���̱� ������ ã�´�,
	BYTEDQ FindAttachTogtherCards(BYTEDQ dqCards);
	// ���� ���̱� ������ ã�´�,
	BYTEDQ FindAttachStraightCards(BYTEDQ dqCards);
	// 7ī�带 ã�´�,
	BYTEDQ FindSevenCards(BYTEDQ dqCards);
	// �ߺ��Ǵ� ī�� ���յ��� �ִ��� �˻��Ѵ�.
	CCards FindDuplicateCards(CCards cCards);

	//�ش� ť�� Ư������ ������ �ִ°�
	BOOL IsDqHasTheCardIndex(BYTEDQ dq, BYTE cardIndex);

	DUALBYTEDQ IsSerialJoker( int *pData, int nMaxIndex, int nJoker, int nCardIndex );
	DUALBYTEDQ IsSerialJoker( BYTE *pData, int nMaxIndex, int nJoker1, int nJoker2, int nCardNum );
	DUALBYTEDQ GetStraightSet( BYTE *pData, int nMaxIndex );
//Variables
private:
	BOOL m_bClickedCardsHands;
	BOOL m_bConsiderJoker;
	CCards m_cCards;	
	BYTEDQ m_dqCards;
};
