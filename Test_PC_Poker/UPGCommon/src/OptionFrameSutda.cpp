#include "stdafx.h"
#include "OptionFrameSutda.h"

#include "OptionDlg.h"
#include "OptionDlgID.h"

COptionFrameSutda::COptionFrameSutda(COptionDlg* dlg)
	: COptionFrameBase(dlg)
{
}

COptionFrameSutda::~COptionFrameSutda()
{

}

void COptionFrameSutda::Init()
{
	// ȿ����
	LSY::CGroup *pGroup = (LSY::CGroup *)m_OptionDlg->GetObject(ID_OPTION_GRP_EFFECT_SOUND_SUTDA);
	pGroup->SetCurSelFromID(m_OptionDlg->m_pMyConfig->bSndFX ? ID_OPTION_RADIO_EFFECTSOUND_ON_SUTDA : ID_OPTION_RADIO_EFFECTSOUND_OFF_SUTDA);
	pGroup->AddHandler(LSY::EM_O_SELCHANGE, LSY::Fnt(m_OptionDlg, &COptionDlg::OnSelChangeEffectSound));

	// �����
	pGroup = (LSY::CGroup *)m_OptionDlg->GetObject(ID_OPTION_GRP_BGM_SOUND_SUTDA);
	pGroup->SetCurSelFromID(m_OptionDlg->m_pMyConfig->bMusicON ? ID_OPTION_RADIO_BGMSOUND_ON_SUTDA : ID_OPTION_RADIO_BGMSOUND_OFF_SUTDA);

	//��Ҹ�
	if (m_OptionDlg->m_bSex)
	{
		// ����
		pGroup = (LSY::CGroup *)m_OptionDlg->GetObject(ID_OPTION_GRP_VOICE_MALE_TYPE_SUTDA);
		pGroup->SetCurSelFromID(m_OptionDlg->m_pMyConfig->SndFxKind == 0 ? ID_OPTION_RADIO_VOICE_MALE_TYPE1_SUTDA : ID_OPTION_RADIO_VOICE_MALE_TYPE2_SUTDA);
		pGroup->SetShow(true);
		pGroup = (LSY::CGroup *)m_OptionDlg->GetObject(ID_OPTION_GRP_VOICE_FEMALE_TYPE_SUTDA);
		pGroup->SetShow(false);
	}
	else
	{
		// ����
		pGroup = (LSY::CGroup *)m_OptionDlg->GetObject(ID_OPTION_GRP_VOICE_FEMALE_TYPE_SUTDA);
		pGroup->SetCurSelFromID(m_OptionDlg->m_pMyConfig->SndFxKind == 0 ? ID_OPTION_RADIO_VOICE_FEMALE_TYPE1_SUTDA : ID_OPTION_RADIO_VOICE_FEMALE_TYPE2_SUTDA);
		pGroup->SetShow(true);
		pGroup = (LSY::CGroup *)m_OptionDlg->GetObject(ID_OPTION_GRP_VOICE_MALE_TYPE_SUTDA);
		pGroup->SetShow(false);
	}

	// ���� ��ư Ÿ��
	pGroup = (LSY::CGroup *)m_OptionDlg->GetObject(ID_OPTION_GRP_BETTINGBUTTON_SUTDA);
	pGroup->SetCurSelFromID(ID_OPTION_RADIO_BETTINGBUTTON_NTYPE_SUTDA + (int)m_OptionDlg->m_pMyConfig->eBetBtnType);
	pGroup->AddHandler(LSY::EM_O_SELCHANGE, LSY::Fnt(m_OptionDlg, &COptionDlg::OnSelChangeBetBtnType));

	// �ڵ� ����
	pGroup = (LSY::CGroup *)m_OptionDlg->GetObject(ID_OPTION_GRP_AUTOHIDDEN_SUTDA);
	pGroup->SetCurSelFromID(m_OptionDlg->m_pMyConfig->bAutoHidden ? ID_OPTION_RADIO_AUTOHIDDE_ON_SUTDA : ID_OPTION_RADIO_AUTOHIDDE_OFF_SUTDA);

	// ������ ä��
	pGroup = (LSY::CGroup *)m_OptionDlg->GetObject(ID_OPTION_GRP_OBSERVER_CHATTING_SUTDA);
	pGroup->SetCurSelFromID(m_OptionDlg->m_pMyConfig->bNoObserverChat ? ID_OPTION_RADIO_OBSERVER_CHATTING_OFF_SUTDA : ID_OPTION_RADIO_OBSERVER_CHATTING_ON_SUTDA);
	pGroup->AddHandler(LSY::EM_O_SELCHANGE, LSY::Fnt(m_OptionDlg, &COptionDlg::OnSelChangeObserverChattingType));

	// ģ�� ���� �˸�
	pGroup = (LSY::CGroup *)m_OptionDlg->GetObject(ID_OPTION_GRP_NOTIFICATION_CONNECT_BUDDY_SUTDA);
	pGroup->SetCurSelFromID(m_OptionDlg->m_pMyConfig->bNotificationConnectBuddy ? ID_OPTION_RADIO_NOTIFICATION_CONNECT_BUDDY_ON_SUTDA : ID_OPTION_RADIO_NOTIFICATION_CONNECT_BUDDY_OFF_SUTDA);
	pGroup->AddHandler(LSY::EM_O_SELCHANGE, LSY::Fnt(m_OptionDlg, &COptionDlg::OnSelChangeNotificationConnectBuddyType));

	// ȿ���� ����
	LSY::CSlideBar *pSlide = (LSY::CSlideBar *)m_OptionDlg->GetObject(ID_OPTION_SLIDE_EFFECT_SOUND_SUTDA);
	pSlide->SetCurValue((float)m_OptionDlg->m_pMyConfig->FxVolume);
	pSlide->AddHandler(LSY::EM_O_CURVALUECHANGE, LSY::Fnt(m_OptionDlg, &COptionDlg::OnCurValueChange));

	// ����� ����
	pSlide = (LSY::CSlideBar *)m_OptionDlg->GetObject(ID_OPTION_SLIDE_BGM_SOUND_SUTDA);
	pSlide->SetCurValue((float)m_OptionDlg->m_pMyConfig->BGMVolume);
	pSlide->AddHandler(LSY::EM_O_CURVALUECHANGE, LSY::Fnt(m_OptionDlg, &COptionDlg::OnCurValueChange));
}

void COptionFrameSutda::Show()
{
	LSY::CImage *pDefaultGameImage = (LSY::CImage *)m_OptionDlg->GetObject(ID_OPTION_IMAGE_CONFIG_ALLGAME);
	LSY::CImage *pSutdaGameImage = (LSY::CImage *)m_OptionDlg->GetObject(ID_OPTION_IMAGE_CONFIG_SUTDA);
	LSY::CImage *pHoolaGameImage = (LSY::CImage *)m_OptionDlg->GetObject(ID_OPTION_IMAGE_CONFIG_HOOLA);

	pDefaultGameImage->SetShow(false);
	pSutdaGameImage->SetShow(true);
	pHoolaGameImage->SetShow(false);
}

void COptionFrameSutda::Save()
{
	// ȿ����
	LSY::CGroup* pGroup = (LSY::CGroup *)m_OptionDlg->GetObject(ID_OPTION_GRP_EFFECT_SOUND_SUTDA);
	m_OptionDlg->m_pMyConfig->bSndFX = (pGroup->GetCurSelID() == ID_OPTION_RADIO_EFFECTSOUND_ON_SUTDA) ? TRUE : FALSE;

	// �����
	pGroup = (LSY::CGroup *)m_OptionDlg->GetObject(ID_OPTION_GRP_BGM_SOUND_SUTDA);
	m_OptionDlg->m_pMyConfig->bMusicON = (pGroup->GetCurSelID() == ID_OPTION_RADIO_BGMSOUND_ON_SUTDA) ? TRUE : FALSE;

	// ����
	if (m_OptionDlg->m_bSex)
	{
		pGroup = (LSY::CGroup *)m_OptionDlg->GetObject(ID_OPTION_GRP_VOICE_MALE_TYPE_SUTDA);
		m_OptionDlg->m_pMyConfig->SndFxKind = (pGroup->GetCurSelID() == ID_OPTION_RADIO_VOICE_MALE_TYPE1_SUTDA) ? 0 : 1;
	}
	else
	{
		pGroup = (LSY::CGroup *)m_OptionDlg->GetObject(ID_OPTION_GRP_VOICE_FEMALE_TYPE_SUTDA);
		m_OptionDlg->m_pMyConfig->SndFxKind = (pGroup->GetCurSelID() == ID_OPTION_RADIO_VOICE_FEMALE_TYPE1_SUTDA) ? 0 : 1;
	}

	// �ڵ� ����
	pGroup = (LSY::CGroup *)m_OptionDlg->GetObject(ID_OPTION_GRP_AUTOHIDDEN_SUTDA);
	m_OptionDlg->m_pMyConfig->bAutoHidden = (pGroup->GetCurSelID() == ID_OPTION_RADIO_AUTOHIDDE_ON_SUTDA) ? TRUE : FALSE;

	// ���� ��ư Ÿ��
	pGroup = (LSY::CGroup *)m_OptionDlg->GetObject(ID_OPTION_GRP_BETTINGBUTTON_SUTDA);
	m_OptionDlg->m_pMyConfig->eBetBtnType = (CONFIG_BETBTN_TYPE)(pGroup->GetCurSelID() - ID_OPTION_RADIO_BETTINGBUTTON_NTYPE_SUTDA);

	// ������ ä��
	pGroup = (LSY::CGroup *)m_OptionDlg->GetObject(ID_OPTION_GRP_OBSERVER_CHATTING_SUTDA);
	m_OptionDlg->m_pMyConfig->bNoObserverChat = (pGroup->GetCurSelID() == ID_OPTION_RADIO_OBSERVER_CHATTING_OFF_SUTDA) ? TRUE : FALSE;
	
	// ģ�� ���� �˸�
	pGroup = (LSY::CGroup *)m_OptionDlg->GetObject(ID_OPTION_GRP_NOTIFICATION_CONNECT_BUDDY_SUTDA);
	m_OptionDlg->m_pMyConfig->bNotificationConnectBuddy = (pGroup->GetCurSelID() == ID_OPTION_RADIO_NOTIFICATION_CONNECT_BUDDY_ON_SUTDA) ? TRUE : FALSE;

	// ���� ���� ����
	LSY::CSlideBar* pSlide = (LSY::CSlideBar *)m_OptionDlg->GetObject(ID_OPTION_SLIDE_BGM_SOUND_SUTDA);
	m_OptionDlg->m_pMyConfig->BGMVolume = (int)pSlide->GetCurValue();

	//ȿ���� ����
	pSlide = (LSY::CSlideBar *)m_OptionDlg->GetObject(ID_OPTION_SLIDE_EFFECT_SOUND_SUTDA);
	m_OptionDlg->m_pMyConfig->FxVolume = (int)pSlide->GetCurValue();
}