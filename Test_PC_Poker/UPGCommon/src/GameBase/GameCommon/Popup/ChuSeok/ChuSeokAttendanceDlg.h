
#pragma once

#include <EventCommon.h>


class UPG_COMMON_EXPORT CChuSeokDlg : public LSY::CLSYLibDialogBase
{
private:

	static const int MAX_ATTENDANCE_CHECK = 28;
	static const int MAX_CONT_ATTENDANCE_CHECK = 5;	
	static const int SEVEN_DAYS_WEEK = 7;	
	static const int MAX_DAYS_ATTEND_REGULARLY = 28;
	static const int TIMER_ID = 3784;
	static const int MSG_ELAPSE_TIME = 1000;

	LSY::CButton* m_pClose;
	LSY::CImage* m_pAttendanceCheck[MAX_ATTENDANCE_CHECK];
	LSY::CImage* m_pContAttendanceCheck[MAX_CONT_ATTENDANCE_CHECK];
	LSY::CImage* m_pAttendingCheck[MAX_CONT_ATTENDANCE_CHECK];
	
	bool m_forcePopup;
	 
	std::vector<promotion::PROMOTION_2018THANKSGIVING_ATTEDANCE_DAY_INFO> m_attendanceTotalInfo;
	INT64 m_finalReward;
	
public:
	
	CChuSeokDlg(CWnd* pParent = NULL);
	~CChuSeokDlg();

	virtual bool OnCreatedProject();
	
	virtual INT_PTR DoModal();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	LRESULT OnClickButton(LSY::CMessage* pMsg);
	
	void HandleResponse(int Signal, int TotSize, char *lpData);	

private:

	void _InitLSYControl();
	void _SetData(int totSize, char* lpData);
	void _CheckAttendance();
	void _ShowRewardPopup();

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
