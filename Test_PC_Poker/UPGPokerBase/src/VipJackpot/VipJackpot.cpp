
#include "stdafx.h"
#include "VipJackpot.h"
#include "../UIInfoData.h"
#include "EventCommon.h"
#include <CommMsgDef_Game.h>
#include "../Sound.h"

CVipJackpot::CVipJackpot()
{
	m_bShowVipJackpotExplain=false;
	m_bShowVipJackpotBetExplain=false;
	m_pTxtScroll_List=NULL;
	m_nTimer=0;
	m_nSetTimer=0;
	m_nWinnerBackIndex=-1;
	m_dwWinnerBackTime=0;
	m_dwWinnerBackEndTime=0;
	m_nEffectIndex=-1;
	m_dwEffectTime=0;
	m_dwEffectEndTime=0;
	m_nEffectSound=0;
	m_nBetEffectIndex=-1;
	m_dwBetEffectTime=0;
	m_dwBetEffectEndTime=0;
	m_nBetPosEffectIndex=-1;
	m_dwBetPosEffectTime=0;
	m_dwBetPosEffectEndTime=0;
	m_nTimerEffectIndex=-1;
	m_dwTimerEffectTime=0;
	m_dwTimerEffectEndTime=0;
	m_dw5TimerEffectTime=0;
	m_n5TimerEffectSound=0;
	m_nStartEffectIndex=-1;
	m_dwStartEffectTime=0;
	m_dwStartEffectEndTime=0;
	m_cvtWinnerEffectIndex.clear();
	m_dwWinnerEffectTime=0;
	m_dwWinnerEffectEndTime=0;
	m_nVipJackpotState=VIPJACKPOT_STATE_NONE;
	m_llVipJackpotMoney=0ll;
	m_nStage=0;
	m_nState=VIPJACKPOT_STAGE_STATE_NONE;
	m_nRoomBetPoint=0;
	m_nReserveBetPoint=0;
	m_cpBetPos=CPoint(0, 0);
	m_bBetAccent=false;
	m_llShowJackpotMoney=0ll;
	m_llShowOldJackpotMoney=0ll;
//#if defined(_DEBUG)
//	m_llVipJackpotMoney=44000000000000ll;
//	m_nStage=rand()%4+1;
//	m_nState=VIPJACKPOT_STAGE_STATE_NONE;
//	m_nRoomBetPoint=1234;
//	m_nReserveBetPoint=4321;
//	m_sWinNames="minerasssg님,torrentsssss님";
//	m_sWinMoney="99조9999억";
//	m_cpBetPos=CPoint(100, 200);
//	m_cvtWinnerPos.push_back(0);
//	m_cvtWinnerPos.push_back(1);
//	m_cvtWinnerPos.push_back(4);
//#endif
	//string sTemp=ChangeID("minerasssg");
	//sTemp=ChangeID("낙서금지되어야");
	//sTemp=ChangeID("mine");
}

CVipJackpot::~CVipJackpot()
{
}

void CVipJackpot::LoadImage()
{
	string strDir=".\\PokerCommon\\data\\";
	_LOADSPRITEDEFINE(&m_sprVipJackpotBack, strDir, "VIPjacpot_ingameUI.spr");	
	_LOADSPRITEDEFINE(&m_sprVipJackpotEffect, strDir, "VIPjptime_effect.spr");	
	_LOADSPRITEDEFINE(&m_sprVipJackpotBtn, strDir, "VIPjacpot_btn.spr");
	_LOADSPRITEDEFINE(&m_sprVipJackpotBet, strDir, "VIPjacpot_beteft.spr");
}

void CVipJackpot::Init(CGameViewBase *pGameView)
{
	if(NULL==pGameView) return;
	LoadImage();

	m_btnVipJackpotExplain.Init(pGameView, &pGameView->Page, 291, 14, &m_sprVipJackpotBtn, 0, IDM_VIPJACKPOTEXPLAIN);
	m_btnVipJackpotBetExplain.Init(pGameView, &pGameView->Page, 160, 50, &m_sprVipJackpotBtn, 1, IDM_VIPJACKPOTBETEXPLAIN);

	m_pTxtScroll_List=new NMBASE::SKINGDI::CNTextScroll(CRect(208, 69, 208+585, 69+19), "", 0, &pGameView->Page, pGameView->GetDC());
	m_pTxtScroll_List->SetAlign(DT_SINGLELINE | DT_LEFT);
	m_pTxtScroll_List->SetColor(RGB(91, 19, 7));
	m_pTxtScroll_List->SetFont(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_16D_BOLD));
#if defined(_DEBUG)
#endif
	SetVipJackpotState(VIPJACKPOT_STATE_NONE, true);
}

void CVipJackpot::DrawBottom(NMBASE::GRAPHICGDI::CPage *pPage, CDC *pDC)
{
	if(false==IsVIPJackpot(GM().GetCurrentGameType())) return;
	if(false==IsVIPRoom(&g_RI)) return;
	DWORD dwTime=timeGetTime();
	pPage->DrawMultiScene(0, 0, &m_sprVipJackpotBack, 0, dwTime);
	DrawJackpotNumber(pPage);
	m_btnVipJackpotExplain.Draw(pDC);
}

void CVipJackpot::DrawJackpot(NMBASE::GRAPHICGDI::CPage *pPage, CDC *pDC)
{
	if(false==IsVIPJackpot(GM().GetCurrentGameType())) return;
	if(false==IsVIPRoom(&g_RI)) return;
	static DWORD dwOldTime=timeGetTime();
	DWORD dwTime=timeGetTime();
	int nVipJackpotNum=2;
	string sStr;
	CFont *pOldFont=pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_BOLD));
	COLORREF clrOld=pDC->GetTextColor();
	UINT oldFlags=pDC->SetTextAlign(TA_CENTER);
	if((VIPJACKPOT_STATE_RESERVE_END>=m_nVipJackpotState)&&(VIPJACKPOT_STATE_NONE<m_nVipJackpotState)){
		pPage->DrawMultiScene(0, 0, &m_sprVipJackpotBack, m_nVipJackpotState, dwTime);
		pDC->SetTextColor(RGB(240, 195, 122));
		sStr=NMBASE::UTIL::format("%d번방", g_RI.RoomNum);
		pDC->TextOut(879+45/2, 47, sStr.c_str(), sStr.size());
		if(false==m_sMininumMoney.empty()){
			pDC->SetTextColor(RGB(244, 181, 47));
			pDC->TextOut(939+45/2, 47, m_sMininumMoney.c_str(), m_sMininumMoney.size());
		}
		if(false==m_sRuleType.empty()){
			pDC->SetTextColor(RGB(179, 163, 123));
			pDC->TextOut(879+103/2, 73, m_sRuleType.c_str(), m_sRuleType.size());
		}
	}
	if((VIPJACKPOT_STATE_RESERVE_NOT_RESERVELIST==m_nVipJackpotState)||(VIPJACKPOT_STATE_RESERVE_NOT_RESERVEPRIZE==m_nVipJackpotState)){
		if(TRUE==m_pTxtScroll_List->ProcessObject(dwTime-dwOldTime)){
			m_pTxtScroll_List->DrawObject(pDC);
		}
		else{
			SetVipJackpotState(VIPJACKPOT_STATE_RESERVE_NOT_END);
		}
	}
	if(0<m_nTimer){
		m_nTimer-=(int)(dwTime-dwOldTime);
		if(0>=m_nTimer){
			m_nTimer=0;
		}
	}
	if((VIPJACKPOT_STATE_TIMEEND_PREV>=m_nVipJackpotState)&&(VIPJACKPOT_STATE_TIMESTART<=m_nVipJackpotState)){
		DrawBetPoint(m_nRoomBetPoint, pPage);
		DrawTimer(m_nTimer, pPage);
		DrawReserveBetPoint(m_nReserveBetPoint, pPage);
		m_btnVipJackpotBetExplain.Draw(pDC);
	}
	else if(VIPJACKPOT_STATE_CHOOSE==m_nVipJackpotState){
		//if(0<m_nTimer){
		//	m_nTimer-=(int)(dwTime-dwOldTime);
		//	if(0>=m_nTimer){
		//		m_nTimer=0;
		//	}
		//}
		DrawTimer(m_nTimer, pPage);
	}
	else if(VIPJACKPOT_STATE_CHOOSE_RESERVE_NOT==m_nVipJackpotState){
		if((-1!=m_nTimerEffectIndex)&&(dwTime>=m_dwTimerEffectTime)){
			DWORD dwTimerEffectTime=dwTime-m_dwTimerEffectTime;
			pPage->DrawMultiScene(0, 0, &m_sprVipJackpotEffect, m_nTimerEffectIndex, dwTimerEffectTime, FALSE);
			if((m_dwTimerEffectEndTime<dwTimerEffectTime)&&(0!=m_dwTimerEffectEndTime)){
				m_nTimerEffectIndex=-1;
				m_dwTimerEffectTime=0;
				m_dwTimerEffectEndTime=0;
				SetVipJackpotState(VIPJACKPOT_STATE_RESERVE_NOT_RESERVELIST);
			}
		}
	}
	else if((VIPJACKPOT_STATE_RESERVE_NOT_END==m_nVipJackpotState)||(VIPJACKPOT_STATE_RESERVE_END==m_nVipJackpotState)){
		if((-1!=m_nTimerEffectIndex)&&(dwTime>=m_dwTimerEffectTime)){
			DWORD dwTimerEffectTime=dwTime-m_dwTimerEffectTime;
			pPage->DrawMultiScene(0, 0, &m_sprVipJackpotEffect, m_nTimerEffectIndex, dwTimerEffectTime, FALSE);
			if((m_dwTimerEffectEndTime<dwTimerEffectTime)&&(0!=m_dwTimerEffectEndTime)){
				m_nTimerEffectIndex=-1;
				m_dwTimerEffectTime=0;
				m_dwTimerEffectEndTime=0;
				SetVipJackpotState(VIPJACKPOT_STATE_CLEAR);
			}
		}
	}
	else if(VIPJACKPOT_STATE_RESERVE_START==m_nVipJackpotState){
		//long long llMoney=44000000000000ll;
		DrawMoney(m_llVipJackpotMoney, pPage);
	}
	else if(VIPJACKPOT_STATE_RESERVE_WINNER==m_nVipJackpotState){
		DrawResult(m_sWinNames, m_sWinMoney, pDC);
		//if(0<m_nTimer){
		//	m_nTimer-=(int)(dwTime-dwOldTime);
		//	if(0>=m_nTimer){
		//		m_nTimer=0;
		//		//SetVipJackpotState(VIPJACKPOT_STATE_RESERVE_END);
		//	}
		//}
	}
	pDC->SetTextColor(clrOld);
	pDC->SelectObject(pOldFont);	
	pDC->SetTextAlign(oldFlags);
	dwOldTime=dwTime;
}


void CVipJackpot::DrawMiddle(NMBASE::GRAPHICGDI::CPage *pPage, CDC *pDC)
{

}

void CVipJackpot::DrawTop(NMBASE::GRAPHICGDI::CPage *pPage, CDC *pDC)
{
	DrawJackpot(pPage, pDC);

	if(false==IsVIPJackpot(GM().GetCurrentGameType())) return;
	if(false==IsVIPRoom(&g_RI)) return;
	static DWORD dwOldTime=timeGetTime();
	DWORD dwTime=timeGetTime();
	if(VIPJACKPOT_STATE_NONE<m_nVipJackpotState){
		if((-1!=m_nEffectIndex)&&(dwTime>=m_dwEffectTime)){
			DWORD dwEffectTime=dwTime-m_dwEffectTime;
			pPage->DrawMultiScene(0, 0, &m_sprVipJackpotEffect, m_nEffectIndex, dwEffectTime, ((0==m_dwEffectEndTime)?(TRUE):(FALSE)));
			if(4==m_nEffectIndex){
				if((950<dwEffectTime)&&(1==m_nEffectSound)){
					m_nEffectSound=0;
				}
			}
			else if(6==m_nEffectSound){
				if((1000<dwEffectTime)&&(4==m_nEffectSound)){
					m_nEffectSound=3;
				}
				else if((2000<dwEffectTime)&&(3==m_nEffectSound)){
					m_nEffectSound=2;
				}
				else if((3000<dwEffectTime)&&(2==m_nEffectSound)){
					m_nEffectSound=1;
				}
				else if((4000<dwEffectTime)&&(1==m_nEffectSound)){
					m_nEffectSound=0;
				}
			}
			if((dwEffectTime>=m_dwEffectEndTime)&&(0!=m_dwEffectEndTime)){
				m_nEffectIndex=-1;
				m_dwEffectTime=0;
				m_dwEffectEndTime=0;
				if(VIPJACKPOT_STATE_TIMESTARTNOTI==m_nVipJackpotState){
					SetVipJackpotState(VIPJACKPOT_STATE_TIMESTART);
				}
				else if(VIPJACKPOT_STATE_TIMEING_RESERVE==m_nVipJackpotState){
					m_nEffectIndex=5;
					m_dwEffectTime=::timeGetTime();
				}
				else if(VIPJACKPOT_STATE_TIMEEND_RESERVE_NOT==m_nVipJackpotState){
					SetVipJackpotState(VIPJACKPOT_STATE_CHOOSE);
				}
				else if(VIPJACKPOT_STATE_TIMEEND_RESERVE==m_nVipJackpotState){
					SetVipJackpotState(VIPJACKPOT_STATE_CHOOSE);
				}
				else if(VIPJACKPOT_STATE_RESERVE_WINNER==m_nVipJackpotState){
				}
			}
		}
		if((VIPJACKPOT_STATE_TIMEEND_PREV>=m_nVipJackpotState)&&(VIPJACKPOT_STATE_TIMESTART<=m_nVipJackpotState)){
			if((-1!=m_nBetEffectIndex)&&(dwTime>=m_dwBetEffectTime)){
				DWORD dwBetEffectTime=dwTime-m_dwBetEffectTime;
				pPage->DrawMultiScene(0, 0, &m_sprVipJackpotEffect, m_nBetEffectIndex, dwBetEffectTime, ((0==m_dwBetEffectEndTime)?(TRUE):(FALSE)));
				if((dwBetEffectTime>=m_dwBetEffectEndTime)&&(0!=m_dwEffectEndTime)){
					m_nBetEffectIndex=-1;
					m_dwBetEffectTime=0;
					m_dwBetEffectEndTime=0;
				}
			}
			if((-1!=m_nBetPosEffectIndex)&&(dwTime>=m_dwBetPosEffectTime)){
				DWORD dwBetPosEffectTime=dwTime-m_dwBetPosEffectTime;
				pPage->DrawMultiScene(m_cpBetPos.x, m_cpBetPos.y, &m_sprVipJackpotBet, m_nBetPosEffectIndex, dwBetPosEffectTime, ((0==m_dwBetPosEffectEndTime)?(TRUE):(FALSE)));
				if((dwBetPosEffectTime>=m_dwBetPosEffectEndTime)&&(0!=m_dwBetPosEffectEndTime)){
					m_nBetPosEffectIndex=-1;
					m_dwBetPosEffectTime=0;
					m_dwBetPosEffectEndTime=0;
				}
			}
		}
		if((-1!=m_nStartEffectIndex)&&(dwTime>=m_dwStartEffectTime)){
			DWORD dwStartEffectTime=dwTime-m_dwStartEffectTime;
			pPage->DrawMultiScene(0, 0, &m_sprVipJackpotEffect, m_nStartEffectIndex, dwStartEffectTime, ((0==m_dwStartEffectEndTime)?(TRUE):(FALSE)));
			if((dwStartEffectTime>=m_dwStartEffectEndTime)&&(0!=m_dwStartEffectEndTime)){
				if(9==m_nStartEffectIndex){
					m_nStartEffectIndex=10;
					m_dwStartEffectTime=::timeGetTime();
					m_dwStartEffectEndTime=0;
				}
				else{
					m_nStartEffectIndex=-1;
					m_dwStartEffectTime=0;
					m_dwStartEffectEndTime=0;
				}
			}
		}
		if((-1!=m_nWinnerBackIndex)&&(dwTime>=m_dwWinnerBackTime)){
			DWORD dwWinnerBackTime=dwTime-m_dwWinnerBackTime;
			pPage->DrawMultiScene(0, 0, &m_sprVipJackpotBack, m_nWinnerBackIndex, dwWinnerBackTime, TRUE);
			if((dwWinnerBackTime>=m_dwWinnerBackEndTime)&&(0!=m_dwWinnerBackEndTime)){
				m_nWinnerBackIndex=-1;
				m_dwWinnerBackTime=0;
				m_dwWinnerBackEndTime=0;
				if(VIPJACKPOT_STATE_RESERVE_WINNER==m_nVipJackpotState){
					for(int i=0; i<(int)m_cvtWinnerPos.size(); i++){
						m_cvtWinnerEffectIndex.push_back(m_cvtWinnerPos[i]+12);
						m_dwWinnerEffectTime=::timeGetTime();
						m_dwWinnerEffectEndTime=m_sprVipJackpotEffect.GetMScenePlayTime(m_cvtWinnerPos[i]+12);
					}
					PlaySound(SND_JP_PRIZE);					
				}
			}
			if(VIPJACKPOT_STATE_RESERVE_WINNER==m_nVipJackpotState){
				if((200<=dwWinnerBackTime)&&(2800>=dwWinnerBackTime)){
					CFont *pOldFont=pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_35D_BOLD));
					COLORREF clrOld=pDC->GetTextColor();
					UINT oldFlags=pDC->SetTextAlign(TA_CENTER);
					pDC->SetTextColor(RGB(91, 19, 7));
					CSize cSize;
					::GetTextExtentPoint32(pDC->GetSafeHdc(), m_sWinNames.c_str(), (int)m_sWinNames.size(), &cSize);
					if(310<cSize.cx){
						int nOldExtra=pDC->GetTextCharacterExtra();
						int nDiff=-1;
						do{
							pDC->SetTextCharacterExtra(nDiff);
							nDiff-=1;
							::GetTextExtentPoint32(pDC->GetSafeHdc(), m_sWinNames.c_str(), (int)m_sWinNames.size(), &cSize);
						}while(310<cSize.cx);
						pDC->TextOut(343+316/2, 307, m_sWinNames.c_str(), m_sWinNames.size());
						pDC->SetTextCharacterExtra(nOldExtra);
					}
					else{
						pDC->TextOut(343+316/2, 307, m_sWinNames.c_str(), m_sWinNames.size());
					}
					pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_40D_BOLD));
					pDC->SetTextColor(RGB(178, 26, 1));
					pDC->TextOut(388+225/2, 424, m_sWinMoney.c_str(), m_sWinMoney.size());
					pDC->SetTextColor(clrOld);
					pDC->SelectObject(pOldFont);		
					pDC->SetTextAlign(oldFlags);
				}
			}
		}
		else{
			if(VIPJACKPOT_STATE_RESERVE_WINNER==m_nVipJackpotState){
				if((false==m_cvtWinnerEffectIndex.empty())&&(dwTime>=m_dwWinnerEffectTime)){
					DWORD dwWinnerEffectTime=dwTime-m_dwWinnerEffectTime;
					CPoint cpPos[5]={CPoint(545+136, 661), CPoint(188+136, 436), CPoint(188+136, 243), CPoint(835+136, 243), CPoint(835+136, 436)};
					CFont *pOldFont=pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_26D_BOLD));
					COLORREF clrOld=pDC->GetTextColor();
					UINT oldFlags=pDC->SetTextAlign(TA_RIGHT);
					pDC->SetTextColor(RGB(178, 26, 1));
					string sWinnerMoney;
					for(int i=0; i<(int)m_cvtWinnerEffectIndex.size(); i++){
						pPage->DrawMultiScene(0, 0, &m_sprVipJackpotEffect, m_cvtWinnerEffectIndex[i], dwWinnerEffectTime, ((0==m_dwWinnerEffectEndTime)?(TRUE):(FALSE)));
						if(200<dwWinnerEffectTime){
							sWinnerMoney=GetJackpotMoneyString(m_cvtWinnerMoney[i]);							
							pDC->TextOut(cpPos[m_cvtWinnerEffectIndex[i]-12].x, cpPos[m_cvtWinnerEffectIndex[i]-12].y, sWinnerMoney.c_str(), sWinnerMoney.size());
						}
					}
					pDC->SetTextColor(clrOld);
					pDC->SelectObject(pOldFont);		
					pDC->SetTextAlign(oldFlags);
					if((dwWinnerEffectTime>=m_dwWinnerEffectEndTime)&&(0!=m_dwWinnerEffectEndTime)){
						m_cvtWinnerEffectIndex.clear();
						m_dwWinnerEffectTime=0;
						m_dwWinnerEffectEndTime=0;
						SetVipJackpotState(VIPJACKPOT_STATE_RESERVE_END);
					}
				}
			}
		}
	}
	if(true==m_bShowVipJackpotExplain){
		pPage->DrawMultiScene(0, 0, &m_sprVipJackpotBack, 1, dwTime);
		//int nStage=rand()%4+1;
		//int nState=rand()%3;
		int nIndex[4]={0,};
		switch(m_nStage){
		case 0: nIndex[0]=13; nIndex[1]=16; nIndex[2]=19; nIndex[3]=22; break; 
		case 1: nIndex[0]=13+m_nState; nIndex[1]=16; nIndex[2]=19; nIndex[3]=22; break;
		case 2: nIndex[0]=13+2; nIndex[1]=16+m_nState; nIndex[2]=19; nIndex[3]=22; break;
		case 3: nIndex[0]=13+2; nIndex[1]=16+2; nIndex[2]=19+m_nState; nIndex[3]=22; break;
		case 4: nIndex[0]=13+2; nIndex[1]=16+2; nIndex[2]=19+2; nIndex[3]=22+m_nState; break;
		default: nIndex[0]=13+2; nIndex[1]=16+2; nIndex[2]=19+2; nIndex[3]=22+2; break;
		}
		for(int i=0; i<4; i++){
			pPage->PutSprAuto(831+i*40, 63, &m_sprVipJackpotBack, nIndex[i], 0);
		}
	}
	else if(true==m_bShowVipJackpotBetExplain){
		if((VIPJACKPOT_STATE_TIMEEND_PREV>=m_nVipJackpotState)&&(VIPJACKPOT_STATE_TIMESTART<=m_nVipJackpotState)){
			pPage->DrawMultiScene(0, 0, &m_sprVipJackpotBack, 5, dwTime);
		}
		else{
			m_bShowVipJackpotBetExplain=false;
		}
	}
	dwOldTime=dwTime;
}

void CVipJackpot::DrawJackpotNumber(NMBASE::GRAPHICGDI::CPage *pPage)
{
#if defined(_JACKPOT_NON_CHANNEL_MONEY)
	INT64 &n64JackPotMoney	  = GDM().GetINT64(GDK_JACKPOT_MONEY);
	INT64 &n64TmpJackPotMoney = GDM().GetINT64(GDK_JACKPOT_MONEY_TEMP);

	m_llShowJackpotMoney=n64TmpJackPotMoney;
	m_llShowOldJackpotMoney=n64JackPotMoney;
#endif

	INT64 llDiff=m_llShowJackpotMoney-m_llShowOldJackpotMoney;
	INT64 llPlus=0;
	if(0==m_llShowOldJackpotMoney) m_llShowOldJackpotMoney=(long long)(m_llShowJackpotMoney*0.9);
	if(m_llShowJackpotMoney>m_llShowOldJackpotMoney){
		if(		llDiff > 10000000000) llPlus=987654321;
		else if(llDiff > 1000000000)  llPlus=98765431;
		else if(llDiff > 100000000)   llPlus=9876541;
		else if(llDiff > 10000000)    llPlus=987651;
		else if(llDiff > 1000000)     llPlus=98761;
		else if(llDiff > 100000)	  llPlus=9871;
		else if(llDiff > 10000)	      llPlus=981;
		else if(llDiff > 1000)        llPlus=91; 
		else if(llDiff > 50 )         llPlus=9;
		else llPlus=1;
		m_llShowOldJackpotMoney += llPlus; // 증가 보여주기
	}
	else if(m_llShowJackpotMoney<m_llShowOldJackpotMoney){
		if(		llDiff < -100000000000000000) llPlus=9999999987654321;
		else if(llDiff < -10000000000000000)  llPlus=999999987654321;
		else if(llDiff < -1000000000000000)   llPlus=99999987654321;
		else if(llDiff < -100000000000000)    llPlus=9999987654321;
		else if(llDiff < -10000000000000)     llPlus=999987654321;
		else if(llDiff < -1000000000000)      llPlus=99987654321;
		else if(llDiff < -100000000000)       llPlus=9987654321;
		else if(llDiff < -10000000000)        llPlus=987654321;
		else if(llDiff < -1000000000)         llPlus=98765431;
		else if(llDiff < -100000000)	      llPlus=9876521;
		else if(llDiff < -10000000)           llPlus=987651;
		else if(llDiff < -1000000)            llPlus=98761;
		else if(llDiff < -100000)	          llPlus=9871;
		else if(llDiff < -10000)	          llPlus=981;
		else if(llDiff < -1000)               llPlus=91; 
		else if(llDiff < -50 )                llPlus=9;
		else llPlus=1;
		m_llShowOldJackpotMoney-=llPlus; // 증가 보여주기
	}
	string sJackpotNum=NMBASE::UTIL::g_MakeCommaMoney(m_llShowOldJackpotMoney);
	int nTotal=sJackpotNum.size();
	int nNum=0;
	char cCur[2]={0,};
	for(int i=nTotal-1; i>=0; i--){
		cCur[0]=(char)sJackpotNum[i];
		if(','==cCur[0]){
			pPage->PutSprAuto(275-(nTotal-1-i)*7, 16, &m_sprVipJackpotBack, 1+10, AB);
		}
		else{
			nNum=atoi((char*)cCur);
			if(0>nNum) nNum=0;
			if(9<nNum) nNum=9;
			pPage->PutSprAuto(275-(nTotal-1-i)*7, 16, &m_sprVipJackpotBack, 1+nNum, AB);
		}
	}
	GDM().SetINT64(GDK_JACKPOT_MONEY, m_llShowOldJackpotMoney);
	GDM().SetINT64(GDK_JACKPOT_MONEY_TEMP, m_llShowJackpotMoney);
}

void CVipJackpot::DrawBetPoint(int nBetPoint, NMBASE::GRAPHICGDI::CPage *pPage)
{
	if(0>nBetPoint){ nBetPoint=0; }
	string sBetPoint=NMBASE::UTIL::format("%d", nBetPoint);
	int nTotal=(int)sBetPoint.size();
	int nNum=0;
	char cCur[2]={0,};
	for(int i=nTotal-1; i>=0; i--){
		cCur[0]=(char)sBetPoint[i];
		nNum=atoi((char*)cCur);
		if(0>nNum) nNum=0;
		if(9<nNum) nNum=9;
		pPage->PutSprAuto(239-(nTotal-1-i)*20, 50, &m_sprVipJackpotBack, 41+nNum, AB);
	}
}

void CVipJackpot::DrawReserveBetPoint(int nBetPoint, NMBASE::GRAPHICGDI::CPage *pPage)
{
	if(0>nBetPoint){ nBetPoint=0; }
	string sBetPoint=NMBASE::UTIL::format("%d", nBetPoint);
	int nTotal=(int)sBetPoint.size();
	int nNum=0;
	char cCur[2]={0,};
	for(int i=nTotal-1; i>=0; i--){
		cCur[0]=(char)sBetPoint[i];
		nNum=atoi((char*)cCur);
		if(0>nNum) nNum=0;
		if(9<nNum) nNum=9;
		pPage->PutSprAuto(551-(nTotal-1-i)*12, 69, &m_sprVipJackpotBack, 114+nNum, AB);
	}
}

void CVipJackpot::DrawTimer(int nTimer, NMBASE::GRAPHICGDI::CPage *pPage)
{
	if(0>nTimer){ nTimer=0; }
	int nTime=(int)(nTimer/1000);
	int nMinute=(nTime/60)%60;
	int nSecond=nTime%60;
	string sMinute=NMBASE::UTIL::format("%02d", nMinute);
	string sSecond=NMBASE::UTIL::format("%02d", nSecond);
	int nTotal=(int)sMinute.size();
	int nNum=0;
	char cCur[2]={0,};
	int nSpriteNum=52;
	int nSpriteNumBar=51;
	if(VIPJACKPOT_STATE_CHOOSE==m_nVipJackpotState){
		nSpriteNum=89;
		nSpriteNumBar=88;
	}
	if((5>=nTime)&&(0!=nTime)){
		nSpriteNum=63;
		nSpriteNumBar=62;
		if(0==m_dw5TimerEffectTime){
			m_dw5TimerEffectTime=::timeGetTime();
			if(4<nTime){
				PlaySound(SND_JP_TIMER);
			}
		}
	}
	if(0==nTime){
		if(0!=m_dw5TimerEffectTime){
			PlaySound(SND_JP_END);
		}
		m_dw5TimerEffectTime=0;
	}
	pPage->PutSprAuto(812, 56, &m_sprVipJackpotBack, nSpriteNumBar, AB);
	for(int i=nTotal-1; i>=0; i--){
		cCur[0]=(char)sMinute[i];
		nNum=atoi((char*)cCur);
		if(0>nNum) nNum=0;
		if(9<nNum) nNum=9;
		pPage->PutSprAuto(794-(nTotal-1-i)*15, 56, &m_sprVipJackpotBack, nSpriteNum+nNum, AB);
	}
	nTotal=(int)sSecond.size();
	for(int i=nTotal-1; i>=0; i--){
		cCur[0]=(char)sSecond[i];
		nNum=atoi((char*)cCur);
		if(0>nNum) nNum=0;
		if(9<nNum) nNum=9;
		pPage->PutSprAuto(833-(nTotal-1-i)*15, 56, &m_sprVipJackpotBack, nSpriteNum+nNum, AB);
	}
	DWORD dwTime=::timeGetTime();
	if(-1!=m_nTimerEffectIndex){
		float fPersent=((float)(m_nSetTimer-m_nTimer))/((float)m_nSetTimer);
		double dwTimerEffectEndTime=m_sprVipJackpotEffect.GetMScenePlayTime(m_nTimerEffectIndex);
		DWORD dwTimerEffectTime=(DWORD)(dwTimerEffectEndTime*fPersent);//dwTime-m_dwTimerEffectTime;
		if(dwTimerEffectEndTime<dwTimerEffectTime){
			dwTimerEffectTime=(DWORD)dwTimerEffectEndTime;
		}
		pPage->DrawMultiScene(0, 0, &m_sprVipJackpotEffect, m_nTimerEffectIndex, dwTimerEffectTime, FALSE);
	}
	if(0!=m_dw5TimerEffectTime){
		DWORD dw5TimerEffectTime=dwTime-m_dw5TimerEffectTime;
		pPage->DrawMultiScene(0, 0, &m_sprVipJackpotEffect, 20, dw5TimerEffectTime, FALSE);
	}
}

void CVipJackpot::DrawMoney(long long llMoney, NMBASE::GRAPHICGDI::CPage *pPage)
{
	if(0ll>llMoney){ llMoney=0ll; }
	int nMoney=((int)(llMoney/100000000000ll));
	int nMoney1=(int)(nMoney/10);
	int nMoney2=(int)(nMoney%10);
	string sMoney1=NMBASE::UTIL::format("%d", nMoney1);
	int nTotal=(int)sMoney1.size();
	if(3<=nTotal){
		nMoney2=0;
	}
	int nNum=0;
	char cCur[2]={0,};
	if(0==nMoney2){
		for(int i=nTotal-1; i>=0; i--){
			cCur[0]=(char)sMoney1[i];
			nNum=atoi((char*)cCur);
			if(0>nNum) nNum=0;
			if(9<nNum) nNum=9;
			pPage->PutSprAuto(514-(nTotal-1-i)*12, 69, &m_sprVipJackpotBack, 114+nNum, AB);
		}
	}
	else{
		for(int i=nTotal-1; i>=0; i--){
			cCur[0]=(char)sMoney1[i];
			nNum=atoi((char*)cCur);
			if(0>nNum) nNum=0;
			if(9<nNum) nNum=9;
			pPage->PutSprAuto(498-(nTotal-1-i)*12, 69, &m_sprVipJackpotBack, 114+nNum, AB);
		}
		pPage->PutSprAuto(509, 69, &m_sprVipJackpotBack, 124, AB);
		pPage->PutSprAuto(514, 69, &m_sprVipJackpotBack, 114+nMoney2, AB);
	}
}

void CVipJackpot::DrawResult(string sName, string sMoney, CDC *pDC)
{
	if(NULL==pDC) return;
	pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_16D_BOLD));
	pDC->SetTextAlign(TA_LEFT);
	string sMiddle= NMBASE::UTIL::format(" 승리! 잭팟%s", strChip_Name[(int)ROOM_CHIPKIND_NORMAL]);
	string sEnd="을 획득하였습니다.!";
	string sStr=NMBASE::UTIL::format("%s%s%s%s", sName.c_str(), sMiddle.c_str(), sMoney.c_str(), sEnd.c_str());
	CSize cSize;
	::GetTextExtentPoint32(pDC->GetSafeHdc(), sStr.c_str(), (int)sStr.size(), &cSize);
	CPoint cpStart=CPoint(196+(609-cSize.cx)/2, 70);
	pDC->SetTextColor(RGB(178, 26, 1));
	pDC->TextOut(cpStart.x, cpStart.y, sName.c_str(), sName.size());
	::GetTextExtentPoint32(pDC->GetSafeHdc(), sName.c_str(), (int)sName.size(), &cSize);
	cpStart.x+=cSize.cx;
	pDC->SetTextColor(RGB(91, 19, 7));
	pDC->TextOut(cpStart.x, cpStart.y, sMiddle.c_str(), sMiddle.size());
	::GetTextExtentPoint32(pDC->GetSafeHdc(), sMiddle.c_str(), (int)sMiddle.size(), &cSize);
	cpStart.x+=cSize.cx;
	pDC->SetTextColor(RGB(178, 26, 1));
	pDC->TextOut(cpStart.x, cpStart.y, sMoney.c_str(), sMoney.size());
	::GetTextExtentPoint32(pDC->GetSafeHdc(), sMoney.c_str(), (int)sMoney.size(), &cSize);
	cpStart.x+=cSize.cx;
	pDC->SetTextColor(RGB(91, 19, 7));
	pDC->TextOut(cpStart.x, cpStart.y, sEnd.c_str(), sEnd.size());
}

BOOL CVipJackpot::OnPacketNotify(char* pName, int Signal, int TotSize, char *lpData)
{
	switch(Signal)
	{
	case SV_VIP_JACKPOT_BET_POINT:
		{
			//유저 아이디로 위치 판단
		}
		break;
	case OPCODE_SV_VIPJACKPOT_START:            // 로그인 후 유저에게 프로모션 정보 전달
		{
			if(NULL!=lpData){
				packet::TSendPacket< promotion::VIPJACKPOT_TIME > Packet;
				Packet.Get(lpData, TotSize );
				promotion::VIPJACKPOT_TIME* pTime=static_cast< promotion::VIPJACKPOT_TIME* >(Packet.Various() );
				SetStage(pTime->nTodayEventCount);
				ErrorLogError(NMBASE::SOCK::CSockMan::GetSocketTxt().c_str(), " OPCODE_SV_VIPJACKPOT_START=%d=%d=%d", m_nStage, m_nRoomBetPoint, m_nReserveBetPoint);
			}
		}
		break;
	case OPCODE_SV_VIPJACKPOT_INITIALIZE:      // 로그인 후 유저에게 프로모션 정보 전달
		{
			if(NULL!=lpData){
				packet::TSendPacket< promotion::VIPJACKPOT_TIME > Packet;
				Packet.Get(lpData, TotSize );
				promotion::VIPJACKPOT_TIME* pTime=static_cast< promotion::VIPJACKPOT_TIME* >(Packet.Various() );
				SetStage(pTime->nTodayEventCount);
				//bool bIsRoom=((pAlaram->nTopHalfCount<=pAlaram->nHalfCount)?(true):(false));
				SetVipJackpotState(VIPJACKPOT_STATE_NONE, false);
				//SetVipJackpotServerState(pAlaram->nEventState, bIsRoom, pAlaram->bJackpotRoom);
				ErrorLogError(NMBASE::SOCK::CSockMan::GetSocketTxt().c_str(), " OPCODE_SV_VIPJACKPOT_INITIALIZE=%d=%d=%d", m_nStage, m_nRoomBetPoint, m_nReserveBetPoint);
			}
		}
		break;
	case OPCODE_SV_VIPJACKPOT_TIME_BEFORE_START:   // 잭팟 타임 시작전(다음판에 잭팟 타임 시작)
		{
			if(NULL!=lpData){
				packet::TSendPacket< promotion::VIPJACKPOT_TIME > Packet;
				Packet.Get(lpData, TotSize );
				promotion::VIPJACKPOT_TIME* pTime=static_cast< promotion::VIPJACKPOT_TIME* >(Packet.Various() );
				SetStage(pTime->nTodayEventCount);
				SetVipJackpotState(VIPJACKPOT_STATE_TIMENOTICE);
				ErrorLogError(NMBASE::SOCK::CSockMan::GetSocketTxt().c_str(), " OPCODE_SV_VIPJACKPOT_TIME_BEFORE_START=%d=%d", m_nStage, pTime->nRemainTimeSec);
			}			
		}
		break;
	case OPCODE_SV_VIPJACKPOT_TIME_START:         // 잭팟 타임 시작
		{
			if(NULL!=lpData){
				packet::TSendPacket< promotion::VIPJACKPOT_TIME > Packet;
				Packet.Get(lpData, TotSize );
				promotion::VIPJACKPOT_TIME* pTime=static_cast< promotion::VIPJACKPOT_TIME* >(Packet.Various() );
				SetVipJackpotState(VIPJACKPOT_STATE_TIMESTARTNOTI);
				m_nSetTimer=m_nTimer=pTime->nRemainTimeSec*1000;
				if(0>m_nSetTimer) m_nSetTimer=0;
				if(0>m_nTimer) m_nTimer=0;
				SetStage(pTime->nTodayEventCount);
				ErrorLogError(NMBASE::SOCK::CSockMan::GetSocketTxt().c_str(), " OPCODE_SV_VIPJACKPOT_TIME_START=%d=%d", m_nStage, pTime->nRemainTimeSec);
			}			
		}
		break;
	case OPCODE_SV_VIPJACKPOT_TIME_BEFORE_END:      // 잭팟 타임 종료전(다음판에 잭팟 타임 종료)
		{
			if(NULL!=lpData){
				packet::TSendPacket< promotion::VIPJACKPOT_TIME > Packet;
				Packet.Get(lpData, TotSize );
				promotion::VIPJACKPOT_TIME* pTime=static_cast< promotion::VIPJACKPOT_TIME* >(Packet.Various() );
				SetVipJackpotState(VIPJACKPOT_STATE_TIMEEND_PREV);
				SetStage(pTime->nTodayEventCount);
				//if(0==m_nSetTimer){
				//	m_nSetTimer=m_nTimer=pTime->nRemainTimeSec*1000;
				//}
				//else{
				//	m_nTimer=pTime->nRemainTimeSec*1000;
				//}
				//if(0>m_nSetTimer) m_nSetTimer=0;
				//if(0>m_nTimer) m_nTimer=0;
				ErrorLogError(NMBASE::SOCK::CSockMan::GetSocketTxt().c_str(), " OPCODE_SV_VIPJACKPOT_TIME_BEFORE_END=%d=%d", m_nStage, pTime->nRemainTimeSec);
			}			
		}
		break;
	case OPCODE_SV_VIPJACKPOT_TIME_END:            // 잭팟 타임 종료
		{
			if(NULL!=lpData){
				packet::TSendPacket< promotion::VIPJACKPOT_TIME > Packet;
				Packet.Get(lpData, TotSize );
				promotion::VIPJACKPOT_TIME* pTime=static_cast< promotion::VIPJACKPOT_TIME* >(Packet.Various() );
				SetStage(pTime->nTodayEventCount);
				ErrorLogError(NMBASE::SOCK::CSockMan::GetSocketTxt().c_str(), " OPCODE_SV_VIPJACKPOT_TIME_END=%d=%d", m_nStage, pTime->nRemainTimeSec);
			}			
		}
		break;
	case OPCODE_SV_VIPJACKPOT_WAIT_PULL_JACKPOT:   // 당첨방 뽑는 중
		{
			if(NULL!=lpData){
				packet::TSendPacket< promotion::VIPJACKPOT_TIME > Packet;
				Packet.Get(lpData, TotSize );
				promotion::VIPJACKPOT_TIME* pTime=static_cast< promotion::VIPJACKPOT_TIME* >(Packet.Various() );
				SetStage(pTime->nTodayEventCount);
				ErrorLogError(NMBASE::SOCK::CSockMan::GetSocketTxt().c_str(), " OPCODE_SV_VIPJACKPOT_WAIT_PULL_JACKPOT=%d=%d", m_nStage, pTime->nRemainTimeSec);
			}	
		}
		break;
	case OPCODE_SV_VIPJACKPOT_TOTAL_WAIT_ROOM_INFO:
		{
			if(NULL!=lpData){
				packet::TSendPacket< promotion::VIPJACKPOT_TOTAL_WAIT_ROOM_INFO > Packet;
				Packet.Get(lpData, TotSize );
				promotion::VIPJACKPOT_TOTAL_WAIT_ROOM_INFO* pRoomInfo=static_cast< promotion::VIPJACKPOT_TOTAL_WAIT_ROOM_INFO* >(Packet.Various() );
				int nChannel=GetLimitMoneyChannel(GM().GetCurrentGameType(), g_RI.llEnterLimitMoney, g_RI.BetMoney);
				bool bIsRoom=false;
				for(int i=0; i<MAX_TOP_VIP_JACKPOT_ROOM; i++){
					if(g_RI.RoomNum==pRoomInfo->stWaitRoomInfo[nChannel][i]){
						bIsRoom=true;
					}
				}
				if(true==bIsRoom){
					SetVipJackpotState(VIPJACKPOT_STATE_TIMEEND_RESERVE);
				}
				else{
					SetVipJackpotState(VIPJACKPOT_STATE_TIMEEND_RESERVE_NOT);
				}
				m_nSetTimer=m_nTimer=pRoomInfo->nRemainTimeSec*1000;
				if(0>m_nSetTimer) m_nSetTimer=0;
				if(0>m_nTimer) m_nTimer=0;
				if(m_nSetTimer<m_nTimer) m_nSetTimer=m_nTimer;
				ErrorLogError(NMBASE::SOCK::CSockMan::GetSocketTxt().c_str(), " OPCODE_SV_VIPJACKPOT_TOTAL_WAIT_ROOM_INFO=%d=%s", m_nSetTimer, bIsRoom?"true":"false");
			}
		}
		break;
	case OPCODE_SV_VIPJACKPOT_BET_POINT:         // 베팅 점수 (총 방의 점수와 현재 베팅 점수 모두 전송됨)
		{
			if(NULL!=lpData){
				packet::TSendPacket< promotion::VIPJACKPOT_BET_POINT > Packet;
				Packet.Get(lpData, TotSize );
				promotion::VIPJACKPOT_BET_POINT* pBetPoint=static_cast< promotion::VIPJACKPOT_BET_POINT* >(Packet.Various() );
				m_nRoomBetPoint=pBetPoint->nRoomJackpotPoint;
				m_nReserveBetPoint=pBetPoint->nMinRoomListBetPoint;
				if(0>m_nRoomBetPoint) m_nRoomBetPoint=0;
				if(0>m_nReserveBetPoint) m_nReserveBetPoint=0;
				bool bIsRoom=((m_nReserveBetPoint<=m_nRoomBetPoint)?(true):(false));
				if(0>=m_nRoomBetPoint) bIsRoom=false;
				if(VIPJACKPOT_STATE_TIMESTART==m_nVipJackpotState){
					SetVipJackpotState(VIPJACKPOT_STATE_TIMEING_RESERVE);
				}
				if((VIPJACKPOT_STATE_TIMEING_RESERVE==m_nVipJackpotState)||(VIPJACKPOT_STATE_TIMEING_RESERVE_NOT==m_nVipJackpotState)){
					if(true==bIsRoom){
						if(VIPJACKPOT_STATE_TIMEING_RESERVE!=m_nVipJackpotState){
							SetVipJackpotState(VIPJACKPOT_STATE_TIMEING_RESERVE, true);
						}
					}
					else{
						if(VIPJACKPOT_STATE_TIMEING_RESERVE_NOT!=m_nVipJackpotState){
							SetVipJackpotState(VIPJACKPOT_STATE_TIMEING_RESERVE_NOT);
						}
					}
				}
				AddVipJackpotBetPoint(pBetPoint->nNowBetPoint);
				ErrorLogError(NMBASE::SOCK::CSockMan::GetSocketTxt().c_str(), " OPCODE_SV_VIPJACKPOT_BET_POINT=%d=%d=%d=%s", m_nRoomBetPoint, m_nReserveBetPoint, pBetPoint->nNowBetPoint, bIsRoom?"true":"false");
				//ErrorLogError(NMBASE::SOCK::CSockMan::GetSocketTxt().c_str(), " OPCODE_SV_VIPJACKPOT_BET_POINT=%d=%d=%d", m_nRoomBetPoint, m_nReserveBetPoint, pBetPoint->nNowBetPoint);
			}
		}
		break;
	case OPCODE_SV_VIPJACKPOT_BETPOINT_ALARM:
		{
			if(NULL!=lpData){
				packet::TSendPacket< promotion::VIPJACKPOT_BET_POINT_ALARM > Packet;
				Packet.Get(lpData, TotSize );
				promotion::VIPJACKPOT_BET_POINT_ALARM* pAlaram=static_cast< promotion::VIPJACKPOT_BET_POINT_ALARM* >(Packet.Various() );
				int nChannel=GetLimitMoneyChannel(GM().GetCurrentGameType(), g_RI.llEnterLimitMoney, g_RI.BetMoney);
				bool bIsRoom=false;
				if(nChannel==pAlaram->iChannel){
					m_nReserveBetPoint=pAlaram->nTopHalfCount;
					bIsRoom=((m_nReserveBetPoint<=m_nRoomBetPoint)?(true):(false));
					if(0>=m_nRoomBetPoint) bIsRoom=false;
					if((VIPJACKPOT_STATE_TIMEING_RESERVE==m_nVipJackpotState)||(VIPJACKPOT_STATE_TIMEING_RESERVE_NOT==m_nVipJackpotState)){
						if(true==bIsRoom){
							if(VIPJACKPOT_STATE_TIMEING_RESERVE!=m_nVipJackpotState){
								SetVipJackpotState(VIPJACKPOT_STATE_TIMEING_RESERVE);
							}
						}
						else{
							if(VIPJACKPOT_STATE_TIMEING_RESERVE_NOT!=m_nVipJackpotState){
								SetVipJackpotState(VIPJACKPOT_STATE_TIMEING_RESERVE_NOT);
							}
						}
					}
				}
				ErrorLogError(NMBASE::SOCK::CSockMan::GetSocketTxt().c_str(), " OPCODE_SV_VIPJACKPOT_BETPOINT_ALARM=%d=%d=%d=%s=%s", m_nStage, pAlaram->nTopHalfCount, m_nRoomBetPoint, (nChannel==pAlaram->iChannel)?"true":"false", bIsRoom?"true":"false");
			}
		}
		break;
	case OPCODE_SV_VIPJACKPOT_ALARM:            // 잭팟 타임 동안 방 입장 유저에게 알림
		{
			if(NULL!=lpData){
				packet::TSendPacket< promotion::VIPJACKPOT_ALARM > Packet;
				Packet.Get(lpData, TotSize );
				promotion::VIPJACKPOT_ALARM* pAlaram=static_cast< promotion::VIPJACKPOT_ALARM* >(Packet.Various() );
				SetStage(pAlaram->nTodayEventCount);
				m_nRoomBetPoint=pAlaram->nHalfCount;
				m_nReserveBetPoint=pAlaram->nTopHalfCount;
				if(0>m_nRoomBetPoint) m_nRoomBetPoint=0;
				if(0>m_nReserveBetPoint) m_nReserveBetPoint=0;
				bool bIsRoom=((m_nReserveBetPoint<=m_nRoomBetPoint)?(true):(false));
				if(0>=m_nRoomBetPoint) bIsRoom=false;
				if((VIPJACKPOT_STATE_CHOOSE!=m_nVipJackpotState)&&(VIPJACKPOT_STATE_TIMEEND_RESERVE!=m_nVipJackpotState)&&(VIPJACKPOT_STATE_TIMEEND_RESERVE_NOT!=m_nVipJackpotState)){
					if(0==m_nSetTimer){
						m_nSetTimer=m_nTimer=pAlaram->nRemainTimeSec*1000;
					}
					else{
						m_nTimer=pAlaram->nRemainTimeSec*1000;
					}
					if(0>m_nSetTimer) m_nSetTimer=0;
					if(0>m_nTimer) m_nTimer=0;
				}
				if(m_nSetTimer<m_nTimer) m_nSetTimer=m_nTimer;
				SetVipJackpotServerState(pAlaram->nEventState, bIsRoom, pAlaram->bJackpotRoom, m_nTimer);
				if((VIPJACKPOT_STATE_TIMEING_RESERVE==m_nVipJackpotState)||(VIPJACKPOT_STATE_TIMEING_RESERVE_NOT==m_nVipJackpotState)){
					if(true==bIsRoom){
						SetVipJackpotState(VIPJACKPOT_STATE_TIMEING_RESERVE);
					}
					else{
						SetVipJackpotState(VIPJACKPOT_STATE_TIMEING_RESERVE_NOT);
					}
				}
				ErrorLogError(NMBASE::SOCK::CSockMan::GetSocketTxt().c_str(), " OPCODE_SV_VIPJACKPOT_ALARM=%d=%d=%d=%d=%d=%d=%s", m_nStage, pAlaram->nEventState, m_nSetTimer, pAlaram->nRemainTimeSec, pAlaram->nHalfCount, pAlaram->nTopHalfCount, bIsRoom?"true":"false");
			}
		}
		break;
	case OPCODE_SV_VIPJACKPOT_JACKPOT_ROUND_START:   // 잭팟방 시작
		{
			if(NULL!=lpData){
				packet::TSendPacket< promotion::VIPJACKPOT_ENTER_JACKPOT_ROOM > Packet;
				Packet.Get(lpData, TotSize );
				promotion::VIPJACKPOT_ENTER_JACKPOT_ROOM* pRoom=static_cast< promotion::VIPJACKPOT_ENTER_JACKPOT_ROOM* >(Packet.Various() );
				SetVipJackpotState(VIPJACKPOT_STATE_RESERVE_START);
				m_llVipJackpotMoney=pRoom->llPrizeMoney;
				ErrorLogError(NMBASE::SOCK::CSockMan::GetSocketTxt().c_str(), " OPCODE_SV_VIPJACKPOT_JACKPOT_ROUND_START=%I64d", m_llVipJackpotMoney);
			}
		}
		break;
	case OPCODE_SV_VIPJACKPOT_TOTAL_EVENT_SUCESS_ROOM_INFO:               // 잭팟방 선정
		{
			if(NULL!=lpData){
				packet::TSendPacket< promotion::VIPJACKPOT_TOTAL_EVENT_SUCESS_ROOM_INFO > Packet;
				Packet.Get(lpData, TotSize );
				promotion::VIPJACKPOT_TOTAL_EVENT_SUCESS_ROOM_INFO* pRoomInfo=static_cast< promotion::VIPJACKPOT_TOTAL_EVENT_SUCESS_ROOM_INFO* >(Packet.Various() );
				//int eType=GetIDXToGameType(GM().GetCurrentGameType());
				bool bIsRoom=false;
				m_sJackpotList.clear();
				for(int i=0; i<MAX_GAME_SU; i++){
					for(int j=0; j<promotion::VIPJACKPOT_CHANNEL_MAX; j++){
						if(0ll>=pRoomInfo->stEventSucessRoomInfo[i][j].llEnterLimitMoney) continue;
						for(int k=0; k<MAX_TOP_VIP_JACKPOT_ROOM; k++){
							if(0>=pRoomInfo->stEventSucessRoomInfo[i][j].iRoomNum[k]) continue;
							//if((eType==pRoomInfo->stEventSucessRoomInfo[i][j].nGameCode)&&(g_RI.llEnterLimitMoney==pRoomInfo->stEventSucessRoomInfo[i][j].llEnterLimitMoney)){
							if(g_RI.llEnterLimitMoney==pRoomInfo->stEventSucessRoomInfo[i][j].llEnterLimitMoney){
								if(g_RI.RoomNum==pRoomInfo->stEventSucessRoomInfo[i][j].iRoomNum[k]){
									bIsRoom=true;
								}
							}
							if(false==m_sJackpotList.empty()){
								m_sJackpotList+=" - ";
							}
							m_sJackpotList+=NMBASE::UTIL::format("%s[%d번방]", GetGameTypeString(pRoomInfo->stEventSucessRoomInfo[i][j].nGameCode[k]).c_str(), pRoomInfo->stEventSucessRoomInfo[i][j].iRoomNum[k]);
						}
					}
				}
				if(true==bIsRoom){
					SetVipJackpotState(VIPJACKPOT_STATE_CHOOSE_RESERVE);
				}
				else{
					SetVipJackpotState(VIPJACKPOT_STATE_CHOOSE_RESERVE_NOT);
				}
				ErrorLogError(NMBASE::SOCK::CSockMan::GetSocketTxt().c_str(), " OPCODE_SV_VIPJACKPOT_TOTAL_EVENT_SUCESS_ROOM_INFO=%s", bIsRoom?"true":"false");
			}
		}
		break;
	case OPCODE_SV_VIPJACKPOT_TOTAL_JACKPOT_EVENT_INFO:            // 잭팟판 종료&당첨자 정보
		{
			if(NULL!=lpData){
				packet::TSendPacket< promotion::VIPJACKPOT_TOTAL_JACKPOT_EVENT_INFO > Packet;
				Packet.Get(lpData, TotSize );
				promotion::VIPJACKPOT_TOTAL_JACKPOT_EVENT_INFO* pEventInfo=static_cast< promotion::VIPJACKPOT_TOTAL_JACKPOT_EVENT_INFO* >(Packet.Various() );
				m_sJackpotList="축하합니다! - ";
				for(int i=0; i<MAX_GAME_SU; i++){
					for(int j=0; j<promotion::VIPJACKPOT_CHANNEL_MAX; j++){
						for(int k=0; k<MAX_TOP_VIP_JACKPOT_ROOM; k++){
							if(0==strlen(pEventInfo->stPrizeInfo[i][j][k].szID)) continue;
							m_sJackpotList+=NMBASE::UTIL::format("%s님[%s,%s/%s방] - ", ChangeID(pEventInfo->stPrizeInfo[i][j][k].szID).c_str(), 
								GetJackpotMoneyString(pEventInfo->stPrizeInfo[i][j][k].llPrizeMoney).c_str(),
								GetGameTypeString(pEventInfo->stPrizeInfo[i][j][k].nGameCode).c_str(),
								GetChannelString(pEventInfo->stPrizeInfo[i][j][k].llEnterLimitMoney).c_str()
								);
							//if(pEventInfo->stPrizeInfo[i][j][k].nGameCode==GetIDXToGameType(GM().GetCurrentGameType())){
							//	if((g_RI.RoomNum==pEventInfo->stPrizeInfo[i][j][k].iRoomNum)&&(g_RI.llEnterLimitMoney==pEventInfo->stPrizeInfo[i][j][k].llEnterLimitMoney)){
							//		m_sWinNames+=NMBASE::UTIL::format("%s님,", pEventInfo->stPrizeInfo[i][j][k].szNick);
							//		llPrizeMoney+=pEventInfo->stPrizeInfo[i][j][k].llPrizeMoney;
							//		nPNum=GM().GetCurrentProcess()->GetPlayerPNum(pEventInfo->stPrizeInfo[i][j][k].szNick);
							//		m_cvtWinnerPos.push_back(nPNum);
							//	}
							//}
							// 	GAME_KIND_SPOKER2,			// 7포커
							// 	GAME_KIND_HILOW,			// 하이로우
							// 	GAME_KIND_BADUGI,			// 로우바둑이
							// 	GAME_KIND_SUDDA,			// 섯다
							// 	GAME_KIND_HOOLA,			// 훌라
							// 	GAME_KIND_NEWPOKER,			// 뉴포커
							// 	GAME_KIND_VSPOKER,			// 맞포커
						}
					}
				}
				if((VIPJACKPOT_STATE_RESERVE_NOT_RESERVELIST==m_nVipJackpotState)||(VIPJACKPOT_STATE_CHOOSE_RESERVE_NOT==m_nVipJackpotState)){
					SetVipJackpotState(VIPJACKPOT_STATE_RESERVE_NOT_RESERVEPRIZE);
				}
				ErrorLogError(NMBASE::SOCK::CSockMan::GetSocketTxt().c_str(), " OPCODE_SV_VIPJACKPOT_TOTAL_JACKPOT_EVENT_INFO");
			}
		}
		break;
	case OPCODE_SV_VIPJACKPOT_PRIZE:
		{
			if(NULL!=lpData){
				packet::TSendPacket< promotion::VIPJACKPOT_PRIZE > Packet;
				Packet.Get(lpData, TotSize );
				promotion::VIPJACKPOT_PRIZE* pPrize=static_cast< promotion::VIPJACKPOT_PRIZE* >(Packet.Various() );
				int nPNum=0;
				long long llPrizeMoney=0ll;
				if(pPrize->nGameCode==GetIDXToGameType(GM().GetCurrentGameType())){
					if((g_RI.RoomNum==pPrize->iRoomNum)&&(g_RI.llEnterLimitMoney==pPrize->llEnterLimitMoney)){
						m_sWinNames.clear();
						m_cvtWinnerPos.clear();
						m_cvtWinnerMoney.clear();
						for(int i=0; i<MAX_PLAYER; i++){
							if(0==strlen(pPrize->szID[i])) continue;
							m_sWinNames+=NMBASE::UTIL::format("%s님,", pPrize->szID[i]);
							nPNum=GM().GetCurrentProcess()->GetPlayerPNum(pPrize->szID[i]);
							m_cvtWinnerPos.push_back(nPNum);
							m_cvtWinnerMoney.push_back(pPrize->llPrizeMoney[i]);
							llPrizeMoney=pPrize->llPrizeMoney[i];
						}
						m_sWinMoney=GetJackpotMoneyString(llPrizeMoney);
						if((VIPJACKPOT_STATE_RESERVE_START==m_nVipJackpotState)||(VIPJACKPOT_STATE_CHOOSE_RESERVE==m_nVipJackpotState)){
							SetVipJackpotState(VIPJACKPOT_STATE_RESERVE_WINNER);
						}
					}
				}
				if(false==m_sWinNames.empty()){
					ErrorLogError(NMBASE::SOCK::CSockMan::GetSocketTxt().c_str(), " OPCODE_SV_VIPJACKPOT_PRIZE=%s=%s", m_sWinNames.c_str(), m_sWinMoney.c_str());
				}
				else{
					ErrorLogError(NMBASE::SOCK::CSockMan::GetSocketTxt().c_str(), " OPCODE_SV_VIPJACKPOT_PRIZE");
				}
			}
		}
		break;
	case OPCODE_SV_VIPJACKPOT_TOP_HALF_COUNT_MS:   // 서버별 후보방 정보 MS로 전달
		break;
	case OPCODE_SV_VIPJACKPOT_DELETE_ROOM_MS:      // 후보 방이 삭제됨
		break;
	case OPCODE_SV_VIPJACKPOT_END_HALF_MS:         // 해당 채널 하프:풀 카운팅 종료
		break;
	case OPCODE_SV_VIPJACKPOT_END:
		{

		}
		break;
	}
	return FALSE;
}

BOOL CVipJackpot::OnCommand(WPARAM wParam, LPARAM lParam )
{
	if(false==IsVIPJackpot(GM().GetCurrentGameType())) return FALSE;
	switch(wParam)
	{
	case IDM_VIPJACKPOTEXPLAIN:
		{
#if defined(_DEBUG)
			NextVipJackpotState();
#else
			m_bShowVipJackpotExplain=!m_bShowVipJackpotExplain;
#endif
		}
		break;
	case IDM_VIPJACKPOTBETEXPLAIN:
		{

#if defined(_DEBUG)
			AddVipJackpotBetPoint(rand()%100);
			SetVipJackpotNum(rand()%4);
#else
			m_bShowVipJackpotBetExplain=!m_bShowVipJackpotBetExplain;
#endif
		}
		break;
	default: return FALSE;
	}

	return TRUE;
}


BOOL CVipJackpot::OnLButtonDown(int x, int y)
{
	if(false==IsVIPJackpot(GM().GetCurrentGameType())) return FALSE;
	if(TRUE==m_btnVipJackpotExplain.OnLButtonDown(x,y)){
		return TRUE;
	}
	else{
		if((VIPJACKPOT_STATE_TIMEEND_PREV>=m_nVipJackpotState)&&(VIPJACKPOT_STATE_TIMESTART<=m_nVipJackpotState)){
			if(TRUE==m_btnVipJackpotBetExplain.OnLButtonDown(x,y)){
				return TRUE;
			}
		}
		if(true==m_bShowVipJackpotExplain){
			m_bShowVipJackpotExplain=false;
		}
		if(true==m_bShowVipJackpotBetExplain){
			m_bShowVipJackpotBetExplain=false;
		}
	}
	return FALSE;
}

BOOL CVipJackpot::OnLButtonUp(int x, int y)
{
	if(false==IsVIPJackpot(GM().GetCurrentGameType())) return FALSE;
	if((VIPJACKPOT_STATE_TIMEEND_PREV>=m_nVipJackpotState)&&(VIPJACKPOT_STATE_TIMESTART<=m_nVipJackpotState)){
		if(TRUE==m_btnVipJackpotBetExplain.OnLButtonUp(x,y)){
			return TRUE;
		}
	}
	return m_btnVipJackpotExplain.OnLButtonUp(x,y);
}

BOOL CVipJackpot::OnMouseMove(int x, int y)
{
	if(false==IsVIPJackpot(GM().GetCurrentGameType())) return FALSE;
	if((VIPJACKPOT_STATE_TIMEEND_PREV>=m_nVipJackpotState)&&(VIPJACKPOT_STATE_TIMESTART<=m_nVipJackpotState)){
		if(TRUE==m_btnVipJackpotBetExplain.OnMouseMove(x,y)){
			return TRUE;
		}
	}
	if(TRUE==m_btnVipJackpotExplain.OnMouseMove(x,y)){
		return TRUE;
	}
	m_bShowVipJackpotExplain=false;
	if(false==m_bShowVipJackpotExplain){
		CRect rc;		
		rc.SetRect(128, 3, 128+165, 3+26);
		POINT pt;
		pt.x=x;
		pt.y=y;
		if(rc.PtInRect(pt ) ){
			m_bShowVipJackpotExplain=true;
		}
	}
	m_bShowVipJackpotBetExplain=false;
	if(false==m_bShowVipJackpotExplain){
		if((VIPJACKPOT_STATE_TIMEEND_PREV>=m_nVipJackpotState)&&(VIPJACKPOT_STATE_NONE<m_nVipJackpotState)){
			CRect rc;		
			rc.SetRect(122, 37, 122+149, 37+54);
			POINT pt;
			pt.x=x;
			pt.y=y;
			if(rc.PtInRect(pt )){
				m_bShowVipJackpotBetExplain=true;
			}
		}
	}
	return FALSE;
}


void CVipJackpot::OnEnterRoom(void)
{
	if(false==IsVIPJackpot(GM().GetCurrentGameType())) return;
	m_bShowVipJackpotExplain=false;
	m_bShowVipJackpotBetExplain=false;
	if(false==IsVIPRoom(&g_RI)){
		SetVipJackpotState(VIPJACKPOT_STATE_NONE);
	}
}

void CVipJackpot::OnStartGame(void)
{
	if(false==IsVIPJackpot(GM().GetCurrentGameType())) return;
	//if(VIPJACKPOT_STATE_TIMESTARTNOTI==m_nVipJackpotState){
	//	SetVipJackpotState(VIPJACKPOT_STATE_TIMESTART);
	//}
	//if(VIPJACKPOT_STATE_CHOOSE_RESERVE==m_nVipJackpotState){
	//	SetVipJackpotState(VIPJACKPOT_STATE_RESERVE_START);
	//}
}

void CVipJackpot::OnBetGame(CPoint cPos, bool bAccent)
{
	if(false==IsVIPJackpot(GM().GetCurrentGameType())) return;
	m_cpBetPos=cPos;
	m_bBetAccent=bAccent;
}

void CVipJackpot::OnOutRoom(void)
{
	if(false==IsVIPJackpot(GM().GetCurrentGameType())) return;
	if((VIPJACKPOT_STATE_RESERVE_NOT_RESERVEPRIZE==m_nVipJackpotState)
		||(VIPJACKPOT_STATE_RESERVE_NOT_END==m_nVipJackpotState)
		||(VIPJACKPOT_STATE_RESERVE_WINNER==m_nVipJackpotState)
		||(VIPJACKPOT_STATE_RESERVE_END==m_nVipJackpotState)){
			m_nState=VIPJACKPOT_STAGE_STATE_END;
	}
	SetVipJackpotState(VIPJACKPOT_STATE_NONE);
}

void CVipJackpot::SetMininumMoney(char* pMininumMoney)
{
	m_sMininumMoney.clear();
	if(NULL==pMininumMoney) return;
	m_sMininumMoney=NMBASE::UTIL::format("%s", pMininumMoney);
}

void CVipJackpot::SetRuleType(char* pRuleType)
{
	m_sRuleType.clear();
	if(NULL==pRuleType) return;
	m_sRuleType=NMBASE::UTIL::format("%s", pRuleType);
}

void CVipJackpot::SetStage(int nStage)
{
	m_nStage=nStage;
	if(0>=m_nStage) m_nStage=1;
	if(4<m_nStage){
		m_nStage=4;
		m_nState=VIPJACKPOT_STAGE_STATE_END;
	}
	if(0==m_nStage){
		m_nState=VIPJACKPOT_STAGE_STATE_NONE;
	}
}

void CVipJackpot::SetStageState(int nStage, int nState)
{
	switch(nState){
	case promotion::VIPJACKPOT_BEFORE_START:
		{
			m_nState=VIPJACKPOT_STAGE_STATE_NONE;
		}
		break;
	case promotion::VIPJACKPOT_PARTY:
		{
			m_nState=VIPJACKPOT_STAGE_STATE_ING;
		}
		break;
	case promotion::VIPJACKPOT_BEFORE_END:
		{
			m_nState=VIPJACKPOT_STAGE_STATE_ING;
		}
		break;
	case promotion::VIPJACKPOT_END:
		{
			m_nState=VIPJACKPOT_STAGE_STATE_ING;
		}
		break;
	case promotion::VIPJACKPOT_WAIT_PULL_JACKPOT_ROOM:
		{
			m_nState=VIPJACKPOT_STAGE_STATE_ING;
		}
		break;
	case promotion::VIPJACKPOT_JACKPOT_GAME_PLAYING:
		{
			m_nState=VIPJACKPOT_STAGE_STATE_ING;
		}
		break;
	case promotion::VIPJACKPOT_NONE:
	default:
		{
			m_nState=VIPJACKPOT_STAGE_STATE_NONE;
		}
		break;
	}
	SetStage(nStage);
	ErrorLogError(NMBASE::SOCK::CSockMan::GetSocketTxt().c_str(), "SetStageState %d %d", m_nStage, m_nState);
}

void CVipJackpot::SetVipJackpotState(int nState, bool bClear)
{
	int nVipJackpotState=m_nVipJackpotState;
	m_nVipJackpotState=nState;
	if(false==IsVIPJackpot(GM().GetCurrentGameType())) return;
	m_nWinnerBackIndex=-1;
	m_dwWinnerBackTime=0;
	m_dwWinnerBackEndTime=0;
	m_nEffectIndex=-1;
	m_dwEffectTime=0;
	m_dwEffectEndTime=0;
	m_nEffectSound=0;
	m_nBetEffectIndex=-1;
	m_dwBetEffectTime=0;
	m_dwBetEffectEndTime=0;
	int nTimerEffectIndex=m_nTimerEffectIndex;
	DWORD dwTimerEffectTime=m_dwTimerEffectTime;
	DWORD dwTimerEffectEndTime=m_dwTimerEffectEndTime;
	DWORD dw5TimerEffectTime=m_dw5TimerEffectTime;
	m_nTimerEffectIndex=-1;
	m_dwTimerEffectTime=0;
	m_dwTimerEffectEndTime=0;
	m_dw5TimerEffectTime=0;
	m_n5TimerEffectSound=0;
	m_nStartEffectIndex=-1;
	m_dwStartEffectTime=0;
	m_dwStartEffectEndTime=0;
	m_cvtWinnerEffectIndex.clear();
	m_dwWinnerEffectTime=0;
	m_dwWinnerEffectEndTime=0;
	switch(m_nVipJackpotState){
	case VIPJACKPOT_STATE_NONE:
		{
			if(true==bClear){
				m_nState=VIPJACKPOT_STAGE_STATE_NONE;
			}
			m_nRoomBetPoint=0;
			m_nReserveBetPoint=0;
			if((true==IsVipJackpotGame())&&(true==IsVIPRoom(&g_RI))){
				GM().SetNotBanishment(false);
			}
			m_sJackpotList.clear();
		}
		break;
	case VIPJACKPOT_STATE_CLEAR:
		{
			m_nState=VIPJACKPOT_STAGE_STATE_NONE;
			SetStage(m_nStage+1);
			SetVipJackpotState(VIPJACKPOT_STATE_NONE, false);
		}
		break;
	case VIPJACKPOT_STATE_TIMENOTICE:
		{
			SetVipJackpotNum(m_nStage-1);
			m_nRoomBetPoint=0;
			m_nReserveBetPoint=0;
			m_nEffectIndex=0;
			m_dwEffectTime=::timeGetTime();
			m_dwEffectEndTime=m_sprVipJackpotEffect.GetMScenePlayTime(m_nEffectIndex);
			PlaySound(SND_JP_NOTI);
			//PlaySound(SND_JP_FX);
		}
		break;
	case VIPJACKPOT_STATE_TIMESTARTNOTI:
		{
			SetVipJackpotNum(m_nStage-1);
			//m_nSetTimer=m_nTimer=30*60*1000;
			m_nTimerEffectIndex=19;
			m_dwTimerEffectTime=::timeGetTime();
			m_dwTimerEffectEndTime=m_sprVipJackpotEffect.GetMScenePlayTime(m_nTimerEffectIndex);
			m_nEffectIndex=3;
			m_dwEffectTime=::timeGetTime();
			m_dwEffectEndTime=m_sprVipJackpotEffect.GetMScenePlayTime(m_nEffectIndex);
			m_nState=VIPJACKPOT_STAGE_STATE_ING;
			//PlaySound(SND_JP_FX);
			PlaySound(SND_JP_START1_BGM);
			PlaySound(SND_JP_START1_FX);
			m_n5TimerEffectSound=1;
		}
		break;
	case VIPJACKPOT_STATE_TIMESTART:
		{
			SetVipJackpotNum(m_nStage-1);
			if(-1!=nTimerEffectIndex){
				m_nTimerEffectIndex=nTimerEffectIndex;
				m_dwTimerEffectTime=dwTimerEffectTime;
				m_dwTimerEffectEndTime=dwTimerEffectEndTime;
			}
			else{
				m_nTimerEffectIndex=19;
				m_dwTimerEffectTime=::timeGetTime();
				m_dwTimerEffectEndTime=m_sprVipJackpotEffect.GetMScenePlayTime(m_nTimerEffectIndex);
			}
			m_nState=VIPJACKPOT_STAGE_STATE_ING;
			m_n5TimerEffectSound=1;
			PlaySound(SND_JP_FX);
		}
		break;
	case VIPJACKPOT_STATE_TIMEING_RESERVE_NOT:
		{
			SetVipJackpotNum(m_nStage-1);
			if(-1!=nTimerEffectIndex){
				m_nTimerEffectIndex=nTimerEffectIndex;
				m_dwTimerEffectTime=dwTimerEffectTime;
				m_dwTimerEffectEndTime=dwTimerEffectEndTime;
			}
			else{
				m_nTimerEffectIndex=19;
				m_dwTimerEffectTime=::timeGetTime();
				m_dwTimerEffectEndTime=m_sprVipJackpotEffect.GetMScenePlayTime(m_nTimerEffectIndex);
			}
			m_nState=VIPJACKPOT_STAGE_STATE_ING;
			m_n5TimerEffectSound=1;
			PlaySound(SND_JP_FX);
		}
		break;
	case VIPJACKPOT_STATE_TIMEING_RESERVE:
		{
			SetVipJackpotNum(m_nStage-1);
			if(false==bClear){
				m_nEffectIndex=4;
				m_dwEffectTime=::timeGetTime();
				m_dwEffectEndTime=m_sprVipJackpotEffect.GetMScenePlayTime(m_nEffectIndex);
			}
			else{
				m_nEffectIndex=5;
				m_dwEffectTime=::timeGetTime();
				m_dwEffectEndTime=0;
			}
			if(-1!=nTimerEffectIndex){
				m_nTimerEffectIndex=nTimerEffectIndex;
				m_dwTimerEffectTime=dwTimerEffectTime;
				m_dwTimerEffectEndTime=dwTimerEffectEndTime;
			}
			else{
				m_nTimerEffectIndex=19;
				m_dwTimerEffectTime=::timeGetTime();
				m_dwTimerEffectEndTime=m_sprVipJackpotEffect.GetMScenePlayTime(m_nTimerEffectIndex);
			}
			m_nState=VIPJACKPOT_STAGE_STATE_ING;
			m_nEffectSound=1;
			PlaySound(SND_JP_FX);
			PlaySound(SND_JP_NOMI1_2);
			m_n5TimerEffectSound=1;
		}
		break;
	case VIPJACKPOT_STATE_TIMEEND_PREV:
		{
			SetVipJackpotNum(m_nStage-1);
			if(-1!=nTimerEffectIndex){
				m_nTimerEffectIndex=nTimerEffectIndex;
				m_dwTimerEffectTime=dwTimerEffectTime;
				m_dwTimerEffectEndTime=dwTimerEffectEndTime;
			}
			else{
				m_nTimerEffectIndex=19;
				m_dwTimerEffectTime=::timeGetTime();
				m_dwTimerEffectEndTime=m_sprVipJackpotEffect.GetMScenePlayTime(m_nTimerEffectIndex);
			}
			m_nState=VIPJACKPOT_STAGE_STATE_ING;
			m_n5TimerEffectSound=1;
			if((true==IsVipJackpotGame())&&(true==IsVIPRoom(&g_RI))){
				GM().SetNotBanishment(true);
			}
			PlaySound(SND_JP_FX);
		}
		break;
	case VIPJACKPOT_STATE_TIMEEND_RESERVE_NOT:
		{
			SetVipJackpotNum(m_nStage-1);
			//m_nSetTimer=m_nTimer=3*60*1000;
			m_nEffectIndex=7;
			m_dwEffectTime=::timeGetTime();
			m_dwEffectEndTime=m_sprVipJackpotEffect.GetMScenePlayTime(m_nEffectIndex);
			m_nTimerEffectIndex=18;
			m_dwTimerEffectTime=::timeGetTime();
			m_dwTimerEffectEndTime=m_sprVipJackpotEffect.GetMScenePlayTime(m_nTimerEffectIndex);
			m_nState=VIPJACKPOT_STAGE_STATE_ING;
			PlaySound(SND_JP_FX);
		}
		break;
	case VIPJACKPOT_STATE_TIMEEND_RESERVE:
		{
			SetVipJackpotNum(m_nStage-1);
			//m_nSetTimer=m_nTimer=3*60*1000;
			m_nEffectIndex=6;
			m_dwEffectTime=::timeGetTime();
			m_dwEffectEndTime=m_sprVipJackpotEffect.GetMScenePlayTime(m_nEffectIndex);
			m_nTimerEffectIndex=18;
			m_dwTimerEffectTime=::timeGetTime();
			m_dwTimerEffectEndTime=m_sprVipJackpotEffect.GetMScenePlayTime(m_nTimerEffectIndex);
			m_nState=VIPJACKPOT_STAGE_STATE_ING;
			m_nEffectSound=4;
			PlaySound(SND_JP_FX);
			PlaySound(SND_JP_NOMI2);
		}
		break;
	case VIPJACKPOT_STATE_CHOOSE:
		{
			SetVipJackpotNum(m_nStage-1);
			if(-1!=nTimerEffectIndex){
				m_nTimerEffectIndex=nTimerEffectIndex;
				m_dwTimerEffectTime=dwTimerEffectTime;
				m_dwTimerEffectEndTime=dwTimerEffectEndTime;
			}
			else{
				m_nTimerEffectIndex=18;
				m_dwTimerEffectTime=::timeGetTime();
				m_dwTimerEffectEndTime=m_sprVipJackpotEffect.GetMScenePlayTime(m_nTimerEffectIndex);
			}
			m_nState=VIPJACKPOT_STAGE_STATE_ING;
			PlaySound(SND_JP_FX);
		}
		break;
	case VIPJACKPOT_STATE_CHOOSE_RESERVE_NOT:
		{
			SetVipJackpotNum(m_nStage-1);
			m_nTimerEffectIndex=17;
			m_dwTimerEffectTime=::timeGetTime();
			m_dwTimerEffectEndTime=m_sprVipJackpotEffect.GetMScenePlayTime(m_nTimerEffectIndex);
			m_nState=VIPJACKPOT_STAGE_STATE_ING;
			PlaySound(SND_JP_FX);
		}
		break;
	case VIPJACKPOT_STATE_RESERVE_NOT_RESERVELIST:
		{
			SetVipJackpotNum(m_nStage-1);
			if(NULL!=m_pTxtScroll_List){
				m_pTxtScroll_List->SetText((char*)m_sJackpotList.c_str());
				m_pTxtScroll_List->StartScroll(0, TRUE);
			}
			m_nState=VIPJACKPOT_STAGE_STATE_ING;
			PlaySound(SND_JP_FX);
		}
		break;
	case VIPJACKPOT_STATE_RESERVE_NOT_RESERVEPRIZE:
		{
			SetVipJackpotNum(m_nStage-1);
			if(NULL!=m_pTxtScroll_List){
				m_pTxtScroll_List->SetText((char*)m_sJackpotList.c_str());
				m_pTxtScroll_List->StartScroll(2, FALSE);
			}
			m_nState=VIPJACKPOT_STAGE_STATE_END;
			PlaySound(SND_JP_FX);
		}
		break;
	case VIPJACKPOT_STATE_RESERVE_NOT_END:
		{
			SetVipJackpotNum(m_nStage-1);
			m_nTimerEffectIndex=17;
			m_dwTimerEffectTime=::timeGetTime();
			m_dwTimerEffectEndTime=m_sprVipJackpotEffect.GetMScenePlayTime(m_nTimerEffectIndex);
			m_nState=VIPJACKPOT_STAGE_STATE_END;
			PlaySound(SND_JP_FX);
		}
		break;
	case VIPJACKPOT_STATE_CHOOSE_RESERVE:
		{
			SetVipJackpotNum(m_nStage-1);
			m_nStartEffectIndex=8;
			m_dwStartEffectTime=::timeGetTime();
			m_dwStartEffectEndTime=m_sprVipJackpotEffect.GetMScenePlayTime(m_nStartEffectIndex);
			m_nState=VIPJACKPOT_STAGE_STATE_ING;
			//PlaySound(SND_JP_FX);
			PlaySound(SND_JP_SELECT);
		}
		break;
	case VIPJACKPOT_STATE_RESERVE_START:
		{
			SetVipJackpotNum(m_nStage-1);
			m_nStartEffectIndex=9;
			m_dwStartEffectTime=::timeGetTime();
			m_dwStartEffectEndTime=m_sprVipJackpotEffect.GetMScenePlayTime(m_nStartEffectIndex);
			m_nState=VIPJACKPOT_STAGE_STATE_ING;
			//PlaySound(SND_JP_FX);
			PlaySound(SND_JP_START2_FX);
			PlaySound(SND_JP_START2_BGM);
		}
		break;
	case VIPJACKPOT_STATE_RESERVE_WINNER:
		{
			SetVipJackpotNum(m_nStage-1);
			m_nSetTimer=m_nTimer=3*1000;
			m_nWinnerBackIndex=19;
			m_dwWinnerBackTime=::timeGetTime();
			if(1<m_cvtWinnerPos.size()){
				m_sprVipJackpotBack.multiscene.ppMScene[m_nWinnerBackIndex]->ppTrack[0]->GetDataScene()->SceneNo=44+m_nStage-1;
			}
			else{
				m_sprVipJackpotBack.multiscene.ppMScene[m_nWinnerBackIndex]->ppTrack[0]->GetDataScene()->SceneNo=40+m_nStage-1;
			}
			m_dwWinnerBackEndTime=m_sprVipJackpotBack.GetMScenePlayTime(m_nWinnerBackIndex);
			m_nState=VIPJACKPOT_STAGE_STATE_END;
			m_nEffectIndex=11;
			m_dwEffectTime=::timeGetTime();
			m_dwEffectEndTime=m_sprVipJackpotEffect.GetMScenePlayTime(m_nEffectIndex);
			//PlaySound(SND_JP_FX);
			PlaySound(SND_JP_WIN_BGM);
		}
		break;
	case VIPJACKPOT_STATE_RESERVE_END:
		{
			SetVipJackpotNum(m_nStage-1);
			m_nTimerEffectIndex=17;
			m_dwTimerEffectTime=::timeGetTime();
			m_dwTimerEffectEndTime=m_sprVipJackpotEffect.GetMScenePlayTime(m_nTimerEffectIndex);
			m_nState=VIPJACKPOT_STAGE_STATE_END;
			if((true==IsVipJackpotGame())&&(true==IsVIPRoom(&g_RI))){
				GM().SetNotBanishment(false);
			}
			PlaySound(SND_JP_FX);
		}
		break;
	}
	ErrorLogError(NMBASE::SOCK::CSockMan::GetSocketTxt().c_str(), StringToVipJackpotState(m_nVipJackpotState).c_str());	
}

void CVipJackpot::SetVipJackpotServerState(int nEventState, bool bIsRoom, bool bJackpotRoom, int nSecond)
{
	//if(VIPJACKPOT_STATE_NONE==m_nVipJackpotState)
	{
		switch(nEventState){
		case promotion::VIPJACKPOT_BEFORE_START:
			{
				SetVipJackpotState(VIPJACKPOT_STATE_TIMENOTICE);
				ErrorLogError(NMBASE::SOCK::CSockMan::GetSocketTxt().c_str(), "VIPJACKPOT_BEFORE_START");
			}
			break;
		case promotion::VIPJACKPOT_PARTY:
			{
				if(true==bIsRoom){
					SetVipJackpotState(VIPJACKPOT_STATE_TIMEING_RESERVE);
				}
				else{
					SetVipJackpotState(VIPJACKPOT_STATE_TIMEING_RESERVE_NOT);
				}
				ErrorLogError(NMBASE::SOCK::CSockMan::GetSocketTxt().c_str(), "VIPJACKPOT_BEFORE_START %s", bIsRoom?"true":"false");
			}
			break;
		case promotion::VIPJACKPOT_BEFORE_END:
			{
				SetVipJackpotState(VIPJACKPOT_STATE_TIMEEND_PREV);
				ErrorLogError(NMBASE::SOCK::CSockMan::GetSocketTxt().c_str(), "VIPJACKPOT_BEFORE_END");
			}
			break;
		case promotion::VIPJACKPOT_END:
			{
				ErrorLogError(NMBASE::SOCK::CSockMan::GetSocketTxt().c_str(), "VIPJACKPOT_BEFORE_END");
			}
			break;
		case promotion::VIPJACKPOT_WAIT_PULL_JACKPOT_ROOM:
			{
				if(5<nSecond){
					if(true==bIsRoom){
						if(VIPJACKPOT_STATE_TIMEEND_RESERVE!=m_nVipJackpotState){
							SetVipJackpotState(VIPJACKPOT_STATE_TIMEEND_RESERVE);
						}
					}
					else{
						if(VIPJACKPOT_STATE_TIMEEND_RESERVE_NOT!=m_nVipJackpotState){
							SetVipJackpotState(VIPJACKPOT_STATE_TIMEEND_RESERVE_NOT);
						}
					}
				}
				else{
					SetVipJackpotState(VIPJACKPOT_STATE_CHOOSE);
				}
				ErrorLogError(NMBASE::SOCK::CSockMan::GetSocketTxt().c_str(), "VIPJACKPOT_WAIT_PULL_JACKPOT_ROOM %s", bIsRoom?"true":"false");
			}
			break;
		case promotion::VIPJACKPOT_JACKPOT_GAME_PLAYING:
			{
				if(true==bJackpotRoom){
					SetVipJackpotState(VIPJACKPOT_STATE_CHOOSE_RESERVE);
				}
				else{
					SetVipJackpotState(VIPJACKPOT_STATE_CHOOSE_RESERVE_NOT);
				}
				ErrorLogError(NMBASE::SOCK::CSockMan::GetSocketTxt().c_str(), "VIPJACKPOT_JACKPOT_GAME_PLAYING %s", bIsRoom?"true":"false");
			}
			break;
		case promotion::VIPJACKPOT_NONE:
			{
				SetVipJackpotState(VIPJACKPOT_STATE_NONE, true);
			}
			break;
		}
	}
}

void CVipJackpot::NextVipJackpotState(void)
{
	int nArrayState[]={
		VIPJACKPOT_STATE_TIMENOTICE, VIPJACKPOT_STATE_TIMESTARTNOTI, VIPJACKPOT_STATE_TIMESTART, VIPJACKPOT_STATE_TIMEING_RESERVE_NOT, VIPJACKPOT_STATE_TIMEING_RESERVE,
		VIPJACKPOT_STATE_TIMEEND_PREV, VIPJACKPOT_STATE_TIMEEND_RESERVE_NOT, VIPJACKPOT_STATE_TIMEEND_RESERVE, VIPJACKPOT_STATE_CHOOSE, VIPJACKPOT_STATE_CHOOSE_RESERVE_NOT,
		VIPJACKPOT_STATE_RESERVE_NOT_RESERVELIST, VIPJACKPOT_STATE_RESERVE_NOT_RESERVEPRIZE, VIPJACKPOT_STATE_RESERVE_NOT_END, VIPJACKPOT_STATE_CHOOSE_RESERVE, VIPJACKPOT_STATE_RESERVE_START,
		VIPJACKPOT_STATE_RESERVE_WINNER, VIPJACKPOT_STATE_RESERVE_END};
		int nTotal=sizeof(nArrayState)/sizeof(int);
		bool bFind=false;
		for(int i=0; i<nTotal; i++){
			if(m_nVipJackpotState==nArrayState[i]){
				if((nTotal-1)==i){
					SetVipJackpotState(nArrayState[0]);
				}
				else{
					SetVipJackpotState(nArrayState[i+1]);
				}
				bFind=true;
				break;
			}
		}
		if(false==bFind){
			SetVipJackpotState(nArrayState[0]);
		}
}

void CVipJackpot::AddVipJackpotBetPoint(int nPoint)
{
	if(0>=nPoint) return;
	int nMScene=2;
	if(10>nPoint){
		nMScene=1;
	}
	int nPoint10=nPoint/10;
	int nPoint01=nPoint%10;
	ErrorLogConsole("AddVipJackpotBetPoint =%d=%d", nPoint, nMScene);
	if(2==nMScene){
		NMBASE::GRAPHICGDI::TRACK_SCENE* pScnData1=(NMBASE::GRAPHICGDI::TRACK_SCENE*)m_sprVipJackpotEffect.multiscene.ppMScene[nMScene]->ppTrack[1]->GetDataScene();
		NMBASE::GRAPHICGDI::TRACK_SCENE* pScnData2=(NMBASE::GRAPHICGDI::TRACK_SCENE*)m_sprVipJackpotEffect.multiscene.ppMScene[nMScene]->ppTrack[2]->GetDataScene();
		if(NULL!=pScnData1){
			pScnData1->SceneNo=33+nPoint10;
		}
		if(NULL!=pScnData2){
			pScnData2->SceneNo=33+nPoint01;
		}
		ErrorLogConsole("AddVipJackpotBetPoint %d=%d=%d=%d", nPoint10, nPoint01, pScnData1->SceneNo, pScnData2->SceneNo);
	}
	else{
		NMBASE::GRAPHICGDI::TRACK_SCENE* pScnData=(NMBASE::GRAPHICGDI::TRACK_SCENE*)m_sprVipJackpotEffect.multiscene.ppMScene[nMScene]->ppTrack[1]->GetDataScene();
		if(NULL!=pScnData){
			pScnData->SceneNo=33+nPoint01;
		}
		ErrorLogConsole("AddVipJackpotBetPoint=%d=%d", nPoint01, pScnData->SceneNo);
	}
	m_nBetEffectIndex=nMScene;
	m_dwBetEffectTime=::timeGetTime();
	m_dwBetEffectEndTime=m_sprVipJackpotEffect.GetMScenePlayTime(m_nBetEffectIndex);
	MakeVIPJackpotPosPoint(nPoint);
}

void CVipJackpot::MakeVIPJackpotPosPoint(int nPoint)
{
	if(0>=nPoint) return;
	int nMScene=0;
	if(10>nPoint){
		nMScene=1;
	}
	int nPoint10=(nPoint/10)%10;
	int nPoint01=nPoint%10;
	ErrorLogConsole("MakeVIPJackpotPoint=%d=%d", nPoint, nMScene);
	if(0==nMScene){
		NMBASE::GRAPHICGDI::TRACK_SCENE* pScnDataP=(NMBASE::GRAPHICGDI::TRACK_SCENE*)m_sprVipJackpotBet.multiscene.ppMScene[nMScene]->ppTrack[1]->GetDataScene();
		NMBASE::GRAPHICGDI::TRACK_SCENE* pScnData1=(NMBASE::GRAPHICGDI::TRACK_SCENE*)m_sprVipJackpotBet.multiscene.ppMScene[nMScene]->ppTrack[2]->GetDataScene();
		NMBASE::GRAPHICGDI::TRACK_SCENE* pScnData2=(NMBASE::GRAPHICGDI::TRACK_SCENE*)m_sprVipJackpotBet.multiscene.ppMScene[nMScene]->ppTrack[3]->GetDataScene();
		if(NULL!=pScnDataP){
			pScnDataP->SceneNo=((true==m_bBetAccent)?(3):(2));
		}
		if(NULL!=pScnData1){
			pScnData1->SceneNo=((true==m_bBetAccent)?(14):(4))+nPoint10;
		}
		if(NULL!=pScnData2){
			pScnData2->SceneNo=((true==m_bBetAccent)?(14):(4))+nPoint01;
		}
		ErrorLogConsole("MakeVIPJackpotPoint2=%d=%d=%d=%d=%s", nPoint10, nPoint01, pScnData1->SceneNo, pScnData2->SceneNo, m_bBetAccent?"true":"false");
	}
	else{
		NMBASE::GRAPHICGDI::TRACK_SCENE* pScnDataP=(NMBASE::GRAPHICGDI::TRACK_SCENE*)m_sprVipJackpotBet.multiscene.ppMScene[nMScene]->ppTrack[1]->GetDataScene();
		NMBASE::GRAPHICGDI::TRACK_SCENE* pScnData=(NMBASE::GRAPHICGDI::TRACK_SCENE*)m_sprVipJackpotBet.multiscene.ppMScene[nMScene]->ppTrack[2]->GetDataScene();
		if(NULL!=pScnDataP){
			pScnDataP->SceneNo=((true==m_bBetAccent)?(3):(2));
		}
		if(NULL!=pScnData){
			pScnData->SceneNo=((true==m_bBetAccent)?(14):(4))+nPoint01;
		}
		ErrorLogConsole("MakeVIPJackpotPoint1=%d=%d=%s", nPoint01, pScnData->SceneNo, m_bBetAccent?"true":"false");
	}
	m_nBetPosEffectIndex=nMScene;
	m_dwBetPosEffectTime=::timeGetTime();
	m_dwBetPosEffectEndTime=m_sprVipJackpotBet.GetMScenePlayTime(m_nBetPosEffectIndex);
	m_bBetAccent=false;
}

void CVipJackpot::SetVipJackpotNum(int nNum)
{
	if(0>nNum) nNum=0;
	if(3<nNum) nNum=3;
	int nTotal=m_sprVipJackpotBack.multiscene.TotMSceneNum;
	NMBASE::GRAPHICGDI::TRACK_SCENE* pScnData=NULL; 
	for(int i=0; i<nTotal; i++){
		for(int j=0; j<m_sprVipJackpotBack.multiscene.ppMScene[i]->TotTrackNum; j++){
			if(NULL==m_sprVipJackpotBack.multiscene.ppMScene[i]->ppTrack[j]->GetDataScene()) continue;
			if((3<=m_sprVipJackpotBack.multiscene.ppMScene[i]->ppTrack[j]->GetDataScene()->SceneNo)&&(6>=m_sprVipJackpotBack.multiscene.ppMScene[i]->ppTrack[j]->GetDataScene()->SceneNo)){
				m_sprVipJackpotBack.multiscene.ppMScene[i]->ppTrack[j]->GetDataScene()->SceneNo=3+nNum;
			}
			if((8<=m_sprVipJackpotBack.multiscene.ppMScene[i]->ppTrack[j]->GetDataScene()->SceneNo)&&(11>=m_sprVipJackpotBack.multiscene.ppMScene[i]->ppTrack[j]->GetDataScene()->SceneNo)){
				m_sprVipJackpotBack.multiscene.ppMScene[i]->ppTrack[j]->GetDataScene()->SceneNo=8+nNum;
			}
			if((24<=m_sprVipJackpotBack.multiscene.ppMScene[i]->ppTrack[j]->GetDataScene()->SceneNo)&&(27>=m_sprVipJackpotBack.multiscene.ppMScene[i]->ppTrack[j]->GetDataScene()->SceneNo)){
				m_sprVipJackpotBack.multiscene.ppMScene[i]->ppTrack[j]->GetDataScene()->SceneNo=24+nNum;
			}
			if((29<=m_sprVipJackpotBack.multiscene.ppMScene[i]->ppTrack[j]->GetDataScene()->SceneNo)&&(32>=m_sprVipJackpotBack.multiscene.ppMScene[i]->ppTrack[j]->GetDataScene()->SceneNo)){
				m_sprVipJackpotBack.multiscene.ppMScene[i]->ppTrack[j]->GetDataScene()->SceneNo=29+nNum;
			}
			if((36<=m_sprVipJackpotBack.multiscene.ppMScene[i]->ppTrack[j]->GetDataScene()->SceneNo)&&(39>=m_sprVipJackpotBack.multiscene.ppMScene[i]->ppTrack[j]->GetDataScene()->SceneNo)){
				m_sprVipJackpotBack.multiscene.ppMScene[i]->ppTrack[j]->GetDataScene()->SceneNo=36+nNum;
			}
			if((40<=m_sprVipJackpotBack.multiscene.ppMScene[i]->ppTrack[j]->GetDataScene()->SceneNo)&&(43>=m_sprVipJackpotBack.multiscene.ppMScene[i]->ppTrack[j]->GetDataScene()->SceneNo)){
				m_sprVipJackpotBack.multiscene.ppMScene[i]->ppTrack[j]->GetDataScene()->SceneNo=40+nNum;
			}
			if((44<=m_sprVipJackpotBack.multiscene.ppMScene[i]->ppTrack[j]->GetDataScene()->SceneNo)&&(47>=m_sprVipJackpotBack.multiscene.ppMScene[i]->ppTrack[j]->GetDataScene()->SceneNo)){
				m_sprVipJackpotBack.multiscene.ppMScene[i]->ppTrack[j]->GetDataScene()->SceneNo=44+nNum;
			}
		}
	}
	nTotal=m_sprVipJackpotEffect.multiscene.TotMSceneNum;
	for(int i=0; i<nTotal; i++){
		for(int j=0; j<m_sprVipJackpotEffect.multiscene.ppMScene[i]->TotTrackNum; j++){
			if(NULL==m_sprVipJackpotEffect.multiscene.ppMScene[i]->ppTrack[j]->GetDataScene()) continue;
			if((70<=m_sprVipJackpotEffect.multiscene.ppMScene[i]->ppTrack[j]->GetDataScene()->SceneNo)&&(73>=m_sprVipJackpotEffect.multiscene.ppMScene[i]->ppTrack[j]->GetDataScene()->SceneNo)){
				m_sprVipJackpotEffect.multiscene.ppMScene[i]->ppTrack[j]->GetDataScene()->SceneNo=70+nNum;
			}
		}
	}
}


string CVipJackpot::StringToVipJackpotState(int nState)
{
	switch(nState){
	case VIPJACKPOT_STATE_NONE: return "VIPJACKPOT_STATE_NONE";
	case VIPJACKPOT_STATE_TIMENOTICE: return "VIPJACKPOT_STATE_TIMENOTICE";
	case VIPJACKPOT_STATE_TIMESTARTNOTI: return "VIPJACKPOT_STATE_TIMESTARTNOTI";
	case VIPJACKPOT_STATE_TIMESTART: return "VIPJACKPOT_STATE_TIMESTART";
	case VIPJACKPOT_STATE_TIMEING_RESERVE_NOT: return "VIPJACKPOT_STATE_TIMEING_RESERVE_NOT";
	case VIPJACKPOT_STATE_TIMEING_RESERVE: return "VIPJACKPOT_STATE_TIMEING_RESERVE";
	case VIPJACKPOT_STATE_TIMEEND_PREV: return "VIPJACKPOT_STATE_TIMEEND_PREV";
	case VIPJACKPOT_STATE_TIMEEND_RESERVE_NOT: return "VIPJACKPOT_STATE_TIMEEND_RESERVE_NOT";
	case VIPJACKPOT_STATE_TIMEEND_RESERVE: return "VIPJACKPOT_STATE_TIMEEND_RESERVE";
	case VIPJACKPOT_STATE_CHOOSE: return "VIPJACKPOT_STATE_CHOOSE";
	case VIPJACKPOT_STATE_CHOOSE_RESERVE_NOT: return "VIPJACKPOT_STATE_CHOOSE_RESERVE_NOT";
	case VIPJACKPOT_STATE_RESERVE_NOT_RESERVELIST: return "VIPJACKPOT_STATE_RESERVE_NOT_RESERVELIST";
	case VIPJACKPOT_STATE_RESERVE_NOT_RESERVEPRIZE: return "VIPJACKPOT_STATE_RESERVE_NOT_RESERVEPRIZE";
	case VIPJACKPOT_STATE_RESERVE_NOT_END: return "VIPJACKPOT_STATE_RESERVE_NOT_END";
	case VIPJACKPOT_STATE_CHOOSE_RESERVE: return "VIPJACKPOT_STATE_CHOOSE_RESERVE";
	case VIPJACKPOT_STATE_RESERVE_START: return "VIPJACKPOT_STATE_RESERVE_START";
	case VIPJACKPOT_STATE_RESERVE_WINNER: return "VIPJACKPOT_STATE_RESERVE_WINNER";
	case VIPJACKPOT_STATE_RESERVE_END: return "VIPJACKPOT_STATE_RESERVE_END";
	}
	return "VIPJACKPOT_STATE_NONE";
}

string CVipJackpot::GetGameTypeString(int eGame)
{
	switch(eGame){
	case jackpot::GAME_7POKER: return "7포커";
	case jackpot::GAME_LOWBD: return "로우바둑이";
	case jackpot::GAME_NEWPOKER: return "뉴포커";
	case jackpot::GAME_HIGHLOW: return "하이로우";
		//case jackpot::GAME_SUTDA: return IDX_GAME_SD;
	case jackpot::GAME_HOOLA: return "훌라";
	}
	return "None";
}

string CVipJackpot::GetChannelString(long long llLimitMoney)
{
	return (string)NMBASE::UTIL::g_NumberToHangul(llLimitMoney);

}

string CVipJackpot::GetJackpotMoneyString(long long llJackpotMoney)
{
	return (string)NMBASE::UTIL::g_MoneyMark(llJackpotMoney, 2);
}

string CVipJackpot::ChangeID(char* pID)
{
	if(NULL==pID) return "";
	int nSize=strlen(pID);
	if(3>=nSize) return "...";
	int nFind=0;
	for(int i=0; i<3; i++){
		if(IsDBCSLeadByte(pID[nSize-1-nFind])){
			nFind+=2;
		}
		else{
			nFind+=1;
		}
	}
	if(nFind>=nSize) return "...";
	string sChange;
	sChange.insert(0, pID, nSize-nFind);
	sChange+="...";
	return sChange;
}

int CVipJackpot::GetIDXToGameType(IDX_GAME eGame)
{
	switch(eGame){
	case IDX_GAME_SP: return jackpot::GAME_7POKER;
	case IDX_GAME_BD: return jackpot::GAME_LOWBD;
	case IDX_GAME_NP: return jackpot::GAME_NEWPOKER;
	case IDX_GAME_HL: return jackpot::GAME_HIGHLOW;
		//case IDX_GAME_SD: return jackpot::GAME_SUTDA;
	case IDX_GAME_HA: return jackpot::GAME_HOOLA;
	}
	return jackpot::GAME_UNKNOWN;
}

IDX_GAME CVipJackpot::GetIDXFromGameType(int eGame)
{
	switch(eGame){
	case jackpot::GAME_7POKER: return IDX_GAME_SP;
	case jackpot::GAME_LOWBD: return IDX_GAME_BD;
	case jackpot::GAME_NEWPOKER: return IDX_GAME_NP;
	case jackpot::GAME_HIGHLOW: return IDX_GAME_HL;
		//case promotion::GAME_SUTDA: return IDX_GAME_SD;
	case jackpot::GAME_HOOLA: return IDX_GAME_HA;
	}
	return IDX_GAME_NULL;
}

bool CVipJackpot::IsVipJackpotGame(void)
{
	switch(GM().GetCurrentGameType()){
	case IDX_GAME_SP: return true;
	case IDX_GAME_BD: return true;
	case IDX_GAME_NP: return true;
	case IDX_GAME_HL: return false;
		//case IDX_GAME_SD: return jackpot::GAME_SUTDA;
	case IDX_GAME_HA: return false;
	}
	return false;
}

int CVipJackpot::GetLimitMoneyChannel(long long llLimitMoney)
{
	if(50ll*MONEY_JO<=llLimitMoney){
		return promotion::VIP_CHANNEL_50000000000000;
	}
	else if(30ll*MONEY_JO<=llLimitMoney){
		return promotion::VIP_CHANNEL_30000000000000;
	}
	else if(10ll*MONEY_JO<=llLimitMoney){
		return promotion::VIP_CHANNEL_10000000000000;
	}
	else if(3ll*MONEY_JO<=llLimitMoney){
		return promotion::VIP_CHANNEL_3000000000000;
	}
	else if(1ll*MONEY_JO<=llLimitMoney){
		return promotion::VIP_CHANNEL_1000000000000;
	}
	if(5000ll*MONEY_UK==llLimitMoney){
		return promotion::VIP_CHANNEL_500000000000;
	}
	return promotion::VIP_CHANNEL_500000000000;
}
//자동화
int CVipJackpot::GetLimitMoneyChannel(IDX_GAME eGame, long long llLimitMoney, long long llSeedMoney)
{
	int nChannel=0;
	switch(eGame){
	case IDX_GAME_SP:
		{
			//if(3000ll*MONEY_UK<=llSeedMoney) nChannel=3;
			//else if(1000ll*MONEY_UK<=llSeedMoney) nChannel=2;
			//else if(300ll*MONEY_UK<=llSeedMoney) nChannel=1;
			//else if(50ll*MONEY_UK<=llSeedMoney) nChannel=0;
			if(3000ll*MONEY_UK<=llSeedMoney){
				if(30ll*MONEY_JO<=llLimitMoney){
					nChannel=3;
				}
				else{
					nChannel=2;
				}
			}
			else if(500ll*MONEY_UK<=llSeedMoney) nChannel=1;
			else if(100ll*MONEY_UK<=llSeedMoney) nChannel=0;
		}
		break;
	case IDX_GAME_BD:
		{
			//if(3000ll*MONEY_UK<=llSeedMoney){
			//	if(30ll*MONEY_JO<=llLimitMoney){
			//		nChannel=3;
			//	}
			//	else{
			//		nChannel=2;
			//	}
			//}
			//else if(1000ll*MONEY_UK<=llSeedMoney) nChannel=2;
			//else if(300ll*MONEY_UK<=llSeedMoney) nChannel=1;
			//else if(50ll*MONEY_UK<=llSeedMoney) nChannel=0;
			if(3000ll*MONEY_UK<=llSeedMoney){
				if(30ll*MONEY_JO<=llLimitMoney){
					nChannel=3;
				}
				else{
					nChannel=2;
				}
			}
			else if(1000ll*MONEY_UK<=llSeedMoney) nChannel=2;
			else if(500ll*MONEY_UK<=llSeedMoney) nChannel=1;
			else if(100ll*MONEY_UK<=llSeedMoney) nChannel=0;
		}
		break;
	case IDX_GAME_NP:
		{
			if(5500ll*MONEY_UK<=llSeedMoney) nChannel=3;
			else if(2500ll*MONEY_UK<=llSeedMoney) nChannel=2;
			else if(700ll*MONEY_UK<=llSeedMoney) nChannel=1;
			else if(300ll*MONEY_UK<=llSeedMoney) nChannel=0;
		}
		break;
	case IDX_GAME_HL:
		{

		}
		break;
	case IDX_GAME_SD:
		{

		}
		break;
	case IDX_GAME_HA:
		{

		}
		break;
	}
	return nChannel;
}

void CVipJackpot::PlaySound(LPCTSTR lpCommand)
{
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) return;	
	PBPlayEffectSound(lpCommand);
}