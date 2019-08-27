#include "stdafx.h"
#include "PromotionManager.h"
#include "PromotionBase.h"
#include "PromotionPokerTime.h"
#include "PromotionJokboJackpot.h"
#include "PromotionJackpotRelease.h"
#include "PromotionUPP.h"
#include "PromotionSpecialJackpot.h"
#include "PromotionComeBack.h"
#include "PromotionLuckyWheel.h"
#include "PromotionSuperMoon.h"
#include "PromotionTotalDdengab.h"
#include "PromotionNewYearLuckyBag.h"
#include "PromotionChuSeokGoldCoin.h"

#include <EventCommon.h>
#include "../ChattingWindowMan/ChattingWindowMan.h"
#include "../shared/SharedDefine.h"
#include "../GlobalPokerBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// ### [ Frame++ ] ###
void CALLBACK PromotionTimerProc(UINT uTimerID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2) 
{ 
	g_cPromotionMan.OnTimer();	
} 


CPromotionManager::CPromotionManager()
:	m_pUIInfoData(NULL),
	m_pGameProcess(NULL),
	m_pChatMan(NULL),
	m_pCurEventTabPromotion(NULL),
	m_nTotalEventTab(0),
	m_nCurEventTabIndex(0),
	m_pShowEndPopupPromotion(NULL),
	m_hPromotionTimer(0)
{
	MM().AddMsgH(UPG_CMK_GET_ENDPOPUP_PROMOTION, Fnt(this, &CPromotionManager::OnGetPromotionEndPopupType));
	MM().AddMsgH(UPG_CMK_INIT_ENDPOPUP_PROMOTION, Fnt(this, &CPromotionManager::OnInitEndPopup));
	MM().AddMsgH(UPG_CMK_PAINT_ENDPOPUP_PROMOTION, Fnt(this, &CPromotionManager::OnPaintEndPopup));
	MM().AddMsgH(UPG_CMK_CLICK_ENDPOPUP_PROMOTION, Fnt(this, &CPromotionManager::OnClickEndPopup));
	MM().AddMsgH(UPG_CMK_DESTROY_ENDPOPUP_PROMOTION, Fnt(this, &CPromotionManager::OnDestroyEndPopup));

	StartTimer();
}

CPromotionManager::~CPromotionManager()
{
	Reset();
	MM().RemoveMsgH(UPG_CMK_GET_ENDPOPUP_PROMOTION);
}

void CPromotionManager::Reset(void)
{
	list<CPromotionBase *>::iterator start = m_listPromotion.begin();
	list<CPromotionBase *>::iterator end = m_listPromotion.end();

	for(;start != end;start++)
	{
		delete *start;
	}

	m_listPromotion.clear();
	m_listPageRect.clear();

	m_nTotalEventTab = 0;
	m_pCurEventTabPromotion = NULL;
	m_nCurEventTabIndex = -1;
	m_pShowEndPopupPromotion = NULL;

	m_sprEventPage.Remove();
	GM().GetPromotionData()->Clear();
}

void CPromotionManager::SetEvent()
{
	if(NULL==m_pChatMan) return;
	m_pChatMan->SetEvent();
}

void CPromotionManager::Init(CUIInfoData *pUIInfoData, CGame *pGame, CChattingWindowMan *pChatMan)
{
	m_pUIInfoData = pUIInfoData;
	m_pGameProcess = pGame->GetGameProcess();
	m_pChatMan = pChatMan;
	m_ptTabStart = pUIInfoData->m_ptEvent;

	string strDir = ".\\PokerCommon\\data\\Event\\";
	_LOADSPRITEDEFINE(&m_sprEventPage, strDir, "eventpage.spr");	


	// 테스트 소스.
	//MakePromotion(EPT_SPECIAL_JACKPOT);
}

BOOL CPromotionManager::OnLButtonDownBottom(int x , int y)
{
	list<CPromotionBase *>::iterator start = m_listPromotion.begin();
	list<CPromotionBase *>::iterator end = m_listPromotion.end();

	for(;start != end;start++)
	{
		if((*start)->OnLButtonDownBottom(x, y)) return TRUE;
	}

	return FALSE;
}

BOOL CPromotionManager::OnLButtonUpBottom(int x , int y)
{
	list<CPromotionBase *>::iterator start = m_listPromotion.begin();
	list<CPromotionBase *>::iterator end = m_listPromotion.end();
	
	for(;start != end;start++)
	{
		if((*start)->OnLButtonUpBottom(x, y)) return TRUE;
	}
	
	return FALSE;
}

BOOL CPromotionManager::OnMouseMoveBottom(int x , int y)
{
	list<CPromotionBase *>::iterator start = m_listPromotion.begin();
	list<CPromotionBase *>::iterator end = m_listPromotion.end();
	
	for(;start != end;start++)
	{
		if((*start)->OnMouseMoveBottom(x, y)) return TRUE;
	}
	
	return FALSE;
}

BOOL CPromotionManager::OnLButtonDownMiddle(int x , int y)
{
	list<CPromotionBase *>::iterator start = m_listPromotion.begin();
	list<CPromotionBase *>::iterator end = m_listPromotion.end();

	for(;start != end;start++)
	{
		if((*start)->OnLButtonDownMiddle(x, y)) return TRUE;
	}

	return FALSE;
}

BOOL CPromotionManager::OnLButtonUpMiddle(int x , int y)
{
	list<CPromotionBase *>::iterator start = m_listPromotion.begin();
	list<CPromotionBase *>::iterator end = m_listPromotion.end();

	for(;start != end;start++)
	{
		if((*start)->OnLButtonUpMiddle(x, y)) return TRUE;
	}

	return FALSE;
}

BOOL CPromotionManager::OnMouseMoveMiddle(int x , int y)
{
	list<CPromotionBase *>::iterator start = m_listPromotion.begin();
	list<CPromotionBase *>::iterator end = m_listPromotion.end();

	for(;start != end;start++)
	{
		if((*start)->OnMouseMoveMiddle(x, y)) return TRUE;
	}

	return FALSE;
}

BOOL CPromotionManager::OnLButtonDownTop(int x , int y)
{
	list<CPromotionBase *>::iterator start = m_listPromotion.begin();
	list<CPromotionBase *>::iterator end = m_listPromotion.end();

	for(;start != end;start++)
	{
		if((*start)->OnLButtonDownTop(x, y)) return TRUE;
	}

	return FALSE;
}

BOOL CPromotionManager::OnLButtonUpTop(int x , int y)
{
	list<CPromotionBase *>::iterator start = m_listPromotion.begin();
	list<CPromotionBase *>::iterator end = m_listPromotion.end();

	for(;start != end;start++)
	{
		if((*start)->OnLButtonUpTop(x, y)) return TRUE;
	}

	return FALSE;
}

BOOL CPromotionManager::OnMouseMoveTop(int x , int y)
{
	list<CPromotionBase *>::iterator start = m_listPromotion.begin();
	list<CPromotionBase *>::iterator end = m_listPromotion.end();

	for(;start != end;start++)
	{
		if((*start)->OnMouseMoveTop(x, y)) return TRUE;
	}

	return FALSE;
}

CPromotionBase *CPromotionManager::GetPromotion(ENUM_PROMOTION_TYPE nType)
{
	return GetFind(nType);
}

void CPromotionManager::ShowPromotion(ENUM_PROMOTION_TYPE nType)
{

	CPromotionBase *pPromotion = GetPromotion(nType);
	if(NULL==pPromotion) return;
	pPromotion->SetActive( true );
	pPromotion->SetDraw( true );
	pPromotion->SetShowTab( true );

	SetCurTabPromotion(nType);

	m_nTotalEventTab = GetTotalEventTab();
	m_listPageRect.clear();
	if(m_nTotalEventTab > 1)
	{
		m_nTotalEventTab -= 1;
		int nX = m_ptTabStart.x + m_pUIInfoData->m_ptEventPage.x - m_pUIInfoData->m_nEventPageGab * m_nTotalEventTab;
		int nY = m_ptTabStart.y + m_pUIInfoData->m_ptEventPage.y;

		CRect rtRect;
		rtRect.top = nY;
		rtRect.bottom = nY + m_sprEventPage.spr[0].yl;

		for(int i=0;i<m_nTotalEventTab;i++)
		{
			rtRect.left = nX;
			rtRect.right = nX + m_sprEventPage.spr[0].xl;

			m_listPageRect.push_back(rtRect);

			nX += m_pUIInfoData->m_nEventPageGab;
		}
	}

}

void CPromotionManager::RemovePromotion(ENUM_PROMOTION_TYPE nType)
{

 	CPromotionBase *pPromotion = GetPromotion(nType);
	if(NULL==pPromotion) return;
	pPromotion->SetActive( false );
	pPromotion->SetDraw( false );
	pPromotion->SetShowTab( false );
// 	if ( NULL != pPromotion )
// 	{
// 		m_listPromotion.remove( pPromotion );
// 	}
	
	int i;
	unsigned int leng = m_listPromotion.size();
	for(i = 0; i<(int)leng; i++){
		CPromotionBase *pNewPromotion = GetPromotionFromIndex(i);
		if(pNewPromotion && pNewPromotion->IsShowTab()){
			SetCurTabPromotion((ENUM_PROMOTION_TYPE)pNewPromotion->GetType());
			break;
		}
	}
	
// 	delete pPromotion;
// 	pPromotion = NULL;

	m_nTotalEventTab = GetTotalEventTab();
	m_listPageRect.clear();
	if(m_nTotalEventTab > 1)
	{
		m_nTotalEventTab -= 1;
		int nX = m_ptTabStart.x + m_pUIInfoData->m_ptEventPage.x - m_pUIInfoData->m_nEventPageGab * m_nTotalEventTab;
		int nY = m_ptTabStart.y + m_pUIInfoData->m_ptEventPage.y;

		CRect rtRect;
		rtRect.top = nY;
		rtRect.bottom = nY + m_sprEventPage.spr[0].yl;

		for(int i=0;i<m_nTotalEventTab;i++)
		{
			rtRect.left = nX;
			rtRect.right = nX + m_sprEventPage.spr[0].xl;

			m_listPageRect.push_back(rtRect);

			nX += m_pUIInfoData->m_nEventPageGab;
		}
	}
}

CPromotionBase	*CPromotionManager::MakePromotion(ENUM_PROMOTION_TYPE nType)
{
	CPromotionBase *pPromotion = GetPromotion(nType);
	if(pPromotion == NULL)
	{
		bool bAllways = false;
		switch(nType)
		{
			//상시프로모션
		case EPT_POKERTIME :
			pPromotion = new CPromotionPokerTime(this, int(nType));
			//8월 프로모션 작업중에 코인획득시 이벤트창 얼럿을 위해 주석처리. 
			// 2012.08.07 류승민.
			//m_pChatMan->SetEvent();
			break;
		case EPT_JOKBOJACKPOT:
			{
				pPromotion = new PromotionJokboJackpot(this, int(nType));
				//m_pChatMan->SetViewPage(CChattingWindowMan::ECTT_EVENT);
				//m_pChatMan->SetEvent();
			}
			break;
			//잭팟대방출
		case EPT_JACKPOTRELEASE:
			{
				pPromotion = new CPromotionJackpotRelease(this, int(nType));
			}
			break;
		case EPT_UPPPOLICY: //UPP정책
			{
				SAFE_NEW(pPromotion, CPromotionUPP(this, int(nType)));
			}
			break;
		case EPT_POKER_COMEBACK:
			{
				SAFE_NEW(pPromotion, CPromotionComeback(this, int(nType)));
			}
			break;
		case EPT_POKER_LUCKYWHEEL:
			{
				SAFE_NEW(pPromotion, CPromotionLuckyWheel(this, int(nType)));
			}
			break;
		case EPT_SPECIAL_JACKPOT:			//스페셜 잭팟 프로모션.
			{
				SAFE_NEW(pPromotion,  CPromotionSpecialJackpot(this, int(nType)));
				//m_pChatMan->SetEvent();
			} 
			break;
		case EPT_POKER_SUPERMOON:
			{
				SAFE_NEW(pPromotion, CPromotionSuperMoon(this, int(nType)));
			}
			break;
		case EPT_POKER_NEWYEAR:
			{
				SAFE_NEW(pPromotion, CPromotionNewYearLuckyBag(this, int(nType)));
			}
			break;
		case EPT_POKER_REWARD_NEWPOKER:
			{
				// 땡값 이벤트 적용
				SAFE_NEW(pPromotion, CPromotionTotalDdengab(this, int(nType)));
			}
			break;
		case EPT_POKER_2018_CHUSEOK:
			{				
				SAFE_NEW(pPromotion, CPromotionChuSeokGoldCoin(this, int(nType)));
			}
			break;
		}

		if(pPromotion != NULL)
		{
			m_listPromotion.push_back(pPromotion);
		}
		
		if(m_pCurEventTabPromotion)
		{
			m_nCurEventTabIndex = GetPromotionIndexFromItem(m_pCurEventTabPromotion);
		}
		
		if(GetTotalEventTab() == 1)			//탭에 하나도 없으면 현재 파라미터로 들어온 프로모션을 활성화 시킨다.
		{
			SetCurTabPromotion(nType);
		}
		CheckPageStatus();
	}
	return pPromotion;
}

BOOL CPromotionManager::OnPacketNotify(char* pName, int Signal, int TotSize, char *lpData)
{
	switch(Signal)
	{
	case OPCODE_SV_GTIME_START:
		MakePromotion(EPT_POKERTIME);
		break;
	case OPCODE_SV_JP_WINNER_ALRAM:
		MakePromotion(EPT_JOKBOJACKPOT);
		break;
		// 바둑이 경기대회로 접속했을 때 날라옴
	case OPCODE_SV_CHAMP_ALRAM:
		{
			SendUILog_Enable(false); //바둑이 경기대회 서버에서는 통계 로그를 남기지 않는다.
		}
		break;
	case OPCODE_SV_RETENTION_START:
		{
			if(NULL!=lpData){
				packet::TSendPacket< promotion::RETENTION_ALRAM > Retention_Packet;
				Retention_Packet.Get( lpData, TotSize );
				promotion::RETENTION_ALRAM* pAlaram = static_cast< promotion::RETENTION_ALRAM* >( Retention_Packet.Various() );
				memcpy(GM().GetRETENTION_ALRAM(), pAlaram, sizeof(promotion::RETENTION_ALRAM));
				if(0<GM().GetRETENTION_ALRAM()->nUserState){
					MM().Call(UPG_CMK_SHOW_PROMOTION_COMEBACK);
					MakePromotion(EPT_POKER_COMEBACK);
				}
			}
		}
		break;
	// 하프 잭팟 프로모션.
	case OPCODE_SV_HALFJACKPOT_START:
		{
			MakePromotion(EPT_SPECIAL_JACKPOT);
		}
		break;
	case OPCODE_SV_VISITE_ALARM:
		{
			MakePromotion(EPT_POKER_LUCKYWHEEL);				
		}
		break;	
	case OPCODE_SV_SUPERMOON_ALARM:
		{
			MakePromotion(EPT_POKER_SUPERMOON);
		}
		break;	
	case SV_REWARD_TOTALPOKER_START:
		{
			MakePromotion(EPT_POKER_REWARD_NEWPOKER);
		}
		break;
	case SV_PROMOTION_COMMON_GAMECOUNT_USERINFO:
		{
			MakePromotion(EPT_POKER_NEWYEAR);
		}
		break;
	case OPCODE_SV_2018THANKSGIVING_GAMECOUNT:
		{
			MakePromotion(EPT_POKER_2018_CHUSEOK);
		}
		break;
	case SV_UPPINFO: 
		{
			MakePromotion(EPT_UPPPOLICY);

			CSV_UPPINFO msg;
			msg.Get(lpData, TotSize);

			static BOOL bFirstMsg = TRUE;
			MM().Call(UPG_CMK_RESET_WARNINGTIMER, (WPARAM)*msg.m_warning);

			switch(*msg.m_usage)
			{
			case 0:// 0 : 최초 로그인시 날라가는거
				{
					CPromotionUPP *pPromotion = (CPromotionUPP*)GetPromotion(EPT_UPPPOLICY);
					if (pPromotion != NULL)
					{
						//프로모션 데이터 세팅
						pPromotion->SetUPPData(&msg);
						if ( ( *msg.m_agree == 0 )&&( *msg.m_level == 2 ) )
							pPromotion->CheckUPPAgreement();
						else pPromotion->ShowUPPInfoDlg();
					}
				}
				break;
			case 1:// 1 : moving 시 날라가는거 
				{
					CPromotionUPP *pPromotion = (CPromotionUPP*)GetPromotion(EPT_UPPPOLICY);
					if (pPromotion != NULL)
					{
						//프로모션 데이터 세팅
						pPromotion->SetUPPData(&msg);
					}
				}
				break;
			case 2:// 2 : 시간알리미응답
				{
					CTimeNotifierDlg dlg(GM().GetMainWnd());
					dlg.DoModal(&msg);
				}
				break;
 			}
// 			if (bFirstMsg && *msg.m_level > 0)
// 			{
// 				bFirstMsg = FALSE;
// 
// 				
// 				CPromotionUPP *pPromotion = (CPromotionUPP*)GetPromotion(EPT_UPPPOLICY);
// 				if (pPromotion == NULL)
// 				{
// 					break;
// 				}
// 
// 				pPromotion->SetUPPData(&msg);
// 				if ( ( *msg.m_agree == 0 )&&( *msg.m_level == 2 ) )
// 				{
// 					pPromotion->CheckUPPAgreement();
// 				}
// 				else
// 				{
// 					pPromotion->ShowUPPInfoDlg();
// 				}
// 			}
// 			else
// 			{
// 				if (bFirstMsg)
// 				{
// 					bFirstMsg = FALSE;
// 					break;
// 				}
// 				CTimeNotifierDlg dlg(GM().GetMainWnd());
// 				dlg.DoModal(&msg);
// 			}			
		}
		break;
	}

	//잭팟대방출은 상시 적용
	MakePromotion( EPT_JACKPOTRELEASE );
	
	list<CPromotionBase *>::iterator start = m_listPromotion.begin();
	list<CPromotionBase *>::iterator end = m_listPromotion.end();

	for(;start != end;start++)
	{
		if((*start)->OnPacketNotify(pName, Signal, TotSize, lpData)) return TRUE;
	}

	return FALSE;
}

BOOL CPromotionManager::OnCommand(WPARAM wParam, LPARAM lParam)
{
	list<CPromotionBase *>::iterator start = m_listPromotion.begin();
	list<CPromotionBase *>::iterator end = m_listPromotion.end();

	for(;start != end;start++)
	{
		if((*start)->OnCommand(wParam, lParam)) return TRUE;
	}

	return FALSE;
}

void CPromotionManager::DrawBottom(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage)
{
	list<CPromotionBase *>::iterator start = m_listPromotion.begin();
	list<CPromotionBase *>::iterator end = m_listPromotion.end();

	for(;start != end;start++)
	{
		(*start)->DrawBottom(pDC, pPage);
	}
}

void CPromotionManager::DrawMiddle(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage)
{
	list<CPromotionBase *>::iterator start = m_listPromotion.begin();
	list<CPromotionBase *>::iterator end = m_listPromotion.end();

	for(;start != end;start++)
	{
		(*start)->DrawMiddle(pDC, pPage);
	}
}

void CPromotionManager::DrawTop(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage)
{
	list<CPromotionBase *>::iterator start = m_listPromotion.begin();
	list<CPromotionBase *>::iterator end = m_listPromotion.end();

	for(;start != end;start++)
	{
		(*start)->DrawTop(pDC, pPage);
	}
}


void CPromotionManager::OnStartGame()
{
	list<CPromotionBase *>::iterator start = m_listPromotion.begin();
	list<CPromotionBase *>::iterator end = m_listPromotion.end();

	for(;start != end;start++)
	{
		(*start)->OnStartGame();
	}
}

void CPromotionManager::Accept_OutRoom()
{
	list<CPromotionBase *>::iterator start = m_listPromotion.begin();
	list<CPromotionBase *>::iterator end = m_listPromotion.end();

	for(;start != end;start++)
	{
		(*start)->Accept_OutRoom();
	}
}

void CPromotionManager::Accept_CreateRoom()
{
	list<CPromotionBase *>::iterator start = m_listPromotion.begin();
	list<CPromotionBase *>::iterator end = m_listPromotion.end();

	for(;start != end;start++)
	{
		(*start)->Accept_CreateRoom();
	}
}

void CPromotionManager::Accept_EnterRoom()
{
	list<CPromotionBase *>::iterator start = m_listPromotion.begin();
	list<CPromotionBase *>::iterator end = m_listPromotion.end();

	for(;start != end;start++)
	{
		(*start)->Accept_EnterRoom();
	}
}

void CPromotionManager::OnEnterRoom()
{
	if(m_pCurEventTabPromotion){
		m_pCurEventTabPromotion->OnEnterRoom();
	}
}

void CPromotionManager::CurPromotionEventTab(bool bShow)
{
	if(m_pCurEventTabPromotion){
		m_pCurEventTabPromotion->CurPromotionEventTab(bShow);
	}
}

void CPromotionManager::OnEndGame()
{
	list<CPromotionBase *>::iterator start = m_listPromotion.begin();
	list<CPromotionBase *>::iterator end = m_listPromotion.end();

	for(;start != end;start++)
	{
		(*start)->OnEndGame();
	}
}



const COLORREF &CPromotionManager::GetChatColor(ENUM_CHAT_COLOR nType)
{
	return m_pUIInfoData->m_listChatColor[nType];
}

CPromotionBase *CPromotionManager::GetFind(ENUM_PROMOTION_TYPE nType)
{
	list<CPromotionBase *>::iterator start = m_listPromotion.begin();
	list<CPromotionBase *>::iterator end = m_listPromotion.end();
	for(;start != end;start++)
	{
		if((*start)->GetType() == nType) return *start;
	}
	return NULL;
}

void CPromotionManager::SetCurTabPromotion(ENUM_PROMOTION_TYPE nType)
{
	if(m_pCurEventTabPromotion && m_pCurEventTabPromotion->GetType() == nType) return;
	CPromotionBase *pPromotion = GetFind(nType);
	if(pPromotion == NULL) return;
	if(!pPromotion->IsShowTab())
	{
		return;
	}
	CurPromotionEventTab(false);
	m_pCurEventTabPromotion = pPromotion;
	m_nCurEventTabIndex = GetCurrentIndex();
}

//이벤트탭 안의 하얀사각버튼에 마우스 다운했을 때
BOOL CPromotionManager::OnEventTabLButtonDown(int &x , int &y)
{
	int nCount = (int)m_listPageRect.size();
	for(int i=0;i<nCount;i++)
	{
		if(m_listPageRect[i].PtInRect(CPoint(x, y)))
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				if (i == 0)
					SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_CHATWINDOW_EVENT_SQUARE_1);
				else if (i == 1)
					SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_CHATWINDOW_EVENT_SQUARE_2);
			#endif //UPGRADE_10210619_ADD_CLICKLOG

			CPromotionBase *pPromotion = GetPromotionFromIndex(i);
			if(pPromotion) SetCurTabPromotion((ENUM_PROMOTION_TYPE)pPromotion->GetType());
			m_nCurEventTabIndex = i;
			return TRUE;
		}
	}

	if(m_pCurEventTabPromotion && m_pCurEventTabPromotion->OnEventTabLButtonDown(x, y)) return TRUE;
	return FALSE;
}

BOOL CPromotionManager::OnEventTabLButtonUp(int &x , int &y)
{
	int nCount = (int)m_listPageRect.size();
	for(int i=0;i<nCount;i++)
	{
		if(m_listPageRect[i].PtInRect(CPoint(x, y)))
		{
			return TRUE;
		}
	}

	if(m_pCurEventTabPromotion && m_pCurEventTabPromotion->OnEventTabLButtonUp(x, y)) return TRUE;
	return FALSE;
}

BOOL CPromotionManager::OnEventTabMouseMove(int &x , int &y)
{
	int nCount = (int)m_listPageRect.size();
	for(int i=0;i<nCount;i++)
	{
		if(m_listPageRect[i].PtInRect(CPoint(x, y)))
		{
			SetCursor(GM().GetCurHandCursor());
			return TRUE;
		}
	}
	
	if(m_pCurEventTabPromotion && m_pCurEventTabPromotion->OnEventTabMouseMove(x, y)) return TRUE;
	return FALSE;
}

void CPromotionManager::DrawEventTab(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage)
{
	if(m_pCurEventTabPromotion){
		if(true==m_pCurEventTabPromotion->IsShowTab()){
			m_pCurEventTabPromotion->DrawEventTab(pDC, pPage, m_ptTabStart);
		}
	}

	if(m_nTotalEventTab > 1)
	{
		int nCount = m_listPageRect.size();

		for(int i=0;i<nCount;i++)
		{
			CRect &rtRect = m_listPageRect[i];
			pPage->PutSprAuto(rtRect.left, rtRect.top, &m_sprEventPage, (m_nCurEventTabIndex == i)?1:0);
		}
	}
}

const int CPromotionManager::GetTotalEventTab(void)
{
	list<CPromotionBase *>::iterator start = m_listPromotion.begin();
	list<CPromotionBase *>::iterator end = m_listPromotion.end();

	int i=0;
	for(;start != end;start++)
	{
		if((*start)->IsShowTab()){
			i++;
		}
	}
	return i;
}

//이벤트 탭의 탭개수(사각버튼) 설정
void CPromotionManager::CheckPageStatus(void)
{
	m_nTotalEventTab = GetTotalEventTab();

	//테스트 코드
	//m_nTotalEventTab = 4;

	
	m_listPageRect.clear();
	if(m_nTotalEventTab > 1)
	{
		int nX = m_ptTabStart.x + m_pUIInfoData->m_ptEventPage.x - m_pUIInfoData->m_nEventPageGab * m_nTotalEventTab;
		int nY = m_ptTabStart.y + m_pUIInfoData->m_ptEventPage.y;

		CRect rtRect;
		rtRect.top = nY;
		rtRect.bottom = nY + m_sprEventPage.spr[0].yl;

		for(int i=0;i<m_nTotalEventTab;i++)
		{
			rtRect.left = nX;
			rtRect.right = nX + m_sprEventPage.spr[0].xl;

			m_listPageRect.push_back(rtRect);

			nX += m_pUIInfoData->m_nEventPageGab;
		}
	}
}

int CPromotionManager::GetCurrentIndex(void)
{
	if(m_pCurEventTabPromotion)
	{
		list<CPromotionBase *>::iterator start = m_listPromotion.begin();
		list<CPromotionBase *>::iterator end = m_listPromotion.end();
		int i=-1;
		for(;start != end;start++)
		{
			if((*start)->IsShowTab()) i++;
			if(*start == m_pCurEventTabPromotion)
			{
				return i;
			}		
		}
	}
	
	return -1;
}

CPromotionBase *CPromotionManager::GetPromotionFromIndex(int nIndex)
{
	list<CPromotionBase *>::iterator start = m_listPromotion.begin();
	list<CPromotionBase *>::iterator end = m_listPromotion.end();
	int i=-1;
	for(;start != end;start++)
	{
		if((*start)->IsShowTab()) i++;
		if(i == nIndex)
		{
			return *start;
		}
	}

	return NULL;
}

int CPromotionManager::GetPromotionIndexFromItem(CPromotionBase *pPromotion)
{
	if(pPromotion == NULL || !pPromotion->IsShowTab()) return -1;
	list<CPromotionBase *>::iterator start = m_listPromotion.begin();
	list<CPromotionBase *>::iterator end = m_listPromotion.end();

	int i=-1;
	for(;start != end;start++)
	{
		if((*start)->IsShowTab()) i++;
		if(*start == pPromotion)
		{
			return i;
		}
	}

	return -1;
}



LRESULT CPromotionManager::OnGetPromotionEndPopupType(WPARAM& wParam, LPARAM& lParam)
{

	list<CPromotionBase *>::iterator start = m_listPromotion.begin();
	list<CPromotionBase *>::iterator end = m_listPromotion.end();
	m_pShowEndPopupPromotion = NULL;
	for(;start != end;start++)
	{
		if((*start)->GetEndPopupType()!=TYPE_ENDPOPUP_NO_EVENT)
		{
			if(m_pShowEndPopupPromotion)
			{
				if(m_pShowEndPopupPromotion->GetPriorities() > (*start)->GetPriorities())
				{
					m_pShowEndPopupPromotion = *start;
				}
			}
			else
			{
				m_pShowEndPopupPromotion = *start;
			}
		}
	}

	if(m_pShowEndPopupPromotion)
	{
		return (LRESULT) m_pShowEndPopupPromotion->GetEndPopupType();
	}

	return (LRESULT) TYPE_ENDPOPUP_NO_EVENT;

}

LRESULT CPromotionManager::OnInitEndPopup(WPARAM& wParam, LPARAM& lParam)
{
	return InitEndPopup((CEndPopup *)lParam, *((CPoint *)wParam))?TRUE:FALSE;;
}

LRESULT CPromotionManager::OnPaintEndPopup(WPARAM& wParam, LPARAM& lParam)
{
	ASSERT(m_pShowEndPopupPromotion != NULL);
	return m_pShowEndPopupPromotion->OnPaintEndPopup((CPaintDC *)lParam)?TRUE:FALSE;
}

LRESULT CPromotionManager::OnClickEndPopup(WPARAM& wParam, LPARAM& lParam)
{
	ASSERT(m_pShowEndPopupPromotion != NULL);
	return m_pShowEndPopupPromotion->OnClickEndPopup((UINT)wParam)?TRUE:FALSE;
}

LRESULT CPromotionManager::OnDestroyEndPopup(WPARAM& wParam, LPARAM& lParam)
{
	ASSERT(m_pShowEndPopupPromotion != NULL);
	m_pShowEndPopupPromotion->OnDestroyEndPopup();
	return TRUE;
}

bool CPromotionManager::InitEndPopup(CEndPopup *pEndPopup, CPoint ptPos)
{
	list<CPromotionBase *>::iterator start = m_listPromotion.begin();
	list<CPromotionBase *>::iterator end = m_listPromotion.end();
	m_pShowEndPopupPromotion = NULL;
	for(;start != end;start++)
	{
		if((*start)->GetEndPopupType()!=TYPE_ENDPOPUP_NO_EVENT)
		{
			if(m_pShowEndPopupPromotion)
			{
				if(m_pShowEndPopupPromotion->GetPriorities() > (*start)->GetPriorities())
				{
					m_pShowEndPopupPromotion = *start;
				}
			}
			else
			{
				m_pShowEndPopupPromotion = *start;
			}
		}
	}

	if(m_pShowEndPopupPromotion)
	{
		return m_pShowEndPopupPromotion->InitEndPopup(pEndPopup, ptPos);
	}
	return false;
}

void CPromotionManager::OnTimer()
{
	list<CPromotionBase *>::iterator start = m_listPromotion.begin();
	list<CPromotionBase *>::iterator end = m_listPromotion.end();
	
	for(;start != end;start++)
	{
		(*start)->OnTimer();
	}
}


BOOL CPromotionManager::StartTimer()
{
	if(m_hPromotionTimer)
		return FALSE;
	
	TIMECAPS caps; memset( &caps, 0, sizeof(caps) );
	timeGetDevCaps( &caps, sizeof(caps) ); 	
	m_hPromotionTimer = timeSetEvent(1000, 0 ,PromotionTimerProc, 0, TIME_PERIODIC|TIME_CALLBACK_FUNCTION); 

	return (m_hPromotionTimer!=0);
}
void CPromotionManager::KillTimer()
{
	if(m_hPromotionTimer == 0) 
		return;
	timeKillEvent(m_hPromotionTimer);
	m_hPromotionTimer = 0;

}

void CPromotionManager::Test()
{
	CPromotionUPP* pPromotion = (CPromotionUPP*)GetPromotion(EPT_UPPPOLICY);
	pPromotion->SetActive(TRUE);
	pPromotion->Test();
}

void CPromotionManager::SetChatViewPage_ToEventTab()
{
	m_pChatMan->SetViewPage(CChattingWindowMan::ECTT_EVENT);
}

void CPromotionManager::AddChatView(LPCTSTR lpStr, COLORREF crNewColor, bool bUserChat)
{
	m_pChatMan->AddChat(lpStr, crNewColor, bUserChat);

}

// 하프잭팟 프로모션이 진행중이면 하프 카운트 증가.
void CPromotionManager::SetHalfJackpotCountPlus()
{
	CPromotionSpecialJackpot* pJackpot = (CPromotionSpecialJackpot*)GetPromotion(EPT_SPECIAL_JACKPOT) ;
	if(NULL == pJackpot)
		return ;

	pJackpot->setHalfCountPlus() ;
}

// 하프잭팟 프로모션이 진행중이면 승자 정보 넘겨줌.
void CPromotionManager::SetWinnerNum(int WinnerNum) 
{
	CPromotionSpecialJackpot* pJackpot = (CPromotionSpecialJackpot*)GetPromotion(EPT_SPECIAL_JACKPOT) ;
	if(NULL == pJackpot)
		return ;

	pJackpot->SetWinnerNum(WinnerNum) ;
}
