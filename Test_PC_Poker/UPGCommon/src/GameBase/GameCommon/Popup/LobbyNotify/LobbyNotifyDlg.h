#pragma once

class UPG_COMMON_EXPORT CLobbyNotifyDlg : public LSY::CLSYLibDialogBase
{
private:
	LSY::CButton* m_pBtnOK;
	LSY::CCheckBox* m_pChkToday;
	
public:
	CLobbyNotifyDlg(CWnd* pParent = NULL);
	~CLobbyNotifyDlg();
	
	virtual bool OnCreatedProject();
	virtual INT_PTR DoModal();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnOK();
	virtual void OnCancel();
	
	LRESULT OnClickButton(LSY::CMessage* pMsg);

private:
	void InitLSYControl();
	void SetData();
};
