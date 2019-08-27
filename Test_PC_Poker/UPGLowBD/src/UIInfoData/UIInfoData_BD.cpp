#include "StdAfx.h"
#include "UIInfoData_BD.h"

namespace ui
{
CUIInfoData_BD::CUIInfoData_BD(void)
{
	Initialize();
}

CUIInfoData_BD::~CUIInfoData_BD(void)
{
	Clear();
}

void CUIInfoData_BD::Clear()
{
	m_mapPos.clear();
	m_mapRect.clear();
	m_mapGab.clear();
	m_mapFloat.clear();
}

void CUIInfoData_BD::Initialize()
{
	Clear();

	//Pos
	m_mapPos.insert( std::make_pair( EPOS_DEALING_TITEL, CPoint( 310, 0 ) )); // 상단 카드 딜링부
	
	// 카드좌표
	m_mapPos.insert( std::make_pair( EPOS_CARD_POS_0, CPoint( 486, 548 ) ));
	m_mapPos.insert( std::make_pair( EPOS_CARD_POS_1, CPoint( 121, 331 ) ));
	m_mapPos.insert( std::make_pair( EPOS_CARD_POS_2, CPoint( 121, 136 ) ));
	m_mapPos.insert( std::make_pair( EPOS_CARD_POS_3, CPoint( 710, 136 ) ));
	m_mapPos.insert( std::make_pair( EPOS_CARD_POS_4, CPoint( 710, 331 ) ));
	// 버튼
	m_mapPos.insert( std::make_pair( EPOS_CHANGE, CPoint( 714, 546 ) ));
	m_mapPos.insert( std::make_pair( EPOS_PASS, CPoint( 849, 546 ) ));

	// 아침, 점심, 저녁
	m_mapPos.insert( std::make_pair( EPOS_CUTINFO, CPoint( 404, 157 ) ));

	// 바꿀 카드를 선택하세요
	m_mapPos.insert( std::make_pair( EPOS_CUTSELECT, CPoint( 351, 276 ) ));

	// 승자 이펙트 텍스트
	m_mapPos.insert( std::make_pair( EPOS_RESULT_0, CPoint( 489, 623 ) ));
	m_mapPos.insert( std::make_pair( EPOS_RESULT_1, CPoint( 124, 405 ) ));
	m_mapPos.insert( std::make_pair( EPOS_RESULT_2, CPoint( 124, 210 ) ));
	m_mapPos.insert( std::make_pair( EPOS_RESULT_3, CPoint( 713, 210 ) ));
	m_mapPos.insert( std::make_pair( EPOS_RESULT_4, CPoint( 713, 405 ) ));
	
	// 커팅숫자 
	m_mapPos.insert( std::make_pair( EPOS_CUTNUM_0, CPoint( 438, 548 ) ));
	m_mapPos.insert( std::make_pair( EPOS_CUTNUM_1, CPoint( 296, 330 ) ));
	m_mapPos.insert( std::make_pair( EPOS_CUTNUM_2, CPoint( 296, 135 ) ));
	m_mapPos.insert( std::make_pair( EPOS_CUTNUM_3, CPoint( 667, 135 ) ));
	m_mapPos.insert( std::make_pair( EPOS_CUTNUM_4, CPoint( 667, 330 ) ));

	// 커팅숫자 Scene Ani P, 1, 2, 3, 4 
	m_mapPos.insert( std::make_pair( EPOS_CUTNUM_SCENE_0, CPoint( 518, 545 ) ));
	m_mapPos.insert( std::make_pair( EPOS_CUTNUM_SCENE_1, CPoint( 223, 345 ) ));
	m_mapPos.insert( std::make_pair( EPOS_CUTNUM_SCENE_2, CPoint( 223, 167 ) ));
	m_mapPos.insert( std::make_pair( EPOS_CUTNUM_SCENE_3, CPoint( 790, 167 ) ));
	m_mapPos.insert( std::make_pair( EPOS_CUTNUM_SCENE_4, CPoint( 790, 345 ) ));

	// 히든오픈키
	m_mapPos.insert( std::make_pair( EPOS_HIDDEN_OPENKEY, CPoint( 487, 632 ) ));
	
	// 시작버튼
	m_mapPos.insert( std::make_pair( EPOS_START_BTN, CPoint( 411, 286 ) ));

	// 자동 올림
	m_mapPos.insert( std::make_pair( EPOS_AUTOUP_BTN, CPoint( 305, 710 ) ));
		
	// 자동 정렬:2012.09.27 YS.Jeong 
	m_mapPos.insert( std::make_pair( EPOS_AUTOSORT_BTN, CPoint( 375, 710 ) ));

	// 딜러좌표
	m_mapPos.insert( std::make_pair( EPOS_DEALER_POS_0, CPoint( 535, 483 ) ));
	m_mapPos.insert( std::make_pair( EPOS_DEALER_POS_1, CPoint( 343, 436 ) ));
	m_mapPos.insert( std::make_pair( EPOS_DEALER_POS_2, CPoint( 343, 243 ) ));
	m_mapPos.insert( std::make_pair( EPOS_DEALER_POS_3, CPoint( 630, 243 ) ));
	m_mapPos.insert( std::make_pair( EPOS_DEALER_POS_4, CPoint( 630, 436 ) ));

	//
	//Rect
	m_mapRect.insert( std::make_pair( ERECT_WININFO, CRect( 0, 0, 164, 14 ) )); // 승,패 이펙트 족보 출력영역

	//
	//Gab
	m_mapGab.insert( std::make_pair( EGAB_CARD_POS_X, 30 ));	    // 카드덱 첫번째, 두번째 간격
	m_mapGab.insert( std::make_pair( EGAB_CARD_POS_OTHER_X, 30 ));  // 카드덱 첫번째, 두번째 간격
	m_mapGab.insert( std::make_pair( EGAB_CUTINFO_X, 64 ));  		// 아침, 점심, 저녁 
	m_mapGab.insert( std::make_pair( EGAB_CUTNUM_Y, 38 ));   		// 컷팅한 숫자 세로간격 컷넘버 시작점의 상대좌표
	m_mapGab.insert( std::make_pair( EGAB_RESULT_X, 0 ));    		// 게임결과 족보영역의 상대좌표 머니
	m_mapGab.insert( std::make_pair( EGAB_RESULT_Y, 19 ));   		// 게임결과 족보영역의 상대좌표 머니

	//
	//Float
	m_mapFloat.insert( std::make_pair( EFLOAT_F4_SPEED, 1.0f ));			 // 처음 4장 카드 이동속도 애니속도 비율
	m_mapFloat.insert( std::make_pair( EFLOAT_F4_TOTAL_CARD_DELAY, 20.0f )); // 처음 4장 카드 이동전 대기시간
	m_mapFloat.insert( std::make_pair( EFLOAT_F4_CARD_DELAY, 5.0f ));   	 // 처음 4장 카드 이동대기( 카드와 카드 )
	m_mapFloat.insert( std::make_pair( EFLOAT_F4_USER_DELAY, 5.0f ));	     // 처음 4장 카드 이동대기( 유저와 유저 )
	m_mapFloat.insert( std::make_pair( EFLOAT_CUT_SPEED, 7.1f ));   		 // 컷팅 카드 이동속도
	m_mapFloat.insert( std::make_pair( EFLOAT_CUT_TOTAL_DELAY, 1.0f ));   	 // 컷팅 카드 이동 전 총 대기시간
	m_mapFloat.insert( std::make_pair( EFLOAT_CUT_CARD_DELAY, 5.0f ));   	 // 컷팅 카드 이동대기( 카드와 카드 )
	m_mapFloat.insert( std::make_pair( EFLOAT_CUT_PASS_DELAY, 15.0f ));   	 // 커팅 패스 이동 전 대기시간
	
	//
	//RGB
	m_mapRGB.insert( std::make_pair( ERGB_BETBTNTEXT, RGB( 164, 201, 88 ) ));	   // 버튼의 베팅 금액 표시 글자색
	m_mapRGB.insert( std::make_pair( ERGB_BETBTNTEXT_SHADOW, RGB( 58, 86, 25 ) )); // 버튼의 베팅 금액 표시 글자의 그림자색

	m_nMaxJokbo = 7;
	m_listJokboName.clear();
	m_listJokboName.push_back("무탑");
	m_listJokboName.push_back("투베이스");
	m_listJokboName.push_back("베이스");
	m_listJokboName.push_back("메이드");
	m_listJokboName.push_back("써드");
	m_listJokboName.push_back("세컨드");
	m_listJokboName.push_back("골프");

	m_listAvatarPos.clear();
	m_listAvatarPos.push_back(CPoint(352, 584));
	m_listAvatarPos.push_back(CPoint(36, 369));
	m_listAvatarPos.push_back(CPoint(36, 174));
	m_listAvatarPos.push_back(CPoint(902, 174));
	m_listAvatarPos.push_back(CPoint(902, 369));

	m_ptJokboHelpPercent = CPoint(20,536);
	m_nJokboHelpHightGab = 28;
	m_rtJokboHelpPercent = CRect(0,0,62,28);

	m_listUserMoneyRect[0] = CRect(487, 668, 487+110, 668+15);

	m_listButtonPos[EBT_SEATKEEPER] = CPoint(444, 710); // 자리지킴이 버튼 위치
	m_listButtonPos[EBT_ONESHOTCHARGE] = CPoint(505, 710); // 원샷충전 버튼 위치
	m_listButtonPos[EBT_CARD_OPEN] = CPoint(551, 710); // OPEN 버튼 위치
	m_listButtonPos[EBT_SINOUT] = CPoint(589, 709); // 관전 버튼 위치
	m_listButtonPos[EBT_BOTTOM_EXIT] = CPoint(628, 709); // 나가기 버튼 위치
	m_listButtonPos[EBT_WAITUSER] = CPoint(668, 710); // 대기 버튼 위치

	m_rtResultJokboYouSize			= CRect(0,0,170,20);
	m_rtResultMoneyYouSize			= CRect(0,0,163,19);

	m_rtResultJokboMySize			= CRect(0,0,170,20);
	m_rtResultMoneyMySize			= CRect(0,0,163,19);

	m_ptGold						= CRect(503, 689, 503+94, 689+11);
}
}