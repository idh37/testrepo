#include "StdAfx.h"
#include "PromotionNewYearLuckyBag.h"

CPromotionNewYearLuckyBag::CPromotionNewYearLuckyBag(CPromotionManager *pManager, int nType) :
	CPromotionBase(pManager, nType)
{
	SetShowTab(false);
	
	// 배경 이미지
	string strDir = ".\\PokerCommon\\data\\Event\\NewYearLuckyBag\\";
	_LOADSPRITEDEFINE(&m_sprBack, strDir, "evt_2018newyear.spr");
	m_cEffect_back.SetSprite(&m_sprBack);
	
	strDir = ".\\PokerCommon\\data\\Event\\NewYearChicken\\";
	_LOADSPRITEDEFINE(&m_sprChicken, strDir, "evt_chicken.spr");
	m_cEffect_chicken.SetSprite(&m_sprChicken);
	
	// 버튼 이미지
	//char* ch = ".\\PokerCommon\\data\\jokbo_onoff.spr";
	char* chBtn = ".\\PokerCommon\\data\\Event\\NewYearLuckyBag\\evt_2018newyearbtn.spr";
	m_sprBtn.Load(chBtn, 555);
	
	// 응모하기 버튼 상태
	m_nBtnState01 = 0;
	m_nBtnState02 = 0;
	m_nBtnState03 = 0;
	m_nBtnState04 = 0;
	
	// 응모 횟수 텍스트
	m_pNum01 = NULL;
	m_pNum02 = NULL;
	m_pNum03 = NULL;
	m_pNum04 = NULL;
	m_pNum05 = NULL;
	
	// 응모 복주머니 개수 텍스트
	m_pBagNum01 = NULL;
	m_pBagNum02 = NULL;
	m_pBagNum03 = NULL;
	m_pBagNum04 = NULL;
	
	// 배너 mouse on 여부
	m_isOnBtn_bag = false;
	m_isOnBtn_goldBag = false;
	m_isOnBtn_chicken = false;
	
	//// 서버에서 받아오는 데이터
	// 복주머니 유저 인포
	m_CurrentGameCount = 0; //현재 게임진행 판수. 보상이 지급되면 0으로 초기화 된다.
	m_TodayRewardCount = 0; //오늘 지급받은 보상횟수(초기화 시간이 되면 0으로 초기화 된다.)
	m_NeedGameCount = 0; //보상 지급을 받기 위해 필요한 게임횟수
	m_TodayRewardLimtCount = 0; //오늘 지급받을 수 있는 보상횟수
	m_TicketCount = 0; //응모권등 사용하기 전까지 보유할수 있는 재화의 갯수
	m_TicketLimitCount = 0; //응모권등 보유할 수 있는 최대치
	// 황금 복주머니 유저 인포
	m_CurrentGameCount_gold = 0; //현재 게임진행 판수. 보상이 지급되면 0으로 초기화 된다.
	m_TodayRewardCount_gold = 0; //오늘 지급받은 보상횟수(초기화 시간이 되면 0으로 초기화 된다.)
	m_NeedGameCount_gold = 0; //보상 지급을 받기 위해 필요한 게임횟수
	m_TodayRewardLimtCount_gold = 0; //오늘 지급받을 수 있는 보상횟수
	m_TicketCount_gold = 0; //응모권등 사용하기 전까지 보유할수 있는 재화의 갯수
	m_TicketLimitCount_gold = 0; //응모권등 보유할 수 있는 최대치
	
	// 복주머니 보상
	//int m_TodayRewardCount; //오늘 지급받은 보상횟수(초기화 시간이 되면 0으로 초기화 된다.)
	//int m_TodayRewardLimtCount; //오늘 지급받을 수 있는 보상횟수
	m_RewardType = ""; //보상타입 = GM.게임머니, GD골드, CP.쿠폰, DC.할인권
	m_RewardAmount = 0; //보상금액(만일 쿠폰이 한개 지급되었다면 여기에 1이 들어감)
	m_RewardAddInfo = ""; //보상에 대한 추가 정보, 만일 쿠폰등이 지급되었다면 쿠폰번호가 들어감.
	//m_ExpiredTime = NULL; //보상 받을 수 있는 만료일. 게임머니나 골드등 즉시 지급 상품이라면 의미 없음
	// 황금 복주머니 보상
	//int m_TodayRewardCount_gold; //오늘 지급받은 보상횟수(초기화 시간이 되면 0으로 초기화 된다.)
	//int m_TodayRewardLimtCount_gold; //오늘 지급받을 수 있는 보상횟수
	m_RewardType_gold = ""; //보상타입 = GM.게임머니, GD골드, CP.쿠폰, DC.할인권
	m_RewardAmount_gold = 0; //보상금액(만일 쿠폰이 한개 지급되었다면 여기에 1이 들어감)
	m_RewardAddInfo_gold = ""; //보상에 대한 추가 정보, 만일 쿠폰등이 지급되었다면 쿠폰번호가 들어감.
	//m_ExpiredTime_gold = NULL; //보상 받을 수 있는 만료일. 게임머니나 골드등 즉시 지급 상품이라면 의미 없음
	
	m_dwRewardStartTime = 0;
	m_dwRewardStartTime_gold = 0;
	
	m_dwChickenTimeStartTime = 0;
	m_dwChickenRewardStartTime = 0;
	
	m_bTimeForChicken = FALSE;
	m_bTimeForNewYear = FALSE;
	
	// 응모권으로 응모할 수 있는 상품 리스트를 요청한다.
	CCL_PROMOTION_COMMON_REQUEST_TICKET_GIFTLIST msg;
	msg.Set(PROMOTION_COMMON_GAMECOUNT_PMCODE_2018_GOLDEN_LUCKYBAG);
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
	
	// 티켓으로 응모한 현황을 요청한다.
	CCL_PROMOTION_COMMON_TICKET_REQUEST_SUBSCRIPTION_LIST msg2;
	msg2.Set(PROMOTION_COMMON_GAMECOUNT_PMCODE_2018_GOLDEN_LUCKYBAG);
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg2.pData, msg2.GetTotalSize());
	
	m_bPlayReward = false;
	m_bPlayReward_gold = false;
	m_bGoldReward = false;
	
	//GM().SetShowEndEventPopup(true);
}

CPromotionNewYearLuckyBag::~CPromotionNewYearLuckyBag(void)
{

}

////////////////////////////////////////////////// 이벤트 //////////////////////////////////////////////////
void CPromotionNewYearLuckyBag::Init(CUIInfoData *pUIInfoData, CGame *pGame, CChattingWindowMan *pChatMan)
{
	// 실행 안됨..
	
	/*
	ASSERT(pUIInfoData != NULL && pGame != NULL);
	
	m_pUIInfoData = pUIInfoData;
	m_pGameProcess = pGame->GetGameProcess();
	m_pGameView = pGame->GetGameView();
	
	AddLoadImageList(&m_sprOnOff,".\\PokerCommon\\data\\jokbo_onoff.spr", true);
	
	//m_btnUse.Init(m_pGameView, &m_pGameView->Page, m_pUIInfoData->m_listButtonPos[EBT_JOKBOHELP_USE].x, m_pUIInfoData->m_listButtonPos[EBT_JOKBOHELP_USE].y, &m_sprOnOff, 2, UM_GH_USE);
	m_btnUse.Init(m_pGameView, &m_pGameView->Page, 0, 0, &m_sprOnOff, 0, 0);
	*/
}

BOOL CPromotionNewYearLuckyBag::OnLButtonUpTop(int &x , int &y)
{
	if (CheckRoom() == false)
		return FALSE;
	
	if (((m_TodayRewardCount >= m_TodayRewardLimtCount) || g_RI.llEnterLimitMoney >= 100000000000) && m_isOnBtn_btn)
	{
		m_pManager->SetChatViewPage_ToEventTab();
		m_pManager->SetCurTabPromotion(CPromotionManager::EPT_POKER_NEWYEAR);
		
		return TRUE;
	}
	
	return FALSE;
}

BOOL CPromotionNewYearLuckyBag::OnMouseMoveTop(int &x, int &y)
{
	if (CheckRoom() == false)
		return FALSE;
	
	// 배너
	switch (GM().GetCurrentGameType())
	{
	// 섯다
	case IDX_GAME_SD:
		m_isOnBtn_bag = (x >= 143 && y >= 71 && x <= 143 + 174 && y <= 71 + 44);
		m_isOnBtn_goldBag = (x > 143 + 174 && y > 71 && x < 143 + 281 && y < 71 + 44);
		m_isOnBtn_btn = (x >= 360 && y >= 74 && x <= 360 + 54 && y <= 74 + 20);
		m_isOnBtn_chicken = (x > 707 && y > 468 && x < 707 + 281 && y < 468 + 44);
		return TRUE;
	// 훌라
	case IDX_GAME_HA:
		m_isOnBtn_bag = (x >= 95 && y >= 57 && x <= 95 + 174 && y <= 57 + 44);
		m_isOnBtn_goldBag = (x > 95 + 174 && y > 57 && x < 95 + 281  && y < 57 + 44);
		m_isOnBtn_btn = (x >= 312 && y >= 60 && x <= 312 + 54 && y <= 60 + 20);
		m_isOnBtn_chicken = (x > 715 && y > 33 && x < 715 + 281 && y < 33 + 44);
		return TRUE;
	// 기타
	default:
		m_isOnBtn_bag = (x >= 12 && y >= 468 && x <= 12 + 174 && y <= 468 + 44);
		m_isOnBtn_goldBag = (x > 12 + 174 && y > 468 && x < 12 + 281 && y < 468 + 44);
		m_isOnBtn_btn = (x >= 229 && y >= 471 && x <= 229 + 54 && y <= 471 + 20);
		m_isOnBtn_chicken = (x > 707 && y > 468 && x < 707 + 281 && y < 468 + 44);
		return TRUE;
	}
	
	return FALSE;
}

BOOL CPromotionNewYearLuckyBag::OnEventTabLButtonDown(int &x , int &y)
{
	//if (CheckRoom() == false)
	//	return FALSE;
	
	if (m_btn01.OnLButtonDown(x, y))
	{
		m_nBtnState01 = 2;
		return TRUE;
	}
	if (m_btn02.OnLButtonDown(x, y))
	{
		m_nBtnState02 = 2;
		return TRUE;
	}
	if (m_btn03.OnLButtonDown(x, y))
	{
		m_nBtnState03 = 2;
		return TRUE;
	}
	if (m_btn04.OnLButtonDown(x, y))
	{
		m_nBtnState04 = 2;
		return TRUE;
	}
	
	return FALSE;
}

BOOL CPromotionNewYearLuckyBag::OnEventTabLButtonUp(int &x , int &y)
{
	//if (CheckRoom() == false)
	//	return FALSE;
	
	if (m_btn01.OnLButtonUp(x, y))
	{
		m_nBtnState01 = 0;
		return TRUE;
	}
	else if (m_btn02.OnLButtonUp(x, y))
	{
		m_nBtnState02 = 0;
		return TRUE;
	}
	else if (m_btn03.OnLButtonUp(x, y))
	{
		m_nBtnState03 = 0;
		return TRUE;
	}
	else if (m_btn04.OnLButtonUp(x, y))
	{
		m_nBtnState04 = 0;
		return TRUE;
	}
	
	return FALSE;
}

BOOL CPromotionNewYearLuckyBag::OnEventTabMouseMove(int &x , int &y)
{
	//if (CheckRoom() == false)
	//	return FALSE;
	
	// 응보하기 버튼
	bool bRet = FALSE;
	m_nBtnState01 = 0;
	m_nBtnState02 = 0;
	m_nBtnState03 = 0;
	m_nBtnState04 = 0;
	
	if (m_btn01.OnMouseMove(x,y))
	{
		m_nBtnState01 = 1;
		bRet = TRUE;
	}
	else if (m_btn02.OnMouseMove(x,y))
	{
		m_nBtnState02 = 1;
		bRet = TRUE;
	}
	else if (m_btn03.OnMouseMove(x,y))
	{
		m_nBtnState03 = 1;
		bRet = TRUE;
	}
	else if (m_btn04.OnMouseMove(x,y))
	{
		m_nBtnState04 = 1;
		bRet = TRUE;
	}
	
	if (bRet)
	{
		//SetCursor(GM().GetCurHandCursor());
	}
	
	return FALSE;
}

/// <summary>
/// 응모권을 사용하여 상품에 응모한다.
/// <summary>
/// <param name=""></param>
/// <returns></returns>
BOOL CPromotionNewYearLuckyBag::OnCommand(WPARAM &wParam, LPARAM &lParam)
{
	// 골드 150만개
	int nGiftIdx = 0;
	char* cGiftName = "";
	
	TICKET_GIFT_INFO *ArrayTicketGiftList_gold = GM().GetTicketGiftList();
	
	// 응모하기 버튼 마우스 클릭 이벤트
	switch(wParam)
	{
	case UM_LB_BTN01:
		{
			// 골드 150만개
			nGiftIdx = ArrayTicketGiftList_gold[0].GiftIndex;
			cGiftName = ArrayTicketGiftList_gold[0].GiftName;
		}
		break;
	case UM_LB_BTN02:
		{
			// 머니 100조
			nGiftIdx = ArrayTicketGiftList_gold[1].GiftIndex;
			cGiftName = ArrayTicketGiftList_gold[1].GiftName;
		}
		break;
	case UM_LB_BTN03:
		{
			// 보너스 쿠폰 50%
			nGiftIdx = ArrayTicketGiftList_gold[2].GiftIndex;
			cGiftName = ArrayTicketGiftList_gold[2].GiftName;
		}
		break;
	case UM_LB_BTN04:
		{
			// 보너스 쿠폰 30%
			nGiftIdx = ArrayTicketGiftList_gold[3].GiftIndex;
			cGiftName = ArrayTicketGiftList_gold[3].GiftName;
		}
		break;
	default:
		return FALSE;
	}
	
	// 응모권을 사용하여 상품에 응모한다.
	CCL_PROMOTION_COMMON_TICKET_REQUEST_SUBSCRIPTION msg;
	msg.Set(PROMOTION_COMMON_GAMECOUNT_PMCODE_2018_GOLDEN_LUCKYBAG, nGiftIdx);
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
	
	return TRUE;
}

void CPromotionNewYearLuckyBag::DrawBottom(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage)
{
	if (CheckRoom() == false)
		return;
	

}

void CPromotionNewYearLuckyBag::DrawMiddle(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage)
{
	if (CheckRoom() == false)
		return;
	
	// 여기는 실행이 안된다????
	
	// 현재 게임 타입이 섯다 일 경우
	/*
	if (GM().GetCurrentGameType() == IDX_GAME_SD)
	{
		pPage->PutSprAuto(256, 92, &m_sprBack, 23 + 0, AB);
		pPage->PutSprAuto(369, 92, &m_sprBack, 23 + 0, AB);
	}
	*/
}

/// <summary>
/// 인게임 왼쪽 아래 이벤트 탭에 설 이벤트 탭 추가
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CPromotionNewYearLuckyBag::DrawTop(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage)
{
	if (CheckRoom() == false)
		return;
	
	int nLuckyBagNum = m_CurrentGameCount;
	int nGoldLuckyBagNum = m_CurrentGameCount_gold;
	// 보상 지급 에니메이션이 실행중이 아닐때만 배너 표시
	DrawBanner(pDC, pPage, nLuckyBagNum, nGoldLuckyBagNum);
	m_bPlayReward = DrawBannerReward(pPage);
	m_bPlayReward_gold = DrawBannerReward_Gold(pPage);
	DrawChickenBanner(pPage);
}

/// <summary>
/// 인게임 왼쪽 아래 이벤트 탭에 설 이벤트 탭 추가
/// <summary>
/// <param name="ptStart">(12, 537)</param>
/// <returns></returns>
void CPromotionNewYearLuckyBag::DrawEventTab(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage, const CPoint &ptStart)
{
	// 현재 시각
	DWORD dwNow = timeGetTime();
	
	// 배경 그리기
	//pPage->PutSprAuto(ptStart.x, ptStart.y, &m_sprBack, 0);
	pPage->DrawMultiScene(0, 0, &m_sprBack, 0, dwNow, TRUE);
	// 이펙트 초기화
	m_cEffect_back.ResetEffect();
	// 이펙트 선택
	m_cEffect_back.ShowEffect(0, TRUE, FALSE);
	// 멀티신 1번트랙을 (25 + nUnits)번 그림으로 변경
	int MyTicketCount = GM().GetMyTicketCount();
	if (MyTicketCount >= 0)
		m_cEffect_back.ChangeScean(1, 25 + ((MyTicketCount / 1) % 10)); // 나의 황금 복주머니 개수 일의 자리
	else
		m_cEffect_back.ChangeScean(1, 24); // 나의 황금 복주머니 개수 일의 자리
	if (MyTicketCount >= 10)
		m_cEffect_back.ChangeScean(2, 25 + ((MyTicketCount / 10) % 10)); // 나의 황금 복주머니 개수 십의 자리
	else
		m_cEffect_back.ChangeScean(2, 24); // 나의 황금 복주머니 개수 십의 자리
	if (MyTicketCount >= 100)
		m_cEffect_back.ChangeScean(3, 25 + ((MyTicketCount / 100) % 10)); // 나의 황금 복주머니 개수 백의 자리
	else
		m_cEffect_back.ChangeScean(3, 24); // 나의 황금 복주머니 개수 백의 자리
	if (MyTicketCount >= 1000)
		m_cEffect_back.ChangeScean(4, 25 + ((MyTicketCount / 1000) % 10)); // 나의 황금 복주머니 개수 천의 자리
	else
		m_cEffect_back.ChangeScean(4, 24); // 나의 황금 복주머니 개수 천의 자리
	if (MyTicketCount >= 10000)
		m_cEffect_back.ChangeScean(5, 25 + ((MyTicketCount / 10000) % 10)); // 나의 황금 복주머니 개수 만의 자리
	else
		m_cEffect_back.ChangeScean(5, 24); // 나의 황금 복주머니 개수 만의 자리
	if (MyTicketCount >= 100000)
		m_cEffect_back.ChangeScean(6, 25 + ((MyTicketCount / 100000) % 10)); // 나의 황금 복주머니 개수 십만의 자리
	else
		m_cEffect_back.ChangeScean(6, 24); // 나의 황금 복주머니 개수 십만의 자리
	
	// 복주머니 배수 아이콘
	//pPage->PutSprAuto(ptStart.x + 198, ptStart.y + 61, &m_sprBack, 73);
	//pPage->PutSprAuto(ptStart.x + 198, ptStart.y + 61 + (34 * 1), &m_sprBack, 74);
	//pPage->PutSprAuto(ptStart.x + 198, ptStart.y + 61 + (34 * 2), &m_sprBack, 75);
	//pPage->PutSprAuto(ptStart.x + 198, ptStart.y + 61 + (34 * 3), &m_sprBack, 76);
	
	CPromotionManager *pManager = m_pManager;
	CChattingWindowMan *pChatMan = NULL;
	CGameViewBase *pGameViewBase = NULL;
	
	if (m_pManager != NULL)
		pChatMan = pManager->GetChattingWindowMan();
	
	if (pChatMan != NULL)
		pGameViewBase = pChatMan->GetGameViewBase();
	
	if (pGameViewBase != NULL)
	{
		// 버튼 1
		m_btn01.Init(pGameViewBase, pPage, ptStart.x + 219, ptStart.y + 52, &m_sprBtn, 0, UM_LB_BTN01);
		m_btn01.SetButtonState(m_nBtnState01);
		m_btn01.Draw(pDC);
		//m_btnTest.Enable(TRUE);
		
		// 버튼 2
		m_btn02.Init(pGameViewBase, pPage, ptStart.x + 219, ptStart.y + 52 + 34, &m_sprBtn, 0, UM_LB_BTN02);
		m_btn02.SetButtonState(m_nBtnState02);
		m_btn02.Draw(pDC);
		
		// 버튼 3
		m_btn03.Init(pGameViewBase, pPage, ptStart.x + 219, ptStart.y + 52 + (34 * 2), &m_sprBtn, 0, UM_LB_BTN03);
		m_btn03.SetButtonState(m_nBtnState03);
		m_btn03.Draw(pDC);
		
		// 버튼 4
		m_btn04.Init(pGameViewBase, pPage, ptStart.x + 219, ptStart.y + 52 + (34 * 3), &m_sprBtn, 0, UM_LB_BTN04);
		m_btn04.SetButtonState(m_nBtnState04);
		m_btn04.Draw(pDC);
		
		int nX = 141 - 12; // x좌표
		int nY = 601 - 537; // y좌표
		int nW = 50; // 너비
		int nH = 20; // 높이
		int nYD = 635 - 601; // y 거리
		int nI = 0; // 인덱스
		DWORD color = RGB(205, 193, 83);
		NMBASE::UTIL::STD_FONT font_index = NMBASE::UTIL::FONT_11D_NORMAL;
		
		char cText01[32];
		char cText02[32];
		char cText03[32];
		char cText04[32];
		
		TICKET_SUBSCRIPTION_INFO *ArraySubscriptionList_gold = GM().GetSubscriptionList();
		
		int n1 = ArraySubscriptionList_gold[0].SB_Count;
		int n2 = ArraySubscriptionList_gold[1].SB_Count;
		int n3 = ArraySubscriptionList_gold[2].SB_Count;
		int n4 = ArraySubscriptionList_gold[3].SB_Count;
		
		// 세 자리 수 마다 콤마 찍어주기
		GetNumberWithComma(n1, cText01);
		GetNumberWithComma(n2, cText02);
		GetNumberWithComma(n3, cText03);
		GetNumberWithComma(n4, cText04);
		
		strcat(cText01, "회");
		strcat(cText02, "회");
		strcat(cText03, "회");
		strcat(cText04, "회");
		
		// 응모 횟수 텍스트 1
		if (m_pNum01 == NULL)
		{
			//CRect rtRect = CRect(ptStart.x + 129, ptStart.y + 64, ptStart.x + 129 + 49, ptStart.y + 64 + 11);
			CRect rtRect = CRect(ptStart.x + nX, ptStart.y + nY + (nYD * nI), ptStart.x + nX + nW, ptStart.y + nY + nH + (nYD * nI));
			nI++;
			m_pNum01 = new NMBASE::SKINGDI::CNText(rtRect, "", 0, pPage, pDC);
			m_pNum01->SetFont(&NMBASE::UTIL::GetMyFont(font_index));
			m_pNum01->SetColor(color);
			m_pNum01->SetAlign(DT_SINGLELINE | DT_RIGHT | DT_TOP);
		}
		m_pNum01->SetText(cText01);
		m_pNum01->DrawText(pDC);
		
		// 응모 횟수 텍스트 2
		if (m_pNum02 == NULL)
		{
			//CRect rtRect = CRect(ptStart.x + 129, ptStart.y + 64, ptStart.x + 129 + 49, ptStart.y + 64 + 11);
			CRect rtRect = CRect(ptStart.x + nX, ptStart.y + nY + (nYD * nI), ptStart.x + nX + nW, ptStart.y + nY + nH + (nYD * nI));
			nI++;
			m_pNum02 = new NMBASE::SKINGDI::CNText(rtRect, "", 0, pPage, pDC);
			m_pNum02->SetFont(&NMBASE::UTIL::GetMyFont(font_index));
			m_pNum02->SetColor(color);
			m_pNum02->SetAlign(DT_SINGLELINE | DT_RIGHT | DT_TOP);
		}
		m_pNum02->SetText(cText02);
		m_pNum02->DrawText(pDC);
		
		// 응모 횟수 텍스트 3
		if (m_pNum03 == NULL)
		{
			//CRect rtRect = CRect(ptStart.x + 129, ptStart.y + 64, ptStart.x + 129 + 49, ptStart.y + 64 + 11);
			CRect rtRect = CRect(ptStart.x + nX, ptStart.y + nY + (nYD * nI), ptStart.x + nX + nW, ptStart.y + nY + nH + (nYD * nI));
			nI++;
			m_pNum03 = new NMBASE::SKINGDI::CNText(rtRect, "", 0, pPage, pDC);
			m_pNum03->SetFont(&NMBASE::UTIL::GetMyFont(font_index));
			m_pNum03->SetColor(color);
			m_pNum03->SetAlign(DT_SINGLELINE | DT_RIGHT | DT_TOP);
		}
		m_pNum03->SetText(cText03);
		m_pNum03->DrawText(pDC);
		
		// 응모 횟수 텍스트 4
		if (m_pNum04 == NULL)
		{
			//CRect rtRect = CRect(ptStart.x + 129, ptStart.y + 64, ptStart.x + 129 + 49, ptStart.y + 64 + 11);
			CRect rtRect = CRect(ptStart.x + nX, ptStart.y + nY + (nYD * nI), ptStart.x + nX + nW, ptStart.y + nY + nH + (nYD * nI));
			nI++;
			m_pNum04 = new NMBASE::SKINGDI::CNText(rtRect, "", 0, pPage, pDC);
			m_pNum04->SetFont(&NMBASE::UTIL::GetMyFont(font_index));
			m_pNum04->SetColor(color);
			m_pNum04->SetAlign(DT_SINGLELINE | DT_RIGHT | DT_TOP);
		}
		m_pNum04->SetText(cText04);
		m_pNum04->DrawText(pDC);
		
		nX = 210 - 12; // x좌표
		nY = 598 - 537; // y좌표
		nW = 20; // 너비
		nH = 10; // 높이
		nYD = 632 - 598; // y 거리
		nI = 0; // 인덱스
		color = RGB(101, 56, 25); // 글자색
		font_index = NMBASE::UTIL::FONT_11D_BOLD; // 폰트
		
		// 필요한 복주머니 갯수
		TICKET_GIFT_INFO *ArrayTicketGiftList_gold = GM().GetTicketGiftList();
		
		sprintf(cText01, "x%d", ArrayTicketGiftList_gold[0].NeedTicketCount);
		sprintf(cText02, "x%d", ArrayTicketGiftList_gold[1].NeedTicketCount);
		sprintf(cText03, "x%d", ArrayTicketGiftList_gold[2].NeedTicketCount);
		sprintf(cText04, "x%d", ArrayTicketGiftList_gold[3].NeedTicketCount);
		
		// 응모 복주머니 개수 텍스트 1
		if (m_pBagNum01 == NULL)
		{
			//CRect rtRect = CRect(ptStart.x + 198, ptStart.y + 61, ptStart.x + 198 + 49, ptStart.y + 61 + 11);
			CRect rtRect = CRect(ptStart.x + nX, ptStart.y + nY + (nYD * nI), ptStart.x + nX + nW, ptStart.y + nY + nH + (nYD * nI));
			nI++;
			m_pBagNum01 = new NMBASE::SKINGDI::CNText(rtRect, "", 0, pPage, pDC);
			m_pBagNum01->SetFont(&NMBASE::UTIL::GetMyFont(font_index));
			m_pBagNum01->SetColor(color);
			m_pBagNum01->SetAlign(DT_SINGLELINE | DT_LEFT | DT_TOP);
		}
		m_pBagNum01->SetText(cText01);
		m_pBagNum01->DrawText(pDC);
		
		// 응모 복주머니 개수 텍스트 2
		if (m_pBagNum02 == NULL)
		{
			//CRect rtRect = CRect(ptStart.x + 198, ptStart.y + 61, ptStart.x + 198 + 49, ptStart.y + 61 + 11);
			CRect rtRect = CRect(ptStart.x + nX, ptStart.y + nY + (nYD * nI), ptStart.x + nX + nW, ptStart.y + nY + nH + (nYD * nI));
			nI++;
			m_pBagNum02 = new NMBASE::SKINGDI::CNText(rtRect, "", 0, pPage, pDC);
			m_pBagNum02->SetFont(&NMBASE::UTIL::GetMyFont(font_index));
			m_pBagNum02->SetColor(color);
			m_pBagNum02->SetAlign(DT_SINGLELINE | DT_LEFT | DT_TOP);
		}
		m_pBagNum02->SetText(cText02);
		m_pBagNum02->DrawText(pDC);
		
		// 응모 복주머니 개수 텍스트 3
		if (m_pBagNum03 == NULL)
		{
			//CRect rtRect = CRect(ptStart.x + 198, ptStart.y + 61, ptStart.x + 198 + 49, ptStart.y + 61 + 11);
			CRect rtRect = CRect(ptStart.x + nX, ptStart.y + nY + (nYD * nI), ptStart.x + nX + nW, ptStart.y + nY + nH + (nYD * nI));
			nI++;
			m_pBagNum03 = new NMBASE::SKINGDI::CNText(rtRect, "", 0, pPage, pDC);
			m_pBagNum03->SetFont(&NMBASE::UTIL::GetMyFont(font_index));
			m_pBagNum03->SetColor(color);
			m_pBagNum03->SetAlign(DT_SINGLELINE | DT_LEFT | DT_TOP);
		}
		m_pBagNum03->SetText(cText03);
		m_pBagNum03->DrawText(pDC);
		
		// 응모 복주머니 개수 텍스트 4
		if (m_pBagNum04 == NULL)
		{
			//CRect rtRect = CRect(ptStart.x + 198, ptStart.y + 61, ptStart.x + 198 + 49, ptStart.y + 61 + 11);
			CRect rtRect = CRect(ptStart.x + nX, ptStart.y + nY + (nYD * nI), ptStart.x + nX + nW, ptStart.y + nY + nH + (nYD * nI));
			nI++;
			m_pBagNum04 = new NMBASE::SKINGDI::CNText(rtRect, "", 0, pPage, pDC);
			m_pBagNum04->SetFont(&NMBASE::UTIL::GetMyFont(font_index));
			m_pBagNum04->SetColor(color);
			m_pBagNum04->SetAlign(DT_SINGLELINE | DT_LEFT | DT_TOP);
		}
		m_pBagNum04->SetText(cText04);
		m_pBagNum04->DrawText(pDC);
	}
}

BOOL CPromotionNewYearLuckyBag::OnPacketNotify(char* pName, int &Signal, int &TotSize, char *lpData)
{
	switch(Signal)
	{
	case SV_PROMOTION_COMMON_GAMECOUNT_USERINFO:
		{
			// 일반적으로 게임판수를 이용하여 진행하는 프로모션에 대한 정보
			// 1000억방 이하
			CSV_PROMOTION_COMMON_GAMECOUNT_USERINFO msg;
			msg.Get(lpData, TotSize);
			
			switch(*msg.m_PM_Code)
			{
			// 2018년 복주머니 이벤트
			case PROMOTION_COMMON_GAMECOUNT_PMCODE_2018_LUCKYBAG:
				{
					m_CurrentGameCount = *msg.m_CurrentGameCount; //현재 게임진행 판수. 보상이 지급되면 0으로 초기화 된다.
					m_TodayRewardCount = *msg.m_TodayRewardCount; //오늘 지급받은 보상횟수(초기화 시간이 되면 0으로 초기화 된다.)
					m_NeedGameCount = *msg.m_NeedGameCount; //보상 지급을 받기 위해 필요한 게임횟수
					m_TodayRewardLimtCount = *msg.m_TodayRewardLimtCount; //오늘 지급받을 수 있는 보상횟수
					m_TicketCount = *msg.m_TicketCount; //응모권등 사용하기 전까지 보유할수 있는 재화의 갯수
					m_TicketLimitCount = *msg.m_TicketLimitCount; //응모권등 보유할 수 있는 최대치

					MM().Call(UPG_CMK_SHOW_CHICKEN_PROMOTION_BTN, TRUE);
					MM().Call(UPG_CMK_SHOW_GOLDBACK_PROMOTION_BTN, TRUE);
				}
				break;
			// 2018년 황금 복주머니 이벤트
			case PROMOTION_COMMON_GAMECOUNT_PMCODE_2018_GOLDEN_LUCKYBAG:
				{
					m_CurrentGameCount_gold = *msg.m_CurrentGameCount; //현재 게임진행 판수. 보상이 지급되면 0으로 초기화 된다.
					m_TodayRewardCount_gold = *msg.m_TodayRewardCount; //오늘 지급받은 보상횟수(초기화 시간이 되면 0으로 초기화 된다.)
					m_NeedGameCount_gold = *msg.m_NeedGameCount; //보상 지급을 받기 위해 필요한 게임횟수
					m_TodayRewardLimtCount_gold = *msg.m_TodayRewardLimtCount; //오늘 지급받을 수 있는 보상횟수
					m_TicketCount_gold = *msg.m_TicketCount; //응모권등 사용하기 전까지 보유할수 있는 재화의 갯수
					m_TicketLimitCount_gold = *msg.m_TicketLimitCount; //응모권등 보유할 수 있는 최대치

					MM().Call(UPG_CMK_SHOW_CHICKEN_PROMOTION_BTN, TRUE);
					MM().Call(UPG_CMK_SHOW_GOLDBACK_PROMOTION_BTN, TRUE);
				}
				break;
			case PROMOTION_COMMON_GAMECOUNT_PMCODE_2018_NEWYEAR_CHICKEN:
				{
					m_CurrentGameCount_chicken = *msg.m_CurrentGameCount; //현재 게임진행 판수. 보상이 지급되면 0으로 초기화 된다.
					m_TodayRewardCount_chicken = *msg.m_TodayRewardCount; //현재 치킨 이벤트 차수.
				}
				break;
			}
		}
		break;
	case SV_PROMOTION_COMMON_GAMECOUNT_GET_REWARD:
		{
			//일반적으로 게임판수를 이용하여 진행하는 프로모션에 대한 보상발생
			CSV_PROMOTION_COMMON_GAMECOUNT_GET_REWARD msg;
			msg.Get(lpData, TotSize);
			
			switch(*msg.m_PM_Code)
			{
			// 2018년 복주머니 이벤트
			case PROMOTION_COMMON_GAMECOUNT_PMCODE_2018_LUCKYBAG:
				{
					m_TodayRewardCount = *msg.m_TodayRewardCount; //오늘 지급받은 보상횟수(초기화 시간이 되면 0으로 초기화 된다.)
					m_TodayRewardLimtCount = *msg.m_TodayRewardLimtCount; //오늘 지급받을 수 있는 보상횟수
					m_RewardType = msg.m_RewardType; //보상타입 = GM.게임머니, GD골드, CP.쿠폰, DC.할인권
					m_RewardAmount = *msg.m_RewardAmount; //보상금액(만일 쿠폰이 한개 지급되었다면 여기에 1이 들어감)
					m_RewardAddInfo = msg.m_RewardAddInfo; //보상에 대한 추가 정보, 만일 쿠폰등이 지급되었다면 쿠폰번호가 들어감.
					m_ExpiredTime = *msg.m_ExpiredTime; //보상 받을 수 있는 만료일. 게임머니나 골드등 즉시 지급 상품이라면 의미 없음
					
					m_dwRewardStartTime = timeGetTime();
				}
				break;
			// 2018년 황금 복주머니 이벤트
			case PROMOTION_COMMON_GAMECOUNT_PMCODE_2018_GOLDEN_LUCKYBAG:
				{
					m_TodayRewardCount_gold = *msg.m_TodayRewardCount; //오늘 지급받은 보상횟수(초기화 시간이 되면 0으로 초기화 된다.)
					m_TodayRewardLimtCount_gold = *msg.m_TodayRewardLimtCount; //오늘 지급받을 수 있는 보상횟수
					m_RewardType_gold = msg.m_RewardType; //보상타입 = GM.게임머니, GD골드, CP.쿠폰, DC.할인권
					m_RewardAmount_gold = *msg.m_RewardAmount; //보상금액(만일 쿠폰이 한개 지급되었다면 여기에 1이 들어감)
					m_RewardAddInfo_gold = msg.m_RewardAddInfo; //보상에 대한 추가 정보, 만일 쿠폰등이 지급되었다면 쿠폰번호가 들어감.
					m_ExpiredTime_gold = *msg.m_ExpiredTime; //보상 받을 수 있는 만료일. 게임머니나 골드등 즉시 지급 상품이라면 의미 없음
					
					m_dwRewardStartTime_gold = timeGetTime();
					m_bGoldReward = true;
				}
				break;
			case PROMOTION_COMMON_GAMECOUNT_PMCODE_2018_NEWYEAR_CHICKEN:
				{
					if(*msg.m_RewardAmount > 0)
					{
						GM().GetPromotionData()->nCnts[0] = 1;
						m_dwChickenRewardStartTime = timeGetTime();
					}
				}
				break;
			}
		}
		break;
	case SV_PROMOTION_COMMON_GAMECOUNT_RUNNING_INFO:
		{
			CSV_PROMOTION_COMMON_GAMECOUNT_RUNNING_INFO msg;
			msg.Get(lpData, TotSize);
			
			switch(*msg.m_PM_Code)
			{
			case PROMOTION_COMMON_GAMECOUNT_PMCODE_2018_NEWYEAR_CHICKEN:
				{
					if(m_bTimeForChicken == FALSE && *msg.m_RunningFlag == TRUE)
					{
						m_dwChickenTimeStartTime = timeGetTime();
					}
					m_bTimeForChicken = *msg.m_RunningFlag;
				}
				break;
			case PROMOTION_COMMON_GAMECOUNT_PMCODE_2018_GOLDEN_LUCKYBAG:
				{
					m_bTimeForNewYear = *msg.m_RunningFlag;
				}
				break;
			}
		}
		break;

	case SV_PROMOTION_COMMON_REWARD_ALL_NOTICE:
		{
			CSV_PROMOTION_COMMON_REWARD_ALL_NOTICE msg;
			msg.Get(lpData, TotSize);

			CString strNewYear = "축하합니다!";
			CTime time = CTime::GetCurrentTime();
			if((2018==time.GetYear())&&(2==time.GetMonth())&&(15<=time.GetDay())&&(18>=time.GetDay()))
			{
				strNewYear.Append(" 설연휴 4배 치킨 지급!!");
			}
			CString str;
			str.Format("%s\n[%s]님이 [%s]-[%s]에서 %s에 당첨되셨습니다~!\n", strNewYear.GetString(), msg.m_NickName, msg.m_GameName, msg.m_ChannelName, msg.m_RewardName);

			AddGameInfoViewEdit(str.GetString(), RGB(143, 141, 102));
		}
		break;
		
	}
	
	return FALSE;
}

////////////////////////////////////////////////// 사용자 정의 //////////////////////////////////////////////////
/// <summary>
/// 인게임 왼쪽 아래에 배너 표시
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CPromotionNewYearLuckyBag::DrawBanner(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage, int nLuckyBagNum, int nGoldLuckyBagNum)
{
	if (CheckRoom() == false)
		return;
	// 혹시 몰라서 에러 핸들링 추가
	if (nLuckyBagNum > 5 || nLuckyBagNum < 0)
	{
		ShowMainMessageDlg("복주머니 정보를 가져올 수 없습니다.");
		return;
	}
	if (nGoldLuckyBagNum > 10 || nGoldLuckyBagNum < 0)
	{
		ShowMainMessageDlg("복주머니 정보를 가져올 수 없습니다.");
		return;
	}
	
	DWORD dwNow = timeGetTime();
	int nMultiScn_banner = 0;
	int nX_ = 0;
	int nY_ = 0;
	
	switch (GM().GetCurrentGameType())
	{
	// 섯다
	case IDX_GAME_SD:
		{
			nMultiScn_banner = 7;
			nX_ = 362; // x좌표
			nY_ = 99; // y좌표
		}
		break;
	// 훌라
	case IDX_GAME_HA:
		{
			nMultiScn_banner = 4;
			nX_ = 314; // x좌표
			nY_ = 85; // y좌표
		}
		break;
	// 기타
	default:
		{
			nMultiScn_banner = 1;
			nX_ = 231; // x좌표
			nY_ = 496; // y좌표
		}
		break;
	}
	
	// 복주머니 이벤트 진행중, 1000억 미만의 하위방
	// 30번째 에니메이션에서 황금 복주머니 배너로 전환 안되도록 조건 추가..(받은 복주머니 30개, 애니메이션 진행 중일때는 전환 x)
	if ((m_TodayRewardCount < m_TodayRewardLimtCount && g_RI.llEnterLimitMoney < 100000000000)
		|| (m_TodayRewardCount == m_TodayRewardLimtCount && m_bPlayReward)
		)
	{
		// 인게임 왼쪽 아래에 배너 표시
		pPage->DrawMultiScene(0, 0, &m_sprBack, nMultiScn_banner, dwNow, TRUE);
		
		// 이펙트 초기화
		m_cEffect_back.ResetEffect();
		// 멀티신 1번 선택
		m_cEffect_back.ShowEffect(nMultiScn_banner, TRUE, FALSE);
		// 멀티신 1번트랙을 (1 + nLuckyBagNum)번 그림으로 변경 - 일의 자리수 변경
		m_cEffect_back.ChangeScean(1, 2 + nLuckyBagNum); // 복주머니 일의자리
		if (nGoldLuckyBagNum < 10)
			m_cEffect_back.ChangeScean(2, 12); // 황금 복주머니 십의자리
		else
			m_cEffect_back.ChangeScean(2, 13 + (nGoldLuckyBagNum / 10)); // 황금 복주머니 십의자리
		m_cEffect_back.ChangeScean(3, 13 + (nGoldLuckyBagNum % 10)); // 황금 복주머니 일의자리
		
		// 복주머니 tooltip
		if (m_isOnBtn_bag)
		{
			pPage->DrawMultiScene(0, 0, &m_sprBack, nMultiScn_banner + 2, dwNow, TRUE);
			// 이펙트 초기화
			m_cEffect_back.ResetEffect();
			// 멀티신 3번 선택
			m_cEffect_back.ShowEffect(nMultiScn_banner + 2, TRUE, FALSE);
			// 멀티신 0번트랙을 (36 + 0)번 그림으로 변경
			m_cEffect_back.ChangeScean(0, 36 + 0);
		}
		// 황금 복주머니 tooltip
		else if (m_isOnBtn_goldBag)
		{
			pPage->DrawMultiScene(0, 0, &m_sprBack, nMultiScn_banner + 2, dwNow, TRUE);
			// 이펙트 초기화
			m_cEffect_back.ResetEffect();
			// 멀티신 3번 선택
			m_cEffect_back.ShowEffect(nMultiScn_banner + 2, TRUE, FALSE);
			// 멀티신 0번트랙을 (36 + 1)번 그림으로 변경
			m_cEffect_back.ChangeScean(0, 36 + 1);
		}
	}
	// 일일보상 획득 완료! 황금 복주머니도 놓치지 마세요.
	else
	{
		// 인게임 왼쪽 아래에 배너 표시
		pPage->DrawMultiScene(0, 0, &m_sprBack, nMultiScn_banner + 1, dwNow, TRUE);
		
		// 이펙트 초기화
		m_cEffect_back.ResetEffect();
		// 멀티신 1번 선택
		m_cEffect_back.ShowEffect(nMultiScn_banner + 1, TRUE, FALSE);
		// 멀티신 1번트랙을 (1 + nLuckyBagNum)번 그림으로 변경 - 일의 자리수 변경
		if (nGoldLuckyBagNum < 10)
			m_cEffect_back.ChangeScean(1, 12); // 황금 복주머니 십의자리
		else
			m_cEffect_back.ChangeScean(1, 13 + (nGoldLuckyBagNum / 10)); // 황금 복주머니 십의자리
		m_cEffect_back.ChangeScean(2, 13 + (nGoldLuckyBagNum % 10)); // 황금 복주머니 일의자리
		
		// 황금 복주머니 tooltip
		if (m_isOnBtn_bag || m_isOnBtn_goldBag)
		{
			pPage->DrawMultiScene(0, 0, &m_sprBack, nMultiScn_banner + 2, dwNow, TRUE);
			// 이펙트 초기화
			m_cEffect_back.ResetEffect();
			// 멀티신 3번 선택
			m_cEffect_back.ShowEffect(nMultiScn_banner + 2, TRUE, FALSE);
			// 멀티신 0번트랙을 (36 + 1)번 그림으로 변경
			m_cEffect_back.ChangeScean(0, 36 + 1); // 황금 복주머니 tooltip 이미지
		}
		
		int nX = nX_; // x좌표
		int nY = nY_; // y좌표
		int nW = 40; // 너비
		int nH = 20; // 높이
		int nYD = 0; // y 거리
		int nI = 0; // 인덱스
		DWORD color = RGB(199, 217, 213);
		NMBASE::UTIL::STD_FONT font_index = NMBASE::UTIL::FONT_11D_NORMAL;
		
		// 응모 횟수 텍스트 1
		if (m_pNum05 == NULL)
		{
			//CRect rtRect = CRect(ptStart.x + 129, ptStart.y + 64, ptStart.x + 129 + 49, ptStart.y + 64 + 11);
			CRect rtRect = CRect(nX, nY + (nYD * nI), nX + nW, nY + nH + (nYD * nI));
			nI++;
			m_pNum05 = new NMBASE::SKINGDI::CNText(rtRect, "", 0, pPage, pDC);
			m_pNum05->SetFont(&NMBASE::UTIL::GetMyFont(font_index));
			m_pNum05->SetColor(color);
			m_pNum05->SetAlign(DT_SINGLELINE | DT_RIGHT | DT_TOP);
		}
		int nMyTicketCount = GM().GetMyTicketCount();
		// 세 자리 수 마다 콤마 찍어주기
		char cMyTicketCount[32];
		CString csMyTicketCount;
		GetNumberWithComma(nMyTicketCount, cMyTicketCount);
		csMyTicketCount.Format(_T("%s"), cMyTicketCount);
		m_pNum05->SetText(csMyTicketCount);
		m_pNum05->DrawText(pDC);
	}
}

/// <summary>
/// 인게임 왼쪽 아래에 배너 표시
/// <summary>
/// <param name=""></param>
/// <returns></returns>
BOOL CPromotionNewYearLuckyBag::DrawBannerReward(NMBASE::GRAPHICGDI::CPage *pPage)
{
	int x = 0, y = 0;
	
	switch (GM().GetCurrentGameType())
	{
	// 섯다
	case IDX_GAME_SD:
		x = 144;
		y = 72;
		break;
	// 훌라
	case IDX_GAME_HA:
		x = 96;
		y = 58;
		break;
	// 기타
	default:
		x = 13;
		y = 469;
		break;
	}
	
	int nRewardIdx = 0;

	switch (m_RewardAmount)
	{
	// 10억 보상
	case 1000000000:
		{
			nRewardIdx = 0;
		}
		break;
	// 30억 보상
	case 3000000000:
		{
			nRewardIdx = 1;
		}
		break;
	// 80억 보상
	case 8000000000:
		{
			nRewardIdx = 2;
		}
		break;
	// 100억 보상
	case 10000000000:
		{
			nRewardIdx = 3;
		}
		break;
	// 150억 보상
	case 15000000000:
		{
			nRewardIdx = 4;
		}
		break;
	// 200억 보상
	case 20000000000:
		{
			nRewardIdx = 5;
		}
		break;
	// 100조 보상
	case 100000000000000:
		{
			nRewardIdx = 6;
		}
		break;
	default:
		return FALSE;
	}
	
	// n억 당첨
	DWORD playTime = m_sprBack.GetMScenePlayTime(13) * 1;
	DWORD dwCurTime = timeGetTime();
	DWORD dwRewardStartTime = m_dwRewardStartTime;
	DWORD dwRewardEndTime = dwRewardStartTime + playTime;
	if (dwCurTime < dwRewardEndTime)
	{
		pPage->DrawMultiScene(x, y, &m_sprBack, 13, dwCurTime - dwRewardStartTime);
		// 이펙트 초기화
		m_cEffect_back.ResetEffect();
		// 멀티신 1번 선택
		m_cEffect_back.ShowEffect(13, TRUE, FALSE);
		// 멀티신 0번트랙을 (39 + nRewardIdx)번 그림으로 변경
		m_cEffect_back.ChangeScean(0, 42 + nRewardIdx);
		
		return TRUE;
	}
	
	// 지급횟수 n/30회
	playTime = m_sprBack.GetMScenePlayTime(10) * 20;
	dwCurTime = timeGetTime();
	dwRewardStartTime = dwRewardEndTime;
	dwRewardEndTime = dwRewardStartTime + playTime;
	if (dwCurTime > dwRewardStartTime && dwCurTime < dwRewardEndTime)
	{
		pPage->DrawMultiScene(x, y, &m_sprBack, 10, dwCurTime - dwRewardStartTime);
		
		// 이펙트 초기화
		m_cEffect_back.ResetEffect();
		// 멀티신 1번 선택
		m_cEffect_back.ShowEffect(10, TRUE, FALSE);
		// 멀티신 n번트랙을 (1 + m_TodayRewardCount)번 그림으로 변경
		if (m_TodayRewardCount < 10)
			m_cEffect_back.ChangeScean(1, 1); // 십의자리
		else
			m_cEffect_back.ChangeScean(1, 2 + (m_TodayRewardCount / 10)); // 십의자리
		m_cEffect_back.ChangeScean(2, 2 + (m_TodayRewardCount % 10)); // 일의자리

		return TRUE;
	}
	
	// 일일보상 획득 완료! 황금 복주머니도 놓치지 마세요.
	if (m_TodayRewardCount == m_TodayRewardLimtCount)
	{
		playTime = m_sprBack.GetMScenePlayTime(11) * 20;
		dwCurTime = timeGetTime();
		dwRewardStartTime = dwRewardEndTime;
		dwRewardEndTime = dwRewardStartTime + playTime;
		if(dwCurTime > dwRewardStartTime && dwCurTime < dwRewardEndTime + playTime)
		{
			pPage->DrawMultiScene(x, y, &m_sprBack, 11, dwCurTime - dwRewardStartTime);

			return TRUE;
		}
	}
	
	return FALSE;
}

/// <summary>
/// 인게임 왼쪽 아래에 배너 표시
/// <summary>
/// <param name=""></param>
/// <returns></returns>
BOOL CPromotionNewYearLuckyBag::DrawBannerReward_Gold(NMBASE::GRAPHICGDI::CPage *pPage)
{
	int x = 0, y = 0;

	switch (GM().GetCurrentGameType())
	{
	// 섯다
	case IDX_GAME_SD:
		x = 311;
		y = 72;
		break;
	// 훌라
	case IDX_GAME_HA:
		x = 263;
		y = 58;
		break;
	// 기타
	default:
		x = 180;
		y = 469;
		break;
	}
	
	// n억 당첨
	DWORD playTime = m_sprBack.GetMScenePlayTime(14) * 1;
	DWORD dwCurTime = timeGetTime();
	DWORD dwRewardStartTime = m_dwRewardStartTime_gold;
	DWORD dwRewardEndTime = dwRewardStartTime + playTime;
	if (dwCurTime < dwRewardEndTime)
	{
		if (m_bGoldReward)
		{
			m_bGoldReward = false;

			// 티켓으로 응모한 현황을 요청한다.
			CCL_PROMOTION_COMMON_TICKET_REQUEST_SUBSCRIPTION_LIST msg2;
			msg2.Set(PROMOTION_COMMON_GAMECOUNT_PMCODE_2018_GOLDEN_LUCKYBAG);
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg2.pData, msg2.GetTotalSize());
		}
		
		pPage->DrawMultiScene(x, y, &m_sprBack, 14, dwCurTime - dwRewardStartTime);

		return TRUE;
	}
	
	return FALSE;
}

void CPromotionNewYearLuckyBag::DrawChickenBanner(NMBASE::GRAPHICGDI::CPage *pPage)
{
	DWORD dwNow = timeGetTime();
	int haIndex = 0;
	if (GM().GetCurrentGameType() == IDX_GAME_HA)
	{
		haIndex = 5;
	}
	
	if(m_bTimeForChicken==FALSE)
	{
		pPage->DrawMultiScene(0, 0, &m_sprChicken, 0+haIndex, dwNow);
	}
	else if(m_dwChickenTimeStartTime > 0 && dwNow < m_dwChickenTimeStartTime + m_sprChicken.GetMScenePlayTime(1+haIndex))
	{
		pPage->DrawMultiScene(0, 0, &m_sprChicken, 1+haIndex, dwNow - m_dwChickenTimeStartTime);
	}
	else if(m_dwChickenRewardStartTime > 0 && dwNow < m_dwChickenRewardStartTime + m_sprChicken.GetMScenePlayTime(4+haIndex))
	{
		pPage->DrawMultiScene(0, 0, &m_sprChicken, 4+haIndex, dwNow - m_dwChickenRewardStartTime);
	}
	else
	{
		m_cEffect_chicken.ShowEffect(2+haIndex, TRUE, FALSE);
		m_cEffect_chicken.ChangeScean(1, 6 + (m_CurrentGameCount_chicken>=10?m_CurrentGameCount_chicken/10:-1));
        m_cEffect_chicken.ChangeScean(2, 6 + m_CurrentGameCount_chicken%10);
        m_cEffect_chicken.ChangeScean(3, 6 + m_TodayRewardCount_chicken/100);
        m_cEffect_chicken.ChangeScean(4, 6 + (m_TodayRewardCount_chicken/10)%10);
        m_cEffect_chicken.ChangeScean(5, 6 + m_TodayRewardCount_chicken%10);
		pPage->DrawMultiScene(0, 0, &m_sprChicken, 2+haIndex, dwNow);
		
		if(m_TodayRewardCount_chicken>=3)
		{
			pPage->DrawMultiScene(0, 0, &m_sprChicken, 3+haIndex, dwNow);
		}
	}
	
	// tooltip
	if (m_isOnBtn_chicken)
	{
		if (GM().GetCurrentGameType() == IDX_GAME_HA)
		{
			pPage->DrawScene(715, 84, &m_sprChicken, 19, dwNow);
		}
		else
		{
			pPage->DrawScene(707, 417, &m_sprChicken, 19, dwNow);
		}
	}
}

bool CPromotionNewYearLuckyBag::CheckRoom()
{
	// 자유방 제외
	if (g_RI.llEnterLimitMoney == 0 || g_RI.bIsMaxRoom || g_RI.ChipKind == 1 || m_bTimeForNewYear == FALSE)
	{
		// 맥스방은 왼쪽하단의 채팅, 족보, 이벤트 탭 영역을 채팅(기존)에서 이벤트로 변경
		if (g_RI.bIsMaxRoom)
		{
			m_pManager->SetChatViewPage_ToEventTab();
			m_pManager->SetCurTabPromotion(CPromotionManager::EPT_POKER_NEWYEAR);
		}
		
		return false;
	}
	
	return true;
}

/// <summary>
/// 세 자리 수 마다 콤마 찍어주기
/// 이 함수 사용하면 안됨! 특정 PC에서 숫자 깨지는 현상 발생.
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CPromotionNewYearLuckyBag::GetNumberWithComma(int nNum, char cBuffer[])
{
	CString csNum;
	csNum.Format(_T("%d"), nNum);
	NUMBERFMT fmt = {0, 0, 3, ".", ",", 1};
	::GetNumberFormat(NULL, NULL, csNum, &fmt, cBuffer, strlen(cBuffer));
}
