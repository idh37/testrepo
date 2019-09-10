#pragma once


#include "../resource.h"

// CPromotionComeBackDlg ��ȭ �����Դϴ�.

class UPG_GAMEBASE_EXPORT CPromotionComeBackDlg : public CUPGDlgBase
{
protected:
	NMBASE::SKINGDI::CMyBitmap		m_BackBit;
	NMBASE::GRAPHICGDI::CPage			m_cPage;
	NMBASE::GRAPHICGDI::xSprite			m_sprBack;	//��������Ʈ
	//NMBASE::SKINGDI::CMyBitmap		m_Complete;
	NMBASE::SKINGDI::CMyButton		m_btnComeBack;
	NMBASE::SKINGDI::CMyButton		m_btnClose;
public:
	CPromotionComeBackDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPromotionComeBackDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_COMEBACK_PROMOTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedComebackOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
