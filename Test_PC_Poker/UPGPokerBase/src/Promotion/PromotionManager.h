#pragma once

#include "../UIInfoData.h"

class CPromotionBase;
class CChattingWindowMan;

class UPG_GAMEBASE_EXPORT CPromotionManager
{
public:
	enum ENUM_PROMOTION_TYPE
	{
	EPT_UNKNOWN = -1,
	EPT_POKERTIME,
	EPT_JOKBOJACKPOT,
	EPT_JACKPOTRELEASE,			//���� �� ����
	EPT_UPPPOLICY,				//UPP��å ����, ���θ���� �ƴ����� ���հ���
	EPT_SPECIAL_JACKPOT,		//����� ���� ���θ��.
	EPT_POKER_COMEBACK,         //2015�� 5�� �Ĺ� ���θ��
	EPT_POKER_LUCKYWHEEL,		//2017�� 5�� ��Ű��
	EPT_POKER_SUPERMOON,		//2017�� 9�� �Ѱ���
	EPT_POKER_REWARD_NEWPOKER,	//2017�� 9�� ����Ŀ ����
	EPT_POKER_NEWYEAR,			//2018�� 2�� ����
	EPT_POKER_2018_CHUSEOK,
	};

protected:
	list<CPromotionBase *> m_listPromotion;
	
	CUIInfoData *m_pUIInfoData;
	CGameProcess *m_pGameProcess;
	CChattingWindowMan *m_pChatMan;
	
	NMBASE::GRAPHICGDI::xSprite m_sprEventPage;
	vector<CRect> m_listPageRect;
	
	//�̺�Ʈ�ǿ� ǥ�õ� ���θ�� ���� ������
	CPromotionBase *m_pCurEventTabPromotion; //���� �������� �ִ� �̺�Ʈ
	int m_nCurEventTabIndex;
	int m_nTotalEventTab;
	CPoint m_ptTabStart;
	CPromotionBase *m_pShowEndPopupPromotion;
	MMRESULT m_hPromotionTimer;

public:
	CPromotionManager();
	virtual ~CPromotionManager();
	
	void Init(CUIInfoData *pUIInfoData, CGame *pGame, CChattingWindowMan *pChatMan);
	void Reset(void);
	
	void SetEvent();
	
	const COLORREF &GetChatColor(ENUM_CHAT_COLOR nType);
	const int GetTotalEventTab(void);
	CPoint GetTabStartPoint(void) { return m_ptTabStart; }
	
	CPromotionBase *GetFind(ENUM_PROMOTION_TYPE nType);
	
	virtual BOOL OnLButtonDownBottom(int x, int y);
	virtual BOOL OnLButtonUpBottom(int x, int y);
	virtual BOOL OnMouseMoveBottom(int x, int y);
	
	virtual BOOL OnLButtonDownMiddle(int x, int y);
	virtual BOOL OnLButtonUpMiddle(int x, int y);
	virtual BOOL OnMouseMoveMiddle(int x, int y);
	
	virtual BOOL OnLButtonDownTop(int x, int y);
	virtual BOOL OnLButtonUpTop(int x, int y);
	virtual BOOL OnMouseMoveTop(int x, int y);
	
	virtual BOOL OnEventTabLButtonDown(int &x, int &y);
	virtual BOOL OnEventTabLButtonUp(int &x, int &y);
	virtual BOOL OnEventTabMouseMove(int &x, int &y);
	
	void ShowPromotion(ENUM_PROMOTION_TYPE nType);
	void RemovePromotion(ENUM_PROMOTION_TYPE nType);
	CPromotionBase *MakePromotion(ENUM_PROMOTION_TYPE nType);
	
	CPromotionBase *GetPromotion(ENUM_PROMOTION_TYPE nType);
	CGameProcess *GetGameProcess(void){return m_pGameProcess;}
	CUIInfoData *GetUIInfoData(void){return m_pUIInfoData;}
	CChattingWindowMan *GetChattingWindowMan(void){return m_pChatMan;}
	
	virtual BOOL OnPacketNotify(char* pName, int Signal, int TotSize, char *lpData);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	
	virtual BOOL StartTimer();
	virtual void KillTimer();
	virtual void OnTimer();	//1�� Ÿ�̸�
	
	virtual void OnStartGame();
	virtual void Accept_OutRoom();
	virtual void Accept_CreateRoom();
	virtual void Accept_EnterRoom();
	virtual void OnEnterRoom();
	virtual void CurPromotionEventTab(bool bShow);
	
	virtual void OnEndGame();
	virtual void DrawEventTab(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage);
	virtual void DrawBottom(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage);
	virtual void DrawMiddle(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage);
	virtual void DrawTop(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage);
	
	virtual bool InitEndPopup(CEndPopup *pEndPopup, CPoint ptPos);

//	virtual void OnDestroy();

	LRESULT OnGetPromotionEndPopupType(WPARAM& wParam, LPARAM& lParam);
	LRESULT OnInitEndPopup(WPARAM& wParam, LPARAM& lParam);
	LRESULT OnPaintEndPopup(WPARAM& wParam, LPARAM& lParam);
	LRESULT OnClickEndPopup(WPARAM& wParam, LPARAM& lParam);
	LRESULT OnDestroyEndPopup(WPARAM& wParam, LPARAM& lParam);

	virtual void SetCurTabPromotion(ENUM_PROMOTION_TYPE nType);
	void SetChatViewPage_ToEventTab();
	void AddChatView(LPCTSTR lpStr, COLORREF crNewColor, bool bUserChat);

	// �������� ���θ���� �������̸� ���� ī��Ʈ ����.
	void SetHalfJackpotCountPlus();
	void HalfJackpotStartEffect();

	void SetWinnerNum(int WinnerNum);

	void Test();

protected:
	virtual CPromotionBase *GetPromotionFromIndex(int nIndex);
	int GetPromotionIndexFromItem(CPromotionBase *pPromotion);
	virtual void CheckPageStatus(void);
	virtual int GetCurrentIndex(void);
};