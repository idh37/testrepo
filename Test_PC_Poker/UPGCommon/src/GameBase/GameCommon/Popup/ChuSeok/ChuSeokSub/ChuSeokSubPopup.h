
#pragma once

#include <EventCommon.h>


class UPG_COMMON_EXPORT CChuSeokSubPopup : public LSY::CLSYLibDialogBase
{
private:
	LSY::CButton* m_pBtnOk;
	LSY::CText* m_pTxtChip;

	std::string m_sTxtChip;
	
public:
	CChuSeokSubPopup(CWnd* pParent = NULL);
	~CChuSeokSubPopup();

	virtual bool OnCreatedProject();
	
	virtual INT_PTR DoModal(CWnd *pParent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	LRESULT OnClickButton(LSY::CMessage* pMsg);
	
	void HandleResponse(int Signal, int TotSize, char *lpData);	
	void SetTxtChip(std::string sChip);

private:
	void InitLSYControl();
	void SetData();

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
