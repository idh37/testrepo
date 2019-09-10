#pragma once
#include "Resource.h"
#include "UPGDlgBase.h"

class UPG_COMMON_EXPORT CPostBoxDlg : public CUPGDlgBase
{
	DECLARE_DYNAMIC(CPostBoxDlg)

private:
	NMBASE::SKINGDI::CMyBitmap m_BackBit;
	NMBASE::SKINGDI::CMyButton	m_CloseBtn;

	CString m_strURL;
	CString m_strFriendName;
	bool	m_bGroup;

	std::map<std::string, std::string>	m_mapIDAndNickName;

public:
	//browser
	NMBASE::UTIL::CWebCtrl			*m_pBrowser;	// <- �߰�
	BOOL CreateBrowser();
	void DestroyBrowser();

public:
	CPostBoxDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPostBoxDlg();
	virtual BOOL PreTranslateMessage(MSG* pMsg);	

	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_POSTBOXDLG };

	virtual int DoModal(LPCTSTR szURL);
	int DoModalSend(LPCTSTR szURL, char* strFirend, bool bGroup );
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	//external function 
	DECLARE_DISPATCH_MAP()
	void ClosePopup( short i );

};
