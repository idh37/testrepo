// SprMoneyDraw.cpp: implementation of the CSprMoneyDraw class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GlobalPokerBase.h"
#include "sound.h"
#include <../src/MessageManager.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

COwnLimit					g_cOwnLimit;
CBanishManager				g_cBanishMan;
CPromotionManager			g_cPromotionMan;
CVipJackpot					g_cVipJackpot;
CWhisperlistComboPopup		g_cWhisperlistComboPopup;


const INT64 g_RoomMoneyRanges[13] = 
{
	0,					//1억 미만				
	100000000,			//1억
	1000000000,			//10억
	10000000000,		//100억
	100000000000,		//1000억
	1000000000000,		//1조
	3000000000000,		//3조
	10000000000000,		//10조
	30000000000000,		//30조
	50000000000000,		//50조
	100000000000000,	//100조
	100000000000000,	//200조
	100000000000000		//300조
};

class CPokerBaseFunctorRegister
{
public:
	CPokerBaseFunctorRegister();
	virtual ~CPokerBaseFunctorRegister(){}
};

CPokerBaseFunctorRegister s_cPokerBaseFunctorRegiste;

LRESULT PBMakeRoomName(WPARAM& wParam, LPARAM& lParam)
{
	CString title = "";

	int nRoomNameNum = (int)rand()%18;

	IDX_GAME eGameType = GM().GetCurrentGameType();
	if( eGameType == IDX_GAME_BD)
	{
		switch(nRoomNameNum){ 
		case 0: title = "오빠! 달려! 저녁까지~"; break;
		case 1: title = "절대 호구를 찾아서~"; break;
		case 2: title = "콜하지 못해 미안해~"; break;
		case 3: title = "골프는 고래도 춤추게 한다"; break;
		case 4: title = "지키는 매너 불어나는 포커머니"; break;
		case 5: title = "물반! 고기반! 막 퍼담는 방~"; break;
		case 6: title = "바둑이와 늑대의 시간"; break;
		case 7: title = "골프와 함께 사라지다"; break;
		case 8: title = "판돈부터 불리고 시작하자~"; break;
		case 9: title = "로우바둑이 한판 하고 가지?"; break;
		case 10: title = "니들이 로우바둑이를 알아?"; break;
		case 11: title = "손맛의 진수를 느껴봐"; break;
		case 12: title = "영웅들이여! 모여라"; break;
		case 13: title = "돈은 잃어도 매너는 잃지 말자"; break;
		case 14: title = "한번 해볼래? 기다릴께~ 들어와!"; break;
		case 15: title = "하루바둑이 범 무서운줄 모른다"; break;
		case 16: title = "콜이냐 다이냐 그것이 문제로다"; break;	
		case 17: title = "골프를 그대 품안에~"; break;
		}
	}
	else if( eGameType == IDX_GAME_SD)
	{
		switch(nRoomNameNum){ 
			case 0 : title = "윈조이 섯다 드디어 떳다"; break;
			case 1 : title = "콜하지 못해 미안해~"; break;
			case 2 : title = "삼팔광땡은 고래도 춤추게 한다"; break;
			case 3 : title = "삼팔광땡을 그대 품안에~"; break;
			case 4 : title = "물반! 고기반! 막 퍼담는 방~"; break;
			case 5 : title = "야인들의 뜨거운 승부"; break;
			case 6 : title = "삼팔광땡과 함께 사라지다."; break;
			case 7 : title = "판돈부터 불리고 시작하자구~"; break;
			case 8 : title = "섯다왕 김사구!"; break;
			case 9 : title = "섯다 한판 하고 가지?"; break;
			case 10 : title = "니들이 섯다를 알아?"; break;
			case 11 : title = "게 섯거라~ 그래 섯다!"; break;
			case 12 : title = "암행어사 출두야~"; break;
			case 13 : title = "땡값으로 빌딩 사자!"; break;
			case 14 : title = "손맛의 진수를 느껴봐"; break;
			case 15 : title = "영웅들이여! 모여라"; break;
			case 16 : title = "강퇴 싫어? 그럼 굿매너~"; break;
			case 17 : title = "◎ 1940 섯다시대 ◎"; break;
		}
	}
	else if( eGameType == IDX_GAME_HA)
	{
		switch(nRoomNameNum)
		{
			case 0 : title = "절대 호구를 찾아서~"; break;
			case 1 : title = "스톱하지 못해 미안해~"; break;
			case 2 : title = "훌라는 고래도 춤추게 한다"; break;
			case 3 : title = "지키는 매너 불어나는 머니"; break;
			case 4 : title = "물반! 고기반! 막 퍼담는 방~"; break;
			case 5 : title = "훌라 한판 하고 가지?"; break;
			case 6 : title = "니들이 훌라를 알아?"; break;
			case 7 : title = "영웅들이여! 모여라"; break;
			case 8 : title = "강퇴 싫어? 그럼 굿매너~"; break;
			case 9 : title = "돈은 잃어도 매너는 잃지 말자"; break;
			case 10 : title = "한번 해볼래? 기다릴께~ 들어와!"; break;
			case 11 : title = "훌라콘서트~"; break;
			case 12 : title = "울라울라~ 훌라훌라~"; break;
			case 13 : title = "윈조이 훌라 한판!"; break;
			case 14 : title = "놀지말고 훌라해요"; break;
			case 15 : title = "훌라 짱~☆"; break;
			case 16 : title = "훌라의 진수~★"; break;
			case 17 : title = "땡큐땡큐 감사합니다!"; break;
			case 18 : title = "스톱! 스톱! 스톱!"; break;
			case 19 : title = "감사할줄 아는 훌라! 땡큐!"; break;
			default : title = "감사할줄 아는 훌라! 땡큐!"; break;
		}
	}
	else
	{
		switch(nRoomNameNum)
		{
		case 0: title = "오빠! 달려! 히든까지~";break;
		case 1: title = "절대 호구를 찾아서~";break;
		case 2: title = "콜하지 못해 미안해~";break;
		case 3: title = "로티는 고래도 춤추게 한다";break;
		case 4: title = "지키는 매너 불어나는 머니";break;
		case 5: title = "물반 고기반! 막 퍼담는 방";break;
		case 6: title = "라스베가스에서 한 판!";break;
		case 7: title = "마카오에서 한 판!";break;
		case 8: title = "몬테카를로에서 한 판!";break;
		case 9: title = "로티플과 함께 사라지다";break;
		case 10: title = "판돈부터 불리고 시작하자~";break;
		case 11: title = "포커 한판 하고 가지?";break;
		case 12: title = "니들이 포커를 알아?";break;
		case 13: title = "손맛의 진수를 느껴봐";break;
		case 14: title = "못먹어도 레이스!!";break;
		case 15: title = "영웅들이여! 모여라";break;
		case 16: title = "널 위해 준비했어~ 로티플";break;
		case 17: title = "놀지말고 포커해욧~!";break;
		}
	}

	*((CString *)lParam) = title;
	return TRUE;
}

bool PBIsSuperRoom(void)
{
	//슈퍼방 로직 제거
	return false;

	// 슈퍼방장 아이템이 사용중이면 슈퍼방 만들기 옵션 자동 체크
	BOOL bExist = FALSE;
	CPlayer *pPlayer = GM().GetCurrentProcess()->GetPlayer(0);
	
	//게임방이나 회원제에 가입해 있다면
	if( UPGCheckPremiumState(&GM().GetMyInfo()->UI) >= CODE_NEW_FAMILY_SILVER || 0 != pPlayer->UI.nPCRoomIPType )
	{
		return true;
	}

	//슈퍼방장 아이템을 사용중일때
	if(UPGIsGameItemUsing(ITEM_SCODE_SUPERMASTER, &bExist)) 
	{
		return true;
	}

	return false;
}

int PBTurnTimeOut(ENUM_TURN_TIMEOUT_TYPE nType)
{
	int nRet = 5;
	switch(nType)
	{
	case ETTT_BETTING:
		nRet = 5;
#if defined(QA_TEST)
		nRet += 30;
#endif
#if defined(_DEBUG)
		nRet += 30;
#endif
		break;
	case ETTT_CARDCHOICE:
		nRet = 5;
		break;
	case ETTT_CUTTING:
		nRet = 5;
		break;
	}

	nRet += TURN_TIMER_WAIT_TIME/1000;

	return nRet;
}

static bool s_bCreateComboPopup = false;

void PBCreateWhisperlistComboPopup(void)
{
	if(!s_bCreateComboPopup)
	{
		s_bCreateComboPopup = true;
		g_cWhisperlistComboPopup.Create(12, 588);
	}
}

CPokerBaseFunctorRegister::CPokerBaseFunctorRegister()
{
	PBRegistSoundFiles();
	MM().AddMsgH(UPG_CMK_GENERATE_RANDOM_ROOM_NAME, Fnt(PBMakeRoomName));
}

bool PBIsComboPopupClose(const CPoint &ptPos)
{
	return MM().Call(PB_IS_COMBO_POPUP_CLOSE, 0, (LPARAM)(&ptPos))?true:false;
}