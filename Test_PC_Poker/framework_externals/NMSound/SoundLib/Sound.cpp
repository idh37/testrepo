// Sound.cpp: implementation of the CSound class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include <conio.h>
#include "Sound.h"
#include "StreamManager.h"

namespace NMBASE
{
	namespace SOUND
	{

		CSound g_Sound;

		CSndObj::CSndObj()
		{
			ClearMember();
		}

		CSndObj::~CSndObj()
		{
			Destroy();
		}

		void CSndObj::ClearMember()
		{
			m_bCreated = FALSE;
			m_bCreateFailed = FALSE;

			m_SndType = SNDTYPE_NONE;
			m_Priority = SNDPRIORITY_NONE;

			m_PlayCnt = 0;
			m_CreatedTime = 0;
			m_LastPlayTime = 0;

			m_pSndBuf = NULL;
			m_pMciMedia = NULL;
			m_pOggStream = NULL;

			m_Volume = 100;
			m_SndPoly = 1;

			m_bFade = FALSE;
			m_FadeStartVol = 100;
			m_FadeTarVol = 100;
			m_FadeStartTime = 0;
			m_FadeDelay = 0;
		}

		void CSndObj::Destroy() 
		{
			ReleaseRes();

			ClearMember();
		}

		BOOL CSndObj::Set(SNDTYPE_ENUM sndtype, LPCTSTR fname, SNDPRIORITY_ENUM priority, int nPoly)
		{
			Destroy();

			m_FileName = fname;
			m_SndType = sndtype;
			m_Priority = priority;
			m_SndPoly = nPoly;

			return TRUE;
		}

		BOOL CSndObj::CreateRes()
		{
			if(m_bCreated || m_bCreateFailed)
				return FALSE;

			if(m_FileName.length() == 0)
			{
				m_bCreateFailed = TRUE;
				return FALSE;
			}

			switch(m_SndType)
			{
			case SNDTYPE_WAV:
				{
					// ���۰� �����Ǿ� ���� ������ ������
					if(!m_pSndBuf)
					{
						m_pSndBuf = new xSoundBuffer;

						if(DS_OK != m_pSndBuf->Create(const_cast<char*>(m_FileName.c_str()), m_SndPoly))
						{
							delete m_pSndBuf;
							m_pSndBuf = NULL;

							m_bCreateFailed = TRUE;
							return FALSE;
						}
					}
				} 
				break;

			case SNDTYPE_MCIMEDIA:
				{
					// MCI�̵�� ��ü�� �����Ǿ� ���� ������ ������
					if(!m_pMciMedia) 
					{
						m_pMciMedia = new CMciMedia;
						if(!m_pMciMedia->Open(const_cast<char*>(m_FileName.c_str()))) 
						{
							delete m_pMciMedia;
							m_pMciMedia = NULL;
							m_bCreateFailed = TRUE;

							return FALSE;
						}
					}
				} 
				break;

			case SNDTYPE_OGGSTREAM:
				{
					// OGG��Ʈ�� ��ü�� �����Ǿ� ���� ������ ������
					if(!m_pOggStream) 
					{
						m_pOggStream = new xOggStream;
						if(!m_pOggStream->Create(const_cast<char*>(m_FileName.c_str()))) 
						{
							delete m_pOggStream;
							m_pOggStream = NULL;
							m_bCreateFailed = TRUE;

							return FALSE;
						}
					}
				} 
				break;

			default:
				return FALSE;
			}

			m_bCreated = TRUE;
			m_CreatedTime = timeGetTime();
			m_LastPlayTime = 0;
			m_PlayCnt = 0;

			return TRUE;
		}

		BOOL CSndObj::ReleaseRes()
		{
			if(!m_bCreated)
				return FALSE;

			if(m_pSndBuf)
			{
				m_pSndBuf->Destroy();
				delete m_pSndBuf;
				m_pSndBuf = NULL;
			}

			if(m_pMciMedia)
			{
				m_pMciMedia->Close();
				delete m_pMciMedia;
				m_pMciMedia = NULL;
			}

			if(m_pOggStream)
			{
				m_pOggStream->Destroy();
				delete m_pOggStream;
				m_pOggStream = NULL;				
			}

			m_bCreated = FALSE;
			m_CreatedTime = 0;
			m_LastPlayTime = 0;
			m_PlayCnt = 0;

			return TRUE;
		}

		BOOL CSndObj::SetVolume(int volume, int channel/*=-1*/)
		{
			if(!m_bCreated)
				return FALSE;

			if(volume < 0) volume = 0;
			if(volume > 100) volume = 100;

			switch(m_SndType) 
			{
			case SNDTYPE_WAV:
				{
					if(!m_pSndBuf)
						return FALSE;

					if(channel >= 0)
					{
						m_pSndBuf->SetSoundVolume(channel, volume);
					}
					else
					{
						for(int i=0; i<m_pSndBuf->m_BufferCount; i++) 
						{
							m_pSndBuf->SetSoundVolume(i, volume);
						}

						m_Volume = volume;
					}

				} 
				break;

			case SNDTYPE_MCIMEDIA:
				{
					if(!m_pMciMedia)
						return FALSE;

					m_pMciMedia->SetVolume((DWORD)volume);

					m_Volume = volume;
				} 
				break;

			case SNDTYPE_OGGSTREAM:
				{
					if(!m_pOggStream)
						return FALSE;

					m_pOggStream->SetVolume(volume);

					m_Volume = volume;
				} 
				break;

			default:
				return FALSE;
			}

			return TRUE;
		}

		//////////////////////////////////////////////////////////////////////
		// Construction/Destruction
		//////////////////////////////////////////////////////////////////////

		// ���� ���ϸ�

		CSound::CSound()
		{
			m_bInited = FALSE;

			m_MainVolume = 95;

			m_bDontUseSndFx = FALSE;
			m_bDontUseMusic = FALSE;

			m_TotRegistSnd = 0;
			m_TotRegistWav = 0;
			m_TotRegistMci = 0;
			m_TotRegistOgg = 0;

			m_TotCreatedSnd = 0;
			m_TotCreatedWav = 0;
			m_TotCreatedMci = 0;
			m_TotCreatedOgg = 0;

			memset(m_BaseFolder, 0, sizeof(m_BaseFolder));
			m_AutoOptimizeTime = 0;
			m_bEnableConsoleLog = FALSE;

#ifdef _DEBUG
			//m_bEnableConsoleLog = TRUE;
#endif
		}

		CSound::~CSound()
		{
			Destroy();
		}

		BOOL CSound::Init(HWND hWnd)
		{
			if(xSound::IsInitDSound)
				return FALSE;

			if(xSound::Init(hWnd) != DS_OK) 
			{
				return FALSE;
			}

			m_bInited = TRUE;

			return TRUE;
		}

		void CSound::Destroy()
		{
			// ogg��Ʈ�� �޴���
			gStreamMan().Destroy();

			ClearAllSound();
			ClearAllPlayList();
			ClearGameLoadList();
		}

		void CSound::ClearGameLoadList(void)
		{
			m_listGameLoadPath.clear();
		}

		void CSound::ReleaseGameLoadList(void)
		{
			set<string>::iterator start = m_listGameLoadPath.begin();
			set<string>::iterator end = m_listGameLoadPath.end();

			STRING2SNDOBJ::iterator findIt;

			for(;start != end;start++)
			{
				findIt = m_SndObjMap.find(*start);
				if(findIt != m_SndObjMap.end())
				{
					SNDPLAYLIST::iterator start_play = m_SndPlayList.begin();
					SNDPLAYLIST::iterator end_play = m_SndPlayList.end();
					for(;start_play != end_play;start_play++)
					{
						if((*start_play)->SndFileName == *start)
						{
							delete *start_play;
							m_SndPlayList.erase(start_play);
							break;
						}
					}

					CSndObj* pObj = findIt->second;
					if(pObj)
						delete pObj;

					m_SndObjMap.erase(findIt);
				}
			}
		}

		void CSound::ClearAllSound()
		{
			STRING2SNDOBJ::iterator itr = m_SndObjMap.begin();

			for(; itr != m_SndObjMap.end();)
			{
				STRING2SNDOBJ::iterator delitr = itr;
				itr++;

				CSndObj* pObj = (*delitr).second;
				if(pObj)
					delete pObj;
			}

			m_SndObjMap.clear();

			m_TotRegistSnd = 0;
			m_TotRegistWav = 0;
			m_TotRegistMci = 0;
			m_TotRegistOgg = 0;

			m_TotCreatedSnd = 0;
			m_TotCreatedWav = 0;
			m_TotCreatedMci = 0;
			m_TotCreatedOgg = 0;

			ConsoleLog("SOUNDLOG> All Sound Resource Destroyed.");
		}

		void CSound::SetBaseFolder(LPCTSTR strfolder)
		{
			std::string str = strfolder;

			int slen = str.length();
			if(slen == 0)
				return;

			if(str.at(slen-1) != '\\') 
			{
				str += "\\";
			}

			strcpy(m_BaseFolder, const_cast<char*>(str.c_str()));
		}

		void CSound::SetSndVolume( int nVolume )
		{
			if( nVolume < 0 || nVolume > 100 )
			{
				nVolume = 95;
			}

			m_MainVolume = nVolume;
		}

		void CSound::SetSndFx(BOOL bUse)
		{
			if(bUse==TRUE)
				m_bDontUseSndFx = FALSE;
			if(bUse==FALSE)
				m_bDontUseSndFx = TRUE;
		}

		void CSound::SetMusic(BOOL bUse)
		{
			if(bUse==TRUE) m_bDontUseMusic = FALSE;
			if(bUse==FALSE) 
			{
				m_bDontUseMusic = TRUE;
				StopAllMusic();
			}
		}

		BOOL CSound::IsRegistered(LPCTSTR fname)
		{
			STRING2SNDOBJ::iterator itr = m_SndObjMap.find(fname);

			if(itr == m_SndObjMap.end())
				return FALSE;

			return TRUE;
		}

		BOOL CSound::MakeFilePath(LPCTSTR fname, char* pStr)
		{
			int slenfname = strlen(fname);
			if(slenfname == 0 || !pStr)
				return FALSE;

			// ���ϸ��� �˻��Ͽ� Ư�� ������ ��Ȯ�ϰ� ������ ���� �� ���ϸ��� ����: ex) c:\test.wav, ..\parent\test.wav
			BOOL bComplete = FALSE;
			for(int i=0; i<slenfname && i<2; i++)
			{
				if((i==0 && fname[i] == '.') || (fname[i] == ':'))
				{
					bComplete = TRUE;
					break;
				}
			}

			std::string fpath;

			if(!bComplete)
			{
				// ���ϸ�(���� ������� �Բ�) ����� ��� �⺻ ���� ������ ����: ex) test.wav, sound\test.wav
				if(strlen(m_BaseFolder) == 0) 
				{
					// �⺻ ������ �����Ǿ� ���� ������ ���� ������ ��ü
					char buf[MAX_PATH] = {0,};
					GetCurrentDirectory(sizeof(buf)-1, buf);
					fpath = buf;

					int slen = fpath.length();
					if(slen == 0)
						return FALSE;

					if(fpath.at(slen-1) != '\\')
					{
						fpath += "\\";
					}
				}
				else
				{
					fpath = m_BaseFolder;
				}
			}

			fpath += fname;

			strcpy(pStr, const_cast<char*>(fpath.c_str()));

			return TRUE;
		}

		void CSound::AutoOptimizeRes()
		{
			DWORD nowtime = timeGetTime();
			if(nowtime - m_AutoOptimizeTime < 10)
				return;

			OptimizeRes();

			m_AutoOptimizeTime = nowtime;
		}

		void CSound::EnableConsoleLog(BOOL bEnable)
		{
			m_bEnableConsoleLog = bEnable;
		}

		BOOL CSound::RegistWav(LPCTSTR fname, SNDPRIORITY_ENUM priority, int nPoly, BOOL bNowCreate)
		{
			AutoOptimizeRes();

			m_listGameLoadPath.insert(fname);

			STRING2SNDOBJ::iterator itr = m_SndObjMap.find(fname);

			// �̹� ��ϵ� �������� �˻�
			if(itr != m_SndObjMap.end())
			{
				return FALSE;
			}

			CSndObj* pObj = new CSndObj;

			char fullfname[MAX_PATH] = {0, };
			MakeFilePath(fname, fullfname);

			pObj->Set(SNDTYPE_WAV, fullfname, priority, nPoly);

			// map�� ����
			m_SndObjMap[fname] = pObj;

			m_TotRegistSnd++;
			m_TotRegistWav++;

			ConsoleLog("SOUNDLOG> %d WAV Registered(TotRes:%d)", m_TotRegistWav, m_TotRegistSnd);

			// ��� ��� �ε�
			if(bNowCreate) 
			{
				if(!pObj->CreateRes())
					return FALSE;

				OnSoundCreate(pObj->m_SndType, fname);
			}

			return TRUE;
		}

		BOOL CSound::RegistMciMedia(LPCTSTR fname, SNDPRIORITY_ENUM priority, BOOL bNowCreate)
		{
			AutoOptimizeRes();

			m_listGameLoadPath.insert(fname);

			STRING2SNDOBJ::iterator itr = m_SndObjMap.find(fname);

			// �̹� ��ϵ� �������� �˻�
			if(itr != m_SndObjMap.end()) 
				return FALSE;

			CSndObj* pObj = new CSndObj;

			char fullfname[MAX_PATH] = {0, };
			MakeFilePath(fname, fullfname);

			pObj->Set(SNDTYPE_MCIMEDIA, fullfname, priority, 1);

			// map�� ����
			m_SndObjMap[fname] = pObj;

			m_TotRegistSnd++;
			m_TotRegistMci++;

			ConsoleLog("SOUNDLOG> %d MCI Registered(TotRes:%d)", m_TotRegistMci, m_TotRegistSnd);

			// ��� ��� �ε�
			if(bNowCreate) 
			{
				if(!pObj->CreateRes())
					return FALSE;

				OnSoundCreate(pObj->m_SndType, fname);
			}

			return TRUE;
		}

		BOOL CSound::RegistOggStream(LPCTSTR fname, SNDPRIORITY_ENUM priority, BOOL bNowCreate)
		{
			AutoOptimizeRes();

			m_listGameLoadPath.insert(fname);

			STRING2SNDOBJ::iterator itr = m_SndObjMap.find(fname);

			// �̹� ��ϵ� �������� �˻�
			if(itr != m_SndObjMap.end())
				return FALSE;

			CSndObj* pObj = new CSndObj;

			char fullfname[MAX_PATH] = {0, };
			MakeFilePath(fname, fullfname);

			pObj->Set(SNDTYPE_OGGSTREAM, fullfname, priority, 1);

			// map�� ����
			m_SndObjMap[fname] = pObj;

			m_TotRegistSnd++;
			m_TotRegistOgg++;

			ConsoleLog("SOUNDLOG> %d OGG Registered(TotRes:%d)", m_TotRegistOgg, m_TotRegistSnd);

			// ��� ��� �ε�
			if(bNowCreate)
			{
				if(!pObj->CreateRes())
					return FALSE;

				OnSoundCreate(pObj->m_SndType, fname);
			}

			return TRUE;
		}


		BOOL CSound::PlayWav(LPCTSTR fname, BOOL bIsLoop, int volume, BOOL bIgnoreBufferFull)
		{
			AutoOptimizeRes();

			if(m_bDontUseSndFx == TRUE) 
				return TRUE;

			volume = m_MainVolume;
			//volume = g_pMainDlg->CFG.FxVolume; // ���� UI ����

			STRING2SNDOBJ::iterator itr = m_SndObjMap.find(fname);

			if(itr == m_SndObjMap.end()) 
			{
				ConsoleLog("SOUNDLOG> Playing failed - Unregistered WAV(%s)", fname);
				return FALSE;
			}

			//
			// (*itr).first  -> Key
			// (*itr).second -> Key�� �ش��ϴ� ���� ������
			//

			CSndObj* pObj = (*itr).second;

			if(pObj->m_bCreateFailed) 
				return FALSE;

			if(pObj->m_SndType != SNDTYPE_WAV)
				return FALSE;

			if(xSound::IsInitDSound) 
			{
				// ��ü�� �����Ǿ� ���� ������ ������
				if(!pObj->m_bCreated) 
				{
					if(!pObj->CreateRes())
						return FALSE;

					OnSoundCreate(pObj->m_SndType, fname);
				}

				pObj->ReleaseRes();
				pObj->CreateRes();

				if(!pObj->m_pSndBuf)
					return FALSE;

				if(bIsLoop) 
					pObj->m_pSndBuf->Play(PLAY_LOOP, volume, 0, bIgnoreBufferFull);
				else 
					pObj->m_pSndBuf->Play(PLAY_ONCE, volume, 0, bIgnoreBufferFull);
			}
			else 
			{
				// DSound�� �ʱ�ȭ �Ǿ� ���� ���� ��� API�� �̿��� Wav ���� �õ�
				char fullfname[MAX_PATH] = {0, };
				MakeFilePath(fname, fullfname);

				PlaySound(fullfname, NULL, SND_ASYNC | SND_FILENAME | SND_NODEFAULT);
			}

			pObj->m_LastPlayTime = timeGetTime();
			pObj->m_PlayCnt++;

			pObj->m_Volume = volume;

			pObj->m_bFade = FALSE;
			pObj->m_FadeStartVol = volume;
			pObj->m_FadeTarVol = volume;

			return TRUE;
		}

		BOOL CSound::PlayMciMedia(LPCTSTR fname, BOOL bIsLoop, int volume)
		{
			AutoOptimizeRes();

			if(m_bDontUseMusic == TRUE) 
				return TRUE;

			STRING2SNDOBJ::iterator itr = m_SndObjMap.find(fname);

			if(itr == m_SndObjMap.end()) 
			{
				ConsoleLog("SOUNDLOG> Playing failed - Unregistered MCI(%s)", fname);
				return FALSE;
			}

			//
			// (*itr).first  -> Key
			// (*itr).second -> Key�� �ش��ϴ� ���� ������
			//

			CSndObj* pObj = (*itr).second;

			if(pObj->m_bCreateFailed) 
				return FALSE;

			if(pObj->m_SndType != SNDTYPE_MCIMEDIA)
				return FALSE;

			// ��ü�� �����Ǿ� ���� ������ ������
			if(!pObj->m_bCreated) 
			{
				if(!pObj->CreateRes())
					return FALSE;

				OnSoundCreate(pObj->m_SndType, fname);
			}

			if(!pObj->m_pMciMedia)
				return FALSE;

			pObj->m_pMciMedia->Stop();
			pObj->m_pMciMedia->SetVolume(volume);
			pObj->m_pMciMedia->Play(0, bIsLoop);
			pObj->m_pMciMedia->SetVolume(volume);

			pObj->m_LastPlayTime = timeGetTime();
			pObj->m_PlayCnt++;

			pObj->m_Volume = volume;

			pObj->m_bFade = FALSE;
			pObj->m_FadeStartVol = volume;
			pObj->m_FadeTarVol = volume;

			return TRUE;
		}

		BOOL CSound::PlayOggStream(LPCTSTR fname, BOOL bIsLoop, int volume, int panning, bool use_thread)
		{
			AutoOptimizeRes();

			if(!xSound::IsInitDSound) 
				return FALSE;

			if(m_bDontUseSndFx == TRUE) 
				return TRUE;

			volume = m_MainVolume;
			//volume = g_pMainDlg->CFG.FxVolume; // ���� UI ���� -> Ogg ȿ������

			STRING2SNDOBJ::iterator itr = m_SndObjMap.find(fname);

			if(itr == m_SndObjMap.end()) 
			{
				ConsoleLog("SOUNDLOG> Playing failed - Unregistered OGG(%s)", fname);
				return FALSE;
			}

			//
			// (*itr).first  -> Key
			// (*itr).second -> Key�� �ش��ϴ� ���� ������
			//

			CSndObj* pObj = (*itr).second;

			if(pObj->m_bCreateFailed) 
				return FALSE;

			if(pObj->m_SndType != SNDTYPE_OGGSTREAM)
				return FALSE;

			// ��ü�� �����Ǿ� ���� ������ ������
			if(!pObj->m_bCreated) 
			{
				if(!pObj->CreateRes())
					return FALSE;

				OnSoundCreate(pObj->m_SndType, fname);
			}

			if(!pObj->m_pOggStream)
				return FALSE;

			pObj->m_pOggStream->Stop();

			if(bIsLoop) 
				pObj->m_pOggStream->Play(PLAY_LOOP, volume, panning, FALSE, use_thread);
			else 
				pObj->m_pOggStream->Play(PLAY_ONCE, volume, panning, FALSE, use_thread);

			pObj->m_LastPlayTime = timeGetTime();
			pObj->m_PlayCnt++;

			pObj->m_Volume = volume;

			pObj->m_bFade = FALSE;
			pObj->m_FadeStartVol = volume;
			pObj->m_FadeTarVol = volume;

			return TRUE;
		}

		BOOL CSound::PlayOggStream_BGM(LPCTSTR fname, BOOL bIsLoop, int volume, int panning, bool use_thread)
		{
			AutoOptimizeRes();

			if(!xSound::IsInitDSound) 
				return FALSE;

			if(m_bDontUseMusic == TRUE) 
				return TRUE;

			STRING2SNDOBJ::iterator itr = m_SndObjMap.find(fname);

			if(itr == m_SndObjMap.end())
			{
				ConsoleLog("SOUNDLOG> Playing failed - Unregistered OGG(%s)", fname);
				return FALSE;
			}

			//
			// (*itr).first  -> Key
			// (*itr).second -> Key�� �ش��ϴ� ���� ������
			//

			CSndObj* pObj = (*itr).second;

			if(pObj->m_bCreateFailed) 
				return FALSE;

			if(pObj->m_SndType != SNDTYPE_OGGSTREAM)
				return FALSE;

			// ��ü�� �����Ǿ� ���� ������ ������
			if(!pObj->m_bCreated) 
			{
				if(!pObj->CreateRes())
					return FALSE;

				OnSoundCreate(pObj->m_SndType, fname);
			}

			if(!pObj->m_pOggStream)
				return FALSE;

			pObj->m_pOggStream->Stop();

			if(bIsLoop) 
				pObj->m_pOggStream->Play(PLAY_LOOP, volume, panning, TRUE, use_thread);
			else 
				pObj->m_pOggStream->Play(PLAY_ONCE, volume, panning, TRUE, use_thread);

			pObj->m_LastPlayTime = timeGetTime();
			pObj->m_PlayCnt++;

			pObj->m_Volume = volume;

			pObj->m_bFade = FALSE;
			pObj->m_FadeStartVol = volume;
			pObj->m_FadeTarVol = volume;

			return TRUE;
		}

		BOOL CSound::StopSndObj(CSndObj* pObj, int channel)
		{
			if(!pObj->m_bCreated || pObj->m_bCreateFailed) 
				return FALSE;

			pObj->m_bFade = FALSE;

			switch(pObj->m_SndType)
			{
			case SNDTYPE_WAV:
				{
					if(!pObj->m_pSndBuf || !pObj->m_pSndBuf->m_Buffer)
						return FALSE;

					if(DS_OK == pObj->m_pSndBuf->Stop(channel))
						return TRUE;
				} 
				break;

			case SNDTYPE_MCIMEDIA:
				{
					if(!pObj->m_pMciMedia)
						return FALSE;

					return pObj->m_pMciMedia->Stop();
				} 
				break;

			case SNDTYPE_OGGSTREAM:
				{
					if(!pObj->m_pOggStream)
						return FALSE;

					pObj->m_pOggStream->Stop();
					return TRUE;
				} 
				break;

			default :
				return FALSE;
			}

			return FALSE;
		}


		BOOL CSound::Stop(LPCTSTR fname, int channel)
		{
			STRING2SNDOBJ::iterator itr = m_SndObjMap.find(fname);

			if(itr == m_SndObjMap.end())
				return FALSE;

			//
			// (*itr).first  -> Key
			// (*itr).second -> Key�� �ش��ϴ� ���� ������
			//

			CSndObj* pObj = (*itr).second;

			return StopSndObj(pObj, channel);
		}

		void CSound::StopAllWav()
		{
			STRING2SNDOBJ::iterator itr = m_SndObjMap.begin();

			for(; itr != m_SndObjMap.end(); itr++)
			{
				CSndObj* pObj = (*itr).second;
				if(pObj) 
				{
					if(pObj->m_SndType != SNDTYPE_WAV)
						continue;

					StopSndObj(pObj, -1);
				}
			}
		}

		void CSound::StopAllMusic()
		{
			STRING2SNDOBJ::iterator itr = m_SndObjMap.begin();

			for(; itr != m_SndObjMap.end(); itr++)
			{
				CSndObj* pObj = (*itr).second;
				if(pObj)
				{
					if(pObj->m_SndType != SNDTYPE_MCIMEDIA && pObj->m_SndType != SNDTYPE_OGGSTREAM)
						continue;

					StopSndObj(pObj, -1);
				}
			}
		}

		void CSound::StopAllBGM()
		{
			STRING2SNDOBJ::iterator itr = m_SndObjMap.begin();

			for(; itr != m_SndObjMap.end(); itr++)
			{
				CSndObj* pObj = (*itr).second;
				if(pObj)
				{					
					if(pObj->m_SndType != SNDTYPE_MCIMEDIA && pObj->m_SndType != SNDTYPE_OGGSTREAM)
						continue;

					if ( pObj->m_pOggStream &&  !pObj->m_pOggStream->IsBGM())
						continue;

					StopSndObj(pObj, -1);
				}
			}

		}

		void CSound::SetVolumeAllMusic(int volume, int channel)
		{
			STRING2SNDOBJ::iterator itr = m_SndObjMap.begin();

			for(; itr != m_SndObjMap.end(); itr++)
			{
				CSndObj* pObj = (*itr).second;
				if(pObj)
				{
					if(pObj->m_SndType != SNDTYPE_MCIMEDIA && pObj->m_SndType != SNDTYPE_OGGSTREAM)
						continue;

					pObj->SetVolume(volume, channel);
				}
			}
		}		

		BOOL CSound::SetVolume(LPCTSTR fname, int volume, int channel/*=-1*/)
		{
			CSndObj* pObj = GetSndObj(fname);
			if(!pObj)
				return FALSE;

			return pObj->SetVolume(volume, channel);
		}

		BOOL CSound::SetFade(LPCTSTR fname, int volume, DWORD delay)
		{
			CSndObj* pObj = GetSndObj(fname);
			if(!pObj)
				return FALSE;

			if(volume < 0)
				volume = 0;
			if(volume > 100)
				volume = 100;

			pObj->m_bFade = TRUE;
			pObj->m_FadeStartVol = pObj->m_Volume;
			pObj->m_FadeTarVol = volume;
			pObj->m_FadeDelay = max(1, delay);
			pObj->m_FadeStartTime = timeGetTime();

			return TRUE;
		}

		// ���� ������Ʈ�� �����͸� ��´�
		CSndObj* CSound::GetSndObj(LPCTSTR fname)
		{
			STRING2SNDOBJ::iterator itr = m_SndObjMap.find(fname);

			if(itr == m_SndObjMap.end())
				return NULL;

			CSndObj* pObj = (*itr).second;

			return pObj;
		}

		void CSound::OptimizeRes()
		{
			STRING2SNDOBJ::iterator itr = m_SndObjMap.begin();

			for(; itr != m_SndObjMap.end(); )
			{
				STRING2SNDOBJ::iterator delitr = itr;
				itr++;

				CSndObj* pObj = (*delitr).second;

				if(!pObj) 
					continue;

				// ���� �������� ���� ���ҽ��� ��ŵ
				if(!pObj->m_bCreated)
					continue;

				// ������ ������ ���ҽ��� �׳� ���ܵ�(���� ���� �õ��� ���̱� ����)
				if(pObj->m_bCreateFailed)
					continue;

				// �������� ���� ������Ʈ�� ���� ��󿡼� ����
				switch(pObj->m_SndType) 
				{
				case SNDTYPE_WAV:
					{
						if(!pObj->m_pSndBuf)
							continue;
						if(pObj->m_pSndBuf->IsPlaying())
							continue;
					} 
					break;

				case SNDTYPE_MCIMEDIA:
					{
						if(!pObj->m_pMciMedia)
							continue;
						if(pObj->m_pMciMedia->IsPlaying())
							continue;
					}
					break;

				case SNDTYPE_OGGSTREAM:
					{
						if(!pObj->m_pOggStream)
							continue;
						if(pObj->m_pOggStream->IsPlaying())
							continue;
					} 
					break;

				default :
					{
						// �� �� ���� ���� ������Ʈ�� ������ ����
						delete pObj;
						m_SndObjMap.erase(delitr);

						continue;
					}
				}

				// ��� �󵵿� ���� ���ҽ��� ������ �������Ѵ�
				BOOL bReleaseRes = FALSE;
				DWORD nowtime = timeGetTime();

				switch(pObj->m_Priority)
				{
				case SNDPRIORITY_ONCEPLAY:
					{
						// �ѹ��� �����ϰ� �ı�(�켱 ������ ���� ����)
						if(pObj->m_PlayCnt > 0)
						{
							// �̹� �ѹ� ���� �Ǿ��ٸ� �ı�
							bReleaseRes = TRUE;
						}
						else 
						{
							if(nowtime - pObj->m_CreatedTime > 1000 * 10)
							{
								// ���ֵ� �� ������ �������� 10�� �ڿ� ������ �ı�
								bReleaseRes = TRUE;
							}
						}
					}
					break;
				case SNDPRIORITY_LOW:
					{
						if(pObj->m_PlayCnt > 0)
						{
							if(nowtime - pObj->m_LastPlayTime > 1000 * 30)
							{
								// ���������� �������� 30�� ������ �ı�
								bReleaseRes = TRUE;
							}
						}
						else 
						{
							if(nowtime - pObj->m_CreatedTime > 1000 * 60 * 5)
							{
								// ���ֵ� �� ������ �������� 5�� �ڿ� ������ �ı�
								bReleaseRes = TRUE;
							}
						}
					} 
					break;

				case SNDPRIORITY_MID:
					{
						if(pObj->m_PlayCnt > 0)
						{
							if(nowtime - pObj->m_LastPlayTime > 1000 * 60 * 3)
							{
								// ���������� �������� 3���� ������ �ı�
								bReleaseRes = TRUE;
							}
						}
						else 
						{
							if(nowtime - pObj->m_CreatedTime > 1000 * 60 * 20)
							{
								// ���ֵ� �� ������ �������� 10�� �ڿ� ������ �ı�
								bReleaseRes = TRUE;
							}
						}
					}
					break;

				case SNDPRIORITY_HIGH:
					{
						if(pObj->m_PlayCnt > 0) 
						{
							if(nowtime - pObj->m_LastPlayTime > 1000 * 60 * 10)
							{
								// ���������� �������� 10�� ������ �ı�
								bReleaseRes = TRUE;
							}
						}
						else 
						{
							if(nowtime - pObj->m_CreatedTime > 1000 * 60 * 30)
							{
								// ���ֵ� �� ������ �������� 30�� �ڿ� ������ �ı�
								bReleaseRes = TRUE;
							}
						}
					} 
					break;

				case SNDPRIORITY_KEEPALIVE:
					{
						// ���ҽ��� �׻� ����д�(�켱 ������ ���� ����)
					} 
					break;

				default:
					bReleaseRes = TRUE;
					break;
				}

				if(bReleaseRes)
				{
					if(pObj->m_bCreated) 
					{
						OnSoundRelease(pObj->m_SndType, ((*delitr).first).c_str());

						// ���� ���ҽ� ������
						pObj->ReleaseRes();
					}
				}
			}
		}

		void CSound::ConsoleLog(const char *fmt, ...)
		{
			if(!m_bEnableConsoleLog)
				return;

			char buf[1024*10];
			va_list va;

			AllocConsole();

			va_start(va, fmt);
			vsprintf(buf, fmt, va);
			va_end(va);

			cputs(buf);
			cputs("\r\n");

			/*
			FILE* fp;
			fp = fopen("c:\\out.txt", "at+");
			if(fp) {
			fputs(buf, fp);
			fclose(fp);
			}
			*/
		} 

		void CSound::OnSoundCreate(SNDTYPE_ENUM sndtype, LPCTSTR fname)
		{
			m_TotCreatedSnd++;
			if(sndtype == SNDTYPE_WAV)
			{
				m_TotCreatedWav++;
				ConsoleLog("SOUNDLOG> %d/%d WAV Created(%s)", m_TotCreatedWav, m_TotRegistWav, fname);
			}
			if(sndtype == SNDTYPE_MCIMEDIA) 
			{
				m_TotCreatedMci++;
				ConsoleLog("SOUNDLOG> %d/%d MCI Created(%s)", m_TotCreatedMci, m_TotRegistMci, fname);
			}
			if(sndtype == SNDTYPE_OGGSTREAM)
			{
				m_TotCreatedOgg++;
				ConsoleLog("SOUNDLOG> %d/%d OGG Created(%s)", m_TotCreatedOgg, m_TotRegistOgg, fname);
			}
		}

		void CSound::OnSoundRelease(SNDTYPE_ENUM sndtype, LPCTSTR fname)
		{
			m_TotCreatedSnd--;
			if(sndtype == SNDTYPE_WAV) m_TotCreatedWav--;
			if(sndtype == SNDTYPE_MCIMEDIA) m_TotCreatedMci--;
			if(sndtype == SNDTYPE_OGGSTREAM) m_TotCreatedOgg--;

			ConsoleLog("SOUNDLOG> Release Sound Object(%s)", fname);
			ConsoleLog("SOUNDLOG> Resource Status : (WAV=%d/%d, MCI=%d/%d, OGG=%d/%d)", 
				m_TotCreatedWav, m_TotRegistWav,
				m_TotCreatedMci, m_TotRegistMci,
				m_TotCreatedOgg, m_TotRegistOgg);
		}

		void CSound::RegularCall()
		{
			// ���� �÷��� ����Ʈ ó��
			ProcessPlayList();

			// ���̵� ��/�ƿ� ó��
			ProcessFade();

			// ���� ���ҽ� ����ȭ
			AutoOptimizeRes();
		}

		BOOL CSound::AddToPlayList(DWORD playtime, LPCTSTR fname, BOOL bIsLoop, int volume , int skind)
		{
			SNDPLAYDATA* pData = new SNDPLAYDATA;
			pData->PlayTime = playtime;
			pData->SndFileName = fname;
			pData->bLoop = bIsLoop;
			pData->Volume = volume;
			pData->nSoundKind = skind;

			m_SndPlayList.push_back(pData);

			return TRUE;
		}

		void CSound::ClearAllPlayList()
		{
			SNDPLAYLIST::iterator itr = m_SndPlayList.begin();

			for(; itr != m_SndPlayList.end();)
			{
				SNDPLAYLIST::iterator delitr = itr;
				itr++;

				SNDPLAYDATA* pData = *delitr;
				if(pData)
					delete pData;
			}

			m_SndPlayList.clear();
		}

		void CSound::ProcessPlayList()
		{
			DWORD curtime = timeGetTime();

			SNDPLAYLIST::iterator itr = m_SndPlayList.begin();

			for(; itr != m_SndPlayList.end();)
			{
				SNDPLAYLIST::iterator delitr = itr;
				itr++;

				SNDPLAYDATA* pData = *delitr;
				if(!pData) 
				{
					m_SndPlayList.erase(delitr);
					continue;
				}

				// �÷����� �ð��� �������� �˻�
				if(pData->PlayTime <= curtime) 
				{
					// �÷����� �ð��� �ʹ� ���� ���� �������� ����
					if(curtime - pData->PlayTime < 500) 
					{
						// ��ϵ��� ���� ���� �����̸� ���
						if(!IsRegistered(pData->SndFileName.c_str())) 
						{

							if( pData->nSoundKind == 0 )
							{
								RegistWav(pData->SndFileName.c_str(), SNDPRIORITY_MID, 1, TRUE);
							}
							else if( pData->nSoundKind == 1 )
							{
								RegistOggStream(pData->SndFileName.c_str(), SNDPRIORITY_MID, 1);
							}
							else
							{
								RegistMciMedia(pData->SndFileName.c_str(), SNDPRIORITY_MID, 1);
							}

						}

						// ���� ����

						if( pData->nSoundKind == 0 )
						{
							PlayWav(pData->SndFileName.c_str(), pData->bLoop, pData->Volume);
						}
						else
						{
							//if( g_Where != WH_GAME  ){
							PlayMciMedia(pData->SndFileName.c_str(), pData->bLoop, pData->Volume);
							//}					
						}				
					}

					// ���� �� ����Ʈ���� ����
					delete pData;
					m_SndPlayList.erase(delitr);
				}
			}
		}

		void CSound::ProcessFade()
		{

			DWORD curtime = timeGetTime();

			STRING2SNDOBJ::iterator itr = m_SndObjMap.begin();

			for(; itr != m_SndObjMap.end(); )
			{
				STRING2SNDOBJ::iterator delitr = itr;
				itr++;

				CSndObj* pObj = (*delitr).second;

				if(!pObj) 
					continue;

				if(!pObj->m_bFade) 
					continue;

				int gap = (curtime - pObj->m_FadeStartTime) / pObj->m_FadeDelay;

				int curvol = pObj->m_FadeStartVol;

				if(pObj->m_FadeStartVol < pObj->m_FadeTarVol)
				{
					curvol += gap;
					if(curvol > pObj->m_FadeTarVol)
						curvol = pObj->m_FadeTarVol;
				}

				if(pObj->m_FadeStartVol > pObj->m_FadeTarVol)
				{
					curvol -= gap;
					if(curvol < pObj->m_FadeTarVol)
						curvol = pObj->m_FadeTarVol;
				}

				if(curvol == pObj->m_FadeTarVol)
				{
					pObj->m_bFade = FALSE;
				}

				if(curvol == pObj->m_Volume)
					continue;

				pObj->m_Volume = curvol;

				// ���� �������� ���� ���ҽ��� ��ŵ
				if(!pObj->m_bCreated)
					continue;

				pObj->SetVolume(pObj->m_Volume);
			}
		}
	} //namespace SOUND

}	//namespace NMBASE
