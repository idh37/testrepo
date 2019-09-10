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
	char FxVolume; // ���� UI ���� 
	BOOL bSndFX;	
	
	BOOL bBetBtnType;		//���� ��ư Ÿ��(����  ����)	
	BOOL bCutStyle;	
	int  nAutoUpType;       // �ٵ��� �ڵ��ø� üũ // 0 : ������ 1: ��� 2 : ���� ��
	BOOL bNoObserverChat;	// ���� ä��  0 ���� 1 �Ⱥ���
	BOOL bInvite;
	BOOL bWhisper;	
		
	//-------------------!!!!�Ʒ����� ����!!!!!!!!----------------
	//-------------------!!!!�Ʒ����� ����!!!!!!!!----------------
	//-------------------!!!!�Ʒ����� ����!!!!!!!!----------------

	BOOL bCardArray;		//FALSE = �⺻��, TRUE = ������
	int bShowBtnMoney; // ���ñݾ� �̸����� // ����Ű
	int  nScreenMode;

	int bTodayHando;	//�����ѵ�

	BOOL bFailToFullScreen;
	//	���� ����(��������, ���ù���)	
	int  SndFxKind; // ### [���� �߰� �۾�] ###
	BOOL bHideOtherBlock;
//	int  MoneyMark; // �Ӵ� ǥ�� ����, �ѱ�
	int  nAutoBet;	//0 = off 1 = 4  2 = 5  3 = 6
	BOOL bAvataAni;	//0 = off 1 = on
	BOOL bShowTip;
	int  nAutoBetSpeed; // 0 ���� 1 ���� 
	int  nMyCardIndex;	// ī�� ����
	int  nSKinKind;		// 0 ��� ��Ų , 1 �׷��� ��Ų
	BOOL bAutoHidden;	// �ڵ����� ��� == 1 , ������ == 0
	BOOL MoneyMark;

	//������ ����
//  	BOOL bEventPopUp;	//�̺�Ʈ �˾� �˸�â ���� ����
//  	int	 nEventPopUpDay;//�̺�Ʈ �˾� �񱳳�¥
//  	int	 nLADay;			//���ƹ�Ÿ ���ű�ȸ�� �ֿ�������.	
//  	int	 nLAOPen;			//���ƹ�Ÿ ���ű�ȸ�� �ֿ�������.	
 
// 	//[��Ŀ �߼� �̺�Ʈ] 2006.08.24
//  	BOOL bBettingPopup;	// 1 ���� �׸����� 0 ���� ��� ����
//  	int  nBettingPopupDay;//�׸����� �� ��¥
// 
// 
//  	BOOL bPigEventPopUp;	//�̺�Ʈ �˾� �˸�â ���� ����
//  	int	 nPigEventPopUpDay;//�̺�Ʈ �˾� �񱳳�¥
// 	BOOL bIdentitySound;	//  FALSE �� ���� ���� �÷��� TRUE �ڱ� ���� ���� ���� �÷���

	
// 	char BgmVolume; // ���� UI ����
// 	char bSingleVoice; // ���� UI ����

	
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
	BOOL	m_bNoObserverChat;		// ���� ä��  0 ���� 1 �Ⱥ���

	char    m_FxVolume; // ���� UI ���� 

	CONFIG *m_pMyConfig;
	

	COptionDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COptionDlg)
	enum { IDD = IDD_OPTION_DIALOG };
	CSliderCtrlEx   m_FxVolumeSlideBar; // ���� UI ����		
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
