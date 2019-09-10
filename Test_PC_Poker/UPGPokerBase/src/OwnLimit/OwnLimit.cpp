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
	m_strGold = "0��ĿĨ";
	m_strMoney = "0����";
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
	//��Ŷ�� ���ƿ��� ���������� �ι� �ʱ�ȭ�� �ϰ� �ȴ�.
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
	m_mapOtherUserInfo.clear();	//���� ���� �� �ٸ� ���� ������ clear
	m_mapOtherUserInfoGold.clear();	//���� ���� �� �ٸ� ���� ������ clear
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
	//�����ѵ� �����ð��� �������� m_stOverMoneyInfo.IsExpired()
	//VVIPȸ���̸鼭 �����ѵ� �ɼ��� ���� �����϶� m_i64MaxPremiumMoneyWithOption == g_MaxServerMoney
	if (m_bShowMark && g_RI.ChipKind == (int)ROOM_CHIPKIND_NORMAL)
	{
		if(m_stOverMoneyInfo.IsExpired() || m_i64MaxPremiumMoneyWithOption == g_MaxServerMoney)
			m_bShowMark = false;
	}
	
	// ���
	if (m_bShowMarkGold && g_RI.ChipKind == (int)ROOM_CHIPKIND_GOLD)
	{
		if(m_stOverGoldInfo.IsExpired() || m_i64MaxPremiumGoldWithOption == g_MaxServerGold)
			m_bShowMarkGold = false;
	}

	// ��� ���� ����
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

	// �Ӵ� �ѵ� �ʰ�
	if (chipKind == (int)ROOM_CHIPKIND_NORMAL)
	{
		if (m_stOverMoneyInfo.Set(llMoney, pExpire))
		{
			m_strMoney = (CString)NMBASE::UTIL::g_NumberToHangul(llMoney);
			m_strMoney.Append(strChip_Name[chipKind]);

			//���� �Ǿ��� ��� �����Ϸ� ���� ����
			if (GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
				ResetNotToday(GM().GetMyInfo()->UI.ID);

			//�ƹ�Ÿ �Ʒ� ������ ����
			//VVIPȸ���̸鼭 �����ѵ� �ɼ��� ���� �����϶�
			if (m_i64MaxPremiumMoneyWithOption == g_MaxServerMoney)
			{
				m_bShowMark = false;
			}
			else
			{
				m_bShowMark = true;
			}

			//����Ʈ
			ShowAniEffect(0);
		}

		strChatMsg.Format("��[%s]�� �����ѵ� �޼�\n �����ѵ� �ʰ��� : %s%s�� ȸ�� �Ǿ����ϴ�.\n", GM().GetMyInfo()->UI.NickName, NMBASE::UTIL::g_MoneyMark(llMoney), strChip_Name[chipKind]);
	}
	// ��� �ѵ� �ʰ�
	else if (chipKind == (int)ROOM_CHIPKIND_GOLD)
	{
		if(m_stOverGoldInfo.Set(llMoney, pExpire))
		{
			m_strGold = (CString)NMBASE::UTIL::g_NumberToHangul(llMoney);
			m_strGold.Append(strChip_Name[chipKind]);

			//���� �Ǿ��� ��� �����Ϸ� ���� ����
			if (GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
				ResetNotToday(GM().GetMyInfo()->UI.ID);

			//�ƹ�Ÿ �Ʒ� ������ ����
			//VVIPȸ���̸鼭 �����ѵ� �ɼ��� ���� �����϶�
			if (m_i64MaxPremiumGoldWithOption == g_MaxServerGold)
			{
				m_bShowMarkGold = false;
			}
			else
			{
				m_bShowMarkGold = true;
			}

			//����Ʈ
			ShowAniEffect(0);
		}

		strChatMsg.Format("��[%s]�� �����ѵ� �޼�\n �����ѵ� �ʰ��� : %s%s�� ȸ�� �Ǿ����ϴ�.\n", GM().GetMyInfo()->UI.NickName, NMBASE::UTIL::g_MoneyMark(llMoney), strChip_Name[chipKind]);
	}
	// ��� ���� ���� �ѵ� �ʰ�
	else if (chipKind == (int)ROOM_CHIPKIND_GOLD_BANK)
	{
		if(m_stOverGoldBankInfo.Set(llMoney, pExpire))
		{
			//m_strGold = (CString)NMBASE::UTIL::g_NumberToHangul(llMoney);
			//m_strGold.Append("���");

			//���� �Ǿ��� ��� �����Ϸ� ���� ����
			if (GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
				ResetNotToday(GM().GetMyInfo()->UI.ID);

			//�ƹ�Ÿ �Ʒ� ������ ����
			//VVIPȸ���̸鼭 �����ѵ� �ɼ��� ���� �����϶�
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

			//����Ʈ
			//ShowAniEffect(0);
		}

		strChatMsg.Format("��[%s]�� %s �����ѵ� �޼�\n %s �����ѵ� �ʰ��� : %s%s�� ȸ�� �Ǿ����ϴ�.\n", GM().GetMyInfo()->UI.NickName, strChip_Name[(int)ROOM_CHIPKIND_GOLD], strChip_Name[(int)ROOM_CHIPKIND_GOLD], NMBASE::UTIL::g_MoneyMark(llMoney), strChip_Name[(int)ROOM_CHIPKIND_GOLD]);
	}
	
	//ä��
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
			//���� �Ǿ��� ��� �����Ϸ� ���� ����
			if (GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
				ResetNotToday(GM().GetMyInfo()->UI.ID);
		}
		strChatMsg.Format("��[%s]�� ȸ�� ���� ����\n �����ѵ� �ʰ��� : %s%s�� ȸ�� �Ǿ����ϴ�.\n", GM().GetMyInfo()->UI.NickName, NMBASE::UTIL::g_MoneyMark(llMoney), strChip_Name[chipKind]);
	}
	else if (chipKind == (int)ROOM_CHIPKIND_GOLD)
	{
		if (m_stRevisionGoldInfo.Set(llMoney, pExpire))
		{
			//���� �Ǿ��� ��� �����Ϸ� ���� ����
			if (GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
				ResetNotToday(GM().GetMyInfo()->UI.ID);
		}

		strChatMsg.Format("��[%s]�� ȸ�� ���� ����\n �����ѵ� �ʰ��� : %s%s�� ȸ�� �Ǿ����ϴ�.\n", GM().GetMyInfo()->UI.NickName, NMBASE::UTIL::g_MoneyMark(llMoney), strChip_Name[chipKind]);
	}
	else if (chipKind == (int)ROOM_CHIPKIND_GOLD_BANK)
	{
		if (m_stRevisionGoldBankInfo.Set(llMoney, pExpire))
		{
			//���� �Ǿ��� ��� �����Ϸ� ���� ����
			if (GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
				ResetNotToday(GM().GetMyInfo()->UI.ID);
		}

		strChatMsg.Format("��[%s]�� ȸ�� ���� ����\n �����ѵ� �ʰ��� : %s%s�� ȸ�� �Ǿ����ϴ�.\n", GM().GetMyInfo()->UI.NickName, NMBASE::UTIL::g_MoneyMark(llMoney), strChip_Name[(int)ROOM_CHIPKIND_GOLD]);
	}

	//ä��
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
		//�̹� �˾�â�� �������� ����
		return;
	}

	if (GM().GetQuickStartInRoom())
	{
		//�ٸ��� �ٷΰ��� ���¸� ����
		GM().SetQuickStartInRoom(false);

		return;
	}

	if (GM().GetPlayDailyGoldEffect() == true)
	{
		GM().SetWaitOwnLimitPopup(true);

		return;
	}

	// 2000�� �Ӵ� or 1500�� ��� �ʰ��� 
	INT64 nMyMoney = GM().GetMyInfo()->UI.GetMoney();
	INT64 nMyGold = GM().GetMyGold();

	if (nMyMoney >= m_i64MaxPremiumMoneyWithOption ||
		nMyGold >= m_i64MaxPremiumGoldWithOption)
	{
		//COwnLimitDlg dlg(GM().GetMainWnd());			
		m_dlgOwnLimit.DoModal(HANDOPOPUP_ACHIEVE_LIMIT_MONEY);//UPGCommon �۾�

		return;
	}
	
	HANDOPOPUP Hando_Popup_Type = HANDOPOPUP_NULL;
	
	//���� VVIP�̰� �ѵ�����ǰ�����̶�� (���������� �ִ� �ִ뺸���ѵ��� ��Ŀ���������� �ִ� �ִ뺸���ѵ��� ���ٸ� )�˾��� ���������ʴ´�.
	if (m_i64MaxPremiumMoneyWithOption != g_MaxServerMoney ||
		m_i64MaxPremiumGoldWithOption != g_MaxServerGold)
	{
		/*
		if (m_stOverMoneyInfo.IsExpired()==FALSE && 
			m_stRevisionMoneyInfo.IsExpired()==FALSE)		//�ѵ�, ���� �Ӵϰ� ���� �߻����� ��
		{		
			Hando_Popup_Type=HANDOPOPUP_COLLECT_BOTH_MONEY;	
		}
		else if (m_stOverMoneyInfo.IsExpired()==FALSE)		//�ѵ� �ʰ� �Ӵϸ� �߻����� ��
		{		
			Hando_Popup_Type=HANDOPOPUP_COLLECT_OVER_MONEY;
		}
		else if (m_stRevisionMoneyInfo.IsExpired()==FALSE)		// ���� �Ӵϸ� �߻����� ��
		{
			Hando_Popup_Type=HANDOPOPUP_COLLECT_REVISION_MONEY;		
		}
		*/

		// 2017.12.13 ���ý��� �߰�
		// �ѵ� �ʰ�, ���� �Ӵ�(����� �������� ���� �߻�)
		// ������� �� ������ �˾��� ����� ��.
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
// 		case SV_USERINFO:				����.. Lobby.cpp���� ó��
// 			return OnReceiveUserInfo(TotSize, lpData);
		case SV_NOTIFY_CHANGEOVERMONEY:	
			return OnReceiveNotifyChangeOverMoney(TotSize, lpData);
	}

	return FALSE;
}


//���� ����â�� ���ӹ�/���� �����ϰ� �����ʿ� �������� ������ �����ʿ��� ����.
BOOL COwnLimit::OnReceiveUserInfo(int TotSize, char *lpData)
{
	CSV_USERINFO msg;
	msg.Get(lpData, TotSize);

	GM().SetWaitResponse(false);

	if ( GM().GetCurrentWhere() != IDX_GAMEWHERE_GAME)
		return FALSE;

	// ���̵� ���̰� 0�̸� ���� ��ܿ� ���� �������
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

	// ���� �ѵ� �ʰ�
	if (strncmp(msg.szID, GM().GetMyInfo()->UI.ID, 15) == 0)
	{
		// �ʰ����� ���� ���
		if (*msg.llOverMoney > 0)
		{
			// �ʰ��� ����
			SetOverMoney(*msg.llOverMoney, *msg.m_ChipKind, msg.stOverMoneyEndDate);
		}

		// ���� �Ӵϰ� ���� ���
		if (*msg.llRevisionMoney > 0)
		{
			// ���� �Ӵ� ����
			SetRevisionMoney(*msg.llRevisionMoney, *msg.m_ChipKind, msg.stRevisionMoneyEndDate);
		}
	}
	// �ٸ� ���� �ѵ� �ʰ�
	else
	{
		SetOtherUserOverMoney(msg.szID, *msg.llOverMoney, *msg.m_ChipKind, msg.stOverMoneyEndDate);
	}

	// �ΰ����� �ƴ� ���
	if (GM().GetCurrentWhere() != IDX_GAMEWHERE_GAME)
	{
		// �˾��� ���
		ShowPopup();
	}

	return TRUE;
}