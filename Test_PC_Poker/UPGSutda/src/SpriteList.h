// SpriteList.h: interface for the CSpriteList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPRITELIST_H__11E4A548_361F_4AD1_A8F6_F6950C28C8D0__INCLUDED_)
#define AFX_SPRITELIST_H__11E4A548_361F_4AD1_A8F6_F6950C28C8D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace spr
{
	enum _SPRITE
	{
		GSPR_V_H_OPEN= 2,	// 히든 오픈
		GSPR_HANDO   = 4,	// 보유한도
		GSPR_ET_MADE,	//메이드이펙트

		GSPR_ALL
	};
}

extern NMBASE::GRAPHICGDI::xSprite g_sprGLOBAL[ spr::GSPR_ALL ];



extern NMBASE::GRAPHICGDI::xSprite g_sprWinLoser;				// 승자/패자 이펙트

extern NMBASE::GRAPHICGDI::xSprite g_sprChip;					// 칩 

extern NMBASE::GRAPHICGDI::xSprite g_sprBettingEffect;			// 배팅 이펙트

extern NMBASE::GRAPHICGDI::xSprite GradeSpr;					// 게임방 이용등급
extern NMBASE::GRAPHICGDI::xSprite ChanceEffectSpr;				// 찬스 아이템
extern NMBASE::GRAPHICGDI::xSprite ChanceVipEffectSpr;				// 찬스 아이템



extern NMBASE::GRAPHICGDI::xSprite g_sprBigCard;				// 세븐 하이로우 카드



extern NMBASE::GRAPHICGDI::xSprite g_sprJackpotNumber;			// 게임방 잭팟 숫자

extern NMBASE::GRAPHICGDI::xSprite g_sprWinSportlight;			// 현재 턴(승리자에게 보이는 스포트라이트)

extern NMBASE::GRAPHICGDI::xSprite g_sprCloseButton;			// 원샷충전 Close 버튼

extern NMBASE::GRAPHICGDI::xSprite g_SprAutoBettingInfoMsg;

extern NMBASE::GRAPHICGDI::xSprite g_sprPenaltyRed;				// 붉은 숫자 땡값 스프라이트
extern NMBASE::GRAPHICGDI::xSprite g_sprPenaltyBlue;			// 붉은 숫자 땡값 스프라이트

extern NMBASE::GRAPHICGDI::xSprite g_sprTurn;					//섯다 턴

void LoadSpriteFile();

#endif // !defined(AFX_SPRITELIST_H__11E4A548_361F_4AD1_A8F6_F6950C28C8D0__INCLUDED_)
