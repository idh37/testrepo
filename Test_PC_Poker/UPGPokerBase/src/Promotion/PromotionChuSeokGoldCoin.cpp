
#include "StdAfx.h"
#include "PromotionChuSeokGoldCoin.h"
#include "PromotionManager.h"

enum eMultiSceneIndex
{
	EMSI_ANIM = 0,
	EMSI_ING = 2,
	EMSI_END = 3,
	EMSI_TOOLTIP = 16,
	EMSI_BEFORE = 18,
};

enum eAnimTrackIndex
{
	EATI_COIN = 2,
	EATI_MONEY = 4,
};

enum eSceneAniIndex
{
	ESAI_COIN_SILVER = 22,
	ESAI_COIN_GOLD = 23,
	ESAI_MONEY_10_GOLD = 27,
	ESAI_MONEY_100_GOLD = 28,
	ESAI_MONEY_500_GOLD = 29,
	ESAI_MONEY_1000_GOLD = 30,
	ESAI_MONEY_5000_GOLD = 31,	
	ESAI_MONEY_10000_GOLD = 32,
	ESAI_MONEY_1000000_GOLD = 33,
};

INT64 CPromotionChuSeokGoldCoin::RUN_LOWER_LIMIT;
INT64 CPromotionChuSeokGoldCoin::RUN_UPPER_LIMIT;


CPromotionChuSeokGoldCoin::CPromotionChuSeokGoldCoin(CPromotionManager *pManager, int nType)
: CPromotionBase(pManager, nType),
m_curEventCnt(0), m_playCnt(0), m_curRemainAnimTime(2), m_coinIndex(-1), m_rewardIndex(-1),
m_drawToolToip(false)
{
	string resourcePath = ".\\PokerCommon\\data\\Event\\ChuSeokGoldCoin\\";
	_LOADSPRITEDEFINE(&m_resource, resourcePath, "evt_2018chusuk.spr");
	m_goldCoinEffect.SetSprite(&m_resource);
	
	MM().AddMsgH(UPG_CMK_SHOW_CHUSEOK_GOLDCOIN, Fnt(this, &CPromotionChuSeokGoldCoin::ShowGoldCoinAnim));
}

CPromotionChuSeokGoldCoin::~CPromotionChuSeokGoldCoin()
{
	MM().RemoveMsgH(UPG_CMK_SHOW_CHUSEOK_GOLDCOIN);
}

void CPromotionChuSeokGoldCoin::DrawMiddle(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage)
{
	if(!_IsEventRoom() || g_RI.ChipKind == ROOM_CHIPKIND_GOLD)
	{
		return;
	}

	DWORD curTime = timeGetTime();

	if(m_curEventCnt >= MAX_NUMBER_EFFECT)
	{
		pPage->DrawMultiScene(0, 0, &m_resource, EMSI_END, curTime);
	}
	else if(m_playCnt)
	{
		pPage->DrawMultiScene(0, 0, &m_resource, EMSI_ING, curTime);
		pPage->DrawMultiScene(0, 0, &m_resource, EMSI_END + m_playCnt, curTime);
	}
	else
	{ 
		pPage->DrawMultiScene(0, 0, &m_resource, EMSI_BEFORE, curTime);
		_DrawTextOfPromotionProgress(pDC, 145, 482);
	}

	if(m_drawToolToip)
	{
		pPage->DrawMultiScene(0, 0, &m_resource, EMSI_TOOLTIP, curTime);
	}

	m_goldCoinEffect.Draw(pDC, pPage);
}

BOOL CPromotionChuSeokGoldCoin::OnMouseMoveMiddle(int& x , int& y)
{
	if(!_IsEventRoom() || g_RI.ChipKind == ROOM_CHIPKIND_GOLD)
	{
		return FALSE;
	}

	CRect resrcArea = CRect(8, 464, 8 + 288, 464 + 54);
	m_drawToolToip = false;

	if(resrcArea.PtInRect(CPoint(x, y)))
	{
		m_drawToolToip = true;
	}

	return FALSE;
}

void CPromotionChuSeokGoldCoin::OnTimer()
{
	if(m_curRemainAnimTime > 0)
	{
		--m_curRemainAnimTime;
	}
}

BOOL CPromotionChuSeokGoldCoin::OnPacketNotify(char* pName, int& signal, int& totSize, char* lpData)
{
	switch(signal)
	{
	case  OPCODE_SV_2018THANKSGIVING_GAMECOUNT:
		_SetData(totSize, lpData);
		break;
	case OPCODE_SV_2018THANKSGIVING_GAMECOUNT_INFO:
		_SetDataLimits(totSize, lpData);
		break;
	}

	return FALSE;
}

LRESULT CPromotionChuSeokGoldCoin::ShowGoldCoinAnim(WPARAM& wParam, LPARAM& lParam)
{
	if(m_playCnt == UNIT_ONE_EFFECT && m_curEventCnt <= MAX_NUMBER_EFFECT)
	{
		if(m_goldCoinEffect.IsShow())
		{
			return TRUE;
		}

		m_goldCoinEffect.ShowEffect(EMSI_ANIM, TRUE, FALSE);
		m_goldCoinEffect.ChangeScean(EATI_COIN, m_coinIndex);
		m_goldCoinEffect.ChangeScean(EATI_MONEY, m_rewardIndex);		

		m_curRemainAnimTime = 2;
		m_playCnt = 0;
	}

	return TRUE;
}

void CPromotionChuSeokGoldCoin::_DrawTextOfPromotionProgress(CDC* pDC, int x, int y)
{
	CString progress;
	CFont* pOldFont = pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_16D_BOLD));
	COLORREF oldTxtClr = pDC->SetTextColor(RGB(240, 163, 42));
	UINT oldTxtAlign = pDC->SetTextAlign(TA_LEFT);

	progress.Format("%d회 / %d회 진행", m_curEventCnt, 30);
	pDC->TextOut(x, y, progress);

	pDC->SelectObject(pOldFont);
	pDC->SetTextColor(oldTxtClr);
	pDC->SetTextAlign(oldTxtAlign);
}

void CPromotionChuSeokGoldCoin::_SetData(int totSize, char* lpData)
{
	packet::TSendPacket<promotion::PROMOTION_2018THANKSGIVING_GAMECOUNT> gameCountPacket;
	gameCountPacket.Get(lpData, totSize);
	promotion::PROMOTION_2018THANKSGIVING_GAMECOUNT* pGameCountData = static_cast<promotion::PROMOTION_2018THANKSGIVING_GAMECOUNT*>(gameCountPacket.Various());

	m_curEventCnt = pGameCountData->nTodayGameCount / UNIT_ONE_EFFECT;
	m_playCnt = pGameCountData->nTodayGameCount % UNIT_ONE_EFFECT;

	if(m_playCnt == 0 && (pGameCountData->llReward || pGameCountData->llAdditionalReward))
	{
		m_playCnt = UNIT_ONE_EFFECT;
	}

	if(pGameCountData->llAdditionalReward)
	{
		m_coinIndex = ESAI_COIN_GOLD;

		if(pGameCountData->llAdditionalReward == 500)
		{
			m_rewardIndex = ESAI_MONEY_500_GOLD;
		}
		else if(pGameCountData->llAdditionalReward == 5000)
		{
			m_rewardIndex = ESAI_MONEY_5000_GOLD;
		}
		else if(pGameCountData->llAdditionalReward == 10000)
		{
			m_rewardIndex = ESAI_MONEY_10000_GOLD;
		}
		else if(pGameCountData->llAdditionalReward == 1000000)
		{
			m_rewardIndex = ESAI_MONEY_1000000_GOLD;
		}
	}
	else if(pGameCountData->llReward)
	{
		m_coinIndex = ESAI_COIN_SILVER;

		if(pGameCountData->llReward == 10)
		{
			m_rewardIndex = ESAI_MONEY_10_GOLD;
		}
		else if(pGameCountData->llReward == 100)
		{
			m_rewardIndex = ESAI_MONEY_100_GOLD;
		}
		else if(pGameCountData->llReward == 1000)
		{
			m_rewardIndex = ESAI_MONEY_1000_GOLD;
		}
		else if(pGameCountData->llReward == 10000)
		{
			m_rewardIndex = ESAI_MONEY_10000_GOLD;
		}
	}

	GM().GetPromotionData()->nCnts[0] = (int)CPromotionManager::EPT_POKER_2018_CHUSEOK;

	GM().SetShowEndEventPopup(true);
}

void CPromotionChuSeokGoldCoin::_SetDataLimits(int totSize, char* lpData)
{
	packet::TSendPacket<promotion::PROMOTION_2018THANKSGIVING_GAMECOUNT_INFO> gameCountInfoPacket;
	gameCountInfoPacket.Get(lpData, totSize);
	promotion::PROMOTION_2018THANKSGIVING_GAMECOUNT_INFO* pGameCountInfoData = static_cast<promotion::PROMOTION_2018THANKSGIVING_GAMECOUNT_INFO*>(gameCountInfoPacket.Various());

	RUN_LOWER_LIMIT = pGameCountInfoData->llRunLowerLimit;
	RUN_UPPER_LIMIT = pGameCountInfoData->llRunUpperLimit;
}

bool CPromotionChuSeokGoldCoin::_IsEventRoom()
{
	if(RUN_LOWER_LIMIT <= g_RI.llEnterLimitMoney && g_RI.llEnterLimitMoney <= RUN_UPPER_LIMIT)
	{
		return true;
	}
	else
	{
		return false;
	}
}