// SpriteList.cpp: implementation of the CSpriteList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "SpriteList.h"
#include "GlobalGame.h"
#include "SelectCard7Poker.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// ���ӵ����(���� Ȯ��)
string GH_BTN_PATH			= ".\\7poker\\data\\gamehelp\\gh_btn.spr";
string DOUMI_PATH			= ".\\7poker\\data\\gamehelp\\doumi.spr";
string JBTABLE_PATH			= ".\\7poker\\data\\Skin\\JbTable.spr";

string AUTOBETPANEL_PATH	= ".\\PokerCommon\\data\\AutobetPanel.spr";	
							  


string GRADE_PATH			= ".\\7poker\\data\\grade.spr";
string CHANCE_PATH			= ".\\7poker\\data\\Chance.spr";
string HIDDENOPEN_PATH		= ".\\7poker\\data\\HiddenOpen.spr";
string HANDOEFFECT_PATH		= ".\\7poker\\data\\HandoEffect.spr";
string MADEEFFECT_PATH		= ".\\PokerCommon\\data\\complete_made.spr";	//���̵� ����Ʈ

// ī��
string BIGCARD_PATH			= ".\\7poker\\data\\card\\BigCard.spr";
string DARKCARD_PATH		= ".\\7poker\\data\\card\\DarkBigCard.spr";
string DISCARD_PATH			= ".\\7poker\\data\\card\\discard.spr";
string CARDDEALING_PATH		= ".\\7poker\\data\\card\\CardDealing.spr";

string LASTCARDOPEN_PATH = ".\\7poker\\data\\card\\LastCardOpen.spr";

// ���
string HIVALUEFFECT_PATH	= ".\\7poker\\data\\jokboEffect\\Effect_7poker.spr";

// Skin ����
string CHATTINGWND_PATH		= ".\\7poker\\data\\Skin\\ChatBox.spr";
string WINLOSER_PATH		= ".\\7poker\\data\\Skin\\winlose.spr";
string RESULTNUMBER_PATH	= ".\\7poker\\data\\Skin\\resultnumber.spr";
string BETBTN_N_PATH		= ".\\7poker\\data\\Skin\\BetBtn_N.spr";
string BETBTN_H_PATH		= ".\\7poker\\data\\Skin\\BetBtn_H.spr";
string BETBTN_P_PATH		= ".\\7poker\\data\\Skin\\BetBtn_P.spr";

// Common ����
string TOPMENUBTN_PATH		= ".\\PokerCommon\\data\\TopMenuBtn.spr";
string JACKPOT_PATH			= ".\\PokerCommon\\data\\Jackpot.spr";
string CHIP_PATH			= ".\\PokerCommon\\data\\Chip.spr";

string TIMER_PATH			= ".\\PokerCommon\\data\\timer.spr";

string SP_CLOSEBTN_PATH		= ".\\PokerCommon\\data\\sp_closeBtn.spr";


string BETTINGEFFECT_PATH	= ".\\PokerCommon\\data\\BettingEffect.spr";
string VIPJACKPOT_BET_PATH	= ".\\PokerCommon\\data\\VIPjacpot_beteft.spr";



NMBASE::GRAPHICGDI::xSprite g_sprGLOBAL[ spr::GSPR_ALL ];

NMBASE::GRAPHICGDI::xSprite g_sprUITopBtn;				// ���� ��ư
NMBASE::GRAPHICGDI::xSprite g_sprWinLoser;				// ����/���� ����Ʈ

NMBASE::GRAPHICGDI::xSprite g_sprChip;					// Ĩ 
NMBASE::GRAPHICGDI::xSprite g_sprBettingEffect;			// ���� ����Ʈ

NMBASE::GRAPHICGDI::xSprite g_sprJokboEffect;			// ���� ���� ȿ�� ����,����,����

NMBASE::GRAPHICGDI::xSprite GradeSpr;					// ���ӹ� �̿���
NMBASE::GRAPHICGDI::xSprite ChanceEffectSpr;			// ���� ������

NMBASE::GRAPHICGDI::xSprite g_sprBigCard;				// ū �̹��� ī��
NMBASE::GRAPHICGDI::xSprite g_sprDarkCard;				// ��ο� ū �̹��� ī��
NMBASE::GRAPHICGDI::xSprite g_sprDisCard;				// ���� ī��
NMBASE::GRAPHICGDI::xSprite g_sprJackpotNumber;			// ���ӹ� ���� ����

NMBASE::GRAPHICGDI::xSprite g_sprResultNumber;			// ��� ����

NMBASE::GRAPHICGDI::xSprite g_sprCardDealing;			// ī�� ���� Animation

NMBASE::GRAPHICGDI::xSprite g_sprCloseButton;			// �������� Close ��ư

NMBASE::GRAPHICGDI::xSprite g_SprAutoBettingInfoMsg;

NMBASE::GRAPHICGDI::xSprite g_sprLastCardOpen;			//����ī�����


void LoadSpriteFile()
{
	/*-------------------------------------------------------------------------------*/
	/*	���� �� ���� spr
	/*-------------------------------------------------------------------------------*/
	_LOADSPRITEDEFINE(g_sprChip, CHIP_PATH, false);
	_LOADSPRITEDEFINE(g_sprBettingEffect, BETTINGEFFECT_PATH, false);	
	//���
	_LOADSPRITEDEFINE(GradeSpr, GRADE_PATH, false);

	//���������� ȿ��
	_LOADSPRITEDEFINE(ChanceEffectSpr, CHANCE_PATH, false);
	
	// ���� �߰��� ��ư�� ����, üũ	
	_LOADSPRITEDEFINE(g_sprGLOBAL[ spr::GSPR_V_H_OPEN ], HIDDENOPEN_PATH );
	_LOADSPRITEDEFINE(g_sprGLOBAL[ spr::GSPR_HANDO ]	, HANDOEFFECT_PATH, false); // �����ѵ��޼� ����Ʈ
	_LOADSPRITEDEFINE(g_sprGLOBAL[ spr::GSPR_ET_MADE ]	, MADEEFFECT_PATH, false); //���̵�����Ʈ

	/*-------------------------------------------------------------------------------*/
	/*	ī�� spr
	/*-------------------------------------------------------------------------------*/	
	
	// �� ī��
	_LOADSPRITEDEFINE(g_sprBigCard, BIGCARD_PATH, false);	
	// ����ī��
	_LOADSPRITEDEFINE(g_sprDisCard, DISCARD_PATH, false);		
	// ������ �ȵ� ī��
	_LOADSPRITEDEFINE(g_sprDarkCard, DARKCARD_PATH, false);		

	_LOADSPRITEDEFINE( g_sprLastCardOpen, LASTCARDOPEN_PATH, false );		

	/*-------------------------------------------------------------------------------*/
	/*	�¸� spr
	/*-------------------------------------------------------------------------------*/	
	//���� ���� ȿ�� ����,����,����
	_LOADSPRITEDEFINE(g_sprJokboEffect, HIVALUEFFECT_PATH, false);		

	_LOADSPRITEDEFINE(g_sprWinLoser, WINLOSER_PATH, false);	

	// ���� ��ư
	_LOADSPRITEDEFINE(g_sprUITopBtn, TOPMENUBTN_PATH, false);	

	// JackPot
	_LOADSPRITEDEFINE(g_sprJackpotNumber, JACKPOT_PATH, false);

	// ��� ����
	_LOADSPRITEDEFINE(g_sprResultNumber, RESULTNUMBER_PATH, false);

	// �������� ��ư
	_LOADSPRITEDEFINE(g_sprCloseButton, SP_CLOSEBTN_PATH, false);

	_LOADSPRITEDEFINE(g_SprAutoBettingInfoMsg, AUTOBETPANEL_PATH, false);
	// ī�� ����
	_LOADSPRITEDEFINE(g_sprCardDealing, CARDDEALING_PATH, false);

	// ä�� ��������Ʈ
	if(g_pChatWinMan)
	{
		g_pChatWinMan->LoadImage(CHATTINGWND_PATH.c_str());
	}

	// �ڸ���Ŵ��
	g_SeatKeeper.LoadImage();

	// ������
	g_ObserverMan.LoadImage();

	// ��������
	g_cOneShotCharge.LoadImage();
	g_SelectCard().LoadImage();

	// ĳ���� ����
	if(g_pGameView)
	{
		// ��ư�� ��� �ε��Ѵ�.
		g_pGameView->m_cGameViewBtnMananager.LoadImage( BETBTN_N_PATH.c_str(), BETBTN_H_PATH.c_str(), BETBTN_P_PATH.c_str() );

		// �߾� �ݾ� ǥ��
		g_pGameView->m_cTotalBetMoney.LoadImage();
		
		// SidePot
		g_pGameView->m_cSidePotMoney.LoadImage(".\\7poker\\data\\side_betting_displayboard.spr");
		
		g_pGameView->m_cPlayerDrawManager.LoadImage(TIMER_PATH.c_str(), &g_sprBettingEffect, &g_sprWinLoser, ".\\7Poker\\data\\AllinEffect.spr");
		g_pGameView->m_cGameHelper.LoadImage(GH_BTN_PATH.c_str(), DOUMI_PATH.c_str(), JBTABLE_PATH.c_str(), ".\\PokerCommon\\data\\jokboinfo.spr");
	
#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
		if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
			g_pGameView->m_cBackGround.LoadImage(".\\7poker\\data\\Skin\\Gameback_toctoc.spr");
		else
			g_pGameView->m_cBackGround.LoadImage(".\\7poker\\data\\Skin\\GameBack.spr");
#else //UPGRADE_20120308_TOCTOC_CHANNELING
			g_pGameView->m_cBackGround.LoadImage(".\\7poker\\data\\Skin\\GameBack.spr");
#endif //UPGRADE_20120308_TOCTOC_CHANNELING

		g_pGameView->m_cTitle.LoadImage(IDX_GAME_SP);
		g_GAMEPROCESS()->m_AllBetChip.LoadImage();
	}	

	// �ѵ��Ӵ�
	g_cOwnLimit.LoadImage(HANDOEFFECT_PATH.c_str());
}