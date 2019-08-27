// Sound.h: interface for the CSound class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOUND_H__EF4CEDCE_DF40_49DC_BF81_995C5AC5DBF3__INCLUDED_)
#define AFX_SOUND_H__EF4CEDCE_DF40_49DC_BF81_995C5AC5DBF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///////////////////////////
#pragma warning(disable:4786)	// map 을 사용하면 나타나는 warning 무력화
///////////////////////////


#include <map>
#include <string>
#include <list>
#include <set>

using namespace std;

#include "SoundDefine.h"
#include "xSound.h"
#include "MciMedia.h"
#include "xOgg.h"

namespace NMBASE
{
	namespace SOUND
	{

		////////////////////////////////////////////////////////////

		// 사운드 파일 타입
		enum SNDTYPE_ENUM
		{
			SNDTYPE_NONE=0,
			SNDTYPE_WAV=1,			// 웨이브 효과음
			SNDTYPE_MCIMEDIA,		// MP3, MIDI 음악(MCI를 이용하여 연주)
			SNDTYPE_OGGSTREAM		// OGG 스트림 음악
		};

		// 사운드 객체 우선 순위
		enum SNDPRIORITY_ENUM
		{
			SNDPRIORITY_NONE=0,
			SNDPRIORITY_ONCEPLAY=1,	// 한번만 플레이하고 객체 파괴(우선 순위 가장 낮음)
			SNDPRIORITY_LOW,
			SNDPRIORITY_MID,
			SNDPRIORITY_HIGH,
			SNDPRIORITY_KEEPALIVE,	// 파괴하지 않고 항상 존재(우선 순위 가장 높음)
		};

		// 사운드 플레이 예약용 데이터 구조체
		struct SNDPLAYDATA
		{
			DWORD  PlayTime;
			string SndFileName;
			BOOL   bLoop;
			int    Volume;
			int    nSoundKind;	//0 wav,1 ogg ,2 mp3
		};



		////////////////////////////////////////////////////////////


		// 사운드 오브젝트 클래스
		class CSndObj
		{
		public:
			BOOL				m_bCreated;
			BOOL				m_bCreateFailed;

			SNDTYPE_ENUM		m_SndType;
			SNDPRIORITY_ENUM	m_Priority;

			int					m_PlayCnt;
			DWORD				m_CreatedTime;
			DWORD				m_LastPlayTime;

			xSoundBuffer*		m_pSndBuf;
			CMciMedia*			m_pMciMedia;
			xOggStream*			m_pOggStream;

			std::string			m_FileName;
			int					m_Volume;
			int					m_SndPoly;

			BOOL				m_bFade;
			int					m_FadeStartVol;
			int					m_FadeTarVol;
			DWORD				m_FadeStartTime;
			DWORD				m_FadeDelay;

			CSndObj();
			~CSndObj();

			void ClearMember();
			void Destroy();

			BOOL Set(SNDTYPE_ENUM sndtype, LPCTSTR fname, SNDPRIORITY_ENUM priority, int nPoly=1);
			BOOL CreateRes();
			BOOL ReleaseRes();

			BOOL SetVolume(int volume, int channel=-1);
		};


		////////////////////////////////////////////////////////////

		// 사운드 오브젝트 맵
		typedef map<string, CSndObj*> STRING2SNDOBJ;

		// 사운드 플레이 리스트
		typedef list<SNDPLAYDATA*> SNDPLAYLIST;


		// 사운드 연주 및 리소스 매니저 클래스
		class NMSOUND_CLASS CSound  
		{
		protected:
			BOOL			m_bInited;

			BOOL			m_bDontUseSndFx;
			BOOL			m_bDontUseMusic;

			int				m_MainVolume;

			int				m_TotRegistSnd;
			int				m_TotRegistWav;
			int				m_TotRegistMci;
			int				m_TotRegistOgg;

			int				m_TotCreatedSnd;
			int				m_TotCreatedWav;
			int				m_TotCreatedMci;
			int				m_TotCreatedOgg;
		protected:
			char			m_BaseFolder[MAX_PATH];
			STRING2SNDOBJ	m_SndObjMap;
			DWORD			m_AutoOptimizeTime;
			BOOL			m_bEnableConsoleLog;

			SNDPLAYLIST		m_SndPlayList;
			std::set<string>	m_listGameLoadPath;

			BOOL MakeFilePath(LPCTSTR fname, char* pStr);
			BOOL StopSndObj(CSndObj* pObj, int channel);
			void AutoOptimizeRes();						// 사운드 리소스 자동 옵티마이징 함수(내부적으로 호출됨)
			void ConsoleLog(const char *fmt, ...);
			void OnSoundCreate(SNDTYPE_ENUM sndtype, LPCTSTR fname);
			void OnSoundRelease(SNDTYPE_ENUM sndtype, LPCTSTR fname);

			void ProcessPlayList();	// 플레이 리스트를 처리(연주할 시점이 된 사운드를 연주 후 삭제)
			void ProcessFade();		// 페이드 인/아웃 처리
		public:
			CSound();
			virtual ~CSound();

			BOOL Init(HWND hWnd);						// 사운드 매니저 초기화(DSound 초기화 포함)
			void Destroy();

			BOOL IsInited(void){return m_bInited;}

			void ClearAllSound();						// 등록 및 생성한 모든 사운드 객체를 파괴함

			void SetSndVolume( int nVolume);
			void SetBaseFolder(LPCTSTR strfolder);		// 기본 폴더 지정(지정하지 않는 경우 현재 디렉토리로 설정됨)
			void SetSndFx(BOOL bUse);					// 사운드 FX 사용 여부
			void SetMusic(BOOL bUse);					// BGM 사용 여부

			BOOL IsRegistered(LPCTSTR fname);			// 등록된 사운드인가?
			void ClearGameLoadList(void);
			void ReleaseGameLoadList(void);

			// 사운드 파일 등록(기본 폴더를 기반으로 전체 파일 경로가 자동 완성됨)
			BOOL RegistWav(LPCTSTR fname, SNDPRIORITY_ENUM priority, int nPoly=1, BOOL bNowCreate=FALSE);
			BOOL RegistMciMedia(LPCTSTR fname, SNDPRIORITY_ENUM priority, BOOL bNowCreate=FALSE);
			BOOL RegistOggStream(LPCTSTR fname, SNDPRIORITY_ENUM priority, BOOL bNowCreate=FALSE);

			// 등록된 사운드 연주 함수
			BOOL PlayWav(LPCTSTR fname, BOOL bIsLoop=FALSE, int volume=100, BOOL bIgnoreBufferFull=TRUE);
			BOOL PlayMciMedia(LPCTSTR fname, BOOL bIsLoop=TRUE, int volume=100);
			BOOL PlayOggStream(LPCTSTR fname, BOOL bIsLoop=FALSE, int volume=100, int panning = 0, bool use_thread = true);
			BOOL PlayOggStream_BGM(LPCTSTR fname, BOOL bIsLoop=TRUE, int volume=100, int panning = 0, bool use_thread = true);	

			// 사운드 연주 멈춤 함수
			BOOL Stop(LPCTSTR fname, int channel=-1);	// channel = -1 은 모든 채널을 의미
			void StopAllWav();
			void StopAllMusic();
			void StopAllBGM();

			// 볼륨 조절 함수
			BOOL SetVolume(LPCTSTR fname, int volume, int channel=-1);	// channel = -1 은 모든 채널을 의미(WAV인 경우만 사용)

			void SetVolumeAllMusic(int volume, int channel=-1); //임의의 뮤직 플레이 상황에서 사운드 볼륨 조절

			// 페이드 인/아웃(volume = 타겟 볼륨, delay = 볼륨이 1줄어드는데 걸리는 시간)
			BOOL SetFade(LPCTSTR fname, int volume, DWORD delay);

			// 기타 편의 함수
			CSndObj* GetSndObj(LPCTSTR fname);		// 사운드 오브젝트의 포인터를 얻는다

			// 사운드 리소스 관리 함수
			void OptimizeRes();							// 사운드 리소스 최적화(우선순위에 따라 필요없는 리소스 파괴)

			// 콘솔 로그 활성화 함수
			void EnableConsoleLog(BOOL bEnable);

			// 일정 주기로 게임 내에서 계속 호출해야하는 함수
			void RegularCall();

			/////////////////// Play List 관련

			// 연주할 사운드를 플레이 리스트에 추가
			BOOL AddToPlayList(DWORD playtime, LPCTSTR fname, BOOL bIsLoop=TRUE, int volume=100 , int skind = 0);

			void ClearAllPlayList();
		};

		NMSOUND_API extern CSound g_Sound;

	} //namespace SOUND

}	//namespace NMBASE


#endif // !defined(AFX_SOUND_H__EF4CEDCE_DF40_49DC_BF81_995C5AC5DBF3__INCLUDED_)
