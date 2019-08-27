#pragma once

//채팅 윈도우 관리자
#include "..\src\ChattingWindowMan\ChattingWindowMan.h"

//베팅칩 관련
#include "..\src\BettingChip\AllBetChip.h"		//전체 칩 표시 및 결과 칩 Animation
#include "..\src\BettingChip\MyBetChip.h"		//자신이 베팅하는 칩Animation

//게임내 버튼 관리자
#include "..\src\GameButtonMan\GameViewButtonMan.h"
//관전자 관리자
#include "..\src\ObserverMan\ObserverMan.h"
//한방충전 관리자
#include "..\src\OneShotCharge\OneShotCharge.h"
//보유한도 관리자
#include "..\src\OwnLimit\OwnLimit.h"

//플레이어 정보 표시 관련 파일들
#include "..\src\Player\PlayerDraw.h"			//각 플레이어의 정보 표시 클래스
#include "..\src\Player\PlayerDrawManager.h"	//전체 플레이어 정보 관리자 클래스
#include "..\src\Player\SprMoneyDraw.h"			//스프라이트를 이용한 머니 표시 클래스

//자리지킴이 관련 클래스
#include "..\src\seatkeeper\SeatKeeper.h"

//사이트팟 관련 클래스
#include "..\src\SidePotMoney\SidePotMoney.h"

//화면 중앙에 총베팅금액 & 콜금액 표시 클래스
#include "..\src\TotalBetMoney\TotalBetMoney.h"

//게임에서 사용되는 각종 좌표, 크기, 색 정보를 가지고 있는 클래스
#include "..\src\UIInfoData.h"

//포커에서 공통을 사용할 함수 및 변수 모음
#include "..\src\GlobalPokerBase.h"

//족보 도우미 관련
#include "..\src\GameHelper\GameHelperMan.h"
#include "..\src\GameHelper\JokboProbability.h"
#include "..\src\GameHelper\OtherUserJokboCheckMan.h"

#include "..\src\shared\KeyDefine.h"
#include "..\src\shared\SharedDefine.h"
#include "..\src\shared\CardDefine.h"

//방만들기 창
#include "..\src\CreateRoom\CreateRoomDlg.h"

//내맘대로 바로입장
#include "..\src\QuickjoinDlg\QuickJoinDlg.h"

//카드 선택창
#include "..\src\SelectCard\SelectCard.h"


#include "..\src\Promotion\PromotionComeBackDlg.h"

//방제목 표시
#include "..\src\RoomTitle\RoomTitle.h"
#include "..\src\BackGround\BackGround.h"

//전광판
#include "..\src\DisplayBoard\DisplayBoard.h"

#include "..\src\BanishMan\BanishMan.h"
#include "..\src\Sound.h"
#include "..\src\Promotion\PromotionManager.h"
#include "..\src\Promotion\PromotionBase.h"
#include "..\src\Promotion\PromotionPokerTime.h"


#include "..\src\VipJackpot\VipJackpot.h"

