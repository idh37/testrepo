#include "StdAfx.h"
#include "PromotionComeback.h"
#include "PromotionManager.h"
#include "PromotionComeBackDlg.h"


CPromotionComeback::CPromotionComeback( CPromotionManager *pManager, int nType )
: CPromotionBase(pManager, nType)
{
	string strDir = ".\\PokerCommon\\data\\Event\\ComeBack\\";
	_LOADSPRITEDEFINE(&m_sprBack, strDir, "gameroom_pan.spr");

	SetShowTab(false);
}

CPromotionComeback::~CPromotionComeback(void)
{

}



void CPromotionComeback::OpenComeBackDlg(void)
{
	if(true == GM().IsEnableComeBackPromotion()){
		MM().Call(UPG_CMK_SHOW_PROMOTION_COMEBACK_DLG);
	}	
}

void CPromotionComeback::Accept_OutRoom()
{
	OpenComeBackDlg();
}

void CPromotionComeback::Accept_CreateRoom()
{
	
}

void CPromotionComeback::Accept_EnterRoom()
{
	
}

void CPromotionComeback::DrawBottom(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage)
{
	if(true==GM().GetRETENTION_ALRAM()->bToday) return;
	pPage->PutSprAuto(11, 490, &m_sprBack, 0) ;


	//폰트/ 색깔
	CFont *pOldFont = pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_13_BOLD));
	COLORREF clrOld = pDC->GetTextColor();
	UINT oldFlags = pDC->SetTextAlign(TA_RIGHT);
	CString str = TEXT("");

	str.Format("%d/%d판", GM().GetRETENTION_ALRAM()->nTodayWinCnt, GM().GetRETENTION_ALRAM()->nTotalWinCnt);
	pDC->SetTextColor(RGB(255, 255, 255));
	pDC->TextOut( 157+11, 6+490, str);


	pDC->SetTextColor(clrOld);
	pDC->SelectObject(pOldFont);	
	pDC->SetTextAlign(oldFlags) ;
}

void CPromotionComeback::DrawMiddle(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage)
{

}

//응모권획득시 그려준다.
void CPromotionComeback::DrawTop(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage)
{

}

void CPromotionComeback::DrawEventTab(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage, const CPoint &ptStart)
{
}

BOOL CPromotionComeback::OnLButtonDownMiddle(int &x , int &y)
{
	return FALSE;
}

BOOL CPromotionComeback::OnLButtonUpMiddle(int &x , int &y)
{
	return FALSE;
}

BOOL CPromotionComeback::OnMouseMoveMiddle(int &x , int &y)
{
	return FALSE;
}

BOOL CPromotionComeback::OnPacketNotify(char* pName, int &Signal, int &TotSize, char *lpData)
{
	switch(Signal)
	{
	case OPCODE_SV_RETENTION_START:
		{
			if(NULL!=lpData){
				packet::TSendPacket< promotion::RETENTION_ALRAM > Retention_Packet;
				Retention_Packet.Get( lpData, TotSize );
				promotion::RETENTION_ALRAM* pAlaram = static_cast< promotion::RETENTION_ALRAM* >( Retention_Packet.Various() );

				memcpy(GM().GetRETENTION_ALRAM(), pAlaram, sizeof(promotion::RETENTION_ALRAM));
				ErrorLogConsole("OPCODE_SV_RETENTION_START %d = %d : %d : %d", pAlaram->nUserState, pAlaram->bToday, pAlaram->nTodayWinCnt, pAlaram->nTotalWinCnt);
				OpenComeBackDlg();
			}
		}
		break;
	case OPCODE_SV_RETENTION_INITIALIZE:
		{
			if(NULL!=lpData){
				//ErrorLogConsole("OPCODE_SV_PLAYCOUNT_ALARM %d = %d : %d", pAlarm->iStep, pAlarm->nPlayCount, pAlarm->bIsComplete);
			}
		}
		break;
	case OPCODE_SV_RETENTION_ALRAM:
		{
			if(NULL!=lpData){
				packet::TSendPacket< promotion::RETENTION_ALRAM > Retention_Packet;
				Retention_Packet.Get( lpData, TotSize );
				promotion::RETENTION_ALRAM* pAlaram = static_cast< promotion::RETENTION_ALRAM* >( Retention_Packet.Various() );

				memcpy(GM().GetRETENTION_ALRAM(), pAlaram, sizeof(promotion::RETENTION_ALRAM));
				ErrorLogConsole("OPCODE_SV_RETENTION_ALRAM %d = %d : %d : %d", pAlaram->nUserState, pAlaram->bToday, pAlaram->nTodayWinCnt, pAlaram->nTotalWinCnt);
			}
		}
		break;
	case OPCODE_SV_RETENTION_REWARD:
		{
			if(NULL!=lpData){
				packet::TSendPacket< promotion::RETENTION_PROMOTION_REWARD > Retention_Packet;
				Retention_Packet.Get( lpData, TotSize );
				promotion::RETENTION_PROMOTION_REWARD* pAlaram = static_cast< promotion::RETENTION_PROMOTION_REWARD* >( Retention_Packet.Various() );
				ErrorLogConsole("OPCODE_SV_RETENTION_REWARD %d = %d : %I64d", pAlaram->bIsComplete, pAlaram->nPlayCount, pAlaram->nReWardMoney);
				if(true==pAlaram->bIsComplete){
					GM().GetRETENTION_ALRAM()->bToday=pAlaram->bIsComplete;
					if((0<=pAlaram->nDay)&&((MAX_RETENTION_MISSION_DAY-1)>=pAlaram->nDay)){
						GM().GetRETENTION_ALRAM()->nMissionComplet[pAlaram->nDay]=1;
					}
					else{
						for(int i=0; i<MAX_RETENTION_MISSION_DAY; i++){
							if(0==GM().GetRETENTION_ALRAM()->nMissionComplet[i]){
								GM().GetRETENTION_ALRAM()->nMissionComplet[i]=1;
								break;
							}
						}
					}
					MM().Call(UPG_CMK_SHOW_PROMOTION_COMEBACK_DLGUPDATE);
					CString strOutput;
					strOutput.Format("축하합니다.\n출석 보상 %s 지급되었습니다.\n", g_NumberToOrientalString( pAlaram->nReWardMoney, 2).GetString());
					ShowMainMessageDlg(strOutput);
				}
				
			}
			//ErrorLogConsole("OPCODE_SV_PLAYCOUNT_END");
		}
		break;
	case OPCODE_SV_RETENTION_END:
		{
			//ErrorLogConsole("OPCODE_SV_PLAYCOUNT_END");
		}
		break;
	}
	return FALSE;
}
