#pragma once

class UPG_COMMON_EXPORT CMessageNotificationPopup : public LSY::CLSYLibWindowsBase
{
public:
	enum ENUM_MESSAGE_NOTIFICATION_STATUS
	{
		EMNS_START_UNKNOWN = -1,
		EMNS_START_SLIDE,
		EMNS_SHOW_DELAY,
		EMNS_END_SLIDE,
	};

	bool								m_bMemo;
	Camel::Dar2::SS::Memo				m_cMemo;
	std::string							m_strConnectUserNickName;

	DWORD								m_dwStartTime;
	DWORD								m_dwEndTime;
	DWORD								m_dwSlideTime;
	DWORD								m_dwDelayTime;
	bool								m_bFirst;

	LSY::CPoint							m_ptPos;
	LSY::CRect							m_rtRect;
	ENUM_MESSAGE_NOTIFICATION_STATUS	m_nStatus;
	bool								m_bMouseOver;
public:
	CMessageNotificationPopup();   // standard constructor
	virtual ~CMessageNotificationPopup(){}

	void InitData(void);
public:
	bool Create(CWnd *pParent, int x, int y);

	void ShowNotification(Camel::Dar2::SS::Memo *pMemo, DWORD dwSlideTime = 1000, DWORD dwDelayTime = 2000);
	void ShowNotification(std::string strConnectBuddy, DWORD dwSlideTime = 1000, DWORD dwDelayTime = 2000);

	virtual bool OnCreatedProject(void);

	LRESULT OnClickButton(LSY::CMessage *pMsg);
	LRESULT OnMouseOver(LSY::CMessage *pMsg);
	LRESULT OnMouseLeave(LSY::CMessage *pMsg);
	LRESULT OnTimer(LSY::CMessage *pMsg);

	static int GetProjectID(void);

	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	CPoint	GetPos(void);
	const bool &IsMouseOver(void){return m_bMouseOver;}
	const ENUM_MESSAGE_NOTIFICATION_STATUS &GetStatus(void){return m_nStatus;}
	const DWORD &GetEndTime(void){return m_dwEndTime;}
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
protected:
	DECLARE_MESSAGE_MAP()
};