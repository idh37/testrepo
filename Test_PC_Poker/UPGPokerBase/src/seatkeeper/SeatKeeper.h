// SeatKeeper.h: interface for the CSeatKeeper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEATKEEPER_H__421A200E_A3BF_4805_A743_B597FDFDA648__INCLUDED_)
#define AFX_SEATKEEPER_H__421A200E_A3BF_4805_A743_B597FDFDA648__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// ��� ���
//
// 1. LoadImage()�� ȣ���Ͽ� Sprite �̹����� �ε��Ѵ�.
// 2. Init()�� ȣ���Ͽ� ������ �ʱ�ȭ �Ѵ�.
// 3. SetChangeThisPoint()�� ������ ����� ������ ȣ���� �Ѵ�. ( OnSetCurrentGame() �Լ����� �־� �ش�. )
//
//

//�ڸ� ��Ŵ��
#define IDM_BTN_USESEATKEEPER	(WM_USER+6500)
#define IDM_BTN_BACKTOGAME		(WM_USER+6501)




//�ڸ� ��Ŵ��
enum IC_STATE_SEATKEEPER
{
	IC_STATE_SEATKEEPER_READY_TO_USE = 0,					// �ڸ���Ŵ�� 3ȸ ����, Ŭ���� ��뿹��
	IC_STATE_SEATKEEPER_REFUSE_TO_USE_FOR_NOT_PLAYING,		// �ڸ���Ŵ�� 3ȸ ����, ���� ������ ���� ����
	IC_STATE_SEATKEEPER_REFUSE_TO_USE_FOR_MULTIPLE_USE,		// �ڸ���Ŵ�� 3ȸ ����, ���ӹ泻 ���û�� �Ұ�
	IC_STATE_SEATKEEPER_REFUSE_TO_USE_FOR_ONE_CANCEL_ONLY,	// �ڸ���Ŵ�� ��� ������, ���� ��� 1ȸ ����
	IC_STATE_SEATKEEPER_READY_TO_CANCEL,					// �ڸ���Ŵ�� ������, Ŭ���� �������(���ӹ泻 1ȸ)
	IC_STATE_SEATKEEPER_OUT_OF_USE,							// �ڸ���Ŵ�� 0ȸ ���� �� ��� Ƚ�� ��� �Ϸ�
	IC_STATE_SEATKEEPER_NORIGHT_TO_USE,						// �ڸ���Ŵ�� �̺���, ����йи� �̻� ��밡��
	IC_STATE_SEATKEEPER_USING,								// �ڸ���Ŵ�� �����, �������� 
	IC_STATE_SEATKEEPER_HIDE,								// ������ ����
}; 


enum SEATKEEPERBUTTON
{
	SEATKEEPERBUTTON_USE = 0,		// �ڸ���Ŵ�� ����ư ( ��ư�޴������� �����Ѵ�. )
	SEATKEEPERBUTTON_BACKTOGAME,	// ���� ������ư
	MAX_SEATKEEPERBUTTON,
};

// ����
enum ESEATKEEPER_STATE
{
	ESK_NULL = 0,		// �̺���
	ESK_OFF,
	ESK_ON,
	ESK_DISABLE,
	ESK_HIDE, 
	MAX_ESK_STATE,
};


class CSV_SEATKEEPER;
class CUIInfoData;
class CGameViewBase;
class CChattingWindowMan;
class CGameProcess;


//�ڸ� ��Ŵ�� 
class UPG_GAMEBASE_EXPORT CSeatKeeper  
{

private:
	//////////////////values/////////////////////

	BOOL m_bInUse;			// ���� �ڸ���Ŵ�̸� ������ΰ�?
	int m_nReservePNum;		// ���� �ڸ���Ŵ�̸� ������ ���� 
	int m_nTurnCnt;			// �ڸ� ��Ŵ�� ��� ���� �� �� ° �ΰ�. 
	int m_nLeftCnt;			// ���� ���� ��� ���� Ƚ�� 
	int m_nMaxCnt;			// �Ϸ� �ִ� ��� ���� Ƚ��
	int m_nCancelHistory;	// ������� ��� ������? (2���̻� �� �� ����)

	char m_UserID[16];		// ��Ŵ�� ����� ���̵� 

	NMBASE::GRAPHICGDI::xSprite m_cSeatKeeperSpr;		// �ڸ���Ŵ�� ��������Ʈ	
	NMBASE::GRAPHICGDI::xSprite m_cSeatKeeperBtnSpr;	// �ڸ���Ŵ�� ����ư	
	
	NMBASE::SKINGDI::CGraphButton m_Button[MAX_SEATKEEPERBUTTON];
	
	NMBASE::UTIL::CNMSSOWebBrowser* m_pBrowser;	// Browser

	CUIInfoData* m_pUIInfoData;		// UI ��ǥ ������
	CGameViewBase* m_pGameView;		// ���� View
	CChattingWindowMan* m_pChatWin;	// ä�� ������

	CGameProcess* m_pGameProcess;	// �������μ���	

	CGameManager* m_pGameManager;	

	std::vector<CPoint>		m_listSeatKeeperWinPos;		//�ڸ���Ŵ�� ������ ��ġ��(0, 1, 2, 3, 4)
	ESEATKEEPER_STATE       m_eSeatKeeperState;			//�ڸ���Ŵ�� ����

	bool	m_bShowBtn;
public:
	CSeatKeeper();
	virtual ~CSeatKeeper();

	virtual BOOL IsUsingSeatKeeper() { return m_bInUse; }			//���� �ڸ���Ŵ�� ������ΰ�?
	virtual int  GetReserveUser()	 { return m_nReservePNum;	}  //�������� ��ȣ 

	virtual BOOL ShouldEnableButton(SEATKEEPERBUTTON Button);

	virtual void LoadImage();	// �̹����� �ε��Ѵ�.(SPR)

	virtual BOOL Init(CUIInfoData *pUIInfoData, CChattingWindowMan* pChatWin);

	virtual void Clear();

	virtual BOOL AskReserve();
	virtual BOOL BackToGame();
	virtual BOOL CancelReserve();

	virtual void ProcessPacket(CSV_SEATKEEPER *pSeatKeeperMsg);
	virtual void Draw(NMBASE::GRAPHICGDI::CPage *pPage, CDC *pDC);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnMouseMove(int x, int y);
	virtual BOOL OnLButtonDown(int x, int y);
	virtual BOOL OnLButtonUp(int x, int y);	
	virtual void OnOutRoom();
	virtual void OnEnterGame();
	virtual void OnStartGame();

	void SetShowSeatKeeperBtn(bool bShow)
	{
		m_bShowBtn = bShow;
	}
	
	virtual int GetSeatKeepUser()		//�ڸ���Ŵ�� ����� ���� ��ȣ ��ȯ 
	{
		if(m_bInUse==FALSE)
			return -1;

		return m_nReservePNum;	
	}

	virtual void RefreshIconButton();
	virtual void SetSeatKeeperIcon(IC_STATE_SEATKEEPER SeatKeeperState);
	virtual IC_STATE_SEATKEEPER GetSeatKeeperState();

	static void OnBeforeNavigate_SeatKeeper(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, LPCTSTR lpszPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel);

	virtual void SetSeatKeeperState( IC_STATE_SEATKEEPER SeatKeeperState, int RemainCnt ); //FamailyIcon���� �Լ� ������

	// ������ �ٲ𶧸��� ȣ���� �ؼ� this������ ���� ������ �ش�.
	virtual void SetChangeThisPoint();
private:

	virtual BOOL CreateBrowser();
	virtual void DestroyBrowser();

	virtual void Draw_BigPanel(NMBASE::GRAPHICGDI::CPage *pPage, CDC *pDC);
	virtual void Draw_SmallPanel(int nPlayerPos, NMBASE::GRAPHICGDI::CPage *pPage, CDC *pDC);

	//etc
	virtual char *FindNickName(const char *pID);

	// ä��â�� ��� �޽���
	virtual void AddChatMessage(char* pMsg, COLORREF color);

	// �Ʒ� ���Լ��� UserStruct.h���� ���� �Խ��ϴ�. (�ΰ��� ������ DLL���Ϸ� export�ȵǴ� ������ Link������ �߻��� �մϴ�.)
	virtual BOOL AbleToUseSeatKeeper(USERINFO* UI)
	{
		return UPGCheckPremiumState(UI) >= CODE_NEW_FAMILY_GOLD || UI->nPCRoomIPType > 0; //�йи� ����̻� Ȥ�� �ǽù游 ��밡���ϴ�.
	}
};

#endif // !defined(AFX_SEATKEEPER_H__421A200E_A3BF_4805_A743_B597FDFDA648__INCLUDED_)
