#pragma once


#include <string>
#include <map>
#include "SoundDefine.h"
#include "sound.h"


using namespace std;

//class xSprite;	// 종환 사용을 안 하는 방향으로 간다.

namespace NMBASE
{
	namespace SOUND
	{

		//////////////////////////////////////////////////////////////////////////////
		// CSoundMananger
		class NMSOUND_CLASS CSoundMananger
		{
		public:
			CSoundMananger();
			virtual ~CSoundMananger();

			void		Initialize( CWnd *pParent );
			bool		RegistAudio( const char *pLuaFilePath, void (*pFunction)() = NULL );
			void		Destroy();

			bool		PlayWav( string strKeySet );
			bool		StopWav( string strKeySet );
			bool		PlayWav( string strKeyType, string strKeyIndex );
			bool		StopWav( string strKeyType, string strKeyIndex );

			// 종환 주석 xsprite때문에...
			//	bool		AutoPlayMSceneSound( xSprite* pSpr, int nMSIndex );
			void		RegularCall();

			bool		PlayWavForDelay( string strKeySet );
			bool		PlayWavForDelay( string strKeyType, string strKeyIndex );
			void		ProcessPlayData();

			bool		OncePlayWav( char* szFilename );
			void		StopAllWav();
			void		StopAllMusic();

			BOOL		GetState();
			void		SetMusic( BOOL bUse );
			void		SetSndFx( BOOL bUse );
			void		SetVolumn( int nVolumn );

		protected:
			// map control
			bool		Clear();
			bool		Attach( char* szKeyType, char* szKeyIndex, char* szFilename );
			bool		AttachFolder( char* szKeyType, char* szKeyIndex, char* szFoldername );
			string	MakeKeySet( string strKeyType, string strKeyIndex );
			bool		IsThisSameExt( string strFilename, string strExt, char cDelim = '.' );

		protected:
			typedef multimap< string, string >	MMAPKEYSET;
			CSound		m_Sound;
			MMAPKEYSET	m_mmapKeySet;
			CWnd*		m_pParent;

		protected:
			typedef struct tagPLAY_DATA
			{
				string strKeySet;
				DWORD		dwDelay;

				tagPLAY_DATA() : strKeySet(""), dwDelay(0)	{}
			} PLAY_DATA;

			typedef list< PLAY_DATA >	PLAYDATALIST;
			enum {	UNIT_DELAY_PLAYDATA = 100,	};

			PLAYDATALIST		m_lstPlayData;

		protected:
			int					m_nVolumn;

		protected:
			// load count control
			void		(*m_pLoadCallback)();
		};


		NMSOUND_CLASS extern CSoundMananger g_NMSoundManager;

	} //namespace SOUND


}	//namespace NMBASE