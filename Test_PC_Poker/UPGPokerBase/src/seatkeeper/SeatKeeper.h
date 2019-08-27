// SeatKeeper.h: interface for the CSeatKeeper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEATKEEPER_H__421A200E_A3BF_4805_A743_B597FDFDA648__INCLUDED_)
#define AFX_SEATKEEPER_H__421A200E_A3BF_4805_A743_B597FDFDA648__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// 사용 방법
//
// 1. LoadImage()를 호출하여 Sprite 이미지를 로드한다.
// 2. Init()를 호출하여 값들을 초기화 한다.
// 3. SetChangeThisPoint()는 게임이 변경될 때마다 호출을 한다. ( OnSetCurrentGame() 함수에서 넣어 준다. )
//
//

//자리 지킴이
#define IDM_BTN_USESEATKEEPER	(WM_USER+6500)
#define IDM_BTN_BACKTOGAME		(WM_USER+6501)




//자리 지킴이
enum IC_STATE_SEATKEEPER
{
	IC_STATE_SEATKEEPER_READY_TO_USE = 0,					// 자리지킴이 3회 남음, 클릭시 사용예약
	IC_STATE_SEATKEEPER_REFUSE_TO_USE_FOR_NOT_PLAYING,		// 자리지킴이 3회 남음, 게임 진쟁중 예약 가능
	IC_STATE_SEATKEEPER_REFUSE_TO_USE_FOR_MULTIPLE_USE,		// 자리지킴이 3회 남음, 게임방내 동시사용 불가
	IC_STATE_SEATKEEPER_REFUSE_TO_USE_FOR_ONE_CANCEL_ONLY,	// 자리지킴이 사용 예약중, 예약 취소 1회 가능
	IC_STATE_SEATKEEPER_READY_TO_CANCEL,					// 자리지킴이 예약중, 클릭시 예약취소(게임방내 1회)
	IC_STATE_SEATKEEPER_OUT_OF_USE,							// 자리지킴이 0회 남음 일 사용 횟수 사용 완료
	IC_STATE_SEATKEEPER_NORIGHT_TO_USE,						// 자리지킴이 미보유, 골드패밀리 이상 사용가능
	IC_STATE_SEATKEEPER_USING,								// 자리지킴이 사용중, 툴팁없음 
	IC_STATE_SEATKEEPER_HIDE,								// 아이콘 숨김
}; 


enum SEATKEEPERBUTTON
{
	SEATKEEPERBUTTON_USE = 0,		// 자리지킴이 사용버튼 ( 버튼메니져에서 관리한다. )
	SEATKEEPERBUTTON_BACKTOGAME,	// 게임 참여버튼
	MAX_SEATKEEPERBUTTON,
};

// 상태
enum ESEATKEEPER_STATE
{
	ESK_NULL = 0,		// 미보유
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


//자리 지킴이 
class UPG_GAMEBASE_EXPORT CSeatKeeper  
{

private:
	//////////////////values/////////////////////

	BOOL m_bInUse;			// 현재 자리지킴이를 사용중인가?
	int m_nReservePNum;		// 현재 자리지킴이를 예약한 유저 
	int m_nTurnCnt;			// 자리 지킴이 사용 이후 몇 턴 째 인가. 
	int m_nLeftCnt;			// 오늘 남은 사용 가능 횟수 
	int m_nMaxCnt;			// 하루 최대 사용 가능 횟수
	int m_nCancelHistory;	// 예약취소 몇번 헀는지? (2번이상 할 수 없다)

	char m_UserID[16];		// 지킴이 사용자 아이디 

	NMBASE::GRAPHICGDI::xSprite m_cSeatKeeperSpr;		// 자리지킴이 스프라이트	
	NMBASE::GRAPHICGDI::xSprite m_cSeatKeeperBtnSpr;	// 자리지킴이 사용버튼	
	
	NMBASE::SKINGDI::CGraphButton m_Button[MAX_SEATKEEPERBUTTON];
	
	NMBASE::UTIL::CNMSSOWebBrowser* m_pBrowser;	// Browser

	CUIInfoData* m_pUIInfoData;		// UI 좌표 관리자
	CGameViewBase* m_pGameView;		// 게임 View
	CChattingWindowMan* m_pChatWin;	// 채팅 윈도우

	CGameProcess* m_pGameProcess;	// 게임프로세스	

	CGameManager* m_pGameManager;	

	std::vector<CPoint>		m_listSeatKeeperWinPos;		//자리지킴이 윈도우 위치값(0, 1, 2, 3, 4)
	ESEATKEEPER_STATE       m_eSeatKeeperState;			//자리지킴이 상태

	bool	m_bShowBtn;
public:
	CSeatKeeper();
	virtual ~CSeatKeeper();

	virtual BOOL IsUsingSeatKeeper() { return m_bInUse; }			//내가 자리지킴이 사용중인가?
	virtual int  GetReserveUser()	 { return m_nReservePNum;	}  //예약사용자 번호 

	virtual BOOL ShouldEnableButton(SEATKEEPERBUTTON Button);

	virtual void LoadImage();	// 이미지를 로드한다.(SPR)

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
	
	virtual int GetSeatKeepUser()		//자리지킴이 사용자 서버 번호 반환 
	{
		if(m_bInUse==FALSE)
			return -1;

		return m_nReservePNum;	
	}

	virtual void RefreshIconButton();
	virtual void SetSeatKeeperIcon(IC_STATE_SEATKEEPER SeatKeeperState);
	virtual IC_STATE_SEATKEEPER GetSeatKeeperState();

	static void OnBeforeNavigate_SeatKeeper(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, LPCTSTR lpszPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel);

	virtual void SetSeatKeeperState( IC_STATE_SEATKEEPER SeatKeeperState, int RemainCnt ); //FamailyIcon에서 함수 가져옴

	// 게임이 바뀔때마다 호출을 해서 this포인터 값을 변경해 준다.
	virtual void SetChangeThisPoint();
private:

	virtual BOOL CreateBrowser();
	virtual void DestroyBrowser();

	virtual void Draw_BigPanel(NMBASE::GRAPHICGDI::CPage *pPage, CDC *pDC);
	virtual void Draw_SmallPanel(int nPlayerPos, NMBASE::GRAPHICGDI::CPage *pPage, CDC *pDC);

	//etc
	virtual char *FindNickName(const char *pID);

	// 채팅창에 출력 메시지
	virtual void AddChatMessage(char* pMsg, COLORREF color);

	// 아래 두함수는 UserStruct.h에서 가져 왔습니다. (두개의 파일이 DLL파일로 export안되는 문제로 Link에러가 발생을 합니다.)
	virtual BOOL AbleToUseSeatKeeper(USERINFO* UI)
	{
		return UPGCheckPremiumState(UI) >= CODE_NEW_FAMILY_GOLD || UI->nPCRoomIPType > 0; //패밀리 골드이상 혹은 피시방만 사용가능하다.
	}
};

#endif // !defined(AFX_SEATKEEPER_H__421A200E_A3BF_4805_A743_B597FDFDA648__INCLUDED_)
