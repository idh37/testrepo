#if !defined(AFX_GAMEVIEW_H__6C3FEBA4_E463_11D3_97A2_0050BF0FBE67__INCLUDED_)
#define AFX_GAMEVIEW_H__6C3FEBA4_E463_11D3_97A2_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GameView.h : header file
//

// ����ȭ
#include "AniSprListMan.h"
#include "./Module/Draw/PlayerDraw_BD.h"
#include "./Module/Draw/PlayerDrawManager_BD.h"
#include "Button/GameButtonBD.h"
#include "BackGround/BackGround_BD.h"
#include "SlideText.h"

/////////////////////////////////////////////////////////////////////////////
// CGameViewLowBD window

#define IDM_CHANGE_BTN		(WM_USER+9007) // ���� �ٲٱ�
#define IDM_PASS_BTN		(WM_USER+9008) // ���� �н�
#define IDM_GAME_START		(WM_USER+9009) // ���� ���� 
#define IDM_GAME_AUTOUP		(WM_USER+9010) // �ڵ� �ø� 
#define IDM_GAME_AUTOSORT	(WM_USER+9011) // �ڵ� ����:2012.09.27 YS.Jeong 


class CCardRuleDisplay;
class CSidePotMoney;

class CGameViewLowBD : public CGameViewBase
{
// Attributes
public:
	bool				m_bFirstLoad;

		
	CGameButtonBD		  m_cGameViewBtnMan;
	CChattingWindowMan	  m_cChatWindow;				// ä�� ������
	CPlayerDrawManager_BD m_cPlayerDrawManager;
	CTotalBetMoney		  m_cTotalBetMoney;
	CBackGround_BD		  m_cBackGround;

	CSidePotMoney		m_cSidePotMoney;			// ���̵���
	CDisplayBoard		m_cDisplayBoard;;			// ���� ���
	CGameHelperMan		m_cGameHelper;

	CSlideText			m_cSlideText;

	// ���콺 ���º���
	BOOL bLClick;
	
			
	CRoomTitle			m_cTitle;
	
	CAniSprListMan		m_cAniSprListManager;		//ȿ�� SDlg ����� �̹��� 
	CAniSprListMan		m_cStayAniSprListManager;	//�׻� ���� �־�� �ϴ� �͵�
public:

	// ### [ Frame++ ] ###
	//void MessagePump();			//UPGCommon �۾� ����

	CGameViewLowBD( CGame *pGame );
	virtual ~CGameViewLowBD();

	void Reset();	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGameViewLowBD)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	void Init(void);
	void draw_btn_func(CDC *pDC);
	void DrawJackPot();
	DWORD GetCurrentPlayTick();
		
	//UPG �۾� �߰� �Ʒ� �Լ���
	//virtual functions
	virtual void ShowEffect_AllowChancePurchase(int nChanceItemNo);
	virtual void OnExitRoom(void);

	virtual const int GetWidth();
	virtual const int GetHeight();
	
	virtual BOOL PreTranslateMessage(MSG* pMsg);


	// Generated message map functions
protected:
	//{{AFX_MSG(CGameViewLowBD)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
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
