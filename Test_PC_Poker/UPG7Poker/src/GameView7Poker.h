#if !defined(AFX_GAMEVIEW_H__6C3FEBA4_E463_11D3_97A2_0050BF0FBE67__INCLUDED_)
#define AFX_GAMEVIEW_H__6C3FEBA4_E463_11D3_97A2_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ���ӿ� ���̴ºκ��� ����ϴ� Ŭ���� 

#include "SprPublicMomeyDraw.h"
#include "AniSprListMan.h"

class CCardRuleDisplay;
class CSidePotMoney;

class CGameView7Poker : public CGameViewBase
{
public:
	CChattingWindowMan	m_cChatWindow;				// ä�� ������

	CGameViewButtonMan  m_cGameViewBtnMananager;	// ������ ��ư �޴���(���ӿ� �ִ� ��� ��ư�� �����Ѵ�.)

	CAniSprListMan		m_cAniSprListManager;		// ȿ�� SDlg ����� �̹��� 

	CAniSprListMan		m_cStayAniSprListManager;	// �׻� ���� �־�� �ϴ� �͵�

	CGameHelperMan		m_cGameHelper;				// ���� �����

	CSidePotMoney		m_cSidePotMoney;			// ���̵���

	CTotalBetMoney		m_cTotalBetMoney;			// ȭ�� �߾� ���ñݾ� ǥ��

	CPlayerDrawManager	m_cPlayerDrawManager;		// �÷��̾���� ������ �׸���.(ī�廩��)

	CDisplayBoard		m_cDisplayBoard;			// ���� ������
	CRoomTitle			m_cTitle;
	CBackGround			m_cBackGround;

private:
	bool m_bOnceInit;

public:

	CGameView7Poker(CGame *pGame);
	virtual ~CGameView7Poker();

	BOOL NoticAreaMouseOver(int x, int y );
	void SetupByConfig();

	DWORD GetCurrentPlayTick();
	
	void OnTotalMoneyDraw();	//�� ���� �ݾ� �ݱݾ� ��ο�

	void Reset();	

	void DrawButton(CDC *pDC);
	void DrawPlayCard(CDC *pDC);		
	void DrawJackPot();

	void DrawPlayBottom(CDC *pDC);
	void DrawPlayMiddle(CDC *pDC);
	void DrawPlayTop(CDC *pDC);

	void OnInit(void);

	//UPG �۾� �߰� �Ʒ� �Լ���
	//virtual functions
	void ShowEffect_AllowChancePurchase(int nChanceItemNo);
	void ShowEffect_OwnLimitEffect(int nPnum);					// �����ѵ� �޼� ����Ʈ
	void ShowEffect_OwnLimitIcon(BOOL bON, INT64 llMoney);		// �����ѵ� �ʰ� ������

	void OnExitRoom(void);

	const int GetWidth();
	const int GetHeight();

	void GameViewSetFocus();

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGameView7Poker)
protected:
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	BOOL PreCreateWindow(CREATESTRUCT& cs);
	BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CGameView7Poker)
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

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GAMEVIEW_H__6C3FEBA4_E463_11D3_97A2_0050BF0FBE67__INCLUDED_)
