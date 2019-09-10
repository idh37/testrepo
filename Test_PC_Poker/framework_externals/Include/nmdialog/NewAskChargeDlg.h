#if !defined(AFX_NEWASKCHARGEDLG_H__CF792989_1B69_40CC_93FA_2F6F8A79FEA4__INCLUDED_)
#define AFX_NEWASKCHARGEDLG_H__CF792989_1B69_40CC_93FA_2F6F8A79FEA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../resource.h"
#include "DialogDefine.h"
#include "UPGSSOWebLib.h"
#include "MyBitmap.h"
#include "HTransRegion.h"
#include "MyButton.h"

// NewAskChargeDlg.h : header file
//
// 
// enum {
// 	ACD_GO_CHARGE = 1000,			// ���� ������ �̵�
// 	ACD_GO_LEADERS,				// ������ ������ �̵�
// 	ACD_ALLIN_INSURANCE,		// ���� ���� �ڼ��� ���� 
// 	ACD_ALLIN_INSURANCE_USE,	// ���� ���� ����ϱ� 
// 	ACD_ALLIN_INSURANCE_BUY,	// ���� ���� �����ϱ� 
// 	ACD_INVINCIBLE_BUY,			// õ�Ϲ��� �����ϱ� 
// 	ACD_INVINCIBLE_DETAILS,		// õ�Ϲ��� �ڼ��� ���� 
// 	ACD_CLOSEWINDOW,			// â�ݱ�
// 	ACD_GO_MAINPAGE,			// ���� �������� �̵� 
// 
// 	ACD_GAME_CONTINUE,			// [DB��� Process] 2004.11.29  ���Ӱ���ϱ�
// 	ACD_GO_LEADERS_ZONE,			// [DB��� Process] 2004.11.29  ������ ȸ����
// 	ACD_GO_AVATA_ZONE,			// [DB��� Process] 2004.11.29  �ƹ�Ÿ ����
// 	ACD_GO_SEARCH_PCROOM,       // [����â ����] 2006.01.04 �ǽù� ã��
// 	// -> �Ʒ� ���� �߰�
// 	ACD_CLOSEPOPUP, // [������ ����ȭ ����]
// };

enum NEW_ASK_CHARGE_DLG_TYPE
{	
	NEW_ASK_CHARGE_DLG_TYPE_FREE_CHARGE_INSU=0,	//��޺������� �켱 �������� (��޺������ִٸ� ��޺��� ���� �޴´�)	
	NEW_ASK_CHARGE_DLG_TYPE_INSU,		//��޺��� �˾� (��޺��踸 üũ )
	NEW_ASK_CHARGE_DLG_TYPE_FREE_CHARGE,	//�������� �˾� (���������� üũ )
	
};



/////////////////////////////////////////////////////////////////////////////
// CNewAskChargeDlg dialog

class NMDIALOG_CLASS CNewAskChargeDlg : public CDialog
{
// Construction
protected:

	int m_nKind;

	CUPGSSOWebBrowser	m_Browser;
	CString             m_strURL;

	CMyBitmap           m_Back;
	CHTransRegion       m_Rgn;

	BOOL CreateBrowser();
	void DestroyBrowser();

	DWORD m_dwStartTime;	

	CMyButton m_BtnX;

public:

// 	static CNewAskChargeDlg *m_pNewAskChargeDlg;
// 	static void OnBeforeNavigate_NewAskCharge(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, LPCTSTR lpszPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel);
// 	static void OnBeforeNavigate_OnNavigateComplete2(LPCTSTR lpszURL);
	
	void Navigate( CString& strURL );

	CNewAskChargeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewAskChargeDlg)
	enum { IDD = IDD_NEW_ASKCHARGEDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewAskChargeDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual int DoModal( int nKind = NEW_ASK_CHARGE_DLG_TYPE_FREE_CHARGE_INSU, const char *pStrURL=NULL);	
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CNewAskChargeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


// ������ ��ü ��� �Լ�
protected:
	void ClosePopup( short wParam );
	void UpdateMoney( BOOL bClosePopup );
	void OpenMarket();
	void JoinInsuRequest();
	void ShowItemPopup( int nTab, LPCTSTR sURL );
	DECLARE_DISPATCH_MAP()

public:
	afx_msg void OnBnClickedButtonX();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWASKCHARGEDLG_H__CF792989_1B69_40CC_93FA_2F6F8A79FEA4__INCLUDED_)
