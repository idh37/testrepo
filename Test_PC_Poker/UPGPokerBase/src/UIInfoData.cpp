// SprMoneyDraw.cpp: implementation of the CSprMoneyDraw class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UIInfoData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CUIInfoData::CUIInfoData()
{
	Init();
}

CUIInfoData::~CUIInfoData()
{
}

void CUIInfoData::Init(void)
{
	m_bObserverChat				= false;
	
	m_rtJokoName				= CRect(443,521,443+220,521+14);
	m_clrJokboNameShadowColor	= RGB(24,66,66);
	m_clrJokboNameColor			= RGB(206,198,132);
	
	m_ptBonusCardBack			= CPoint(349, 81);
	m_ptBonusCardGab			= CPoint(5, 4);
	m_ptBonusOpen				= CPoint(460, 222);
	m_ptRuleKind				= CPoint(411, 180);
	m_ptDisCardGab				= CPoint(-31, 77);
	
	m_clrResultJokboNameWinColor	= RGB(183,183,183);
	m_clrResultJokboNameLoseColor	= RGB(127,127,126);
	m_clrResultMoneyWinColor		= RGB(186,150,78);
	m_clrResultMoneyLoseColor		= RGB(148,78,78);
	
	m_rtResultJokboYouSize			= CRect(0,0,213,20);
	m_rtResultMoneyYouSize			= CRect(0,0,207,19);
	m_ptResultBackYouGab			= CPoint(-2, 77);
	
	m_rtResultJokboMySize			= CRect(0,0,220,20);
	m_rtResultMoneyMySize			= CRect(0,0,213,19);
	m_ptResultBackMyGab				= CPoint(-2, 78);
	
	m_ptResultJokboNameGab			= CPoint(2, -7);
	m_ptResultMoneyGab				= CPoint(5, 13);
	
	m_ptTimer						= CPoint(0, 0);
	
	m_ptlistBossMarkGab.push_back(CPoint(0, 72));
	m_ptlistBossMarkGab.push_back(CPoint(0, 72));
	m_ptlistBossMarkGab.push_back(CPoint(0, 72));
	
	m_ptlistDealerMarkGab.push_back(CPoint(0, 82));
	m_ptlistDealerMarkGab.push_back(CPoint(0, 82));
	m_ptlistDealerMarkGab.push_back(CPoint(0, 82));
	
	m_ptlistHostMarkGab.push_back(CPoint(22, -16));
	m_ptlistHostMarkGab.push_back(CPoint(22, -16));
	m_ptlistHostMarkGab.push_back(CPoint(22, -16));
	
	m_listCardStartPos.push_back(CPoint(444, 548));
	m_listCardStartPos.push_back(CPoint(121, 331));
	m_listCardStartPos.push_back(CPoint(121, 136));
	m_listCardStartPos.push_back(CPoint(667, 136));
	m_listCardStartPos.push_back(CPoint(667, 331));
	
	m_listAvatarPos.push_back(CPoint(345, 584));
	m_listAvatarPos.push_back(CPoint(36, 369));
	m_listAvatarPos.push_back(CPoint(36, 174));
	m_listAvatarPos.push_back(CPoint(902, 174));
	m_listAvatarPos.push_back(CPoint(902, 369));
	
	m_szAvatar					= CSize(70, 74);				// 아바타가 잘리는 문제 때문에 Width값을 기존에 +10을 해준다.
	
	m_listUserMoneyRect.push_back(CRect(445, 668, 445+135, 668+15));
	m_listUserMoneyRect.push_back(CRect(115, 449, 115+216, 449+16));
	m_listUserMoneyRect.push_back(CRect(115, 254, 115+216, 254+16));
	m_listUserMoneyRect.push_back(CRect(669, 254, 669+216, 254+16));
	m_listUserMoneyRect.push_back(CRect(669, 449, 669+216, 449+16));
	
	m_nCardGab					= 19;
	m_nMyCardGab				= 20;
	m_ptOverHaveMoneyGab		= CPoint(-18,-23);
	
	m_rtBackTotalMoney			= CRect(401, 413, 401+199, 413+17);
	m_rtBackCallMoney			= CRect(403, 444 , 403+199, 444+14);
	
	m_ptUserNameGab				= CPoint(-10, 81);
	m_rtUserName				= CRect(0, 0, 100, 14);
	m_clrUserNameColor			= RGB(173, 198, 181);
	m_clrUserNameShadowColor	= RGB(24, 16, 8);
	
	m_listLevelGab.push_back(CPoint(0, -16));
	m_listLevelGab.push_back(CPoint(0, -16));
	m_listLevelGab.push_back(CPoint(0, -16));
	
	m_ptReservation				= CPoint(1, 54);
	
	m_rtMyTotalBettingMoney		= CRect(0, 0, 268, 13);
	m_ptMyTotalBettingMoney		= CPoint(716, 532);
	m_clrMyTotalBettingMoneyText = RGB(164, 150, 71);
	m_clrMyTotalBettingMoneyOutline = RGB(0, 0, 0);
	
	m_rtBettingMoney			= CRect(0, 0, 268, 13);
	m_clrBettingMoneyText		= RGB(194, 194, 194);
	m_clrBettingMoneyOutline	= RGB(0, 0, 0);
	
	m_listBettingMoneyPos.push_back(CPoint(716, 519));
	m_listBettingMoneyPos.push_back(CPoint(47, 307));
	m_listBettingMoneyPos.push_back(CPoint(47, 112));
	m_listBettingMoneyPos.push_back(CPoint(665, 112));
	m_listBettingMoneyPos.push_back(CPoint(665, 307));
	
	m_ptJacpotBack				= CPoint(124, 0);
	m_ptJacpotNumberStart		= CPoint(300, 15);
	m_nJacpotNumberWidth		= 8;
	
	//규제안 : 대기하기 상태 이미지 위치
	m_ptWaitAlertPos = CPoint(410, 480);
	
	//포커, 뉴포커, 
	m_listLimitIcon[0].push_back(CPoint(310, 635));
	m_listLimitIcon[0].push_back(CPoint(33, 372));
	m_listLimitIcon[0].push_back(CPoint(33, 176));
	m_listLimitIcon[0].push_back(CPoint(951, 176));
	m_listLimitIcon[0].push_back(CPoint(951, 372));
	
	m_listLimitIcon[1].push_back(CPoint(366, 675));
	m_listLimitIcon[1].push_back(CPoint(69, 364));
	m_listLimitIcon[1].push_back(CPoint(69, 176));
	m_listLimitIcon[1].push_back(CPoint(922, 176));
	m_listLimitIcon[1].push_back(CPoint(922, 364));
	
	m_listButtonPos.push_back(CPoint(714, 546));		//EBT_PING_N
	m_listButtonPos.push_back(CPoint(804, 546));		//EBT_DADANG_N
	m_listButtonPos.push_back(CPoint(894, 546));		//EBT_CHECK_N
	m_listButtonPos.push_back(CPoint(714, 623));		//EBT_HALF_N
	m_listButtonPos.push_back(CPoint(804, 623));		//EBT_CALL_N
	m_listButtonPos.push_back(CPoint(894, 623));		//EBT_DIE_N
	
	m_listButtonPos.push_back(CPoint(714, 584));		//EBT_PING_H
	m_listButtonPos.push_back(CPoint(714, 622));		//EBT_DADANG_H
	m_listButtonPos.push_back(CPoint(849, 584));		//EBT_CHECK_H
	m_listButtonPos.push_back(CPoint(714, 660));		//EBT_HALF_H
	m_listButtonPos.push_back(CPoint(714, 546));		//EBT_CALL_H
	m_listButtonPos.push_back(CPoint(849, 546));		//EBT_DIE_H
	
	m_listButtonPos.push_back(CPoint(849, 584));		//EBT_PING_P
	m_listButtonPos.push_back(CPoint(714, 546));		//EBT_DADANG_P
	m_listButtonPos.push_back(CPoint(849, 546));		//EBT_CHECK_P
	m_listButtonPos.push_back(CPoint(714, 622));		//EBT_HALF_P
	m_listButtonPos.push_back(CPoint(849, 622));		//EBT_CALL_P
	m_listButtonPos.push_back(CPoint(849, 660));		//EBT_DIE_P
	m_listButtonPos.push_back(CPoint(714, 584));		//EBT_QUARTER_P
	
	m_listButtonPos.push_back(CPoint(850, 598));		//EBT_PING_V
	m_listButtonPos.push_back(CPoint(715, 547));		//EBT_DADANG_V
	m_listButtonPos.push_back(CPoint(850, 547));		//EBT_CHECK_V
	m_listButtonPos.push_back(CPoint(715, 649));		//EBT_HALF_V
	m_listButtonPos.push_back(CPoint(850, 649));		//EBT_CALL_V
	m_listButtonPos.push_back(CPoint(715, 598));		//EBT_DIE_V
	
	m_listButtonPos.push_back(CPoint(779, 7));			//EBT_GAME_TIP
	m_listButtonPos.push_back(CPoint(715, 7));			//EBT_CAPTURE
	m_listButtonPos.push_back(CPoint(747, 7));			//EBT_OPTION
	m_listButtonPos.push_back(CPoint(924, 7));			//EBT_MINIMIZE
	m_listButtonPos.push_back(CPoint(948, 7));			//EBT_CHANGEDISPLAY
	m_listButtonPos.push_back(CPoint(972, 7));			//EBT_TOP_EXIT
	m_listButtonPos.push_back(CPoint(835, 7));			//EBT_USER_GRADE
	m_listButtonPos.push_back(CPoint(804, 708));		//EBT_INVITE
	m_listButtonPos.push_back(CPoint(714, 708));		//EBT_OBSERVERVIEW
	m_listButtonPos.push_back(CPoint(538, 710));		//EBT_CARD_OPEN
	m_listButtonPos.push_back(CPoint(576, 709));		//EBT_SINOUT
	m_listButtonPos.push_back(CPoint(616, 709));		//EBT_BOTTOM_EXIT
	m_listButtonPos.push_back(CPoint(157, 697));		//EBT_JOKBOHELP_ON
	m_listButtonPos.push_back(CPoint(214, 697));		//EBT_JOKBOHELP_OFF
	m_listButtonPos.push_back(CPoint(157, 697));		//EBT_JOKBOHELP_USE
	m_listButtonPos.push_back(CPoint(604, 519));		//EBT_JOKBOHELP_INFO
	m_listButtonPos.push_back(CPoint(11, 514));			//EBT_CHAT
	m_listButtonPos.push_back(CPoint(105, 514));		//EBT_JOKBO
	m_listButtonPos.push_back(CPoint(199, 514));		//EBT_EVENT
	m_listButtonPos.push_back(CPoint(253, 711));		//EBT_BADUSER
	m_listButtonPos.push_back(CPoint(90, 716));			//EBT_WHISPERLIST
	m_listButtonPos.push_back(CPoint(396, 378));		//EBT_SIDEPOT
	m_listButtonPos.push_back(CPoint(431, 710));		//EBT_SEATKEEPER 
	m_listButtonPos.push_back(CPoint(492, 710));		//EBT_ONESHOTCHARGE
	m_listButtonPos.push_back(CPoint(894, 708));		//EBT_POKERSHOP
	m_listButtonPos.push_back(CPoint(305, 686));		//EBT_LOSSMONEYWARNING
	m_listButtonPos.push_back(CPoint(529, 686));		//EBT_LOSSMONEYWARNINGCLOSE
	m_listButtonPos.push_back(CPoint(657, 710));		//EBT_WAITUSER
	m_listButtonPos.push_back(CPoint(604, 490));		//EBT_COMPLAIN
	m_listButtonPos.push_back(CPoint(522, 709));		//EBT_GIVEUP
	
	m_ptBtnMoneyDrawRect = CRect(0,0,74,12);
	m_ptBtnMonyDrawGab.push_back(CPoint(-78, -17));		// N Type
	m_ptBtnMonyDrawGab.push_back(CPoint(-73, -17));		// H Type
	m_ptBtnMonyDrawGab.push_back(CPoint(-73, -17));		// P Type
	
	m_rtBetBtnCombo = CRect(870, 521, 870+105, 521+18);	//베팅 버튼 타입 선택 콤보박스의 렉트
	
	m_clrBtnBetText = RGB(132, 214, 247);
	m_clrBtnBetTextShadow = RGB(0, 66, 74);
	m_rtBtnBetText = CRect(0, 0, 82, 17);
	m_rtObserverCount = CRect(0, 0, 16, 20);
	m_clrObserverCount = RGB(187, 171, 145);
	m_ptObserverCountGab = CPoint(54, 1);
	
	m_nMaxJokbo = 12;
	
	m_listJokboName.push_back("탑");
	m_listJokboName.push_back("원페어");
	m_listJokboName.push_back("투페어");
	m_listJokboName.push_back("트리플");
	m_listJokboName.push_back("스트레이트");
	m_listJokboName.push_back("백스트레이트");
	m_listJokboName.push_back("마운틴");
	m_listJokboName.push_back("플러쉬");
	m_listJokboName.push_back("풀하우스");
	m_listJokboName.push_back("포카드");
	m_listJokboName.push_back("스티플");
	m_listJokboName.push_back("로티플");

	m_rtJokboHelpPercent				= CRect(0,0,68,16);
	m_ptJokboHelpPercent				= CPoint(17,542);
	m_nJokboHelpHightGab				= 16;
	m_ptJokboHelpCompleteGab			= CPoint(-4, -2);
	m_clrJokboHelpNoPossibilityName		= RGB(49,57,57);
	m_clrJokboHelpPercent				= RGB(148,140,99);
	m_clrJokboHelpPossibilityName		= RGB(49,107,115);
	m_clrJokboHelpMyJokboName			= RGB(165,148,66);
	m_ptJokboHelpPercentGab				= CPoint(46,0);
	m_ptJokboHelpBack					= CPoint(614, 540);
	m_ptJokboHelpUseDlg					= CPoint(694, 441);
	m_ptJokboHelpUseDlgUseBtnGab		= CPoint(62, 43);
	m_ptJokboHelpUseDlgCancelBtnGab		= CPoint(126, 43);
	m_ptJokboHelpUseDlgOK				= CPoint(94, 43);
	m_ptJokboArrowGab					= CPoint(6, -7);
	m_ptJokboInfo						= CPoint(148,574);

	m_listUserChangeGameInfoBackPos.push_back(CPoint(-31, 96));
	m_listUserChangeGameInfoBackPos.push_back(CPoint(55, 85));
	m_listUserChangeGameInfoBackPos.push_back(CPoint(-123, 85));

	m_rtUserChangeGameInfoMoney			= CRect(10, 4, 10+114, 4+13);
	m_rtUserChangeGameInfoInfo			= CRect(10, 21, 10+114, 21+13);
	m_rtUserChangeGameInfoLimit			= CRect(10, 38, 10+114, 38+13);
	m_clrUserChangeGameInfoMoneyWin		= RGB(121,167,203);
	m_clrUserChangeGameInfoMoneyLose	= RGB(215,113,73);
	m_clrUserChangeGameInfoInfo			= RGB(255,247,247);
	m_clrUserChangeGameInfoLimitMoney	= RGB(255,249,119);

	m_ptCardDeck						= CPoint(441, 71);
	m_rtRoomTitle						= CRect(0, 0, 254, 15);
	m_ptRoomTitle						= CPoint(370, 39);
	m_clrRoomTitle						= RGB(156, 157, 81);
	m_clrRoomTitleOutLine				= RGB(0, 0, 0);
	

	m_listChatColor.push_back(RGB(143, 141, 102));		//ECC2_NOTIFY
	m_listChatColor.push_back(RGB(109, 160, 168));		//ECC2_USEITEM
	m_listChatColor.push_back(RGB(146, 146, 146));		//ECC2_CHAT
	m_listChatColor.push_back(RGB(199, 117, 116));		//ECC2_SERVMSG
	m_listChatColor.push_back(RGB(146, 140,  89));		//ECC2_WHISPER_SEND
	m_listChatColor.push_back(RGB( 78, 147,  54));		//ECC2_WHISPER_RECEIVE
	m_listChatColor.push_back(RGB(212, 211, 126));		//ECC2_RESULT
	m_listChatColor.push_back(RGB(212, 211, 126));		//ECC2_RESULT_WINNER
	m_listChatColor.push_back(RGB(255, 180,   0));		//ECC2_RESULT_WINMONEY
	m_listChatColor.push_back(RGB(255, 222,   1));		//ECC2_RESULT_JOKBO
	m_listChatColor.push_back(RGB(156, 156, 156));		//ECC2_RESULT_LOSER
	m_listChatColor.push_back(RGB(156, 156, 156));		//ECC2_RESULT_LOSEMONEY
	m_listChatColor.push_back(RGB(199, 117, 116));		//ECC2_RESULT_JACKPOT

	m_ptInfo							= CPoint(15, 531);
	m_szInfo							= CSize(296, 105);//CSize(296, 155);
	m_ptChat							= CPoint(15, 631);
	m_szChat							= CSize(296, 75);
	m_ptEvent							= CPoint(12, 537);
	m_ptEdit							= CPoint(20, 716);//귓말 삭제 CPoint(110, 715);//CPoint(18, 713);
	m_szEdit							= CSize(230, 12);//귓말 삭제 CSize(140, 12);//CSize(210, 12);
	m_ptInfoScroll						= CPoint(274, 540);
	m_nInfoScrollHeight					= 98;
	m_ptChatScroll						= CPoint(274, 638);//CPoint(274, 540);
	m_nChatScrollHeight					= 73;				//160;
	m_ptEventPage						= CPoint(269, 4);
	m_nEventPageGab						= 13;

	m_listPlayerBackPos.push_back(CPoint(304,512));
	m_listPlayerBackPos.push_back(CPoint(16,319));
	m_listPlayerBackPos.push_back(CPoint(16,126));
	m_listPlayerBackPos.push_back(CPoint(663,126));
	m_listPlayerBackPos.push_back(CPoint(663,319));

	//바닥칩의 위치를 가지는 리스트
	m_ptBackChipCenter = CPoint(498, 283);

	std::vector<CPoint> listPos;

	//1개일때
	listPos.push_back(CPoint(478-m_ptBackChipCenter.x, 264-m_ptBackChipCenter.y));
	m_listBackChipGabPos.push_back(listPos);
	listPos.clear();

	//2개일때
	listPos.push_back(CPoint(457-m_ptBackChipCenter.x, 264-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(500-m_ptBackChipCenter.x, 264-m_ptBackChipCenter.y));
	m_listBackChipGabPos.push_back(listPos);
	listPos.clear();

	//3개일때
	listPos.push_back(CPoint(478-m_ptBackChipCenter.x, 245-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(457-m_ptBackChipCenter.x, 280-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(500-m_ptBackChipCenter.x, 280-m_ptBackChipCenter.y));
	m_listBackChipGabPos.push_back(listPos);
	listPos.clear();

	//4개일때
	listPos.push_back(CPoint(447-m_ptBackChipCenter.x, 245-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(490-m_ptBackChipCenter.x, 245-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(469-m_ptBackChipCenter.x, 280-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(521-m_ptBackChipCenter.x, 280-m_ptBackChipCenter.y));
	m_listBackChipGabPos.push_back(listPos);
	listPos.clear();

	//5개일때
	listPos.push_back(CPoint(435-m_ptBackChipCenter.x, 245-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(478-m_ptBackChipCenter.x, 245-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(521-m_ptBackChipCenter.x, 245-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(457-m_ptBackChipCenter.x, 280-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(500-m_ptBackChipCenter.x, 280-m_ptBackChipCenter.y));
	m_listBackChipGabPos.push_back(listPos);
	listPos.clear();

	//6개일때
	listPos.push_back(CPoint(427-m_ptBackChipCenter.x, 245-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(470-m_ptBackChipCenter.x, 245-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(513-m_ptBackChipCenter.x, 245-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(449-m_ptBackChipCenter.x, 280-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(492-m_ptBackChipCenter.x, 280-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(535-m_ptBackChipCenter.x, 280-m_ptBackChipCenter.y));
	m_listBackChipGabPos.push_back(listPos);
	listPos.clear();

	//7개일때
	listPos.push_back(CPoint(457-m_ptBackChipCenter.x, 229-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(500-m_ptBackChipCenter.x, 229-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(435-m_ptBackChipCenter.x, 264-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(478-m_ptBackChipCenter.x, 264-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(521-m_ptBackChipCenter.x, 264-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(457-m_ptBackChipCenter.x, 299-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(500-m_ptBackChipCenter.x, 299-m_ptBackChipCenter.y));
	m_listBackChipGabPos.push_back(listPos);
	listPos.clear();

	//8개일때
	listPos.push_back(CPoint(436-m_ptBackChipCenter.x, 229-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(479-m_ptBackChipCenter.x, 229-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(414-m_ptBackChipCenter.x, 264-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(457-m_ptBackChipCenter.x, 264-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(500-m_ptBackChipCenter.x, 264-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(543-m_ptBackChipCenter.x, 264-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(479-m_ptBackChipCenter.x, 299-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(522-m_ptBackChipCenter.x, 299-m_ptBackChipCenter.y));
	m_listBackChipGabPos.push_back(listPos);
	listPos.clear();

	//9개일때
	listPos.push_back(CPoint(414-m_ptBackChipCenter.x, 229-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(457-m_ptBackChipCenter.x, 229-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(500-m_ptBackChipCenter.x, 229-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(435-m_ptBackChipCenter.x, 264-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(478-m_ptBackChipCenter.x, 264-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(521-m_ptBackChipCenter.x, 264-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(457-m_ptBackChipCenter.x, 299-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(500-m_ptBackChipCenter.x, 299-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(543-m_ptBackChipCenter.x, 299-m_ptBackChipCenter.y));
	m_listBackChipGabPos.push_back(listPos);
	listPos.clear();

	//10개일때
	listPos.push_back(CPoint(436-m_ptBackChipCenter.x, 229-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(479-m_ptBackChipCenter.x, 229-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(522-m_ptBackChipCenter.x, 229-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(414-m_ptBackChipCenter.x, 264-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(457-m_ptBackChipCenter.x, 264-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(500-m_ptBackChipCenter.x, 264-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(543-m_ptBackChipCenter.x, 264-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(436-m_ptBackChipCenter.x, 299-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(479-m_ptBackChipCenter.x, 299-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(522-m_ptBackChipCenter.x, 299-m_ptBackChipCenter.y));
	m_listBackChipGabPos.push_back(listPos);
	listPos.clear();

	//11개일때
	listPos.push_back(CPoint(414-m_ptBackChipCenter.x, 229-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(457-m_ptBackChipCenter.x, 229-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(500-m_ptBackChipCenter.x, 229-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(392-m_ptBackChipCenter.x, 264-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(435-m_ptBackChipCenter.x, 264-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(478-m_ptBackChipCenter.x, 264-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(521-m_ptBackChipCenter.x, 264-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(564-m_ptBackChipCenter.x, 264-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(457-m_ptBackChipCenter.x, 299-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(500-m_ptBackChipCenter.x, 299-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(543-m_ptBackChipCenter.x, 299-m_ptBackChipCenter.y));
	m_listBackChipGabPos.push_back(listPos);
	listPos.clear();

	//12개일때
	listPos.push_back(CPoint(393-m_ptBackChipCenter.x, 229-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(436-m_ptBackChipCenter.x, 229-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(479-m_ptBackChipCenter.x, 229-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(522-m_ptBackChipCenter.x, 229-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(414-m_ptBackChipCenter.x, 264-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(457-m_ptBackChipCenter.x, 264-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(500-m_ptBackChipCenter.x, 264-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(543-m_ptBackChipCenter.x, 264-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(436-m_ptBackChipCenter.x, 299-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(479-m_ptBackChipCenter.x, 299-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(522-m_ptBackChipCenter.x, 299-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(565-m_ptBackChipCenter.x, 299-m_ptBackChipCenter.y));
	m_listBackChipGabPos.push_back(listPos);
	listPos.clear();

	//13개일때
	listPos.push_back(CPoint(414-m_ptBackChipCenter.x, 229-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(457-m_ptBackChipCenter.x, 229-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(500-m_ptBackChipCenter.x, 229-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(543-m_ptBackChipCenter.x, 229-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(392-m_ptBackChipCenter.x, 264-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(435-m_ptBackChipCenter.x, 264-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(478-m_ptBackChipCenter.x, 264-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(521-m_ptBackChipCenter.x, 264-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(564-m_ptBackChipCenter.x, 264-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(414-m_ptBackChipCenter.x, 299-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(457-m_ptBackChipCenter.x, 299-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(500-m_ptBackChipCenter.x, 299-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(543-m_ptBackChipCenter.x, 299-m_ptBackChipCenter.y));
	m_listBackChipGabPos.push_back(listPos);
	listPos.clear();

	//14개일때
	listPos.push_back(CPoint(393-m_ptBackChipCenter.x, 229-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(436-m_ptBackChipCenter.x, 229-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(479-m_ptBackChipCenter.x, 229-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(522-m_ptBackChipCenter.x, 229-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(565-m_ptBackChipCenter.x, 229-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(414-m_ptBackChipCenter.x, 264-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(457-m_ptBackChipCenter.x, 264-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(500-m_ptBackChipCenter.x, 264-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(543-m_ptBackChipCenter.x, 264-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(393-m_ptBackChipCenter.x, 299-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(436-m_ptBackChipCenter.x, 299-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(479-m_ptBackChipCenter.x, 299-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(522-m_ptBackChipCenter.x, 299-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(565-m_ptBackChipCenter.x, 299-m_ptBackChipCenter.y));
	m_listBackChipGabPos.push_back(listPos);
	listPos.clear();

	//15개일때
	listPos.push_back(CPoint(371-m_ptBackChipCenter.x, 229-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(414-m_ptBackChipCenter.x, 229-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(457-m_ptBackChipCenter.x, 229-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(500-m_ptBackChipCenter.x, 229-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(543-m_ptBackChipCenter.x, 229-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(392-m_ptBackChipCenter.x, 264-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(435-m_ptBackChipCenter.x, 264-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(478-m_ptBackChipCenter.x, 264-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(521-m_ptBackChipCenter.x, 264-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(564-m_ptBackChipCenter.x, 264-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(414-m_ptBackChipCenter.x, 299-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(457-m_ptBackChipCenter.x, 299-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(500-m_ptBackChipCenter.x, 299-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(543-m_ptBackChipCenter.x, 299-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(586-m_ptBackChipCenter.x, 299-m_ptBackChipCenter.y));
	m_listBackChipGabPos.push_back(listPos);
	listPos.clear();

	//16개일때
	listPos.push_back(CPoint(414-m_ptBackChipCenter.x, 214-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(457-m_ptBackChipCenter.x, 214-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(500-m_ptBackChipCenter.x, 214-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(543-m_ptBackChipCenter.x, 214-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(392-m_ptBackChipCenter.x, 249-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(435-m_ptBackChipCenter.x, 249-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(478-m_ptBackChipCenter.x, 249-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(521-m_ptBackChipCenter.x, 249-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(564-m_ptBackChipCenter.x, 249-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(414-m_ptBackChipCenter.x, 284-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(457-m_ptBackChipCenter.x, 284-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(500-m_ptBackChipCenter.x, 284-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(543-m_ptBackChipCenter.x, 284-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(435-m_ptBackChipCenter.x, 319-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(478-m_ptBackChipCenter.x, 319-m_ptBackChipCenter.y));
	listPos.push_back(CPoint(521-m_ptBackChipCenter.x, 319-m_ptBackChipCenter.y));
	m_listBackChipGabPos.push_back(listPos);
	listPos.clear();


	m_ptChipEndGab						= CPoint(-373, -201);
	m_dwChipMoveDelayTime				= 500;

	m_rtDisplayBoard					= CRect(0, 0, 355, 12);
	m_ptDisplayBoard					= CPoint(319, 10);
	m_clrDisplayNormal					= RGB(57,103,108);
	m_clrDisplayJokbo					= RGB(161,159,159);
	m_clrDisplayMoney					= RGB(155,156,81);

	m_ptBettingCmd						= CPoint(0,0);

	m_rtSidePotMoneyName				= CRect(0,0,50,15);
	m_ptSidePotMoneyNameGab				= CPoint(3, 0);
	m_clrSidePotMoneyName				= RGB(0,0,0);
	m_rtSidePotMoney					= CRect(0,0,160,15);
	m_ptSidePotMoneyGab					= CPoint(37, 0);
	m_clrSidePotMoney					= RGB(0,0,0);
	m_ptSidePotResult					= CPoint(712, 544);
	m_rtSidePotResultVS					= CRect(0, 0, 52, 18);
	m_ptSidePotResultVSGab				= CPoint(71, 29);
	m_nSidePotResultGab					= 19;
	m_rtSidePotResultMoney				= CRect(0, 0, 94, 18);
	m_ptSidePotResultMoneyGab			= CPoint(124, 29);
	m_rtSidePotResultTotalMoney			= CRect(0, 0, 120, 15);
	m_ptSidePotResultTotalMoneyGab		= CPoint(99, 136);

	m_rtHighJokboUserName				= CRect(0,0,252,32);
	m_ptHighJokboUserName				= CPoint(376, 328);
	m_clrHighJokboUserName				= RGB(57,103,108);
	m_rtHighJokboName					= CRect(0,0,252,32);
	m_ptHighJokboName					= CPoint(376, 344);
	m_clrHighJokboName					= RGB(161,159,159);
	m_rtHighJokboWinMoney				= CRect(0,0,246,20);
	m_ptHighJokboWinMoney				= CPoint(379,370);
	m_clrHighJokboWinMoney				= RGB(255,204,102);
	m_rtHighJokboJacpotMoney			= CRect(0,0,256,17);
	m_ptHighJokboJacpotMoney			= CPoint(374,411);
	m_clrHighJokboJacpotMoney			= RGB(255,222,0);

	m_ptObserver						= CPoint(714, 521);
	m_ptObserverBtnGab					= CPoint(58, 102);
	m_ptObserverReservePriorityGab		= CPoint(150, 79);

	// 아바타를 기준으로한 상대좌표
	m_listSeatKeeperWinPos.push_back(CPoint(99, -48));
	m_listSeatKeeperWinPos.push_back(CPoint(115, -22));
	m_listSeatKeeperWinPos.push_back(CPoint(115, -24));
	m_listSeatKeeperWinPos.push_back(CPoint(-205, -24));
	m_listSeatKeeperWinPos.push_back(CPoint(-205, -22));

	//자리지킴이 윈도우를 기준으로한 상대좌표
	m_listTurnDisplayPos.push_back(CPoint(13, 34));			
	m_listTurnDisplayPos.push_back(CPoint(38, 34));			
	m_listTurnDisplayPos.push_back(CPoint(63, 34));			
	m_listTurnDisplayPos.push_back(CPoint(88, 34));			
	m_listTurnDisplayPos.push_back(CPoint(113, 34));	

	m_TurnTextPos						= CPoint(72, 60);
	m_SeatKeeperJoinButPos				= CPoint(18, 122);
	m_rtSeatKeeperWeb					= CRect(153, 2, 522, 169);
	m_clrSeatKeeperTurn					= RGB(144, 144, 144);

	m_dwBombCardOpenMoveTime			= 200;
	m_ptBombEffectGab					= CPoint(-50, -30);
	m_ptJokerEffectGab					= CPoint(0,0);
	m_dwBombEffectGab					= 10;
	m_dwStartGameDealingTimeGab			= 180;
	m_fStartGameDealingCardSpeed		= 1.0f;
	m_dwDealingTimeGab					= 220;
	m_fDealingCardSpeed					= 1.0f;


	m_listSameIPGab.push_back(CPoint(62, 0));
	m_listSameIPGab.push_back(CPoint(62, 0));
	m_listSameIPGab.push_back(CPoint(-13, 0));

	m_listSameIPMarkRight.push_back(true);
	m_listSameIPMarkRight.push_back(true);
	m_listSameIPMarkRight.push_back(false);

	m_ptAutoBettingPos					= CPoint(352, 272);

	m_ptOneShotDlg						= CPoint(207, 537);
	m_ptOneShotDlg_btnOnceGab			= CPoint(16, 37);
	m_ptOneShotDlg_btnTwiceGab			= CPoint(120, 37);
	m_ptOneShotDlg_btnCloseGab			= CPoint(166,112);

	m_rtOneShotPossibilityMoneySize		= CRect(0,0,68,13);
	m_ptOneShotPossibilityMoneyGab		= CPoint(154, 10);
	m_clrPossibilityMoneyOutline		= RGB(0,0,0);
	m_clrPossibilityMoney				= RGB(193, 188, 96);
	m_rtOneShotOnceTextSize				= CRect(0,0,92,20);
	m_ptOneShotOnceTextGab				= CPoint(19, 40);
	m_rtOneShotTwiceTextSize			= CRect(0,0,92,20);
	m_ptOneShotTwiceTextGab				= CPoint(123, 40);
	m_clrTextOutline					= RGB(141,175,37);
	m_clrText							= RGB(59,78,0);
	m_rtOneShotError					= CRect(0,0,122,42);
	m_ptOneShotErrorGab					= CPoint(96, 52);
	m_clrResultOutline					= RGB(1, 0, 0);
	m_clrResultNormal					= RGB(153, 153, 153);
	m_clrResultEmphasis					= RGB(252, 212, 63);
	m_rtMsgSize							= CRect(0,0,122,12);
	m_ptSuccessMoneyGab					= CPoint(96, 56);
	m_nLineGab							= 16;
	m_ptReservationGab					= CPoint(96, 48);

	// 보유한도 초과 영역과 규제안 제한문구 영역이 겹쳐서 수정.
	/*m_ptOwnLimitMarkGab					= CPoint(-5, 97);*/
	m_ptOwnLimitMarkGab					= CPoint(-5, -35);
	// 보유한도 초과 툴팁에 골드 내용도 추가 되어 수정.
	m_ptOwnLimitTipGab					= CPoint(-37, -70);
	m_rtOwnLimitMoneySize				= CRect(0, 0, 136, 15);
	m_ptOwnLimitMoneyGab				= CPoint(6, 24);
	m_ptOwnLimitGoldGab					= CPoint(0, 15);
	m_clrOwnLimitMoney					= RGB(255,255,255);

	m_ptChipStartGab					= CPoint(106,58);

	m_ptSelectCardFirstCard_Choice		= CPoint(315, 261);
	m_ptSelectCardFirstCard_Normal		= CPoint(359, 261);
	m_rtSelectCardFirstCard_Choice		= CRect(12, 47, 93, 165);
	m_rtSelectCardFirstCard_Normal		= CRect(12, 47, 93, 165);
	m_nSelectCardCardGab				= 89;
	m_ptSlectCardSelectMarkGab			= CPoint(-1, -1);


	// 내맘대로 바로입장 
	m_ptQuickJoinDlgSearchOFF			= CPoint(36, 69);			// 내맘대로 바로입장 사용/ 사용안함
	m_ptQuickJoinDlgSearchON			= CPoint(126, 69);			

	/*--- 설정버튼 ---*/
	// 7POKER
	//규제안 관련하여 버튼 위치 수정
	SQUICKJOINAPPLYBUT sQJABut;
	sQJABut.ptButton[0] = CPoint(81, 379);
	sQJABut.ptButton[1] = CPoint(171, 379);
	m_listQuickJoinDlgApplayBut.push_back(sQJABut);
	// LOWBD
	sQJABut.ptButton[0] = CPoint(81, 219);
	sQJABut.ptButton[1] = CPoint(171, 219);
	m_listQuickJoinDlgApplayBut.push_back(sQJABut);
	// NEWPOKER
	sQJABut.ptButton[0] = CPoint(81, 274);
	sQJABut.ptButton[1] = CPoint(171, 274);
	m_listQuickJoinDlgApplayBut.push_back(sQJABut);
	// SUTDA	
	sQJABut.ptButton[0] = CPoint(81, 274);
	sQJABut.ptButton[1] = CPoint(171, 274);
	m_listQuickJoinDlgApplayBut.push_back(sQJABut);
	// HOOLA -> 제정의 필요	
	sQJABut.ptButton[0] = CPoint(81, 330);
	sQJABut.ptButton[1] = CPoint(171, 330);
	m_listQuickJoinDlgApplayBut.push_back(sQJABut);

	m_listPokerTimeEffectPos.push_back(CPoint(452, 661));
	m_listPokerTimeEffectPos.push_back(CPoint(137, 444));
	m_listPokerTimeEffectPos.push_back(CPoint(137, 250));
	m_listPokerTimeEffectPos.push_back(CPoint(664, 251));
	m_listPokerTimeEffectPos.push_back(CPoint(664, 446));

	m_rtPokerTimeEffect = CRect(0,0,145,12);

	m_listRoomInfoMinimumMoney[IDX_GAME_SP] = CRect(644, 69, 644+150, 69+15);
	m_listRoomInfoMinimumMoney[IDX_GAME_NP] = CRect(644, 69, 644+150, 69+15);
	m_listRoomInfoMinimumMoney[IDX_GAME_BD] = CRect(653, 69, 653+106, 69+15);
	m_listRoomInfoMinimumMoney[IDX_GAME_HL] = CRect(644, 69, 644+150, 69+15);
	m_listRoomInfoMinimumMoney[IDX_GAME_SD] = CRect(644, 69, 644+150, 69+15);
	m_listRoomInfoMinimumMoney[IDX_GAME_HA] = CRect(692, 81, 692+178, 81+15);

	m_listRoomInfoRule[IDX_GAME_SP] = CRect(644, 89, 644+150, 89+15);
	m_listRoomInfoRule[IDX_GAME_NP] = CRect(644, 89, 644+150, 89+15);
	m_listRoomInfoRule[IDX_GAME_BD] = CRect(653, 89, 653+106, 89+15);
	m_listRoomInfoRule[IDX_GAME_HL] = CRect(644, 89, 644+150, 89+15);
	m_listRoomInfoRule[IDX_GAME_SD] = CRect(644, 89, 644+150, 89+15);
	m_listRoomInfoRule[IDX_GAME_HA] = CRect(682, 101, 682+198, 101+15);//CRect(692, 101, 692+178, 101+15);

	m_listRoomInfoRuleColor[IDX_GAME_SP] = RGB( 98,136,164);
	m_listRoomInfoRuleColor[IDX_GAME_NP] = RGB( 98,136,164);
	m_listRoomInfoRuleColor[IDX_GAME_BD] = RGB(132, 90, 66);
	m_listRoomInfoRuleColor[IDX_GAME_HL] = RGB(240,189, 189);
	m_listRoomInfoRuleColor[IDX_GAME_SD] = RGB(160,148, 102);
	m_listRoomInfoRuleColor[IDX_GAME_HA] = RGB( 98,136,164);

	m_rtRoomInfoPokerKind			= CRect(652, 79, 652+36, 79+15);
	m_clrRoomInfoOutline			= RGB(42,42,42);
	m_clrRoomInfoMinimumMoney		= RGB(173,165,42);

	m_ptGold						= CRect(462, 689, 462+116, 689+12);
	m_clrGold						= RGB(148,142,79);
	m_clrGoldDiff					= RGB(255,219,95);
	m_clrGoldReward					= RGB(167,142,37);

	//테스트 코드
	//DWORD dwSize = 0;
	//char strTemp[1024];
	//dwSize = GetPrivateProfileString("TEST", "Start Dealing Gab","150",strTemp,1024,".\\test.ini");
	//if(dwSize)
	//{
	//	m_dwStartGameDealingTimeGab = (DWORD)atoi(strTemp);
	//}

	//dwSize = GetPrivateProfileString("TEST", "Start Card Speed","1.3",strTemp,1024,".\\test.ini");
	//if(dwSize)
	//{
	//	m_fStartGameDealingCardSpeed = (float)atof(strTemp);
	//}

	//dwSize = GetPrivateProfileString("TEST", "Dealing Gab","200",strTemp,1024,".\\test.ini");
	//if(dwSize)
	//{
	//	m_dwDealingTimeGab = (DWORD)atoi(strTemp);
	//}

	//dwSize = GetPrivateProfileString("TEST", "Card Speed","1.0",strTemp,1024,".\\test.ini");
	//if(dwSize)
	//{
	//	m_fDealingCardSpeed = (float)atof(strTemp);
	//}
}

const char *CUIInfoData::GetJokboName(int nKind)
{
	ASSERT(nKind >= 0 && nKind < m_nMaxJokbo);

	if(nKind < 0 || nKind >= m_nMaxJokbo)
		return NULL;

	return m_listJokboName[nKind].c_str();
}