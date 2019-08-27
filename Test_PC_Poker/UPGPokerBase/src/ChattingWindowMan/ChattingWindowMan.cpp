// ChattingWindowMan.cpp: implementation of the CChattingWindowMan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChattingWindowMan.h"
#include "../UIInfoData.h"
#include <CommMsgDef_Game.h>
#include "../GlobalPokerBase.h"
#include "../GameHelper/GameHelperMan.h"

#define _RGB16BIT555(r,g,b) ((b & 31) + ((g & 31) << 5) + ((r & 31) << 10))

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#pragma warning(disable:4996)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void OnProcessSendChat(const char *str);
CChattingWindowMan *s_pChattinWindowMan = NULL;

CChattingWindowMan::CChattingWindowMan()
{
	m_pChatting = NULL;
	m_pInfoBox	= NULL;
	m_pSendEdit = NULL;	
	m_pUIInfoData = NULL;
	m_pGameView = NULL;
	m_pGame = NULL;

	m_bAddEvent = false;
	m_bAddChat = false;
	m_pGameHelperMan = NULL;

	for (int i = 0; i < 10; ++i)
	{
		m_parrTextGrade[i] = NULL;
		m_parrTextID[i] = NULL;
		m_parrTextMoney[i] = NULL;
	}

	for (int i = 0; i < 5; ++i)
	{
		m_parrTextRoundGrade[i] = NULL;
		m_parrTextRoundID[i] = NULL;
		m_parrTextRoundMoney[i] = NULL;
	}


	m_pTextRoundInfo = NULL;

	//선택된 대화상대
	m_rcChatInfo.SetRect( 16, 715, 16 + 70, 715 + 15 );

	MM().AppendMsgH(UPG_CMK_CHANGE_VIEW, Fnt(this, &CChattingWindowMan::OnChangeView));
}

CChattingWindowMan::~CChattingWindowMan()
{
	for (int i = 0; i < 10; ++i)
	{
		if (m_parrTextGrade[i] != NULL)
		{
			delete m_parrTextGrade[i];
			m_parrTextGrade[i] = NULL;
		}

		if (m_parrTextID[i] != NULL)
		{
			delete m_parrTextID[i];
			m_parrTextID[i] = NULL;
		}

		if (m_parrTextMoney[i] != NULL)
		{
			delete m_parrTextMoney[i];
			m_parrTextMoney[i] = NULL;
		}
	}

	for (int i = 0; i < 5; ++i)
	{
		if(m_parrTextRoundGrade[i] != NULL)
		{
			delete m_parrTextRoundGrade[i];
			m_parrTextRoundGrade[i] = NULL;
		}
		if(m_parrTextRoundID[i] != NULL)
		{
			delete m_parrTextRoundID[i];
			m_parrTextRoundID[i] = NULL;
		}
		if(m_parrTextRoundMoney[i] != NULL)
		{
			delete m_parrTextRoundMoney[i];
			m_parrTextRoundMoney[i] = NULL;
		}
	}

	if (m_pTextRoundInfo != NULL)
	{
		delete m_pTextRoundInfo;
		m_pTextRoundInfo = NULL;
	}
}

LRESULT CChattingWindowMan::OnChangeView(WPARAM &wParam, LPARAM &lParam)
{
	IDX_GAMEWHERE nView = (IDX_GAMEWHERE)wParam;
	if(nView == IDX_GAMEWHERE_LOBY) 
		PBCreateWhisperlistComboPopup();
	return TRUE;
}

void CChattingWindowMan::OnDestroy()
{
	if( m_pChatting != NULL )
	{
		delete m_pChatting;
	}
	
	if( m_pInfoBox != NULL ) 
	{
		delete m_pInfoBox;
	}

	if( m_pSendEdit != NULL )
	{
		delete m_pSendEdit;
	}
}

void CChattingWindowMan::LoadImage(LPCTSTR lpChatSprPath, LPCTSTR lpTabEffectPath)
{
	AddLoadImageList(&m_sprChatBox,lpChatSprPath,true);

	if (lpTabEffectPath){
		AddLoadImageList(&m_sprTabEffect,lpTabEffectPath,true);
	}
	else{
		AddLoadImageList(&m_sprTabEffect,".\\PokerCommon\\data\\chattingtabeffect.spr",true);
	}

//#if defined(_DEBUG)
//	AddLoadImageList(&m_sprTest, ".\\PokerCommon\\data\\gauge.spr", true);
//#endif
}

void CChattingWindowMan::SetCurrnetChat(void)
{
	if(s_pChattinWindowMan)
		MM().RemoveMsgH(PB_IS_COMBO_POPUP_CLOSE);

	s_pChattinWindowMan = this;

	if(s_pChattinWindowMan)
		MM().AddMsgH(PB_IS_COMBO_POPUP_CLOSE, Fnt(this, &CChattingWindowMan::OnIsComboPopupClose));

	OnSetWhisper();
}

void CChattingWindowMan::Init (CUIInfoData *pUIInfoData, CGameViewBase *pGameView, CGameHelperMan *pGameHelperMan)
{
	m_pUIInfoData = pUIInfoData;
	m_pGameView = pGameView;
	m_pGame = m_pGameView->GetGame();

	m_nXp = pUIInfoData->m_ptChat.x;
	m_nYp = pUIInfoData->m_ptChat.y;
	m_pPage = &pGameView->Page;

	m_pGameHelperMan = pGameHelperMan;

	m_pChatting = new NMBASE::SKINGDI::CNChatView(m_nXp, m_nYp, m_pUIInfoData->m_szChat.cx, m_pUIInfoData->m_szChat.cy,m_pGameView,m_pPage, true);	
	m_pChatting->SetFontIndex(NMBASE::UTIL::FONT_11D_NORMAL);
	m_pChatting->SetOutlineColor(RGB(0, 0, 0));

	//채팅 입력 창
	m_pSendEdit = new NMBASE::SKINGDI::CNEditBox;
	m_pSendEdit->Init(m_pUIInfoData->m_ptEdit.x,m_pUIInfoData->m_ptEdit.y,m_pUIInfoData->m_szEdit.cx,m_pUIInfoData->m_szEdit.cy,m_pGameView,m_pPage);
	m_pSendEdit->SetFontIndex(NMBASE::UTIL::FONT_11D_NORMAL);
	m_pSendEdit->pSendEdit->SetProcSendChatFn(OnProcessSendChat);

	//정보창 
	m_nXp = pUIInfoData->m_ptInfo.x;
	m_nYp = pUIInfoData->m_ptInfo.y;

	m_pInfoBox = new NMBASE::SKINGDI::CNChatView(m_nXp, m_nYp, m_pUIInfoData->m_szInfo.cx, m_pUIInfoData->m_szInfo.cy,m_pGameView,m_pPage, true);	
	m_pInfoBox->SetFontIndex(NMBASE::UTIL::FONT_11D_NORMAL);
	m_pInfoBox->SetOutlineColor(RGB(0, 0, 0));

	SetViewPage(ECTT_CHAT);
}

void CChattingWindowMan::InitButton(NMBASE::GRAPHICGDI::xSprite *pBadButton)
{
	m_btnChat.Init(m_pGameView,m_pPage,m_pUIInfoData->m_listButtonPos[EBT_CHAT].x, m_pUIInfoData->m_listButtonPos[EBT_CHAT].y,&m_sprChatBox,0,IDM_TABVIEWCHAT);
	m_btnJokbo.Init(m_pGameView,m_pPage,m_pUIInfoData->m_listButtonPos[EBT_JOKBO].x, m_pUIInfoData->m_listButtonPos[EBT_JOKBO].y,&m_sprChatBox,1,IDM_TABVIEWJOKBO);

	m_btnEvent.Init(m_pGameView,m_pPage,m_pUIInfoData->m_listButtonPos[EBT_EVENT].x, m_pUIInfoData->m_listButtonPos[EBT_EVENT].y,&m_sprChatBox,2,IDM_TABVIEWEVENT);

	m_btnLift.Init(m_pGameView, m_pPage,m_pUIInfoData->m_listButtonPos[EBT_WHISPERLIST].x, m_pUIInfoData->m_listButtonPos[EBT_WHISPERLIST].y, &m_sprChatBox, 6, IDM_WHISPERLIST );

	m_btnBadUser.Init(m_pGameView,m_pPage,m_pUIInfoData->m_listButtonPos[EBT_BADUSER].x,m_pUIInfoData->m_listButtonPos[EBT_BADUSER].y,pBadButton, 5, IDM_BADUSER);

	//채팅창
	m_pChatting->BindScrollBtn(&m_sprChatBox, m_pUIInfoData->m_ptChatScroll, m_pUIInfoData->m_nChatScrollHeight, 8, 5, 4, 8300);
	//정보창
	m_pInfoBox->BindScrollBtn(&m_sprChatBox, m_pUIInfoData->m_ptInfoScroll, m_pUIInfoData->m_nInfoScrollHeight, 3, 5, 4, 8400);

	//섯다에 족보탭은 없음
	if( m_pGame->GetGameType() == IDX_GAME_SD ||
		m_pGame->GetGameType() == IDX_GAME_HA)
	{
		m_btnJokbo.Show(FALSE);
	}
}


void CChattingWindowMan::ReSetWindowPos(int x , int y)
{
	m_nXp += x;
	m_nYp += y;

	if( m_pChatting != NULL )
	{
		m_pChatting->OnLButtonDown(x,y);
	}

	if( m_pInfoBox != NULL )
	{
		m_pInfoBox->OnLButtonDown( x, y );
	}
}

BOOL CChattingWindowMan::OnLButtonDown( int x, int y)
{
	m_btnChat.OnLButtonDown(x,y);
	m_btnJokbo.OnLButtonDown(x,y);
	m_btnEvent.OnLButtonDown(x,y);

	switch(m_nViewTab)
	{
	case ECTT_EVENT:
		if(g_cPromotionMan.OnEventTabLButtonDown(x, y)) return TRUE;

		break;
	case ECTT_JOKBO:
		if (m_pGameHelperMan)	//NULL 일수도 있음 (섯다)
		{
			if(m_pGameHelperMan->OnLButtonDown(x,y)) return TRUE;
		}
		break;
	case ECTT_CHAT:
		{
			CRect rc;
			rc = CRect(0, 0, m_pUIInfoData->m_szEdit.cx, m_pUIInfoData->m_szEdit.cy);
			rc.OffsetRect(m_pUIInfoData->m_ptEdit);
			CPoint pt(x, y);

			if(rc.PtInRect( pt) ) 
			{
				if( m_pSendEdit != NULL )
				{
					//2009.11.17 정책 변경
					m_pSendEdit->pSendEdit->SetFocus();
				}		
				return 1;
			}	

			if( m_pChatting && m_pChatting->OnLButtonDown(x,y) ) return TRUE;

			if( m_pInfoBox && m_pInfoBox->OnLButtonDown(x,y) ) return TRUE;

			
			m_btnBadUser.OnLButtonDown(x,y);
			//귓말 삭제
			//m_btnLift.OnLButtonDown( x, y );
		}
		break;
	}

	return FALSE;
}

BOOL CChattingWindowMan::OnLButtonUp( int x, int y)
{
	m_btnChat.OnLButtonUp(x,y);
	m_btnJokbo.OnLButtonUp(x,y);
	m_btnEvent.OnLButtonUp(x,y);
	
	switch(m_nViewTab)
	{
	case ECTT_EVENT:
		if(g_cPromotionMan.OnEventTabLButtonUp(x, y)) return TRUE;

		break;
	case ECTT_JOKBO:
		if (m_pGameHelperMan)//NULL 일수도 있음 (섯다)
		{
			if(m_pGameHelperMan->OnLButtonUp(x,y)) return TRUE;
		}
		break;
	case ECTT_CHAT:
		if(m_btnBadUser.OnLButtonUp(x,y)) return TRUE;
		//귓말 삭제
		//if( m_btnLift.OnLButtonUp( x, y ) ) return TRUE;
		if(m_pChatting && m_pChatting->OnLButtonUp(x,y)) return TRUE;
		if(m_pInfoBox && m_pInfoBox->OnLButtonUp(x,y)) return TRUE;

		break;
	}

	return FALSE;
}

BOOL CChattingWindowMan::OnMouseMove( int x, int y)
{
	POINT pt;
	pt.x = x;
	pt.y = y;

	if (m_pGameHelperMan)//NULL 일수도 있음 (섯다)
	{
		m_pGameHelperMan->ClearInfo();
	}

	if(m_btnChat.OnMouseMove(x,y))
	{
		SetCursor(GM().GetCurHandCursor());
		return TRUE;
	}

	if(m_btnJokbo.OnMouseMove(x,y))
	{
		SetCursor(GM().GetCurHandCursor());
		return TRUE;
	}

	if(m_btnEvent.OnMouseMove(x,y))
	{
		SetCursor(GM().GetCurHandCursor());
		return TRUE;
	}

	switch(m_nViewTab)
	{
	case ECTT_EVENT:
		POINT ptPoint;
		ptPoint.x = x;
		ptPoint.y = y;
		if(g_cPromotionMan.OnEventTabMouseMove(x, y)) return TRUE;

		break;
	case ECTT_JOKBO:
		if (m_pGameHelperMan)//NULL 일수도 있음 (섯다)
		{
			if(m_pGameHelperMan->OnMouseMove(x,y)) return TRUE;
		}
		break;
	case ECTT_CHAT:
		if(m_btnBadUser.OnMouseMove(x,y))
		{
			SetCursor(GM().GetCurHandCursor());
			return TRUE;
		}
		
		//귓말 삭제
		/*
 		if( m_btnLift.OnMouseMove( x, y ) )
 		{
 			SetCursor(GM().GetCurHandCursor());
 			return TRUE;
 		}
		*/

		if(m_pChatting && m_pChatting->OnMouseOver(pt))
		{
			SetCursor(GM().GetCurHandCursor());
			return TRUE;
		}

		if(m_pInfoBox && m_pInfoBox->OnMouseOver(pt))
		{
			SetCursor(GM().GetCurHandCursor());
			return TRUE;
		}
		
		break;
	}

	return FALSE;
}

BOOL CChattingWindowMan::OnMouseWheel( int x, int y )
{
	if(m_nViewTab == ECTT_CHAT) if(m_pChatting && m_pChatting->OnMouseWheel(x,y)) return TRUE;
	
	if(m_nViewTab == ECTT_CHAT) if( m_pInfoBox && m_pInfoBox->OnMouseWheel(x,y) ) return TRUE;

	return FALSE;
}

void CChattingWindowMan::Draw( CDC *pDC )
{
	
	if( pDC == NULL ) return;

	m_btnChat.Draw(pDC);
	m_btnJokbo.Draw(pDC);
	m_btnEvent.Draw(pDC);

	if(m_bAddChat) 
	{
		m_pPage->DrawScene(m_pUIInfoData->m_listButtonPos[EBT_CHAT].x - 1, m_pUIInfoData->m_listButtonPos[EBT_CHAT].y - 1,&m_sprTabEffect,0,timeGetTime());
	}

	if(m_bAddEvent) 
	{
		m_pPage->DrawScene(m_pUIInfoData->m_listButtonPos[EBT_EVENT].x - 1, m_pUIInfoData->m_listButtonPos[EBT_EVENT].y - 1,&m_sprTabEffect,1,timeGetTime());
	}

	switch(m_nViewTab)
	{
	case ECTT_EVENT:
		{
 			g_cPromotionMan.DrawEventTab(pDC, m_pPage);
		} 
		break;
	case ECTT_JOKBO:
		{
			m_pPage->PutSprAuto(11, 537, &m_sprChatBox, 37);

			if (m_pGameHelperMan)//NULL 일수도 있음 (섯다)
			{
				m_pGameHelperMan->Draw(pDC, m_pPage);
			}
		}
		break;
	case ECTT_CHAT:
		{
			m_pPage->PutSprAuto(11, 537, &m_sprChatBox, 36);
						
			if(m_pChatting)
				m_pChatting->Draw(pDC);

			if( m_pSendEdit != NULL )
 			{
 				m_pSendEdit->Draw(pDC);
 			}

			m_btnBadUser.Draw(pDC);
			//귓말 삭제
			//m_btnLift.Draw( pDC );

			//귓말사용자보기
			//DrawWhisperList( pDC );

			if( m_pInfoBox ) m_pInfoBox->Draw(pDC);
		} 
		break;
	}

	if (m_pGameHelperMan)//NULL 일수도 있음 (섯다)
	{
		m_pGameHelperMan->DrawOtherUserJokboArrow(pDC, m_pPage);
	}
}

void CChattingWindowMan::OnSetWhisper(void)
{
	m_strChatInfo = g_cWhisperlistComboPopup.GetText().c_str();
}

//귓말사용자보기
void CChattingWindowMan::DrawWhisperList( CDC *pDC )
{	
	pDC->SetTextColor( RGB(187, 171, 145) );

	//선택된 대화상대
	pDC->DrawText( m_strChatInfo, &m_rcChatInfo, DT_CENTER | DT_VCENTER|DT_SINGLELINE | DT_END_ELLIPSIS );
}

void CChattingWindowMan::OnTimer()
{

}

void CChattingWindowMan::ResetChatting_STR()
{
	m_bAddChat = false;

	if( m_pChatting != NULL )
	{
		m_pChatting->ResetChat();
	}

	if( m_pInfoBox != NULL )
	{
		m_pInfoBox->ResetChat();
	}
}

void CChattingWindowMan::SetViewPage(const ENUM_CHATTING_TAB_TYPE &nViewTab/* = ECT_CHAT*/)
{	
	m_nViewTab = nViewTab;

	m_btnChat.Enable(TRUE);
	m_btnJokbo.Enable(TRUE);
	m_btnEvent.Enable(TRUE);

	switch(m_nViewTab)
	{
	case ECTT_CHAT:
		m_bAddChat = false;
		m_btnChat.Enable(FALSE);

		if( m_pChatting != NULL )
		{
			m_pChatting->SetShow(TRUE);
		}

		if( m_pInfoBox != NULL )
		{
			m_pInfoBox->SetShow( TRUE );
		}
		g_cPromotionMan.CurPromotionEventTab(false);
		break;
	case ECTT_JOKBO:
		m_btnJokbo.Enable(FALSE);
		if( m_pChatting != NULL )
		{
			m_pChatting->SetShow(FALSE);
		}

		if( m_pInfoBox != NULL )
		{
			m_pInfoBox->SetShow(FALSE);
		}
		g_cPromotionMan.CurPromotionEventTab(false);
		break;
	case ECTT_EVENT:
		m_bAddEvent = false;
		m_btnEvent.Enable(FALSE);
	
		if( m_pChatting != NULL )
		{
			m_pChatting->SetShow(FALSE);
		}

		if( m_pInfoBox != NULL )
		{
			m_pInfoBox->SetShow(FALSE);
		}
		g_cPromotionMan.CurPromotionEventTab(true);
		break;
	}
}

void CChattingWindowMan::OnCommand(WPARAM wParam)
{
	switch(wParam) 
	{
			// 족보등급 보이기 , 채팅창 보이기
	case IDM_TABVIEWCHAT:
		#ifdef UPGRADE_10210619_ADD_CLICKLOG
			SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_CHATWINDOW_CHATTING);
		#endif //UPGRADE_10210619_ADD_CLICKLOG

		SetViewPage(ECTT_CHAT);
		break;
	case IDM_TABVIEWJOKBO:
		#ifdef UPGRADE_10210619_ADD_CLICKLOG
			SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_CHATWINDOW_JOKBO);
		#endif //UPGRADE_10210619_ADD_CLICKLOG

		SetViewPage(ECTT_JOKBO);
		break;
	case IDM_TABVIEWEVENT:
		#ifdef UPGRADE_10210619_ADD_CLICKLOG
			SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_CHATWINDOW_EVENT);
		#endif //UPGRADE_10210619_ADD_CLICKLOG

		SetViewPage(ECTT_EVENT);
		break;
	case IDM_WHISPERLIST://귓말사용자보기
		g_cWhisperlistComboPopup.SetShow(!g_cWhisperlistComboPopup.IsShow());
		break;
	case IDM_SETWHISPER:
		OnSetWhisper();
		break;
	}
}

void CChattingWindowMan::SelectNextTab(void)
{
	ENUM_CHATTING_TAB_TYPE nViewTab = (ENUM_CHATTING_TAB_TYPE)((int)m_nViewTab + 1);
	if(nViewTab >= ECTT_MAX)
		nViewTab = ECTT_CHAT;

	SetViewPage(nViewTab);
}

void CChattingWindowMan::AddChat(LPCTSTR lpStr, COLORREF crNewColor, bool bUserChat)
{
	// 4 이상 연속 숫자는 ****로 표시하기
	std::string msg = lpStr;
	msg = g_ConverNumberToAsterisk(msg);
	m_pChatting->AddText((char *)msg.c_str(), crNewColor);

	if(bUserChat && m_nViewTab != ECTT_CHAT)
	{
		m_bAddChat = true;
	}
}

void CChattingWindowMan::AddInfo(LPCTSTR lpStr, COLORREF crNewColor, bool bUserChat)
{
	m_pInfoBox->AddText((char *)lpStr, crNewColor);

	if(bUserChat && m_nViewTab != ECTT_CHAT)
	{
		m_bAddChat = true;
	}
}

void CChattingWindowMan::SystemKeyReturn(LPCTSTR str)
{
	m_pSendEdit->pSendEdit->SetWindowText(str);
	m_pSendEdit->pSendEdit->SendMessage(WM_KEYDOWN, VK_RETURN, 0);
}

void CChattingWindowMan::SetEditFocus(void)
{
	m_pSendEdit->pSendEdit->SetFocus();
}

bool CChattingWindowMan::IsEditFocus(void)
{
	return (GetFocus() == m_pSendEdit->pSendEdit->GetSafeHwnd())?true:false;
}

CString CChattingWindowMan::GetEditText(void)
{
	char str[256]={0,};//대화 내용 읽어 오기
	m_pSendEdit->pSendEdit->GetWindowText(str, 128);
	return str;
}

COLORREF CChattingWindowMan::GetChatColor(int nColor)
{
	return m_pUIInfoData->m_listChatColor[(ENUM_CHAT_COLOR)nColor];
}

bool CChattingWindowMan::CheckWhisperSendChat(const char *str)
{
	//return false; //귓말기능 삭제 (2011/01/17)

	// 닉네임 작업 2006/11/09
	// 귓말인지 검사(대소문자 구분 없음)

	if(strnicmp(str, "/set ", 5)==0)
	{
		char* id=strtok((char*)&str[5], " ");
		char* money=(char*)strtok(NULL, " ");
		if (money==NULL) return false;
		INT64 pmoney=_atoi64(money);
		if (pmoney<0) return false;

		if (strcmp(id, "me")==0)
		{
			CCL_DEBUG_SETMONEY smmsg;
			smmsg.Set(GM().GetMyInfo()->UI.ID, CCL_DEBUG_SETMONEY::SET_MONEY, pmoney);
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), smmsg.pData, smmsg.GetTotalSize());
		}

		if (strcmp(id, "gold")==0)
		{
			CCL_DEBUG_SETMONEY smmsg;
			smmsg.Set(GM().GetMyInfo()->UI.ID, CCL_DEBUG_SETMONEY::SET_GOLD, pmoney);
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), smmsg.pData, smmsg.GetTotalSize());
		}
	}
	
	//모두에게 채팅을 보내는 모드로 변경됨
	if(_stricmp(str, "/a") == 0 || strcmp(str, "/ㅁ") == 0)
	{
		AddChattingWhisperList("", false, false, true);
		return true;
	}
	
	if(strnicmp(str, "/a ", 3) == 0 || strncmp(str, "/ㅁ ", 3) == 0)
	{
		AddChattingWhisperList("", false, false, true);
		return false;
	}

	if( _stricmp(str, "/w") == 0 || strcmp(str, "/ㅈ") == 0 )
	{
		//네이트 귓말막기
		//if( GM().GetMyInfo()->UI.SiteCode == NMBASE::UTIL::SNO_NATE ) return true;

		AddInfo("귓말하기 도움말 : /w [아이디] [보내실내용] 형식에 맞춰 입력해주세요.\n", GetChatColor(ECC2_NOTIFY),false);
		return true;
	}

	//자동귓말체크
	bool Autowhisper = false;
	if( strnicmp( str, "/w ", 3 ) == 0 || strnicmp( str, "/ㅈ ", 3 ) == 0 )
	{
		Autowhisper = false;
	}
	else if( !g_cWhisperlistComboPopup.IsAllUser() )
	{
		Autowhisper = true;
	}

	if( Autowhisper == false && _strnicmp(str, "/w ", 3) != 0 && strncmp(str, "/ㅈ ", 3) != 0 ) 
	{	
		return false;
	}

	//네이트 귓말막기
	//if( GM().GetMyInfo()->UI.SiteCode==NMBASE::UTIL::SNO_NATE ) return true;

	int i=0;
	int now=0;

	char strChat[256]={0,};
	char strDestNickName[256]={0,};

	if( Autowhisper )
	{
		std::string strWhisper = g_cWhisperlistComboPopup.GetText();
		memcpy( &strDestNickName, strWhisper.c_str(), strWhisper.size() );
		memcpy( &strChat, str, (int)strlen(str) );
	}
	else
	{
		for(i=0; i<(int)strlen(str); i++)
		{
			if(str[i] == ' ')		
				break;		
		}
		i++;

		for(;i<(int)strlen(str); i++)
		{
			if(str[i] == ' ') 					
				break;

			strDestNickName[now] = str[i]; now++;
		}
		i++;

		now=0;
		for(;i<(int)strlen(str); i++)
		{
			strChat[now] = str[i]; now++;
		}
	}

	if(strlen(strDestNickName) == 0 || strlen(strDestNickName) >= 20 || strlen(strChat) == 0) 
	{
		AddInfo("귓말하기 도움말 : /w [아이디] [보내실내용] 형식에 맞춰 입력해주세요.\n", GetChatColor(ECC2_NOTIFY), false);	

		return true;
	}

	int DestUNum=-1;
	int num=-1;

// 	SMALLUSERINFO* pSUI = GetUserSmallInfo( strDestNickName );
// 	//널이어도 리턴금지
// 	if( pSUI == NULL ) DestUNum =-1;
// 	if( pSUI ) DestUNum = pSUI->UNum; 

	CString outstr;

//여기까지

	if (strcmp(strDestNickName, GM().GetMyInfo()->UI.NickName)==0 || strcmp(strDestNickName, GM().GetMyInfo()->UI.ID)==0)
	{
		outstr.Format("[알림] 자기 자신에게는 귓말을 보낼 수 없습니다.\n");
		AddChat(outstr, GetChatColor(ECC2_NOTIFY), false);		
		return true;
	}

	char szTChat[512];
	NMBASE::UTIL::NMIsBadToRep( strChat, szTChat, 512 );

	if( g_RI.State == 1 )
	{
		BOOL bDont = FALSE;
		const int nMaxPlayer = m_pGame->GetMaxPlayer();
		CGameProcess *pProcess = m_pGame->GetGameProcess();
		
		for( int nNum = 0; nNum < nMaxPlayer; ++nNum )
		{
			if( strcmp( strDestNickName, pProcess->GetPlayer(nNum)->UI.NickName ) == 0 || strcmp( strDestNickName, pProcess->GetPlayer(nNum)->UI.ID ) == 0) 
			{
				bDont=TRUE;
				break; 
			}
		}

		// 관리자모드작업
		if( g_bAdminUser ) bDont = FALSE;

		if( bDont )
		{
			outstr.Format("[알림] 게임중에는 같은방 플레이어에게 귓말을 보낼 수 없습니다.\n");
			AddChat( outstr, GetChatColor(ECC2_NOTIFY), false );
			return true;
		}
	}

	outstr.Format( "[귓말보냄:%s] %s\n", strDestNickName , szTChat );
	AddGameChatViewEdit( outstr, GetChatColor(ECC2_WHISPER_SEND), false );

	//귓말아이디추가
	AddChattingWhisperList( strDestNickName, true, false, false );
	//OnAddWhipserUser( true, strDestNickName );

	CPlayer *pPlayer = GM().GetMyInfo();
	
	// 관리자모드작업
	if( !g_bAdminUser )
	{
		// ### [ 관전기능 ] ###		//귓말제거
		GM().SendWhisper(strChat, strDestNickName);

// 		CSV_WHISPERCHAT wcmsg;
// 		wcmsg.Set(pPlayer->UI.ID, pPlayer->UI.NickName,DestUNum, strDestNickName, strChat);
// 		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), wcmsg.pData, wcmsg.GetTotalSize());
	}
	else
	{
		CNM_ADMIN_WHISPERCHAT wcmsg;
		wcmsg.Set(pPlayer->UI.ID, DestUNum, strDestNickName,  strChat);
		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), wcmsg.pData, wcmsg.GetTotalSize());					
	}
	
	CString savechat;
	savechat.Format("[귓말보냄:%s] %s\n", strDestNickName , szTChat );
	AddLobbyHideChatText(&savechat);

	return true;
}

void CChattingWindowMan::SetEvent(void)
{
	if(m_nViewTab != ECTT_EVENT) m_bAddEvent = true;
}

void OnProcessSendChat(const char *str)
{
	CString outstr;

	//2009.11.17 정책 변경 주석 처리
// 	if( !IsValidObserverChat() )
// 	{				
// 		if( GDM().GetBool(GDK_OBSERVER_INFO_MSG) == false )
// 		{	
// 			GDM().SetBool(GDK_OBSERVER_INFO_MSG, true);
// 			AddGameChatViewEdit("[알림]관전자 채팅끄기를 설정하신 분께는\n관전채팅 내용이 보여지지 않습니다\n", s_pChattinWindowMan->GetChatColor(ECC2_NOTIFY));
// 		}
// 	}
	
	// 관전 입장시 게임 참여 가능해야 채팅 가능
	if( GM().GetMyInfo()->JoinState == 0
		&&((g_RI.ChipKind == 0 && g_RI.llEnterLimitMoney >GM().GetMyInfo()->UI.GetMoney())
		|| (g_RI.ChipKind == 1 && g_RI.llEnterLimitMoney > GM().GetMyGold())))
	{
		CString nostr;
		nostr.Format("[알림] 게임에 참여 가능한 보유머니가 있어야 채팅이 가능합니다.\n");
		AddGameChatViewEdit(nostr, s_pChattinWindowMan->GetChatColor(ECC2_NOTIFY));
		return;
	}	

	if(GM().GetMyInfo()->JoinState == 0 && !(g_RI.ChipKind == 1 && g_RI.llEnterLimitMoney >= 500000))
	{
		return;
	}

	// 대화 도배 검사
	if(NoChat.CheckNoChat(str)==TRUE)
	{
		CString nostr;
		nostr.Format("[알림] 대화 도배로 인하여 2분간 채팅이 금지된 상태입니다.\n");
		AddGameChatViewEdit(nostr, s_pChattinWindowMan->GetChatColor(ECC2_NOTIFY));
		return;
	}			

	//if(s_pChattinWindowMan->CheckWhisperSendChat(str)) return;

#ifdef _DEBUG
	// 특수 작업용(개발자 사용목적)
	if(strcmp(str, "/go")==0)
	{
		//GAME()->DoPrepareGame();
		return;
	}

	if(strcmp(str, "/q")==0)
	{
		ExitGameRequest();
		return;
	}

	if(strcmp(str, "/crash")==0)
	{
		char *temp = NULL;
		*temp = 't';
		return;
	}

	//
	//if(_strnicmp(str, "/set ", 5)==0)
	//{
	//	char strDestID[256]={0,};
	//	int i=0;
	//	int now=0;
	//	for(i=0; i<(int)strlen(str); i++)
	//	{
	//		if(str[i] == ' ') break;
	//	}
	//	i++;
	//	
	//	for(;i<(int)strlen(str); i++)
	//	{
	//		if(str[i] == ' ') break;
	//		strDestID[now] = str[i]; now++;
	//	}
	//	i++;
	//	
	//	now=0;
	//	char strChat[256]={0,};
	//	for(;i<(int)strlen(str); i++)
	//	{
	//		strChat[now] = str[i]; now++;
	//	}
	//	
	//	if(strlen(strDestID) == 0) return;
	//	if(strlen(strDestID) >= 16) return;
	//	if(strlen(strChat) == 0) return;				
	//	
	//	INT64 money = _atoi64(strChat);
	//	
	//	if( money < 0 )return;
	//	
	//	CCL_DEBUG_SET_USER msg;
	//	
	//	if( strcmp(strDestID,"me") == 0 ){
	//		strcpy_s(strDestID, sizeof(strDestID), pMyPlayer->UI.ID);
	//		msg.Set( CCL_DEBUG_SET_USER::SET_MONEY,strDestID,money);
	//	}					
	//	else if( strcmp(strDestID , "fam") == 0 ){
	//		strcpy_s(strDestID, sizeof(strDestID), pMyPlayer->UI.ID);
	//		msg.Set( CCL_DEBUG_SET_USER::SET_FAMILY,strDestID,money);
	//	}					
	//	
	//	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
	//	return;
	//}

	//if(_strnicmp(str, "/pcr  ", 5)==0)
	//{
	//	char* id=strtok(&((char *)str)[5], " ");
	//	char* code=strtok(NULL, " ");
	//	if (code==NULL) return;
	//	int pcroomcode=atoi(code);
	//	if (pcroomcode<0 || pcroomcode>2) return;
	//	
	//	if (strcmp(id, "me")==0) id = pMyPlayer->UI.ID;
	//	
	//	if (strcmp(id,"all")==0)
	//	{
	//		if (GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
	//		{
	//			CGameProcess *pProcess = GM().GetCurrentProcess();
	//			CPlayer *pPlayer;
	//			for(int i=0; i<MAX_PLAYER; i++)
	//			{
	//				pPlayer = pProcess->GetPlayer(i);
	//				if (strlen(pPlayer->UI.ID)>0)
	//				{
	//					CCL_DEBUG_SET_USER smmsg;
	//					smmsg.Set(CCL_DEBUG_SET_USER::SET_PCROOM,pPlayer->UI.ID, pcroomcode);
	//					NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), smmsg.pData, smmsg.GetTotalSize());
	//				}
	//			}
	//		}
	//	}
	//	else
	//	{
	//		CCL_DEBUG_SET_USER smmsg;
	//		smmsg.Set(CCL_DEBUG_SET_USER::SET_PCROOM,id, pcroomcode);
	//		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), smmsg.pData, smmsg.GetTotalSize());
	//	}
	//	return;			
	//}	
#endif
	
	CPlayer *pMyPlayer = GM().GetMyInfo();
	// 메인서버에게 채팅 문자열을 보낸다
	CCL_CHATDATA msg;
		// ### [ 관전기능 ] ###  
	if(pMyPlayer->UI.AdminGrade > 0)
		msg.Set(g_RI.RoomNum, "관리자", 0, (char *)str);
	else
		msg.Set(g_RI.RoomNum, pMyPlayer->UI.ID, 0, (char *)str);

	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
}

LRESULT CChattingWindowMan::OnIsComboPopupClose(WPARAM &wParam, LPARAM &lParam)
{
	//귓말 삭제
	/*
	CPoint *pPoint = (CPoint *)lParam;
	ASSERT(pPoint != NULL);

	if(m_btnLift.PtInRect(pPoint->x, pPoint->y)) return FALSE;
	*/
	return TRUE;
}

CChattingWindowMan *GetCurrentChttingWindowMan(void)
{
	return s_pChattinWindowMan;
}