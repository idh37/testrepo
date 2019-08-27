#include "stdafx.h"
#include "SoundManager.h"

namespace NMBASE
{
	namespace SOUND
	{

		CSoundMananger g_NMSoundManager;

		//////////////////////////////////////////////////////////////////////////////
		// CSoundMananger class

		CSoundMananger::CSoundMananger()
		{
			m_pLoadCallback = NULL;
			Clear();
			m_nVolumn = 100;
			m_pParent = NULL;
		}

		CSoundMananger::~CSoundMananger()
		{
			Clear();
		}

		bool CSoundMananger::RegistAudio( const char *pLuaFilePath, void (*pFunction)() )
		{
			m_pLoadCallback = pFunction;

			// 루아때문에 종환 주석 나중에 관련 부분을 본다.
			/*lua_State* pLS = lua_open();

			if( pLS == NULL )
			{
			AfxMessageBox( "CSoundMananger::RegistAudio error - pLS가 초기화 되지 않았습니다." );
			return false;
			}

			lua_tinker::dofile( pLS, pLuaFilePath );

			lua_tinker::class_add<CSoundMananger>( pLS, "CNMSpriteManager" );
			lua_tinker::class_def<CSoundMananger>( pLS, "Attach", &CSoundMananger::Attach );
			lua_tinker::class_def<CSoundMananger>( pLS, "AttachFolder", &CSoundMananger::AttachFolder );

			lua_tinker::call<void>( pLS, "RegistAudio", this );

			lua_close( pLS );*/

			return true;
		}
		bool CSoundMananger::PlayWav( std::string strKeySet )
		{
			if( strKeySet.length() <= 0 )	return false;

			// Get Random Access
			MMAPKEYSET::iterator itfind = m_mmapKeySet.find( strKeySet );
			if( itfind == m_mmapKeySet.end() )
			{
				TRACE( "nothing - CSoundMananger::PlayWav( %s )\n", strKeySet.c_str() );
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
				TRACE( "Count==0 - CSoundMananger::PlayWav( %s )\n", strKeySet.c_str() );
				return false;
			}
			int nRandKey = rand()%nCount;
			for( int i=0; i<nRandKey; i++ )	itLow++;

			// set filename
			std::string strFilename = (itLow->second);
			if( strFilename.length() == 0 ) return false;

			BOOL bRet = FALSE;
			if( IsThisSameExt( strFilename, "wav" ) )
			{
				bRet = m_Sound.PlayWav( strFilename.c_str(), FALSE, m_nVolumn, TRUE );
			}
			else if( IsThisSameExt( strFilename, "ogg" ) )
			{
				bRet = m_Sound.PlayOggStream( strFilename.c_str(), FALSE, m_nVolumn, TRUE );
			}

			if( bRet == FALSE )
			{
				TRACE( "Error - CSoundMananger::PlayWav( %s )\n", strKeySet.c_str() );
				return false;
			}
			//	TRACE( "CSoundMananger::PlayWav( %s )\n", strKeySet.c_str() );

			return true;
		}

		bool CSoundMananger::StopWav( std::string strKeySet )
		{
			MMAPKEYSET::iterator itLow = m_mmapKeySet.lower_bound( strKeySet );
			if( itLow == m_mmapKeySet.end() ) return false;
			int nCount = m_mmapKeySet.count( strKeySet );
			if( nCount == 0 ) return false;

			for( int i=0; i<nCount; i++ )
			{
				m_Sound.Stop( (char*)((itLow->second).c_str()) );
				itLow++;
			}

			return true;
		}

		bool CSoundMananger::PlayWav( std::string strKeyType, std::string strKeyIndex )
		{
			return PlayWav( MakeKeySet( strKeyType, strKeyIndex ) );
		}

		bool CSoundMananger::StopWav( std::string strKeyType, std::string strKeyIndex )
		{
			return StopWav( MakeKeySet( strKeyType, strKeyIndex ) );
		}
		//
		//bool CSoundMananger::AutoPlayMSceneSound( xSprite* pSpr, int nMSIndex )
		//{
		//	// 멀티씬의 사운드를 플레이 리스트에 추가하여 자동으로 연주되도록 함(CSound::RegularCall()을 주기적으로 호출해주어야함)
		//
		//	if( !pSpr )
		//		return false;
		//
		//	
		//	if( m_pParent )
		//	{
		//		if( IsMinimized( m_pParent->GetSafeHwnd() ) )
		//			return false;
		//	}
		//
		//	DWORD curtime = timeGetTime();
		//
		//	QUERYMSCENE_SOUND sndquery;
		//	sndquery.Reset();
		//	while( pSpr->QueryMSceneSound( &sndquery, nMSIndex, 0, 0xFFFFFFFF, FALSE ) )
		//	{
		//		// 등록되지 않은 사운드 파일이면 등록
		//		if( !m_Sound.IsRegistered( sndquery.SndFile ) )			
		//			m_Sound.RegistWav( sndquery.SndFile, SNDPRIORITY_MID, 1, FALSE );
		//			//m_Sound.RegistWav(sndquery.SndFile, SNDPRIORITY_ONCEPLAY, 1, FALSE);
		//
		//		m_Sound.AddToPlayList( curtime + sndquery.PlayTime, sndquery.SndFile, FALSE, sndquery.Volume );
		//	}
		//
		//	return true;
		//}

		void CSoundMananger::RegularCall()
		{
			ProcessPlayData();
			m_Sound.RegularCall();
		}

		bool CSoundMananger::PlayWavForDelay( std::string strKeySet )
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

		bool CSoundMananger::PlayWavForDelay( std::string strKeyType, std::string strKeyIndex )
		{
			return PlayWavForDelay( MakeKeySet( strKeyType, strKeyIndex ) );
		}

		void CSoundMananger::ProcessPlayData()
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

		bool CSoundMananger::OncePlayWav( char* szFilename )
		{
			m_Sound.RegistWav( szFilename, SNDPRIORITY_ONCEPLAY );
			m_Sound.PlayWav( szFilename );
			return true;
		}

		void CSoundMananger::StopAllWav()
		{
			m_Sound.StopAllWav();
		}

		void CSoundMananger::StopAllMusic()
		{
			m_Sound.StopAllMusic();
		}

		void CSoundMananger::Initialize( CWnd *pParent )
		{
			m_pParent = pParent;
			m_Sound.Init( m_pParent->GetSafeHwnd() );
		}

		BOOL CSoundMananger::GetState()
		{
			return m_Sound.IsInited();
		}

		void CSoundMananger::Destroy()
		{
			m_Sound.Destroy();
		}

		void CSoundMananger::SetMusic( BOOL bUse )
		{
			m_Sound.SetMusic( bUse );
		}

		void CSoundMananger::SetSndFx( BOOL bUse )
		{
			m_Sound.SetSndFx( bUse );
		}

		void CSoundMananger::SetVolumn( int nVolumn )
		{
			if( nVolumn < 0 || nVolumn >= 100 )	return;

			m_Sound.SetSndVolume( nVolumn );
		}

		bool CSoundMananger::Clear()
		{
			m_mmapKeySet.clear();

			return true;
		}

		bool CSoundMananger::Attach( char* szKeyType, char* szKeyIndex, char* szFilename )
		{
			std::string	strKeySet = MakeKeySet( szKeyType, szKeyIndex );

			// regist wav/ogg file
			std::string strFilename = szFilename;
			BOOL bRet = FALSE;
			if( IsThisSameExt( strFilename, "wav" ) )
			{
				bRet = m_Sound.RegistWav( szFilename, SNDPRIORITY_MID, 1, false );
			}
			else if( IsThisSameExt( strFilename, "ogg" ) )
			{
				bRet = m_Sound.RegistOggStream( szFilename, SNDPRIORITY_MID, false );
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

			TRACE( "CSoundMananger::Attach( %s, %s )\n", strKeySet.c_str(), strFilename.c_str() );

			/////////////////////////////////////////////////////////////////////
			// 오디오 데이터 한개 세팅 완료 메세지
			if( m_pLoadCallback )
				m_pLoadCallback();
			/////////////////////////////////////////////////////////////////////

			return true;
		}

		bool CSoundMananger::AttachFolder( char* szKeyType, char* szKeyIndex, char* szFoldername )
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


		std::string CSoundMananger::MakeKeySet( std::string strKeyType, std::string strKeyIndex )
		{
			// make KeySet : "szKeyType-szKeyIndex"
			static std::string strKeySet;
			strKeySet = strKeyType + "-" + strKeyIndex;
			return strKeySet;
		}

		bool CSoundMananger::IsThisSameExt( std::string strFilename, std::string strExt, char cDelim )
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

	} // namespace SOUND

}	//namespace NMBASE