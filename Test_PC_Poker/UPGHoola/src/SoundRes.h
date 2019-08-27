// SoundRes.h: interface for the CGameRes class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOUNDRES_H__E97073DD_5C3D_4D92_8719_5B471E26DE9B__INCLUDED_)
#define AFX_SOUNDRES_H__E97073DD_5C3D_4D92_8719_5B471E26DE9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 사운드 리소스를 정의하고 등록한다
void RegistSoundFiles();
#define SND05 ("Hoola\\sound\\05.wav")	// 자기턴
#define SND06 ("Hoola\\sound\\06.wav")	// 결과 승자
#define SND07 ("Hoola\\sound\\07.wav")	// 결과 패자

#define SND08 ("Hoola\\sound\\08.wav")	// 카드 가져올 때
#define SND09 ("Hoola\\sound\\09.wav")	// 카드 버릴 때 
#define SND10 ("Hoola\\sound\\10.wav")	// 카드 바닥에 붙는 소리

#define SND16 ("Hoola\\sound\\16.wav")   // 카드 클릭 / 위아래 움직이는 소리

#define SND17 ("Hoola\\sound\\17.wav")   // 탱큐 들어갔을 때
#define SND18 ("Hoola\\sound\\18.wav")   // 탱큐 타이머

#define SND19 ("Hoola\\sound\\19.wav")	// 시간제한1
#define SND20 ("Hoola\\sound\\20.wav")	// 10초 타이머
#define SND21 ("Hoola\\sound\\timer.wav")	// 물방울 타이머

#define SND26 ("Hoola\\sound\\26.wav")   // 골드판 내거

#define SND30 ("Hoola\\sound\\30.wav")	// 카드 정렬 
#define SND31 ("Hoola\\sound\\31.wav")	// 등록/붙이기 

#define SND35 ("Hoola\\sound\\35.wav")	// 잭팟 
#define SND37 ("Hoola\\sound\\37.wav")	// 미션 완료 

#define SND38 ("Hoola\\sound\\38.wav")	// 시작 프레임
#define SND39 ("Hoola\\sound\\thankblock.wav") // 땡큐 블럭

// 공통 효과음
#define SND40 ("Hoola\\sound\\40.wav")		// 초대 알림
#define SND45 ("Hoola\\sound\\45.wav")		// 버튼 눌림 소리
#define SND50 ("Hoola\\sound\\whisper.wav")	// 귓말 알림
// [PC방 사운드 추가] 2005.06.21
#define SND55 ("Hoola\\sound\\pcbang_in.wav") // PC방 사운드
#define SND56 ("Hoola\\sound\\Login.wav") // 접속 
#define SND57 ("Hoola\\sound\\EnterRoom.wav") // 게임방 입장
#define SND58 ("Hoola\\sound\\GameStart.wav") // 게임 시작

#define SND59 ("Hoola\\sound\\hoola_clear.wav") // 족보완성시


#define	MUSIC_OGG01 ("Hoola\\Sound\\BACK1.ogg")		// OGG 음악 1  대기실
#define	MUSIC_NORMAL ("Hoola\\Sound\\BACK2.ogg")		// OGG 음악 2  일반판
#define	MUSIC_GOLD ("Hoola\\Sound\\BACK3.ogg")		// OGG 음악 2  골드판
#define	MUSIC_OGG04 ("Hoola\\Sound\\BACK4.ogg")		// OGG 음악 2  잭팟판


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

// 사운드 리소스를 정의하고 등록한다
void RegistSoundFiles();
void RegVoiceFiles();
void RegVoice( int nIndex, std::string sFileName );

void PlayVoice( BOOL bSex, int nSndFxKind, int nAction, int nProbability = 100 );
BOOL CanPlay( int nProbability );

// 멀티씬의 사운드를 플레이 리스트에 추가하여 자동으로 연주되도록 함(CSound::RegularCall()을 주기적으로 호출해주어야함)
BOOL AutoPlayMSceneSound( NMBASE::GRAPHICGDI::xSprite* pSpr, int mscn );



#endif // !defined(AFX_SOUNDRES_H__E97073DD_5C3D_4D92_8719_5B471E26DE9B__INCLUDED_)
