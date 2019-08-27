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
	// 카드 속성
	int			m_nCardNo;	// 카드번호(0 ~ 51, 0 ~ 12: 클로버  13 ~ 25:다이아몬드 26~38 :하트  39 ~ 51:스페이드)
	int			m_nPNum;	// 카드의 소유자(0-7:플레이어)		
	int			m_nIndex;	// 카드 인덱스 	

	// 카드 상태
	BOOL		m_bShow;	// 화면에 보여지는가?
	BOOL		m_bFront;	// 앞면이 보이는 상태인가?
	BOOL		m_bMove;	// 이동중인가?	
	// 이벤트에 관련된 변수.
	BOOL		m_bEvent;	// 카드 이벤트가 있는가?
	BOOL		m_bEvent1;	// 카드 이벤트가 있는가?

	// 카드 이동 관련 변수
	CPoint		m_ptPos;			// 카드가 그려질 좌표

	// 폭탄카드 변수(내가 폭탄 카드이다.
	BOOL		m_bBombCard;	
	BOOL		m_bBombShow;

	BOOL		m_bJokerCard; // 내가 조커 카드다
	int			m_nJokerCard; // 조커 카드 값

	// 카드를 어둡게 칠한다 (승리자만 적용)
	BOOL		m_bDark;	
	BOOL		m_bIsBomb;
	BOOL		m_bMoveStart;
	BOOL		m_bOPen;		// FALSE 뒷면 상태 , TRUE 앞면 
	BOOL		m_bDirect;	//      오른쪽    ,		      왼쪽
	int			m_nMovelen;
	
	int			m_nDelay;	// 이동시 딜레이 타임
	CMovementEx	m_cMove;
public:	
	CMyCard();
	virtual ~CMyCard();

	int  GetCardNum();

	BOOL PtInCard(int x, int y);
	void Init(int cardnum, int pnum,BOOL bbomb = FALSE, int index = 0);	// 카드 셋팅
	void Clear();			// 모두 클리어
	void Reset();			// 한판 끝나고 
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

	// 좌표 값 	
	void Draw( NMBASE::GRAPHICGDI::CPage *pPage, BOOL bFold = FALSE, BOOL bFoldOpen = FALSE );	

	void SetCardFace(BOOL bopen, int delay,BOOL bdirect);		
	void CardMove();

// 
// 	BOOL IsHiddenCard(){return m_bHiddenCard;}						// 히든 카드다.
// 	void SetHiddenCard(BOOL bFlag){m_bHiddenCard = bFlag;}
// 
// 	BOOL IsShowHiddenCard(){return m_bShowHiddenCard;}				// 히든 카드 보여준다.
// 	void SetShowHiddenCard(BOOL bFlag){m_bShowHiddenCard = bFlag;}

};

#endif // !defined(AFX_MYCARD_H__D047206A_6B00_4D98_A0DA_D6F76BFAD6FA__INCLUDED_)
