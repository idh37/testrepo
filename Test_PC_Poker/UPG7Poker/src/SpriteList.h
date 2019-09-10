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
		GSPR_V_H_OPEN= 2,	// ���� ����
		GSPR_HANDO   = 4,	// �����ѵ�
		GSPR_ET_MADE,	//���̵�����Ʈ

		GSPR_ALL
	};
}

extern NMBASE::GRAPHICGDI::xSprite g_sprGLOBAL[ spr::GSPR_ALL ];

extern NMBASE::GRAPHICGDI::xSprite g_sprUITopBtn;				// ���� ��ư

extern NMBASE::GRAPHICGDI::xSprite g_sprWinLoser;				// ����/���� ����Ʈ

extern NMBASE::GRAPHICGDI::xSprite g_sprChip;					// Ĩ 

extern NMBASE::GRAPHICGDI::xSprite g_sprBettingEffect;			// ���� ����Ʈ

extern NMBASE::GRAPHICGDI::xSprite g_sprJokboEffect;			// ���� ���� ȿ�� ����,����,����

extern NMBASE::GRAPHICGDI::xSprite GradeSpr;					// ���ӹ� �̿���
extern NMBASE::GRAPHICGDI::xSprite ChanceEffectSpr;				// ���� ������

extern NMBASE::GRAPHICGDI::xSprite g_sprBigCard;				// ���� ���̷ο� ī��

extern NMBASE::GRAPHICGDI::xSprite g_sprDarkCard;				// ���� ���� ��� ���� ī�� ȿ��

extern NMBASE::GRAPHICGDI::xSprite g_sprDisCard;				// ���� ī��

extern NMBASE::GRAPHICGDI::xSprite g_sprLastCardOpen;			//����ī�����

extern NMBASE::GRAPHICGDI::xSprite g_sprJackpotNumber;			// ���ӹ� ���� ����

extern NMBASE::GRAPHICGDI::xSprite g_sprWinSportlight;			// ���� ��(�¸��ڿ��� ���̴� ����Ʈ����Ʈ)

extern NMBASE::GRAPHICGDI::xSprite g_sprResultNumber;			// ��� ����

extern NMBASE::GRAPHICGDI::xSprite g_sprCardDealing;			// ī�� ���� Animation

extern NMBASE::GRAPHICGDI::xSprite g_sprCloseButton;			// �������� Close ��ư

extern NMBASE::GRAPHICGDI::xSprite g_SprAutoBettingInfoMsg;

void LoadSpriteFile();

#endif // !defined(AFX_SPRITELIST_H__11E4A548_361F_4AD1_A8F6_F6950C28C8D0__INCLUDED_)
