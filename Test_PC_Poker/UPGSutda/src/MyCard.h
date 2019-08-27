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
const int CARD_WIDTH	= 68;
const int CARD_HEIGHT	= 102;

#define DEALINGCARDGAP	7			// 유저당 딜링카드 애니가 총 7개씩 있다.

class CMyCard  
{
public:
	CMyCard();
	virtual ~CMyCard();

private:
	int  m_nCardNum;				// 카드번호(0 ~ 51, 0 ~ 12: 클로버  13 ~ 25:다이아몬드 26~38 :하트  39 ~ 51:스페이드)
	int  m_nCardMonthValue;			// 패의 분류 (1-12월... (13=서비스패))

	CPoint m_ptCardDrawPos;			// 카드가 그려질 좌표

	BOOL m_bCardShow;				// 화면에 보여지는가?     (0:안보임 1:보임)
	BOOL m_bCardFront;				// 앞면이 보이는 상태인가?(0:뒷면   1:앞면)	

	BOOL m_bDark;					// 카드를 어둡게 칠한다 (승리자만 적용)

	BOOL m_bHiddenCard;				// 히든 카드다.
	BOOL m_bShowHiddenCard;			// 히든 카드 보여준다.

	BOOL m_bCardTornOut;			// 찢어진 카드를 보여줄것인가? (0: 정상 1: 찢어진 카드)

	BOOL m_bEvent;					// 카드 이벤트가 있는가?
	BOOL m_bEvent1;					// 카드 이벤트가 있는가?
	BOOL m_bEvent2;					// 카드 이벤트가 있는가?
	
	CMovementEx	m_cMove;			// 카드이동처리(멀티씬 사용)
	CARDEVENT	m_cEvent;
	CARDEVENT	m_cEvent1;
	CARDEVENT	m_cEvent2;

	

public:


	BOOL IsPtInCard(POINT ptPoint);
	void Init(int nCardnum);			// 카드 셋팅
	void Clear();						// 모두 클리어
	void Reset();						// 한판 끝나고 
	void OnTimer();

	void SetCardPos(int tx, int ty);	// 카드 위치

	// 좌표 값 	
	void Draw( NMBASE::GRAPHICGDI::CPage *pPage, BOOL bDie = FALSE, BOOL bFoldOpen = FALSE );

	int  GetCardNum(){return m_nCardNum;}
	void SetCardNum(int nNum);

	// 추가 되어야 할 함수
	CMovementEx& GetMove(void){return m_cMove;}
	const CPoint& GetPos(void){return m_ptCardDrawPos;}

	void SetEvent(int eventcode, int option=0, int option1 = 0, int option2 = 0);
	void SetEvent1(int eventcode, int option=0, int option1 = 0, int option2 = 0);
	void SetEvent2(int eventcode, int option=0, int option1=0, int option2=0);

	void ClearEvent();
	void ClearEvent1();	
	void ClearEvent2();	

	BOOL IsCardShow(){return m_bCardShow;}						
	void SetCardShow(BOOL bFlag){m_bCardShow = bFlag;}

	BOOL IsCardFront(){return m_bCardFront;}					
	void SetCardFront(BOOL bFlag)
	{
		m_bCardFront = bFlag;

	/*	CString ssss;
		if(m_bCardFront)
			ssss = "앞면";
		else
			ssss = "뒷면";

		char szTemp1[256] = {0, };
		sprintf(szTemp1, "------ CMyCard::SetCardFront => 카드번호:%d %s\n", m_nCardNum, ssss.GetBuffer());
		OutputDebugString(szTemp1);*/
	}

	void SetMoveEnd(bool bFlag){m_cMove.SetMoveEnd(bFlag);}

	BOOL IsResultDarkDraw(){return m_bDark;}						// 카드를 어둡게 칠한다 (승리자만 적용)
	void SetResultDarkDraw(BOOL bFlag){m_bDark = bFlag;}				

	BOOL IsHiddenCard(){return m_bHiddenCard;}						// 히든 카드다.
	void SetHiddenCard(BOOL bFlag){m_bHiddenCard = bFlag;}

	BOOL IsShowHiddenCard(){return m_bShowHiddenCard;}				// 히든 카드 보여준다.
	void SetShowHiddenCard(BOOL bFlag){m_bShowHiddenCard = bFlag;}


//섯다용 추가

	//섯다 쪼으기 동작
private:
	BOOL m_bLButtonDown;
	BOOL m_bReadyToJJogi;
	BOOL m_bCompeteJJogi;
	CPoint m_ptJJokgiComplete;	//쪼으기 완료시 위치
	CPoint m_ptJJokgiMouseBackup;		//쪼으기 마우스 시작 위치

	int m_nCardBlinkTime;			//카드 깜빡거림 효과 시간
	BOOL m_bCardBlink;
public:	
	BOOL IsReadyToJJogi(){return m_bReadyToJJogi;}				// 히든 카드 보여준다.
	void SetReadyToJJogi(BOOL bReadyToJJogi) { m_bReadyToJJogi = bReadyToJJogi; }
	void SetReadyToJJogi(BOOL bReadyToJJogi, CPoint ptJJokgiComplete);
	
	void CompleteJJogi(BOOL bNotifyServer, CPoint ptFinalPos);	//쪼으기를 완료 시킨다.
	BOOL IsCompleteJJogi() { return m_bCompeteJJogi; }
	
	BOOL OnMouseMove(int x, int y);
	BOOL OnLButtonDown(int x, int y);
	BOOL OnLButtonUp(int x, int y);

	void MoveCard(CPoint ptEnd, int nIndex, DWORD dwMoveTime, DWORD dwDelay);
	void SetCardTornOut(BOOL bFlag);

	int GetCardMonthValue();

	void SetCardBlinking(int nTime);		//카드 깜빡거리는 효과

	

};

#endif // !defined(AFX_MYCARD_H__DBBCF71C_D638_4ECA_BDE3_A16992451925__INCLUDED_)
