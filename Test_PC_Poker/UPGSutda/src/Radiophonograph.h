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
	BOOL m_bBoardMove;		//������ �ִϸ��̼� ����
	BOOL m_bVolume;			//���� �������ΰ� üũ
	
	int m_myVolume;		//���� ������ ����ũ��
	int m_NowPlayMusic; //���� �÷��̵ǰ� �ִ� ����
	int m_MusicNum;		//���忡�� ���õ� ����(�÷��̵ǰ� �ִ� ���ǰ��� Ʋ�����ִ�.)
	int m_BoardState;	//������ ���� ����
	int m_bPlayRecord;	//3���� ���� ������ ���� �Ⱥ���

	DWORD m_dwInRecord;		//���ڵ� �ֱ�
	DWORD m_dwPlayRecord;	//���ڵ� ���ư���
	DWORD m_dwTurnRecord;	//���ڵ� ������ 
	DWORD m_dwBGBoard;		//BGM���� ������ 

	CRect m_ScrollBaseRect;		//	��ũ�ѹ� ����

	NMBASE::GRAPHICGDI::xSprite m_InRecordspr;		//���ڵ� �ֱ�
	NMBASE::GRAPHICGDI::xSprite m_PlayRecordspr;	//���ڵ� ���ư���
	NMBASE::GRAPHICGDI::xSprite m_TurnRecordspr;	//���ڵ� ������ 
	NMBASE::GRAPHICGDI::xSprite m_MoveSpeakerSpr;	//����Ŀ �����̱�
	NMBASE::GRAPHICGDI::xSprite m_Bgm_On_OffBtnspr;	//on_off��ư ��������Ʈ
	NMBASE::GRAPHICGDI::xSprite m_BoardBtnspr;		//on_off��ư ��������Ʈ
	NMBASE::GRAPHICGDI::xSprite m_BGBoardBack;		//BGM �̵��ִ� 
	NMBASE::GRAPHICGDI::xSprite m_BGBoardSpr;			//BGM �̵��ִ� 

	NMBASE::GRAPHICGDI::xSprite m_PlayListSpr;	//���� ������ �ִ� ���� 
	NMBASE::GRAPHICGDI::xSprite SoundBtnSpr;	//	
	NMBASE::GRAPHICGDI::xSprite MusicListBtnSpr;//���Ǽ��ù�ư
	NMBASE::GRAPHICGDI::xSprite m_BgmViewSpr;	//equalizerǥ��

	NMBASE::SKINGDI::CGraphButton	m_BgmOnBtn;	//onoff��ư
	NMBASE::SKINGDI::CGraphButton	m_BgmOffBtn;//onoff��ư
	NMBASE::SKINGDI::CGraphButton	m_BgBoardOnBtn;//����ON	
	NMBASE::SKINGDI::CGraphButton	m_BgBoardOffBtn;//����OFF

	NMBASE::SKINGDI::CGraphButton	m_PlayBtn;	//�÷��̹�ư 
	NMBASE::SKINGDI::CGraphButton	m_StopBtn;	//�����ư 
	NMBASE::SKINGDI::CGraphButton	m_VolumeBtn;

	NMBASE::SKINGDI::CGraphButton    m_Music1Btn;
	NMBASE::SKINGDI::CGraphButton    m_Music2Btn;
	NMBASE::SKINGDI::CGraphButton    m_Music3Btn;
	NMBASE::SKINGDI::CGraphButton    m_Music4Btn;
	NMBASE::SKINGDI::CGraphButton    m_Music5Btn;

	void Init( CWnd *pWnd, NMBASE::GRAPHICGDI::CPage *pPgae );
	void ReStart();	//������� �⺻�������� ����
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
	void SetRelativeVolume(int nVolume);	//0-100������ �ʹ�Ŀ�� 60-100 �� ��ġ�� �ٲ㼭 ����
};

#endif // !defined(AFX_RADIOPHONOGRAPH_H__B7ADBDD9_40B2_4BDB_8EA5_43A8E5F1F5B3__INCLUDED_)
