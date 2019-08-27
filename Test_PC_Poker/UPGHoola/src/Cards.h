#pragma once
typedef std::deque<BYTE> BYTEDQ;
typedef std::deque<typedef std::deque<BYTE>> DUALBYTEDQ;

class CCards
{
public:

	CCards(void);
	~CCards(void);
	
	void ClearCards();
public:
	BYTEDQ m_dqAvailable;

	DUALBYTEDQ m_dqStraight;
	DUALBYTEDQ m_dqTogether;
	BYTEDQ m_dqAttachStr;
	BYTEDQ m_dqAttachTog;
	BYTEDQ m_dqSeven;

//	CCards m_cDuplicateCards;
};