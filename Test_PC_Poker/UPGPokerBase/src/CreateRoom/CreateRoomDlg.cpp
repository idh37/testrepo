// CreateRoomDlg.cpp : implementation file
// 

#include "stdafx.h"
#include "CreateRoomDlg.h"
#include "../GlobalPokerBase.h"
#include "../shared/SharedDefine.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define TIMER_CREATEROOMDLG_CHECKWHERE (2040)
/////////////////////////////////////////////////////////////////////////////
// CCreateRoomDlg dialog
CRect stComboSize(0,0,196,200);
CRect stRdSize(0,0,13,13);

// CCreateRoomDlg::CCreateRoomDlg(CWnd* pParent /*=NULL*/)
CCreateRoomDlg::CCreateRoomDlg(IDX_GAME nGame,CWnd* pParent /*=NULL*/)
: CUPGDlgBase(CCreateRoomDlg::IDD, pParent), m_nGame(nGame)
, m_MaxRoomMoney1(0)
, m_MaxRoomMoney2(0)
{
	m_i64MinimumMoney = -1;
	m_i64SeedMoney = 0;
	m_sAutoBet = 0;
	m_sBetRuleKind = 0;
	m_nChipKind = ROOM_CHIPKIND_NORMAL;
	m_nGameMode = CHOICE_GAME;
	m_nFormKind = FORMKIND_CHOICE;
	m_nGameType = RULETYPE_7POKER;
	m_nMaxUserNum = 5;

	m_cHollowBrush.CreateStockObject(HOLLOW_BRUSH);
	m_bVIPEnterLimitYN = false;

	m_bIsMaxRoom = FALSE;
	m_bFriend = false;
	m_bUnionMode = false;
	m_bBetRule = false;

	m_bShowMaxRoomDesc = FALSE;
	m_bShowRdBlind = FALSE;
	m_bShowRdUnion = FALSE;

	m_nBottomImgRdUnion = 0;

	m_i64InMoney = 0;
	m_MaxRoomMoney1 = 0;
	m_MaxRoomMoney2 = 0;

	m_bUseWebSelectedRoom = false;
	ZeroMemory(&m_WebSelectedRoomOption, sizeof(m_WebSelectedRoomOption));

	m_stBetRuleToolTip.bBetRuleToolTip = false;
}


void CCreateRoomDlg::DoDataExchange(CDataExchange* pDX)
{
	CUPGDlgBase::DoDataExchange(pDX);
	// {{AFX_DATA_MAP(CCreateRoomDlg)
	
	// �� ����
	DDX_Control(pDX, IDC_COMBO_LIMIT_USER2, m_cbLimitUserNum);
	// �⺻ �Ӵ�
	DDX_Control(pDX, IDC_COMBO_AUTOBET_SEED, m_cbAutoBetSeed);
	// ���� ����
	DDX_Control(pDX, IDC_COMBO_MINIMUM_MONEY, m_cbMinimumMoney);
	DDX_Control(pDX, IDC_BTN_NORMAL, m_rdNormal);
	DDX_Control(pDX, IDC_BTN_GOLD, m_rdGold);
	DDX_Control(pDX, IDC_BTN_FRIEND, m_rdFriend);
	DDX_Control(pDX, IDC_BTN_MODE1, m_rdMode1);
	DDX_Control(pDX, IDC_BTN_MODE2, m_rdMode2);
	
	// ����on, ����off ���� ��ư
	DDX_Control(pDX, IDC_BTN_STOP1, m_rdStop1);
	DDX_Control(pDX, IDC_BTN_STOP2, m_rdStop2);

	DDX_Control(pDX, IDC_BTN_UNION1, m_rdUnion1);
	DDX_Control(pDX, IDC_BTN_UNION2, m_rdUnion2);
	
	DDX_Control(pDX, IDC_BTN_MAXROOM3, m_rdMaxRoom3);
	DDX_Control(pDX, IDC_BTN_MAXROOM4, m_rdMaxRoom4);
	DDX_Control(pDX, IDC_EDIT_MRMONEY_INPUT1, m_edMRMoney_Input1);
	DDX_Control(pDX, IDC_EDIT_MRMONEY_INPUT2, m_edMRMoney_Input2);

	DDX_Control(pDX, IDC_BTN_BLIND1, m_rdBlind1);
	DDX_Control(pDX, IDC_BTN_BLIND2, m_rdBlind2);

	DDX_Control(pDX, IDC_BTN_OPEN_MAXDESC, m_btnOpenMaxDesc);
	DDX_Control(pDX, IDC_BTN_CLOSE_MAXDESC, m_btnCloseMaxDesc);

	DDX_Control(pDX, IDC_BTN_BETRULE1, m_rdBetRule1);
	DDX_Control(pDX, IDC_BTN_BETRULE2, m_rdBetRule2);

	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_EDIT_ROOMTITLE, m_edRoomTitle);
	DDX_Text(pDX, IDC_EDIT_ROOMTITLE, m_strRoomTitle);
	DDV_MaxChars(pDX, m_strRoomTitle, 30);
	// }}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_MRMONEY_INPUT1, m_MaxRoomMoney1);
	DDV_MinMaxUInt(pDX, m_MaxRoomMoney1, 0, 500);
	DDX_Text(pDX, IDC_EDIT_MRMONEY_INPUT2, m_MaxRoomMoney2);
	DDV_MinMaxUInt(pDX, m_MaxRoomMoney2, 0, 9999);
}


BEGIN_MESSAGE_MAP(CCreateRoomDlg, CUPGDlgBase)
	// {{AFX_MSG_MAP(CCreateRoomDlg)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()
	// }}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
	ON_CBN_SELCHANGE(IDC_COMBO_MINIMUM_MONEY, &CCreateRoomDlg::OnCbnSelchangeMinimumMony)
	ON_CBN_SELCHANGE(IDC_COMBO_AUTOBET_SEED, &CCreateRoomDlg::OnCbnSelchangeAutobetSeed)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT_MRMONEY_INPUT1, &CCreateRoomDlg::OnEnChangeEditMrmoneyInput1)
	ON_EN_CHANGE(IDC_EDIT_MRMONEY_INPUT2, &CCreateRoomDlg::OnEnChangeEditMrmoneyInput2)
	ON_CBN_SELCHANGE(IDC_COMBO_LIMIT_USER2, &CCreateRoomDlg::OnCbnSelchangeComboLimitUser)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCreateRoomDlg message handlers

BOOL CCreateRoomDlg::OnInitDialog() 
{
	CUPGDlgBase::OnInitDialog();

	CDataRoomOptionList *pDataRoomOptionList = GM().GetDataRoomOptionList();
	if (pDataRoomOptionList == NULL) return FALSE;

	m_nChipKind = (int)ROOM_CHIPKIND_NORMAL;
	if (GM().getRoomListTabState()==2 && GM().GetMyGold() > 0)
		m_nChipKind = (int)ROOM_CHIPKIND_GOLD;

	ROOM_CREATEOPTION *pRoomCreationOption = NULL;
	pRoomCreationOption = pDataRoomOptionList->GetRoomCreationOption( (ROOM_CHIPKIND)m_nChipKind );
	if (pRoomCreationOption == NULL) return FALSE;

	ROOMOPTION *pDefaultRoomOption = pDataRoomOptionList->GetDefaultRoomOption( 0, (ROOM_CHIPKIND)m_nChipKind );
	if (pDefaultRoomOption == NULL) return FALSE;

	INT64 llDefalutEnterLimitMoney = 0;
	INT64 llDefalutEnterSeedMoney = 0;
	if (m_bUseWebSelectedRoom)
	{
		// ������ ���õ� ��ɼ� ����
		llDefalutEnterLimitMoney = m_WebSelectedRoomOption.llEnterLimitMoney;
		llDefalutEnterSeedMoney = m_WebSelectedRoomOption.llSeedMoney;
	}
	else
	{
		// �⺻����Ʈ ��ɼ����� �����.
		llDefalutEnterLimitMoney = GM().GetRoomOptionLimitMoney(pDefaultRoomOption, true);
		llDefalutEnterSeedMoney = GM().GetRoomOptionSeedMoney(pDefaultRoomOption, true);
	}

	// ���̾�α� ��Ʈ�� �����ǰ� ��ġ ������
	NMBASE::UTIL::CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);	

	m_stBetRuleToolTip.bBetRuleToolTip = m_nGame != IDX_GAME_HA;	// ���÷� ������ ���������ϴ°�? ����Ŀ, ���̷ο�, 7��Ŀ

	InitCBLimitUser();	

	// ���̾Ƹ�� ����� �ƽ��� ������ ���� ��ư �ʱ�ȭ.
	m_btnOpenMaxDesc.ShowWindow(FALSE);
	m_btnCloseMaxDesc.ShowWindow(FALSE);

	m_imgHideMaxDescBack.LoadBitmapFile( "PokerCommon\\image\\CreateRoom\\HideMROption.bmp" );
	m_imgHideMaxGoldDescBack.LoadBitmapFile( "PokerCommon\\image\\CreateRoom\\HideMROption_gold.bmp" );

	m_imgRdTypeBG.LoadBitmapFile( "PokerCommon\\image\\CreateRoom\\selectchennel.bmp" );

	m_imgRdBlind.LoadBitmapFile( "PokerCommon\\image\\CreateRoom\\blind.bmp" );
	m_imgRdBetRule.LoadBitmapFile( "PokerCommon\\image\\CreateRoom\\betrule.bmp" );
	m_imgRdUnion.LoadBitmapFile( "PokerCommon\\image\\CreateRoom\\union.bmp" );

	m_strBackFile.Format("PokerCommon\\image\\CreateRoom\\back_%s.bmp", GetGameStr(m_nGame));
	
	CRect rt;	

	rt.SetRect(0,0,42,15);
	rt.OffsetRect(97,255);
	GetDlgItem(IDC_EDIT_MRMONEY_INPUT1)->MoveWindow(&rt);

	rt.SetRect(0,0,42,15);
	rt.OffsetRect(167,255);
	GetDlgItem(IDC_EDIT_MRMONEY_INPUT2)->MoveWindow(&rt);
	
	HideOptionBtns();

	switch(m_nGame)
	{
	case IDX_GAME_HA:
		break;
	case IDX_GAME_HL:
		m_rdNormal.Init(94,92,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
		m_rdFriend.Init(201,92,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
		m_rdNormal.AddToGroup(&m_rdFriend);
		m_rdNormal.SetCheck(TRUE);

		// ��ȭ ä�� ����
		m_rdNormal.ShowWindow(SW_SHOW);
		m_rdFriend.ShowWindow(SW_SHOW);
		break;
	default:
		m_rdNormal.Init(94,92,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
		m_rdGold.Init(167,92,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
		m_rdFriend.Init(240,92,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
		m_rdNormal.AddToGroup(&m_rdGold);
		m_rdGold.AddToGroup(&m_rdFriend);
		if (m_nChipKind==(int)ROOM_CHIPKIND_GOLD)
			m_rdGold.SetCheck(TRUE);
		else
			m_rdNormal.SetCheck(TRUE);

		// ��ȭ ä�� ����
		m_rdNormal.ShowWindow(SW_SHOW);
		m_rdGold.ShowWindow(SW_SHOW);
		m_rdFriend.ShowWindow(SW_SHOW);

		if (GM().GetMyGold() <= 0)
			m_rdGold.EnableWindow(SW_HIDE);
		if (GM().GetMyInfo()->UI.GetRealPMoney() < MONEY_UK)
			m_rdNormal.EnableWindow(SW_HIDE);
		break;
	}

	switch(m_nGame)
	{
		// ���̷ο�
	case IDX_GAME_HL:
		break;
		// �Ƕ�
	case IDX_GAME_HA:
		m_rdStop1.ShowWindow(SW_SHOW);
		m_rdStop2.ShowWindow(SW_SHOW);
		
		m_rdStop1.Init(94,115,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
		m_rdStop2.Init(201,115,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
		m_rdStop1.AddToGroup(&m_rdStop2);
		m_rdStop1.SetCheck(TRUE);	
		
		break;
		// 7��Ŀ, ����Ŀ, ����
	case IDX_GAME_SP:
	case IDX_GAME_NP:
	case IDX_GAME_SD:
		m_nBottomImgRdUnion = 202;

		break;
		// �ο�ٵ���
	case IDX_GAME_BD:				
		m_rdStop1.EnableWindow(FALSE);
		m_rdStop2.EnableWindow(FALSE);

		m_rdBlind1.Init(93,174,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
		m_rdBlind2.Init(201,174,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
		m_rdBlind1.AddToGroup(&m_rdBlind2);
		m_rdBlind1.SetCheck(TRUE);

		m_bShowRdBlind = llDefalutEnterLimitMoney >= 10 * MONEY_JO || GetCheckRdGold();

		m_rdBetRule1.Init(93,194,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
		m_rdBetRule2.Init(201,194,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
		m_rdBetRule1.AddToGroup(&m_rdBetRule2);
		m_rdBetRule1.SetCheck(TRUE);	

		if (m_bShowRdBlind)
		{
			m_rdBlind1.ShowWindow(SW_SHOW);
			m_rdBlind2.ShowWindow(SW_SHOW);

			if (GetCheckRdGold())
			{
				m_rdBetRule1.ShowWindow(SW_SHOW);
				m_rdBetRule2.ShowWindow(SW_SHOW);
				SYSTEMTIME systemTime;
				GetLocalTime(&systemTime);
				COleDateTime CurTime(systemTime);
				COleDateTime EndTime(GM().GetMorningBanTime());
				if(EndTime.GetStatus() == COleDateTime::valid && EndTime>CurTime)
				{
					m_rdBetRule1.SetCheck(FALSE);	
					m_rdBetRule2.SetCheck(TRUE);	
					m_rdBetRule1.EnableWindow(FALSE);
				}
			}
		}

		m_nBottomImgRdUnion = 178;

		rt.SetRect(0,0,95,100);
		rt.OffsetRect(96,206);
		GetDlgItem(IDC_EDIT_MRMONEY_INPUT1)->MoveWindow(&rt);

		rt.SetRect(0,0,42,15);
		rt.OffsetRect(166,206);
		GetDlgItem(IDC_EDIT_MRMONEY_INPUT2)->MoveWindow(&rt);
		
		break;
	}

	m_rdUnion1.Init(94,m_nBottomImgRdUnion,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
	m_rdUnion2.Init(201,m_nBottomImgRdUnion,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
	m_rdUnion1.AddToGroup(&m_rdUnion2);
	m_rdUnion1.SetCheck(TRUE);

	rt.SetRect(0,0,178,13);
	rt.OffsetRect(98,44);
	GetDlgItem(IDC_EDIT_ROOMTITLE)->MoveWindow(&rt);

	MM().Call(UPG_CMK_GENERATE_RANDOM_ROOM_NAME, 0, (LPARAM)(&m_strRoomTitle));
	m_edRoomTitle.SetWindowText(m_strRoomTitle);
	m_edRoomTitle.SetSel(0, m_strRoomTitle.GetLength());
	m_edRoomTitle.SetFocus();	
	rt = stComboSize;
	rt.OffsetRect(94, m_nGame == IDX_GAME_BD ? 125 : 149);

	// ������ ���� �������� ��ǥ ����
	m_cbMinimumMoney.MoveWindow(rt);
	m_cbMinimumMoney.ResetContent();
	m_listMinimumMoney.clear();
	m_listSeedMoney.clear();

	bool bVipRoom = false;
	ROOMOPTION *pRoomOption = NULL;
	int i = 0;

	for(i=0;i<pRoomCreationOption->nArrayCnt;i++)
	{
		pRoomOption = &(pRoomCreationOption->arrRoomOption[i]);

		if (pRoomOption->llEnterLimitMoney >= VIPRoom_Money(GM().GetCurrentGameType()))
		{
			bVipRoom = true;
			break;
		}
	}
	
	CString strMoney;
	int nIndex = 0;
	int nDefault = 0;
	pRoomOption = NULL;


	for(i=0;i<pRoomCreationOption->nArrayCnt;i++)
	{
		pRoomOption = &(pRoomCreationOption->arrRoomOption[i]);
		m_listMinimumMoney.insert(pRoomOption->llEnterLimitMoney);
		m_listSeedMoney.push_back(pRoomOption->llSeedMoney);
	}

	set<INT64>::iterator start = m_listMinimumMoney.begin();
	set<INT64>::iterator end = m_listMinimumMoney.end();

	bool bZero = false;
	set<INT64>::iterator itZero;

	for(;start != end;start++)
	{
		// �������� ���� �ּ� ó��
		if (*start == 0)
		{
			bZero = true;
			itZero = start;
			continue;
		}
		else
		{
			strMoney.Format("%s%s�̻�", NMBASE::UTIL::g_NumberToHangul(*start), strChip_Name[m_nChipKind]);
		}

		nIndex = m_cbMinimumMoney.AddString(strMoney);
		m_cbMinimumMoney.SetItemData(nIndex, (DWORD_PTR)&(*start));
		if (llDefalutEnterLimitMoney == *start) nDefault = nIndex;
	}

	if (bZero)
	{
		if (!GetCheckRdGold())
		{
			strMoney = "���Ѿ���";
		}
		else
		{
			strMoney = "MAX ��";
		}

		if (!GetCheckRdGold() || IsCreateMaxRoom())
		{
			nIndex = m_cbMinimumMoney.AddString(strMoney);
			m_cbMinimumMoney.SetItemData(nIndex, (DWORD_PTR)&(*itZero));
			if (llDefalutEnterLimitMoney == 0) nDefault = nIndex;
		}
	}

	if (m_cbMinimumMoney.GetCount() == 1)
	{
		m_cbMinimumMoney.EnableWindow(FALSE);
		m_cbMinimumMoney.SetCurSel(0);
	}
	else if (m_bFriend)
	{
		m_cbMinimumMoney.EnableWindow(FALSE);
		m_cbMinimumMoney.SetCurSel(m_cbMinimumMoney.GetCount()-1);
	}
	else
	{
		m_cbMinimumMoney.EnableWindow(TRUE);
		m_cbMinimumMoney.SetCurSel(nDefault);
	}

	if (((m_nGame == IDX_GAME_BD || m_nGame == IDX_GAME_SP || m_nGame == IDX_GAME_NP)
		&& !GetCheckRdGold() && llDefalutEnterLimitMoney < 10 * MONEY_JO)
		|| llDefalutEnterLimitMoney == 0 || (GetCheckRdGold() && llDefalutEnterLimitMoney < 300000))
	{
		m_edRoomTitle.ModifyStyle(NULL, WS_DISABLED, NULL);
		m_edRoomTitle.SetWindowPos(&CWnd::wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

		MM().Call(UPG_CMK_GENERATE_RANDOM_ROOM_NAME, 0, (LPARAM)(&m_strRoomTitle));
		m_edRoomTitle.SetWindowText(m_strRoomTitle);
		m_edRoomTitle.EnableWindow(FALSE);
	}

	if (m_bUseWebSelectedRoom)
	{
		if (SetComboAutoBetSeed(llDefalutEnterLimitMoney, m_WebSelectedRoomOption.llSeedMoney, m_WebSelectedRoomOption.sAutoBet) == FALSE) 
			return FALSE;
	}
	else
	{
		if (SetComboAutoBetSeed(llDefalutEnterLimitMoney, llDefalutEnterSeedMoney) == FALSE)
			return FALSE;
	}
	
	m_nGameType = RULETYPE_7POKER;
	SetTimer(TIMER_CREATEROOMDLG_CHECKWHERE, 100, NULL);

	NMBASE::SKINGDI::WindowResize2Bitmap(m_hWnd, m_strBackFile);
	UpdateData(FALSE);
	
	InitPosBtns();

	return FALSE;
}

void CCreateRoomDlg::InitCBLimitUser()
{
	// ������ ��ġ�� ũ�⸦ �����ϴ� Rect
	CRect rtForLimitUser;
	rtForLimitUser.SetRect(0,0,193,180);	// ũ��
	rtForLimitUser.OffsetRect(94,64);	// ��ġ

	// ������ �޺��ڽ��� Display���� ���� �ݿ�
	m_cbLimitUserNum.EnableWindow(TRUE);
	m_cbLimitUserNum.MoveWindow(rtForLimitUser);
	m_cbLimitUserNum.ResetContent();
	
	// Gold ���� 2�ι� ���� �Ұ�.
	int index = -1;
	if( m_nChipKind == (int)ROOM_CHIPKIND_NORMAL )
	{
		m_cbLimitUserNum.AddString("2��");
		m_cbLimitUserNum.SetItemData(++index, 2);
	}
	m_cbLimitUserNum.AddString("3��");
	m_cbLimitUserNum.SetItemData(++index, 3);
	m_cbLimitUserNum.AddString("4��");
	m_cbLimitUserNum.SetItemData(++index, 4);
	m_cbLimitUserNum.AddString("5��");
	m_cbLimitUserNum.SetItemData(++index, 5);
	m_cbLimitUserNum.SetCurSel(m_cbLimitUserNum.GetCount()-1);
}

BOOL CCreateRoomDlg::SetComboAutoBetSeed(INT64 llEnterLimitMoney, INT64 llDefaultSeedMoney, short sDefaultAutobetParam)
{	
	// ���� �޴����� �ִ� ��ɼ� ����Ʈ�� �޾ƿ´�, ���� ���� ��� ����
	CDataRoomOptionList *pDataRoomOptionList = GM().GetDataRoomOptionList();
	if (pDataRoomOptionList == NULL) return FALSE;

	ROOM_CREATEOPTION *pRoomCreationOption = pDataRoomOptionList->GetRoomCreationOption( (ROOM_CHIPKIND)m_nChipKind );
	if (pRoomCreationOption == NULL) return FALSE;
	
	// ����Ʈ ��ɼ� �ޱ�
	ROOMOPTION *pDefaultRoomOption = pDataRoomOptionList->GetDefaultRoomOption( 0, (ROOM_CHIPKIND)m_nChipKind );
	if (pDefaultRoomOption == NULL) return FALSE;

	ROOMOPTION *pRoomOption = NULL;

	CRect rt = stComboSize;
	rt.OffsetRect(94, m_nGame == IDX_GAME_BD ? 149 : 173);

	m_cbAutoBetSeed.MoveWindow(rt);
	m_cbAutoBetSeed.ResetContent();

	int i = 0;
	CString strMoney, strAutoBet, strCardOrMoneySeting;

	int nIndex = 0;
	int nDefault = 0;

	// 
	MAP_LIST_RULE_ITER iter = m_listRule.begin();
	while ( iter != m_listRule.end() )
	{
		map< BetRule*, RULE >::iterator iterRule = ( iter->second ).begin();

		while( iterRule !=  ( iter->second ).end() )
		{
			delete iterRule->first;

			iterRule++;
		}

		++iter;
	}
	
	m_listRule.clear();
	
	short sDefaultAutoBet = 0; // ���� ������ ����
	char cCardSetting = 0;
	INT64 i64DefaultSeed = 0;

	for(i=0;i<pRoomCreationOption->nArrayCnt;i++)
	{
		pRoomOption = &(pRoomCreationOption->arrRoomOption[i]);
		if (pRoomOption->llEnterLimitMoney == llEnterLimitMoney && pRoomOption->llSeedMoney > 0)
		{
			RULE rule;
			rule.i64Seed = pRoomOption->llSeedMoney;
			rule.sAutoBet = pRoomOption->sAutoBet;
			rule.sBetRuleKind = pRoomOption->sBetRuleKind;	// ����Ʈ���÷�
			rule.cCardSetting = pRoomOption->cCardSetting;
			rule.fBetMoneyMulti = pRoomOption->fBetMoneyMulti;
			rule.llEnterLimitMoney = pRoomOption->llEnterLimitMoney;

			BetRule* pBetRuleKey = NULL;
			pBetRuleKey = new BetRule;

			pBetRuleKey->sAutoBet = pRoomOption->sAutoBet;

			m_listRule[ pRoomOption->llSeedMoney ][ pBetRuleKey ] = rule;

			if (pRoomOption->cDefault)
			{
				sDefaultAutoBet = pRoomOption->sAutoBet;
				i64DefaultSeed = pRoomOption->llSeedMoney;
				cCardSetting = pRoomOption->cCardSetting;
			}
		}
	}

	// �������ѸӴϿ� �´� �⺻�� �Է��� �ƴ� �Ű������� �ش� �������ѱ������� �õ�ӴϿ� ���÷��� ���� �Է��Ѵ�
	// ����Ʈ �õ�Ӵ�(llDefaultSeedMoney) ���� �Űܺ����� �Ѿ�°�쿡�� �ش� �õ�ӴϷ� �޺��ڽ����� �������ش�.
	if (llDefaultSeedMoney!=0)
	{
		i64DefaultSeed = llDefaultSeedMoney;
	}

	// ����Ʈ ���÷�(sDefaultAutobetParam) ���� �Űܺ����� �Ѿ�°�쿡�� �ش� ���÷�� �޺��ڽ����� �������ش�.
	if (sDefaultAutobetParam!=-1)
	{
		sDefaultAutoBet = sDefaultAutobetParam;
	}

	MAP_LIST_RULE_ITER start = m_listRule.begin();
	MAP_LIST_RULE_ITER end = m_listRule.end();

	map< BetRule*, RULE >::iterator start_auto, end_auto;
	
	for(;start != end;start++)
	{
		start_auto = start->second.begin();
		end_auto = start->second.end();

		for(;start_auto != end_auto;start_auto++)
		{
			if (m_nGame != IDX_GAME_HA)
			{
				if (start->first == 6000 * MONEY_UK && IsGiveUpRoom() == false)
					continue;

				strMoney.Format("%s%s/", NMBASE::UTIL::g_NumberToHangul(start->first), strChip_Name[m_nChipKind]);
				
				// ���� ������ �߰� ����. ������ ���� �Ǹ鼭 ������.
				if ( m_nGame ==  IDX_GAME_SD )
				{
					strMoney.Format("%s%s", NMBASE::UTIL::g_NumberToHangul(start->first), strChip_Name[m_nChipKind]);
				}
				else
				{
					strMoney.Format("%s%s/", NMBASE::UTIL::g_NumberToHangul(start->first), strChip_Name[m_nChipKind]);
					
					// ������ ���� ���� ī�弼�� �߰�
					strCardOrMoneySeting = GetCardSettingName(m_nGame, start_auto->second.cCardSetting );
					strMoney += strCardOrMoneySeting;
				}

				nIndex = m_cbAutoBetSeed.AddString(strMoney);
			}
			else
			{
				// �õ�Ӵ� 4000���� ��ǹ濡�� �ش�
				if (start->first == 4000 * MONEY_UK && IsGiveUpRoom() == false)
					continue;
				
				if ( start->first > 0)
				{
					strMoney.Format("%s", g_GetSeedMoneyToText(m_nGame, start->first));

					// �õ�Ӵ� 4000���� �� �տ�
					if (start->first == 4000 * MONEY_UK)
						nIndex = m_cbAutoBetSeed.InsertString(0, strMoney);
					else
						nIndex = m_cbAutoBetSeed.AddString(strMoney);
				}
				else
				{
					continue; 
				}				
			}
			
			m_cbAutoBetSeed.SetItemData(nIndex, (DWORD_PTR)&(start_auto->second));
			
			// ����Ʈ �ɼ� ����.
			if (cCardSetting ==  start_auto->second.cCardSetting && i64DefaultSeed == start->first) 
				nDefault = nIndex;			
		}
	}

	m_cbAutoBetSeed.SetCurSel(nDefault);

	// �ɼ� ������ 1�� �̻��̶� ���̵��� ����. 0�� �϶��� ������.
	if (m_cbAutoBetSeed.GetCount() < 1) m_cbAutoBetSeed.EnableWindow(FALSE);
	else m_cbAutoBetSeed.EnableWindow(TRUE);
	m_cbAutoBetSeed.ShowWindow(SW_SHOW);
	RefreshBetRuleToolTip();  // ���÷� �ȳ� ���� ����
	return TRUE;
}

void CCreateRoomDlg::OnOK()
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	if (m_strRoomTitle.GetLength() == 0)
	{
		ShowMainMessageDlg("������ �Է��ϼ���");

		return;
	}
	
	// �� ���� �弳 �˻�
	if (NMBASE::UTIL::NMIsBad((char *)m_strRoomTitle.GetString()) == TRUE)
	{
		ShowMainMessageDlg((char*)g_StrMan.Get(_T("BAD_CHAT2")).operator LPCTSTR() );
		GetDlgItem(IDC_EDIT_ROOMTITLE)->SetFocus();
		
		return;
	}
	
	int nIndex = m_cbAutoBetSeed.GetCurSel();
	
	if (nIndex == -1)
	{
		ShowMainMessageDlg("�õ�Ӵ�/���÷��� ������ �ּ���");
		
		return;
	}
	
	DWORD_PTR dwItemData = m_cbAutoBetSeed.GetItemData(nIndex);
	RULE *pRule = (RULE *)dwItemData;
	
	m_i64SeedMoney = pRule->i64Seed;
	m_sAutoBet = pRule->sAutoBet;
	m_sBetRuleKind = pRule->sBetRuleKind;
	cCardSetting = pRule->cCardSetting; // ������ �����ؼ� ī�弼�ð� �߰�
	fBetMoneyMulti = pRule->fBetMoneyMulti; // ������ �����ؼ� ������ �߰�
	
	nIndex = m_cbMinimumMoney.GetCurSel();
	if (nIndex == -1)
	{
		ShowMainMessageDlg("���������� ������ �ּ���");
		return;
	}

	dwItemData = m_cbMinimumMoney.GetItemData(nIndex);
	m_i64MinimumMoney = *((INT64 *)dwItemData);
	
	// �ƽ���
	if ( m_bIsMaxRoom )
	{
		INT64 nTemp1 = (INT64)m_MaxRoomMoney1 * (INT64)10000; // ������
		INT64 nTemp2 = (INT64)m_MaxRoomMoney2; // �ϴ���

		m_i64InMoney = nTemp1 + nTemp2; // �����ȱݾ�

		if (!IsCreateMaxRoom())
		{
			ShowMainMessageDlg("���̾Ƹ�� ����� ȸ���Ը� ���� �����մϴ�");

			return;
		}

		if (m_i64InMoney == 0)
		{
			ShowMainMessageDlg("max�� �ڻ��� �Է����ּ���!");
			
			return;
		}
		
		if (!GetCheckRdGold())
		{
			// �Ӵ� ä���϶��� 1���� ���Ѵ�.
			m_i64InMoney *= (INT64)100000000;
			if ( m_i64InMoney < 10000000000000 || m_i64InMoney > 500000000000000 )
			{
				ShowMainMessageDlg("10���̻� ~ 500�����ϱ����� �Է� �����մϴ�");
				
				return;
			}
		}
		else
		{
			if ( m_i64InMoney < 100000 || m_i64InMoney > 5000000 )
			{

				CString csMsg;

				csMsg.Format("10��%s�̻� ~ 500��%s���ϱ����� �Է� �����մϴ�", strChip_Name[(int)ROOM_CHIPKIND_GOLD], strChip_Name[(int)ROOM_CHIPKIND_GOLD]);
				ShowMainMessageDlg(csMsg);
				
				return;
			}

			if(m_bShowRdUnion)
				m_bUnionMode = m_rdUnion1.GetCheck();
		}
	}
	
	if (m_i64MinimumMoney == 0)
		m_bVIPEnterLimitYN = true;
	
	switch(m_nGame)
	{
	case IDX_GAME_NP:
		m_nChipKind = !GetCheckRdGold() ? ROOM_CHIPKIND_NORMAL : ROOM_CHIPKIND_GOLD;
		m_nGameMode = m_rdMode1.GetCheck() ? BOMB_GAME : JOKER_GAME;
		m_nMaxUserNum = m_cbLimitUserNum.GetItemData(m_cbLimitUserNum.GetCurSel());

		break;
	case IDX_GAME_HA:
		m_nGameMode = m_rdMode1.GetCheck() ? hoola::eFORM_KIND_JOKER : hoola::eFORM_KIND_NORMAL;
		m_nGameType = m_rdStop1.GetCheck() ? hoola::eRULE_TYPE_STOP : hoola::eRULE_TYPE_WAR;
		m_nMaxUserNum = m_cbLimitUserNum.GetItemData(m_cbLimitUserNum.GetCurSel());

		break;
	case IDX_GAME_SP:
		m_nChipKind = !GetCheckRdGold()?ROOM_CHIPKIND_NORMAL:ROOM_CHIPKIND_GOLD;
		m_nGameMode = m_rdMode1.GetCheck()?CHOICE_GAME:NORMAL_GAME;

		m_nMaxUserNum = m_cbLimitUserNum.GetItemData(m_cbLimitUserNum.GetCurSel());

		break;
	case IDX_GAME_HL:
		m_nGameMode = m_rdMode1.GetCheck()?CHOICE_GAME:NORMAL_GAME;
		m_nMaxUserNum = m_cbLimitUserNum.GetItemData(m_cbLimitUserNum.GetCurSel());

		break;
	case IDX_GAME_BD:
		m_nChipKind = !GetCheckRdGold()?ROOM_CHIPKIND_NORMAL:ROOM_CHIPKIND_GOLD;
		m_nGameMode = CHOICE_GAME;	// �ο�ٵ��� Formkind �⺻���� CHOICE_GAME
		
		if (m_bShowRdBlind && m_rdBlind1.GetCheck())
		{
			m_nFormKind = FORMKIND_BLIND;
		}
		else
		{
			m_nFormKind = FORMKIND_CHOICE;
		}
		
		m_bBetRule = m_bShowRdBlind && GetCheckRdGold() && m_rdBetRule1.GetCheck();
		m_nMaxUserNum = m_cbLimitUserNum.GetItemData(m_cbLimitUserNum.GetCurSel());
		
		break;
	case IDX_GAME_SD:
		m_nGameMode = m_rdMode1.GetCheck()?SD_2CARD:SD_3CARD;
		m_nMaxUserNum = m_cbLimitUserNum.GetItemData(m_cbLimitUserNum.GetCurSel());
		
		break;
	}
	
	EndDialog(IDOK);
}

void CCreateRoomDlg::OnCancel()
{
	// TODO: Add extra cleanup here
	EndDialog(IDCANCEL);
}

void CCreateRoomDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if (cdc.m_hDC == NULL) return;

	if (m_imgBack.m_hObject) m_imgBack.DeleteObject();

	m_imgBack.LoadBitmapFile(m_strBackFile);

	cdc.SelectObject(&m_imgBack);

	// �ƽ���
	if (m_bShowMaxRoomDesc)
	{
		CDC tempDC;
		tempDC.CreateCompatibleDC(&dc);
		if ( tempDC.m_hDC == NULL )
			return;
		
		cdc.SetBkMode( TRANSPARENT );
		tempDC.SetBkMode( TRANSPARENT );
		
		if (m_rdGold.GetCheck())
		{
			tempDC.SelectObject( &m_imgHideMaxGoldDescBack );	
			cdc.BitBlt( m_ptMaxDescPos.x, m_ptMaxDescPos.y, m_imgHideMaxGoldDescBack.GetWidth(), m_imgHideMaxGoldDescBack.GetHeight(), &tempDC, 0, 0, SRCCOPY );
		}
		else
		{
			tempDC.SelectObject( &m_imgHideMaxDescBack );	
			cdc.BitBlt( m_ptMaxDescPos.x, m_ptMaxDescPos.y, m_imgHideMaxDescBack.GetWidth(), m_imgHideMaxDescBack.GetHeight(), &tempDC, 0, 0, SRCCOPY );
		}

		tempDC.DeleteDC();
	}

	if (GM().GetCurrentGameType() != IDX_GAME_HL && GM().GetCurrentGameType() != IDX_GAME_HA)
	{
		CDC tempDC;
		tempDC.CreateCompatibleDC(&dc);
		if ( tempDC.m_hDC == NULL ) return;

		cdc.SetBkMode( TRANSPARENT );
		tempDC.SetBkMode( TRANSPARENT );

		tempDC.SelectObject( &m_imgRdTypeBG );	

		cdc.BitBlt( 94, 92, m_imgRdTypeBG.GetWidth(), m_imgRdTypeBG.GetHeight(), &tempDC, 0, 0, SRCCOPY );

		tempDC.DeleteDC();
	}	

	if (GM().GetCurrentGameType() == IDX_GAME_BD)
	{
		if (m_bShowRdBlind)
		{
			CDC tempDC;
			tempDC.CreateCompatibleDC(&dc);
			if ( tempDC.m_hDC == NULL ) return;

			cdc.SetBkMode( TRANSPARENT );
			tempDC.SetBkMode( TRANSPARENT );

			tempDC.SelectObject( &m_imgRdBlind );	

			cdc.BitBlt( 25, 174, m_imgRdBlind.GetWidth(), m_imgRdBlind.GetHeight(), &tempDC, 0, 0, SRCCOPY );

			if (GetCheckRdGold())
			{
				tempDC.SelectObject( &m_imgRdBetRule );	

				cdc.BitBlt( 25, 194, m_imgRdBetRule.GetWidth(), m_imgRdBetRule.GetHeight(), &tempDC, 0, 0, SRCCOPY );
			}

			tempDC.DeleteDC();
		}
	}

	if (m_bShowRdUnion)
	{
		CDC tempDC;
		tempDC.CreateCompatibleDC(&dc);
		if ( tempDC.m_hDC == NULL ) return;

		cdc.SetBkMode( TRANSPARENT );
		tempDC.SetBkMode( TRANSPARENT );

		tempDC.SelectObject( &m_imgRdUnion );	

		cdc.BitBlt( 25, m_nBottomImgRdUnion, m_imgRdUnion.GetWidth(), m_imgRdUnion.GetHeight(), &tempDC, 0, 0, SRCCOPY );

		tempDC.DeleteDC();
	}

	// ���÷� �ȳ� ����
	if (m_stBetRuleToolTip.bBetRuleToolTip)
	{
		cdc.SetBkMode(TRANSPARENT);
		cdc.SetTextColor(RGB(69,69,69));
		cdc.SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));
		cdc.TextOut(m_stBetRuleToolTip.rtToolTIpRect.left, m_stBetRuleToolTip.rtToolTIpRect.top, m_stBetRuleToolTip.str1stLine);
		cdc.TextOut(m_stBetRuleToolTip.rtToolTIpRect.left, m_stBetRuleToolTip.rtToolTIpRect.top+18, m_stBetRuleToolTip.str2ndLine);

	}

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);

	m_imgBack.DeleteObject();

	cdc.DeleteDC();	
}

void CCreateRoomDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
	CUPGDlgBase::OnLButtonDown(nFlags, point);
}

BOOL CCreateRoomDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	return TRUE;
}

HBRUSH CCreateRoomDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CUPGDlgBase::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: Change any attributes of the DC here
	switch(nCtlColor)
	{
	case CTLCOLOR_STATIC:
		// The Slider Control has CTLCOLOR_STATIC, but doesn't let
		// the background shine through,
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255,255,255));
		TCHAR lpszClassName[255];
		GetClassName(pWnd->m_hWnd, lpszClassName, 255);
		if (_tcscmp(lpszClassName, TRACKBAR_CLASS) == 0)
			return CUPGDlgBase::OnCtlColor(pDC, pWnd, nCtlColor);
	case CTLCOLOR_BTN:
		// let static controls shine through
		pDC->SetBkMode(TRANSPARENT);
		return HBRUSH(m_cHollowBrush);
	default:
		break;
	}

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CCreateRoomDlg::OnDestroy() 
{
	CUPGDlgBase::OnDestroy();

	// 
	MAP_LIST_RULE_ITER iter = m_listRule.begin();
	while ( iter != m_listRule.end() )
	{
		map< BetRule*, RULE >::iterator iterRule = ( iter->second ).begin();

		while( iterRule !=  ( iter->second ).end() )
		{
			delete iterRule->first;

			iterRule++;
		}

		++iter;
	}

	m_listRule.clear();

	// TODO: Add your message handler code here
	if ( m_imgHideMaxDescBack.m_hObject ) m_imgHideMaxDescBack.DeleteObject();
	if ( m_imgHideMaxGoldDescBack.m_hObject ) m_imgHideMaxGoldDescBack.DeleteObject();
	if ( m_imgRdBlind.m_hObject ) m_imgRdBlind.DeleteObject();
	if ( m_imgRdBetRule.m_hObject ) m_imgRdBetRule.DeleteObject();

	m_cHollowBrush.DeleteObject();
	KillTimer(TIMER_CREATEROOMDLG_CHECKWHERE);
}

/// <summary>
/// �������� �޺��ڽ� �ɼ� ���ý� �����Լ�
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CCreateRoomDlg::OnCbnSelchangeMinimumMony()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�. 
	int nSel = m_cbMinimumMoney.GetCurSel();
	if (nSel == -1) return;
	INT64 *pi64Money = (INT64 *)m_cbMinimumMoney.GetItemData(nSel);
	SetComboAutoBetSeed(*pi64Money);
	m_i64MinimumMoney = *pi64Money;
	
	if (m_nGame == IDX_GAME_HA)
	{
		// ��ǹ� ����
		SetGiveUpRoom();
	}
	
	if (((m_nGame == IDX_GAME_BD || m_nGame == IDX_GAME_SP || m_nGame == IDX_GAME_NP)
		&& !GetCheckRdGold() && m_i64MinimumMoney < 10 * MONEY_JO)
		|| m_i64MinimumMoney == 0 || (GetCheckRdGold() && m_i64MinimumMoney < 300000))
	{
		m_edRoomTitle.ModifyStyle(NULL, WS_DISABLED, NULL);
		m_edRoomTitle.SetWindowPos(&CWnd::wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

		MM().Call(UPG_CMK_GENERATE_RANDOM_ROOM_NAME, 0, (LPARAM)(&m_strRoomTitle));
		m_edRoomTitle.SetWindowText(m_strRoomTitle);
		m_edRoomTitle.EnableWindow(FALSE);
	}
	else
	{
		m_edRoomTitle.ModifyStyle(WS_DISABLED, NULL, NULL);
		m_edRoomTitle.SetWindowPos(&CWnd::wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		m_edRoomTitle.EnableWindow(TRUE);
	}

	DWORD_PTR dwItemData = m_cbMinimumMoney.GetItemData(nSel);
	INT64 *pNum = (INT64 *)dwItemData;


	// �ƽ��Է�â ����
	CRect rt;
	HideOptionBtns();

	m_btnOK.ShowWindow(SW_SHOW);
	m_btnCancel.ShowWindow(SW_SHOW);

	// �Ʒ��� ���� ���õ� �޺��ڽ��� ���뿡 ���� ���̾�α� �Ӽ��� ��ġ�� ũ�Ⱑ ���� ���Ѵ�.
	if (*pNum == 0)
	{
		// ģ������ �ƽ��� ������ ���� �ʴ´�. 2018.05.10
		if (IsCreateMaxRoom() && !m_rdFriend.GetCheck()) // ���̾Ƹ�� ����� �ƽ�, 3�� ����.
		{
			if (m_nGame != IDX_GAME_HA)
			{
				m_btnOpenMaxDesc.ShowWindow(SW_SHOW);
			}

			// ���� �ƽ��游 ���� ����
			m_rdMaxRoom3.ShowWindow(SW_HIDE);
			m_rdMaxRoom4.ShowWindow(SW_HIDE);

			m_rdMaxRoom3.SetCheck(FALSE);
			m_rdMaxRoom4.SetCheck(TRUE);

			if (m_nGame == IDX_GAME_HA)
			{
				m_strBackFile = "PokerCommon\\image\\CreateRoom\\back_hoola.bmp";
				RedrawWindow();
				return;
			}

			// �ƽ����� ��� ����
			if (!GetCheckRdGold() || m_nGame == IDX_GAME_HL) {
				m_strBackFile.Format("PokerCommon\\image\\CreateRoom\\back_%s_free_max.bmp", GetGameStr(m_nGame));
			}
			else {
				m_strBackFile.Format("PokerCommon\\image\\CreateRoom\\back_%s_max_gold.bmp", GetGameStr(m_nGame));
				m_bShowRdUnion = IsCreateUnionRoom();
			}

			// �ƽ������� Ȱ��ȭ�Ǹ� ������ư�� �ݾ��Է� â�� Ȱ��ȭ �Ѵ�.
			if (!GetCheckRdGold()){
				m_rdMaxRoom3.ShowWindow(SW_SHOW);
				m_rdMaxRoom4.ShowWindow(SW_SHOW);
				m_rdMaxRoom3.SetCheck(TRUE);
				m_rdMaxRoom4.SetCheck(FALSE);
			}
			else
			{
				m_bIsMaxRoom = true;
				m_rdMaxRoom3.ShowWindow(SW_HIDE);
				m_rdMaxRoom4.ShowWindow(SW_HIDE);
				m_edMRMoney_Input1.ShowWindow(SW_SHOW);
				m_edMRMoney_Input2.ShowWindow(SW_SHOW);
				m_edMRMoney_Input1.EnableWindow( TRUE );
				m_edMRMoney_Input2.EnableWindow( TRUE );
			}

			if (m_bShowRdUnion)
			{
				m_rdUnion1.ShowWindow(SW_SHOW);
				m_rdUnion2.ShowWindow(SW_SHOW);
			}
			
		}else // �ƴϸ� �Ϲ� 3��
		{
			if (m_nGame == IDX_GAME_HA)
			{
				m_strBackFile = "PokerCommon\\image\\CreateRoom\\back_hoola.bmp";
				RedrawWindow();
				return;
			}

			m_strBackFile.Format("PokerCommon\\image\\CreateRoom\\back_%s_free.bmp", GetGameStr(m_nGame));
		}
	}
	else
	{
		if (m_nGame != IDX_GAME_HA)
		{
			m_btnOpenMaxDesc.ShowWindow(SW_HIDE);
			m_btnCloseMaxDesc.ShowWindow(SW_HIDE);

			m_bShowMaxRoomDesc = false;

			m_btnOK.ShowWindow( TRUE );
			m_btnCancel.ShowWindow( TRUE );
		}

		m_strBackFile.Format("PokerCommon\\image\\CreateRoom\\back_%s.bmp", GetGameStr(m_nGame));

		if (m_nGame == IDX_GAME_BD)
		{
			m_bShowRdBlind = m_i64MinimumMoney >= 10 * MONEY_JO || GetCheckRdGold();
			if (m_bShowRdBlind)
			{
				m_rdBlind1.ShowWindow(SW_SHOW);
				m_rdBlind2.ShowWindow(SW_SHOW);
			}

			if (GetCheckRdGold())
			{
				m_rdBetRule1.ShowWindow(SW_SHOW);
				m_rdBetRule2.ShowWindow(SW_SHOW);
				SYSTEMTIME systemTime;
				GetLocalTime(&systemTime);
				COleDateTime CurTime(systemTime);
				COleDateTime EndTime(GM().GetMorningBanTime());
				if (EndTime.GetStatus() == COleDateTime::valid && EndTime > CurTime)
				{
					m_rdBetRule1.SetCheck(FALSE);
					m_rdBetRule2.SetCheck(TRUE);
					m_rdBetRule1.EnableWindow(FALSE);
				}
			}
		}

		m_rdMaxRoom3.ShowWindow(SW_HIDE);
		m_rdMaxRoom4.ShowWindow(SW_HIDE);

		m_edMRMoney_Input1.ShowWindow(SW_HIDE);
		m_edMRMoney_Input2.ShowWindow(SW_HIDE);
	}

	// ���̾�α�â�� ����� �缳��
	NMBASE::SKINGDI::WindowResize2Bitmap(m_hWnd, m_strBackFile);
	UpdateData(TRUE);	

	// ��ư��ġ �����
	SetPosBtns();

	RefreshBetRuleToolTip(); // ���÷� �ȳ� ���� ����

	Invalidate(TRUE);
}

void CCreateRoomDlg::OnCbnSelchangeAutobetSeed()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	RefreshBetRuleToolTip(); // ���÷� �ȳ� ���� ����
}


void CCreateRoomDlg::OnTimer(UINT nIDEvent)
{
	if (nIDEvent==TIMER_CREATEROOMDLG_CHECKWHERE)
	{
		if (GM().GetCurrentWhere()!=IDX_GAMEWHERE_LOBY)
		{
			EndDialog(IDCANCEL);			
		}
	}
	CUPGDlgBase::OnTimer(nIDEvent);
}

void CCreateRoomDlg::OnEnChangeEditMrmoneyInput1()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CUPGDlgBase::OnInitDialog() �Լ��� ������ 
	// �ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UINT MaxRoomMoney = GetDlgItemInt( IDC_EDIT_MRMONEY_INPUT1, NULL, FALSE );

	if ( MaxRoomMoney < 0 || MaxRoomMoney > 500 )
	{
		ShowMainMessageDlg("0~500 ������ ���� �Է��ϼ���"); 
		SetDlgItemInt( IDC_EDIT_MRMONEY_INPUT1, 0 , FALSE );
	}


}

void CCreateRoomDlg::OnEnChangeEditMrmoneyInput2()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CUPGDlgBase::OnInitDialog() �Լ��� ������ 
	// �ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UINT MaxRoomMoney = GetDlgItemInt( IDC_EDIT_MRMONEY_INPUT2, NULL, FALSE );

	if ( MaxRoomMoney < 0 || MaxRoomMoney > 9999 )
	{
		ShowMainMessageDlg("0~9999 ������ ���� �Է��ϼ���");
		SetDlgItemInt( IDC_EDIT_MRMONEY_INPUT2, 0 , FALSE );
	}
}

BOOL CCreateRoomDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	switch(wParam)
	{
	case IDC_BTN_NORMAL:
	case IDC_BTN_GOLD:
	case IDC_BTN_FRIEND:
		{
			if(m_rdFriend.GetCheck() && m_bFriend)
				return FALSE;

			m_nChipKind = !GetCheckRdGold()?ROOM_CHIPKIND_NORMAL:ROOM_CHIPKIND_GOLD;
			m_bFriend = m_rdFriend.GetCheck();

			InitCBLimitUser();

			CDataRoomOptionList *pDataRoomOptionList = GM().GetDataRoomOptionList();
			if (pDataRoomOptionList == NULL) return FALSE;

			ROOM_CREATEOPTION *pRoomCreationOption = NULL;
			pRoomCreationOption = pDataRoomOptionList->GetRoomCreationOption( (ROOM_CHIPKIND)m_nChipKind );
			if (pRoomCreationOption == NULL) return FALSE;

			ROOMOPTION *pDefaultRoomOption = pDataRoomOptionList->GetDefaultRoomOption( 0, (ROOM_CHIPKIND)m_nChipKind );
			if (pDefaultRoomOption == NULL) return FALSE;
			
			CString strMoney;
			int nIndex = 0;
			int nDefault = 0;
			ROOMOPTION *pRoomOption = NULL;
			pRoomOption = NULL;

			m_cbLimitUserNum.SetCurSel(m_cbLimitUserNum.GetCount()-1);
			m_cbMinimumMoney.ResetContent();
			m_listMinimumMoney.clear();
			m_listSeedMoney.clear();
			set<INT64>	listMinimumMoney;
			for( int i=0; i < pRoomCreationOption->nArrayCnt; i++ ){
				pRoomOption = &(pRoomCreationOption->arrRoomOption[i]);
				m_listMinimumMoney.insert(pRoomOption->llEnterLimitMoney);
				m_listSeedMoney.push_back(pRoomOption->llSeedMoney);
			}
			set<INT64>::iterator start = m_listMinimumMoney.begin();
			set<INT64>::iterator end = m_listMinimumMoney.end();

			bool bZero = false;
			set<INT64>::iterator itZero;

			INT64 llDefalutEnterLimitMoney = 0;

			for (; start != end; start++)
			{
				if (*start == 0)
				{
					bZero = true;
					itZero = start;
					continue;
				}
				strMoney.Format("%s%s�̻�", NMBASE::UTIL::g_NumberToHangul(*start), strChip_Name[m_nChipKind]);

				nIndex = m_cbMinimumMoney.AddString(strMoney);
				if (0==llDefalutEnterLimitMoney)
				{
					llDefalutEnterLimitMoney = *start;
				}
				m_cbMinimumMoney.SetItemData(nIndex, (DWORD_PTR)&(*start));
				if (GM().GetRoomOptionLimitMoney(pDefaultRoomOption, true) == *start) nDefault = nIndex;
			}
			
			if (bZero)
			{
				if (!GetCheckRdGold())
				{
					strMoney = "���Ѿ���";
				}
				else
				{
					strMoney = "MAX ��";
				}
				
				if (!GetCheckRdGold() || IsCreateMaxRoom())
				{
					nIndex = m_cbMinimumMoney.AddString(strMoney);
					m_cbMinimumMoney.SetItemData(nIndex, (DWORD_PTR)&(*itZero));
					if (GM().GetRoomOptionLimitMoney(pDefaultRoomOption, true) == 0) nDefault = nIndex;
				}
			}
			
			if (m_cbMinimumMoney.GetCount() == 1)
			{
				m_cbMinimumMoney.EnableWindow(FALSE);
				m_cbMinimumMoney.SetCurSel(0);
			}
			else if (m_bFriend)
			{
				m_cbMinimumMoney.EnableWindow(FALSE);
				m_cbMinimumMoney.SetCurSel(m_cbMinimumMoney.GetCount()-1);
			}
			else
			{
				m_cbMinimumMoney.EnableWindow(TRUE);
				m_cbMinimumMoney.SetCurSel(nDefault);
			}
			
			if (SetComboAutoBetSeed(GM().GetRoomOptionLimitMoney(pDefaultRoomOption, true), GM().GetRoomOptionSeedMoney(pDefaultRoomOption, true)) == FALSE && !m_rdFriend.GetCheck())
				return FALSE;
			
			// �ƽ������� Ȱ��ȭ�Ǹ� ������ư�� �ݾ��Է� â�� Ȱ��ȭ �Ѵ�.
			m_rdMaxRoom3.ShowWindow(SW_HIDE);
			m_rdMaxRoom4.ShowWindow(SW_HIDE);
			m_rdMaxRoom3.SetCheck(TRUE);
			m_rdMaxRoom4.SetCheck(FALSE);

			if (m_bFriend)
			{
				m_btnOpenMaxDesc.ShowWindow(SW_HIDE);
				m_btnCloseMaxDesc.ShowWindow(SW_HIDE);
			}

			m_edMRMoney_Input1.ShowWindow(SW_HIDE);
			m_edMRMoney_Input2.ShowWindow(SW_HIDE);
			OnCbnSelchangeMinimumMony();

			Invalidate( FALSE );
		}
		break;
	// ���̾Ƹ��ȸ���� �ƽ��漳��
	case IDC_BTN_MAXROOM3:
		{
			m_bIsMaxRoom = false;
			m_edMRMoney_Input1.ShowWindow( FALSE );
			m_edMRMoney_Input2.ShowWindow( FALSE );
			m_edMRMoney_Input1.EnableWindow( FALSE );
			m_edMRMoney_Input2.EnableWindow( FALSE );
		}		
		break;
	case IDC_BTN_MAXROOM4:	
		{
			m_bIsMaxRoom = true;
			if (!m_bShowMaxRoomDesc)
			{
				m_edMRMoney_Input1.ShowWindow( TRUE );
				m_edMRMoney_Input2.ShowWindow( TRUE );
				m_edMRMoney_Input1.EnableWindow( TRUE );
				m_edMRMoney_Input2.EnableWindow( TRUE );
			}
		}	
		break;

	case IDC_BTN_OPEN_MAXDESC:
		m_btnOpenMaxDesc.ShowWindow(SW_HIDE);
		m_btnCloseMaxDesc.ShowWindow(SW_SHOW);

		m_bShowMaxRoomDesc = true;

		m_edMRMoney_Input1.ShowWindow( FALSE );
		m_edMRMoney_Input2.ShowWindow( FALSE );

		m_btnOK.ShowWindow( SW_HIDE );
		m_btnCancel.ShowWindow( SW_HIDE );

		Invalidate(TRUE);
		break;
	case IDC_BTN_CLOSE_MAXDESC:
		m_btnOpenMaxDesc.ShowWindow(SW_SHOW);
		m_btnCloseMaxDesc.ShowWindow(SW_HIDE);

		m_bShowMaxRoomDesc = false;

		m_btnOK.ShowWindow( SW_SHOW );
		m_btnCancel.ShowWindow( SW_SHOW );

		if (m_bIsMaxRoom)
		{
			m_edMRMoney_Input1.ShowWindow( TRUE );
			m_edMRMoney_Input2.ShowWindow( TRUE );
			m_edMRMoney_Input1.EnableWindow( TRUE );
			m_edMRMoney_Input2.EnableWindow( TRUE );
		}

		Invalidate(TRUE);
		break;
	case IDC_BTN_BLIND1:
	case IDC_BTN_BLIND2:
		RefreshBetRuleToolTip();
		break;
	case IDC_BTN_MODE1:
	case IDC_BTN_MODE2:
	case IDC_BTN_STOP1:
	case IDC_BTN_STOP2:
		SetGiveUpRoom();
		break;
	}
	RedrawWindow();

	return CUPGDlgBase::OnCommand(wParam, lParam);
}

BOOL CCreateRoomDlg::IsCreateDiaRoom()
{
#if defined(RESTRICTION_LEVEL)
	// return TRUE;
	if (true == GM().IsDiaMemberRooms()){
		return TRUE;
	}
#endif
	return FALSE;
}


BOOL CCreateRoomDlg::IsCreateMaxRoom()
{
	if (GM().GetMyInfo()->UI.PremMenuNo == CODE_NEW_FAMILY_DIAMOND ) return TRUE;

	return FALSE;
}

BOOL CCreateRoomDlg::IsCreateUnionRoom()
{
	if (GM().GetMyInfo()->UI.cafeMemberGrade_ >= CafeSystem::MemberGrade::CAFE_MEMBER && GM().GetMyInfo()->UI.unitedCafeInfo_.cafeID_ > 0 ) return TRUE;

	return FALSE;
}

BOOL CCreateRoomDlg::IsCreateMaxGoldRoom()
{
	if ( m_rdGold.GetCheck() && GM().GetMyInfo()->UI.PremMenuNo == CODE_NEW_FAMILY_DIAMOND ) return TRUE;

	return FALSE;
}

BOOL CCreateRoomDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	if ( pMsg->message == WM_KEYDOWN  )
	{
		if (pMsg->wParam==VK_TAB)
		{
			BOOL bRet = CUPGDlgBase::PreTranslateMessage(pMsg);
			Invalidate(FALSE);			
			return bRet;
		}
	}

	return CUPGDlgBase::PreTranslateMessage(pMsg);

}


void CCreateRoomDlg::SetWebSelectedRoomOption(ROOMOPTION* pRoomOption)
{
	if (pRoomOption)
	{
		m_bUseWebSelectedRoom = true;
		m_WebSelectedRoomOption = *pRoomOption;
	}
	else
	{
		m_bUseWebSelectedRoom = false;
	}	
}

void CCreateRoomDlg::RefreshBetRuleToolTip()
{
	m_stBetRuleToolTip.str1stLine.Empty();
	m_stBetRuleToolTip.str2ndLine.Empty();
	
	int nIndex = m_cbAutoBetSeed.GetCurSel();
	if (nIndex == -1) 
	{ 
		return; 
	}
	
	int nSel = m_cbMinimumMoney.GetCurSel();
	if (nSel == -1) return;

	DWORD_PTR dData = m_cbMinimumMoney.GetItemData(nSel);
	INT64 *pNum = (INT64 *)dData;

	if (*pNum == 0)
	{
		if (IsCreateMaxRoom() && !m_rdFriend.GetCheck())
			return;
	}

	DWORD_PTR dwItemData = m_cbAutoBetSeed.GetItemData(nIndex);
	RULE *pRule = (RULE *)dwItemData;
	short sAutoBet = pRule->sAutoBet;

	int cardSet = pRule->cCardSetting;
	int betMulti = (int)pRule->fBetMoneyMulti;

	if (m_nGame==IDX_GAME_BD)
	{
		if (((ROOM_CHIPKIND)m_nChipKind==ROOM_CHIPKIND_NORMAL && *pNum < 10000000000000) || m_rdBlind1.GetCheck()==FALSE)
		{
			switch(cardSet)
			{
			case 0:
				m_stBetRuleToolTip.str1stLine = "��ħ������ ������ ���� �˴ϴ�.";
				break;
			case 1:
				m_stBetRuleToolTip.str1stLine = "��ħ���� ������ ���� �˴ϴ�.";
				break;
			}	
		}	
		else
		{
			m_stBetRuleToolTip.str1stLine = "�ڽ��� �а� ������ �ʴ� ���¿���";
			m_stBetRuleToolTip.str2ndLine = "��ħ�� ������ ����˴ϴ�.";
		}
	}
	else if (m_nGame==IDX_GAME_SD)
	{
		m_stBetRuleToolTip.str1stLine = "�����и� ���� �� ������ ���� �˴ϴ�.";
	}
	else if (m_nGame==IDX_GAME_NP)
	{
		switch(cardSet)
		{

		case 5:
			m_stBetRuleToolTip.str1stLine = "5���� ī�带 ���� �� ������ ���� �˴ϴ�.";
			break;
		case 6:
			m_stBetRuleToolTip.str1stLine = "6���� ī�带 ���� �� ������ ���� �˴ϴ�.";
			break;
		}
	}
	else 
	{
		switch(cardSet)
		{
		case 4:
			m_stBetRuleToolTip.str1stLine = "4���� ī�带 ���� �� ������ ���� �˴ϴ�.";
			break;
		case 5:
			m_stBetRuleToolTip.str1stLine = "5���� ī�带 ���� �� ������ ���� �˴ϴ�.";
			break;
		case 6:
			m_stBetRuleToolTip.str1stLine = "6���� ī�带 ���� �� ������ ���� �˴ϴ�.";
			break;
		}
	}
	
	RedrawWindow();
	InvalidateRect(&m_stBetRuleToolTip.rtToolTIpRect);
}

/// <summary>
/// ������ �۾� : ������ ������ ���ý� �����Լ� 2014.01
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CCreateRoomDlg::OnCbnSelchangeComboLimitUser()
{	
	// �Ϲݹ濡���� 2,3,4�ι� ���� �����ϰ� ����. 2019.4.1 ��ġ shisky
	if (m_bFriend)
	{
		m_cbMinimumMoney.SetCurSel(m_cbMinimumMoney.GetCount() - 1);
		m_cbMinimumMoney.EnableWindow(FALSE);
		OnCbnSelchangeMinimumMony();
	}
	else if (m_cbMinimumMoney.GetCount() > 1)
	{
		m_cbMinimumMoney.EnableWindow(TRUE);
		// ��ǹ� ����
		SetGiveUpRoom();
	}
	RedrawWindow();
}

BOOL CCreateRoomDlg::GetCheckRdGold()
{
	if (m_nGame == IDX_GAME_SP || m_nGame == IDX_GAME_NP || m_nGame == IDX_GAME_BD || m_nGame == IDX_GAME_SD)
	{
		if (m_rdGold.GetCheck())
			return true;
	}
	return false;
}

/// <summary>
/// ��ǹ� ����
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CCreateRoomDlg::SetGiveUpRoom()
{
	int nSel = m_cbMinimumMoney.GetCurSel();
	if (nSel == -1) return;
	INT64 *pi64Money = (INT64 *)m_cbMinimumMoney.GetItemData(nSel);
	
	SetComboAutoBetSeed(*pi64Money);
	m_i64MinimumMoney = *pi64Money;
}

/// <summary>
/// ��ǹ� ����
/// <summary>
/// <param name=""></param>
/// <returns></returns>
bool CCreateRoomDlg::IsGiveUpRoom()
{	
	if ((ROOM_CHIPKIND)m_nChipKind==ROOM_CHIPKIND_GOLD)
		return false;

	// ### ��ǹ� ���� ###	
	int nSel = m_cbMinimumMoney.GetCurSel();
	if (nSel == -1) return false;
	INT64 *pi64Money = (INT64 *)m_cbMinimumMoney.GetItemData(nSel);

	// m_cbLimitUserNum.GetCurSel() - 0 : 2��, 1 : 3��, 2 : 4��, 3 : 5��
	if (m_cbLimitUserNum.GetCurSel() == 0) // �� ���� - 2��
	{
		switch(m_nGame)
		{
		case IDX_GAME_SP:
		case IDX_GAME_HL:
			if (m_rdMode2.GetCheck() == TRUE && // ���� ��� - �Ϲ�
				(*pi64Money == 10 * MONEY_JO || *pi64Money == 30 * MONEY_JO)) // ���� ���� - 10�� �̻� or 30�� �̻�
			{
				return true;
			}
			break;
		case IDX_GAME_BD:
			if (*pi64Money == 30 * MONEY_JO || *pi64Money == 50 * MONEY_JO) // ���� ���� - 30�� �̻� or 50�� �̻�
			{
				return true;
			}
			break;
		case IDX_GAME_NP:
			if (m_rdMode2.GetCheck() == TRUE && // ���� ��� - ��Ŀ
				(*pi64Money == 30 * MONEY_JO || *pi64Money == 50 * MONEY_JO)) // ���� ���� - 30�� �̻� or 50�� �̻�
			{
				return true;
			}
			break;
		case IDX_GAME_SD:
			if (m_rdMode1.GetCheck() == TRUE && // ���� ��� - 2��
				(*pi64Money == 50 * MONEY_JO || *pi64Money == 100 * MONEY_JO)) // ���� ���� - 50�� �̻� or 100�� �̻�
			{
				return true;
			}
			break;
		case IDX_GAME_HA:
			if (m_rdMode1.GetCheck() == TRUE && // ���� ��� - ��Ŀ
				m_rdStop2.GetCheck() == TRUE && // ���� �ɼ� - ���� off 
				(*pi64Money == 30 * MONEY_JO || *pi64Money == 100 * MONEY_JO)) // ���� ���� - 30�� �̻� or 100�� �̻�
			{
				return true;
			}
			break;
		}
	}
	
	return false;
}

void CCreateRoomDlg::HideOptionBtns()
{
	m_bShowMaxRoomDesc = FALSE;
	m_bIsMaxRoom = FALSE;
	m_edMRMoney_Input1.ShowWindow(SW_HIDE);
	m_edMRMoney_Input2.ShowWindow(SW_HIDE);
	m_edMRMoney_Input1.EnableWindow(FALSE);
	m_edMRMoney_Input2.EnableWindow(FALSE);

	m_bShowRdBlind = FALSE;
	m_rdBlind1.ShowWindow(SW_HIDE);
	m_rdBlind2.ShowWindow(SW_HIDE);	
	
	m_rdBetRule1.ShowWindow(SW_HIDE);
	m_rdBetRule2.ShowWindow(SW_HIDE);

	m_bShowRdUnion = FALSE;
	m_rdUnion1.ShowWindow(SW_HIDE);
	m_rdUnion2.ShowWindow(SW_HIDE);
}

void CCreateRoomDlg::InitPosBtns()
{
	m_btnOK.Init(0, 0, ".\\PokerCommon\\image\\CreateRoom\\btn_create.bmp", 4);
	m_btnOK.SetClickSoundID(SND_BUTTON_CLICK_B);
	m_btnCancel.Init(0, 0, ".\\PokerCommon\\image\\CreateRoom\\cancel.bmp", 4);

	m_btnOpenMaxDesc.Init(0, 0, ".\\PokerCommon\\image\\CreateRoom\\btn_open_desc.bmp", 4);
	m_btnCloseMaxDesc.Init(0, 0, ".\\PokerCommon\\image\\CreateRoom\\btn_close_desc.bmp", 4);

	m_rdMode1.Init(0, 0, ".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp", NULL, this);
	m_rdMode2.Init(0, 0, ".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp", NULL, this);
	m_rdMode1.AddToGroup(&m_rdMode2);
	m_rdMode1.SetCheck(TRUE);

	if (m_nGame == IDX_GAME_BD)
	{
		m_rdMode1.ShowWindow(SW_HIDE);
		m_rdMode2.ShowWindow(SW_HIDE);
	}
	else
	{
		m_rdMode1.ShowWindow(SW_SHOW);
		m_rdMode2.ShowWindow(SW_SHOW);
	}

	m_rdMaxRoom3.Init(0, 0, ".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp", NULL, this);// �̼���
	m_rdMaxRoom4.Init(0, 0, ".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp", NULL, this);// ����
	m_rdMaxRoom3.AddToGroup(&m_rdMaxRoom4);
	m_rdMaxRoom3.SetCheck(TRUE);

	m_rdMaxRoom3.ShowWindow(SW_HIDE);
	m_rdMaxRoom4.ShowWindow(SW_HIDE);

	SetPosBtns();
}

void CCreateRoomDlg::SetPosBtns()
{
	CRect rtBtn;
	CRect rtDlg;
	GetClientRect(&rtDlg);
	int btnPosY = rtDlg.Height() - 45; // 7poker 370 - 45

	rtBtn.SetRect(0, 0, 81, 31); // ũ��
	rtBtn.OffsetRect(55, btnPosY); // ��ġ
	m_btnOK.MoveWindow(rtBtn);

	rtBtn.SetRect(0, 0, 81, 31); // ũ��
	rtBtn.OffsetRect(172, btnPosY); // ��ġ
	m_btnCancel.MoveWindow(rtBtn);

	rtBtn = stRdSize; // ũ��
	rtBtn.OffsetRect(94, m_nGame == IDX_GAME_HA ? 91 : 116); // ��ġ
	m_rdMode1.MoveWindow(rtBtn);

	rtBtn = stRdSize; // ũ��
	rtBtn.OffsetRect(201, m_nGame == IDX_GAME_HA ? 91 : 116); // ��ġ
	m_rdMode2.MoveWindow(rtBtn);

	m_stBetRuleToolTip.rtToolTIpRect.SetRect(36, btnPosY - 62, 288, btnPosY - 29);

	rtBtn.SetRect(0, 0, 36, 14); // ũ��
	rtBtn.OffsetRect(240, btnPosY - 75); // ��ġ
	m_btnOpenMaxDesc.MoveWindow(rtBtn);
	m_btnCloseMaxDesc.MoveWindow(rtBtn);

	m_ptMaxDescPos = CPoint(14, btnPosY - 60);

	rtBtn = stRdSize; // ũ��
	rtBtn.OffsetRect(94, btnPosY - 74); // ��ġ
	m_rdMaxRoom3.MoveWindow(rtBtn);

	rtBtn = stRdSize; // ũ��
	rtBtn.OffsetRect(201, btnPosY - 74); // ��ġ
	m_rdMaxRoom4.MoveWindow(rtBtn);

	rtBtn.SetRect(0, 0, 42, 15);
	rtBtn.OffsetRect(97, btnPosY - 47);
	GetDlgItem(IDC_EDIT_MRMONEY_INPUT1)->MoveWindow(rtBtn);

	rtBtn.SetRect(0, 0, 42, 15);
	rtBtn.OffsetRect(167, btnPosY - 47);
	GetDlgItem(IDC_EDIT_MRMONEY_INPUT2)->MoveWindow(rtBtn);

	RedrawWindow();
}

CString CCreateRoomDlg::GetGameStr(int nGame)
{
	CString retStr;
	switch (nGame)
	{
	case IDX_GAME_NP:
		retStr = "newpoker";
		break;
	case IDX_GAME_HA:
		retStr = "hoola";
		break;
	case IDX_GAME_SP:
		retStr = "7poker";
		break;
	case IDX_GAME_HL:
		retStr = "hl";
		break;
	case IDX_GAME_BD:
		retStr = "lb";
		break;
	case IDX_GAME_SD:
		retStr = "sutda";
		break;
	default:
		retStr = "";
		break;
	}
	return retStr;
}