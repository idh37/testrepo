// GameViewButtonMan.cpp: implementation of the CGameViewButtonMan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameViewButtonMan.h"
#include "../UIInfoData.h"
#include "../shared/KeyDefine.h"
#include "../ChattingWindowMan/ChattingWindowMan.h"
#include "../GlobalPokerBase.h"
#include <CommMsgDef_Game.h>
#include "../Sound.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

DWORD G_KEYDEF[ KEY_DEF_ALL ][ MAX_GAME_KEY ]; 

enum LIMIT_BET_BUTTON_TYPE
{
	LIMIT_BET_BUTTON_TYPE_SMALL,
	LIMIT_BET_BUTTON_TYPE_BIG,
};

#define MAX_ROUND 5
eLIMIT_X g_LimitBetMultiflier[MAX_ROUND][MAX_LIMIT_BET_STEP]=
{
	//0���� (������)
	{
		LIMIT_1X, LIMIT_1X, LIMIT_1X
	},
	//1���� (��ħ��)
	{
		LIMIT_3X,		//����1 (LIMIT_BET_STEP_1)
		LIMIT_5X,		//����2 (LIMIT_BET_STEP_2)
		LIMIT_10X,		//����  (LIMIT_BET_STEP_3)
	},
	//2���� (��ħ)
	{
		LIMIT_10X,		//����1 (LIMIT_BET_STEP_1)
		LIMIT_20X,		//����2 (LIMIT_BET_STEP_2)
		LIMIT_40X		//����  (LIMIT_BET_STEP_3)
	},
	//3���� (����)
	{
		LIMIT_20X,		//����1 (LIMIT_BET_STEP_1)
		LIMIT_40X,		//����2 (LIMIT_BET_STEP_2)
		LIMIT_60X		//����  (LIMIT_BET_STEP_3)
	},
	//4���� (����)
	{
		LIMIT_40X,		//����1 (LIMIT_BET_STEP_1)
		LIMIT_60X,		//����2 (LIMIT_BET_STEP_2)
		LIMIT_80X		//����  (LIMIT_BET_STEP_3)
	}
};

//const INT64  MAX_MONEY = 3300000000000000;
const INT64  MAX_MONEY = 33000000000000;
const INT64  MAX_MONEY_FOR_GAME = 9900000000000;
//const INT64  MAX_MONEY_FOR_GAME = 11700000000000;
// �ι�° ���� ����.
//const INT64  MAX_MONEY_FOR_GAME = 15600000000000;

CGameViewButtonMan::CGameViewButtonMan()
{
	m_pParent = NULL;
	m_pPage = NULL;
	m_ctrlBtnList.Init();
	m_ctrlBetBtnList.Init();
	m_pUIInfoData = NULL;
	m_eBtnDirectionType = CONFIG_BETBTN_TYPE_N;

	int i=0;

	for ( i = 0; i < NT_ALL; i++ )
	{
		m_pNTMoney[ i ] = NULL;
	}

	m_bBtnClick = false;
	m_bBPing =  m_bBCheck = m_bBCall = m_bBMax = m_bBDDa = m_bFold = FALSE;

	m_eSmallBetStep = LIMIT_BET_STEP_1;
	m_eBigBetStep = LIMIT_BET_STEP_2;
	m_nLastRound=0;
	m_eLimitBetType = LIMIT_BET_TYPE_NORMAL;

	m_bInculdeBetttingBtn=true;
	m_bIsShowLossMoneyText = false;
	m_bIsShowOnceLossMoneyText = false;


	//������ : ����ϱ� ��ư
	m_bIsWaitUser = false;
	m_bChangePassState = false;

	nBetPingMoney = 0;
	nBetDDaMoney = 0;
	nBetChkMoney = 0;
	nBetHalfMoney = 0;
	nBetCallMoney = 0;
	m_nExitSubScript=0;

	m_PreState = DAILYLOSS_STATE_NORMAL;
}

CGameViewButtonMan::~CGameViewButtonMan()
{
	m_ctrlBtnList.Init();
	m_ctrlBetBtnList.Init();

	int i=0;
	for ( i = 0; i < NT_ALL; i++ )
	{
		if ( m_pNTMoney [ i ] )
		{
			delete m_pNTMoney[ i ];
			m_pNTMoney[ i ] = NULL;
		}
		m_pNTMoney[ i ] = NULL;
	}
}

void CGameViewButtonMan::LoadImage( LPCTSTR strBetBtn_N, LPCTSTR strBetBtn_H, LPCTSTR strBetBtn_P )
{
	AddLoadImageList(&m_sprUITopBtn,".\\PokerCommon\\data\\TopMenuBtn.spr",true);
	AddLoadImageList(&m_sprUIBottomBtn,".\\PokerCommon\\data\\UIBtn.spr",true);
	AddLoadImageList(&m_sprUICardOpenBtn,".\\PokerCommon\\data\\MyCardOpenBtn.spr",true);

	//���� ��ư �̹�����
	if (strBetBtn_N)
		AddLoadImageList(&m_sprBetBtn_N, strBetBtn_N, true);//"BettingBtn_V.spr"
	if (strBetBtn_H)
		AddLoadImageList(&m_sprBetBtn_H, strBetBtn_H, true);//"BettingBtn_H.spr"
	if (strBetBtn_P)
		AddLoadImageList(&m_sprBetBtn_P, strBetBtn_P, true);//"BettingBtn_V.spr"
		
	AddLoadImageList(&m_sprOneShotChargeBtn,".\\PokerCommon\\data\\oneshotChargeBtn.spr",true);

	//���ù�ư Ÿ�� �޺��ڽ�
	AddLoadImageList(&m_sprBetBtnCombo, ".\\PokerCommon\\data\\BetBtnCombo.spr", true);

	// ������ ���ҽ� ���� �ε�
	AddLoadImageList(&m_sprLossMoney, ".\\PokerCommon\\data\\Changemoney.spr", true);
	AddLoadImageList(&m_sprLossMoneyBtn, ".\\PokerCommon\\data\\ChangemoneyBtn.spr", true);
	AddLoadImageList(&m_sprLossMoneyBtn2, ".\\PokerCommon\\data\\ChangemoneyBtn2.spr", true);

	//������ : ����ϱ� ���� �̹���
	AddLoadImageList(&m_sprWaitUserAlert, ".\\PokerCommon\\data\\Waiting.spr", true);

	//������ : �����ϱ��ư
	AddLoadImageList(&m_sprGamestartBack, ".\\PokerCommon\\data\\ObserverBack.spr", true);

	AddLoadImageList(&m_sprBtn_gamestart_HL, ".\\PokerCommon\\data\\Btn_gamestart_HL.spr", true);
	AddLoadImageList(&m_sprBtn_gamestart_SD, ".\\PokerCommon\\data\\Btn_gamestart_SD.spr", true);
	AddLoadImageList(&m_sprBtn_gamestart_BD, ".\\PokerCommon\\data\\Btn_gamestart_BD.spr", true);
	AddLoadImageList(&m_sprBtn_gamestart_NP, ".\\PokerCommon\\data\\Btn_gamestart_NP.spr", true);
	AddLoadImageList(&m_sprBtn_gamestart_SP, ".\\PokerCommon\\data\\Btn_gamestart_SP.spr", true);

	AddLoadImageList(&m_sprShowFullExplain, ".\\PokerCommon\\data\\NoticeFbet.spr", true);
	AddLoadImageList(&m_sprComplain, ".\\PokerCommon\\data\\Btn_complain.spr", true);
}

void CGameViewButtonMan::Init(CGameViewBase *pView, CUIInfoData *pUIInfoData, CChattingWindowMan *pChattingWindowMan)
{
	m_bMouseOver = 0;
	m_nMousOverIndex = -1;
	m_pPage = &pView->Page;
	m_pParent = pView;
	m_pUIInfoData = pUIInfoData;
	m_pChattingWindowMan = pChattingWindowMan;
	m_nLastCmdBackup = RAISE_LAST_CMD_NULL;

	if (m_bInculdeBetttingBtn)
	{
		//���ù�ư Ÿ�� �޺��ڽ�
		m_cmbBetBtn.Init(m_pParent, m_pPage, &m_pUIInfoData->m_rtBetBtnCombo, IDM_BETBTNTYPE_COMBO, &m_sprBetBtnCombo, 0, 3 );
		m_cmbBetBtn.AddString(" N Ÿ�� ��ư");
		m_cmbBetBtn.AddString(" H Ÿ�� ��ư");
		m_cmbBetBtn.AddString(" P Ÿ�� ��ư");
		m_cmbBetBtn.SetFont(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_12D_BOLD));
		m_cmbBetBtn.SetTextColor(RGB(99,99,99));

		for(int i=0; i<m_cmbBetBtn.GetCount(); i++)
		{
			m_cmbBetBtn.SetTextColor(i, RGB(99,99,99));
		}

		m_cmbBetBtn.SetBackColor(NMBASE::GRAPHICGDI::GetRGBmix(0,0,0));
		m_cmbBetBtn.SetBorderColor(NMBASE::GRAPHICGDI::GetRGBmix(41,41,41));
	}
	
	// �׷��� ��ư �ʱ�ȭ
	//=======================================================================
	// ���� ��ư
	m_btnTip.Init(m_pParent, m_pPage, m_pUIInfoData->m_listButtonPos[EBT_GAME_TIP].x, m_pUIInfoData->m_listButtonPos[EBT_GAME_TIP].y, &m_sprUITopBtn, 0, IDM_TIPBTN);
	m_ctrlBtnList.Add(&m_btnTip);

	m_btnCapture.Init(m_pParent, m_pPage, m_pUIInfoData->m_listButtonPos[EBT_CAPTURE].x, m_pUIInfoData->m_listButtonPos[EBT_CAPTURE].y, &m_sprUITopBtn, 1, IDM_CAPTURE);//ȭ��ĸ��
	m_ctrlBtnList.Add(&m_btnCapture);

	m_btnOption.Init(m_pParent, m_pPage, m_pUIInfoData->m_listButtonPos[EBT_OPTION].x, m_pUIInfoData->m_listButtonPos[EBT_OPTION].y, &m_sprUITopBtn, 2,IDM_OPTION);
	m_ctrlBtnList.Add(&m_btnOption);

	m_btnMinimize.Init(m_pParent, m_pPage	, m_pUIInfoData->m_listButtonPos[EBT_MINIMIZE].x, m_pUIInfoData->m_listButtonPos[EBT_MINIMIZE].y, &m_sprUITopBtn, 3, IDM_MINIMIZE);
	m_ctrlBtnList.Add(&m_btnMinimize);

	m_btnChangeDisplay.Init(m_pParent, m_pPage, m_pUIInfoData->m_listButtonPos[EBT_CHANGEDISPLAY].x, m_pUIInfoData->m_listButtonPos[EBT_CHANGEDISPLAY].y, &m_sprUITopBtn, 4, IDM_CHANGEDISPLAY);

	if (IsFullScreenState())
		m_btnChangeDisplay.SetSprite(&m_sprUITopBtn,6);// ### [ ������� ] ###

	m_ctrlBtnList.Add(&m_btnChangeDisplay);

	m_btnExit.Init(m_pParent, m_pPage, m_pUIInfoData->m_listButtonPos[EBT_TOP_EXIT].x, m_pUIInfoData->m_listButtonPos[EBT_TOP_EXIT].y, &m_sprUITopBtn, 5, IDM_EXIT);
	m_btnExit.SetMouseClickSound(SND_BUTTON_CLICK_C);
	m_ctrlBtnList.Add(&m_btnExit);

	m_btnUseGrade.Init(m_pParent, m_pPage, m_pUIInfoData->m_listButtonPos[EBT_USER_GRADE].x, m_pUIInfoData->m_listButtonPos[EBT_USER_GRADE].y, &m_sprUITopBtn, 7, IDM_GRADE);
	m_ctrlBtnList.Add(&m_btnUseGrade);
	//=======================================================================

	//=======================================================================
	//�Ʒ��� ��ư

	//������ ���� : �ʴ��ϱ� ��ư ��ſ� �ٸ��� ���� ��ư���� ��ü 
	m_btnGotoOtherRoom.Init(m_pParent, m_pPage, m_pUIInfoData->m_listButtonPos[EBT_INVITE].x, m_pUIInfoData->m_listButtonPos[EBT_INVITE].y, &m_sprUIBottomBtn, 6,IDM_INVITE);
	m_ctrlBtnList.Add(&m_btnGotoOtherRoom);

	// ������Ȳ ��ư(������ ���� ��ư �Ʒ�)
	m_btnObserverView.Init(m_pParent, m_pPage, m_pUIInfoData->m_listButtonPos[EBT_OBSERVERVIEW].x, m_pUIInfoData->m_listButtonPos[EBT_OBSERVERVIEW].y, &m_sprUIBottomBtn, 4, IDM_OBSERVERVIEW);
	m_ctrlBtnList.Add(&m_btnObserverView);

	if (GM().GetCurrentGameType() == IDX_GAME_HA)
	{
		// ����ϱ� ��ư(��� ������ �Ʒ�)
		m_btnGiveUp.Init(m_pParent, m_pPage, m_pUIInfoData->m_listButtonPos[EBT_GIVEUP].x, m_pUIInfoData->m_listButtonPos[EBT_GIVEUP].y, &m_sprUIBottomBtn, 8, IDM_GIVEUP);
		m_btnGiveUp.SetMouseClickSound(SND_BUTTON_CLICK_A);
		m_ctrlBtnList.Add(&m_btnGiveUp);
		//m_btnGiveUp.Enable(false);
		m_btnGiveUp.Show(false);
	}

	// ���� ī�� ���� ��ư(��� ������ �Ʒ�)
	if (GM().GetCurrentGameType() != IDX_GAME_HA)
	{
		m_btnCardOpen.Init(m_pParent, m_pPage, m_pUIInfoData->m_listButtonPos[EBT_CARD_OPEN].x,	m_pUIInfoData->m_listButtonPos[EBT_CARD_OPEN].y, &m_sprUICardOpenBtn, 1, IDM_CARD_OPEN);
		m_btnCardOpen.SetMouseClickSound(SND_BUTTON_CLICK_A);
		m_ctrlBtnList.Add(&m_btnCardOpen);
		m_btnCardOpen.Show(false);
	}
	
	// ����, ���� ��ư(��� ������ �Ʒ�)
	m_btnSInOut.Init(m_pParent, m_pPage, m_pUIInfoData->m_listButtonPos[EBT_SINOUT].x, m_pUIInfoData->m_listButtonPos[EBT_SINOUT].y, &m_sprUIBottomBtn, BTN_PARTICIPATION_INDEX, IDM_EMERGE);
	m_btnSInOut.SetMouseClickSound(SND_BUTTON_CLICK_A);
	m_ctrlBtnList.Add(&m_btnSInOut);

	// ������ ��ư(��� ������ �Ʒ�)
	m_btnExitBottom.Init(m_pParent, m_pPage, m_pUIInfoData->m_listButtonPos[EBT_BOTTOM_EXIT].x, m_pUIInfoData->m_listButtonPos[EBT_BOTTOM_EXIT].y, &m_sprUIBottomBtn, 3, IDM_EXIT_BOTTOM);
	m_btnExitBottom.SetMouseClickSound(SND_BUTTON_CLICK_C);
	m_ctrlBtnList.Add(&m_btnExitBottom);

	m_btnPokerShop.Init(m_pParent, m_pPage, m_pUIInfoData->m_listButtonPos[EBT_POKERSHOP].x, m_pUIInfoData->m_listButtonPos[EBT_POKERSHOP].y, &m_sprUIBottomBtn, 0, IDM_POKER_SHOP);
	m_ctrlBtnList.Add(&m_btnPokerShop);

	m_btnInGameBadUserComplain.Init(m_pParent, m_pPage, m_pUIInfoData->m_listButtonPos[EBT_COMPLAIN].x, m_pUIInfoData->m_listButtonPos[EBT_COMPLAIN].y, &m_sprComplain, 1, IDM_COMPLAIN);
	m_ctrlBtnList.Add(&m_btnInGameBadUserComplain);

	//// ������ ��ư
	//m_btnMoneyWarning.Init(m_pParent, m_pPage, m_pUIInfoData->m_listButtonPos[EBT_LOSSMONEYWARNING].x, m_pUIInfoData->m_listButtonPos[EBT_LOSSMONEYWARNING].y, &m_sprLossMoneyBtn, 0, IDM_LOSSMONEY_WARNING);
	//m_ctrlBtnList.Add(&m_btnMoneyWarning);
	//m_btnMoneyWarningClose.Init(m_pParent, m_pPage, m_pUIInfoData->m_listButtonPos[EBT_LOSSMONEYWARNINGCLOSE].x, m_pUIInfoData->m_listButtonPos[EBT_LOSSMONEYWARNINGCLOSE].y, &m_sprLossMoneyBtn2, 0, IDM_LOSSMONEY_WARNINGCLOSE);
	//m_ctrlBtnList.Add(&m_btnMoneyWarningClose);
	
	//if (GM().GetCurrentGameType() == IDX_GAME_SD)
	//{
	//	m_btnMoneyWarning.MoveWindow(m_pUIInfoData->m_listButtonPos[EBT_LOSSMONEYWARNING].x+55, m_pUIInfoData->m_listButtonPos[EBT_LOSSMONEYWARNING].y);
	//	m_btnMoneyWarningClose.MoveWindow(m_pUIInfoData->m_listButtonPos[EBT_LOSSMONEYWARNINGCLOSE].x+55, m_pUIInfoData->m_listButtonPos[EBT_LOSSMONEYWARNINGCLOSE].y);
	//} 
	//else
	//{
	//	m_btnMoneyWarning.MoveWindow(m_pUIInfoData->m_listButtonPos[EBT_LOSSMONEYWARNING].x, m_pUIInfoData->m_listButtonPos[EBT_LOSSMONEYWARNING].y);
	//	m_btnMoneyWarningClose.MoveWindow(m_pUIInfoData->m_listButtonPos[EBT_LOSSMONEYWARNINGCLOSE].x, m_pUIInfoData->m_listButtonPos[EBT_LOSSMONEYWARNINGCLOSE].y);
	//}

	//m_btnMoneyWarning.Show(FALSE);
	//m_btnMoneyWarningClose.Show(FALSE);

	//������ ����ϱ� ��ư (m_listButtonPos���� ������� ������ ��ǥ�� ����Ѵ�)
	m_btnWaitUser.Init(m_pParent, m_pPage, m_pUIInfoData->m_listButtonPos[EBT_WAITUSER].x, m_pUIInfoData->m_listButtonPos[EBT_WAITUSER].y, &m_sprUIBottomBtn, 7, IDM_WAIT_USER);
	m_btnWaitUser.SetMouseClickSound(SND_BUTTON_CLICK_A);
	m_ctrlBtnList.Add(&m_btnWaitUser);
	
	m_btnWaitUser.Show(FALSE);
	
	//������ �����ϱ� ��ư
	m_btnGameStart_HL.Init(m_pParent, m_pPage, 785, 594, &m_sprBtn_gamestart_HL, 0, IDM_START_HL);
	m_btnGameStart_SD.Init(m_pParent, m_pPage, 785, 594, &m_sprBtn_gamestart_SD, 0, IDM_START_SD);
	m_btnGameStart_BD.Init(m_pParent, m_pPage, 785, 594, &m_sprBtn_gamestart_BD, 0, IDM_START_BD);
	m_btnGameStart_NP.Init(m_pParent, m_pPage, 785, 594, &m_sprBtn_gamestart_NP, 0, IDM_START_NP);
	m_btnGameStart_SP.Init(m_pParent, m_pPage, 785, 594, &m_sprBtn_gamestart_SP, 0, IDM_START_SP);

	m_ctrlBtnList.Add(&m_btnGameStart_HL);
	m_ctrlBtnList.Add(&m_btnGameStart_SD);
	m_ctrlBtnList.Add(&m_btnGameStart_BD);
	m_ctrlBtnList.Add(&m_btnGameStart_NP);
	m_ctrlBtnList.Add(&m_btnGameStart_SP);

	m_btnGameStart_HL.Show(FALSE);
	m_btnGameStart_SD.Show(FALSE);
	m_btnGameStart_BD.Show(FALSE);
	m_btnGameStart_NP.Show(FALSE);
	m_btnGameStart_SP.Show(FALSE);
	//=======================================================================

	m_btnGameStart.Show(FALSE);

	if (m_bInculdeBetttingBtn)
	{
		m_btnDie.Init(m_pParent, m_pPage, m_pUIInfoData->m_listButtonPos[EBT_DIE_H].x, m_pUIInfoData->m_listButtonPos[EBT_DIE_H].y, &m_sprBetBtn_H, 0, IDM_DIE);
		m_ctrlBetBtnList.Add(&m_btnDie);
		m_btnCheck.Init(m_pParent, m_pPage, m_pUIInfoData->m_listButtonPos[EBT_CHECK_H].x, m_pUIInfoData->m_listButtonPos[EBT_CHECK_H].y, &m_sprBetBtn_H, 1, IDM_CHECK);
		m_ctrlBetBtnList.Add(&m_btnCheck);
		m_btnCall.Init(m_pParent, m_pPage, m_pUIInfoData->m_listButtonPos[EBT_CALL_H].x, m_pUIInfoData->m_listButtonPos[EBT_CALL_H].y, &m_sprBetBtn_H, 2, IDM_CALL);
		m_ctrlBetBtnList.Add(&m_btnCall);
		m_btnPing.Init(m_pParent, m_pPage, m_pUIInfoData->m_listButtonPos[EBT_PING_H].x, m_pUIInfoData->m_listButtonPos[EBT_PING_H].y, &m_sprBetBtn_H, 3, IDM_PING);
		m_ctrlBetBtnList.Add(&m_btnPing);

		//������ : �����ư ��ſ� Ǯ��ư ����
		if (true==GM().IsChangeFullDDang()){
			m_btnFull.Init(m_pParent, m_pPage, m_pUIInfoData->m_listButtonPos[EBT_DADANG_H].x, m_pUIInfoData->m_listButtonPos[EBT_DADANG_H].y, &m_sprBetBtn_H, 7, IDM_FULL);
			m_ctrlBetBtnList.Add(&m_btnFull);
		}
		else{
			m_btnDadang.Init(m_pParent, m_pPage, m_pUIInfoData->m_listButtonPos[EBT_DADANG_H].x, m_pUIInfoData->m_listButtonPos[EBT_DADANG_H].y, &m_sprBetBtn_H, 4, IDM_DADANG);
			m_ctrlBetBtnList.Add(&m_btnDadang);
		}

		m_btnHalf.Init(m_pParent, m_pPage, m_pUIInfoData->m_listButtonPos[EBT_HALF_H].x, m_pUIInfoData->m_listButtonPos[EBT_HALF_H].y, &m_sprBetBtn_H, 5, IDM_HALF);
		m_ctrlBetBtnList.Add(&m_btnHalf);

		m_btnBDRuleToolTip.Init(m_pParent, m_pPage, 713, 492, &m_sprBetBtn_H, 53, IDM_BDRULE_TIP, 1);
		m_ctrlBtnList.Add(&m_btnBDRuleToolTip);
	}
	
	// ��ư�� ���ñݾ� �׸���
	CRect rt = m_pUIInfoData->m_rtBtnBetText;

	int i = 0;
	
	for ( i = 0; i < NT_ALL; i++ )
	{
		m_pNTMoney[ i ] = new NMBASE::SKINGDI::CNText( rt, "", i, m_pPage, pView->m_pDC );
		ASSERT(m_pNTMoney[ i ] != NULL);
		m_pNTMoney[ i ]->SetAlign( DT_CENTER | DT_WORDBREAK | DT_SINGLELINE | DT_VCENTER);
		m_pNTMoney[ i ]->SetColor( m_pUIInfoData->m_clrBtnBetText );
		m_pNTMoney[ i ]->SetFont( &NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL) );
		m_pNTMoney[ i ]->SetOutLine( m_pUIInfoData->m_clrBtnBetTextShadow );
	}

	SetBetBtnType(g_Config.eBetBtnType, true); 
}

void CGameViewButtonMan::Reset(void)
{
	//MakeHalfBtnAllinBtn(FALSE); 
}

void CGameViewButtonMan::ResetGame()
{
	//�ٵ��� ��ʸ�Ʈ ����Ʈ ����
	m_eSmallBetStep = LIMIT_BET_STEP_1;
	m_eBigBetStep = LIMIT_BET_STEP_2;
	m_nLastRound = 0;
	
	MakeHalfBtnAllinBtn(FALSE);

	//������ : ������ �ʿ��ϸ� Ǯ��ư�� �̿��Ѵ�->�����۾�
	if (true==GM().IsChangeFullDDang()){
		MakeFullBtnAllinBtn(FALSE);
	}
	else{
		MakeDadangBtnAllinBtn(FALSE); 
	}	
}

void CGameViewButtonMan::SetLimitBetType(LIMIT_BET_TYPE eLimitBetType)
{
	m_eLimitBetType = eLimitBetType;

	if (m_eLimitBetType==LIMIT_BET_TYPE_FINAL)
		MakeSmallBtnAllinBtn(TRUE);
}

void CGameViewButtonMan::OnChangeRound()
{
	//�ٵ��� ��ʸ�Ʈ ����Ʈ ����

	if (m_nLastRound != g_Poker.nRound)
	{
		//�ٵ��� ��ʸ�Ʈ ����Ʈ ����
		m_eSmallBetStep = LIMIT_BET_STEP_1;
		m_eBigBetStep = LIMIT_BET_STEP_2;
		m_nLastRound=g_Poker.nRound;
	}
}

void CGameViewButtonMan::SetOverImageIndex(int Cmd, int nButtonNumber )
{

	m_btnHalf.SetOverImageIndex( nButtonNumber );
}

BOOL CGameViewButtonMan::OnLButtonDown(int x, int y)
{
	if (m_cmbBetBtn.OnLButtonDown(x,y))
		return TRUE;
	
	if (m_ctrlBtnList.OnLButtonDown(x,y))
		return TRUE;

	//�ڵ������϶� ��ư Ŭ���� ���ϰ� ��
	if ( g_Poker.nAutoBetCmd <= 0 )
	{
		if (m_ctrlBetBtnList.OnLButtonDown(x,y))
			return TRUE;
	}
	
	if (m_btnGameStart.OnLButtonDown(x,y))
		return TRUE;

	return FALSE;
}

BOOL CGameViewButtonMan::OnLButtonUp(int x, int y)
{
	if (m_cmbBetBtn.OnLButtonUp(x,y))
		return TRUE;

	if (m_ctrlBtnList.OnLButtonUp(x,y))
		return TRUE;

	//�ڵ������϶� ��ư Ŭ���� ���ϰ� ��
	if ( g_Poker.nAutoBetCmd <= 0 )
	{
		if (m_ctrlBetBtnList.OnLButtonUp(x,y))
			return TRUE;
	}
	
	if (m_btnGameStart.OnLButtonUp(x,y))
		return TRUE;

	return FALSE;
}

BOOL CGameViewButtonMan::OnMouseMove(int x, int y)
{
	if (m_bInculdeBetttingBtn)
	{
		if (m_cmbBetBtn.OnMouseMove(x,y))
			return TRUE;
	}
	
	if (m_ctrlBtnList.OnMouseMove(x,y))
	{
		SetCursor(GM().GetCurHandCursor());
		return TRUE;
	}
	
	if (m_ctrlBetBtnList.OnMouseMove(x,y))
	{
		SetCursor(GM().GetCurHandCursor());
		return TRUE;
	}
	
	if (m_btnGameStart.OnMouseMove(x,y))
	{
		SetCursor(GM().GetCurHandCursor());
		return TRUE;
	}
	
	return FALSE;
}

void CGameViewButtonMan::DrawBtn(CDC *pDC)
{
	if (GM().GetCurrentWhere() != IDX_GAMEWHERE_GAME)
		return;

	BOOL bShowBDTooltip = GetShowBDTooltip() && m_eBtnDirectionType != CONFIG_BETBTN_TYPE_H;
	// ������ �۾�. // �ʴ� ��� ��� �ٸ��� �ٷΰ���� ����
	if (2<=g_RI.cMemberRoom){
		m_btnGotoOtherRoom.Show(FALSE);
		m_btnGotoOtherRoom.Enable(FALSE);
		m_btnObserverView.Show(FALSE);
		m_btnObserverView.Enable(FALSE);
		m_btnSInOut.Show(FALSE);
		m_btnSInOut.Enable(FALSE);
		m_btnExitBottom.Show(FALSE);
		m_btnExitBottom.Enable(FALSE);
		m_btnPokerShop.Show(FALSE);
		m_btnPokerShop.Enable(FALSE);
	}
	else{
		m_btnGotoOtherRoom.Show(g_RI.cafeID_==0);
		m_btnGotoOtherRoom.Enable(g_RI.cafeID_==0 && !bShowBDTooltip);
		m_btnObserverView.Show(TRUE);
		m_btnObserverView.Enable(!bShowBDTooltip);
		m_btnSInOut.Show(TRUE);
		m_btnSInOut.Enable(TRUE);
		m_btnExitBottom.Show(TRUE);
		m_btnExitBottom.Enable(TRUE);
		m_btnPokerShop.Show(TRUE);
		m_btnPokerShop.Enable(!bShowBDTooltip);
		m_cmbBetBtn.Show(!(GetShowBDTooltip() && m_eBtnDirectionType == CONFIG_BETBTN_TYPE_H));
	}
	//P type���� ���� ��ư�׸��� (�׳� ��Ȱ��ȭ�� �׷��ش�.) - ����� ���� �ʴ´�.
	if (m_eBtnDirectionType==CONFIG_BETBTN_TYPE_P && m_btnHalf.IsShow())
	{
		if (m_pPage)
		{
			m_pPage->PutSprAuto(m_pUIInfoData->m_listButtonPos[EBT_QUARTER_P].x, m_pUIInfoData->m_listButtonPos[EBT_QUARTER_P].y, &m_sprBetBtn_P, 55);
		}
	}

	//m_ctrlBtnList.Draw(pDC);
	m_btnGameStart.Draw(pDC);

	CGameProcess *pProcess = GM().GetCurrentProcess();
	CPlayer *pPlayer = pProcess->GetPlayer(0);
	int nMyIndex = pPlayer->ServPNum;

	// ��������� �����ư ��Ȱ��
	if (pProcess->m_nCurPlayer != nMyIndex && (g_RI.ChipKind == 1 || m_eBtnDirectionType==CONFIG_BETBTN_TYPE_H))
	{
		m_btnDie.Enable(FALSE);
		m_btnCheck.Enable(FALSE);
		m_btnCall.Enable(FALSE);
		m_btnPing.Enable(FALSE);
		m_btnDadang.Enable(FALSE);
		m_btnFull.Enable(FALSE);
		m_btnHalf.Enable(FALSE);
		m_bShowNTM[ NT_DIE ] = false;
		m_bShowNTM[ NT_PNG ] = false;
		m_bShowNTM[ NT_CHK ] = false;
		m_bShowNTM[ NT_CAL ] = false;
		m_bShowNTM[ NT_DAD ] = false;
		m_bShowNTM[ NT_HAF ] = false;
		m_bFold = FALSE;
		m_bBCheck = FALSE;
		m_bBCall = FALSE;
		m_bBPing = FALSE;
		m_bBDDa = FALSE;
		m_bBMax = FALSE;
	}

	m_ctrlBetBtnList.Draw(pDC);

	//������ ���� ��ư Ŭ���� �� �׵θ� Ȱ��ȭ (�����ư�� Ǯ��ư���� �̹��� ��ü)
	if (m_pPage)
	{
		switch(m_eBtnDirectionType){
			case  CONFIG_BETBTN_TYPE_N:
				if ( m_bBCall && GM().GetCurrentProcess()->IsReservedButton(RESERVESTATE_BET_BET_CMD_CALL) )
					m_pPage->PutSprAuto(m_pUIInfoData->m_listButtonPos[EBT_CALL_N].x, m_pUIInfoData->m_listButtonPos[EBT_CALL_N].y, &m_sprBetBtn_N, 52);
				if ( m_bFold && GM().GetCurrentProcess()->IsReservedButton(RESERVESTATE_BET_BET_CMD_FOLD) )
					m_pPage->PutSprAuto(m_pUIInfoData->m_listButtonPos[EBT_DIE_N].x, m_pUIInfoData->m_listButtonPos[EBT_DIE_N].y, &m_sprBetBtn_N, 52);
				if (true==GM().IsChangeFullDDang()){
					if ( m_bBDDa && GM().GetCurrentProcess()->IsReservedButton(RESERVESTATE_BET_BET_CMD_FULL) )
						m_pPage->PutSprAuto(m_pUIInfoData->m_listButtonPos[EBT_DADANG_N].x, m_pUIInfoData->m_listButtonPos[EBT_DADANG_N].y, &m_sprBetBtn_N, 52);
				}
				else{
					if ( m_bBDDa && GM().GetCurrentProcess()->IsReservedButton(RESERVESTATE_BET_BET_CMD_DADANG) )
						m_pPage->PutSprAuto(m_pUIInfoData->m_listButtonPos[EBT_DADANG_N].x, m_pUIInfoData->m_listButtonPos[EBT_DADANG_N].y, &m_sprBetBtn_N, 52);
				}
				if ( m_bBMax && GM().GetCurrentProcess()->IsReservedButton(RESERVESTATE_BET_BET_CMD_HALF) )
					m_pPage->PutSprAuto(m_pUIInfoData->m_listButtonPos[EBT_HALF_N].x, m_pUIInfoData->m_listButtonPos[EBT_HALF_N].y, &m_sprBetBtn_N, 52);
				break;


			case  CONFIG_BETBTN_TYPE_H:
				if ( m_bBCall && GM().GetCurrentProcess()->IsReservedButton(RESERVESTATE_BET_BET_CMD_CALL) )
					m_pPage->PutSprAuto(m_pUIInfoData->m_listButtonPos[EBT_CALL_H].x, m_pUIInfoData->m_listButtonPos[EBT_CALL_H].y, &m_sprBetBtn_H, 52);
				if ( m_bFold && GM().GetCurrentProcess()->IsReservedButton(RESERVESTATE_BET_BET_CMD_FOLD) )
					m_pPage->PutSprAuto(m_pUIInfoData->m_listButtonPos[EBT_DIE_H].x, m_pUIInfoData->m_listButtonPos[EBT_DIE_H].y, &m_sprBetBtn_H, 52);
				if (true==GM().IsChangeFullDDang()){
					if ( m_bBDDa && GM().GetCurrentProcess()->IsReservedButton(RESERVESTATE_BET_BET_CMD_FULL) )
						m_pPage->PutSprAuto(m_pUIInfoData->m_listButtonPos[EBT_DADANG_H].x, m_pUIInfoData->m_listButtonPos[EBT_DADANG_H].y, &m_sprBetBtn_H, 52);
				}
				else{
					if ( m_bBDDa && GM().GetCurrentProcess()->IsReservedButton(RESERVESTATE_BET_BET_CMD_DADANG) )
						m_pPage->PutSprAuto(m_pUIInfoData->m_listButtonPos[EBT_DADANG_H].x, m_pUIInfoData->m_listButtonPos[EBT_DADANG_H].y, &m_sprBetBtn_H, 52);
				}
				if ( m_bBMax && GM().GetCurrentProcess()->IsReservedButton(RESERVESTATE_BET_BET_CMD_HALF) )
					m_pPage->PutSprAuto(m_pUIInfoData->m_listButtonPos[EBT_HALF_H].x, m_pUIInfoData->m_listButtonPos[EBT_HALF_H].y, &m_sprBetBtn_H, 52);
				break;


			case  CONFIG_BETBTN_TYPE_P:
				if ( m_bBCall && GM().GetCurrentProcess()->IsReservedButton(RESERVESTATE_BET_BET_CMD_CALL) )
					m_pPage->PutSprAuto(m_pUIInfoData->m_listButtonPos[EBT_CALL_P].x, m_pUIInfoData->m_listButtonPos[EBT_CALL_P].y, &m_sprBetBtn_P, 56);
				if ( m_bFold && GM().GetCurrentProcess()->IsReservedButton(RESERVESTATE_BET_BET_CMD_FOLD) )
					m_pPage->PutSprAuto(m_pUIInfoData->m_listButtonPos[EBT_DIE_P].x, m_pUIInfoData->m_listButtonPos[EBT_DIE_P].y, &m_sprBetBtn_P, 56);
				if (true==GM().IsChangeFullDDang()){
					if ( m_bBDDa && GM().GetCurrentProcess()->IsReservedButton(RESERVESTATE_BET_BET_CMD_FULL) )
						m_pPage->PutSprAuto(m_pUIInfoData->m_listButtonPos[EBT_DADANG_P].x, m_pUIInfoData->m_listButtonPos[EBT_DADANG_P].y, &m_sprBetBtn_P, 56);
				}
				else{
					if ( m_bBDDa && GM().GetCurrentProcess()->IsReservedButton(RESERVESTATE_BET_BET_CMD_DADANG) )
						m_pPage->PutSprAuto(m_pUIInfoData->m_listButtonPos[EBT_DADANG_P].x, m_pUIInfoData->m_listButtonPos[EBT_DADANG_P].y, &m_sprBetBtn_P, 56);
				}
				if ( m_bBMax && GM().GetCurrentProcess()->IsReservedButton(RESERVESTATE_BET_BET_CMD_HALF) )
					m_pPage->PutSprAuto(m_pUIInfoData->m_listButtonPos[EBT_HALF_P].x, m_pUIInfoData->m_listButtonPos[EBT_HALF_P].y, &m_sprBetBtn_P, 57);
				break;
		}
	}

	//������ : ��ư Ÿ���� �ٲ�� �ƽ�üũ�Ͽ� �ƽ���ư���� Ȱ��
	SetShowBetMony();

	for ( int i = 0; i < NT_ALL; i++ )
	{
		if ( m_pNTMoney [ i ] && m_bShowNTM[ i ] )
		{
			// �������̴�
			if (pProcess->m_nCurPlayer == nMyIndex )
			{
				//���ñݾ� ǥ��
				m_pNTMoney [ i ]->DrawText( pDC );
			}else{
#if defined(_DEBUG)
				//���ñݾ� ǥ��
				m_pNTMoney [ i ]->DrawText( pDC );
#else
				//������ �����Ͽ� �������� ��� ���� ����
				if (true==GM().IsChangeFullDDang()){
					if (m_bBDDa && GM().GetCurrentProcess()->IsReservedButton(RESERVESTATE_BET_BET_CMD_FULL))
					{
						m_pNTMoney [ NT_DAD ]->SetText("���� ��");
						m_pNTMoney [ NT_DAD ]->DrawText( pDC );
					}else if (m_bBDDa )
					{
						m_pNTMoney [ NT_DAD ]->SetText("�����ϱ�");
						m_pNTMoney [ NT_DAD ]->DrawText( pDC );
					}
				}
				else{
					if (m_bBDDa && GM().GetCurrentProcess()->IsReservedButton(RESERVESTATE_BET_BET_CMD_DADANG))
					{
						m_pNTMoney [ NT_DAD ]->SetText("���� ��");
						m_pNTMoney [ NT_DAD ]->DrawText( pDC );
					}else if (m_bBDDa )
					{
						m_pNTMoney [ NT_DAD ]->SetText("�����ϱ�");
						m_pNTMoney [ NT_DAD ]->DrawText( pDC );
					}
				}
				if (m_bBMax && GM().GetCurrentProcess()->IsReservedButton(RESERVESTATE_BET_BET_CMD_HALF))
				{
					m_pNTMoney [ NT_HAF ]->SetText("���� ��");
					m_pNTMoney [ NT_HAF ]->DrawText( pDC );
				}else if (m_bBMax ) 
				{
					m_pNTMoney [ NT_HAF ]->SetText("�����ϱ�");
					m_pNTMoney [ NT_HAF ]->DrawText( pDC );
				}

				if (m_bBCall && GM().GetCurrentProcess()->IsReservedButton(RESERVESTATE_BET_BET_CMD_CALL))
				{
					m_pNTMoney [ NT_CAL ]->SetText("���� ��");
					m_pNTMoney [ NT_CAL ]->DrawText( pDC );
				}else if (m_bBCall )
				{
					m_pNTMoney [ NT_CAL ]->SetText("�����ϱ�");
					m_pNTMoney [ NT_CAL ]->DrawText( pDC ); 
				}

				if (m_bFold && GM().GetCurrentProcess()->IsReservedButton(RESERVESTATE_BET_BET_CMD_FOLD))
				{
					m_pNTMoney [ NT_DIE ]->SetText("���� ��");
					m_pNTMoney [ NT_DIE ]->DrawText( pDC );
				}else if (m_bFold )
				{
					m_pNTMoney [ NT_DIE ]->SetText("�����ϱ�");
					m_pNTMoney [ NT_DIE ]->DrawText( pDC );
				}
#endif
			}
		}
	}
	if (GM().GetCurrentGameType() != IDX_GAME_HA){ 

		//������ : ���Ӻ� ���۹�ư�� �ٸ���, �����¸� Ȱ��, ������ ��쿡 Ȱ��, �濡 �����ڰ� 2�� �̻��� ��� Ȱ��
		if (true == m_bIsWaitUser && (strcmp(g_RI.ID, GM().GetMyInfo()->UI.ID) == 0) && g_RI.NowUserNum == 2 && g_RI.State == 0 && 2>g_RI.cMemberRoom)
		{
			m_pPage->PutSprAuto(715, 546, &m_sprGamestartBack, 0, AB);

			switch( GM().GetCurrentGameType() )
			{
			case IDX_GAME_SP:
				m_btnGameStart_SP.Show(TRUE);
				break;
			case IDX_GAME_NP:
				m_btnGameStart_NP.Show(TRUE);
				break;
			case IDX_GAME_BD:
				m_btnGameStart_BD.Show(TRUE);
				break;
			case IDX_GAME_SD:
				m_btnGameStart_SD.Show(TRUE);
				break;
			case IDX_GAME_HL:
				m_btnGameStart_HL.Show(TRUE);
				break;
			}
		}else{
			m_btnGameStart_HL.Show(FALSE);
			m_btnGameStart_SD.Show(FALSE);
			m_btnGameStart_BD.Show(FALSE);
			m_btnGameStart_NP.Show(FALSE);
			m_btnGameStart_SP.Show(FALSE);
		}

		//������ : ����ư -> ���̾Ƹ�� ȸ���̰� ���� Ȱ��ȭ
		if ( GM().GetMyInfo()->UI.PremMenuNo == CODE_NEW_FAMILY_DIAMOND && (strcmp(g_RI.ID, GM().GetMyInfo()->UI.ID) == 0) && g_RI.NowUserNum <= 2 && 2>g_RI.cMemberRoom ){
			m_btnWaitUser.Show(TRUE);
			m_btnWaitUser.Enable(TRUE);
		}else{
			m_btnWaitUser.Show(FALSE);
			m_btnWaitUser.Enable(FALSE);

			// Test LDH
			//m_btnWaitUser.Show(TRUE);
			//m_btnWaitUser.Enable(TRUE);
		} 

		//������ : ��� ���� �ȳ� �̹���, ����߿����� ����
		if (true == m_bIsWaitUser && g_RI.State == 0 && 2>g_RI.cMemberRoom )
		{ 
			m_pPage->PutSprAuto(m_pUIInfoData->m_ptWaitAlertPos.x, m_pUIInfoData->m_ptWaitAlertPos.y, &m_sprWaitUserAlert, 0);
		}
	}

	m_ctrlBtnList.Draw(pDC);//������ : �����ϱ� ��ư�� �߰��Ǿ� ���ù�ư �Ʒ��� �̵�, ���͹�ư�� �������� �Ʒ��� �̵�

	if (m_bInculdeBetttingBtn)
	{
		//���ù�ư���� �޺��׸���
		if (m_pPage)
			m_pPage->PutSprAuto(m_pUIInfoData->m_rtBetBtnCombo.left, m_pUIInfoData->m_rtBetBtnCombo.top, &m_sprBetBtnCombo, 3);

		m_cmbBetBtn.Draw(pDC);
	}

	if (2>g_RI.cMemberRoom){
		//������ ���� ǥ��
		CString Observer_str;
		Observer_str.Format("%d", g_RI.NowObserverNum);

		CRect rtRect = m_pUIInfoData->m_rtObserverCount;
		rtRect.OffsetRect(m_pUIInfoData->m_listButtonPos[EBT_OBSERVERVIEW] + m_pUIInfoData->m_ptObserverCountGab);

		pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(m_pUIInfoData->m_clrObserverCount);
		pDC->DrawText(Observer_str, Observer_str.GetLength(), &rtRect, DT_SINGLELINE | DT_VCENTER | DT_RIGHT);
	}
	if (true==GM().IsShowFullExplain()){
		if (m_pPage){
			m_pPage->PutSprAuto(707, 475, &m_sprShowFullExplain, 0, AB);
		}
	}
	static DWORD dwOldTime=timeGetTime();
	DWORD dwTime=timeGetTime();

	if (0<m_nExitSubScript){
		m_nExitSubScript-=(int)(dwTime-dwOldTime);
		if (0>=m_nExitSubScript){
			m_nExitSubScript=0;
		}
	}

	if(GetShowBDTooltip())
	{
		if (m_pPage)
		{
			switch(m_eBtnDirectionType){
				case  CONFIG_BETBTN_TYPE_N:
					m_pPage->DrawMultiScene(0, 0, &m_sprBetBtn_N, 0, dwTime);
					break;
				case  CONFIG_BETBTN_TYPE_H:
					m_pPage->DrawMultiScene(0, 0, &m_sprBetBtn_H, 0, dwTime);
					break;
				case  CONFIG_BETBTN_TYPE_P:
					m_pPage->DrawMultiScene(0, 0, &m_sprBetBtn_P, 0, dwTime);
					break;
			}
		}
	}	
	m_btnBDRuleToolTip.Enable(GetShowBDTooltip());

	dwOldTime=dwTime;
}

void CGameViewButtonMan::SetChangeDisplayButton(bool bFull)
{
	if (bFull)
	{
		m_btnChangeDisplay.SetSprite(&m_sprUITopBtn,6);// ### [ ������� ] ###
	}
	else
	{
		m_btnChangeDisplay.SetSprite(&m_sprUITopBtn,4);// ### [ ������� ] ###
	}
}

void CGameViewButtonMan::SetLastCmdBackup( RAISE_LAST_CMD eLastCmd, bool bReSet )
{
	if ( eLastCmd < RAISE_LAST_CMD_LIMIT_1X ) return;

	eLIMIT_X eLimitX = GetLimitMultiflierWithLastCmd(eLastCmd);
	LIMIT_BET_STEP eBetStep = GetLimitBetBtnStep(g_Poker.nRound, eLimitX);

	if (GetBigBetBtnStep() <= eBetStep)
	{
		IncLimitBetBtnStep();//���ñݾ��� �� �ܰ� ��
	}
	
	if (eBetStep==LIMIT_BET_STEP_3)
	{
		//�������� ��Ȱ��ȭ��Ű��

		//������ �������� �����ư�� ������� �� Ǯ��ư���� ��ü 2014.01 -> �����۾�
		if (true==GM().IsChangeFullDDang()){
			m_btnFull.Enable(FALSE);
		}
		else{
			m_btnDadang.Enable(FALSE);
		}
		m_bBDDa = FALSE;
		m_bShowNTM[ NT_DAD ] = false;
	}
}

int  CGameViewButtonMan::GetLastCmdBackup()
{
	return m_nLastCmdBackup;
} 


//���ù�ư�� bDisable�� ���� Ȱ��/��Ȱ�� �Ѵ�.
void CGameViewButtonMan::SetBettinBtnDiable(bool bDisable)
{
	//������ : �ο�ٵ��� ����ڵ�(ī��ٲٱ� ��ư ��ο� �Ŀ� �� �Լ��� �� �� ȣ�� �Ѵ�.)
	if (m_bChangePassState)
	{
		m_bChangePassState = false;

		return;
	}

	ClearFlag();
	ResetMaxBtn();

	OnChangeRound();//���� üũ
	CGameProcess *pProcess = GM().GetCurrentProcess();
	CPlayer *pPlayer = pProcess->GetPlayer(0);

	//������ �۾� �����ư ��� 2014.01
	// ### [ ������� ] ###
	if ( !g_bMyObserver && pPlayer->PlayState == 1 )
	{
		int nMyIndex = pPlayer->ServPNum;

		RAISEUSER ru;
		memset(&ru, 0, sizeof(RAISEUSER));
		memcpy(&ru, &g_Poker.RU[nMyIndex], sizeof(RAISEUSER));

		//if (ru.nRemainRaiseCount < 1 )
		//{
		//	ShowReserveBtn(false); 
		//}else
		//{
			// ī�� ���� ���� ���� ������ �����ư ������ ����. ( ī�� ���ÿ� ������� ������ ����)
			if (GM().GetCurrentGameType() == IDX_GAME_SD)
			{
				//������ : ���� �������� ���� ��� ���� ��ư Ȱ��
				if (GM().GetCurrentProcess()->CanReserve() && g_RI.State == 1)
				{
					ShowReserveBtn(); // ���� ��ư Ȱ��ȭ.
				}
				else
				{
					ShowReserveBtn(false); 
				}
			}
			else if (GM().GetCurrentGameType() == IDX_GAME_BD)
			{ 

				if ( true == GM().GetCurrentProcess()->CheckRoundForCardSet())
				{
					//������ : ���� �������� ���� ��� ���� ��ư Ȱ��
					bool bb = GM().GetCurrentProcess()->CanReserve();

					if (GM().GetCurrentProcess()->CanReserve() && g_RI.State == 1)
					{
						ShowReserveBtn(); // ���� ��ư Ȱ��ȭ.
					}
					else
					{
						ShowReserveBtn(false); 
					}
				}
			}
			else
			{
				if ( true == GM().GetCurrentProcess()->CheckReceivedAllCards())
				{
					//������ : ���� �������� ���� ��� ���� ��ư Ȱ��
					if (GM().GetCurrentProcess()->CanReserve() && g_RI.State == 1)
					{
						ShowReserveBtn(); // ���� ��ư Ȱ��ȭ.
					}
					else
					{
						ShowReserveBtn(false); 
					}
				}
			}
		//}
	}
	else
	{
		ShowReserveBtn(false); 
	}

	if (bDisable)
	{
		return;
	}
	
	// ### [ ������� ] ###
	if ( !m_bBtnClick && !g_bMyObserver && pPlayer->PlayState == 1 )
	{
		int nMyIndex = pPlayer->ServPNum;

		RAISEUSER ru;
		memset(&ru, 0, sizeof(RAISEUSER));

		// �������̴�
		if (pProcess->m_nCurPlayer == nMyIndex )
		{ 
			//������ �����Ͽ� ����� �����ư �������
			ShowReserveBtn(false); 

			//�ڵ� ������ �ƴҶ��� �ڽſ� �Ͽ��� �Ҹ��� ���ش�.
			//������ ī��������� �������϶��� �� ���� ����.
			if (GM().GetCurrentGameType() == IDX_GAME_BD)
			{
				if (pProcess->CheckRoundForCardSet())
				{
					if (g_Poker.nAutoBetCmd <= 0)
						PBPlayEffectSound(SND_MYTURN);
				}
			}
			else
			{
				if (pProcess->CheckReceivedAllCards())
				{
					if (g_Poker.nAutoBetCmd <= 0)
						PBPlayEffectSound(SND_MYTURN);
				}
			}

			memcpy(&ru, &g_Poker.RU[nMyIndex], sizeof(RAISEUSER));

			if (ru.bFold)
				return;

			INT64 nTotal = pProcess->GetRealBet();
			INT64 nRaise = pProcess->GetRaiseBat();
			INT64 nCall = nRaise - ru.nNowBat;

			if (g_Poker.nAutoBetNodie > 0)
			{
				m_btnDie.Enable(FALSE);
				m_bFold = FALSE;
			}
			else
			{
				m_btnDie.Enable(TRUE);
				m_bFold = TRUE;

				//������ : ���� ������ ��ư �׸��� Ÿ������ ����
				if (GM().GetCurrentProcess()->IsReservedButton(RESERVESTATE_BET_BET_CMD_FOLD))
				{
					GM().GetCurrentProcess()->SendDieButton();

					return;
				}
			}

			if (g_Poker.bDistCard)
			{
				m_btnCheck.Enable(TRUE);
				m_bBCheck = TRUE;
			}
			else
			{
				if (g_Poker.bBtnCheck)
				{
					m_btnCheck.Enable(TRUE);
					m_bBCheck = TRUE;
				}
				else
				{
					m_btnCall.Enable(TRUE);
					m_bBCall = TRUE;
					m_bShowNTM[ NT_CAL ] = true;

					//������ : ���� ������ ��ư �׸��� Ÿ������ ����
					if (GM().GetCurrentProcess()->IsReservedButton(RESERVESTATE_BET_BET_CMD_CALL))
					{
						GM().GetCurrentProcess()->SendCallButton();

						return;
					}
				}
			}

			BOOL bOutable = FALSE;

			if (g_Poker.bBtnBet == FALSE && !g_Poker.bDistCard)
			{
				RunReservedBet();

				return; 
			}

			if (ru.bCall == FALSE)
			{ 
				// ����������..���ð���
				//if (ru.nPMoney > nRaise)
				if (ru.nPMoney > nCall)
				{
					//��������ñݾ��� 0���϶�
					if ( nRaise==0 )
					{
						m_btnPing.Enable(TRUE);
						m_bBPing = TRUE;
						m_bShowNTM[ NT_PNG ] = true;
					}

					// �Ѹ��̶� ���������̶��...�Ѵ�
					if ((ru.nPMoney > nRaise) && (nRaise > 0) && (!g_Poker.bDistCard))
					{ 
						//������ �������� �����ư ������� �� Ǯ��ư���� ��ü�� �Ʒ� �ּ�ó�� 2014.01->�����۾�
						if (true==GM().IsChangeFullDDang()){
						}
						else{
							m_btnDadang.Enable(TRUE);
							m_bBDDa = TRUE;
							m_bShowNTM[ NT_DAD ] = true;
						}

						//������ : ���� ������ ��ư �׸��� Ÿ������ ����
						if (true==GM().IsChangeFullDDang()){
						}
						else{
							if (GM().GetCurrentProcess()->IsReservedButton(RESERVESTATE_BET_BET_CMD_DADANG)){
								// �Ӵϰ� ��� �ƽ��������� �ٲ� �׳� ����Ǿ��ִ� ��ư�� Ȱ��ȭ�Ǿ������� (������ �ƽ����·� �ٲ� ���翹���� �Ǿ��־��ٸ� �ƽ��� ���̽� ����)
								 GM().GetCurrentProcess()->SendDdaButton();
								 return;
							}
						}
					}
					if ( ru.nPMoney >= pProcess->GetMaxMoney(nTotal))
					{
						m_btnHalf.Enable(TRUE);
						m_bBMax = TRUE;
						m_bShowNTM[ NT_HAF ] = true;

						//������ : ���� ������ ��ư �׸��� Ÿ������ ����
						if (GM().GetCurrentProcess()->IsReservedButton(RESERVESTATE_BET_BET_CMD_HALF))
						{
							/*if (CheckReservedBetting(nBetHalfMoney))
							{
								GM().GetCurrentProcess()->SetReserveButton(RESERVESTATE_BET_NONE);
							}else{
								GM().GetCurrentProcess()->SendHalfButton();
							}*/

							// �Ӵϰ� ��� �ƽ��������� �ٲ� �׳� ����Ǿ��ִ� ��ư�� Ȱ��ȭ�Ǿ������� (������ �ƽ����·� �ٲ� ���������� �Ǿ��־��ٸ� �ƽ��� ���̽� ����)
							GM().GetCurrentProcess()->SendHalfButton();
							return;
						}
						if ( ru.nPMoney >= pProcess->GetMaxMoneyForFull(nTotal)){
							int spnum = pProcess->GetServPNum_ByPN(0);
							//Ǯ��ư�� ������ư�� ���� Ȱ��ȭ �Ѵ�.
							if (true==GM().IsChangeFullDDang()){
								m_btnFull.Enable(TRUE);
								m_bBDDa = TRUE;
								m_bShowNTM[ NT_DAD ] = true;
							}
							// ���� ģ����忡�� �ƽ����� �����ϸ� ù ���ÿ��� ��ư Ȱ��ȭ 2018.05.15
							else if(g_RI.bIsFriendRoom && g_Poker.RU[spnum].nPMoney >= GM().GetLossMaxMoneyForGame())
							{
								m_btnDadang.Enable(TRUE);
								m_bBDDa = TRUE;
								m_bShowNTM[ NT_DAD ] = true;
							}
							if (GM().GetCurrentProcess()->IsReservedButton(RESERVESTATE_BET_BET_CMD_FULL))
							{
								GM().GetCurrentProcess()->SendDdaButton();
								return;
							}
						}
					}
					else 
					{
						if ( ru.nPMoney > 0 )
						{
							MakeHalfBtnAllinBtn(TRUE); //���� ��ư�� ���ι�ư���� ��ü�Ѵ�.

							m_btnHalf.Enable(TRUE);
							m_bBMax = TRUE;
							m_bShowNTM[ NT_HAF ] = true;

							//������ : ���� ������ ��ư �׸��� Ÿ������ ����
							if (GM().GetCurrentProcess()->IsReservedButton(RESERVESTATE_BET_BET_CMD_HALF))
							{									
								// ���� ���� ���¿��ٸ�.. �����̴��� ���� ���� 
								GM().GetCurrentProcess()->SendHalfButton();

								return;
							}

							//Ǯ��ư�� ������ư�� ���� Ȱ��ȭ �Ѵ�.
							if (true==GM().IsChangeFullDDang()){
								m_btnFull.Enable(FALSE);
								m_bBDDa = FALSE;
								m_bShowNTM[ NT_DAD ] = false;
							}
						}
					}
				}
			} 

			//������ �����ؼ� ī�弼�� ó��
			switch(GM().GetCurrentGameType())
			{
				case IDX_GAME_SP:
				case IDX_GAME_HL:
				case IDX_GAME_NP:
					if ( false == GM().GetCurrentProcess()->CheckReceivedAllCards())
					{
						if (m_bBCall)
						{
							GM().GetCurrentProcess()->SendCallButton();
						}
						else if (m_bBCheck)
						{
							GM().GetCurrentProcess()->SendCheckButton();
						}
					}
					break;
				case IDX_GAME_BD:
					bool state = GM().GetCurrentProcess()->CheckRoundForCardSet();
					if ( false == state )
					{
						if (m_bBCall)
						{
							GM().GetCurrentProcess()->SendCallButton();
						}
						else if (m_bBCheck)
						{
							GM().GetCurrentProcess()->SendCheckButton();
						}
					}
					break; 
			}
		} // end of my turn
	}
	
	SetShowBetMony();
	FirstBetCheck();
}

// ��ħ ���� ���� üũ �Լ�
void CGameViewButtonMan::FirstBetCheck()
{
	if(IsFirstBet())
	{
		// ù ������ ���� �� ����
		m_btnCheck.Enable(FALSE);
		m_btnPing.Enable(FALSE);
		m_btnDadang.Enable(FALSE);
		m_btnFull.Enable(FALSE);
		m_bShowNTM[ NT_CHK ] = false;
		m_bShowNTM[ NT_PNG ] = false;
		m_bShowNTM[ NT_DAD ] = false;
		m_bBCheck = FALSE;
		m_bBPing = FALSE;
		m_bBDDa = FALSE;
	}
}

// ù ���� ����
BOOL CGameViewButtonMan::IsFirstBet()
{
	INT64 curSeedMoney = g_RI.CurSeedMoney;
	CPlayer *pPlayer = GM().GetCurrentProcess()->GetPlayer(0);

	if (g_Poker.RU[pPlayer->ServPNum].nTotBat > curSeedMoney)
		return FALSE;

	if (g_RI.bIsFriendRoom)
		return FALSE;

	// ���� �Ӵϰ� 0���϶��� üũ��ư ����
	if (GetMaxMoney(nBetHalfMoney, pPlayer->ServPNum) == 0)
		return FALSE;

	switch(GM().GetCurrentGameType())
	{
	case IDX_GAME_SP:
		{
			if (g_RI.ChipKind == 0 && (curSeedMoney < 10ll*MONEY_UK || curSeedMoney > 500ll*MONEY_UK))	
				return FALSE;

			if (g_RI.ChipKind == 1)	
				return FALSE;
		}
		break;
	case IDX_GAME_NP:
		{
			if (g_RI.ChipKind == 0 && (curSeedMoney < 10ll*MONEY_UK || curSeedMoney > 500ll*MONEY_UK))	
				return FALSE;

			if (g_RI.ChipKind == 1 && g_RI.llEnterLimitMoney < 300000)		
				return FALSE;
		}
		break;
	case IDX_GAME_HL:
		if (g_RI.ChipKind == 0 && (curSeedMoney < 10ll*MONEY_UK || curSeedMoney > 700ll*MONEY_UK))
			return FALSE;
		break;
	case IDX_GAME_BD:
		{
			if (g_RI.ChipKind == 0 && (curSeedMoney < 10ll*MONEY_UK))	
				return FALSE;

			// ���� ����ε庣������ ��ü ����.
			//if (g_RI.ChipKind == 1 && g_RI.llEnterLimitMoney < 300000)	
			//	return FALSE;
		}
		break;
	case IDX_GAME_SD:
		{
			if (g_RI.ChipKind == 1 && g_RI.llEnterLimitMoney < 100000)	
				return FALSE;

			if (g_RI.ChipKind == 0)	
				return FALSE;
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}
	
void CGameViewButtonMan::RunReservedBet()
{
	CGameProcess *pProcess = GM().GetCurrentProcess();
	CPlayer *pPlayer = pProcess->GetPlayer(0);
	int nMyIndex = pPlayer->ServPNum;

	if (pProcess->m_nCurPlayer != nMyIndex )
	{
		return;
	}
	if (true==GM().IsChangeFullDDang()){
		if ( m_bBDDa && GM().GetCurrentProcess()->IsReservedButton(RESERVESTATE_BET_BET_CMD_FULL))
		{
			if (CheckReservedBetting(nBetDDaMoney))
			{
				GM().GetCurrentProcess()->SetReserveButton(RESERVESTATE_BET_NONE);
			}else{
				GM().GetCurrentProcess()->SendDdaButton();
			}
		}
	}
	else{
		if ( m_bBDDa && GM().GetCurrentProcess()->IsReservedButton(RESERVESTATE_BET_BET_CMD_DADANG))
		{
			if (CheckReservedBetting(nBetDDaMoney))
			{
				GM().GetCurrentProcess()->SetReserveButton(RESERVESTATE_BET_NONE);
			}else{
				GM().GetCurrentProcess()->SendDdaButton();
			}
		}
	}
	if ( m_bBMax && GM().GetCurrentProcess()->IsReservedButton(RESERVESTATE_BET_BET_CMD_HALF))
	{
		if (CheckReservedBetting(nBetHalfMoney))
		{
			GM().GetCurrentProcess()->SetReserveButton(RESERVESTATE_BET_NONE);
		}else{
			GM().GetCurrentProcess()->SendHalfButton();
		}
	}

	if ( m_bBCall && GM().GetCurrentProcess()->IsReservedButton(RESERVESTATE_BET_BET_CMD_CALL))
	{
		GM().GetCurrentProcess()->SendCallButton();
	}

	if ( m_bFold && GM().GetCurrentProcess()->IsReservedButton(RESERVESTATE_BET_BET_CMD_FOLD))
	{
		GM().GetCurrentProcess()->SendDieButton();
	}
}

void CGameViewButtonMan::ShowReserveBtn(bool bShow)
{
	if (true == bShow && g_RI.State == 1) // ���� ��ư Ȱ��
	{
		CGameProcess *pProcess = GM().GetCurrentProcess();
		CPlayer *pPlayer = pProcess->GetPlayer(0);

		int nMyIndex = pPlayer->ServPNum;

		RAISEUSER ru;
		memset(&ru, 0, sizeof(RAISEUSER));

		memcpy(&ru, &g_Poker.RU[nMyIndex], sizeof(RAISEUSER));

		//����, �ο�ٵ��̴� �ݵ� üũ�ؾ��ϴ°�? �� call�� ��������???
		if (ru.bFold /*|| ru.bCall*/) return;

		////�ο�ٵ��̴� ù ���ÿ��� 0�� �ü�����.
		//if (GM().GetCurrentGameType() != IDX_GAME_BD && GM().GetCurrentGameType() != IDX_GAME_NP && GM().GetCurrentGameType() != IDX_GAME_SD)
		//{ 
		//	//���̽� ���� Ƚ���� 0�̸�..
		//	if (ru.nRemainRaiseCount < 0) return;
		//}

		m_btnDie.Enable(TRUE);
		m_bFold = TRUE;

		m_btnCall.Enable(TRUE);
		m_bBCall = TRUE;
		m_bShowNTM[ NT_CAL ] = TRUE;

		//�����̸� ������ ������ ���� ����
		if (ru.nPMoney > 0)
		{
			if (true==GM().IsChangeFullDDang()){
				m_btnFull.Enable(TRUE);
			}
			else{
				m_btnDadang.Enable(TRUE);//������ �������� �����ư�� Ǯ��ư���� ��ü 2014.01 -> �����۾�
			}
			m_bBDDa = TRUE;
			m_bShowNTM[ NT_DAD ] = TRUE;

			m_btnHalf.Enable(TRUE);
			m_bBMax = TRUE;
			m_bShowNTM[ NT_HAF ] = TRUE;
		}

		//�ƽ��̸� ������ ������ ���� ����
		if (ru.nPMoney <= 0 || CheckMaxFlag())
		{
			if (true==GM().IsChangeFullDDang()){
				m_btnFull.Enable(FALSE);
			}
			else{
				m_btnDadang.Enable(FALSE);
			}
			m_bBDDa = FALSE;
			m_bShowNTM[ NT_DAD ] = FALSE;

			m_btnHalf.Enable(FALSE);
			m_bBMax = FALSE;
			m_bShowNTM[ NT_HAF ] = FALSE;
		}

	}
	else // ���� ��ư ��Ȱ��
	{
		//������ Ǯ��ư ��� -> ���Ĺݿ�
		if (true==GM().IsChangeFullDDang()){
			m_btnFull.Enable(bShow);
			m_bBDDa = bShow;
			m_bShowNTM[ NT_DAD ] = bShow;
		}
		else{
			m_btnDadang.Enable(bShow);
			m_bBDDa = bShow;
			m_bShowNTM[ NT_DAD ] = bShow;
		}

		m_btnCall.Enable(bShow);
		m_bBCall = bShow;
		m_bShowNTM[ NT_CAL ] = bShow;

		m_btnHalf.Enable(bShow);
		m_bBMax = bShow;
		m_bShowNTM[ NT_HAF ] = bShow;

		m_btnDie.Enable(bShow);
		m_bFold = bShow;
	}
}

//������ : �ƽ� ���� üũ
bool CGameViewButtonMan::CheckMaxFlag()
{
	//������ �ƽ����� üũ���� �ʴ´�.
	if (g_RI.ChipKind == 1)
	{
		return false;
	}

	CGameProcess *pProcess = GM().GetCurrentProcess();
	int spnum = pProcess->GetServPNum_ByPN(0);
	INT64 lossMoney = -1 * GM().GetLossMoney();
	INT64 TotalBetMoney = g_Poker.RU[spnum].nRealBat;
	INT64 resultMoney = lossMoney + TotalBetMoney;
	//yoo [20150120]
	if ((INT64)GM().GetRestraintInfo().BasicLossMoneyLimit == (INT64)GM().GetLossMaxMoney() && GM().GetEnableOver() == false)
	{
		/*if (resultMoney >= (INT64)MAX_MONEY)*/
		if (resultMoney >= (INT64)GM().GetLossMaxMoney())
		{
			return true;
		}
	}

	return false;
}

//������ : �ƽ��ݾװ� �սǱݾ� üũ
bool CGameViewButtonMan::CheckReservedBetting(const INT64 bettingMoney)
{
	//������ �ƽ����� üũ���� �ʴ´�.
	if (g_RI.ChipKind == 1)
	{
		return false;
	}

	CGameProcess *pProcess = GM().GetCurrentProcess();
	int spnum = pProcess->GetServPNum_ByPN(0);
	INT64 lossMoney = -1 * GM().GetLossMoney();
	INT64 TotalBetMoney = g_Poker.RU[spnum].nRealBat;
	INT64 resultMoney = lossMoney + TotalBetMoney + bettingMoney;

	//yoo [20150120]
	if ((INT64)GM().GetRestraintInfo().BasicLossMoneyLimit == (INT64)GM().GetLossMaxMoney() && GM().GetEnableOver() == false)
	{
		/*if (resultMoney > (INT64)MAX_MONEY)*/
		if (resultMoney > (INT64)GM().GetLossMaxMoney())		
		{
			return true;
		}
	}

	return false;
}

void CGameViewButtonMan::ClearFlag()
{
	if (GM().GetCurrentWhere() != IDX_GAMEWHERE_GAME)
		return;

	m_bBPing =  m_bBCheck = m_bBCall = m_bBMax = m_bBDDa = m_bFold = FALSE;

	m_btnDie.Enable(FALSE);
	m_btnCheck.Enable(FALSE);
	m_btnCall.Enable(FALSE);
	m_btnPing.Enable(FALSE);
	//������ �������� �����ư�� Ǯ��ư���� ��ü�� �Ʒ� �ּ� ó�� 2014.01->�����۾�
	if (true==GM().IsChangeFullDDang()){
		m_btnFull.Enable(FALSE);
	}
	else{
		m_btnDadang.Enable(FALSE);
	}
	m_btnHalf.Enable(FALSE);
	memset( m_bShowNTM, 0, sizeof( m_bShowNTM ));
}

void CGameViewButtonMan::EnterRoom_Button_Init( int bBetRuleKind )
{
	m_btnExit.Enable(TRUE);

//	if ( bBetRuleKind == BETRULE_KIND_REAL )
//	{
		m_btnDie.SetOverImageUseFlag( FALSE );
		m_btnCheck.SetOverImageUseFlag( FALSE );
		m_btnCall.SetOverImageUseFlag( FALSE );
		m_btnPing.SetOverImageUseFlag( FALSE );

		//������ �������� �����ư�� Ǯ��ư���� ��ü�� �ּ� 2014.01->�����۾�
		if (true==GM().IsChangeFullDDang()){
			m_btnFull.SetOverImageUseFlag( FALSE );
		}
		else{
			m_btnDadang.SetOverImageUseFlag( FALSE );
		}

		m_btnHalf.SetOverImageUseFlag( FALSE );

	//������ : ������, �����ÿ� ����� ���� ���θ� �˻�
	SetShowBetMony();

}

void CGameViewButtonMan::SetBetBtnType(CONFIG_BETBTN_TYPE eBtnDirectionType, bool bForceType/* = false*/)
{
	if (m_pUIInfoData == NULL) return; //�ʱ�ȭ�� �ȵǾ������� �����Ѵ�.
	if (!bForceType && (m_eBtnDirectionType == eBtnDirectionType)) return;

	NMBASE::GRAPHICGDI::xSprite *pSpr = NULL; 	
	NMBASE::GRAPHICGDI::xSprite *pLimitBtnSpr = NULL;
	NMBASE::GRAPHICGDI::xSprite *pMoneyBtnSpr = NULL;

	if ( eBtnDirectionType == CONFIG_BETBTN_TYPE_H)
	{
		pSpr = &m_sprBetBtn_H;
	}
	else if ( eBtnDirectionType == CONFIG_BETBTN_TYPE_P)
	{
		pSpr = &m_sprBetBtn_P;
	}
	else
	{
		pSpr = &m_sprBetBtn_N;
	}
 
	if ( pSpr == NULL )
	{
		const int ErrLog = 101;//���ù�ư �ε� ����
		NMBASE::UTIL::WriteProfile_int(GM().GetMyInfo()->UI.ID, "ErrLog", ErrLog);
		return;
	}

	m_eBtnDirectionType = eBtnDirectionType;
	SetKeyDirect( m_eBtnDirectionType );

//	if ( g_RI.sBetRuleKind == BETRULE_KIND_REAL )
//	{
// 		m_btnDie.SetOverImageUseFlag( FALSE );
// 		m_btnCheck.SetOverImageUseFlag( FALSE );
// 		m_btnCall.SetOverImageUseFlag( FALSE );
// 		m_btnPing.SetOverImageUseFlag( FALSE );
// 		m_btnDadang.SetOverImageUseFlag( FALSE );
// 		m_btnHalf.SetOverImageUseFlag( FALSE );


		ENUM_BUTTON_TYPE eBtnType_DIE;
		ENUM_BUTTON_TYPE eBtnType_CHECK;
		ENUM_BUTTON_TYPE eBtnType_CALL;
		ENUM_BUTTON_TYPE eBtnType_PING;
		ENUM_BUTTON_TYPE eBtnType_DADANG;
		ENUM_BUTTON_TYPE eBtnType_HALF;
		
		if (m_eBtnDirectionType==CONFIG_BETBTN_TYPE_H)
		{
			eBtnType_DIE = EBT_DIE_H;
			eBtnType_CHECK = EBT_CHECK_H;
			eBtnType_CALL = EBT_CALL_H;
			eBtnType_PING = EBT_PING_H;
			eBtnType_DADANG = EBT_DADANG_H;
			eBtnType_HALF = EBT_HALF_H;
		}
		else if (m_eBtnDirectionType==CONFIG_BETBTN_TYPE_P)
		{
			eBtnType_DIE = EBT_DIE_P;
			eBtnType_CHECK = EBT_CHECK_P;
			eBtnType_CALL = EBT_CALL_P;
			eBtnType_PING = EBT_PING_P;
			eBtnType_DADANG = EBT_DADANG_P;
			eBtnType_HALF = EBT_HALF_P;
		}
		else
		{
			eBtnType_DIE = EBT_DIE_N;
			eBtnType_CHECK = EBT_CHECK_N;
			eBtnType_CALL = EBT_CALL_N;
			eBtnType_PING = EBT_PING_N;
			eBtnType_DADANG = EBT_DADANG_N;
			eBtnType_HALF = EBT_HALF_N;
		}

		m_btnDie.SetButtonPos(m_pUIInfoData->m_listButtonPos[eBtnType_DIE].x, m_pUIInfoData->m_listButtonPos[eBtnType_DIE].y);
		m_btnDie.SetSprite(pSpr,0);

		m_btnCheck.SetButtonPos(m_pUIInfoData->m_listButtonPos[eBtnType_CHECK].x, m_pUIInfoData->m_listButtonPos[eBtnType_CHECK].y);
		m_btnCheck.SetSprite(pSpr,1);

		m_btnCall.SetButtonPos(m_pUIInfoData->m_listButtonPos[eBtnType_CALL].x, m_pUIInfoData->m_listButtonPos[eBtnType_CALL].y);
		m_btnCall.SetSprite(pSpr,2);

		m_btnPing.SetButtonPos(m_pUIInfoData->m_listButtonPos[eBtnType_PING].x, m_pUIInfoData->m_listButtonPos[eBtnType_PING].y);
		m_btnPing.SetSprite(pSpr,3);

		//������ Ǯ��ư ����
		int nBtnNo = 0;
		if (true==GM().IsChangeFullDDang()){
			m_btnFull.SetButtonPos(m_pUIInfoData->m_listButtonPos[eBtnType_DADANG].x, m_pUIInfoData->m_listButtonPos[eBtnType_DADANG].y);
			nBtnNo = m_btnFull.GetButtonNumber();
			m_btnFull.SetSprite(pSpr,nBtnNo);
		}
		else{
			m_btnDadang.SetButtonPos(m_pUIInfoData->m_listButtonPos[eBtnType_DADANG].x, m_pUIInfoData->m_listButtonPos[eBtnType_DADANG].y);
			nBtnNo = m_btnDadang.GetButtonNumber();
			m_btnDadang.SetSprite(pSpr,nBtnNo);
		}

		m_btnHalf.SetButtonPos(m_pUIInfoData->m_listButtonPos[eBtnType_HALF].x, m_pUIInfoData->m_listButtonPos[eBtnType_HALF].y);
		nBtnNo = m_btnHalf.GetButtonNumber();
		m_btnHalf.SetSprite(pSpr,nBtnNo);

		m_btnBDRuleToolTip.SetButtonPos(713, eBtnDirectionType == CONFIG_BETBTN_TYPE_H ? 492 : 688);
		m_btnBDRuleToolTip.SetSprite(pSpr,eBtnDirectionType == CONFIG_BETBTN_TYPE_P ? 58 : 53);
		m_btnBDRuleToolTip.Enable(FALSE);

	ReSetPostionNTMoney(); // ��ư�� �Ӵ� �׸���

	if (m_bInculdeBetttingBtn)
	{
		//�޺��ڽ� ����
		m_cmbBetBtn.SetCurSel((int)m_eBtnDirectionType);
	}
}

void CGameViewButtonMan::ReSetPostionNTMoney()
{
	CRect rt;

	if (m_eBtnDirectionType<0 || m_eBtnDirectionType>CONFIG_BETBTN_TYPE_P)
	{
		m_eBtnDirectionType = CONFIG_BETBTN_TYPE_N;
	}

	CPoint ptButtonRightBttom;
	ptButtonRightBttom.x = m_btnHalf.m_Xp + m_btnHalf.m_Width;
	ptButtonRightBttom.y = m_btnHalf.m_Yp + m_btnHalf.m_Height;

	rt = m_pUIInfoData->m_ptBtnMoneyDrawRect;
	rt.OffsetRect(ptButtonRightBttom.x + m_pUIInfoData->m_ptBtnMonyDrawGab[m_eBtnDirectionType].x ,ptButtonRightBttom.y + m_pUIInfoData->m_ptBtnMonyDrawGab[m_eBtnDirectionType].y);
	if ( m_pNTMoney[ NT_HAF ] != NULL )
	{
		m_pNTMoney[ NT_HAF ]->ReSetPostion( rt );
	}

	ptButtonRightBttom.x = m_btnCall.m_Xp + m_btnCall.m_Width;
	ptButtonRightBttom.y = m_btnCall.m_Yp + m_btnCall.m_Height;

	rt = m_pUIInfoData->m_ptBtnMoneyDrawRect;
	rt.OffsetRect(ptButtonRightBttom.x + m_pUIInfoData->m_ptBtnMonyDrawGab[m_eBtnDirectionType].x ,ptButtonRightBttom.y + m_pUIInfoData->m_ptBtnMonyDrawGab[m_eBtnDirectionType].y);
	if ( m_pNTMoney[ NT_CAL ] != NULL )
	{
		m_pNTMoney[ NT_CAL ]->ReSetPostion( rt );
	}

	//������ �������� �����ư�� Ǯ��ư���� ��ü�� �Ʒ� 2�� �ּ� 2014.01->�����۾�
	//������ ���� �Ŀ� Ǯ��ư ��� ����
	if (true==GM().IsChangeFullDDang()){
		ptButtonRightBttom.x = m_btnFull.m_Xp + m_btnFull.m_Width;
		ptButtonRightBttom.y = m_btnFull.m_Yp + m_btnFull.m_Height;
	}
	else{
		ptButtonRightBttom.x = m_btnDadang.m_Xp + m_btnDadang.m_Width;
		ptButtonRightBttom.y = m_btnDadang.m_Yp + m_btnDadang.m_Height;
	}
	rt = m_pUIInfoData->m_ptBtnMoneyDrawRect;
	rt.OffsetRect(ptButtonRightBttom.x + m_pUIInfoData->m_ptBtnMonyDrawGab[m_eBtnDirectionType].x ,ptButtonRightBttom.y + m_pUIInfoData->m_ptBtnMonyDrawGab[m_eBtnDirectionType].y);
	if ( m_pNTMoney[ NT_DAD ] != NULL )
	{
		m_pNTMoney[ NT_DAD ]->ReSetPostion( rt );
	}

	ptButtonRightBttom.x = m_btnPing.m_Xp + m_btnPing.m_Width;
	ptButtonRightBttom.y = m_btnPing.m_Yp + m_btnPing.m_Height;

	rt = m_pUIInfoData->m_ptBtnMoneyDrawRect;
	rt.OffsetRect(ptButtonRightBttom.x + m_pUIInfoData->m_ptBtnMonyDrawGab[m_eBtnDirectionType].x ,ptButtonRightBttom.y + m_pUIInfoData->m_ptBtnMonyDrawGab[m_eBtnDirectionType].y);
	if ( m_pNTMoney[ NT_PNG ] != NULL )
	{
		m_pNTMoney[ NT_PNG ]->ReSetPostion( rt );
	}

	//������ ���� ��ư Fold�� ���� display�߰�
	ptButtonRightBttom.x = m_btnDie.m_Xp + m_btnDie.m_Width;
	ptButtonRightBttom.y = m_btnDie.m_Yp + m_btnDie.m_Height;

	rt = m_pUIInfoData->m_ptBtnMoneyDrawRect;
	rt.OffsetRect(ptButtonRightBttom.x + m_pUIInfoData->m_ptBtnMonyDrawGab[m_eBtnDirectionType].x ,ptButtonRightBttom.y + m_pUIInfoData->m_ptBtnMonyDrawGab[m_eBtnDirectionType].y);
	if ( m_pNTMoney[ NT_DIE ] != NULL )
	{
		m_pNTMoney[ NT_DIE ]->ReSetPostion( rt );
	}
}

void CGameViewButtonMan::SetShowBetMony()
{
	//������ �����Ͽ� �� ��ư�� �ƽ��Ӵ� ��� �񱳸� ���� ����
	INT64 RestMoney[4] = {0,}; //0:call, 1:ping, 2:dadang, 3:half

	INT64 nPreBetMoney = 0;
	CString szMoney = "";

	CGameProcess *pProcess = GM().GetCurrentProcess();
	int spnum = pProcess->GetServPNum_ByPN(0);
	nPreBetMoney = pProcess->GetRaiseBat() - g_Poker.RU[spnum].nNowBat;

	//������ �����Ͽ� �ƽ��� üũ
	szMoney = GetMoneyString( GetMaxMoney(nPreBetMoney, spnum) );
	RestMoney[0] = GetMaxMoney(nPreBetMoney, spnum);
	
	nBetCallMoney = nPreBetMoney;
	if (m_bBCall) m_pNTMoney[ NT_CAL ]->SetText( szMoney );
	else m_pNTMoney[ NT_CAL ]->SetText( "" );
	
	INT64 nTotal = 0;

	nTotal = pProcess->GetRealBet();
	nPreBetMoney = pProcess->GetMaxMoney( nTotal );
	
	//������ �����Ͽ� �ƽ��� üũ
	szMoney = GetMoneyString( GetMaxMoney(nPreBetMoney, spnum) );
	RestMoney[3] = GetMaxMoney(nPreBetMoney, spnum);

	nBetHalfMoney = nPreBetMoney;
	if (m_bBMax) m_pNTMoney[ NT_HAF ]->SetText( szMoney );
	else m_pNTMoney[ NT_HAF ]->SetText( "" );
	
	//���������� ���� �Ʒ� 3�� �ּ� -> �����۾�
	if(g_RI.bIsFriendRoom && g_Poker.RU[spnum].nPMoney >= GM().GetLossMaxMoneyForGame()){
		nPreBetMoney = g_Poker.RU[spnum].nPMoney;
	}
	else if (true==GM().IsChangeFullDDang()){
		nPreBetMoney = pProcess->GetMaxMoneyForFull(nTotal);
	}
	else{
		nPreBetMoney = pProcess->m_i64Double; //�߰�
	}

	szMoney = GetMoneyString( GetMaxMoney(nPreBetMoney, spnum) );
	RestMoney[2] = GetMaxMoney(nPreBetMoney, spnum);
	
	nBetDDaMoney = nPreBetMoney;
	if (m_bBDDa) m_pNTMoney[ NT_DAD ]->SetText( szMoney );
	else m_pNTMoney[ NT_DAD ]->SetText( "" );
	
	nPreBetMoney = g_RI.CurSeedMoney;
	
	//������ �����Ͽ� �ƽ��� üũ
	szMoney = GetMoneyString( GetMaxMoney(nPreBetMoney, spnum) );
	RestMoney[1] = GetMaxMoney(nPreBetMoney, spnum);

	nBetPingMoney = nPreBetMoney;
	if (m_bBPing) m_pNTMoney[ NT_PNG ]->SetText( szMoney ); 
	else m_pNTMoney[ NT_PNG ]->SetText( "" );

	//���������� ������ Ǯ��ư���� �Ǹ鼭 �ݾ�
	nTotal = pProcess->GetRealBet();
	nPreBetMoney = pProcess->GetMaxMoneyForFull( nTotal );

	////������ �����Ͽ�  �ƽ� ��ư���� ����
	//0:call, 1:ping, 2:dadang, 3:half
	int BUTTON_ID = 0;
	CPlayer *pPlayer = pProcess->GetPlayer(0);
	int nMyIndex = pPlayer->ServPNum;

	//�ּ� ��ư�� �ƽ� ���� üũ
	INT64 maxMoney  = 0;
	INT64 llCheckMoney=0ll;
	GM().GetLimitToCheckMoney(GM().GetCurrentGameArray(), spnum, llCheckMoney);
	m_bMaxState = false;

	INT64 curSeedMoney = g_RI.CurSeedMoney;

	if (IsFirstBet())
	{
		m_bBPing = false;
		m_bBDDa = false;
		m_pNTMoney[ NT_HAF ]->SetText( GetMoneyString( GetMaxMoney(nBetHalfMoney, spnum) ) );
	}

	for(int i=0; i<4; i++)
	{
		if ( RestMoney[i] >= llCheckMoney )
		{
			switch( i )
			{
				case 0:
					if ( !m_bBCall ) continue;
					break;
				case 1:
					if ( !m_bBPing ) continue;
					break;
				case 2:
					if ( !m_bBDDa ) continue;
					break;
				case 3:
					if ( !m_bBMax ) continue;
					break;
			}
			BUTTON_ID = i;
			maxMoney = RestMoney[i] +  g_Poker.RU[nMyIndex].nRealBat;
			m_bMaxState = true;
			break;
		}
	}

	//0:call, 1:ping, 2:dadang, 3:half
	// ������ ���� ���� �� �ƽ���ư üũ�� ������ ����.
	if ( pPlayer->PlayState == 1 )
	{
		if ( m_bMaxState )
		{
			if (g_RI.ChipKind != 1) MakeMaxBtn(BUTTON_ID, m_bMaxState , maxMoney);
		}
		else
		{
			if (g_RI.ChipKind != 1) MakeMaxBtn(-1, m_bMaxState, maxMoney);
		}
	}
}

//������ �����Ͽ� �ƽ��� üũ
INT64 CGameViewButtonMan::GetMaxMoney( INT64 nMoney, int spnum )
{
	INT64 nPreBetMoney = 0;
	nPreBetMoney = nMoney;
	m_bMaxState = false;

	if (g_RI.ChipKind == 1)
	{
		// ����üũ �߰�. ��� �ӴϺ��� �����Ӵϰ� ������ �����ӴϷ� ������.
		return nPreBetMoney>g_Poker.RU[spnum].nPMoney?g_Poker.RU[spnum].nPMoney:nPreBetMoney;
	}

	INT64 llBetMoney=0ll;
	GM().GetLimitToBetMoney(GM().GetCurrentGameArray(), spnum, nPreBetMoney, llBetMoney);

	return llBetMoney;
}

void CGameViewButtonMan::SetSmallBetBtnStep(LIMIT_BET_STEP eLimitBetStep)
{
	m_eSmallBetStep = eLimitBetStep;
}

void CGameViewButtonMan::SetBigBetBtnStep(LIMIT_BET_STEP eLimitBetStep)
{
	m_eBigBetStep = eLimitBetStep;
}

LIMIT_BET_STEP CGameViewButtonMan::GetSmallBetBtnStep()
{
	return m_eSmallBetStep;
}

LIMIT_BET_STEP CGameViewButtonMan::GetBigBetBtnStep()
{
	return m_eBigBetStep;
}

eLIMIT_X CGameViewButtonMan::GetBigBtnLimitMultiplier()
{
	///���� ���� ��ư�� ���
	return GetLimitMultiflierWithBetStep(g_Poker.nRound, GetBigBetBtnStep());;
}

eLIMIT_X CGameViewButtonMan::GetSmallBtnLimitMultiplier()
{
	///���� �������� ��ư�� ���
	return GetLimitMultiflierWithBetStep(g_Poker.nRound, GetSmallBetBtnStep()); 	
}

BET_CMD CGameViewButtonMan::GetBigBetBtnCmd(INT64 llMyMoney)
{
	CGameProcess *pProcess = GM().GetCurrentProcess();

	if (pProcess==NULL)
		return BET_CMD_LIMIT_1X;//����

	if (llMyMoney < pProcess->GetLimitRaiseMoney(GetBigBtnLimitMultiplier()) )
	{
		return BET_CMD_ALLIN;
	}

	eLIMIT_X eLimitX = GetBigBtnLimitMultiplier();  ///���� ������ ���

	return GetBetCmdWithLimitMultiflier( eLimitX );
}

BET_CMD CGameViewButtonMan::GetSmallBetBtnCmd(INT64 llMyMoney)
{
	if (m_eLimitBetType==LIMIT_BET_TYPE_FINAL)	//����� ��� ���������� ���ι�ư���ιٲ��
	{
		return BET_CMD_ALLIN;
	}
	
	CGameProcess *pProcess = GM().GetCurrentProcess();

	if (pProcess==NULL)
		return BET_CMD_LIMIT_1X; //����

	if (llMyMoney < pProcess->GetLimitRaiseMoney(GetSmallBtnLimitMultiplier()) )
	{
		return BET_CMD_ALLIN;
	}
	
	//���� ���������� ���
	eLIMIT_X eLimitX = GetSmallBtnLimitMultiplier();
	
	return GetBetCmdWithLimitMultiflier( eLimitX );
}

//������ư�� �ܰ踦 �Ѵܰ� �ø���. (����1->����2, ����2->����)
void CGameViewButtonMan::IncLimitBetBtnStep()
{
	
	if (m_eBigBetStep >= MAX_LIMIT_BET_STEP -1)
	{

		return;
	}

	m_eBigBetStep = (LIMIT_BET_STEP) (m_eBigBetStep +1);
	m_eSmallBetStep = (LIMIT_BET_STEP) (m_eSmallBetStep +1);

}


eLIMIT_X CGameViewButtonMan::GetLimitMultiflierWithBetStep(int nRound, LIMIT_BET_STEP eLimitBetStep)
{
	if (nRound<=0 || nRound>=MAX_ROUND)
		return LIMIT_1X;
	if (eLimitBetStep<0 || eLimitBetStep>=MAX_LIMIT_BET_STEP)
		return LIMIT_1X;

	return g_LimitBetMultiflier[nRound][eLimitBetStep];
}

eLIMIT_X CGameViewButtonMan::GetLimitMultiflierWithLastCmd(RAISE_LAST_CMD eLastCmd)
{

	switch(eLastCmd)
	{
	case RAISE_LAST_CMD_LIMIT_1X:  // 1��
		return LIMIT_1X;
	case RAISE_LAST_CMD_LIMIT_2X:  // 2��
		return LIMIT_2X;
	case RAISE_LAST_CMD_LIMIT_3X:  // 3��
		return LIMIT_3X;
	case RAISE_LAST_CMD_LIMIT_5X:  // 5��
		return LIMIT_5X;
	case RAISE_LAST_CMD_LIMIT_10X:  // 10��
		return LIMIT_10X;
	case RAISE_LAST_CMD_LIMIT_20X:  // 20��
		return LIMIT_20X;
	case RAISE_LAST_CMD_LIMIT_30X:  // 30��
		return LIMIT_30X;
	case RAISE_LAST_CMD_LIMIT_40X:  // 40��
		return LIMIT_40X;
	case RAISE_LAST_CMD_LIMIT_60X:  // 60��
		return LIMIT_60X;
	case RAISE_LAST_CMD_LIMIT_80X:  // 80��
		return LIMIT_80X;
	case RAISE_LAST_CMD_LIMIT_100X:  // 100��
		return LIMIT_100X;
	case RAISE_LAST_CMD_LIMIT_120X:  // 120��
		return LIMIT_120X;
	}

	ASSERT(1);///���� enum �� switch���� �߰��ʿ�
	return LIMIT_1X;
}

BET_CMD CGameViewButtonMan::GetBetCmdWithLimitMultiflier(eLIMIT_X eLimitMultiflier)
{
	switch(eLimitMultiflier)
	{
		case LIMIT_1X:  // 1��
			return BET_CMD_LIMIT_1X;
		case LIMIT_2X:  // 2��
			return BET_CMD_LIMIT_2X;
		case LIMIT_3X:  // 3��
			return BET_CMD_LIMIT_3X;
		case LIMIT_5X:  // 5��
			return BET_CMD_LIMIT_5X;
		case LIMIT_10X:  // 10��
			return BET_CMD_LIMIT_10X;
		case LIMIT_20X:  // 20��
			return BET_CMD_LIMIT_20X;
		case LIMIT_30X:  // 30��
			return BET_CMD_LIMIT_30X;
		case LIMIT_40X:  // 40��
			return BET_CMD_LIMIT_40X;
		case LIMIT_60X:  // 60��
			return BET_CMD_LIMIT_60X;
		case LIMIT_80X:  // 80��
			return BET_CMD_LIMIT_80X;
		case LIMIT_100X:  // 100��
			return BET_CMD_LIMIT_100X;
		case LIMIT_120X:  // 120��
			return BET_CMD_LIMIT_120X;
	}

	 ASSERT(1);///�ش� enum�߰��ʿ�
	return BET_CMD_LIMIT_1X;
}

eLIMIT_X CGameViewButtonMan::GetLimitMultiflierWithBetCmd(BET_CMD eBetCmd)
{
	switch(eBetCmd)
	{
	case BET_CMD_LIMIT_1X:  // 1��
		return LIMIT_1X;
	case BET_CMD_LIMIT_2X:  // 2��
		return LIMIT_2X;
	case BET_CMD_LIMIT_3X:  // 3��
		return LIMIT_3X;
	case BET_CMD_LIMIT_5X:  // 5��
		return LIMIT_5X;
	case BET_CMD_LIMIT_10X:  // 10��
		return LIMIT_10X;
	case BET_CMD_LIMIT_20X:  // 20��
		return LIMIT_20X;
	case BET_CMD_LIMIT_30X:  // 30��
		return LIMIT_30X;
	case BET_CMD_LIMIT_40X:  // 40��
		return LIMIT_40X;
	case BET_CMD_LIMIT_60X:  // 60��
		return LIMIT_60X;
	case BET_CMD_LIMIT_80X:  // 80��
		return LIMIT_80X;
	case BET_CMD_LIMIT_100X:  // 100��
		return LIMIT_100X;
	case BET_CMD_LIMIT_120X:  // 120��
		return LIMIT_120X;
	}

	ASSERT(1);///���� enum �� switch���� �߰��ʿ�
	return LIMIT_1X;
}





LIMIT_BET_STEP CGameViewButtonMan::GetLimitBetBtnStep( int nRound , eLIMIT_X eLimitMultiflier )
{
	int index = 0;

	for( int i = 0; i < MAX_LIMIT_BET_STEP; i++ )
	{
		if ( g_LimitBetMultiflier[ nRound ][i] == eLimitMultiflier )
		{
			index = i;
			break;
		}
	}

	return (LIMIT_BET_STEP)index;
}

CString CGameViewButtonMan::GetMoneyString(INT64 nMoney)
{
	CString str = g_GetMoneyString(nMoney);
	str.Append(strChip_Count[g_RI.ChipKind]);

	return str;
}

void CGameViewButtonMan::SetKeyDirect( CONFIG_BETBTN_TYPE eBtnDirectionType )
{
	InitializeKeyValue( eBtnDirectionType  );
}

void CGameViewButtonMan::InitializeKeyValue( CONFIG_BETBTN_TYPE eBtnDirectionType  )
{
	memset( G_KEYDEF, 0, sizeof( G_KEYDEF) );

	// Betting // 0 : ���� 1 : ����
	if ( eBtnDirectionType == CONFIG_BETBTN_TYPE_H)
	{
		//H TYPE ���ù�ư ����Ű (�����е� ����Ű�� ���)

		G_KEYDEF[ KEY_BET_DIE ][ 0 ] = SET_VKEY_NOR( rule::SKEY_8);
		//G_KEYDEF[ KEY_BET_DIE ][ 1 ] = SET_VKEY_EXT( rule::SKEY_GRAY_PGDN );
		G_KEYDEF[ KEY_BET_DIE ][ 2 ] = SET_VKEY_NOR( rule::SKEY_GRAY_UP );  //VK_NUMPAD4 

		G_KEYDEF[ KEY_BET_CHK ][ 0 ] = SET_VKEY_NOR( rule::SKEY_5 );
		//G_KEYDEF[ KEY_BET_CHK ][ 1 ] = SET_VKEY_EXT( rule::SKEY_GRAY_PGUP );
		G_KEYDEF[ KEY_BET_CHK ][ 2 ] = SET_VKEY_NOR( rule::SKEY_GRAY_CENTER );  //VK_NUMPAD8

		G_KEYDEF[ KEY_BET_CAL ][ 0 ] = SET_VKEY_NOR( rule::SKEY_7 );
		//G_KEYDEF[ KEY_BET_CAL ][ 1 ] = SET_VKEY_EXT( rule::SKEY_GRAY_END );
		G_KEYDEF[ KEY_BET_CAL ][ 2 ] = SET_VKEY_NOR( rule::SKEY_GRAY_HOME );  //VK_NUMPAD2

		G_KEYDEF[ KEY_BET_PNG ][ 0 ] = SET_VKEY_NOR( rule::SKEY_4 );
		//G_KEYDEF[ KEY_BET_PNG ][ 1 ] = SET_VKEY_EXT( rule::SKEY_GRAY_INS );
		G_KEYDEF[ KEY_BET_PNG ][ 2 ] = SET_VKEY_NOR( rule::SKEY_GRAY_LEFT );//VK_NUMPAD5

		G_KEYDEF[ KEY_BET_DAD ][ 0 ] = SET_VKEY_NOR( rule::SKEY_1 );
		//G_KEYDEF[ KEY_BET_DAD ][ 1 ] = SET_VKEY_EXT( rule::SKEY_GRAY_HOME );
		G_KEYDEF[ KEY_BET_DAD ][ 2 ] = SET_VKEY_NOR( rule::SKEY_GRAY_END );  //VK_NUMPAD7

		G_KEYDEF[ KEY_BET_HAF ][ 0 ] = SET_VKEY_NOR( rule::SKEY_0 );
		//G_KEYDEF[ KEY_BET_HAF ][ 1 ] = SET_VKEY_EXT( rule::SKEY_GRAY_DEL );
		G_KEYDEF[ KEY_BET_HAF ][ 2 ] = SET_VKEY_NOR( rule::SKEY_GRAY_INS );   //VK_NUMPAD1
	}
	else if ( eBtnDirectionType == CONFIG_BETBTN_TYPE_P )
	{
		//P TYPE ���ù�ư ����Ű (�����е� ����Ű�� ���)

		G_KEYDEF[ KEY_BET_DIE ][ 0 ] = SET_VKEY_NOR( rule::SKEY_PERIOD);
		//G_KEYDEF[ KEY_BET_DIE ][ 1 ] = SET_VKEY_EXT( rule::SKEY_GRAY_PGDN );
		G_KEYDEF[ KEY_BET_DIE ][ 2 ] = SET_VKEY_NOR( rule::SKEY_GRAY_DEL );  //VK_NUMPAD4 

		G_KEYDEF[ KEY_BET_CHK ][ 0 ] = SET_VKEY_NOR( rule::SKEY_8 );
		//G_KEYDEF[ KEY_BET_CHK ][ 1 ] = SET_VKEY_EXT( rule::SKEY_GRAY_PGUP );
		G_KEYDEF[ KEY_BET_CHK ][ 2 ] = SET_VKEY_NOR( rule::SKEY_GRAY_UP );  //VK_NUMPAD8

		G_KEYDEF[ KEY_BET_CAL ][ 0 ] = SET_VKEY_NOR( rule::SKEY_2 );
		//G_KEYDEF[ KEY_BET_CAL ][ 1 ] = SET_VKEY_EXT( rule::SKEY_GRAY_END );
		G_KEYDEF[ KEY_BET_CAL ][ 2 ] = SET_VKEY_NOR( rule::SKEY_GRAY_DOWN );  //VK_NUMPAD2

		G_KEYDEF[ KEY_BET_PNG ][ 0 ] = SET_VKEY_NOR( rule::SKEY_5 );
		//G_KEYDEF[ KEY_BET_PNG ][ 1 ] = SET_VKEY_EXT( rule::SKEY_GRAY_INS );
		G_KEYDEF[ KEY_BET_PNG ][ 2 ] = SET_VKEY_NOR( rule::SKEY_GRAY_CENTER );//VK_NUMPAD5

		G_KEYDEF[ KEY_BET_DAD ][ 0 ] = SET_VKEY_NOR( rule::SKEY_7 );
		//G_KEYDEF[ KEY_BET_DAD ][ 1 ] = SET_VKEY_EXT( rule::SKEY_GRAY_HOME );
		G_KEYDEF[ KEY_BET_DAD ][ 2 ] = SET_VKEY_NOR( rule::SKEY_GRAY_HOME );  //VK_NUMPAD7

		G_KEYDEF[ KEY_BET_HAF ][ 0 ] = SET_VKEY_NOR( rule::SKEY_1 );
		//G_KEYDEF[ KEY_BET_HAF ][ 1 ] = SET_VKEY_EXT( rule::SKEY_GRAY_DEL );
		G_KEYDEF[ KEY_BET_HAF ][ 2 ] = SET_VKEY_NOR( rule::SKEY_GRAY_END );   //VK_NUMPAD1
	}
	else 
	{
		//N TYPE ���ù�ư ����Ű

		G_KEYDEF[ KEY_BET_DIE ][ 0 ] = SET_VKEY_NOR( rule::SKEY_3 );
		G_KEYDEF[ KEY_BET_DIE ][ 1 ] = SET_VKEY_EXT( rule::SKEY_GRAY_PGDN );
		G_KEYDEF[ KEY_BET_DIE ][ 2 ] = SET_VKEY_NOR( rule::SKEY_GRAY_PGDN );  //VK_NUMPAD3 

		G_KEYDEF[ KEY_BET_CHK ][ 0 ] = SET_VKEY_NOR( rule::SKEY_6 );
		G_KEYDEF[ KEY_BET_CHK ][ 1 ] = SET_VKEY_EXT( rule::SKEY_GRAY_PGUP );
		G_KEYDEF[ KEY_BET_CHK ][ 2 ] = SET_VKEY_NOR( rule::SKEY_GRAY_RIGHT ); //VK_NUMPAD6

		G_KEYDEF[ KEY_BET_CAL ][ 0 ] = SET_VKEY_NOR( rule::SKEY_2 );
		G_KEYDEF[ KEY_BET_CAL ][ 1 ] = SET_VKEY_EXT( rule::SKEY_GRAY_END );
		G_KEYDEF[ KEY_BET_CAL ][ 2 ] = SET_VKEY_NOR( rule::SKEY_GRAY_DOWN );  //VK_NUMPAD2

		G_KEYDEF[ KEY_BET_PNG ][ 0 ] = SET_VKEY_NOR( rule::SKEY_4 );
		G_KEYDEF[ KEY_BET_PNG ][ 1 ] = SET_VKEY_EXT( rule::SKEY_GRAY_INS );
		G_KEYDEF[ KEY_BET_PNG ][ 2 ] = SET_VKEY_NOR( rule::SKEY_GRAY_LEFT );  //VK_NUMPAD4 

		G_KEYDEF[ KEY_BET_DAD ][ 0 ] = SET_VKEY_NOR( rule::SKEY_5 );
		G_KEYDEF[ KEY_BET_DAD ][ 1 ] = SET_VKEY_EXT( rule::SKEY_GRAY_HOME );
		G_KEYDEF[ KEY_BET_DAD ][ 2 ] = SET_VKEY_NOR( rule::SKEY_GRAY_CENTER );//VK_NUMPAD5 

		G_KEYDEF[ KEY_BET_HAF ][ 0 ] = SET_VKEY_NOR( rule::SKEY_1 );
		G_KEYDEF[ KEY_BET_HAF ][ 1 ] = SET_VKEY_EXT( rule::SKEY_GRAY_DEL );
		G_KEYDEF[ KEY_BET_HAF ][ 2 ] = SET_VKEY_NOR( rule::SKEY_GRAY_END );   //VK_NUMPAD1 

	}

	// ���� �� ���� ����Ű ���� �����
	G_KEYDEF[ KEY_HIDDEN_OPEN ][ 0 ] = SET_VKEY_EXT( rule::SKEY_GRAY_UP );
	G_KEYDEF[ KEY_HIDDEN_OPEN ][ 1 ] = SET_VKEY_NOR( rule::SKEY_GRAY_PLUS );
	G_KEYDEF[ KEY_HIDDEN_OPEN ][ 2 ] = SET_VKEY_NOR( rule::SKEY_GRAY_DEL );

	G_KEYDEF[ KEY_HIDDEN_SLOW ][ 0 ] = SET_VKEY_EXT( rule::SKEY_GRAY_DOWN ); // ���̱� ����

	

	G_KEYDEF[ KEY_CARD_SELECT_1 ][ 0 ] = SET_VKEY_NOR( rule::SKEY_1 );
	G_KEYDEF[ KEY_CARD_SELECT_1 ][ 1 ] = SET_VKEY_NOR( rule::SKEY_GRAY_END );
	G_KEYDEF[ KEY_CARD_SELECT_1 ][ 2 ] = SET_VKEY_EXT(rule::SKEY_GRAY_DEL );
	G_KEYDEF[ KEY_CARD_SELECT_1 ][ 3 ] = SET_VKEY_NOR( rule::SKEY_F1 );

	G_KEYDEF[ KEY_CARD_SELECT_2 ][ 0 ] = SET_VKEY_NOR( rule::SKEY_2 );
	G_KEYDEF[ KEY_CARD_SELECT_2 ][ 1 ] = SET_VKEY_NOR( rule::SKEY_GRAY_DOWN );
	G_KEYDEF[ KEY_CARD_SELECT_2 ][ 2 ] = SET_VKEY_EXT( rule::SKEY_GRAY_END );
	G_KEYDEF[ KEY_CARD_SELECT_2 ][ 3 ] = SET_VKEY_NOR( rule::SKEY_F2 );

	G_KEYDEF[ KEY_CARD_SELECT_3 ][ 0 ] = SET_VKEY_NOR( rule::SKEY_3 );
	G_KEYDEF[ KEY_CARD_SELECT_3 ][ 1 ] = SET_VKEY_NOR( rule::SKEY_GRAY_PGDN );
	G_KEYDEF[ KEY_CARD_SELECT_3 ][ 2 ] = SET_VKEY_EXT( rule::SKEY_GRAY_PGDN );
	G_KEYDEF[ KEY_CARD_SELECT_3 ][ 3 ] = SET_VKEY_NOR( rule::SKEY_F3 );

	G_KEYDEF[ KEY_CARD_SELECT_4 ][ 0 ] = SET_VKEY_NOR( rule::SKEY_4 );
	G_KEYDEF[ KEY_CARD_SELECT_4 ][ 1 ] = SET_VKEY_NOR( rule::SKEY_GRAY_LEFT );
	G_KEYDEF[ KEY_CARD_SELECT_4 ][ 2 ] = SET_VKEY_EXT( rule::SKEY_GRAY_INS );
	G_KEYDEF[ KEY_CARD_SELECT_4 ][ 3 ] = SET_VKEY_NOR( rule::SKEY_F4 );
}

void CGameViewButtonMan::SetObserverBtn(int nNum)
{
	GDM().SetINT64(GDK_OBSERVER_STEP, 0);

	//1~99������ ���ڴ� ������� �ʴ´�......
	//���� ���°���??????? �̻���..
	if (nNum >= 1 && nNum < 100)
	{
		GDM().SetINT64(GDK_OBSERVER_STEP, 1);
	}
	else
	{
		switch(nNum)
		{
		//������ �������� ��û
		case 100:
			{
				m_btnSInOut.Init(m_pParent, m_pPage, m_pUIInfoData->m_listButtonPos[EBT_SINOUT].x, m_pUIInfoData->m_listButtonPos[EBT_SINOUT].y, &m_sprUIBottomBtn, BTN_PARTICIPATION_INDEX,IDM_PARTICIPATION);
				GDM().SetINT64(GDK_OBSERVER_STEP, 2);
			}
			break;
		//������ �������� ���
		case 200:
			m_btnSInOut.Init(m_pParent, m_pPage, m_pUIInfoData->m_listButtonPos[EBT_SINOUT].x, m_pUIInfoData->m_listButtonPos[EBT_SINOUT].y, &m_sprUIBottomBtn, BTN_PARTICIPATION_INDEX,IDM_EMERGE);
			break;
		//���� �߿� �������� ���
		case 300:
			m_btnSInOut.Init(m_pParent, m_pPage, m_pUIInfoData->m_listButtonPos[EBT_SINOUT].x, m_pUIInfoData->m_listButtonPos[EBT_SINOUT].y, &m_sprUIBottomBtn, BTN_PARTICIPATION_INDEX,IDM_PARTICIPATION);
			break;
		//���� �߿� �������� ��û
		case 400:
			m_btnSInOut.Init(m_pParent, m_pPage, m_pUIInfoData->m_listButtonPos[EBT_SINOUT].x, m_pUIInfoData->m_listButtonPos[EBT_SINOUT].y, &m_sprUIBottomBtn, BTN_PARTICIPATION_INDEX,IDM_PARTICIPATION);
			break;
		//���� �߿� �������� ����
		case 500:
			m_btnSInOut.Init(m_pParent, m_pPage, m_pUIInfoData->m_listButtonPos[EBT_SINOUT].x, m_pUIInfoData->m_listButtonPos[EBT_SINOUT].y, &m_sprUIBottomBtn, BTN_PARTICIPATION_INDEX,IDM_PARTICIPATION);
			break;
		}
	}
}

void CGameViewButtonMan::InitObserverBtn(void)
{
	GDM().SetINT64(GDK_OBSERVER_STEP, 0);
	m_btnSInOut.Init(m_pParent, m_pPage, m_pUIInfoData->m_listButtonPos[EBT_SINOUT].x, m_pUIInfoData->m_listButtonPos[EBT_SINOUT].y, &m_sprUIBottomBtn, BTN_PARTICIPATION_INDEX,IDM_EMERGE);

	m_btnSInOut.Show(TRUE);
	m_btnSInOut.Enable(TRUE);
}

//������ ���� �սǱݾ׿����� ���â ����
void CGameViewButtonMan::SetLossMoneyWarning(bool enterroom) 
{
	//if (m_PreState == GM().GetLossMoneyState()) 
	//	return;
	m_btnMoneyWarning.Show(FALSE);
	m_btnMoneyWarningClose.Show(FALSE);
	return;
	INT64 nLossmoney = GM().GetLossMoney();
	INT64 nCheckMoney = GM().GetRestraintInfo().WarningLossMoney_Room;
	if (nLossmoney<0) nLossmoney = -1*nLossmoney;
	if ( nLossmoney < nCheckMoney)
	{
		m_bIsShowLossMoneyText = false;

		m_btnMoneyWarning.Show(FALSE);
		m_btnMoneyWarningClose.Show(FALSE);
		return;
	}

	m_PreState = GM().GetLossMoneyState();
	if (DAILYLOSS_STATE_NORMAL == m_PreState)
	{
		m_bIsShowLossMoneyText = false;

		m_btnMoneyWarning.Show(FALSE); 
		m_btnMoneyWarningClose.Show(FALSE);

	}else if (DAILYLOSS_STATE_WARNING <= m_PreState)
	{
		if ( nLossmoney > nCheckMoney)
		{
			if (true == enterroom)
			{
				m_bIsShowOnceLossMoneyText = true;
				m_bIsShowLossMoneyText = false;

				m_btnMoneyWarning.Show(TRUE);
				m_btnMoneyWarningClose.Show(FALSE);
			}else
			{
				if (false == m_bIsShowOnceLossMoneyText)
				{ 
					m_bIsShowOnceLossMoneyText = true;
					m_bIsShowLossMoneyText = true;

					m_btnMoneyWarning.Show(FALSE);
					m_btnMoneyWarningClose.Show(TRUE);
				}else
				{
					m_bIsShowLossMoneyText = false;

					m_btnMoneyWarning.Show(TRUE);
					m_btnMoneyWarningClose.Show(FALSE);
				}
			}
		}
	}
}


//������ : ����� �߰�(���̾Ƹ�� ȸ��)
void CGameViewButtonMan::SetWaitFlag(bool isWait)
{
	m_bIsWaitUser = isWait;

	// ��� ��� Ȱ��ȭ�� ���� ä��â ���� �߰�.
	if (true == m_bIsWaitUser)
	{
		CString str;
		if (g_RI.State == 0) // ��� ���� �� Ȱ��ȭ �Ǹ�..
		{
			str.Format("�����ӽ����� ���� ��� [���ӽ���]�� Ŭ�� ���ּ���.\n");
		}else
		{
			str.Format("�̹� ���� ����� �� ��� ���·� ���ϴ�. ���ӽ����� ���� ��� [���ӽ���]�� Ŭ�� ���ּ���.\n");
		}
		AddGameInfoViewEdit(str.GetString(), m_pUIInfoData->m_listChatColor[ECC2_NOTIFY]);
	}
}


BOOL CGameViewButtonMan::OnCommand( WPARAM wParam, LPARAM lParam )
{
	switch(wParam)
	{

		//������ : ����ϱ��ư
	case IDM_WAIT_USER:
		{
			CCL_ASK_WAIT_INROOM msg;
			msg.Set(!m_bIsWaitUser);
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
		}
		break;

	case IDM_START_BD:
	case IDM_START_HL:
	case IDM_START_NP:
	case IDM_START_SD:
	case IDM_START_SP:
		{
			CCL_ASK_WAIT_INROOM msg;
			msg.Set(false);
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
		}
		break;
		
	case IDM_INVITE:
		//������ ��ɺ��� : �ʴ��ϱ� -> �ٸ��� �ٷΰ��� 2014.01
		/*
		#ifdef UPGRADE_10210619_ADD_CLICKLOG
			SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_BETAREA_INVITE);
		#endif //UPGRADE_10210619_ADD_CLICKLOG

		// ���� ���
		{
			CInviteDlg *pInvite = GetInviteDlg();
			if (pInvite)
				pInvite->PopUp();

 		}
		*/

		// ������ �߰� : �ٸ��� �ٷΰ���
		{
			if (0==m_nExitSubScript){
				#ifdef UPGRADE_10210619_ADD_CLICKLOG
					if (wParam == IDM_EXIT)
					{
						SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_TITLEBAR_CLOSE);
					}
					else if (wParam == IDM_EXIT_BOTTOM)
					{
						SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_MYCARDAREA_EXIT);
					}
				#endif //UPGRADE_10210619_ADD_CLICKLOG

				if ((m_pParent->m_bExitSubscript == FALSE) && (GDM().GetINT64(GDK_OBSERVER_STEP) == 2))
				{
					m_pParent->SendMessage(WM_COMMAND, IDM_PARTICIPATION, 0);
				}

				CPlayer *pPlayer = GM().GetMyInfo();

				// ������ �߰�.
				CCL_EXITSUBSCRIPT esmsg;
				esmsg.Set(g_RI.RoomNum, pPlayer->UI.UNum, pPlayer->UI.ID, m_pParent->m_bExitSubscript?2:1, 1);
				NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), esmsg.pData, esmsg.GetTotalSize());
				m_nExitSubScript=1500;
				GM().SetQuickStartInRoom(true);
			}
		}
		break;
	case IDM_POKER_SHOP:
		{
#ifdef _BETAVER
			ShowMainMessageDlg( false , "���� �غ����Դϴ�." );
			break;
#endif
		
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_BETAREA_POKERSHOP);
			#endif //UPGRADE_10210619_ADD_CLICKLOG

			if (g_bMyObserver) ShowGameItemDlg(false);
			else AddGameInfoViewEdit("���� �����߿��� ��Ŀ������ �̿��� �� �����ϴ�. ���� �߿��� �̿��Ͻ� �� �ֽ��ϴ�.\n", m_pUIInfoData->m_listChatColor[ECC2_NOTIFY]); 
		}
		break;
	case IDM_CHANGEDISPLAY:
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_TITLEBAR_WINDOWMODE);
			#endif //UPGRADE_10210619_ADD_CLICKLOG
			// ���� ���
			ChangeDisplayMode();
		}
		break;
	case IDM_MINIMIZE:
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_TITLEBAR_MINIMIZE);
			#endif //UPGRADE_10210619_ADD_CLICKLOG

			GM().GetMainWnd()->ShowWindow(SW_MINIMIZE);
		}
		break;
	case IDM_CAPTURE:
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_TITLEBAR_CAPTURE);
			#endif //UPGRADE_10210619_ADD_CLICKLOG

			SaveScreenCapture();
		} 
		break;
	case IDM_OBSERVERVIEW:
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_BETAREA_OBSERVE_STATUS);
			#endif //UPGRADE_10210619_ADD_CLICKLOG

			CObserverDlg *pObserverDlg = GetObserverDlg();
			//if (!::IsWindow(pObserverDlg->GetSafeHwnd()))
			//{
			//	pObserverDlg->Create(IDD_DLG_OBSERVER, NULL);
			//}

			if (pObserverDlg->IsWindowVisible())	{
				pObserverDlg->SendMessage(WM_COMMAND, IDCANCEL);
				pObserverDlg->CenterWindow(m_pParent);
				m_pChattingWindowMan->SetEditFocus();
			}
			else
			{
				pObserverDlg->ShowWindow(SW_SHOW);
				m_pChattingWindowMan->SetEditFocus();
			}
		} 
		break;
	case IDM_OPTION:
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_TITLEBAR_OPTION);
			#endif //UPGRADE_10210619_ADD_CLICKLOG

			// ���� ���
			GM().ShowOptionDlg();
		} 
		break;
	case IDM_EXIT:
	case IDM_EXIT_BOTTOM:
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				if (wParam == IDM_EXIT)
				{
					SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_TITLEBAR_CLOSE);
				}
				else if (wParam == IDM_EXIT_BOTTOM)
				{
					SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_MYCARDAREA_EXIT);
				}
			#endif //UPGRADE_10210619_ADD_CLICKLOG

			if ((m_pParent->m_bExitSubscript == FALSE) && (GDM().GetINT64(GDK_OBSERVER_STEP) == 2))
			{
				m_pParent->SendMessage(WM_COMMAND, IDM_PARTICIPATION, 0);
			}

			CPlayer *pPlayer = GM().GetMyInfo();

			// ������ �߰�.
			CCL_EXITSUBSCRIPT esmsg;
			esmsg.Set(g_RI.RoomNum, pPlayer->UI.UNum, pPlayer->UI.ID, m_pParent->m_bExitSubscript?2:1, 0);
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), esmsg.pData, esmsg.GetTotalSize());
		}
		break;

	case IDM_LOSSMONEY_WARNING:
		{
			m_btnMoneyWarning.Show(FALSE);
			m_btnMoneyWarningClose.Show(TRUE);

			m_bIsShowLossMoneyText = true;
		}
		break;
	case IDM_LOSSMONEY_WARNINGCLOSE:
		{
			m_btnMoneyWarning.Show(TRUE);
			m_btnMoneyWarningClose.Show(FALSE);
			
			m_bIsShowLossMoneyText = false;
		}
		break;
		
	case IDM_TIPBTN:
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_TITLEBAR_GAMERULE);
			#endif //UPGRADE_10210619_ADD_CLICKLOG

			OpenGameManualPage();
		}
		break;

	case IDM_BETBTNTYPE_COMBO:
		{
			g_Config.eBetBtnType = (CONFIG_BETBTN_TYPE) lParam;

			if (g_Config.eBetBtnType == CONFIG_BETBTN_TYPE_N)
				SendUILog_Basic(CSSTATS_UINO_POKER_ROOM_PERSONAL_AREA_FUNCTION_KEY_TYPE_N);
			else if (g_Config.eBetBtnType == CONFIG_BETBTN_TYPE_H)
				SendUILog_Basic(CSSTATS_UINO_POKER_ROOM_PERSONAL_AREA_FUNCTION_KEY_TYPE_H);
			else if (g_Config.eBetBtnType == CONFIG_BETBTN_TYPE_P)
				SendUILog_Basic(CSSTATS_UINO_POKER_ROOM_PERSONAL_AREA_FUNCTION_KEY_TYPE_P);

			if (g_Config.eBetBtnType <0 || g_Config.eBetBtnType> CONFIG_BETBTN_TYPE_P)
				g_Config.eBetBtnType = CONFIG_BETBTN_TYPE_N;

			SetBetBtnType(g_Config.eBetBtnType);

		}
		break;
	case IDM_COMPLAIN:
		{
			GM().BadUserReportGameChat(true);
		}
		break;
	case IDM_BDRULE_TIP:
		{
			NMBASE::UTIL::WriteProfile_int(GM().GetMyInfo()->UI.ID, "MorningCheckDay", GM().GetTodayFrom2019());
			GM().SetMorningCheckDay(GM().GetTodayFrom2019());
			m_btnBDRuleToolTip.Enable(FALSE);
		}
	default: 
		return FALSE;
	}
	return TRUE;
}

void CGameViewButtonMan::MakeHalfBtnAllinBtn(BOOL bAllin)
{
	//���ι�ư���� �ٲ��ش�.
	m_bAllin = bAllin;

	int nSprNo = 0;
	if (bAllin)
	{
		nSprNo = 6;//���ι�ư SPR NO
	}
	else
	{
		nSprNo = 5; //������ư SPR NO
	}

	if (m_eBtnDirectionType==CONFIG_BETBTN_TYPE_H)
		m_btnHalf.SetSprite(m_btnHalf.GetSprite(), nSprNo);
	else if (m_eBtnDirectionType==CONFIG_BETBTN_TYPE_P)
		m_btnHalf.SetSprite(m_btnHalf.GetSprite(), nSprNo);
	else		
		m_btnHalf.SetSprite(m_btnHalf.GetSprite(), nSprNo);

}

void CGameViewButtonMan::MakeDadangBtnAllinBtn(BOOL bAllin)
{
	//�����ư�� ���ι�ư���� �ٲ��ش�.
	m_bAllin = bAllin;

	int nSprNo =0;

	if (bAllin)
	{
		nSprNo = 6;//���ι�ư SPR NO
	}
	else
	{
		//������ �������� �����ư�� Ǯ��ư���� ��ü 2014.01
		nSprNo = 4; //�����ư SPR NO
	}

	if (m_eBtnDirectionType==CONFIG_BETBTN_TYPE_H)
		m_btnDadang.SetSprite(m_btnDadang.GetSprite(), nSprNo);
	else if (m_eBtnDirectionType==CONFIG_BETBTN_TYPE_P)
		m_btnDadang.SetSprite(m_btnDadang.GetSprite(), nSprNo);
	else
		m_btnDadang.SetSprite(m_btnDadang.GetSprite(), nSprNo);
}

//������ �������� �����ư�� Ǯ��ư���� ��ü������ �Լ��߰� 2014.01
void CGameViewButtonMan::MakeFullBtnAllinBtn(BOOL bAllin)
{
	//�����ư�� ���ι�ư���� �ٲ��ش�.
	int nSprNo =0;
	if (bAllin)
	{
		nSprNo = 6;//���ι�ư SPR NO
	}
	else
	{
		nSprNo = 7; //Ǯ��ư SPR NO
	}

	if (m_eBtnDirectionType==CONFIG_BETBTN_TYPE_H)
		m_btnFull.SetSprite(m_btnFull.GetSprite(), nSprNo);
	else if (m_eBtnDirectionType==CONFIG_BETBTN_TYPE_P)
		m_btnFull.SetSprite(m_btnFull.GetSprite(), nSprNo);
	else
		m_btnFull.SetSprite(m_btnFull.GetSprite(), nSprNo);
}

//������ �������� �ƽ���ư ��ü 2014.01
void CGameViewButtonMan::MakeMaxBtn(int BUTTON_IDM , BOOL isMax, INT64 maxMoney)
{
	//0:call, 1:ping, 2:dadang, 3:half

	if (isMax)
	{
		ResetMaxBtn();

		CGameProcess *pProcess = GM().GetCurrentProcess();
		CPlayer *pPlayer = pProcess->GetPlayer(0);
		int nMyIndex = pPlayer->ServPNum;

		
		// �����ʰ� �ƴҶ��� �ƽ�üũ�� �����ϰ� Ǯ� ���� ��ư������ �ƽ���ư�� ������ ���·� ���̵��� ����.
		/*if (pProcess->m_nCurPlayer != nMyIndex ) return;*/

		//// �����ʿ��� üũ
		if (pProcess->m_nCurPlayer != nMyIndex )
		{
			switch (BUTTON_IDM)
			{ 
			case IDM_CHECK:
				m_btnCheck.SetSprite(m_btnCheck.GetSprite(), 12);
				break;
			case 0:
				//m_btnCall.SetSprite(m_btnCall.GetSprite(), 2);
				//yoo
				// ���� �ݾ��� 0���̸� �ƽ��� �������� ����.
				if(nBetCallMoney > 0)
					m_btnCall.SetSprite(m_btnCall.GetSprite(), 9);

				if (m_bBMax && m_bBCall){
					m_btnHalf.Enable(FALSE);
					m_bBMax = FALSE;
					m_bShowNTM[ NT_HAF ] = false;
				}
				if (true==GM().IsChangeFullDDang()){
					m_btnFull.Enable(FALSE);
					m_bBDDa = FALSE;
					m_bShowNTM[ NT_DAD ] = false;
				}
				else{
					if (m_bBDDa && m_bBCall){
						m_btnDadang.Enable(FALSE);
						m_bBDDa = FALSE;
						m_bShowNTM[ NT_DAD ] = false;
					}
				}
				break;
			case 1:
				m_btnPing.SetSprite(m_btnPing.GetSprite(), 10);

				if (m_bBMax && m_bBPing)
				{
					m_btnHalf.Enable(FALSE);
					m_bBMax = FALSE;
					m_bShowNTM[ NT_HAF ] = false;
				}
				if (true==GM().IsChangeFullDDang()){
					m_btnFull.Enable(FALSE);
					m_bBDDa = FALSE;
					m_bShowNTM[ NT_DAD ] = false;
				}
				else{
					if (m_bBDDa && m_bBPing ){
						m_btnDadang.Enable(FALSE);
						m_bBDDa = FALSE;
						m_bShowNTM[ NT_DAD ] = false;
					}
				}
				break;
			case 2:
				if (true==GM().IsChangeFullDDang()){
					m_btnFull.SetSprite(m_btnFull.GetSprite(), 11);
				}
				else
				{
					m_btnDadang.SetSprite(m_btnDadang.GetSprite(), 11);
				}
				if (m_bBMax && m_bBDDa && !g_RI.bIsFriendRoom)
				{
					m_btnHalf.Enable(FALSE);
					m_bBMax = FALSE;
					m_bShowNTM[ NT_HAF ] = false;
				}
				break;
			case 3:
				m_btnHalf.SetSprite(m_btnHalf.GetSprite(), 8);
				break;
			}
			if (maxMoney == 0)
			{
				if (m_bBDDa){
					if (true==GM().IsChangeFullDDang())
					{
						m_btnFull.Enable(FALSE);
					}
					else
					{
						m_btnDadang.Enable(FALSE);
					}
					m_bBDDa = FALSE;
					m_bShowNTM[ NT_DAD ] = false;
				}

				if (m_bBMax)
				{
					m_btnHalf.Enable(FALSE);
					m_bBMax = FALSE;
					m_bShowNTM[ NT_HAF ] = false;
				}
			}

		}
		else
		{
			switch (BUTTON_IDM)
			{ 
			case IDM_CHECK:
				m_btnCheck.SetSprite(m_btnCheck.GetSprite(), 12);
				break;
			case 0:
				// ���� �ݾ��� 0���̸� �ƽ��� �������� ����.
				if(nBetCallMoney > 0)
					m_btnCall.SetSprite(m_btnCall.GetSprite(), 9);

				if (m_bBMax && m_bBCall)
				{
					m_btnHalf.Enable(FALSE);
					m_bBMax = FALSE;
					m_bShowNTM[ NT_HAF ] = false;
				}
				if (true==GM().IsChangeFullDDang()){
					m_btnFull.Enable(FALSE);
					m_bBDDa = FALSE;
					m_bShowNTM[ NT_DAD ] = false;
				}
				else{
					if (m_bBDDa && m_bBCall){
						m_btnDadang.Enable(FALSE);
						m_bBDDa = FALSE;
						m_bShowNTM[ NT_DAD ] = false;
					}
				}
				break;
			case 1:
				m_btnPing.SetSprite(m_btnPing.GetSprite(), 10);

				if (m_bBMax && m_bBPing)
				{
					m_btnHalf.Enable(FALSE);
					m_bBMax = FALSE;
					m_bShowNTM[ NT_HAF ] = false;
				}

				if (true==GM().IsChangeFullDDang())
				{
					m_btnFull.Enable(FALSE);
					m_bBDDa = FALSE;
					m_bShowNTM[ NT_DAD ] = false;
				}
				else{
					if (m_bBDDa && m_bBPing )
					{
						m_btnDadang.Enable(FALSE);
						m_bBDDa = FALSE;
						m_bShowNTM[ NT_DAD ] = false;
					}
				}
				break;
			case 2:
				if (true==GM().IsChangeFullDDang())
				{
					m_btnFull.SetSprite(m_btnFull.GetSprite(), 11);
				}
				else{
					m_btnDadang.SetSprite(m_btnDadang.GetSprite(), 11);
				}

				if (m_bBMax && m_bBDDa && !g_RI.bIsFriendRoom)
				{
					m_btnHalf.Enable(FALSE);
					m_bBMax = FALSE;
					m_bShowNTM[ NT_HAF ] = false;
				}

				break;

			case 3:
				m_btnHalf.SetSprite(m_btnHalf.GetSprite(), 8);
				break;
			}

			if (*m_pNTMoney[ NT_CHK ]->GetText() == '0')
			{
				m_btnCheck.Enable(FALSE);
				m_bBCheck = FALSE;
				m_bShowNTM[ NT_CHK ] = false;
			}

			if (*m_pNTMoney[ NT_CAL ]->GetText() == '0')
			{
				//�ƽ��ݾ��� 0�� ��� ������ call��ư
				m_btnCall.SetSprite(m_btnCall.GetSprite(), 2);
			}

			if (*m_pNTMoney[ NT_PNG ]->GetText() == '0')
			{
				m_btnPing.Enable(FALSE);
				m_bBPing = FALSE;
				m_bShowNTM[ NT_PNG ] = false;
			}

			if (*m_pNTMoney[ NT_DAD ]->GetText() == '0')
			{
				if (true==GM().IsChangeFullDDang()){
					m_btnFull.Enable(FALSE);
				}
				else{
					m_btnDadang.Enable(FALSE);
				}
				m_bBDDa = FALSE;
				m_bShowNTM[ NT_DAD ] = false;
			}

			if (*m_pNTMoney[ NT_HAF ]->GetText() == '0')
			{
				m_btnHalf.Enable(FALSE);
				m_bBMax = FALSE;
				m_bShowNTM[ NT_HAF ] = false;
			}
		}
	}else{
		m_btnCheck.SetSprite(m_btnCheck.GetSprite(), 1); //�� ��ư�� SPR ID ( n * 4 )
		m_btnCall.SetSprite(m_btnCall.GetSprite(), 2);
		m_btnPing.SetSprite(m_btnPing.GetSprite(), 3);
		
		if (false == m_bAllin)
		{
			if (true==GM().IsChangeFullDDang()){
				m_btnFull.SetSprite(m_btnFull.GetSprite(), 7);
			}
			else{
				m_btnDadang.SetSprite(m_btnDadang.GetSprite(), 4);
			}
			m_btnHalf.SetSprite(m_btnHalf.GetSprite(), 5);
		}
	}

}


void CGameViewButtonMan::ResetMaxBtn()
{
	m_bMaxState = false;
	m_btnCheck.SetSprite(m_btnCheck.GetSprite(), 1); //�� ��ư�� SPR ID ( n * 4 )
	m_btnCall.SetSprite(m_btnCall.GetSprite(), 2);
	m_btnPing.SetSprite(m_btnPing.GetSprite(), 3);
	if (true==GM().IsChangeFullDDang()){
		m_btnFull.SetSprite(m_btnFull.GetSprite(), 7);
	}
	else{
		m_btnDadang.SetSprite(m_btnDadang.GetSprite(), 4);
	}
	m_btnHalf.SetSprite(m_btnHalf.GetSprite(), 5);
}

BOOL CGameViewButtonMan::GetShowBDTooltip()
{
	return GM().GetCurrentGameType() == IDX_GAME_BD && !g_bMyObserver && g_RI.bMorningBet == true &&
		GM().GetMorningCheckDay()!=GM().GetTodayFrom2019() && g_RI.State == 1 && g_RI.nChangeType == 0;
}