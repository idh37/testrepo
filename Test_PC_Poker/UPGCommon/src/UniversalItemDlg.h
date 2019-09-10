#if !defined(AFX_UNIVERSALITEMDLG_H__DE90054B_5342_40DE_AFC8_93FE7F43F507__INCLUDED_)
#define AFX_UNIVERSALITEMDLG_H__DE90054B_5342_40DE_AFC8_93FE7F43F507__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UPGDlgBase.h"

enum 
{
	TYPE_CHOICEAVATAR = 1,		//���̽� �ƹ�Ÿ 
	TYPE_DICEAVATAR,			//�ֻ��� �ƹ�Ÿ (�űԻ�ǰ�߰�)
	TYPE_CHALLENGE99AVATAR,		//����99 �ƹ�Ÿ (�űԻ�ǰ�߰�)
	TYPE_CARIBBEANAVATAR,		//�����ɸ���� �ƹ�Ÿ (�űԻ�ǰ�߰�)
	MAX_TYPE_ITEM,
	TYPE_CARD_BINGO,			//����� ī�����
	TYPE_CARD_PINBAL,			//����! �ɺ� : 2012.10.30 YS.Jeong (7��)
	TYPE_CARD_PINBAL2,			//����! �ɺ� : 2012.10.30 YS.Jeong (8��)
	TYPE_BIGWHEEL,				//���� ������.
	TYPE_SCISSORS,				//���������� ������.
};


class UPG_COMMON_EXPORT CUniversalItemDlg : public CUPGDlgBase
{
public:
	NMBASE::UTIL::CNMSSOWebBrowser m_Browser;

	NMBASE::SKINGDI::CMyBitmap m_Back;
//	NMBASE::SKINGDI::CMyBitmap m_BmpSubTitle;

	CString		m_strSubtitle;

	int		  m_nItemType;
	CString   m_URL;	

	NMBASE::SKINGDI::CHTransRegion m_Rgn;

	int		  m_nDefaultItem;

	DWORD     m_dwStartTime;


// Construction
public:
	CUniversalItemDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUniversalItemDlg)
	enum { IDD = IDD_DIALOG_UNIVERSAL_ITEM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUniversalItemDlg)
	public:
	virtual int DoModal( int nItemType, CString& sURL );
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUniversalItemDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	virtual void OnCancel();
	virtual void OnOK();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// ������ ��ü ��� �Լ�
protected:
	void ClosePopup( int nOpenShopItemIndex );
	void UpdateMoney( BOOL bClosePopup );
	DECLARE_DISPATCH_MAP()

public:
	int GetDefaultItem() { return m_nDefaultItem; }

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UNIVERSALITEMDLG_H__DE90054B_5342_40DE_AFC8_93FE7F43F507__INCLUDED_)
