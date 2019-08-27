#pragma once

#include "PromotionBase.h"
#include "PromotionManager.h"
#include "../ChattingWindowMan/ChattingWindowMan.h"
#include "../UIInfoData.h"

#define UM_LB_BTN01 (WM_USER + 201801)
#define UM_LB_BTN02 (WM_USER + 201802)
#define UM_LB_BTN03 (WM_USER + 201803)
#define UM_LB_BTN04 (WM_USER + 201804)
#define UM_LB_BTN05 (WM_USER + 201805)

class CChattingWindowMan;

class UPG_GAMEBASE_EXPORT CPromotionNewYearLuckyBag : public CPromotionBase
{
protected:
	// 배경 그림
	NMBASE::GRAPHICGDI::xSprite m_sprBack;
	CEffectBase m_cEffect_back;
	NMBASE::GRAPHICGDI::xSprite m_sprChicken;
	CEffectBase m_cEffect_chicken;
	
	// 응모하기 버튼
	NMBASE::GRAPHICGDI::xSprite m_sprBtn;
	NMBASE::SKINGDI::CGraphButton m_btn01;
	NMBASE::SKINGDI::CGraphButton m_btn02;
	NMBASE::SKINGDI::CGraphButton m_btn03;
	NMBASE::SKINGDI::CGraphButton m_btn04;
	
	// 응모하기 버튼 상태 - 0: none, 1: over, 2: click
	int m_nBtnState01;
	int m_nBtnState02;
	int m_nBtnState03;
	int m_nBtnState04;
	
	// 응모 횟수 텍스트
	NMBASE::SKINGDI::CNText *m_pNum01;
	NMBASE::SKINGDI::CNText *m_pNum02;
	NMBASE::SKINGDI::CNText *m_pNum03;
	NMBASE::SKINGDI::CNText *m_pNum04;
	NMBASE::SKINGDI::CNText *m_pNum05;
	
	// 응모 복주머니 개수 텍스트
	NMBASE::SKINGDI::CNText *m_pBagNum01;
	NMBASE::SKINGDI::CNText *m_pBagNum02;
	NMBASE::SKINGDI::CNText *m_pBagNum03;
	NMBASE::SKINGDI::CNText *m_pBagNum04;
	
	// 배너 mouse on 여부
	bool m_isOnBtn_bag;
	bool m_isOnBtn_goldBag;
	bool m_isOnBtn_btn;	// 응모하기 버튼
	bool m_isOnBtn_chicken;
	
	DWORD m_dwRewardStartTime;
	DWORD m_dwRewardStartTime_gold;
	
	//// 서버에서 받아오는 데이터
	// 복주머니 유저 인포
	int m_CurrentGameCount; //현재 게임진행 판수. 보상이 지급되면 0으로 초기화 된다.
	//int m_TodayRewardCount; //오늘 지급받은 보상횟수(초기화 시간이 되면 0으로 초기화 된다.)
	int m_NeedGameCount; //보상 지급을 받기 위해 필요한 게임횟수
	//int m_TodayRewardLimtCount; //오늘 지급받을 수 있는 보상횟수
	int m_TicketCount; //응모권등 사용하기 전까지 보유할수 있는 재화의 갯수
	int m_TicketLimitCount; //응모권등 보유할 수 있는 최대치
	// 황금 복주머니 유저 인포
	int m_CurrentGameCount_gold; //현재 게임진행 판수. 보상이 지급되면 0으로 초기화 된다.
	//int m_TodayRewardCount_gold; //오늘 지급받은 보상횟수(초기화 시간이 되면 0으로 초기화 된다.)
	int m_NeedGameCount_gold; //보상 지급을 받기 위해 필요한 게임횟수
	//int m_TodayRewardLimtCount_gold; //오늘 지급받을 수 있는 보상횟수
	int m_TicketCount_gold; //응모권등 사용하기 전까지 보유할수 있는 재화의 갯수
	int m_TicketLimitCount_gold; //응모권등 보유할 수 있는 최대치
	// 치킨타임 이벤트 유저 인포
	int m_CurrentGameCount_chicken; //현재 게임진행 판수. 보상이 지급되면 0으로 초기화 된다.
	int m_TodayRewardCount_chicken;
	
	// 복주머니 보상
	int m_TodayRewardCount; //오늘 지급받은 보상횟수(초기화 시간이 되면 0으로 초기화 된다.)
	int m_TodayRewardLimtCount; //오늘 지급받을 수 있는 보상횟수
	char *m_RewardType; //보상타입 = GM.게임머니, GD골드, CP.쿠폰, DC.할인권
	INT64 m_RewardAmount; //보상금액(만일 쿠폰이 한개 지급되었다면 여기에 1이 들어감)
	char *m_RewardAddInfo; //보상에 대한 추가 정보, 만일 쿠폰등이 지급되었다면 쿠폰번호가 들어감.
	SYSTEMTIME m_ExpiredTime; //보상 받을 수 있는 만료일. 게임머니나 골드등 즉시 지급 상품이라면 의미 없음
	// 황금 복주머니 보상
	int m_TodayRewardCount_gold; //오늘 지급받은 보상횟수(초기화 시간이 되면 0으로 초기화 된다.)
	int m_TodayRewardLimtCount_gold; //오늘 지급받을 수 있는 보상횟수
	char *m_RewardType_gold; //보상타입 = GM.게임머니, GD골드, CP.쿠폰, DC.할인권
	INT64 m_RewardAmount_gold; //보상금액(만일 쿠폰이 한개 지급되었다면 여기에 1이 들어감)
	char *m_RewardAddInfo_gold; //보상에 대한 추가 정보, 만일 쿠폰등이 지급되었다면 쿠폰번호가 들어감.
	SYSTEMTIME m_ExpiredTime_gold; //보상 받을 수 있는 만료일. 게임머니나 골드등 즉시 지급 상품이라면 의미 없음
	// 치킨 보상
	DWORD m_dwChickenTimeStartTime;
	DWORD m_dwChickenRewardStartTime; //오늘 보상지급
	
	//치킨 타임인지 여부
	BOOL m_bTimeForChicken;
	BOOL m_bTimeForNewYear;
	bool m_bPlayReward;
	bool m_bPlayReward_gold;
	bool m_bGoldReward;
	
public:
	// 생성자, 소멸자
	CPromotionNewYearLuckyBag(CPromotionManager *pManager, int nType);
	virtual ~CPromotionNewYearLuckyBag(void);
	
	// 가상
	virtual void Init(CUIInfoData *pUIInfoData, CGame *pGame, CChattingWindowMan *pChatMan);
	
	virtual BOOL OnLButtonUpTop(int &x, int &y);
	virtual BOOL OnMouseMoveTop(int &x, int &y);
	
	virtual BOOL OnEventTabLButtonDown(int &x, int &y);
	virtual BOOL OnEventTabLButtonUp(int &x, int &y);
	virtual BOOL OnEventTabMouseMove(int &x, int &y);
	
	virtual BOOL OnCommand(WPARAM &wParam, LPARAM &lParam);
	
	virtual void DrawBottom(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage);
	virtual void DrawMiddle(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage);
	virtual void DrawTop(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage);
	virtual void DrawEventTab(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage, const CPoint &ptStart);
	
	virtual BOOL OnPacketNotify(char* pName, int &Signal, int &TotSize, char *lpData);
	
protected:
	//// 사용자 정의
	// 인게임 왼쪽 아래에 배너 표시
	void DrawBanner(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage, int nLuckyBagNum, int nGoldLuckyBagNum);
	BOOL DrawBannerReward(NMBASE::GRAPHICGDI::CPage *pPage);
	BOOL DrawBannerReward_Gold(NMBASE::GRAPHICGDI::CPage *pPage);
	void DrawChickenBanner(NMBASE::GRAPHICGDI::CPage *pPage);
	
	bool CheckRoom();
	void GetNumberWithComma(int nNum, char cBuffer[]);
};
