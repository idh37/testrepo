#include "StdAfx.h"
#include "ChatGroup.h"
#include "WhisperlistCombo.h"
#include "../../WidgetID.h"

#define _DEF_WHISPERLIST_ALL_USER	"전체대화"
#define _DEF_MAX_WHIPER_ID 6

CChatGroup::CChatGroup()
:	m_pEditClose(NULL),
	m_pBtnOpen(NULL),
	m_pImgBack(NULL),
	m_pBtnClose(NULL),
	m_pEditOpen(NULL),
	m_pChatList(NULL),
	m_pBtn112(NULL),
	m_bFirst(true),
	m_pComboClose(NULL),
	m_pComboOpen(NULL),
	m_pSprReceiveChat(NULL),
	m_pGrpClose(NULL),
	m_bOpen(false)
{
}

CChatGroup::~CChatGroup()
{
}

void CChatGroup::ShowRoomList(bool bShow)
{
	if(m_bOpen)
	{
		m_pGrpClose->SetShow(false);
		m_pImgBack->SetShow(true);

		m_pBtn112->SetShow(true);
	}
	else
	{
		m_pGrpClose->SetShow(true);
		m_pImgBack->SetShow(false);

		m_pBtn112->SetShow(false);
	}
}

void CChatGroup::OnCreateChild(LSY::CObjectBase *pObject)
{
	if(pObject == NULL) return;

	bool bAddChild = false;
	switch(pObject->GetID())
	{
	case ID_LOBBY_GRP_CHAT_CLOSE:				m_pGrpClose					= (LSY::CGroup *)pObject;		break;
	case ID_LOBBY_EDIT_CHAT_CLOSE:				m_pEditClose				= (LSY::CEdit *)pObject;		break;
	case ID_LOBBY_BTN_CHATTIN_OPEN:				m_pBtnOpen					= (LSY::CButton *)pObject;		bAddChild = true;	break;
	case ID_LOBBY_COMBO_WHISPERLIST_CLOSE:		m_pComboClose				= (CWhisperlistCombo *)pObject;	break;
	case ID_LOBBY_SPR_RECEIVE_WHISPER_CHAT:		m_pSprReceiveChat			= (LSY::CSprite *)pObject;		break;

	case ID_LOBBY_IMG_CHAT_OPEN_BACK:			m_pImgBack					= (LSY::CImage *)pObject;		break;
	case ID_LOBBY_BTN_CHATTIN_CLOSE:			m_pBtnClose					= (LSY::CButton *)pObject;		bAddChild = true;	break;
	case ID_LOBBY_RICH_CHAT_LIST:				m_pChatList					= (LSY::CRichText *)pObject;	break;
	case ID_LOBBY_EDIT_CHAT_OPEN:				m_pEditOpen					= (LSY::CEdit *)pObject;		break;
	case ID_LOBBY_COMBO_WHISPERLIST_OPEN:		m_pComboOpen				= (CWhisperlistCombo *)pObject;	break;

	case ID_LOBBY_BTN_CHAT_112:					m_pBtn112					= (LSY::CButton *)pObject;		bAddChild = true;	break;
	}

	if(bAddChild)
	{
		pObject->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CChatGroup::OnClickButton));
	}

	__super::OnCreateChild(pObject);
}

void CChatGroup::OnCreated(void)
{
	OnCreateChild(m_pGrpClose->GetFirstChildFromID(ID_LOBBY_COMBO_WHISPERLIST_CLOSE));
	OnCreateChild(m_pGrpClose->GetFirstChildFromID(ID_LOBBY_EDIT_CHAT_CLOSE));
	OnCreateChild(m_pGrpClose->GetFirstChildFromID(ID_LOBBY_BTN_CHATTIN_OPEN));
	OnCreateChild(m_pGrpClose->GetFirstChildFromID(ID_LOBBY_SPR_RECEIVE_WHISPER_CHAT));

	OnCreateChild(m_pImgBack->GetFirstChildFromID(ID_LOBBY_BTN_CHATTIN_CLOSE));
	OnCreateChild(m_pImgBack->GetFirstChildFromID(ID_LOBBY_RICH_CHAT_LIST));
	OnCreateChild(m_pImgBack->GetFirstChildFromID(ID_LOBBY_EDIT_CHAT_OPEN));
	OnCreateChild(m_pImgBack->GetFirstChildFromID(ID_LOBBY_COMBO_WHISPERLIST_OPEN));

	m_pEditClose->AddHandler(LSY::EM_O_FOCUSIN, LSY::Fnt(this, &CChatGroup::OnEditFocusIn));
	m_pEditOpen->AddHandler(LSY::EM_O_FOCUSIN, LSY::Fnt(this, &CChatGroup::OnEditFocusIn));

	m_pEditClose->AddHandler(LSY::EM_O_RETURN, LSY::Fnt(this, &CChatGroup::OnEditReturn));
	m_pEditOpen->AddHandler(LSY::EM_O_RETURN, LSY::Fnt(this, &CChatGroup::OnEditReturn));

	// ctrl+c, ctrl+v, ctrl+x 기능 막기
	m_pEditClose->SetUseClipboard(false) ;
	m_pEditOpen->SetUseClipboard(false) ;

	m_pSprReceiveChat->StopAnimation();

	RaiseChatView(false);

	__super::OnCreated();
}

void CChatGroup::AddWhisper(const std::string &strWhisper, const bool &bSetCurWhisper, const bool &bStartAnimation, const bool &bAllUser)
{
	if(bAllUser)
	{
		m_pComboClose->SetAllUser();
		m_pComboOpen->SetAllUser();
	}
	else
	{
		m_pComboClose->AddWhisper(strWhisper, bSetCurWhisper, bStartAnimation);
		m_pComboOpen->AddWhisper(strWhisper, bSetCurWhisper, bStartAnimation);
	}
}

bool CChatGroup::CheckSendChat(IN OUT std::string &strText, OUT std::string &strWhisperUserID, OUT bool &bAllChat)
{
	CString strTemp;
	strTemp = strText.c_str();
	strTemp.Trim();

	bAllChat = false;
	strWhisperUserID = "";

	char str[256]={0,};
	strncpy(str, strTemp.GetString(), 255);

	if(strlen(str)==0) return false;
	if(strlen(str)==1 && str[0]==-1) return false;

	// 문장 끝에 붙는 쓸데 없는 코드를 삭제함
	if(str[strlen(str)-1]==1) str[strlen(str)-1]=0;

	// 허용되지 않은 특수문자를 사용하였다면 공백문자로 바꿈
	int slen = strlen(str);
	for(int s=0; s<slen-1; s++) {
		// 8:backspace, 9:tab, 10:linefeed, 13:carriage return
		if(str[s] == 8 || str[s] == 9/* || str[s] == 10*/ || str[s] == 13) {
			str[s] = ' ';
		}
	}

	CLobbyView *pLobbyView = (CLobbyView *)GetLobbyWindow();
	CLobbyProcess *pLobbyProcess = pLobbyView->GetLobbyProcess();

	if(strnicmp(str, "/set qj", 7)==0)
	{
		int i = min(g_MyGameItem.nNum, MAX_ITEM_NUM-1);
		g_MyGameItem.nNum++;
		if(g_MyGameItem.nNum>MAX_ITEM_NUM) g_MyGameItem.nNum = MAX_ITEM_NUM;
		g_MyGameItem.Item[i].Code=ITEM_SCODE_QUICKJOINER;
		COleDateTime now = g_GetSyncTime();
		now.GetAsSystemTime(g_MyGameItem.Item[i].StartDate);
		g_MyGameItem.Item[i].Use=1; 
		g_MyGameItem.Item[i].UseDay=100;
	
		pLobbyProcess->RefreshQJButton();
		return false;
	}

	if(strnicmp(str, "/set ", 5)==0)
	{
		char* id=strtok(&str[5], " ");
		char* money=strtok(NULL, " ");
		char* unit=strtok(NULL, " ");
		if (money==NULL) return false;
		INT64 pmoney=_atoi64(money);
		if (pmoney<0) return false;
		
		if (unit!=NULL)
		{
			if (strcmp(unit, "만")==0)
			{
				pmoney *= MONEY_MAN;
			}
			else if (strcmp(unit, "억")==0)
			{
				pmoney *= MONEY_UK;
			}
			else if (strcmp(unit, "조")==0)
			{
				pmoney *= MONEY_JO;
			}
		}

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
		return false;
	}

	if(strnicmp(str, "/add ", 5)==0)
	{
		char* id=strtok(&str[5], " ");
		char* money=strtok(NULL, " ");
		if (money==NULL) return false;
		INT64 pmoney=_atoi64(money);
		if (pmoney<0) return false;

		if (strcmp(id, "me")==0)
		{
			//INT64 mymoney = GM().GetMyInfo()->UI.GetMoney(IsTournament());
			//GM().GetMyInfo()->UI.SetMoney(mymoney+pmoney, IsTournament());

			/*INT64 mymoney = GM().GetMyInfo()->UI.GetMoney();
			GM().GetMyInfo()->UI.SetMoney(mymoney+pmoney);*/

			INT64 mymoney = GM().GetMyInfo()->UI.GetRealPMoney();
			GM().GetMyInfo()->UI.SetRealPMoney(mymoney+pmoney);
		}
		return false;
	}	

	if(stricmp(str, "/weblogin")==0)
	{
		CLobbyEventDlg dlg(GM().GetMainWnd());
		dlg.DoModal("http://login.netmarble.net/Login/Login.asp?L_etc=IsAdult&l_url=http%3A%2F%2Fgame1%2Enetmarble%2Enet%2Fpoker%2Findex%2Easp%3Fgame%3Dspoker2");
		return false;
	}

	strTemp = str;

	/* 귓말 삭제하기 2018.03 단축키 삭제
	//모두에게 채팅을 보내는 모드로 변경됨
	if(_stricmp(str, "/a") == 0 || strcmp(str, "/ㅁ") == 0)
	{
		bAllChat = true;
		return false;
	}
	
	if(_stricmp(str, "/w") == 0 || strcmp(str, "/ㅈ") == 0)
	{
		AddLobbyChatViewEdit("귓말하기 도움말 : /w [아이디] [보내실내용] 형식에 맞춰 입력해주세요.\n", RGB(0, 44, 93));
		return false;
	}

	strText = strTemp.GetString();

	if(strnicmp(str, "/a ", 3) == 0 || strncmp(str, "/ㅁ ", 3) == 0)
	{
		if(strnicmp(str, "/a ", 3) == 0)
		{
			strTemp.Right(strTemp.GetLength() - 3);
		}
		else
		{
			strTemp.Right(strTemp.GetLength() - 4);
		}
		strTemp.TrimLeft();
		strText = strTemp.GetString();
		bAllChat = true;
		return true;
	}

	if(strnicmp(str, "/w ", 3)==0 || strncmp(str, "/ㅈ ", 3)==0)
	{
		char strDestNickName[256]={0,};
		int i=0;
		int now=0;
		for(i=0; i<(int)strlen(str); i++)
		{
			if(str[i] == ' ') break;
		}
		i++;

		for(;i<(int)strlen(str); i++)
		{
			if(str[i] == ' ') break;
			strDestNickName[now] = str[i]; now++;
		}
		i++;

		now=0;
		char strChat[256]={0,};
		for(;i<(int)strlen(str); i++)
		{
			strChat[now] = str[i]; now++;
		}

		if(strlen(strDestNickName) == 0 || strlen(strDestNickName) >= 20 || strlen(strChat) == 0) 
		{
			AddLobbyChatViewEdit("귓말하기 도움말 : /w [아이디] [보내실내용] 형식에 맞춰 입력해주세요.\n", RGB(0, 44, 93));	
			return false;
		}

		int DestUNum=-1;
		int num=-1;

		if (strcmp(strDestNickName, GM().GetMyInfo()->UI.NickName)==0 || strcmp(strDestNickName, GM().GetMyInfo()->UI.ID)==0)
		{
			ShowMainMessageDlg("자기 자신에게는 귓말을 보낼 수 없습니다.");
			return false;
		}

		strTemp = strChat;
		strTemp.TrimLeft();
		strText = strTemp.GetString();

		strWhisperUserID = strDestNickName;
		return true;
	}
	*/

	return true;
}

LSY::CObjectBase *CChatGroup::NewObject(void)
{
	return new CChatGroup;
}

LRESULT CChatGroup::OnEditFocusIn(LSY::CMessage *msg)
{
	LSY::CMO_FocusIn *pMsg = (LSY::CMO_FocusIn *)msg;
	LSY::CEdit *pEdit = (LSY::CEdit *)pMsg->GetObject();
	if(pEdit->GetText() == "채팅내용을 입력하세요 [Enter]")
	{
		m_pEditClose->SetText("");
		m_pEditOpen->SetText("");
	}

	return NULL;
}

LRESULT CChatGroup::OnEditReturn(LSY::CMessage *msg)
{
	LSY::CMO_Return *pMsg = (LSY::CMO_Return *)msg;
	LSY::CEdit *pEdit = (LSY::CEdit *)pMsg->GetObject();
	std::string strText = pEdit->GetText();
	if(strText == "") return TRUE;

	if( CheckPapering( (char *)strText.c_str() ) )
		return TRUE;

	pEdit->SetText("");

	bool bAllUser = false;
	std::string strWhisperUserID;

	bool bSendMsg = CheckSendChat(strText, strWhisperUserID, bAllUser);

	if(bAllUser)
	{
		AddChattingWhisperList("", false, false, true);
	}
	else if(strWhisperUserID != "")
	{
		AddChattingWhisperList(strWhisperUserID.c_str(), true, false, false);
	}
	else if(!m_pComboClose->IsAllUser())
	{
		strWhisperUserID = m_pComboClose->GetText();
	}

	if(bSendMsg == false) return FALSE;

	if(bAllUser || strWhisperUserID == "")
	{
		CSV_CHATDATA msg;
		if(GM().GetMyInfo()->UI.AdminGrade > 0)
			msg.Set("관리자","관리자",  strText.c_str());
		else
			msg.Set(GM().GetMyInfo()->UI.ID, GM().GetMyInfo()->UI.NickName, strText.c_str());

		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
	}
	else
	{
		char szTChat[NMBASE::SKINGDI::MAX_TEXT_LEN] = {0,};
		NMBASE::UTIL::NMIsBadToRep( (char *)strText.c_str(), szTChat, NMBASE::SKINGDI::MAX_TEXT_LEN-1 );

		// ### [ 관전기능 ] ###
		if (!g_bAdminUser)
		{
			CString outstr;	
			outstr.Format("[%s]님에게 귓말 : %s\n", strWhisperUserID.c_str() , szTChat );	
			AddLobbyChatViewEdit(outstr, RGB(90, 0, 167));	

			GM().SendWhisper(strText.c_str(), strWhisperUserID.c_str());
		}
		else
		{
			CNM_ADMIN_WHISPERCHAT wcmsg;
			wcmsg.Set(GM().GetMyInfo()->UI.ID, -1, strWhisperUserID.c_str(),  (char *)strText.c_str());
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), wcmsg.pData, wcmsg.GetTotalSize());					
		}

		CString savechat;		
		savechat.Format("[%s]님에게 귓말 : %s\n", strWhisperUserID.c_str() , szTChat );			//- lorddan ( 2008.11.20 ) - Hide Chat 수정
		AddLobbyHideChatText(&savechat);
	}

	RaiseChatView(true);

	return TRUE;
}

LRESULT CChatGroup::OnClickButton(LSY::CMessage *msg)
{
	LSY::CMO_MouseLClick *pMsg = (LSY::CMO_MouseLClick *)msg;
	LSY::CButton *pBtn = (LSY::CButton *)pMsg->GetObject();
	CWnd *pWnd = GetObjectMan()->GetWnd();
	switch(pBtn->GetID())
	{
	case ID_LOBBY_BTN_CHATTIN_OPEN:
		SendUILog_Basic(CSSTATS_UINO_POKER_LOBBY_CHAT_OPEN);
		RaiseChatView(true);
		break;
	case ID_LOBBY_BTN_CHATTIN_CLOSE:
		SendUILog_Basic(CSSTATS_UINO_POKER_LOBBY_CHAT_CLOSE);
		RaiseChatView(false);
		break;
	case ID_LOBBY_BTN_CHAT_112:
		pWnd->SendMessage(LCV_BUTTON_ID_112);
		break;
	}
	return TRUE;
}

void CChatGroup::RaiseChatView(bool bShow)
{
	if(m_bOpen == bShow) return;

	m_bOpen = bShow;

	CLobbyView *pLobbyView = (CLobbyView *)GetLobbyWindow();
	if(m_bOpen)
	{
		m_pGrpClose->SetShow(false);
		m_pImgBack->SetShow(true);

		m_pBtn112->SetShow(true);
		m_pSprReceiveChat->StopAnimation();
		m_pEditOpen->SetText(m_pEditClose->GetText());
		pLobbyView->SetTargetNotiPos(377, 428);
	}
	else
	{
		m_pGrpClose->SetShow(true);
		m_pImgBack->SetShow(false);

		m_pBtn112->SetShow(false);
		m_pEditClose->SetText(m_pEditOpen->GetText());
		pLobbyView->SetTargetNotiPos(377, 575);
	}
}

void CChatGroup::EnableChat(bool bEnable)
{
	if (bEnable)
	{
		if(!m_bFirst)
		{
			m_pEditClose->SetText("");
			m_pEditOpen->SetText("");
		}

		m_pEditClose->SetEnable(true);
		m_pEditOpen->SetEnable(true);

		m_pBtnOpen->SetEnable(true);
	}
	else
	{
		RaiseChatView(false);

		m_pEditClose->SetText("최상위 보유머니 구간에서는 대기실 채팅 기능을 이용할 수 없습니다.");
		m_pEditOpen->SetText("최상위 보유머니 구간에서는 대기실 채팅 기능을 이용할 수 없습니다.");

		m_pEditClose->SetEnable(false);
		m_pEditOpen->SetEnable(false);

		m_pBtnOpen->SetEnable(false);
	}

	if(m_bFirst)
	{
		m_bFirst = false;
	}
}

void CChatGroup::SetFocusChat(void)
{
	if(m_bOpen) GetObjectMan()->SetFocus(m_pEditOpen);
	else GetObjectMan()->SetFocus(m_pEditClose);
}

void CChatGroup::ClearChatHistory(void)
{
	m_pChatList->Clear();
}

void CChatGroup::AddText(const char *pMsg, COLORREF color)
{
	// 4 이상 연속 숫자는 ****로 표시하기
	std::string msg = pMsg;
	msg = g_ConverNumberToAsterisk(msg);
	pMsg = msg.c_str();

	LSY::FONTINFOLIST listFont = m_pChatList->GetFontList();
	LSY::FONTINFOLISTITER start = listFont.begin();
	LSY::FONTINFOLISTITER end = listFont.end();

	int nIndex = 0;
	for(;start != end;++start, ++nIndex)
	{
		if(start->m_clrColor == color)
			break;
	}

	if(start == end)
	{
		LSY::FONTINFO font = *listFont.begin();
		font.m_clrColor = color;
		m_pChatList->AddFont(font);
		nIndex = listFont.size();
	}

	m_pChatList->SetCurrentFont(nIndex);
	m_pChatList->AddText(pMsg);
}

bool CChatGroup::CheckPapering( char* szBuf )
{
	if( szBuf == NULL )	return false;

	if( m_PaperingChat.CheckNoChat( szBuf ) == TRUE )
	{
		CString strTemp = "[알림] 대화 도배로 인하여 2분간 채팅이 금지된 상태입니다.\n" ;

		CWnd *pWnd = GetObjectMan()->GetWnd();
		pWnd->SendMessage(LCV_EDIT_SENDCHAT, (WPARAM)NMBASE::SKINGDI::WIDGET_ID_EDIT_WARNING, (LPARAM)(&strTemp));

		return true;
	}

	return false;
}

void CChatGroup::StartRecevieChatAnimation(void)
{
	if(!m_pImgBack->IsShow())
	{
		m_pSprReceiveChat->StartAnimation();
	}
}
