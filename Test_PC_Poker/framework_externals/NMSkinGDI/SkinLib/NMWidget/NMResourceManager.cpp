#include "stdafx.h"
#include "./NMWidget/NMResourceManager.h"

namespace NMBASE
{
	namespace SKINGDI
	{

		CNMSpriteManager NMRESSM;
		CNMAudioMananger NMRESAM;

		//////////////////////////////////////////////////////////////////////////////
		// CNMSpriteManager class
		CNMSpriteManager::CNMSpriteManager()
		{
			m_pLoadCallback = NULL;
			Clear();
			MakeDefaultSprite();
		}

		CNMSpriteManager::~CNMSpriteManager()
		{
			Clear();
		}

		bool CNMSpriteManager::RegistSprite( const char *pLuaFilePath, void (*pFunction)())
		{
			m_pLoadCallback = pFunction;

			lua_State* pLS = lua_open();

			if( pLS == NULL )
			{
				AfxMessageBox( "CNMSpriteManager::RegistSprite error - pLS가 초기화 되지 않았습니다." );
				return false;
			}

			lua_tinker::dofile( pLS, pLuaFilePath );

			lua_tinker::class_add<CNMSpriteManager>( pLS, "CNMSpriteManager" );
			lua_tinker::class_def<CNMSpriteManager>( pLS, "Attach", &CNMSpriteManager::Attach );

			lua_tinker::call<void>( pLS, "RegistSprite", this );

			lua_close( pLS );

			return true;
		}

		bool CNMSpriteManager::RegistSkin( int nSkinIndex , const char *pLuaFilePath)
		{
			lua_State* pLS = lua_open();

			if( pLS == NULL )
			{
				AfxMessageBox( "CNMSpriteManager::RegistSkin error - pLS가 초기화 되지 않았습니다." );
				return false;
			}

			lua_tinker::dofile( pLS, pLuaFilePath );

			lua_tinker::class_add<CNMSpriteManager>( pLS, "CNMSpriteManager" );
			lua_tinker::class_def<CNMSpriteManager>( pLS, "Attach", &CNMSpriteManager::Attach );

			lua_tinker::call<void>( pLS, "RegistSkin", this, nSkinIndex );

			lua_close( pLS );

			return true;
		}

		bool CNMSpriteManager::RegistWindowMode( int nSkinIndex, int nMode , const char *pLuaFilePath)
		{
			lua_State* pLS = lua_open();

			if( pLS == NULL )
			{
				AfxMessageBox( "CNMSpriteManager::RegistSkin error - pLS가 초기화 되지 않았습니다." );
				return false;
			}

			lua_tinker::dofile( pLS, pLuaFilePath );

			lua_tinker::class_add<CNMSpriteManager>( pLS, "CNMSpriteManager" );
			lua_tinker::class_def<CNMSpriteManager>( pLS, "Attach", &CNMSpriteManager::Attach );

			lua_tinker::call<void>( pLS, "RegistWindowMode", this, nSkinIndex, nMode );

			lua_close( pLS );

			return true;
		}


		NMBASE::GRAPHICGDI::xSprite* CNMSpriteManager::GetSprite( std::string strKey )
		{
			if( m_mapSprite.find( strKey ) == m_mapSprite.end() )
			{
				return &m_DefaultSpr;
			}
			else
			{
				if( !m_mapSprite[strKey] )
					return &m_DefaultSpr;
			}

			return m_mapSprite[strKey];
		}

		bool CNMSpriteManager::Clear()
		{
			for( MAPSPRITE::iterator it = m_mapSprite.begin(); it != m_mapSprite.end(); it++ )
			{
				if( it->second )
					delete it->second;
			}
			m_mapSprite.clear();

			return true;
		}

		bool CNMSpriteManager::Attach( char* szKey, char* szFilename )
		{
			std::string strKey = szKey;

			bool bExist = false;
			NMBASE::GRAPHICGDI::xSprite* pSprite = NULL;
			if( m_mapSprite.find( strKey ) != m_mapSprite.end() )
			{
				pSprite = m_mapSprite[strKey];

				BOOL bRet = pSprite->Load( szFilename );
				if( bRet == FALSE )
				{
					CString strMsg;
					strMsg.Format( "spr change error!\n %s", szFilename );
					AfxMessageBox( strMsg );

					return false;
				}
			}
			else
			{
				pSprite = new NMBASE::GRAPHICGDI::xSprite();

				BOOL bRet = pSprite->Load( szFilename );
				if( bRet == FALSE )
				{
					CString strMsg;
					strMsg.Format( "spr load error!\n %s", szFilename );
					AfxMessageBox( strMsg );
					delete pSprite;

					return false;
				}

				m_mapSprite.insert( MAPSPRITE::value_type( strKey, pSprite ) );

				TRACE( "CNMSpriteManager::Attach( newtype )( %s, %s )\n", szKey, szFilename );

				/////////////////////////////////////////////////////////////////////
				// 스프라이트 데이터 한개 로딩 완료 메세지
				if( m_pLoadCallback )
					m_pLoadCallback();
				/////////////////////////////////////////////////////////////////////
			}

			return true;
		}

		void CNMSpriteManager::MakeDefaultSprite()
		{
			// 	if( m_DefaultSpr.Load( ".\\Data\\Common\\Default.spr" ) == FALSE )
			// 	{
			// 		// 디폴트 이미지도 없을때는 다른식의 예외 처리가 필요하다.
			// 		AfxMessageBox( "프로그램 실행에 필요한 파일을 읽을 수 없습니다.\n프로그램을 재설치 하시는 것을 권장합니다." );
			// 	}   
		}  

		//////////////////////////////////////////////////////////////////////////////
		// CNMAudioMananger class

		CNMAudioMananger::CNMAudioMananger()
		{
			m_pLoadCallback = NULL;
			Clear();
			m_nVolumn = 100;
			m_pParent = NULL;
		}

		CNMAudioMananger::~CNMAudioMananger()
		{
			Clear();
		}

		bool CNMAudioMananger::RegistAudio( const char *pLuaFilePath, void (*pFunction)() )
		{
			m_pLoadCallback = pFunction;

			lua_State* pLS = lua_open();

			if( pLS == NULL )
			{
				AfxMessageBox( "CNMAudioMananger::RegistAudio error - pLS가 초기화 되지 않았습니다." );
				return false;
			}

			lua_tinker::dofile( pLS, pLuaFilePath );

			lua_tinker::class_add<CNMAudioMananger>( pLS, "CNMSpriteManager" );
			lua_tinker::class_def<CNMAudioMananger>( pLS, "Attach", &CNMAudioMananger::Attach );
			lua_tinker::class_def<CNMAudioMananger>( pLS, "AttachFolder", &CNMAudioMananger::AttachFolder );

			lua_tinker::call<void>( pLS, "RegistAudio", this );

			lua_close( pLS );

			return true;
		}
		bool CNMAudioMananger::PlayWav( std::string strKeySet, int sndIndex )
		{
			if( strKeySet.length() <= 0 )	return false;

			// Get Random Access
			MMAPKEYSET::iterator itfind = m_mmapKeySet.find( strKeySet );
			if( itfind == m_mmapKeySet.end() )
			{
				TRACE( "nothing - CNMAudioMananger::PlayWav( %s )\n", strKeySet.c_str() );
				return false;
			}

			MMAPKEYSET::iterator itLow = m_mmapKeySet.lower_bound( strKeySet );
			if( itLow == m_mmapKeySet.end() )
			{
				return false;
			}
			int nCount = m_mmapKeySet.count( strKeySet );
			if( nCount == 0 )
			{
				TRACE( "Count==0 - CNMAudioMananger::PlayWav( %s )\n", strKeySet.c_str() );
				return false;
			}

			// 인덱스 지정 play 추가 [10/20/2011 guesty]
			int nKey = 0;
			if(sndIndex < 0 || sndIndex >= nCount)
			{
				nKey = rand()%nCount;
			}
			else
			{	
				nKey = sndIndex;
			}

			for( int i=0; i<nKey; i++ )	itLow++;
			
			// set filename
			std::string strFilename = (itLow->second);
			if( strFilename.length() == 0 ) return false;

			BOOL bRet = FALSE;
			if( IsThisSameExt( strFilename, "wav" ) )
			{
				bRet = NMBASE::SOUND::g_Sound.PlayWav( strFilename.c_str(), FALSE, m_nVolumn, TRUE );
			}
			else if( IsThisSameExt( strFilename, "ogg" ) )
			{
				bRet = NMBASE::SOUND::g_Sound.PlayOggStream_BGM( strFilename.c_str(), FALSE, m_nVolumn, TRUE );
			}

			if( bRet == FALSE )
			{
				TRACE( "Error - CNMAudioMananger::PlayWav( %s )\n", strKeySet.c_str() );
				return false;
			}
			//	TRACE( "CNMAudioMananger::PlayWav( %s )\n", strKeySet.c_str() );

			return true;
		}

		bool CNMAudioMananger::StopWav( std::string strKeySet )
		{
			MMAPKEYSET::iterator itLow = m_mmapKeySet.lower_bound( strKeySet );
			if( itLow == m_mmapKeySet.end() ) return false;
			int nCount = m_mmapKeySet.count( strKeySet );
			if( nCount == 0 ) return false;

			for( int i=0; i<nCount; i++ )
			{
				NMBASE::SOUND::g_Sound.Stop( (char*)((itLow->second).c_str()) );
				itLow++;
			}

			return true;
		}

		bool CNMAudioMananger::PlayWav( std::string strKeyType, std::string strKeyIndex )
		{
			return PlayWav( MakeKeySet( strKeyType, strKeyIndex ) );
		}

		bool CNMAudioMananger::StopWav( std::string strKeyType, std::string strKeyIndex )
		{
			return StopWav( MakeKeySet( strKeyType, strKeyIndex ) );
		}

		// BGM용 반복 재생 필요해서 추가 [11/21/2011 guesty]
		bool CNMAudioMananger::PlayLoop( std::string strKeySet, int sndIndex )
		{
			if( strKeySet.length() <= 0 )	return false;

			// Get Random Access
			MMAPKEYSET::iterator itfind = m_mmapKeySet.find( strKeySet );
			if( itfind == m_mmapKeySet.end() )
			{
				TRACE( "nothing - CNMAudioMananger::PlayWav( %s )\n", strKeySet.c_str() );
				return false;
			}

			MMAPKEYSET::iterator itLow = m_mmapKeySet.lower_bound( strKeySet );
			if( itLow == m_mmapKeySet.end() )
			{
				return false;
			}
			int nCount = m_mmapKeySet.count( strKeySet );
			if( nCount == 0 )
			{
				TRACE( "Count==0 - CNMAudioMananger::PlayWav( %s )\n", strKeySet.c_str() );
				return false;
			}

			// 인덱스 지정 play 추가 [10/20/2011 guesty]
			int nKey = 0;
			if(sndIndex < 0 || sndIndex >= nCount)
			{
				nKey = rand()%nCount;
			}
			else
			{	
				nKey = sndIndex;
			}

			for( int i=0; i<nKey; i++ )	itLow++;

			// set filename
			std::string strFilename = (itLow->second);
			if( strFilename.length() == 0 ) return false;

			BOOL bRet = FALSE;
			if( IsThisSameExt( strFilename, "wav" ) )
			{
				bRet = NMBASE::SOUND::g_Sound.PlayWav( strFilename.c_str(),  TRUE, m_nVolumn, TRUE );
			}
			else if( IsThisSameExt( strFilename, "ogg" ) )
			{
				bRet = NMBASE::SOUND::g_Sound.PlayOggStream_BGM( strFilename.c_str(), TRUE, m_nVolumn, TRUE );
			}

			if( bRet == FALSE )
			{
				TRACE( "Error - CNMAudioMananger::PlayWav( %s )\n", strKeySet.c_str() );
				return false;
			}
			//	TRACE( "CNMAudioMananger::PlayWav( %s )\n", strKeySet.c_str() );

			return true;
		}

		bool CNMAudioMananger::AutoPlayMSceneSound( NMBASE::GRAPHICGDI::xSprite* pSpr, int nMSIndex )
		{
			// 멀티씬의 사운드를 플레이 리스트에 추가하여 자동으로 연주되도록 함(CSound::RegularCall()을 주기적으로 호출해주어야함)

			if( !pSpr )
				return false;


			if( m_pParent )
			{
				if( IsMinimized( m_pParent->GetSafeHwnd() ) )
					return false;
			}

			DWORD curtime = timeGetTime();

			NMBASE::GRAPHICGDI::QUERYMSCENE_SOUND sndquery;
			sndquery.Reset();
			while( pSpr->QueryMSceneSound( &sndquery, nMSIndex, 0, 0xFFFFFFFF, FALSE ) )
			{
				// 등록되지 않은 사운드 파일이면 등록
				if( !NMBASE::SOUND::g_Sound.IsRegistered( sndquery.SndFile ) )			
					NMBASE::SOUND::g_Sound.RegistWav( sndquery.SndFile, NMBASE::SOUND::SNDPRIORITY_MID, 1, FALSE );
				//NMBASE::SOUND::g_Sound.RegistWav(sndquery.SndFile, SNDPRIORITY_ONCEPLAY, 1, FALSE);

				NMBASE::SOUND::g_Sound.AddToPlayList( curtime + sndquery.PlayTime, sndquery.SndFile, FALSE, sndquery.Volume );
			}

			return true;
		}

		void CNMAudioMananger::RegularCall()
		{
			ProcessPlayData();
			NMBASE::SOUND::g_Sound.RegularCall();
		}

		bool CNMAudioMananger::PlayWavForDelay( std::string strKeySet )
		{
			if( m_pParent )
			{
				if( IsMinimized( m_pParent->GetSafeHwnd() ) )
					return false;
			}

			PLAY_DATA tmpPlayData;
			tmpPlayData.strKeySet = strKeySet;
			tmpPlayData.dwDelay = GetTickCount() + 10 + ( m_lstPlayData.size() * UNIT_DELAY_PLAYDATA );
			m_lstPlayData.push_back(tmpPlayData);

			return true;
		}

		bool CNMAudioMananger::PlayWavForDelay( std::string strKeyType, std::string strKeyIndex )
		{
			return PlayWavForDelay( MakeKeySet( strKeyType, strKeyIndex ) );
		}

		void CNMAudioMananger::ProcessPlayData()
		{
			if( m_lstPlayData.empty() )	return;

			DWORD dwCurrentTick = GetTickCount();

			while( !m_lstPlayData.empty() )
			{
				PLAYDATALIST::iterator it = m_lstPlayData.begin();

				if( dwCurrentTick >= it->dwDelay )
				{
					PlayWav( it->strKeySet );
					m_lstPlayData.erase(it);
				}
				else
				{
					break;
				}
			}
		}

		bool CNMAudioMananger::OncePlayWav( char* szFilename )
		{
			NMBASE::SOUND::g_Sound.RegistWav( szFilename, NMBASE::SOUND::SNDPRIORITY_ONCEPLAY );
			NMBASE::SOUND::g_Sound.PlayWav( szFilename );
			return true;
		}

		void CNMAudioMananger::StopAllWav()
		{
			NMBASE::SOUND::g_Sound.StopAllWav();
		}

		void CNMAudioMananger::StopAllMusic()
		{
			NMBASE::SOUND::g_Sound.StopAllMusic();
		}

		void CNMAudioMananger::Initialize( CWnd *pParent )
		{
			m_pParent = pParent;
			NMBASE::SOUND::g_Sound.Init( m_pParent->GetSafeHwnd() );
		}

		BOOL CNMAudioMananger::GetState()
		{
			return NMBASE::SOUND::g_Sound.IsInited();
		}

		void CNMAudioMananger::Destroy()
		{
			NMBASE::SOUND::g_Sound.Destroy();
		}

		void CNMAudioMananger::SetMusic( BOOL bUse )
		{
			NMBASE::SOUND::g_Sound.SetMusic( bUse );
		}

		void CNMAudioMananger::SetSndFx( BOOL bUse )
		{
			NMBASE::SOUND::g_Sound.SetSndFx( bUse );
		}

		void CNMAudioMananger::SetVolumn( int nVolumn )
		{
			if( nVolumn < 0 || nVolumn >= 100 )	return;

			NMBASE::SOUND::g_Sound.SetSndVolume( nVolumn );
		}

		bool CNMAudioMananger::SetVolumn( std::string strKeySet, int nVolumn, int sndIndex )
		{
			if( strKeySet.length() <= 0 )
				return false;

			// Get Random Access
			MMAPKEYSET::iterator itfind = m_mmapKeySet.find( strKeySet );
			if( itfind == m_mmapKeySet.end() )
			{
				TRACE( "nothing - CNMAudioMananger::SetVolumn( %s )\n", strKeySet.c_str() );
				return false;
			}

			MMAPKEYSET::iterator itLow = m_mmapKeySet.lower_bound( strKeySet );
			if( itLow == m_mmapKeySet.end() )
			{
				return false;
			}
			int nCount = m_mmapKeySet.count( strKeySet );
			if( nCount == 0 )
			{
				TRACE( "Count==0 - CNMAudioMananger::SetVolumn( %s )\n", strKeySet.c_str() );
				return false;
			}

			int nKey = 0;
			if(sndIndex >= 0 && sndIndex < nCount)
			{	
				nKey = sndIndex;
			}

			for( int i=0; i<nKey; i++ )
				itLow++;

			// set filename
			std::string strFilename = (itLow->second);
			if( strFilename.length() == 0 )
				return false;

			BOOL bRet = FALSE;
			bRet = NMBASE::SOUND::g_Sound.SetVolume( strFilename.c_str(), nVolumn );

			if( bRet == FALSE )
			{
				TRACE( "Error - CNMAudioMananger::SetVolume( %s )\n", strKeySet.c_str() );
				return false;
			}
			//	TRACE( "CNMAudioMananger::PlayWav( %s )\n", strKeySet.c_str() );

			return true;
		}

		bool CNMAudioMananger::Clear()
		{
			m_mmapKeySet.clear();

			return true;
		}

		bool CNMAudioMananger::Attach( char* szKeyType, char* szKeyIndex, char* szFilename )
		{
			std::string	strKeySet = MakeKeySet( szKeyType, szKeyIndex );

			// regist wav/ogg file
			std::string strFilename = szFilename;
			BOOL bRet = FALSE;
			if( IsThisSameExt( strFilename, "wav" ) )
			{
				bRet = NMBASE::SOUND::g_Sound.RegistWav( szFilename, NMBASE::SOUND::SNDPRIORITY_MID, 1, false );
			}
			else if( IsThisSameExt( strFilename, "ogg" ) )
			{
				bRet = NMBASE::SOUND::g_Sound.RegistOggStream( szFilename, NMBASE::SOUND::SNDPRIORITY_MID, false );
			}

			if( bRet == FALSE )
			{
				CString strMsg;
				strMsg.Format( "%s : %s regist error!\n", strKeySet.c_str(), strFilename.c_str() );
				AfxMessageBox( strMsg );
				return false;
			}

			// regist m_mmapKeySet
			m_mmapKeySet.insert( MMAPKEYSET::value_type( strKeySet, strFilename ) );

			TRACE( "CNMAudioMananger::Attach( %s, %s )\n", strKeySet.c_str(), strFilename.c_str() );

			/////////////////////////////////////////////////////////////////////
			// 오디오 데이터 한개 세팅 완료 메세지
			if( m_pLoadCallback )
				m_pLoadCallback();
			/////////////////////////////////////////////////////////////////////

			return true;
		}

		bool CNMAudioMananger::AttachFolder( char* szKeyType, char* szKeyIndex, char* szFoldername )
		{
			CFileFind	fileFinder;
			BOOL bWorking = FALSE;

			char szDir[512]={0,};
			GetCurrentDirectory( sizeof(szDir)-1, szDir );
			CString strFindName;
			strFindName.Format( "%s%s*.ogg", szDir, szFoldername );

			bWorking = fileFinder.FindFile( strFindName );
			while( bWorking )
			{
				bWorking = fileFinder.FindNextFile();
				wsprintf( szDir, fileFinder.GetFilePath() );
				Attach( szKeyType, szKeyIndex, szDir );
			}

			fileFinder.Close();
			return true;
		}


		std::string CNMAudioMananger::MakeKeySet( std::string strKeyType, std::string strKeyIndex )
		{
			// make KeySet : "szKeyType-szKeyIndex"
			static std::string strKeySet;
			strKeySet = strKeyType + "-" + strKeyIndex;
			return strKeySet;
		}

		bool CNMAudioMananger::IsThisSameExt( std::string strFilename, std::string strExt, char cDelim )
		{
			if( strExt.find(cDelim) < strExt.size() )
				return false;

			std::string::size_type ext_pos = strFilename.rfind(cDelim);
			if( ext_pos >= strFilename.size() )
				return false;

			std::string ext_text( strFilename.begin()+ext_pos+1, strFilename.end() );
			if( strExt.size() != ext_text.size())
				return false;

			return ( 0 == strnicmp( ext_text.c_str(), strExt.c_str(), strExt.size() ) );
		}



		//////////////////////////////////////////////////////////////////////////////
		// Help functions

		/////////////////////////////////////////////////////////////////////////////////
		//const char szAudioDBFile[] = "Script/DBAudio.lua";
		//const char szAudioDBFile[] = "Script/DBAudio.nml";
		//const char szSpriteDBFile[] = "Script/DBSprite.lua";
		//const char szSpriteDBFile[] = "Script/DBSprite.nml";
		//const char szGameAnimationLua[] = "Script/GameAnimation.lua";
		//const char szGameAnimationLua[] = "Script/GameAnimation.nml";
		//const char szGameSeatLua[] = "Script/GameSeat.lua";
		//const char szGameSeatLua[] = "Script/GameSeat.nml";
		//////////////////////////////////////////////////////////////////////////////

		// static CString strFindName = "";
		// char* GetSafeLuaFile( char* szFilename )
		// {
		//	if( szFilename == NULL )		return "default";
		//	if( strlen(szFilename) <= 0 )	return "default";
		//
		//	CFileFind fileFinder;
		//	char szDir[512]={0,};
		//	GetCurrentDirectory( sizeof(szDir)-1, szDir );
		//
		//	strFindName.Format( "%s\\Lobby\\Script\\%s.lua", szDir, szFilename );
		//	if( fileFinder.FindFile( strFindName ) )
		//	{
		//		fileFinder.Close();
		//		return (char*)strFindName.operator LPCTSTR();
		//	}
		//	else
		//	{
		//		fileFinder.Close();
		//		strFindName.Format( "%s\\Lobby\\Script\\%s.nml", szDir, szFilename );
		//		if( fileFinder.FindFile( strFindName ) )
		//		{
		//			fileFinder.Close();
		//			return (char*)strFindName.operator LPCTSTR();
		//		}
		//		fileFinder.Close();
		//		AfxMessageBox( "프로그램 실행에 필요한 파일을 읽을 수 없습니다.\n프로그램을 재설치 하시는 것을 권장합니다." );
		//	}
		//
		//	return "default";
		//}

	} //namespace SKINGDI

}//namespace NMBASE