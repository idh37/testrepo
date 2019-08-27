#pragma once

#include <string>
#include <map>
#include "lua_tinker.h"
#include "skingdidefine.h"


namespace NMBASE
{
	namespace SKINGDI
	{

		class CNMSpriteManager;
		class CNMAudioMananger;

		NMSKINGDI_CLASS extern CNMSpriteManager NMRESSM;
		NMSKINGDI_CLASS extern CNMAudioMananger NMRESAM;

		//////////////////////////////////////////////////////////////////////////////
		// CNMSpriteManager
		class NMSKINGDI_CLASS CNMSpriteManager
		{
		public:
			CNMSpriteManager();
			virtual ~CNMSpriteManager();

			// Interface
			bool		RegistSprite( const char *pLuaFilePath, void (*pFunction)() = NULL );
			bool		RegistSkin( int nSkinIndex , const char *pLuaFilePath);
			bool		RegistWindowMode( int nSkinIndex, int nMode , const char *pLuaFilePath);
			NMBASE::GRAPHICGDI::xSprite*	GetSprite( std::string strKey );

		protected:
			// map control
			bool		Clear();
			bool		Attach( char* szKey, char* szFilename );

			void		MakeDefaultSprite();

		protected:
			typedef std::map< std::string, NMBASE::GRAPHICGDI::xSprite* >		MAPSPRITE;
			MAPSPRITE	m_mapSprite;
			NMBASE::GRAPHICGDI::xSprite		m_DefaultSpr;

		protected:
			// load count control
			void		(*m_pLoadCallback)();
		};

		//////////////////////////////////////////////////////////////////////////////
		// CNMAudioMananger
		class NMSKINGDI_CLASS CNMAudioMananger
		{
		public:
			CNMAudioMananger();
			virtual ~CNMAudioMananger();

			void		Initialize( CWnd *pParent );
			bool		RegistAudio( const char *pLuaFilePath, void (*pFunction)() = NULL );
			void		Destroy();

			bool		PlayWav( std::string strKeySet, int sndIndex = -1);
			bool		StopWav( std::string strKeySet );
			bool		PlayWav( std::string strKeyType, std::string strKeyIndex );
			bool		StopWav( std::string strKeyType, std::string strKeyIndex );

			// BGM용 반복 재생 필요해서 추가 [11/21/2011 guesty]
			bool		PlayLoop( std::string strKeySet, int sndIndex = -1 );
			
			bool		AutoPlayMSceneSound( NMBASE::GRAPHICGDI::xSprite* pSpr, int nMSIndex );
			void		RegularCall();

			bool		PlayWavForDelay( std::string strKeySet );
			bool		PlayWavForDelay( std::string strKeyType, std::string strKeyIndex );
			void		ProcessPlayData();

			bool		OncePlayWav( char* szFilename );
			void		StopAllWav();
			void		StopAllMusic();

			BOOL		GetState();
			void		SetMusic( BOOL bUse );
			void		SetSndFx( BOOL bUse );
			void		SetVolumn( int nVolumn );

			bool		SetVolumn( std::string strKeySet, int nVolumn, int sndIndex );


		protected:
			// map control
			bool		Clear();
			bool		Attach( char* szKeyType, char* szKeyIndex, char* szFilename );
			bool		AttachFolder( char* szKeyType, char* szKeyIndex, char* szFoldername );
			std::string	MakeKeySet( std::string strKeyType, std::string strKeyIndex );
			bool		IsThisSameExt( std::string strFilename, std::string strExt, char cDelim = '.' );

		protected:
			typedef std::multimap< std::string, std::string >	MMAPKEYSET;
			//NMBASE::SOUND::CSound		m_Sound;
			MMAPKEYSET	m_mmapKeySet;
			CWnd*		m_pParent;

		protected:
			typedef struct tagPLAY_DATA
			{
				std::string strKeySet;
				DWORD		dwDelay;

				tagPLAY_DATA() : strKeySet(""), dwDelay(0)	{}
			} PLAY_DATA;

			typedef std::list< PLAY_DATA >	PLAYDATALIST;
			enum {	UNIT_DELAY_PLAYDATA = 100,	};

			PLAYDATALIST		m_lstPlayData;

		protected:
			int					m_nVolumn;

		protected:
			// load count control
			void		(*m_pLoadCallback)();
		};

		//////////////////////////////////////////////////////////////////////////////
		// CNMFontManager
		class NMSKINGDI_CLASS CNMFontManager
		{
			// 설계중
		};


		//////////////////////////////////////////////////////////////////////////////
		// Help functions
		//char* GetSafeLuaFile( char* szFilename );
	} //namespace SKINGDI

}//namespace NMBASE