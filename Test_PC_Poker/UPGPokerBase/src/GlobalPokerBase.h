#pragma once
#include "./OwnLimit/OwnLimit.h"
#include "./BanishMan/BanishMan.h"
#include "./Promotion/PromotionManager.h"
#include "define_sutda.h"
#include "./ChattingWindowMan/WhisperlistComboPopup.h"
#include "./VipJackpot/VipJackpot.h"

#define PB_IS_COMBO_POPUP_CLOSE "PB_IS_COMBO_POPUP_CLOSE"
//정해진 횟수 이상으로 자동 선택을 하게 되면 강퇴 당한다.
#define MAX_TURN_TIMEOUT_LIMIT_COUNT	2

//베팅 대기시간을 설정
#define TURN_TIMER_WAIT_TIME			5000

enum ENUM_TURN_TIMEOUT_TYPE
{
	ETTT_BETTING = 0,
	ETTT_CARDCHOICE,
	ETTT_CUTTING,
};

UPG_GAMEBASE_EXPORT extern const INT64 g_RoomMoneyRanges[13];

UPG_GAMEBASE_EXPORT extern COwnLimit			g_cOwnLimit;
UPG_GAMEBASE_EXPORT extern CBanishManager		g_cBanishMan;
UPG_GAMEBASE_EXPORT extern CPromotionManager	g_cPromotionMan;
UPG_GAMEBASE_EXPORT extern CVipJackpot			g_cVipJackpot; 

//prefix => PB : Poker Base
//룸이름을 만들어서 가져오는 함수
UPG_GAMEBASE_EXPORT LRESULT PBMakeRoomName(WPARAM& wParam, LPARAM& lParam);
UPG_GAMEBASE_EXPORT bool PBIsSuperRoom(void);
UPG_GAMEBASE_EXPORT int PBTurnTimeOut(ENUM_TURN_TIMEOUT_TYPE nType);
UPG_GAMEBASE_EXPORT void PBCreateWhisperlistComboPopup(void);

UPG_GAMEBASE_EXPORT bool PBIsComboPopupClose(const CPoint &ptPos);

UPG_GAMEBASE_EXPORT extern CWhisperlistComboPopup	g_cWhisperlistComboPopup;