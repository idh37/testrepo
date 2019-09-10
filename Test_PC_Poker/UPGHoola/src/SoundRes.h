// SoundRes.h: interface for the CGameRes class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOUNDRES_H__E97073DD_5C3D_4D92_8719_5B471E26DE9B__INCLUDED_)
#define AFX_SOUNDRES_H__E97073DD_5C3D_4D92_8719_5B471E26DE9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ���� ���ҽ��� �����ϰ� ����Ѵ�
void RegistSoundFiles();
#define SND05 ("Hoola\\sound\\05.wav")	// �ڱ���
#define SND06 ("Hoola\\sound\\06.wav")	// ��� ����
#define SND07 ("Hoola\\sound\\07.wav")	// ��� ����

#define SND08 ("Hoola\\sound\\08.wav")	// ī�� ������ ��
#define SND09 ("Hoola\\sound\\09.wav")	// ī�� ���� �� 
#define SND10 ("Hoola\\sound\\10.wav")	// ī�� �ٴڿ� �ٴ� �Ҹ�

#define SND16 ("Hoola\\sound\\16.wav")   // ī�� Ŭ�� / ���Ʒ� �����̴� �Ҹ�

#define SND17 ("Hoola\\sound\\17.wav")   // ��ť ���� ��
#define SND18 ("Hoola\\sound\\18.wav")   // ��ť Ÿ�̸�

#define SND19 ("Hoola\\sound\\19.wav")	// �ð�����1
#define SND20 ("Hoola\\sound\\20.wav")	// 10�� Ÿ�̸�
#define SND21 ("Hoola\\sound\\timer.wav")	// ����� Ÿ�̸�

#define SND26 ("Hoola\\sound\\26.wav")   // ����� ����

#define SND30 ("Hoola\\sound\\30.wav")	// ī�� ���� 
#define SND31 ("Hoola\\sound\\31.wav")	// ���/���̱� 

#define SND35 ("Hoola\\sound\\35.wav")	// ���� 
#define SND37 ("Hoola\\sound\\37.wav")	// �̼� �Ϸ� 

#define SND38 ("Hoola\\sound\\38.wav")	// ���� ������
#define SND39 ("Hoola\\sound\\thankblock.wav") // ��ť ��

// ���� ȿ����
#define SND40 ("Hoola\\sound\\40.wav")		// �ʴ� �˸�
#define SND45 ("Hoola\\sound\\45.wav")		// ��ư ���� �Ҹ�
#define SND50 ("Hoola\\sound\\whisper.wav")	// �Ӹ� �˸�
// [PC�� ���� �߰�] 2005.06.21
#define SND55 ("Hoola\\sound\\pcbang_in.wav") // PC�� ����
#define SND56 ("Hoola\\sound\\Login.wav") // ���� 
#define SND57 ("Hoola\\sound\\EnterRoom.wav") // ���ӹ� ����
#define SND58 ("Hoola\\sound\\GameStart.wav") // ���� ����

#define SND59 ("Hoola\\sound\\hoola_clear.wav") // �����ϼ���


#define	MUSIC_OGG01 ("Hoola\\Sound\\BACK1.ogg")		// OGG ���� 1  ����
#define	MUSIC_NORMAL ("Hoola\\Sound\\BACK2.ogg")		// OGG ���� 2  �Ϲ���
#define	MUSIC_GOLD ("Hoola\\Sound\\BACK3.ogg")		// OGG ���� 2  �����
#define	MUSIC_OGG04 ("Hoola\\Sound\\BACK4.ogg")		// OGG ���� 2  ������


#define LEFTONE			1
#define GOLDSTAGE2		2
#define GOLDSTAGE4		3
#define BORING			4
#define REGIST			5
#define THANKU			6
#define THANKBAK		7
#define STOPBAK			8
#define JACKPOT			9
#define DUMP			10
#define STOP			11
#define HOOLA			12

typedef map< int, deque< string > >::iterator VoiceMapIter;
typedef map< int, deque< string > >::value_type VoiceMapValueType;
extern map< int, deque< string > > g_VoiceMap;

// ���� ���ҽ��� �����ϰ� ����Ѵ�
void RegistSoundFiles();
void RegVoiceFiles();
void RegVoice( int nIndex, std::string sFileName );

void PlayVoice( BOOL bSex, int nSndFxKind, int nAction, int nProbability = 100 );
BOOL CanPlay( int nProbability );

// ��Ƽ���� ���带 �÷��� ����Ʈ�� �߰��Ͽ� �ڵ����� ���ֵǵ��� ��(CSound::RegularCall()�� �ֱ������� ȣ�����־����)
BOOL AutoPlayMSceneSound( NMBASE::GRAPHICGDI::xSprite* pSpr, int mscn );



#endif // !defined(AFX_SOUNDRES_H__E97073DD_5C3D_4D92_8719_5B471E26DE9B__INCLUDED_)
