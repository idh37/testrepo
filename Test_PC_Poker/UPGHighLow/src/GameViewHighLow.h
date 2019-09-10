#pragma once


// ���ӿ� ���̴ºκ��� ����ϴ� Ŭ���� 

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
	CChattingWindowMan	m_cChatWindow;				// ä�� ������

	CGameButtonHL  m_cGameViewBtnMananager;	// ������ ��ư �޴���(���ӿ� �ִ� ��� ��ư�� �����Ѵ�.)

	CAniSprListMan		m_cAniSprListManager;		// ȿ�� SDlg ����� �̹��� 

	CAniSprListMan		m_cStayAniSprListManager;	// �׻� ���� �־�� �ϴ� �͵�

	CGameHelperManHighLow		m_cGameHelper;				// ���� �����

	CSidePotMoney		m_cSidePotMoney;			// ���̵���

	CTotalBetMoney		m_cTotalBetMoney;			// ȭ�� �߾� ���ñݾ� ǥ��

	CPlayerDrawManager_HL	m_cPlayerDrawManager;		// �÷��̾���� ������ �׸���.(ī�廩��)

	CDisplayBoard		m_cDisplayBoard;			// ���� ������
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

