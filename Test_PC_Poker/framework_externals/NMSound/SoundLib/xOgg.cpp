// xOgg.cpp
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include <assert.h>
#include <math.h>
#include "xOgg.h"
#include "StreamManager.h"

namespace NMBASE
{
	namespace SOUND
	{
		//
		//#ifdef OGGLIB_USEDLL
		//	// DLL ���
		//	#ifdef _DEBUG
		//		#pragma comment ( lib, "ogg_d.lib" )
		//		#pragma comment ( lib, "vorbis_d.lib" )
		//		#pragma comment ( lib, "vorbisfile_d.lib" )
		//	#else
		//		#pragma comment ( lib, "ogg.lib" )
		//		#pragma comment ( lib, "vorbis.lib" )
		//		#pragma comment ( lib, "vorbisfile.lib" )
		//	#endif
		//#else
		//	// Static Library ���
		//	#ifdef _DEBUG
		//		// ���� ����� ���̺귯���� ��ũ�ؾ��ϳ� ������ �־ ������ ���̺귯���� ��ü��
		//		//#pragma comment ( lib, "ogg_static_d.lib" )
		//		//#pragma comment ( lib, "vorbis_static_d.lib" )
		//		//#pragma comment ( lib, "vorbisfile_static_d.lib" )
		//
		//		#ifdef _MT
		//			// ��Ƽ ������� ����� ���̺귯�� �浹�� ���ϱ� ���Ͽ� ��Ŀ �ɼ� ����
		//			#pragma message( "OGG Library : turnning on linker option '/nodefaultlib:libc.lib'" )
		//			#pragma comment ( linker, "/nodefaultlib:libc.lib")	// LIBC.LIB �� LIBCMT.LIB�� �浹�� ���ϱ� ����
		//		#endif
		//
		//		#pragma comment ( lib, "ogg_static.lib" )
		//		#pragma comment ( lib, "vorbis_static.lib" )
		//		#pragma comment ( lib, "vorbisfile_static.lib" )
		//	#else
		//		#pragma comment ( lib, "ogg_static.lib" )
		//		#pragma comment ( lib, "vorbis_static.lib" )
		//		#pragma comment ( lib, "vorbisfile_static.lib" )
		//	#endif
		//#endif


#define SOUND_8BIT				8
#define SOUND_16BIT				16

#define SOUND_11K				11025
#define SOUND_22K				22050
#define SOUND_44K				44100

#define SOUND_MONO				1
#define SOUND_STEREO			2

#define SOUND_WAVE_FMT_PCM			0x01
#define SOUND_WAVE_FMT_MS_ADPCM		0x02
#define SOUND_WAVE_FMT_IMA_ADPCM	0x11

#define SOUND_READ_FROM_CURRENT		DSBLOCK_FROMWRITECURSOR
#define SOUND_READ_ENTIRE			DSBLOCK_ENTIREBUFFER

#define	SOUND_LITTLE_ENDIAN			0
#define	SOUND_BIG_ENDIAN			1
#define	SOUND_ENDIAN				SOUND_LITTLE_ENDIAN

#define	SOUND_SIGN_TYPE_SIGNED		1
#define	SOUND_SIGN_TYPE_UNSIGNED	0
#define	SOUND_SIGN_TYPE				SOUND_SIGN_TYPE_SIGNED

		//////////////////////////////////////////////////////////////////////////////////

		class COggAutoLock
		{
		public:

			xOggStream* m_pOggStream;

			COggAutoLock(xOggStream* pOggStream)
			{
				m_pOggStream = pOggStream;
				m_pOggStream->Lock();
			}

			~COggAutoLock()
			{
				m_pOggStream->Unlock();
			}
		};

		//////////////////////////////////////////////////////////////////////////////////


		// Constructor
		xOggFile::xOggFile()
		{
			// Initialise data members
			m_pFormat = NULL;
			m_nBlockAlign= 0;
			m_nAvgDataRate = 0;
			m_nDataSize = 0;
			m_nBytesPlayed = 0;
			m_nDuration = 0;

			m_desc.Reset();

			m_bEnteredLoop = FALSE;
			m_pExitLoopFlag = new UINT;
			*m_pExitLoopFlag = 0;
		}

		// Destructor
		xOggFile::~xOggFile()
		{
			// Free memory
			if(m_pFormat) 
				GlobalFree(m_pFormat);

			if(m_desc.pt) 
			{
				fclose(m_desc.pt);
				ov_clear(&m_desc.vf);
				m_desc.Reset();
			}

			if(m_bEnteredLoop)
			{
				if(m_pExitLoopFlag)
					*m_pExitLoopFlag = 1;
			}
			else
			{
				if(m_pExitLoopFlag) 
				{
					*m_pExitLoopFlag = 0;
					delete m_pExitLoopFlag;
					m_pExitLoopFlag = NULL;
				}
			}
		}

		// Open
		BOOL xOggFile::Open(LPCTSTR pszFilename)
		{
			assert( pszFilename );

			if(NULL == pszFilename)
				return FALSE;

			wave_fourcc		fourcc; 
			memset( &fourcc, 0, sizeof(fourcc) );

			UINT fmt_size = 0;

			FILE* fp = fopen( pszFilename, "rb" );

			// �׷� ������ ������ ����
			if( fp == NULL ) 
				return FALSE;

			// ��� Fourcc�� �д´�. ����� ���� �������� ����.
			if( fread( &fourcc, 4, 1, fp ) == EOF )
			{
				fclose(fp);
				return FALSE;
			}

			// Ogg������ �ƴϸ� ����.
			if(memcmp (fourcc.fourcc, "OggS", 4) != 0)
			{
				fclose(fp);
				return FALSE;
			}

			m_desc.Reset();

			m_desc.pt = fp;
			fseek( fp, -4, SEEK_CUR );	// rewind for read ogg

			// ogg open�ϰ�...
			if( ov_open( fp, &m_desc.vf, NULL, 0 ) < 0) 
			{ 
				fclose(fp);
				return FALSE;
			}

			// ������ ���� �Ŀ�...
			if( (m_desc.vi = ov_info(&m_desc.vf, -1)) == NULL)
			{
				fclose(fp);
				return FALSE;
			}

			m_pFormat = (WAVEFORMATEX *) GlobalAlloc(GMEM_FIXED, sizeof(WAVEFORMATEX));
			memset(m_pFormat, 0, sizeof(WAVEFORMATEX));

			if(NULL == m_pFormat)
				return FALSE;

			m_pFormat->cbSize = 0;
			m_pFormat->wFormatTag = WAVE_FORMAT_PCM;
			m_pFormat->nChannels = (unsigned short)m_desc.vi->channels;
			m_pFormat->nSamplesPerSec = m_desc.vi->rate;
			m_pFormat->wBitsPerSample = 16;	//m_desc.vi->;
			m_pFormat->nBlockAlign = (unsigned short)(m_pFormat->wBitsPerSample / 8 * m_pFormat->nChannels);
			m_pFormat->nAvgBytesPerSec = m_pFormat->nSamplesPerSec * m_pFormat->nBlockAlign;

			m_desc.size = (UINT)(ov_pcm_total(&m_desc.vf, -1) * m_pFormat->nBlockAlign);
			m_desc.start_position = 0;

			// Init some member data from format chunk
			m_nBlockAlign = m_pFormat->nBlockAlign;
			m_nAvgDataRate = m_pFormat->nAvgBytesPerSec;

			// Cue for streaming

#pragma region Legacy Code
			// CueFile();
#pragma endregion Legacy Code

			if(CueFile() == FALSE)
				return FALSE;

			// Init some member data from data chunk
			m_nDataSize = m_desc.size;
			m_nDuration = (UINT)(((__int64) m_nDataSize * 1000) / m_pFormat->nAvgBytesPerSec);

			return TRUE;
		}

		// Cue
		BOOL xOggFile::CueFile(void)
		{
			if(ov_seekable(&m_desc.vf) != 0)
			{
				// ó������ �̵�
				if(ov_pcm_seek(&m_desc.vf, 0) == 0)
				{
					// Reset byte counter
					m_nBytesPlayed = 0;
					return TRUE;
				}
			}

			return FALSE;

#pragma region Legacy Code
			//if(ov_seekable(&m_desc.vf) != 0)
			//{
			//	ov_pcm_seek(&m_desc.vf, 0);	// ó������ �̵�

			//	// Reset byte counter
			//	m_nBytesPlayed = 0;

			//	return TRUE;
			//}
			//else
			//{
			//	return FALSE;
			//}
#pragma endregion Legacy Code
		}

		// Read
		UINT xOggFile::Read(BYTE * pbDest, UINT cbSize)
		{
			assert(m_pFormat);

			if(NULL == m_pFormat)
				return 0;

			UINT readbyte = 0;

			if(m_bEnteredLoop)
				return 0;

			m_bEnteredLoop = TRUE;
			UINT* pFlag = m_pExitLoopFlag;

			while(readbyte < cbSize) 
			{
				if(pFlag)
				{
					if(*pFlag == 1)
					{
						delete pFlag;
						return 0;
					}
				}
				else 
				{
					return 0;
				}

				if(IsBadWritePtr(pbDest, min(4, cbSize)))
					return 0;

				if(IsBadReadPtr(m_pFormat, 4))
					return 0;

				UINT rsize = cbSize - readbyte;
				UINT read = ov_read(&m_desc.vf, (char*)(pbDest + readbyte), rsize, SOUND_ENDIAN, m_pFormat->wBitsPerSample / 8, SOUND_SIGN_TYPE, &m_desc.ogg_current_section );

				readbyte += read;

				if(read == 0)
					break;
			}

			// Successful read keep running total of number of data bytes read
			m_nBytesPlayed += readbyte;

			m_bEnteredLoop = FALSE;

			return readbyte;
		}

		// GetSilenceData
		BYTE xOggFile::GetSilenceData(void)
		{
			BYTE bSilenceData = 0;

			// Silence data depends on format of Wave file
			if(m_pFormat)
			{
				if(m_pFormat->wBitsPerSample == 8)
				{
					// For 8-bit formats (unsigned, 0 to 255)
					// Packed DWORD = 0x80808080;
					bSilenceData = 0x80;
				}
				else if(m_pFormat->wBitsPerSample == 16)
				{
					// For 16-bit formats (signed, -32768 to 32767)
					// Packed DWORD = 0x00000000;
					bSilenceData = 0x00;
				}
			}

			return bSilenceData;
		}


		//////////////////////////////////////////////////////////////////////////////////////////

		// Constructor
		xOggStream::xOggStream()
		{
			// Initialize data members
			m_poggfile = NULL;
			m_pdsb = NULL;
			m_pTimer = NULL;
			m_bPlaying = m_bCued = FALSE;
			m_bIsLoop = FALSE;
			m_lVolume = 100;
			m_lPanning = 0;
			m_lInService = FALSE;
			m_cbBufOffset = 0;
			m_nBufLength = 2000;
			m_cbBufSize = 0;
			m_nBufService = 250;
			m_nDuration = 0;
			m_nTimeStarted = 0;
			m_nTimeElapsed = 0;			

			m_bBGM = FALSE;

			InitializeCriticalSection(&m_CriticalSection);
		}

		// Destructor
		xOggStream::~xOggStream()
		{
			Destroy();

			DeleteCriticalSection(&m_CriticalSection);
		}

		// Create
		BOOL xOggStream::Create(LPCTSTR pszFilename)
		{
			BOOL fRtn = TRUE;

			if(!xSound::IsInitDSound) 
			{
				return FALSE;
			}

			if(pszFilename)
			{
				// Create a new CWaveFile object
				if(m_poggfile = new xOggFile)
				{
					// Open given file
					if(m_poggfile->Open(pszFilename))
					{
						// Calculate sound buffer size in bytes
						m_cbBufSize = (m_poggfile->GetAvgDataRate() * m_nBufLength) / 1000;
						m_cbBufSize = (m_cbBufSize > m_poggfile->GetDataSize()) ? m_poggfile->GetDataSize() : m_cbBufSize;

						// Get duration of sound
						m_nDuration = m_poggfile->GetDuration();

						// Create sound buffero
						HRESULT hr;
						memset(&m_dsbd, 0, sizeof (DSBUFFERDESC));
						m_dsbd.dwSize = sizeof (DSBUFFERDESC);
						//m_dsbd.dwFlags = DSBCAPS_GETCURRENTPOSITION2|DSBCAPS_CTRLPAN|DSBCAPS_CTRLVOLUME|DSBCAPS_CTRLFREQUENCY|DSBCAPS_LOCSOFTWARE;
						m_dsbd.dwFlags = DSBCAPS_GETCURRENTPOSITION2|DSBCAPS_CTRLPAN|DSBCAPS_CTRLVOLUME|DSBCAPS_CTRLFREQUENCY|DSBCAPS_LOCSOFTWARE|DSBCAPS_GLOBALFOCUS;
						//m_dsbd.dwFlags = DSBCAPS_GETCURRENTPOSITION2|DSBCAPS_CTRLDEFAULT;
						m_dsbd.dwBufferBytes = m_cbBufSize;
						m_dsbd.lpwfxFormat = m_poggfile->m_pFormat;

						hr = xSound::GetDS()->CreateSoundBuffer(&m_dsbd, &m_pdsb, NULL);
						if(hr == DS_OK)
						{
							// Cue for playback
							Cue();
						}
						else 
						{
							fRtn = FALSE;
						}
					}
					else
					{
						// Error opening file
						delete m_poggfile;
						m_poggfile = NULL;
						fRtn = FALSE;
					}
				}
				else 
				{
					fRtn = FALSE;
				}
			}
			else
			{
				fRtn = FALSE;
			}

			return fRtn;
		}

		// Destroy
		BOOL xOggStream::Destroy(void)
		{
			// �ڵ� ũ��Ƽ�ü���
			COggAutoLock autolock(this);

			BOOL fRtn = TRUE;

			// Stop playback
			Stop();

			// Release DirectSound buffer
			SAFE_RELEASE(m_pdsb);

			// Delete xOggFile object
			if(m_poggfile) 
			{
				delete(m_poggfile);
				m_poggfile = NULL;
			}

			return fRtn;
		}

		// WriteWaveData
		BOOL xOggStream::WriteWaveData(UINT size)
		{
			if(!IsPlaying())
			{
				return FALSE;
			}

			HRESULT hr;
			LPBYTE lpbuf1 = NULL;
			LPBYTE lpbuf2 = NULL;
			DWORD dwsize1 = 0;
			DWORD dwsize2 = 0;
			DWORD dwbyteswritten1 = 0;
			DWORD dwbyteswritten2 = 0;
			BOOL fRtn = TRUE;

			// Lock the sound buffer
			hr = m_pdsb->Lock(m_cbBufOffset, size, (LPVOID*)&lpbuf1, &dwsize1, (LPVOID*)&lpbuf2, &dwsize2, 0);
			
			if(hr == DS_OK) 
			{
				// Write data to sound buffer
				if((dwbyteswritten1 = m_poggfile->Read(lpbuf1, dwsize1)) == dwsize1) 
				{
					// Second write required?
					if(lpbuf2) 
					{
						if((dwbyteswritten2 = m_poggfile->Read(lpbuf2, dwsize2)) == dwsize2)
						{
							// Both write operations successful!
						}
						else 
						{
							fRtn = FALSE;
						}
					}
				}
				else 
				{
					fRtn = FALSE;
				}

				// Update our buffer offset and unlock sound buffer
				m_cbBufOffset = (m_cbBufOffset + dwbyteswritten1 + dwbyteswritten2) % m_cbBufSize;
				m_pdsb->Unlock (lpbuf1, dwbyteswritten1, lpbuf2, dwbyteswritten2);
			}
			else 
			{
				fRtn = FALSE;
			}

			return fRtn;
		}

		// WriteSilence
		BOOL xOggStream::WriteSilence(UINT size)
		{
			if(!IsPlaying())
			{
				return FALSE;
			}

			HRESULT hr;
			LPBYTE lpbuf1 = NULL;
			LPBYTE lpbuf2 = NULL;
			DWORD dwsize1 = 0;
			DWORD dwsize2 = 0;
			DWORD dwbyteswritten1 = 0;
			DWORD dwbyteswritten2 = 0;
			BOOL fRtn = TRUE;

			// Lock the sound buffer
			hr = m_pdsb->Lock(m_cbBufOffset, size, (LPVOID*)&lpbuf1, &dwsize1, (LPVOID*)&lpbuf2, &dwsize2, 0);
			if(hr == DS_OK)
			{
				BYTE bSilence = m_poggfile->GetSilenceData();

				if(IsPlaying()) 
				{
					// Write silence to sound buffer
					memset (lpbuf1, bSilence, dwsize1);
					dwbyteswritten1 = dwsize1;

					// Second write required?
					if(lpbuf2)
					{
						memset(lpbuf2, bSilence, dwsize1);
						dwbyteswritten2 = dwsize2;
					}
				}

				// Update our buffer offset and unlock sound buffer
				m_cbBufOffset = (m_cbBufOffset + dwbyteswritten1 + dwbyteswritten2) % m_cbBufSize;
				m_pdsb->Unlock(lpbuf1, dwbyteswritten1, lpbuf2, dwbyteswritten2);
			}
			else 
				fRtn = FALSE;

			return fRtn;
		}

		// GetMaxWriteSize
		DWORD xOggStream::GetMaxWriteSize(void)
		{
			DWORD dwWriteCursor, dwPlayCursor, dwMaxSize;

			// Get current play position
			if(m_pdsb->GetCurrentPosition(&dwPlayCursor, &dwWriteCursor) == DS_OK)
			{
				if(m_cbBufOffset <= dwPlayCursor)
				{
					// Our write position trails play cursor
					dwMaxSize = dwPlayCursor - m_cbBufOffset;
				}
				else
				{
					// Play cursor has wrapped
					dwMaxSize = m_cbBufSize - m_cbBufOffset + dwPlayCursor;
				}
			}
			else
			{
				// GetCurrentPosition call failed
				dwMaxSize = 0;
			}

			return dwMaxSize;
		}

		// Service Buffer
		BOOL xOggStream::ServiceBuffer(bool &bDelete)
		{
			COggAutoLock autolock(this);
// 			if(!IsPlaying() || m_bDeleteQueue)
// 			{
// 				// ��Ʈ�� �޴��� �������
// 				RegisterStreamMan(FALSE);
// 
// 								
// 				return FALSE;
// 			}

			if(!IsPlaying())
				return FALSE;

			BOOL fRtn = TRUE;

			// Check for reentrance
			if(InterlockedExchange(&m_lInService, TRUE) == FALSE)
			{
				// Maintain elapsed time count
				m_nTimeElapsed = timeGetTime() - m_nTimeStarted;

				// Stop if all of sound has played
				if(m_nTimeElapsed < m_nDuration)
				{
					// All of sound not played yet, send more data to buffer
					DWORD dwFreeSpace = GetMaxWriteSize();

					// Determine free space in sound buffer
					if(dwFreeSpace)
					{
						// See how much wave data remains to be sent to buffer
						DWORD dwDataRemaining = m_poggfile->GetNumBytesRemaining();
						if(dwDataRemaining == 0)
						{
							// All wave data has been sent to buffer
							if(WriteSilence(dwFreeSpace) == FALSE)
							{
								// Error writing silence data
								fRtn = FALSE;
							}
						}
						else if(dwDataRemaining >= dwFreeSpace)
						{
							// Enough wave data remains to fill free space in buffer
							if(WriteWaveData(dwFreeSpace) == FALSE)
							{
								// Error writing wave data
								fRtn = FALSE;
							}
						}
						else
						{
							// Some wave data remains, but not enough to fill free space
							if(WriteWaveData(dwDataRemaining) == TRUE)
							{
								if(WriteSilence(dwFreeSpace - dwDataRemaining) == FALSE)
								{
									// Error writing silence data
									fRtn = FALSE;
								}
							}
							else
							{
								// Error writing wave data
								fRtn = FALSE;
							}
						}
					}
					else
					{
						// No free space in buffer for some reason
						fRtn = FALSE;
					}
				}
				else
				{
					bool use_thread = m_pTimer ? false : true;

					// ��Ʈ�� �޴��� �������
					///RegisterStreamMan(FALSE);

					Stop();

					if(m_bIsLoop)
					{
						Play(PLAY_LOOP, m_lVolume, m_lPanning, m_bBGM, use_thread);
					}
				}

				// Reset reentrancy semaphore
				InterlockedExchange(&m_lInService, FALSE);				
			}
			else
			{
				// Service routine reentered. Do nothing
				fRtn = FALSE;
			}
			return fRtn;
		}

		// Cue
		void xOggStream::Cue(void)
		{
			if(!m_bCued)
			{
				// Reset buffer ptr
				m_cbBufOffset = 0;

				// Reset file ptr
				if(m_poggfile->CueFile() == TRUE)
				{
					// Reset DirectSound buffer
					m_pdsb->SetCurrentPosition(0);

					// Fill buffer with wave data
					if(WriteWaveData(m_cbBufSize) == FALSE)
						return;

					m_bCued = TRUE;
				}

#pragma region Legacy Code
				// Reset file ptr
				//m_poggfile->CueFile();

				// Reset DirectSound buffer
				//m_pdsb->SetCurrentPosition(0);

				//// Fill buffer with wave data
				//if(!WriteWaveData(m_cbBufSize))
				//	return;

				//m_bCued = TRUE;
#pragma endregion Legacy Code
			}
		}

		BOOL xOggStream::IsPlaying()
		{
			if(m_bPlaying)
				return TRUE;

			return FALSE;
		}

		// Play
		void xOggStream::Play(BOOL bLoopFlag, LONG lVolume, LONG lPanning, BOOL bBGM, bool use_thread)
		{
			// �ڵ� ũ��Ƽ�ü���

			m_bIsLoop = bLoopFlag;

			if(!xSound::IsInitDSound)
				return;

			if(m_pdsb)
			{
				// Stop if playing
				if(IsPlaying())
					Stop ();

				// �÷��� ����
				SetPlaying(TRUE);

				// Cue for playback if necessary
				if(!m_bCued) Cue ();

				SetVolume(lVolume);
				SetPanning(lPanning);

				// Begin DirectSound playback
				HRESULT hr = m_pdsb->Play(0, 0, DSBPLAY_LOOPING);
				if(hr == DS_OK)
				{
					// Sound Type Setting
					m_bBGM = bBGM;

					// Playback begun
					m_bCued = FALSE;
					m_nTimeStarted = timeGetTime();

					if (use_thread)
					{
						RegisterStreamMan(TRUE);	//OGG ��Ʈ�� �޴����� ���
					}
					else
					{
						if (m_pTimer)
							m_pTimer->Stop();
						else
							m_pTimer = new xMMTimer;

						// Kick off timer to service buffer
						if(m_pTimer)
						{
							m_pTimer->Create(m_nBufService, m_nBufService, (DWORD)(this), TimerCallback);
						}
					}
				}
				else 
				{
					SetPlaying(FALSE);
				}
			}
		}

		// Stop
		void xOggStream::Stop(void)
		{
			// Delete Timer object
 			if(m_pTimer)
				m_pTimer->Stop();

			if(IsPlaying())
			{
				// Stop DirectSound playback
				if(xSound::IsInitDSound && m_pdsb) 
					m_pdsb->Stop();

				SetPlaying(FALSE);
				//InterlockedExchange(&m_bDeleteQueue, TRUE);				
			}

			if (m_pTimer)
			{
				delete m_pTimer;
				m_pTimer = NULL;
			}
			else
				// ��Ʈ�� �޴��� �������
				RegisterStreamMan(FALSE);
		}

		// Set Volume of Sound Stream
		BOOL xOggStream::SetVolume(LONG lVolume)
		{
			if (lVolume > 100) lVolume = 100;
			if (lVolume <= 0) lVolume = 1;

			m_lVolume = lVolume;

			lVolume = static_cast<LONG>(-2000.0f * log10(100.0f / lVolume));
			if (lVolume <= -4000) lVolume = -10000;

			return (m_pdsb->SetVolume(lVolume) != DS_OK) ? FALSE : TRUE;
		}

		// Set panning of Sound Stream
		BOOL xOggStream::SetPanning(LONG lPanning)
		{
			if(lPanning > 100)
				lPanning = 100;
			if(lPanning < -100) 
				lPanning = -100;

			m_lPanning = lPanning;

			lPanning *= 100;  // lPanning = -10000 ~ 10000

			return (m_pdsb->SetPan(lPanning) != DS_OK) ? FALSE : TRUE;
		}


		// TimerCallback
		BOOL xOggStream::TimerCallback(DWORD dwUser)
		{
			// dwUser contains ptr to xOggStream object
			xOggStream* pOggStream = (xOggStream*) dwUser;

			if(IsBadReadPtr(pOggStream, sizeof(xOggStream)))
				return FALSE;

			// �ڵ� ũ��Ƽ�ü���
			COggAutoLock autolock(pOggStream);

			if(!pOggStream->IsPlaying())
				return FALSE;

			bool bAutoDelete = false;
			return pOggStream->ServiceBuffer(bAutoDelete);
		}

		void xOggStream::SetPlaying(BOOL bFlag)
		{
			m_bPlaying = bFlag;		
		}

		void xOggStream::RegisterStreamMan(BOOL bRegister)
		{
			//
			// OGG ��Ʈ�� �޴����� ����ϰų� �����Ѵ�.
			if (bRegister)
			{
				if(m_bBGM)
				{
					gStreamMan().AddStreamBGM(this);
				}
				else
				{
					gStreamMan().AddStream(this);
				}
			}
			else
			{
				if(m_bBGM)
				{
					gStreamMan().DeleteFrontBGM(this);
				}
				else
				{
					gStreamMan().DeleteFront(this);
				}				
			}
		}

	} //namespace SOUND

}	//namespace NMBASE