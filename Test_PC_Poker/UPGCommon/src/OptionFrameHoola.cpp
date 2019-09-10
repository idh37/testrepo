#include "stdafx.h"
#include "OptionFrameHoola.h"

#include "OptionDlg.h"
#include "OptionDlgID.h"

COptionFrameHoola::COptionFrameHoola(COptionDlg* dlg)
	: COptionFrameBase(dlg)
{
}

COptionFrameHoola::~COptionFrameHoola()
{

}

void COptionFrameHoola::Init()
{
	// ȿ����
	LSY::CGroup* pEffectSound = (LSY::CGroup *)m_OptionDlg->GetObject(ID_OPTION_GRP_EFFECT_SOUND_HOOLA);
	pEffectSound->SetCurSelFromID(m_OptionDlg->m_pMyConfig->bSndFX ? ID_OPTION_RADIO_EFFECTSOUND_ON_HOOLA : ID_OPTION_RADIO_EFFECTSOUND_OFF_HOOLA);
	pEffectSound->AddHandler(LSY::EM_O_SELCHANGE, LSY::Fnt(m_OptionDlg, &COptionDlg::OnSelChangeEffectSound));

	// �����
	LSY::CGroup* pbackgroundSound = (LSY::CGroup *)m_OptionDlg->GetObject(ID_OPTION_GRP_BGM_SOUND_HOOLA);
	pbackgroundSound->SetCurSelFromID(m_OptionDlg->m_pMyConfig->bMusicON ? ID_OPTION_RADIO_BGMSOUND_ON_HOOLA : ID_OPTION_RADIO_BGMSOUND_OFF_HOOLA);

	// ��Ҹ�
	if (m_OptionDlg->m_bSex)
	{
		// ����
		LSY::CGroup* pGroup = (LSY::CGroup *)m_OptionDlg->GetObject(ID_OPTION_GRP_VOICE_MALE_TYPE_HOOLA);
		pGroup->SetCurSelFromID(m_OptionDlg->m_pMyConfig->SndFxKind == 0 ? ID_OPTION_RADIO_VOICE_MALE_TYPE1_HOOLA : ID_OPTION_RADIO_VOICE_MALE_TYPE2_HOOLA);
		pGroup->SetShow(true);
		pGroup = (LSY::CGroup *)m_OptionDlg->GetObject(ID_OPTION_GRP_VOICE_FEMALE_TYPE_HOOLA);
		pGroup->SetShow(false);
	}
	else
	{
		// ����
		LSY::CGroup* pGroup = (LSY::CGroup *)m_OptionDlg->GetObject(ID_OPTION_GRP_VOICE_FEMALE_TYPE_HOOLA);
		pGroup->SetCurSelFromID(m_OptionDlg->m_pMyConfig->SndFxKind == 0 ? ID_OPTION_RADIO_VOICE_FEMALE_TYPE1_HOOLA : ID_OPTION_RADIO_VOICE_FEMALE_TYPE2_HOOLA);
		pGroup->SetShow(true);
		pGroup = (LSY::CGroup *)m_OptionDlg->GetObject(ID_OPTION_GRP_VOICE_MALE_TYPE_HOOLA);
		pGroup->SetShow(false);
	}

	// ������ ä��
	LSY::CGroup* pGroup = (LSY::CGroup *)m_OptionDlg->GetObject(ID_OPTION_GRP_OBSERVER_CHATTING_HOOLA);
	pGroup->SetCurSelFromID(m_OptionDlg->m_pMyConfig->bNoObserverChat ? ID_OPTION_RADIO_OBSERVER_CHATTING_OFF_HOOLA : ID_OPTION_RADIO_OBSERVER_CHATTING_ON_HOOLA);
	pGroup->AddHandler(LSY::EM_O_SELCHANGE, LSY::Fnt(m_OptionDlg, &COptionDlg::OnSelChangeObserverChattingType));

	// ģ�� ���� �˸�
	pGroup = (LSY::CGroup *)m_OptionDlg->GetObject(ID_OPTION_GRP_NOTIFICATION_CONNECT_BUDDY_HOOLA);
	pGroup->SetCurSelFromID(m_OptionDlg->m_pMyConfig->bNotificationConnectBuddy ? ID_OPTION_RADIO_NOTIFICATION_CONNECT_BUDDY_ON_HOOLA : ID_OPTION_RADIO_NOTIFICATION_CONNECT_BUDDY_OFF_HOOLA);
	pGroup->AddHandler(LSY::EM_O_SELCHANGE, LSY::Fnt(m_OptionDlg, &COptionDlg::OnSelChangeNotificationConnectBuddyType));

	// ȿ���� ����
	LSY::CSlideBar* pSlide = (LSY::CSlideBar *)m_OptionDlg->GetObject(ID_OPTION_SLIDE_EFFECT_SOUND_HOOLA);
	pSlide->SetCurValue((float)m_OptionDlg->m_pMyConfig->FxVolume);
	pSlide->AddHandler(LSY::EM_O_CURVALUECHANGE, LSY::Fnt(m_OptionDlg, &COptionDlg::OnCurValueChange));

	// ����� ����
	pSlide = (LSY::CSlideBar *)m_OptionDlg->GetObject(ID_OPTION_SLIDE_BGM_SOUND_HOOLA);
	pSlide->SetCurValue((float)m_OptionDlg->m_pMyConfig->BGMVolume);
	pSlide->AddHandler(LSY::EM_O_CURVALUECHANGE, LSY::Fnt(m_OptionDlg, &COptionDlg::OnCurValueChange));
}

void COptionFrameHoola::Show()
{
	LSY::CImage *pDefaultGameImage = (LSY::CImage *)m_OptionDlg->GetObject(ID_OPTION_IMAGE_CONFIG_ALLGAME);
	LSY::CImage *pSutdaGameImage = (LSY::CImage *)m_OptionDlg->GetObject(ID_OPTION_IMAGE_CONFIG_SUTDA);
	LSY::CImage *pHoolaGameImage = (LSY::CImage *)m_OptionDlg->GetObject(ID_OPTION_IMAGE_CONFIG_HOOLA);

	pDefaultGameImage->SetShow(false);
	pSutdaGameImage->SetShow(false);
	pHoolaGameImage->SetShow(true);
}

void COptionFrameHoola::Save()
{
	// ȿ���� Ȱ��ȭ ����
	LSY::CGroup *pGroup = (LSY::CGroup *)m_OptionDlg->GetObject(ID_OPTION_GRP_EFFECT_SOUND_HOOLA);
	m_OptionDlg->m_pMyConfig->bSndFX = (pGroup->GetCurSelID() == ID_OPTION_RADIO_EFFECTSOUND_ON_HOOLA) ? TRUE : FALSE;

	// ����� Ȱ��ȭ ����
	pGroup = (LSY::CGroup *)m_OptionDlg->GetObject(ID_OPTION_GRP_BGM_SOUND_HOOLA);
	m_OptionDlg->m_pMyConfig->bMusicON = (pGroup->GetCurSelID() == ID_OPTION_RADIO_BGMSOUND_ON_HOOLA) ? TRUE : FALSE;

	// ����
	if (m_OptionDlg->m_bSex)
	{
		pGroup = (LSY::CGroup *)m_OptionDlg->GetObject(ID_OPTION_GRP_VOICE_MALE_TYPE_HOOLA);
		m_OptionDlg->m_pMyConfig->SndFxKind = (pGroup->GetCurSelID() == ID_OPTION_RADIO_VOICE_MALE_TYPE1_HOOLA) ? 0 : 1;
	}
	else
	{
		pGroup = (LSY::CGroup *)m_OptionDlg->GetObject(ID_OPTION_GRP_VOICE_FEMALE_TYPE_HOOLA);
		m_OptionDlg->m_pMyConfig->SndFxKind = (pGroup->GetCurSelID() == ID_OPTION_RADIO_VOICE_FEMALE_TYPE1_HOOLA) ? 0 : 1;
	}

	// ������ ä��
	pGroup = (LSY::CGroup *)m_OptionDlg->GetObject(ID_OPTION_GRP_OBSERVER_CHATTING_HOOLA);
	m_OptionDlg->m_pMyConfig->bNoObserverChat = (pGroup->GetCurSelID() == ID_OPTION_RADIO_OBSERVER_CHATTING_OFF_HOOLA) ? TRUE : FALSE;

	// ģ�� ���� �˸�
	pGroup = (LSY::CGroup *)m_OptionDlg->GetObject(ID_OPTION_GRP_NOTIFICATION_CONNECT_BUDDY_HOOLA);
	m_OptionDlg->m_pMyConfig->bNotificationConnectBuddy = (pGroup->GetCurSelID() == ID_OPTION_RADIO_NOTIFICATION_CONNECT_BUDDY_ON_HOOLA) ? TRUE : FALSE;

	// ����� ����
	LSY::CSlideBar* pSlide = (LSY::CSlideBar *)m_OptionDlg->GetObject(ID_OPTION_SLIDE_BGM_SOUND_HOOLA);
	m_OptionDlg->m_pMyConfig->BGMVolume = (int)pSlide->GetCurValue();

	// ȿ���� ����
	pSlide = (LSY::CSlideBar *)m_OptionDlg->GetObject(ID_OPTION_SLIDE_EFFECT_SOUND_HOOLA);
	m_OptionDlg->m_pMyConfig->FxVolume = (int)pSlide->GetCurValue();
}