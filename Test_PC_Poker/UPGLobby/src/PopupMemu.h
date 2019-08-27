#pragma once

#define UPGM_POPUPMEMU_OPEN		(CM_MESSAGE_LAST-1)
#define UPGM_POPUPMEMU_CLOSE	(CM_MESSAGE_LAST-2)

class CBlackBuddy;
class CBuddyBase;

class UPG_LOBBY_EXPORT CPopupMemu : public LSY::CLSYLibWindowsBase
{
protected:
	DECLARE_DYNAMIC(CPopupMemu)
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
	CPopupMemu();
	virtual ~CPopupMemu();

	void AddMemu(int nID, LPCTSTR lpText, bool bEnable = true);

	bool Create(int x, int y);

	virtual bool OnCreatedProject(void);
	LRESULT OnClickButton(LSY::CMessage *pMsg);
	LRESULT OnMouseOver(LSY::CMessage *pMsg);
	LRESULT OnMouseLeave(LSY::CMessage *pMsg);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};