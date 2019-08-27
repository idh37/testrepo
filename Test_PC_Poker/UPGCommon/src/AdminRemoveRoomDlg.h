#pragma once

class UPG_COMMON_EXPORT CAdminRemoveRoomDlg : public CUPGDlgBase
{
public:
	CAdminRemoveRoomDlg(CWnd* pParent = NULL);   // standard constructor

	enum { IDD = IDD_ADMIN_REMOVEROOM };
	CComboBox	m_cmbMsg;
	CString	m_strMsg;
	CString	m_text;

	int m_RoomNo;
	CString m_strRoomTitle;

public:
	virtual int DoModal(int RoomNo, const char* szRoomTitle);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
};