#include "stdafx.h"
#include "Sound.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void PBRegistWav(bool bMan, LPCTSTR lpCommand)
{
	std::string strPath = bMan?SND_PATH_M:SND_PATH_F;
	strPath += lpCommand;

	NMBASE::SOUND::g_Sound.RegistWav(strPath.c_str(), NMBASE::SOUND::SNDPRIORITY_KEEPALIVE, 1, FALSE);
}

void PBRegistEffectWav(LPCTSTR lpCommand)
{
	std::string strPath = SND_PATH_EFFECT;
	strPath += lpCommand;

	NMBASE::SOUND::g_Sound.RegistWav(strPath.c_str(), NMBASE::SOUND::SNDPRIORITY_KEEPALIVE, 1, FALSE);
}

void PBPlaySound(bool bMan, LPCTSTR lpCommand)
{
	std::string strPath = bMan?SND_PATH_M:SND_PATH_F; 
	strPath += lpCommand;

	NMBASE::SOUND::g_Sound.PlayWav( strPath.c_str() );
}

void PBPlayEffectSound(LPCTSTR lpCommand)
{
	std::string strPath = SND_PATH_EFFECT;
	strPath += lpCommand;

	NMBASE::SOUND::g_Sound.PlayWav( strPath.c_str() );
}

void PBStopEffectSound(LPCTSTR lpCommand)
{
	std::string strPath = SND_PATH_EFFECT;
	strPath += lpCommand;

	NMBASE::SOUND::g_Sound.Stop( strPath.c_str() );
}

void PBRegistSoundFiles()
{
	//남성 베팅 음성 등록
	PBRegistWav(true, SND_ALLIN);
	PBRegistWav(true, SND_ALLIN_H);
	PBRegistWav(true, SND_CALL);
	PBRegistWav(true, SND_CALL_H);
	PBRegistWav(true, SND_CHECK);
	PBRegistWav(true, SND_CHECK_H);
	PBRegistWav(true, SND_DDANG);
	PBRegistWav(true, SND_DDANG_H);
	PBRegistWav(true, SND_DIE);
	PBRegistWav(true, SND_DIE_H);
	PBRegistWav(true, SND_HALF);
	PBRegistWav(true, SND_HALF_H);
	PBRegistWav(true, SND_PING);
	PBRegistWav(true, SND_PING_H);

	//규제안 : 맥스, 풀 사운드 추가
	PBRegistWav(true, SND_MAX);
	PBRegistWav(true, SND_MAX_H);
	PBRegistWav(true, SND_FULL);
	PBRegistWav(true, SND_FULL_H);
	

	//여성 베팅 음성 등록
	PBRegistWav(false, SND_ALLIN);
	PBRegistWav(false, SND_ALLIN_H);
	PBRegistWav(false, SND_CALL);
	PBRegistWav(false, SND_CALL_H);
	PBRegistWav(false, SND_CHECK);
	PBRegistWav(false, SND_CHECK_H);
	PBRegistWav(false, SND_DDANG);
	PBRegistWav(false, SND_DDANG_H);
	PBRegistWav(false, SND_DIE);
	PBRegistWav(false, SND_DIE_H);
	PBRegistWav(false, SND_HALF);
	PBRegistWav(false, SND_HALF_H);
	PBRegistWav(false, SND_PING);
	PBRegistWav(false, SND_PING_H);

	//규제안 : 맥스, 풀 사운드 추가
	PBRegistWav(false, SND_MAX);
	PBRegistWav(false, SND_MAX_H);
	PBRegistWav(false, SND_FULL);
	PBRegistWav(false, SND_FULL_H);

	//이펙트 사운드
	PBRegistEffectWav(SND_LOSE_ALLIN);
	PBRegistEffectWav(SND_LOSE_NORMAL);
	PBRegistEffectWav(SND_START);
	PBRegistEffectWav(SND_WIN_HIGH);
	PBRegistEffectWav(SND_WIN_LOW);
	PBRegistEffectWav(SND_CARD_SELECT);
	PBRegistEffectWav(SND_CARD_MOVE);
	PBRegistEffectWav(SND_CHIP);
	PBRegistEffectWav(SND_MYTURN);
	PBRegistEffectWav(SND_HIDDEN_OPEN);
	PBRegistEffectWav(SND_JOKERSHOW);
	PBRegistEffectWav(SND_JOKERCHANGE);
	PBRegistEffectWav(SND_BONUS_MOVE_CENTER);
	PBRegistEffectWav(SND_TIMER);
	PBRegistEffectWav(SND_HANDOEFT);
	PBRegistEffectWav(SND_VIPROOMJOIN);
	PBRegistEffectWav(SND_USERIN);
	PBRegistEffectWav(SND_USEROUT);
	PBRegistEffectWav(SND_MADE);
	PBRegistEffectWav(SND_PROMOTION_SUMMER);

	PBRegistEffectWav(SND_JP_END);
	PBRegistEffectWav(SND_JP_FX);
	PBRegistEffectWav(SND_JP_NOMI1_2);
	PBRegistEffectWav(SND_JP_NOMI2);
	PBRegistEffectWav(SND_JP_NOTI);
	PBRegistEffectWav(SND_JP_PRIZE);
	PBRegistEffectWav(SND_JP_SELECT);
	PBRegistEffectWav(SND_JP_START1_BGM);
	PBRegistEffectWav(SND_JP_START1_FX);
	PBRegistEffectWav(SND_JP_START2_BGM);
	PBRegistEffectWav(SND_JP_START2_FX);
	PBRegistEffectWav(SND_JP_TIMER);
	PBRegistEffectWav(SND_JP_WIN_BGM);

	//바둑이 커팅
	//남성
	PBRegistWav(true, SND_CUT_1);
	PBRegistWav(true, SND_CUT_2);
	PBRegistWav(true, SND_CUT_3);
	PBRegistWav(true, SND_CUT_4);
	PBRegistWav(true, SND_PASS);
	PBRegistWav(true, SND_MON);
	PBRegistWav(true, SND_AFT);
	PBRegistWav(true, SND_NIG);
	//여성
	PBRegistWav(false, SND_CUT_1);
	PBRegistWav(false, SND_CUT_2);
	PBRegistWav(false, SND_CUT_3);
	PBRegistWav(false, SND_CUT_4);
	PBRegistWav(false, SND_PASS);
	PBRegistWav(false, SND_MON);
	PBRegistWav(false, SND_AFT);
	PBRegistWav(false, SND_NIG);

	NMBASE::SOUND::g_Sound.RegistOggStream(SND_HIDDEN_BGM, NMBASE::SOUND::SNDPRIORITY_MID, FALSE);

	NMBASE::SOUND::g_Sound.ClearGameLoadList();
}

// 멀티씬의 사운드를 플레이 리스트에 추가하여 자동으로 연주되도록 함(CSound::RegularCall()을 주기적으로 호출해주어야함)
BOOL PBAutoPlayMSceneSound(NMBASE::GRAPHICGDI::xSprite* pSpr, int mscn)
{
	if(!pSpr)
		return FALSE;

	DWORD curtime = timeGetTime();

	NMBASE::GRAPHICGDI::QUERYMSCENE_SOUND sndquery;
	sndquery.Reset();
	while(pSpr->QueryMSceneSound(&sndquery, mscn, 0, 0xFFFFFFFF, TRUE))
	{
		// 등록되지 않은 사운드 파일이면 등록
		if(!NMBASE::SOUND::g_Sound.IsRegistered(sndquery.SndFile)) 
			NMBASE::SOUND::g_Sound.RegistWav(sndquery.SndFile, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE);

		NMBASE::SOUND::g_Sound.AddToPlayList(curtime + sndquery.PlayTime, sndquery.SndFile, FALSE, sndquery.Volume);
	}

	return TRUE;
}