#pragma once
#include "./OwnLimit/OwnLimit.h"
#include "./BanishMan/BanishMan.h"
#include "./Promotion/PromotionManager.h"
#include "define_sutda.h"
#include "./ChattingWindowMan/WhisperlistComboPopup.h"
#include "./VipJackpot/VipJackpot.h"

#define PB_IS_COMBO_POPUP_CLOSE "PB_IS_COMBO_POPUP_CLOSE"
//������ Ƚ�� �̻����� �ڵ� ������ �ϰ� �Ǹ� ���� ���Ѵ�.
#define MAX_TURN_TIMEOUT_LIMIT_COUNT	2

//���� ���ð��� ����
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
//���̸��� ���� �������� �Լ�
UPG_GAMEBASE_EXPORT LRESULT PBMakeRoomName(WPARAM& wParam, LPARAM& lParam);
UPG_GAMEBASE_EXPORT bool PBIsSuperRoom(void);
UPG_GAMEBASE_EXPORT int PBTurnTimeOut(ENUM_TURN_TIMEOUT_TYPE nType);
UPG_GAMEBASE_EXPORT void PBCreateWhisperlistComboPopup(void);

UPG_GAMEBASE_EXPORT bool PBIsComboPopupClose(const CPoint &ptPos);

UPG_GAMEBASE_EXPORT extern CWhisperlistComboPopup	g_cWhisperlistComboPopup;