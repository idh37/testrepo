#if !defined(AFX_OPTIONDLG_H__FCB8FE23_C46D_407B_A254_4FD7D76BF010__INCLUDED_)
#define AFX_OPTIONDLG_H__FCB8FE23_C46D_407B_A254_4FD7D76BF010__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionDlg.h : header file
//

#include "../resource.h"
#include "DialogDefine.h"
//#include "UserStruct.h"
#include "MYButton.h"
#include "MyBitmap.h"
#include "hTransRegion.h"
#include "BmpRadioBtn.h"
#include "SliderCtrlEx.h"

/////////////////////////////////////////////////////////////////////////////
// COptionDlg dialog

#define MODE_OPTION		0
#define MODE_MACRO		1
#define MODE_DSEVENT	2

#define IDC_EDIT_MACRO					1511

typedef struct
{
	char FxVolume; // 사운드 UI 수정 
	BOOL bSndFX;	
	
	BOOL bBetBtnType;		//베팅 버튼 타입(가로  세로)	
	BOOL bCutStyle;	
	int  nAutoUpType;       // 바둑이 자동올림 체크 // 0 : 사용않함 1: 사용 2 : 선택 전
	BOOL bNoObserverChat;	// 관전 채팅  0 보기 1 안보기
	BOOL bInvite;
	BOOL bWhisper;	
		
	//-------------------!!!!아래삭제 예정!!!!!!!!----------------
	//-------------------!!!!아래삭제 예정!!!!!!!!----------------
	//-------------------!!!!아래삭제 예정!!!!!!!!----------------

	BOOL bCardArray;		//FALSE = 기본형, TRUE = 일자형
	int bShowBtnMoney; // 베팅금액 미리보기 // 단축키
	int  nScreenMode;

	int bTodayHando;	//보유한도

	BOOL bFailToFullScreen;
	//	음성 구분(느끼버전, 섹시버전)	
	int  SndFxKind; // ### [사운드 추가 작업] ###
	BOOL bHideOtherBlock;
//	int  MoneyMark; // 머니 표기 숫자, 한글
	int  nAutoBet;	//0 = off 1 = 4  2 = 5  3 = 6
	BOOL bAvataAni;	//0 = off 1 = on
	BOOL bShowTip;
	int  nAutoBetSpeed; // 0 보통 1 빠름 
	int  nMyCardIndex;	// 카드 종류
	int  nSKinKind;		// 0 블루 스킨 , 1 그레이 스킨
	BOOL bAutoHidden;	// 자동히든 사용 == 1 , 사용안함 == 0
	BOOL MoneyMark;

	//사용안함 삭제
//  	BOOL bEventPopUp;	//이벤트 팝업 알림창 오픈 여부
//  	int	 nEventPopUpDay;//이벤트 팝업 비교날짜
//  	int	 nLADay;			//행운아바타 구매기회가 주워졌는지.	
//  	int	 nLAOPen;			//행운아바타 구매기회가 주워졌는지.	
 
// 	//[포커 추석 이벤트] 2006.08.24
//  	BOOL bBettingPopup;	// 1 오늘 그만열기 0 오늘 계속 열기
//  	int  nBettingPopupDay;//그만열기 한 날짜
// 
// 
//  	BOOL bPigEventPopUp;	//이벤트 팝업 알림창 오픈 여부
//  	int	 nPigEventPopUpDay;//이벤트 팝업 비교날짜
// 	BOOL bIdentitySound;	//  FALSE 각 음성 사운드 플레이 TRUE 자기 설정 음성 사운드 플레이

	
// 	char BgmVolume; // 사운드 UI 수정
// 	char bSingleVoice; // 사운드 UI 수정

	
}CONFIG;

class NMDIALOG_CLASS COptionDlg : public CDialog
{
// Construction
public:

	CMyBitmap	Back;
	CBrush		m_HollowBrush;
	CImageList	m_CheckImage;
	CHTransRegion m_Rgn;


	BOOL	m_bFx;
	BOOL	m_bBetBtnStyle;	
	BOOL	m_bCutStyle;
	BOOL	m_bInvite;		
	BOOL	m_bWhisper;			
	BOOL	m_bNoObserverChat;		// 관전 채팅  0 보기 1 안보기

	char    m_FxVolume; // 사운드 UI 수정 

	CONFIG *m_pMyConfig;
	

	COptionDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COptionDlg)
	enum { IDD = IDD_OPTION_DIALOG };
	CSliderCtrlEx   m_FxVolumeSlideBar; // 사운드 UI 수정		
	CBmpRadioBtn	m_radioFxOn, m_radioFxOff;
	CBmpRadioBtn	m_radioBetBtnVertical, m_radioBetBtnHorizontal;	
	CBmpRadioBtn	m_radioCutStyle1, m_radioCutStyle2;		
	CBmpRadioBtn	m_raidoObserverChatOn, m_raidoObserverChatOff;
	CBmpRadioBtn	m_raidoInviteOn, m_raidoInviteOff;
	CBmpRadioBtn	m_raidoWhisperOn, m_raidoWhisperOff;
	CMyButton	m_CancelBtn;
	CMyButton	m_OKBtn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	public:
	int DoModal(void);


// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COptionDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONDLG_H__FCB8FE23_C46D_407B_A254_4FD7D76BF010__INCLUDED_)
