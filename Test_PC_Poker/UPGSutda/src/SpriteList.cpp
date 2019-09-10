// SpriteList.cpp: implementation of the CSpriteList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "SpriteList.h"
#include "GlobalGame.h"
#include "SelectCardSutda.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// ���ӵ����(���� Ȯ��)
string GH_BTN_PATH			= ".\\sutda\\data\\gamehelp\\gh_btn.spr";
string DOUMI_PATH			= ".\\sutda\\data\\gamehelp\\doumi.spr";
string JBTABLE_PATH			= ".\\sutda\\data\\Skin\\JbTable.spr";

string AUTOBETPANEL_PATH	= ".\\PokerCommon\\data\\AutobetPanel.spr";	
							  


string GRADE_PATH			= ".\\sutda\\data\\grade.spr";
string CHANCE_PATH			= ".\\sutda\\data\\Chance.spr";
string HIDDENOPEN_PATH		= ".\\sutda\\data\\HiddenOpen.spr";
string HANDOEFFECT_PATH		= ".\\sutda\\data\\HandoEffect.spr";
string MADEEFFECT_PATH		= ".\\PokerCommon\\data\\complete_made.spr";	//���̵� ����Ʈ

// ī��
string BIGCARD_PATH			= ".\\sutda\\data\\card\\BigCard.spr";




// Skin ����
string CHATTINGWND_PATH		= ".\\sutda\\data\\Skin\\ChatBox.spr";
string CHATTIN_TAB_EFFECT_PATH		= ".\\sutda\\data\\Skin\\chattingtabeffect.spr";
string WINLOSER_PATH		= ".\\sutda\\data\\Skin\\winlose.spr";
string RESULTNUMBER_PATH	= ".\\sutda\\data\\Skin\\resultnumber.spr";
string BETBTN_N_PATH		= ".\\sutda\\data\\Skin\\BetBtn_N.spr";
string BETBTN_H_PATH		= ".\\sutda\\data\\Skin\\BetBtn_H.spr";
string BETBTN_P_PATH		= ".\\sutda\\data\\Skin\\BetBtn_P.spr";

// Common ����
string TOPMENUBTN_PATH		= ".\\PokerCommon\\data\\TopMenuBtn.spr";
string JACKPOT_PATH			= ".\\PokerCommon\\data\\Jackpot.spr";
string CHIP_PATH			= ".\\PokerCommon\\data\\Chip.spr";

string TIMER_PATH			= ".\\sutda\\data\\Anidata\\timer.spr";				//���ٿ� Ÿ�̸� ����Ʈ�� �����ִ�.

string SP_CLOSEBTN_PATH		= ".\\PokerCommon\\data\\sp_closeBtn.spr";


string BETTINGEFFECT_PATH	= ".\\sutda\\data\\BettingEffect.spr";		//���ٿ� ��������Ʈ�� �����ִ�.

string PENATYMONEY_RED_PATH		= ".\\sutda\\data\\PenaltyRed.spr";
string PENATYMONEY_BLUE_PATH		= ".\\sutda\\data\\PenaltyBlue.spr";

string CALLMONEY_PATH			= ".\\sutda\\data\\CallMoney.spr";

string SUTDA_TURN_PATH		= ".\\sutda\\data\\turn.spr";

string SUTDA_HIDDENOPENTOOLTIP_PATH = ".\\sutda\\data\\HiddenOpen.spr";

NMBASE::GRAPHICGDI::xSprite g_sprGLOBAL[ spr::GSPR_ALL ];

NMBASE::GRAPHICGDI::xSprite g_sprWinLoser;				// ����/���� ����Ʈ

NMBASE::GRAPHICGDI::xSprite g_sprChip;					// Ĩ 
NMBASE::GRAPHICGDI::xSprite g_sprBettingEffect;			// ���� ����Ʈ

NMBASE::GRAPHICGDI::xSprite GradeSpr;					// ���ӹ� �̿���
NMBASE::GRAPHICGDI::xSprite ChanceEffectSpr;			// ���� ������

NMBASE::GRAPHICGDI::xSprite g_sprBigCard;				// ū �̹��� ī��

NMBASE::GRAPHICGDI::xSprite g_sprJackpotNumber;			// ���ӹ� ���� ����


NMBASE::GRAPHICGDI::xSprite g_sprCloseButton;			// �������� Close ��ư

NMBASE::GRAPHICGDI::xSprite g_SprAutoBettingInfoMsg;


NMBASE::GRAPHICGDI::xSprite g_sprPenaltyRed;				// ���� ���� ���� ��������Ʈ
NMBASE::GRAPHICGDI::xSprite g_sprPenaltyBlue;			// ���� ���� ���� ��������Ʈ
NMBASE::GRAPHICGDI::xSprite g_sprTurn;					//���� ��
NMBASE::GRAPHICGDI::xSprite g_HiddenOpenSpr;			//���� ���� ���� ����Ű ����


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
		


	/*-------------------------------------------------------------------------------*/
	/*	�¸� spr
	/*-------------------------------------------------------------------------------*/	

	_LOADSPRITEDEFINE(g_sprWinLoser, WINLOSER_PATH, false);	
	
	// JackPot
	_LOADSPRITEDEFINE(g_sprJackpotNumber, JACKPOT_PATH, false);

	// �������� ��ư
	_LOADSPRITEDEFINE(g_sprCloseButton, SP_CLOSEBTN_PATH, false);

	_LOADSPRITEDEFINE(g_SprAutoBettingInfoMsg, AUTOBETPANEL_PATH, false);
	

	// ���� ����
	_LOADSPRITEDEFINE(g_sprPenaltyRed, PENATYMONEY_RED_PATH, false);
	_LOADSPRITEDEFINE(g_sprPenaltyBlue, PENATYMONEY_BLUE_PATH, false);


	_LOADSPRITEDEFINE(g_sprPenaltyBlue, PENATYMONEY_BLUE_PATH, false);

	_LOADSPRITEDEFINE(g_sprTurn, SUTDA_TURN_PATH, false);			//���� ��
	_LOADSPRITEDEFINE(g_HiddenOpenSpr, SUTDA_HIDDENOPENTOOLTIP_PATH, false);		//���� ���� ���� ����Ű ����
	


	

	// ä�� ��������Ʈ
	if(g_pChatWinMan)
	{
		g_pChatWinMan->LoadImage(CHATTINGWND_PATH.c_str(), CHATTIN_TAB_EFFECT_PATH.c_str() );
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
		g_pGameView->m_cTotalBetMoney.LoadImage(CALLMONEY_PATH.c_str(), CALLMONEY_PATH.c_str());

		// SidePot
		g_pGameView->m_cSidePotMoney.LoadImage(".\\NewPoker\\data\\side_betting_displayboard.spr");

		g_pGameView->m_cPlayerDrawManager.LoadImage(TIMER_PATH.c_str(), 
													&g_sprBettingEffect, 
													&g_sprWinLoser, 
													".\\sutda\\data\\AllinEffect.spr", 
													&g_sprTurn , 
													".\\sutda\\data\\bg.spr",
													".\\sutda\\data\\Level.spr");
//		g_pGameView->m_cGameHelper.LoadImage(GH_BTN_PATH.c_str(), DOUMI_PATH.c_str(), JBTABLE_PATH.c_str(), ".\\PokerCommon\\data\\jokboinfo.spr");

#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
		if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
			g_pGameView->m_cBackGround.LoadImage(".\\sutda\\data\\Skin\\Gameback_toctoc.spr");
		else
			g_pGameView->m_cBackGround.LoadImage(".\\sutda\\data\\Skin\\GameBack.spr");
#else //UPGRADE_20120308_TOCTOC_CHANNELING
		g_pGameView->m_cBackGround.LoadImage(".\\sutda\\data\\Skin\\GameBack.spr");
#endif //UPGRADE_20120308_TOCTOC_CHANNELING

		g_pGameView->m_cTitle.LoadImage(IDX_GAME_SD);
		//g_GAMEPROCESS()->m_AllBetChip.LoadImage();

		//���� ������
		g_pGameView->m_Radiophonograph.LoadImage();

		//���� �ִϸ��̼�
		g_pGameView->m_AniQue.LoadImage();

		g_pGameView->m_cAskRematchPopup.LoadImage();	

		g_pGameView->m_cChipBoard.LoadImage();

	}	

	// �ѵ��Ӵ�
	g_cOwnLimit.LoadImage(HANDOEFFECT_PATH.c_str());
}
