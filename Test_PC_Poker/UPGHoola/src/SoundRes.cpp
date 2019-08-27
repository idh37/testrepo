// SOUNDRes.cpp: implementation of the CGameRes class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SoundRes.h"

#include "GlobalGame.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

map< int, deque< string > > g_VoiceMap;

void RegistSoundFiles()
{
	// [사운드 객체 우선 순위]
	//
	// SNDPRIORITY_ONCEPLAY  : 한번만 플레이하고 객체 파괴(우선 순위 가장 낮음) - 생성하면 10초 후 파괴됨
	// SNDPRIORITY_LOW       : 마지막 연주 시점에서 30초 후 파괴(생성 후 최고 5분 대기)
	// SNDPRIORITY_MID       : 마지막 연주 시점에서 3분 후 파괴(생성 후 최고 10분 대기)
	// SNDPRIORITY_HIGH      : 마지막 연주 시점에서 10분 후 파괴(생성 후 최고 30분 대기)
	// SNDPRIORITY_KEEPALIVE : 파괴하지 않고 항상 존재(우선 순위 가장 높음)
	//
	// * 한 번 등록된 사운드 객체는 파괴되더라도 Play시 즉시 생성 됨
	// * WAV 사운드 등록 시 채널 개수를 최대한 줄여서 생성할 것
	//   (대부분의 상황에서는 1개 채널이면 충분함)
	// * 마지막 인자에 TRUE를 넘기면 등록 즉시 사운드 파일을 로드함
	//   FALSE를 넘기는 경우 Play시에 즉시 생성 함
	//   (로딩이 오래 걸리는 대형 파일이 아닌 경우는 대부분 FALSE를 사용하는 것이 좋음)
	
	NMBASE::SOUND::g_Sound.RegistWav(SND05, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE);	// 자기턴
	NMBASE::SOUND::g_Sound.RegistWav(SND06, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE);	// 결과 승자
	NMBASE::SOUND::g_Sound.RegistWav(SND07, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE);	// 결과 패자

	NMBASE::SOUND::g_Sound.RegistWav(SND08, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE);	// 카드 가져올 때
	NMBASE::SOUND::g_Sound.RegistWav(SND09, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE);	// 카드 버릴 때
	NMBASE::SOUND::g_Sound.RegistWav(SND10, NMBASE::SOUND::SNDPRIORITY_MID, 5, FALSE);	// 카드 바닥에 붙는 소리

	NMBASE::SOUND::g_Sound.RegistWav(SND16, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE);	// 카드 클릭 / 위아래 움직이는 소리

	NMBASE::SOUND::g_Sound.RegistWav(SND17, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE);	// 탱큐 들어갔을 때
	NMBASE::SOUND::g_Sound.RegistWav(SND18, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE);	// 탱큐 타이머

	NMBASE::SOUND::g_Sound.RegistWav(SND19, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE);	// 시간제한1
	NMBASE::SOUND::g_Sound.RegistWav(SND20, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE);	// 10초 타이머
	NMBASE::SOUND::g_Sound.RegistWav(SND21, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE);	// 물방울 타이머

	NMBASE::SOUND::g_Sound.RegistWav(SND26, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE);	// 골드판

	NMBASE::SOUND::g_Sound.RegistWav(SND30, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE);	// 카드 정렬 
	NMBASE::SOUND::g_Sound.RegistWav(SND31, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE);	// 등록/붙이기

	NMBASE::SOUND::g_Sound.RegistWav(SND35, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE);	// 잭팟
	NMBASE::SOUND::g_Sound.RegistWav(SND37, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE);	// 미션 완료 

	NMBASE::SOUND::g_Sound.RegistWav(SND38, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE);	// 시작 프레임
	NMBASE::SOUND::g_Sound.RegistWav(SND39, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE);	// 땡큐 블럭

	// 공통 효과음
	NMBASE::SOUND::g_Sound.RegistWav(SND40, NMBASE::SOUND::SNDPRIORITY_LOW, 1, FALSE);	// 초대 알림
	NMBASE::SOUND::g_Sound.RegistWav(SND45, NMBASE::SOUND::SNDPRIORITY_LOW, 1, FALSE);	// 버튼 눌림 소리
	NMBASE::SOUND::g_Sound.RegistWav(SND50, NMBASE::SOUND::SNDPRIORITY_LOW, 1, FALSE);	// 귓말 알림

	NMBASE::SOUND::g_Sound.RegistWav(SND55, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE); // PC방 사운드
	NMBASE::SOUND::g_Sound.RegistWav(SND56, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE); // 접속 
	NMBASE::SOUND::g_Sound.RegistWav(SND57, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE); // 게임방 입장
	NMBASE::SOUND::g_Sound.RegistWav(SND58, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE); // 게임 시작
	NMBASE::SOUND::g_Sound.RegistWav(SND59, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE); // 족보 완성시

	// 음악(OGG 파일)
	NMBASE::SOUND::g_Sound.RegistOggStream(MUSIC_OGG01, NMBASE::SOUND::SNDPRIORITY_MID, TRUE);		// OGG 음악 1
	NMBASE::SOUND::g_Sound.RegistOggStream(MUSIC_NORMAL, NMBASE::SOUND::SNDPRIORITY_MID, TRUE);		// OGG 음악 2
	NMBASE::SOUND::g_Sound.RegistOggStream(MUSIC_GOLD, NMBASE::SOUND::SNDPRIORITY_MID, TRUE);		// OGG 음악 3
	NMBASE::SOUND::g_Sound.RegistOggStream(MUSIC_OGG04, NMBASE::SOUND::SNDPRIORITY_MID, TRUE);		// OGG 음악 4

	RegVoiceFiles();
}

// 멀티씬의 사운드를 플레이 리스트에 추가하여 자동으로 연주되도록 함(CSound::RegularCall()을 주기적으로 호출해주어야함)
BOOL AutoPlayMSceneSound( NMBASE::GRAPHICGDI::xSprite* pSpr, int mscn )
{
	if(!pSpr)
		return FALSE;

	DWORD curtime = timeGetTime();

	NMBASE::GRAPHICGDI::QUERYMSCENE_SOUND sndquery;
	sndquery.Reset();
	while(pSpr->QueryMSceneSound(&sndquery, mscn, 0, 0xFFFFFFFF, FALSE))
	{
		// 등록되지 않은 사운드 파일이면 등록
		if(!NMBASE::SOUND::g_Sound.IsRegistered(sndquery.SndFile)) 
			NMBASE::SOUND::g_Sound.RegistWav(sndquery.SndFile, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE);

		NMBASE::SOUND::g_Sound.AddToPlayList(curtime + sndquery.PlayTime, sndquery.SndFile, FALSE, sndquery.Volume);
	}

	return TRUE;
}

void RegVoiceFiles()
{
	FILE *fp = fopen( "Hoola\\Sound\\Voice\\Voice.dat", "r" );
	if ( fp == NULL )
	{
		AfxMessageBox( "음성 파일 로딩을 실패하였습니다. 음성 출력이 불가합니다." );
		return;
	}

	int nVoiceKind;
	char sAction[MAX_PATH], sFileName[MAX_PATH];

	while ( !feof( fp ) )
	{
		fscanf( fp, "%d %s %s", &nVoiceKind, sAction, sFileName );

		std::string sAct = sAction;

		if ( sAct == "LEFTONE" )
		{
			RegVoice( (nVoiceKind<<16) + 1, sFileName );
		}
		else if ( sAct == "GOLDSTAGE2" )
		{
			RegVoice( (nVoiceKind<<16) + 2, sFileName );
		}
		else if ( sAct == "GOLDSTAGE4" )
		{
			RegVoice( (nVoiceKind<<16) + 3, sFileName );
		}
		else if ( sAct == "BORING" )
		{
			RegVoice( (nVoiceKind<<16) + 4, sFileName );
		}
		else if ( sAct == "REGIST" )
		{
			RegVoice( (nVoiceKind<<16) + 5, sFileName );
		}
		else if ( sAct == "THANKU" )
		{
			RegVoice( (nVoiceKind<<16) + 6, sFileName );
		}
		else if ( sAct == "THANKBAK" )
		{
			RegVoice( (nVoiceKind<<16) + 7, sFileName );
		}
		else if ( sAct == "STOPBAK" )
		{
			RegVoice( (nVoiceKind<<16) + 8, sFileName );
		}
		else if ( sAct == "JACKPOT" )
		{
			RegVoice( (nVoiceKind<<16) + 9, sFileName );
		}
		else if ( sAct == "DUMP" )
		{
			RegVoice( (nVoiceKind<<16) + 10, sFileName );
		}
		else if ( sAct == "STOP" )
		{
			RegVoice( (nVoiceKind<<16) + 11, sFileName );
		}
		else if ( sAct == "HOOLA" )
		{
			RegVoice( (nVoiceKind<<16) + 12, sFileName );
		}
	}
}

void RegVoice( int nIndex, std::string sFileName )
{
	std::string sPath;

	sPath = "Hoola\\Sound\\Voice\\";

	switch ( (nIndex>>16) )
	{
	case 1: // M1
		sPath = sPath + "M1\\";
		break;

	case 2: // M2
		sPath = sPath + "M2\\";
		break;

	case 3: // F1
		sPath = sPath + "F1\\";
		break;

	case 4: // F2
		sPath = sPath + "F2\\";
		break;

	default:
		return;
	}

	VoiceMapIter Iter = g_VoiceMap.find( nIndex );

	if ( Iter == g_VoiceMap.end() )
	{
		std::deque< std::string > dqFileName;

		dqFileName.push_back( sPath + sFileName );

		g_VoiceMap.insert( VoiceMapValueType( nIndex, dqFileName ) );

		NMBASE::SOUND::g_Sound.RegistOggStream((sPath + sFileName).c_str(), NMBASE::SOUND::SNDPRIORITY_ONCEPLAY, FALSE);
	}
	else
	{
		std::deque< std::string >* dqFileName = &Iter->second;

		dqFileName->push_back( sPath + sFileName );

		NMBASE::SOUND::g_Sound.RegistOggStream((sPath + sFileName).c_str(), NMBASE::SOUND::SNDPRIORITY_ONCEPLAY, FALSE);
	}
}

void PlayVoice( BOOL bSex, int nSndFxKind, int nAction, int nProbability)
{
	int nType = 1;

	if (bSex)
	{
		nType = (nSndFxKind==0)?1:2;
	}
	else
	{
		nType = (nSndFxKind==1)?3:4;
	}

	if (nSndFxKind == -1)
	{
		//연습상대의 nSndFxKind를 -1로 세팅했고 해당 플레이어는 사운드를 들려주지 않는다
		nType = 0;
	}

	int nIndex = (nType<<16) + nAction;

	VoiceMapIter Iter = g_VoiceMap.find( nIndex );

	if ( Iter != g_VoiceMap.end() )
	{
		std::deque< std::string >* dqFileName = &Iter->second;

		int nSize = dqFileName->size();

		int nPlayNum = rand() % nSize;

		if ( CanPlay( nProbability ) ) 
		{
			NMBASE::SOUND::g_Sound.PlayOggStream( Iter->second[nPlayNum].c_str(), FALSE );			
		}
	}
}

BOOL CanPlay( int nProbability )
{
	if ( nProbability == 100 )
	{
		return TRUE;
	}
	else if ( nProbability == 0 )
	{
		return FALSE;
	}
	else
	{
		if ( (rand()%100) < nProbability )
			return TRUE;
	}
	return FALSE;
}