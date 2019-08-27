#pragma once

namespace spr
{	
	enum _SPRITE
	{
		GSPR_V_H_OPEN,	// 히든 오픈
		GSPR_ET_MADE,	//메이드이펙트		

		GSPR_ALL
		
	};
}

extern NMBASE::GRAPHICGDI::xSprite GLOBAL_SPR[ spr::GSPR_ALL ];

extern NMBASE::GRAPHICGDI::xSprite g_sprResultNumber;		// 결과 숫자

extern NMBASE::GRAPHICGDI::xSprite g_sprWinLoseBack;		// 승패결과 표시 배경

extern NMBASE::GRAPHICGDI::xSprite g_sprJBTable;			// 족보 테이블
// 단축키

extern NMBASE::GRAPHICGDI::xSprite g_sprBombSetAni;		//폭탄 효과
extern NMBASE::GRAPHICGDI::xSprite g_sprTabBtn;			// 채팅,족보 버튼

extern NMBASE::GRAPHICGDI::xSprite g_sprJokboEffect;				//로티플
extern NMBASE::GRAPHICGDI::xSprite g_sprBodyItem;
extern NMBASE::GRAPHICGDI::xSprite g_sprAllInEft;				// 올인 관련 효과
extern NMBASE::GRAPHICGDI::xSprite g_sprBetCmd;				// 베팅커멘드 

extern NMBASE::GRAPHICGDI::xSprite g_sprGrade;
extern NMBASE::GRAPHICGDI::xSprite g_sprAutoBettingSmall;		//설정된 베팅룰 보기
extern NMBASE::GRAPHICGDI::xSprite g_sprAutoBetting;			//자동베팅 설정 배경
extern NMBASE::GRAPHICGDI::xSprite g_sprAutoBetSetUp;
extern NMBASE::GRAPHICGDI::xSprite g_sprAutoBettingInfoMsg;

extern NMBASE::GRAPHICGDI::xSprite g_sprChangeRoomInfoMsg;	//방정보 변경 예약 알림 메세지

/////////////////////////////////////////////////////////////////////////
//카드 부분
extern NMBASE::GRAPHICGDI::xSprite g_sprBigCard;		// 카드
extern NMBASE::GRAPHICGDI::xSprite g_sprDarkBigCard;	// 카드
extern NMBASE::GRAPHICGDI::xSprite g_sprBigBombCard;	// 카드
extern NMBASE::GRAPHICGDI::xSprite g_sprSmallBombCard;	// 카드
extern NMBASE::GRAPHICGDI::xSprite g_sprDisCard;		//버린카드

extern NMBASE::GRAPHICGDI::xSprite g_sprLastCardOpen;			//히든카드오픈

extern NMBASE::GRAPHICGDI::xSprite g_sprJokerCardEffect;
extern NMBASE::GRAPHICGDI::xSprite g_sprCardDealing;		//카드 딜링 스프라이트
extern NMBASE::GRAPHICGDI::xSprite g_sprBombCardDealing;	//폭탄카드 딜링 스프라이트

extern NMBASE::GRAPHICGDI::xSprite g_sprSDLG_ProFlie;			// 프로필

//뉴도신지수 2006.3.2
extern NMBASE::GRAPHICGDI::xSprite g_sprAllinChargeInfo;

extern NMBASE::GRAPHICGDI::xSprite g_sprJackpoText;			// 잭팟 지급 애니
extern NMBASE::GRAPHICGDI::xSprite g_sprJackPotBackEffect;	// 잭팟 지급  애니 
extern NMBASE::GRAPHICGDI::xSprite g_sprJackPot;				// 게임방 잭팟
extern NMBASE::GRAPHICGDI::xSprite g_sprChip;					// 칩
extern NMBASE::GRAPHICGDI::xSprite g_sprClose;

extern NMBASE::GRAPHICGDI::xSprite g_ChanceEffectSpr;		//찬스아바타 이펙트