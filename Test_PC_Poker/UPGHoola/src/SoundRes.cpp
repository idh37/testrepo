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
	// [���� ��ü �켱 ����]
	//
	// SNDPRIORITY_ONCEPLAY  : �ѹ��� �÷����ϰ� ��ü �ı�(�켱 ���� ���� ����) - �����ϸ� 10�� �� �ı���
	// SNDPRIORITY_LOW       : ������ ���� �������� 30�� �� �ı�(���� �� �ְ� 5�� ���)
	// SNDPRIORITY_MID       : ������ ���� �������� 3�� �� �ı�(���� �� �ְ� 10�� ���)
	// SNDPRIORITY_HIGH      : ������ ���� �������� 10�� �� �ı�(���� �� �ְ� 30�� ���)
	// SNDPRIORITY_KEEPALIVE : �ı����� �ʰ� �׻� ����(�켱 ���� ���� ����)
	//
	// * �� �� ��ϵ� ���� ��ü�� �ı��Ǵ��� Play�� ��� ���� ��
	// * WAV ���� ��� �� ä�� ������ �ִ��� �ٿ��� ������ ��
	//   (��κ��� ��Ȳ������ 1�� ä���̸� �����)
	// * ������ ���ڿ� TRUE�� �ѱ�� ��� ��� ���� ������ �ε���
	//   FALSE�� �ѱ�� ��� Play�ÿ� ��� ���� ��
	//   (�ε��� ���� �ɸ��� ���� ������ �ƴ� ���� ��κ� FALSE�� ����ϴ� ���� ����)
	
	NMBASE::SOUND::g_Sound.RegistWav(SND05, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE);	// �ڱ���
	NMBASE::SOUND::g_Sound.RegistWav(SND06, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE);	// ��� ����
	NMBASE::SOUND::g_Sound.RegistWav(SND07, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE);	// ��� ����

	NMBASE::SOUND::g_Sound.RegistWav(SND08, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE);	// ī�� ������ ��
	NMBASE::SOUND::g_Sound.RegistWav(SND09, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE);	// ī�� ���� ��
	NMBASE::SOUND::g_Sound.RegistWav(SND10, NMBASE::SOUND::SNDPRIORITY_MID, 5, FALSE);	// ī�� �ٴڿ� �ٴ� �Ҹ�

	NMBASE::SOUND::g_Sound.RegistWav(SND16, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE);	// ī�� Ŭ�� / ���Ʒ� �����̴� �Ҹ�

	NMBASE::SOUND::g_Sound.RegistWav(SND17, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE);	// ��ť ���� ��
	NMBASE::SOUND::g_Sound.RegistWav(SND18, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE);	// ��ť Ÿ�̸�

	NMBASE::SOUND::g_Sound.RegistWav(SND19, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE);	// �ð�����1
	NMBASE::SOUND::g_Sound.RegistWav(SND20, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE);	// 10�� Ÿ�̸�
	NMBASE::SOUND::g_Sound.RegistWav(SND21, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE);	// ����� Ÿ�̸�

	NMBASE::SOUND::g_Sound.RegistWav(SND26, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE);	// �����

	NMBASE::SOUND::g_Sound.RegistWav(SND30, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE);	// ī�� ���� 
	NMBASE::SOUND::g_Sound.RegistWav(SND31, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE);	// ���/���̱�

	NMBASE::SOUND::g_Sound.RegistWav(SND35, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE);	// ����
	NMBASE::SOUND::g_Sound.RegistWav(SND37, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE);	// �̼� �Ϸ� 

	NMBASE::SOUND::g_Sound.RegistWav(SND38, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE);	// ���� ������
	NMBASE::SOUND::g_Sound.RegistWav(SND39, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE);	// ��ť ��

	// ���� ȿ����
	NMBASE::SOUND::g_Sound.RegistWav(SND40, NMBASE::SOUND::SNDPRIORITY_LOW, 1, FALSE);	// �ʴ� �˸�
	NMBASE::SOUND::g_Sound.RegistWav(SND45, NMBASE::SOUND::SNDPRIORITY_LOW, 1, FALSE);	// ��ư ���� �Ҹ�
	NMBASE::SOUND::g_Sound.RegistWav(SND50, NMBASE::SOUND::SNDPRIORITY_LOW, 1, FALSE);	// �Ӹ� �˸�

	NMBASE::SOUND::g_Sound.RegistWav(SND55, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE); // PC�� ����
	NMBASE::SOUND::g_Sound.RegistWav(SND56, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE); // ���� 
	NMBASE::SOUND::g_Sound.RegistWav(SND57, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE); // ���ӹ� ����
	NMBASE::SOUND::g_Sound.RegistWav(SND58, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE); // ���� ����
	NMBASE::SOUND::g_Sound.RegistWav(SND59, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE); // ���� �ϼ���

	// ����(OGG ����)
	NMBASE::SOUND::g_Sound.RegistOggStream(MUSIC_OGG01, NMBASE::SOUND::SNDPRIORITY_MID, TRUE);		// OGG ���� 1
	NMBASE::SOUND::g_Sound.RegistOggStream(MUSIC_NORMAL, NMBASE::SOUND::SNDPRIORITY_MID, TRUE);		// OGG ���� 2
	NMBASE::SOUND::g_Sound.RegistOggStream(MUSIC_GOLD, NMBASE::SOUND::SNDPRIORITY_MID, TRUE);		// OGG ���� 3
	NMBASE::SOUND::g_Sound.RegistOggStream(MUSIC_OGG04, NMBASE::SOUND::SNDPRIORITY_MID, TRUE);		// OGG ���� 4

	RegVoiceFiles();
}

// ��Ƽ���� ���带 �÷��� ����Ʈ�� �߰��Ͽ� �ڵ����� ���ֵǵ��� ��(CSound::RegularCall()�� �ֱ������� ȣ�����־����)
BOOL AutoPlayMSceneSound( NMBASE::GRAPHICGDI::xSprite* pSpr, int mscn )
{
	if(!pSpr)
		return FALSE;

	DWORD curtime = timeGetTime();

	NMBASE::GRAPHICGDI::QUERYMSCENE_SOUND sndquery;
	sndquery.Reset();
	while(pSpr->QueryMSceneSound(&sndquery, mscn, 0, 0xFFFFFFFF, FALSE))
	{
		// ��ϵ��� ���� ���� �����̸� ���
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
		AfxMessageBox( "���� ���� �ε��� �����Ͽ����ϴ�. ���� ����� �Ұ��մϴ�." );
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
		//��������� nSndFxKind�� -1�� �����߰� �ش� �÷��̾�� ���带 ������� �ʴ´�
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