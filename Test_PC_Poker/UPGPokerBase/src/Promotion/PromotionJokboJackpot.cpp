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

	// 이펙트가 보일때 잭팟을 노출을 해줘야 하기때문에 이벤트를 등록 한다.
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
	case OPCODE_SV_JP_WINNER_ALRAM:// 잭팟 당첨금을 클라이언트에 전송
		{


			packet::TSendPacket< jackpot::JP_WIN > JP_WIN_Packet;
			JP_WIN_Packet.Get( lpData, TotSize );
			jackpot::JP_WIN* pAlaram = static_cast< jackpot::JP_WIN* >( JP_WIN_Packet.Various() );

			// 잭팟이 터진경우.
			m_bJackpotShow = TRUE;			

			m_strNickName		= pAlaram->JData.szNickName;					// 닉네임
			m_strJackpotMoney	= (CString)NMBASE::UTIL::g_MoneyMark(pAlaram->JData.llBonusPMoney);	// 잭팟 금액
			m_nRoomNumber		= pAlaram->JData.sRoomNum;						// 방번호

			// 족보이름
			switch( pAlaram->JData.sJokbo ) // 101:로티플(스페이드) 1022:로티플(다이아) 103:로티플(하트) 104:로티플(클로버) 105:패턴골프
			{
			case jackpot::EJACKPOT_RT_S:
				m_strJokbo="스페이드 로티플";
				break;
			case jackpot::EJACKPOT_RT_D:
				m_strJokbo="다이아 로티플";
				break;
			case jackpot::EJACKPOT_RT_H:
				m_strJokbo="하트 로티플";
				break;
			case jackpot::EJACKPOT_RT_C:
				m_strJokbo="크로바 로티플";
				break;
			case jackpot::EJACKPOT_BSFLUSH:
			case jackpot::EJACKPOT_SFULSH:
				m_strJokbo="스트레이트 플러시";
				break;
			case jackpot::EJACKPOT_QUADS:
				m_strJokbo="포카드";
				break;

			case jackpot::EJACKPOT_BPG:
				m_strJokbo="패턴골프";
				break;
			case jackpot::EJACKPOT_BG:
				m_strJokbo="골프";
				break;
			case jackpot::EJACKPOT_BS:
				m_strJokbo="세컨드";
				break;
			case jackpot::EJACKPOT_HOOLA:
				m_strJokbo="3스톱으";
				break;
			default:
				{
				m_strJokbo="---";
				}break;
			}


			// 게임코드 -> 게임인덱스
			IDX_GAME nGameIndex = g_GetGameIndex( pAlaram->JData.sGameCode );
			CGame *pGame = GM().GetGame(nGameIndex);
			if (pGame)
			{
				m_strGameName = pGame->GetGameName();
			}		

			CString strChatMsg_Game, strChatMsg_Lobby;
			CString strShowMsg;
			strShowMsg.Format("[잭팟] %s %s방 %s님 %s당첨!\n",
								m_strGameName.GetBuffer(),
								NMBASE::UTIL::g_MakeShortMoneyStrEx(pAlaram->JData.llLimitMoney),
								m_strNickName.GetBuffer(),										
								m_strJackpotMoney.GetBuffer());

			if (GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME && GAME_CODE_DP != pAlaram->JData.sGameCode)
			{
				// 게임방 안이면

				if (GM().GetCurrentGameCode() == pAlaram->JData.sGameCode)
				{
					// 같은 게임에서 잭팟이 터졌으면

					if (m_strNickName == GM().GetMyInfo()->UI.NickName)
					{
						// 내가 당첨됐으면
						strChatMsg_Game.Format("[잭팟] %s님, %s, %s방에서 %s로 %s 잭팟 당첨되셨습니다!\n",
												m_strNickName.GetBuffer(),										
												m_strGameName.GetBuffer(),
												NMBASE::UTIL::g_MakeShortMoneyStrEx(pAlaram->JData.llLimitMoney),
												m_strJokbo.GetBuffer(),
												m_strJackpotMoney.GetBuffer());
					}
					else if (g_RI.llEnterLimitMoney <= 0)
					{
						// 자유방이면
						strChatMsg_Game = strShowMsg;
					}
					else if (g_RI.llEnterLimitMoney <= pAlaram->JData.llLimitMoney)
					{
						// 자유방이 아니고 당첨된 방이 내 방의 시드머니 이상이면
						strChatMsg_Game = strShowMsg;
					}
				}
				else if (pAlaram->JData.llLimitMoney >= 1000000000000)
				{
					// 다른 게임에서 잭팟이 터졌고 해당 방이 VIP 경기장이면
					strChatMsg_Game = strShowMsg;
				}

				//채팅 출력 (게임방)
	 			if(!strChatMsg_Game.IsEmpty())
 				{
 					AddGameChatViewEdit(strChatMsg_Game, m_sChatColor);
 				}
			}
			else if (GetCurrentWhere() == IDX_GAMEWHERE_LOBY)
			{
				// 로비이면
				//로비용 메세지
				strChatMsg_Lobby.Format("%s [%s방] %d번 게임방에서 %s님이 %s로 잭팟%s %s 획득했습니다.\n",
					m_strGameName.GetBuffer(),
					NMBASE::UTIL::g_MakeShortMoneyStrEx(pAlaram->JData.llLimitMoney),
					m_nRoomNumber, 
					m_strNickName.GetBuffer(),										
					m_strJokbo.GetBuffer(),
					strChip_Name[(int)ROOM_CHIPKIND_NORMAL],
					m_strJackpotMoney.GetBuffer());

				//채팅출력  (대기실)
				AddLobbyChatViewEdit(strChatMsg_Lobby, RGB(115, 0, 0));
			}

			// 잭팟 금액을 초기화 시킨다.
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
		// 이펙트가 뜬 후 0.81초 후에 잭팟 이팩트가 보여져야 한다.
		m_dwCurTime = timeGetTime();
		m_dwCurTime = m_dwCurTime - m_dwStartTime;

		if(m_dwCurTime >= 810)
		{
			m_bJackpotEffectShow = TRUE;
			m_bJokboEffectStart = FALSE;
		}
	}	


	// 자신의 방에서 잭팟이 터진경우만 보여준다.
	if(m_bJackpotEffectShow && m_bJackpotShow && m_bMyRoomJackPot)
	{
		m_dwCurTime = timeGetTime();
		m_dwCurTime = m_dwCurTime - m_dwStartTime;

		if( m_dwCurTime >= m_dwEndTime)
		{			
			m_bJackpotShow   = FALSE;		// 잭팟이 터졌는지 체크
			m_bMyRoomJackPot = FALSE;		// 자신의 방에서 잭팟 터짐
			m_bJackpotEffectShow = FALSE;
			return;
		}

		pPage->DrawMultiScene(0, 0, &m_sprEffect, 0, m_dwCurTime, FALSE);	


		// 1초후 잭팟 머니를 표시한다.
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
