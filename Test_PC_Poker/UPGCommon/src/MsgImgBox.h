#if !defined(AFX_MSGIMGBOX_H__5D42F5A6_69B3_4F4A_9095_3BA307FAB62E__INCLUDED_)
#define AFX_MSGIMGBOX_H__5D42F5A6_69B3_4F4A_9095_3BA307FAB62E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MsgImgBox.h : header file
//
#include "resource.h"
#include "UPGDlgBase.h"
/////////////////////////////////////////////////////////////////////////////
// CMsgImgBox dialog

enum BTN_TYPE{
	TPYE_YES =0,
	TYPE_YESNO,	//예 , 아니오
	TYPE_OK,	//확인
	TYPE_OKNO,	//확인 취소
};

class UPG_COMMON_EXPORT CMsgImgBox : public CUPGDlgBase
{
// Construction
public:
	CMsgImgBox(CWnd* pParent = NULL);   // standard constructor
	virtual int DoModal(CString bgpath,int btntype = TYPE_OKNO);
	virtual int DoModal(CString bgpath,CString Msg,CRect rt, int btntype = TYPE_OKNO,BOOL bText = FALSE);

	CString	m_BgPath;
	CString	m_Msg;

	int		m_nBtn_type;

	BOOL	m_bText;
	CRect	m_TextRect;

// Dialog Data
	//{{AFX_DATA(CMsgImgBox)
	enum { IDD = IDD_MSG_IMGBOX };
	NMBASE::SKINGDI::CMyButton	m_CancelBtn;
	NMBASE::SKINGDI::CMyButton	m_OKBtn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMsgImgBox)
	public:	
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMsgImgBox)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSGIMGBOX_H__5D42F5A6_69B3_4F4A_9095_3BA307FAB62E__INCLUDED_)
