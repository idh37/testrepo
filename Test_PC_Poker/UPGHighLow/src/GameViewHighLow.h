#pragma once


// 게임에 보이는부분을 담담하는 클래스 

#include "SprPublicMomeyDraw.h"
#include "AniSprListMan.h"
#include "GameButtonHL.h"
#include "GameHelperManHighLow.h"
#include "PlayerDrawManager_HL.h"

class CCardRuleDisplay;
class CSidePotMoney;

class CGameViewHighLow : public CGameViewBase
{
public:
	CChattingWindowMan	m_cChatWindow;				// 채팅 윈도우

	CGameButtonHL  m_cGameViewBtnMananager;	// 게임의 버튼 메니저(게임에 있는 모든 버튼을 관리한다.)

	CAniSprListMan		m_cAniSprListManager;		// 효과 SDlg 등등의 이미지 

	CAniSprListMan		m_cStayAniSprListManager;	// 항상 남아 있어야 하는 것들

	CGameHelperManHighLow		m_cGameHelper;				// 게임 도우미

	CSidePotMoney		m_cSidePotMoney;			// 사이드팟

	CTotalBetMoney		m_cTotalBetMoney;			// 화면 중앙 베팅금액 표시

	CPlayerDrawManager_HL	m_cPlayerDrawManager;		// 플레이어에대한 모든것을 그린다.(카드빼고)

	CDisplayBoard		m_cDisplayBoard;			// 게임 전광판
	CRoomTitle			m_cTitle;
	CBackGround			m_cBackGround;
private:
	bool m_bOnceInit;

public:

	CGameViewHighLow(CGame *pGame);
	virtual ~CGameViewHighLow();

	BOOL NoticAreaMouseOver(int x, int y );
	void SetupByConfig();

	DWORD GetCurrentPlayTick();
	
	void OnTotalMoneyDraw();	//총 베팅 금액 콜금액 드로우

	void Reset();	

	void DrawButton(CDC *pDC);
	void DrawPlayCard(CDC *pDC);		
	void DrawJackPot();

	void DrawPlayBottom(CDC *pDC);
	void DrawPlayMiddle(CDC *pDC);
	void DrawPlayTop(CDC *pDC);

	void OnInit(void);

	//UPG 작업 추가 아래 함수들
	//virtual functions
	void ShowEffect_AllowChancePurchase(int nChanceItemNo);
	void ShowEffect_OwnLimitEffect(int nPnum);					// 보유한도 달성 이펙트
	void ShowEffect_OwnLimitIcon(BOOL bON, INT64 llMoney);		// 보유한도 초과 아이콘

	void OnExitRoom(void);

	const int GetWidth();
	const int GetHeight();

	void GameViewSetFocus();

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGameViewHighLow)
protected:
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	BOOL PreCreateWindow(CREATESTRUCT& cs);
	BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CGameViewHighLow)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

