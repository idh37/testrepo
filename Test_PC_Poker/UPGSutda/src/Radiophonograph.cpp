// Radiophonograph.cpp: implementation of the CRadiophonograph class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Radiophonograph.h"
#include "GlobalGame.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define RECT_BGM_PANEL (CRect(907, 107, 1000, 240))


#define SUTDA_SKIN_NUM (1)

#define IDM_BGMONBTN	150
#define IDM_BGMOFFBTN	151
#define IDM_BOARDONBTN  152
#define IDM_BOARDOFFBTN 153
#define	IDM_STOPBTN		154
#define IDM_PLAYBTN		155
#define	IDM_VOLUMEBTN	156
#define IDM_MUSICLIST1  157
#define IDM_MUSICLIST2	158
#define IDM_MUSICLIST3	159
#define IDM_MUSICLIST4	160
#define IDM_MUSICLIST5	161
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
int moveyp = 10;

CRadiophonograph::CRadiophonograph()
{
	m_pParent = NULL;
	m_MusicName = NULL;

	m_dwInRecord = 0;		//���ڵ� �ֱ�
	m_dwPlayRecord = 0;		//���ڵ� ���ư���
	m_dwTurnRecord = 0;		//���ڵ� ������ 
}

CRadiophonograph::~CRadiophonograph()
{
	m_pParent = NULL;
}

void CRadiophonograph::LoadImage(void)
{
	//��������Ʈ �б�
	AddLoadImageList(&m_InRecordspr,"Sutda\\data\\Bgm\\BgmAni_1.spr", false);
	AddLoadImageList(&m_PlayRecordspr,"Sutda\\data\\Bgm\\BgmAni_2.spr", false);
	AddLoadImageList(&m_TurnRecordspr,"Sutda\\data\\Bgm\\BgmAni_3.spr", false);
	AddLoadImageList(&m_MoveSpeakerSpr,"Sutda\\data\\Bgm\\BgmAni_4.spr", false);

	AddLoadImageList(&m_BGBoardBack,"Sutda\\data\\Bgm\\BgmBg.spr", false);
	AddLoadImageList(&m_Bgm_On_OffBtnspr,"Sutda\\data\\Bgm\\BgmBtn.spr", false);
	AddLoadImageList(&m_BoardBtnspr,"Sutda\\data\\Bgm\\BgmBoardBtn.spr", false);
	AddLoadImageList(&SoundBtnSpr,"Sutda\\data\\Bgm\\SoundBtn.spr", false);
	AddLoadImageList(&MusicListBtnSpr,"Sutda\\data\\Bgm\\Bgmtm.spr", false);

	AddLoadImageList(&m_PlayListSpr,"Sutda\\data\\Bgm\\BgmList.spr", false);
	AddLoadImageList(&m_BgmViewSpr,"Sutda\\data\\Bgm\\BgmView.spr", false);

}

void CRadiophonograph::Init( CWnd *pWnd, NMBASE::GRAPHICGDI::CPage *ppgae)
{
	m_pParent = pWnd;
	m_Page = ppgae;

	m_MusicName = NULL;
	
	m_bInRecord = FALSE;
	m_bTurnRecord = FALSE;
	m_bBoardMove = FALSE;
	m_bVolume = FALSE;

	m_myVolume = 85;
	m_NowPlayMusic = -1;
	m_MusicNum = 0;
	m_BoardState = 0;
	m_bPlayRecord = 0;

	m_dwInRecord = timeGetTime();		//���ڵ� �ֱ�
	m_dwPlayRecord = timeGetTime();		//���ڵ� ���ư���
	m_dwTurnRecord = timeGetTime();		//���ڵ� ������ 
	m_dwBGBoard = timeGetTime();		//bgm���� �����̱�

	
	

	m_BgmOnBtn.Init( m_pParent, m_Page, 859, 84, &m_Bgm_On_OffBtnspr, 0, IDM_BGMONBTN, 3);
	m_BgmOffBtn.Init( m_pParent, m_Page, 859, 84, &m_Bgm_On_OffBtnspr, 1, IDM_BGMOFFBTN, 3);
	m_BgmOffBtn.Show(FALSE);

	m_BgBoardOnBtn.Init( m_pParent, m_Page, 983, 100 + moveyp, &m_BoardBtnspr, 0, IDM_BOARDONBTN, 3);
	m_BgBoardOffBtn.Init( m_pParent, m_Page, 890, 100 + moveyp, &m_BoardBtnspr, 1, IDM_BOARDOFFBTN, 3);
	m_BgBoardOffBtn.Show(FALSE);

	m_StopBtn.Init( m_pParent, m_Page, 985, 121 + moveyp, &SoundBtnSpr, 2, IDM_STOPBTN, 4);
	m_StopBtn.Show(FALSE);
	m_PlayBtn.Init( m_pParent, m_Page, 971, 121 + moveyp, &SoundBtnSpr, 1, IDM_PLAYBTN, 4);
	m_PlayBtn.Show(FALSE);
	m_VolumeBtn.Init( m_pParent, m_Page, 937, 121 + moveyp, &SoundBtnSpr, 0, IDM_VOLUMEBTN, 4);
	m_VolumeBtn.Show(FALSE);

	m_Music1Btn.Init( m_pParent, m_Page, 914, 152 + moveyp, &MusicListBtnSpr, 0, IDM_MUSICLIST1, 4);
	m_Music2Btn.Init( m_pParent, m_Page, 914, 166 + moveyp, &MusicListBtnSpr, 1, IDM_MUSICLIST2, 4);
	m_Music3Btn.Init( m_pParent, m_Page, 914, 180 + moveyp, &MusicListBtnSpr, 2, IDM_MUSICLIST3, 4);
	m_Music4Btn.Init( m_pParent, m_Page, 914, 194 + moveyp, &MusicListBtnSpr, 3, IDM_MUSICLIST4, 4);
	m_Music5Btn.Init( m_pParent, m_Page, 914, 210 + moveyp, &MusicListBtnSpr, 4, IDM_MUSICLIST5, 4);
}

void CRadiophonograph::Reset()
{

	m_bInRecord = FALSE;
	m_bTurnRecord = FALSE;
	m_bBoardMove = FALSE;
	m_bVolume = FALSE;

	m_BoardState = 0;
	m_bPlayRecord = 0;

	m_dwInRecord = 0;		//���ڵ� �ֱ�
	m_dwPlayRecord = 0;		//���ڵ� ���ư���
	m_dwTurnRecord = 0;		//���ڵ� ������ 
	m_dwBGBoard = 0;		//bgm���� �����̱�

	m_BgBoardOnBtn.Show(TRUE);
	m_BgBoardOffBtn.Show(FALSE);

	m_StopBtn.Show(FALSE);
	m_PlayBtn.Show(FALSE);
	m_VolumeBtn.Show(FALSE);
}

void CRadiophonograph::ReStart()
{
	//m_myVolume = 85;	
	SetRadioVolume(g_Config.BGMVolume);
	SetOnOff(TRUE);
}

void CRadiophonograph::PlayMusic( int r_kind )
{
	switch( r_kind)
	{
	case 0:
		{
			m_MusicName = MUSIC_OGG01;
			m_NowPlayMusic = 0;
			SetMusicOn(0);
			
			NMBASE::SOUND::g_Sound.PlayOggStream_BGM(MUSIC_OGG01 , TRUE, m_myVolume );
		}break;
	
	case 1:
		{
			m_NowPlayMusic = 1;		
			m_MusicName = MUSIC_OGG02;
			SetMusicOn(1);

			NMBASE::SOUND::g_Sound.PlayOggStream_BGM( MUSIC_OGG02 , TRUE, m_myVolume );
		}break;
	case 2:
		{
			m_NowPlayMusic = 2;
			m_MusicName = MUSIC_OGG03;
			SetMusicOn(2);

			NMBASE::SOUND::g_Sound.PlayOggStream_BGM( MUSIC_OGG03 , TRUE, m_myVolume );
		}break;
	case 3:
		{
			m_NowPlayMusic = 3;
			m_MusicName = MUSIC_OGGTheme;
			SetMusicOn(3);

			NMBASE::SOUND::g_Sound.PlayOggStream_BGM( MUSIC_OGGTheme , TRUE, m_myVolume );
		}break;
	case 4:
		{
			m_NowPlayMusic = 4;
			m_MusicName = MUSIC_OGG04;
			SetMusicOn(4);

			NMBASE::SOUND::g_Sound.PlayOggStream_BGM( MUSIC_OGG04 , TRUE, m_myVolume );
		}
	}
}

void CRadiophonograph::StopMusic()
{
	switch( m_NowPlayMusic )
	{
	case 0:
		{
			NMBASE::SOUND::g_Sound.Stop( MUSIC_OGG01 );
		}break;
	
	case 1:
		{
			NMBASE::SOUND::g_Sound.Stop( MUSIC_OGG02 );
		}break;
		
	case 2:
		{
			NMBASE::SOUND::g_Sound.Stop( MUSIC_OGG03 );
		}break;
	case 3:
		{
			NMBASE::SOUND::g_Sound.Stop( MUSIC_OGGTheme );
		}break;
	case 4:
		{
			NMBASE::SOUND::g_Sound.Stop( MUSIC_OGG04 );
		}break;
	}
}

void CRadiophonograph::SetMusicOn( int r_kind )
{
	switch( r_kind )
	{
	case 0:
		{
			m_MusicNum = 0;

			m_Music1Btn.Enable(FALSE);
			m_Music2Btn.Enable(TRUE);
			m_Music3Btn.Enable(TRUE);
			m_Music4Btn.Enable(TRUE);
			m_Music5Btn.Enable(TRUE);
		}break;
	case 1:
		{
			m_MusicNum = 1;

			m_Music1Btn.Enable(TRUE);
			m_Music2Btn.Enable(FALSE);
			m_Music3Btn.Enable(TRUE);
			m_Music4Btn.Enable(TRUE);
			m_Music5Btn.Enable(TRUE);
		}break;
	case 2:
		{
			m_MusicNum = 2;
				
			m_Music1Btn.Enable(TRUE);
			m_Music2Btn.Enable(TRUE);
			m_Music3Btn.Enable(FALSE);
			m_Music4Btn.Enable(TRUE);
			m_Music5Btn.Enable(TRUE);
		}break;
	case 3:
		{
			m_MusicNum = 3;
				
			m_Music1Btn.Enable(TRUE);
			m_Music2Btn.Enable(TRUE);
			m_Music3Btn.Enable(TRUE);
			m_Music4Btn.Enable(FALSE);
			m_Music5Btn.Enable(TRUE);
		}break;
	case 4:
		{
			m_MusicNum = 4;
				
			m_Music1Btn.Enable(TRUE);
			m_Music2Btn.Enable(TRUE);
			m_Music3Btn.Enable(TRUE);
			m_Music4Btn.Enable(TRUE);
			m_Music5Btn.Enable(FALSE);
		}break;

	}
}

#include <conio.h>
int offset=0;

//������ �����Ѵ�.
void CRadiophonograph::SetVolume(int x)
{
	if (m_MusicName==NULL)
		return;

	int startx = 930;
	int endx = 950;

//	AllocConsole();
	x -= offset;

//	_cprintf("[%d] ", x);

	if(!m_bVolume) return;	
	
	if(x < startx ) x = startx;
	if( x > endx ) x = endx;

//	_cprintf("  => [%d] \n", x);

	m_VolumeBtn.MoveWindow( x, m_VolumeBtn.m_Yp);

	float ftt = ( float )( m_VolumeBtn.m_Xp - startx ) / ( float )(endx - startx);
	//float ftt = ( float )( m_VolumeBtn.m_Xp - startx ) / ( (endx - startx) - m_VolumeBtn.m_Width );
	int per = ( int )( ftt * 100.0f );
	
// 	if( m_VolumeBtn.m_Xp == startx) per = 0;
// 	else per += 50;
	
// 	//1�ȼ��� 2�������� ��� 60�� �⺻ġ�� �ش�
// 	int volume = (m_VolumeBtn.m_Xp - startx) * 2 + 60;
// 	per = volume;

	m_myVolume = per;

// 	if (m_MusicName)
// 		NMBASE::SOUND::g_Sound.SetVolume( m_MusicName, m_myVolume );

	SetRelativeVolume(m_myVolume);

	g_Config.BGMVolume = m_myVolume;

//	_cprintf("[%d] \n", per);
}

void CRadiophonograph::SetRelativeVolume(int nVolume)
{
	//0-100 ������ ������ 60-100 �������� �ٲ۴�/
	//100:40 = nVolume : nRelativeVolume;
	//nRelativeVolume = 40*nVolume /100
	
	#pragma region Delete
	/*const int MIN_VOLUME = 60;

	int nRelativeVolume = 0;
	if (nVolume > 0)
		nRelativeVolume = ((100 - MIN_VOLUME) * nVolume) / 100;

	nRelativeVolume += MIN_VOLUME;
	if (m_MusicName)
		NMBASE::SOUND::g_Sound.SetVolume(m_MusicName, nRelativeVolume);*/
	#pragma endregion

	if (m_MusicName != NULL)
		NMBASE::SOUND::g_Sound.SetVolume(m_MusicName, nVolume);
}

void CRadiophonograph::SetRadioVolume( int volume )
{
	int per = volume;
	if( per < 0 || per > 100 )
		per = 100;

	m_myVolume = per;

	SetRelativeVolume(m_myVolume);	

	int startX = 930;
	int endX = 950;

	//int Btnxp = ((volume-60)/2) + 930;

	int nBtnXp = 0;

	int nTemp = ((endX - startX) * volume);
	if (0 < nTemp)
		nBtnXp = startX + (nTemp / 100);

	if (nBtnXp < startX) nBtnXp = startX;
	if (nBtnXp > endX) nBtnXp = endX;

	m_VolumeBtn.MoveWindow( nBtnXp, m_VolumeBtn.m_Yp);
}

//���̵���&�ƿ� ������ �Ѵ�.
void CRadiophonograph::SetFade( BOOL b_set )
{
	//�������� �ƴҶ��� �����Ѵ�.
	if( m_bPlayRecord != 2 ) return;

	//���� ����  m_MusicName �� NULL �ΰ�� 
	
	//b_set ���󺹱� !b_set �Ҹ��� ���δ�
	if (m_MusicName)
	{
		if( b_set )
			NMBASE::SOUND::g_Sound.SetFade(m_MusicName, m_myVolume, 200);
		else
			NMBASE::SOUND::g_Sound.SetFade(m_MusicName, 10, 100);
	}
}

//�������ư
void CRadiophonograph::On_OffBgm( BOOL b_set, int r_kind )
{
	switch( r_kind )
	{
	case 0:
		{
			m_bInRecord = b_set;
			m_bTurnRecord = FALSE;
			
			//���ڵ带 �۵��Ҷ�
			if( m_bInRecord )
			{
				m_dwInRecord = timeGetTime();
				
				m_bPlayRecord = 0;	//�۵���ư�� ���������� ���ڵ����� �׷����� �ʴ´�. 
			}
			//���ڵ带 ���⶧
			else
			{
				m_bPlayRecord = 1;	//������ ���������� ������ ���ڵ����� �׷��ش�.

				//�÷��̵� �ð����� �־��ش�.
				m_dwPlayRecord = ( timeGetTime() - m_dwPlayRecord);
				m_dwTurnRecord = ( timeGetTime() - m_dwTurnRecord);
			}			
		}break;
	case 1:
		{
			m_bPlayRecord = 2;
			m_dwPlayRecord = timeGetTime();
		}break;
	case 2:
		{
			m_bTurnRecord = b_set;
		}break;
	}
}

//�����ư
void CRadiophonograph::On_OffBoard(bool bOn)
{
	if (bOn)
	{
		m_bBoardMove = TRUE;
		m_BoardState = 1;
		m_dwBGBoard = timeGetTime();
		m_BgBoardOnBtn.Show(FALSE);
		m_BgBoardOffBtn.Show(TRUE);
	}
	else//off��ư  (BGM���带 �����Ѵ�)
	{

		m_bBoardMove = TRUE;
		m_BoardState = 0;
		m_dwBGBoard = timeGetTime();
					
		m_BgBoardOnBtn.Show(TRUE);
		m_BgBoardOffBtn.Show(FALSE);
	}
}

//�ִϸ��̼Ǻ��� �ð��� ���ؿ´�. 
DWORD CRadiophonograph::GetAniTime( int r_kind )
{
	DWORD dwTime = timeGetTime();
	DWORD dwTime1 = 0;
	
	if( r_kind == 0 ) dwTime1 = abs((int) (dwTime - m_dwInRecord) );
	else if( r_kind == 1 ) dwTime1 = abs((int) ( dwTime - m_dwPlayRecord) );	//���ڵ尡 ���ư���&equalizerǥ��
	else if( r_kind == 2 )  dwTime1 = abs( (int) (dwTime - m_dwTurnRecord) );
	else if( r_kind == 3 )  dwTime1 = abs( (int) (dwTime - m_dwBGBoard) );

	return dwTime1;
}

void CRadiophonograph::Draw_Top( CDC *pDC )
{
	//����׷��ֱ�
	if( m_BoardState )	//�����ִ� ��Ȳ
	{
		if(m_bBoardMove) 
		{
			m_Page->DrawScene( 979, 96 + moveyp, &m_BGBoardSpr, 0, GetAniTime( 3 ), FALSE );
		}
		else 
		{	
			//m_Page->PutSprAuto( 886, 96 + moveyp, &m_BGBoardBack , g_pGameView->m_SkinNum, HB );
			m_Page->PutSprAuto( 886, 96 + moveyp, &m_BGBoardBack , SUTDA_SKIN_NUM, GB, 20 );				

			m_StopBtn.Draw(pDC);
			m_PlayBtn.Draw(pDC);
			m_VolumeBtn.Draw(pDC);

			m_Music1Btn.Draw(pDC);
			m_Music2Btn.Draw(pDC);
			m_Music3Btn.Draw(pDC);
			m_Music4Btn.Draw(pDC);
			m_Music5Btn.Draw(pDC);

			m_Page->PutSprAuto( 916, 105 + moveyp, &m_PlayListSpr , m_NowPlayMusic);
			//���ڵ尡 ���ư��� 
			if( m_bPlayRecord == 2)
					m_Page->DrawScene( 979, 103 + moveyp, &m_BgmViewSpr, 0, GetAniTime( 1 ) );
		}

		m_BgBoardOffBtn.Draw(pDC);

	}
	else
	{		
		if(m_bBoardMove)  m_Page->DrawScene( 979, 96 + moveyp, &m_BGBoardSpr, 1, GetAniTime( 3 ), FALSE );
		else m_Page->PutSprAuto( 979, 96 + moveyp, &m_BGBoardBack , SUTDA_SKIN_NUM);

		m_BgBoardOnBtn.Draw(pDC);
	}


}

void CRadiophonograph::Draw_Bottom( CDC *pDC )
{

	m_Page->SetTempClip(0,34,1000,500);
	m_Page->EnableTempClip(TRUE);

	

	
	if(m_bInRecord )
	{
		m_Page->DrawScene( 855, 40, &m_InRecordspr, 0, GetAniTime( 0 ), FALSE );
	}

	//���ڵ尡 ���ư��� 
	if( m_bPlayRecord == 2)
	{
		m_Page->DrawScene( 858, 52, &m_PlayRecordspr, 0, GetAniTime( 1 ) );
	}
	//���ڵ尡 �����ֶ�
	else if( m_bPlayRecord == 1) m_Page->DrawScene( 858, 52, &m_PlayRecordspr, 0, m_dwPlayRecord );

	if( m_bTurnRecord)
	{
		m_Page->DrawScene( 921, 75, &m_TurnRecordspr, 0, GetAniTime( 2 ) );
		m_Page->DrawScene( 765, 0, &m_MoveSpeakerSpr, SUTDA_SKIN_NUM, GetAniTime( 2 ) );
		
	}
	else
	{
		m_Page->DrawScene( 921, 75, &m_TurnRecordspr, 0, m_dwTurnRecord );
		m_Page->DrawScene( 765, 0, &m_MoveSpeakerSpr, SUTDA_SKIN_NUM, m_dwTurnRecord );
	}


	m_BgmOnBtn.Draw(pDC);
	m_BgmOffBtn.Draw(pDC);


	m_Page->EnableTempClip(FALSE);
}

void CRadiophonograph::OnTimer()
{
	if( m_bInRecord && m_InRecordspr.IsSceneTimeOver( 0, GetAniTime( 0 ) ) )
	{
		On_OffBgm( FALSE, 0 );
		On_OffBgm( TRUE, 1 );
		On_OffBgm( TRUE, 2 );
		
		PlayMusic( m_MusicNum );
	}

	//�������� �������� �����Ѵ�.
	if( m_bBoardMove && m_BGBoardSpr.IsSceneTimeOver(m_BoardState, GetAniTime(3)))
	{
		m_bBoardMove = FALSE;
		
		if( m_BoardState == 0 )
		{
			m_StopBtn.Show(FALSE);
			m_PlayBtn.Show(FALSE);
			m_VolumeBtn.Show(FALSE);
		}
		else if( m_BoardState == 1 )
		{
			m_StopBtn.Show(TRUE);
			m_PlayBtn.Show(TRUE);
			m_VolumeBtn.Show(TRUE);
		}
		
	}
}

BOOL CRadiophonograph::OnLButtonDbClick( int x, int y )
{

	if(m_Music1Btn.PtInRect( x, y ) )
	{
		if( m_NowPlayMusic == 0 ) return FALSE;

		SetOnOff( FALSE );
		SetOnOff( TRUE );
		
		return TRUE;
	}
	if(m_Music2Btn.PtInRect( x, y ) ) 
	{
		if( m_NowPlayMusic == 1 ) return FALSE;
		
		SetOnOff( FALSE );
		SetOnOff( TRUE );
		
		return TRUE;
	}
	if(m_Music3Btn.PtInRect( x, y ) )
	{
		if( m_NowPlayMusic == 2 ) return FALSE;
		
		SetOnOff( FALSE );
		SetOnOff( TRUE );

		return TRUE;
	}

	if(m_Music4Btn.PtInRect( x, y ) )
	{
		if( m_NowPlayMusic == 3 ) return FALSE;
		
		SetOnOff( FALSE );
		SetOnOff( TRUE );

		return TRUE;
	}

	if(m_Music5Btn.PtInRect( x, y ) )
	{
		if( m_NowPlayMusic == 4 ) return FALSE;
		
		SetOnOff( FALSE );
		SetOnOff( TRUE );

		return TRUE;
	}

	if (m_BoardState)
	{		
		if (RECT_BGM_PANEL.PtInRect(CPoint(x,y)))
			return TRUE;
	}

	return FALSE;
}

void CRadiophonograph::SetOnOff(BOOL r_bset)
{
	if( r_bset )
	{
		NMBASE::SOUND::g_Sound.PlayWav( SND19 );

		m_BgmOnBtn.Show(FALSE);
		m_BgmOffBtn.Show(TRUE);

		On_OffBgm(TRUE, 0);	//�����⸦ �۵���Ų��.	
	}
	else
	{
		NMBASE::SOUND::g_Sound.PlayWav( SND18 );

		m_BgmOnBtn.Show(TRUE);
		m_BgmOffBtn.Show(FALSE);

		On_OffBgm(FALSE, 0);	//�����⸦ �����.	

		StopMusic();
	}
}

BOOL CRadiophonograph::OnLButtonDown( int x, int y )
{
	if( m_BgmOnBtn.OnLButtonDown( x, y ))
	{
		SetOnOff( TRUE );

		//��������Ⱑ �������� ���ش�.
		if( g_Config.bMusicON == FALSE )
		{
			g_Config.bMusicON = TRUE;
			NMBASE::SOUND::g_Sound.SetMusic((!g_Config.bMusicON) ^ 1);
		}

		return TRUE;
	}
	
	if( m_BgmOffBtn.OnLButtonDown( x, y ))
	{
		SetOnOff(FALSE);
		return TRUE;
	}
	if(m_BgBoardOnBtn.OnLButtonDown( x, y ) ) return TRUE;
	if(m_BgBoardOffBtn.OnLButtonDown( x, y ) ) return TRUE;
	
	if(m_StopBtn.OnLButtonDown( x, y ) ) return TRUE;
	if(m_PlayBtn.OnLButtonDown( x, y ) ) return TRUE;
	if(m_VolumeBtn.OnLButtonDown( x, y ) )
	{
		m_bVolume = TRUE;

		offset = x-m_VolumeBtn.m_Xp;
		return TRUE;
	}

	if(m_Music1Btn.OnLButtonDown( x, y ) ) return TRUE;
	if(m_Music2Btn.OnLButtonDown( x, y ) ) return TRUE;
	if(m_Music3Btn.OnLButtonDown( x, y ) ) return TRUE;
	if(m_Music4Btn.OnLButtonDown( x, y ) ) return TRUE;
	if(m_Music5Btn.OnLButtonDown( x, y ) ) return TRUE;


	if (m_BoardState)
	{		
		if (RECT_BGM_PANEL.PtInRect(CPoint(x,y)))
			return TRUE;
	}

	return FALSE;
}

BOOL CRadiophonograph::OnLButtonUp( int x, int y )
{
	if( m_BgmOnBtn.OnLButtonUp( x, y ) )
	{
		return TRUE;
	}
	if( m_BgmOffBtn.OnLButtonUp( x, y ) )
	{
		return TRUE;
	}
	if(m_BgBoardOnBtn.OnLButtonUp( x, y ) )
	{
	//	m_dwBGBoard = timeGetTime();
		On_OffBoard(true);
		return TRUE;
	}
	if(m_BgBoardOffBtn.OnLButtonUp( x, y ) )
	{
	//	m_dwBGBoard = timeGetTime();
		On_OffBoard(false);
		return TRUE;
	}

	if(m_StopBtn.OnLButtonUp( x, y ) )
	{
		SetOnOff( FALSE );
		return TRUE;
	}
	if(m_PlayBtn.OnLButtonUp( x, y ) )
	{
		SetOnOff(FALSE);
		SetOnOff( TRUE );
		return TRUE;
	}
	if(m_VolumeBtn.OnLButtonUp( x, y ) )
	{
		m_bVolume = FALSE;
		return TRUE;
	}
	
	if(m_Music1Btn.OnLButtonUp( x, y ) )
	{
		SetMusicOn(0);
		return TRUE;
	}

	if(m_Music2Btn.OnLButtonUp( x, y ) )
	{
		SetMusicOn(1);
		return TRUE;
	}

	if(m_Music3Btn.OnLButtonUp( x, y ) )
	{
		SetMusicOn(2);
		return TRUE;
	}
	
	if(m_Music4Btn.OnLButtonUp( x, y ) )
	{
		SetMusicOn(3);
		return TRUE;
	}

	if(m_Music5Btn.OnLButtonUp( x, y ) )
	{
		SetMusicOn(4);
		return TRUE;
	}

	if(m_bVolume == TRUE) {
		m_VolumeBtn.SetButtonState(0);
		m_bVolume = FALSE;
	}

	return FALSE;
}

BOOL CRadiophonograph::OnMouseMove( int x, int y )
{
	m_BgmOnBtn.OnMouseMove( x, y );
	m_BgmOffBtn.OnMouseMove( x, y );
	m_BgBoardOnBtn.OnMouseMove( x, y );
	m_BgBoardOffBtn.OnMouseMove( x, y );

	m_StopBtn.OnMouseMove( x, y );
	m_PlayBtn.OnMouseMove( x, y );

	if(m_bVolume == TRUE) {
		SetVolume( x );
	}
	if(m_VolumeBtn.OnMouseMove( x, y ))
	{
		
	}

	m_Music1Btn.OnMouseMove( x, y );
	m_Music2Btn.OnMouseMove( x, y );
	m_Music3Btn.OnMouseMove( x, y );
	m_Music4Btn.OnMouseMove( x, y );
	m_Music5Btn.OnMouseMove( x, y );

	if (m_BoardState)
	{		
		
		if (RECT_BGM_PANEL.PtInRect(CPoint(x,y)))
		{
			SetCursor(GM().GetCurSelCursor());
			return TRUE;
		}
	}


	return FALSE;
}