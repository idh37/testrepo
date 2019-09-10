#pragma once

namespace spr
{	
	enum _SPRITE
	{
		GSPR_V_H_OPEN,	// ���� ����
		GSPR_ET_MADE,	//���̵�����Ʈ		

		GSPR_ALL
		
	};
}

extern NMBASE::GRAPHICGDI::xSprite GLOBAL_SPR[ spr::GSPR_ALL ];

extern NMBASE::GRAPHICGDI::xSprite g_sprResultNumber;		// ��� ����

extern NMBASE::GRAPHICGDI::xSprite g_sprWinLoseBack;		// ���а�� ǥ�� ���

extern NMBASE::GRAPHICGDI::xSprite g_sprJBTable;			// ���� ���̺�
// ����Ű

extern NMBASE::GRAPHICGDI::xSprite g_sprBombSetAni;		//��ź ȿ��
extern NMBASE::GRAPHICGDI::xSprite g_sprTabBtn;			// ä��,���� ��ư

extern NMBASE::GRAPHICGDI::xSprite g_sprJokboEffect;				//��Ƽ��
extern NMBASE::GRAPHICGDI::xSprite g_sprBodyItem;
extern NMBASE::GRAPHICGDI::xSprite g_sprAllInEft;				// ���� ���� ȿ��
extern NMBASE::GRAPHICGDI::xSprite g_sprBetCmd;				// ����Ŀ��� 

extern NMBASE::GRAPHICGDI::xSprite g_sprGrade;
extern NMBASE::GRAPHICGDI::xSprite g_sprAutoBettingSmall;		//������ ���÷� ����
extern NMBASE::GRAPHICGDI::xSprite g_sprAutoBetting;			//�ڵ����� ���� ���
extern NMBASE::GRAPHICGDI::xSprite g_sprAutoBetSetUp;
extern NMBASE::GRAPHICGDI::xSprite g_sprAutoBettingInfoMsg;

extern NMBASE::GRAPHICGDI::xSprite g_sprChangeRoomInfoMsg;	//������ ���� ���� �˸� �޼���

/////////////////////////////////////////////////////////////////////////
//ī�� �κ�
extern NMBASE::GRAPHICGDI::xSprite g_sprBigCard;		// ī��
extern NMBASE::GRAPHICGDI::xSprite g_sprDarkBigCard;	// ī��
extern NMBASE::GRAPHICGDI::xSprite g_sprBigBombCard;	// ī��
extern NMBASE::GRAPHICGDI::xSprite g_sprSmallBombCard;	// ī��
extern NMBASE::GRAPHICGDI::xSprite g_sprDisCard;		//����ī��

extern NMBASE::GRAPHICGDI::xSprite g_sprLastCardOpen;			//����ī�����

extern NMBASE::GRAPHICGDI::xSprite g_sprJokerCardEffect;
extern NMBASE::GRAPHICGDI::xSprite g_sprCardDealing;		//ī�� ���� ��������Ʈ
extern NMBASE::GRAPHICGDI::xSprite g_sprBombCardDealing;	//��źī�� ���� ��������Ʈ

extern NMBASE::GRAPHICGDI::xSprite g_sprSDLG_ProFlie;			// ������

//���������� 2006.3.2
extern NMBASE::GRAPHICGDI::xSprite g_sprAllinChargeInfo;

extern NMBASE::GRAPHICGDI::xSprite g_sprJackpoText;			// ���� ���� �ִ�
extern NMBASE::GRAPHICGDI::xSprite g_sprJackPotBackEffect;	// ���� ����  �ִ� 
extern NMBASE::GRAPHICGDI::xSprite g_sprJackPot;				// ���ӹ� ����
extern NMBASE::GRAPHICGDI::xSprite g_sprChip;					// Ĩ
extern NMBASE::GRAPHICGDI::xSprite g_sprClose;

extern NMBASE::GRAPHICGDI::xSprite g_ChanceEffectSpr;		//�����ƹ�Ÿ ����Ʈ