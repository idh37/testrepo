#include "StdAfx.h"
#include "PromotionJokboJackpot.h"
#include "PromotionManager.h"

PromotionJokboJackpot::PromotionJokboJackpot(CPromotionManager *pManager, int nType)
: CPromotionBase(pManager, nType)
{
	string strDir = ".\\PokerCommon\\data\\Event\\jokbojackpot\\";
	_LOADSPRITEDEFINE(&m_sprEffect, strDir, "jackpot.spr");

	CUIInfoData *pUIInfoData = m_pManager->GetUIInfoData();

	if(pUIInfoData)
	{
		m_rtJackpotMoneyRect = pUIInfoData->m_rtHighJokboJacpotMoney;
		m_rtJackpotMoneyRect.OffsetRect(pUIInfoData->m_ptHighJokboJacpotMoney);		

		if(pUIInfoData->m_listChatColor.size() > ECC2_RESULT_JACKPOT)
		{
			m_sChatColor = pUIInfoData->m_listChatColor[ECC2_RESULT_JACKPOT];
		}
		else
		{
			m_sChatColor = RGB(199, 117, 116);
		}
	}

	// ����Ʈ�� ���϶� ������ ������ ����� �ϱ⶧���� �̺�Ʈ�� ��� �Ѵ�.
	MM().AddMsgH(UPG_CMK_SET_JACKPOTEFFECT_SHOW, Fnt(this, &PromotionJokboJackpot::SetJackpotEffectShow));

	m_bJackpotEffectShow = FALSE;
	m_bJokboEffectStart = FALSE;
	m_bMyRoomJackPot = FALSE;

	m_dwStartTime = 0;
}

PromotionJokboJackpot::~PromotionJokboJackpot(void)
{
	MM().RemoveMsgH(UPG_CMK_SET_JACKPOTEFFECT_SHOW);
}

LRESULT PromotionJokboJackpot::SetJackpotEffectShow(WPARAM &wParam, LPARAM &lParam)
{
	m_bJokboEffectStart = TRUE;
	m_dwStartTime = timeGetTime();

	return TRUE;
};

BOOL PromotionJokboJackpot::OnLButtonDownBottom(int &x , int &y)
{
	return FALSE;
}

BOOL PromotionJokboJackpot::OnLButtonUpBottom(int &x , int &y)
{
	return FALSE;
}

BOOL PromotionJokboJackpot::OnMouseMoveBottom(int &x , int &y)
{
	return FALSE;
}

BOOL PromotionJokboJackpot::OnLButtonDownMiddle(int &x , int &y)
{
	return FALSE;
}

BOOL PromotionJokboJackpot::OnLButtonUpMiddle(int &x , int &y)
{
	return FALSE;
}

BOOL PromotionJokboJackpot::OnMouseMoveMiddle(int &x , int &y)
{
	return FALSE;
}

BOOL PromotionJokboJackpot::OnLButtonDownTop(int &x , int &y)
{
	return FALSE;
}

BOOL PromotionJokboJackpot::OnLButtonUpTop(int &x , int &y)
{
	return FALSE;
}

BOOL PromotionJokboJackpot::OnMouseMoveTop(int &x , int &y)
{
	return FALSE;
}

BOOL PromotionJokboJackpot::OnEventTabLButtonDown(int &x , int &y)
{
	return FALSE;
}

BOOL PromotionJokboJackpot::OnEventTabLButtonUp(int &x , int &y)
{
	return FALSE;
}

BOOL PromotionJokboJackpot::OnEventTabMouseMove(int &x , int &y)
{
	return FALSE;
}

BOOL PromotionJokboJackpot::OnPacketNotify(char* pName, int &Signal, int &TotSize, char *lpData)
{
	switch(Signal)
	{	
	case OPCODE_SV_JP_WINNER_ALRAM:// ���� ��÷���� Ŭ���̾�Ʈ�� ����
		{


			packet::TSendPacket< jackpot::JP_WIN > JP_WIN_Packet;
			JP_WIN_Packet.Get( lpData, TotSize );
			jackpot::JP_WIN* pAlaram = static_cast< jackpot::JP_WIN* >( JP_WIN_Packet.Various() );

			// ������ �������.
			m_bJackpotShow = TRUE;			

			m_strNickName		= pAlaram->JData.szNickName;					// �г���
			m_strJackpotMoney	= (CString)NMBASE::UTIL::g_MoneyMark(pAlaram->JData.llBonusPMoney);	// ���� �ݾ�
			m_nRoomNumber		= pAlaram->JData.sRoomNum;						// ���ȣ

			// �����̸�
			switch( pAlaram->JData.sJokbo ) // 101:��Ƽ��(�����̵�) 1022:��Ƽ��(���̾�) 103:��Ƽ��(��Ʈ) 104:��Ƽ��(Ŭ�ι�) 105:���ϰ���
			{
			case jackpot::EJACKPOT_RT_S:
				m_strJokbo="�����̵� ��Ƽ��";
				break;
			case jackpot::EJACKPOT_RT_D:
				m_strJokbo="���̾� ��Ƽ��";
				break;
			case jackpot::EJACKPOT_RT_H:
				m_strJokbo="��Ʈ ��Ƽ��";
				break;
			case jackpot::EJACKPOT_RT_C:
				m_strJokbo="ũ�ι� ��Ƽ��";
				break;
			case jackpot::EJACKPOT_BSFLUSH:
			case jackpot::EJACKPOT_SFULSH:
				m_strJokbo="��Ʈ����Ʈ �÷���";
				break;
			case jackpot::EJACKPOT_QUADS:
				m_strJokbo="��ī��";
				break;

			case jackpot::EJACKPOT_BPG:
				m_strJokbo="���ϰ���";
				break;
			case jackpot::EJACKPOT_BG:
				m_strJokbo="����";
				break;
			case jackpot::EJACKPOT_BS:
				m_strJokbo="������";
				break;
			case jackpot::EJACKPOT_HOOLA:
				m_strJokbo="3������";
				break;
			default:
				{
				m_strJokbo="---";
				}break;
			}


			// �����ڵ� -> �����ε���
			IDX_GAME nGameIndex = g_GetGameIndex( pAlaram->JData.sGameCode );
			CGame *pGame = GM().GetGame(nGameIndex);
			if (pGame)
			{
				m_strGameName = pGame->GetGameName();
			}		

			CString strChatMsg_Game, strChatMsg_Lobby;
			CString strShowMsg;
			strShowMsg.Format("[����] %s %s�� %s�� %s��÷!\n",
								m_strGameName.GetBuffer(),
								NMBASE::UTIL::g_MakeShortMoneyStrEx(pAlaram->JData.llLimitMoney),
								m_strNickName.GetBuffer(),										
								m_strJackpotMoney.GetBuffer());

			if (GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME && GAME_CODE_DP != pAlaram->JData.sGameCode)
			{
				// ���ӹ� ���̸�

				if (GM().GetCurrentGameCode() == pAlaram->JData.sGameCode)
				{
					// ���� ���ӿ��� ������ ��������

					if (m_strNickName == GM().GetMyInfo()->UI.NickName)
					{
						// ���� ��÷������
						strChatMsg_Game.Format("[����] %s��, %s, %s�濡�� %s�� %s ���� ��÷�Ǽ̽��ϴ�!\n",
												m_strNickName.GetBuffer(),										
												m_strGameName.GetBuffer(),
												NMBASE::UTIL::g_MakeShortMoneyStrEx(pAlaram->JData.llLimitMoney),
												m_strJokbo.GetBuffer(),
												m_strJackpotMoney.GetBuffer());
					}
					else if (g_RI.llEnterLimitMoney <= 0)
					{
						// �������̸�
						strChatMsg_Game = strShowMsg;
					}
					else if (g_RI.llEnterLimitMoney <= pAlaram->JData.llLimitMoney)
					{
						// �������� �ƴϰ� ��÷�� ���� �� ���� �õ�Ӵ� �̻��̸�
						strChatMsg_Game = strShowMsg;
					}
				}
				else if (pAlaram->JData.llLimitMoney >= 1000000000000)
				{
					// �ٸ� ���ӿ��� ������ ������ �ش� ���� VIP ������̸�
					strChatMsg_Game = strShowMsg;
				}

				//ä�� ��� (���ӹ�)
	 			if(!strChatMsg_Game.IsEmpty())
 				{
 					AddGameChatViewEdit(strChatMsg_Game, m_sChatColor);
 				}
			}
			else if (GetCurrentWhere() == IDX_GAMEWHERE_LOBY)
			{
				// �κ��̸�
				//�κ�� �޼���
				strChatMsg_Lobby.Format("%s [%s��] %d�� ���ӹ濡�� %s���� %s�� ����%s %s ȹ���߽��ϴ�.\n",
					m_strGameName.GetBuffer(),
					NMBASE::UTIL::g_MakeShortMoneyStrEx(pAlaram->JData.llLimitMoney),
					m_nRoomNumber, 
					m_strNickName.GetBuffer(),										
					m_strJokbo.GetBuffer(),
					strChip_Name[(int)ROOM_CHIPKIND_NORMAL],
					m_strJackpotMoney.GetBuffer());

				//ä�����  (����)
				AddLobbyChatViewEdit(strChatMsg_Lobby, RGB(115, 0, 0));
			}

			// ���� �ݾ��� �ʱ�ȭ ��Ų��.
			MM().Call(UPG_CMK_CLEAR_JACKPOTMONEY, (WPARAM)(&pAlaram->JData.sGameCode), (LPARAM)(&pAlaram->JData.llLimitMoney));

			m_dwStartTime = timeGetTime();
			m_dwEndTime = m_sprEffect.GetMScenePlayTime(0);

			return FALSE;
		}

		return TRUE;
	}

	return FALSE;
}


BOOL PromotionJokboJackpot::OnCommand(WPARAM &wParam, LPARAM &lParam)
{
	return FALSE;
}

void PromotionJokboJackpot::DrawBottom(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage)
{
}

void PromotionJokboJackpot::DrawMiddle(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage)
{
}

void PromotionJokboJackpot::DrawTop(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage)
{
	if(m_bJokboEffectStart)
	{
		// ����Ʈ�� �� �� 0.81�� �Ŀ� ���� ����Ʈ�� �������� �Ѵ�.
		m_dwCurTime = timeGetTime();
		m_dwCurTime = m_dwCurTime - m_dwStartTime;

		if(m_dwCurTime >= 810)
		{
			m_bJackpotEffectShow = TRUE;
			m_bJokboEffectStart = FALSE;
		}
	}	


	// �ڽ��� �濡�� ������ ������츸 �����ش�.
	if(m_bJackpotEffectShow && m_bJackpotShow && m_bMyRoomJackPot)
	{
		m_dwCurTime = timeGetTime();
		m_dwCurTime = m_dwCurTime - m_dwStartTime;

		if( m_dwCurTime >= m_dwEndTime)
		{			
			m_bJackpotShow   = FALSE;		// ������ �������� üũ
			m_bMyRoomJackPot = FALSE;		// �ڽ��� �濡�� ���� ����
			m_bJackpotEffectShow = FALSE;
			return;
		}

		pPage->DrawMultiScene(0, 0, &m_sprEffect, 0, m_dwCurTime, FALSE);	


		// 1���� ���� �Ӵϸ� ǥ���Ѵ�.
		if( m_dwCurTime >= 800 && m_dwCurTime < 2700)
		{
			CUIInfoData *pUIInfoData = m_pManager->GetUIInfoData();

			if(pUIInfoData)
			{			
				pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_12D_BOLD));
				pDC->SetTextColor(pUIInfoData->m_clrHighJokboJacpotMoney);
				pDC->DrawText(m_strJackpotMoney, &m_rtJackpotMoneyRect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
			}
		}
	}		
}

void PromotionJokboJackpot::DrawEventTab(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage, const CPoint &ptStart)
{
}

const bool PromotionJokboJackpot::IsShowTab(void)
{
	return false;
}
