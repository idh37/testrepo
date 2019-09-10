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
		GSPR_HANDO   = 0,	// �����ѵ�
		GSPR_ALL
	};
}

extern NMBASE::GRAPHICGDI::xSprite g_sprGLOBAL[ spr::GSPR_ALL ];

extern NMBASE::GRAPHICGDI::xSprite g_sprUITopBtn;				// ���� ��ư

extern NMBASE::GRAPHICGDI::xSprite GradeSpr;					// ���ӹ� �̿���

extern NMBASE::GRAPHICGDI::xSprite ChanceEffectSpr;			// ���� ������

extern NMBASE::GRAPHICGDI::xSprite g_sprJackpotNumber;			// ���ӹ� ���� ����

extern NMBASE::GRAPHICGDI::xSprite g_sprCloseButton;			// �������� Close ��ư

extern NMBASE::GRAPHICGDI::xSprite g_sprWinLoser;


//�Ƕ� ��������Ʈ
extern NMBASE::GRAPHICGDI::xSprite g_sprRegOk;
extern NMBASE::GRAPHICGDI::xSprite g_sprNoThankBtn;
extern NMBASE::GRAPHICGDI::xSprite g_sprThankTimer;
extern NMBASE::GRAPHICGDI::xSprite g_sprTimer;
extern NMBASE::GRAPHICGDI::xSprite g_sprBigCard;
extern NMBASE::GRAPHICGDI::xSprite g_sprGameBackPart;
extern NMBASE::GRAPHICGDI::xSprite g_sprJackPotInfo;
extern NMBASE::GRAPHICGDI::xSprite g_sprJackPotAni;
extern NMBASE::GRAPHICGDI::xSprite g_sprMission;
extern NMBASE::GRAPHICGDI::xSprite g_sprMissionComp;
extern NMBASE::GRAPHICGDI::xSprite g_sprGold;
extern NMBASE::GRAPHICGDI::xSprite g_sprEffect;
extern NMBASE::GRAPHICGDI::xSprite g_sprCardSelect;
extern NMBASE::GRAPHICGDI::xSprite g_sprEffectBak;
extern NMBASE::GRAPHICGDI::xSprite g_sprSelectCard;
extern NMBASE::GRAPHICGDI::xSprite g_sprHoola;
extern NMBASE::GRAPHICGDI::xSprite g_sprMagic;
extern NMBASE::GRAPHICGDI::xSprite g_sprCardNum;
extern NMBASE::GRAPHICGDI::xSprite g_sprMyCard;
extern NMBASE::GRAPHICGDI::xSprite g_sprFocus;
extern NMBASE::GRAPHICGDI::xSprite g_sprSmallCard;
extern NMBASE::GRAPHICGDI::xSprite g_sprCardCount;
extern NMBASE::GRAPHICGDI::xSprite g_sprAllin;
extern NMBASE::GRAPHICGDI::xSprite g_sprResult;
extern NMBASE::GRAPHICGDI::xSprite g_sprWaiting;
extern NMBASE::GRAPHICGDI::xSprite g_sprSignBoard;
extern NMBASE::GRAPHICGDI::xSprite g_sprSunPickup;
extern NMBASE::GRAPHICGDI::xSprite g_sprIntro;
extern NMBASE::GRAPHICGDI::xSprite g_sprPracticsAvatar;
extern NMBASE::GRAPHICGDI::xSprite g_SprCardShortcut;
extern NMBASE::GRAPHICGDI::xSprite g_SprTurnEffect;
extern NMBASE::GRAPHICGDI::xSprite g_SprResultOK;
//yoo 2015.01 ������
extern NMBASE::GRAPHICGDI::xSprite g_sprLimitIcon;

void LoadSpriteFile();

#endif // !defined(AFX_SPRITELIST_H__11E4A548_361F_4AD1_A8F6_F6950C28C8D0__INCLUDED_)
