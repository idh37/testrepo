// Radiophonograph.h: interface for the CRadiophonograph class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RADIOPHONOGRAPH_H__B7ADBDD9_40B2_4BDB_8EA5_43A8E5F1F5B3__INCLUDED_)
#define AFX_RADIOPHONOGRAPH_H__B7ADBDD9_40B2_4BDB_8EA5_43A8E5F1F5B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "Graphic.h"
//#include "GraphButton.h"

class CRadiophonograph
{
public:
	CRadiophonograph();
	virtual ~CRadiophonograph();

	CWnd			*m_pParent;
	NMBASE::GRAPHICGDI::CPage *m_Page;

	char* m_MusicName;
	
	BOOL m_bInRecord;
	BOOL m_bTurnRecord;
	BOOL m_bBoardMove;		//보드의 애니메이션 제어
	BOOL m_bVolume;			//볼륨 조정중인가 체크
	
	int m_myVolume;		//내가 선택한 볼륨크기
	int m_NowPlayMusic; //현재 플레이되고 있는 음악
	int m_MusicNum;		//보드에서 선택된 음악(플레이되고 있는 음악과는 틀릴수있다.)
	int m_BoardState;	//보드의 현재 상태
	int m_bPlayRecord;	//3가지 동장 움직임 멈춤 안보임

	DWORD m_dwInRecord;		//레코드 넣기
	DWORD m_dwPlayRecord;	//레코드 돌아가기
	DWORD m_dwTurnRecord;	//레코드 돌리기 
	DWORD m_dwBGBoard;		//BGM보드 움직임 

	CRect m_ScrollBaseRect;		//	스크롤바 영역

	NMBASE::GRAPHICGDI::xSprite m_InRecordspr;		//레코드 넣기
	NMBASE::GRAPHICGDI::xSprite m_PlayRecordspr;	//레코드 돌아가기
	NMBASE::GRAPHICGDI::xSprite m_TurnRecordspr;	//레코드 돌리기 
	NMBASE::GRAPHICGDI::xSprite m_MoveSpeakerSpr;	//스피커 움직이기
	NMBASE::GRAPHICGDI::xSprite m_Bgm_On_OffBtnspr;	//on_off버튼 스프라이트
	NMBASE::GRAPHICGDI::xSprite m_BoardBtnspr;		//on_off버튼 스프라이트
	NMBASE::GRAPHICGDI::xSprite m_BGBoardBack;		//BGM 이동애니 
	NMBASE::GRAPHICGDI::xSprite m_BGBoardSpr;			//BGM 이동애니 

	NMBASE::GRAPHICGDI::xSprite m_PlayListSpr;	//현재 나오고 있는 음악 
	NMBASE::GRAPHICGDI::xSprite SoundBtnSpr;	//	
	NMBASE::GRAPHICGDI::xSprite MusicListBtnSpr;//음악선택버튼
	NMBASE::GRAPHICGDI::xSprite m_BgmViewSpr;	//equalizer표시

	NMBASE::SKINGDI::CGraphButton	m_BgmOnBtn;	//onoff버튼
	NMBASE::SKINGDI::CGraphButton	m_BgmOffBtn;//onoff버튼
	NMBASE::SKINGDI::CGraphButton	m_BgBoardOnBtn;//보드ON	
	NMBASE::SKINGDI::CGraphButton	m_BgBoardOffBtn;//보드OFF

	NMBASE::SKINGDI::CGraphButton	m_PlayBtn;	//플레이버튼 
	NMBASE::SKINGDI::CGraphButton	m_StopBtn;	//스톱버튼 
	NMBASE::SKINGDI::CGraphButton	m_VolumeBtn;

	NMBASE::SKINGDI::CGraphButton    m_Music1Btn;
	NMBASE::SKINGDI::CGraphButton    m_Music2Btn;
	NMBASE::SKINGDI::CGraphButton    m_Music3Btn;
	NMBASE::SKINGDI::CGraphButton    m_Music4Btn;
	NMBASE::SKINGDI::CGraphButton    m_Music5Btn;

	void Init( CWnd *pWnd, NMBASE::GRAPHICGDI::CPage *pPgae );
	void ReStart();	//방입장시 기본설정으로 셋팅
	void Reset();
	void Draw_Top( CDC *pDC );
	void Draw_Bottom( CDC *pDC );
	void OnTimer();
	
	void PlayMusic( int r_kind );
	void StopMusic();
	void SetMusicOn( int r_kind );
	void SetVolume(int x);
	void SetRadioVolume( int volume );

	void On_OffBgm( BOOL b_set, int r_kind );
	void On_OffBoard( bool bOn);

	void SetOnOff(BOOL r_bset);

	void SetFade( BOOL b_set );

	DWORD GetAniTime( int r_kind );

	BOOL OnLButtonDbClick( int x, int y );
	BOOL OnLButtonDown( int x, int y );
	BOOL OnLButtonUp( int x, int y );
	BOOL OnMouseMove( int x, int y );


	
	void LoadImage(void);

protected:
	void SetRelativeVolume(int nVolume);	//0-100구간이 너무커서 60-100 의 수치로 바꿔서 저장
};

#endif // !defined(AFX_RADIOPHONOGRAPH_H__B7ADBDD9_40B2_4BDB_8EA5_43A8E5F1F5B3__INCLUDED_)
