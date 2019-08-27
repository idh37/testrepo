#include "StdAfx.h"
#include "PromotionSpecialJackpot.h"
#include "PromotionManager.h"


CPromotionSpecialJackpot::CPromotionSpecialJackpot(CPromotionManager *pManager, int nType):CPromotionBase(pManager, nType)
{
	//if (GM().GetCurrentGameType() != IDX_GAME_SP)
	//{
	//	return;
	//}
	//
	//m_nType = nType;

	m_bIsJackpotRoom = false ;
	m_bIsHalfCountStart = false ;
	m_bIsShowStartEffect = false ;
	m_bIsShowEndEffect = false ;
	m_bIsShowEndText = false ;
	m_bIsShowJackpotmoney = false ;
	m_bIsShowRewardJPMoneyEffect = false ;

	m_bIsShowJPRoomEndEffect = false ;

	m_bisShowWinner = false ;

	m_nJPTimeState = State_NoJPTime ;

	m_nPlayTime = 0 ;
	m_nBackSpriteCount = 0 ;

	m_nBestHalfCount = 0;
	m_nMyhalfCount = 0;

	m_nJPRoomNotiCount = 0 ;

	m_llhalfJackpotMoney = 0 ;
	m_nChannel = 0 ;

	m_dwStartEndTextTime = 0 ;
	m_dwNowEndTextTime = 0 ;

	m_nMyChannelRoomNum = 0 ;
	m_nBetRuleHalfCountCheck = 0 ;

	m_nRewardJPMoneyCount = 0 ;
	m_nWinDataCount = 0 ;

	for(int i = 0 ; i < 19 ; i++)
		m_nJackpotMoney[i] = 0  ;

	for(int i = 0 ; i < 7 ; i++ )
	{
		for(int j = 0 ; j < MAX_TOP_HALF_ROOM; j++ )
		{
			m_nRoomNum[i][j] = -1;
		}
	}

	for(int i = 0 ; i < 46 ; i++ )
	{
		m_stWinnerData[i].nChannel = -1 ;
		m_stWinnerData[i].nRoomNum = -1 ;
		sprintf(m_stWinnerData[i].szMyChannelID, "") ;
	}

	string strDir = ".\\PokerCommon\\data\\Event\\SpecialJacpot\\";
	_LOADSPRITEDEFINE(&m_sprJackpotBack, strDir, "jp_back.spr");
	_LOADSPRITEDEFINE(&m_sprJackpotInform, strDir, "jp_inform.spr");
	_LOADSPRITEDEFINE(&m_sprJackpotNum, strDir, "number.spr");

	m_cBackEffect.SetSprite(&m_sprJackpotInform);
	m_cAlertEffect.SetSprite(&m_sprJackpotInform);

	memset( &m_Jackpot_Info, 0, sizeof( promotion::_HALFJACKPOT_ALARM  ) );
}

//소멸자
CPromotionSpecialJackpot::~CPromotionSpecialJackpot(void)
{

}

void CPromotionSpecialJackpot::Reset() 
{
	m_bIsJackpotRoom = false ;
	m_bIsHalfCountStart = false ;
	m_bIsShowStartEffect = false ;
	m_bIsShowEndEffect = false ;
	m_bIsShowEndText = false ;
	m_bIsShowJackpotmoney = false ;
	m_bIsShowRewardJPMoneyEffect = false ;

	m_bIsShowJPRoomEndEffect = false ;

	m_bisShowWinner = false ;

	m_nJPTimeState = State_NoJPTime ;

	m_nPlayTime = 0 ;
	m_nBackSpriteCount = 0 ;

	m_dwStartEndTextTime = 0 ;
	m_dwNowEndTextTime = 0 ;

	m_nBestHalfCount = 0;
	m_nMyhalfCount = 0;

	m_nJPRoomNotiCount = 0 ;

	m_nRewardJPMoneyCount = 0 ;

	m_nMyChannelRoomNum = 0 ;

	m_llhalfJackpotMoney = 0 ;
	m_nChannel = 0 ;

	m_nWinDataCount = 0 ;
	m_nBetRuleHalfCountCheck = 0 ;

	for(int i = 0 ; i < 19 ; i++)
		m_nJackpotMoney[i] = 0  ;

	for(int i = 0 ; i < 7 ; i++ )
	{
		for(int j = 0 ; j < MAX_TOP_HALF_ROOM; j++ )
		{
			m_nRoomNum[i][j] = -1;
		}
	}

	for(int i = 0 ; i < 46 ; i++ )
	{
		m_stWinnerData[i].nChannel = -1 ;
		m_stWinnerData[i].nRoomNum = -1 ;
		sprintf(m_stWinnerData[i].szMyChannelID, "") ;
	}
}

void CPromotionSpecialJackpot::Accept_OutRoom()
{
	m_bIsHalfCountStart = false ;
	m_bIsJackpotRoom = false ;
}

void CPromotionSpecialJackpot::Accept_CreateRoom()
{
	if(100000000 == g_RI.llEnterLimitMoney)
	{
		m_nChannel = 0 ;

	}else if(1000000000 == g_RI.llEnterLimitMoney)
	{
		m_nChannel = 1 ;

	}else if(10000000000 == g_RI.llEnterLimitMoney)
	{
		m_nChannel = 2 ;

	}else if(100000000000 == g_RI.llEnterLimitMoney)
	{
		m_nChannel = 3 ;

	}else if(1000000000000 == g_RI.llEnterLimitMoney)
	{
		m_nChannel = 4 ;

	}else if(3000000000000 == g_RI.llEnterLimitMoney)
	{
		m_nChannel = 5 ;

	}else if(10000000000000 <= g_RI.llEnterLimitMoney)
	{
		m_nChannel = 6 ;

	}else
	{
		m_nChannel = -1 ;
	}
}

void CPromotionSpecialJackpot::Accept_EnterRoom()
{
	if(100000000 == g_RI.llEnterLimitMoney)
	{
		m_nChannel = 0 ;

	}else if(1000000000 == g_RI.llEnterLimitMoney)
	{
		m_nChannel = 1 ;

	}else if(10000000000 == g_RI.llEnterLimitMoney)
	{
		m_nChannel = 2 ;

	}else if(100000000000 == g_RI.llEnterLimitMoney)
	{
		m_nChannel = 3 ;

	}else if(1000000000000 == g_RI.llEnterLimitMoney)
	{
		m_nChannel = 4 ;

	}else if(3000000000000 == g_RI.llEnterLimitMoney)
	{
		m_nChannel = 5 ;

	}else if(10000000000000 <= g_RI.llEnterLimitMoney)
	{
		m_nChannel = 6 ;

	}else
	{
		m_nChannel = -1 ;
	}
}

//미션창을 그린다.
void CPromotionSpecialJackpot::DrawTop(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage)
{
	// 로우바둑이와 뉴포커가 아니면 리턴.
	if ((GM().GetCurrentGameType() != IDX_GAME_BD && GM().GetCurrentGameType() != IDX_GAME_NP) || 0 == g_RI.llEnterLimitMoney )
	{
		return ;
	}

	COLORREF clrStrTime = RGB(133, 115, 63);
	COLORREF clrStrBesthalf = RGB(45, 82, 120);

	//텍스트
	CString TimeStr = TEXT("");

	switch(m_nJPTimeState)
	{
	case State_NoJPTime:
		break ;
	case State_JPTimeReady_Start:
		{
			pPage->DrawScene( 0 , 0, &m_sprJackpotBack, m_nBackSpriteCount, 0, FALSE);

			// 준비 시간은 모두 끝났지만, 이전판이 종료 되지 않았을 때/
			if(m_nPreTime >= 0)
			{
				TimeStr.Format(TEXT("(%d 분 후)"), m_nPreTime) ;
		

				pDC->SetTextColor(clrStrTime);
				pDC->TextOut( 252, 23, TimeStr);
			}
		}
		break ;
	case State_JPTimeStart_notCount:
		{
			pPage->DrawScene( 0 , 0, &m_sprJackpotBack, 17, 0, FALSE);
		}
		break ;
	case State_JPTimeStart_Count:
		{
			pPage->DrawScene( 0 , 0, &m_sprJackpotBack, m_nBackSpriteCount, 0, FALSE);

			int min, sec ;
			min = m_nPlayTime / 60;
			sec = m_nPlayTime % 60;
			pDC->SetTextColor(clrStrTime);
			TimeStr.Format(TEXT("%d"), min) ;
			if(min < 10)
				pDC->TextOut( 243, 44, TimeStr);
			else
				pDC->TextOut( 236, 44, TimeStr);

			TimeStr.Format(TEXT("%02d"), sec) ;		
			pDC->TextOut( 280, 44, TimeStr);

			pDC->SetTextColor(clrStrBesthalf);
			
			CSize sizeNumber = 0;
			TimeStr.Format(TEXT("%d"), m_nBestHalfCount) ;
			::GetTextExtentPoint(pDC->m_hDC, TimeStr, TimeStr.GetLength(), &sizeNumber);
			pDC->TextOut( 288 - sizeNumber.cx, 63, TimeStr);

			//if(m_nMyhalfCount > 10)
			//{
			//	pPage->PutSprAuto(110, 48, &m_sprJackpotNum, 11 + m_nMyhalfCount/10) ;
			//	pPage->PutSprAuto(127, 48, &m_sprJackpotNum, 11 + m_nMyhalfCount%10) ;

			//}else
			//{
			//	pPage->PutSprAuto(127, 48, &m_sprJackpotNum, 11 + m_nMyhalfCount%10) ;
			//}

			CFont *pOldFont = pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11_NORMAL));
			COLORREF clrStrOld = pDC->GetTextColor();
		
			pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_14D_NORMAL));

			pDC->SetTextColor(clrStrTime);
			TimeStr.Format(TEXT("%d"), m_nMyhalfCount) ;
			
			if(m_nMyhalfCount < 10 )
				pDC->TextOut( 137, 52, TimeStr);
			else if(m_nMyhalfCount < 100 )
				pDC->TextOut( 125, 52, TimeStr);
			else if(m_nMyhalfCount < 1000 )
				pDC->TextOut( 113, 52, TimeStr);

			//성능을 위한 최적화
			pDC->SetTextColor(clrStrOld);
			pDC->SelectObject(pOldFont);

			// 잭팟 머니를 보여주는 상태라면...
			if(true == m_bIsShowJackpotmoney)
			{
				m_llhalfJackpotMoney = ChangejackpotMoney(m_llhalfJackpotMoney) ;

				char str[19] = {0,};
				sprintf(str, "%I64d", m_llhalfJackpotMoney) ;

				int nCount = 0 ;
				for(int i = 0; i<19; i++)
				{
					if(str[i] == NULL)
					{
						nCount = i ;
						break ;
					}
				}

				int nCommaCount = 1;
				for(int i = 19; i > 0; i--)
				{
					if(0 == nCommaCount%4 && nCount != 0)
					{
						pPage->PutSprAuto(120+i*10, 22, &m_sprJackpotNum, 10) ;

					}else if(nCount-1 >= 0)
					{
						pPage->PutSprAuto(120+i*10, 22, &m_sprJackpotNum, str[nCount-1] - 48) ;
						nCount-- ;

					}else if(nCount-1 < 0)
					{
						break ;
					}
					nCommaCount++ ;
				}
			}
		}
		break ;
	case State_JPTimeEnd_Count:
		{
			pPage->DrawScene( 0 , 0, &m_sprJackpotBack, 6, 0, FALSE);

			int min, sec ;
			min = m_nPlayTime / 60;
			sec = m_nPlayTime % 60;

			if(0 == min)
			{
				if( 0 >= sec )
					sec = 0 ;
			}

			pDC->SetTextColor(clrStrTime);
			TimeStr.Format(TEXT("%d"), min) ;
			if(min < 10)
				pDC->TextOut( 243, 44, TimeStr);
			else
				pDC->TextOut( 236, 44, TimeStr);

			TimeStr.Format(TEXT("%02d"), sec) ;		
			pDC->TextOut( 280, 44, TimeStr);

			pDC->SetTextColor(clrStrBesthalf);

			CSize sizeNumber = 0;
			TimeStr.Format(TEXT("%d"), m_nBestHalfCount) ;
			::GetTextExtentPoint(pDC->m_hDC, TimeStr, TimeStr.GetLength(), &sizeNumber);
			pDC->TextOut( 288 - sizeNumber.cx, 63, TimeStr);

			//if(m_nMyhalfCount > 10)
			//{
			//	pPage->PutSprAuto(110, 48, &m_sprJackpotNum, 11 + m_nMyhalfCount/10) ;
			//	pPage->PutSprAuto(127, 48, &m_sprJackpotNum, 11 + m_nMyhalfCount%10) ;

			//}else
			//{
			//	pPage->PutSprAuto(127, 48, &m_sprJackpotNum, 11 + m_nMyhalfCount%10) ;
			//}

			CFont *pOldFont = pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11_NORMAL));
			COLORREF clrStrOld = pDC->GetTextColor();

			pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_14D_NORMAL));

			pDC->SetTextColor(clrStrTime);
			TimeStr.Format(TEXT("%d"), m_nMyhalfCount) ;

			if(m_nMyhalfCount < 10 )
				pDC->TextOut( 137, 52, TimeStr);
			else if(m_nMyhalfCount < 100 )
				pDC->TextOut( 125, 52, TimeStr);
			else if(m_nMyhalfCount < 1000 )
				pDC->TextOut( 113, 52, TimeStr);

			//성능을 위한 최적화
			pDC->SetTextColor(clrStrOld);
			pDC->SelectObject(pOldFont);
		}
		break ;
	case State_JPTimeEnd_notCount:
		{
			pPage->DrawScene( 0 , 0, &m_sprJackpotBack, 18, 0, FALSE);
		}
		break ;
	case State_JPRoomChoice:
		{
			//pPage->DrawScene( 0 , 0, &m_sprJackpotBack, 18, 0, FALSE);

			if(true == m_bIsJackpotRoom )
				pPage->DrawScene( 0 , 0, &m_sprJackpotBack, 20, 0, FALSE);
			else if(false == m_bIsJackpotRoom)
				pPage->DrawScene( 0 , 0, &m_sprJackpotBack, 21, 0, FALSE);
		}
		break ;
	case State_JPRoomChoiceEnd:
		{
			if(true == m_bIsJackpotRoom )
				pPage->DrawScene( 0 , 0, &m_sprJackpotBack, 20, 0, FALSE);
			else if(false == m_bIsJackpotRoom)
				pPage->DrawScene( 0 , 0, &m_sprJackpotBack, 21, 0, FALSE);
		}
		break ;
	case State_JPRoom_Start:
		{
			if(true == m_bIsJackpotRoom )
			{
				pPage->DrawScene( 0 , 0, &m_sprJackpotBack, 23, 0, FALSE);

				char str[19] = {0,};
				sprintf(str, "%I64d", m_llhalfJackpotMoney) ;

				int nCount = 0 ;
				for(int i = 0; i<19; i++)
				{
					if(str[i] == NULL)
					{
						nCount = i ;
						break ;
					}
				}

				int nCommaCount = 1;
				for(int i = 19; i > 0; i--)
				{
					if(0 == nCommaCount%4 && nCount != 0)
					{
						pPage->PutSprAuto(120+i*10, 22, &m_sprJackpotNum, 10) ;

					}else if(nCount-1 >= 0)
					{
						pPage->PutSprAuto(120+i*10, 22, &m_sprJackpotNum, str[nCount-1] - 48) ;
						nCount-- ;

					}else if(nCount-1 < 0)
					{
						break ;
					}
					nCommaCount++ ;
				}
			}else if(false == m_bIsJackpotRoom)
			{
				pPage->DrawScene( 0 , 0, &m_sprJackpotBack, 24, 0, FALSE);
				
				CSize sizeNumber = 0;
				TimeStr.Format(TEXT("%s - %d번방"), m_szMyChannel, m_nMyChannelRoomNum) ;
				::GetTextExtentPoint(pDC->m_hDC, TimeStr, TimeStr.GetLength(), &sizeNumber);

				pDC->SetTextColor(clrStrTime);
				pDC->TextOut( 130 - sizeNumber.cx, 22, TimeStr);
			}
		}
		break ;
	case State_JPRoom_End:
		{
			if(true == m_bIsJackpotRoom )
			{
				pPage->DrawScene( 0 , 0, &m_sprJackpotBack, 13, 0, FALSE);

				CSize sizeNumber = 0;
				TimeStr.Format(TEXT("%s"), m_szMyChannelID) ;
				::GetTextExtentPoint(pDC->m_hDC, TimeStr, TimeStr.GetLength(), &sizeNumber);

				pDC->SetTextColor(clrStrTime);
				pDC->TextOut( 151 - sizeNumber.cx, 23, TimeStr);

				pDC->TextOut( 125 - sizeNumber.cx, 61, TimeStr);
			
			}else if(false == m_bIsJackpotRoom )
			{
				pPage->DrawScene( 0 , 0, &m_sprJackpotBack, 25, 0, FALSE);

				CSize sizeNumber = 0;
				TimeStr.Format(TEXT("%s"), m_szMyChannelID) ;
				::GetTextExtentPoint(pDC->m_hDC, TimeStr, TimeStr.GetLength(), &sizeNumber);

				pDC->SetTextColor(clrStrTime);
				pDC->TextOut( 144 - sizeNumber.cx, 23, TimeStr);

				pDC->TextOut( 125 - sizeNumber.cx, 54, TimeStr);
			}
		}
		break ;

	case State_JPRoomNoMyChannel:
		{
			pPage->DrawScene( 0 , 0, &m_sprJackpotBack, 21, 0, FALSE);
		}
		break ;
	}

	m_cAlertEffect.SetPos( 0, 0 );

	//이펙트 그리기 활성화
	if (m_cAlertEffect.IsShow() == TRUE)
	{
		m_cAlertEffect.Draw(pDC, pPage);

		m_dwNowEndTextTime = timeGetTime();
		DWORD dwElapsed = m_dwNowEndTextTime - m_dwStartEndTextTime;

		if(dwElapsed >= 3750)
		{
			m_bIsShowEndText = false ;
		}

		if(	true == m_bIsShowEndText )
		{
			CSize sizeNumber = 0;
			TimeStr.Format(TEXT("%s"), m_szMyChannelID) ;
			::GetTextExtentPoint(pDC->m_hDC, TimeStr, TimeStr.GetLength(), &sizeNumber);

			pDC->SetTextColor(clrStrTime);
			pDC->TextOut( 883 - sizeNumber.cx, 671, TimeStr);

			char str[19] = {0,};
			sprintf(str, "%I64d", m_llhalfJackpotMoney) ;

			int nCount = 0 ;
			for(int i = 0; i<19; i++)
			{
				if(str[i] == NULL)
				{
					nCount = i ;
					break ;
				}
			}

			int nCommaCount = 1;
			for(int i = 19; i > 0; i--)
			{
				if(0 == nCommaCount%4 && nCount != 0)
				{
					pPage->PutSprAuto(728+i*10, 691, &m_sprJackpotNum, 10) ;

				}else if(nCount-1 >= 0)
				{
					pPage->PutSprAuto(728+i*10, 691, &m_sprJackpotNum, str[nCount-1] - 48) ;
					nCount-- ;

				}else if(nCount-1 < 0)
				{
					break ;
				}
				nCommaCount++ ;
			}
		}
	}

	m_cBackEffect.SetPos( 0, 0 );

	//이펙트 그리기 활성화
	if (m_cBackEffect.IsShow() == TRUE)
	{
		m_cBackEffect.Draw(pDC, pPage);

		CPoint Cp[5] = {CPoint(622,622),CPoint(305,405), CPoint(305,210), CPoint(833,210), CPoint(833,405)}  ;

		CSize sizeNumber = 0;
		TimeStr.Format(TEXT("%s"), m_szJackpotMoney) ;
		::GetTextExtentPoint(pDC->m_hDC, TimeStr, TimeStr.GetLength(), &sizeNumber);

		pDC->SetTextColor(clrStrTime);
		pDC->TextOut( Cp[m_nWinnerNum].x - sizeNumber.cx, Cp[m_nWinnerNum].y, TimeStr);
	}
}

////이펙트 함수
void CPromotionSpecialJackpot::ShowJackpotTimeStartEffect()
{
	m_cAlertEffect.ShowEffect(4, TRUE);
}

void CPromotionSpecialJackpot::ShowJackpotTimeEndEffect()
{
	m_cAlertEffect.ShowEffect(6, TRUE);
}

void CPromotionSpecialJackpot::ShowJackpotGameStartEffect()
{
	m_cAlertEffect.ShowEffect(7, TRUE);
}

void CPromotionSpecialJackpot::ShowJackpotGameEndEffect()
{
	m_cAlertEffect.ShowEffect(9, TRUE);
}

void CPromotionSpecialJackpot::ShowRewardJPMoneyEffect()
{
	m_cBackEffect.ShowEffect(11 + m_nWinnerNum, TRUE);
}

void CPromotionSpecialJackpot::HideRewardJPMoneyEffect()
{
	m_cBackEffect.ResetEffect() ;
}

BOOL CPromotionSpecialJackpot::OnPacketNotify(char* pName, int &Signal, int &TotSize, char *lpData)
{
	if (GM().GetCurrentGameType() != IDX_GAME_BD && GM().GetCurrentGameType() != IDX_GAME_NP )
	{
		return FALSE;
	}

	switch(Signal)
	{
		// 시작 정보
	case OPCODE_SV_HALFJACKPOT_START:
		{
			//packet::TSendPacket< promotion::ADULTCARD_ALARM > AdultCard_Packet;
			//AdultCard_Packet.Get( lpData, TotSize );
			//promotion::ADULTCARD_ALARM* pAlarm = static_cast< promotion::ADULTCARD_ALARM* >( AdultCard_Packet.Various() );
			//SetEventInfo( pAlarm );
		}break;

		//06시 초기화
	case OPCODE_SV_HALFJACKPOT_INITIALIZE:
		{
			//packet::TSendPacket< promotion::ADULTCARD_ALARM > AdultCard_Packet;
			//AdultCard_Packet.Get( lpData, TotSize );
			//promotion::ADULTCARD_ALARM* pAlarm = static_cast< promotion::ADULTCARD_ALARM* >( AdultCard_Packet.Various() );
			//SetEventInfo( pAlarm );

			////초기화
			Reset() ;

		}break;

		// 잭팟 타임 시작전
	case OPCODE_SV_HALFJACKPOT_TIME_BEFORE_START:
		{	
			packet::TSendPacket< promotion::HALFJACKPOT_TIME > JackpotTime_Packet;
			JackpotTime_Packet.Get( lpData, TotSize );
			promotion::HALFJACKPOT_TIME* pTime = static_cast< promotion::HALFJACKPOT_TIME* >( JackpotTime_Packet.Various() );
	
			m_nJPTimeState = State_JPTimeReady_Start ;

			m_nPlayTime = pTime->nRemainTimeSec ;
			m_nPreTime = (m_nPlayTime/60)+1 ;

			if(m_nPreTime > 10)
				m_nPreTime = 10 ;

			m_nBackSpriteCount = 14 ;
			
		}break;

		// 잭팟 타임 시작
	case OPCODE_SV_HALFJACKPOT_TIME_START:
		{	
			packet::TSendPacket< promotion::HALFJACKPOT_TIME > JackpotTime_Packet;
			JackpotTime_Packet.Get( lpData, TotSize );
			promotion::HALFJACKPOT_TIME* pTime = static_cast< promotion::HALFJACKPOT_TIME* >( JackpotTime_Packet.Various() );

			m_nPlayTime = pTime->nRemainTimeSec ;
			m_nPreTime = m_nPlayTime/60 ;
		
			// 잭팟 타임 시작 시 게임방 내의 유저만 다음판부터 시작 이펙트를 노출해준다. 
			if(IDX_GAMEWHERE_GAME == GM().GetCurrentWhere())
			{
				if(g_RI.NowUserNum == 1)
				{
					m_nJPTimeState = State_JPTimeStart_Count ;

					ShowJackpotTimeStartEffect() ;

				}else
				{
					m_bIsShowStartEffect = true ;

					m_nJPTimeState = State_JPTimeStart_notCount ;
				}
			}else
			{
				m_nJPTimeState = State_JPTimeStart_Count ;
			}
		}break;

		// 잭팟 타임 종료전
	case OPCODE_SV_HALFJACKPOT_TIME_BEFORE_END:
		{	
			
		}break;

		// 잭팟 타임 종료
	case OPCODE_SV_HALFJACKPOT_TIME_END:
		{	
			// 잭팟 타임 시작 시 게임방 내의 유저만 다음판부터 시작 이펙트를 노출해준다. 
			if(IDX_GAMEWHERE_GAME == GM().GetCurrentWhere())
			{
				if(g_RI.NowUserNum == 1)
				{
					m_nJPTimeState = State_JPTimeEnd_notCount ;

					ShowJackpotTimeEndEffect() ;
				
				}else
				{
					m_bIsShowEndEffect = true ;
					m_nJPTimeState = State_JPTimeEnd_Count ;
				}
			
			}else
			{
				//m_nJPTimeState = State_JPTimeEnd_notCount ;
				Reset() ;
			}
		}break;

		// 누적 잭팟 머니 알림
	case OPCODE_SV_HALFJACKPOT_MONEY:
		{
			packet::TSendPacket< promotion::HALFJACKPOT_MONEY > JackpotTime_Packet;
			JackpotTime_Packet.Get( lpData, TotSize );
			promotion::HALFJACKPOT_MONEY* pMoney = static_cast< promotion::HALFJACKPOT_MONEY* >( JackpotTime_Packet.Various() );
			
			setJackpotMoney(pMoney) ;
		}break;

		// 잭팟 타임 동안 알림 // 방입장시, 판 종료 시
	case OPCODE_SV_HALFJACKPOT_ALARM:
		{
			packet::TSendPacket< promotion::_HALFJACKPOT_ALARM > JackpotTime_Packet;
			JackpotTime_Packet.Get( lpData, TotSize );
			promotion::_HALFJACKPOT_ALARM* pAlarm = static_cast< promotion::_HALFJACKPOT_ALARM* >( JackpotTime_Packet.Various() );
			
			SetEventInfo( pAlarm );

			setJackpotMoneyInRoom() ;

		}break;	

		// 잭팟방 선정
	case OPCODE_SV_HALFJACKPOT_ROOM:
		{
			packet::TSendPacket< promotion::_HALFJACKPOT_ROOM > JackpotTime_Packet;
			JackpotTime_Packet.Get( lpData, TotSize );
			promotion::_HALFJACKPOT_ROOM* pRoom = static_cast< promotion::_HALFJACKPOT_ROOM* >( JackpotTime_Packet.Various() );

			SetJackpotRoom(pRoom) ;

		}break;

		// 잭팟판 종료&당첨자 정보
	case OPCODE_SV_HALFJACKPOT_PRIZE:
		{
			packet::TSendPacket< promotion::HALFJACKPOT_PRIZE > JackpotTime_Packet;
			JackpotTime_Packet.Get( lpData, TotSize );
			promotion::HALFJACKPOT_PRIZE* pPrize = static_cast< promotion::HALFJACKPOT_PRIZE* >( JackpotTime_Packet.Various() );

			if(m_nWinDataCount < 46)
			{
				m_stWinnerData[m_nWinDataCount].nChannel = pPrize->iChannel ;
				m_stWinnerData[m_nWinDataCount].nRoomNum = pPrize->iRoomNum ;

				memset(m_stWinnerData[m_nWinDataCount].szMyChannelID, NULL, sizeof(m_stWinnerData[m_nWinDataCount].szMyChannelID)) ;

				if(NULL == pPrize->szNick[0])
					sprintf(m_stWinnerData[m_nWinDataCount].szMyChannelID, "%s", pPrize->szID);
				else 
					sprintf(m_stWinnerData[m_nWinDataCount].szMyChannelID, "%s", pPrize->szNick);

				m_nWinDataCount++ ;
			}

			if(false == m_bisShowWinner)
			{
				if(IDX_GAMEWHERE_GAME == GM().GetCurrentWhere())
				{
					if(true == m_bIsJackpotRoom)
					{
						if(	pPrize->iRoomNum == g_RI.RoomNum )
						{
							memset(m_szMyChannelID, NULL, sizeof(m_szMyChannelID)) ;

							if(NULL == pPrize->szNick[0])
								sprintf(m_szMyChannelID, "%s", pPrize->szID);
							else 
								sprintf(m_szMyChannelID, "%s", pPrize->szNick);

							convertIDtoProtect(m_szMyChannelID) ;
							convertJPMoneytoStr(pPrize->llPrizeMoney);

							m_nJPTimeState = State_JPRoom_End ;

							m_nPlayTime = 600 ;
							m_nPreTime = m_nPlayTime/60 ;

							//m_bIsShowJPRoomEndEffect = true ;

							ShowJackpotGameEndEffect() ;
							m_llhalfJackpotMoney = pPrize->llPrizeMoney ;
							m_bIsShowEndText = true ;

							m_dwStartEndTextTime = timeGetTime();

							// 하프 잭팟 머니 지급 이펙트 노출.
							m_bIsShowRewardJPMoneyEffect = true ;
							m_nRewardJPMoneyCount = 5 ;
							ShowRewardJPMoneyEffect();

							m_bIsShowJPRoomEndEffect = false ;
							m_bisShowWinner = true ;
						}
					}else
					{
						if(	pPrize->iChannel == m_nChannel )
						{
							if(0 < pPrize->llPrizeMoney )
							{
								memset(m_szMyChannelID, NULL, sizeof(m_szMyChannelID)) ;

								if(NULL == pPrize->szNick[0])
									sprintf(m_szMyChannelID, "%s", pPrize->szID);
								else 
									sprintf(m_szMyChannelID, "%s", pPrize->szNick);

								convertIDtoProtect(m_szMyChannelID) ;
								convertJPMoneytoStr(pPrize->llPrizeMoney);

								m_nJPTimeState = State_JPRoom_End ;

								m_nPlayTime = 600 ;
								m_nPreTime = m_nPlayTime/60 ;

								m_bisShowWinner = true ;

							}else if(0 >= pPrize->llPrizeMoney)
							{
								m_nJPTimeState = State_JPRoomNoMyChannel ;

								m_nPlayTime = 180 ;
							}
						}
					}
				}else
				{
					//memset(m_szMyChannelID, NULL, sizeof(m_szMyChannelID)) ;

					//if(NULL == pPrize->szNick[0])
					//	sprintf(m_szMyChannelID, "%s", pPrize->szID);
					//else 
					//	sprintf(m_szMyChannelID, "%s", pPrize->szNick);

					//convertIDtoProtect(m_szMyChannelID) ;

					//m_nJPTimeState = State_JPRoom_End ;

					//m_nPlayTime = 600 ;
					//m_nPreTime = m_nPlayTime/60 ;

					Reset() ;
					m_bisShowWinner = true ;
				}
			}
		}break;

		// 잭팟판 
	case OPCODE_SV_HALFJACKPOT_ENTER_JACKPOT_ROOM:
		{
			packet::TSendPacket< promotion::HALFJACKPOT_ENTER_JACKPOT_ROOM > JackpotTime_Packet;
			JackpotTime_Packet.Get( lpData, TotSize );
			promotion::HALFJACKPOT_ENTER_JACKPOT_ROOM* pRoom = static_cast< promotion::HALFJACKPOT_ENTER_JACKPOT_ROOM* >( JackpotTime_Packet.Various() );

			m_nJPTimeState = State_JPRoomChoice ;
			m_bIsJackpotRoom = true ;
			m_llhalfJackpotMoney = pRoom->llPrizeMoney ;
	
			if(100000000 == g_RI.llEnterLimitMoney)
			{
				m_nChannel = 0 ;
			
			}else if(1000000000 == g_RI.llEnterLimitMoney)
			{
				m_nChannel = 1 ;
			
			}else if(10000000000 == g_RI.llEnterLimitMoney)
			{
				m_nChannel = 2 ;
			
			}else if(100000000000 == g_RI.llEnterLimitMoney)
			{
				m_nChannel = 3 ;
			
			}else if(1000000000000 == g_RI.llEnterLimitMoney)
			{
				m_nChannel = 4 ;
			
			}else if(3000000000000 == g_RI.llEnterLimitMoney)
			{
				m_nChannel = 5 ;
			
			}else if(10000000000000 <= g_RI.llEnterLimitMoney)
			{
				m_nChannel = 6 ;

			}else
			{
				m_nChannel = -1 ;
			}		

		}break;

	case OPCODE_SV_HALFJACKPOT_JACKPOT_ROUND_START:
		{
			packet::TSendPacket< promotion::HALFJACKPOT_ENTER_JACKPOT_ROOM > JackpotTime_Packet;
			JackpotTime_Packet.Get( lpData, TotSize );
			promotion::HALFJACKPOT_ENTER_JACKPOT_ROOM* pRoom = static_cast< promotion::HALFJACKPOT_ENTER_JACKPOT_ROOM* >( JackpotTime_Packet.Various() );

			m_nJPTimeState = State_JPRoom_Start ;
			m_llhalfJackpotMoney = pRoom->llPrizeMoney ;

			m_bIsJackpotRoom = true ;
			m_bisShowWinner = false ;
			
			ShowJackpotGameStartEffect() ;

		}break;	

		//이벤트 종료 알림
	case OPCODE_SV_HALFJACKPOT_END:
		{
			//packet::TSendPacket< promotion::ADULTCARD_ALARM > AdultCard_Packet;
			//AdultCard_Packet.Get( lpData, TotSize );
			//promotion::ADULTCARD_ALARM* pAlarm = static_cast< promotion::ADULTCARD_ALARM* >( AdultCard_Packet.Various() );

		}break;
	}

	return FALSE;
}

void CPromotionSpecialJackpot::convertIDtoProtect(char* str)
{
	int nCount = 4 ;
	int nNicklen = 0 ;
	for(int i = 0; i < NICK_LEN; i++)
	{
		if(m_szMyChannelID[i] != NULL )
			nNicklen++ ;
	}

	if(nNicklen <= 5)
	{
		nCount = 2 ;
	
	}else
	{
		if(0 == nNicklen % 2)
		{
			nCount = nNicklen - 4 ;
		}else if(1 == nNicklen % 2)
		{
			nCount = nNicklen - 3 ;
		}
	}

	for(int i = nCount; i < NICK_LEN; i++)
	{
		if(m_szMyChannelID[i] != NULL )
			m_szMyChannelID[i] = '*' ;
	}
}

void CPromotionSpecialJackpot::Clear_Info()
{
	memset( &m_Jackpot_Info, 0, sizeof( promotion::_HALFJACKPOT_ALARM  ) );
}

void CPromotionSpecialJackpot::SetEventInfo( promotion::_HALFJACKPOT_ALARM *pInfo )	//판 진행시 마다
{
	if (pInfo==NULL) 
		return;

	memcpy( &m_Jackpot_Info, pInfo, sizeof( promotion::_HALFJACKPOT_ALARM ) );

	m_nChannel = m_Jackpot_Info.iChannel ;
	m_nMyhalfCount = m_Jackpot_Info.nHalfCount ;
	m_nBestHalfCount = m_Jackpot_Info.nTopHalfCount;			// 채널별 후보방에 들기 위한 하프 카운트
	m_nPlayTime = m_Jackpot_Info.nRemainTimeSec ;
	m_nPreTime = m_nPlayTime/60 ;
	
	setRoomJackpotTimeBackState(m_nPreTime) ;

	// 새로 방입장시 다시 하프 카운트 가능한지 체크.
	if(false == m_bIsHalfCountStart)
	{
		if( g_RI.NowUserNum >= 4)
		{
			int nCount = 0 ;
			for(int i = 0 ;i < g_RI.NowUserNum; i++)
			{
				if(1 == g_Poker.PS[i].JoinState) // 게임에 참가중이면.
				{
					nCount++ ;
				}
			}

			if(nCount >= 4)
			{
				m_bIsHalfCountStart = true ;

			}else
			{
				m_bIsHalfCountStart = false ;
			}

		}else
		{
			m_bIsHalfCountStart = false ;
		}
	}
}

//버튼이벤트
BOOL CPromotionSpecialJackpot::OnLButtonDownBottom(int &x , int &y)
{
	//테스트 코드.
	//if ( x < 200 && y < 200 )
	//{
	//	for(int i = 0 ; i < 5; i++)
	//	{
	//		m_cTestBackEffect[i].ShowEffect(11 + i, TRUE);
	//	}
	//}

	return FALSE;
}

//마우스 오버를 설정한다
BOOL CPromotionSpecialJackpot::OnMouseMoveBottom(int &x , int &y)
{
	//if ( x > 804 && x < (804+186) && y > 65 && y < (65+42) )
	//{
	//	m_bIsOverFlag = TRUE;
	//}else{
	//	m_bIsOverFlag = FALSE;
	//}

	return FALSE;
}

void CPromotionSpecialJackpot::OnTimer()
{
	switch(m_nJPTimeState)
	{
	case State_NoJPTime:
		break ;
	case State_JPTimeReady_Start:
		{
			if(0 <= m_nPreTime )
			{
				if(0 < m_nPlayTime)
				{
					m_nPlayTime-- ;

					if(m_nPreTime > (m_nPlayTime/60 + 1))
					{
						m_nPreTime-- ;
					}
					setRoomReadyBackState(m_nPreTime) ;		
				}
			}
		}
		break ;
	case State_JPTimeStart_notCount:
	case State_JPTimeStart_Count:
		{
			if(0 <= m_nPreTime )
			{
				if(0 < m_nPlayTime)
				{
					m_nPlayTime-- ;

					if(m_nPreTime > m_nPlayTime/60)
					{
						m_nPreTime-- ;										
					}
					setRoomJackpotTimeBackState(m_nPreTime) ;
				}
			}
		}
		break ;
	case State_JPTimeEnd_Count:
		break ;
	case State_JPTimeEnd_notCount:
		break ;
	case State_JPRoomChoice:
		break ;
	case State_JPRoomChoiceEnd:
		//if(0 < m_nPlayTime)
		//{
		//	m_nPlayTime-- ;

		//}else
		//{
		//	m_nJPTimeState = State_JPRoom_Start ;
		//}
		break ;
	case State_JPRoom_Start:
		break ;
	case State_JPRoom_End:

		if(0 <= m_nPreTime )
		{
			if(0 < m_nPlayTime)
			{
				m_nPlayTime-- ;

				if(m_nPreTime > m_nPlayTime/60)
				{
					m_nPreTime-- ;

					if(m_nPreTime == 0)
					{
						Reset() ;
					}
				}
			}
		}
		break ;

	case State_JPRoomNoMyChannel:
		{
			m_nPlayTime-- ;

			if(m_nPlayTime <= 0)
			{
				Reset() ;
			}
		}
		break ;
	}

	if(true == m_bIsShowRewardJPMoneyEffect)
	{
		if(0 < m_nRewardJPMoneyCount)
		{
			m_nRewardJPMoneyCount-- ;
			if( 0 >= m_nRewardJPMoneyCount)
			{
				HideRewardJPMoneyEffect();
				m_bIsShowRewardJPMoneyEffect = false ;
			}
		}
	}
}

void CPromotionSpecialJackpot::setRoomReadyBackState(int nTime)
{
	if(0 == nTime%3)
	{
		m_nBackSpriteCount = 15 ;

	}else if(1 == nTime%3)
	{
		m_nBackSpriteCount = 14 ;

	}else if(2 == nTime%3)
	{
		m_nBackSpriteCount = 16 ;
	}
}

void CPromotionSpecialJackpot::setRoomJackpotTimeBackState(int nTime) 
{
	if(0 <= nTime && 5 > m_nPlayTime)
	{
		m_nBackSpriteCount = 6 ;
		m_bIsShowJackpotmoney = false ;
		return ;
	}

	m_bIsShowJackpotmoney = false ;
	if(1 == nTime%2)
	{
		m_nBackSpriteCount = 4 ;

	}else if(0 == nTime%2)
	{
		m_nBackSpriteCount = 19 ;
		m_bIsShowJackpotmoney = true ;
	}
}

void CPromotionSpecialJackpot::OnStartGame()
{
	// 잭팟 타임 시작 활성
	if(State_JPTimeStart_notCount == m_nJPTimeState )
	{
		m_nJPTimeState = State_JPTimeStart_Count ;

		if(true == m_bIsShowStartEffect)
		{		
			m_bIsShowStartEffect = false ;

			ShowJackpotTimeStartEffect() ;
		}	
	
	}
	
	//else if (State_JPRoomChoiceEnd == m_nJPTimeState)
	//{
	//	if(false == m_bIsJackpotRoom)
	//	{				
	//		setChannelString(g_RI.llEnterLimitMoney) ;
	//		m_nJPTimeState = State_JPRoom_Start ;
	//	}
	//}

	if( g_RI.NowUserNum >= 4)
	{
		int nCount = 0 ;
		for(int i = 0 ;i < g_RI.NowUserNum; i++)
		{
			if(1 == g_Poker.PS[i].JoinState) // 게임에 참가중이면.
			{
				nCount++ ;
			}
		}

		if(nCount == g_RI.NowUserNum)
		{
			m_bIsHalfCountStart = true ;
		
		}else
		{
			m_bIsHalfCountStart = false ;
		}
	
	}else
	{
		m_bIsHalfCountStart = false ;
	}

	m_nBetRuleHalfCountCheck = 0 ;
	if(g_RI.nAutoBetKind > 0 )
	{
		m_nBetRuleHalfCountCheck = g_RI.nAutoBetKind%10 ;
	}

	HideRewardJPMoneyEffect() ;
}

void CPromotionSpecialJackpot::OnEndGame()	//게임이 끝나고 리셋 될때
{
	if(State_JPTimeEnd_Count == m_nJPTimeState )
	{
		m_nJPTimeState = State_JPTimeEnd_notCount ;
	}

	if(true == m_bIsShowEndEffect)
	{
		m_bIsShowEndEffect = false ;
		ShowJackpotTimeEndEffect() ;
	}

	//if(true == m_bIsShowJPRoomEndEffect )
	//{
	//	ShowJackpotGameEndEffect() ;
	//	m_bIsShowEndText = true ;

	//	m_dwStartEndTextTime = timeGetTime();

	//	// 하프 잭팟 머니 지급 이펙트 노출.
	//	m_bIsShowRewardJPMoneyEffect = true ;
	//	m_nRewardJPMoneyCount = 5 ;
	//	ShowRewardJPMoneyEffect();

	//	m_bIsShowJPRoomEndEffect = false ;
	//}
}

void CPromotionSpecialJackpot::setJackpotMoney(promotion::HALFJACKPOT_MONEY* pMoney) 
{
	if (pMoney==NULL) 
		return;

	memcpy( &m_Jackpot_MoneyInfo, pMoney, sizeof( promotion::HALFJACKPOT_MONEY ) );
	
	if( 0 == g_RI.llEnterLimitMoney )
		m_llhalfJackpotMoney = 0 ;

	m_llhalfJackpotMoney = pMoney->llJackpotMoney[m_nChannel] ;
}

void CPromotionSpecialJackpot::setJackpotMoneyInRoom()
{
	if( 0 == g_RI.llEnterLimitMoney )
		m_llhalfJackpotMoney = 0 ;

	m_llhalfJackpotMoney = m_Jackpot_MoneyInfo.llJackpotMoney[m_nChannel] ;
}

// 잭팟 방 선정.
void CPromotionSpecialJackpot::SetJackpotRoom(promotion::HALFJACKPOT_ROOM* pRoom )
{
	if(0 != pRoom->nCount)
	{
		for(int i = 0 ; i < pRoom->nCount; i++)
		{
			m_nRoomNum[pRoom->iChannel][i] = pRoom->iRoomNum[i] ;
		}
	}

	if( 7 <= m_nJPRoomNotiCount)
	{
		return ;
	}

	if(false == m_bIsJackpotRoom)
	{
		for(int i = 0 ; i < pRoom->nCount; i++)
		{
			if(m_nChannel == pRoom->iChannel)
			{
				m_nMyChannelRoomNum = pRoom->iRoomNum[i] ;

				if( g_RI.RoomNum == pRoom->iRoomNum[i] )
				{

					m_llhalfJackpotMoney = pRoom->llJackpotMoney ;
					m_nJPTimeState = State_JPRoomChoice ;
					m_bIsJackpotRoom = true ;
				}

				setChannelString(pRoom->llEnterLimitMoney[i]) ;	
			}
		}
	}

	m_nJPRoomNotiCount++;
	if(7 == m_nJPRoomNotiCount)
	{
		if(false == m_bIsJackpotRoom)
		{
			if(State_JPRoomChoice != m_nJPTimeState && State_NoJPTime != m_nJPTimeState)
			{
				m_nJPTimeState = State_JPRoomNoMyChannel ;
				m_nPlayTime = 180 ;
			}
		}
	}
}

void CPromotionSpecialJackpot::setHalfCountPlus() 
{
	if(State_JPTimeStart_Count == m_nJPTimeState || State_JPTimeEnd_Count == m_nJPTimeState )
	{
		if(	true ==	m_bIsHalfCountStart )
		{
			if(m_nBetRuleHalfCountCheck <= 0 )
			{
				m_nMyhalfCount++ ;

			}else if(m_nBetRuleHalfCountCheck > 0)
			{
				m_nBetRuleHalfCountCheck-- ;
			}
		}				
	}
}

INT64 CPromotionSpecialJackpot::ChangejackpotMoney(INT64 jackpotMoney)
{
	INT64 plus = 0;
	if(		jackpotMoney > 10000000000000)	plus = 987654321;
	else if(jackpotMoney > 1000000000000)	plus = 98765431;
	else if(jackpotMoney > 100000000000)	plus = 9876541;
	else if(jackpotMoney > 10000000000)		plus = 987651;
	else if(jackpotMoney > 1000000000)		plus = 98761;
	else if(jackpotMoney > 100000000)		plus = 9871;
	else if(jackpotMoney > 10000000)		plus = 981;
	else if(jackpotMoney > 1000000)			plus = 91;
	else if(jackpotMoney > 100000)			plus = 91;
	else if(jackpotMoney > 10000)			plus = 9;
	else if(jackpotMoney > 1000)			plus = 1; 
	else if(jackpotMoney > 50 )				plus = 1;
	else plus = 1;

	jackpotMoney+=plus; // 증가 보여주기
	
	return jackpotMoney;
}

void CPromotionSpecialJackpot::setChannelString(INT64 LimitEnterMoney) 
{
	if(0 == m_nChannel )
	{
		sprintf(m_szMyChannel, "1억방") ;
	
	}else if(1 == m_nChannel )
	{
		sprintf(m_szMyChannel, "10억방") ;
	
	}else if(2 == m_nChannel )
	{
		sprintf(m_szMyChannel, "100억방") ;
	
	}else if(3 == m_nChannel )
	{
		sprintf(m_szMyChannel, "1000억방") ;
	
	}else if(4 == m_nChannel )
	{
		sprintf(m_szMyChannel, "1조방") ;
	
	}else if(5 == m_nChannel )
	{
		sprintf(m_szMyChannel, "3조방") ;
	
	}else if(6 == m_nChannel )
	{
		if(10000000000000 == LimitEnterMoney)
		{
			sprintf(m_szMyChannel, "10조방") ;
		
		}else if(30000000000000 == LimitEnterMoney)
		{
			sprintf(m_szMyChannel, "30조방") ;
		
		}else if(50000000000000 == LimitEnterMoney)
		{
			sprintf(m_szMyChannel, "50조방") ;
		
		}else if(100000000000000 == LimitEnterMoney)
		{
			sprintf(m_szMyChannel, "100조방") ;
		}
	}
	
}

void CPromotionSpecialJackpot::convertJPMoneytoStr(INT64 jackpotmoney)
{
	if(jackpotmoney == 1000000000000 )
		sprintf(m_szJackpotMoney, "1조") ;
	else if(jackpotmoney > 1000000000000 )
		sprintf(m_szJackpotMoney, "%I64d조%I64d억%I64d만%I64d", jackpotmoney/1000000000000, (jackpotmoney%1000000000000)/100000000, (jackpotmoney%100000000)/10000, jackpotmoney%10000) ;
	else if(jackpotmoney < 1000000000000 )
		sprintf(m_szJackpotMoney, "%I64d억%I64d만%I64d", (jackpotmoney%1000000000000)/100000000, (jackpotmoney%100000000)/10000, jackpotmoney%10000) ;

	return ;
}
