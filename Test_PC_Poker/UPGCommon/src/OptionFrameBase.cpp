#include "stdafx.h"
#include "OptionFrameBase.h"

#include "OptionDlg.h"
#include "OptionDlgID.h"

COptionFrameBase::COptionFrameBase(COptionDlg* dlg)
{
	m_OptionDlg = dlg;
}

COptionFrameBase::~COptionFrameBase()
{
}

void COptionFrameBase::Init()
{
	// 효과음
	LSY::CGroup* pEffectSound = (LSY::CGroup *)m_OptionDlg->GetObject(ID_OPTION_GRP_EFFECT_SOUND);
	pEffectSound->SetCurSelFromID(m_OptionDlg->m_pMyConfig->bSndFX ? ID_OPTION_RADIO_EFFECTSOUND_ON : ID_OPTION_RADIO_EFFECTSOUND_OFF);
	pEffectSound->AddHandler(LSY::EM_O_SELCHANGE, LSY::Fnt(m_OptionDlg, &COptionDlg::OnSelChangeEffectSound));

	// 배팅 버튼 타입
	LSY::CGroup* pBettingButton = (LSY::CGroup *)m_OptionDlg->GetObject(ID_OPTION_GRP_BETTINGBUTTON_TYPE);
	pBettingButton->SetCurSelFromID(ID_OPTION_RADIO_BETTINGBUTTON_NTYPE + (int)m_OptionDlg->m_pMyConfig->eBetBtnType);
	pBettingButton->AddHandler(LSY::EM_O_SELCHANGE, LSY::Fnt(m_OptionDlg, &COptionDlg::OnSelChangeBetBtnType));

	// 로우바둑이 단축키
	LSY::CGroup* pHotKeyRadio = (LSY::CGroup *)m_OptionDlg->GetObject(ID_OPTION_GRP_CUTTING_HOTKEY_TYPE);
	pHotKeyRadio->SetCurSelFromID(m_OptionDlg->m_pMyConfig->bCutStyle ? ID_OPTION_RADIO_CUTTING_HOTKEY_FUNCTION : ID_OPTION_RADIO_CUTTING_HOTKEY_NUMBER);
	pHotKeyRadio->AddHandler(LSY::EM_O_SELCHANGE, LSY::Fnt(m_OptionDlg, &COptionDlg::OnSelChangeHotKeyType));

	// 관전자 채팅
	LSY::CGroup* pObserverChatRadio = (LSY::CGroup *)m_OptionDlg->GetObject(ID_OPTION_GRP_OBSERVER_CHATTING);
	pObserverChatRadio->SetCurSelFromID(m_OptionDlg->m_pMyConfig->bNoObserverChat ? ID_OPTION_RADIO_OBSERVER_CHATTING_OFF : ID_OPTION_RADIO_OBSERVER_CHATTING_ON);
	pObserverChatRadio->AddHandler(LSY::EM_O_SELCHANGE, LSY::Fnt(m_OptionDlg, &COptionDlg::OnSelChangeObserverChattingType));

	// 친구 접속 알림
	LSY::CGroup* pNotificationConnectBuddyRadio = (LSY::CGroup *)m_OptionDlg->GetObject(ID_OPTION_GRP_NOTIFICATION_CONNECT_BUDDY);
	pNotificationConnectBuddyRadio->SetCurSelFromID(m_OptionDlg->m_pMyConfig->bNotificationConnectBuddy ? ID_OPTION_RADIO_NOTIFICATION_CONNECT_BUDDY_ON : ID_OPTION_RADIO_NOTIFICATION_CONNECT_BUDDY_OFF);
	pNotificationConnectBuddyRadio->AddHandler(LSY::EM_O_SELCHANGE, LSY::Fnt(m_OptionDlg, &COptionDlg::OnSelChangeNotificationConnectBuddyType));

	// 효과음 볼륨
	LSY::CSlideBar *pEffectSlide = (LSY::CSlideBar *)m_OptionDlg->GetObject(ID_OPTION_SLIDE_EFFECT_SOUND);
	pEffectSlide->SetCurValue((float)m_OptionDlg->m_pMyConfig->FxVolume);
	pEffectSlide->AddHandler(LSY::EM_O_CURVALUECHANGE, LSY::Fnt(m_OptionDlg, &COptionDlg::OnCurValueChange));
}

void COptionFrameBase::Show()
{
	LSY::CImage *pDefaultGameImage = (LSY::CImage *)m_OptionDlg->GetObject(ID_OPTION_IMAGE_CONFIG_ALLGAME);
	LSY::CImage *pSutdaGameImage = (LSY::CImage *)m_OptionDlg->GetObject(ID_OPTION_IMAGE_CONFIG_SUTDA);
	LSY::CImage *pHoolaGameImage = (LSY::CImage *)m_OptionDlg->GetObject(ID_OPTION_IMAGE_CONFIG_HOOLA);

	pDefaultGameImage->SetShow(true);
	pSutdaGameImage->SetShow(false);
	pHoolaGameImage->SetShow(false);
}

void COptionFrameBase::Save()
{
	// 효과음
	LSY::CGroup* pGroup = (LSY::CGroup *)m_OptionDlg->GetObject(ID_OPTION_GRP_EFFECT_SOUND);
	m_OptionDlg->m_pMyConfig->bSndFX = (pGroup->GetCurSelID() == ID_OPTION_RADIO_EFFECTSOUND_ON) ? TRUE : FALSE;

	// 배팅 버튼 타입
	pGroup = (LSY::CGroup *)m_OptionDlg->GetObject(ID_OPTION_GRP_BETTINGBUTTON_TYPE);
	m_OptionDlg->m_pMyConfig->eBetBtnType = (CONFIG_BETBTN_TYPE)(pGroup->GetCurSelID() - ID_OPTION_RADIO_BETTINGBUTTON_NTYPE);

	// 로우바둑이 단축키
	pGroup = (LSY::CGroup *)m_OptionDlg->GetObject(ID_OPTION_GRP_CUTTING_HOTKEY_TYPE);
	m_OptionDlg->m_pMyConfig->bCutStyle = (pGroup->GetCurSelID() == ID_OPTION_RADIO_CUTTING_HOTKEY_FUNCTION) ? TRUE : FALSE;

	// 관전자 채팅
	pGroup = (LSY::CGroup *)m_OptionDlg->GetObject(ID_OPTION_GRP_OBSERVER_CHATTING);
	m_OptionDlg->m_pMyConfig->bNoObserverChat = (pGroup->GetCurSelID() == ID_OPTION_RADIO_OBSERVER_CHATTING_OFF) ? TRUE : FALSE;

	// 친구 접속 알림
	pGroup = (LSY::CGroup *)m_OptionDlg->GetObject(ID_OPTION_GRP_NOTIFICATION_CONNECT_BUDDY);
	m_OptionDlg->m_pMyConfig->bNotificationConnectBuddy = (pGroup->GetCurSelID() == ID_OPTION_RADIO_NOTIFICATION_CONNECT_BUDDY_ON) ? TRUE : FALSE;

	// 효과음 볼륨
	LSY::CSlideBar* pSlide = (LSY::CSlideBar *)m_OptionDlg->GetObject(ID_OPTION_SLIDE_EFFECT_SOUND);
	m_OptionDlg->m_pMyConfig->FxVolume = (int)pSlide->GetCurValue();
}