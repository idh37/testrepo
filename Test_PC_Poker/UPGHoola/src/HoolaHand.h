#pragma once
#include "Cards.h"

class CHoolaHand
{	
//Functions
public:
	CHoolaHand(void);
	~CHoolaHand(void);

	// 카드 한장을 자신의 손에 쥔다
	void PushCard(int nCardIndex);
	// 특정 카드들을 뽑아온다.
	CCards PopCards(CCards cCards);
	// 등록 가능한 카드들을 가져온다.
	CCards GetAvailableCards();
	// Hand안의 모든 데이터를 초기화 한다.
	void Reset();


private:
	// 카드 정보를 업데이트 한다.
	void Update(void);
	// 묶음조합을 찾는다.
	DUALBYTEDQ FindStraightCards(BYTEDQ dqCards);
	// 낱장조합을 찾는다,
	DUALBYTEDQ FindTogtherCards(BYTEDQ dqCards);
	// 낱장 붙이기 조합을 찾는다,
	BYTEDQ FindAttachTogtherCards(BYTEDQ dqCards);
	// 묶음 붙이기 조합을 찾는다,
	BYTEDQ FindAttachStraightCards(BYTEDQ dqCards);
	// 7카드를 찾는다,
	BYTEDQ FindSevenCards(BYTEDQ dqCards);
	// 중복되는 카드 조합들이 있는지 검사한다.
	CCards FindDuplicateCards(CCards cCards);

	//해당 큐가 특정값을 가지고 있는가
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
