// SpriteList.cpp: implementation of the CSpriteList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "SpriteList.h"
#include "GlobalGame.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif						  


string GRADE_PATH			= ".\\hoola\\data\\grade.spr";
string CHANCE_PATH			= ".\\hoola\\data\\Chance.spr";
string HANDOEFFECT_PATH		= ".\\hoola\\data\\HandoEffect.spr";

// Skin ����
string CHATTINGWND_PATH		= ".\\hoola\\data\\Skin\\ChatBox.spr";
string CHATTIN_TAB_EFFECT_PATH		= ".\\hoola\\data\\Skin\\chattingtabeffect.spr";
string WINLOSER_PATH		= ".\\hoola\\data\\Skin\\winlose.spr";

// Common ����
string TOPMENUBTN_PATH		= ".\\PokerCommon\\data\\TopMenuBtn.spr";
string JACKPOT_PATH			= ".\\PokerCommon\\data\\Jackpot.spr";
string TIMER_PATH			= ".\\PokerCommon\\data\\timer.spr";
string SP_CLOSEBTN_PATH		= ".\\PokerCommon\\data\\sp_closeBtn.spr";

NMBASE::GRAPHICGDI::xSprite g_sprGLOBAL[ spr::GSPR_ALL ];
NMBASE::GRAPHICGDI::xSprite g_sprUITopBtn;				// ���� ��ư
NMBASE::GRAPHICGDI::xSprite GradeSpr;					// ���ӹ� �̿���
NMBASE::GRAPHICGDI::xSprite ChanceEffectSpr;			// ���� ������
NMBASE::GRAPHICGDI::xSprite g_sprJackpotNumber;			// ���ӹ� ���� ����
NMBASE::GRAPHICGDI::xSprite g_sprCloseButton;			// �������� Close ��ư
NMBASE::GRAPHICGDI::xSprite g_sprWinLoser;

// Hoola ����
NMBASE::GRAPHICGDI::xSprite g_sprRegOk;
NMBASE::GRAPHICGDI::xSprite g_sprNoThankBtn;
NMBASE::GRAPHICGDI::xSprite g_sprThankTimer;
NMBASE::GRAPHICGDI::xSprite g_sprTimer;
NMBASE::GRAPHICGDI::xSprite g_sprBigCard;
NMBASE::GRAPHICGDI::xSprite g_sprGameBackPart;
NMBASE::GRAPHICGDI::xSprite g_sprJackPotInfo;
NMBASE::GRAPHICGDI::xSprite g_sprJackPotAni;
NMBASE::GRAPHICGDI::xSprite g_sprMission;
NMBASE::GRAPHICGDI::xSprite g_sprMissionComp;
NMBASE::GRAPHICGDI::xSprite g_sprGold;
NMBASE::GRAPHICGDI::xSprite g_sprEffect;
NMBASE::GRAPHICGDI::xSprite g_sprCardSelect;
NMBASE::GRAPHICGDI::xSprite g_sprEffectBak;
NMBASE::GRAPHICGDI::xSprite g_sprSelectCard;
NMBASE::GRAPHICGDI::xSprite g_sprHoola;
NMBASE::GRAPHICGDI::xSprite g_sprMagic;
NMBASE::GRAPHICGDI::xSprite g_sprCardNum;
NMBASE::GRAPHICGDI::xSprite g_sprMyCard;
NMBASE::GRAPHICGDI::xSprite g_sprFocus;
NMBASE::GRAPHICGDI::xSprite g_sprSmallCard;
NMBASE::GRAPHICGDI::xSprite g_sprCardCount;
NMBASE::GRAPHICGDI::xSprite g_sprAllin;
NMBASE::GRAPHICGDI::xSprite g_sprResult;
NMBASE::GRAPHICGDI::xSprite g_sprWaiting;
NMBASE::GRAPHICGDI::xSprite g_sprSignBoard;
NMBASE::GRAPHICGDI::xSprite g_sprSunPickup;
NMBASE::GRAPHICGDI::xSprite g_sprIntro;
NMBASE::GRAPHICGDI::xSprite g_sprPracticsAvatar;
NMBASE::GRAPHICGDI::xSprite g_SprCardShortcut;
NMBASE::GRAPHICGDI::xSprite g_SprTurnEffect;
NMBASE::GRAPHICGDI::xSprite g_SprResultOK;
NMBASE::GRAPHICGDI::xSprite g_sprLimitIcon;





void LoadSpriteFile()
{
	/*-------------------------------------------------------------------------------*/
	/*	���� �� ���� spr
	/*-------------------------------------------------------------------------------*/
	//���
	_LOADSPRITEDEFINE(GradeSpr, GRADE_PATH, false);

	//���������� ȿ��
	_LOADSPRITEDEFINE(ChanceEffectSpr, CHANCE_PATH, false);
	
	// ���� �߰��� ��ư�� ����, üũ	
	_LOADSPRITEDEFINE(g_sprGLOBAL[ spr::GSPR_HANDO ]	, HANDOEFFECT_PATH, false); // �����ѵ��޼� ����Ʈ


	_LOADSPRITEDEFINE(g_sprWinLoser, WINLOSER_PATH, false);	

	// ���� ��ư
	_LOADSPRITEDEFINE(g_sprUITopBtn, TOPMENUBTN_PATH, false);	

	// JackPot
	_LOADSPRITEDEFINE(g_sprJackpotNumber, JACKPOT_PATH, false);

	// �������� ��ư
	_LOADSPRITEDEFINE(g_sprCloseButton, SP_CLOSEBTN_PATH, false);


	// ä�� ��������Ʈ
	if(g_pChatWinMan)
	{
		g_pChatWinMan->LoadImage(CHATTINGWND_PATH.c_str(),CHATTIN_TAB_EFFECT_PATH.c_str());
	}

	// �ڸ���Ŵ��
	g_SeatKeeper.LoadImage();

	// ������
	g_ObserverMan.LoadImage();

	// ��������
	g_cOneShotCharge.LoadImage();

	// ĳ���� ����
	if(g_pGameView)
	{
		// ��ư�� ��� �ε��Ѵ�.
		g_pGameView->m_cGameViewBtnMananager.LoadImage(NULL, NULL, NULL);
		g_pGameView->m_cPlayerDrawManager.LoadImage(TIMER_PATH.c_str(), NULL, &g_sprWinLoser, ".\\hoola\\data\\AllinEffect.spr");

#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
		if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
			g_pGameView->m_cBackGround.LoadImage(".\\hoola\\data\\Skin\\Gameback_toctoc.spr");
		else
			g_pGameView->m_cBackGround.LoadImage(".\\hoola\\data\\Skin\\GameBack.spr");
#else //UPGRADE_20120308_TOCTOC_CHANNELING
		g_pGameView->m_cBackGround.LoadImage(".\\hoola\\data\\Skin\\GameBack.spr");
#endif //UPGRADE_20120308_TOCTOC_CHANNELING

		g_pGameView->m_cTitle.LoadImage(IDX_GAME_HA);
	}

	// �ѵ��Ӵ�
	g_cOwnLimit.LoadImage(HANDOEFFECT_PATH.c_str());


	//�Ƕ� ��������Ʈ
	string temp = ".\\hoola\\data\\hoola\\RegOk.spr";
	_LOADSPRITEDEFINE(g_sprRegOk,			temp, false);
	temp = ".\\hoola\\data\\hoola\\NoThankyou.spr";
	_LOADSPRITEDEFINE(g_sprNoThankBtn,	temp, false);
	temp = ".\\hoola\\data\\hoola\\ThankTimer.spr";
	_LOADSPRITEDEFINE(g_sprThankTimer,	temp, false);
	temp = ".\\hoola\\data\\hoola\\Timer.spr";
	_LOADSPRITEDEFINE(g_sprTimer,			temp, false);
	temp = ".\\hoola\\data\\hoola\\BigCard.spr";
	_LOADSPRITEDEFINE(g_sprBigCard,			temp, false);
	temp = ".\\hoola\\data\\hoola\\GameBackPart.spr";
	_LOADSPRITEDEFINE(g_sprGameBackPart,			temp, false);
	temp = ".\\hoola\\data\\hoola\\JackPot.spr";
	_LOADSPRITEDEFINE(g_sprJackPotInfo,		temp, false);
	temp = ".\\hoola\\data\\hoola\\JackpotAni.spr";
	_LOADSPRITEDEFINE(g_sprJackPotAni,	temp, false);
	temp = ".\\hoola\\data\\hoola\\Mission.spr";
	_LOADSPRITEDEFINE(g_sprMission,		temp, false);
	temp = ".\\hoola\\data\\hoola\\MissionClear.spr";
	_LOADSPRITEDEFINE(g_sprMissionComp,		temp, false);
	temp = ".\\hoola\\data\\hoola\\GoldStage.spr";
	_LOADSPRITEDEFINE(g_sprGold,			temp, false);
	temp = ".\\hoola\\data\\hoola\\Effect.spr";
	_LOADSPRITEDEFINE(g_sprEffect,		temp, false);
	temp = ".\\hoola\\data\\hoola\\Cardselect.spr";
	_LOADSPRITEDEFINE(g_sprCardSelect,	temp, false);
	temp = ".\\hoola\\data\\hoola\\EffectBak.spr";
	_LOADSPRITEDEFINE(g_sprEffectBak,		temp, false);
	temp = ".\\hoola\\data\\hoola\\SelectCard.spr";
	_LOADSPRITEDEFINE(g_sprSelectCard,		temp, false);
	temp = ".\\hoola\\data\\hoola\\Hoola.spr";
	_LOADSPRITEDEFINE(g_sprHoola,			temp, false);
	temp = ".\\hoola\\data\\hoola\\MagicCard.spr";
	_LOADSPRITEDEFINE(g_sprMagic,			temp, false);
	temp = ".\\hoola\\data\\hoola\\CardNum.spr";
	_LOADSPRITEDEFINE(g_sprCardNum,			temp, false);
	temp = ".\\hoola\\data\\hoola\\MyCard.spr";
	_LOADSPRITEDEFINE(g_sprMyCard,			temp, false);
	temp = ".\\hoola\\data\\hoola\\focus.spr";
	_LOADSPRITEDEFINE(g_sprFocus,			temp, false);
	temp = ".\\hoola\\data\\hoola\\SmallCard.spr";
	_LOADSPRITEDEFINE(g_sprSmallCard,			temp, false);
	temp = ".\\hoola\\data\\hoola\\CardCount.spr";
	_LOADSPRITEDEFINE(g_sprCardCount,			temp, false);
	temp = ".\\hoola\\data\\hoola\\Allin.spr";
	_LOADSPRITEDEFINE(g_sprAllin,			temp, false);
	temp = ".\\hoola\\data\\hoola\\GameResult.spr";
	_LOADSPRITEDEFINE(g_sprResult,			temp, false);
	temp = ".\\hoola\\data\\hoola\\Waiting.spr";
	_LOADSPRITEDEFINE(g_sprWaiting,			temp, false);
	temp = ".\\hoola\\data\\hoola\\SignBoard.spr";
	_LOADSPRITEDEFINE(g_sprSignBoard,			temp, false);
	temp = ".\\hoola\\data\\hoola\\Sunpickup.spr";
	_LOADSPRITEDEFINE(g_sprSunPickup,			temp, false);
	temp = ".\\hoola\\data\\hoola\\Intro.spr";
	_LOADSPRITEDEFINE(g_sprIntro,			temp, false);
	temp = ".\\hoola\\data\\hoola\\PracticsAvatar.spr";
	_LOADSPRITEDEFINE(g_sprPracticsAvatar,	temp, false);
	temp = ".\\hoola\\data\\hoola\\CardShortcut.spr";
	_LOADSPRITEDEFINE(g_SprCardShortcut,	temp, false);	
	temp = ".\\hoola\\data\\hoola\\Turneffect.spr";
	_LOADSPRITEDEFINE(g_SprTurnEffect,	temp, false);
	temp = ".\\hoola\\data\\hoola\\ResultOkBtn.spr";
	_LOADSPRITEDEFINE(g_SprResultOK,	temp, false);	
	temp = ".\\PokerCommon\\data\\LimitIcon.spr";
	_LOADSPRITEDEFINE(g_sprLimitIcon,	temp, false);	
	
}
