#pragma once

#define UPGM_POPUPMEMU_OPEN		(CM_MESSAGE_LAST-1)
#define UPGM_POPUPMEMU_CLOSE	(CM_MESSAGE_LAST-2)

class CBlackBuddy;
class CBuddyBase;

class UPG_COMMON_EXPORT CChattingPopupMemu : public LSY::CLSYLibWindowsBase
{
protected:
	DECLARE_DYNAMIC(CChattingPopupMemu)
	DECLARE_DISPATCH_MAP()
	DECLARE_MESSAGE_MAP()
protected:
	class CMemuData
	{
	public:
		int			m_nID;
		std::string m_strText;
		bool		m_bEnable;
	public:
		CMemuData():m_nID(-1),m_strText(""),m_bEnable(true){}
		virtual ~CMemuData(){}
	};

	std::list<CMemuData>							m_listMenu;
	LSY::CImage										*m_pImage;
	CRect											m_rtRect;
public:
	CChattingPopupMemu();
	virtual ~CChattingPopupMemu();

	void AddMemu(int nID, LPCTSTR lpText, bool bEnable = true);

	bool Create(int x, int y, CWnd *pParent);

	virtual bool OnCreatedProject(void);
	LRESULT OnClickButton(LSY::CMessage *pMsg);
	LRESULT OnMouseOver(LSY::CMessage *pMsg);
	LRESULT OnMouseLeave(LSY::CMessage *pMsg);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

UPG_COMMON_EXPORT extern CChattingPopupMemu *GetIDToChattingRoomDlg(const int &nID);