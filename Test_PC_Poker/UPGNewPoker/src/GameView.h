#if !defined(AFX_GAMEVIEW_H__6C3FEBA4_E463_11D3_97A2_0050BF0FBE67__INCLUDED_)
#define AFX_GAMEVIEW_H__6C3FEBA4_E463_11D3_97A2_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GameView.h : header file
//

#include "GameOverResult.h"

#include "./Dialog/Admin/AdminControlMan.h"

#include "./SprDlg/SprDlgList.h"
#include <../src/ObjectList.h>

/////////////////////////////////////////////////////////////////////////////
// CGameViewNewPoker window

#define RESTART_TIMER 20
#define MYTURN_TIMER 30

// ���� Ĩ ��  (ȭ����ǥ)
#define BETPAGE_X 259 //271//289
#define BETPAGE_Y 226 //239//235

// 2004.06.29 ���۹�ư 
#define IDM_GAMESTART		(WM_USER+7123)

// [�ڵ�����]
#define IDM_AUTO_BTN		(WM_USER+7124)
#define IDM_AUTO_ON			(WM_USER+7125)
#define IDM_AUTO_OFF		(WM_USER+7126)
#define IDM_AUTO_49			(WM_USER+7129)
#define IDM_AUTO_59			(WM_USER+7127)
#define IDM_AUTO_69			(WM_USER+7128)

#define UM_WINDCARD_TIMER	(WM_USER+8001)

#define IDM_GAMEOVER_ANGBUY	(WM_USER+9003) // ��ȣõ�� �����ϱ�
#define IDM_GAMEOVER_ANGUSE	(WM_USER+9004) // ��ȣõ�� ����ϱ�

#define IDM_AUTOBET_CLOSEBTN	(WM_USER+9006) // ���亣�� ���� �ݱ�

#define IDM_BTN_EXCESSMONEY		(WM_USER+9205)

#define STEP_0_TOPBTN				0
#define STEP_1_AUTOBETBTN			1
#define STEP_2_ACTIVESTARTBTN		2

class CCharBox;

class CGameViewNewPoker : public CGameViewBase
{
// Attributes
public:
	BOOL					m_bIsPlay;
	bool					m_bFirstLoad;

	CGameViewButtonMan		m_cGameViewBtnMan;
	CChattingWindowMan		m_cChatWindow;
	CPlayerDrawManager		m_cPlayerDrawManager;
	CTotalBetMoney			m_cTotalBetMoney;
	CGameHelperMan			m_cGameHelper;
	CRoomTitle				m_cTitle;
	CBackGround				m_cBackGround;
	
	// ���콺 ���º���
	BOOL					m_bLClick;

	int						m_nTipCnt;
	int						m_nRoomKindCnt;

	BOOL					m_bSkinKind;

	CGameOverResult			m_cGameOverResult;

	// [ ƫ�� �÷��� ]
	CString					m_strBtnMsgTip; // 1 ���� 2 üũ 3 �� 4 �� 5 ���� 6 �ƽ�
	int						m_nTipX;
	int						m_nTipY;

	CAdminControlMan		m_cAdminCtrlMan;

	CObjectList				m_cAniSprListMan;

	int						m_nChangeRoomInfoMsgKind;
	DWORD					m_dwChangeRoomInfoMsgTime;
	CString					m_strChangeRoomInfoMsg;

	CSprDlgList				m_cSprDlgMan;		//��������Ʈ ���̾�α�
	CDisplayBoard			m_cDisplayBoard;
	CSidePotMoney			m_cSidePotMoney;

	void OnBtnCall();
	void OnBtnCheck();
	void OnBtnFold();
	void OnBtnMax();
	void OnBtnDda();
	void OnBtnPing();

	void SelectChoiceCard(int index,  int discard , BOOL bautoselect = FALSE);
	BOOL OnCardLButtonDown(int xp, int yp);
	BOOL OnCardLButtonUp(int xp, int yp);
	BOOL OnCardMouseMove(int xp, int yp);
	void DrawMyJokboAndJokerEffect(CDC *pDC);

	DWORD GetCurrentPlayTick();
	void DrawJackPot();

	void MakeAVI(CDC *pDC);

//	CBitmap ArrayBmp[999];
//	int		debugcnt;
public:
	CGameViewNewPoker(CGame *pGame);
	virtual ~CGameViewNewPoker();

	void MessagePump();
	void Reset();	

	void DrawPlayBottom(CDC *pDC);
	void DrawPlayMiddle(CDC *pDC);
	void DrawPlayTop(CDC *pDC);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGameViewNewPoker)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	virtual const int GetWidth();
	virtual const int GetHeight();

	virtual void ShowEffect_AllowChancePurchase(int nChanceItemNo);	// ���������� ȹ��� ȿ���� �����ش�.
// Implementation
public:
	void draw_btn_func(CDC &MemDC);
	void draw_card_func(CDC *mdc);

	void Init(void);

	// ��ĵ �ڵ带 �м���
	BOOL IsSacnCode(int nScanCode, int &scankey, int &extended, int &previous);
	virtual BOOL PreTranslateMessage(MSG* pMsg);



// 	void SetBtnState(BOOL bDisable=TRUE);
// 	void ClearFlag();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGameViewNewPoker)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GAMEVIEW_H__6C3FEBA4_E463_11D3_97A2_0050BF0FBE67__INCLUDED_)