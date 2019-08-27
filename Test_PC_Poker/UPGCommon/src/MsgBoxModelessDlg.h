
#pragma once


class UPG_COMMON_EXPORT CMsgBoxModelessDlg : public LSY::CLSYLibDialogBase
{
private:

	static const int HEIGHT_PER_LINE = 13;

	bool m_forceOrder;
	std::string m_msg;

	LSY::CText* m_pTextMsg;
	LSY::CImage* m_pImgBackground;

public:

	CMsgBoxModelessDlg(CWnd* pParent = NULL);
	~CMsgBoxModelessDlg();

	virtual bool OnCreatedProject();

	LRESULT OnClickButton(LSY::CMessage* pMsg);
	LRESULT OnRedrawTextRange(LSY::CMessage* pMsg);

	BOOL ShowMsg(HWND parentHWnd, LPCTSTR msg);	
	BOOL ShowWindow(bool bShow, LPCTSTR msg);

private:

	void _ForceWinEnableOrdering(bool beforePopup);
};
