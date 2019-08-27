#pragma once

class CAddUserSelectDlg;

class UPG_COMMON_EXPORT CAddUserSelectDlgConnectUserList : public LSY::CList
{
public:
	CAddUserSelectDlg	*m_pDlg;

public:
	CAddUserSelectDlgConnectUserList(CAddUserSelectDlg *pDlg);
	virtual ~CAddUserSelectDlgConnectUserList();

	virtual int AddItem(LPARAM lParam);
	virtual bool DeleteItem(LPARAM lParam);
	virtual void DeleteAllItem(void);

	bool CheckAllItemCheck(void);
	void ClearAllItemCheck(void);
};