// Sound.h: interface for the CSound class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOUND_H__EF4CEDCE_DF40_49DC_BF81_995C5AC5DBF3__INCLUDED_)
#define AFX_SOUND_H__EF4CEDCE_DF40_49DC_BF81_995C5AC5DBF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///////////////////////////
#pragma warning(disable:4786)	// map �� ����ϸ� ��Ÿ���� warning ����ȭ
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

		// ���� ���� Ÿ��
		enum SNDTYPE_ENUM
		{
			SNDTYPE_NONE=0,
			SNDTYPE_WAV=1,			// ���̺� ȿ����
			SNDTYPE_MCIMEDIA,		// MP3, MIDI ����(MCI�� �̿��Ͽ� ����)
			SNDTYPE_OGGSTREAM		// OGG ��Ʈ�� ����
		};

		// ���� ��ü �켱 ����
		enum SNDPRIORITY_ENUM
		{
			SNDPRIORITY_NONE=0,
			SNDPRIORITY_ONCEPLAY=1,	// �ѹ��� �÷����ϰ� ��ü �ı�(�켱 ���� ���� ����)
			SNDPRIORITY_LOW,
			SNDPRIORITY_MID,
			SNDPRIORITY_HIGH,
			SNDPRIORITY_KEEPALIVE,	// �ı����� �ʰ� �׻� ����(�켱 ���� ���� ����)
		};

		// ���� �÷��� ����� ������ ����ü
		struct SNDPLAYDATA
		{
			DWORD  PlayTime;
			string SndFileName;
			BOOL   bLoop;
			int    Volume;
			int    nSoundKind;	//0 wav,1 ogg ,2 mp3
		};



		////////////////////////////////////////////////////////////


		// ���� ������Ʈ Ŭ����
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

		// ���� ������Ʈ ��
		typedef map<string, CSndObj*> STRING2SNDOBJ;

		// ���� �÷��� ����Ʈ
		typedef list<SNDPLAYDATA*> SNDPLAYLIST;


		// ���� ���� �� ���ҽ� �Ŵ��� Ŭ����
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
			void AutoOptimizeRes();						// ���� ���ҽ� �ڵ� ��Ƽ����¡ �Լ�(���������� ȣ���)
			void ConsoleLog(const char *fmt, ...);
			void OnSoundCreate(SNDTYPE_ENUM sndtype, LPCTSTR fname);
			void OnSoundRelease(SNDTYPE_ENUM sndtype, LPCTSTR fname);

			void ProcessPlayList();	// �÷��� ����Ʈ�� ó��(������ ������ �� ���带 ���� �� ����)
			void ProcessFade();		// ���̵� ��/�ƿ� ó��
		public:
			CSound();
			virtual ~CSound();

			BOOL Init(HWND hWnd);						// ���� �Ŵ��� �ʱ�ȭ(DSound �ʱ�ȭ ����)
			void Destroy();

			BOOL IsInited(void){return m_bInited;}

			void ClearAllSound();						// ��� �� ������ ��� ���� ��ü�� �ı���

			void SetSndVolume( int nVolume);
			void SetBaseFolder(LPCTSTR strfolder);		// �⺻ ���� ����(�������� �ʴ� ��� ���� ���丮�� ������)
			void SetSndFx(BOOL bUse);					// ���� FX ��� ����
			void SetMusic(BOOL bUse);					// BGM ��� ����

			BOOL IsRegistered(LPCTSTR fname);			// ��ϵ� �����ΰ�?
			void ClearGameLoadList(void);
			void ReleaseGameLoadList(void);

			// ���� ���� ���(�⺻ ������ ������� ��ü ���� ��ΰ� �ڵ� �ϼ���)
			BOOL RegistWav(LPCTSTR fname, SNDPRIORITY_ENUM priority, int nPoly=1, BOOL bNowCreate=FALSE);
			BOOL RegistMciMedia(LPCTSTR fname, SNDPRIORITY_ENUM priority, BOOL bNowCreate=FALSE);
			BOOL RegistOggStream(LPCTSTR fname, SNDPRIORITY_ENUM priority, BOOL bNowCreate=FALSE);

			// ��ϵ� ���� ���� �Լ�
			BOOL PlayWav(LPCTSTR fname, BOOL bIsLoop=FALSE, int volume=100, BOOL bIgnoreBufferFull=TRUE);
			BOOL PlayMciMedia(LPCTSTR fname, BOOL bIsLoop=TRUE, int volume=100);
			BOOL PlayOggStream(LPCTSTR fname, BOOL bIsLoop=FALSE, int volume=100, int panning = 0, bool use_thread = true);
			BOOL PlayOggStream_BGM(LPCTSTR fname, BOOL bIsLoop=TRUE, int volume=100, int panning = 0, bool use_thread = true);	

			// ���� ���� ���� �Լ�
			BOOL Stop(LPCTSTR fname, int channel=-1);	// channel = -1 �� ��� ä���� �ǹ�
			void StopAllWav();
			void StopAllMusic();
			void StopAllBGM();

			// ���� ���� �Լ�
			BOOL SetVolume(LPCTSTR fname, int volume, int channel=-1);	// channel = -1 �� ��� ä���� �ǹ�(WAV�� ��츸 ���)

			void SetVolumeAllMusic(int volume, int channel=-1); //������ ���� �÷��� ��Ȳ���� ���� ���� ����

			// ���̵� ��/�ƿ�(volume = Ÿ�� ����, delay = ������ 1�پ��µ� �ɸ��� �ð�)
			BOOL SetFade(LPCTSTR fname, int volume, DWORD delay);

			// ��Ÿ ���� �Լ�
			CSndObj* GetSndObj(LPCTSTR fname);		// ���� ������Ʈ�� �����͸� ��´�

			// ���� ���ҽ� ���� �Լ�
			void OptimizeRes();							// ���� ���ҽ� ����ȭ(�켱������ ���� �ʿ���� ���ҽ� �ı�)

			// �ܼ� �α� Ȱ��ȭ �Լ�
			void EnableConsoleLog(BOOL bEnable);

			// ���� �ֱ�� ���� ������ ��� ȣ���ؾ��ϴ� �Լ�
			void RegularCall();

			/////////////////// Play List ����

			// ������ ���带 �÷��� ����Ʈ�� �߰�
			BOOL AddToPlayList(DWORD playtime, LPCTSTR fname, BOOL bIsLoop=TRUE, int volume=100 , int skind = 0);

			void ClearAllPlayList();
		};

		NMSOUND_API extern CSound g_Sound;

	} //namespace SOUND

}	//namespace NMBASE


#endif // !defined(AFX_SOUND_H__EF4CEDCE_DF40_49DC_BF81_995C5AC5DBF3__INCLUDED_)
