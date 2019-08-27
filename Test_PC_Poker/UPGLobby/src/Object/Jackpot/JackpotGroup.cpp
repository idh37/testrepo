#include "StdAfx.h"
#include "JackpotGroup.h"
#include "JackpotMoney.h"
#include "JackpotCurrentInfoDlg.h"
#include "../../WidgetID.h"

#define _DEF_JACKPOT_TIMER 123459

CJackpotGroup::CJackpotGroup()
:	m_pJackpotMoney(NULL),
	m_pJackpotRelease(NULL),
	m_pBtnJackpotDetail(NULL),
	m_pJackpotDetailDlg(NULL),
	m_pSprJackpotRelease(NULL)
{
	m_bIsShowJackpot = false ;
	m_bIsArriveAllNoti = false ;
	m_nJackpotState = State_NoJPTime ;

	m_nPlayTime = 0 ;
	m_nPreTime = 0 ;

	m_dwTime = 0 ;

	m_nJPEndNotiCount = 0 ;
	m_nJPEndNotiPlayCount = 0 ;
	m_nJPWinnerNotiCount = 0 ;
	m_nJPWinnerNotiPlayCount = 0 ;

	m_nJPRoomNotiCount = 0;
	m_nJPChoiceRoomCount = 0 ;
	m_nJPWinnerCount = 0 ;

	m_nWinnerNotiLoopCount = 0 ;

	for(int i =0 ; i< 16; i++)
	{
		m_stJPEndNoti[i].isInData  = false ;
		m_stJPEndNoti[i].str = "" ;

		m_nArriveRoomNum[i] = -1;
	}

	for(int i =0 ; i< 46; i++)
	{
		m_stJPWinnerNoti[i].isInData  = false ;	
		m_stJPWinnerNoti[i].str = "" ;
	}
}

CJackpotGroup::~CJackpotGroup()
{
	GetObjectMan()->KillTimer(_DEF_JACKPOT_TIMER);
}

void CJackpotGroup::OnCreateChild(LSY::CObjectBase *pObject)
{
	if(pObject == NULL) return;
	bool bAddChild = false;
	switch(pObject->GetID())
	{
	case ID_LOBBY_NUMBER_JACKPOT:
		{
			m_pJackpotMoney			= (CJackpotMoney *)pObject;
			m_pJackpotMoney->SetShow(true);
		}
		break;
	case ID_LOBBY_IMG_JACKPOT_RELEASE:
		{
			m_pJackpotRelease		= (LSY::CImage *)pObject;
			m_pJackpotRelease->SetShow(false);

			pObject->AddHandler(LSY::EM_O_MOUSEOVER, LSY::Fnt(this, &CJackpotGroup::OnReleaseMouseOver));
			pObject->AddHandler(LSY::EM_O_MOUSELEAVE, LSY::Fnt(this, &CJackpotGroup::OnReleaseMouseLeave));
		}
		break;
	case ID_LOBBY_BTN_JACKPOT_DETAIL:			m_pBtnJackpotDetail		= (LSY::CButton *)pObject;		bAddChild = true; break;
	case ID_LOBBY_DLG_JACKPOT_DISPLAY:
		{
			m_pJackpotDetailDlg		= (CJackpotCurrentInfoDlg *)pObject;
			m_pJackpotDetailDlg->SetShow(false);

			pObject->AddHandler(LSY::EM_O_LOCALCHILDNOTIFY, LSY::Fnt(this, &CJackpotGroup::OnDetailDlgLocalChildNotify));
		}
		break;
	case ID_LOBBY_SPR_JAOKPOT_RELEASE:
		{
			m_pSprJackpotRelease	= (LSY::CSprite *)pObject;
			m_pSprJackpotRelease->StopAnimation();
		}
		break;

	case ID_LOBBY_SPR_HALFJACKPOT_BACK:
		{
			m_pHalfJackpotBack = (LSY::CSprite *)pObject;
			m_pHalfJackpotBack->SetPlayIndex(1);
			m_pHalfJackpotBack->StartAnimation() ;
			m_pHalfJackpotBack->SetShow(false) ;

			GetObjectMan()->SetTimer(_DEF_JACKPOT_TIMER, 1000, LSY::Fnt(this, &CJackpotGroup::OnTimer));
		}
		break ;

	case ID_LOBBY_HALFJACKPOT_TEXT:
		{
			m_pHalfJackpotNotiText = (LSY::CText *)pObject;
			m_pHalfJackpotNotiText->SetShow(false) ;
		}
		break ;

	case ID_LOBBY_HALFJACKPOT_CHANNEL_TEXT:
		{
			m_pHalfJackpotChannelText = (LSY::CText *)pObject;
			m_pHalfJackpotChannelText->SetShow(false) ;
		}
		break ;	

	case ID_LOBBY_HALFJACKPOTIMAGENUM:
		{
			memset( &m_pHalfjackpotMoney, 0, sizeof(promotion::HALFJACKPOT_MONEY) );
			
			m_pHalfJackpotJackpotMoney = (CHalfJackpotMoney *)pObject;
			m_pHalfJackpotJackpotMoney->SetShow(false) ;
		}
		break ;
	}			

	if(bAddChild)
	{
		pObject->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CJackpotGroup::OnClickButton));
	}

	__super::OnCreateChild(pObject);
}

LRESULT CJackpotGroup::OnReleaseMouseOver(LSY::CMessage *msg)
{
	LSY::CMO_MouseOver *pMsg = (LSY::CMO_MouseOver *)msg;
	LSY::CImage *pImage = (LSY::CImage *)pMsg->GetObject();
	if(pImage->GetID() == ID_LOBBY_IMG_JACKPOT_RELEASE)
	{
		if(!m_pSprJackpotRelease->IsShow())	m_pSprJackpotRelease->StartAnimation();
	}
	else if(pImage->GetIndex() == 51)
	{
		if(!m_pSprJackpotRelease->IsShow())	m_pSprJackpotRelease->StartAnimation();
	}

	return TRUE;
}

LRESULT CJackpotGroup::OnReleaseMouseLeave(LSY::CMessage *msg)
{
	LSY::CMO_MouseLeave *pMsg = (LSY::CMO_MouseLeave *)msg;
	LSY::CImage *pImage = (LSY::CImage *)pMsg->GetObject();

	if(pImage->GetID() == ID_LOBBY_IMG_JACKPOT_RELEASE)
	{
		if(m_pSprJackpotRelease->IsShow())	m_pSprJackpotRelease->StopAnimation();
	}
	else if(pImage->GetIndex() == 51)
	{
		if(m_pSprJackpotRelease->IsShow())	m_pSprJackpotRelease->StopAnimation();
	}

	return TRUE;
}

LRESULT CJackpotGroup::OnDetailDlgLocalChildNotify(LSY::CMessage *msg)
{
	LSY::CMO_LocalChildNotify *pMsg = (LSY::CMO_LocalChildNotify *)msg;
	LSY::CMessage *pMsg2 = pMsg->GetMessage();

	switch(pMsg2->GetID())
	{
	case LSY::EM_O_MOUSEOVER:
		return OnReleaseMouseOver(pMsg2);
	case LSY::EM_O_MOUSELEAVE:
		return OnReleaseMouseLeave(pMsg2);
	}

	return FALSE;
}

LRESULT CJackpotGroup::OnClickButton(LSY::CMessage *msg)
{
	LSY::CMO_MouseLClick *pMsg = (LSY::CMO_MouseLClick *)msg;
	LSY::CButton *pBtn = (LSY::CButton *)pMsg->GetObject();
	CWnd *pWnd = GetObjectMan()->GetWnd();
	switch(pBtn->GetID())
	{
	case ID_LOBBY_BTN_JACKPOT_DETAIL:
		m_pJackpotDetailDlg->SetShowDlg();
		break;
	}
	return TRUE;
}

void CJackpotGroup::SetJackpotRelease(void)
{
	int GameCode = -1;

	if( GM().GetCurrentGameType() == IDX_GAME_SP )
	{
		GameCode = 0;
	}
	else if( GM().GetCurrentGameType() == IDX_GAME_BD )
	{
		GameCode = 1;
	}
	else if( GM().GetCurrentGameType() == IDX_GAME_NP )
	{
		GameCode = 2;
	}
	else
	{
		SetJackpotRelease(false);
		return;
	}

	for( int i = 0; i < jackpot::DEF_POKERGAME_NUM; i++ )
	{
		if( i != GameCode ) continue;

		for(int j = 0; j < jackpot::DEF_MONEYRANGE_NUM; ++j )
		{
			if( g_JackPotMoneyInfo.stJPBase[ i ].bQuadrillionTime[ j ] )
			{
				SetJackpotRelease(true);
				return;
			}
		}
	}

	SetJackpotRelease(false);
}

void CJackpotGroup::SetJackpotRelease(bool bRelease)
{
	m_pJackpotRelease->SetShow(bRelease);
	m_pJackpotMoney->SetShow(!bRelease);
	GDM().SetBool( GDK_JACKPOT_RELEASE, bRelease );
}

// ÇÏÇÁÀèÆÌ °ü·Ã ÇÔ¼ö
void CJackpotGroup::SetHalfJackpotReady(UINT32 nTime)
{
	m_dwPreTime = m_dwNowTime = timeGetTime();
	m_dwTime = 0 ;

	m_pHalfJackpotBack->SetPlayIndex(1);
	m_pHalfJackpotBack->StartAnimation() ;
	m_pHalfJackpotBack->SetShow(true) ;

	m_pBtnJackpotDetail->SetShow(false) ;

	m_bIsShowJackpot = true ;
	
	m_nPlayTime = nTime ;
	m_nPreTime = (m_nPlayTime/60)+1 ;

	SetHalfJackpotReadyNotiText(m_nPreTime) ;

	m_nJackpotState = State_JPTimeReady_Start ;
}

void CJackpotGroup::SetHalfJackpotTimeStart(UINT32 nTime)
{
	m_dwPreTime = m_dwNowTime = timeGetTime();
	m_dwTime = 0 ;

	m_pHalfJackpotBack->SetPlayIndex(1);
	m_pHalfJackpotBack->StartAnimation() ;
	m_pHalfJackpotBack->SetShow(true) ;

	m_pBtnJackpotDetail->SetShow(false) ;

	m_bIsShowJackpot = true ;

	m_nPlayTime += nTime ;
	m_nPreTime = (m_nPlayTime/60)+1 ;

	SetHalfJackpotStartNotiText(m_nPreTime) ;

	m_nJackpotState = State_JPTimeStart ;
}

void CJackpotGroup::SetHalfJackpotTimeBeforeEnd()
{
	m_dwPreTime = m_dwNowTime = timeGetTime();
	m_dwTime = 0 ;

	m_pHalfJackpotBack->SetPlayIndex(1);
	m_pHalfJackpotBack->StartAnimation() ;
	m_pHalfJackpotBack->SetShow(true) ;

	m_pHalfJackpotNotiText->ClearSlideString(true) ;
	m_pHalfJackpotNotiText->SetText("Àá½ÃÈÄ ÀèÆÌÆÄÆ¼°¡ Á¾·áµË´Ï´Ù.(10ÃÊ ÈÄ)") ;
	m_pHalfJackpotNotiText->SetShow(true) ;

	m_bIsShowJackpot = true ;

	m_nPlayTime = 10 ;
	m_nPreTime = 0 ;

	m_nJackpotState = State_JPTimeBeforeEnd ;
}

void CJackpotGroup::SetHalfJackpotTimeEnd()
{
	m_pHalfJackpotNotiText->ClearSlideString(true) ;
	m_pHalfJackpotNotiText->SetText("Àá½Ã ÈÄ ÀèÆÌ¹æÀ» ¹ßÇ¥ÇÕ´Ï´Ù.") ;

	m_pHalfJackpotJackpotMoney->SetShow(false) ;
	m_pHalfJackpotChannelText->SetShow(false) ;

	m_nJackpotState = State_JPTimeEnd ;
}

void CJackpotGroup::SetHalfJackpotAlarm(promotion::_HALFJACKPOT_ALARM* pAlarm )
{
	m_dwPreTime = m_dwNowTime = timeGetTime();
	m_dwTime = 0 ;

	m_nPlayTime = pAlarm->nRemainTimeSec ;
	m_nPreTime = (m_nPlayTime/60)+1 ;

	if(	State_JPTimeReady_Start  == m_nJackpotState )
	{
		SetHalfJackpotReadyNotiText(m_nPreTime) ;
	
	}else if(State_JPTimeStart  == m_nJackpotState)
	{
		SetHalfJackpotStartNotiText(m_nPreTime) ;
	}
}

void CJackpotGroup::SetHalfJackpotMoney(promotion::HALFJACKPOT_MONEY* pMoney)
{
	memcpy(&m_pHalfjackpotMoney, pMoney, sizeof(promotion::HALFJACKPOT_MONEY)) ;

	if(true == m_bIsShowJackpot)
	{
		if(State_JPTimeStart == m_nJackpotState)
		{
			SetHalfJackpotMoney(m_nPreTime) ;
		}
	}
}

void CJackpotGroup::SetHalfJackpotRoom(promotion::HALFJACKPOT_ROOM* pRoom)
{
	m_nJPRoomNotiCount++;
	if(7 == m_nJPRoomNotiCount)
	{
		m_nJackpotState = State_JPRoomChoiceEnd ;
	
	}else if( 7 < m_nJPRoomNotiCount)
	{
		return ;
	}

	if(m_nJPEndNotiCount >= 16)
		return ;

	if(false == m_stJPEndNoti[m_nJPEndNotiCount].isInData)
	{
		if(0 == pRoom->nCount )
			return ;
		
		char str[128] = {0,} ;
		if(6 > pRoom->iChannel)
		{
			m_stJPEndNoti[m_nJPEndNotiCount].isInData = true ;

			if(0 == pRoom->iChannel)
			{
				sprintf(str, "1¾ï¹æ %d¹ø¹æÀÌ ÀèÆÌ¹æÀ¸·Î ¼±Á¤µÇ¾ú½À´Ï´Ù.", pRoom->iRoomNum[0]) ;
			}else if(1 == pRoom->iChannel)
			{
				sprintf(str, "10¾ï¹æ %d¹ø¹æÀÌ ÀèÆÌ¹æÀ¸·Î ¼±Á¤µÇ¾ú½À´Ï´Ù.", pRoom->iRoomNum[0]) ;
			}else if(2 == pRoom->iChannel)
			{
				sprintf(str, "100¾ï¹æ %d¹ø¹æÀÌ ÀèÆÌ¹æÀ¸·Î ¼±Á¤µÇ¾ú½À´Ï´Ù.", pRoom->iRoomNum[0]) ;
			}else if(3 == pRoom->iChannel)
			{
				sprintf(str, "1000¾ï¹æ %d¹ø¹æÀÌ ÀèÆÌ¹æÀ¸·Î ¼±Á¤µÇ¾ú½À´Ï´Ù.", pRoom->iRoomNum[0]) ;
			}else if(4 == pRoom->iChannel)
			{
				sprintf(str, "1Á¶¹æ %d¹ø¹æÀÌ ÀèÆÌ¹æÀ¸·Î ¼±Á¤µÇ¾ú½À´Ï´Ù.", pRoom->iRoomNum[0]) ;
			}else if(5 == pRoom->iChannel)
			{
				sprintf(str, "3Á¶¹æ %d¹ø¹æÀÌ ÀèÆÌ¹æÀ¸·Î ¼±Á¤µÇ¾ú½À´Ï´Ù.", pRoom->iRoomNum[0]) ;
			}

			m_stJPEndNoti[m_nJPEndNotiCount].str = str ;
			m_nJPEndNotiCount++ ;
		}else
		{
			for(int i = 0 ; i < pRoom->nCount; i++ )
			{
				m_stJPEndNoti[m_nJPEndNotiCount].isInData = true ;

				INT64 EnterLimitMoney = pRoom->llEnterLimitMoney[i] ; 
				if(10000000000000 == EnterLimitMoney)
				{
					sprintf(str, "10Á¶¹æ %d¹ø¹æÀÌ ÀèÆÌ¹æÀ¸·Î ¼±Á¤µÇ¾ú½À´Ï´Ù.", pRoom->iRoomNum[i]) ;
				}else if(30000000000000 == EnterLimitMoney)
				{
					sprintf(str, "30Á¶¹æ %d¹ø¹æÀÌ ÀèÆÌ¹æÀ¸·Î ¼±Á¤µÇ¾ú½À´Ï´Ù.", pRoom->iRoomNum[i]) ;
				}else if(50000000000000 == EnterLimitMoney)
				{
					sprintf(str, "50Á¶¹æ %d¹ø¹æÀÌ ÀèÆÌ¹æÀ¸·Î ¼±Á¤µÇ¾ú½À´Ï´Ù.", pRoom->iRoomNum[i]) ;
				}else if(100000000000000 == EnterLimitMoney)
				{
					sprintf(str, "100Á¶¹æ %d¹ø¹æÀÌ ÀèÆÌ¹æÀ¸·Î ¼±Á¤µÇ¾ú½À´Ï´Ù.", pRoom->iRoomNum[i]) ;
				}
				
				m_stJPEndNoti[m_nJPEndNotiCount].str = str ;
				m_nJPEndNotiCount++ ;
			}
		}
		m_nJPChoiceRoomCount++ ;
	}
}

void CJackpotGroup::SetHalfJackpotRoomEnd(promotion::HALFJACKPOT_PRIZE* pPrize)
{
	if(false == m_bIsArriveAllNoti)
	{
		for(int i = 0 ; i < 16 ; i++)
		{
			if(-1 != m_nArriveRoomNum[i])
			{
				if(m_nArriveRoomNum[i] == pPrize->iRoomNum )
				{
					break ;
				}		
			}else if(-1 == m_nArriveRoomNum[i])
			{
				m_nJPWinnerCount++;
				if(m_nJPChoiceRoomCount == m_nJPWinnerCount)
				{
					m_bIsArriveAllNoti = true ;	
				}
			}
		}
	}

	if(m_nJPWinnerNotiCount >= 46)
		return ;

	if(false == m_stJPWinnerNoti[m_nJPWinnerNotiCount].isInData)
	{
		if(0 == pPrize->llPrizeMoney)
			return ;

		char str[128] = {0,} ;
		char strName[NICK_LEN] = {0,} ;

		m_stJPWinnerNoti[m_nJPWinnerNotiCount].isInData = true ;
		
		if(NULL == pPrize->szNick[0])
			sprintf(strName, "%s", pPrize->szID) ;
		else 
			sprintf(strName, "%s", pPrize->szNick) ;

		int nCount = 4 ;
		int nNicklen = 0 ;
		for(int i = 0; i < NICK_LEN; i++)
		{
			if(strName[i] != NULL )
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
			if(strName[i] != NULL )
				strName[i] = '*' ;
		}
		
		convertJPMoneytoStr(pPrize->llPrizeMoney) ;

		if(6 > pPrize->iChannel)
		{	
			if(0 == pPrize->iChannel)
			{
				sprintf(str, "1¾ï¹æ %s´Ô %s È¹µæ!", strName, m_szJackpotMoney) ;
			}else if(1 == pPrize->iChannel)
			{
				sprintf(str, "10¾ï¹æ %s´Ô %s È¹µæ!",strName, m_szJackpotMoney) ;
			}else if(2 == pPrize->iChannel)
			{
				sprintf(str, "100¾ï¹æ %s´Ô %s È¹µæ!", strName, m_szJackpotMoney) ;
			}else if(3 == pPrize->iChannel)
			{
				sprintf(str, "1000¾ï¹æ %s´Ô %s È¹µæ!", strName, m_szJackpotMoney) ;
			}else if(4 == pPrize->iChannel)
			{
				sprintf(str, "1Á¶¹æ %s´Ô %s È¹µæ!", strName, m_szJackpotMoney) ;
			}else if(5 == pPrize->iChannel)
			{
				sprintf(str, "3Á¶¹æ %s´Ô %s È¹µæ!", strName, m_szJackpotMoney) ;
			}

			m_stJPWinnerNoti[m_nJPWinnerNotiCount].str = str ;
			m_nJPWinnerNotiCount++ ;
		}else
		{
			if(10000000000000 == pPrize->llEnterLimitMoney)
			{
				sprintf(str, "10Á¶¹æ %s´Ô %s È¹µæ!", strName, m_szJackpotMoney) ;
			}else if(30000000000000 == pPrize->llEnterLimitMoney)
			{
				sprintf(str, "30Á¶¹æ %s´Ô %s È¹µæ!", strName, m_szJackpotMoney) ;
			}else if(50000000000000 == pPrize->llEnterLimitMoney)
			{
				sprintf(str, "50Á¶¹æ %s´Ô %s È¹µæ!", strName, m_szJackpotMoney) ;
			}else if(100000000000000 == pPrize->llEnterLimitMoney)
			{
				sprintf(str, "100Á¶¹æ %s´Ô %s È¹µæ!", strName, m_szJackpotMoney) ;
			}

			m_stJPWinnerNoti[m_nJPWinnerNotiCount].str = str ;
			m_nJPWinnerNotiCount++ ;
		}
	}
}

void CJackpotGroup::SetHalfJackpotInit()
{
	ResetHalfJackpot() ;
}

void CJackpotGroup::ResetHalfJackpot() 
{
	m_bIsShowJackpot = false ;
	m_bIsArriveAllNoti = false ;
	m_nJackpotState = State_NoJPTime ;

	m_nPlayTime = 0 ;
	m_nPreTime = 0 ;

	m_dwTime = 0 ;

	m_nJPEndNotiCount = 0 ;
	m_nJPEndNotiPlayCount = 0 ;
	m_nJPWinnerNotiCount = 0 ;
	m_nJPWinnerNotiPlayCount = 0 ;

	m_nJPRoomNotiCount = 0;
	m_nJPChoiceRoomCount = 0 ;
	m_nJPWinnerCount = 0 ;

	m_nWinnerNotiLoopCount = 0 ;

	for(int i =0 ; i< 16; i++)
	{
		m_stJPEndNoti[i].isInData  = false ;
		m_stJPEndNoti[i].str = "" ;

		m_nArriveRoomNum[i] = -1;
	}

	for(int i =0 ; i< 46; i++)
	{
		m_stJPWinnerNoti[i].isInData  = false ;	
		m_stJPWinnerNoti[i].str = "" ;
	}

	m_pHalfJackpotBack->SetShow(false);
	m_pHalfJackpotNotiText->ClearSlideString(true) ;
	m_pHalfJackpotNotiText->SetShow(false);
	m_pHalfJackpotChannelText->ClearSlideString(true) ;
	m_pHalfJackpotChannelText->SetShow(false);
	m_pHalfJackpotJackpotMoney->SetShow(false);

	//ÇÏÇÁ ÀèÆÌÀÌ ³¡³ª¸é µðÅ×ÀÏ ¹öÆ° È°¼ºÈ­.
	m_pBtnJackpotDetail->SetShow(true) ;
}

LRESULT CJackpotGroup::OnTimer(LSY::CMessage *msg)
{
	m_dwNowTime = timeGetTime();
	DWORD dwTime = m_dwNowTime - m_dwPreTime;
	m_dwTime += dwTime ;
	m_dwPreTime = m_dwNowTime ;

	if(m_dwTime < 1000 )
	{
		return TRUE ;
	
	}else if(m_dwTime >= 1000)
	{
		m_dwTime -= 1000 ;
	}

	if(true == m_bIsShowJackpot)
	{
		switch(m_nJackpotState)
		{
		case State_JPTimeReady_Start: // ÀèÆÌ Å¸ÀÓ ÁØºñ.
			{
				if(m_nPlayTime > 0)
				{
					m_nPlayTime-- ;
					if(m_nPreTime > (m_nPlayTime/60) + 1)
					{
						m_nPreTime-- ;
						SetHalfJackpotReadyNotiText(m_nPreTime) ;
					}
				}
			}
			break ;
		case State_JPTimeStart: // ÀèÆÌ Å¸ÀÓ ½ÃÀÛ ÈÄ.
			{
				if(m_nPlayTime > 0)
				{
					m_nPlayTime-- ;
					if(m_nPreTime > (m_nPlayTime/60) + 1)
					{
						m_nPreTime-- ;

						if(m_nPreTime <= 30)
						{
							SetHalfJackpotStartNotiText(m_nPreTime) ; 
						}
					}
				}
			}
			break ;

		case State_JPTimeEnd: // ÀèÆÌ Å¸ÀÓ Á¾·á
			{
			}
			break ;
		case State_JPRoomChoice: // ¹æ¼±Á¤ ´ë±â Áß.
			{
			}
			break ;
		case State_JPRoomChoiceEnd: // ¹æ¼±Á¤ Á¾·á Áß.
			{
				m_nPlayTime-- ;

				if(0 >= m_nPlayTime )
				{
					SetChangeRoomNoti() ;
				}
			}
			break ;
		case State_JPRoom_End: // ÀèÆÌÆÇ Á¾·á. ´çÃ·ÀÚ ±Ý¾× ³ëÃâ.
			{
				m_nPlayTime-- ;

				if(0 >= m_nPlayTime )
				{
					SetChangeWinnerNoti() ;
				}
			}
			break ;
		case State_JPTimt_ALLEND: // ÀèÆÌÅ¸ÀÓ ¸ðµÎ Á¾·á ÈÄ 30ÃÊ µô·¹ÀÌ.
			{
				if(m_nPlayTime > 0)
				{
					m_nPlayTime-- ;

				}else if(m_nPlayTime <= 0)
				{
					ResetHalfJackpot() ;
				}
			}
			break ;			
		}
	}

	return TRUE ;
}

void CJackpotGroup::SetChangeRoomNoti()
{
	if(-1 == m_nJPEndNotiPlayCount || m_nJPEndNotiPlayCount > 16 )
	{
		m_nPlayTime = 10 ;
		//m_nJPEndNotiPlayCount = 0 ;

		if(true == m_bIsArriveAllNoti)
		{
			m_nWinnerNotiLoopCount = 1 ;
			m_nJackpotState = State_JPRoom_End ;
		}

		return ;
	}

	if(true == m_stJPEndNoti[m_nJPEndNotiPlayCount].isInData)
	{
		m_pHalfJackpotNotiText->ClearSlideString(true) ;
		m_pHalfJackpotNotiText->SetText(m_stJPEndNoti[m_nJPEndNotiPlayCount].str) ;

		m_nJPEndNotiPlayCount++ ;
		m_nPlayTime = 10 ;
	
	}else
	{
		m_nJPEndNotiPlayCount = -1 ;
	}
}

void CJackpotGroup::SetChangeWinnerNoti()
{
	if(-1 == m_nJPWinnerNotiPlayCount || m_nJPWinnerNotiPlayCount > 16 )
	{
		if(0 < m_nWinnerNotiLoopCount)
		{
			m_nWinnerNotiLoopCount-- ;
			m_nJPWinnerNotiPlayCount = 0 ;
		
		}else if(0 >= m_nWinnerNotiLoopCount)
		{
			m_pHalfJackpotNotiText->ClearSlideString(true) ;

			m_nPlayTime = 20 ;
			m_nJackpotState = State_JPTimt_ALLEND ;
		}
	}

	if(true == m_stJPWinnerNoti[m_nJPWinnerNotiPlayCount].isInData)
	{
		m_pHalfJackpotNotiText->ClearSlideString(true) ;
		m_pHalfJackpotNotiText->SetText(m_stJPWinnerNoti[m_nJPWinnerNotiPlayCount].str) ;

		m_nJPWinnerNotiPlayCount++ ;
		m_nPlayTime = 21 ;

	}else
	{
		m_nJPWinnerNotiPlayCount = -1 ;
	}
}

void CJackpotGroup::SetHalfJackpotReadyNotiText(int nTime)
{
	char strTemp[128] = {0,};
	std::string str ;

	m_pHalfJackpotNotiText->SetShow(true) ;
	m_pHalfJackpotNotiText->ClearSlideString(true) ;

	if(0 == nTime%4)
	{
		sprintf(strTemp, "ÇÏÇÁÈ½¼ö°¡ ³ôÀ»¼ö·Ï ÀèÆÌÆÇ È¹µæ°¡´É¼ºÀÌ ³ô¾ÆÁý´Ï´Ù.(%dºÐ)", nTime) ;
		str = strTemp ;

		m_pHalfJackpotNotiText->SetText(str) ;

	}else if(1 == nTime%4)
	{
		sprintf(strTemp, "ÀèÆÌ ÆÄÆ¼´Â 30ºÐ°£ ÁøÇàµË´Ï´Ù.(%dºÐ)", nTime) ;
		str = strTemp ;

		m_pHalfJackpotNotiText->SetText(str) ;

	}else if(2 == nTime%4)
	{
		sprintf(strTemp, "Àá½Ã ÈÄ ÀèÆÌÆÄÆ¼°¡ ½ÃÀÛµË´Ï´Ù.(%dºÐ)", nTime) ;
		str = strTemp ;

		m_pHalfJackpotNotiText->SetText(str) ;

	}else if(3 == nTime%4)
	{
		sprintf(strTemp, "ÀèÆÌÆÇ ÃÖÁ¾ ½ÂÀÚ¿¡°Ô ÀèÆÌ%sÀÌ Áö±ÞµË´Ï´Ù. (%dºÐ)", strChip_Name[(int)ROOM_CHIPKIND_NORMAL], nTime) ;
		str = strTemp ;

		m_pHalfJackpotNotiText->SetText(str) ;
	}
}

void CJackpotGroup::SetHalfJackpotStartNotiText(int nTime)
{
	char strTemp[128] = {0,};
	std::string str ;

	m_pHalfJackpotBack->SetPlayIndex(1);
	m_pHalfJackpotBack->StartAnimation() ;

	m_pHalfJackpotNotiText->SetShow(true) ;
	m_pHalfJackpotChannelText->SetShow(false) ;
	m_pHalfJackpotJackpotMoney->SetShow(false) ;

	m_pHalfJackpotNotiText->ClearSlideString(true) ;

	if(nTime == 30 || nTime == 16 || nTime == 2 )
	{
		sprintf(strTemp, "ÇöÀç ÀèÆÌÆÄÆ¼ÁßÀÔ´Ï´Ù.(%dºÐ)", nTime) ;
		str = strTemp ;
		
		m_pHalfJackpotNotiText->SetText(str) ;
	
	}else if(nTime == 29 || nTime == 15 || nTime == 1 )
	{
		sprintf(strTemp, "ÀèÆÌ ÆÄÆ¼´Â 30ºÐ°£ ÁøÇàµË´Ï´Ù.(%dºÐ)", nTime) ;
		str = strTemp ;

		m_pHalfJackpotNotiText->SetText(str) ;
	
	}else if(nTime == 28 || nTime == 14 )
	{
		sprintf(strTemp, "ÇÏÇÁÈ½¼ö°¡ ³ôÀ»¼ö·Ï ÀèÆÌÆÇ È¹µæ°¡´É¼ºÀÌ ³ô¾ÆÁý´Ï´Ù.(%dºÐ)", nTime) ;
		str = strTemp ;

		m_pHalfJackpotNotiText->SetText(str) ;
	
	}else if(nTime == 27 || nTime == 13 )
	{
		sprintf(strTemp, "ÀèÆÌÆÇ ÃÖÁ¾ ½ÂÀÚ¿¡°Ô´Â °Å¾×ÀÇ ÀèÆÌ¸Ó´Ï°¡ Áö±ÞµË´Ï´Ù. (%dºÐ)", nTime) ;
		str = strTemp ;

		m_pHalfJackpotNotiText->SetText(str) ;
	
	}else if((nTime <= 26 && nTime >= 17) || (nTime <= 12 && nTime >= 3))
	{
		m_pHalfJackpotBack->SetPlayIndex(0);
		m_pHalfJackpotBack->StartAnimation() ;

		if(nTime >= 17 )
		{
			nTime -= 17 ;
		}else
		{
			nTime -= 3 ;
		}

		if(nTime == 9)
			sprintf(strTemp, "[100Á¶¹æ]") ;
		else if(nTime == 8)
			sprintf(strTemp, "[50Á¶¹æ]") ;
		else if(nTime == 7)
			sprintf(strTemp, "[30Á¶¹æ]") ;
		else if(nTime == 6)
			sprintf(strTemp, "[10Á¶¹æ]") ;
		else if(nTime == 5)
			sprintf(strTemp, "[3Á¶¹æ]") ;
		else if(nTime == 4)
			sprintf(strTemp, "[1Á¶¹æ]") ;
		else if(nTime == 3)
			sprintf(strTemp, "[1000¾ï¹æ]") ;
		else if(nTime == 2)
			sprintf(strTemp, "[100¾ï¹æ]") ;
		else if(nTime == 1)
			sprintf(strTemp, "[10¾ï¹æ]") ;
		else if(nTime == 0)
			sprintf(strTemp, "[1¾ï¹æ]") ;		

		str = strTemp ;

		if(nTime >= 6)
			nTime = 6 ;

		INT64 lljackpotMoney = m_pHalfjackpotMoney.llJackpotMoney[nTime] ;
		m_pHalfJackpotJackpotMoney->SetNumber(lljackpotMoney) ;
		m_pHalfJackpotJackpotMoney->SetShow(true) ;

		m_pHalfJackpotNotiText->SetShow(false) ;

		m_pHalfJackpotChannelText->SetText(str) ;
		m_pHalfJackpotChannelText->SetShow(true) ;
	
	}else if(nTime == 0 )
	{
		sprintf(strTemp, "ÀèÆÌÆÇ ÃÖÁ¾ ½ÂÀÚ¿¡°Ô´Â °Å¾×ÀÇ ÀèÆÌ¸Ó´Ï°¡ Áö±ÞµË´Ï´Ù. (%dºÐ)", nTime) ;
		str = strTemp ;

		m_pHalfJackpotNotiText->SetText(str) ;
	}
}

void CJackpotGroup::SetHalfJackpotMoney(int nTime)
{
	if((nTime <= 26 && nTime >= 17) || (nTime <= 12 && nTime >= 3))
	{
		char strTemp[128] = {0,};
		std::string str ;

		m_pHalfJackpotBack->SetPlayIndex(0);
		m_pHalfJackpotBack->StartAnimation() ;

		if(nTime >= 17 )
		{
			nTime -= 17 ;
		}else
		{
			nTime -= 3 ;
		}

		if(nTime == 9)
			sprintf(strTemp, "[100Á¶¹æ]") ;
		else if(nTime == 8)
			sprintf(strTemp, "[50Á¶¹æ]") ;
		else if(nTime == 7)
			sprintf(strTemp, "[30Á¶¹æ]") ;
		else if(nTime == 6)
			sprintf(strTemp, "[10Á¶¹æ]") ;
		else if(nTime == 5)
			sprintf(strTemp, "[3Á¶¹æ]") ;
		else if(nTime == 4)
			sprintf(strTemp, "[1Á¶¹æ]") ;
		else if(nTime == 3)
			sprintf(strTemp, "[1000¾ï¹æ]") ;
		else if(nTime == 2)
			sprintf(strTemp, "[100¾ï¹æ]") ;
		else if(nTime == 1)
			sprintf(strTemp, "[10¾ï¹æ]") ;
		else if(nTime == 0)
			sprintf(strTemp, "[1¾ï¹æ]") ;		

		str = strTemp ;

		if(nTime >= 6)
			nTime = 6 ;

		INT64 lljackpotMoney = m_pHalfjackpotMoney.llJackpotMoney[nTime] ;
		m_pHalfJackpotJackpotMoney->SetNumber(lljackpotMoney) ;
		m_pHalfJackpotJackpotMoney->SetShow(true) ;

		m_pHalfJackpotNotiText->SetShow(false) ;

		m_pHalfJackpotChannelText->SetText(str) ;
		m_pHalfJackpotChannelText->SetShow(true) ;
	}
}

void CJackpotGroup::convertJPMoneytoStr(INT64 jackpotmoney)
{
	jackpotmoney = jackpotmoney/100000000 ;

	if(jackpotmoney == 10000 )
		sprintf(m_szJackpotMoney, "1Á¶") ;
	else if(jackpotmoney > 10000 )
		sprintf(m_szJackpotMoney, "%I64dÁ¶%I64d¾ï", jackpotmoney/10000, jackpotmoney%10000) ;
	else if(jackpotmoney < 10000 )
		sprintf(m_szJackpotMoney, "%I64d¾ï", jackpotmoney%10000) ;
	
	return ;
}