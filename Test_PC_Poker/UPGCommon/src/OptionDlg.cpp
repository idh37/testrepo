#include "stdafx.h"
#include "OptionDlg.h"
#include "OptionDlgID.h"
#include "GlobalBase.h"

#include "OptionFrameBase.h"
#include "OptionFrameSutda.h"
#include "OptionFrameHoola.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

COptionDlg::COptionDlg(CWnd* pParent /*=NULL*/)
	: LSY::CLSYLibDialogBase(pParent),
	m_pMyConfig(NULL),
	m_OptionFrame(NULL),
	m_eConfigGameType(CONFIG_GAME_TYPE_ALLGAME)
{

}

COptionDlg::~COptionDlg()
{
	// 메모리 해제
	m_OptionFrame.reset();
	m_OptionFrame = nullptr;
}

BEGIN_MESSAGE_MAP(COptionDlg, LSY::CLSYLibDialogBase)
END_MESSAGE_MAP()


bool COptionDlg::OnCreatedProject(void)
{
	ASSERT(m_pMyConfig != NULL);

	// 옵션 프레임 생성
	m_OptionFrame = CreateOptionFrame(m_eConfigGameType);

	// 활성화
	m_OptionFrame->Init();
	m_OptionFrame->Show();

	// 적용 버튼
	LSY::CButton *pButton = (LSY::CButton *)GetObject(ID_OPTION_BTN_OK);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &COptionDlg::OnClickButton));

	// 취소 버튼
	pButton = (LSY::CButton *)GetObject(ID_OPTION_BTN_CANCEL);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &COptionDlg::OnClickButton));

	// 상단 탭 버튼
	LSY::CTabControl *pTab = (LSY::CTabControl *)GetObject(ID_OPTION_TAB_GAMETYPE);
	pTab->AddHandler(LSY::EM_O_SELCHANGE, LSY::Fnt(this, &COptionDlg::OnSelChangeOptionType));

	return true;
}

INT_PTR COptionDlg::DoModal(IDX_GAME eGame, bool bMySex, bool bGameOptionTab/* = true*/)
{
	m_pMyConfig = &g_Config;
	m_bGameOptionTab = bGameOptionTab;
	m_bSex = bMySex;

	if (eGame == IDX_GAME_SD)
	{
		m_eConfigGameType = CONFIG_GAME_TYPE_SUTDA;
	}
	else if (eGame == IDX_GAME_HA)
	{
		m_eConfigGameType = CONFIG_GAME_TYPE_HOOLA;
	}
	else
	{
		m_eConfigGameType = CONFIG_GAME_TYPE_ALLGAME;
	}

	return LSY::CLSYLibDialogBase::DoModal(GetObjectMan(), ID_OPTION);
}


shared_ptr<COptionFrameBase> COptionDlg::CreateOptionFrame(CONFIG_GAME_TYPE gameType)
{
	if (gameType == CONFIG_GAME_TYPE_SUTDA)
	{
		return make_shared<COptionFrameSutda>(this);
	}
	else if (gameType == CONFIG_GAME_TYPE_HOOLA)
	{
		return make_shared<COptionFrameHoola>(this);
	}
	else
	{
		return make_shared<COptionFrameBase>(this);
	}
}

LRESULT COptionDlg::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CMO_MouseLClick *msg = (LSY::CMO_MouseLClick *)pMsg;
	switch (msg->GetObject()->GetID())
	{
	case ID_OPTION_BTN_OK:
		OnOK();
		break;
	case ID_OPTION_BTN_CANCEL:
		OnCancel();
		break;
	}
	return TRUE;
}

LRESULT COptionDlg::OnCurValueChange(LSY::CMessage *pMsg)
{
	LSY::CMO_CurValueChange *msg = (LSY::CMO_CurValueChange *)pMsg;

	int id = msg->GetObject()->GetID();
	switch (id)
	{
	case ID_OPTION_SLIDE_EFFECT_SOUND:
	case ID_OPTION_SLIDE_EFFECT_SOUND_SUTDA:
	case ID_OPTION_SLIDE_EFFECT_SOUND_HOOLA:
	{
		int nCurValue = (int)msg->GetValue();
		NMBASE::SOUND::g_Sound.SetSndVolume(nCurValue);
		NMBASE::SKINGDI::NMRESAM.SetVolumn(nCurValue);
	}
	break;
	case ID_OPTION_SLIDE_BGM_SOUND_SUTDA:
	case ID_OPTION_SLIDE_BGM_SOUND_HOOLA:
	{
		int nCurValue = (int)msg->GetValue();
		GM().GetCurrentGame()->SetBGMVolume(nCurValue);
	}
	break;
	}

	return TRUE;
}

LRESULT COptionDlg::OnSelChangeOptionType(LSY::CMessage *msg)
{
	LSY::CMO_SelChange *pMsg = (LSY::CMO_SelChange *)msg;
	int nIndex = pMsg->GetValue();

#ifdef UPGRADE_10210619_ADD_CLICKLOG
	if (nIndex == 0)
	{
		if (GM().GetCurrentWhere() == IDX_GAMEWHERE_LOBY)
			SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_TITLEBAR_OPTION_GAMEOPTION);
		else if (GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
			SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_TITLEBAR_OPTION_GAMEOPTION);
	}
	else if (nIndex == 1)
	{
		if (GM().GetCurrentWhere() == IDX_GAMEWHERE_LOBY)
			SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_TITLEBAR_OPTION_RECEIVEOPTION);
		else if (GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
			SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_TITLEBAR_OPTION_RECEIVEOPTION);
	}
#endif //UPGRADE_10210619_ADD_CLICKLOG

	return TRUE;
}

LRESULT COptionDlg::OnSelChangeEffectSound(LSY::CMessage *msg)
{
	LSY::CMO_SelChange *pMsg = (LSY::CMO_SelChange *)msg;
	int nIndex = pMsg->GetValue();

#ifdef UPGRADE_10210619_ADD_CLICKLOG
	if (nIndex == 0)
	{
		if (GM().GetCurrentWhere() == IDX_GAMEWHERE_LOBY)
			SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_TITLEBAR_OPTION_GAMEOPTION_SOUND_ON);
		else if (GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
			SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_TITLEBAR_OPTION_GAMEOPTION_SOUND_ON);
	}
	else if (nIndex == 1)
	{
		if (GM().GetCurrentWhere() == IDX_GAMEWHERE_LOBY)
			SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_TITLEBAR_OPTION_GAMEOPTION_SOUND_OFF);
		else if (GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
			SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_TITLEBAR_OPTION_GAMEOPTION_SOUND_OFF);
	}
#endif //UPGRADE_10210619_ADD_CLICKLOG

	return TRUE;
}

LRESULT COptionDlg::OnSelChangeBetBtnType(LSY::CMessage *msg)
{
	LSY::CMO_SelChange *pMsg = (LSY::CMO_SelChange *)msg;
	int nIndex = pMsg->GetValue();

#ifdef UPGRADE_10210619_ADD_CLICKLOG
	if (nIndex == 0)
	{
		if (GM().GetCurrentWhere() == IDX_GAMEWHERE_LOBY)
			SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_TITLEBAR_OPTION_GAMEOPTION_BETBTN_N);
		else if (GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
			SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_TITLEBAR_OPTION_GAMEOPTION_BETBTN_N);
	}
	else if (nIndex == 1)
	{
		if (GM().GetCurrentWhere() == IDX_GAMEWHERE_LOBY)
			SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_TITLEBAR_OPTION_GAMEOPTION_BETBTN_H);
		else if (GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
			SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_TITLEBAR_OPTION_GAMEOPTION_BETBTN_H);
	}
	else if (nIndex == 2)
	{
		if (GM().GetCurrentWhere() == IDX_GAMEWHERE_LOBY)
			SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_TITLEBAR_OPTION_GAMEOPTION_BETBTN_P);
		else if (GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
			SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_TITLEBAR_OPTION_GAMEOPTION_BETBTN_P);
	}
#endif //UPGRADE_10210619_ADD_CLICKLOG

	return TRUE;
}

LRESULT COptionDlg::OnSelChangeHotKeyType(LSY::CMessage *msg)
{
	LSY::CMO_SelChange *pMsg = (LSY::CMO_SelChange *)msg;
	int nIndex = pMsg->GetValue();

#ifdef UPGRADE_10210619_ADD_CLICKLOG
	if (nIndex == 0)
	{
		if (GM().GetCurrentWhere() == IDX_GAMEWHERE_LOBY)
			SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_TITLEBAR_OPTION_GAMEOPTION_CUTBTN_FKEY);
		else if (GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
			SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_TITLEBAR_OPTION_GAMEOPTION_CUTBTN_FKEY);
	}
	else if (nIndex == 1)
	{
		if (GM().GetCurrentWhere() == IDX_GAMEWHERE_LOBY)
			SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_TITLEBAR_OPTION_GAMEOPTION_CUTBTN_NUM);
		else if (GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
			SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_TITLEBAR_OPTION_GAMEOPTION_CUTBTN_NUM);
	}
#endif //UPGRADE_10210619_ADD_CLICKLOG

	return TRUE;
}

LRESULT COptionDlg::OnSelChangeObserverChattingType(LSY::CMessage *msg)
{
	LSY::CMO_SelChange *pMsg = (LSY::CMO_SelChange *)msg;
	int nIndex = pMsg->GetValue();

#ifdef UPGRADE_10210619_ADD_CLICKLOG
	if (nIndex == 0)
	{
		if (GM().GetCurrentWhere() == IDX_GAMEWHERE_LOBY)
			SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_TITLEBAR_OPTION_GAMEOPTION_SHOW_OBSERVER_CHAT_ON);
		else if (GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
			SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_TITLEBAR_OPTION_GAMEOPTION_OBSERVER_SHOWCHAT_ON);
	}
	else if (nIndex == 1)
	{
		if (GM().GetCurrentWhere() == IDX_GAMEWHERE_LOBY)
			SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_TITLEBAR_OPTION_GAMEOPTION_SHOW_OBSERVER_CHAT_OFF);
		else if (GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
			SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_TITLEBAR_OPTION_GAMEOPTION_OBSERVER_SHOWCHAT_OFF);
	}
#endif //UPGRADE_10210619_ADD_CLICKLOG

	return TRUE;
}

LRESULT COptionDlg::OnSelChangeNotificationMemoType(LSY::CMessage *msg)
{
	return FALSE;

	LSY::CMO_SelChange *pMsg = (LSY::CMO_SelChange *)msg;
	int nIndex = pMsg->GetValue();

#ifdef UPGRADE_10210619_ADD_CLICKLOG
	if (nIndex == 0)
	{
		if (GM().GetCurrentWhere() == IDX_GAMEWHERE_LOBY)
			SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_TITLEBAR_OPTION_GAMEOPTION_MEMO_ALARM_ON);
		else if (GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
			SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_TITLEBAR_OPTION_GAMEOPTION_MEMO_ALARM_ON);
	}
	else if (nIndex == 1)
	{
		if (GM().GetCurrentWhere() == IDX_GAMEWHERE_LOBY)
			SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_TITLEBAR_OPTION_GAMEOPTION_MEMO_ALARM_OFF);
		else if (GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
			SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_TITLEBAR_OPTION_GAMEOPTION_MEMO_ALARM_OFF);
	}
#endif //UPGRADE_10210619_ADD_CLICKLOG

	return TRUE;
}

LRESULT COptionDlg::OnSelChangeNotificationConnectBuddyType(LSY::CMessage *msg)
{
	LSY::CMO_SelChange *pMsg = (LSY::CMO_SelChange *)msg;
	int nIndex = pMsg->GetValue();

#ifdef UPGRADE_10210619_ADD_CLICKLOG
	if (nIndex == 0)
	{
		if (GM().GetCurrentWhere() == IDX_GAMEWHERE_LOBY)
			SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_TITLEBAR_OPTION_GAMEOPTION_FRIEND_LOGIN_ALARM_ON);
		else if (GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
			SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_TITLEBAR_OPTION_GAMEOPTION_FRIEND_LOGIN_ALARM_ON);
	}
	else if (nIndex == 1)
	{
		if (GM().GetCurrentWhere() == IDX_GAMEWHERE_LOBY)
			SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_TITLEBAR_OPTION_GAMEOPTION_FRIEND_LOGIN_ALARM_OFF);
		else if (GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
			SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_TITLEBAR_OPTION_GAMEOPTION_FRIEND_LOGIN_ALARM_OFF);
	}
#endif //UPGRADE_10210619_ADD_CLICKLOG

	return TRUE;
}

LRESULT COptionDlg::OnSelChangeReceiveInvitationType(LSY::CMessage *msg)
{
	LSY::CMO_SelChange *pMsg = (LSY::CMO_SelChange *)msg;
	int nIndex = pMsg->GetValue();

#ifdef UPGRADE_10210619_ADD_CLICKLOG
	if (nIndex == 0)
	{
		if (GM().GetCurrentWhere() == IDX_GAMEWHERE_LOBY)
			SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_TITLEBAR_OPTION_RECEIVEOPTION_REQUEST_INVITE_FROMFRIEND);
		else if (GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
			SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_TITLEBAR_OPTION_RECEIVEOPTION_REQUEST_INVITE_FROMFRIEND);
	}
	else if (nIndex == 1)
	{
		if (GM().GetCurrentWhere() == IDX_GAMEWHERE_LOBY)
			SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_TITLEBAR_OPTION_RECEIVEOPTION_REQUEST_INVITE_ALL);
		else if (GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
			SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_TITLEBAR_OPTION_RECEIVEOPTION_REQUEST_INVITE_ALL);
	}
	else if (nIndex == 2)
	{
		if (GM().GetCurrentWhere() == IDX_GAMEWHERE_LOBY)
			SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_TITLEBAR_OPTION_RECEIVEOPTION_REQUEST_INVITE_IGNORE);
		else if (GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
			SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_TITLEBAR_OPTION_RECEIVEOPTION_REQUEST_INVITE_IGNORE);
	}
#endif //UPGRADE_10210619_ADD_CLICKLOG

	return TRUE;
}

LRESULT COptionDlg::OnSelChangeReceiveChatType(LSY::CMessage *msg)
{
	LSY::CMO_SelChange *pMsg = (LSY::CMO_SelChange *)msg;
	int nIndex = pMsg->GetValue();

#ifdef UPGRADE_10210619_ADD_CLICKLOG
	if (nIndex == 0)
	{
		if (GM().GetCurrentWhere() == IDX_GAMEWHERE_LOBY)
			SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_TITLEBAR_OPTION_RECEIVEOPTION_REQUEST_TALK_FROMFRIEND);
		else if (GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
			SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_TITLEBAR_OPTION_RECEIVEOPTION_REQUEST_TALK_FROMFRIEND);
	}
	else if (nIndex == 1)
	{
		if (GM().GetCurrentWhere() == IDX_GAMEWHERE_LOBY)
			SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_TITLEBAR_OPTION_RECEIVEOPTION_REQUEST_TALK_ALL);
		else if (GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
			SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_TITLEBAR_OPTION_RECEIVEOPTION_REQUEST_TALK_ALL);
	}
	else if (nIndex == 2)
	{
		if (GM().GetCurrentWhere() == IDX_GAMEWHERE_LOBY)
			SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_TITLEBAR_OPTION_RECEIVEOPTION_REQUEST_TALK_IGNORE);
		else if (GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
			SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_TITLEBAR_OPTION_RECEIVEOPTION_REQUEST_TALK_IGNORE);
	}
#endif //UPGRADE_10210619_ADD_CLICKLOG

	return TRUE;
}

LRESULT COptionDlg::OnSelChangeReceiveWhisperType(LSY::CMessage *msg)
{
	LSY::CMO_SelChange *pMsg = (LSY::CMO_SelChange *)msg;
	int nIndex = pMsg->GetValue();

#ifdef UPGRADE_10210619_ADD_CLICKLOG
	if (nIndex == 0)
	{
		if (GM().GetCurrentWhere() == IDX_GAMEWHERE_LOBY)
			SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_TITLEBAR_OPTION_RECEIVEOPTION_REQUEST_WHISPER_FROMFRIEND);
		else if (GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
			SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_TITLEBAR_OPTION_RECEIVEOPTION_REQUEST_WHISPER_FROMFRIEND);
	}
	else if (nIndex == 1)
	{
		if (GM().GetCurrentWhere() == IDX_GAMEWHERE_LOBY)
			SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_TITLEBAR_OPTION_RECEIVEOPTION_REQUEST_WHISPER_ALL);
		else if (GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
			SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_TITLEBAR_OPTION_RECEIVEOPTION_REQUEST_WHISPER_ALL);
	}
	else if (nIndex == 2)
	{
		if (GM().GetCurrentWhere() == IDX_GAMEWHERE_LOBY)
			SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_TITLEBAR_OPTION_RECEIVEOPTION_REQUEST_WHISPER_IGNORE);
		else if (GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
			SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_TITLEBAR_OPTION_RECEIVEOPTION_REQUEST_WHISPER_IGNORE);
	}
#endif //UPGRADE_10210619_ADD_CLICKLOG

	return TRUE;
}

LRESULT COptionDlg::OnSelChangeReceiveMemoType(LSY::CMessage *msg)
{
	LSY::CMO_SelChange *pMsg = (LSY::CMO_SelChange *)msg;
	int nIndex = pMsg->GetValue();

#ifdef UPGRADE_10210619_ADD_CLICKLOG
	if (nIndex == 0)
	{
		if (GM().GetCurrentWhere() == IDX_GAMEWHERE_LOBY)
			SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_TITLEBAR_OPTION_RECEIVEOPTION_REQUEST_MEMO_FROMFRIEND);
		else if (GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
			SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_TITLEBAR_OPTION_RECEIVEOPTION_REQUEST_MEMO_FROMFRIEND);
	}
	else if (nIndex == 1)
	{
		if (GM().GetCurrentWhere() == IDX_GAMEWHERE_LOBY)
			SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_TITLEBAR_OPTION_RECEIVEOPTION_REQUEST_MEMO_ALL);
		else if (GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
			SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_TITLEBAR_OPTION_RECEIVEOPTION_REQUEST_MEMO_ALL);
	}
	else if (nIndex == 2)
	{
		if (GM().GetCurrentWhere() == IDX_GAMEWHERE_LOBY)
			SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_TITLEBAR_OPTION_RECEIVEOPTION_REQUEST_MEMO_IGNORE);
		else if (GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
			SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_TITLEBAR_OPTION_RECEIVEOPTION_REQUEST_MEMO_IGNORE);
	}
#endif //UPGRADE_10210619_ADD_CLICKLOG

	return TRUE;
}

void COptionDlg::OnOK()
{
	LSY::CGroup *pGroup = NULL;
	LSY::CSlideBar *pSlide = NULL;

#ifdef UPGRADE_10210619_ADD_CLICKLOG
	LSY::CTabControl *pTab = (LSY::CTabControl *)GetObject(ID_OPTION_TAB_GAMETYPE);
	LSY::CRadio *pRadio = pTab->GetCurSel();

	if (pRadio->GetID() == ID_OPTION_RAD_GAMEOPTION)
	{
		if (GM().GetCurrentWhere() == IDX_GAMEWHERE_LOBY)
			SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_TITLEBAR_OPTION_GAMEOPTION_SET);
		else if (GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
			SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_TITLEBAR_OPTION_GAMEOPTION_SET);
	}

#endif //UPGRADE_10210619_ADD_CLICKLOG

	// 설정 저장
	m_OptionFrame->Save();
	//		GM().GetCurrentGameView()->SetupByConfig();
	GM().GetCurrentGame()->SetupByConfig();

	NMBASE::SOUND::g_Sound.PlayWav(SND_BUTTON_CLICK_A);
	LSY::CLSYLibDialogBase::OnOK();
}

void COptionDlg::OnCancel()
{
#ifdef UPGRADE_10210619_ADD_CLICKLOG
	LSY::CTabControl *pTab = (LSY::CTabControl *)GetObject(ID_OPTION_TAB_GAMETYPE);
	LSY::CRadio *pRadio = pTab->GetCurSel();

	if (pRadio->GetID() == ID_OPTION_RAD_GAMEOPTION)
	{
		if (GM().GetCurrentWhere() == IDX_GAMEWHERE_LOBY)
			SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_TITLEBAR_OPTION_GAMEOPTION_CANCEL);
		else if (GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
			SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_TITLEBAR_OPTION_GAMEOPTION_CANCEL);
	}

#endif //UPGRADE_10210619_ADD_CLICKLOG

	NMBASE::SOUND::g_Sound.SetSndVolume(m_pMyConfig->FxVolume);
	NMBASE::SKINGDI::NMRESAM.SetVolumn(m_pMyConfig->FxVolume);
	NMBASE::SOUND::g_Sound.PlayWav(SND_BUTTON_CLICK_A);
	LSY::CLSYLibDialogBase::OnCancel();
}