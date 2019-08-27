#pragma once

class CAddUserSelectDlg;

class UPG_COMMON_EXPORT CAddUserSelectDlgBuddyList : public LSY::CList
{
public:
	CAddUserSelectDlg				*m_pDlg;
	std::map<CBuddyFriend *, bool>	m_mapBuddyCheck;
public:
	CAddUserSelectDlgBuddyList(CAddUserSelectDlg *pDlg);
	virtual ~CAddUserSelectDlgBuddyList();

	virtual int AddItem(LPARAM lParam);
	virtual bool DeleteItem(LPARAM lParam);
	virtual void DeleteAllItem(void);

	bool CheckAllItemCheck(void);
	void ClearAllItemCheck(void);
};