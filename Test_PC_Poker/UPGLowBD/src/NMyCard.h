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

	POINT m_size;		// 스프라이트 크기 (카드 크기)	
	// 카드 속성
	
	int  m_nPNum;			// 카드의 소유자(0-7:플레이어)		
	

	

	// 카드 상태
	bool m_bShow;			// 화면에 보여지는가?
	bool m_bFront;			// 앞면이 보이는 상태인가?	
	bool m_bStay;			// 카드 덱으로 이동후 대기 상태 일때는 안그린다.

	// 카드 이동 관련 변수
	int m_Xp, m_Yp;			// 카드가 그려질 좌표	

	
	BOOL bFoldDark;	//포기시 다이 표시
	BOOL bMoveStart;
	BOOL bOPen;		// FALSE 뒷면 상태 , TRUE 앞면 
	BOOL bDirect;	//      오른쪽    ,		      왼쪽
	int	 nMovelen;	
	int  nDelay;	// 이동시 딜레이 타임 
	
	
	//히든 카드 관련
	int	m_nHinddenArrow;
	
	void Init(int cardnum, int pnum, int index );	// 카드 셋팅
	bool PtInCard(int x, int y);
	void Clear();			// 모두 클리어
	void Reset();			// 한판 끝나고 
	void OnTimer();
	
	void SetPos(int tx, int ty);
	// 좌표 값 	
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

	// 유효한 카드넘버
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

	//히든카드
	void SetHiddenCard( bool bHidden )
	{
		m_bHiddenCard = bHidden;
	}

	bool IsHiddenCard() const
	{
		return m_bHiddenCard;
	}

	//히든카드보이기
	void SetHiddenMovingComplete( bool bShowCard )
	{
		m_bHiddenMovingComplete = bShowCard;
	}

	bool IsHiddenMovingComplete() const 
	{
		return m_bHiddenMovingComplete;
	}

	// 어둡게 그리기
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

	// 이동에 관련된 변수.
	CNMovement* m_pMovement;
	int  m_nCardNo;					// 카드번호(0 ~ 51, 0 ~ 12: 클로버  13 ~ 25:다이아몬드 26~38 :하트  39 ~ 51:스페이드)

	bool m_bHiddenCard;				// 히든 카드 다.
	bool m_bHiddenMovingComplete;	// 히든 카드 보여준다.
	bool m_bDark;					// 카드를 어둡게 그린다. (승리자만 적용)
	bool m_bSelected;				// 바꾸기 선택 플래그
	int  m_nIndex;					// 카드 카운트 인덱스


};

#endif // !defined(AFX_NMYCARD_H__DBBCF71C_D638_4ECA_BDE3_A16992451925__INCLUDED_)
