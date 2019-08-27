// OwnLimit.cpp: implementation of the COwnLimit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OwnLimit.h"
#include "OwnLimitDlg.h"
#include "../UIInfoData.h"
#include "../Sound.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
COwnLimit::COwnLimit()
{
	m_i64MaxPremiumMoneyWithOption=0;
	m_i64MaxPremiumGoldWithOption=0;
	m_i64MaxPremiumGoldBankWithOption=0;
	m_bShowMark = false;
	m_bShowMarkGold = false;
	m_pUIInfoData = NULL;
	m_pGameProcess = NULL;
	m_pGameView = NULL;
	m_pPage = NULL;
	m_bMouseOver = false;
	m_strGold = "0포커칩";
	m_strMoney = "0코인";
}

COwnLimit::~COwnLimit()
{
	m_mapOtherUserInfo.clear();
	m_mapOtherUserInfoGold.clear();
	m_mapEffectPlayInfo.clear();
	m_bShowMark = false;
	m_bShowMarkGold = false;
}

void COwnLimit::LoadImage(LPCTSTR lpEffectPath)
{
	AddLoadImageList(&m_sprHando,".\\PokerCommon\\data\\LimitMoney.spr", true);
	AddLoadImageList(&m_sprHandoEffect,lpEffectPath, true);
}

void COwnLimit::Init(CUIInfoData *pInfoData, CGameProcess *pProcess)
{
	//패킷이 날아오는 순서때문에 두번 초기화를 하게 된다.
	m_pUIInfoData	= pInfoData;
	m_pGameProcess	= pProcess;
	m_pGameView		= m_pGameProcess->GetGame()->GetGameView();
	m_pPage			= &m_pGameView->Page;

	if(m_sprHando.spr)
	{
		m_rtMark = CRect(0,0,m_sprHando.spr[2].xl,m_sprHando.spr[2].yl);
		m_rtMark.OffsetRect(m_pUIInfoData->m_listAvatarPos[0] + m_pUIInfoData->m_ptOwnLimitMarkGab);
	}
	m_bMouseOver = false;
}

void COwnLimit::OnEnterRoom(CUIInfoData *pInfoData, CGameProcess *pProcess)
{
	Init(pInfoData, pProcess);
	m_bMouseOver = false;
	CheckExpire();
}

void COwnLimit::OnExitRoom(void)
{
	m_mapOtherUserInfo.clear();	//방을 나갈 때 다른 유저 정보는 clear
	m_mapOtherUserInfoGold.clear();	//방을 나갈 때 다른 유저 정보는 clear
	ShowPopup();
}

void COwnLimit::OnGameOver(void)
{
	CheckExpire();
}

BOOL COwnLimit::OnMouseMove(int x , int y)
{
	m_bMouseOver = false;

	if( (m_bShowMark || m_bShowMarkGold) &&	m_rtMark.PtInRect(CPoint(x,y)))
	{
		SetCursor(GM().GetCurHandCursor());
		m_bMouseOver = true;
		return TRUE;
	}
	return FALSE;
}

void COwnLimit::CheckExpire()
{
	//보유한도 유예시간이 지났을때 m_stOverMoneyInfo.IsExpired()
	//VVIP회원이면서 보유한도 옵션을 가진 유저일때 m_i64MaxPremiumMoneyWithOption == g_MaxServerMoney
	if (m_bShowMark && g_RI.ChipKind == (int)ROOM_CHIPKIND_NORMAL)
	{
		if(m_stOverMoneyInfo.IsExpired() || m_i64MaxPremiumMoneyWithOption == g_MaxServerMoney)
			m_bShowMark = false;
	}
	
	// 골드
	if (m_bShowMarkGold && g_RI.ChipKind == (int)ROOM_CHIPKIND_GOLD)
	{
		if(m_stOverGoldInfo.IsExpired() || m_i64MaxPremiumGoldWithOption == g_MaxServerGold)
			m_bShowMarkGold = false;
	}

	// 골드 적립 통장
	if (m_bShowMarkGoldBank && g_RI.ChipKind == (int)ROOM_CHIPKIND_GOLD_BANK)
	{
		if(m_stOverGoldBankInfo.IsExpired() || m_i64MaxPremiumGoldBankWithOption == g_MaxServerGold)
			m_bShowMarkGoldBank = false;
	}
}

void COwnLimit::ResetNotToday(const char *pMyID)
{
	for(int i=HANDOPOPUP_NULL+1; i<HANDOPOPUP_MAX; i++)
	{
		CString strKey;
		strKey.Format("DoNotShowHandoThisDay_%d", i);
		NMBASE::UTIL::WriteProfile_int( pMyID, strKey, 0 );
	}
}

void COwnLimit::SetOverMoney(INT64 llMoney, int chipKind, SYSTEMTIME *pExpire)
{
	CString strChatMsg;

	// 머니 한도 초과
	if (chipKind == (int)ROOM_CHIPKIND_NORMAL)
	{
		if (m_stOverMoneyInfo.Set(llMoney, pExpire))
		{
			m_strMoney = (CString)NMBASE::UTIL::g_NumberToHangul(llMoney);
			m_strMoney.Append(strChip_Name[chipKind]);

			//세팅 되었을 경우 오늘하루 열기 리셋
			if (GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
				ResetNotToday(GM().GetMyInfo()->UI.ID);

			//아바타 아래 아이콘 영역
			//VVIP회원이면서 보유한도 옵션을 가진 유저일때
			if (m_i64MaxPremiumMoneyWithOption == g_MaxServerMoney)
			{
				m_bShowMark = false;
			}
			else
			{
				m_bShowMark = true;
			}

			//이펙트
			ShowAniEffect(0);
		}

		strChatMsg.Format("◈[%s]님 보유한도 달성\n 보유한도 초과금 : %s%s이 회수 되었습니다.\n", GM().GetMyInfo()->UI.NickName, NMBASE::UTIL::g_MoneyMark(llMoney), strChip_Name[chipKind]);
	}
	// 골드 한도 초과
	else if (chipKind == (int)ROOM_CHIPKIND_GOLD)
	{
		if(m_stOverGoldInfo.Set(llMoney, pExpire))
		{
			m_strGold = (CString)NMBASE::UTIL::g_NumberToHangul(llMoney);
			m_strGold.Append(strChip_Name[chipKind]);

			//세팅 되었을 경우 오늘하루 열기 리셋
			if (GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
				ResetNotToday(GM().GetMyInfo()->UI.ID);

			//아바타 아래 아이콘 영역
			//VVIP회원이면서 보유한도 옵션을 가진 유저일때
			if (m_i64MaxPremiumGoldWithOption == g_MaxServerGold)
			{
				m_bShowMarkGold = false;
			}
			else
			{
				m_bShowMarkGold = true;
			}

			//이펙트
			ShowAniEffect(0);
		}

		strChatMsg.Format("◈[%s]님 보유한도 달성\n 보유한도 초과금 : %s%s이 회수 되었습니다.\n", GM().GetMyInfo()->UI.NickName, NMBASE::UTIL::g_MoneyMark(llMoney), strChip_Name[chipKind]);
	}
	// 골드 적립 통장 한도 초과
	else if (chipKind == (int)ROOM_CHIPKIND_GOLD_BANK)
	{
		if(m_stOverGoldBankInfo.Set(llMoney, pExpire))
		{
			//m_strGold = (CString)NMBASE::UTIL::g_NumberToHangul(llMoney);
			//m_strGold.Append("골드");

			//세팅 되었을 경우 오늘하루 열기 리셋
			if (GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
				ResetNotToday(GM().GetMyInfo()->UI.ID);

			//아바타 아래 아이콘 영역
			//VVIP회원이면서 보유한도 옵션을 가진 유저일때
			/*
			if (m_i64MaxPremiumGoldBankWithOption == g_MaxServerGoldBank)
			{
				m_bShowMarkGoldBank = false;
			}
			else
			{
				m_bShowMarkGoldBank = true;
			}
			*/

			//이펙트
			//ShowAniEffect(0);
		}

		strChatMsg.Format("◈[%s]님 %s 적립한도 달성\n %s 적립한도 초과금 : %s%s이 회수 되었습니다.\n", GM().GetMyInfo()->UI.NickName, strChip_Name[(int)ROOM_CHIPKIND_GOLD], strChip_Name[(int)ROOM_CHIPKIND_GOLD], NMBASE::UTIL::g_MoneyMark(llMoney), strChip_Name[(int)ROOM_CHIPKIND_GOLD]);
	}
	
	//채팅
	AddGameInfoViewEdit(strChatMsg, RGB(227, 227, 0));
}

void COwnLimit::SetOtherUserOverMoney(const char *pID, INT64 llMoney, int chipKind, SYSTEMTIME *pExpire)
{
	int pn = GM().GetCurrentProcess()->GetPlayerPNum(pID);
	if (pn<0)
		return;	

	if (m_mapOtherUserInfo.find(pID)==m_mapOtherUserInfo.end())
	{
		LimitMoneyInfo LMI;
		if(chipKind == (int)ROOM_CHIPKIND_GOLD)
		{
			m_mapOtherUserInfoGold.insert(pair<CString, LimitMoneyInfo>(pID, LMI));	
		}
		else
		{
			m_mapOtherUserInfo.insert(pair<CString, LimitMoneyInfo>(pID, LMI));		
		}
	}

	if(chipKind == (int)ROOM_CHIPKIND_GOLD)
	{
		if (m_mapOtherUserInfoGold[pID].Set(llMoney, pExpire))
		{
			ShowAniEffect(pn);
		}
	}
	else
	{
		if (m_mapOtherUserInfo[pID].Set(llMoney, pExpire))
		{
			ShowAniEffect(pn);
		}
	}
}

void COwnLimit::SetRevisionMoney(INT64 llMoney, int chipKind, SYSTEMTIME *pExpire)
{
	CString strChatMsg;

	if (chipKind == (int)ROOM_CHIPKIND_NORMAL)
	{
		if (m_stRevisionMoneyInfo.Set(llMoney, pExpire))
		{
			//세팅 되었을 경우 오늘하루 열기 리셋
			if (GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
				ResetNotToday(GM().GetMyInfo()->UI.ID);
		}
		strChatMsg.Format("◈[%s]님 회원 상태 변경\n 보유한도 초과금 : %s%s이 회수 되었습니다.\n", GM().GetMyInfo()->UI.NickName, NMBASE::UTIL::g_MoneyMark(llMoney), strChip_Name[chipKind]);
	}
	else if (chipKind == (int)ROOM_CHIPKIND_GOLD)
	{
		if (m_stRevisionGoldInfo.Set(llMoney, pExpire))
		{
			//세팅 되었을 경우 오늘하루 열기 리셋
			if (GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
				ResetNotToday(GM().GetMyInfo()->UI.ID);
		}

		strChatMsg.Format("◈[%s]님 회원 상태 변경\n 보유한도 초과금 : %s%s이 회수 되었습니다.\n", GM().GetMyInfo()->UI.NickName, NMBASE::UTIL::g_MoneyMark(llMoney), strChip_Name[chipKind]);
	}
	else if (chipKind == (int)ROOM_CHIPKIND_GOLD_BANK)
	{
		if (m_stRevisionGoldBankInfo.Set(llMoney, pExpire))
		{
			//세팅 되었을 경우 오늘하루 열기 리셋
			if (GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
				ResetNotToday(GM().GetMyInfo()->UI.ID);
		}

		strChatMsg.Format("◈[%s]님 회원 상태 변경\n 보유한도 초과금 : %s%s이 회수 되었습니다.\n", GM().GetMyInfo()->UI.NickName, NMBASE::UTIL::g_MoneyMark(llMoney), strChip_Name[(int)ROOM_CHIPKIND_GOLD]);
	}

	//채팅
	AddGameInfoViewEdit(strChatMsg, RGB(227, 227, 0));
}

void COwnLimit::ShowAniEffect(int p)
{
	const int nMaxPlayer = m_pGameProcess->GetMaxPlayer();
	if(p<0 || p>=nMaxPlayer) return;

	EFFECT_PLAY_INFO info;
	info.m_dwStartTime = timeGetTime();
	info.m_dwEndTime = info.m_dwStartTime + m_sprHandoEffect.GetMScenePlayTime(p);

	m_mapEffectPlayInfo[p] = info;
	PBPlayEffectSound(SND_HANDOEFT);
}

void COwnLimit::ShowPopup()
{
#ifdef _BETAVER
	return;
#endif	

	//COwnLimitDlg dlg(GM().GetMainWnd());	

	if (m_dlgOwnLimit.GetSafeHwnd())
	{
		//이미 팝업창이 떠있으면 리턴
		return;
	}

	if (GM().GetQuickStartInRoom())
	{
		//다른방 바로가기 상태면 리턴
		GM().SetQuickStartInRoom(false);

		return;
	}

	if (GM().GetPlayDailyGoldEffect() == true)
	{
		GM().SetWaitOwnLimitPopup(true);

		return;
	}

	// 2000조 머니 or 1500만 골드 초과시 
	INT64 nMyMoney = GM().GetMyInfo()->UI.GetMoney();
	INT64 nMyGold = GM().GetMyGold();

	if (nMyMoney >= m_i64MaxPremiumMoneyWithOption ||
		nMyGold >= m_i64MaxPremiumGoldWithOption)
	{
		//COwnLimitDlg dlg(GM().GetMainWnd());			
		m_dlgOwnLimit.DoModal(HANDOPOPUP_ACHIEVE_LIMIT_MONEY);//UPGCommon 작업

		return;
	}
	
	HANDOPOPUP Hando_Popup_Type = HANDOPOPUP_NULL;
	
	//내가 VVIP이고 한도업상품가입이라면 (내가가질수 있는 최대보유한도와 포커에서가질수 있는 최대보유한도가 같다면 )팝업은 보여주지않는다.
	if (m_i64MaxPremiumMoneyWithOption != g_MaxServerMoney ||
		m_i64MaxPremiumGoldWithOption != g_MaxServerGold)
	{
		/*
		if (m_stOverMoneyInfo.IsExpired()==FALSE && 
			m_stRevisionMoneyInfo.IsExpired()==FALSE)		//한도, 보정 머니가 같이 발생했을 때
		{		
			Hando_Popup_Type=HANDOPOPUP_COLLECT_BOTH_MONEY;	
		}
		else if (m_stOverMoneyInfo.IsExpired()==FALSE)		//한도 초과 머니만 발생했을 때
		{		
			Hando_Popup_Type=HANDOPOPUP_COLLECT_OVER_MONEY;
		}
		else if (m_stRevisionMoneyInfo.IsExpired()==FALSE)		// 보정 머니만 발생했을 때
		{
			Hando_Popup_Type=HANDOPOPUP_COLLECT_REVISION_MONEY;		
		}
		*/

		// 2017.12.13 골드시스템 추가
		// 한도 초과, 보정 머니(멤버스 변경으로 인해 발생)
		// 상관없이 한 종류의 팝업만 띄우기로 함.
		if (m_stOverMoneyInfo.IsExpired() == FALSE || 
			m_stRevisionMoneyInfo.IsExpired() == FALSE ||
			m_stOverGoldInfo.IsExpired() == FALSE || 
			m_stRevisionGoldInfo.IsExpired() == FALSE)
		{
			Hando_Popup_Type=HANDOPOPUP_COLLECT_OVER_MONEY;
		}
	}
	
	if (m_stOverGoldBankInfo.IsExpired() == FALSE || 
		m_stRevisionGoldBankInfo.IsExpired() == FALSE)
	{
		Hando_Popup_Type = HANDOPOPUP_COLLECT_OVER_GOLDBANK;
	}
	
	if (Hando_Popup_Type != HANDOPOPUP_NULL)
	{
		//COwnLimitDlg dlg(GM().GetMainWnd());	
		m_dlgOwnLimit.DoModal(Hando_Popup_Type);
	}
	
//#if defined(_DEBUG)
//	else{
//		Hando_Popup_Type=HANDOPOPUP_COLLECT_OVER_MONEY;
//		m_dlgOwnLimit.DoModal(Hando_Popup_Type);
//	}
//#endif
}

void COwnLimit::DrawBottom(void)
{
	bool bShowMark = m_bShowMarkGold || m_bShowMark;
	if(m_mapEffectPlayInfo.find(0) != m_mapEffectPlayInfo.end())
	{
		bShowMark = false;
	}

	if(bShowMark && !g_bMyObserver)
	{
		CPoint ptPos = m_pUIInfoData->m_listAvatarPos[0] + m_pUIInfoData->m_ptOwnLimitMarkGab;
		m_pPage->PutSprAuto(ptPos.x, ptPos.y, &m_sprHando, m_bMouseOver?1:0);
	}
}

void COwnLimit::DrawTop(CDC *pDC)
{
	bool bShowMark = m_bShowMarkGold || m_bShowMark;
	if(m_mapEffectPlayInfo.find(0) != m_mapEffectPlayInfo.end())
	{
		bShowMark = false;
	}

	if(m_mapEffectPlayInfo.size())
	{
		DWORD dwCurTime = timeGetTime();
		map<int, EFFECT_PLAY_INFO>::iterator start = m_mapEffectPlayInfo.begin();
		map<int, EFFECT_PLAY_INFO>::iterator end = m_mapEffectPlayInfo.end();
		while(start != end)
		{
			if(start->second.m_dwStartTime <= dwCurTime)
			{
				if(start->second.m_dwEndTime <= dwCurTime)
				{
					start = m_mapEffectPlayInfo.erase(start);
					continue;
				}

				m_pPage->DrawMultiScene(0, 0, &m_sprHandoEffect, start->first, dwCurTime - start->second.m_dwStartTime, FALSE);
			}
			start++;
		}
	}

	if(bShowMark && m_bMouseOver)
	{
		CPoint ptPos = m_pUIInfoData->m_listAvatarPos[0] + m_pUIInfoData->m_ptOwnLimitMarkGab + m_pUIInfoData->m_ptOwnLimitTipGab;
		m_pPage->PutSprAuto(ptPos.x, ptPos.y, &m_sprHando, 3);

		ptPos += m_pUIInfoData->m_ptOwnLimitMoneyGab;
		CRect rtRect = m_pUIInfoData->m_rtOwnLimitMoneySize;
		rtRect.OffsetRect(ptPos);

		CFont *pFontOld = pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));
		COLORREF clrOld = pDC->SetTextColor(m_pUIInfoData->m_clrOwnLimitMoney);

		pDC->DrawText(m_strMoney, m_strMoney.GetLength(), &rtRect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
		rtRect.OffsetRect(m_pUIInfoData->m_ptOwnLimitGoldGab);
		pDC->DrawText(m_strGold, m_strGold.GetLength(), &rtRect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

		pDC->SetTextColor(clrOld);
		pDC->SelectObject(pFontOld);
	}
}


BOOL COwnLimit::OnPacketNotify(char* pName, int Signal, int TotSize, char *lpData)
{
#ifdef _BETAVER
	return FALSE;
#endif
	switch(Signal)
	{
// 		case SV_USERINFO:				삭제.. Lobby.cpp에서 처리
// 			return OnReceiveUserInfo(TotSize, lpData);
		case SV_NOTIFY_CHANGEOVERMONEY:	
			return OnReceiveNotifyChangeOverMoney(TotSize, lpData);
	}

	return FALSE;
}


//유저 정보창은 게임방/대기실 동일하고 게임쪽에 존재하지 때문에 게임쪽에서 띄운다.
BOOL COwnLimit::OnReceiveUserInfo(int TotSize, char *lpData)
{
	CSV_USERINFO msg;
	msg.Get(lpData, TotSize);

	GM().SetWaitResponse(false);

	if ( GM().GetCurrentWhere() != IDX_GAMEWHERE_GAME)
		return FALSE;

	// 아이디 길이가 0이면 현재 명단에 없는 사용자임
	if(strlen(msg.UI->ID) == 0)
		return FALSE;

	static BOOL bProccessed = FALSE;
	if(bProccessed != FALSE) 
		return FALSE;
	bProccessed = TRUE;
	ShowProfileDialog(msg.UI, NULL);
	bProccessed = FALSE;
	return TRUE;
}



BOOL COwnLimit::OnReceiveNotifyChangeOverMoney(int TotSize, char *lpData)
{
	CSV_NOTIFY_CHANGEOVERMONEY msg;
	msg.Get(lpData,TotSize);

	// 내가 한도 초과
	if (strncmp(msg.szID, GM().GetMyInfo()->UI.ID, 15) == 0)
	{
		// 초과금이 있을 경우
		if (*msg.llOverMoney > 0)
		{
			// 초과금 세팅
			SetOverMoney(*msg.llOverMoney, *msg.m_ChipKind, msg.stOverMoneyEndDate);
		}

		// 보정 머니가 있을 경우
		if (*msg.llRevisionMoney > 0)
		{
			// 보정 머니 세팅
			SetRevisionMoney(*msg.llRevisionMoney, *msg.m_ChipKind, msg.stRevisionMoneyEndDate);
		}
	}
	// 다른 유저 한도 초과
	else
	{
		SetOtherUserOverMoney(msg.szID, *msg.llOverMoney, *msg.m_ChipKind, msg.stOverMoneyEndDate);
	}

	// 인게임이 아닐 경우
	if (GM().GetCurrentWhere() != IDX_GAMEWHERE_GAME)
	{
		// 팝업을 띄움
		ShowPopup();
	}

	return TRUE;
}